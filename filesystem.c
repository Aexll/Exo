#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curses.h>
#include "Filesystem.h"
#include "Marcos.h"
#include "Color.h"
#include "Options.h"
#include "libs/h/Bin.h"
#include "libs/h/Database.h"

#define C_COMMENT '!'
#define C_QUESTION '#'
#define C_ATTRIBUTE '$'

/* ifs */
#define IF_DO 2
#define IF_SKIP 1
#define IF_NO 0

/* anchor */
#define ANCHOR_UP 0
#define ANCHOR_DOWN 1

/* test if the file is not finished */
#define FEOFF (!feof(file))

void openFile(FILE** file,int index,char* ext) {
	/* open file named with index and an extention */

	unsigned nameSize = 0;
	char* fileName;
	STRING* repertory = NULL;

	/* close previous files */
	if (file && *file) fclose(*file);
	if (file) *file = NULL;

	if (ext[0] == 't') {
		repertory = DB_found(GAMEINFO, FILE_TXTFOLDER, INFO_SEPARATOR, INFO_LINKING);
	}
	else if (ext[0] == 'i') {
		repertory = DB_found(GAMEINFO, FILE_IMGFOLDER, INFO_SEPARATOR, INFO_LINKING);
	}
	else if (ext[0] == 'e') {
		repertory = DB_found(GAMEINFO, FILE_ENDFOLDER, INFO_SEPARATOR, INFO_LINKING);
	}
	else if (ext[0] == 'a') {
		repertory = DB_found(GAMEINFO, FILE_ASCFOLDER, INFO_SEPARATOR, INFO_LINKING);
	}
	if (repertory && repertory->str) {

		nameSize = repertory->size + nbDigit(index) + 8;
		fileName = malloc(nameSize * sizeof(char));

		if (fileName) {

			sprintf_s(fileName, nameSize - 1, "%s/%d.%s", repertory->str, index, ext);
			if (file){
				fopen_s(file, fileName, "r");

				/* if the file didnt opened correctly, open the error file */
				if (!(file && *file)){

					printf("\033[0;31mfile failed to open\033[0;37m\n");

					/* get file error emplacement */
					Str_free(repertory);
					repertory = DB_found(GAMEINFO, FILE_ERROR, INFO_SEPARATOR, INFO_LINKING);

					nameSize = repertory->size + 7;

					/* open error file */
					free(fileName);
					fileName = malloc(nameSize * sizeof(char));
					if (fileName){
						sprintf_s(fileName, nameSize - 1, "%s.%s", repertory->str, ext);
						fopen_s(file, fileName, "r");

						/* print if error file is valid */
						if (file && *file) 
							printf("\033[0;32m-> error file opened\033[0;37m\n");
						else printf("\033[0;31m-> error file cant open !\033[0;37m\n");

					}else printf("\033[0;31mmalloc error\033[0;37m\n");
				}
			}
			else printf("\033[0;31mUnvalid file pointer\033[0;37m\n");
			free(fileName);

		}else printf("\033[0;31mmalloc error\033[0;37m\n");

	}
	else printf("\033[0;31mgame option dont have 'txt folder' value or 'img folder' value \033[0;37m\n");

	Str_free(repertory);
	repertory = NULL;
}

