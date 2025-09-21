#pragma once
#include "c2c_packets.pb.h"
#include "client_context.h"
#include "d2c_packets.pb.h"
#include <stdint.h>
#include <uv.h>

void handle_c2c_packet(c2c_envelope* envelope, const struct sockaddr* sockaddr,
        client_context* context);
void handle_d2c_packet(d2c_envelope* envelope, uv_stream_t* stream, client_context* context);
