# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>
# include <sys/ipc.h>
# include <sys/shm.h>
# include <wait.h>
# include <string.h>


typedef struct {
    int pos_w;
    int val;
} Shared;


int main() {
    int pid = fork();
    if (pid < 0) {
        printf("Failed to fork !\n");
        exit(1);
    }
    else if (pid == 0) {
        // Write ###########################
        void* shm = NULL;
        Shared* shared = NULL;
        int shmid = shmget(1000, 100, 0666 |  IPC_CREAT);
        if (shmid == -1) {
            printf("SHMGET ERROR !\n");
            exit(1);
        }
        shm = shmat(shmid, NULL, 0);
        if (shm == (void* )-1) {
            printf("SHMAT ERROR !\n");
            exit(1);
        }
        shared = (Shared* )shm;
        shared->pos_w = 0;
        int count = 1;
        for (int i = 0; i < 10; i++) {
            while (shared->pos_w == 1) {
                sleep(1);
                printf("Waiting...\n");
            }
            sleep(1);
            printf("Input the %d number: ", count);
            count++;
            int tmp;
            scanf("%d", &tmp);
            shared->pos_w = 1;
            shared->val = tmp;
        }
        if (shmdt(shm) == -1) {
            printf("SHMDT ERROR !\n");
            exit(1);
        }
        sleep(1);
        exit(0);
    } else {
        int pid = fork();
        if (pid < 0) {
            printf("Failed to fork !\n");
            exit(1);
        }
        else if (pid == 0) {
            // Read ############################
            void* shm = NULL;
            Shared* shared = NULL;
            int shmid = shmget(1000, 100, 0666 |  IPC_CREAT);
            if (shmid == -1) {
                printf("SHMGET ERROR !\n");
                exit(1);
            }
            // Connect the shared memory
            shm = shmat(shmid, NULL, 0);
            if (shm == (void* )-1) {
                printf("SHMAT ERROR !\n");
                exit(1);
            }
            shared = (Shared* )shm;
            // Set write
            shared->pos_w = 0;
            int count = 0;
            while (1) {
                if (shared->pos_w == 1) {
                    count++;
                    int _v = shared->val * shared->val;
                    shared->pos_w = 0;
                    printf("The num is: %d and the square is %d\n", shared->val, _v);
                    if (count > 10) break;
                }
            }
            // Split the shared memory with current process
            if (shmdt(shm) == -1) {
                printf("SHMDT ERROR !\n");
                exit(1);
            }
            if (shmctl(shmid, IPC_RMID, NULL) == -1) {
                printf("SHMCTL ERROR !\n");
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
