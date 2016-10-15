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

#ifndef VWINDEF_H_DEFINED
#define VWINDEF_H_DEFINED
#include <vcl.h>
#include "VDef.h"
#include <xmlparser.h>

#define NFORMVAGR 6
#define NTILANNEFORM 6
#define NFORMOSUUS 6
#define NSEURANTAFORM 6
#define NJOUKKFORM 6
#define NTAPFORM 4

#define FLDID_KILPNO 1
#define FLDID_JOUKKID 2
#define FLDID_PIIRI 3
#define FLDID_SEURA 4
#define FLDID_JOUKKNO 5
#define FLDID_JOUKKNIMI 6
#define FLDID_MAA 7
#define FLDID_SARJA 8
#define FLDID_ERA 9
#define FLDID_ILMLISTA 10
#define FLDID_PISTE1 11
#define FLDID_PISTE2 12
#define FLDID_PISTE3 13
#define FLDID_KILPID 14
#define FLDID_SUKUNIMI 15
#define FLDID_ETUNIMI 16
#define FLDID_ARVO 17
#define FLDID_OSSEURA 18
#define FLDID_BADGE 19
#define FLDID_BADGE2 20
#define FLDID_LAINA 21
#define FLDID_LAINA2 22
#define FLDID_RATA   23
#define FLDID_TARK 24
#define FLDID_AMPSAKKO 25
#define FLDID_SAKKO 26
#define FLDID_OSPISTE1 27
#define FLDID_OSPISTE2 28
#define FLDID_OSPISTE3 29
#define FLDID_SELITYS 30
#define FLDID_LAHTO 31
#define FLDID_LAHTOLAJI 32
#define FLDID_MAALI 33
#define FLDID_SIJA 34
#define FLDID_NIMI 35
#define FLDID_VAAIKA 36
#define FLDID_VASIJA 37

/*
void VIWRRECT(int r1, int c1, int r2, int c2, char *txt);
void VIDSPMSG(char *txt);
void scclrmsg(int y, int x, int x2);

#define viwrrect(r1, c1, r2, c2, txt, fg, bg, opt) \
	VIWRRECT((r1), (c1), (r2), (c2), (txt))
#define vidspmsg(row, col, fg, bg, txt) VIDSPMSG((txt))
*/

#define selectopt(st, opt, sel) Win_selectopt((st), (opt), (sel))
#define wselectopt(st, opt, sel) Win_wselectopt((st), (opt), (sel))
void scpclr(void);
//void sccurset(int x, int y);
void DSPQ(INT r,INT cn);
void WRITEYHT(wchar_t *st, INT cn);
static const System::Uitypes::TColor clVaaleanHarmaa = System::Uitypes::TColor(0xe0e0e0);
int ShiftStateInt(TShiftState KeyShift);
TShiftState IntShiftState(int state);
void writewarning_w(wchar_t *txt, int wait);

#define WM_MYMSGNEWCARD		(WM_APP + 2201)
#define WM_MYMSGCLOSEAPPL  	(WM_APP + 2202)
#define WM_MYMSGDSPMSG		(WM_APP + 2203)
#define WM_MYMSGDSPERRORMSG	(WM_APP + 2204)
#define WM_MYLUKUMAARAT		(WM_APP + 2205)
#define WM_MYSELECTOPT		(WM_APP + 2206)
#define WM_MYDISPLTIMER		(WM_APP + 2207)
#define WM_AUTOTULOSTUS 	(WM_APP + 2208)
#define WM_MYADDSAKKO	 	(WM_APP + 2209)
#define WM_MYECODE		 	(WM_APP + 2210)
#define WM_MYSCROLL		 	(WM_APP + 2211)
#define WM_MYUUSIAIKA		(WM_APP + 2212)
#define WM_MYPAIVITA		(WM_APP + 2214)
#define WM_MYHAEAVOIMET     (WM_APP + 2215)
#define WM_MYTIEDOT 		(WM_APP + 2216)
#define WM_MYMSGDSPWARNMSG	(WM_APP + 2217)

struct TMyMessage {
//	Cardinal Msg;
	int Msg;
	int wparam;
	int lparam;
	LRESULT Result;
	};

extern wchar_t *aptr;

typedef struct {
	UnicodeString  text;
	int         style;
	int			justify;
	} cellTp;

//---------------------------------------------------------------------------
typedef struct {
	wchar_t Caption[20];
	int Width;
	int Justify;
	int Count;
	int Order;
	bool Visible;
} ColTp;

//---------------------------------------------------------------------------
class EdtField {
public:
	wchar_t Caption[30];
	TEdit   *Edt;
	TLabel	*Lbl;
	TEdit	*EOrd;
	TCheckBox *CB;
	void	*Data;
	int		DataType;  // 0: int, 2: wchar_t, 3: lähtöaika
	int		Count;

