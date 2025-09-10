#include "c2d_packets.pb.h"
#include <uv.h>

void send_client_list_request(uv_tcp_t* server);
void send_greet(uv_tcp_t* server, char* destination_name, uint32_t source_port);
