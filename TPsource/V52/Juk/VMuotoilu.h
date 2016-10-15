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

#ifndef VMuotoiluH
#define VMuotoiluH

#include "VDeclare.h"
//---------------------------------------------------------------------------

#define PRKOODIPIT 50

#define EPSON  1
#define LJETII 2
#define POSTSCRIPT 3
#define PROPRIEMU 4
#define TXTTAB 5
#define CANON  6
#define GDIPRINTER 7

#define F_SJ 	0
#define F_EDSJ 	1
#define F_KNO 	2
#define F_JOUK 	3
#define F_MAA 	4
#define F_JOUKNM 5
#define F_JOUKID 6
#define F_NIMI 	7
#define F_ARVO 	8
#define F_LISNO 9
#define F_BADGE 10
#define F_RATA  11
#define F_OSSRA 12
#define F_TLS 	13
#define F_ERO 	14
#define F_SAK 	15
#define F_TARK 	16
#define F_PIST 	17
#define F_OSASJ 18
#define F_OSATLS 19
#define F_SEURSJ 20
#define F_SRA 21
#define F_SUKUNIMI 22
#define F_ETUNIMI  23
#define F_VSJ  24
#define F_OSOTS  25

#define MAXFLD  30

int LueXMLMuotoilu(wchar_t *MuotFile);
void TallXMLMuotoilu(wchar_t *MuotFile);
void initHtmlHeader(bool pakota);

typedef struct {
	int id;
	wchar_t name[20];
	wchar_t ots[20];
	int pos;
	int len;
	int oik;
	int akt;
	} FldFrmtTp;

//---------------------------------------------------------------------------

extern FldFrmtTp dspflds[];
extern FldFrmtTp fileflds[];
extern FldFrmtTp fileflds_suunn[];
extern FldFrmtTp prtflds_suunn[];
extern FldFrmtTp prtflds[];
extern FldFrmtTp prtfldsk[];
extern FldFrmtTp filefldsk[];

extern GDIfontTp CurrFnt;
extern GDIfontTp DefFnt;
extern GDIfontTp TitleFnt;
extern GDIfontTp HeaderFnt;
extern GDIfontTp BoldFnt;
extern GDIfontTp SmallFnt;
extern GDIfontTp CourFnt;
extern GDIfontTp RastiVaHeaderFnt;
extern GDIfontTp RastiVaFnt;

extern tulosmuottp tulosmuot;
extern tulosmuottp filetulosmuot;

extern int n_prtflds;
extern int n_prtfldsk;
extern int n_fileflds;
extern int n_filefldsk;
extern int n_dspflds;
extern FldFrmtTp *frmflds[];
extern int n_frmflds[];

extern wchar_t init_fnt[PRKOODIPIT+1];
extern wchar_t pots_on[PRKOODIPIT+1];
extern wchar_t pots_pois[PRKOODIPIT+1];
extern wchar_t aots_on[PRKOODIPIT+1];
extern wchar_t aots_pois[PRKOODIPIT+1];
extern wchar_t lih_on[PRKOODIPIT+1];
extern wchar_t lih_pois[PRKOODIPIT+1];
extern wchar_t pienet_on[PRKOODIPIT+1];
extern wchar_t pienet_pois[PRKOODIPIT+1];
extern wchar_t paaots[125];
extern wchar_t sarjaots[125];
extern wchar_t alaots[141];
extern wchar_t initfont_emrap[];
extern wchar_t initfont_emva[];

extern INT valinelendef, vapagelendef, vapageskip, tarkrappageskip;
extern INT valinelen, vapagelen;

#endif
