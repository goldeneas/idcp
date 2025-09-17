#pragma once
#include "common/list.h"
#include "common/wrapper/client_info.h"
#include "wrapper/client_list.h"
#include <stdint.h>
#include <uv.h>

typedef struct server_context_s {
    list client_list;
    list greet_list;
} server_context;

server_context server_context_init(void);
client_info* server_context_get_client_info(uv_tcp_t* client, server_context* context);
