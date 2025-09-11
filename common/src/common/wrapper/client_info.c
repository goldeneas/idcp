#include "client_info.h"
#include "common.pb.h"
#include "common/wrapper/strncpy.h"
#include <stddef.h>
#include <string.h>

size_t id_count = 1;

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

void common_client_from_client_info(common_client* dst, client_info* src) {
    common_client common = common_client_init_zero;
    common.has_id = true;
    common.has_name = true;

    name_strncpy(common.name, src->name);
    common.id = src->id;

    *dst = common;
}
