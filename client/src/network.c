#include "network.h"
#include "common.h"
#include "callback.h"
#include "c2d_packets.pb.h"
#include "d2c_packets.pb.h"
#include <stddef.h>
#include <stdint.h>
#include <uv.h>

void send_greet(uv_tcp_t* server, char* destination_name, uint32_t source_port) {
    greet_packet packet = greet_packet_init_zero;

    packet.has_source_port = true; 
    packet.has_destination_name = true;

    packet.source_port = source_port;
    strcpy(packet.destination_name, destination_name);

    SEND_PACKET_BASE(greet, c2d_envelope, packet, server);
}

void list_clients(uv_tcp_t* server) {
    list_clients_packet packet = list_clients_packet_init_zero;
    
    SEND_PACKET_BASE(list_clients, c2d_envelope, packet, server);
}
