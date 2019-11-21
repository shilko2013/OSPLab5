#include "info.h"
#include "server.h"

info_t* info;

int main() {
    if (!connect_with_shm(&info))
        return 1;
    init_server(info);
    proc_signals(info);
    serve_server(info, 1);
    return 0;
}
