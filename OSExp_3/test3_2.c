#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <pthread.h>

void *my_thread1(void* _v) {
    printf("The parameter is %d\n", *((int* )_v));
    pthread_exit(NULL);
}

void *my_thread2(void* _c) {
    printf("The parameter is %c\n", *((char* )_c));
    pthread_exit(NULL);
}

int main() {
    pthread_t id_1, id_2;
    int ret, _val = 5;
    char _ch = 'a';
    ret = pthread_create(&id_1, NULL, (void* ) my_thread1, &_val);
    if (ret != 0) {
        printf("Failed to create thread !\n");
        exit(1);
    }
    ret = pthread_create(&id_2, NULL, (void* ) my_thread2, &_ch);
    if (ret != 0) {
        printf("Failed to create thread !\n");
        exit(1);
    }
    pthread_join(id_1, NULL);
    pthread_join(id_2, NULL);
    return 0;
}