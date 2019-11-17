//
// Created by shilk on 11/17/2019.
//

#ifndef LAB5_SERVER_H
#define LAB5_SERVER_H

void init_server(info_t* info);
void serve_server(info_t* info, int timeout);
int connect_with_shm(info_t** info);
int connect_server_with_queue();
void serve_queue_server(info_t* info, int timeout);

#endif //LAB5_SERVER_H
