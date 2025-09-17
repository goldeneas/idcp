#pragma once
#include <uv.h>
#include "client_context.h"

void handle_tty_command(const uv_buf_t* buf, client_context* context);
void handle_tty_input(const uv_buf_t* buf, client_context* context);
