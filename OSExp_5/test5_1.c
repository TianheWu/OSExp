# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/ipc.h>
# include <sys/msg.h>
# include <wait.h>
# include <string.h>



typedef struct {
    long int msg_type;
    char s[100];
} Msg;


int main() {
    int pid;
    pid = fork();
    if (pid < 0) {
        printf("Failed to fork !\n");
        exit(1);
    }
    else if (pid == 0) {
        char buf[100];
        Msg msg;
        int msgid = msgget(1000, IPC_CREAT | 0666);
        if (msgid == -1) {
            printf("Failed in calling msgget");
            exit(1);
        }
        while (1) {
            printf("Input the string: ");
            scanf("%s", buf);
            msg.msg_type = 1;
            strcpy(msg.s, buf);
            
            // Send message
            if (msgsnd(msgid, &msg, 100, 0) == -1) {
                printf("msgsnd error!\n");
                exit(1);
            }

            // End enter
            if (strncmp(msg.s, "exit", 4) == 0) break;
            sleep(1);
        }
        exit(0);
    } else {
        int pid;
        pid = fork();
        if (pid < 0) {
            printf("Failed to fork !\n");
            exit(1);
        }
        else if (pid == 0) {
            long int msg_type = 1;
            Msg msg;
            int msgid = msgget(1000, IPC_CREAT | 0666);
            if (msgid == -1) {
                perror("Failed in calling msgget");
                exit(1);
            }
            while (1) {
                if (msgrcv(msgid, &msg, sizeof(msg.s), msg_type, 0) == -1) {
                    printf("msgrcv error\n");
                    exit(1);
                }
                if (strncmp(msg.s, "exit", 4) == 0) break;
                printf("MESSAGE IS: %s\n", msg.s);
            }
            if (msgctl(msgid, IPC_RMID, NULL) == -1) {
                printf("msgctl error\n");
                exit(1);
            }
            exit(0);
        } else {
            wait(NULL);
            printf("FINISHED!\n");
            exit(0);
        }
        exit(0);
    }
    return 0;
}
