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

#if defined(__BORLANDC__)
#pragma -K -a1
#endif
#include <windows.h>
#include <bvideo.h>
#include <butil.h>
#include <bkeybrd.h>
#include <tputil.h>
#define TRUE 1
#define FALSE 0


extern INT16 ySize, xSize, errMsgBox;
int writeerrorOn;

void writeerror(char *msgin, int kesto)
{
   char msg[81], save[500];

   erbeep();
   strncpy(msg, msgin, 80);
   msg[80] = 0;
   if (errMsgBox) {
	  oemtoansi(msg, 0);
	   MessageBoxA(NULL, msg, "", MB_OK | MB_SETFOREGROUND | MB_TASKMODAL);
	   return;
	  }
	memset(msg+strlen(msg), ' ', 80-strlen(msg));
	msg[80] = 0;
	writeerrorOn = 1;
	virdrect(ySize - 1, 0, ySize - 1, 79, save, 1);
	vidspmsg(ySize-1,0,0,7,msg);
   if (kesto < 0) return;
   if (kesto > 0)
	   Sleep(kesto);
   else {
	  kbflush();
	  viwrrect(ySize-1, 68, ySize-1, 79, " Paina Enter", 0, 7, 0);
	  readkbd(msg, 1, 0);
	  kbflush();
	  }
   viwrrect(ySize-1, 0, ySize-1, 79, save, 7, 0, 1);
   writeerrorOn = 0;
}

void writeerror_w(wchar_t *msgin, int kesto)
{
   char save[500], che;
   wchar_t wmsg[201];

   erbeep();
   wcsncpy(wmsg, msgin, 200);
   wmsg[200] = 0;
   if (errMsgBox) {
	   MessageBoxW(NULL, wmsg, L"", MB_OK | MB_SETFOREGROUND | MB_TASKMODAL);
	   return;
	  }
    wmsg[80] = 0;
	wmemset(wmsg+wcslen(wmsg), L' ', 80-wcslen(wmsg));
	writeerrorOn = 1;
	virdrect(ySize - 1, 0, ySize - 1, 79, save, 1);
	vidspwmsg(ySize-1,0,0,7,wmsg);
   if (kesto < 0) return;
   if (kesto > 0)
	   Sleep(kesto);
   else {
	  kbflush();
	  vidspwmsg(ySize-1, 68, 0, 7, L" Paina Enter");
	  readkbd(&che, 1, 0);
	  kbflush();
	  }
   viwrrect(ySize-1, 0, ySize-1, 79, save, 7, 0, 1);
   writeerrorOn = 0;
}
