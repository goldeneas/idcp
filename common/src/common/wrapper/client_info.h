#pragma once
#include "common.pb.h"
#include <netinet/in.h>
#include <stdbool.h>
#include <stdint.h>

#define CLIENT_LIST_NAME_MAX_SIZE 16
#define CLIENT_ADDRESS_MAX_SIZE 16

typedef uint32_t client_id;

typedef struct client_s {
    in_port_t port;
    client_id id;
    char name[CLIENT_LIST_NAME_MAX_SIZE];
    char address[CLIENT_ADDRESS_MAX_SIZE];
} client_info;

client_info client_info_init(char* name, char* address, in_port_t port);
bool client_info_equals(void* left, void* right);
bool client_id_equals(void* left, void* right);
void common_client_from_client_info(common_client* dst, client_info* src);
