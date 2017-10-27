#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#define BUF_SIZE 1000
#if !defined TRUE && FALSE
#define TRUE 1
#define FALSE 0
#endif

int main(int argc, char *argv[]) {
	FILE *ptr_rules;
	FILE *ptr_keywords;
	char line_buf[BUF_SIZE];

	if (argc != 2) {
		printf("\nIt takes one argument that is the rulebase file\n");
		return 1;
	}
	ptr_keywords = fopen("keywords", "r");
	if (!ptr_keywords) {
		printf("\nFile 'keywords' must be in the same directory\n");
		return 1;
	}

	ptr_rules = fopen(argv[1], "r");
	if (!ptr_rules) {
		printf("\nRule file cannot be opened. Check if it's in same directory\n");
		return 1;
	}

	while (fgets (line_buf, BUF_SIZE, ptr_rules) != NULL) {
		printf("%s", line_buf);
	}
	fclose(ptr_keywords);
	fclose(ptr_rules);

	return 0;
}
