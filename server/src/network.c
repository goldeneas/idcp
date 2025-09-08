#include "network.h"
#include "pb_encode.h"

void send_motd(uv_tcp_t* client, motd_packet* packet) {
    bool status;

    uv_buf_t buf = {
        .base = malloc(sizeof(motd_packet)),
        .len = sizeof(motd_packet),
    };

    pb_ostream_t stream = pb_ostream_from_buffer((pb_byte_t*) buf.base, buf.len);
    status = pb_encode(&stream, motd_packet_fields, packet);

    if (!status) {
        fprintf(stderr, "Something went wrong when encoding motd: %s", PB_GET_ERROR(&stream));
        return;
    }

    uv_write_t wr;
    uv_write(&wr, (uv_stream_t*) client, &buf, 1, NULL);
}
