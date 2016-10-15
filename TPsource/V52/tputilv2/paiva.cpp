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

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <tputil.h>

char *paiva(void)
{
	time_t clock;
	struct tm *now;
	static char ct[10] = "";

	time(&clock);
	now = localtime(&clock);
	sprintf(ct, "%02d.%02d.%02d", now->tm_mday, now->tm_mon+1,
																now->tm_year%100);
	return(ct);
}

wchar_t *wpaiva(void)
{
	static wchar_t wct[10];

	ansitowcs(wct, paiva(), 10);
	return(wct);
}

