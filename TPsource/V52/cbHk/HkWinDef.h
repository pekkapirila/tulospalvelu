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

#ifndef HKWINDEF_DEFINED
#define HKWINDEF_DEFINED

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>
#include "HkDef.h"
#include <xmlparser.h>

#define selectopt(st, opt, sel) Win_selectopt((st), (opt), (sel))
#define wselectopt(st, opt, sel) Win_wselectopt((st), (opt), (sel))
void SuljeKaikki(void);
int UStrToAika(UnicodeString USt, int t0);
void inputaika_prompt(INT32 *t,int t0, int l, wchar_t *prompt, wchar_t *ret, TForm *Sender = NULL);
void inputint_prompt(INT32 *i, wchar_t *prompt, wchar_t *ret, TForm *Sender = NULL);
void inputstr_prompt(wchar_t *st, int l, wchar_t *prompt, wchar_t *ret, TForm *Sender = NULL);
wchar_t FirstChar(UnicodeString str);
int select3(int nButton, wchar_t *Teksti, wchar_t *Otsikko,
		wchar_t *Vast1, wchar_t *Vast2, wchar_t *Vast3, TComponent *Sender);
void Win_wselectopt(wchar_t *msg, wchar_t *opts, wchar_t *sel);
void Win_selectopt(char *msg, char *opts, char *sel);
void nayta_virhesanomat(void);
wchar_t *stdDateStr(wchar_t *st, int pvm);
int decodeStdDateStr(wchar_t *st);
int ShiftStateInt(TShiftState KeyShift);
TShiftState IntShiftState(int state);
void writewarning_w(wchar_t *txt, int wait);

static const System::Uitypes::TColor clVaaleanHarmaa = System::Uitypes::TColor(0xe0e0e0);
//#define clVaaleanHarmaa 0xe0e0e0

#define MAXTAPAHTUMA 400

#define FLDID_KILPNO	1
#define FLDID_KILPID	2
#define FLDID_INTID		3
#define FLDID_PIIRI		4
#define FLDID_SUKUNIMI	5
#define FLDID_ETUNIMI	6
#define FLDID_ARVO		7
#define FLDID_SEURA		8
#define FLDID_SEURALYH	9
#define FLDID_YHD		10
#define FLDID_JK		11
#define FLDID_MAA		12
#define FLDID_SARJA		13
#define FLDID_SUKUP		14
#define FLDID_SYNT		15
#define FLDID_IKASRJ	16
#define FLDID_ARVRYH	17
#define FLDID_ILMLISTA	18
#define FLDID_PISTE1	19
#define FLDID_PISTE2	20
#define FLDID_PISTE3	21
#define FLDID_BIB		22
#define FLDID_VSARJA	23
#define FLDID_BADGE		24
#define FLDID_LAINA		25
#define FLDID_BADGE2	26
#define FLDID_LAINA2	27
#define FLDID_LAHTOARV	28
#define FLDID_LAHTOTOT	29
#define FLDID_TULOS		30
#define FLDID_TARK		31
#define FLDID_SIJA		32
#define FLDID_AMPSAKKO	33
#define FLDID_SAKKO		34
#define FLDID_RATA		35
#define FLDID_ERA		36
#define FLDID_SELITYS	37
#define FLDID_PVPISTE1	38
#define FLDID_PVPISTE2	39
#define FLDID_ENNATYS	40
#define FLDID_TAVOITE	41
#define FLDID_FLAGS 	42
#define FLDID_VATULOS	43
#define FLDID_VASIJA	44
#define FLDID_NIMI		100


#define N_LL_LAJI    7

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
#define WM_MYVAIHDAVAIHE	(WM_APP + 2213)
#define WM_MYPAIVITA		(WM_APP + 2214)
#define WM_MYHAEAVOIMET		(WM_APP + 2215)
#define WM_MYTIEDOT 		(WM_APP + 2216)
#define WM_MYMSGDSPWARNMSG	(WM_APP + 2217)

#define HT_GRIDCOLS 17

struct TMyMessage {
	Cardinal Msg;
	int wparam;
	int lparam;
	LRESULT Result;
	};

struct perr_ptrs_tp {
	wchar_t *ots;
	wchar_t *msg;
	int vast;
	};

typedef struct {
	wchar_t name[20];
	int id;
	INT akt;
	UINT len;
	INT oik;
	} FLD2;

extern FLD2 exp_kentat[];

extern int Kexp_kentat, Pexp_kentat, Vexp_kentat, Xexp_kentat;

extern struct perr_ptrs_tp perr_ptrs;

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
//---------------------------------------------------------------------------
typedef struct {
	wchar_t ref[22];
	wchar_t linkki[162];
	} tietolinkkitp;

extern tietolinkkitp *HenkLinkki;

typedef struct {
	int Kilpno;
   UnicodeString  Nimi;
   UnicodeString  Seura;
   UnicodeString  Maa;
   int Sarja;
   int Piste;
   int Sija;
   int ySija;
   INT32 Tulos;
   INT32 yTulos;
   int pisteet;
   UnicodeString  Tark;
   } tapTp;

extern int viimTap;
extern tapTp Tapahtuma[MAXTAPAHTUMA];
extern bool TapEtunimiensin;

extern tagListTp seurIkkTags[];

