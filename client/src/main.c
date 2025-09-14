#include "client_context.h"
#include "settings.h"
#include "callback.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <uv.h>

int main(void) {
    client_context client_context = client_context_init();

    uv_loop_t* loop = uv_default_loop();
    loop->data = &client_context;

    uv_tcp_t server;
    uv_tcp_init(loop, &server);
    client_context.server = &server;

    uv_tty_t tty;
    uv_tty_init(loop, &tty, 0, 1);
    uv_read_start((uv_stream_t*) &tty, alloc_buffer_cb, read_tty_buffer_cb);

    struct sockaddr_in address;
    uv_ip4_addr(SERVER_ADDR, SERVER_PORT, &address);

    uv_connect_t* req = malloc(sizeof(uv_connect_t));
    uv_tcp_connect(req, &server, (const struct sockaddr*) &address, connect_cb);

    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);

    free(req);
    return 0;
}
