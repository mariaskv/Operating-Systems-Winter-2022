#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <semaphore.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

#include "lines.h"
#include "producer.h"
#include "consumer.h"

#define N 100
#define SEM_NAME1 "semaphore1"
#define SEM_NAME2 "semaphore2"
#define SEM_NAME3 "semaphore3"
#define SEM_NAME4 "semaphore4"
#define SEM_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)


union semun{
    int val;
    struct semid_ds *buf;
    unsigned short *array;
};

int main(int argc, char* argv[]){

    int K, Nproducers, lines;
    char text[N];

    struct mem{
        int id;
        int pidmatch;
        char string[N];
    };

    if(argc < 4){
        printf("Please give all the arguements needed\n");
        return -1;
    }

    K = atoi(argv[1]); //the K childs that are ment to create
    Nproducers = atoi(argv[2]); //the N processes that every child has to do
    lines = n_lines(argv[3]) ; //the size of the text file

    if(lines == -1)
        return -1;

    // create a key
    key_t key = ftok("main.c", 'R');
    if(key == -1){
        perror("Ftok Error");
        return -1;
    }

    // create a shared memory
    int shmid = shmget(key, sizeof(struct mem), IPC_CREAT|0666);
    if(shmid == -1){
        perror("Shmget Error");
        return -1;
    }

    // assign this segment to a pointer 
    struct mem *data = (struct mem *) shmat(shmid, (char*) 0, 0);
    if(data == (struct mem*) - 1){
        perror("Shmat Error");
        return -1;
    }

    sem_t *sem1 = sem_open(SEM_NAME1, O_CREAT, SEM_PERMS, 1);

    if(sem1 == SEM_FAILED){
        perror("SemOpen Error/Reader");
    }

    sem_t *sem2 = sem_open(SEM_NAME2, O_CREAT, SEM_PERMS, 0);

    if(sem2 == SEM_FAILED){
        perror("SemOpen Error/Writer");
    }

    sem_t *sem3 = sem_open(SEM_NAME3, O_CREAT, SEM_PERMS, 0);

    if(sem3 == SEM_FAILED){
        perror("SemOpen Error/Writer");
    }

    sem_t *sem4 = sem_open(SEM_NAME4, O_CREAT, SEM_PERMS, 0);

    if(sem4 == SEM_FAILED){
        perror("SemOpen Error/Writer");
    }

    int pid;
    for(int i = 0; i < Nproducers; i++){
        pid = fork();
        if(pid == 0){
            break;
        }
        else if(pid < 0){
            perror("Fork Error");
            return -1;
        }
        else{
        }
    }

    srand(time(NULL) + getpid());
        if(pid == 0){ //child
            clock_t begin = clock();
            for(int j = 0; j < K; j++){
                sem_wait(sem1);
                data->pidmatch = getpid();
                int pos = rand()%lines +1;
                data->id = pos;
                sem_post(sem2);
                sem_wait(sem3);
                consumer(data->string, getpid(),data->id);
                sem_post(sem1);
            }
            clock_t end = clock();
            double time = (double)((end - begin))/CLOCKS_PER_SEC;
            printf("Child with id: %d terminated with average time: %f\n", getpid(), time);
        }
        else if(pid > 0){ //parent  
            for(int j = 0; j < Nproducers*K; j++){ 
                sem_wait(sem2);
                producer(data->string, argv[3], data->id, N, data->pidmatch);
                sem_post(sem3);
            }
            for(int i = 0; i < Nproducers*K; i++){
                wait(NULL); 
            }
        }



    sem_close(sem1);
    sem_unlink(SEM_NAME1);
    sem_close(sem2);
    sem_unlink(SEM_NAME2);
    sem_close(sem3);
    sem_unlink(SEM_NAME3);
    sem_close(sem4);
    sem_unlink(SEM_NAME4);

    shmdt(data);
    shmctl(shmid, IPC_RMID, (struct shmid_ds*) 0 );

    return 0;
}