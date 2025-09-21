#include "network_handlers.h"
#include "c2c_packets.pb.h"
#include "client_context.h"
#include "common.pb.h"
#include "common/log.h"
#include "common/str_utils.h"
#include "d2c_packets.pb.h"
#include "common/network_handlers.h"
#include "pb.h"
#include "peer_list.h"
#include <stdio.h>
#include <sys/socket.h>
#include <uv.h>

void on_greet_established(greet_established_packet* packet, uv_stream_t* stream,
        client_context* context) {
    log_info("Client at %s:%i has accepted our greeting", packet->client_address, packet->client_port);
    peer_list_add(packet->client_address, packet->client_port, packet->sa_family, &context->peer_list);
}

void on_motd(motd_packet* packet, uv_stream_t* stream, client_context* context) {
    log_info("Server '%s' sent the following MOTD:", packet->server_name);
    log_info("%s", packet->motd);
}

void on_client_list(client_list_packet* packet, uv_stream_t* stream, client_context* context) {
    log_info("Here are the available clients:");

    pb_size_t count = packet->clients_count;
    if (count == 0) {
        log_info("There is nobody connected. How lonely...");
        return;
    }

    for (int i = 0; i < packet->clients_count; i++) {
        common_client* client = &packet->clients[i];
        printf("%s (%i), ", client->name, client->id);
    }

    printf("\n");
}

void handle_d2c_packet(d2c_envelope* envelope, uv_stream_t* stream, client_context* context) {
    HANDLE_PACKET_BRANCH(motd, envelope, d2c_envelope, on_motd, stream, context);
    HANDLE_PACKET_BRANCH(client_list, envelope, d2c_envelope, on_client_list, stream, context);
    HANDLE_PACKET_BRANCH(greet_established, envelope, d2c_envelope, on_greet_established, stream, context);

    FATAL("Received unhandled packet tag");
}

void on_peer_message(message_packet* packet, const struct sockaddr* sockaddr, client_context* context) {
    char* text = packet->text;
    str_remove_nl(text);

    log_info("Someone said: %s", text);
}

void on_peer_keepalive(keepalive_packet* packet, const struct sockaddr* sockaddr, client_context* context) {

}

void handle_c2c_packet(c2c_envelope* envelope, const struct sockaddr* sockaddr, client_context* context) {
    HANDLE_DATAGRAM_BRANCH(message, envelope, c2c_envelope, on_peer_message, sockaddr, context);
    HANDLE_DATAGRAM_BRANCH(keepalive, envelope, c2c_envelope, on_peer_keepalive, sockaddr, context);

    FATAL("Client sent unhandled packet tag");
}
