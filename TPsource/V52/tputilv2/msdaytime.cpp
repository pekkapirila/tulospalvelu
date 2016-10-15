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
#include <stdio.h>
#include <windows.h>

long msdaytime(void)
   {
   SYSTEMTIME stm;

   GetLocalTime(&stm);
   return(3600000L*stm.wHour + 60000L*stm.wMinute + 1000L*stm.wSecond +
	  stm.wMilliseconds);
   }

wchar_t *ISOdatetime(int des, bool zone)
{
   SYSTEMTIME stm, sstm;
   TIME_ZONE_INFORMATION tz;
   static wchar_t ln[100];
   int bias;


   GetSystemTime(&sstm);
   GetLocalTime(&stm);
   GetTimeZoneInformation(&tz);
   swprintf(ln, L"%04d-%02d-%02dT%02d:%02d:%02d.%03d", stm.wYear, stm.wMonth,
	   stm.wDay, stm.wHour, stm.wMinute, stm.wSecond, stm.wMilliseconds);
   if (des > 0 && des <= 3)
	   ln[20 + des] = 0;
   else
	   ln[19] = 0;
   bias = (sstm.wHour - stm.wHour + 36) % 24 - 12;
   if (zone)
	   swprintf(ln+wcslen(ln), L"%c%02d:00", bias > 0 ? L'-' : L'+',  abs(bias));
   return(ln);
}


