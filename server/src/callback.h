#pragma once
#include <uv.h>

void after_write_cb(uv_write_t* req, int status);
void write_cb(uv_write_t* req, int status);
void connection_cb(uv_stream_t*, int);
void alloc_buffer_cb(uv_handle_t* handle, size_t suggested_size, uv_buf_t* buf);
void read_c2d_buffer_cb(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf);
