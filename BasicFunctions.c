#include <stdio.h>
#include "Marcos.h"
#include "BasicFunctions.h"

int isDigit(char c) {
	return ('0' <= c && c <= '9');
}

int mod(int a, int b) {

	return a < 0 ? b : (a>b ? 0 : a);
}

int logof10(int a) {

	int i = 1;
	while (a >= 10) {
		a /= 10;
		i++;
	}

	return i;

}

int nbDigit(int a) {
	/* return the number of char in a number (-1 count as two chars) */

	if (a >= 0) {
		return logof10(a);
	}
	else {
		return 1+logof10(-a);
	}
}

int CharInString(char c, char* str) {

	while (str && *str) {
		if (c == *str) return 1;
		str++;
	}
	return 0;
}

int sumUntil(FILE* file,int memory[MEMSIZE],char* cptr,char* clist) {
	/*	return the sum until a char in a string given is reach 
		return 0 by default
	*/

	int sum = 0;
	int offset = 0;
	int adress = 0;
	int increment = 0;

	if (cptr && clist) {
		while (!CharInString(*cptr, clist)) {

			if (*cptr == 'p') sum = M_IMI;
			if (*cptr == '+') increment++;
			if (*cptr == '-') increment--;
			if (*cptr == '#') adress = 1;
			if (*cptr == 'i') offset += M_INV;
			if (*cptr == 'v') offset += M_QVALID;
			if (*cptr >= '0' && *cptr <= '9') {
				sum *= 10;
				sum += *cptr - '0';
			}
			*cptr = fgetc(file);
		}
	}
	if(memory)
		return adress ? memory[sum + offset] + increment : sum + offset + increment;
	else
		return adress ? increment : sum + offset + increment;
}

int nbChar(char* str) {

	int count = 0;
	while (str && *str) {
		count++;
		str++;
	}
	return count;
}

void clamp(int* value, int min, int max) {

	if (value) {
		if (*value < min) *value = min;
		if (*value > max) *value = max;
	}
}