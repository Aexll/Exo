#pragma once
#include <stdio.h>
#include "Marcos.h"

int isDigit(char a);
int mod(int a, int b);
int logof10(int a);
int nbDigit(int a);
int sumUntil(FILE* file, int memory[MEMSIZE], char* cptr, char* c);
int CharInString(char c, char* str);
int nbChar(char* str);
void clamp(int* value, int min, int max);