void wreadFile(WINDOW** win, FILE* file,int memory[MEMSIZE], LISTS* qlist) {
	/* read the file content and do action based on the content */

	char c;

	int xmax;
	int ymax;
	getmaxyx(*win, ymax, xmax);

	STRING* squestion = NULL;


	int sleepTime = 100;

	if (file) {

		
		//clear the windows content
		wclear(*win);
		//draw the box of the windows
		box(*win, 0, 0);
		//set cusor at the begining of the windows
		wmove(*win, 2, 2);

		//read loop
		while (FEOFF) {
			c = fgetc(file);

			switch (c) {

			/* comment */
			case C_COMMENT:

				while (FEOFF && fgetc(file) != C_COMMENT);
				break;


			/* question */
			case '#':

				/* get question index */
				if (FEOFF) c = fgetc(file);

				int sum = sumUntil(file, memory, &c, "#");

				/* create new question */
				squestion = Str_newEmpty();

				/* fill question string */
				File_fillStrTo(file, squestion, '\n');

				/* add question to the list */
				LISTS_set(qlist, sum, squestion);
				
				/* debug */
				//LISTS_print(qlist);

				break;

			/* backline */
			case '\n':
				wmove(*win, getcury(*win) + 1, 2);
				break;

			/* attribute */
			case C_ATTRIBUTE:

				if (FEOFF) c = fgetc(file);

				switch (c) {

				case 'u':
					wattron(*win, A_UNDERLINE);
					break;
				case 'r':
					wattron(*win, COLOR_PAIR(COLORPAIR_R));
					break;
				case 'g':
					wattron(*win, COLOR_PAIR(COLORPAIR_G));
					break;
				case 'b':
					wattron(*win, COLOR_PAIR(COLORPAIR_B));
					break;
				case 'y':
					wattron(*win, COLOR_PAIR(COLORPAIR_Y));
					break;

				}
				break;

			/* Sleeping time */
			case '<':
				sleepTime = 0;
				while (FEOFF && (c = fgetc(file)) != '>') {

					if (isDigit(c)) {

						sleepTime *= 10;
						sleepTime += c - '0';
					} 
				}
				napms(sleepTime);
				break;

			/* Quik sleep */
			case '>':
				napms(sleepTime);
				break;


			/* end */
			case '/':
				if (FEOFF) c = fgetc(file);

				/* attribut */
				if (c == C_ATTRIBUTE) {
					if (FEOFF) c = fgetc(file);

					switch (c) {

					case 'u':
						wattroff(*win, A_UNDERLINE);
						break;
					case 'r':
						wattroff(*win, COLOR_PAIR(COLORPAIR_R));
						break;
					case 'g':
						wattroff(*win, COLOR_PAIR(COLORPAIR_G));
						break;
					case 'b':
						wattroff(*win, COLOR_PAIR(COLORPAIR_B));
						break;
					case 'y':
						wattroff(*win, COLOR_PAIR(COLORPAIR_Y));
						break;

					}
					break;
				}


			/* force char */
			case '\\':
				if (FEOFF) c = fgetc(file);
				wprintw(*win, "%c", c);
				wrefresh(*win);
				break;

			/* command */
			case '|':
				if (FEOFF) c = fgetc(file);
				switch (c) {

				/* set memory */
				case '|':
					allocMemo(file,memory);
					break;

				}
				break;
			
			/* normal caracter */
			default:

				/* jump line if the text is too big */
				if(getcurx(*win) >= xmax-2) wmove(*win, getcury(*win) + 1, 2);
				wprintw(*win, "%c", c);
				wrefresh(*win);
				break;
			
			
			}
			/**/
		}
		/* after reading the file */
	}
	//if the file pointer is unvalid
	else printf("Unvalid File\n");
}

void wputQuestion(WINDOW* win, int memory[MEMSIZE], int selected, LISTS* qlist) {
	/* print questions to a windows, and highlight the selected one */

	int wsx, wsy;
	getmaxyx(win, wsy, wsx);

	/* adjuste position depending on the screen size */
	const int x90 = wsx - 22;
	
	wclear(win);
	box(win, 0, 0);
	wmove(win, 0, 0);

	int cptr = 0;
	while (LISTS_get(qlist, cptr)) {

		wmove(win, 2 + cptr*2, 4);

		if (cptr == selected) wattron(win, A_REVERSE);

		/* set the question in grey or yellow */
		if (!memory[M_QVALID + cptr]) wattron(win, COLOR_PAIR(COLORPAIR_L));
		if (memory[M_QVALID + cptr] == 2) wattron(win, COLOR_PAIR(COLORPAIR_Y));

		wprintw(win, "[%d] %s", cptr+1, LISTS_get(qlist, cptr)->str);

		if (memory[M_QVALID + cptr] == 2) wattroff(win, COLOR_PAIR(COLORPAIR_Y));
		if (!memory[M_QVALID + cptr]) wattroff(win, COLOR_PAIR(COLORPAIR_L));

		if (cptr == selected) wattroff(win, A_REVERSE);
		cptr++;

	}

	if (-1 == selected) wattron(win, A_REVERSE);
	mvwprintw(win, 2, x90, "[i] inventory ");
	if (-1 == selected) wattroff(win, A_REVERSE);

	if (-2 == selected) wattron(win, A_REVERSE);
	mvwprintw(win, 4, x90, "[m] menu      ");
	if (-2 == selected) wattroff(win, A_REVERSE);

	if (-3 == selected) wattron(win, A_REVERSE);
	mvwprintw(win, 6, x90, "[q] quit      ");
	if (-3 == selected) wattroff(win, A_REVERSE);

	/* print |s */
	for(cptr = 0; cptr < wsy-1; cptr++) {
		mvwprintw(win, cptr, x90-8, "%c",179);
	}

	wrefresh(win);

}

