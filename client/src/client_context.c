#include "client_context.h"
#include <uv.h>

client_context client_context_init(uv_loop_t* loop) {
    client_context context;
    context.connected_discovery = false;
    context.connected_client = false;
    context.loop = loop;

    return context;
}

void client_context_set_username(char* username, client_context* context) {
    context->username = username;
}
