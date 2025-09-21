#include "network.h"
#include "callback.h"
#include "common/network.h"
#include "common/wrapper/strncpy.h"
#include "d2c_packets.pb.h"
#include <bits/sockaddr.h>
#include <netinet/in.h>
#include <stdint.h>
#include <sys/socket.h>
#include <uv.h>

void send_greet_established(uv_tcp_t* client, struct sockaddr_storage* sockaddr,
        in_port_t beacon_port) {
    sa_family_t family;
    char address[40];
    in_port_t port;
    socket_extract_info(sockaddr, &port, address, &family, 39);

    greet_established_packet packet = greet_established_packet_init_zero;

    packet.has_client_address = true;
    packet.has_client_port = true;
    packet.has_sa_family = true;

    packet.client_port = beacon_port;
    packet.sa_family = family;
    client_address_strncpy(packet.client_address, address);

    MAKE_ENVELOPE(greet_established, d2c_envelope, packet)
    send_d2c_envelope(envelope, client, after_write_cb);
}

void send_motd(uv_tcp_t* client, char* server_name, char* motd) {
    motd_packet packet = motd_packet_init_zero;

    packet.has_motd = true;
    packet.has_server_name = true;

    motd_strncpy(packet.motd, motd);
    server_name_strncpy(packet.server_name, server_name);

    MAKE_ENVELOPE(motd, d2c_envelope, packet);
    send_d2c_envelope(envelope, client, after_write_cb);
}

void send_client_list(uv_tcp_t* client, list* client_list) {
    client_list_packet packet = client_list_packet_init_zero;

    packet.clients_count = client_list->length;
    client_list_strcpy(packet.clients, client_list);

    MAKE_ENVELOPE(client_list, d2c_envelope, packet);
    send_d2c_envelope(envelope, client, after_write_cb);
} 
