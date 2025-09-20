#pragma once
#include <string.h>

#define HANDLE_COMMAND_BRANCH(base_name, command_name, handler_function, args, args_len, context)   \
    if (strcmp(base_name, command_name) == 0) {                                                     \
        handler_function(args, args_len, context);                                                  \
        return;                                                                                     \
    }
