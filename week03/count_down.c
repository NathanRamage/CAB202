#include <stdio.h>

void count_down( void )
{
	printf("Count down from 304, by steps of 5, finishing before lower bound (15) is reached.");
	for( int i = 304; i > 15; i -= 5)
	{
		printf("\n%d", i);
	}
}

int main( void )
{
	count_down();
	return 0;
}
