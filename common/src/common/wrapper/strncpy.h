#pragma once
#include "common.pb.h"
#include "d2c_packets.pb.h"
#include "c2d_packets.pb.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <pb.h>

// scary macro magic!!
// it's just a cool, safe strncpy

#define STRNCPY_IMPL(packet_field_name, packet_struct)                      \
    static inline void packet_field_name##_strncpy(char* dst, char* src) {  \
        size_t max_size = sizeof(((packet_struct*) 0)->packet_field_name);  \
        strncpy(dst, src, max_size);                                        \
        dst[max_size - 1] = '\0';                                           \
    }                                                                       \

STRNCPY_IMPL(motd, motd_packet)
STRNCPY_IMPL(server_name, motd_packet)
STRNCPY_IMPL(name, common_client)
