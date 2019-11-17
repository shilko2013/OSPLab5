#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define WAIT_TIME 1000000

pthread_t thread1;
pthread_t thread2;

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

const int DATA_LENGTH = 26;
char *data = "abcdefghijklmnopqrstuvwxyz";

void *change_case(void *args) {
    while (1) {
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < DATA_LENGTH; ++i)
            if (data[i] > 0x60)
                data[i] -= 0x20;
            else
                data[i] += 0x20;
        pthread_mutex_unlock(&mutex);
        usleep(WAIT_TIME);
    }
}

void *revert(void *args) {
    while (1) {
        pthread_mutex_lock(&mutex);
        for (int i = 0; i < DATA_LENGTH / 2; ++i) {
            char temp = data[i];
            data[i] = data[DATA_LENGTH - i - 1];
            data[DATA_LENGTH - i - 1] = temp;
        }
        pthread_mutex_unlock(&mutex);
        usleep(WAIT_TIME);
    }
}

int init() {
    pthread_mutex_init(&mutex, NULL);
    if (pthread_create(&thread1, NULL, change_case, NULL) == -1) {
        perror("error in pthread_create");
        return 0;
    }
    //pthread_join(thread1, NULL);
    if (pthread_create(&thread2, NULL, revert, NULL) == -1) {
        perror("error in pthread_create");
        return 0;
    }
    //pthread_join(thread2, NULL);
    return 1;
}

void print_data() {
    pthread_mutex_lock(&mutex);
    printf("Data: ");
    for (int i = 0; i < DATA_LENGTH; ++i) {
        printf("%c ", data[i]);
    }
    printf("\n");
    fflush(stdout);
    pthread_mutex_unlock(&mutex);
}

void serve() {
    init();
    while (1) {
        print_data();
        usleep(WAIT_TIME);
    }
}

int main() {
    serve();
    return 0;
}