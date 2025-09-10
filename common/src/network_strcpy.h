#pragma once
#include "log.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <pb.h>

#define STRCPY_IMPL(packet_field_name, max_size)                            \
    static inline void strcpy_##packet_field_name(char* dst, char* src) {   \
        strncpy(dst, src, max_size);                                        \
        dst[max_size - 1] = '\0';                                           \
    }                                                                       \

#define MTRX_STRCPY_IMPL(packet_field_name, max_count, max_size)                                    \
    static inline void mtrx_strcpy_##packet_field_name(char dst[][max_size], char src[][max_size],  \
            pb_size_t count) {                                                                      \
        if (count > max_count) {                                                                    \
            log_warn("Exceeded max_count (%i) for %s: count is %i",                                 \
                    max_count, #packet_field_name, count);                                          \
            return;                                                                                 \
        }                                                                                           \
                                                                                                    \
        for (uint32_t i = 0; i < count; i++) {                                                      \
            log_info("i is %i", i);                                                                 \
            char* entry = src[i];                                                                   \
            if (entry == NULL) {                                                                    \
                log_warn("Found null entry in mtrx_strcpy! Count: %i", i);                          \
            }                                                                                       \
                                                                                                    \
            strncpy(dst[i], entry, max_size);                                                       \
            dst[i][max_size - 1] = '\0';                                                            \
        }                                                                                           \
    }                                                                                               \

STRCPY_IMPL(motd, 1024)
STRCPY_IMPL(server_name, 256)

MTRX_STRCPY_IMPL(clients, 1024, 16)
