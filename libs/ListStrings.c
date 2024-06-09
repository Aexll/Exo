#include <stdio.h>
#include <stdlib.h>
#include "h/ListStrings.h"

#define DEBUG 0
#define ISDB if(DEBUG)


void LISTS_free(LISTS** list){
    /* free a list */

    LISTS* valist = NULL;
    if(list) valist = *list;

    if(valist){

        ISDB printf("\033[0;32mFreeing: \033[0;37m");
        ISDB LISTS_show(valist);
        LISTS_free(&valist->next);
        if(valist->value){
            Str_free(valist->value);
        }
        free(valist);
    }
    list = NULL;
}

LISTS* LISTS_new(STRING* value,LISTS* parent){
    /* create a new list */

    LISTS* list = NULL;
    list = malloc(sizeof(LISTS));

    if(list){
        list->next = NULL;
        list->size = 0;
        list->value = value;
        list->parent = parent;
    }

    return list;
}

LISTS* LISTS_add(LISTS* list,STRING* value){
    /* add a new value to the list */

    LISTS* newlist;
    newlist = NULL;

    if(list){
        if(list->next){
            newlist = LISTS_add(list->next,value);
        }else{
            newlist = LISTS_new(value,list);
            list->next = newlist;
        }
    }

    return newlist;
}

STRING* LISTS_get(LISTS* list,int i){
    /* get value at index */

    if(list){

        if(i<=0){
            return list->value;
        }else{
            return LISTS_get(list->next,i-1);
        }

    }else{
        return NULL;
    }

}

LISTS* LISTS_set(LISTS* list,int i,STRING* value){
    /* set value at index */

    if(list){

        if(i<=0){
            list->value = value;
        }else{
            if(!list->next){
                list->next=LISTS_new(NULL,list);
            }
            return LISTS_set(list->next,i-1,value);
            
        }
    }

    return list;
}

LISTS* LISTS_pop(LISTS** list,int i){
    /* remove from list and return the list at index i */

    LISTS* deflist = NULL;
    if(list) deflist = *list;


    if(deflist){

        /* if i is out of range, remove the last index */
        if(!deflist->next) i=0;

        /* code */
        if(i==0){

            
            ISDB printf("\n");


            if(deflist->parent){
                if(deflist->next){
                    deflist->parent->next = deflist->next;
                    deflist->next->parent = deflist->parent;
                }else{
                    deflist->parent->next = NULL;
                }
            }else{
                if(deflist->next){
                    deflist->parent = NULL;
                    deflist->next->parent = NULL;
                    *list = deflist->next;
                }else{
                    *list = NULL;
                    ISDB printf("Alone ");
                }
            }

            ISDB printf("Poping : ");
            ISDB LISTS_show(deflist);

            /*
            if(list->parent){

                if(list->next){
                    list->parent->next = list->next;
                }else{
                    list->parent->next = NULL;
                }
                

            }else{

                
                oldlist = list;
                list = list->next;
            }
            */

            deflist->next = NULL;
            deflist->parent = NULL;

            return deflist;

        }else{
            return LISTS_pop(&deflist->next,i-1);
        }

    }else{
        ISDB printf("list is null\n");
    }

    return deflist;

}

void LISTS_remove(LISTS** list,int i){
    /* pop then free a list index i */

    LISTS* topop;
    topop = LISTS_pop(list,i);
    LISTS_free(&topop);
}

unsigned LISTS_length(LISTS* list){
    /* return LISTS length */

    if(list){ 
        return 1+LISTS_length(list->next);
    }

    return 0;
}

void LISTS_empty(LISTS* list) {
    /* free the list excepte himself */

    if (list) {

        LISTS_free(&list->next);
        list->next = NULL;
        list->size = 0;
        Str_free(list->value);
        list->value = NULL;
    }
}

void LISTS_print(LISTS* list) {
    /* print the list with beautiful colors */

    if (list) {

        if (list->parent) printf("\033[0;32m>\033[0;37m");
        else printf("\033[0;33mx-\033[0;37m");
        if (list->value && list->value->str) printf("[%s]", list->value->str);
        else printf("[\033[0;34mNULL\033[0;37m]");
        if (list->next) printf("\033[0;32m-\033[0;37m");
        else printf("\033[0;33m-x\033[0;37m");
        LISTS_print(list->next);
    }
}

void LISTS_debugPrint(LISTS* list) {
    /* print the list with beautiful colors and more details */

    printf("\033[0;35m\033[0;37m");
    if (list) LISTS_print(list);
    else printf("\033[0;34mNULL\033[0;37m");
    printf("\033[0;35m\n\033[0;37m");
}

void LISTS_show(LISTS* list) {
    /* print only one list block */

    printf("\033[0;30m \033[0;37m");
    if (list) {
        if (list->parent) {
            printf("\033[0;30m[%s]\033[0;37m", list->parent->value->str);
            printf("\033[0;32m->\033[0;37m");
        }
        else {
            printf("\033[0;30m[/]\033[0;37m");
            printf("\033[0;33mx-\033[0;37m");
        }

        printf("\033[0;37m[%s]\033[0;37m", list->value->str);

        if (list->next) {
            printf("\033[0;32m->\033[0;37m");
            printf("\033[0;30m[%s]\033[0;37m", list->next->value->str);
        }
        else {
            printf("\033[0;33m-x\033[0;37m");
            printf("\033[0;30m[/]\033[0;37m");
        }
    }
    else {
        printf("\033[0;31mnull list\033[0;37m");
    }
    printf("\033[0;30m \033[0;37m");
    printf("\n");
}