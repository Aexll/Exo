#include <stdio.h>
#include <stdlib.h>
#include "h/Files.h"
#include "h/Bin.h"


LISTS* DB_storeToList(char* dbname){
    /* put values of a file in a LISTS */

    FILE* file;
    STRING* str;
    LISTS* list = NULL;

    str = Str_newEmpty();
    fopen_s(&file,dbname, "r");

    if(file){
        while(!feof(file)){

            File_fillStrTo(file, str, '\n');

            if(list){
                LISTS_add(list,str);
            }else{
                list = LISTS_new(str,NULL);
            }
        
            str = Str_newEmpty();
        }
        Str_free(str);
        fclose(file);
    }
    return list;
}

unsigned long DB_readBin(char* dbname) {
    /* get bin value of a file by his name */

    FILE* file;
    unsigned long value = -1;

    fopen_s(&file,dbname, "r");
    if (file) {
        value = File_readBin(file);
        fclose(file);
    }
    return value;
}

void DB_write(char* dbname, char* str) {
    /* write in a file a string */

    FILE* file;

    fopen_s(&file, dbname, "w");
    if (file) {
        File_write(file, str);
        fclose(file);
    }
}

STRING* DB_found(const char* databaseName, const char* key, char a, char b)
{
    /* get value in a dictionary-like file format */
    /* a is the break, b is the link */

    FILE* file = NULL;
    STRING* value = NULL;

    if (databaseName)
    {
        fopen_s(&file, databaseName, "r");
    }
    if (file)
    {

        value = Str_newEmpty();

        /* loop until we fond the first string */
        while (!feof(file) && !value->str)
        {
            if (File_stest(file, key) && fgetc(file) == b)
            {
                File_fillStrTo(file, value, a);
            }
            else
            {
                File_jump(file, a);
            }
        }

        fclose(file);
    }

    return value;
}

int DB_findInt(const char* databaseName, const char* key, char a, char b) {
    /* find int in a file after a certain patern */
    /* a is the break, b is the link */

    int nb = 0;
    STRING* tempstr;
    tempstr = DB_found(databaseName, key, a, b);
    if (tempstr) {
        nb = Str_int(tempstr);
        Str_free(tempstr);
        tempstr = NULL;
    }
    return nb;
}