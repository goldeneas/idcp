#include "callback.h"
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

    uv_read_start(client->handle, alloc_buffer_cb, read_d2c_buffer_cb);
    log_info("Connected to server!");

    client_context* context = client->handle->loop->data;
    context->connected = true;

    send_client_list_request((uv_tcp_t*) client->handle);
}

void alloc_buffer_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf) {
    buf->base = malloc(suggested_size);
    buf->len = suggested_size;
}

void after_write_cb(uv_write_t* wr, int status) {
    write_req* req = (write_req*) wr;

    free(req->buf.base);
    free(req);

    if (status == 0) { return; }
    log_error("Error on write: %s", uv_strerror(status));
}

void read_d2c_buffer_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf) {
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

    log_debug("Received message from server");
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
    handle_tty_command(buf, context);
}

void close_cb(uv_handle_t* handle) {
    client_context* context = handle->loop->data;
    context->connected = false;

    free(handle);

    log_info("Disconnected");
}
