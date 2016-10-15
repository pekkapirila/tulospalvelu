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
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "globals.h"
#include "console.h"
#include "size.h"  /* for resizeConBufAndWindow() */

HANDLE hStdIn, hStdOut, hConIn, hConOut; /* standard input, output handles */

int initcon(TCHAR *title)
{
  BOOL bSuccess;
  DWORD dwMode;
  CONSOLE_SCREEN_BUFFER_INFO csbi; /* used to get cursor position */

  /* check if Win32s, if so, display notice and terminate */
  if( GetVersion() & 0x80000000 && (GetVersion() & 0xFF) == 3)
	{
	MessageBoxW(NULL,
		L"This application cannot run on Windows 3.1.\n"
		L"This application will now terminate.",
		L"Error: Windows NT Required to Run",  MB_OK );
	return(1);
    }

  bSuccess = SetConsoleOutputCP(850);

  /* Free the console and immediately allocate a new one. This is done so */
  /* that when debugging under ntsd, the application output will not be */
  /* intermingled with the debugger output. This also makes cleanup */
  /* much easier; we won't need to restore the cursor, buffer/console */
  /* sizes, screen colors, etc. since we'll have our own console window */
  /* that will go away when we terminate. */

  /* Here's a trick we'll use to guess whether we're starting from a */
  /* console or GUI app: if the cursor is at 0,0, we'll assume that we've */
  /* started from a GUI app and not bother freeing/allocing a new console. */
  /* This will also avoid cases where the new console is not in the */
  /* foreground (AllocConsole does not guarantee a foreground window). */
  bSuccess = GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE),
             &csbi);
  PERR(bSuccess, "GetConsoleScreenBufferInfo");
  if (csbi.dwCursorPosition.X | csbi.dwCursorPosition.Y) /* either non-zero? */
    {
    bSuccess = FreeConsole();
    PERR(bSuccess, "FreeConsole");
    /* C run-time standard-handle I/O will not work from now on unless we */
    /* "fix" the C standard handles to reference the console standard */
    /* handles which will be created by the AllocConsole call below. */
    /* In this sample, we won't be using C run-time I/O, so it's not an */
    /* issue. */
    bSuccess = AllocConsole();
    PERR(bSuccess, "AllocConsole");
    }
  /* let's put up a meaningful console title */
  bSuccess = SetConsoleTitle(title);
  PERR(bSuccess, "SetConsoleTitle");
  /* get the standard handles */
  hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
  hConOut = hStdOut;
  PERR(hStdOut != INVALID_HANDLE_VALUE, "GetStdHandle");
  hStdIn = GetStdHandle(STD_INPUT_HANDLE);
  hConIn = hStdIn;
  PERR(hStdIn != INVALID_HANDLE_VALUE, "GetStdHandle");
  /* set up mouse and window input */
  bSuccess = GetConsoleMode(hStdIn, &dwMode);
  PERR(bSuccess, "GetConsoleMode");
  /* when turning off ENABLE_LINE_INPUT, you MUST also turn off */
  /* ENABLE_ECHO_INPUT. */
  bSuccess = SetConsoleMode(hStdIn, (dwMode & ~(ENABLE_LINE_INPUT |
      ENABLE_ECHO_INPUT | ENABLE_PROCESSED_INPUT | ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT)));
  PERR(bSuccess, "SetConsoleMode");
  /* save then hide the cursor */
  //cci.dwSize = 100;
  //cci.bVisible = FALSE;
  //bSuccess = SetConsoleCursorInfo(hStdOut, &cci);
  //ERR(bSuccess, "SetConsoleCursorInfo");
  /* resize console to CONX * CONY and put list of console APIs up */

  return(0);
}
