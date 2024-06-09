#include <stdio.h>
#include <stdlib.h>
#include "h/Files.h"

void File_jump(FILE* file, char to)
{
    /* jump to the next to */

    while (!feof(file) && fgetc(file) != to);
}

long File_Cur_get(FILE *file){

    return ftell(file);
}

void File_Cur_set(FILE* file,long pos){

    fseek(file, pos, SEEK_SET);
}

unsigned long File_readBin(FILE* file) {

    char c;
    unsigned long value = 0;

    while ((c = fgetc(file)) != EOF) {
        if (c == '0' || c == '1') {
            value <<= 1;
            value += (c == '1');
        }
    }
    return value;
}

void File_write(FILE* file, char* str) {

    if (file && str) fprintf(file, "%s", str);
}

void File_fillStrTo(FILE* file, STRING* string, char to)
{
    /* fill string with files char until char is == to */
    char c;
    while (!feof(file) && (c = fgetc(file)) != to) {
        Str_putchar(string, c);
    }
}

int File_stest(FILE* file, const char* string)
{

    if (file && string) {

        while (!feof(file) && fgetc(file) == *string) {

            string++;
            if (*string == '\0') {

                return 1;
            }
        }
    }
    return 0;
}

/* 
ftell = return cursor pos
fseek = set cursor pos from given pos
rewind = set cursor pos at start
*/

