FUNC main z
{
    CREATE n;
    IN n;

    CREATE fact;
    fact = DO factorial n;

    OUT fact;
}


FUNC factorial n
{
    IF n = 0
        RET 1;

    IF n # 0
    {
        CREATE x;
        x = n - 1;
        RET n * DO factorial x;
    }
}