void allocMemo(FILE* file,int memory[MEMSIZE]) {
	/* define wich question open what file */
	/* set the image */
	/* say if we reach a ending */
	/* add items to inventory */
	/* goes trought ifs contions */

	char c;
	int sum, ifs, oldvalue, comparaison;
	comparaison = oldvalue = ifs = sum = 0;
	char temp;
	memset(memory, 0, sizeof(int));

	/* set all question to be accessible (1 : accessible, 2 = you have to item require, 0 = blocked) */
	for (int i = M_QVALID; i < M_IMI; i++) {
		memory[i] = 1;
	}

	if(file){
		while (!feof(file)) {

			c = fgetc(file);

			switch (ifs) {

			/* go in the if code */
			case IF_DO:

				if (c == '}') ifs = 0;

			/* normal code */
			case IF_NO:


				switch (c) {

				case '?':
					/* if statement */

					oldvalue = sumUntil(file, memory, &c, "=><!");
					if (c == '=') comparaison = 0;
					if (c == '>') comparaison = 1;
					if (c == '<') comparaison = 2;
					if (c == '!') comparaison = 3;
					c = fgetc(file);
					sum = sumUntil(file, memory, &c, "{");

					/* comapre oldvalue to sum with comparaison */
					/* and set if we sould read whats in the if */
					switch (comparaison)
					{
					case 0:
						if (oldvalue == sum) ifs = IF_DO;
						else ifs = IF_SKIP;
						break;
					case 1:
						if (oldvalue > sum) ifs = IF_DO;
						else ifs = IF_SKIP;
						break;
					case 2:
						if (oldvalue < sum) ifs = IF_DO;
						else ifs = IF_SKIP;
						break;
					case 3:
						if (oldvalue != sum) ifs = IF_DO;
						else ifs = IF_SKIP;
						break;
					}
					break;
					/* end ? */

				case '[':
					/* set a memory value */

					oldvalue = sumUntil(file, memory, &c, "<+-");
					/* temp are the symbole <, +, - */
					temp = c;

					/* clamp oldvalue to memory size */
					oldvalue = oldvalue >= MEMSIZE ? MEMSIZE - 1 : oldvalue;

					c = fgetc(file);
					sum = sumUntil(file, memory, &c, "]");

					switch (temp)
					{
					case '<':
						memory[oldvalue] = sum;
						break;
					case '+':
						memory[oldvalue] += sum;
						break;
					case '-':
						memory[oldvalue] -= sum;
						break;
					}
					break;
					/* end [ */

				case '@':
					/* show ending */

					c = fgetc(file);
					sum = sumUntil(file, memory, &c, "@");
					addEnding(sum);
					memory[M_END] = sum+1;

					break;

				}
				break;

			/* dont go in the if code */
			case IF_SKIP:

				if (c == '}') ifs = IF_NO;
				break;

			}




		}
	}

	/* debug */
	//printMemory(memory);
}

void clearQuestions(LISTS* qlist) {
	/* self explaining */

	LISTS_empty(qlist);
}

void updateWindow(WINDOW* win) {
	/* refresh the windows with his box */

	box(win, 0, 0);
	wrefresh(win);
}

void wprintCentred(WINDOW* win,int cury,int curx,char c) {
	/* outdated, no longer used anymore */
	/* was used to print text in the text adjusted */
	/* it was working tho */

	for (int i = 1; i < curx; i++)
	{

		//wmove(win_img,1, i);
		mvwprintw(win, cury, i, "%c", mvwinch(win, cury, i + 1));
		wrefresh(win);

		//________:a
		//_______a:a
		//_______a:aa
		//______aa:aa
		//______aa:aaa
		//_____aaa:aaa
		//_____aaa:aaaa
		//____aaaa:aaaa
		//____aaaa:aaaaa
		//___aaaaa:aaaaa
		//___aaaaa:aaaaaa

		//print
		//print <<

	}
	mvwprintw(win, cury, curx, "%c", c);
	wrefresh(win);

}

