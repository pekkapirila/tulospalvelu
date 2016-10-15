#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#pragma hdrstop
#include <sys\stat.h>
#include "boxf.h"
#include <tputil.h>
#define TRUE 1
#define FALSE 0
#define MAXNBLOCK 1024
#define UNDERSCORE  =  '_'

void lopetus(void);

static tapagestack tapagestk;
static tapagemap tapgmap;
static int  passup;
static tapageptr pageptr1, pageptr2;
static taitem procitem1, procitem2;
static tarecordbuffer tarecbuf;
extern INT32 maaliajat[10];
extern int ok;
int ainauusirec = 0;

void tacompress(void *buf, int n)
{
	int i;
	unsigned *pint;
	unsigned short *pshort;

	pint = (unsigned *)buf;
	pshort = (unsigned short *) buf;
	for (i = 0; i < n; i++, pshort++, pint++) *pshort = (unsigned short) *pint;
	for (i = 0; i < n; i++, pshort++) *pshort = 0;
}

void taexpand(void *buf, int n)
{
	int i;
	unsigned *pint;
	unsigned short *pshort;

	pint = (unsigned *)buf;
	pshort = (unsigned short *) buf;
	for (i = n-1; i >= 0; i--) {
		if (pshort[i] == 0xffff) pint[i] = 0xffffffff;
		else pint[i] = pshort[i];
	}
}

int getrec(datafile *dataf, DATAREF r, void *buffer)
{
DWORD ls, lrecl;

  lrecl = r * dataf->recl;
  ls = SetFilePointer(dataf->hDatf, lrecl, NULL, FILE_BEGIN);
  if (ls != lrecl) {
     perr(dataf->flnm, GetLastError(), "getrec", r);
     return(-1);
  }
  ReadFile(dataf->hDatf, buffer, dataf->recl, &ls, NULL);
  if (ls < dataf->recl){
     perr(dataf->flnm, GetLastError(), "getrec", r);
     return(-1);
  }
  return(0);
}

int putrec(datafile *dataf, DATAREF r, void *buffer)
{
DWORD ls, lrecl;

  lrecl = r * dataf->recl;
  ls = SetFilePointer(dataf->hDatf, lrecl, NULL, FILE_BEGIN);
  if (ls != lrecl) {
     perr(dataf->flnm, GetLastError(), "putrec", r);
     return(-1);
  }
  WriteFile(dataf->hDatf, buffer, dataf->recl, &ls, NULL);
  if (ls < dataf->recl){
     perr(dataf->flnm, GetLastError(), "putrec", r);
     return(-1);
  }
  return(0);
}

void makefile(datafile *dataf, char *fname, unsigned reclen)
{

	memset(&tarecbuf, 0, sizeof(tarecordbuffer));
	dataf->hDatf = CreateFile(fname, GENERIC_WRITE | GENERIC_READ, 
		FILE_SHARE_READ, NULL, CREATE_ALWAYS, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);

	if(dataf->hDatf == INVALID_HANDLE_VALUE) {
		ok = FALSE;
		perr(fname, GetLastError(), "makefile", 0);
	}
	else {
		dataf->recl = reclen;
	    dataf->firstfree = 0xFFFFFFFF;
		dataf->numberfree = 0;
		dataf->int1 = 0;
		dataf->int2 = 0;
		memmove(&tarecbuf,&(dataf->firstfree),4*sizeof(DATAREF));
		tacompress(&tarecbuf, 4);
		putrec(dataf,0,&tarecbuf);
		dataf->numrec = 1;
		dataf->flnm = fname;
		ok = TRUE;
	}
}

