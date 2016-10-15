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

#ifndef TPDEFDEFINED
#define TPDEFDEFINED

#define KB_S_N_UP 72
#define KB_S_N_DOWN 80
#define KB_S_N_PGUP 73
#define KB_S_N_PGDN 81
#define KB_S_N_END 79
#define BS      8
#define KB_TAB     9
#define BACKTAB 11
#define KB_LF     10
#define FF     12
#define KB_CR     13
#define EF     26
#define ESC    27
#define UP      5
#define LEFT   19
#define RIGHT   4
#define DOWN   24
#define PGUP    18
#define PGDN    3
#define C_PGUP 19
#define C_PGDN  2

#define HYVPAK  9
#define HYL 17
#define SOH 1
#define STX 2
#define ETX 3
#define EOT 4
#define ACK 6
#define NAK 21
#define MAXODOTUS 10
#define PRBLOCKSIZE 2048
#define PRBLOCKBITS 11

#define OK 0
#define INV_PORT 2
#define PORT_NOT_OPEN 3
#define OUT_Q_FULL 7
#define IN_Q_EMPTY 10
#define NO_PORT_FOUND 6
#define STATUS_ERR    0x00F3
#define DAYTICKS 1573040L
#define COMINTVAL 9
#define COMINTR 0x66
#define COM1ADS 0x03F8
#define COM2ADS 0x02F8

#define SU 0
#define SV 2
#define EN 1
#define LANG SU
#define MAXLANG 2

#define TRUE    1
#define FALSE   0
#define UNDERSCORE  =  '_'
#define CRIT_VEC 0x24

#define PORTBASE 15900

#define CSEK 10L
#define KSEK (10L*CSEK)
#define SEK (10L*KSEK)
#define MINUUTTI (600L*KSEK)
#define TUNTI (36000L*KSEK)
#define VRK (24*TUNTI)

#define AIKAJAK (10000L/SEK)
#define TMAALI0 (-24*TUNTI)

#define ABS(i) labs(i)
#define NOLLAA(x) memset((char *)(x), 0, sizeof((x)))
#define mene(x,y)  gotoxy((x),(y))
#define elimbl(s) stpcvt((s),6)
#define elimwbl(s) wstpcvt((s),6)
#define naytateksti(y,x,f,b,s) vidspwmsg((y),(x),(f),(b),(s))
#define clrscr() scpclr()
#define clearscreen() scpclr()
#define inputwstr(s,l,x,y,term,tc,numfl) inputwstr2((s),(l),(x),(y),(term),(tc),(numfl))
#define biostime(x,y) timeticks()
#define T_TIME(x) t_time_l((x), (t0))
#define NORMKELLO(t) (((t)+60L*TUNTI)%(24L*TUNTI)-12L*TUNTI)
#define NORMKELLO_A(t) normkello_a((t))
#define NORMTULOS(t) (((t)+48L*TUNTI)%(24L*TUNTI))
#define NORMTULOS_A(t) normtulos_a((t))
#define AIKATOSTR(t, t0) aikatostr_t((t), (t0))
#define AIKATOSTRS(s, t, t0) aikatostr_ts((s), (t), (t0))
#define AIKATOWSTRS(s, t, t0) aikatowstr_ts((s), (t), (t0))
#define AIKATOWSTR(t, t0) aikatowstr_t((t), (t0))
#define WSTRTOAIKA(st, t0) wstrtoaika_t((st), (t0))
#define INPUTAIKA(tm,t0,l,x,y,term,tc) \
   inputaika_t((tm),(t0),(l),(x),(y),(term),(tc))
#define INPUTAIKAW(tm,t0,l,x,y,term,tc) inputaika_tw((tm),(t0),(l),(x),(y),(term),(tc))
#define INPUTINT(x1,x2,x3,x4,x5,x6)  inputlong((x1),(x2),(x3),(x4),(x5),(x6))
#define INPUTINTW(x1,x2,x3,x4,x5,x6)  inputlongw((x1),(x2),(x3),(x4),(x5),(x6))

#define KB_C_C_A 0
#define KB_S_C_A 1
typedef struct {
   char ch;
   char keycode;
	} IV_KEY;

typedef INT32 TMAALI;
typedef UINT32 TULOS;
typedef void *void_pointer;

typedef struct {
	bool inSendCopy;
	int valilaji;
	int aloituslaji;
	int cn;
	int Jono;
	int NJono;
	double ValiKerroin;
	int ens;
	int viim;
	int t_alku;
	int t_vert;
	int t_aloitus;
	int RecLen;
	int LahRivi;
	wchar_t ComFileName[200];
	FILE *ComFile;
} sendtestparamtp;

typedef struct{
   int  baud;
   int  bits;
   int  sbits;
   char  parity;
   } comparamtp;

#endif // TPDEFDEFINED

