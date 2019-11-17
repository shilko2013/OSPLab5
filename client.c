//
// Created by shilk on 11/17/2019.
//

#include <stdio.h>
#include <sys/msg.h>
#include "info.h"

int queue_id;

void print_info(info_t *info) {
    printf("PID: %li, GID: %li, UID: %li\n", info->pid, info->gid, info->uid);
    printf("Server working %li seconds\n ", info->time);
    printf("Average load system time for 1 minute: %.3lf,\n\t 5 minutes: %.3lf,\n\t 15 minutes: %.3lf\n",
           info->system_load[0],
           info->system_load[1],
           info->system_load[2]);
}

void serve_client(info_t *info, int timeout) {
    do {
        print_info(info);
        if (timeout < 0)
            return;
        sleep(timeout);
    } while (timeout >= 0);
}

int connect_client_with_queue() {
    if ((queue_id = msgget(SHARED_KEY, IPC_CREAT | MODE)) == -1) {
        perror("error with msgget");
        return 0;
    }
    return 1;
}

int client_get_msg(info_t *info) {
    if (msgrcv(queue_id, info, sizeof(info), 1, 0) == -1)
        return 0;
    return 1;
}

int client_send_msg(char msg) {
    if (msgsnd(queue_id, &msg, sizeof(char), 0) == -1) {
        perror("error with msgsnd");
        return 0;
    }
    return 1;
}

int get_queue_info(info_t *info) {
    if (client_send_msg('y'))
        return 0;
    if (client_get_msg(info))
        return 0;
    return 1;
}

void serve_queue_client(info_t *info, int timeout) {
    do {
        if (!get_queue_info(info))
            return;
        print_info(info);
        if (timeout < 0)
            return;
        sleep(timeout);
    } while (timeout >= 0);
}