void openfile(datafile *dataf, char *fname, unsigned reclen)
   {
   unsigned flen;
   DWORD Err = 0;
   char msg[81];

	ok = FALSE;
	dataf->hDatf = CreateFile(fname, GENERIC_WRITE | GENERIC_READ, 
		FILE_SHARE_READ, NULL, OPEN_EXISTING, 
		FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);
	if (dataf->hDatf == INVALID_HANDLE_VALUE) {
		if ((Err = GetLastError()) != ERROR_FILE_NOT_FOUND) {
			perr(fname, Err, "openfile", 0);
		}
	}
	else {
		if((flen = GetFileSize(dataf->hDatf, NULL)) >= reclen) {
			dataf->recl = reclen;
			getrec(dataf,0,&tarecbuf);
			taexpand(&tarecbuf, 4);
			memmove(&(dataf->firstfree),&tarecbuf,4*sizeof(DATAREF));
			memcpy(maaliajat, &tarecbuf.ii.tm, min(reclen-4*sizeof(DATAREF),40));
			dataf->numrec = flen / reclen;
			dataf->flnm = fname;
			if (ainauusirec || ((dataf->firstfree == 0xffffffff || (dataf->numberfree > 0 &&
				(dataf->firstfree > 0 && dataf->firstfree < dataf->numrec)))
				&& reclen * dataf->numrec == flen
				&& dataf->int2 == dataf->numrec)) {
				ok = TRUE;
			}
		}
		if (!ok) {
			CloseHandle(dataf->hDatf);
			sprintf(msg, "Tiedoston %s otsikkoalue virheellinen", fname);
			writeerror(msg, 0);
		}
    }
}

void closefile(datafile *dataf)
{
  if (dataf->recl < 8) return;
  dataf->int2 = dataf->numrec;
  memmove(&tarecbuf,&dataf->firstfree,4*sizeof(DATAREF));
  memcpy(&tarecbuf.ii.tm, maaliajat, min(dataf->recl-4*sizeof(DATAREF),40));
  tacompress(&tarecbuf, 4);
  putrec(dataf,0,&tarecbuf);
  CloseHandle(dataf->hDatf);
}

void newrec(datafile *dataf, DATAREF *r)
{
  if (ainauusirec || dataf->firstfree == 0xffffffff)
  {
    *r = dataf->numrec++;
  }
  else
  {
    *r = dataf->firstfree;
    getrec(dataf,*r,&tarecbuf);
    dataf->firstfree = tarecbuf.ii.i & 0xffff;
    if (dataf->firstfree == 0xffff)
       dataf->firstfree = 0xffffffff;
    dataf->numberfree--;
  }
}

void addrec(datafile *dataf, DATAREF *r, void *buffer)
{
  newrec(dataf,r);
  putrec(dataf,*r,buffer);
}

void deleterec(datafile *dataf, DATAREF r)
{
  tarecbuf.ii.i = dataf->firstfree;
  putrec(dataf,r,&tarecbuf);
  dataf->firstfree = r;
   if (dataf->firstfree == 0xffff)
      dataf->firstfree = 0xffffffff;
  dataf->numberfree++;
}

DATAREF  filelen(datafile *dataf)
{
  return(dataf->numrec);
}

DATAREF usedrecs(datafile *dataf)
{
  return(dataf->numrec - dataf->numberfree - 1);
}

/*
void pgdump(indexfile *idxf)
{
   int i,j;
   extern FILE *lstf;

   fprintf(lstf,"\nroot = %d, firstfr = %ud, numused = %ud\n", idxf->rr,
      idxf->dataf.firstfree, usedrecs(&idxf->dataf));
   for (i=0; i<PAGESTACKSIZE; i++){
      fprintf(lstf, "%2d %6ud %3d  %3d  %6ud\n", i,
         tapagestk[i].indexfptr, tapagestk[i].pageref,
         tapagestk[i].page.itemsonpage, tapagestk[i].page.bckwpageref);
      for (j=0; j<tapagestk[i].page.itemsonpage; j++)
         fprintf(lstf, "%10d %6ud %6ud %3d %7.7s\n", j,
            tapagestk[i].page.itemarray[j].pageref,
            tapagestk[i].page.itemarray[j].dataref,
            tapagestk[i].page.itemarray[j].key[0],
            tapagestk[i].page.itemarray[j].key+1);
      }
}
*/

