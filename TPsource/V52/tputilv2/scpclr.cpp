/**
*
* Name		scpclr -- Clear current display page
*
* Synopsis	scpclr();
*
* Description	This function clears the current display page (as
*		recorded in b_curpage) whether it is active or not.
*
* Returns	(None:	function return type is void.)
*
* Version	6.00 (C)Copyright Blaise Computing Inc.  1986-1989
*
**/
#include <windows.h>

void scpclr()
{
	extern HANDLE hConOut;
	CONSOLE_SCREEN_BUFFER_INFO ScBufInfo;
	COORD  Pos;
	DWORD N;

	if (!GetConsoleScreenBufferInfo(hConOut, &ScBufInfo)) {
		GetLastError();
		return;
	}
	Pos.X = 0;
	for (Pos.Y = 0; Pos.Y < ScBufInfo.dwSize.Y; Pos.Y++) {
		if (!FillConsoleOutputAttribute(hConOut, FOREGROUND_RED | FOREGROUND_GREEN |
         FOREGROUND_BLUE | FOREGROUND_INTENSITY, ScBufInfo.dwSize.X, Pos, &N)) {
			GetLastError();
			return;
		}
		if (!FillConsoleOutputCharacter(hConOut, ' ', ScBufInfo.dwSize.X, Pos, &N)) {
			GetLastError();
			return;
		}
	}
    return;
}

