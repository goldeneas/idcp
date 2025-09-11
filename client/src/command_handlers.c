#include "command_handlers.h"
#include "common/log.h"
#include "common/command_handlers.h"
#include "settings.h"
#include <string.h>

void on_test(char* buf, client_context* context) {
    log_info("works!");
}

void handle_tty_command(const uv_buf_t* buf, client_context* context) {
    // TODO: remove newline character
    char* cmd = buf->base;
    char* base_name = strtok(cmd, " ");

    HANDLE_COMMAND_BRANCH(base_name, "test", on_test, context);

    log_info("Unknown command: '%s'. Type /help for help", base_name);
}
