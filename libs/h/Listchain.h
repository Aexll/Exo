#include <stdio.h>
#include <stdlib.h>

typedef struct{
    void* next;
    void* parent;
    char* value;
    int size;
} LIST;

void LIST_free(LIST* LIST);
LIST* LIST_new(void* value,void* parent);
LIST* LIST_add(LIST* LIST,void* value);
void* LIST_get(LIST* LIST,int i);
LIST* LIST_set(LIST* LIST,int i,void* value);
LIST* LIST_pop(LIST* LIST,int i);
void LIST_remove(LIST* LIST,int i);
void LIST_print(LIST* LIST);