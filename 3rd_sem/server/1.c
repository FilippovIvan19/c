#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/select.h>


struct connector
{
    int in_pipe[2];
    int out_pipe[2];
    int capacity;
    char* buffer;
    char* curpos;
    int filled;
};



//читаем отсюда
#define RD_END 0
#define WR_END 1


#define MIN_CON_BUF 1024
#define MAX_CON_BUF (MIN_CON_BUF * 128)
#define CHILD_BUF_SIZE MAX_CON_BUF


long N;
struct connector* con = NULL;


void child_work(int num)
{
    for (int i = 1; i < N; i++)
    {
        if (i != num)
        {
            close(con[i - 1].out_pipe[RD_END]);
            close(con[i - 1].out_pipe[WR_END]);
            close(con[i].in_pipe[RD_END]);
            close(con[i].in_pipe[WR_END]);
        }
        else
        {
            close(con[i - 1].out_pipe[WR_END]);
            close(con[i].in_pipe[RD_END]);
        }
    }
    int in  = con[num - 1].out_pipe[RD_END];
    int out = con[num].in_pipe[WR_END];
    free(con);
    char buffer[CHILD_BUF_SIZE] = {0};
    int read_count = -1;
    int write_count = -1;
    do {
        if (num == 2)
            sleep(1);
        read_count = read(in, buffer, MIN_CON_BUF);
        write_count = write(out, buffer, read_count);
        if (read_count != write_count)
            break;
    } while (read_count > 0);

    close(out);
    close(in);
}


void parent_work(void)
{
    for (int i = 0; i < N - 1; i++)
    {
        close(con[i].out_pipe[RD_END]);
        close(con[i].in_pipe[WR_END]);
    }


    int buf_size = MIN_CON_BUF;
    for (int i = N - 1; i >= 0; i--)
    {
        con[i].capacity = buf_size;
        con[i].curpos = con[i].buffer = (char*)calloc(con[i].capacity, sizeof(char));
        con[i].filled = 0;
        if (buf_size * 3 < MAX_CON_BUF)
            buf_size *= 3;
    }


    int max_fd = -1;
    while (1)
    {
        fd_set readfds, writefds;
        FD_ZERO(&readfds);
        FD_ZERO(&writefds);
        int starting_fds = 0;

        for (int i = 0; i < N; i++)
        {
            if (con[i].in_pipe[RD_END] > max_fd)
                max_fd = con[i].in_pipe[RD_END];
            if (con[i].out_pipe[WR_END] > max_fd)
                max_fd = con[i].out_pipe[WR_END];

            if ((con[i].in_pipe[RD_END] != -1) && (con[i].filled == 0))
            {
                FD_SET(con[i].in_pipe[RD_END], &readfds);
                starting_fds++;
            }
            if ((con[i].out_pipe[WR_END] != -1) && (con[i].filled != 0))
            {
                FD_SET(con[i].out_pipe[WR_END], &writefds);
                starting_fds++;
            }
        }
        if (!starting_fds)
            break;
        int ready_count = select(max_fd + 1, &readfds, &writefds, NULL, NULL);
        if (!ready_count)
            break;


        for (int i = 0; i < N; i++)
        {
            if (FD_ISSET(con[i].in_pipe[RD_END], &readfds))
            {
                int read_count = read(con[i].in_pipe[RD_END], con[i].buffer, con[i].capacity);
                con[i].filled += read_count;
                if (read_count == 0)
                {
                    close(con[i].in_pipe[RD_END]);
                    close(con[i].out_pipe[WR_END]);
                    con[i].in_pipe[RD_END] = con[i].out_pipe[WR_END] = -1;
                }
            }
            if (FD_ISSET(con[i].out_pipe[WR_END], &writefds))
            {
                int write_count = write(con[i].out_pipe[WR_END], con[i].curpos, con[i].filled);
                if (write_count < con[i].filled)
                    con[i].curpos += write_count;
                else
                    con[i].curpos = con[i].buffer;
                con[i].filled -= write_count;
            }
        }
    }

    for (int i = 0; i < N; i++)
        free(con[i].buffer);
    
    free(con);
}


int main(int argc, char* argv[])
{
    if (argc != 3)
        return -1;
    
    char *endptr;
    N = strtol(argv[2], &endptr, 10) + 1;
    //N-1 children

    con = (struct connector*)calloc(N, sizeof(struct connector));
    int fd = open(argv[1], O_RDONLY);

    for (int i = 1; i < N - 1; i++)
    {
        pipe(con[i].out_pipe);
        fcntl(con[i].out_pipe[WR_END], F_SETFL, O_WRONLY | O_NONBLOCK);
        pipe(con[i].in_pipe);
        fcntl(con[i].in_pipe[RD_END], F_SETFL, O_RDONLY | O_NONBLOCK);
    }

    con[0].in_pipe[RD_END] = -1;
    con[0].in_pipe[WR_END] = -1;
    con[0].out_pipe[RD_END] = fd;
    con[0].out_pipe[WR_END] = -1;

    con[N - 1].in_pipe[RD_END] = -1;
    con[N - 1].in_pipe[WR_END] = 1;
    con[N - 1].out_pipe[RD_END] = -1;
    con[N - 1].out_pipe[WR_END] = -1;


    for (int i = 1; i < N; i++) // с 1го по n-й ребёнок
    {
        int pid = fork();
        if (pid == 0)
        {
            child_work(i);
            return 0;
        }
    }

    parent_work();
    return 0;
}