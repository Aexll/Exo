#pragma once
#define MEMSIZE 255				//la taille de la mémoire
#define	QSIZE 255				//size in char of the questions
#define QNB 10					//nb of questions max
#define M_QVALID (QNB)			//the memeory adress where question are valid or not
#define M_IMI (QNB+QNB)			//the memory adress where we stocks the image index to show
#define M_END (M_IMI+1)			//the memory adress where we stocks if we end a run
#define M_INV (M_IMI+2)			//the memory adress where we stocks the inventory
#define C_BLANK 219				//caracter of the border
#define EXT_TXT "txt"			//text file extentions
#define EXT_IMG "im"			//image file extentions
#define REPERTORY "files"		//folder name where the files are
#define REPERTORY_TXT "files/txt" //folder where txt are
#define REPERTORY_IMG "files/img" //folder where img are
#define ERROR_FILE_NAME "error" //name of the error file
#define OPTIONNB 4
#define FIRSTIMAGE 18

/* game options parameters */
#define INFO_SEPARATOR '\n'
#define INFO_LINKING ':'
#define ASC_TITLE "asc_title"
#define ASC_CONTINUE "asc_continue"
#define ASC_OPTION "asc_option"
#define ASC_ENDINGS "asc_endings"

#define GAMEINFO "files/gameinfo"
#define FILE_SAVE "save file"
#define FILE_ITEMS "items file"
#define FILE_ENDINGS "endings file"
#define FILE_TXTFOLDER "txt folder"
#define FILE_IMGFOLDER "img folder"
#define FILE_ENDFOLDER "end folder"
#define FILE_ASCFOLDER "asc folder"
#define FILE_ERROR "error file"

//DB_found(GAMEINFO, FILE_SAVE, INFO_SEPARATOR, INFO_LINKING);

/*qqqqqvvvvviiiii*/