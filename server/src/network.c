#include "network.h"
#include "callback.h"
#include "d2c_packets.pb.h"
#include "network_strcpy.h"
#include "pb.h"
#include <stdint.h>
#include <string.h>
#include <uv.h>

void send_motd(uv_tcp_t* client, char* server_name, char* motd) {
    motd_packet packet = motd_packet_init_zero;

    packet.has_motd = true;
    packet.has_server_name = true;

    strcpy_motd(packet.motd, motd);
    strcpy_server_name(packet.server_name, server_name);

    SEND_PACKET_BASE(motd, d2c_envelope, packet, client);
}

void send_client_list(uv_tcp_t* client, char** clients, pb_size_t connected_clients) {
    client_list_packet packet = client_list_packet_init_zero;

    packet.clients_count = connected_clients;
    mtrx_strcpy_clients(packet.clients, clients, connected_clients);

    SEND_PACKET_BASE(client_list, d2c_envelope, packet, client);
} 
