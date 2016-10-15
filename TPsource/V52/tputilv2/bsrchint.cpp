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
#define MAXLEVEL 500

INT bsrchint(INT32 x, void *base, INT nelem, INT width, INT pos, INT fl32)
   {
   INT32 w, px;
   INT l, r, p;

   if (!nelem) return(-1);
   w = width;
   r=nelem-1;
   switch (fl32) {
      case 0 :
         px = *(INT16 *) ((char huge *)base + r*w + pos);
         break;
      case 1 :
         px = *(INT32 *) ((char huge *)base + r*w + pos);
         break;
      case 2 :
         px = *(UINT16 *) ((char huge *)base + r*w + pos);
         break;
      case 3 :
         px = *(UINT32 *) ((char huge *)base + r*w + pos);
         break;
      }
   if (x == px) return(r);
   if (x > px) return(-nelem-1);
   l=0;
   p=-1;
   while (p < 0 || r > l) {
      p = (r + l) / 2;
      switch (fl32) {
         case 0 :
            px = *(INT16 *) ((char huge *)base + p*w + pos);
            break;
         case 1 :
            px = *(INT32 *) ((char huge *)base + p*w + pos);
            break;
         case 2 :
            px = *(UINT16 *) ((char huge *)base + p*w + pos);
            break;
         case 3 :
            px = *(UINT32 *) ((char huge *)base + p*w + pos);
            break;
         }
      if (x > px && l < p) l = p;
      else if (x < px && r > p) r = p;
      else break;
      }
   if (x == px) return(p);
   if (x > px) return(-p-2);
   return(-p-1);
   }


