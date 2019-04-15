#include <stdio.h>
#include <assert.h>

const char* s = NULL;


int GetG(const char* str);
int GetE();
int GetT();
int GetP();
int GetN();



int GetG(const char* str)
{
    s = str;
    int val = GetE();
    assert(*s == '\0');

    return val;
}

int GetE()
{
    int val = GetT();

    while (*s == '-' || *s == '+')
    {
        int op = *s;
        s++;
        int val2 = GetT();

        if (op == '-')
            val -= val2;
        else
            val += val2;
    }

    return val;
}

int GetT()
{
    int val = GetP();

    while (*s == '*' || *s == '/')
    {
        int op = *s;
        s++;
        int val2 = GetP();

        if (op == '*')
            val *= val2;
        else
            val /= val2;
    }

    return val;
}

int GetP()
{
    if (*s == '(')
    {
        s++;
        int val = GetE();
        assert(*s == ')');
        s++;
        return val;
    }
    else
        return GetN();
}

int GetN()
{
    const char* s_0 = s;
    int val = 0;

    while ('0' <= *s && *s <= '9')
    {
        val = val*10 + *s - '0';
        s++;
    }

    assert( s != s_0);
    return val;
}

int main()
{
    char str_in[100];

    scanf("%s", str_in);

    printf("%d", GetG(str_in));

}
















