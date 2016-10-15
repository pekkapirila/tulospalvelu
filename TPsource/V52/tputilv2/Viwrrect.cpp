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
#include <tputil.h>

char ansitooemch(char ch);
char *ansitooem(char *line0);

int norm_fore = 7;
int norm_back = 0;
int inv_back = 7;
int inv_fore = 0;

int viwrrect(int u_row, int u_col, int l_row, int l_col,
			const char *buf, int fore, int back, int option)
{
	extern HANDLE hConOut;  // handle of a console screen buffer
	CHAR_INFO Buffer[8000], BufOld[8000];    // address of buffer that receives data
	COORD dwBufferSize;     // column-row size of destination buffer
	COORD dwBufferCoord;    // upper-left cell to write to
	SMALL_RECT ReadRegion;   // address of rectangle to read from
	PSMALL_RECT lpRR;
	int bsize, i;
	DWORD Attr;


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
   if (fore == 7 && back == 0) {
      fore = norm_fore;
      back = norm_back;
      }
   else if (fore == 0 && back == 7) {
      fore = inv_fore;
      back = inv_back;
      }
	if (!ReadConsoleOutput(hConOut, BufOld, dwBufferSize, dwBufferCoord,
		lpRR)) {
		GetLastError();
		return(-1);
	}
	if (!option) {
      memcpy(Buffer, BufOld, bsize * sizeof(CHAR_INFO));
		Attr = fore + 16 * back;
		for (i = 0; i < bsize; i++) {
			Buffer[i].Attributes = (unsigned short) Attr;
			if (fore == norm_fore && (buf[i] == 216 || buf[i] == 248))
				Buffer[i].Attributes -= 2;
			Buffer[i].Char.AsciiChar = ansitooemch(buf[i]);
		}
	}
	else {
		memcpy(Buffer, buf, bsize * sizeof(CHAR_INFO));
		((char *) Buffer)[bsize] = 0;
		ansitooem((char *) Buffer);
	}
   if (memcmp(Buffer, BufOld, bsize * sizeof(CHAR_INFO)) && 
		!WriteConsoleOutputA(hConOut, Buffer, dwBufferSize, dwBufferCoord,
		lpRR)) {
		GetLastError();
		return(-1);
	}
	return(bsize);
}

int vidspmsg(int row, int col, int fore, int back, const char *msg)
{
	viwrrect(row, col, row, col + strlen(msg) - 1, msg, fore, back, 0);
	return(strlen(msg));
}