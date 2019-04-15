#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char * const suit = "cshd";
char * const rank = "23456789TJQKA";



struct Card {
    char rank;    // достоинство
    char suit;    // масть
};




int check(struct Card * hand)
{
    int table[13];
    int i = 0;

    for (i = 0; i < 13; i++)
    {
        table[i] = 0;
    }

    struct Card * p = hand;

    while (p->rank != 0)
    {
        char r = p->rank;

        if ('2' <= r && r <= '9')
            table[r - '2']++;
        else
            switch (r)
            {
                case 'T':
                    table[8]++;
                    break;
                case 'J':
                    table[9]++;
                    break;
                case 'Q':
                    table[10]++;
                    break;
                case 'K':
                    table[11]++;
                    break;
                case 'A':
                    table[12]++;
                    break;
            }

        p++;
    }

    int answ = -1;

    int count2 = 0;
    for (i = 0; i < 13; i++)
    {
        if (table[i] == 2)
        {
            count2++;
            answ = i;
        }
    }

    if (count2 != 2)
        answ = -1;

    return answ;
}

int main()
{
    Card hand[] = {{'Q', 's'}, {'A','h'}, {'9', 'd'}, {0, 0}};

    printf("%d\n", check(hand));

}
