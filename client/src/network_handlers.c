#include "network_handlers.h"
#include "common/log.h"
#include "d2c_packets.pb.h"
#include "common/network_handlers.h"
#include <stdio.h>
#include <uv.h>

void handle_d2c_packet(d2c_envelope* envelope, uv_stream_t* stream) {
    HANDLE_PACKET_BRANCH(motd, envelope, d2c_envelope, on_motd, stream);
    HANDLE_PACKET_BRANCH(client_list, envelope, d2c_envelope, on_client_list, stream);

    FATAL("Received unhandled packet tag");
}

void on_motd(motd_packet* packet, uv_stream_t* stream) {

}

void on_client_list(client_list_packet* packet, uv_stream_t* stream) {
    log_info("Here are the available clients:");

    for (int i = 0; i < packet->clients_count; i++) {
        printf("%s ", packet->clients[i]);
    }

    printf("\n");
}
