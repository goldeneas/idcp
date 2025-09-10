#include "network.h"
#include "assert_wrapper.h"
#include "c2d_packets.pb.h"
#include "log.h"
#include "pb_encode.h"
#include <netinet/in.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <uv.h>
#include <uv/unix.h>

#define SEND_ENVELOPE_BASE(envelope_struct, envelope, stream, write_cb)                             \
    do {                                                                                            \
        ASSERT_NE(envelope.which_payload, 0);                                                       \
                                                                                                    \
        size_t envelope_size;                                                                       \
        if (!pb_get_encoded_size(&envelope_size, envelope_struct##_fields, &envelope)) {            \
            log_error("Could not get encoded size for %s envelope!", #envelope_struct);             \
            return;                                                                                 \
        }                                                                                           \
                                                                                                    \
        write_req* wr = alloc_write_request(envelope_size);                                       \
                                                                                                    \
        pb_ostream_t pb_stream = pb_ostream_from_buffer((pb_byte_t*) wr->buf.base, wr->buf.len);    \
        bool status = pb_encode(&pb_stream, envelope_struct##_fields, &envelope);                   \
                                                                                                    \
        if (!status) {                                                                              \
            log_error("Something went wrong when encoding motd: %s", PB_GET_ERROR(&pb_stream));     \
            return;                                                                                 \
        }                                                                                           \
                                                                                                    \
        uv_write(&wr->req, stream, &wr->buf, 1, write_cb);                                          \
    } while(0)                                                                                      \

write_req* alloc_write_request(size_t base_size) {
    write_req* wr = malloc(sizeof(write_req));

    wr->buf = uv_buf_init(malloc(base_size), base_size); 

    return wr;
}

in_port_t get_socket_port(uv_tcp_t* client) {
    in_port_t port;
    struct sockaddr_storage peer_address = get_sockaddr(client);

    if (peer_address.ss_family == AF_INET6) {
        struct sockaddr_in6* addr_in = (struct sockaddr_in6*) &peer_address; 
        port = addr_in->sin6_port;
    } else {
        struct sockaddr_in* addr_in = (struct sockaddr_in*) &peer_address;
        port = addr_in->sin_port;
    }

    return ntohs(port);
}

void get_socket_addr(uv_tcp_t* client, char* address, int len) {
    struct sockaddr_storage peer_address = get_sockaddr(client);

    if (peer_address.ss_family == AF_INET6) {
        struct sockaddr_in6* addr_in = (struct sockaddr_in6*) &peer_address; 
        uv_ip6_name(addr_in, address, len);
    } else {
        struct sockaddr_in* addr_in = (struct sockaddr_in*) &peer_address;
        uv_ip4_name(addr_in, address, len);
    }
}

struct sockaddr_storage get_sockaddr(uv_tcp_t* client) {
    struct sockaddr_storage peer_address;
    int name_len = sizeof(struct sockaddr_storage);
    uv_tcp_getpeername(client, (struct sockaddr*) &peer_address, &name_len);

    return peer_address;
}

void send_c2d_envelope(c2d_envelope envelope, uv_stream_t* stream, uv_write_cb cb) {
    SEND_ENVELOPE_BASE(c2d_envelope, envelope, stream, cb);
}

void send_d2c_envelope(d2c_envelope envelope, uv_stream_t* stream, uv_write_cb cb) {
    SEND_ENVELOPE_BASE(d2c_envelope, envelope, stream, cb);
}

void send_c2c_envelope(c2c_envelope envelope, uv_stream_t* stream, uv_write_cb cb) {
    SEND_ENVELOPE_BASE(c2c_envelope, envelope, stream, cb);
}
