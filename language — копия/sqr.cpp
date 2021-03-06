FUNC main z
{
	CREATE a;
	CREATE b;
	CREATE c;
	CREATE x;
	CREATE xx;

	IN a;
	IN b;
	IN c;

	CREATE nroots;
	CREATE discr;
	CREATE sqrt_discr;


	IF a = 0
	{
		nroots = DO solveline b c;
		OUT nroots;
		IF nroots = 1
		{
            x = -1 * c / b;
            OUT x;
        }

    };

    IF a # 0
	{
		discr = b * b - 4 * a * c;

		IF discr > 0
		{
            nroots = 2;
            OUT nroots;

			sqrt_discr = @discr;
			x  = (-1 * b + sqrt_discr) / (2 * a);
			xx = (-1 * b - sqrt_discr) / (2 * a);
            OUT x;
            OUT xx;
		};

		IF discr < 0
		{
            nroots = 0;
            OUT nroots;
		};

		IF discr = 0
		{
            nroots = 1;
            OUT nroots;

            x = -1 * b / 2 / a;
            OUT x;
		};

	};

}


FUNC solveline a b
{
	IF a = 0
	{
        IF b = 0
			RET 3;
        IF b # 0
			RET 0;
    };

    IF a # 0
        RET 1;
}
