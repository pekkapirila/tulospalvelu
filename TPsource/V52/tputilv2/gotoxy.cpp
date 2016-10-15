// Pekka Pirila's sports timekeeping program (Finnish: tulospalveluohjelma)
// Copyright (C) 2015 Pekka Pirila 

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <windows.h>

void gotoxy(int x, int y)
{
	extern HANDLE hConOut;
	COORD CurPos;

	CurPos.X = x;
	CurPos.Y = y;
	if (!SetConsoleCursorPosition(hConOut, CurPos)) {
		GetLastError();
		return;
	}
}

void getxy(int *x, int *y)
{
	extern HANDLE hConOut;
	CONSOLE_SCREEN_BUFFER_INFO ConsoleScreenBufferInfo;

	if (!GetConsoleScreenBufferInfo(hConOut, &ConsoleScreenBufferInfo)) {
		GetLastError();
		*x = 0;
		*y = 0;
		return;
	}
	*x = ConsoleScreenBufferInfo.dwCursorPosition.X;
	*y = ConsoleScreenBufferInfo.dwCursorPosition.Y;
}

int wherey(void)
{
	int x, y;

	getxy(&x, &y);
	return(y);
}

int wherex(void)
{
	int x, y;

	getxy(&x, &y);
	return(x);
}