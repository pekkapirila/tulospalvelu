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

#include "VDeclare.h"
#include <bvideo.h>

static scr *helps[11];
#ifndef __linux__
static char saveline[80];
#else
static USHORT saveline[80];
#endif
extern scr help0;
extern scr help1;
extern scr help2;
extern scr help3;
extern scr help4;
extern scr help5;
extern scr help6;
extern scr help7;
extern scr help8;

void prscr(scr *s)
{
   int i;
   char line[82],*p1, *p2;

   for (i=0; i<s->nl; i++) {
      p2 = s->l[i].t;
      p1 = line;
      while (*p2) *(p1++) = *(p2++);
      viwrrect(s->l[i].r, s->l[i].c,s->l[i].r, s->l[i].c+p1-line-1,line,
         7,0,0);
      }
}

void help(int ih)
{
   int i, x, y, h, l, tcomfl0;
   char ch;
#ifdef MAALI
   static char txt[] =
      "Valitse sivu 1, 2, 3, 4, 5, 6, 7, 8 tai 9,  <Esc> lopettaaksesi";
   static char val[] = "123456789\x1B";
#else
   static char txt[] =
      "Valitse sivu 1, 2, 3, 4, 5, 6 tai 7,  <Esc> lopettaaksesi";
   static char val[] = "1234567\x1B";
#endif

   tcomfl0 = tcomfl;
   tcomfl = TRUE;
   helps[0] = &help0;
   helps[1] = &help1;
   helps[2] = &help2;
   helps[3] = &help3;
   helps[4] = &help4;
   helps[5] = &help5;
   helps[6] = &help6;
#ifdef MAALI
   helps[7] = &help7;
   helps[8] = &help8;
#endif
   sccurst(&y, &x, &h, &l);
   virdrect(2,0,20,64,savebox,0);
   virdrect(22,0,22,79,saveline,0);
   do {
      for (i=2; i<21; i++) scclrmsg(i,0,65);
      prscr(helps[ih]);
      ch = ' ';
      selectopt(txt,val,&ch);
      if (ch == ESC) break;
      ih = ch - '1';
      }
   while (1);
#ifndef __linux__
   viwrrect(2,0,20,64,savebox,7,0,0);
   viwrrect(22,0,22,79,saveline,7,0,0);
#else   
   virewrrect(2,0,20,64,savebox,0);
   virewrrect(22,0,22,79,saveline,0);
#endif   
   sccurset(y,x);
   tcomfl = tcomfl0;
}

