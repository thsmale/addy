#include <stdio.h>
#include <string.h>

/**
 * A custom string library
 * Cause C strings are so pesky
 */

void strlen_test();
void snprintf_test();
void strcmp_test();
void printf_test();
void return_strings();
void char_array_test();
char *return_string_literal();
char *return_string_array();
char *return_string_pointer();
char *return_string_snprintf();
void print_alphabet();
void print_ascii_table();

int main() {

	char *multiline = "one line\
			   two line\
			   three line\
			   four fish";
	if (strlen(multiline) < 10) {
		fprintf(stderr, "Unable to print multiline error\n");
	}


	printf_test();

	/*
	return_strings();

	strlen_test();
	strcmp_test();
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

void strlen_test() {
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

	if (strlen("abc") != 3) {
		fprintf(stderr, "Cannot find length of string literal\n");
	}

	/*
	 * Seg fault, must pass char pointer
	*/
	char buf[69] = {'a', 'b', 'c', '\0'};
	if(strlen(buf) != 3) {
		fprintf(stderr, "Unable to find strlen of declared buffer\n");
	}

	char *x = "\0abc";
	char *y = "a\0bc";
	char *z = "abc\0";

	if ((strlen(x) != 0) && (strlen(y) != 1) && (strlen(z) != 3)) 
		fprintf(stderr, "Expecting strlen to stop counting at first null terminator\n");
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

char*  return_string_snprintf() {
	char buf[4];
	char *x = "abc";
	snprintf(buf, sizeof(char) * 4, "%s", x);
	char *ptr = buf;
	return ptr;
}

/**
 * Test how strings are returned from functions
 * stack vs heap
 */
void return_strings() {
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

	ret = return_string_snprintf();
	if (strcmp(ret, "abc") != 0) 
		fprintf(stderr, "failed to return snprintf buffer from function\n");

}

/**
 * Tests to see what these strings look like when printing them
 */
void printf_test() {
	printf("Null terminator char = %c\n", '\0');

	// Not null terminating this will produce weird output
	char buf[69] = {'a', 'b', 'c', '\0'};
	printf("Expect null terminated char array %s to equal abc\n", buf);

	char empty[10];
	printf("Empty char array %s\n", empty); 
	char nil[5] = {'\0'};
	printf("Expect empty null terminated char array %s to equal %c\n", nil, '\0');

	char *x = "nice";
	printf("Expect char pointer %s to equal nice\n", x);

	print_alphabet();
	print_ascii_table();

	printf("hello %s", print());
}

void print() {
	printf("world\n");
}


void char_array_test() {
	// char array
	char buf[4] = {'a', 'b', 'c', '\0'};
	printf("%s\n", buf);

	// iterate over buffer
	char *ptr = buf;
	while (*ptr != '\0') {
		printf("%c", *ptr);
		ptr++;
	}
	printf("\n");
}

/**
 * Prints values 33 - 126
 * 0-32 are non-printing characters
 * 127 is DEL character
 */
void print_ascii_table() {
	for (int i = 0; i < 128; ++i) {
		printf("%c", i);
	}
	printf("\n");
}

/**
 * Alphabet is 65-122 in ASCII table
 */
void print_alphabet() {
	for (int i = 65; i <= 122; ++i) {
		printf("%c", i);
	}
	printf("\n");
}
