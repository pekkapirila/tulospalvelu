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

#include <windows.h>
#include <stdlib.h>
#include <tputil.h>

int aani=2;
int sarjaluku=10;
UCHAR sarjanimi[10], dr1;
int spoolfl,wait,listfl, status_on, maxnblock, trlate, tcomfl;
int t0tics=360000L;
int ok;
UCHAR copybuf[8192];
int iobufr, nchdble, nchsgle;
UCHAR sgle[20], dble[20];
UCHAR savebox[2000];

void lopetus(void) 
{ 
	exit(0);
}
