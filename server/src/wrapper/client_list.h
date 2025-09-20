#pragma once
#include "pb.h"
#include <stdint.h>
#include <sys/socket.h>
#include <uv.h>
#include "common/wrapper/client_info.h"
#include "common/list.h"

list client_list_init(void);
void client_list_add_client(client_info* client, list* client_list);
void client_list_strcpy(common_client* dst, list* client_list);
void client_list_remove_client(client_info* client, list* client_list);
client_info* client_list_get_client_info(uv_tcp_t* client, list* client_list);
