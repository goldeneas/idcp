#pragma once
#include "c2d_packets.pb.h"
#include "server_context.h"
#include <uv.h>

void handle_c2d_packet(c2d_envelope* envelope, uv_stream_t* stream, server_context* context);
