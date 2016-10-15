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

int nextkey(indexfile *idxf, int *procdatref, UCHAR *pkey)
{
int  r;
tapageptr pagptr;

  {
	if( idxf->pp == 0 )
	  r = idxf->rr;
	else
	{
	  tagetpage(idxf,idxf->path[idxf->pp-1].pageref,&pagptr);
	  r = pagptr->pageref[idxf->path[idxf->pp-1].itemarrindex-1];
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
		  memcpy(pkey, pagptr->key + idxf->keyl *
			 idxf->path[idxf->pp-1].itemarrindex, idxf->keyl);
		  *procdatref = pagptr->
			   dataref[idxf->path[idxf->pp-1].itemarrindex];
		  idxf->path[idxf->pp-1].itemarrindex++;
		}
	  else idxf->pp = 0;
	}
	__dbbox__ok = (idxf->pp != 0);
  }
	return(!__dbbox__ok);
}



