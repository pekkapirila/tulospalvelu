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
#ifndef _CONSOLE
#define _CONSOLE
#endif
#include <string.h>
#include <bstrings.h>
#include <bkeybrd.h>
#include <bvideo.h>
#include <tputil.h>
#define TRUE 1
#define FALSE 0
#ifdef TARKNO
#define NNUM 6
#define CPOS 7
#else
#define NNUM 4
#define CPOS 3
#endif

extern int spoolfl;

void inputkno_os(int *kno, int *os, int x, int y, char *term, char *tc)
   {
   char tarr[6];
   char ch ,ch2;
   int i,j;
   char st[10];
   int okk;
   short os_s;

   memset(tarr, ' ', sizeof(tarr));
#ifdef TARKNO
   viwrrect(y,x,y,x+7,"    - - ",7,0,0);
#else
   viwrrect(y,x,y,x+5,"    - ",7,0,0);
#endif
   kbready(&ch, &i);
   if (ch < '0' || ch > '9') {
      j = *kno;
      for (i = 3; i >= 0 && j; i--) {
         tarr[i] = j%10 + '0';
         j /= 10;
         }
      }
   *kno = 0;
   i = 0;
   do {
      okk = TRUE;
      sccurset(y,x+CPOS);
      do {
         ch = readkbd(&ch2,TRUE,spoolfl);
         switch (ch) {
            case 8   :
               if (i > 0) {
                  i--;
                  if (i > 0) {
                     for (j = 0; j <= i; j++)
                        tarr[NNUM-1-j] = tarr[NNUM-2-j];
                     tarr[NNUM-1-i] = ' ';
                     }
                  }
               break;
            case '0' :
            case '1' :
            case '2' :
            case '3' :
            case '4' :
            case '5' :
            case '6' :
            case '7' :
            case '8' :
            case '9' :
               if (i == NNUM) erbeep();
               else {
                  i++;
                  for (j = NNUM - i; j < NNUM-1; j++)
                     tarr[j] = tarr[j+1];
                  tarr[NNUM-1] = ch;
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
            case 27  :
            case '*' :
            case 13  :  break;
            default  :  if (stschind(ch, term) == -1) erbeep();
            }
#ifdef TARKNO
         sprintf((char *)st,"%c%c%c%c-%c-%c",tarr[0],tarr[1],tarr[2],tarr[3],
            tarr[4],tarr[5]);
#else
         sprintf((char *)st,"%c%c%c%c-%c",tarr[0],tarr[1],tarr[2],tarr[3],
            tarr[4]);
#endif
         viwrrect(y,x,y,x+CPOS,st,7,0,0);
         sccurset(y,x+CPOS);
      } while (stschind(ch, term) == -1);
      if (ch == 13 || ch == '*') {
#ifdef TARKNO
         if (ch == '*')
            {
            memmove(tarr, tarr+2, 4);
#endif
            memcpy(st, tarr, 4);
            st[4] = 0;
            *kno = atoi(st);
            os_s = (short) *os;
            viwrrect(y,x,y,x+3,tarr,7,0,0);
#ifdef TARKNO
            viwrrect(y,x+CPOS,y,x+CPOS," ",7,0,0);
#else
            if (ch == 13)
#endif
               inputint(&os_s, 1, x+5, y, "+\r\x1b", &ch);
            tarr[4] = '0' + *os;
            *os = os_s;
            viwrrect(y,x+5,y,x+5,tarr+4,7,0,0);
#ifdef TARKNO
            tarr[5] = '0' + tarkno_os(*kno,*os);
            viwrrect(y,x+7,y,x+7,tarr+5,7,0,0);
            }
         else
            {
            memcpy(st, tarr, 4);
            st[4] = 0;
            *kno = atoi(st);
            *os = tarr[4] - '0';
            if ((*kno != 0) &&
                (*os < 1 || tarkno_os(*kno,*os) != tarr[5] - '0')) {
               writeerror("Virheellinen kilpailunumero tai osuus",1000);
               okk = FALSE;
               }
            }
#else
            if (*kno != 0 && *os < 1) {
               writeerror("Virheellinen osuus",1000);
               okk = FALSE;
               }
#endif
         }
      } while (!okk);
   *tc = ch;
   }


void inputkno_osw(int *kno, int *os, int x, int y, wchar_t *wterm, wchar_t *wtc)
{
	char tc, term[40];
	int j;

	wcstooem(term, wterm, 39);
	for (j = 0; wterm[j]; j++)
		if (wterm[j] >= 201 && wterm[j] <= 210)
			term[j] = (char) wterm[j];
	term[39] = 0;
	inputkno_os(kno, os, x, y, term, &tc);
	if (tc >= 201 && tc <= 210)
		*wtc = tc;
	else
		*wtc = oemtowchar(tc);
}
