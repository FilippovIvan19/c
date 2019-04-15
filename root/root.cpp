/// @file root.cpp

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

const int NUM_INF = -1;
const float eps = 0.000001;

int SolveLine(float a, float b, float* x);
int SolveSquare(float a, float b, float c, float* x1, float* x2);


int main()
{
	float a = 0, b = 0, c = 0, x1 = 0, x2 = 0;
	printf("Enter the coefficients\n");
	if(scanf("%f %f %f", &a, &b, &c) != 3)
	{
        printf("you should put in 3 numbers");
        exit(1);
    }
	int nRoots = SolveSquare(a, b, c, &x1, &x2);

	switch (nRoots)
	{
		case 0:
		{
			printf("no roots");
			break;
		}
		case 1:
		{
			printf("one root: x = %4.2f", x1);
			break;
		}
		case 2:
		{
			printf("two roots: x1 = %4.2f x2 = %4.2f", x1, x2);
			break;
		}
		case NUM_INF:
		{
			printf("infinity number of roots");
			break;
		}
		default: printf("error with nRoots. nRoots = %d");
	}
}


//-------------------------------------------------------------------------------
//! Solves a linear equation ax + b = 0
//!
//! @param [in]	a		a - coefficient a
//! @param [in] b		b - coefficient b
//! @param [out] x		Pointer to the root
//!
//! @return Number of roots
//!
//! @note In case of infinite number of roots, return NUM_INF
//-------------------------------------------------------------------------------
int SolveLine(float a, float b, float* x)
{
	if (fabs(a) < eps)
			return (fabs(b) < eps)? NUM_INF : 0;
		else
		{
			*x = -b / a;
			return 1;
		}
}

//-------------------------------------------------------------------------------
//! Solves a square equation ax^2 + bx + c = 0
//!
//! @param [in]	a		a - coefficient a
//! @param [in] b		b - coefficient b
//! @param [in] c		c - coefficient c
//! @param [out] x1		Pointer to the 1st root
//! @param [out] x2		Pointer to the 2nd root
//!
//! @return Number of roots
//!
//! @note In case of infinite number of roots, return NUM_INF
//-------------------------------------------------------------------------------
int SolveSquare(float a, float b, float c, float* x1, float* x2)
{
	assert(isfinite (a));
	assert(isfinite (b));
	assert(isfinite (c));

	assert(x1 != NULL);
	assert(x2 != NULL);
	assert(x1 != x2);

	float discr = 0, sqrt_discr = 0;
	if (fabs(a) < eps)
		return SolveLine(b, c, x1);
	else
	{
		discr = b * b - 4 * a * c;
		if (discr < 0)
			return 0;
		else
		{
			sqrt_discr = sqrt(discr);
			*x1 = (-b + sqrt_discr) / (2 * a);
			*x2 = (-b - sqrt_discr) / (2 * a);
			return (discr != 0)? 2 : 1;
		}
	}
}
