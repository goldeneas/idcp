#pragma once
#include <string.h>

#define HANDLE_COMMAND_BRANCH(buf, command_name, handler_function, context)                     \
    if (strcmp(buf, command_name) == 0) {                                                       \
        handler_function(buf, context);                                                         \
        return;                                                                                 \
    }                                                                                           \
