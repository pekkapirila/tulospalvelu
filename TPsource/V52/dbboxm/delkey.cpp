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

static void underflow(indexfile *idxf, int prpgref, int prpgref2, int r,
                                                int *pagetoosmall)
{
int  i,k,litem;
int  lpageref ;
tapageptr pagptr, pageptr2,l;

  tagetpage(idxf,prpgref,&pagptr);
  tagetpage(idxf,prpgref2,&pageptr2);
  if( r < pagptr->itemsonpage )
  {
    r = r + 1;
    lpageref = pagptr->pageref[r-1];
    tagetpage(idxf,lpageref,&l);
    k = (l->itemsonpage - ORDER + 1) / 2;
    copypgtopg(pageptr2, pagptr, (int)(ORDER-1), (int)(r-1), idxf->keyl);
    pageptr2->pageref[ORDER-1] = l->bckwpageref;
    if( k > 0 )
    {
      for ( i = 1 ; i <= k - 1 ; i++)
        copypgtopg(pageptr2, l, (int)(i+ORDER-1), (int)(i-1), idxf->keyl);
      copypgtopg(pagptr, l, (int)(r-1), (int)(k-1), idxf->keyl);
      pagptr->pageref[r-1] = lpageref;
      l->bckwpageref = l->pageref[k-1];
      l->itemsonpage = l->itemsonpage - k;
      for ( i = 1 ; i <= l->itemsonpage ; i++)
        copypgtopg(l, l, (int)(i-1), (int)(i+k-1), idxf->keyl);
      pageptr2->itemsonpage = ORDER - 1 + k;
      *pagetoosmall = FALSE;
    }
    else
    {
      for ( i = 1 ; i <= ORDER ; i++)
        copypgtopg(pageptr2, l, (int)(i+ORDER-1), (int)(i-1), idxf->keyl);
      for ( i = r ; i <= pagptr->itemsonpage - 1 ; i++)
        copypgtopg(pagptr, pagptr, (int)(i-1), i, idxf->keyl);
      pageptr2->itemsonpage = PAGESIZE;
      pagptr->itemsonpage = pagptr->itemsonpage - 1;
      tareturnpage(idxf, lpageref);
      *pagetoosmall = pagptr->itemsonpage < ORDER;
    }
  }
  else
  {
    if( r == 1 ) lpageref = pagptr->bckwpageref;
    else lpageref = pagptr->pageref[r - 2];
    tagetpage(idxf,lpageref,&l);
    litem = l->itemsonpage + 1;
    k = (litem - ORDER) / 2;
    if( k > 0 )
    {
      for ( i = ORDER - 1 ; i >= 1 ; i--)
        copypgtopg(pageptr2, pageptr2, (int)(i+k-1), (int)(i-1), idxf->keyl);
      copypgtopg(pageptr2, pagptr, (int)(k-1), (int)(r-1), idxf->keyl);
      pageptr2->pageref[k-1] = pageptr2->bckwpageref;
      litem = litem - k;
      for ( i = k - 1 ; i >= 1 ; i--)
        copypgtopg(pageptr2, l, (int)(i-1), (int)(i+litem-1), idxf->keyl);
      pageptr2->bckwpageref = l->pageref[litem-1];
      copypgtopg(pagptr, l, (int)(r-1), (int)(litem-1), idxf->keyl);
      pagptr->pageref[r-1] = prpgref2;
      l->itemsonpage = litem - 1;
      pageptr2->itemsonpage = ORDER - 1 + k;
      *pagetoosmall = FALSE;
    }
    else
    {
      copypgtopg(l, pagptr, (int)(litem-1), (int)(r-1), idxf->keyl);
      l->pageref[litem-1] = pageptr2->bckwpageref;
      for ( i = 1 ; i <= ORDER - 1 ; i++)
        copypgtopg(l, pageptr2, (int)(i+litem-1), (int)(i-1), idxf->keyl);
      l->itemsonpage = PAGESIZE;
      pagptr->itemsonpage = pagptr->itemsonpage - 1;
      tareturnpage(idxf, prpgref2);
      *pagetoosmall = pagptr->itemsonpage < ORDER;
    }
  }
}

static void dela(indexfile *idxf, int prpgref, int prpgref2,
                                       int *pagetoosmall, int k)
{
int  c;
int  xpageref ;
tapageptr  pageptr2, pagptr;

  tagetpage(idxf,prpgref2,&pageptr2);
  {
    xpageref = pageptr2->pageref[pageptr2->itemsonpage-1];
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
      pageptr2->pageref[pageptr2->itemsonpage-1] = pagptr->pageref[k-1];
      copypgtopg(pagptr, pageptr2, (int)(k-1), (int)(pageptr2->itemsonpage-1),
         idxf->keyl);
      pageptr2->itemsonpage--;
      *pagetoosmall = (pageptr2->itemsonpage < ORDER);
    }
  }
}

static void delb(indexfile *idxf, int *procdatref, int prpgref,
                              int *pagetoosmall, UCHAR *pkey)
{
int  c,i,k,l,r;
int  xpageref;
tapageptr pagptr;

  if( prpgref == 0 )
  {
    __dbbox__ok = FALSE;
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
                        pagptr->key + idxf->keyl * (k-1),
                        *procdatref,
                        pagptr->dataref[k-1],
                        idxf->allowduplkeys,
                        idxf->keyl);
        if( c <= 0 )
          r = k - 1;
        if( c >= 0 )
          l = k + 1;
      } while (!( l > r));
      if( r == 0 ) xpageref = pagptr->bckwpageref;
      else xpageref = pagptr->pageref[r-1];
      if( l - r > 1 )
      {
        *procdatref = pagptr->dataref[k-1];
        if( xpageref == 0 )
        {
          pagptr->itemsonpage--;
          *pagetoosmall = (pagptr->itemsonpage < ORDER);
          for ( i = k ; i <= pagptr->itemsonpage ; i++)
            copypgtopg(pagptr, pagptr, (int)(i-1), i, idxf->keyl);
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

int deletekey(indexfile *idxf, int *procdatref, UCHAR *pkey)
{
int  pagetoosmall ;
tapageptr pagptr;

	__dbbox__ok = TRUE;
	delb(idxf, procdatref, idxf->rr, &pagetoosmall, pkey);
	if( pagetoosmall )
	{
	  tagetpage(idxf,idxf->rr,&pagptr);
	  if( pagptr->itemsonpage == 0 )
	  {
		tareturnpage(idxf, idxf->rr);
		idxf->rr = pagptr->bckwpageref;
	  }
	}
	idxf->pp = 0;
	return(!__dbbox__ok);
}



