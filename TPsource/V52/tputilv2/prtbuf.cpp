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

#if defined(__BORLANDC__)
#pragma -K -a1
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>
#include <bvideo.h>
#include <tputil.h>

extern char sgle[], dble[];
extern int iobufr, nchdble, nchsgle;

void putstr(char* buf, char *s, int x, int y, int w, int rf, int df)
   {
   int  l;
   char *to;

   if (w <= 0) return;
   to = buf + ((y-1)*iobufr+x-1);
   if( df )
      {
      memset(to, 240, w);
      to += w;
     }
	l = strlen(s);
   if ( l > w ) l = w;
   if( rf ) to += (w - l);
   memcpy(to, s, l);
   }

void putwstr(wchar_t* buf, wchar_t *s, int x, int y, int w, int rf, int df)
	{
	int  l;
	wchar_t *to;

	if (w <= 0) return;
	to = buf + ((y-1)*iobufr+x-1);
	if( df )
		{
		memset(to, 240, 2*w);
		to += w;
	  }
	l = wcslen(s);
	if ( l > w ) l = w;
	if( rf ) to += (w - l);
	memcpy(to, s, 2*l);
	}

void putint(char *buf, int i, int x, int y, int w, int rd, int df)
	{
	char  s[10];

	_itoa(i,s,10);
	putstr(buf, s,x,y,w,rd,df);
	}

void putintw(wchar_t *buf, int i, int x, int y, int w, int rd, int df)
	{
	wchar_t  s[20];

	_itow(i,s,10);
	putwstr(buf, s,x,y,w,rd,df);
	}

int printbuf(PRFILE *prtf, char *buf, int nrivi)
	{
	int  i, nd;
	char  b[140];
	char *ptr, *aptr, *rptr, *bptr, *sptr;

	rptr = buf;
	for ( i = 0 ; i < nrivi ; i++)
		{
		ptr = rptr + iobufr - 1;
		while ( *ptr == ' ') {
			if ( --ptr < rptr ) break;
			}
		bptr = b;
		nd = 0;
		for ( aptr = rptr; aptr <= ptr; aptr++) {
			if( *aptr != 240) {
				*(bptr++) = *aptr;
				if ( nd > 0 && --nd == 0 )
					for (sptr = sgle; sptr < sgle + nchsgle; sptr++)
						*(bptr++) = *sptr;
				}
			else if( nd++ == 0 )
				for (sptr = dble; sptr < dble + nchdble; sptr++)
					*(bptr++) = *sptr;
			}
		if (nd > 0)
			for (sptr = sgle; sptr < sgle + nchsgle; sptr++)
				*(bptr++) = *sptr;
		*(bptr++) = 13;
		*(bptr++) = 10;
		if (sendchars(b, (int)(bptr-b), prtf)) return(1);
		rptr += iobufr;
		}
	return(0);
	}

int printwbuf(PRFILE *prtf, wchar_t *buf, int nrivi)
	{
	int  i;
	wchar_t  b[140];
	wchar_t *ptr, *aptr, *rptr, *bptr;

	rptr = buf;
	for ( i = 0 ; i < nrivi ; i++)
		{
		ptr = rptr + iobufr - 1;
		while ( *ptr == ' ') {
			if ( --ptr < rptr ) break;
			}
		bptr = b;
		for ( aptr = rptr; aptr <= ptr; aptr++) {
			*(bptr++) = *aptr;
			}
		*(bptr++) = 13;
		*(bptr++) = 10;
		if (sendwchars(b, (int)(bptr-b), prtf)) return(1);
		rptr += iobufr;
		}
	return(0);
	}

