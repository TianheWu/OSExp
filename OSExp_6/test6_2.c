# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <wait.h>
# include <string.h>
# include <signal.h>

# define ERROR 1
# define FINISH 0

static void sig_user(int signo) {
    if (signo == SIGINT) {
        printf("\nThe number of SIGINT signal is %d\n", signo);
        printf("The process pid is %d\n", getpid());
    }
    else if (signo == SIGUSR1) {
        printf("\nThe number of SIGUSR1 signal is %d\n", signo);
        printf("The process pid is %d\n", getpid());
    }
    exit(FINISH);
}

int main() {
    pid_t pid;
    pid = fork();
    if (pid < 0) {
        printf("Failed to fork !\n");
        exit(ERROR);
    }
    else if (pid == 0) {
        printf("Child process is waiting...\n");
        while (1) pause();
        exit(FINISH);
    } else {
        if (signal(SIGINT, sig_user) == SIG_ERR) {
            printf("Can't catch SIGINT\n");
            exit(ERROR);
        }
        if (signal(SIGUSR1, sig_user) == SIG_ERR) {
            printf("Can't catch SIGUSER1\n");
            exit(ERROR);
        }
        printf("Father process is waiting...\n");
        while (1) pause();
        exit(FINISH);
    }
    return 0;
}

