
Decimal div10n (Decimal a, unsigned int n)
{
    Decimal b;
    if (a.n - n >= 0)
    {
        b.n = a.n - n;

        for (unsigned int i = n; i <= a.n; i++)
            (b.a)[i - n] = (a.a)[i];
    }
    else
    {
        (b.a)[0] = '0';
        b.n = 0;
    }

    return b;
}
