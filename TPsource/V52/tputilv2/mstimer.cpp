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

#include <time.h>
#include <sys/timeb.h>

static time_t alkuaika;

void initmstimer(void)
   {
   struct timeb tb;

   ftime(&tb);
   alkuaika = tb.time;
   }

long mstimer(void)
   {
   struct timeb tb;
   long t;

   ftime(&tb);
   t = (long)(1000*(tb.time-alkuaika) + tb.millitm);
   return(t);
   }

