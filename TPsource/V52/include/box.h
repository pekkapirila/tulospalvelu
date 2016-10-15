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

#define ORDER 8
#define MAXKEYLEN 25
#define PAGESIZE 2*ORDER
#define MAXHEIGHT 8
#define PAGESTACKSIZE 10
#define MAXDATARECSIZE 64

#pragma pack(1)

typedef struct {
        short handle;
        short firstfree;
        short numberfree;
        short int1;
        short int2;
        short recl;
        short numrec;
        char *flnm;
        } datafile;
typedef char takeystr[MAXKEYLEN];
typedef struct {
            short dataref,pageref;
            takeystr key;
        } taitem;
typedef struct {
             short itemsonpage;
             short bckwpageref;
             taitem itemarray[PAGESIZE];
        } tapage;
typedef tapage *tapageptr;
typedef struct {
             short pageref,itemarrindex;
        } tasearchstep;
typedef tasearchstep tapath[MAXHEIGHT];
typedef struct {
              datafile dataf;
              short allowduplkeys;
              short keyl,rr,pp;
              tapath path;
        } indexfile;
typedef indexfile *indexfileptr;
typedef struct {
              tapage page;
              indexfileptr indexfptr;
              short pageref;
              short updated;
        } tastackrec;
typedef tastackrec *tastackrecptr;
typedef tastackrec tapagestack[PAGESTACKSIZE];
typedef short tapagemap[PAGESTACKSIZE];
typedef struct {
             short i,i2,i3,i4,tm;
        } itm;         
typedef union { 
             tastackrec page;
             char r[MAXDATARECSIZE];
             itm ii;
        } tarecordbuffer ;

#ifdef __cplusplus
extern "C" {
#endif

int ioerr(char *fname, int oper, long iores);
int getrec(datafile *datf, unsigned r, void *buffer);
int putrec(datafile *datf, unsigned r, void *buffer);
void makefile(datafile *datf, char *fname, unsigned reclen);
void openfile(datafile *datf, char *fname, unsigned reclen);
void closefile(datafile *datf);
void addrec(datafile *datf, short *r, void *buffer);
void deleterec(datafile *datf, unsigned r);
short  filelen(datafile *datf);
short usedrecs(datafile *datf);
void initindex(void);
void makeindex(indexfile *idxf, char *fname, short keylen, short s);
void openindex(indexfile *idxf, char *fname, short keylen, short s);
void closeindex(indexfile *idxf);
void clearkey(indexfile *idxf);
void nextkey(indexfile *idxf, short *procdatref, char *pkey);
void prevkey(indexfile *idxf, short *procdatref, char *pkey);
void findkey(indexfile *idxf, short *procdatref, char *pkey);
void searchkey(indexfile *idxf, short *procdatref, char *pkey);
void addkey(indexfile *idxf, short procdatref, char *pkey);
void deletekey(indexfile *idxf, short *procdatref, char *pkey);

#ifdef __cplusplus
}
#endif



