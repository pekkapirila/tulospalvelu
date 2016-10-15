/**
*
* Name		STSCHIND -- Returns the position of the first occurrence
*		of a character within a string.
*
* Synopsis	index = stschind (check, psearch);
*
*		int index	     The position of check within
*				     psearch.
*		char check	     Character to be searched for.
*		const char *psearch  Pointer to the string to be
*				     searched.
*
* Description	STSCHIND searches the string psearch for the character
*		check, returning the position of its first occurrence.
*		If the character does not occur, -1 is returned.
*
* Returns	index		The position of check in psearch.
*
* Version	6.00 (C)Copyright Blaise Computing Inc.  1987, 1989
*
**/


#include <stdio.h>	/* For definition of NULL.		    */
#include <string.h>

#include "bstrings.h"


int stschind (char check, const char *psearch)
{
	 const char *p;

	 if (check == 0)
		 return (-1);
	 if ((p = strchr (psearch, check)) == NULL)
	return (-1);

	 return ((int) (p - psearch));
}

int wcswcind (wchar_t check, const wchar_t *psearch)
{
	 const wchar_t *p;

	 if (check == 0)
		 return (-1);
	 if ((p = wcschr (psearch, check)) == NULL)
		return (-1);

	 return ((int) (p - psearch));
}


