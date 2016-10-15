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

int ProcessSpecialKey(INPUT_RECORD *Buf);

static char ctrlkey[25] = {94,95,96,97,98,99,100,101,102,103,
      69,70,119,141,132,74,115,76,116,78,117,145,118,146,147};
static char altkey[25] = {104,105,106,107,108,109,110,111,112,113,
      69,70,151,152,153,74,155,76,157,78,159,160,161,162,163};
static char shiftkey[10]= {84,85,86,87,88,89,89,91,92,93};
                      
char readkbd(char  *ch2, int wait, int dummy)
{
	extern HANDLE hConIn;
	INPUT_RECORD Buf;
	DWORD nRead;
	WORD KC;
	char ch;

	if (wait) {
		do {
			if(!ReadConsoleInputA(hConIn, &Buf, 1, &nRead)) {
				GetLastError();
				return(-1);
				}
		} while(nRead && (Buf.EventType != KEY_EVENT || !Buf.Event.KeyEvent.bKeyDown ||
			((KC = Buf.Event.KeyEvent.wVirtualKeyCode) >= VK_SHIFT && KC <= VK_CAPITAL) || 
			(KC > VK_F12 && KC < 0xBA) || KC >= VK_PROCESSKEY || ProcessSpecialKey(&Buf)));
	}
	else {
		do {
			if (!PeekConsoleInputA(hConIn, &Buf, 1, &nRead)) {
				GetLastError();
				return(255);
				}
			if (nRead) {
				if (!ReadConsoleInputA(hConIn, &Buf, 1, &nRead)) {
				GetLastError();
				return(255);
				}
			}
		} while(nRead && (Buf.EventType != KEY_EVENT || !Buf.Event.KeyEvent.bKeyDown ||
			((KC = Buf.Event.KeyEvent.wVirtualKeyCode) >= VK_SHIFT && KC <= VK_CAPITAL) || 
			(KC > VK_F12 && KC < 0xBA) || KC >= VK_PROCESSKEY));
	}
	*ch2 = 0;
	ch = 0;
	if (nRead) {
      if (!ProcessSpecialKey(&Buf)) {
		   ch = (char) Buf.Event.KeyEvent.uChar.AsciiChar;
         if (ch == 224)
            ch = 0;
		   *ch2 = (char) Buf.Event.KeyEvent.wVirtualScanCode;
         if ((Buf.Event.KeyEvent.dwControlKeyState & (LEFT_CTRL_PRESSED | RIGHT_CTRL_PRESSED))
            && *ch2 >= 59 && *ch2 <= 83)
            *ch2 = ctrlkey[*ch2-59];
         if ((Buf.Event.KeyEvent.dwControlKeyState & SHIFT_PRESSED)
				&& ((*ch2 >= 59 && *ch2 <= 68) || *ch2 == 15)) {
					if (*ch2 == 15)
						ch = 11;
					else
						*ch2 = shiftkey[*ch2-59];
				}
         if ((Buf.Event.KeyEvent.dwControlKeyState & LEFT_ALT_PRESSED)) {
            if (*ch2 >= 59 && *ch2 <= 83)
               *ch2 = altkey[*ch2-59];
            if (ch >= 97 && ch <= 122)
               ch = 0;
				if (*ch2 >= 2 && *ch2 <= 11) {
					*ch2 += 118;
					ch = 0;
					}
            }
         }
	   }
	return(ch);
}


