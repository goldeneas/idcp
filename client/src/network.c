#include "network.h"
#include "callback.h"
#include "c2d_packets.pb.h"
#include "common/log.h"
#include "common/network.h"
#include "common/wrapper/client_info.h"
#include <stddef.h>
#include <stdint.h>
#include <uv.h>

void send_greet(uv_tcp_t* server, client_id client_id) {
    greet_packet packet = greet_packet_init_zero;

    packet.client_id = client_id;
    packet.has_client_id = true;

    SEND_PACKET_BASE(greet, c2d_envelope, packet, server);
}

void send_client_list_request(uv_tcp_t* server) {
    client_list_request_packet packet = client_list_request_packet_init_zero;
    
    SEND_PACKET_BASE(client_list_request, c2d_envelope, packet, server);
}
