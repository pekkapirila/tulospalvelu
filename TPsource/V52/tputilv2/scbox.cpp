/**
*
* Name		scbox -- Draw a box on the current display page
*			 using character graphics
*
* Synopsis	ercode = scbox(u_row,u_col,l_row,l_col,
*			     boxtype,boxchar,attrib);
*		int  ercode	  Error code:  0 if okay, 1 if box
*				  dimensions are invalid.
*		int  u_row,u_col  Upper left corner of window.
*		int  l_row,l_col  Lower right corner of window.
*		int  boxtype	  Nature of characters to be used for
*				  the box:  -1 if boxchar to be used,
*				  0 to 15 if IBM box characters to be
*				  used (see below).
*		char boxchar	  Character to be used for drawing the
*				  box if boxtype is -1.
*		int  attrib	  The attribute to be used on the vacant
*				  lines.  It has the background attribute
*				  in the high order four bits of the low
*				  order byte, and the foreground in the
*				  low order nybble.
*
* Description	This function draws a box on the current display page.
*
*		If boxtype is -1, then the box will be drawn using the
*		character specified as boxchar.  If boxtype is in the
*		range 0 to 15, then the box will be drawn using the
*		box-drawing characters in the IBM PC character set.  The
*		actual value of boxtype specifies whether the sides of
*		the box are drawn with single or double lines, according
*		to the following table:
*
*		       Box Type       Bottom  Right   Top    Left
*		       ------------------------------------------
*			0   (0000)	 S	S      S      S
*			1   (0001)	 S	S      S      D
*			2   (0010)	 S	S      D      S
*			3   (0011)	 S	S      D      D
*			4   (0100)	 S	D      S      S
*			5   (0101)	 S	D      S      D
*			6   (0110)	 S	D      D      S
*			7   (0111)	 S	D      D      D
*			8   (1000)	 D	S      S      S
*			9   (1001)	 D	S      S      D
*		       10   (1010)	 D	S      D      S
*		       11   (1011)	 D	S      D      D
*		       12   (1100)	 D	D      S      S
*		       13   (1101)	 D	D      S      D
*		       14   (1110)	 D	D      D      S
*		       15   (1111)	 D	D      D      D
*
*		If a box type is chosen from this table, corner
*		characters will be automatically chosen to match the
*		sides.
*
*		The box dimensions are considered invalid if any corner
*		lies beyond the screen; or if the height or width of the
*		box is less than two characters; or if the upper left
*		corner (u_row,u_col) is below or to the right of the
*		lower right corner (l_row,l_col).  If this occurs then
*		1 is returned as the value of the function.
*
* Returns	ercode		Error code:  0 if okay,
*					     1 if box dimensions are invalid.
*
* Version	6.00 (C)Copyright Blaise Computing Inc.  1985,1987,1989
*
**/

#include <windows.h>
#include "bscreens.h"

    /* Macro to write a single character at a given point on the      */
    /* screen							      */

#define  write_one(row,col,ch,fore,back)	  \
	 {					  \
		sccurset(row,col);		  \
		scattrib(fore,back,ch,1);	  \
	 }

    /* Macro to write more than one character at a given point on the */
    /* screen							      */

#define write_some(row,col,ch,fore,back,number)  \
	 {					  \
	    if (number) 			  \
	    {					  \
		sccurset(row,col);		  \
		scattrib(fore,back,ch,number);	  \
	    }					  \
	 }

int scbox(u_row,u_col,l_row,l_col,boxtype,boxchar,attrib)
int  u_row,u_col,l_row,l_col,boxtype;
char boxchar;
int  attrib;
{
    int  save_row,save_col,high,low;
    int  row,width,fore,back;
    char topleft,topright,botleft,botright,
	 top,bottom,left,right;       /* Characters to write	      */
	extern HANDLE hConOut;
	CONSOLE_SCREEN_BUFFER_INFO ScBufInfo;

    /* Tables of box characters 				      */

    static char upleft[]	      /* Top left corners	      */
			   = {'\332','\326','\325','\311'},
		upright[]	      /* Top right corners	      */
			   = {'\277','\270','\267','\273'},
		lowleft[]	      /* Bottom left corners	      */
			   = {'\300','\323','\324','\310'},
		lowright[]	      /* Bottom right corners	      */
			   = {'\331','\275','\276','\274'};

    static char horiz[] 	      /* Horizontal lines	      */
			   = {'\304','\315'},
		vert[]		      /* Vertical lines 	      */
			   = {'\263','\272'};

    /* Validate the box position and dimensions 		      */

	if (!GetConsoleScreenBufferInfo(hConOut, &ScBufInfo)) {
		GetLastError();
		return 1;
	}
    if (	 0 >  u_row
	  || u_row >= l_row
	  || l_row >= ScBufInfo.dwSize.Y
	  ||	 0 >  u_col
	  || u_col >= l_col
	  || l_col >= ScBufInfo.dwSize.X	 )
	return 1;

    width = l_col - u_col - 1;	      /* Distance between the sides   */
	fore = attrib & 0xff;
	back = attrib >> 8;

    /* Look up the box characters from the tables		      */

    if (boxtype < 0)
	topleft  = topright = botleft = botright
		 = left     = right   = top	 = bottom   = boxchar;
    else
    {
	topleft  = upleft  [  boxtype	    & 3];
	topright = upright [ (boxtype >> 1) & 3];
	botleft  = lowleft [((boxtype >> 2) & 2) | (boxtype & 1)];
	botright = lowright[ (boxtype >> 2) & 3];

	top	 = horiz[(boxtype >> 1) & 1];
	bottom	 = horiz[(boxtype >> 3) & 1];
	left	 = vert [ boxtype	& 1];
	right	 = vert [(boxtype >> 2) & 1];
    }

    /* Draw the box, starting from upper left corner, working	      */
    /* horizontally.						      */

				      /* Save cursor location	      */
    sccurst(&save_row,&save_col,&high,&low);

    write_one (u_row,u_col,	 topleft, fore,back);
    write_some(u_row,(u_col + 1),top,	  fore,back,width);
    write_one (u_row,l_col,	 topright,fore,back);

    for (row = u_row + 1; row < l_row; row++)
    {
	write_one(row,u_col,left, fore,back);	    /* Left side      */
	write_one(row,l_col,right,fore,back);	    /* Right side     */
    }

    write_one (l_row,u_col,	 botleft, fore,back);
    write_some(l_row,(u_col + 1),bottom,  fore,back,width);
    write_one (l_row,l_col,	 botright,fore,back);

    sccurset(save_row,save_col);      /* Restore cursor location      */

    return 0;
}

