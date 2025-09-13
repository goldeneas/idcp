#pragma once
#include "wrapper/client_list.h"

typedef struct server_context_s {
    client_list client_list;
} server_context;

server_context server_context_init(void);
