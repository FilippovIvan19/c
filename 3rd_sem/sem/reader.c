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


    ADD_ACTION(STEP, WAIT_ZERO, NO_FLAGS)
    ADD_ACTION(ALIVE, WAIT_ZERO, NO_FLAGS)
    ADD_ACTION(STEP, 1, SEM_UNDO)
    ADD_ACTION(ALIVE, 1, SEM_UNDO)
    ADD_ACTION(ALLOW_WR, 1, SEM_UNDO)
    EXEC_ACTIONS

    ADD_ACTION(STEP, -1, IPC_NOWAIT)
    ADD_ACTION(STEP, 1, NO_FLAGS)
    ADD_ACTION(STEP, -2, NO_FLAGS)
    ADD_ACTION(STEP, 2, SEM_UNDO)
    EXEC_ACTIONS

    while (1)
    {
        ADD_ACTION(STEP, -2, IPC_NOWAIT)
        ADD_ACTION(STEP, 2, NO_FLAGS)
        ADD_ACTION(ALLOW_RD, -1, NO_FLAGS)
        EXEC_ACTIONS

            if (buf->size > 0)
                write(1, buf->buf, buf->size);
            else
                break;
            
        ADD_ACTION(STEP, -2, IPC_NOWAIT)
        ADD_ACTION(STEP, 2, NO_FLAGS)
        ADD_ACTION(ALLOW_WR, 1, NO_FLAGS)
        EXEC_ACTIONS
    }

    shmdt(buf);
    shmctl(shm_id, IPC_RMID, NULL);

    return 0;
}
