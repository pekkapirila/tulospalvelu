/**
*
* Name		STPJUST -- Justify a string within a field.
*
* Synopsis	presult = stpjust(ptarget,psource,fill,fldsize,code);
*
*		char *presult	Pointer to the resulting target string
*		char *ptarget	Pointer to the target string, which must
*				be at least (fldsize + 1) bytes long.
*		const char *psource
*				Address of the source string
*		char fill	Character to be used for padding
*		int  fldsize	Size of field to be filled
*		int  code	Type of justification:
*				JUST_LEFT, JUST_CENTER, or JUST_RIGHT.
*
* Description	This function justifies or centers a string within a
*		field of a specified size, padding with a given
*		character if necessary.  The resulting string is exactly
*		fldsize characters long.
*
*		If the source string has more than fldsize characters,
*		it is truncated to fit into the target.  Characters from
*		the left, center, or right portion of the source are
*		used depending on whether left, center, or right
*		justification is specified, respectively.
*
*		If the source string is fewer than fldsize bytes long
*		(not counting the trailing NUL ('\0')), the remaining
*		space is filled with the fill character.  If left
*		justification is specified, the filling takes place on
*		the right; if right justification, on the left; if
*		centering, on both sides.
*
*		If code has an unknown value, left justification is
*		performed.
*
* Returns	presult 	Pointer to the altered target string.
*		*ptarget	The altered target string.
*
* Version	6.00 (C)Copyright Blaise Computing Inc.  1986, 1987, 1989
*
**/


#include <string.h>

#include "bstrings.h"


char *stpjust(char  *ptarget, const char *psource, char  fill, int fldsize, int code)
{
    int diff,i;
    int 	 numleft;
    char *savetarget = ptarget;

    if (fldsize < 0)
	fldsize = 0;

    if ((diff = ((int) strlen(psource)) - fldsize) >= 0)
    {			      /* Use only a portion of source	      */
	switch (code)
	{
	    case JUST_RIGHT:	      /* Skip leftmost characters     */
		psource += diff;
		break;
	    case JUST_CENTER:	      /* Use center characters	      */
		psource += diff >> 1;
		break;
	    case JUST_LEFT:	      /* Use leftmost characters      */
	    default:
		break;
	}
	while (fldsize--)
	    *ptarget++ = *psource++;
    }
    else
    {			      /* There's extra space to fill          */
	diff = -diff;	      /* diff is number of spaces to fill     */
	switch (code)
	{		      /* numleft = number of spaces on left   */
	    case JUST_RIGHT:
		numleft = diff;
		break;
	    case JUST_CENTER:
		numleft = diff / 2;
		break;
	    case JUST_LEFT:
	    default:
		numleft = 0;
		break;
	}
	for (i = numleft; i; i--)
	    *ptarget++ = fill;	   /* Add the fill chars on the left  */
	while (*psource)
	    *ptarget++ = *psource++;   /* Copy the string itself      */
	for (i = diff - numleft; i; i--)
	    *ptarget++ = fill;	   /* Add the fill chars on the right */
    }
    *ptarget = '\0';
    return savetarget;
}

