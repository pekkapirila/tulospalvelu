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

#include <malloc.h>
#include <string.h>
#include <tptype.h>
#define MAXLEVEL 1000

void qsortint(void *base, INT nelem, INT width, INT pos, INT fl32)
   {
   char *i1, *i2, *ir;
   char *v, *t, *b;
   INT32 w, ero;
   INT i, l, r, p;
   INT st[MAXLEVEL];

   l=0;
   w = width;
   r=nelem-1;
   p=1;
   v = (char *) malloc(width);
   t = (char *) malloc(width);
   b = (char *)base;
   do {
      if (r > l) {
         i1 = b + (l - 1) * w;
         i2 = b + r*w;
         ir = i2;
         memmove(v, ir, width);
         do {
            do {
               i1 += width;
               switch (fl32) {
                  case 0 :
                     ero = *(INT16 *)(v + pos) - *(INT16 *)(i1 + pos);
                     break;
				  case 1 :
					 if (*(INT32 *)(v + pos) > *(INT32 *)(i1 + pos))
						 ero = 1;
					 else if (*(INT32 *)(v + pos) < *(INT32 *)(i1 + pos))
						 ero = -1;
					 else
						 ero = 0;
					 break;
				  case 2 :
					 if (*(UINT16 *)(v + pos) > *(UINT16 *)(i1 + pos)) ero = 1;
					 else if (*(UINT16 *)(v + pos) < *(UINT16 *)(i1 + pos)) ero = -1;
					 else ero = 0;
					 break;
				  case 3 :
					 if (*(UINT32 *)(v + pos) > *(UINT32 *)(i1 + pos)) ero = 1;
					 else if (*(UINT32 *)(v + pos) < *(UINT32 *)(i1 + pos)) ero = -1;
					 else ero = 0;
					 break;
				  }
			   } while (ero > 0);
			do {
			   i2 -= width;
			   switch (fl32) {
				  case 0 :
					 ero = *(INT16 *)(v + pos) - *(INT16 *)(i2 + pos);
					 break;
				  case 1 :
					 if (*(INT32 *)(v + pos) > *(INT32 *)(i2 + pos)) ero = 1;
					 else if (*(INT32 *)(v + pos) < *(INT32 *)(i2 + pos)) ero = -1;
					 else ero = 0;
					 break;
				  case 2 :
					 if (*(UINT16 *)(v + pos) > *(UINT16 *)(i2 + pos)) ero = 1;
					 else if (*(UINT16 *)(v + pos) < *(UINT16 *)(i2 + pos)) ero = -1;
					 else ero = 0;
					 break;
				  case 3 :
					 if (*(UINT32 *)(v + pos) > *(UINT32 *)(i2 + pos)) ero = 1;
					 else if (*(UINT32 *)(v + pos) < *(UINT32 *)(i2 + pos)) ero = -1;
					 else ero = 0;
					 break;
				  }
			   } while (ero < 0 && i2 > i1);
			if (i2 <= i1) break;
			memmove(t, i1, width);
			memmove(i1, i2, width);
			memmove(i2, t, width);
            } while (1);
         memmove(t, i1, width);
         memmove(i1, ir, width);
		 memmove(ir, t, width);
         i = (UINT16) ((i1 - b) / width);
         if (p > MAXLEVEL-2) goto lopeta;
         if (i-l > r-i) {
            st[p++] = l;
            st[p++] = i-1;
            l = i+1;
            }
         else {
            st[p++] = i+1;
            st[p++] = r;
            r = i-1;
            }
         }
      else {
         r = st[--p];
         l = st[--p];
         }
      } while ( p >= 0 );
   lopeta:
   free(v);
   free(t);
   }


