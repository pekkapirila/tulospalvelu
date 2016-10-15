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

#include <vcl.h>

int WideCharToMultiByte(UINT CodePage, DWORD dwFlags, LPCWSTR lpWideCharStr, int cchWideChar,
	LPSTR lpMultiByteStr, int cbMultiByte, LPCSTR lpDefaultChar, LPBOOL lpUsedDefaultChar)
{
	UTF8String US;
	US = lpWideCharStr;
	memset(lpMultiByteStr, 0, cbMultiByte);
	strncpy(lpMultiByteStr, US.c_str(), cbMultiByte-1);
	return(strlen(lpMultiByteStr));
}

int MultiByteToWideChar(UINT CodePage, DWORD dwFlags, LPCSTR lpMultiByteStr, int cbMultiByte,
	LPWSTR lpWideCharStr, int cchWideChar)
{
	UnicodeString US = UTF8ToString(lpMultiByteStr);
	memset(lpWideCharStr, 0, cchWideChar*sizeof(wchar_t));
	wcsncpy(lpWideCharStr, US.c_str(), cchWideChar-1);
}
