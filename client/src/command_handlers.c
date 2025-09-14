#include "command_handlers.h"
#include "client_context.h"
#include "common/log.h"
#include "common/command_handlers.h"
#include "settings.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>
#include <uv.h>
#include "network.h"

void on_help(char* buf, client_context* context) {
    log_info("No help yet! :)");
}

void on_greet(char* buf, client_context* context) {
    uint32_t other_id = buf[1];
    uv_tcp_t* server = context->server;

    send_greet(server, other_id);
}

void handle_tty_command(const uv_buf_t* buf, client_context* context) {
    if (buf->len <= 0) { return; }

    bool is_command = buf->base[0] == STDIN_CMD_SYMBOL;
    if (!is_command) { return; }

    char* cmd = &buf->base[1];

    size_t nl_pos = strcspn(cmd, "\n");
    if (nl_pos == 0) { return; }
    cmd[nl_pos] = '\0';

    char* base_name = strtok(cmd, " ");

    HANDLE_COMMAND_BRANCH(base_name, "help", on_help, cmd, context);
    HANDLE_COMMAND_BRANCH(base_name, "greet", on_greet, cmd, context);

    log_info("Unknown command: '%s'. Type /help for help", base_name);
}
