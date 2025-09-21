#include "keepalive.h"
#include "client_context.h"
#include "common/network.h"
#include "network.h"
#include "common/list.h"
#include "common/log.h"
#include "peer_list.h"
#include <sys/socket.h>

void keepalive_iter(uv_timer_t* handle) {
    client_context* context = handle->loop->data;
    list* peer_list = &context->peer_list;

    for (size_t i = 0; i < peer_list->length; i++) {
        peer_entry* entry = list_get(i, peer_list);
        struct sockaddr_storage storage = socket_get_sockaddr_storage(entry->address, entry->port,
                entry->sa_family);
        send_keepalive((struct sockaddr*) &storage, &context->beacon);
    }
}
