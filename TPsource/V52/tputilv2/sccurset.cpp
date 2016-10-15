/**
*
* Name		sccurset -- Set the cursor position on the current
*			    display page.
*
* Synopsis	cpos = sccurset(row,col);
*
*		int cpos;	  Return position (row,col)
*		int row;	  row position
*		int col;	  column position
*
* Description	SCCURSET sets the cursor to the position specified by
*		row and col on the current page.  The current page must
*		be the same as the active display page (i.e., currently
*		displayed) for the cursor to be seen to move.
*
* Returns	cpos		  The cursor position with the row
*				  in the high order byte and the column
*				  in the low order byte.
*
* Version	6.00 (C)Copyright Blaise Computing Inc.  1983,1987,1989
*
**/

#include <windows.h>

int sccurset(int row, int col)
{
	extern HANDLE hConOut;
	COORD CurPos;

	CurPos.X = col;
	CurPos.Y = row;
	if (!SetConsoleCursorPosition(hConOut, CurPos)) {
		GetLastError();
		return(-1);
	}
	return (256*row+col);
}