class seurIkkParamClass {
public:
   int Sarja;
   int Va;
   int EraVal;
   bool naytaAlisarja;
   bool naytaMaa;
   bool naytaHyl;
   bool seuraaLahtoja;
   int minTaso;
   bool naytaPiilotetut;
   bool naytaOdotetut;
   bool rajaus;
   bool EtuSuku;
   int seuraTieto;
   bool kokotulos;
   bool eroaika;
   int kierrosaika;
   bool kaikkiajat;
   bool vainjarj;
   bool smartJrj;
   bool lahtoaika;
   bool yhttulos;
   int Ennatys;
   int Tavoite;
   bool viimeisin;
   bool laskeYhtpisteet;
   int AaniLkm;
   int FontSize;
   bool Lihavoitu;
   int ColW[20];
   int Colors[10];
   int AktColor;
   int laika;
   int korRaja[2];
   int ScrollVali;
   int ScrollRiviKesto;
   bool ScrollOn;
   int ScrollType;
   bool KokoRuutu;
   int ScrollSarjat[MAXSARJALUKU+MAXYHD+1];
   int odRaja;
   int k_pv;
   int Era;
   seurIkkParamClass(void);
   void operator=(seurIkkParamClass&);
   bool operator==(seurIkkParamClass&);
   bool operator!=(seurIkkParamClass&);
   int writeParams(TextFl *outfl, int level);
   int readParams(xml_node *node, int *inode, int nnode);
};

extern seurIkkParamClass seurIkkParam[];
extern seurIkkParamClass seurIkkParam0;

class tapIkkParamClass {
public:
   int Va;
   bool naytaMaa;
   bool KokoRuutu;
   bool EtuSuku;
   int seuraTieto;
   bool yhttulos;
   int Ennatys;
   int Tavoite;
   int FontSize;
   bool Lihavoitu;
   int ColW[16];
   int laika;
   tapIkkParamClass(void);
   void operator=(tapIkkParamClass&);
   int writeParams(TextFl *outfl, int level);
   int readParams(xml_node *node, int *inode, int nnode);
};

extern tapIkkParamClass tapIkkParam[];
extern tapIkkParamClass tapIkkParam0;

class ilmIkkParamClass {
public:
	bool IdOn;
	bool KilpnoOn;
	bool LisnoOn;
	bool SNimiOn;
	bool ENimiOn;
	bool SarjaOn;
	bool SukupOn;
	bool IkasrjOn;
	bool WRKoodiOn;
	bool ArvoOn;
	bool SeuraOn;
	bool SeuralyhOn;
	bool MaaOn;
	bool YhdistysOn;
	bool JoukkueOn;
	bool LasnaOn;
	bool BibOn;
	bool BadgeOn;
	bool LAOn;
	bool TasOn;
	bool RataOn;
	bool PiiriOn;
	bool KnohakuOn;
	bool LisnohakuOn;
	bool NimihakuOn;
	bool NimiHThakuOn;
	bool HTSarjaOn;
	bool SeurahakuOn;
	bool SeuralisaOn;
	bool MerkAvoimeksiOn;
	bool HTBadgeOn;
	bool AutoLaikaOn;
	bool JIilmOn;
	bool AutoKnoOn;
	bool LisnoTarkOn;
	int  LisaysIx;
	int  JarjestysIx;
	int  PaivatIx;
	int  OId;
	int  OKilpno;
	int  OLisno;
	int  OSNimi;
	int  OENimi;
	int  OSarja;
	int  OWRKoodi;
	int  OArvo;
	int  OSeura;
	int  OSeuralyh;
	int  OMaa;
	int  OYhdistys;
	int  OJoukkue;
	int  OLasna;
	int  OBib;
	int  OBadge;
	int  OLA;
	int  OTas;
	int  ORata;
	int  OPiiri;
	int  OSukup;
	int  OIkasrj;
	int  EdtVakBibLisa;
	int IlmKentat;
   ilmIkkParamClass(void);
   void operator=(ilmIkkParamClass&);
   int writeParams(TextFl *outfl, int level);
   int readParams(xml_node *node, int *inode, int nnode);
};

extern ilmIkkParamClass ilmIkkParam;
extern ilmIkkParamClass ilmIkkParam0;

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

extern aikavertIkkParamClass aikavertIkkParam0;

class statusIkkParamClass {
public:
	bool PalkJako;
	int ParasSija;
	int yhteysikkuna;
	statusIkkParamClass(void);
	void operator=(statusIkkParamClass&);
	int writeParams(TextFl *outfl, int level);
	int readParams(xml_node *node, int *inode, int nnode);
};

extern statusIkkParamClass statusIkkParam;
extern statusIkkParamClass statusIkkParam0;

class emitIkkParamClass {
public:
	bool NaytaSeura;
	bool NaytaKortinKoodi;
	bool HenkTietokanta;
	bool Seuraluettelo;
	bool VahvIlmAina;
	int  KuntoKone;
	bool MerkLainat;
	bool TietokantaSarjat;
	bool VaadiToisto;
	bool LueVialliset;
	bool EmitvaihtoVaiheille;
	emitIkkParamClass(void);
	void operator=(emitIkkParamClass&);
	int writeParams(TextFl *outfl, int level);
	int readParams(xml_node *node, int *inode, int nnode);
};

extern emitIkkParamClass emitIkkParam;
extern emitIkkParamClass emitIkkParam0;

class NaytaTulosIkkParamClass {
public:
	int NaytaTulos;
	int OkKesto;
	int OngKesto;
	int LahtoRaja;
	wchar_t Teksti[15][30];
	NaytaTulosIkkParamClass(void);
	void operator=(NaytaTulosIkkParamClass&);
	int writeParams(TextFl *outfl, int level);
	int readParams(xml_node *node, int *inode, int nnode);
};

#ifdef EI_OLE

class PACKAGE TPPEdit : public TEdit
{
protected:
	virtual void __fastcall KeyDown(WORD &Key, TShiftState Shift);

public:
	__fastcall TPPEdit(TComponent *Owner);
};
//---------------------------------------------------------------------------
#endif // EI_OLE

#endif  // !HKWINDEF_DEFINED

