#include "list.h"
#include "c2c_packets.pb.h"
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#define LIST_DEFAULT_CAPACITY 10

list list_init(size_t payload_size, list_equals_fn equals_fn) {
    list list;
    list.payload_size = payload_size;
    list.capacity = LIST_DEFAULT_CAPACITY;
    list.length = 0;
    list.array = malloc(payload_size * LIST_DEFAULT_CAPACITY);
    list.equals_fn = equals_fn;

    return list;
}

void list_destroy(list* list) {
    free(list->array);
}

void list_resize(list* old) {
    list list;
    list.payload_size = list.payload_size;
    list.capacity = list.capacity * 2;
    list.length = list.length;

    list.array = malloc(list.payload_size * list.capacity);
    memcpy(list.array, old->array, old->capacity);

    list_destroy(old);
}

void list_remove_first(void* payload, list* list) {
    if (list->length <= 0) { return; }

    for (size_t i = 0; i < list->length; i++) {
        void* curr = list_get(i, list);
        bool equals = list->equals_fn(curr, payload);

        if (!equals) { continue; }

        // we need to shift left
        if (i < list->length - 1) {
            void* dst = list_get(i, list);
            void* src = list_get(i+1, list);
            size_t bytes = list->payload_size * (list->length - i - 1);
            memmove(dst, src, bytes);
        }

        list->length -= 1;
        return;
    }
}

void* list_get(size_t idx, list* list) {
    return (char*) list->array + idx * list->payload_size;
}

void* list_find(void* payload, list* list) {
    if (list->length == 0) { return NULL; }

    for (size_t i = 0; i < list->length; i++) {
        void* curr = list_get(i, list);
        bool equals = list->equals_fn(curr, payload);

        if (equals) { return curr; }
    }

    return NULL;
}

void* list_push_back(void* payload, list* list) {
    if (list->length >= list->capacity) {
        list_resize(list);
    }

    void* dest = list_get(list->length, list);
    memcpy(dest, payload, list->payload_size);

    list->length += 1;

    return dest;
}
