/**
*
* Name		scwrite -- Write copies of a character
*
* Synopsis	iret = scwrite(ch,cnt);
*
*		int  iret	  Return value is always 0
*		char ch 	  Character to write
*		unsigned cnt	  Number of copies of ch to write
*
* Description	SCWRITE displays cnt copies of the character ch on the
*		current display screen without changing the
*		currently-set attribute.  The characters are written on
*		the current page starting at the current cursor
*		position.
*
*		Unexpected results may occur if you write beyond the end
*		of the screen, or if (in graphics mode) you write beyond
*		the current text row.  This function will not scroll the
*		screen.
*
*		SCATTRIB is preferable for graphics modes because it
*		allows control of the characters' color.  SCWRITE uses
*		color 1 in graphics modes.
*
* Version	6.00 (C)Copyright Blaise Computing Inc.  1983,1987,1989
*
**/

#include <windows.h>
#include "bscreens.h"

char ansitooemch(char ch);

int scwrite(char ch, unsigned cnt)
{
	extern HANDLE hConOut;  // handle to a console screen buffer
	COORD Coord;
	DWORD N;
	int x, y, a, b;

	sccurst(&y, &x, &a, &b);
	Coord.X = x;
	Coord.Y = y;
	N = cnt;
	ch = ansitooemch(ch);
	if (!FillConsoleOutputAttribute(hConOut, FOREGROUND_RED | FOREGROUND_GREEN |
      FOREGROUND_BLUE | FOREGROUND_INTENSITY, N, Coord, &N)) {
		GetLastError();
		return 1;
   }
	if (!FillConsoleOutputCharacter(hConOut, ch, N, Coord, &N)) {
		GetLastError();
		return 1;
	}
	return(0);
}
