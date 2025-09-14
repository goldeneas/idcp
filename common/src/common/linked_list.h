#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef bool (*linked_list_equals_fn)(void* left, void* right);
typedef void (*linked_list_print_elem_fn)(void* element);

typedef struct node_s {
    struct node_s* next;
    void* payload;
} linked_node;

typedef struct linked_list_s {
    linked_node* root;
    size_t payload_size;
} linked_list;

void linked_node_destroy(linked_node* node);
linked_list linked_list_init(size_t payload_size);
linked_node* linked_list_append(void* payload, linked_list* list);
linked_node* linked_list_find_or_append(void* payload, linked_list* list,
        linked_list_equals_fn equals_fn);
linked_node* linked_list_find(void* payload, linked_list* list, linked_list_equals_fn equals_fn);
bool linked_list_contains(void* payload, linked_list* list, linked_list_equals_fn equals_fn);
linked_node* linked_list_create_node(void* payload, size_t payload_size);
void linked_list_print(linked_list* list, linked_list_print_elem_fn print_elem_fn);
