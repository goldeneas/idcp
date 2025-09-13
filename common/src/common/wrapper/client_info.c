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

bool client_id_equals(void* left, void* right) {
    client_id* left_id = (client_id*) left;
    client_id* right_id = (client_id*) right;

    return left_id == right_id;
}

bool client_info_equals(void* left, void* right) {
    client_info* left_info = (client_info*) left;
    client_info* right_info = (client_info*) right;

    return client_id_equals(&left_info->id, &right_info->id);
}

void common_client_from_client_info(common_client* dst, client_info* src) {
    common_client common = common_client_init_zero;
    common.has_id = true;
    common.has_name = true;

    name_strncpy(common.name, src->name);
    common.id = src->id;

    *dst = common;
}
