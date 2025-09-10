#pragma once
#include "common.h"
#include "d2c_packets.pb.h"
#include "pb.h"
#include <stdint.h>
#include <uv.h>

void send_motd(uv_tcp_t* client, char* server_name, char* motd);
void send_client_list(uv_tcp_t* client, char** clients, pb_size_t clients_count);
