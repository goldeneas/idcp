#include <uv.h>

void connect_cb(uv_connect_t*, int);
void alloc_buffer_cb(uv_handle_t*, size_t, uv_buf_t*);
void read_c2d_buffer_cb(uv_stream_t*, ssize_t, const uv_buf_t*);
void read_tty_buffer_cb(uv_stream_t*, ssize_t, const uv_buf_t*);
void after_write_cb(uv_write_t* wr, int status);
void close_cb(uv_handle_t* handle);
void read_beacon_buffer_cb(uv_udp_t* handle, ssize_t nread, const uv_buf_t* buf,
        const struct sockaddr* addr, unsigned flags);
