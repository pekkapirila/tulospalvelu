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

wchar_t *wrivptr[NRMAX];
static int rm, rl, rluku, rofs, rofs0;
static int rivi, newline;

static void wrapstr(wchar_t *s, int y, int x, wchar_t ch)
   {
   static int l;
   int r, n, n1, len;
   wchar_t *pp;
   static int yksi;

   if (ch != 255) l = wcslen(s);
   if (iscurpos<0 || iscurpos>l) iscurpos = l;
   if (newline) {
      for (pp = s; *pp; pp++)
         if (*pp == L'|') *pp = L'\n';
      }
   pp = s + iscurpos;
   if (ch != 255 || yksi) {
      if (ch != 255) {
		 if (rivi >= 0 && ch != L'|' && ch != L'\n' && pp >= wrivptr[rivi]) {
			if (rivi == rm-1 && wrivptr[rivi]-s > l+x-79)
			   wrivptr[rm] = s + l;
			else if(wrivptr[rivi+1][-1] == L' ' ||
			   wrivptr[rivi+1][-1] == L'|' || wrivptr[rivi+1][-1] == L'\n');
			else if ((*wrivptr[rivi+1] == L' ' || *wrivptr[rivi+1] == L'|'
			   || *wrivptr[rivi+1] == L'\n')
			   && wrivptr[rivi+1]-wrivptr[rivi] < 79-x) wrivptr[rivi+1]++;
			else if(wrivptr[rivi+1][-2] == L' ' ||
			   wrivptr[rivi+1][-2] == L'|' || wrivptr[rivi+1][-2] == L'\n')
			   wrivptr[rivi+1]--;
			else goto next;
			yksi = TRUE;
			goto jatko;
			}
		 }
	  next:
	  wrivptr[0] = s;
	  r = 1;
	  while (r < NRMAX - 1 && wrivptr[r-1] - s < l) {
		 wrivptr[r] = wrivptr[r-1];
//         if (rivptr[r] - s > l - rl) rivptr[r] = s + l;
//         else
			{
			n = wcswcind(L'|', wrivptr[r]);
			n1 = wcswcind(L'\n', wrivptr[r]);
			if (n < 0) n = rl;
			if (n1 < 0) n1 = rl;
			n = min(n, n1);
			if (n < rl - 1)
			   wrivptr[r] += n + 1;
			else
			   while ((n = wcswcind(L' ', wrivptr[r])) >= 0 &&
				  wrivptr[r] - wrivptr[r-1] + n < rl - 1)
				  wrivptr[r] += n + 1;
			}
		 if (wrivptr[r] - wrivptr[r-1] <= 0) wrivptr[r] += rl;
		 if (wrivptr[r] - s > l) wrivptr[r] = s + l;
		 r++;
		 }
	  yksi = FALSE;
	  ch = 0;
	  rm = r - 1;
	  }
   for (rivi = 0; rivi < NRMAX - 1 && wrivptr[rivi+1] - s <= iscurpos &&
	  wrivptr[rivi+1] < s + wcslen(s); rivi++);
   rofs = max(rofs, rivi - rluku + 1);
   rofs = min(rivi, rofs);
   jatko:
   sccurset(y+rivi-rofs,(pp-wrivptr[rivi]) + x);
   if (ch != 255 || rofs != rofs0 || yksi) {
	  if (yksi && ch != 255) {
		 clrln(y + rivi - rofs);
		 viwrrectw(y+rivi-rofs,x,y+rivi-rofs,x-1+(wrivptr[rivi+1]-
			wrivptr[rivi]), wrivptr[rivi], 7,0,0);
		 }
	  else {
		 yksi = FALSE;
		 n = min(rluku, rm - rofs);
		 for (r = 0; r < rluku; r++) clrln(y+r);
		 for (r = 0; r < n; r++) {
			len = wrivptr[r+rofs+1]-wrivptr[r+rofs];
			if (wrivptr[r+rofs+1][-1] == L'\n') len--;
			viwrrectw(y+r,x,y+r,x-1+len,wrivptr[r+rofs], 7,0,0);
			}
		 }
	  }
   rofs0 = rofs;
   }

static void wrylos(wchar_t *s, int nr)
   {
   int r,i;

   for (r = 0; r < NRMAX - 1 && wrivptr[r+1] - s <= iscurpos &&
	  wrivptr[r+1] < s + wcslen(s); r++);
   for (i=0; (i<nr) && (r>0); i++) {
	  iscurpos -= wrivptr[r] - wrivptr[r-1];
	  if (iscurpos >= wrivptr[r] - s) iscurpos = (wrivptr[r] - s) - 1;
	  r--;
	  }
   }

static void wralas(wchar_t *s, int nr)
   {
   int r,i,len;

   len = wcslen(s);
   for (r = 0; (r < NRMAX - 1) && (wrivptr[r+1] - s <= iscurpos) &&
	  (wrivptr[r+1] - s <  len); r++);
   for (i=0; i<nr; i++) {
	  iscurpos += wrivptr[r+1] - wrivptr[r];
	  if (iscurpos > len) {
		 iscurpos = len;
		 return;
		 }
	  r++;
	  }
   if (iscurpos >= wrivptr[r+1] - s) iscurpos = (wrivptr[r+1] - s) - 1;
   }

wchar_t *wpageedit(wchar_t *s, unsigned l, int y, int x, int y1, int x1,
   const wchar_t *term, wchar_t *tc)
   {
   wchar_t term0[20] = L"\x03\x05\x06\x09\x12\x18\xFF";

   monirivi = TRUE;
   newline = FALSE;
   rivi = -1;
   rm = 0;
   rl = x1 - x + 1;
   rluku = y1 - y + 1;
   rofs = 0;
   rofs0 = 0;
   wcscat(term0,term);
   if (wcswcind(L'\n', s) >= 0) newline = TRUE;
   wrapstr(s,y,x,L' ');
   do {
	  inputwstr(s,l,x,y,term0,tc,FALSE);
	  switch (*tc) {
		 case   3 : wralas(s,2*rluku-rivi+rofs-1);
					*tc = L'\xff';
					break;
		 case   5 : wrylos(s,1);
					*tc = L'\xff';
					break;
		 case   6 : iscurpos = wrivptr[rivi+1] - s - 1;
					*tc = L'\xff';
					break;
		 case   9 : iscurpos = wrivptr[rivi] - s;
					*tc = L'\xff';
					break;
		 case  18 : wrylos(s,rluku+rivi-rofs-1);
					*tc = L'\xff';
					break;
		 case  24 : wralas(s,1);
					*tc = L'\xff';
					break;
		 }
	  wrapstr(s,y,x,*tc);
	  }
   while (wcswcind(*tc,term) == -1);
   rivi = -1;
   wrapstr(s,y,x,L' ');
   monirivi =FALSE;
   return(s);
   }
