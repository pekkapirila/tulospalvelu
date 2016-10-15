/**
*
*  BUTIL.H    Header file for Turbo C TOOLS Utility functions
*
*  Version 6.00  (C)Copyright Blaise Computing Inc.  1987-1989
*
**/

#ifndef DEF_BUTIL		/* Prevent redefinition.	    */

#include <string.h>
#include <stdlib.h>
#include <windows.h>

/********************************************************************/
/* Constant values for UTINTFLG, UTAMOVE, UTSAFCPY, UTCTLBRK, UTANSI*/
/********************************************************************/

#define uthiword(a)   (((a)>>16)&0xffffL)   /* High word of long a. */
#define utloword(a)   ((a)&0xffffL)	    /* Low  word of long a. */

				/* Combine high word a, low  word b.*/
#define utwdlong(a,b) ((((0xffffL&(long)(a)))<<16)|		     \
		       (0xffffL&(long)(b)))

#define uthibyte(a)   (((a)>>8)&0x00ff) /* High byte of word a.     */
#define utlobyte(a)   ((a)&0x00ff)	/* Low	byte of word a.     */

				/* Combine high byte a, low byte b. */
#define utbyword(a,b) ((((a)&0x00ff)<<8)|((b)&0x00ff))

#define uthinyb(a)    (((a)>>4)&0x000f) /* High nybble of byte a.   */
#define utlonyb(a)    ((a)&0x000f)	/* Low nybble  of byte a.   */

				/* Combine high nybble a, low	    */
				/* nybble b.			    */
#define utnybbyt(a,b) ((((a)&0x000f)<<4)|((b)&0x000f))


/********************************************************************/
/* Miscellaneous macros.					    */
/********************************************************************/

#define NIL		((void *) 0)	/* Universal invalid data   */
					/* pointer.		    */

					/* Far version of NIL.	    */
#define FARNIL		((void far *) 0)

#define utsign(a)	((a)>0?1:((a)==0?0:-1)) /* Arithmetic sign. */
#define utskip		printf("\n")            /* CR/LF.           */

			/* Make a sound of "freq" frequency with the*/
			/* speaker for a specified duration.	    */
#define utsound(freq, dura)   Beep((freq),55*(dura))

#define utsleep(dura)  Sleep((DWORD)(55*(dura)))

/********************************************************************/
/* Macros to perform range checking and limiting.		    */
/********************************************************************/

/* Beware:  the following three macros (UTMAX, UTMIN, and UTABS)    */
/* may have side effects because they evaluate their arguments more */
/* than once.							    */

				/* Maximum of two values.	    */
#define utmax(a,b) ((a) > (b) ? (a) : (b))
#ifndef max
#define max(a,b) utmax(a,b)
#endif

				/* Minimum of two values.	    */
#define utmin(a,b) ((a) <= (b) ? (a) : (b))
#ifndef min
#define min(a,b) utmin(a,b)
#endif

#define utabs(x) ((x)<0 ? -(x) : (x))	/* Absolute value.	    */

			/* Return 1 if a is outside the range	    */
			/* defined by l and h, 0 if not.	    */
#define utrange(a,l,h)	(((a) < (l)) || ((a) > (h)))
#define utoutrng(a,l,h) utrange(a,l,h)

			/* Confine a to the range defined by b and c*/
#define utbound(a,b,c)						     \
    {								     \
	register int uttemp;					     \
								     \
	if ((a) < (uttemp = (b)) || (a) > (uttemp = (c)))	     \
	    (a) = uttemp;					     \
    }

			/* Prevent a from exceeding b.		    */
#define utuplim(a,b)						     \
    {								     \
	register int uttemp;					     \
								     \
	if ((a) > (uttemp = (b)))				     \
	    (a) = uttemp;					     \
    }

			/* Prevent a from being less than b.	    */
#define utlowlim(a,b)						     \
    {								     \
	register int uttemp;					     \
								     \
	if ((a) < (uttemp = (b)))				     \
	    (a) = uttemp;					     \
    }

#define DEF_BUTIL  1	/* Prevent second reading of these	    */
			/* definitions. 			    */

#endif			/* Ends "#ifndef DEF_BUTIL".                */

