#include <stdio.h>

int main()
{
	float y, x = 0;
	while (x < 7)
	{
		if (x < 4)
			y = x;
		else
			if (x > 4)
				y = x * x;
			else
				y = 1;
		printf("x=%f\t", x);
		printf("y=%5.2f\n", y);
		x = x + 2;
	}
}
