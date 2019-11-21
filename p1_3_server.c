#include "info.h"
#include "server.h"

info_t* info;

int main() {
    if (!connect_server_with_mmap_file(&info))
        return 1;
    init_server(info);
    serve_server(info, 1);
    return 0;
}
