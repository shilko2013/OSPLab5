#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/sem.h>
#include "info.h"

pthread_t thread1;
pthread_t thread2;

struct sembuf sem_inc = {
        .sem_num = 0,
        .sem_op = 1,
        .sem_flg = IPC_NOWAIT
};

struct sembuf sem_dec = {
        .sem_num = 0,
        .sem_op = -1,
        .sem_flg = IPC_NOWAIT
};

struct sembuf sem_check = {
        .sem_num = 0,
        .sem_op = 0,
        .sem_flg = IPC_NOWAIT
};

union semun {
    int val;
    struct semid_ds *buf;
    ushort * array;
} argument;

int sem_id;

const int DATA_LENGTH = 26;
char data[] = "abcdefghijklmnopqrstuvwxyz";

void take_sem() {
    while (semop(sem_id, &sem_check, 1));
    semop(sem_id, &sem_inc, 1);
}

void free_sem() {
    semop(sem_id, &sem_dec, 1);
}

void *change_case(void *args) {
    take_sem();
    int i;
    for (i = 0; i < DATA_LENGTH; ++i)
        if (data[i] > 0x60)
            data[i] -= 0x20;
        else
            data[i] += 0x20;
    free_sem();
    pthread_exit(0);
}

void *revert(void *args) {
    take_sem();
    int i;
    for (i = 0; i < DATA_LENGTH / 2; ++i) {
        char temp = data[i];
        data[i] = data[DATA_LENGTH - i - 1];
        data[DATA_LENGTH - i - 1] = temp;
    }
    free_sem();
    pthread_exit(0);
}

int init_random() {
    if (rand() % 2 == 0) {
        if (pthread_create(&thread1, NULL, change_case, NULL) == -1) {
            perror("error in pthread_create");
            return 0;
        }
        pthread_join(thread1, NULL);
    } else {
        if (pthread_create(&thread2, NULL, revert, NULL) == -1) {
            perror("error in pthread_create");
            return 0;
        }
        pthread_join(thread2, NULL);
    }
    return 1;
}

void print_data() {
    take_sem();
    printf("Data: ");
    int i;
    for (i = 0; i < DATA_LENGTH; ++i) {
        printf("%c ", data[i]);
    }
    printf("\n");
    fflush(stdout);
    free_sem();
}

void serve() {
    if ((sem_id = semget(SHARED_KEY, 1, IPC_CREAT | MODE)) == -1) {
        perror("error in semget:");
        exit(1);
    }
    argument.val = 0;
    if (semctl(sem_id, 0, SETVAL, argument) < 0) {
        perror("error in semctl:");
        exit(1);
    }
    while (1) {
        init_random();
        print_data();
        sleep(1);
    }
}

int main() {
    serve();
    return 0;
}
