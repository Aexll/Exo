#include <stdio.h>
#include <curses.h>
#include "libs/h/ListStrings.h"
#include "Reference.h"

void closeGame(MAINREF mainRef) {

	if (mainRef.qlist) LISTS_free(&mainRef.qlist);

	if (mainRef.ilist) LISTS_free(&mainRef.ilist);

	if (mainRef.endinglist) LISTS_free(&mainRef.endinglist);
	
	/* closing text file */
	if(mainRef.file.txt && *mainRef.file.txt){
		fclose(*mainRef.file.txt);
	}

	/* closing image file */
	if (mainRef.file.img && *mainRef.file.img) {
		fclose(*mainRef.file.img);
	}
	

	/* deallocates memory of the windows */
	endwin();
	delwin(mainRef.win.win_questions);
	delwin(mainRef.win.win_text);
	delwin(mainRef.win.win_img);
	delwin(mainRef.win.win_menuImg);
	delwin(mainRef.win.win_menuText);
	delwin(mainRef.win.win_menuOptions);
	delwin(mainRef.win.win_menuSuccess);
	delscreen(SP);
	printf("-end-\n");
	printf("\033[0;32mQuiting\n\033[0;37m");
	
}