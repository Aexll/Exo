#pragma once
#define COLOR_OFFSET 17

#define COLORPAIR_W 1	//white
#define COLORPAIR_R 2	//red
#define COLORPAIR_G 3	//green
#define COLORPAIR_B 4	//blue
#define COLORPAIR_Y 5	//yellow
#define COLORPAIR_L 6	//grey
#define COLORPAIR_O 9	//orange

#define COLOR_R 1000,0,0
#define COLOR_G 0,1000,0
#define COLOR_B 0,0,1000
#define COLOR_Y 1000,1000,0
#define COLOR_L 250, 250, 250

typedef struct
{
	short r;
	short g;
	short b;
} COLOR;