int openMenu(WINDOW* win) {
	/* loop until the player make a choice then return the player's choice */

	int loop = 0;
	int cursor = 0;

	showMenu(win, cursor);
	updateWindow(win);
	
	while (!loop) {
		int c = wgetch(win);
		switch (c) {

		case KEY_UP:
			cursor = mod(cursor - 1, 3);
			showMenu(win, cursor);
			break;

		case KEY_DOWN:
			cursor = mod(cursor + 1, 3);
			showMenu(win, cursor);
			break;

		case '\n':
			switch (cursor) {
			case 0:
				loop = 1;
				return 0;
			case 1:
				return 1;
			case 2:
				return 2;
			case 3:
				return 3;
			}
			break;
		}
	}
	return 0;
}

void showMenu(WINDOW* win,int cursor) {
	/* show the menu and highlight the cursor's choice */

	wclear(win);

	int ymax = 0;
	int xmax = 0;
	getmaxyx(win, ymax, xmax);

	
	if (cursor == 0) wattron(win, A_BLINK);
	mvwprintw(win, 1, 1, "     Start     ");
	if (cursor == 0) wattroff(win, A_BLINK);

	wattron(win, A_ITALIC);
	if (cursor == 1) wattron(win, A_BLINK);
	mvwprintw(win, 2, 1, "    Options    ");
	if (cursor == 1) wattroff(win, A_BLINK);
	wattroff(win, A_ITALIC);

	if (cursor == 2) wattron(win, A_BLINK);
	mvwprintw(win, 3, 1, "    Endings    ");
	if (cursor == 2) wattroff(win, A_BLINK);

	if (cursor == 3) wattron(win, A_BLINK);
	mvwprintw(win, ymax - 2, 1, "   Quit game   ");
	if (cursor == 3) wattroff(win, A_BLINK);

	updateWindow(win);



}

int openOptions(WINDOW* win, OPTION* options) {
	/* loop until options closed, show all the options */

	int loop = 0;
	int cursor = 0;

	showOptions(win, options, cursor);
	updateWindow(win);

	while (!loop) {
		int c = wgetch(win);
		switch (c) {

		case KEY_UP:
			cursor = mod(cursor - 1, OPTIONNB-1);
			showOptions(win, options, cursor);
			break;

		case KEY_DOWN:
			cursor = mod(cursor + 1, OPTIONNB - 1);
			showOptions(win, options, cursor);
			break;

		case '\n':
			switch (cursor) {
			case OPTIONNB-1:
				loop = 1;
				break;

			case 2:
				resetSave();
				wattron(win, A_REVERSE);
				mvwprintw(win, 3, 1, "Reseted !      ");
				wattroff(win, A_REVERSE);
				break;
			default:
				setOption(options, cursor);
				showOptions(win, options, cursor);
				break;
			}
			break;
		}
	}

	return 0;
}

void showOptions(WINDOW* win, OPTION* options, int cursor) {
	/* show options content */

	wclear(win);
	
	/* option ascii */

	/* title */
	if (cursor == 0) wattron(win, A_BLINK);
	mvwprintw(win, 1, 1, "Ascii mode     ");
	if (cursor == 0) wattroff(win, A_BLINK);
	/* previsualize the boolean */
	if (options->asciimages) wattron(win, COLOR_PAIR(COLORPAIR_G));
	else wattron(win, COLOR_PAIR(COLORPAIR_R));
	mvwprintw(win,1,14, "%c%c", 219, 219);
	if (options->asciimages) wattroff(win, COLOR_PAIR(COLORPAIR_G));
	else wattroff(win, COLOR_PAIR(COLORPAIR_R));

	
	/* option brightness */

	/* title */
	if (cursor == 1) wattron(win, A_BLINK);
	mvwprintw(win, 2, 1, "Brightness     ");
	if (cursor == 1) wattroff(win, A_BLINK);

	/* previsualize the brightness */
	const int bright = options->brightness * 4;
	init_color(9, bright, bright, bright);
	init_pair(9, 9, 9);
	wattron(win, COLOR_PAIR(9));
	mvwprintw(win, 2, 14, "%c%c", 219, 219);
	wattroff(win, COLOR_PAIR(9));
	wrefresh(win);


	/* option reset save */
	if (cursor == 2) wattron(win, A_BLINK);
	mvwprintw(win, 3, 1, "Reset save     ");
	if (cursor == 2) wattroff(win, A_BLINK);


	/* back */
	if (cursor == 3) wattron(win, A_BLINK);
	mvwprintw(win, 6, 1, "< Back         ");
	if (cursor == 3) wattroff(win, A_BLINK);

	updateWindow(win);
}

