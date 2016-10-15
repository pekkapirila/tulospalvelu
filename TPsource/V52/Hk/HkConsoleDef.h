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

#ifndef HKCONSOLEDEF_DEFINED
#define HKCONSOLEDEF_DEFINED

#include "HkDef.h"

#define RV 1

#define LUKUMX  76
#ifdef  EMIT
#define LUKUMY  0
#else
#define LUKUMY  1
#endif

#if !defined(AMPUSUUNN)

#define SNME    1
#define ENME    2
#define ARVE    3
#define LISE    4
#define WRE     5
#define EME     6
#define LAINAE  7
#define SEE     8
#define SELE    9
#define ALUE   10
#define MAAE   11
#define YHDE   12
#define LAHE   13
#define TLSE   14
#define TRKE   15
#define YTLSE  16
#define SJE    17
#define YSJE   18
#define VAE    19
#define SRJE   20
#define NOE    21
#define MAE    22
#define LAH1E  23
#define TAE    24
#define SAKE   25
#define RATAE   26
#define ERE    27
#define MAXE   27

#define NPE    11
#define PIE    12
#define T1E    14
#define T2E    15
#define T3E    16
#define T4E    17
#define SBE    18
#define NQE    19


#else       // AMPUSUUNN

#define NME    1
#define SEE    2
#define ALE    4
#define MME    3
#define LIE    5
#define SAE    7
#define ARE    8
#define RYE    6
#define EME    9
#define LAE    10
#define TUE    17
//#define MA1E   12
#define OS1E   13
//#define SK1E   14
#define TAE    15
//#define LA1E   18
//#define MA2E   16
#define OS2E   14
//#define SK2E   18
#define MAE    11
#define OS0E   12
#define TRE    16
#define NOE    22
#define MAXE   22

#define NOX    19
#define NOY    3
#define SAX    54
#define SAY    3
#define NMX    19
#define NMY    5
#define EMX    51
#define ARX    54
#define ARY    5
#define SEX    19
#define SEY    7
#define ALX    54
#define MMX    38
#define RYX    38
#define RYY    9
#define LIX    19
#define LIY    9
#define LAX    11
#define LAY    19
#define MAX    22
#define MAY    19
#define OSX    33
#define OSY    19  
#define SKX    45
#define SKY    15
#define TRX    44
#define TRY    19
#define TUX    50
#define TUY    19
#define SIX    62
#define SIY    19
#define VAY    TUY-2

#endif     // AMPUSUUNN

#define clrtxt(Y, X0, X1) viwrrect((Y),(X0),(Y),(X1),tyhjarivi,7,0,0)
#define clrln(Y) clrtxt((Y), 0, 79)

typedef struct {
	wchar_t ots[20];
	int xots;
	int x;
	int y;
	int len;
	int active;
	} EDITFLD;


#endif
