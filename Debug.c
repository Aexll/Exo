#include "Marcos.h"

#define MEMORY_PRINT_NB 30

void printMemory(int memory[MEMSIZE]) {

	printf("\x1b[31m");
	printf("\x1b[4m");
	printf("MEMORY                                   \n");
	printf("  0   1   2   3   4   5   6   7   8   9  ");
	printf("\x1b[0m");

	for (int i = 0; i < MEMSIZE && i < MEMORY_PRINT_NB; i++)
	{
		if (i % 10 == 0)printf("\n");
		if (memory[i] < 10)printf(" ");
		if (memory[i] < 100)printf(" ");
		if (memory[i] == 1)printf("\x1b[32m");
		if (memory[i] == 0)printf("\x1b[34m");
		printf("%d ", memory[i]);
	}
	printf("\x1b[0m\n");
}

void printQuestions(char** questions) {

	int i = 0;
	int j = 0;

	printf("\nquestions : \n");

	if (questions) {

		while (questions[i]) {

			printf("	%d: ", i);
			j = 0;
			while (questions[i][j] != '\0') {
				printf("%c", questions[i][j]);
				j++;
			}
			printf("\n");
			i++;
		}

	}
	printf("end\n");
}