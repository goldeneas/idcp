#pragma once
#include <stdbool.h>
#include <uv.h>

typedef struct client_context_s {
    bool connected;
    char* name;
    uv_loop_t* loop;

    uv_tcp_t* handle;
    uv_connect_t* req;
} client_context;

client_context client_context_init(uv_loop_t* loop);
void client_context_set_name(char* name, client_context* context);
