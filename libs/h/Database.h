#pragma once
#include <stdio.h>
#include "Files.h"

LISTS* DB_storeToList(char*);
unsigned long DB_readBin(char*);
void DB_write(char*, char*);
STRING* DB_found(const char* databaseName, const char* key, char a, char b);
int DB_findInt(const char* databaseName, const char* key, char a, char b);