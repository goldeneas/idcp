#pragma once
#include "pb.h"
#include "wrapper/client_list.h"
#include <stdint.h>
#include <uv.h>

void send_motd(uv_tcp_t* client, char* server_name, char* motd);
void send_client_list(uv_tcp_t* client, client_list* list);
