#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef bool (*linked_list_equals)(void* left, void* right);

typedef struct node_s {
    struct node_s* next;
    void* payload;
} linked_node;

typedef struct linked_list_s {
    linked_node* root;
    size_t payload_size;

    linked_list_equals equals_fn;
} linked_list;

linked_list linked_list_init(linked_list_equals equals_fn, size_t payload_size);
linked_node* linked_list_append(void* payload, linked_list* list);
linked_node* linked_list_find_or_append(void* payload, linked_list* list);
linked_node* linked_list_find(void* payload, linked_list* list);
linked_node* linked_list_create_node(void* payload, size_t payload_size);
bool linked_list_contains(void* payload, linked_list* list);
