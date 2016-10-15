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
#include <stdlib.h>
#include <string.h>
#include <bstrings.h>
#include <bvideo.h>
#include <tputil.h>
#define TRUE 1
#define FALSE 0
#define NRMAX 300
#define clrln(Y) scclrmsg((Y), 0, 80)

extern int monirivi, iscurpos;

char *rivptr[NRMAX];
static int rm, rl, rluku, rofs, rofs0;
static int rivi, newline;

static void wrapstr(char *s, int y, int x, char ch)
   {
   static int l;
   int r, n, n1, len;
   char *pp;
   static int yksi;

   if (ch != 255) l = strlen(s);
   if (iscurpos<0 || iscurpos>l) iscurpos = l;
   if (newline) {
      for (pp = s; *pp; pp++)
         if (*pp == '|') *pp = '\n';
      }
   pp = s + iscurpos;
   if (ch != 255 || yksi) {
      if (ch != 255) {
         if (rivi >= 0 && ch != '|' && ch != '\n' && pp >= rivptr[rivi]) {
            if (rivi == rm-1 && rivptr[rivi]-s > l+x-79)
               rivptr[rm] = s + l;
            else if(rivptr[rivi+1][-1] == ' ' ||
               rivptr[rivi+1][-1] == '|' || rivptr[rivi+1][-1] == '\n');
            else if ((*rivptr[rivi+1] == ' ' || *rivptr[rivi+1] == '|'
               || *rivptr[rivi+1] == '\n')
               && rivptr[rivi+1]-rivptr[rivi] < 79-x) rivptr[rivi+1]++;
            else if(rivptr[rivi+1][-2] == ' ' ||
               rivptr[rivi+1][-2] == '|' || rivptr[rivi+1][-2] == '\n')
               rivptr[rivi+1]--;
            else goto next;
            yksi = TRUE;
            goto jatko;
            }
         }
      next:
      rivptr[0] = s;
      r = 1;
      while (r < NRMAX - 1 && rivptr[r-1] - s < l) {
         rivptr[r] = rivptr[r-1];
//         if (rivptr[r] - s > l - rl) rivptr[r] = s + l;
//         else
            {
            n = stschind('|', rivptr[r]);
            n1 = stschind('\n', rivptr[r]);
            if (n < 0) n = rl;
            if (n1 < 0) n1 = rl;
            n = min(n, n1);
            if (n < rl - 1)
               rivptr[r] += n + 1;
            else
               while ((n = stschind(' ', rivptr[r])) >= 0 &&
                  rivptr[r] - rivptr[r-1] + n < rl - 1)
                  rivptr[r] += n + 1;
            }
         if (rivptr[r] - rivptr[r-1] <= 0) rivptr[r] += rl;
         if (rivptr[r] - s > l) rivptr[r] = s + l;
         r++;
         }
      yksi = FALSE;
      ch = 0;
      rm = r - 1;
      }
   for (rivi = 0; rivi < NRMAX - 1 && rivptr[rivi+1] - s <= iscurpos &&
      rivptr[rivi+1] < s + strlen(s); rivi++);
   rofs = max(rofs, rivi - rluku + 1);
   rofs = min(rivi, rofs);
   jatko:
   sccurset(y+rivi-rofs,(pp-rivptr[rivi]) + x);
   if (ch != 255 || rofs != rofs0 || yksi) {
      if (yksi && ch != 255) {
         clrln(y + rivi - rofs);
         viwrrect(y+rivi-rofs,x,y+rivi-rofs,x-1+(rivptr[rivi+1]-
            rivptr[rivi]), rivptr[rivi], 7,0,0);
         }
      else {
         yksi = FALSE;
         n = min(rluku, rm - rofs);
         for (r = 0; r < rluku; r++) clrln(y+r);
         for (r = 0; r < n; r++) {
            len = rivptr[r+rofs+1]-rivptr[r+rofs];
            if (rivptr[r+rofs+1][-1] == '\n') len--;
            viwrrect(y+r,x,y+r,x-1+len,rivptr[r+rofs], 7,0,0);
            }
         }
      }
   rofs0 = rofs;
   }

static void wrylos(char *s, int nr)
   {
   int r,i;

   for (r = 0; r < NRMAX - 1 && rivptr[r+1] - s <= iscurpos &&
      rivptr[r+1] < s + strlen(s); r++);
   for (i=0; (i<nr) && (r>0); i++) {
      iscurpos -= rivptr[r] - rivptr[r-1];
      if (iscurpos >= rivptr[r] - s) iscurpos = (rivptr[r] - s) - 1;
      r--;
      }
   }

static void wralas(char *s, int nr)
   {
   int r,i,len;

   len = strlen(s);
   for (r = 0; (r < NRMAX - 1) && (rivptr[r+1] - s <= iscurpos) &&
      (rivptr[r+1] - s <  len); r++);
   for (i=0; i<nr; i++) {
      iscurpos += rivptr[r+1] - rivptr[r];
      if (iscurpos > len) {
         iscurpos = len;
         return;
         }
      r++;
      }
   if (iscurpos >= rivptr[r+1] - s) iscurpos = (rivptr[r+1] - s) - 1;
   }

char *pageedit(char *s, unsigned l, int y, int x, int y1, int x1,
   const char *term, char *tc)
   {
   char term0[20] = "\x03\x05\x06\x09\x12\x18\xFF";

   monirivi = TRUE;
   newline = FALSE;
   rivi = -1;
   rm = 0;
   rl = x1 - x + 1;
   rluku = y1 - y + 1;
   rofs = 0;
   rofs0 = 0;
   strcat(term0,term);
   if (stschind('\n', s) >= 0) newline = TRUE;
   wrapstr(s,y,x,' ');
   do {
      inputstr(s,l,x,y,term0,tc,FALSE);
      switch (*tc) {
         case   3 : wralas(s,2*rluku-rivi+rofs-1);
                    *tc = '\xff';
                    break;
         case   5 : wrylos(s,1);
                    *tc = '\xff';
                    break;
         case   6 : iscurpos = rivptr[rivi+1] - s - 1;
                    *tc = '\xff';
                    break;
         case   9 : iscurpos = rivptr[rivi] - s;
                    *tc = '\xff';
                    break;
         case  18 : wrylos(s,rluku+rivi-rofs-1);
                    *tc = '\xff';
                    break;
         case  24 : wralas(s,1);
                    *tc = '\xff';
                    break;
         }
      wrapstr(s,y,x,*tc);
      }
   while (stschind(*tc,term) == -1);
   rivi = -1;
   wrapstr(s,y,x,' ');
   monirivi =FALSE;
   return(s);
   }
