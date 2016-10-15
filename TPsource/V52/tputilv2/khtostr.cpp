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

void khtostr(char kh, char *str)
	{
	switch (kh) {
		case 'K' :
			strcpy(str, "Kesk.");
		break;
		case 'H' :
			strcpy(str, "Hyl.");
		break;
		case 'E' :
			strcpy(str, "Ei l„ht.");
		break;
		case 'P' :
			strcpy(str, "Poissa");
		break;
		case 'L' :
			strcpy(str, "Leim.hyv");
		break;
		default  :
			strcpy(str, "-");
		}
	}

void khtowstr(wchar_t kh, wchar_t *str)
	{
	switch (kh) {
		case L'K' :
			wcscpy(str, L"Kesk.");
		break;
		case L'H' :
			wcscpy(str, L"Hyl.");
		break;
		case L'E' :
			wcscpy(str, L"Ei läht.");
		break;
		case L'P' :
			wcscpy(str, L"Poissa");
		break;
		case L'L' :
			wcscpy(str, L"Leim.hyv");
		break;
		default  :
			wcscpy(str, L"-");
		}
	}
