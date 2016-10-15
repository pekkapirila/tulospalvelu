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

#include <stdlib.h>
#include <string.h>

wchar_t *wconcat(const wchar_t *s1, const wchar_t *s2, const wchar_t *s3 /* = NULL */)
{
	static wchar_t str[1000];

	memset(str, 0, sizeof(str));
	wcsncpy(str, s1, sizeof(str)/2-1);
	wcsncpy(str+wcslen(str), s2, sizeof(str)/2-wcslen(str)-1);
	if (s3 != NULL)
		wcsncpy(str+wcslen(str), s3, sizeof(str)/2-wcslen(str)-1);
	return(str);
}

