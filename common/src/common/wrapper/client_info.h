#pragma once
#include "common.pb.h"
#include <stdbool.h>
#include <stdint.h>

#define CLIENT_LIST_NAME_MAX_SIZE 16

typedef uint32_t client_id;

typedef struct client_s {
    char name[CLIENT_LIST_NAME_MAX_SIZE];
    client_id id;
} client_info;

client_info client_info_init(const char* name);
void common_client_from_client_info(common_client* dst, client_info* src);
bool client_equals(client_id* left, client_id* right);
