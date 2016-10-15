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

extern int spoolfl;

void inputkno(int *kno, int x, int y, char *term, char *tc)
{
   char tarr[5];
   char ch ,ch2;
   int i,j,k;
   char st[7];
   int okk;

   *kno = 0;
   memset(tarr, ' ', 5);
   viwrrect(y,x,y,x+5,"    - ",7,0,0);
   i = 0;
   do {
     okk = TRUE;
     sccurset(y,x+5);
     do {
          ch = readkbd(&ch2,TRUE,spoolfl);
          switch (ch) {
            case 8   : {
                           if (i > 0)
                           {
                             i = i - 1;
                             if (i > 0)  for (j = 0; j <= i; j++)
                                               tarr[4-j] = tarr[3-j];
                             tarr[4-i] = ' ';
                           }
                       } break;
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
            case '*' : {
                           if (i == 5)  erbeep();
                           else
                           {
                             i = i + 1;
                             j = 5 - i;
                             while (j < 4)
                               {
                                 tarr[j] = tarr[j+1];
                                 j = j + 1;
                               }
                             tarr[4] = ch;
                           }
                         } break;
            case 00  : {
                         switch (ch2)
                            {
                            case 15 : ch = 11; break;       /* backtab */
                            case 59 :                         /* F1 */
                            case 60 :                         /* F2 */
                            case 61 :                         /* F3 */
                            case 62 :                         /* F4 */
                            case 64 :                         /* F6 */ 
                            case 65 :                         /* F7 */ 
                            case 66 :                         /* F8 */ 
                            case 68 : ch = ch2 + 142; break;  /* F10 */
                            case 63 :                         /* F5  */
                            case 72 : ch = 5;
                                      break;                  /*up*/
                            case 67 :                          /* F9 */
                            case 80 : ch = 24;
                                      break;               /*down*/
                            case 73 : ch = 18; break;      /*pgup*/
                            case 81 : ch = 3; break;      /*pgdn*/
                            case 118: ch = 2; break;     /*Ctrl-PgDn*/
                            case 132: ch = 19; break;     /*Ctrl-PgUp*/
                            }
                         if (stschind(ch, term) == -1) erbeep();
                         } break;
            case 27  :
            case 13  :  break;
            default  :  if (stschind(ch, term) == -1) erbeep();
         }
         sprintf(st,"%c%c%c%c-%c",tarr[0],tarr[1],tarr[2],tarr[3],
            tarr[4]);
         viwrrect(y,x,y,x+5,st,7,0,0);
         sccurset(y,x+5);
     } while (stschind(ch, term) == -1);
     if (ch == 13 || ch == '*') 
     {
         for (k = 0; k <= 3; k++) { 
            if (tarr[k] == ' ')  st[k] = '0';
                            else st[k] = tarr[k];
         }
         *kno = atoi(st);
       if (tarr[4] == '*')
       {
         tarr[4] = 48+tarkno(*kno);
         viwrrect(y,x+5,y,x+5,tarr+4,7,0,0);
       } else
         if ((*kno != 0) && (tarkno(*kno) != tarr[4] - 48))
         {
           writeerror("Virheellinen kilpailunumero",1000);
           okk = FALSE;
         }
     }
  } while (!okk);
  *tc = ch;
}

void inputknow(int *kno, int x, int y, wchar_t *wterm, wchar_t *wtc)
{
	char tc, term[40];
	int j;

	wcstooem(term, wterm, 39);
	for (j = 0; wterm[j]; j++)
		if (wterm[j] >= 201 && wterm[j] <= 210)
			term[j] = (char) wterm[j];
	term[39] = 0;
	inputkno(kno, x, y, term, &tc);
	if (tc >= 201 && tc <= 210)
		*wtc = tc;
	else
		*wtc = oemtowchar(tc);
}

