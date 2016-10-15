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
#pragma -K
#endif
#include <ctype.h>

char upcasesc(char ch)
{
   int i;
   char c;

   switch(ch) {
      case 130 :  return('');
      case 132 :  return('Ž');
      case 148 :  return('™');
      case 134 :  return('');
      case 129 :  return('š');
   }
   i = ch;
   c = toupper(i);
	if (c >= 224)
		c -= 32;
   return(c);
}

wchar_t upcasewsc(wchar_t ch)
{
	if  ((ch >= L'a' && ch <= L'z') ||
		(ch >= 224 && ch <= 254 && ch != 247)
		) {
		return(ch-32);
		}
	return(ch);
}

