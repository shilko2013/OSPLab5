//
// Created by shilk on 11/17/2019.
//

#ifndef LAB5_INFO_H
#define LAB5_INFO_H

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <time.h>

typedef struct info {
    pid_t pid;
    uid_t uid;
    gid_t gid;
    time_t time;
    double system_load[3];
} info_t;

#ifndef SHARED_KEY
#define SHARED_KEY 8152
#endif //SHARED_KEY

#ifndef MODE
#define MODE 0640
#endif //MODE

#ifndef MMAP_FILE
#define MMAP_FILE "mmap_file"
#endif //MMAP_FILE

#ifndef PORT
#define PORT 1901
#endif //PORT

#ifndef SOCK_PATH
#define SOCK_PATH "/tmp/socket_lab5"
#endif //SOCK_PATH

#endif //LAB5_INFO_H
