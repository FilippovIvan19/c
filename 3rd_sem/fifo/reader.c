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


int main(int argc, char *argv[])
{
    const char *file_name = argv[1];
    int file_fd = open(file_name, O_RDONLY);
    if (file_fd == -1)
    {
        printf("error with opening file\n");
        return -1;
    }
    char buf[BUF_SIZE] = "";

    int pid = getpid();
    char data_fifo_name[DATA_FIFO_NAME_LEN] = "";
    sprintf(data_fifo_name, DATA_FIFO_NAME_TEMPLATE, pid);

    mkfifo(data_fifo_name, 0777);

    int data_fifo_read_fd = open(data_fifo_name, O_RDONLY | O_NONBLOCK);
    int data_fifo_write_fd = open(data_fifo_name, O_WRONLY);
    
    mkfifo(PID_FIFO_NAME, 0777);
    int pid_fifo_fd = open(PID_FIFO_NAME, O_WRONLY); 
    //start sinhr
    write(pid_fifo_fd, &pid, sizeof(int));

    sleep(2);
    
    close(data_fifo_read_fd);
    int read_count = -1;

    // start/////////////////////////////////
    while ((read_count = read(file_fd, buf, BUF_SIZE)) > 0)
    {
        write(data_fifo_write_fd, buf, read_count);///////////////
    }
    // end/////////////////////////////////
    
    close(data_fifo_write_fd);
    // end sinhr
    close(pid_fifo_fd);
    close(file_fd);
    return 0;
}

// ждёт когда подключится printer чтобы write не выпал с ошибкой