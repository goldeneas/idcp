#include "client_list.h"
#include "common.pb.h"
#include "common/list.h"
#include "common/log.h"
#include "common/network.h"
#include "common/wrapper/client_info.h"
#include <uv.h>

list client_list_init(void) {
    list client_list = list_init(sizeof(client_info));
    return client_list;
}

void client_list_add_client(client_info* client, list* client_list) {
    list_push_back(client, client_list);
}

void client_list_remove_client(client_info* client, list* client_list) {
    list_remove_first(client, client_list, client_info_equals);
}

client_info* client_list_get_client_info(uv_tcp_t* client, list* client_list) {
    char address[40];
    in_port_t port;
    struct sockaddr_storage storage = socket_get_sockaddr_storage(client);
    socket_extract_info(&storage, &port, address, 39);

    for (size_t i = 0; i < client_list->length; i++) {
        client_info* curr = list_get(i, client_list);
        if (curr->port != port) { continue; }
        if (strcmp(curr->address, address) != 0) { continue; }

        return curr;
    }

    return NULL;
}

void client_list_strcpy(common_client* dst, list* client_list) {
    for (uint32_t i = 0; i < client_list->length; i++) {
        client_info* entry = list_get(i, client_list);
        common_client_from_client_info(&dst[i], entry);
    }
}
