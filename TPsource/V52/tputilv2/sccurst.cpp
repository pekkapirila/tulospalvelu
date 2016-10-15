/**
*
* Name		sccurst -- Returns the cursor position and size from
*			    current display page
*
* Synopsis	is_off = sccurst(prow,pcol,phigh,plow);
*
*		int is_off	  1 if cursor is off, 0 if on
*		int *prow	  Pointer to row value	    returned
*		int *pcol	  Pointer to column value   returned
*		int *phigh	  Pointer to high scan line returned
*		int *plow	  Pointer to low scan line  returned
*
* Description	SCCURST returns the current cursor location on the
*		current display device and page and the current size of
*		the cursor.  It also reports whether the cursor is on or
*		off.
*
*		Blaise C TOOLS do not record cursor sizes and on/off
*		states for inactive display pages.  That is, the cursor
*		size for the active page also applies to all inactive
*		pages.
*
*		This function operates by querying BIOS about the status
*		of the cursor.	Some IBM programs alter the cursor size
*		without alerting BIOS about the change, so the BIOS may
*		report inaccurate information about cursor size.
*		SCCURST is subject to such inaccuracies.
*
* Returns	is_off		  1 if cursor is off, 0 if on
*		*prow,*pcol,*phigh,*plow
*
* Version	6.00 (C)Copyright Blaise Computing Inc.  1986,1987,1989
*
**/

#include <windows.h>

int sccurst(int *prow, int *pcol, int *phigh, int *plow)
{
	extern HANDLE hConOut;
	CONSOLE_SCREEN_BUFFER_INFO ScBufInfo;
	CONSOLE_CURSOR_INFO CurInfo;

	if (!GetConsoleScreenBufferInfo(hConOut, &ScBufInfo)) {
		GetLastError();
		return(-1);
	}
	*prow = ScBufInfo.dwCursorPosition.Y;
	*pcol = ScBufInfo.dwCursorPosition.X;
	if (!GetConsoleCursorInfo(hConOut, &CurInfo)) {
		GetLastError();
		return(-1);
	}
    *phigh = CurInfo.dwSize;
    *plow  = 1;

    return (CurInfo.bVisible);
}
