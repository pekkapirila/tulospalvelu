// Pekka Pirila's sports timekeeping program (Finnish: tulospalveluohjelma)
// Copyright (C) 2015 Pekka Pirila 

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include <tputil.h>
//int viwrrect(int u_row, int u_col, int l_row, int l_col,
//			const char *buf, int fore, int back, int option);

void draw_hline(int row, int col, int len)
   {
   char hline[81];
   int i;

   for (i = 0; i < 80; i++)
	   hline[i] = 183;
   viwrrect(row, col, row, col+len-1, hline, 7, 0, 0);
   }

void draw_vline(int row, int col, int len)
   {
   char vline[51];
   int i;

   for (i = 0; i < 50; i++)
	   vline[i] = 166;
   viwrrect(row, col, row+len-1, col, vline, 7, 0, 0);
   }

void draw_hline2(int row, int col, int len)
   {
   char hline[81];
   int i;

   for (i = 0; i < 80; i++)
	   hline[i] = 192;

   viwrrect(row, col, row, col+len-1, hline, 7, 0, 0);
   }

void draw_vline2(int row, int col, int len)
   {
   char vline[51];
   int i;

   for (i = 0; i < 50; i++)
	   vline[i] = 173;
   viwrrect(row, col, row+len-1, col, vline, 7, 0, 0);
   }

void draw_grchar2(int row, int col, int ch)
   {
   char cgr[] = "   È Í¼Ê ÉºÌ»Ë¹Î";
   char st[2];

   st[0] = cgr[ch]-13;
   viwrrect(row,col,row,col,st,7,0,0);
}
void draw_grchar(int row, int col, int ch)
   {
   char cgr[] = "   À ÷ÙÁ Ú³Ã¿Â´Å";
   char st[2];

   st[0] = cgr[ch]-13;
   viwrrect(row,col,row,col,st,7,0,0);
   }

int scbox(int u_row, int u_col, int l_row, int l_col, int boxtype, char boxchar, int attrib)
{
	if (boxtype == 15) {
		draw_vline2(u_row+1, u_col, l_row-u_row-1);
		draw_vline2(u_row+1, l_col, l_row-u_row-1);
		draw_hline2(u_row, u_col+1, l_col-u_col-1);
		draw_hline2(l_row, u_col+1, l_col-u_col-1);
		draw_grchar2(u_row, u_col, 9);
		draw_grchar2(u_row, l_col, 12);
		draw_grchar2(l_row, u_col, 3);
		draw_grchar2(l_row, l_col, 6);
	}
	else {
		draw_vline(u_row+1, u_col, l_row-u_row-1);
		draw_vline(u_row+1, l_col, l_row-u_row-1);
		draw_hline(u_row, u_col+1, l_col-u_col-1);
		draw_hline(l_row, u_col+1, l_col-u_col-1);
		draw_grchar(u_row, u_col, 9);
		draw_grchar(u_row, l_col, 12);
		draw_grchar(l_row, u_col, 3);
		draw_grchar(l_row, l_col, 6);	
	}
	return(0);
}