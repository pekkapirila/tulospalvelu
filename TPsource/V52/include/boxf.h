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

#include <tptype.h>
#include <windows.h>
#define ORDER 8
#define MAXKEYLEN 25
#define PAGESIZE 2*ORDER
#define MAXHEIGHT 8
#define PAGESTACKSIZE 10
#define TARECLEN (MAXKEYLEN+4)*PAGESIZE+4

typedef unsigned DATAREF;
typedef unsigned short SDATAREF;

#pragma pack(push,1)
typedef struct {
	HANDLE hDatf;
   DATAREF firstfree;
   DATAREF numberfree;
   DATAREF int1;
   DATAREF int2;
   unsigned recl;
   DATAREF numrec;
   char *flnm;
   } datafile;

typedef struct {
   unsigned i,i2,i3,i4,tm;
   } itm;         

typedef union {
   char r[TARECLEN]; /* Tilaa varattava suurimman tietueen verran */
   itm ii;
   } tarecordbuffer ;

typedef char takeystr[MAXKEYLEN];
typedef struct {
            SDATAREF dataref;
            SDATAREF pageref;
            takeystr key;
        } taitem;
typedef struct {
             short itemsonpage;
             SDATAREF bckwpageref;
             taitem itemarray[PAGESIZE];
        } tapage;
typedef tapage *tapageptr;
typedef struct {
             int pageref,itemarrindex;
        } tasearchstep;
typedef tasearchstep tapath[MAXHEIGHT];
typedef struct {
              datafile dataf;
              int allowduplkeys;
              unsigned keyl;
              DATAREF rr;
              int pp;
              tapath path;
        } indexfile;
typedef indexfile *indexfileptr;
typedef struct {
              tapage page;
              indexfileptr indexfptr;
              DATAREF pageref;
              int updated;
        } tastackrec;
typedef tastackrec *tastackrecptr;
typedef tastackrec tapagestack[PAGESTACKSIZE];
typedef int tapagemap[PAGESTACKSIZE];
/*
typedef union { 
             tastackrec page;
             unsigned char r[MAXDATARECSIZE];
             itm ii;
        } tarecordbuffer ;
*/
#pragma pack(pop)

#ifdef __cplusplus
extern "C" {
#endif

//int ioerr(char *fname, int oper, long iores);
int getrec(datafile *datf, DATAREF r, void *buffer);
int putrec(datafile *datf, DATAREF r, void *buffer);
void makefile(datafile *datf, char *fname, unsigned reclen);
void openfile(datafile *datf, char *fname, unsigned reclen);
void closefile(datafile *datf);
void addrec(datafile *datf, DATAREF *r, void *buffer);
void deleterec(datafile *datf, DATAREF r);
DATAREF filelen(datafile *datf);
DATAREF usedrecs(datafile *datf);
void newrec(datafile *datf, DATAREF *r);
void initindex(void);
void makeindex(indexfile *idxf, char *fname, unsigned keylen, int s);
void openindex(indexfile *idxf, char *fname, unsigned keylen, int s);
void closeindex(indexfile *idxf);
void clearkey(indexfile *idxf);
void nextkey(indexfile *idxf, DATAREF *procdatref, char *pkey);
void prevkey(indexfile *idxf, DATAREF *procdatref, char *pkey);
void findkey(indexfile *idxf, DATAREF *procdatref, char *pkey);
void searchkey(indexfile *idxf, DATAREF *procdatref, char *pkey);
void addkey(indexfile *idxf, DATAREF procdatref, char *pkey);
void deletekey(indexfile *idxf, DATAREF procdatref, char *pkey);
void clearkey(indexfile *idxf);
void copypgtoitem(taitem *item, tapageptr pgptr, int i, int keyl);
void copyitemtopg(tapageptr pgptr, taitem *item, int i, int keyl);
void copypgtopg(tapageptr pgptr2, tapageptr pgptr1, int i2, int i1,
   unsigned keyl);

#ifdef __cplusplus
   }
#endif
