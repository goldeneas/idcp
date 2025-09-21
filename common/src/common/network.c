#include "network.h"
#include "assert_wrapper.h"
#include "c2d_packets.pb.h"
#include "log.h"
#include "pb_encode.h"
#include <bits/sockaddr.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <uv.h>
#include <uv/unix.h>

#define SEND_ENVELOPE_BASE(envelope_struct, envelope, stream)                                   \
    ASSERT_NE(envelope.which_payload, 0);                                                       \
                                                                                                \
    size_t envelope_size;                                                                       \
    if (!pb_get_encoded_size(&envelope_size, envelope_struct##_fields, &envelope)) {            \
        log_error("Could not get encoded size for %s envelope!", #envelope_struct);             \
        return;                                                                                 \
    }                                                                                           \
                                                                                                \
    write_req* wr = alloc_write_request(envelope_size);                                         \
                                                                                                \
    pb_ostream_t pb_stream = pb_ostream_from_buffer((pb_byte_t*) wr->buf.base, wr->buf.len);    \
    bool status = pb_encode(&pb_stream, envelope_struct##_fields, &envelope);                   \
                                                                                                \
    if (!status) {                                                                              \
        log_error("Something went wrong when encoding motd: %s", PB_GET_ERROR(&pb_stream));     \
        return;                                                                                 \
    }                                                                                           \

write_req* alloc_write_request(size_t len) {
    write_req* wr = malloc(sizeof(write_req));
    wr->buf = uv_buf_init(malloc(len), len); 

    return wr;
}

struct sockaddr_storage socket_get_sockaddr_storage(char* address, in_port_t port,
        sa_family_t family) {
    socklen_t addrlen;
    struct sockaddr_storage storage;

    if (family == AF_INET) {
        struct sockaddr_in* dst_in4 = (struct sockaddr_in *) &storage;

        addrlen = sizeof(*dst_in4);
        memset(dst_in4, 0, addrlen);
        dst_in4->sin_family = family;
        dst_in4->sin_port = htons(port);
        inet_pton(family, address, &dst_in4->sin_addr);
    } else {
        struct sockaddr_in6* dst_in6 = (struct sockaddr_in6 *) &storage;

        addrlen = sizeof(*dst_in6);
        memset(dst_in6, 0, addrlen);
        dst_in6->sin6_family = family;
        dst_in6->sin6_port = htons(port);
        // unnecessary because of the memset(): dst_in6->sin6_flowinfo = 0;
        inet_pton(family, address, &dst_in6->sin6_addr);
    }

    return storage;
}

struct sockaddr_storage socket_get_sockaddr_storage_ex(uv_tcp_t* client) {
    struct sockaddr_storage peer_address;
    int name_len = sizeof(struct sockaddr_storage);
    uv_tcp_getpeername(client, (struct sockaddr*) &peer_address, &name_len);

    return peer_address;
}

void socket_extract_info(struct sockaddr_storage* sockaddr, in_port_t* port, char* address,
        sa_family_t* family, int len) {
    if (sockaddr != NULL) {
        if (sockaddr->ss_family == AF_INET6) {
            struct sockaddr_in6* addr_in = (struct sockaddr_in6*) sockaddr; 
            uv_ip6_name(addr_in, address, len);
        } else {
            struct sockaddr_in* addr_in = (struct sockaddr_in*) sockaddr;
            uv_ip4_name(addr_in, address, len);
        }
    }

    if (port != NULL) {
        if (sockaddr->ss_family == AF_INET6) {
            struct sockaddr_in6* addr_in = (struct sockaddr_in6*) sockaddr; 
            *port = ntohs(addr_in->sin6_port);
        } else {
            struct sockaddr_in* addr_in = (struct sockaddr_in*) sockaddr;
            *port = ntohs(addr_in->sin_port);
        }
    }

    if (family != NULL) {
        *family = sockaddr->ss_family;
    }
}

void send_c2d_envelope(c2d_envelope envelope, uv_tcp_t* handle, uv_write_cb cb) {
    uv_stream_t* stream = (uv_stream_t*) handle;

    SEND_ENVELOPE_BASE(c2d_envelope, envelope, stream);
    uv_write(&wr->wr.tcp, stream, &wr->buf, 1, cb);
}

void send_d2c_envelope(d2c_envelope envelope, uv_tcp_t* handle, uv_write_cb cb) {
    uv_stream_t* stream = (uv_stream_t*) handle;

    SEND_ENVELOPE_BASE(d2c_envelope, envelope, stream);
    uv_write(&wr->wr.tcp, stream, &wr->buf, 1, cb);
}

void send_c2c_envelope(c2c_envelope envelope, struct sockaddr* sockaddr, uv_udp_t* beacon,
        uv_udp_send_cb cb) {
    SEND_ENVELOPE_BASE(c2c_envelope, envelope, sockaddr);
    uv_udp_send(&wr->wr.udp, beacon, &wr->buf, 1, sockaddr, cb);
}
