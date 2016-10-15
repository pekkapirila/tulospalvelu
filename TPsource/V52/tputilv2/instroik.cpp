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
#include <string.h>
#include <bstrings.h>
#include <bvideo.h>
#include <tputil.h>
#define TRUE 1
#define FALSE 0

char *inputstr_oik(char *ss, unsigned l, int x, int y, const char *term, char *tc,
   int num_fl)
   {
   char tarr[80];
   char ch ,ch2, fill;
   int xo;
   unsigned i, j;

   if (ss[0] == '0') fill = '0';
   else fill = ' ';
   xo = x + l - 1;
   memset(tarr, ' ', sizeof(tarr));
   i = min(strlen(ss), l);
   memcpy(tarr + l - i, ss, i);
   i = 0;
   do {
      viwrrect(y,x,y,xo,tarr,7,0,0);
      sccurset(y,xo);
      ch = readkbd(&ch2,TRUE,0);
      switch (ch) {
         case 8   :
            if (i > 0) {
               i--;
               if (i > 0) {
                  for (j = 0; j <= i; j++)
                     tarr[l-1-j] = tarr[l-2-j];
                  tarr[l-1-i] = ss[l-1-i];
                  }
               else {
                  i = 0;
                  memmove(tarr+1,tarr,l);
                  tarr[0] = fill;
                  }
               }
            break;
         case 0  :
            switch (ch2) {
               case 15 : ch = 11; break;         /* backtab */
               case 59 :                         /* F1 */
               case 60 :                         /* F2 */
               case 61 :                         /* F3 */
               case 62 :                         /* F4 */
               case 64 :                         /* F6 */ 
               case 65 :                         /* F7 */ 
               case 66 :                         /* F8 */ 
               case 68 : ch = ch2 + 142; break;  /* F10 */
               case 63 :                         /* F5  */
               case 72 : ch = 5;  break;         /*up*/
               case 67 :                         /* F9 */
               case 80 : ch = 24; break;         /*down*/
               case 73 : ch = 18; break;         /*pgup*/
               case 81 : ch = 3;  break;         /*pgdn*/
               case 118: ch = 2;  break;         /*Ctrl-PgDn*/
               case 132: ch = 19; break;         /*Ctrl-PgUp*/
               }
            if (stschind(ch, term) == -1) erbeep();
            break;
         default  :
            if (stschind(ch, term) != -1) break;
            if (i == l || (num_fl && ch < '0' && ch > '9')) {
               erbeep();
               }
            else {
               i++;
               for (j = l - i; j < l - 1; j++)
                  tarr[j] = tarr[j+1];
               tarr[l-1] = ch;
               }
            break;
         }
   } while (stschind(ch, term) == -1);
   if (ch == 13 || ch == '*') {
      memcpy(ss, tarr, l);
      ss[l] = 0;
      }
   *tc = ch;
   return(ss);
   }


