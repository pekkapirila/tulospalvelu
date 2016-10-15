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
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <bstrings.h>
#include <bvideo.h>
#include <tputil.h>

char inputch(char *s, int x, int y, const char *term, char *tc,
                                                       int numfl)
   {
   char cs, ch, che;

   cs = *s;
   sccurset(y,x);
   for(;;) {
      viwrrect(y, x, y, x, s, SC_WHITE, SC_BLACK, 0);
      ch = readkbd(&che,1,0);
      if (ch == 0) {
         ch=254;
         switch ((UCHAR) che) {
            case 15 : ch = 11; break;       /* backtab */
            case 59 :                         /* F1 */
            case 60 :                         /* F2 */
            case 61 :                         /* F3 */
            case 62 :                         /* F4 */
            case 63 :                         /* F5  */
            case 64 :                         /* F6 */ 
            case 65 :                         /* F7 */
            case 66 :                         /* F8 */
            case 67 :                         /* F9 */
            case 68 : ch = che + 142; break;  /* F10 */
            case 72 : ch = 5; break;          /*up*/
            case 80 : ch = 24; break;         /*down*/
            case 73 : ch = 18; break;         /*pgup*/
            case 81 : ch = 3; break;          /*pgdn*/
            case 118 : ch = 2; break;         /*Ctrl-PgDn*/
            case 132 : ch = 19; break;        /*Ctrl-PgUp*/
            }                  /* switch(che) */
         }
      if (stschind(ch,term) > -1) break;
      if (ch < 32 || ch > 154 || ch == 127 ||
         (numfl && stschind(ch, "0123456789-+ ") != -1))
         erbeep();
      else
         cs = ch;
      }
   *tc = ch;
   if (ch != 27) *s = cs;
   return(*s);
   }
