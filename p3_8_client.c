#include "info.h"
#include "client.h"

info_t* info;

int main() {
    info = malloc(sizeof(info_t));
    if (!connect_client_socket())
        return 1;
    serve_socket_client(info, -1);
    free(info);
    return 0;
}
