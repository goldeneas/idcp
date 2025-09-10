#pragma once
#include "c2d_packets.pb.h"
#include <uv.h>

void handle_c2d_packet(c2d_envelope* envelope, uv_stream_t* stream);

void on_client_list_request(client_list_request* packet, uv_stream_t* stream);
