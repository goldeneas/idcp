#include "network.h"
#include "log.h"
#include "pb_encode.h"
#include "callback.h"
#include "c2d_packets.pb.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

void send_greet(uv_tcp_t* server, greet_packet packet) {
    bool status;
    size_t envelope_size;

    c2d_envelope envelope = c2d_envelope_init_zero;
    envelope.which_payload = c2d_envelope_greet_tag;
    envelope.payload.greet = packet; 

    if (!pb_get_encoded_size(&envelope_size, c2d_envelope_fields, &envelope)) {
        log_error("Could not get encoded size for greet packet!");
        return;
    }

    uv_buf_t buf = {
        .base = malloc(envelope_size),
        .len = envelope_size,
    };

    log_info("buf len is %d", envelope_size);
    pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t*) buf.base, buf.len);
    status = pb_encode(&stream, c2d_envelope_fields, &envelope);

    if (!status) {
        log_error("Something went wrong when encoding motd: %s", PB_GET_ERROR(&stream));
        return;
    }

    uv_write_t* wr = malloc(sizeof(uv_write_t));
    wr->data = &buf;

    // TODO: Fix after write free
    uv_write(wr, (uv_stream_t*) server, &buf, 1, NULL);
}
