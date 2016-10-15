/**
*
* Name		STPCVT -- String conversion.
*
* Synopsis	presult = stpcvt (psource,conv);
*
*		char *presult	  Pointer to the converted string
*		char *psource	  Pointer to the source string
*		int conv	  Conversion code
*
* Description	STPCVT converts the string psource according to the
*		instructions specified in conv and returns a pointer to
*		the altered string.  The string psource is itself
*		altered.  The possible conversion codes are:
*
*		RWHITE	  (1) - Discard all white space.
*		RLWHITE   (2) - Discard all leading white space.
*		RTWHITE   (4) - Discard all trailing white space.
*		REDUCE	  (8) - Reduce all white space to one space.
*		NOQUOTE  (16) - Characters enclosed in single or double
*				quotes are not to be altered.
*		TOUP	 (32) - Convert lower case characters to upper case.
*		TOLOW	 (64) - Convert upper case characters to lower case.
*		RCONTROL(128) - Discard all control characters.
*
*		The conversion codes can be combined by adding them
*		together.  Except for 16, the codes are executed in the
*		order shown.  For example, 33 discards all white space
*		and converts all lower case characters to upper case
*		characters.  The code 49 does the same, but no
*		conversion takes place within quoted substrings.
*
*		Notice that white space and control characters are
*		defined to be ASCII characters (code less than 128) as
*		reported by isascii().	Therefore, the special
*		characters having ASCII codes greater than 127 are not
*		altered.
*
*		Trailing white space will not be protected if unbalanced
*		quoted substrings are present.
*
* Returns	presult 	Pointer to the converted string.
*		psource 	Pointer to the altered source string.
*				It is the same as presult.
*
* Version	6.00 (C)Copyright Blaise Computing Inc.  1983, 1987, 1989
*
**/

#include <ctype.h>		/* Definition of char manipulation	*/
				/* macros.				*/

#include "bstrings.h"
#include "tputil.h"

#define TRUE	   1
#define FALSE	   0
#define BLANK	 ' '

char *stpcvt(char *psource, int conv)
{
    char *pfrom        = psource;	/* Next character to get    */
					/* from source. 	    */

    register char *pto = psource;	/* Next position to fill in */
					/* target.		    */
    register char c;
    char quote_char = '\0';

    /* Convenient flags (not to be changed):			    */

    int rlwhite      = conv & RLWHITE;
    int rwhite	     = conv & RWHITE;
    int reduce	     = (!rwhite) && (conv & REDUCE);
    int ckquotes     = conv & NOQUOTE;
    int to_up	     = conv & TOUP;
    int to_low	     = conv & TOLOW;
    int rcontrol     = conv & RCONTROL;

    /* Flags indicating current state as we process the string	    */
    /* (these change as we encounter various characters):	    */

    int in_white     = FALSE;	/* Not in a white field yet.	    */
    int hit_nonwhite = FALSE;	/* No nonwhite chars encountered.   */
    int quote_on     = FALSE;	/* Not in a quote yet.		    */

    /* This main loop handles everything but trailing white space.  */
    /* Each pass handles one character c from psource, writing zero */
    /* or one character back to psource.  We can process psource in */
    /* place this way because we're never adding additional chars.  */

    while ((c = *pfrom++) != '\0')
    {
	if (quote_on)
	{				/* We're inside a quoted    */
	    *pto++ = c; 		/* substring.		    */
	    if (c == quote_char)
		quote_on = FALSE;	/* Found end of the quote.  */
	}
	else if (ckquotes && ((c == '"') || (c == '\'')))
	{				/* Begin new quote.	    */
	    *pto++ = c;
	    in_white	 = FALSE;
	    hit_nonwhite = TRUE;
	    quote_on	 = TRUE;
	    quote_char	 = c;
	}
	else if (isspace(c) && isascii(c))
	{
	    if (rwhite)
		;	/* Do nothing with character c. 	    */

	    else if (rlwhite && !hit_nonwhite)
		;	/* Leading white space to omit-- do nothing */
			/* with character c.			    */

	    else if (reduce)
	    {
		if (in_white)
		    ;	/* We're within white block-- do nothing    */
			/* with character c.			    */

		else
		{	/* Begin block of white space.		    */
		    *pto++ = BLANK;
		    in_white = TRUE;
		}
	    }

	    else if (rcontrol && iscntrl (c))
		;	/* Remove control characters which are	    */
			/* defined as whitespace.		    */

	    else
			/* Just copy normally . 		    */
		*pto++ = c;
	}
	else if (iscntrl(c) && isascii(c))
	{
	    in_white	 = FALSE;
	    hit_nonwhite = TRUE;

	    if (rcontrol)
		;	/* Don't copy the ctrl char.                */

	    else
			/* Just copy normally.			    */
		*pto++ = c;
	}
	else
	{				/* Simple nonwhite or	    */
	    in_white	 = FALSE;	/* control character.	    */
	    hit_nonwhite = TRUE;

	    if (isascii(c))
	    {
		if (to_up)
		    c = toupper(c);

		if (to_low)
		    c = tolower(c);
	    }

	    *pto++ = c; /* Just copy normally.			    */
	}
    }
    *pto = '\0';

    /* Now handle trailing white space...			    */

    if (conv & RTWHITE)
	for (c = *--pto;
	     isspace(c) && isascii(c) && (pto >= psource);
	     c = *--pto)
		*pto = '\0';

    return (psource);
}

