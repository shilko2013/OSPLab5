#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

sem_t semaphore;

pthread_t thread1;
pthread_t thread2;

pthread_attr_t attr_t;

const int DATA_LENGTH = 26;
char data[] = "abcdefghijklmnopqrstuvwxyz";

void *change_case(void *args) {
    sem_wait(&semaphore);
    int i;
    for (i = 0; i < DATA_LENGTH; ++i)
        if (data[i] > 0x60)
            data[i] -= 0x20;
        else
            data[i] += + 0x20;
    sem_post(&semaphore);
    pthread_exit(0);
}

void *revert(void *args) {
    sem_wait(&semaphore);
    int i;
    for (i = 0; i < DATA_LENGTH / 2; ++i) {
        char temp = data[i];
        data[i] = data[DATA_LENGTH - i - 1];
        data[DATA_LENGTH - i - 1] = temp;
    }
    sem_post(&semaphore);
    pthread_exit(0);
}

int init_random() {
    pthread_attr_init(&attr_t);
    if (rand() % 2 == 0) {
        if (pthread_create(&thread1, &attr_t, change_case, NULL) == -1) {
            perror("error in pthread_create");
            return 0;
        }
        pthread_join(thread1, NULL);
    } else {
        if (pthread_create(&thread2, &attr_t, revert, NULL) == -1) {
            perror("error in pthread_create");
            return 0;
        }
        pthread_join(thread2, NULL);
    }
    return 1;
}

void print_data() {
    sem_wait(&semaphore);
    printf("Data: ");
    int i;
    for (i = 0; i < DATA_LENGTH; ++i) {
        printf("%c ", data[i]);
    }
    printf("\n");
    fflush(stdout);
    sem_post(&semaphore);
}

void serve() {
    sem_init(&semaphore, 0, 1);
    while (1) {
        init_random();
        print_data();
        sleep(1);
    }
}

void destroy() {
    sem_destroy(&semaphore);
}

int main() {
    serve();
    destroy();
    return 0;
}
