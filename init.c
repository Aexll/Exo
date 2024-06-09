#include <stdio.h>
#include <windows.h>
#include <curses.h>

#define FULLSCREEN 1

void init() {

	/* set console in fullscreen */
	if(FULLSCREEN){
		keybd_event(VK_MENU, 0x38, 0, 0);
		keybd_event(VK_RETURN, 0x1c, 0, 0);
		keybd_event(VK_RETURN, 0x1c, KEYEVENTF_KEYUP, 0);
		keybd_event(VK_MENU, 0x38, KEYEVENTF_KEYUP, 0);
	}

	initscr();
	//cbreak();	/*		make ctrl+c quit			*/
	//raw();	/*		accept all input rawly		*/
	noecho();	/*		dont put player input		*/

	
	//test if color are usable
	if (!has_colors()) {

		printf("\033[0;31mColor not suported\033[0;37m\n");
	}
	else {
		printf("\033[0;32mColor suported !\033[0;37m\n");


		/* test if custom color are aviable */
		if (!can_change_color()) {

			printw("\033[0;31mcant change color\033[0;37m\n");
		}
		else {

			printf("\033[0;32mCan change color !\033[0;37m\n");
		}


		/* starting the color */
		start_color();
	}



	/* set visibilty of the cursos (dont work wery well, but this is all i can do) */
	curs_set(0);

	printf("-start-\n");

}