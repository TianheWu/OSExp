#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <pthread.h>

void *my_thread1() {
    printf("Created myThread1 successfully !\n");
    pthread_exit(NULL);
}

void *my_thread2() {
    printf("Created myThread2 successfully !\n");
    pthread_exit(NULL);
}

int main() {
    pthread_t id_1, id_2;
    int ret;
    ret = pthread_create(&id_1, NULL, (void* ) my_thread1, NULL);
    if (ret != 0) {
        printf("Failed to create thread !\n");
        exit(1);
    }
    ret = pthread_create(&id_2, NULL, (void* ) my_thread2, NULL);
    if (ret != 0) {
        printf("Failed to create thread !\n");
        exit(1);
    }
    pthread_join(id_1, NULL);
    pthread_join(id_2, NULL);
    return 0;


}