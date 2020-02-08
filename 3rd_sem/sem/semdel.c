#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/stat.h>

#define SEM_PATH "header.h"
#define SEM_PROJ_ID 1
#define SHM_PROJ_ID 2
#define BUF_SIZE 1

int main(int argc, char const *argv[])
{

    key_t sem_key = ftok(SEM_PATH, SEM_PROJ_ID);
    int sem_id = semget(sem_key, 4, 0777);
    semctl(sem_id, 0, IPC_RMID);
    return 0;
}