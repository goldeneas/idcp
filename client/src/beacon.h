#pragma once
#include "client_context.h"
#include <uv.h>

void beacon_start(int port, uv_loop_t* loop, client_context* context);
