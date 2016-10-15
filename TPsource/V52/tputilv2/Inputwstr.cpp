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
#define TRUE 1
#define FALSE 0
#define clrln(Y) scclrmsg((Y), 0, 80)

static int monirivi = FALSE;
static unsigned iscurpos = 0;
extern INT16 ySize;

wchar_t *inputwstr2(wchar_t *s, unsigned l, int x, int y, const wchar_t *term,
		wchar_t *tc, int numfl)
   {
   int d;
   int nch;
   wchar_t cm, ch, che;
   static wchar_t ifl = TRUE;

   if (!monirivi) {
      d = 1;
      s[l] = 0;
      iscurpos = 0;
      ifl = FALSE;
      nch = wcslen(s);
	  vidspwmsg(y,x,SC_WHITE,SC_BLACK,s);
      sccurset(y,x+nch);
      scwrite('_',l - nch);
      }
   cm = ifl ? L'I' : L'R';
   viwrrectw(ySize-3,78,ySize-3,78,&cm,0,7,0);
   do {
      if (((numfl && s[iscurpos] == L'.') ||
         (numfl == -1 && s[iscurpos] == L',')) &&
         (iscurpos + d <= wcslen(s)))  iscurpos += d;
      d = 1;
      if (!monirivi) sccurset(y,x + iscurpos);
      ch = readkbd_w(&che,1);
      if (monirivi && ch == 13) ch = L'|';
      if (wcswcind(L'+',term) != -1 && ch == L'+') break;
      if (ch >= 32 && wcswcind(ch, term) == -1) {
         if ((iscurpos < l) && (!numfl || wcswcind(ch, L"0123456789-+ ")
            != -1)) {
            if (ifl || ch == L'|') {
               if (wcslen(s) == l) delwchar(s,l-1,1);
               inswchar(ch,s,iscurpos);
               }
            else {
               if (wcslen(s) == iscurpos) inswchar(ch,s,iscurpos);
               else s[iscurpos] = ch;
               }
            iscurpos++;
            }
         else erbeep();
         }
      else switch (ch) {
         case 8   :
         case 127 : if ((iscurpos > 0) && numfl >= 0) {
                        iscurpos--;
                        delwchar(s,iscurpos,1);
                     } 
                     else erbeep(); break;
         case 0   :  ch=254;
                     switch (che) {
                        case 15 : ch = 11; break;       /* backtab */
                        case 59 :                         /* F1 */
                        case 60 :                         /* F2 */
                        case 61 :                         /* F3 */
                        case 62 :                         /* F4 */
                        case 64 :                         /* F6 */ 
                        case 68 : ch = che + 142; break;  /* F10 */
                        case 63 :                         /* F5  */
                        case 72 : ch = 5;
                                  break;                  /*up*/
                        case 65 :                         /* F7 */
                        case 75 : ch = 255;
                                  if (iscurpos > 0) {
                                     iscurpos--;
                                     d = -1;
                                     } 
                                  else erbeep(); break;     /*left*/
                        case 66 :                          /* F8 */
                        case 77 : ch = 255;
                                  if (iscurpos < wcslen(s)) iscurpos++;
                                  else erbeep(); break;    /*right*/
                        case 67 :                          /* F9 */
                        case 80 : ch = 24;
                                  break;               /*down*/
                        case 71 : if(monirivi) ch = 9;
                                  else iscurpos = 0;
                                  break;              /*alkuun*/
                        case 73 : ch = 18; break;      /*pgup*/
                        case 79 : if (monirivi) ch = 6;
                                  else iscurpos = wcslen(s);
                                  break;              /*loppuun*/
                        case 81 : ch = 3; break;      /*pgdn*/
                        case 82 : ch = 255;
                                  if (numfl >= 0) {
                                     ifl = !ifl;
                                     cm = ifl ? L'I' : L'R';
                                     viwrrectw(22,78,22,78,&cm,0,7,0);
                                     }
                                  break;
                        case 83 : if ((iscurpos < wcslen(s)) &&
                                     numfl>=0) {
                                     delwchar(s,iscurpos,1);
                                     }
                                  break;
                       case 115 : ch = 255;
                                  while ((iscurpos > 0) &&
                                     (s[iscurpos-1] == ' ')) iscurpos--;
                                  while ((iscurpos > 0) &&
                                     (s[iscurpos-1] != ' ')) iscurpos--;
                                  break;        /* edellinen sana */
                       case 116 : ch = 255;
                                  while ((iscurpos < wcslen(s)) &&
                                     (s[iscurpos] != ' ')) iscurpos++;
                                  while ((iscurpos < wcslen(s)) &&
                                     (s[iscurpos] == ' ')) iscurpos++;
                                  break;         /* seuraava sana */
                       case 117 :
                       case 166 : if (numfl >= 0) {
                                     delwchar(s,iscurpos,l);
                                     }
                                  break;
                       case 118 : ch = 2; break;     /*Ctrl-PgDn*/
                       case 132 : ch = 19; break;     /*Ctrl-PgUp*/
                        default : erbeep();
                        }                  /* switch(che) */
                        break;             /* of case ch==0 */
         default : if (wcswcind(ch,term) == -1) erbeep();
      }                                   /*  switch(ch)  */
      if (!monirivi) {
         vidspwmsg(y,x,SC_WHITE,SC_BLACK,s);
		 nch = wcslen(s);
         sccurset(y,x+nch);
         scwrite('_',l - nch);
         }
   } while (!monirivi && wcswcind(ch,term) == -1);
   if (!monirivi) {
      iscurpos = wcslen(s);
      sccurset(y,x + iscurpos);
      scwrite(' ',l - iscurpos);
      }
   *tc = ch;
   return(s);
}

/*
wchar_t *inputwstr(wchar_t *ws, unsigned l, int x, int y, const wchar_t *wterm,
		wchar_t *wtc, int numfl)
{
	char str[82]="", term[40], tc;
	int i;

	wcstooem(str, ws, l);
	wcstooem(term, (wchar_t *) wterm, 39);
	for (i = 0; wterm[i]; i++)
		if (wterm[i] >= 201 && wterm[i] <= 210)
			term[i] = (char) wterm[i];
	inputstr(str, l, x, y, term, &tc, numfl);
	if (tc >= 201 && tc <= 210)
		*wtc = tc;
	else
		*wtc = oemtowchar(tc);
	return(oemtowcs(ws, str, l, 0));
}
*/