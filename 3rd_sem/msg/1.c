#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/wait.h>


long check_arg(int argc, char const *argv[], int n)
{
    if (argc <= n)
    {
        printf("few arguments\n");
        return -1;
    }

    char *endptr;
    errno = 0;
    long num = strtol(argv[n], &endptr, 10);

    if (*endptr != '\0')
    {
        printf("argument is not a number\n");
        return -2;
    }

    if (errno == ERANGE)
    {
        printf("overfow or underflow\n");
        return -3;
    }

    return num;
}


#define FIRST_TYPE 1
#define MSG_SIZE 1


struct msgbuf {
    long type;
    char text[MSG_SIZE];
};

int main(int argc, char const *argv[])
{
    // setbuf(stdout,NULL);
    int qid = msgget(IPC_PRIVATE, 0666);
    int count = check_arg(argc, argv, 1);
    int pid = -1;
    struct msgbuf msg;    

    int i = 0;
    for (i = 0; i < count; i++)
    {
        pid = fork();
        if (!pid)
            break;
    }

    if (pid)
    {
        msg.type = FIRST_TYPE;
        msgsnd(qid, &msg, MSG_SIZE, 0);
        // printf("%5ld sent\n", msg.type);
        
        msgrcv(qid, &msg, MSG_SIZE, FIRST_TYPE + count, 0);
        // printf("%5ld recieved\n", msg.type);

        msgctl(qid, IPC_RMID, 0);
        return 0;
    }
    
    msgrcv(qid, &msg, MSG_SIZE, i + FIRST_TYPE, 0);
    // printf("%5ld recieved\n", msg.type);


    printf("%d ", i + 1);

    msg.type += 1;
    // sleep(1);
    msgsnd(qid, &msg, MSG_SIZE, 0);
    // printf("%5ld sent\n", msg.type);

}

// setbuf(stdout,NULL);
// fflush(stdout)