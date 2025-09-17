#include "network.h"
#include "callback.h"
#include "common/network.h"
#include "common/wrapper/strncpy.h"
#include "d2c_packets.pb.h"
#include <stdint.h>
#include <uv.h>

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
