#include "info.h"
#include "server.h"

info_t* info;

int main() {
    info = malloc(sizeof(info_t));
    if (!connect_server_socket())
        return 1;
    init_server(info);
    serve_socket_server(info);
    free(info);
    return 0;
}
