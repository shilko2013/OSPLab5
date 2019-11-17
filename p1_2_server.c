#include "info.h"
#include "server.h"

info_t* info;

int main() {
    if (!connect_server_with_queue())
        return 1;
    init_server(info);
    serve_queue_server(info, 1);
    return 0;
}