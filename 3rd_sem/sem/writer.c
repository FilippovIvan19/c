#include "header.h"

int main(int argc, char const *argv[])
{
    int sem_id = open_sem();
    int shm_id = open_shm();
    struct smart_buf *buf = shmat(shm_id, NULL, 0);

    printf("%d %d %d %d\n", semctl(sem_id, 0, GETVAL), semctl(sem_id, 1, GETVAL),
        semctl(sem_id, 2, GETVAL), semctl(sem_id, 3, GETVAL));

    int act_num = 0;
    struct sembuf sops[MAX_ACTION_COUNT];

    char const *file_name = argv[1];
    int in_file_fd = open(file_name, O_RDONLY);

    ADD_ACTION(STEP, -1, NO_FLAGS)
    ADD_ACTION(ALIVE, -1, NO_FLAGS)
    ADD_ACTION(ALIVE, WAIT_ZERO, NO_FLAGS)
    ADD_ACTION(ALIVE, 1, NO_FLAGS)

    ADD_ACTION(STEP, 2, SEM_UNDO)
    ADD_ACTION(ALIVE, 1, SEM_UNDO)
    ADD_ACTION(ALLOW_RD, 1, SEM_UNDO)
    ADD_ACTION(ALLOW_RD, -1, NO_FLAGS)
    EXEC_ACTIONS

    while (1)
    {
        ADD_ACTION(STEP, -2, IPC_NOWAIT)
        ADD_ACTION(STEP, 2, NO_FLAGS)
        ADD_ACTION(ALLOW_WR, -1, NO_FLAGS)
        EXEC_ACTIONS
        
            buf->size = read(in_file_fd, buf->buf, BUF_SIZE);
            
        ADD_ACTION(STEP, -2, IPC_NOWAIT)
        ADD_ACTION(STEP, 2, NO_FLAGS)
        ADD_ACTION(ALLOW_RD, 1, NO_FLAGS)
        EXEC_ACTIONS
        
        if (buf->size <= 0)
            break;
    }

    shmdt(buf);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
