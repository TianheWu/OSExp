# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <unistd.h>
# include <wait.h>
# include <pthread.h>
# include <semaphore.h>

# define N 2


int buf[2], in = 0, out = 0, fin = 0;


// r_w_ read and write mutex
// cur_n current number in buf
// max_n the max number can product
// p_m_ plus and mul mutex
// p_ ans m_ is consume with across way
// r12_ and r21_ is product with across way


sem_t r_w_, cur_n, max_n, p_m_, p_, m_, r12_, r21_;


int take() {
    int _val = buf[out++];
    out %= N;
    return _val;
}

void put(int _val) {
    buf[in++] = _val;
    in %= N;
}

void *read1() {
    int val;
    FILE *f1 = fopen("dat1.txt", "r");
    while (!feof(f1)) {
        sem_wait(&r12_);
        sem_wait(&max_n);
        sem_wait(&r_w_);
        fscanf(f1, "%d", &val);
        put(val);
        // printf("Put data %d from dat1\n", val);
        sem_post(&r_w_);
        sem_post(&cur_n);
        sem_post(&r21_);
    }
}

void *read2() {
    int val;
    FILE *f2 = fopen("dat2.txt", "r");
    while (!feof(f2)) {
        sem_wait(&r21_);
        sem_wait(&max_n);
        sem_wait(&r_w_);
        fscanf(f2, "%d", &val);
        put(val);
        // printf("Put data %d from dat2\n", val);
        sem_post(&r_w_);
        sem_post(&cur_n);
        sem_post(&r12_);
    }
}

void *op_plus() {
    while (1) {
        sem_wait(&p_);
        sem_wait(&p_m_);
        sem_wait(&cur_n);
        if (fin == 20)
            break;
        sem_wait(&r_w_);
        int val_1 = take();
        fin++;
        sem_post(&r_w_);
        sem_post(&max_n);

        sem_wait(&cur_n);
        sem_wait(&r_w_);
        int val_2 = take();
        printf("%d + %d = %d\n", val_1, val_2, val_1 + val_2);
        fin++;
        sem_post(&r_w_);
        sem_post(&max_n);
        sem_post(&p_m_);
        sem_post(&m_);
        if (fin == 20) {
            sem_post(&cur_n);
            break;
        }
    }
}

void *op_mul() {
    while (1) {
        sem_wait(&m_);
        sem_wait(&p_m_);
        sem_wait(&cur_n);
        if (fin == 20)
            break;
        sem_wait(&r_w_);
        int val_1 = take();
        fin++;
        sem_post(&r_w_);
        sem_post(&max_n);

        sem_wait(&cur_n);
        sem_wait(&r_w_);
        int val_2 = take();
        printf("%d * %d = %d\n", val_1, val_2, val_1 * val_2);
        fin++;
        sem_post(&r_w_);
        sem_post(&max_n);
        sem_post(&p_m_);
        sem_post(&p_);
        if (fin == 20) {
            sem_post(&cur_n);
            break;
        }
    }
}

int main() {
    pthread_t id_1, id_2, id_3, id_4;
    sem_init(&r_w_, 0, 1);
    sem_init(&cur_n, 0, 0);
    sem_init(&max_n, 0, 2);
    sem_init(&p_m_, 0, 1);
    sem_init(&p_, 0, 1);
    sem_init(&m_, 0, 0);
    sem_init(&r12_, 0, 1);
    sem_init(&r21_, 0, 0);

    int ret;
    ret = pthread_create(&id_1, NULL, (void* ) read1, NULL);
    if (ret != 0) {
        printf("Failed to create thread !\n");
        exit(1);
    }
    ret = pthread_create(&id_2, NULL, (void* ) read2, NULL);
    if (ret != 0) {
        printf("Failed to create thread !\n");
        exit(1);
    }
    ret = pthread_create(&id_3, NULL, (void* ) op_plus, NULL);
    if (ret != 0) {
        printf("Failed to create thread !\n");
        exit(1);
    }
    ret = pthread_create(&id_4, NULL, (void* ) op_mul, NULL);
    if (ret != 0) {
        printf("Failed to create thread !\n");
        exit(1);
    }

    pthread_join(id_1, NULL);
    pthread_join(id_2, NULL);
    pthread_join(id_3, NULL);
    pthread_join(id_4, NULL);
    printf("FINISHED!\n");
    return 0;
}