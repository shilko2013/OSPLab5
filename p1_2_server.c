#include "info.h"
#include "server.h"


int main() {
    info_t* info = malloc(sizeof(info_t));
    if (!connect_server_with_queue())
        return 1;
    init_server(info);
    serve_queue_server(info, 1);
    free(info);
    return 0;
}
