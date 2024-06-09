#pragma once
#include <stdio.h>

typedef struct{
    char* str;
    size_t size; /* size doesent contain the \0 */
    size_t buffer;
}STRING;

/* create a new STRING empty */
STRING* Str_newEmpty();

/* create a new STRING containing str */
STRING* Str_new(size_t,char*);

/* free the memory taken by a STRING */
void Str_free(STRING*);

/* free the memory taken by a STRING */
char* Str_get(STRING*);

/* string->str */
void Str_set(STRING*,size_t,char*);

/* append STRING with a char and his size */
void Str_append(STRING*,size_t,char*);

/* append STRING with another STRING */
void Str_concat(STRING* stringA, STRING* stringB);

/* remove and free string->str without removing the STRING */
void Str_clean(STRING*);

/* remove the last character of the STRING (/0 dont count) */
void Str_pop(STRING*);

/* add a char at the end of the STRING (optimized) */
void Str_putchar(STRING*, char);

/* print the string and his infos for debuging */
void Str_show(STRING*);

/* return a int version of the string */
int Str_int(STRING* string);
