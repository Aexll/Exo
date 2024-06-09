#include <stdio.h>
#include <stdlib.h>
#include "h/String.h"

STRING* Str_newEmpty(){

    STRING* string;

    string = malloc(sizeof(STRING));

    if(string){

        string->str = NULL;
        string->size = 0;
        string->buffer = 0;
    }

    return string;
}

STRING* Str_new(size_t size,char* value){

    STRING* string;

    string = Str_newEmpty();
    Str_append(string,size,value);

    return string;
}

void Str_set(STRING* string,size_t strsize,char* str){

    Str_clean(string);
    Str_append(string,strsize,str);
}

void Str_free(STRING* string){

    if(string){
        
        if(string->str) free(string->str);
    
        free(string);
        string = NULL;
    }
}

char* Str_get(STRING* string){

    if(string) return string->str;
    
    return NULL;
}

void Str_append(STRING* string,size_t strsize,char* str){

    int i = 0;
    size_t newsize;
    char* tempbuffer;

    if(string){

        if(str){
            
            if(string->buffer<=strsize){

                string->buffer += strsize + 1;
            }
            
            newsize = string->size + strsize;
            
            tempbuffer = NULL;
            tempbuffer = realloc(string->str, string->buffer  * sizeof(char));
            if (tempbuffer)
            {
                string->str = tempbuffer;
            }
            
            while(string->size+i < newsize && str[i]!='\0'){
                
                string->str[string->size+i] = str[i];
                i++;
            }
    
            /* shrink memory to fit the size of the string */
            tempbuffer = NULL;
            tempbuffer = realloc(string->str, string->size+i+1  * sizeof(char));
            if (tempbuffer)
            {
                string->str = tempbuffer;
                string->buffer = string->size+i+1;
            }
    
            /* set the last char to be \0 */
            string->str[string->size+i] = '\0';
            /*printf("%ld -> %c\n",string->size+i,'#');*/ /* debug */
            /* set the STRING size */
            string->size = string->size+i;
        }
    }
}

void Str_clean(STRING* string){

    if(string){

        if(string->str){

            free(string->str);
            string->str = NULL;
            string->size = 0;
        }
    }
}

void Str_pop(STRING* string){
    /* remove the last char before the \0 */

    string->str[string->size-1]='\0';
    string->size-=1;

}

void Str_concat(STRING* stringA,STRING* stringB){

    Str_append(stringA,stringB->size,stringB->str);
}

void Str_putchar(STRING* string, char c) {

    char* tempbuffer;

    if (string) {

        /* test if the allocated buffer is enought, else expand it*/
        if (string->str) {
            if (string->size >= string->buffer - 1) {
                string->buffer += 20;
                tempbuffer = realloc(string->str, string->buffer * sizeof(char));
                if (tempbuffer) {
                    string->str = tempbuffer;
                }
            }
        }
        else {
            string->str = malloc(10 * sizeof(char));
            string->buffer += 10;
        }

        /* put the new char */
        if (string->str) {

            string->str[string->size] = c;
            string->str[string->size + 1] = '\0';
            string->size += 1;
        }
    }
}

void Str_show(STRING* string){
    /* show details of a STRING */

    if (string)
    {

        printf("\033[0;30m________________________________________\033[0;37m\n");
        if(string->str) printf("\033[0;36mstr\033[0;37m    -> %s\n",string->str);
        else printf("\033[0;36mstr\033[0;37m    -> \033[0;34mNULL\033[0;37m\n");
        printf("\033[0;32msize\033[0;37m   -> %ld\n",string->size);
        printf("\033[0;34mbuffer\033[0;37m -> %ld\n",string->buffer);
    }
}

int Str_int(STRING* string) {

    int nb = 0;
    int i = 0;
    if (string) {
        for (i = 0; i < string->size; i++) {

            if (string->str[i] >= '0' && string->str[i] <= '9') {
                nb *= 10;
                nb += string->str[i] - '0';
            }
        }
    }
    return nb;
}