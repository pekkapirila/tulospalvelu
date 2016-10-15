/**
*
*  BKEYBRD.H	Header file for BIOS keyboard functions for Turbo C TOOLS
*
*  Version	6.00 (C)Copyright Blaise Computing Inc.  1986,1987,1989
*
**/

#ifndef DEF_BKEYBRD		/* Prevent redefinition.	    */
#define DEF_BKEYBRD 1		/* Prevent second reading of these  */
				/* definitions. 		    */

#include <ctype.h>
#include "butil.h"

/* Function declarations.					    */

#ifdef __cplusplus
//extern "C" {
#endif


		/* Normal BIOS operations.			    */
int   cdecl kbready (char *, int *);	/* Check for a keystroke.   */
int   cdecl kbgetkey (int *);		/* Await & return char and  */
					/*   scan code via BIOS.    */
int   cdecl kbflush (void);		/* Flush keyboard buffer.   */

#ifdef __cplusplus
//}
#endif

#endif				/* Ends "#ifndef DEF_BKEYBRD"       */