void initindex(void)
{
int  i ;

  for ( i = 0 ; i < PAGESTACKSIZE ; i++)
  {
    tapagestk[i].indexfptr = NULL;
    tapagestk[i].updated = FALSE;
    tapgmap[i] = i + 1;
  }
}

static void tapack(tapage *page, unsigned keyl)
{
int  i ;
char *p;

  p = (char *)page;
  if( keyl != MAXKEYLEN )
    for ( i = 0 ; i < PAGESIZE ; i++) {
      memmove(&p[(i) * (keyl+4) + 4],&(page->itemarray[i]),keyl+4);
      }
}

static void taunpack(tapage *page, unsigned keyl)
{
int  i ;
char *p;

  p = (char *)page;
  if( keyl != MAXKEYLEN )
    for ( i = PAGESIZE - 1; i >= 0 ; i--) {
      memmove(&(page->itemarray[i]),&p[(i) * (keyl+4) + 4], keyl+4);
      }
}

void makeindex(indexfile *idxf, char *fname, unsigned keylen, int s)
{
unsigned  k ;

  k = (keylen + 4)*PAGESIZE + 4;
  makefile(&(idxf->dataf),fname,k);
  idxf->allowduplkeys = (s != 0);
  idxf->keyl = keylen;
  idxf->rr = 0;
  idxf->pp = 0;
}

void openindex(indexfile *idxf, char *fname, unsigned keylen, int s)
{
unsigned  k ;

  k = (keylen + 4) * PAGESIZE + 4;
  openfile(&(idxf->dataf),fname,k);
  idxf->allowduplkeys = (s != 0);
  idxf->keyl = keylen;
  idxf->rr = idxf->dataf.int1;
  idxf->pp = 0;
}

void closeindex(indexfile *idxf)
{
int  i ;

  for ( i = 0 ; i < PAGESTACKSIZE ; i++)
      if( tapagestk[i].indexfptr == idxf )
      {
        tapagestk[i].indexfptr = NULL;
        if( tapagestk[i].updated )
        {
          tapack(&tapagestk[i].page,idxf->keyl);
          putrec(&idxf->dataf,tapagestk[i].pageref,&tapagestk[i].page);
          tapagestk[i].updated = FALSE;
        }
      }
  idxf->dataf.int1 = idxf->rr;
  closefile(&(idxf->dataf));
}

static void talast(int i)
{
int  j,k ;

  j = 1;
  while ( (tapgmap[j-1] != i) && (j < PAGESTACKSIZE) )
    j = j + 1;
  for ( k = j ; k <= PAGESTACKSIZE - 1 ; k++)
    tapgmap[k-1] = tapgmap[k];
  tapgmap[PAGESTACKSIZE-1] = i;
}

static void tagetpage(indexfile *idxf, DATAREF r, tapageptr *pgptr)
{
int  i     ;
int  found ;

  i = 0;
  do {
    found = (tapagestk[i].indexfptr == idxf) &&
            (tapagestk[i].pageref == r);
    i++;
  } while (!( found || (i == PAGESTACKSIZE)));

  if( !found )
  {
      i = tapgmap[0] - 1;
      if( tapagestk[i].updated )
      {
         tapack(&tapagestk[i].page,tapagestk[i].indexfptr->keyl);
         putrec(&(tapagestk[i].indexfptr->dataf),
                     tapagestk[i].pageref,&tapagestk[i].page);
      }
      getrec(&(idxf->dataf),r,&tapagestk[i].page);
      taunpack(&tapagestk[i].page,idxf->keyl);
      tapagestk[i].indexfptr = idxf;
      tapagestk[i].pageref = r;
      tapagestk[i].updated = FALSE;
      i++;
  }
  talast(i);
  *pgptr = (tapageptr) &(tapagestk[i-1]);
}

