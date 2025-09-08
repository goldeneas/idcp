#include "d2c_packets.pb.h"
#include "settings.h"
#include "callback.h"
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/socket.h>
#include <uv.h>
#include <uv/unix.h>
#include "common.h"

motd_packet motd = {
    .connectedClients = 1,
};

int main(void) {
    uv_loop_t* loop = uv_default_loop();

    uv_tcp_t server;
    uv_tcp_init(loop, &server);

    struct sockaddr_in address;
    uv_ip4_addr("0.0.0.0", SERVER_PORT, &address);

    uv_tcp_bind(&server, (const struct sockaddr*) &address, 0);
    int result = uv_listen((uv_stream_t*) &server, DEFAULT_BACKLOG, connection_cb);
    if (result) {
        fprintf(stderr, "Listen error: %s\n", uv_strerror(result));
        return -1;
    }

    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);

    return 0;
}
