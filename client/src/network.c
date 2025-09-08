#include "network.h"
#include "common.h"
#include "callback.h"
#include "c2d_packets.pb.h"
#include <stddef.h>
#include <stdlib.h>
#include <uv.h>

void send_greet(uv_tcp_t* server, greet_packet packet) {
    c2d_envelope envelope = c2d_envelope_init_zero;
    envelope.which_payload = c2d_envelope_greet_tag;
    envelope.payload.greet = packet; 

    send_c2d_envelope(envelope, (uv_stream_t*) server, after_write_cb);
}
