#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>
#include <pthread.h>

int cmp_1(const void* a, const void* b) {
    return *(int* )a >= *(int* )b ? 0 : 1;
}

int cmp_2(const void* a, const void* b) {
    return *(int* )a <= *(int* )b ? 0 : 1;
}

void *my_thread1(int* a) {
    int arr[10];
    for (int i = 0; i < 10; i++)
        arr[i] = a[i];
    qsort(arr, 10, sizeof(int), cmp_1);
    for (int i = 0; i < 10; i++)
        printf("%d ", arr[i]);
    printf("\n");
    pthread_exit(NULL);
}

void *my_thread2(int* a) {
    int arr[10];
    for (int i = 0; i < 10; i++)
        arr[i] = a[i];
    qsort(arr, 10, sizeof(int), cmp_2);
    for (int i = 0; i < 10; i++)
        printf("%d ", arr[i]);
    printf("\n");
    pthread_exit(NULL);
}

int main() {
    pthread_t id_1, id_2;
    int ret;
    int a[10] = {1, 4, 7, 0, 12, 32, 2, 8, 9, 71};
    ret = pthread_create(&id_1, NULL, (void* ) my_thread1, &a);
    if (ret != 0) {
        printf("Failed to create thread !\n");
        exit(1);
    }
    ret = pthread_create(&id_2, NULL, (void* ) my_thread2, &a);
    if (ret != 0) {
        printf("Failed to create thread !\n");
        exit(1);
    }
    pthread_join(id_1, NULL);
    pthread_join(id_2, NULL);
    return 0;
}