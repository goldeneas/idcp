#pragma once
#include "c2c_packets.pb.h"
#include "c2d_packets.pb.h"
#include "d2c_packets.pb.h"
#include <netinet/in.h>
#include <stddef.h>
#include <sys/socket.h>
#include <uv.h>

typedef struct _write_req_t {
    uv_write_t req;
    uv_buf_t buf;
} write_req_t;

write_req_t* alloc_write_request(size_t len);
void destroy_write_request(write_req_t* req);

void get_socket_addr(uv_tcp_t* client, char* address, int len);
in_port_t get_socket_port(uv_tcp_t* client);
struct sockaddr_storage get_sockaddr(uv_tcp_t* client);

void send_c2d_envelope(c2d_envelope envelope, uv_stream_t* stream, uv_write_cb cb);
void send_d2c_envelope(d2c_envelope envelope, uv_stream_t* stream, uv_write_cb cb);
void send_c2c_envelope(c2c_envelope envelope, uv_stream_t* stream, uv_write_cb cb);
