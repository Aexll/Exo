//#define _CRTDBG_MAP_ALLOC

// /* uncoments to test for memory leaks */
//#include <crtdbg.h>

#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include "Filesystem.h"
#include "Marcos.h"
#include "Color.h"
#include "libs/h/Database.h"
#include "BasicFunctions.h"
#include "Debuging.h"
#include "Reference.h"

int main() {
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF); // debug

	/* init */
	init();

	/* set blink to false */
	PDC_set_blink(FALSE);

	/* loading time */
	napms(100);


	/* __________ INIT VARIABLES __________ */

	/* windows size */
	resize_term(2000, 2000);
	int xmax = 2000, ymax = 2000;
	getmaxyx(stdscr, ymax, xmax);
	resize_term(ymax, xmax);

	/* define constant based on the actual screen size */
	const int y40	= ymax/2 + ymax/9;
	const int y50	= ymax - ymax / 8 - ymax / 7;
	const int y25	= y50 / 2;
	const int y8	= ymax / 8;
	const int y4	= ymax / 16;
	const int x2	= xmax / 120;
	const int x4	= xmax / 60;
	const int x8	= xmax / 30;
	const int x16	= xmax / 15;
	
	/* loading time */
	napms(100);

	/* main references, contening all varaible to free at the end */
	MAINREF mainRef;

	/* option */
	OPTION gameOptions;
	gameOptions.asciimages = 0;
	gameOptions.brightness = 250;

	/* windows */
	WINDOW* win_questions = newwin(		ymax / 3	, (xmax / 2) - x8	, ymax - y25	, (xmax / 2) + x4	);//questions windows
	WINDOW* win_text = newwin(			ymax / 3	, (xmax / 2)		, ymax - y25	, x4				);//text windows
	WINDOW* win_img = newwin(			y40			, xmax - x8			, 2				, x4				);//image windows
	WINDOW* win_menuImg = newwin(		ymax - 2	, xmax - 4			, 1				, 2					);//menu windows
	WINDOW* win_menuText = newwin(		y8			, x16+1				, ymax / 2 - y4	, xmax / 2 - x8		);//menu text windows
	WINDOW* win_menuOptions = newwin(	y8			, x16+1				, ymax / 2 - y4	, xmax / 2 - x8		);//menu options windows
	WINDOW* win_menuSuccess = newwin(	y8*2		, x16*8				, ymax/2 - y4*2	, xmax / 2 - x16*4	);//menu options windows
	WINREFS winowsReferences;
	winowsReferences.win_questions = win_questions;
	winowsReferences.win_text = win_text;
	winowsReferences.win_img = win_img;
	winowsReferences.win_menuImg = win_menuImg;
	winowsReferences.win_menuText = win_menuText;
	winowsReferences.win_menuOptions = win_menuOptions;
	winowsReferences.win_menuSuccess = win_menuSuccess;
	mainRef.win = winowsReferences;


	/* files */
	FILE* txtFile = NULL;
	FILE* imgFile = NULL;
	FILEREFS filesRefs;
	filesRefs.txt = &txtFile;
	filesRefs.img = &imgFile;
	mainRef.file = filesRefs;

	/* memory */
	int memory[MEMSIZE];
	memset(memory, 0, sizeof(int) * MEMSIZE);
	memory[M_END] = 0;
	
	/* questions */
	LISTS* qlist;
	qlist = LISTS_new(NULL, NULL);
	mainRef.qlist = qlist;

	/* item list */
	LISTS* itemlist;
	itemlist = DB_storeToList("files/items");
	mainRef.ilist = itemlist;

	/* endings list */
	LISTS* endinglist = NULL;
	endinglist = DB_storeToList("files/endings");
	mainRef.endinglist = endinglist;

	int cursor = 0;
	int inputLoop = 0;
	int mainGameLoop = 0;
	int userInput = 0;

	refresh();

	nodelay(win_menuText, TRUE);	//
	keypad(win_menuText, TRUE);		//
	nodelay(win_questions, TRUE);	//
	keypad(win_questions, TRUE);	//make the keyboard arrows usable

	updateWindow(win_questions);
	updateWindow(win_text);
	updateWindow(win_img);

	


	/* __________ INIT COLORS __________ */
	init_pair(COLORPAIR_W, COLOR_WHITE, COLOR_BLACK);
	init_pair(COLORPAIR_R, COLOR_RED, COLOR_BLACK);
	init_pair(COLORPAIR_B, COLOR_BLUE, COLOR_BLACK);
	init_pair(COLORPAIR_G, COLOR_GREEN, COLOR_BLACK);
	init_pair(COLORPAIR_Y, COLOR_YELLOW, COLOR_BLACK);
	init_color(12, COLOR_L);
	init_pair(COLORPAIR_L, 12, 0);

	const int asc_title = DB_findInt(GAMEINFO, ASC_TITLE, INFO_SEPARATOR, INFO_LINKING);
	const int asc_option = DB_findInt(GAMEINFO, ASC_OPTION, INFO_SEPARATOR, INFO_LINKING);
	const int asc_endings = DB_findInt(GAMEINFO, ASC_ENDINGS, INFO_SEPARATOR, INFO_LINKING);


	/* __________ MENU __________ */

	while(1){

		/* show endings ? memory[M_END] contain (the ending to show)+1 and 0 will return to the menu without endings */
		if (memory[M_END]) {
			showEnding(win_menuImg, memory[M_END]-1);
			/* wait till enter is pressed */
			while (wgetch(win_menuText) != '\n');
		}
		memory[M_END] = 0;
		mainGameLoop = 0;

		/* set the first file to be open */
		cursor = 0;
		memory[cursor] = 0;
	
		/* show the main menu */
		showMenu(win_menuText,cursor);
		updateWindow(win_menuImg);
		updateWindow(win_menuText);

		/* menu loop */
		int menuState = 0;
		do
		{
			/* show the main menu ascii image */
			wclear(win_menuImg);
			showAsciiImage(win_menuImg, 0, 5, asc_title);

			/* get what button the user has chosen */
			menuState = openMenu(win_menuText);
			switch(menuState){
			case 0:
				/* start game */
				break;
			case 1:
				/* open options */
				wclear(win_menuImg);
				showAsciiImage(win_menuImg, 0, 10, asc_option);
				openOptions(win_menuText, &gameOptions);
				break;
			case 2:
				/* open success */
				wclear(win_menuImg);
				showAsciiImage(win_menuImg, 0, 10, asc_endings);
				showUnlockedEndings(win_menuSuccess);
				while (wgetch(win_menuText) != '\n');
				wclear(win_menuSuccess);
				wrefresh(win_menuSuccess);
				break;
			case 3:
				/* close game */
				closeGame(mainRef);
				return 0; 
			}
		} while (menuState!=0);

	

		/* __________ GAME __________ */

		/* hide the menu box */
		wclear(win_menuImg);
		wrefresh(win_menuImg);

		/*
		for (int i = 0; i < 3; i++) {
			flash();
			napms(100);
		}
		*/

		/* draw img windows borders */
		updateWindow(win_img);

		memory[M_IMI] = FIRSTIMAGE;


		while(!mainGameLoop){

			inputLoop = 0;

		
			/* clear the used questions memory */
			wclear(win_questions);
			clearQuestions(qlist);
			updateWindow(win_questions);

			memory[M_END] = 0;

			/* open and read file */
			openFile(&txtFile, memory[cursor], EXT_TXT);
			wreadFile(&win_text, txtFile, memory, qlist);

			/* if we reach a end */
			if (memory[M_END]) {
				inputLoop = 1;
				mainGameLoop = 1;
			}

			/* show image */
			openFile(&imgFile, memory[M_IMI], EXT_IMG);
			affImage(win_img, imgFile, 0, 0, gameOptions);

			/* it dont mork anymore :( */
			fixColor(win_img);

			/* show questions */
			cursor = 0;
			wputQuestion(win_questions, memory, cursor, qlist);  
			updateWindow(win_questions);

			/* debug */
			//printMemory(memory);


	
			/* input system */
			while(!inputLoop){

				userInput = wgetch(win_questions);

				int cursmov = 0;
				if (userInput == KEY_UP) cursmov = -1;
				if (userInput == KEY_DOWN) cursmov = 1;
				if (userInput == KEY_RIGHT) cursor = -1;
				if (userInput == KEY_LEFT) cursor = 0;

				switch(userInput) {

				
				case KEY_UP:
				case KEY_DOWN:

					/* move the cursor */
					if (cursor >= 0) {

						/* dont move the cursos over unvalid choices */
						const qlistSize = LISTS_length(qlist);
						int i = 0;
						do {
							cursor = mod(cursor + cursmov, qlistSize - 1);
							i++;
						} while (!memory[M_QVALID + cursor] && i < qlistSize + 1);

					}else{

						if (cursor == -1 && cursmov == -1) cursor = -3;
						else if (cursor == -3 && cursmov == 1) cursor = -1;
						else cursor -= cursmov;
					}

				case KEY_RIGHT:
				case KEY_LEFT:

					/* update question windows */
					wputQuestion(win_questions, memory, cursor, qlist);
					updateWindow(win_questions);
					break;

				case '\n':

					if (cursor>=0) {
						inputLoop = 1;
					}
					else {
						if (cursor == -1) {

							case 'i':
							/* inventory */

							wclear(win_questions);
							showInventory(win_questions, memory, itemlist);
							while ( wgetch(win_questions) != '\n');
							wclear(win_questions);
							wputQuestion(win_questions, memory, cursor, qlist);
							updateWindow(win_questions);
						}
						else if (cursor == -2) {

							case 'm':
							/* menu  */

							inputLoop = 1;
							mainGameLoop = 1;
						}
						else {

							case 'q':
							/* quit  */

							wattron(win_questions, A_REVERSE);
							mvwprintw(win_questions, 6, 90, "[q] really ?  ");
							wattroff(win_questions, A_REVERSE);
							mvwprintw(win_questions, 7, 90, "   the game   ");
							mvwprintw(win_questions, 8, 90, "  wont save ! ");
							updateWindow(win_questions);

							/* wait for player to confirm his input */
							int temp_loop = 1;
							while (temp_loop) {
								userInput = wgetch(win_questions);
								if (userInput == '\n' || userInput == 'q') {
									closeGame(mainRef);
									return 0;
								}
								if (userInput == KEY_UP || userInput == KEY_DOWN || userInput == KEY_RIGHT || userInput == KEY_LEFT) {
									temp_loop = 0;
								}
							}

							wmove(win_questions, 6, 90);
							wattron(win_questions, A_REVERSE);
							wprintw(win_questions, "[q] quit    ");
							wattroff(win_questions, A_REVERSE);
							updateWindow(win_questions);
						}
					}
					break;
				}
			}
		}
	}

	closeGame(mainRef);
	return 0;
}