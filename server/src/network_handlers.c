#include "network_handlers.h"
#include "common/log.h"
#include "common/network.h"
#include "common/network_handlers.h"
#include "common/wrapper/client_info.h"
#include "wrapper/client_list.h"
#include "network.h"
#include "server_context.h"
#include "wrapper/greet_list.h"
#include <uv.h>

void on_client_list_request(client_list_request_packet* packet, uv_stream_t* stream,
        server_context* context) {
    list* client_list = &context->client_list;
    send_client_list((uv_tcp_t*) stream, client_list);
}

void on_client_greet(greet_packet* packet, uv_stream_t* stream, server_context* context) {
    client_id to = packet->client_id;
    if (to == 0) { return; }

    client_info* info = server_context_get_client((uv_tcp_t*) stream, context);
    greet_list_set_greet(to, info->id, &context->greet_list);
}

void handle_c2d_packet(c2d_envelope* envelope, uv_stream_t* stream, server_context* context) {
    HANDLE_PACKET_BRANCH(client_list_request, envelope, c2d_envelope, on_client_list_request,
            stream, context);

    HANDLE_PACKET_BRANCH(greet, envelope, c2d_envelope, on_client_greet, stream, context);

    FATAL("Received unhandled packet tag");
}
