#pragma once
#include <netinet/in.h>
#include <stddef.h>
#include <sys/socket.h>
#include <uv.h>

typedef struct _write_req_t {
    uv_write_t* wr;
    uv_buf_t buf;
} write_req_t;

write_req_t alloc_write_request(char* base, size_t len);
void destroy_write_request(write_req_t* req);

void get_client_address(uv_tcp_t* client, char* address, int len);
in_port_t get_client_port(uv_tcp_t* client);
struct sockaddr_storage get_client_sockaddr(uv_tcp_t* client);
