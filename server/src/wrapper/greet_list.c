#include "greet_list.h"
#include "common/linked_list.h"
#include "common/list.h"
#include "common/log.h"
#include "common/wrapper/client_info.h"
#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void greet_entry_linked_list_print_elem(void* element) {
    client_id* id = (client_id*) element;
    log_debug("Link: %u", *id);
}

void greet_table_print_elem(void* element) {
    greet_entry* entry = (greet_entry*) element;
    log_debug("Entry at %p", entry);
    log_debug("Entry has id %i", entry->requester_id);
    log_debug("Entry has linked_list at %p", &entry->list);

    linked_list_print(&entry->list, greet_entry_linked_list_print_elem);
}

bool greet_entry_equals(void* left, void* right) {
    greet_entry* left_entry = (greet_entry*) left;
    greet_entry* right_entry = (greet_entry*) right;

    return client_id_equals(&left_entry->requester_id, &right_entry->requester_id);
}

list greet_list_init(void) {
    list list = list_init(sizeof(greet_entry));
    return list;
}

greet_entry greet_entry_init(client_id id) {
    greet_entry entry;
    entry.requester_id = id;
    entry.list = linked_list_init(sizeof(client_id));

    return entry;
}

void greet_list_set_greet(client_id to, client_id from, list* greet_list) {
    greet_entry* target = list_find((greet_entry*) &from, greet_list, greet_entry_equals);
        
    if (target == NULL) {
        greet_entry entry = greet_entry_init(from);
        target = list_push_back(&entry, greet_list);
    }

    linked_list* client_list = &target->list; 
    if (linked_list_contains(&to, client_list, greet_entry_equals)) {
        return;
    }

    linked_list_append(&to, client_list);
}

void greet_list_destroy(list* greet_list) {
    list_destroy(greet_list);
}
