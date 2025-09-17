#pragma once
#include <stdbool.h>
#include <sys/socket.h>
#include <uv.h>

typedef struct client_context_s {
    bool connected_discovery;

    bool connected_client;
    struct sockaddr client_addr;

    uv_udp_t beacon;
    uv_tcp_t handle;
    uv_connect_t req;

    uv_loop_t* loop;

    char* username;
} client_context;

client_context client_context_init(uv_loop_t* loop);
void client_context_set_username(char* username, client_context* context);
