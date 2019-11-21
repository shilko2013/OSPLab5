#include "info.h"
#include "client.h"

info_t* info;

int main() {
    if (!connect_client_with_mmap_file(&info))
        return 1;
    serve_client(info, -1);
    return 0;
}
