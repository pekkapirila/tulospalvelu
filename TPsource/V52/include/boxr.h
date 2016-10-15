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

#ifndef BOXRDEFINED
#define BOXRDEFINED

#ifdef WIN32
#include <windows.h>
#endif
#include "tptype.h"

#pragma pack(push,1)
//#pragma pack(1)
typedef unsigned int DATAREF;

typedef struct {
#ifdef WIN32
   HANDLE hDatf;
#else
   HANDLE handle;
#endif
   DATAREF firstfree;
   DATAREF numberfree;
   DATAREF int1;
   DATAREF int2;
   unsigned recl;
   DATAREF numrec;
   char *flnm;
   } datafile;

typedef struct {
   UINT32 i,i2,i3,i4,tm;
   } itm;         

typedef union {
   char r[2]; /* Tilaa varattava suurimman tietueen verran */
   itm ii;
   } tarecordbuffer ;

#pragma pack(pop)

#ifdef __cplusplus_x
extern "C" {
#endif

//int ioerr(char *fname, int oper, long iores);
int getrec(datafile *datf, DATAREF r, void *buffer);
int putrec(datafile *datf, DATAREF r, void *buffer);
int makefile(datafile *datf, char *fname, unsigned reclen);
int openfile(datafile *datf, char *fname, unsigned reclen);
int makefile_w(datafile *datf, wchar_t *fname, unsigned reclen);
int openfile_w(datafile *datf, wchar_t *fname, unsigned reclen);
void closefile(datafile *datf);
void addrec(datafile *datf, DATAREF *r, void *buffer);
void addrec0(datafile *datf, DATAREF *r, void *buffer);
void deleterec(datafile *datf, DATAREF r);
DATAREF filelen(datafile *datf);
DATAREF usedrecs(datafile *datf);
void newrec(datafile *datf, DATAREF *r);

#ifdef __cplusplus_x
   }
#endif

#endif

