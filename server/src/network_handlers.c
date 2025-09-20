#include "network_handlers.h"
#include "common/list.h"
#include "common/log.h"
#include "common/network.h"
#include "common/network_handlers.h"
#include "common/wrapper/client_info.h"
#include "wrapper/address_list.h"
#include "wrapper/client_list.h"
#include "network.h"
#include "server_context.h"
#include "wrapper/greet_list.h"
#include <netinet/in.h>
#include <sys/socket.h>
#include <uv.h>

void on_client_list_request(client_list_request_packet* packet, uv_stream_t* stream,
        server_context* context) {
    list* client_list = &context->client_list;
    send_client_list((uv_tcp_t*) stream, client_list);
}

void on_client_greet(greet_packet* packet, uv_stream_t* stream, server_context* context) {
    client_id to = packet->client_id;
    if (to == 0) { return; }

    list* client_list = &context->client_list;
    client_info* info = client_list_get_client_info((uv_tcp_t*) stream, client_list);
    if (info == NULL) {
        log_info("Could not get client info");
        return;
    }

    client_id from = info->id;
    greet_list_set_greet(to, from, &context->greet_list);

    bool is_mutual = greet_list_is_greet_mutual(to, from, &context->greet_list);
    if (!is_mutual) { return; }

    list* address_list = &context->address_list;
    struct sockaddr_storage* left_addr = address_list_get_address(from, address_list);
    struct sockaddr_storage* right_addr = address_list_get_address(to, address_list);

    uv_tcp_t* left_handle = address_list_get_stream(from, address_list);
    uv_tcp_t* right_handle = address_list_get_stream(to, address_list);

    send_greet_established(left_handle, left_addr);
    send_greet_established(right_handle, right_addr);
}

void handle_c2d_packet(c2d_envelope* envelope, uv_stream_t* stream, server_context* context) {
    HANDLE_PACKET_BRANCH(client_list_request, envelope, c2d_envelope, on_client_list_request,
            stream, context);
    HANDLE_PACKET_BRANCH(greet, envelope, c2d_envelope, on_client_greet, stream, context);

    FATAL("Received unhandled packet tag");
}
