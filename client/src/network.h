#include "c2d_packets.pb.h"
#include <uv.h>

void send_greet(uv_tcp_t* server, char* destination_name, uint32_t source_port);
void list_clients(uv_tcp_t* server);