	__fastcall EdtField(void);
	__fastcall ~EdtField(void);
	void Set(wchar_t *C, TEdit *E, TLabel *L, TEdit *O, TCheckBox *B, void *D, int T, int N);
};

typedef struct {
	wchar_t name[20];
	int id;
	INT akt;
	UINT len;
	INT oik;
	} FLD2;

extern FLD2 exp_kentat[];

extern int Kexp_kentat, Pexp_kentat, Vexp_kentat;

//---------------------------------------------------------------------------
typedef struct {
	wchar_t ref[22];
	wchar_t linkki[162];
	} tietolinkkitp;

extern tietolinkkitp *HenkLinkki;

extern int ppi0;
//---------------------------------------------------------------------------

int UStrToAika(UnicodeString USt, int t0);
void Win_selectopt(char *st, char *opt, char *sel);
void Win_wselectopt(wchar_t *st, wchar_t *opt, wchar_t *sel);
void inputaika_prompt(INT32 *t,int t0, int l, wchar_t *prompt, wchar_t *ret, TForm *Sender = NULL);
void inputint_prompt(INT32 *i, wchar_t *prompt, wchar_t *ret, TForm *Sender = NULL);
void inputstr_prompt(wchar_t *st, int l, wchar_t *prompt, wchar_t *ret, TForm *Sender = NULL);
int select3(int nButton, wchar_t *Teksti, wchar_t *Otsikko,
		wchar_t *Vast1, wchar_t *Vast2, wchar_t *Vast3, TComponent *Sender);
void Wselectxy(INT y, INT x, wchar_t *prompt, wchar_t *term, wchar_t *tc);
void nayta_virhesanomat(void);

class seurIkkParamClass {
public:
   int Sarja;
   int Va;
   int Osuus;
   bool OsJarj;
   bool Ero;
   bool EtuSuku;
   int FontSize;
   bool Lihavoitu;
   int ColW[11];
   int laika;
   int Seuraa;
   int ScrollVali;
   int ScrollRiviKesto;
   bool ScrollOn;
   int ScrollType;
   bool KokoRuutu;
   int ScrollSarjat[MAXSARJALUKU+MAXYHD+1];
   seurIkkParamClass(void);
   void operator=(seurIkkParamClass&);
   int writeParams(TextFl *outfl, int level);
   int readParams(xml_node *node, int *inode, int nnode);
};

class tapIkkParamClass {
public:
   int Va;
   bool naytaMaa;
   bool KokoRuutu;
   bool EtuSuku;
   int FontSize;
   bool Lihavoitu;
   int ColW[11];
   int laika;
   bool OhitaTarkSan;
   tapIkkParamClass(void);
   void operator=(tapIkkParamClass&);
   int writeParams(TextFl *outfl, int level);
   int readParams(xml_node *node, int *inode, int nnode);
};

class aikavertIkkParamClass {
public:
	int Jono1;
	int Jono2;
	int AikaVert;
	int NoVert;
	int FontSize;
	int AikaRajat[2];
	int ColW[24];
	int Visible[24];
	aikavertIkkParamClass(void);
	void operator=(aikavertIkkParamClass&);
	int writeParams(TextFl *outfl, int level);
	int readParams(xml_node *node, int *inode, int nnode);
};

class statusIkkParamClass {
public:
	int esitykset;
	int ylahdot;
	int emittiedot;
	int emitkoodeja;
	int yhteysikkuna;
	statusIkkParamClass(void);
	void operator=(statusIkkParamClass&);
	int writeParams(TextFl *outfl, int level);
	int readParams(xml_node *node, int *inode, int nnode);
};

class emitIkkParamClass {
public:
	bool NaytaKortinKoodi;
	int  KoneTeht;
	bool MerkLainat;
	bool VaadiToisto;
	bool LueVialliset;
	emitIkkParamClass(void);
	void operator=(emitIkkParamClass&);
	int writeParams(TextFl *outfl, int level);
	int readParams(xml_node *node, int *inode, int nnode);
};

class luentaIkkParamClass {
public:
	int Tunnistus;
	int Koodi;
	bool Huomautaohitetusta;
	bool Huomautasuljetusta;
	bool EhdOsuus;
	luentaIkkParamClass(void);
	void operator=(luentaIkkParamClass&);
	int writeParams(TextFl *outfl, int level);
	int readParams(xml_node *node, int *inode, int nnode);
};

class NaytaTulosIkkParamClass {
public:
	int NaytaTulos;
	int OkKesto;
	int OngKesto;
	wchar_t Teksti[15][30];
	NaytaTulosIkkParamClass(void);
	void operator=(NaytaTulosIkkParamClass&);
	int writeParams(TextFl *outfl, int level);
	int readParams(xml_node *node, int *inode, int nnode);
};

#endif
