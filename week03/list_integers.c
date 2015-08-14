#include <stdio.h>

void list_integers( void ) {
	printf("The integer subrange from 21 to 312...");
	for( int i = 21; i < 313; i++ )
	{
		printf("\n%d", i);
	}
}

int main( void ) {
	list_integers();
	return 0;
}