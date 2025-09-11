#include "client_list.h"
#include "common.pb.h"
#include "common/log.h"
#include "common/wrapper/client_info.h"
#include <math.h>

void client_list_append(client_info client, client_list* client_list) {
    if (client_list->clients_count >= CLIENT_LIST_MAX_COUNT) {
        FATAL("Cannot append client to client_list: max count reached");
        return;
    }

    client_list->arr[client_list->clients_count] = client;
    client_list->clients_count += 1;
}

void client_list_strcpy(common_client* dst, client_list* client_list) {
    for (uint32_t i = 0; i < client_list->clients_count; i++) {
        client_info* entry = &client_list->arr[i];
        common_client_from_client_info(&dst[i], entry);
    }
}

void client_list_remove(client_id id, client_list* client_list) {
    for (int i = 0; i < client_list->clients_count; i++) {
        client_info* client = &client_list->arr[i];
        if (!client_equals(&id, &client->id)) { continue; }

        FATAL("client_list_remove not implemented yet");
    }
}
