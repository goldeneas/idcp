#include "client_info.h"
#include "common.pb.h"
#include "common/wrapper/strncpy.h"
#include <netinet/in.h>
#include <stddef.h>
#include <stdint.h>
#include <string.h>

size_t id_count = 1;

client_info client_info_init(char* name, char* address, in_port_t port) {
    size_t id = id_count;
    id_count += 1;

    client_info client_info;
    client_info.id = id;
    client_info.port = port;

    strncpy(client_info.address, address, CLIENT_ADDRESS_MAX_SIZE);
    name_strncpy(client_info.name, name);

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
