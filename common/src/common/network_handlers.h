#pragma once

#define HANDLE_PACKET_BRANCH(field_name, envelope, envelope_struct, handler_function, stream,   \
        context)                                                                                \
    if (envelope->which_payload == envelope_struct##_##field_name##_tag) {                      \
        handler_function(&envelope->payload.field_name, stream, context);                       \
        return;                                                                                 \
    }                                                                                           \
