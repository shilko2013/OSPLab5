#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define WAIT_TIME 1000000

pthread_t thread1;
pthread_t thread2;
pthread_t thread3;

pthread_rwlock_t rwlock = PTHREAD_RWLOCK_INITIALIZER;

const int DATA_LENGTH = 26;
char data[] = "abcdefghijklmnopqrstuvwxyz";

void *change_case(void *args) {
    while (1) {
        pthread_rwlock_wrlock(&rwlock);
        int i;
        for (i = 0; i < DATA_LENGTH; ++i)
            if (data[i] > 0x60)
                data[i] -= 0x20;
            else
                data[i] += 0x20;
        pthread_rwlock_unlock(&rwlock);
        usleep(WAIT_TIME);
    }
}

void *revert(void *args) {
    while (1) {
        pthread_rwlock_wrlock(&rwlock);
        int i;
        for (i = 0; i < DATA_LENGTH / 2; ++i) {
            char temp = data[i];
            data[i] = data[DATA_LENGTH - i - 1];
            data[DATA_LENGTH - i - 1] = temp;
        }
        pthread_rwlock_unlock(&rwlock);
        usleep(WAIT_TIME);
    }
}

void *print_big_letters_count(void *args) {
    while (1) {
        pthread_rwlock_rdlock(&rwlock);
        int count = 0;
        int i;
        for (i = 0; i < DATA_LENGTH; ++i) {
            if (data[i] > 0x60)
                ++count;
        }
        printf("Count of big letters: %d\n", count);
        pthread_rwlock_unlock(&rwlock);
        usleep(WAIT_TIME);
    }
}

int init() {
    pthread_rwlock_init(&rwlock, NULL);
    if (pthread_create(&thread1, NULL, change_case, NULL) == -1) {
        perror("error in pthread_create");
        return 0;
    }
    //pthread_join(thread1, NULL);
    if (pthread_create(&thread2, NULL, revert, NULL) == -1) {
        perror("error in pthread_create");
        return 0;
    }
    if (pthread_create(&thread3, NULL, print_big_letters_count, NULL) == -1) {
        perror("error in pthread_create");
        return 0;
    }
    //pthread_join(thread2, NULL);
    return 1;
}

void print_data() {
    pthread_rwlock_wrlock(&rwlock);
    printf("Data: ");
    int i;
    for (i = 0; i < DATA_LENGTH; ++i) {
        printf("%c ", data[i]);
    }
    printf("\n");
    fflush(stdout);
    pthread_rwlock_unlock(&rwlock);
}

void serve() {
    while (1) {
        print_data();
        usleep(WAIT_TIME);
    }
}

int main() {
    init();
    serve();
    return 0;
}
