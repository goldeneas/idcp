#include "client_context.h"
#include <uv.h>

client_context client_context_init(uv_loop_t* loop) {
    client_context context;
    context.handle = NULL;
    context.connected = false;
    context.req = NULL;
    context.loop = loop;

    return context;
}

void client_context_set_name(char* name, client_context* context) {
    context->name = name;
}
