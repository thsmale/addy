#include<stdlib.h>
#include<stdio.h>

int main() {
	char *x = malloc(sizeof(char) * 100);
	printf("size of x %lu\n", sizeof(x));
	char y[100];
	printf("size of y %lu\n", sizeof(y));
}