static void tanewpage(indexfile *idxf, DATAREF *r, tapageptr *pgptr)
{
int  i ;

  i = tapgmap[0] - 1;
  {
    if( tapagestk[i].updated )
    {
      tapack(&tapagestk[i].page,tapagestk[i].indexfptr->keyl);
      putrec(&(tapagestk[i].indexfptr->dataf),tapagestk[i].pageref,
                     &tapagestk[i].page);
    }
    newrec(&(idxf->dataf),r);
    tapagestk[i].indexfptr = idxf;
    tapagestk[i].pageref = *r;
    tapagestk[i].updated = FALSE;
  }
  talast(i+1);
  *pgptr = (tapageptr) &(tapagestk[i]);
}

static void taupdatepage(tapageptr pgptr)
{
tastackrecptr p;

  p = (tastackrecptr) pgptr;
  p->updated = TRUE;
}

static void tareturnpage(tapageptr pgptr)
{
tastackrecptr p;

    p = (tastackrecptr) pgptr;
    deleterec(&(p->indexfptr->dataf),p->pageref);
    p->indexfptr = NULL;
    p->updated = FALSE;
}

static int tacompkeys(char *k1, char *k2, unsigned dr1, unsigned dr2,
   int dup, int klen)
{
   int kl;

  kl = memcmp(k1,k2,klen);
  if(kl == 0) {
    if (dup) {
       if (dr1 == dr2) return(0);
       else if ((long) dr1 - (long) dr2 > 0) return(1);
       else return(-1);
       }
    else return(0);
  }
  else
    if(kl > 0) return(1);
    else return(- 1);
}

void clearkey(indexfile *idxf)
{
  idxf->pp = 0;
}

void nextkey(indexfile *idxf, DATAREF *procdatref, char *pkey)
{
DATAREF  r;
tapageptr pagptr;

  {
    if( idxf->pp == 0 )
      r = idxf->rr;
    else
    {
      tagetpage(idxf,idxf->path[idxf->pp-1].pageref,&pagptr);
      r = pagptr->itemarray[idxf->path[idxf->pp-1].itemarrindex-1].
                                                             pageref;
    }
    while ( r != 0 )
    {
      idxf->pp = idxf->pp + 1;
      {
        idxf->path[idxf->pp-1].pageref = r;
        idxf->path[idxf->pp-1].itemarrindex = 0;
      }
      tagetpage(idxf,r,&pagptr);
      r = pagptr->bckwpageref;
    }
    if( idxf->pp != 0 )
    {
      while ((idxf->pp > 1) &&
         (idxf->path[idxf->pp-1].itemarrindex == pagptr->itemsonpage))
      {
        idxf->pp--;
        tagetpage(idxf,idxf->path[idxf->pp-1].pageref,&pagptr);
      }
      if( idxf->path[idxf->pp-1].itemarrindex < pagptr->itemsonpage )
        {
          memcpy(pkey, pagptr->itemarray[idxf->path[idxf->pp-1].
             itemarrindex].key,idxf->keyl);
          *procdatref = pagptr->
               itemarray[idxf->path[idxf->pp-1].itemarrindex].dataref;
          idxf->path[idxf->pp-1].itemarrindex++;
        }
      else idxf->pp = 0;
    }
    ok = (idxf->pp != 0);
  }
}

