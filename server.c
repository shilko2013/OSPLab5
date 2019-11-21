//
// Created by shilk on 11/17/2019.
//
#include <stdio.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/msg.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <signal.h>
#include <string.h>
#include <sys/un.h>
#ifdef __sun //for solarisOS
#include <sys/loadavg.h>
#endif
#include "info.h"

time_t start_t;
int shm_id, queue_id, fd;
int listenfd = -1, connectfd = -1;
char *SOCK_PATH = 0;

info_t* info_sig;

void init_start_time() {
    start_t = time(NULL);
}

void init_server(info_t *info) {
    init_start_time();
    info->pid = getpid();
    info->uid = getuid();
    info->gid = getgid();
    info->time = 0;
    getloadavg(info->system_load, 3);
}

void update(info_t *info) {
    info->time = time(NULL) - start_t;
    getloadavg(info->system_load, 3);
}

void serve_server(info_t *info, int timeout) {
    do {
        update(info);
        if (timeout < 0)
            return;
        sleep(timeout);
    } while (timeout >= 0);
}

int connect_with_shm(info_t **info) {
    if ((shm_id = shmget(SHARED_KEY, sizeof(info_t), IPC_CREAT | MODE)) == -1) {
        perror("error with shmget");
        return 0;
    }
    if ((*info = shmat(shm_id, NULL, 0)) == NULL) {
        perror("error with shmat");
        return 0;
    }
    return 1;
}

int connect_server_with_queue() {
    if ((queue_id = msgget(SHARED_KEY, IPC_CREAT | MODE)) == -1) {
        perror("error with msgget");
        return 0;
    }
    return 1;
}

int serve_queue_server_wait_msg() {
    queue_util_msg* q_m = malloc(sizeof(queue_util_msg));
    if (msgrcv(queue_id, q_m, sizeof(queue_util_msg), MSG_UTIL, 0) == -1) {
        free(q_m);
        return 0;
    }
    if (q_m->msg != 'y') {
        free(q_m);
        return 0;
    }
    free(q_m);
    return 1;
}

int server_send_msg(info_t *info) {
    queue_info_msg* q_m = malloc(sizeof(queue_info_msg));
    memcpy(&q_m->msg, info, sizeof(info_t));
    q_m->msg_type = MSG_INFO;
    if (msgsnd(queue_id, q_m, sizeof(queue_info_msg), 0) == -1) {
        perror("error with msgsnd");
        return 0;
    }
    return 1;
}

void serve_queue_server(info_t *info, int timeout) {
    do {
        update(info);
        if (serve_queue_server_wait_msg())
            if (!server_send_msg(info))
                return;
        if (timeout < 0)
            return;
        sleep(timeout);
    } while (timeout >= 0);
}

int connect_server_with_mmap_file(info_t **info) {
    if ((fd = open(MMAP_FILE, O_RDWR | O_CREAT, MODE)) < 0) {
        perror("error in open");
        return 0;
    }
    if (truncate(MMAP_FILE, sizeof(info_t)) < 0) {
        perror("error in truncate");
        return 0;
    }
    if ((*info = (info_t *) mmap(0, sizeof(info_t), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0)) == MAP_FAILED) {
        perror("error in mmap");
        return 0;
    }
    return 1;
}

void handle_socket_signal(int signal) {
    if (listenfd >= 0)
        close(listenfd);
    if (connectfd >= 0)
        close(connectfd);
}

int connect_server_socket() {
    SOCK_PATH = getenv("HOME");
    strcat(SOCK_PATH, _SOCK_PATH);
    signal(SIGABRT, handle_socket_signal);
    signal(SIGTERM, handle_socket_signal);
    signal(SIGKILL, handle_socket_signal);
    signal(SIGINT, handle_socket_signal);
    struct sockaddr_un serv_addr = {
            .sun_family= AF_UNIX
    };
    strcpy(serv_addr.sun_path, SOCK_PATH);
    unlink(SOCK_PATH);
    listenfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (listenfd < 0) {
        perror("error in socket:");
        return 0;
    }
    if (bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
        perror("error in bind:");
        close(listenfd);
        return 0;
    }
    listen(listenfd, 10);
    return 1;
}

void serve_socket_server(info_t *info) {
    while (1) {
        connectfd = accept(listenfd, (struct sockaddr *) NULL, NULL);
        update(info);
        write(connectfd, info, sizeof(info_t));
        close(connectfd);
        connectfd = -1;
    }
    close(listenfd);
}

void print_pid(int sig) { printf("PID: %lu\n", info_sig->pid); }

void print_puid(int sig) { printf("PUID: %lu\n", info_sig->uid); }

void print_pgid(int sig) { printf("PGID: %lu\n", info_sig->gid); }

void print_time(int sig) { printf("TIME: %lu\n", info_sig->time); }

void print_load(int sig) {
    printf("Average load system time for 1 minute: %.3lf,\n\t 5 minutes: %.3lf,\n\t 15 minutes: %.3lf\n",
           info_sig->system_load[0], info_sig->system_load[1], info_sig->system_load[2]);
}

void proc_signals(info_t *info) {
    info_sig = info;
    signal(SIGHUP, print_pid);
    signal(SIGINT, print_puid);
    signal(SIGTERM, print_pgid);
    signal(SIGUSR1, print_time);
    signal(SIGUSR2, print_load);
}
