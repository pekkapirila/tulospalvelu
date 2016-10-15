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

#include <boxr.h>

#define MAXKEYLEN 10
#define ORDER 16
#define PAGESIZE 2*ORDER
#define MAXHEIGHT 5

typedef struct {
            int dataref,pageref;
            UCHAR key[MAXKEYLEN];
        } taitem;
typedef struct {
             int itemsonpage;
             int bckwpageref;
             int dataref[PAGESIZE];
             int pageref[PAGESIZE];
             UCHAR key[2];
        } tapage;
typedef tapage *tapageptr;
typedef struct {
        int o, s;
        } fptr;
typedef union {
        tapageptr dp;
        fptr os;
        } darrref;
typedef struct {
        int recl;
        int maxnpage;
        int numrec;
        darrref *ptr;
        } dataarr;
typedef struct {
             int pageref,itemarrindex;
        } tasearchstep;
typedef tasearchstep tapath[MAXHEIGHT];
typedef struct {
              dataarr dataf;
              int allowduplkeys;
              int keyl,rr,pp;
              tapath path;
        } indexfile;
typedef indexfile *indexfileptr;

#ifdef __cplusplus_x
extern "C" {
#endif

void initindex(void);
void makeindex(indexfile *idxf, char *fname, int keylen, int s);
void openindex(indexfile *idxf, char *fname, int keylen, int s);
void closeindex(indexfile *idxf);
void clearkey(indexfile *idxf);
int nextkey(indexfile *idxf, int *procdatref, UCHAR *pkey);
int prevkey(indexfile *idxf, int *procdatref, UCHAR *pkey);
int findkey(indexfile *idxf, int *procdatref, UCHAR *pkey);
int searchkey(indexfile *idxf, int *procdatref, UCHAR *pkey);
int addkey(indexfile *idxf, int procdatref, UCHAR *pkey);
int deletekey(indexfile *idxf, int *procdatref, UCHAR *pkey);
void tagetpage(indexfile *idxf, int r, tapageptr *pgptr);
void tanewpage(indexfile *idxf, int *r, tapageptr *pgptr);
void tareturnpage(indexfile *idxf, int pageref);
int tacompkeys(UCHAR *k1, UCHAR *kf2, int dr1, int dr2, int dup,
   int klen);
void clearkey(indexfile *idxf);
void copypgtoitem(taitem *item, tapageptr pgptr, int i, int keyl);
void copyitemtopg(tapageptr pgptr, taitem *item, int i, int keyl);
void copypgtopg(tapageptr pgptr2, tapageptr pgptr1, int i2, int i1,
   int keyl);

#ifdef __cplusplus_x
   }
#endif

