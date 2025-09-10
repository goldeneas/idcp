#include "client.h"
#include <stddef.h>
#include <string.h>

size_t id_count = 0;

client_info client_info_init(const char* name) {
    size_t id = id_count;
    id_count += 1;

    client_info client_info;
    client_info.id = id;
    strncpy(client_info.name, name, CLIENT_LIST_NAME_MAX_SIZE);
    client_info.name[CLIENT_LIST_NAME_MAX_SIZE - 1] = '\0';

    return client_info;
}

bool client_equals(client_id* left, client_id* right) {
    return *left == *right;
}
