#pragma once
#include "../settings.h"
#include "c2c_packets.pb.h"
#include "common/linked_list.h"
#include "common/wrapper/client_info.h"
#include "common/list.h"
#include <stddef.h>

typedef struct greet_entry_s {
    client_id requester_id;
    linked_list list;
} greet_entry;


list greet_list_init(void);
void greet_list_set_greet(client_id to, client_id from, list* table);
bool greet_list_is_greet_mutual(client_id left, client_id right, list* greet_list);
void greet_list_destroy(list* greet_list);
void greet_list_print_elem(void* element);
