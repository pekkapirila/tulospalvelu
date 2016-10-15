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
#include <stdio.h>
#include <stdlib.h>

unsigned lue_v_pv(void)
{
	time_t ltime;
	struct tm *localtm;

	time(&ltime);
	localtm = localtime(&ltime);
	return(localtm->tm_yday+1);
}

void as_v_pv(unsigned v_pv)
{
	time_t ltime;
	struct tm *localtm;
	char st[12];

	time(&ltime);
	localtm = localtime(&ltime);
	if (localtm->tm_yday+1 != (int) v_pv) {
		ltime += (v_pv-1-localtm->tm_yday) * 86400;
		localtm = localtime(&ltime);
		sprintf(st, "DATE %u.%u.%u", localtm->tm_mday, localtm->tm_mon+1,
			localtm->tm_year+1900);
		system(st);
	}
}
