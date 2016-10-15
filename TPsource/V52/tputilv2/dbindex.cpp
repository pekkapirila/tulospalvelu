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

#include <malloc.h>
#include <string.h>

#define cmpkey(k1,k2,keylen) memcmp((k1),(k2),(keylen))
#define srmovh(to, fr, reclen) memcpy((to),(fr),(reclen))
#pragma pack(1)
typedef struct {
   char * ix;
   int keyl;
   int recl;
   int nrec;
   int pos;
   char *(*keyf)(void);
   } kilpindex;

static int __tputil__ok;

char * bsrch(char *key, char *base, int *nelem, int keylen, int width);

//static int reclen, keylen;
//static int reclen0, keylen0;

void setcmp(int keyl, int recl)
   {
   }

void pushcmp(void)
   {
   }

void popcmp(void)
   {
   }

int addix(kilpindex *idx, char *key, int d)
{
   char  *ip;
   char  *ip1;
   long in;
   long reclen;

   __tputil__ok = 1;
   ip = bsrch(key, idx->ix, &(idx->nrec), idx->keyl, idx->recl);

/* ip osoittaa 1. alkioon, joka on v„hint„„n key:n suuruinen        */

   reclen = idx->recl;
   ip1 = ip - idx->nrec *reclen;
   in = idx->ix - ip1;
   memmove((ip + idx->recl), (ip), in);
   memcpy(ip + idx->keyl, (char *) &d, 4);
   memcpy(ip, key, idx->keyl);
   idx->nrec++;
   return(!__tputil__ok);
}

int delix(kilpindex *idx, char *key, int d)
{
   char  *ip;
   char  *ip1;
   int *id = NULL;
   long in, reclen;

   __tputil__ok=1;
   if (idx->nrec <= 0) {
	   __tputil__ok = 0;
	   return(1);
	   }
   ip = bsrch(key, idx->ix, &(idx->nrec), idx->keyl, idx->recl);

/* ip osoittaa 1. alkioon, joka on v„hint„„n key:n suuruinen        */

   if (!__tputil__ok) return(1);      /* poistu, jos key kaikkia alkioita suurempi */
   id = (int *) (ip+idx->keyl);
   while (!cmpkey(key, ip, idx->keyl)) {
	  id = (int *) (ip+idx->keyl);
	  if (*id == d) break;
	  ip += idx->recl;
	  }
   if (d != *id) {
		 __tputil__ok=0;
		 return(1);
	  }
   idx->nrec--;
   reclen = idx->recl;
   ip1 = ip - idx->nrec *reclen;
   in = idx->ix - ip1;
   memmove((ip), (ip + idx->recl), in);
   return(!__tputil__ok);
}

int searchix(kilpindex *idx, char *key, int *d)
{
   char  *ip;

   __tputil__ok=1;
	ip = bsrch(key, idx->ix, &(idx->nrec), idx->keyl, idx->recl);
   if (!__tputil__ok) ip -= idx->recl;
   memcpy( (char *) d, (ip + idx->keyl), 4);
   idx->pos = (int) ((ip - idx->ix)/idx->recl);
   memcpy(  key, (ip ), idx->keyl);
   return(!__tputil__ok);
}

int nextix(kilpindex *idx, char *key, int *d)
{
   char  *ip;
   long reclen;

   if ( ++(idx->pos) < idx->nrec ) __tputil__ok = 1;
   else {
      idx->pos = 0;
      __tputil__ok=0;
      }
   reclen = idx->recl;
   ip = idx->ix + idx->pos * reclen;
   memcpy( (char *) d, (ip + idx->keyl), 4);
   memcpy(  key, (ip ), idx->keyl);
   return(!__tputil__ok);
}

int previx(kilpindex *idx, char *key, int *d)
{
   char  *ip;
   long reclen;

   if ( --(idx->pos) >= 0 ) __tputil__ok = 1;
   else {
      idx->pos = idx->nrec-1;
      __tputil__ok=0;
      }
   reclen = idx->recl;
   ip = idx->ix + idx->pos * reclen;
   memcpy( (char *) d, (ip + idx->keyl), 4);
   memcpy(  key, ip, idx->keyl);
   return(!__tputil__ok);
}

char * bsrch(char *key, char *base, int *nelem, int keylen, int width)
{
   register int i, u;
   int a, c;
   long w;
   char  *p;

   a=0;
   u=*nelem;
   w=width;
   __tputil__ok=1;
   while (a<u) {
      i=(a+u) >> 1;
      p=base + i*w;
      c=cmpkey(key, p, keylen);
      if (!c) {
         while (p-base > 0 && !cmpkey(key, (p-width), keylen))
            p -= width;
         return(p);
         }
      if (c<0) u=i;
      else a=i+1;
      }
   if (u==*nelem) __tputil__ok=0;
   return((base + u*w));
}

void quicksort(char *base, int nelem, int keylen, int width)
{
   char *i1, *i2, *ir;
   char *v, *t;
   long w;
   int i, l, r, p;
   int st[50];

   l=0;
   w = width;
   r=nelem-1;
   p=1;
   v = (char *) malloc(width);
   t = (char *) malloc(width);
   do
      {
      if ( r>l ) {
         i1 = base + ( ((long)l) - 1) * w;
         i2 = ir = base + r*w;
         srmovh(v, ir, width);
         do
            {
            do
               {
               i1 += width;
               }
            while ( cmpkey(v,i1, keylen) > 0 );
            do
               {
               i2 -= width;
               }
            while ( cmpkey(v,i2, keylen) < 0 && i2 > i1);
            if (i2 <= i1)
               break;
            srmovh( t, i1, width);
            srmovh( i1, i2, width);
            srmovh( i2,  t, width);
            }
         while (1);
         srmovh( t, i1, width);
         srmovh( i1, ir, width);
         srmovh( ir,  t, width);
         i = (unsigned) ((i1 - base) / width);
         if ( (i-l) > (r-i) ) {
            st[p++] = l;
            st[p++] = i-1;
            l = i+1;
            }
         else {
            st[p++] = i+1;
            st[p++] = r;
            r = i-1;
            }
         }
      else {
         r = st[--p];
         l = st[--p];         
         }
      }
   while ( p >= 0 );
   free(v);
   free(t);
}

