#include <stdio.h>



int main()
{
    int xlt[1000], ylt[1000], xrb[1000], yrb[1000], s = 0, n;

    scanf ("%d", &n);

    for (int i = 0; i < n; i++)
        scanf ("%d%d%d%d", &xlt[i], &ylt[i], &xrb[i], &yrb[i]);

    for (int i = 0; i < n; i++)
        if (ylt[n - 1] < yrb[i])
            s++;

    printf ("%d", s);


    return 0;
}
