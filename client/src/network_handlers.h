#pragma once
#include "d2c_packets.pb.h"
#include <stdint.h>
#include <uv.h>

void handle_d2c_packet(d2c_envelope* envelope, uv_stream_t* stream);

void on_motd(motd_packet* packet, uv_stream_t* stream);
void on_client_list(client_list_packet* packet, uv_stream_t* stream);
