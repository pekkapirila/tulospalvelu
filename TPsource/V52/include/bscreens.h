/**
*
*  BSCREENS.H	Header file for Turbo C TOOLS Screen Functions
*
*  Version 6.00 (C) Copyright Blaise Computing Inc. 1983, 1987-1989
*
**/

#ifndef DEF_BSCREENS		      /* Prevent second reading of    */
#define DEF_BSCREENS 1		      /* these definitions.	      */

#include "butil.h"

#define  NORMAL     7		      /* Common attributes for the    */
#define  REVERSE    112 	      /* monochrome adapter	      */
#define  UNDERLINE  1
#define  INTENSITY  8
#define  MONOBLINK  128

#define  SC_BLACK   0		      /* (Byte) values representing   */
#define  SC_BLUE    1		      /* color attributes.	      */
#define  SC_GREEN   2
#define  SC_CYAN    3
#define  SC_RED     4
#define  SC_MAGENTA 5
#define  SC_BROWN   6
#define  SC_WHITE   7

    /* Maximum dimensions of usual PC screens			      */

#define  PC_ROWS      25
#define  PC_COLS      80
#define  LAST_ROW     (PC_ROWS - 1)
#define  LAST_COL     (PC_COLS - 1)
#define  PC_BIG_ROWS  50
#define  MAX_DEVICES  2 	      /* Color and/or monochrome      */
#define  MAX_PAGES    8 	      /* Pages per video adapter      */
				      /* (8 maximum on EGA)	      */

    /* Scrolling directions					      */

#define  SCR_UP      0
#define  SCR_DOWN    1
#define  SCR_RIGHT   0
#define  SCR_LEFT    1

    /* Function declarations					      */
#ifdef __cplusplus_x
extern "C" {
#endif

int cdecl  scattrib(int,int,char,unsigned);    /* Write copies of a character with attributes */
				      /*			      */
void cdecl scpclr(void);	      /* Clear the current page       */
				      /*			      */
int cdecl scclrmsg(int,int,int);      /* Clear a message	      */
				      /*			      */
int cdecl  sccurset(int,int);	      /* Set the cursor position      */
				      /*			      */
int cdecl  sccurst(int *,int *,int *, /* Return position & size of    */
		   int *);	      /* cursor on current page.      */
				      /*			      */
int cdecl  scwrite(char,unsigned);    /* Write copies of a character  */
				      /*			      */
int cdecl  scbox(int,int,int,int,     /* Draw a box		      */
		 int,char,int);       /*			      */
				      /*			      */

#ifdef __cplusplus_x
}
#endif
#endif				      /* Ends "#ifndef DEF_BSCREENS"  */
