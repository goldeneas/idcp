#include "server_context.h"
#include "common/list.h"
#include "common/log.h"
#include "common/network.h"
#include "common/wrapper/client_info.h"
#include "wrapper/address_list.h"
#include "wrapper/client_list.h"
#include "wrapper/greet_list.h"
#include <netinet/in.h>
#include <stdatomic.h>
#include <stddef.h>
#include <string.h>
#include <sys/socket.h>
#include <uv.h>

server_context server_context_init(void) {
    server_context ctx;
    ctx.client_list = client_list_init();
    ctx.greet_list = greet_list_init();
    ctx.address_list = address_list_init();

    return ctx;
}

void server_context_add_client(client_info* info, uv_tcp_t* client, server_context* context) {
    client_list_add_client(info, &context->client_list);
    address_list_add_address(info->id, client, &context->address_list);
}

uv_tcp_t* server_context_get_client(client_id id, server_context* context) {
    FATAL("Unimplemented");
}

client_info* server_context_get_client_info(uv_tcp_t* client, server_context* context) {
    list* client_list = &context->client_list; 

    char address[40];
    in_port_t port;
    struct sockaddr_storage storage = socket_get_sockaddr_storage(client);
    socket_extract_info(&storage, &port, address, 39);

    log_info("Searching for %s:%i", address, port);

    for (size_t i = 0; i < client_list->length; i++) {
        client_info* curr = list_get(i, client_list);
        if (curr->port != port) { continue; }
        if (strcmp(curr->address, address) != 0) { continue; }

        return curr;
    }

    return NULL;
}
