#include <stdlib.h>

char* foo() {
	char *foo = malloc(sizeof(char) * 1000);
	foo = "does this memory get automatically freed?";
	return foo;
}

int main() {
	printf("%c", foo);
	return 0;
}
