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

#define DAYTICKS 1573040L

long t_time_l(long ts, int t0)
   {
   union {
      long lt;
      struct {
         short l;
         short h;
         } it;
	  } tics;
   long t0tics;

   t0tics = (DAYTICKS * t0) / 24;
   tics.lt = ts - t0tics;
   if (tics.lt < -DAYTICKS/2) tics.lt += DAYTICKS;
   else if (tics.lt >= DAYTICKS/2) tics.lt -= DAYTICKS;
   tics.lt += tics.lt << 2;
   tics.lt -= tics.lt >>13;
   tics.lt += tics.it.h;
   ts = (tics.lt * 225L) >> 11;
   if (ts >= 432000L) ts -= 864000L;
   return (ts);
   }


