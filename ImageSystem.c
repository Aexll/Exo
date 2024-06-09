#include <stdio.h>
#include <curses.h>
#include "Color.h"
#include "Options.h"

#define DRAWCHAR "#"

#define PIXELBRIGHTNESS 250

void putpixel(WINDOW* win, int x, int y,COLOR pcolor,int index,int mode) {
	/* put a pixel (2 blank char) at position in a window */

	init_color(index, pcolor.r, pcolor.g, pcolor.b);
	init_pair(index, index, index * !mode);
	wattron(win,COLOR_PAIR(index));
	mvwprintw(win, y, x, DRAWCHAR);
	mvwprintw(win, y, x+1, DRAWCHAR);
	wattroff(win,COLOR_PAIR(index));
	

}

void fixColor(WINDOW* win) {
	/* this is to patch the wired bug after printing an image */
	/* dont ask me why this bug */
	/* or why this code fix it	*/

	/* at least it work	*/
	/* update : it does not longer work :( */

	const int debugValue = 100;
	const int debugColor = 0;

	init_color(debugValue, debugColor, debugColor, debugColor);
	init_pair(debugValue, debugValue, debugValue);
	wattron(win, COLOR_PAIR(debugValue));
	mvwprintw(win, 0, 0, "%c",218);
	wattroff(win, COLOR_PAIR(debugValue));
	wrefresh(win);
}

void affImage(WINDOW* win,FILE* file, int x, int y,OPTION options) {
	/* print a image from image file to a windows */
	/* image files are fond in the img directory the the extention .im */

	int mode = options.asciimages;


	int ymax = 0;
	int xmax = 0;
	getmaxyx(win, ymax, xmax);
	
	char c;
	int r_x = 0;
	int r_y = 0;
	int index = 0;
	COLOR pcolor;

	if(file){
	
		c = fgetc(file);

		while (!feof(file)) {

			switch (c)
			{

			case '\n':
				r_y += 1;
				r_x = 0;
				c = fgetc(file);

		
			case ',':
				c = fgetc(file);
				if (isDigit(c)) pcolor.r = (c - '0');
				else pcolor.r = 0;
				c = fgetc(file);
				if (isDigit(c)) pcolor.g = (c - '0');
				else pcolor.r = 0;
				c = fgetc(file);
				if (isDigit(c)) pcolor.b = (c - '0');
				else pcolor.r = 0;
				index = pcolor.r + pcolor.g * 5 + pcolor.b * 25;

				pcolor.r = pcolor.r * options.brightness;
				pcolor.g = pcolor.g * options.brightness;
				pcolor.b = pcolor.b * options.brightness;

				if(r_y< ymax-2)
					putpixel(win, 1 + x + r_x*2, 1 + y + r_y, pcolor, index + COLOR_OFFSET, mode);
			
				r_x += 1;
				break;

			default:
				break;

			}
			c = fgetc(file);
		}
		wrefresh(win);

		fixColor(win);
	}
}
