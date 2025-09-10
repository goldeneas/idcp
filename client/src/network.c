#include "network.h"
#include "callback.h"
#include "c2d_packets.pb.h"
#include "common/network.h"
#include "common/wrapper/strncpy.h"
#include <stddef.h>
#include <stdint.h>
#include <uv.h>

void send_greet(uv_tcp_t* server, char* destination_name, uint32_t source_port) {
    greet_packet packet = greet_packet_init_zero;

    packet.has_source_port = true; 
    packet.has_destination_name = true;

    packet.source_port = source_port;
    destination_name_strncpy(packet.destination_name, destination_name);

    SEND_PACKET_BASE(greet, c2d_envelope, packet, server);
}

void send_client_list_request(uv_tcp_t* server) {
    client_list_request packet = client_list_request_init_zero;
    
    SEND_PACKET_BASE(client_list_request, c2d_envelope, packet, server);
}
