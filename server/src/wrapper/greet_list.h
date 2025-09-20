#pragma once
#include "../settings.h"
#include "c2c_packets.pb.h"
#include "common/linked_list.h"
#include "common/wrapper/client_info.h"
#include "common/list.h"
#include <netinet/in.h>
#include <stddef.h>

typedef struct greet_node_s {
    client_id target_id;
    in_port_t beacon_port;
} greet_node;

typedef struct greet_entry_s {
    client_id requester_id;
    linked_list list;
} greet_entry;


list greet_list_init(void);
void greet_list_set_greet(client_id to, client_id from, in_port_t beacon_port, list* table);
bool greet_list_is_greet_mutual(client_id left, client_id right, list* greet_list);
void greet_list_destroy(list* greet_list);
void greet_list_print_elem(void* element);
greet_node* greet_list_find_node(client_id id, linked_list* linked_list);
in_port_t greet_list_get_beacon_port(client_id from, client_id target, list* greet_list);
greet_entry* greet_list_find_entry(client_id id, list* greet_list);
