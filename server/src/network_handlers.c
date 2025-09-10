#include "network_handlers.h"
#include "common/log.h"
#include "common/network_handlers.h"
#include "common/wrapper/client.h"
#include "common/wrapper/client_list.h"
#include "network.h"
#include <uv.h>

void handle_c2d_packet(c2d_envelope* envelope, uv_stream_t* stream) {
    HANDLE_PACKET_BRANCH(client_list_request, envelope, c2d_envelope, on_client_list_request, stream);

    FATAL("Received unhandled packet tag");
}

void on_client_list_request(client_list_request* packet, uv_stream_t* stream) {
    client_info nicola = client_info_init("nicola");
    client_info ivan = client_info_init("ivan");

    client_list list;
    client_list_append(nicola, &list);
    client_list_append(ivan, &list);

    send_client_list((uv_tcp_t*) stream, &list);
}
