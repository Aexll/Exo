#pragma once
#include "Marcos.h"
#include "Options.h"
#include "Color.h"
#include "libs/h/ListStrings.h"
#include "Reference.h"

/* initialize the game */
void init();

/* image system */
void putpixel(WINDOW* win, int x, int y, struct COLOR pcolor, int index, int mode);
void affImage(WINDOW* win, FILE* file, int x, int y, OPTION options);
void fixColor(WINDOW* win);

/* files */
void openFile(FILE** file, int index, char* ext);
void wreadFile(WINDOW** win, FILE* file, int memory[MEMSIZE], LISTS* qlist);

/* memory */
void allocMemo(FILE* file, int memory[MEMSIZE]);

/* questions */
void wputQuestion(WINDOW* win, int memory[MEMSIZE], int selected, LISTS* qlist);
void clearQuestions(LISTS* qlist);

/* menu */
void showMenu(WINDOW* win, int cursor);
int openMenu(WINDOW* win);

/* options */
int openOptions(WINDOW* win, OPTION* options);
void showOptions(WINDOW* win, OPTION* options, int cursor);
void setOption(OPTION* options, int cursor);

/* inventory */
void showInventory(WINDOW* win, int memory[MEMSIZE], LISTS* itemlist);

/* endings */
void showUnlockedEndings(WINDOW* win);

/* ending */
void showEnding(WINDOW* win, int nb);
void addEnding(int nb);

/* misc */
void updateWindow(WINDOW* win);
void wprintCentred(WINDOW* win, int cury, int curz, char c);
void closeGame(MAINREF mainRef);
void resetSave();
void showAsciiImage(WINDOW* win, int anchor, int ypos, int nb);
