#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <features.h>
#include <sys/ioctl.h>


#define BUF_SIZE 4
#define DATA_FIFO_NAME_LEN 20

#define PID_FIFO_NAME "pid_fifo"
#define DATA_FIFO_NAME_TEMPLATE "data_fifo_%d"


int readpid(int pid_fd, int *pid);


int main(int argc, char *argv[])
{
    mkfifo(PID_FIFO_NAME, 0777);

    int pid_fifo_fd = open(PID_FIFO_NAME, O_RDONLY);
    // start sinhr
    int pid = 0;
    readpid(pid_fifo_fd, &pid);
    // start борьба за чтение пида/////////////////////////////////

    char data_fifo_name[DATA_FIFO_NAME_LEN] = "";
    sprintf(data_fifo_name, DATA_FIFO_NAME_TEMPLATE, pid);

    int data_fifo_fd = open(data_fifo_name, O_RDONLY | O_NONBLOCK);/////////////

    if (data_fifo_fd == -1)
    {
        printf("error with opening data fifo\n");
        return -1;
    }

    int flags = fcntl(data_fifo_fd, F_GETFL);
    flags &= ~O_NONBLOCK;
    fcntl(data_fifo_fd, F_SETFL, flags);

    char buf[BUF_SIZE] = "";
    int read_count = -1;
    while ((read_count = read(data_fifo_fd, buf, BUF_SIZE)) > 0)
    {
        write(1, buf, read_count);
    }
    // end sinhr
    close(pid_fifo_fd);
    close(data_fifo_fd);
    unlink(data_fifo_name);
    return 0;
    // end crit readpid/////////////////////////////////
}


int readpid(int pid_fd, int *pid)
{
    int write_fd = open(PID_FIFO_NAME, O_WRONLY);
    read(pid_fd, pid, sizeof(int)); //crit
    close(write_fd);
    return 0;
}

// борются за доступ к datafifo тк доступ к ней получает только один printer.