void prevkey(indexfile *idxf, DATAREF *procdatref, char *pkey)
{
DATAREF  r;
tapageptr pagptr;

  {
    if( idxf->pp == 0 )
      r = idxf->rr;
    else
      {
        tagetpage(idxf,idxf->path[idxf->pp-1].pageref,&pagptr);
        idxf->path[idxf->pp-1].itemarrindex--;
        if( idxf->path[idxf->pp-1].itemarrindex == 0 )
          r = pagptr->bckwpageref;
        else r = pagptr->itemarray[idxf->path[idxf->pp-1].
                                          itemarrindex-1].pageref;
      }
    while ( r != 0 )
    {
      tagetpage(idxf,r,&pagptr);
      idxf->path[idxf->pp].pageref = r;
      idxf->path[idxf->pp].itemarrindex = pagptr->itemsonpage;
      idxf->pp++;
      r = pagptr->itemarray[pagptr->itemsonpage-1].pageref;
    }
    if( idxf->pp != 0 )
    {
      while ( (idxf->pp > 1) &&
         (idxf->path[idxf->pp-1].itemarrindex == 0) )
      {
        idxf->pp--;
        tagetpage(idxf,idxf->path[idxf->pp-1].pageref,&pagptr);
      }
      if( idxf->path[idxf->pp-1].itemarrindex > 0 )
        {
          memcpy(pkey, pagptr->itemarray[idxf->path[idxf->pp-1].
             itemarrindex-1].key, idxf->keyl);
          *procdatref = pagptr->
             itemarray[idxf->path[idxf->pp-1].itemarrindex-1].dataref;
        }
      else idxf->pp = 0;
    }
    ok = (idxf->pp != 0);
  }
}

static void tafindkey(indexfile *idxf, DATAREF *procdatref, char *pkey)
{
int  c,k,l,r;
DATAREF prpgref;
tapageptr pagptr;

  {
    ok = FALSE;
    idxf->pp = 0;
    prpgref = idxf->rr;
    while ( (prpgref != 0) && ! ok )
    {
      idxf->pp++;
      idxf->path[idxf->pp-1].pageref = prpgref;
      tagetpage(idxf,prpgref,&pagptr);
      {
        l = 1;
        r = pagptr->itemsonpage;
        while (l <= r) {
          k = (l + r) / 2;
          c = tacompkeys(pkey,
                          pagptr->itemarray[k-1].key,
                          0,
                          pagptr->itemarray[k-1].dataref,
                          idxf->allowduplkeys,
                          idxf->keyl);
          if (c <= 0) r = k - 1;
          if (c >= 0) l = k + 1;
        }
        if( l - r > 1 )
        {
          *procdatref = pagptr->itemarray[k-1].dataref;
          r = k;
          ok = TRUE;
        }
        if( r == 0 )  prpgref = pagptr->bckwpageref;
        else prpgref = pagptr->itemarray[r-1].pageref;
      }
      idxf->path[idxf->pp-1].itemarrindex = r;
    }
    if(!ok && (idxf->pp > 0))
    {
      while ((idxf->pp > 1) &&
         (idxf->path[idxf->pp-1].itemarrindex == 0)) idxf->pp--;
      if (idxf->path[idxf->pp-1].itemarrindex == 0) idxf->pp = 0;
    }
  }
}

void findkey(indexfile *idxf, DATAREF *procdatref, char *pkey)
{
char tempkey[MAXKEYLEN];

   tafindkey(idxf,procdatref,pkey);
   if(!ok && idxf->allowduplkeys )
   {
      memcpy(tempkey, pkey, idxf->keyl);
      nextkey(idxf,procdatref,pkey);
      ok = (ok && !memcmp(pkey, tempkey, idxf->keyl));
   }
}

void searchkey(indexfile *idxf, DATAREF *procdatref, char *pkey)
{

   tafindkey(idxf,procdatref,pkey);
   if(!ok) nextkey(idxf,procdatref,pkey);
}

