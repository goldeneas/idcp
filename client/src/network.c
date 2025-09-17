#include "network.h"
#include "c2c_packets.pb.h"
#include "callback.h"
#include "c2d_packets.pb.h"
#include "common/log.h"
#include "common/network.h"
#include "common/wrapper/client_info.h"
#include "common/wrapper/strncpy.h"
#include <stddef.h>
#include <stdint.h>
#include <string.h>
#include <uv.h>

void send_greet(uv_tcp_t* handle, client_id client_id) {
    greet_packet packet = greet_packet_init_zero;

    packet.client_id = client_id;
    packet.has_client_id = true;

    SEND_PACKET_BASE(greet, c2d_envelope, packet, handle);
}

void send_client_list_request(uv_tcp_t* handle) {
    client_list_request_packet packet = client_list_request_packet_init_zero;
    
    SEND_PACKET_BASE(client_list_request, c2d_envelope, packet, handle);
}

void send_message(struct sockaddr* addr, char* text, size_t len) {
    message_packet packet = message_packet_init_zero;

    if (len >= sizeof(packet.text)) {
        log_error("Tried sending text with length %i: max length reached.");
        return;
    }

    packet.has_text = true;
    text_strncpy(packet.text, text);

    SEND_P
}
