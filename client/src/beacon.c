#include "beacon.h"
#include "client_context.h"
#include "callback.h"
#include "common/log.h"
#include <stdbool.h>
#include <uv.h>

void beacon_start(int port, uv_loop_t* loop, client_context* context) {
    int result = uv_udp_init(loop, &context->beacon);
    if (result != 0) {
        FATAL("Beacon start error");
    }

    struct sockaddr_in sockaddr;
    uv_ip4_addr("0.0.0.0", port, &sockaddr);

    uv_udp_bind(&context->beacon, (struct sockaddr*) &sockaddr, 0);
    uv_udp_recv_start(&context->beacon, alloc_buffer_cb, read_beacon_buffer_cb);

    log_info("Beacon started on 0.0.0.0:%i", port);
}
