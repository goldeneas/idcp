#include "server_context.h"
#include "common/list.h"
#include "common/network.h"
#include "common/wrapper/client_info.h"
#include "wrapper/client_list.h"
#include "wrapper/greet_list.h"
#include <netinet/in.h>
#include <stddef.h>
#include <string.h>

server_context server_context_init(void) {
    server_context ctx;
    ctx.client_list = client_list_init();
    ctx.greet_list = greet_list_init();

    return ctx;
}

client_info* server_context_get_client(uv_tcp_t* client, server_context* context) {
    list* client_list = &context->client_list; 

    char address[40];
    get_socket_addr(client, address, 39);
    in_port_t port = get_socket_port(client);

    for (size_t i = 0; i < client_list->length; i++) {
        client_info* curr = list_get(i, client_list);
        if (curr->port != port) { continue; }
        if (strcmp(curr->address, address) != 0) { continue; }

        return curr;
    }

    return NULL;
}
