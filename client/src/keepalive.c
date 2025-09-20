#include "keepalive.h"
#include "client_context.h"
#include "network.h"
#include "common/list.h"
#include "common/log.h"
#include "peer_list.h"

void keepalive_iter(uv_timer_t* handle) {
    client_context* context = handle->loop->data;
    list* peer_list = &context->peer_list;

    for (size_t i = 0; i < peer_list->length; i++) {
        peer_entry* entry = list_get(i, peer_list);
        send_keepalive(addr, &context->beacon);
    }
}