static void insert(indexfile *idxf, DATAREF prpgref1, int r)
{
int i;
DATAREF prpgref2;

  tagetpage(idxf,prpgref1,&pageptr1);
  if(pageptr1->itemsonpage < PAGESIZE )
  {
      pageptr1->itemsonpage++;
      for ( i = pageptr1->itemsonpage-1 ; i >= r + 1 ; i--)
        memcpy(&pageptr1->itemarray[i], &pageptr1->itemarray[i - 1],
                                                   sizeof(taitem));
      memcpy(&pageptr1->itemarray[r], &procitem1, sizeof(taitem));
      passup = FALSE;
  }
  else
  {
    tanewpage(idxf,&prpgref2,&pageptr2);
    if( r <= ORDER )
    {
      if(r == ORDER) memcpy(&procitem2, &procitem1, sizeof(taitem));
      else
      {
        memcpy(&procitem2,&pageptr1->itemarray[ORDER-1],sizeof(taitem));
        for ( i = ORDER-1 ; i >= r + 1 ; i--)
          memcpy(&pageptr1->itemarray[i], &pageptr1->itemarray[i - 1],
                                                       sizeof(taitem));
        memcpy(&pageptr1->itemarray[r], &procitem1, sizeof(taitem));
      }
      for ( i = 0 ; i < ORDER ; i++)
          memcpy(&pageptr2->itemarray[i], &pageptr1->itemarray[i+ORDER],
                                                       sizeof(taitem));
    }
    else
    {
        r = r - ORDER;
        memcpy(&procitem2, &pageptr1->itemarray[ORDER],
                                                   sizeof(taitem));
        for ( i = 0 ; i < r - 1 ; i++)
          memcpy(&pageptr2->itemarray[i], 
              &pageptr1->itemarray[i + ORDER + 1], sizeof(taitem));
        memcpy(&pageptr2->itemarray[r-1], &procitem1, sizeof(taitem));
        for ( i = r; i < ORDER ; i++)
          memcpy(&pageptr2->itemarray[i], &pageptr1->itemarray[i+ORDER],
                                                        sizeof(taitem));
    }
    pageptr1->itemsonpage = ORDER;
    pageptr2->itemsonpage = ORDER;
    pageptr2->bckwpageref = procitem2.pageref;
    procitem2.pageref = prpgref2;
    memcpy(&procitem1, &procitem2, sizeof(taitem));
    taupdatepage(pageptr2);
  }
  taupdatepage(pageptr1);
}

static void search(indexfile *idxf, DATAREF procdatref, char *pkey,
   DATAREF prpgref1)
{
   int  c,k,l,r;
   
   if( prpgref1 == 0 )
   {
      passup = TRUE;
      memcpy(procitem1.key, pkey, idxf->keyl);
      procitem1.dataref = procdatref;
      procitem1.pageref = 0;
   }
   else
   {
    tagetpage(idxf,prpgref1,&pageptr1);
    {
      l = 1;
      r = pageptr1->itemsonpage;
      do {
        k = (l + r) / 2;
        c = tacompkeys(pkey,
                        pageptr1->itemarray[k-1].key,
                        procdatref,
                        pageptr1->itemarray[k-1].dataref,
                        idxf->allowduplkeys,
                        idxf->keyl);
        if( c <= 0 )
          r = k - 1;
        if( c >= 0 )
          l = k + 1;
      } while (!( r < l));
      if( l - r > 1 )
      {
        ok = FALSE;
        passup = FALSE;
      }
      else
      {
        if( r == 0 ) search(idxf,procdatref,pkey,
                             pageptr1->bckwpageref);
        else search(idxf,procdatref,pkey,
                        pageptr1->itemarray[r-1].pageref);
        if( passup ) insert(idxf, prpgref1, r);
      }
    }
  }
}

void addkey(indexfile *idxf, DATAREF procdatref, char *pkey)
{
DATAREF prpgref1;

    ok = TRUE;
    search(idxf,procdatref,pkey,idxf->rr);
    if( passup )
    {
        prpgref1 = idxf->rr;
        tanewpage(idxf,&idxf->rr,&pageptr1);
        pageptr1->itemsonpage = 1;
        pageptr1->bckwpageref = prpgref1;
        memcpy(&pageptr1->itemarray[0], &procitem1, sizeof(taitem));
        taupdatepage(pageptr1);
    }
    idxf->pp = 0;
}

