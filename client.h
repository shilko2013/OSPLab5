//
// Created by shilk on 11/17/2019.
//

#ifndef LAB5_CLIENT_H
#define LAB5_CLIENT_H

#include "info.h"
#include "server.h"

void serve_client(info_t *info, int timeout);
void serve_queue_client(info_t *info, int timeout);
int connect_with_shm(info_t** info);
int connect_client_with_queue();

#endif //LAB5_CLIENT_H
