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
//#define TESTPRT
#include <windows.h>

int WinVersion(void)
   {
   OSVERSIONINFO osvi;
	int NTversion;

   osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
   GetVersionEx(&osvi);
   if (osvi.dwPlatformId != VER_PLATFORM_WIN32_NT) {
       // Windows 95/98/Me
		NTversion = 0;
      }
   else {
      if (osvi.dwMajorVersion > 4) {
               // Windows 2000, XP
			NTversion = 5;
         }
      else {
               // Windows NT4
			NTversion = 4;
			}
		}
	return(NTversion);
	}
