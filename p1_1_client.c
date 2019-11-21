#include "info.h"
#include "client.h"

info_t* info;

int main() {
    if (!connect_with_shm(&info))
        return 1;
    serve_client(info, -1);
    return 0;
}
