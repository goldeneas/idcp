#include "address_list.h"
#include "common/list.h"
#include "common/network.h"
#include "common/wrapper/client_info.h"
#include <stdbool.h>
#include <sys/socket.h>
#include <uv.h>

bool address_list_equals_fn(void* left, void* right) {
    address_entry* left_entry = left;
    address_entry* right_entry = right;

    return client_id_equals(&left_entry->id, &right_entry->id);
}

list address_list_init(void) {
    list address_list = list_init(sizeof(address_entry));
    return address_list;
}

void address_list_add_address(client_id id, uv_tcp_t* stream, list* address_list) {
    struct sockaddr_storage storage = socket_get_sockaddr_storage(stream);

    address_entry entry = {
        .addr_storage = storage,
        .id = id,
        .handle = stream
    };

    list_push_back(&entry, address_list);
}

uv_tcp_t* address_list_get_stream(client_id id, list* address_list) {
    address_entry* entry = list_find(&id, address_list, address_list_equals_fn);
    if (entry == NULL) { return NULL; }

    return entry->handle;
}

struct sockaddr_storage* address_list_get_address(client_id id, list* address_list) {
    address_entry* entry = list_find(&id, address_list, address_list_equals_fn);
    if (entry == NULL) { return NULL; }

    return &entry->addr_storage;
}
