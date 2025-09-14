#pragma once
#include <stdbool.h>
#include <uv.h>

typedef struct client_context_s {
    bool connected;
    uv_tcp_t* server;
} client_context;

client_context client_context_init(void);
