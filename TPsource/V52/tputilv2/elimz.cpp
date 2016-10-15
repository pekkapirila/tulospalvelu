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

void elimz(char *aikastr)
   {
   if (aikastr[0] == '0') {
      aikastr[0] = ' ';
      if (aikastr[1] == '0') {
         aikastr[1] = ' ';
         aikastr[2] = ' ';
         if (strlen((char *)aikastr) >= 8 && aikastr[3] == '0') {
            aikastr[3] = ' ';
            if (aikastr[4] == '0') {
               aikastr[4] = ' ';
               aikastr[5] = ' ';
               if (aikastr[6] == '0')
                  aikastr[6] = ' ';
               }

            }
         }
      }
	}

void elimwz(wchar_t *aikastr)
	{
	if (aikastr[0] == L'0') {
		aikastr[0] = L' ';
		if (aikastr[1] == L'0') {
			aikastr[1] = L' ';
			aikastr[2] = L' ';
			if (wcslen(aikastr) >= 8 && aikastr[3] == L'0') {
				aikastr[3] = L' ';
				if (aikastr[4] == L'0') {
					aikastr[4] = L' ';
					aikastr[5] = L' ';
					if (aikastr[6] == L'0')
						aikastr[6] = L' ';
					}

				}
			}
		}
	}