wchar_t *wstpcvt(wchar_t *psource, int conv)
{
	wchar_t *pfrom        = psource;	/* Next character to get    */
					/* from source. 	    */

	wchar_t *pto = psource;	/* Next position to fill in */
					/* target.		    */
	wchar_t c;
	wchar_t quote_char = '\0';

	/* Convenient flags (not to be changed):			    */

	int rlwhite      = conv & RLWHITE;
	int rwhite	     = conv & RWHITE;
	int reduce	     = (!rwhite) && (conv & REDUCE);
	int ckquotes     = conv & NOQUOTE;
	int to_up	     = conv & TOUP;
	int to_low	     = conv & TOLOW;
	int rcontrol     = conv & RCONTROL;

	/* Flags indicating current state as we process the string	    */
	/* (these change as we encounter various characters):	    */

	int in_white     = FALSE;	/* Not in a white field yet.	    */
	int hit_nonwhite = FALSE;	/* No nonwhite chars encountered.   */
	int quote_on     = FALSE;	/* Not in a quote yet.		    */

	/* This main loop handles everything but trailing white space.  */
	/* Each pass handles one character c from psource, writing zero */
	/* or one character back to psource.  We can process psource in */
	/* place this way because we're never adding additional chars.  */

	while ((c = *pfrom++) != L'\0')
	{
	if (quote_on)
	{				/* We're inside a quoted    */
		*pto++ = c; 		/* substring.		    */
		if (c == quote_char)
		quote_on = FALSE;	/* Found end of the quote.  */
	}
	else if (ckquotes && ((c == L'"') || (c == L'\'')))
	{				/* Begin new quote.	    */
		*pto++ = c;
		in_white	 = FALSE;
		hit_nonwhite = TRUE;
		quote_on	 = TRUE;
		quote_char	 = c;
	}
	else if (iswspace(c) && iswascii(c))
	{
		if (rwhite)
		;	/* Do nothing with character c. 	    */

	    else if (rlwhite && !hit_nonwhite)
		;	/* Leading white space to omit-- do nothing */
			/* with character c.			    */

	    else if (reduce)
	    {
		if (in_white)
		    ;	/* We're within white block-- do nothing    */
			/* with character c.			    */

		else
		{	/* Begin block of white space.		    */
		    *pto++ = BLANK;
		    in_white = TRUE;
		}
	    }

		else if (rcontrol && iscntrl (c))
		;	/* Remove control characters which are	    */
			/* defined as whitespace.		    */

	    else
			/* Just copy normally . 		    */
		*pto++ = c;
	}
	else if (iscntrl(c) && iswascii(c))
	{
		in_white	 = FALSE;
		hit_nonwhite = TRUE;

		if (rcontrol)
		;	/* Don't copy the ctrl char.                */

		else
			/* Just copy normally.			    */
		*pto++ = c;
	}
	else
	{				/* Simple nonwhite or	    */
	    in_white	 = FALSE;	/* control character.	    */
	    hit_nonwhite = TRUE;

		if (iswascii(c))
		{
		if (to_up)
			c = towupper2(c);

		if (to_low)
			c = towlower2(c);
		}

		*pto++ = c; /* Just copy normally.			    */
	}
	}
	*pto = '\0';

	/* Now handle trailing white space...			    */

	if (conv & RTWHITE)
	for (c = *--pto;
		 iswspace(c) && iswascii(c) && (pto >= psource);
		 c = *--pto)
		*pto = '\0';

	return (psource);
}

