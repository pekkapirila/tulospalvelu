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

#include <tptype.h>
#include <windows.h>

#define DAYTICKS 1573040U

INT32 timeticks(void)
   {
   SYSTEMTIME stm;
   UINT32 t;

   GetLocalTime(&stm);
   t = (stm.wHour%3)*36000+stm.wMinute*600+stm.wSecond*10 +stm.wMilliseconds/100;
   t = ((DAYTICKS/80U) * t) / 10800U;
   t += (DAYTICKS/8) * (stm.wHour/3);
   return((INT32) t);
   }
