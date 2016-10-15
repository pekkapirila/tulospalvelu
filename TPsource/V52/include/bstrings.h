/**
*
*  BSTRINGS.H-- Header file for Turbo C TOOLS String functions.
*
*  Version	6.00  (C)Copyright Blaise Computing Inc.  1983,1987,1989
*
**/

#ifndef DEF_BSTRINGS		      /* Prevent redefinition.	    */

#define DEF_BSTRING  1			/* Prevent second reading   */
					/* of these definitions.    */

#include <stdlib.h>

/* Direction codes for STPJUST					    */

#define JUST_LEFT   (-1)
#define JUST_CENTER   0
#define JUST_RIGHT    1


/* Format codes for STPCVT					    */

#define RWHITE	   1	/* Remove white space.			    */
#define RLWHITE    2	/* Remove leading white space.		    */
#define RTWHITE    4	/* Remove trailing white space. 	    */
#define REDUCE	   8	/* Reduce white space to 1 blank.	    */
#define NOQUOTE   16	/* Quoted substrings not altered.	    */
#define TOUP	  32	/* Convert to upper case.		    */
#define TOLOW	  64	/* Convert to lower case.		    */
#define RCONTROL 128	/* Remove all control characters.	    */

/* Function declarations					    */

#ifdef __cplusplus
//extern "C" {
#endif

char * cdecl stpcvt(char *,int);	/* Convert a string.	    */
					/*			    */
char * cdecl stpexpan(char *,		/* Expand TABs in a string. */
	     char *,int,int);		/*			    */
					/*			    */
char * cdecl stpjust(char *,		/* Justify a string within  */
	     const char *,char,int,int);/* a field.		    */
					/*			    */
char * cdecl stptabfy(char *,int);	/* Compress blanks to TABs. */
					/*			    */
char * cdecl stpxlate(char *,		/* Translate a string	    */
	     const char *,const char *);/*			    */
					/*			    */
int    stschind(char,const char *);	/* Find a char in a string. */

int    wcswcind(wchar_t, const wchar_t *);	/* Find a char in a string. */

#ifdef __cplusplus
//}
#endif

#endif					/* End "#ifndef DEF_BSTRING"*/

