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

int virdrect(int u_row, int u_col, int l_row, int l_col,
			 char *buf, int option)
{
	extern HANDLE hConOut;  // handle of a console screen buffer
	CHAR_INFO Buffer[8000];    // address of buffer that receives data
	COORD dwBufferSize;     // column-row size of destination buffer
	COORD dwBufferCoord;    // upper-left cell to write to
	SMALL_RECT ReadRegion;   // address of rectangle to read from
	PSMALL_RECT lpRR;
	int bsize, i;

	lpRR = &ReadRegion;
	ReadRegion.Left = (SHORT) u_col;
	ReadRegion.Top = (SHORT) u_row;
	ReadRegion.Right = (SHORT) l_col;
	ReadRegion.Bottom = (SHORT) l_row;
	dwBufferSize.X = l_col - u_col + 1;
	dwBufferSize.Y = l_row - u_row + 1;	
	dwBufferCoord.X = 0;
	dwBufferCoord.Y = 0;
	bsize = dwBufferSize.X*dwBufferSize.Y;
	if (!ReadConsoleOutputA(hConOut, Buffer, dwBufferSize, dwBufferCoord,
		lpRR)) {
		GetLastError();
		return(-1);
	}
	if (!option) {
		for (i = 0; i < bsize; i++) {
			buf[i] = Buffer[i].Char.AsciiChar;
			if (buf[i] >= 179 && buf[i] <= 205)
				buf[i] -= 13;
		}
	}
	else {
		memcpy(buf, Buffer, bsize * sizeof(CHAR_INFO));
	}
	return(bsize);
}
 
