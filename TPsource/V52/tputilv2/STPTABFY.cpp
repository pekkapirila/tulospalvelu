/**
*
* Name		STPTABFY -- Replace blanks with tab characters.
*
* Synopsis	presult = stptabfy(psource,incr);
*
*		char *presult	Pointer to converted psource.
*		char *psource	Pointer to the source string.
*		int  incr	Interval separating tab stops.
*
* Description	This function replaces blank spaces (' ') with tab
*		characters ('\t') so as to possibly shorten the length
*		of a string in memory.	Use of tab characters is
*		minimal:  that is, whenever a single blank space will
*		serve instead of a tab, the blank is used.
*
*		Tab stops are defined as follows:  If incr is positive
*		then tab stops are set every incr characters starting
*		from position 0 (i.e., the first character of the
*		line).	If incr is 0 or negative, then tab stops are
*		set at each position, so that all blanks and tabs in
*		psource are copied intact.
*
*		Existing tab characters are left intact.  They are
*		regarded as advancing to the next tab stop.
*
*		STPTABFY regards all characters in psource (except
*		TABs and NULs) as graphic characters occupying one
*		space in the result.
*
* Returns	presult 	Pointer to the altered source string.
*		*psource	The altered source string.
*
* Version	6.00 (C)Copyright Blaise Computing Inc.  1987, 1989
*
**/


#include "bstrings.h"

#define  BLANK	 ' '
#define  NEWLINE '\n'
#define  TAB	 '\t'


char *stptabfy(char *psource, int incr)
{
    char c;
    int col = 0;	/* Column counter (modulo incr).      */
    int numblanks = 0; /* Number of blanks we've saved up.   */

    char *pfrom = psource;
    char *pto	 = psource;

    do
    {
	switch (c = *pfrom++)
	{
	    case BLANK:
		numblanks++;
		col++;
		if ((incr <= 0) ||
		    (col % incr == 0))
		{
		    *pto++    = (char) ((numblanks > 1) ? TAB : BLANK);
		    numblanks = 0;
		}
		break;

	    case TAB:
		col	  =
		numblanks = 0;		/* Discard the saved blanks   */
		*pto++	  = TAB;
		break;

	    default:
		col++;
		for (; numblanks; numblanks--)
		    *pto++ = BLANK;	    /* Spill any saved blanks */
		*pto++ = c;
		break;
	}
    } while (c);

    return (psource);
}

