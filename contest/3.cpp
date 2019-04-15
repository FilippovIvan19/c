
char * convert (char * dst, const char * src)
{
    char* s = (char*)src;
    int pos = 0;

    while (*s == '0')
        s++;



    int num;
    if ('a' <= *s && *s >= 'f')
        num = *s - 'a';
    else if ('A' <= *s && *s >= 'F')
        num = *s - 'A';
    else
        num = *s - '0';

    dst[pos] = (num / 8) % 2 + '0';
    if ( !(pos == 0 && num % 8) )
        pos++;


    dst[pos] = (num / 4) % 2 + '0';
    if ( !(pos == 0 && num % 8) )
        pos++;


    dst[pos] = (num / 2) % 2 + '0';
    if ( !(pos == 0 && num % 8) )
        pos++;


    dst[pos] = (num / 1) % 2 + '0';
    if ( !(pos == 0 && num % 8) )
        pos++;



    return dst;
}

int main()
{
    char d[10];
    printf("%s\n", convert(d, "ABC"));	// 110100011
}
