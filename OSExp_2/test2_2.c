#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main() {
    int pid_1, pid_2;
    pid_1 = fork();
    if (pid_1 < 0) {
        printf("Failed to fork !");
        exit(1);
    }
    else if (pid_1 == 0) {
        for (int i = 0; i < 1000; i++)
            printf("b");
    }
    else {
        pid_2 = fork();
        if (pid_2 < 0) {
            printf("Failed to fork !\n");
            exit(1);
        }
        else if (pid_2 == 0) {
            for (int i = 0; i < 1000; i++)
                printf("c");
        }
        else {
            for (int i = 0; i < 1000; i++)
                printf("a");
        }
        exit(0);
    }
    return 0;
}