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
#define fmemcpy(p2,p1,l) movhmem((UCHAR far *)(p2),(UCHAR far *)(p1),l)

void lopetus(void);

   extern int __dbbox__ok;

static void tafindkey(indexfile *idxf, int *procdatref, UCHAR *pkey)
{
int  c,k,l,r,prpgref;
tapageptr pagptr;

  {
    __dbbox__ok = FALSE;
    idxf->pp = 0;
    prpgref = idxf->rr;
    while ( (prpgref != 0) && ! __dbbox__ok )
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
                          pagptr->key + idxf->keyl * (k-1),
                          0,
                          pagptr->dataref[k-1],
                          idxf->allowduplkeys,
                          idxf->keyl);
          if (c <= 0) r = k - 1;
          if (c >= 0) l = k + 1;
        }
        if( l - r > 1 )
        {
          *procdatref = pagptr->dataref[k-1];
          r = k;
          __dbbox__ok = TRUE;
        }
        if( r == 0 )  prpgref = pagptr->bckwpageref;
        else prpgref = pagptr->pageref[r-1];
      }
      idxf->path[idxf->pp-1].itemarrindex = r;
    }
    if(!__dbbox__ok && (idxf->pp > 0))
    {
      while ((idxf->pp > 1) &&
         (idxf->path[idxf->pp-1].itemarrindex == 0)) idxf->pp--;
      if (idxf->path[idxf->pp-1].itemarrindex == 0) idxf->pp = 0;
    }
  }
}

int findkey(indexfile *idxf, int *procdatref, UCHAR *pkey)
{
UCHAR tempkey[MAXKEYLEN];

   tafindkey(idxf,procdatref,pkey);
   if(!__dbbox__ok && idxf->allowduplkeys )
   {
	  memcpy(tempkey, pkey, idxf->keyl);
	  nextkey(idxf,procdatref,pkey);
	  __dbbox__ok = (__dbbox__ok && !memcmp(pkey, tempkey, idxf->keyl));
   }
	return(!__dbbox__ok);
}

int searchkey(indexfile *idxf, int *procdatref, UCHAR *pkey)
{

   tafindkey(idxf,procdatref,pkey);
   if(!__dbbox__ok) nextkey(idxf,procdatref,pkey);
	return(!__dbbox__ok);
}


