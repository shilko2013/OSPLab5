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

typedef struct queue_util_message {
    long msg_type;
    char msg;
} queue_util_msg;

typedef struct queue_info_message {
    long msg_type;
    info_t msg;
} queue_info_msg;

#ifndef SHARED_KEY
#define SHARED_KEY 8152
#endif //SHARED_KEY

#ifndef MODE
#define MODE 0640
#endif //MODE

#ifndef MMAP_FILE
#define MMAP_FILE "mmap_file"
#endif //MMAP_FILE

#ifndef _SOCK_PATH
#define _SOCK_PATH "/socket/socket_lab5"
#endif //SOCK_PATH

#ifndef MSG_INFO
#define MSG_INFO 2
#endif //MSG_INFO

#ifndef MSG_UTIL
#define MSG_UTIL 3
#endif //MSG_UTIL

#endif //LAB5_INFO_H
