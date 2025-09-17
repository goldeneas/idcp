#include "client_context.h"
#include "settings.h"
#include "callback.h"
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <uv.h>

int main(void) {

    uv_loop_t* loop = uv_default_loop();
    client_context client_context = client_context_init(loop);
    loop->data = &client_context;

    uv_tty_t tty;
    uv_tty_init(loop, &tty, 0, 1);
    uv_read_start((uv_stream_t*) &tty, alloc_buffer_cb, read_tty_buffer_cb);

    uv_run(loop, UV_RUN_DEFAULT);
    uv_loop_close(loop);
    return 0;
}
