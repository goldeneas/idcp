#pragma once
#include "c2c_packets.pb.h"
#include "c2d_packets.pb.h"
#include "d2c_packets.pb.h"
#include <netinet/in.h>
#include <stddef.h>
#include <sys/socket.h>
#include <uv.h>

typedef struct write_req_s {
    union {
        uv_write_t tcp;
        uv_udp_send_t udp;
    } wr;

    uv_buf_t buf;
} write_req;

#define MAKE_ENVELOPE(tag_name, envelope_struct, packet)                            \
    envelope_struct envelope = envelope_struct##_init_zero;                         \
    envelope.which_payload = envelope_struct##_##tag_name##_tag;                    \
    envelope.payload.tag_name = packet;                                             \

write_req* alloc_write_request(size_t len);
void destroy_write_request(write_req* req);

void socket_extract_info(struct sockaddr_storage* sockaddr, in_port_t* port, char* address, int len);
struct sockaddr_storage socket_get_sockaddr_storage(uv_tcp_t* client);

void send_c2d_envelope(c2d_envelope envelope, uv_tcp_t* stream, uv_write_cb cb);
void send_d2c_envelope(d2c_envelope envelope, uv_tcp_t* stream, uv_write_cb cb);
void send_c2c_envelope(c2c_envelope envelope, struct sockaddr* sockaddr, uv_udp_t* beacon,
        uv_udp_send_cb cb);
