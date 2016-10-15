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

//---------------------------------------------------------------------------

#ifndef HkMuotoiluH
#define HkMuotoiluH

#include "HkDeclare.h"
//---------------------------------------------------------------------------

#define PRKOODIPIT 50

#define EPSON  1
#define LJETII 2
#define POSTSCRIPT 3
#define PROPRIEMU 4
#define TXTTAB 5
#define CANON  6
#define GDIPRINTER 7

#define F_SJ 		0
#define F_KNO 		1
#define F_NIMI 		2
#define F_ETUNIMI 	3
#define F_SUKUNIMI 	4
#define F_ARVO 		5
#define F_LISNO 	6
#define F_LISNO2 	7
#define F_BADGE 	8
#define F_SRA 		9
#define F_SRALYH 	10
#define F_MAA 		11
#define F_YHD 		12
#define F_JOUK 		13
#define F_ALISRJ 	14
#define F_TLS 		15
#define F_ERO 		16
#define F_SAK 		17
#define F_TARK 		18
#define F_AIKA 		19
#define F_PIST 		20
#define F_OSASJ 	21
#define F_OSATLS 	22
//#define F_SEURSJ 	23

//#define F_LAJI 	 16
//#define F_YPIST  17

#define N_LL_LAJI    7

#define LLF_KNO  0
#define LLF_SARJA  1
#define LLF_ALISARJA  2
#define LLF_NIMI 3
#define LLF_AIKA 4
#define LLF_SEURA 5
#define LLF_MAA  (LLF_SEURA+1)
#define LLF_LISNO  (LLF_MAA+1)
#define LLF_LISNO2   (LLF_LISNO+1)
#define LLF_EMIT   (LLF_LISNO2+1)
#define LLF_EMIT2   (LLF_EMIT+1)
#define LLF_MAKSU  (LLF_EMIT2+1)
#define LLF_RANKI  (LLF_MAKSU+1)
#define LLF_PISTE2 (LLF_RANKI+1)
#define LLF_LISTA  (LLF_PISTE2+1)
#define LLF_AIKA2  (LLF_LISTA+1)
#define  NFLISA  N_PV
#define  NLLFIELDS LLF_AIKA2

int LueXMLMuotoilu(wchar_t *MuotFile);
void TallXMLMuotoilu(wchar_t *MuotFile);
wchar_t *css_string(tulostusparamtp *tulprm, int laji);

typedef struct {
	int id;
	wchar_t name[20];
	wchar_t ots[20];
	int pos;
	int len;
	int oik;
	int askel;
	int akt;
	} FldFrmtTp;

typedef struct {
   wchar_t *nimi;
   wchar_t ots[20];
   int  pos[N_LL_LAJI];
   int  len[N_LL_LAJI];
   int  tas[N_LL_LAJI];
   } llfldtp;
//---------------------------------------------------------------------------
typedef struct {
	wchar_t  luetots0[123];
	wchar_t  luetots1[123];
	wchar_t  luetots21[61];
	wchar_t  luetots22[93];
	wchar_t  luetots3[123];
	int tabs[2];
	int vaihdanimet[2][N_LL_LAJI];
	int pvfl[2][N_LL_LAJI];
	int klfl[2][N_LL_LAJI];
	int matkafl[2][N_LL_LAJI];
	int tarknofl[2][N_LL_LAJI];
	int aikaporras[2][N_LL_LAJI-4];
	int marg[2][N_LL_LAJI];
	} llparamtp;

//---------------------------------------------------------------------------

extern FldFrmtTp dspflds[];
extern FldFrmtTp fileflds[];
extern FldFrmtTp fileflds_suunn[];
extern FldFrmtTp mobilflds[];
extern FldFrmtTp prtflds_suunn[];
extern FldFrmtTp prtflds[];
extern FldFrmtTp prtfldsm[];
extern FldFrmtTp prtfldsm_suunn[];
extern FldFrmtTp prtflds1[];
extern FldFrmtTp fileflds_meri5[];
extern FldFrmtTp prtflds_meri5[];
extern FldFrmtTp prtfldsm_meri5[];
extern FldFrmtTp prtflds1_meri5[];

extern llfldtp tulkentta1[15+N_PV];
extern llfldtp tulkentta2[15+N_PV];
extern llfldtp tulkentta_suunn_1[15+N_PV];
extern llfldtp tulkentta_suunn_2[15+N_PV];
extern llfldtp *tulkentta[2];
extern llparamtp llparam;

extern GDIfontTp CurrFnt;
extern GDIfontTp DefFnt;
extern GDIfontTp TitleFnt;
extern GDIfontTp HeaderFnt;
extern GDIfontTp CourFnt;
extern GDIfontTp RastiVaHeaderFnt;
extern GDIfontTp RastiVaFnt;

extern tulosmuottp tulosmuot;
extern tulosmuottp filetulosmuot;
extern tulosmuottp mobiltulosmuot;

extern int n_prtflds;
extern int n_prtfldsm;
extern int n_prtflds1;
extern int n_fileflds;
extern int n_mobilflds;
extern int n_dspflds;
extern FldFrmtTp *frmflds[];
extern int n_frmflds[];

extern wchar_t init_fnt[PRKOODIPIT+1];
extern wchar_t pots_on[PRKOODIPIT+1];
extern wchar_t pots_pois[PRKOODIPIT+1];
extern wchar_t aots_on[PRKOODIPIT+1];
extern wchar_t aots_pois[PRKOODIPIT+1];
extern wchar_t paaots[125];
extern wchar_t sarjaots[125];
extern wchar_t alaots[141];
extern   wchar_t initfont_emrap[];
extern   wchar_t initfont_emva[];

extern INT valinelendef, vapagelendef, vapageskip, tarkrappageskip;
extern INT valinelen, vapagelen;

#endif
