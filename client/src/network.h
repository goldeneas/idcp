#include "c2c_packets.pb.h"
#include "c2d_packets.pb.h"
#include "common/wrapper/client_info.h"
#include <sys/socket.h>
#include <uv.h>

void send_client_list_request(uv_tcp_t* handle);
void send_greet(uv_tcp_t* handle, client_id client_id);
void send_message(struct sockaddr* addr, char* text, uv_udp_t* beacon);
void send_keepalive(struct sockaddr* addr, uv_udp_t* beacon);