static void underflow(indexfile *idxf, DATAREF prpgref,
   DATAREF prpgref2, int r, int *pagetoosmall)
{
int  i,k,litem;
DATAREF  lpageref ;
tapageptr pagptr, pageptr2,l;

  tagetpage(idxf,prpgref,&pagptr);
  tagetpage(idxf,prpgref2,&pageptr2);
  if( r < pagptr->itemsonpage )
  {
    r = r + 1;
    lpageref = pagptr->itemarray[r-1].pageref;
    tagetpage(idxf,lpageref,&l);
    k = (l->itemsonpage - ORDER + 1) / 2;
    memcpy(&pageptr2->itemarray[ORDER-1], &pagptr->itemarray[r-1],
                                                sizeof(taitem));
    pageptr2->itemarray[ORDER-1].pageref = l->bckwpageref;
    if( k > 0 )
    {
      for ( i = 1 ; i <= k - 1 ; i++)
        memcpy(&pageptr2->itemarray[i+ORDER-1], &l->itemarray[i-1],
                                                   sizeof(taitem));
      memcpy(&pagptr->itemarray[r-1], &l->itemarray[k-1],
                                                   sizeof(taitem));
      pagptr->itemarray[r-1].pageref = lpageref;
      l->bckwpageref = l->itemarray[k-1].pageref;
      l->itemsonpage = l->itemsonpage - k;
      for ( i = 1 ; i <= l->itemsonpage ; i++)
        memcpy(&l->itemarray[i-1], &l->itemarray[i+k-1],sizeof(taitem));
      pageptr2->itemsonpage = ORDER - 1 + k;
      *pagetoosmall = FALSE;
      taupdatepage(l);
    }
    else
    {
      for ( i = 1 ; i <= ORDER ; i++)
        memcpy(&pageptr2->itemarray[i+ORDER-1], &l->itemarray[i-1],
                                                   sizeof(taitem));
      for ( i = r ; i <= pagptr->itemsonpage - 1 ; i++)
        memcpy(&pagptr->itemarray[i-1], &pagptr->itemarray[i],
                                                sizeof(taitem));
      pageptr2->itemsonpage = PAGESIZE;
      pagptr->itemsonpage = pagptr->itemsonpage - 1;
      tareturnpage(l);
      *pagetoosmall = pagptr->itemsonpage < ORDER;
    }
    taupdatepage(pageptr2);
  }
  else
  {
    if( r == 1 ) lpageref = pagptr->bckwpageref;
    else lpageref = pagptr->itemarray[r - 2].pageref;
    tagetpage(idxf,lpageref,&l);
    litem = l->itemsonpage + 1;
    k = (litem - ORDER) / 2;
    if( k > 0 )
    {
      for ( i = ORDER - 1 ; i >= 1 ; i--)
        memcpy(&pageptr2->itemarray[i+k-1], &pageptr2->itemarray[i-1],
                                                      sizeof(taitem));
      memcpy(&pageptr2->itemarray[k-1], &pagptr->itemarray[r-1],
                                                      sizeof(taitem));
      pageptr2->itemarray[k-1].pageref = pageptr2->bckwpageref;
      litem = litem - k;
      for ( i = k - 1 ; i >= 1 ; i--)
        memcpy(&pageptr2->itemarray[i-1], &l->itemarray[i+litem-1],
                                                   sizeof(taitem));
      pageptr2->bckwpageref = l->itemarray[litem-1].pageref;
      memcpy(&pagptr->itemarray[r-1], &l->itemarray[litem-1],
                                                   sizeof(taitem));
      pagptr->itemarray[r-1].pageref = prpgref2;
      l->itemsonpage = litem - 1;
      pageptr2->itemsonpage = ORDER - 1 + k;
      *pagetoosmall = FALSE;
      taupdatepage(pageptr2);
    }
    else
    {
      memcpy(&l->itemarray[litem-1], &pagptr->itemarray[r-1],
                                                sizeof(taitem));
      l->itemarray[litem-1].pageref = pageptr2->bckwpageref;
      for ( i = 1 ; i <= ORDER - 1 ; i++)
        memcpy(&l->itemarray[i+litem-1], &pageptr2->itemarray[i-1],
                                                    sizeof(taitem));
      l->itemsonpage = PAGESIZE;
      pagptr->itemsonpage = pagptr->itemsonpage - 1;
      tareturnpage(pageptr2);
      *pagetoosmall = pagptr->itemsonpage < ORDER;
    }
    taupdatepage(l);
  }
  taupdatepage(pagptr);
}

