#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "String.h"

typedef struct LISTS{
    struct LISTS* next;
    struct LISTS* parent;
    STRING* value;
    int size;
} LISTS;

void LISTS_show(LISTS* list);
void LISTS_free(LISTS** LISTS);
LISTS* LISTS_new(STRING* value,LISTS* parent);
LISTS* LISTS_add(LISTS* LISTS,STRING* value);
STRING* LISTS_get(LISTS* LISTS,int i);
LISTS* LISTS_set(LISTS* LISTS,int i,STRING* value);
LISTS* LISTS_pop(LISTS** LISTS,int i);
void LISTS_remove(LISTS** LISTS,int i);
void LISTS_print(LISTS* LISTS);
void LISTS_debugPrint(LISTS* list);
unsigned LISTS_length(LISTS* list);
void LISTS_empty(LISTS* list);