void setOption(OPTION* options, int cursor) {
	/* set an option */

	switch (cursor) {

	case 0:
		options->asciimages = !(options->asciimages);
		break;
	case 1:
		options->brightness = (options->brightness + 50)%300;
		break;
	}
}

void showInventory(WINDOW* win, int memory[MEMSIZE], LISTS* itemlist) {
	/* show inventory content */

	int wsx, wsy;
	getmaxyx(win, wsy, wsx);

	const int x90 = wsx - 22;
	
	wattron(win, COLOR_PAIR(COLORPAIR_Y));
	
	wmove(win, 1, 1);
	wprintw(win, "[Inventory]");

	/* show all items the player own */
	int y = 3;
	int x = 2;
	for (unsigned i = 0; i < LISTS_length(itemlist); i++) {
		if (memory[M_INV + i]) {
			wmove(win, y, x);
			wprintw(win, "[%s x%d]",LISTS_get(itemlist,i)->str, memory[M_INV + i]);
			y += 2;
			if (y >= 20) {
				y = 3;
				x += 40;
			}
		}

	}
	wattroff(win, COLOR_PAIR(COLORPAIR_Y));

	/* show the exit button */
	wmove(win, 2, x90);
	wattron(win, A_REVERSE);
	wprintw(win, "[i] Exit > ");
	wattroff(win, A_REVERSE);

	/* print the |s */
	int cptr = 0;
	for (cptr = 0; cptr < wsy-1; cptr++) {
		mvwprintw(win, cptr, x90 - 8, "%c",179);
	}

	updateWindow(win);

}

void showUnlockedEndings(WINDOW* win) {
	/* show unlocked endings */

	unsigned long nb = 0;
	STRING* tempstr;

	int ymax;
	int xmax;
	getmaxyx(win, ymax, xmax);

	/* find the save file and opens it */
	tempstr = DB_found(GAMEINFO, FILE_SAVE, INFO_SEPARATOR, INFO_LINKING);
	if (tempstr) {
		nb = DB_readBin(tempstr->str);
		Str_free(tempstr);
		tempstr = NULL;
	}

	wclear(win);
	mvwprintw(win, 1, 1, "Endings unlocked :");
	wmove(win, 3, 3);

	/* endings list */
	LISTS* endinglist = NULL;
	endinglist = DB_storeToList("files/endings");

	/* print the endings unlocked */
	int y = 5;
	int x = 5;
	for (unsigned i = 0; i < LISTS_length(endinglist); i++) {
		if (nb & powtwo(i)) {
			wattron(win, COLOR_PAIR(COLORPAIR_Y));
			mvwprintw(win, y, x, "%s", LISTS_get(endinglist, i)->str);
			wattroff(win, COLOR_PAIR(COLORPAIR_Y));
		}
		else 
		{
			wattron(win, COLOR_PAIR(COLORPAIR_B));
			mvwprintw(win, y, x, "[?]");
			wattroff(win, COLOR_PAIR(COLORPAIR_B));
		}
		y+=2;
		if (y >= 12) {
			x += 34;
			y = 5;
		}
			
	}

	/* draw the windows box grey */
	wattron(win, COLOR_PAIR(COLORPAIR_L));
	updateWindow(win);
	wattroff(win, COLOR_PAIR(COLORPAIR_L));

	/* free the ending list */
	LISTS_free(&endinglist);
}

