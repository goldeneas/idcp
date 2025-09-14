#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef bool (*list_equals_fn)(void* left, void* right);
typedef void (*list_print_elem_fn)(void* element);

typedef struct list_s {
    size_t capacity;
    size_t length;

    size_t payload_size;
    void* array;
} list;

list list_init(size_t payload_size);
void* list_push_back(void* payload, list* list);
void list_resize(list* old);
void list_remove_first(void* payload, list* list, list_equals_fn equals_fn);
void* list_find(void* payload, list* list, list_equals_fn equals_fn);
void* list_get(size_t idx, list* list);
void list_destroy(list* list);
void list_print(list* list, list_print_elem_fn print_elem_fn);
