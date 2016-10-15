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

long t_time(long ts, int t0)
   {
   long tics;
   long t0tics;

   t0tics = (DAYTICKS * t0) / 24;
   tics = ts - t0tics;
   if (tics < -876120L) tics += DAYTICKS;
   else if (tics > 876120L) tics -= DAYTICKS;
   tics -= tics >>13;
   return ((tics * 225L) >> 12);
   }


