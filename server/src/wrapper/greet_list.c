#include "greet_list.h"
#include "common/linked_list.h"
#include "common/list.h"
#include "common/log.h"
#include "common/wrapper/client_info.h"
#include <math.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

void greet_entry_linked_list_print_elem(void* element) {
    greet_node* id = (greet_node*) element;
    log_debug("Link: %u", id->target_id);
}

bool greet_node_equals_fn(void* left, void* right) {
    greet_node* left_node = (greet_node*) left;
    greet_node* right_node = (greet_node*) right;

    return client_id_equals(&left_node->target_id, &right_node->target_id);
}

void greet_list_print_elem(void* element) {
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
    entry.list = linked_list_init(sizeof(greet_node));

    return entry;
}

in_port_t greet_list_get_beacon_port(client_id from, client_id target, list* greet_list) {
    greet_entry* entry = greet_list_find_entry(from, greet_list);
    greet_node* node = greet_list_find_node(target, &entry->list);

    return node->beacon_port;
}

bool greet_list_is_greet_mutual(client_id left, client_id right, list* greet_list) {
    greet_entry* left_entry = greet_list_find_entry(left, greet_list);
    greet_entry* right_entry = greet_list_find_entry(right, greet_list);

    if (left_entry == NULL || right_entry == NULL) { return false; }

    bool left_contains = linked_list_contains(&right, &left_entry->list, greet_node_equals_fn);
    bool right_contains = linked_list_contains(&left, &right_entry->list, greet_node_equals_fn);

    return left_contains && right_contains;
}

greet_node* greet_list_find_node(client_id id, linked_list* linked_list) {
    greet_node tmp;
    tmp.target_id = id;

    linked_node* node = linked_list_find(&tmp, linked_list, greet_node_equals_fn); 
    return node->payload;
}

greet_entry* greet_list_find_entry(client_id id, list* greet_list) {
    greet_entry tmp;
    tmp.requester_id = id;

    return list_find(&tmp, greet_list, greet_entry_equals);
}

void greet_list_set_greet(client_id to, client_id from, in_port_t beacon_port, list* greet_list) {
    greet_entry tmp;
    tmp.requester_id = from;

    greet_entry* target = list_find(&tmp, greet_list, greet_entry_equals);
        
    if (target == NULL) {
        greet_entry entry = greet_entry_init(from);
        target = list_push_back(&entry, greet_list);
    }

    linked_list* client_list = &target->list; 
    if (linked_list_contains(&to, client_list, greet_entry_equals)) {
        return;
    }

    greet_node node;
    node.target_id = to;
    node.beacon_port = beacon_port;
    linked_list_append(&node, client_list);
}

void greet_list_destroy(list* greet_list) {
    list_destroy(greet_list);
}
