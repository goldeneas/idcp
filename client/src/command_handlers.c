#include "command_handlers.h"
#include "client.h"
#include "client_context.h"
#include "common/list.h"
#include "common/log.h"
#include "common/command_handlers.h"
#include "settings.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <uv.h>
#include "network.h"

void on_help_cmd(char* args[], client_context* context) {
    log_info("No help yet! :)");
}

void on_greet_cmd(char* args[], client_context* context) {
    char* end;
    uint32_t other_id = strtol(args[1], &end, 10);
    uv_tcp_t* handle = context->handle;

    send_greet(handle, other_id);
}

void on_connect_cmd(char* args[], client_context* context) {
    char* end;
    char* address = args[1];
    uint32_t port = strtol(args[2], &end,10);

    client_connect(address, port, context->loop, context);
}

void on_disconnect_cmd(char* args[], client_context* context) {
    client_disconnect(context);
}

void handle_tty_command(const uv_buf_t* buf, client_context* context) {
    if (buf->len <= 0) { return; }

    bool is_command = buf->base[0] == STDIN_CMD_SYMBOL;
    if (!is_command) { return; }

    char* cmd = &buf->base[1];

    size_t nl_pos = strcspn(cmd, "\n");
    if (nl_pos == 0) { return; }
    cmd[nl_pos] = '\0';

    int i = 0;
    char* args[MAX_STDIN_CMD_ARG_SIZE];
    char* token = strtok(cmd, " ");

    while (token != NULL) {
        args[i] = token;
        i += 1;
        token = strtok(NULL, " ");
    }

    char* cmd_name = args[0];
    HANDLE_COMMAND_BRANCH(cmd_name, "help", on_help_cmd, args, context);
    HANDLE_COMMAND_BRANCH(cmd_name, "greet", on_greet_cmd, args, context);
    HANDLE_COMMAND_BRANCH(cmd_name, "connect", on_connect_cmd, args, context);
    HANDLE_COMMAND_BRANCH(cmd_name, "disconnect", on_disconnect_cmd, args, context);

    log_info("Unknown command: '%s'. Type /help for help", cmd_name);
}
