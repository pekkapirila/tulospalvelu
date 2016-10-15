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

#include <string.h>
#include "tputil.h"

char *keyfromname(char *key, char *nimi, int keylen, int flags)
{
	char *ns, str[80];
	int i;
	unsigned int k;

	strncpy(str, nimi, 79);
	str[79] = 0;
	ns = aakjarjstr(str);
	memset(key,0,keylen);
	for (i = 0; i <= keylen-2; i += 2, ns += 3)
	{
		if (!ns[0])
			break;
		k = 1600U * chint(ns[0]);
		if (ns[1]) {
			k += 40U * chint(ns[1]);
			if (ns[2])
				k += chint(ns[2]);
			}
		if (flags & 1) {
			k ^= 0xffff;
			}
		key[i] = (k >> 8) & 0x00FF;
		key[i+1] = k & 0x00FF;
		if (!ns[1] || !ns[2])
			break;
	}
	return(key);
}

char *keyfromwname(char *key, wchar_t *wnimi, int keylen, int flags)
{
	char nimi[82];

	wcstooem(nimi, wnimi, 80);
	return(keyfromname(key, nimi, keylen, flags));
}
