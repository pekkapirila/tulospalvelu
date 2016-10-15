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

#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <tputil.h>

#ifdef _BORLAND_
#include <vcl.h>
#endif

int MbsToWcs(wchar_t *wst, char *st, int len)
{
//#ifdef _BORLAND_
//	Utf8ToUnicode(wst, st, len);
//#else
	MultiByteToWideChar(CP_UTF8, 0, st, -1, wst, len);
//#endif
	return(wcslen(wst));
}

int WcsToMbs(char *st, wchar_t *wst, int len)
{
//#ifdef _BORLAND_
//	UnicodeToUtf8(st, wst, len);
//#else
	WideCharToMultiByte(CP_UTF8, 0, wst, -1, st, len, 0, 0);
//#endif
	return(strlen(st));
}

