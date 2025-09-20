#include "client_context.h"
#include "common/list.h"
#include "common/wrapper/client_info.h"
#include "peer_list.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <uv.h>

client_context client_context_init(uv_loop_t* loop) {
    client_context context;
    context.loop = loop;
    context.peer_list = peer_list_init();

    return context;
}

void client_context_set_username(char* username, client_context* context) {
    context->username = username;
}
