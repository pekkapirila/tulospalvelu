/**
*
* Name		STPXLATE -- Translate a string.
*
* Synopsis	presult = stpxlate(psource,ptable,ptrans);
*
*		char *presult	     Pointer to translated string.
*		char *psource	     Pointer to source string.
*		const char *ptable   Pointer to string table of
*				     characters to be translated.
*		const char *ptrans   Pointer to translation string.
*
* Description	This function translates the string psource according to
*		the instructions specified by ptable and ptrans.  If
*		psource is the null string (""), the null string is
*		returned; otherwise for each character psource[k] of
*		psource, the corresponding character of presult,
*		presult[k] is defined as follows:
*
*		     = psource[k] if psource[k] is not in ptable.
*
*		     = BLANK if the position of psource[k] in ptable
*		       is greater than the length of ptrans.
*
*		     = trans[j] otherwise, where j is the position of
*		       psource[k] in ptable.
*
* Returns	presult 	Pointer to the translated string.
*		psource 	The same as presult; psource
*				translated.
*
* Version	6.00 (C)Copyright Blaise Computing Inc.  1983,1987,1989
*
**/


#include <stdio.h>	/* For definition of NULL.		    */
#include <string.h>

#include "bstrings.h"

#define BLANK	' '
#define NUL	'\0'


char *stpxlate(char *psource, const char *ptable, const char *ptrans)
{
	int tindex;
    int 	 len_trans;
    char ch, *ptemp;

    len_trans = (int) strlen (ptrans);	/* Length of ptrans.	    */
    ptemp     = psource;

    while ((ch = *psource) != NUL)
    {
	tindex = stschind (ch, ptable);
	if (tindex >= 0)		/* psource[i] is in ptable. */
	   if ((tindex + 1) > len_trans)/* Indices start from zero. */
	      *psource = BLANK;
	   else
	      *psource = ptrans[tindex];
	psource++;
    }

    return (ptemp);
}

