/**
*
* Name		scclrmsg -- Clear a message on the screen.
*
* Synopsis	iret = scclrmsg(row,col,len);
*
*		int iret	  0 is always returned.
*		int row,col	  Position to clear.
*		int len 	  Length of message to clear.
*
* Description	This function simply clears a message at a specified
*		screen position without changing the screen display
*		attributes.  The current cursor position is saved and
*		restored.
*
*		The area cleared may wrap beyond the current line.
*
* Version	6.00 (C)Copyright Blaise Computing Inc.  1983,1987,1989
*
**/

#include "bscreens.h"

#define BLANK	' '

int scclrmsg(int row, int col, int len)
{
    int c_row,c_col,high,low;

    sccurst(&c_row,&c_col,&high,&low);/* Current position.	      */
    sccurset(row,col);
    scwrite((char) BLANK,len);	      /* Write len blanks.	      */
    sccurset(c_row,c_col);	      /* Return the original position.*/

    return(0);
}
