#include "client_list.h"
#include "common.pb.h"
#include "common/list.h"
#include "common/log.h"
#include "common/wrapper/client_info.h"

list client_list_init(void) {
    list client_list = list_init(sizeof(client_info));
    return client_list;
}

void client_list_add_client(client_info* client, list* client_list) {
    list_push_back(client, client_list);
}

void client_list_remove_client(client_info* client, list* client_list) {
    list_remove_first(client, client_list, client_info_equals);
}

void client_list_strcpy(common_client* dst, list* client_list) {
    for (uint32_t i = 0; i < client_list->length; i++) {
        client_info* entry = list_get(i, client_list);
        common_client_from_client_info(&dst[i], entry);
    }
}
