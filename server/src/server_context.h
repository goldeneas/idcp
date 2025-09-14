#pragma once
#include "common/list.h"
#include "wrapper/client_list.h"

typedef struct server_context_s {
    client_list client_list;
    list greet_map;
} server_context;

server_context server_context_init(void);
