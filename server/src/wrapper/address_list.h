#pragma once
#include "common/list.h"
#include "common/wrapper/client_info.h"
#include <sys/socket.h>
#include <uv.h>

typedef struct address_entry_s {
    client_id id;
    struct sockaddr_storage addr_storage;
    uv_tcp_t* handle;
} address_entry;

list address_list_init(void);
void address_list_add_address(client_id id, uv_tcp_t* stream, list* address_list);
struct sockaddr_storage* address_list_get_address(client_id id, list* address_list);
uv_tcp_t* address_list_get_stream(client_id id, list* address_list);
