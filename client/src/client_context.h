#pragma once
#include <stdbool.h>

typedef struct client_context_s {
    bool connected;
} client_context;

client_context client_context_init(void);