static void dela(indexfile *idxf, DATAREF prpgref, DATAREF prpgref2,
   int *pagetoosmall, int k)
{
int  c;
DATAREF  xpageref ;
tapageptr  pageptr2, pagptr;

  tagetpage(idxf,prpgref2,&pageptr2);
  {
    xpageref = pageptr2->itemarray[pageptr2->itemsonpage-1].pageref;
    if( xpageref != 0 )
    {
      c = pageptr2->itemsonpage;
      dela(idxf,prpgref,xpageref,pagetoosmall,k);
      if( *pagetoosmall ) underflow(idxf,prpgref2,xpageref,c,
                                                   pagetoosmall);
    }
    else
    {
      tagetpage(idxf,prpgref,&pagptr);
      pageptr2->itemarray[pageptr2->itemsonpage-1].pageref =
                                 pagptr->itemarray[k-1].pageref;
      memcpy(&pagptr->itemarray[k-1], &pageptr2->
           itemarray[pageptr2->itemsonpage-1], sizeof(taitem));
      pageptr2->itemsonpage--;
      *pagetoosmall = (pageptr2->itemsonpage < ORDER);
      taupdatepage(pagptr);
      taupdatepage(pageptr2);
    }
  }
}

static void delb(indexfile *idxf, DATAREF *procdatref, DATAREF prpgref,
                              int *pagetoosmall, char *pkey)
{
int  c,i,k,l,r;
DATAREF  xpageref;
tapageptr pagptr;

  if( prpgref == 0 )
  {
    ok = FALSE;
    *pagetoosmall = FALSE;
  }
  else
  {
      tagetpage(idxf,prpgref,&pagptr);
      l = 1;
      r = pagptr->itemsonpage;
      do {
        k = (l + r) / 2;
        c = tacompkeys(pkey,
                        pagptr->itemarray[k-1].key,
                        *procdatref,
                        pagptr->itemarray[k-1].dataref,
                        idxf->allowduplkeys,
                        idxf->keyl);
        if( c <= 0 )
          r = k - 1;
        if( c >= 0 )
          l = k + 1;
      } while (!( l > r));
      if( r == 0 ) xpageref = pagptr->bckwpageref;
      else xpageref = pagptr->itemarray[r-1].pageref;
      if( l - r > 1 )
      {
        *procdatref = pagptr->itemarray[k-1].dataref;
        if( xpageref == 0 )
        {
          pagptr->itemsonpage--;
          *pagetoosmall = (pagptr->itemsonpage < ORDER);
          for ( i = k ; i <= pagptr->itemsonpage ; i++)
            memcpy(&pagptr->itemarray[i-1], &pagptr->itemarray[i],
                                                   sizeof(taitem));
          taupdatepage(pagptr);
        }
        else
        {
          dela(idxf,prpgref,xpageref,pagetoosmall,k);
          if( *pagetoosmall )
            underflow(idxf,prpgref,xpageref,r,pagetoosmall);
        }
      }
      else
      {
        delb(idxf, procdatref, xpageref, pagetoosmall, pkey);
        if( *pagetoosmall )
          underflow(idxf,prpgref,xpageref,r,pagetoosmall);
      }
  }
}

void deletekey(indexfile *idxf, DATAREF procdatref, char *pkey)
{
int  pagetoosmall ;
tapageptr pagptr;

    ok = TRUE;
    delb(idxf, &procdatref, idxf->rr, &pagetoosmall, pkey);
    if( pagetoosmall )
    {
      tagetpage(idxf,idxf->rr,&pagptr);
      if( pagptr->itemsonpage == 0 )
      {
        idxf->rr = pagptr->bckwpageref;
        tareturnpage(pagptr);
      }
    }
    idxf->pp = 0;
}


