#include "address_list.h"
#include "common/list.h"
#include "common/network.h"
#include <sys/socket.h>
#include <uv.h>

list address_list_init(void) {
    list address_list = list_init(sizeof(address_entry));
    return address_list;
}

void address_list_add_address(client_id id, uv_tcp_t* stream, list* address_list) {
    struct sockaddr_storage addr_storage =  get_sockaddr(stream);

    address_entry entry = {
        .addr_storage = addr_storage,
        .id = id
    };

    list_push_back(&entry, address_list);
}
