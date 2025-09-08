#pragma once
#include "common.h"
#include "d2c_packets.pb.h"

void send_motd(uv_tcp_t* client, motd_packet* motd);
