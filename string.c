#include <stdio.h>
#include <string.h>

/**
 * A custom string library
 * Cause C strings are so pesky
 */

void string_length_test();
void snprintf_test();
void strcmp_test();
char *return_string_literal();
char *return_string_array();
char *return_string_pointer();

int main () {

	char *multiline = "one line\
			   two line\
			   three line\
			   four fish";
	if (strlen(multiline) < 10) {
		fprintf(stderr, "Unable to print multiline error\n");
	}

	char *ret = return_string_literal();
	if (strcmp(ret, "return string") != 0) {
		printf("Not able to return string literal from function");
	}

	ret = return_string_pointer();
	if (strcmp(ret, "return_string_pointer") != 0) 
		fprintf(stderr, "Not able to return char pointer from function");


	ret = return_string_array();
	for(int i = 0; i < 3; ++i) {
		if (ret[i] != 'z') {
			fprintf(stderr, "Not able to return buffer from function\n");
		}
	}

	/*
	strcmp_test();
	string_length_test();
	snprintf_test();
	*/

	return 0;


}

/**
 * strcmp
 * @param x char pointer
 * @param y char pointer
 */
void strcmp_test() {

	char *str, *strr;
	str = "I can initialize it down here";
	strr = "null terminated string\0";
	printf("%s, %s\n", str, strr);

	/**
	 * Can only compare two char pointers
	if (strcmp("a", "a") != 0)
		fprintf(stderr, "Failed to see if two characters were equal\n");
	*/

	/**
	 * '' is treated as type int
	char *ptr = 'a';
	 */

	char *msg = "yup";
	char *copy1 = msg;
	char *copy2 = msg;
	if (strcmp(copy1, copy2) != 0)
		fprintf(stderr, "double quotes does not equal single quotes\n");

	if (strcmp("apple", "apple\0") != 0) {
		fprintf(stderr, "non null terminated string != null terminated string\n");
	}
}

void string_length_test() {
	char *a;
	char *b = "";
	char *c = "\0";
	char *d = "insert mc here";
	char *e = "null terminated string\0";
	printf("%lu, %lu, %lu, %lu, %lu\n", strlen(a), strlen(b), strlen(c), strlen(d), strlen(e));
	//printf("%s\n%s\n%s\n%s\n%s\n", a, b, c, d, e);

	char *apple = "apple";
	char *null_terminated_apple = "apple\0";
	if(strlen(apple) != strlen(null_terminated_apple)) {
		printf("Expecting null terminated string to be same length as terminated string\n");
	}
}

void snprintf_test() {
	printf("\n\n******snprintf tests*********\n");
	char x[80];
	int y = 2;
	snprintf(x, sizeof(x), "y = %i\n", y);
	printf("%s\n", x);

	/*
	char *greeting;
	greeting = "how are you\0";
	snprintf(greeting, sizeof(greeting) * 80, "%s", " doing great how are you ");
	printf("greeting = %s\n", greeting);
	*/


	/**
	 * snprintf will overwrite the contents of the buffer
	 */
	char cat[80];
	char *head = "con\0";
	char *middle = "cat\0";
	char *tail = "nate\0";
	snprintf(cat, sizeof(char) * 80, "%s", head);
	snprintf(cat, sizeof(char) * 80, "%s", middle);
	snprintf(cat, sizeof(char) * 80, "%s", tail);
	if(strcmp(cat, "concatnate") == 0) {
		printf("%s == concatnate\n", cat);
		printf("Expecting snprintf to concatenate");
	}

	char buffer[80];
	char *message = "hello world\0";
	snprintf(buffer, sizeof(char) * strlen(message), "%s", message);
	printf("%s\n", buffer);
	
	/*
	char *buf;
	message = "yoshi";
	snprintf(buf, sizeof(char) * strlen(message), "%s", message);
	printf("%s\n", buf);

	char *bleh; 
	message = "null terminated string\0";
	snprintf(bleh, sizeof(char) * strlen(message), "%s", message);
	printf("%s\n", bleh);
	*/
}

char* return_string_literal() {
	return "return string";
}

char* return_string_pointer() {
	char *x = "return_string_pointer";
	return x;
}

char* return_string_array() {
	char buffer[3] = {'z', 'z', 'z'};
	char *ptr = buffer;
	// cannot return just buffer
	return ptr;
}

