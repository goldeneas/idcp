#include "callback.h"
#include "assert_wrapper.h"
#include "log.h"
#include "network.h"
#include "pb.h"
#include "pb_decode.h"
#include "c2d_packets.pb.h"
#include "common.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <uv.h>

void connection_cb(uv_stream_t* server, int status) {
    ASSERT_GE(status, 0);

    uv_tcp_t* client = malloc(sizeof(uv_tcp_t));
    uv_tcp_init(server->loop, client);

    if (uv_accept(server, (uv_stream_t*) client) == 0) {
        char address[40];
        get_socket_addr(client, address, 39);
        int port = get_socket_port(client);

        log_info("A new client has connected! [%s:%hu]", address, port);
        uv_read_start((uv_stream_t*) client, alloc_buffer_cb, read_buffer_cb);
    } else {
        uv_close((uv_handle_t*) client, NULL);
        free(client);
    }
}

void after_write_cb(uv_write_t* req, int status) {

}

void alloc_buffer_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

void read_buffer_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    if (nread < 0 && nread != UV_EOF) {
        FATAL("nread < 0 while in read_buffer_cb");
    }

    if (nread == UV_EOF) {
        log_info("A client has disconnected");
        return;
    }

    pb_istream_t pb_stream = pb_istream_from_buffer((uint8_t*) buf->base, nread);

    c2d_envelope envelope = c2d_envelope_init_zero;
    ASSERT(pb_decode(&pb_stream, c2d_envelope_fields, &envelope));
    
    log_info("Got message: %u", envelope.which_payload);
    greet_packet* greet = (greet_packet*) &envelope.payload;
    log_info("greet is: %s", greet->destinationName);
}