void showEnding(WINDOW* win,int nb) {
	/* show an ending based on its index */
	/* wont wait for user input */
	
	char c;
	int anchor = 0;
	int offset = 0;
	int index = 0;
	
	FILE* file = NULL;
	openFile(&file, nb, "end");

	wclear(win);

	if (file) {

		/* read the first line to get ending infos */
		while ((c = fgetc(file)) != EOF)
		{
			if (c == '(')
			{

				anchor = 0;
				offset = 0;
				index = 0;

				/* if the windows is anchor up or down (up is 0) */
				c = fgetc(file);
				anchor = sumUntil(file, (int*)0, &c, ",") ? ANCHOR_DOWN : ANCHOR_UP;

				/* get the ascii image offset from the anchor point */
				if (c != EOF){
					c = fgetc(file);
					offset = sumUntil(file, (int*)0, &c, ",");
				}

				/* get the ascii image index */
				if (c != EOF) {
				c = fgetc(file);
				index = sumUntil(file, (int*)0, &c, ")");
				}

				/* show ascii image */
				showAsciiImage(win, anchor, offset, index);
			}
		}
		fclose(file);
	}
	else printf("\033[0;31mend file cant open\033[0;30m\n");
	updateWindow(win);

	

}

void addEnding(int nb) {
	/* add ending to the save file */

	unsigned long k;
	unsigned size = 0;
	char string[255];
	STRING* tempstr = NULL;
	LISTS* endinglist = NULL;

	/* get number of endings */
	tempstr = DB_found(GAMEINFO, FILE_ENDINGS, INFO_SEPARATOR, INFO_LINKING);
	if(tempstr){
		
		endinglist = DB_storeToList(tempstr->str);
		if (endinglist) {
			size = LISTS_length(endinglist);
			LISTS_free(&endinglist);
		}
		Str_free(tempstr);
		tempstr = NULL;
	}



	/* get the save file emplacement and add the ending */
	tempstr = DB_found(GAMEINFO, FILE_SAVE, INFO_SEPARATOR, INFO_LINKING);
	if (tempstr) {

		k = DB_readBin(tempstr->str);

		k |= powtwo(nb);

		bintostr(k, size, string);
		DB_write(tempstr->str, string);

		Str_free(tempstr);
		tempstr = NULL;
	}

}

void resetSave(){
	/* reset the save file */

	STRING* tempstr = NULL;
	tempstr = DB_found(GAMEINFO, FILE_SAVE, INFO_SEPARATOR, INFO_LINKING);
	if (tempstr)
	{
		DB_write(tempstr->str, "0");
		Str_free(tempstr);
		tempstr = NULL;
	}
}

void showAsciiImage(WINDOW* win,int anchor,int ypos, int nb) {
	/* print to a window and ascii image */
	/* ascii image are fond in the asc directory with the .asc extention*/

	char c;
	int textSizex = 0;
	int textSizey = 0;
	int ymax = 0;
	int xmax = 0;
	int texty = 1;
	int textx = 1;

	getmaxyx(win, ymax, xmax);



	FILE* file = NULL;
	openFile(&file, nb, "asc");
	if (file) {

		/* read the first line to get asc image infos */
		while ((c = fgetc(file)) != '\n' && c != EOF)
		{
			if (c == 'x')
			{
				c = fgetc(file);
				textSizex = sumUntil(file, (int*)0, &c, "x\n");
			}
			if (c == 'y')
			{
				c = fgetc(file);
				textSizey = sumUntil(file, (int*)0, &c, "y\n");
			}
		}
		/**/


		/*calculate the right emplacement to print the ascii image*/
		textx = xmax / 2 - textSizex / 2;
		clamp(&textx, 0, xmax);

		texty = ypos;
		if (anchor == ANCHOR_UP) {
			texty = ypos+1;
		}
		else if (anchor == ANCHOR_DOWN) {
			ypos += textSizey;
			texty = ymax - ypos;
		}
		clamp(&texty, 0, ymax-textSizey);

		/*move the cursos to the right position */
		wmove(win, texty, textx);


		/* show the text */
		while (!feof(file))
		{
			c = fgetc(file);

			if (c != '\n') {

				/* if the image is bigger than the windows sizedont print char */
				if (getcurx(win) < xmax-1) wprintw(win, "%c", c);
			}
			else {
				wmove(win, getcury(win) + 1, textx);
			}

		}

		fclose(file);
	}
	else printf("\033[0;31masc file cant open\033[0;30m\n");
	updateWindow(win);
}