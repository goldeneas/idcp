#pragma once
#include "../settings.h"
#include "c2c_packets.pb.h"
#include "common/linked_list.h"
#include "common/wrapper/client_info.h"
#include <stddef.h>

typedef struct greet_entry_s {
    client_id client;
    linked_list list;
} greet_entry;
