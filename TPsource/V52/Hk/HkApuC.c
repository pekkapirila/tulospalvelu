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

#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <process.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <bstrings.h>
#pragma hdrstop
#include <sys/stat.h>
#include <dos.h>
//#include "HkConsoleDef.h"
#include <tputil.h>
#include <size.h>

//INT32 t0_regnly;
INT16 xSize = 80, ySize = 25;
INT spoolfl = FALSE;
INT aani = 1;
//char tarecbuf[KILPBUFSIZE];
#ifdef _CONSOLE
INT16 errMsgBox = 0;
#else
INT16 errMsgBox = 1;
#endif

extern   HANDLE hStdIn, hStdOut, hConIn, hConOut;
void wkirjloki(wchar_t *tmsg);

void kirjloki(char *tmsg)
	{
	wchar_t wmsg[200];

	wkirjloki(oemtowcs(wmsg, tmsg, 198, 0));
	}

void resizeConBufAndWindow2(short xSize, short ySize)
{
      resizeConBufAndWindow(hConOut, xSize, ySize);
}