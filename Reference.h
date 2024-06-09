#pragma once
#include <curses.h>
#include <stdlib.h>
#include "libs/h/ListStrings.h"


typedef struct {

	WINDOW* win_questions;
	WINDOW* win_text;
	WINDOW* win_img;
	WINDOW* win_menuImg;
	WINDOW* win_menuText;
	WINDOW* win_menuOptions;
	WINDOW* win_menuSuccess;

}
WINREFS;

typedef struct {

	FILE** txt;
	FILE** img;

}
FILEREFS;

typedef struct {

	WINREFS win;
	FILEREFS file;
	LISTS* qlist;
	LISTS* ilist;
	LISTS* endinglist;

}
MAINREF;