//
// Created by shilk on 11/17/2019.
//

#include <stdio.h>
#include <sys/msg.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <netinet/in.h>
#include "info.h"

int queue_id, fd;
int sockfd;

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

int connect_client_with_mmap_file(info_t** info) {
    if ((fd = open(MMAP_FILE, O_RDWR | O_CREAT, MODE)) < 0) {
        perror("error in open");
        return 0;
    }
    if ((*info = (info_t *) mmap(0, sizeof(info_t), PROT_READ, MAP_SHARED, fd,0)) == MAP_FAILED){
        perror("error in mmap");
        return 0;
    }
    return 1;
}

int connect_client_socket() {
    if((sockfd = socket(AF_UNIX, SOCK_STREAM, 0)) < 0) {
        perror("error in socket:");
        return 0;
    }
    return 1;
}

int get_socket_info(info_t* info) {
    struct sockaddr_in serv_addr = {
            .sin_family = AF_UNIX,
            .sin_port = htons(PORT)
    };
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("error in connect:");
        return 0;
    }
    if ( read(sockfd, info, sizeof(info_t)) <= 0) {
        perror("error in read:");
        return 0;
    }
    return 1;
}

void serve_socket_client(info_t* info, int timeout) {
    do {
        if (!get_socket_info(info))
            return;
        print_info(info);
        if (timeout < 0)
            return;
        sleep(timeout);
    } while (timeout >= 0);
    close(sockfd);
}