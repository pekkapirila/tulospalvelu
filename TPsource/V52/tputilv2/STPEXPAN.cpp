/**
*
* Name		STPEXPAN -- Replace tab characters with blanks.
*
* Synopsis	presult = stpexpan(ptarget,psource,incr,tarsize);
*
*		char *presult	NIL if conversion was completed; else
*				pointer to next character in psource.
*		char *ptarget	Pointer to the target string.
*		char *psource	Pointer to the source string.
*		int  incr	Interval separating tab stops.
*		int tarsize	The maximum size of the target string.
*
* Description	This function replaces tab characters ('\t') with enough
*		blank spaces (' ') to advance to the following tab
*		position.
*
*		Definition of tab stops:  If incr is positive then tab
*		stops are set every incr characters starting from
*		position 0 (i.e., the first character of the line).  If
*		incr is 0 or negative, then no tab stops are set, and
*		any tab characters in psource are copied intact to
*		ptarget.
*
*		STPEXPAN regards all characters in psource (except tabs
*		and NULs) as graphic characters occupying one space in
*		ptarget.
*
*		Warning:  ptarget and psource must not overlap.
*
* Returns	presult 	Pointer to the remainder of psource if
*				there wasn't enough room in ptarget;
*				otherwise NIL.
*		ptarget 	Pointer to the altered target string.
*
* Version	6.00 (C)Copyright Blaise Computing Inc.  1987,1989
*
**/


#include "bstrings.h"
#include "butil.h"

#define  BLANK	 ' '
#define  NEWLINE '\n'
#define  TAB	 '\t'
#define  NUL	 '\0'


char *stpexpan(char *ptarget, char *psource, int incr, int tarsize)
{
    char c;
    int len;		/* Running total of characters put  */
				/* into ptarget, also index of next */
				/* char in ptarget.		    */
    int numspaces;

    len = 0;
    tarsize--;
    while (((c = *psource) != NUL) && (len < tarsize))
    {
	switch (c)
	{
	    case TAB:
		if ((incr > 0) &&
		    ((numspaces = (incr - (len % incr))) != 0))
		{
		    if ((len += numspaces) < tarsize)
		    {		/* There's enough room.             */
			while (numspaces--)
			    *ptarget++ = BLANK;
			break;
		    }
		    else		/* There isn't enough room, */
			continue;	/* so quit.		    */
		}

	    /* Else TAB expansion is not in effect:		    */
	    /* just fall through and copy the TAB to ptarget.	    */

	    default:
		*ptarget++ = c;
		len++;
		break;
	}
	psource++;
    }
    *ptarget = '\0';
    return (c ? psource : NULL);
}

