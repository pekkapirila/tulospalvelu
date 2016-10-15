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

#include <wchar.h>
#include <stdlib.h>
#include <tputil.h>

char wcrtooemch(wchar_t ch)
	{
	char chbuf[4];
	int chlen;

	static char ISOtoOEM[256] =
	{  0,   1,   2,   3,   4,   5,   6,   7,   8,   9,
	  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,
	  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,
	  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,
	  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,
	  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
	  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,
	  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,
	  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,
	  90,  91,  92,  93,  94,  95,  96,  97,  98,  99,
	 100, 101, 102, 103, 104, 105, 106, 107, 108, 109,
	 110, 111, 112, 113, 114, 115, 116, 117, 118, 119,
	 120, 121, 122, 123, 124, 125, 126, 127, 128, 129,
	 130, 131, 132, 133, 134, 135, 136, 137, 138, 139,
	 140, 141, 142, 143, 144, 145, 146, 147, 148, 149,
	 150, 151, 152, 153, 154, 155, 156, 157, 158, 159,
	 160, 161, 162, 163, 164, 165, 179, 180, 181, 182,
	 183, 184, 185, 186, 187, 188, 189, 190, 191, 192,
	 193, 194, 195, 196, 197, 198, 199, 200, 201, 202,
	 203, 204, 205,  65,  65,  65, 142, 143, 146, 128,
	 144, 144, 144,  69,  73,  73,  73,  73,  68, 165,
	  79,  79,  79,  79, 153, 215, 153,  85,  85,  85,
	 154,  89, 112, 225, 133, 160, 131,  97, 132, 134,
	 145, 135, 138, 130, 136, 137, 141, 161, 140, 139,
	 111, 164, 149, 162, 147, 111, 148, 247, 148, 151,
	 163, 150, 129, 152, 254, 152};

   chlen = wctomb(chbuf, ch);
   if (chlen != 1) {
	   chbuf[0] = 254;
   }
   return(ISOtoOEM[chbuf[0]]);
   }

char *wcstooem(char *out, wchar_t *in, int len)
   {
   char *p;
   int i;

   for (i = 0, p = out; i < len; i++, in++, p++) {
		*p = wcrtooemch(*in);
		if (*in == 0)
			break;
	   }
   return(out);
   }

