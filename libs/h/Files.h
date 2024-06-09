#pragma once
#include "ListStrings.h"

void File_jump(FILE*, char to);
void File_Cur_set(FILE*,long);
unsigned long File_readBin(FILE*);
void File_write(FILE*, char*);
void File_fillStrTo(FILE* file, STRING* string, char to);
void File_fillStrTo(FILE* file, STRING* string, char to);
int File_stest(FILE* file, const char* string);