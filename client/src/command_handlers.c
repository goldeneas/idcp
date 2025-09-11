#include "command_handlers.h"
#include "common/log.h"
#include "common/command_handlers.h"

void on_test(char* buf, client_context* context) {
    log_info("works!");
}

void handle_tty_command(const uv_buf_t* buf, client_context* context) {
    char* cmd = (char*) buf;

    // HANDLE_COMMAND_BRANCH(cmd, "test", on_test, context);
    
    if (strcmp(cmd, "test") == 0) {
        on_test(cmd, context);
        return;
    }    

    log_info("Unknown command. Type /help for help");
}
