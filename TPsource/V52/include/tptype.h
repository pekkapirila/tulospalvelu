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

#ifndef TPTYPE_DEFINED
#define TPTYPE_DEFINED

#define near
#define huge
#define far
#ifdef __linux__
#define cdecl
#define O_BINARY 0
typedef unsigned long DWORD;
typedef int HANDLE;
typedef void * LPVOID;
#endif

typedef unsigned char UCHAR;
typedef int INT;
typedef unsigned int UINT;
typedef signed char INT8;
typedef unsigned char UINT8;
typedef short INT16;
typedef unsigned short UINT16;
typedef int INT32;
typedef unsigned int UINT32;
typedef unsigned long ULONG;
#define I64MAXINT 2147483648LL
#define I64MAXUINT 4294967296LL

#endif
