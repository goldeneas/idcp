#include "linked_list.h"
#include "c2c_packets.pb.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

linked_list linked_list_init(linked_list_equals equals_fn, size_t payload_size) {
    linked_list list;
    list.equals_fn = equals_fn;
    list.payload_size = payload_size;
    list.root = NULL;

    return list;
}

linked_node* linked_list_find_or_append(void* payload, linked_list* list) {
    linked_node* node = linked_list_find(payload, list);
    if (node != NULL) {
        return node;
    }

    return linked_list_append(payload, list);
}

linked_node* linked_list_append(void* payload, linked_list* list) {
    linked_node* payload_node = linked_list_create_node(payload, list->payload_size);

    if (list->root == NULL) {
        list->root = payload_node;
        return payload_node;
    }

    linked_node* tmp = list->root;
    while (tmp->next != NULL) {
        tmp = tmp->next;
    }

    tmp->next = payload_node;
    return payload_node;
}

bool linked_list_contains(void* payload, linked_list* list) {
    return linked_list_find(payload, list) != NULL;
}

linked_node* linked_list_find(void* payload, linked_list* list) {
    linked_node* node = list->root;

    while (node != NULL) {
        bool equals = list->equals_fn(payload, node->payload);
        if (equals) { return node; }

        node = node->next;
    }

    return NULL;
}

linked_node* linked_list_create_node(void* payload, size_t payload_size) {
    linked_node* node = malloc(payload_size);
    node->payload = payload;
    node->next = NULL;

    return node;
}
