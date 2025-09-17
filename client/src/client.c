#include "client.h"
#include "client_context.h"
#include "common/log.h"
#include "pb.h"
#include "settings.h"
#include "callback.h"
#include <stdlib.h>
#include <uv.h>

bool client_connect(char* address, int port, uv_loop_t* loop, client_context* context) {
    if (context->connected_discovery) {
        log_info("You are already connected to a server!");
        return false;
    }

    uv_tcp_t* handle = &context->handle;
    uv_tcp_init(loop, handle);

    struct sockaddr_in sockaddr;
    uv_ip4_addr(address, port, &sockaddr);

    uv_connect_t* req = &context->req;
    uv_tcp_connect(req, handle, (const struct sockaddr*) &sockaddr, connect_cb);

    return true;
}

void client_disconnect(client_context* context) {
    if (!context->connected_discovery) {
        return;
    }

    uv_close((uv_handle_t*) &context->handle, close_cb);
}


