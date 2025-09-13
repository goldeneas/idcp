#pragma once
#include <stdbool.h>
#include <stddef.h>

typedef bool (*list_equals_fn)(void* left, void* right);

typedef struct list_s {
    size_t capacity;
    size_t length;

    size_t payload_size;
    void* array;

    list_equals_fn equals_fn;
} list;

list list_init(size_t payload_size, list_equals_fn equals_fn);
void* list_push_back(void* payload, list* list);
void list_resize(list* old);
void list_remove_first(void* payload, list* list);
void* list_find(void* payload, list* list);
void list_destroy(list* list);
