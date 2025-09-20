#pragma once
#include "common/list.h"
#include "common/wrapper/client_info.h"

typedef struct peer_entry_s {
    char address[17];
    in_port_t port;
} peer_entry;

list peer_list_init(void);
void peer_list_add(char* address, in_port_t port, list* peer_list);
