#include <stdio.h>
/*
 * A comprehensive test for pointers
 */

int main() {
	// Reassign null pointer
	int *x = 0; // no '\0';
	int y = 777;
	x = &y;
	int *x = 7; // na 
	printf("%i, %i, %i\n", *x, y);
	return 0;
}
