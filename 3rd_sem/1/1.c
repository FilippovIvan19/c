#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


void print_range(long num)
{
    if (num >= 1)
        for (long i = 1; i < num; ++i)
            printf("%ld ", i);
    else
        for (long i = 1; i > num; --i)
            printf("%ld ", i);
    
    printf("%ld\n", num);
}


int main(int argc, char const *argv[])
{
    if (argc < 2)
    {
        printf("no argument\n");
        return 1;
    }
    else if (argc > 2)
    {
        printf("too many arguments\n");
        return 2;
    }

    char *endptr;
    errno = 0;
    long num = strtol(argv[1], &endptr, 10);

    if (*endptr != '\0')
    {
        printf("argument is not a number\n");
        return 3;
    }

    if (errno == ERANGE)
    {
        perror("overfow or underflow");
        return 4;
    }


    print_range(num);
    return 0;
}