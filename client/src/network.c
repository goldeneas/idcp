#include "network.h"
#include "c2c_packets.pb.h"
#include "callback.h"
#include "c2d_packets.pb.h"
#include "client_context.h"
#include "common/log.h"
#include "common/network.h"
#include "common/wrapper/client_info.h"
#include "common/wrapper/strncpy.h"
#include "settings.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <uv.h>

void send_keepalive(struct sockaddr* addr, uv_udp_t* beacon) {
    keepalive_packet packet = keepalive_packet_init_zero;

    MAKE_ENVELOPE(keepalive, c2c_envelope, packet);
    send_c2c_envelope(envelope, addr, beacon, after_write_beacon_buffer_cb);
}

void send_greet(uv_tcp_t* handle, client_id client_id) {
    greet_packet packet = greet_packet_init_zero;

    packet.beacon_port = BEACON_PORT;
    packet.client_id = client_id;
    packet.has_client_id = true;
    packet.has_beacon_port = true;

    MAKE_ENVELOPE(greet, c2d_envelope, packet);
    send_c2d_envelope(envelope, handle, after_write_cb);
}

void send_client_list_request(uv_tcp_t* handle) {
    client_list_request_packet packet = client_list_request_packet_init_zero;

    MAKE_ENVELOPE(client_list_request, c2d_envelope, packet);
    send_c2d_envelope(envelope, handle, after_write_cb);
}

void send_message(struct sockaddr* addr, char* text, uv_udp_t* beacon) {
    message_packet packet = message_packet_init_zero;

    packet.has_text = true;
    text_strncpy(packet.text, text);

    MAKE_ENVELOPE(message, c2c_envelope, packet);
    send_c2c_envelope(envelope, addr, beacon, after_write_beacon_buffer_cb);
}
