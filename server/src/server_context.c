#include "server_context.h"
#include "wrapper/client_list.h"
#include "wrapper/greet_list.h"

server_context server_context_init(void) {
    server_context ctx;
    ctx.client_list = client_list_init();
    ctx.greet_list = greet_list_init();

    return ctx;
}
