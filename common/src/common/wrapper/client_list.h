#pragma once
#include "pb.h"
#include <stdint.h>
#include "client_info.h"

#define CLIENT_LIST_MAX_COUNT 1024

typedef struct client_list_s {
    client_info arr[CLIENT_LIST_MAX_COUNT];
    pb_size_t clients_count;
} client_list;

void client_list_append(client_info client, client_list* client_list);
void client_list_strcpy(common_client* dst, client_list* client_list);
void client_list_remove(client_id id, client_list* client_list);
