#pragma once
#include "common/list.h"
#include "common/wrapper/client_info.h"
#include <bits/sockaddr.h>

typedef struct peer_entry_s {
    char address[17];
    in_port_t port;
    sa_family_t sa_family;
} peer_entry;

list peer_list_init(void);
void peer_list_add(char* address, in_port_t port, sa_family_t family, list* peer_list);
