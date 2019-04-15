#include <stdio.h>




char * convert (char * dst, const char * src)
{
    char* s = (char*)src;
    int s1 = 0;

    while (*s == '0')
        s++;

    while (*s != '\0')
    {

        if ('0' <= *s && *s <= '7')
        {
            dst[s1] = '0';
            if (s1 == 0)
                s1--;
        }
        else
            dst[s1] = '1';

        s1++;

        if ('4' <= *s && *s <= '7' || 'c' <= *s && *s <= 'f' || 'C' <= *s && *s <= 'F')
            dst[s1] = '1';
        else
        {
            dst[s1] = '0';
            if (s1 == 0)
                s1--;
        }

        s1++;

        if ('2' <= *s && *s <= '3' || '6' <= *s && *s <= '7' || 'A' <= *s && *s <= 'B' || 'E' <= *s && *s <= 'F' || 'a' <= *s && *s <= 'b' || 'e' <= *s && *s <= 'f')
            dst[s1] = '1';
        else
        {
            dst[s1] = '0';
            if (s1 == 0)
                s1--;
        }

        s1++;

        if (*s == '1' || *s == '3' || *s == '5' || *s == '7' || *s == '9' || *s == 'b' || *s == 'B' || *s == 'd' || *s == 'D' || *s == 'f' || *s == 'F')
            dst[s1] = '1';
        else
        {
            dst[s1] = '0';
            if (s1 == 0)
                s1--;
        }

        s1++;

        s++;
    }

    return dst;
}

int main()
{
    char d[10];
    printf("%s\n", convert(d, "ABC"));	// 110100011
}
