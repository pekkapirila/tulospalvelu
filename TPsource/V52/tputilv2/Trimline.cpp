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
#include <stdlib.h>

char *trimline(char *line)
{
  char *p;

  for (p = line+strlen(line)-1; p >= line && *p == '\n' || *p == '\r' || *p == ' '; p--)
	 *p = 0;
  return(line);
}

wchar_t *wtrimline(wchar_t *line)
{
  wchar_t *p;

  for (p = line+wcslen(line)-1; p >= line && *p == L'\n' || *p == L'\r' || *p == L' '; p--)
	 *p = 0;
  return(line);
}

