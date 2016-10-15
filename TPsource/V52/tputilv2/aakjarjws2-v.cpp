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
#include <ctype.h>
#include "tputil.h"


UCHAR *aakjarjstr2(UCHAR *knimi)
   {
   UCHAR *p,*pmax,ch1,ch2;
   static UCHAR s[31] = "";

   s[30] = 0;
   pmax = s + 30;
   memset(s, ' ',30);
   p = s;
   if (knimi[2] == ' ' && (((ch1 = toupper(knimi[0])) == 'A' &&
      ((ch2 = toupper(knimi[1])) == 'V' || ch2 == 'F')) ||
      (ch1 == 'D' && toupper(knimi[1]) == 'E')) && knimi[3] > ' ')
      knimi += 3;
   if (knimi[3] == ' ' && toupper(knimi[0]) == 'V' &&
      toupper(knimi[2]) == 'N' &&
      ((ch1 = toupper(knimi[1])) == 'O' || ch1 == 'A') && knimi[4] > ' ') 
      knimi += 4;
   while (*knimi != 0 && p < pmax) {
      if (*knimi != '-') {
         if (*knimi > 127) {
            switch (*knimi) {
			   case 200 :
			   case 201 :
			   case 202 :
			   case 203 :
			   case 232 :
			   case 233 :
			   case 234 :
			   case 235 :
               case '‚' :
               case '' : *p = 'E'; break;
			   case 220 :
			   case 252 :
               case '' :
               case 'š' : *p = 'Y'; break;
			   case 197 :
			   case 229 :
               case '†' :
               case '' : *p = 91; break;
			   case 196 :
			   case 228 :
               case '„' :
               case 'Ž' : *p = 92; break;
			   case 214 :
			   case 246 :
               case '”' :
               case '™' : *p = 93; break;
               default  : *p = *knimi;
               }
            }
         else if (*knimi >= 'a' && *knimi <= 'z') *p = *knimi - 32;
         else *p = *knimi;
         p++;
         }
      knimi++;
      }
   return(s);
   }
