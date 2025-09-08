#include "network.h"
#include "pb_encode.h"
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <uv.h>
#include <uv/unix.h>

write_req_t alloc_write_request(char* base, size_t len) {
    uv_write_t* wr = malloc(sizeof(uv_write_t));
    uv_buf_t buf = uv_buf_init(base, len);

    return (write_req_t) {
        .buf = buf,
        .wr = wr,
    };
}

void destroy_write_request(write_req_t* req) {
    free(req->buf.base);
    free(req);
}

in_port_t get_client_port(uv_tcp_t* client) {
    in_port_t port;
    struct sockaddr_storage peer_address = get_client_sockaddr(client);

    if (peer_address.ss_family == AF_INET6) {
        struct sockaddr_in6* addr_in = (struct sockaddr_in6*) &peer_address; 
        port = addr_in->sin6_port;
    } else {
        struct sockaddr_in* addr_in = (struct sockaddr_in*) &peer_address;
        port = addr_in->sin_port;
    }

    return ntohs(port);
}

void get_client_address(uv_tcp_t* client, char* address, int len) {
    struct sockaddr_storage peer_address = get_client_sockaddr(client);

    if (peer_address.ss_family == AF_INET6) {
        struct sockaddr_in6* addr_in = (struct sockaddr_in6*) &peer_address; 
        uv_ip6_name(addr_in, address, len);
    } else {
        struct sockaddr_in* addr_in = (struct sockaddr_in*) &peer_address;
        uv_ip4_name(addr_in, address, len);
    }
}

struct sockaddr_storage get_client_sockaddr(uv_tcp_t* client) {
    struct sockaddr_storage peer_address;
    int name_len = sizeof(struct sockaddr_storage);
    uv_tcp_getpeername(client, (struct sockaddr*) &peer_address, &name_len);

    return peer_address;
}
