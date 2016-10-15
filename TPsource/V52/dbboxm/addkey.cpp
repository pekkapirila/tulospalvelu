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

   extern int __dbbox__ok;
   static int  passup;
   static tapageptr pageptr1, pageptr2;
   static taitem procitem1, procitem2;

static void insert(indexfile *idxf, int prpgref1, int *r)
{
int i;
int prpgref2;

  tagetpage(idxf,prpgref1,&pageptr1);
  if(pageptr1->itemsonpage < PAGESIZE )
  {
      pageptr1->itemsonpage++;
      for ( i = pageptr1->itemsonpage-1 ; i >= *r + 1 ; i--)
        copypgtopg(pageptr1, pageptr1, i, (int)(i - 1), idxf->keyl);
		copyitemtopg(pageptr1, &procitem1, *r, idxf->keyl);
      passup = FALSE;
  }
  else
  {
    tanewpage(idxf,&prpgref2,&pageptr2);
    if( *r <= ORDER )
    {
      if(*r == ORDER) memcpy(&procitem2, &procitem1, idxf->keyl);
      else
      {
        copypgtoitem(&procitem2, pageptr1, ORDER-1 ,idxf->keyl);
        for ( i = ORDER-1 ; i >= *r + 1 ; i--)
          copypgtopg(pageptr1, pageptr1, i, (int)(i - 1), idxf->keyl);
        copyitemtopg(pageptr1, &procitem1, *r, idxf->keyl);
      }
      for ( i = 0 ; i < ORDER ; i++)
          copypgtopg(pageptr2, pageptr1, i, (int)(i+ORDER), idxf->keyl);
    }
    else
    {
        *r = *r - ORDER;
        copypgtoitem(&procitem2, pageptr1, ORDER, idxf->keyl);
        for ( i = 0 ; i < *r - 1 ; i++)
          copypgtopg(pageptr2, pageptr1, i, (int)(i + ORDER + 1), idxf->keyl);
        copyitemtopg(pageptr2, &procitem1, (int)(*r-1), idxf->keyl);
        for ( i = *r; i < ORDER ; i++)
          copypgtopg(pageptr2, pageptr1, i, (int)(i+ORDER), idxf->keyl);
    }
    pageptr1->itemsonpage = ORDER;
	 pageptr2->itemsonpage = ORDER;
    pageptr2->bckwpageref = procitem2.pageref;
    procitem2.pageref = prpgref2;
    memcpy(&procitem1, &procitem2, idxf->keyl+4);
  }
}

static void search(indexfile *idxf, int procdatref, UCHAR *pkey,
                                                   int prpgref1)
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
                        pageptr1->key + idxf->keyl * (k-1),
                        procdatref,
                        pageptr1->dataref[k-1],
                        idxf->allowduplkeys,
                        idxf->keyl);
        if( c <= 0 )
          r = k - 1;
        if( c >= 0 )
          l = k + 1;
      } while (!( r < l));
      if( l - r > 1 )
      {
        __dbbox__ok = FALSE;
        passup = FALSE;
      }
      else
      {
        if( r == 0 ) search(idxf,procdatref,pkey,
                             pageptr1->bckwpageref);
        else search(idxf,procdatref,pkey,
                        pageptr1->pageref[r-1]);
        if( passup ) insert(idxf, prpgref1, &r);
      }
    }
  }
}

int addkey(indexfile *idxf, int procdatref, UCHAR *pkey)
{
int prpgref1;

	__dbbox__ok = TRUE;
	 search(idxf,procdatref,pkey,idxf->rr);
	if( passup )
	{
		prpgref1 = idxf->rr;
		  tanewpage(idxf,&idxf->rr,&pageptr1);
		pageptr1->itemsonpage = 1;
		pageptr1->bckwpageref = prpgref1;
		copyitemtopg(pageptr1, &procitem1, 0, idxf->keyl);
	}
	idxf->pp = 0;
	return(!__dbbox__ok);
}



