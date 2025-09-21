#include "callback.h"
#include "c2c_packets.pb.h"
#include "client_context.h"
#include "command_handlers.h"
#include "d2c_packets.pb.h"
#include "common/log.h"
#include "network.h"
#include "common/network.h"
#include "network_handlers.h"
#include "pb_decode.h"
#include <stdlib.h>
#include <string.h>
#include <uv.h>

void connect_cb(uv_connect_t* client, int status) {
    if (status < 0) {
        fprintf(stderr, "Error on connection: %s\n", uv_strerror(status));
        return;
    }

    uv_read_start(client->handle, alloc_buffer_cb, read_c2d_buffer_cb);
    log_info("Connected to server!");

    client_context* context = client->handle->loop->data;
    context->connected_discovery = true;

    send_client_list_request((uv_tcp_t*) client->handle);
}

void alloc_buffer_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

void after_write_cb(uv_write_t* wr, int status) {
    if (status== 0) { return; }
    log_error("Error on write: %s", uv_strerror(status));
}

void read_c2d_buffer_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    if (nread < 0 && nread != UV_EOF) {
        uv_close((uv_handle_t*) stream, NULL);
        FATAL("nread < 0");
    }

    if (nread == UV_EOF) {
        log_info("Connection to server lost. Please connect to a new server");
        return;
    }

    client_context* client_context = stream->loop->data;
    pb_istream_t pb_stream = pb_istream_from_buffer((uint8_t*) buf->base, nread);

    d2c_envelope envelope = d2c_envelope_init_zero;
    if (!pb_decode(&pb_stream, d2c_envelope_fields, &envelope)) {
        log_debug("Received a message that is not a d2c_envelope");
        return;
    }

    handle_d2c_packet(&envelope, stream, client_context);
}

void read_tty_buffer_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
    if (nread < 0 && nread != UV_EOF) {
        uv_close((uv_handle_t*) stream, NULL);
        FATAL("nread < 0");
    }

    if (nread == UV_EOF) {
        log_debug("nread == 0");
        return;
    }

    client_context* context = stream->loop->data;
    handle_tty_input(buf, context);
}

void read_beacon_buffer_cb(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf,
        const struct sockaddr* addr, unsigned flags) {
    if (nread <= 0) { return; }

    if (nread == UV_EOF) {
        log_info("Connection to client lost.");
        return;
    }

    client_context* client_context = handle->loop->data;
    pb_istream_t pb_stream = pb_istream_from_buffer((uint8_t*) buf->base, nread);

    c2c_envelope envelope = c2c_envelope_init_zero;
    if (!pb_decode(&pb_stream, c2c_envelope_fields, &envelope)) {
        log_debug("Beacon received a message that is not a c2c_envelope");
        return;
    }

    handle_c2c_packet(&envelope, addr, client_context);
}

void after_write_beacon_buffer_cb(uv_udp_send_t* req, int status) {
    if (status < 0) {
        log_error("Fatal error when sending UDP: %s", uv_strerror(status));
        return;
    }

    free(req);
}

void close_cb(uv_handle_t* handle) {
    client_context* context = handle->loop->data;
    context->connected_discovery = false;

    log_info("Disconnected");
}
