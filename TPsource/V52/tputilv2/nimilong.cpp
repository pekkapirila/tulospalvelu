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

#include "tputil.h"

UINT32 nimilong(char *nimi)
{
   char *ns;

   ns = aakjarjstr(nimi);
   return(102400000L * chint(ns[0]) + 2560000L * chint(ns[1]) +
      64000L * chint(ns[2]) + 1600L * chint(ns[3]) + 40L * chint(ns[4])
      + chint(ns[6]));
}

UINT32 wnimilong(wchar_t *wnimi)
{
	char nimi[62];

	wcstooem(nimi, wnimi, 60);
	return(nimilong(nimi));
}

