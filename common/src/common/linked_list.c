#include "linked_list.h"
#include "c2c_packets.pb.h"
#include "common/list.h"
#include "common/log.h"
#include "common/wrapper/client_info.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

linked_list linked_list_init(size_t payload_size) {
    linked_list list;
    list.payload_size = payload_size;
    list.root = NULL;

    return list;
}

linked_node* linked_list_find_or_append(void* payload, linked_list* list,
        linked_list_equals_fn equals_fn) {
    linked_node* node = linked_list_find(payload, list, equals_fn);
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

bool linked_list_contains(void* payload, linked_list* list, linked_list_equals_fn equals_fn) {
    return linked_list_find(payload, list, equals_fn) != NULL;
}

linked_node* linked_list_find(void* payload, linked_list* list, linked_list_equals_fn equals_fn) {
    linked_node* node = list->root;

    while (node != NULL) {
        bool equals = equals_fn(payload, node->payload);
        if (equals) { return node; }

        node = node->next;
    }

    return NULL;
}

linked_node* linked_list_create_node(void* payload, size_t payload_size) {
    linked_node* node = malloc(sizeof(linked_node));

    node->payload = malloc(payload_size);
    memcpy(node->payload, payload, payload_size);
    node->next = NULL;

    return node;
}

void linked_node_destroy(linked_node* node) {
    free(node->payload);
    free(node);
}

void linked_list_print(linked_list* list, linked_list_print_elem_fn print_elem_fn) {
    log_debug("Printing linked list at %p", (void*)list);

    if (!list) {
        log_debug("Linked list is NULL");
        return;
    }

    linked_node* current = list->root;
    size_t index = 0;
    while (current) {
        print_elem_fn(current->payload);
        current = current->next;
        index++;
    }

    if (index == 0) {
        log_debug("List is empty");
    }
}
