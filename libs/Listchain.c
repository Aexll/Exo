#include <stdio.h>
#include <stdlib.h>
#include "h/Listchain.h"


void LIST_free(LIST* list){
    /* free a list */

    if(list->next){
        LIST_free(list->next);
    }
    printf("Free Memory %s\n", list->value);
    free(list);
    list = NULL;
}

LIST* LIST_new(void* value,void* parent){
    /* create a new list */

    LIST* list = NULL;
    list = malloc(sizeof(LIST));

    if(list){
        list->next = NULL;
        list->size = 0;
        list->value = value;
        list->parent = parent;
    }

    return list;
}

LIST* LIST_add(LIST* list,void* value){
    /* add a new value to the list */

    LIST* newlist;

    if(list->next){
        newlist = LIST_add( (LIST*)(list->next),value);
    }else{
        newlist = LIST_new(value,list);
        list->next = newlist;
    }

    return newlist;
}

void* LIST_get(LIST* list,int i){
    /* get value at index */

    if(list){

        if(i<=0){
            return list->value;
        }else{
            return LIST_get(list->next,i-1);
        }

    }else{
        return NULL;
    }

}

LIST* LIST_set(LIST* list,int i,void* value){
    /* set value at index */

    if(list){

        if(i<=0){
            list->value = value;
        }else{
            if(!list->next){
                list->next=LIST_new(NULL,list);
            }
            return LIST_set(list->next,i-1,value);
            
        }
    }

    return list;
}

LIST* LIST_pop(LIST* list,int i){

    if(list){

        /* code */
        if(i==0){
            ((LIST*)(list->parent))->next = list->next;
            list->next = NULL;
            list->parent = NULL;
            return list;
        }else{
            return LIST_pop(list->next,i-1);
        }

    }

    return list;

}

void LIST_remove(LIST* list,int i){

    LIST_free(LIST_pop(list,i));
}

void LIST_print(LIST* list){
    
    if(list){

        printf("%p -> %s\n",list->value,list->value);
        LIST_print(list->next);
    }
}