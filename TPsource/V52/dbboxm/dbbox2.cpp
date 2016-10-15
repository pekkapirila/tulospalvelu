#include <stdlib.h>
#include <malloc.h>
#include <io.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <butil.h>
#include <boxm.h>
#include <tputil.h>
#include "dbint.h"
#define TRUE 1
#define FALSE 0
#define MAXNBLOCK 1024
#define UNDERSCORE  =  '_'

void lopetus(void);

   extern int minkilpno, maxkilpno;
   extern int __dbbox__ok;

int makedarr(dataarr *datf, int reclen, int maxnrec)
{
	datf->recl = reclen;
	datf->maxnpage = maxnrec / ORDER + 2;
	datf->ptr = (darrref *) malloc(4 * datf->maxnpage + 4);
	if (datf->ptr == NULL) {
	   writeerror(
		  "Muisti ei riit„ indeksiosoitintaulukolle, ajo lopetetaan",
		  2000);
	   lopetus();
	   }
	datf->ptr[0].os.s = 0;
	datf->ptr[0].os.o = -1;
	datf->numrec = 0;
	__dbbox__ok = TRUE;
	return(!__dbbox__ok);
}

void closedarr(dataarr *datf)
{
   int i;

   for (i=1; i<=datf->numrec; i++)
      if (datf->ptr[i].os.s != 0) free(datf->ptr[i].dp);
   free(datf->ptr);
   datf->ptr = NULL;
   datf->numrec = 0;
}

void newdrec(dataarr *datf, int *r)
{
   if (datf->ptr[0].os.o == -1) *r = ++datf->numrec;
   else {
      *r = datf->ptr[0].os.o;
      datf->ptr[0].os.o = datf->ptr[*r].os.o;
      }
   datf->ptr[*r].dp = (tapage *) malloc(datf->recl);
   if (datf->ptr[*r].dp == NULL) {
      writeerror("Muisti ei riit„ indeksitaulukolle, ajo lopetetaan",
            2000);
      lopetus();
      }
}

void deletedrec(dataarr *datf, int r)
{
   free(datf->ptr[r].dp);
   datf->ptr[r].dp = datf->ptr[0].dp;
   datf->ptr[0].os.o = r;
}

void initindex(void)
{

}

void makeindex(indexfile *idxf, UCHAR *fname, int keylen, int s)
{
int  k ;
UCHAR *ss;

  ss = fname;
  k = (keylen + 4)*PAGESIZE + 4;
  makedarr(&(idxf->dataf),k,(int)(maxkilpno-minkilpno));
  idxf->allowduplkeys = (s != 0);
  idxf->keyl = keylen;
  idxf->rr = 0;
  idxf->pp = 0;
}

void closeindex(indexfile *idxf)
{
   closedarr(&idxf->dataf);
}

void tagetpage(indexfile *idxf, int r, tapageptr *pgptr)
{
   *pgptr = idxf->dataf.ptr[r].dp;
}

void tanewpage(indexfile *idxf, int *r, tapageptr *pgptr)
{
   newdrec(&idxf->dataf, r);
   *pgptr = idxf->dataf.ptr[*r].dp;
}

void tareturnpage(indexfile *idxf, int pageref)
{
    deletedrec(&(idxf->dataf), pageref);
}

int tacompkeys(UCHAR *k1, UCHAR *kf2, int dr1, int dr2, int dup,
   int klen)
{
   int kl;
   UCHAR k2[MAXKEYLEN];

  memcpy(k2, kf2, klen);
  kl = memcmp(k1,k2,klen);
  if(kl == 0) {
    if( dup ) return(dr1 - dr2);
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

void copypgtoitem(taitem *item, tapageptr pgptr, int i, int keyl)
{
   item->dataref = pgptr->dataref[i];
   item->pageref = pgptr->pageref[i];
   memcpy(item->key, pgptr->key + i*keyl, keyl);
}

void copyitemtopg(tapageptr pgptr, taitem *item, int i, int keyl)
{
   pgptr->dataref[i] = item->dataref;
   pgptr->pageref[i] = item->pageref;
   memcpy(pgptr->key + i*keyl, item->key, keyl);
}

void copypgtopg(tapageptr pgptr2, tapageptr pgptr1, int i2, int i1,
   int keyl)
{
   pgptr2->dataref[i2] = pgptr1->dataref[i1];
   pgptr2->pageref[i2] = pgptr1->pageref[i1];
   memcpy(pgptr2->key + i2*keyl, pgptr1->key + i1*keyl, keyl);
}
