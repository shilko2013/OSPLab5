#include "info.h"
#include "client.h"

info_t* info;

int main() {
    if (!connect_client_with_queue())
        return 1;
    serve_queue_client(info, -1);
    return 0;
}
