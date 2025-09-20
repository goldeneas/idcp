#include "server_context.h"
#include "common/list.h"
#include "common/log.h"
#include "common/network.h"
#include "common/wrapper/client_info.h"
#include "wrapper/address_list.h"
#include "wrapper/client_list.h"
#include "wrapper/greet_list.h"
#include <netinet/in.h>
#include <stdatomic.h>
#include <stddef.h>
#include <string.h>
#include <sys/socket.h>
#include <uv.h>

server_context server_context_init(void) {
    server_context ctx;
    ctx.client_list = client_list_init();
    ctx.greet_list = greet_list_init();
    ctx.address_list = address_list_init();

    return ctx;
}

void server_context_add_client(client_info* info, uv_tcp_t* client, server_context* context) {
    client_list_add_client(info, &context->client_list);
    address_list_add_address(info->id, client, &context->address_list);
}
