#include "c2d_packets.pb.h"
#include <uv.h>

void send_greet(uv_tcp_t* client, greet_packet packet);
