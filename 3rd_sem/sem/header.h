#include <stdio.h>
#include <limits.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <unistd.h>

#define $d(x) printf(#x " %d\n", x);
#define $s(x) printf(#x "\n");


#define ADD_ACTION(num, val, flags) \
    sops[act_num].sem_num = num;            \
    sops[act_num].sem_op = val;             \
    sops[act_num].sem_flg = flags;          \
    act_num++;

#define EXEC_ACTIONS \
    if (semop(sem_id, sops, act_num) == -1) \
        my_exit("semop error");             \
    act_num = 0;


#define PROG_FILE "header.h"
#define SEM_PROJ_ID 1
#define SHM_PROJ_ID 2
#define BUF_SIZE 2
#define SEM_COUNT 4
#define MAX_ACTION_COUNT 20

#define ALIVE 0
#define STEP 1
#define ALLOW_RD 2
#define ALLOW_WR 3

#define WAIT_ZERO 0
#define NO_FLAGS 0


struct smart_buf
{
    unsigned int size;
    char buf[BUF_SIZE];
};
#define SMART_BUF_SIZE sizeof(struct smart_buf)


#define PRINT_CASE(name) \
    case name:                  \
        printf("error:\n");     \
        printf(#name "\n");     \
        break;                  \


void my_exit(const char *str)
{
    perror(str);
    exit(EXIT_FAILURE);
}


int open_sem()
{
    key_t key = ftok(PROG_FILE, SEM_PROJ_ID);
    if (key == -1)
        my_exit("semkey error");

    int sem_id = semget(key, SEM_COUNT, 0666 | IPC_CREAT);
    if (sem_id == -1)
        my_exit("semget error");

    return sem_id;
}


int open_shm()
{
    key_t key = ftok(PROG_FILE, SHM_PROJ_ID);
    if (key == -1)
        my_exit("shmkey error");

    int shm_id = shmget(key, SMART_BUF_SIZE, 0666 | IPC_CREAT);
    if (shm_id == -1)
        my_exit("shmget error");

    return shm_id;
}