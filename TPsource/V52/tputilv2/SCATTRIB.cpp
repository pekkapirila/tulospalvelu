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
//#include "bscreens.h"
#include "tputil.h"

int scattrib(int fore, int back, char ch, unsigned cnt)
{
	extern HANDLE hConOut;  // handle to a console screen buffer
	COORD Coord;
	DWORD N;
	int x, y, a, b;

	sccurst(&y, &x, &a, &b);
	Coord.X = x;
	Coord.Y = y;
	N = cnt;
	if (!FillConsoleOutputCharacter(hConOut, ch, N, Coord, &N)) {
		GetLastError();
		return(-1);
	}
	if (!FillConsoleOutputAttribute(hConOut, (WORD)(16*back+fore), N, Coord, &N)) {
		GetLastError();
		return(-1);
	}
	return(0);
}
