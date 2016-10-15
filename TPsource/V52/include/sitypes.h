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

typedef struct {
        char  cc;
        char  ct[2];
        } SI5ctp;

typedef struct {
        char  ccx;
        SI5ctp  c[5];
        } SI5row;

typedef struct {
        char  stx;
		  char  CD49;
		  char  CSI;
		  char  CI[4];
        char  CN[2];
        char  CNS;
        char  fill1[10];
        char  SN[2];
        char  ST[2];
        char  FT[2];
        char  PC;
        char  fill2;
        char  CT[2];
        char  SW;
        char  SNS;
        char  SNCS;
        char  fill3[2];
        SI5row row[6];
		  char  CS;
		  char  etx;
        } SI5tp;

typedef struct {
        char  PTD;
        char  CN;
        char  PT[2];
        } SI6P;

typedef struct {
        char   stx;
		  char   CD97;
		  char   CSI;
		  char   BN;
        SI6P    punch[32];
		  char  CS;
		  char  etx;
		  } SI6PBLK;

typedef struct {
        char   stx;
		  char   CD97;
		  char   CSI;
		  char   BN;
        UINT32  DStr;
        UINT32  fill1;
        char   CTI;
        char   CPP;
        char   CN[4];
        UINT16  CNCRC;
        UINT16  LastC;
        char   PP;
        char   PP1;
        SI6P    fi;
        SI6P    st;
        SI6P    chk;
        SI6P    clr;
        SI6P    last;
        UINT32  SN;
        UINT32  fill2;
        char    surname[20];
        char    firstname[20];
        char    country[4];
        char    club[36];
		  char  CS;
		  char  etx;
		  SI6PBLK pblk[2];  
        } SI6tp;

 