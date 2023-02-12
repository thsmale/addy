#include<stdlib.h>
#include<stdio.h>
#include<string.h>

void size(char *buf) {
	printf("will any of these print the length\n");
	printf("size of buf is %lu\n", sizeof buf);
	printf("size of buf is %lu\n", sizeof *buf);
	printf("size of buf is %lu\n", sizeof &buf);
	printf("\n");

	printf("strlen will %lu\n", strlen(buf));
	//printf("strlen will %lu\n", strlen(*buf));
	//printf("strlen will %lu\n", strlen(&buf));
}

int main() {
	char *x = malloc(sizeof(char) * 100);
	printf("size of x %lu\n", sizeof(x));
	char y[100];
	printf("size of y %lu\n", sizeof(y));
	size(&y);

}
