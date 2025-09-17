#pragma once
#include "common/list.h"
#include "common/wrapper/client_info.h"
#include <sys/socket.h>
#include <uv.h>

typedef struct address_entry_s {
    client_id id;
    struct sockaddr_storage addr_storage;
} address_entry;

list address_list_init(void);
void address_list_add_address(client_id id, uv_tcp_t* stream, list* address_list);
