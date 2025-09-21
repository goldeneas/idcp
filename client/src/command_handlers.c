#include "command_handlers.h"
#include "c2c_packets.pb.h"
#include "client.h"
#include "client_context.h"
#include "common/list.h"
#include "common/log.h"
#include "common/command_handlers.h"
#include "common/network.h"
#include "common/str_utils.h"
#include "peer_list.h"
#include "settings.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <uv.h>
#include "network.h"

void on_help_cmd(char* args[], int args_len, client_context* context) {
    log_info("No help yet! :)");
}

void on_greet_cmd(char* args[], int args_len, client_context* context) {
    char* end;
    uint32_t other_id = strtol(args[1], &end, 10);
    uv_tcp_t* handle = &context->handle;

    send_greet(handle, other_id);
}

void on_connect_cmd(char* args[], int args_len, client_context* context) {
    char* end;
    char* address = args_len >= 2 ? args[1] : "0.0.0.0";
    uint32_t port = args_len >= 3 ?  strtol(args[2], &end, 10) : DEFAULT_CONNECT_PORT;

    client_connect(address, port, context->loop, context);
}

void on_disconnect_cmd(char* args[], int args_len, client_context* context) {
    client_disconnect(context);
}

void handle_tty_command(const uv_buf_t* buf, client_context* context) {
    char* cmd = &buf->base[1];
    str_remove_nl(cmd);

    int args_len = 0;
    char* args[MAX_STDIN_CMD_ARG_SIZE];
    char* token = strtok(cmd, " ");

    while (token != NULL) {
        args[args_len] = token;
        args_len += 1;
        token = strtok(NULL, " ");
    }

    char* cmd_name = args[0];
    HANDLE_COMMAND_BRANCH(cmd_name, "help", on_help_cmd, args, args_len, context);
    HANDLE_COMMAND_BRANCH(cmd_name, "greet", on_greet_cmd, args, args_len, context);
    HANDLE_COMMAND_BRANCH(cmd_name, "connect", on_connect_cmd, args, args_len, context);
    HANDLE_COMMAND_BRANCH(cmd_name, "disconnect", on_disconnect_cmd, args, args_len, context);

    log_info("Unknown command: '%s'. Type /help for help", cmd_name);
}

void handle_tty_input(const uv_buf_t* buf, client_context* context) {
    if (buf->len <= 0) { return; }

    bool is_command = buf->base[0] == STDIN_CMD_SYMBOL;
    if (is_command) {
        handle_tty_command(buf, context);
        return;
    }

    list* peer_list = &context->peer_list;

    for (size_t i = 0; i < peer_list->length; i++) {
        peer_entry* entry = list_get(i, peer_list);
        struct sockaddr_storage storage = socket_get_sockaddr_storage(entry->address, entry->port,
                entry->sa_family);
        send_message((struct sockaddr*) &storage, buf->base, &context->beacon);
    }
}
