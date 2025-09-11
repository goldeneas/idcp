#include "network_handlers.h"
#include "client_context.h"
#include "common/log.h"
#include "d2c_packets.pb.h"
#include "common/network_handlers.h"
#include "pb.h"
#include <stdio.h>
#include <uv.h>

void handle_d2c_packet(d2c_envelope* envelope, uv_stream_t* stream, client_context* context) {
    HANDLE_PACKET_BRANCH(motd, envelope, d2c_envelope, on_motd, stream, context);
    HANDLE_PACKET_BRANCH(client_list, envelope, d2c_envelope, on_client_list, stream, context);

    FATAL("Received unhandled packet tag");
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
        printf("%s, ", packet->clients[i].name);
    }

    printf("\n");
}
