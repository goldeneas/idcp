#include "peer_list.h"
#include "common/list.h"
#include "common/network.h"
#include "common/wrapper/strncpy.h"
#include <bits/sockaddr.h>
#include <netinet/in.h>
#include <sys/socket.h>

list peer_list_init(void) {
    return list_init(sizeof(peer_entry));
}

void peer_list_add(char* address, in_port_t port, sa_family_t family, list* peer_list) {
    peer_entry entry;

    client_address_strncpy(entry.address, address);
    entry.port = port;
    entry.sa_family = family;
    
    list_push_back(&entry, peer_list);
}
