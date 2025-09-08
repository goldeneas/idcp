#include "callback.h"
#include "common.h"
#include "log.h"
#include "network.h"
#include "c2d_packets.pb.h"
#include <stdlib.h>
#include <string.h>

void connect_cb(uv_connect_t* req, int status) {
    if (status < 0) {
        fprintf(stderr, "Error on connection: %s\n", uv_strerror(status));
        return;
    }

    uv_read_start(req->handle, alloc_buffer_cb, read_buffer_cb);
    log_info("Connected to server!");

    greet_packet pkt = greet_packet_init_zero;
    pkt.sourcePort = 20;
    strcpy(pkt.destinationName, "hello");
    send_greet((uv_tcp_t*) req->handle, pkt);
}

void alloc_buffer_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

void read_buffer_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {

}

void after_write_cb(uv_write_t* wr, int status) {
    write_req_t* req = (write_req_t*) wr;

    free(req->buf.base);
    free(req);

    if (status == 0) { return; }
    log_error("Error on write: %s", uv_strerror(status));
}
