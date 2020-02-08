#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>




int is_same_file(int fd1, int fd2)
{
    struct stat stat1;
    struct stat stat2;

    assert(fstat(fd1, &stat1) == 0 && fstat(fd2, &stat2) == 0);

    return stat1.st_ino == stat2.st_ino && stat1.st_dev == stat2.st_dev;
}

int main(int argc, char const *argv[])
{
    int fd1 = open("file", O_RDONLY);

    
    int fd2 = open("file", O_WRONLY);
    int fd3 = open("file1", O_RDONLY);
    int fd4 = open("file1", O_WRONLY);


    printf("%d\n", is_same_file(fd1, fd2));


    printf("%d %d %d %d\n", 
        is_same_file(fd1, fd2),
        is_same_file(fd1, fd3),
        is_same_file(fd1, fd4),
        is_same_file(fd3, fd4)
    );

    close(fd1);
    close(fd2);
    close(fd3);
    close(fd4);
    
    return 0;
}
