#pragma once
#include "client_context.h"
#include <stdbool.h>
#include <sys/socket.h>
#include <uv.h>

bool client_connect(char* address, int port, uv_loop_t* loop, client_context* context);
void client_disconnect(client_context* context);
void client_set_name(char* name, client_context* context);
