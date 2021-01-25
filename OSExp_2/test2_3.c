#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <wait.h>

int main() {
    int fd[2], pid_1, pid_2;
    char wr[100], re[100];
    if (pipe(fd) == -1) {
        printf("Create pipe error");
        exit(1);
    }
    pid_1 = fork();
    if (pid_1 < 0) {
        printf("Failed to fork !");
        exit(1);
    }
    else if (pid_1 == 0) {
        sprintf(wr, "Child 1 is sending a message!");
        write(fd[1], wr, 50);
        exit(0);
    } else {
        pid_2 = fork();
        if (pid_2 < 0) {
            printf("Create pipe error");
            exit(1);
        }
        else if (pid_2 == 0) {
            sprintf(wr, "Child 2 is sending a message!");
            write(fd[1], wr, 50);
            exit(0);
        } else {
            wait(NULL);
            read(fd[0], re, 50);
            printf("%s\n", re);
            wait(NULL);
            read(fd[0], re, 50);
            printf("%s\n", re);
            exit(0);
        }
    }
    return 0;
}