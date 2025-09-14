#include "c2d_packets.pb.h"
#include "common/wrapper/client_info.h"
#include <uv.h>

void send_client_list_request(uv_tcp_t* server);
void send_greet(uv_tcp_t* server, client_id client_id);
