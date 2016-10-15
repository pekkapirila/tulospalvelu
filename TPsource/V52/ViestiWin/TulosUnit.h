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

#ifndef TulosUnitH
#define TulosUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Vcl.Menus.hpp>
#include "VDef.h"
#include "UnitJoukkuetiedot.h"
//---------------------------------------------------------------------------

#define MAXSEURCOL 11

class TScrollGrid : public TCustomGrid
{
public:
	void __fastcall SInvalidateRow(int ARow);
};
//---------------------------------------------------------------------------
class TTilanneForm : public TForm
{
__published:	// IDE-managed Components
   TStringGrid *TulosGrid;
	TMainMenu *MainMenu1;
	TMenuItem *Nyt1;
	TMenuItem *Maatunnus1;
	TMenuItem *N1;
	TMenuItem *Vainjrjestyspiste1;
	TMenuItem *Pisteet1;
	TMenuItem *Kaikkiajat1;
	TMenuItem *Maali1;
	TMenuItem *N1vliaika1;
	TMenuItem *N2vliaika1;
	TMenuItem *N3vliaika1;
	TMenuItem *N4vliaika1;
	TMenuItem *N5vliaika1;
	TMenuItem *N6vliaika1;
	TMenuItem *N7vliaika1;
	TMenuItem *N8vliaka1;
	TMenuItem *N9vliaika1;
	TMenuItem *N10vliaikaA1;
	TMenuItem *N11vliaikaB1;
	TMenuItem *N12vliaikaC1;
	TMenuItem *N13vliaikaD1;
	TMenuItem *N14vliaikaE1;
	TMenuItem *N15vliaikaF1;
	TMenuItem *N16vliaikaG1;
	TMenuItem *N17vliaikaH1;
	TMenuItem *N18vliaikaI1;
	TMenuItem *N19vliaikaJ1;
	TMenuItem *N20vliaikaK1;
	TMenuItem *N21vliaikaL1;
	TMenuItem *N22vliaikaM1;
	TMenuItem *N23vliaikaN1;
	TMenuItem *N24vliaikaO1;
	TMenuItem *N25vliaikaP1;
	TMenuItem *N26vliaikaQ1;
	TMenuItem *N27vliaikaR1;
	TMenuItem *N28vliaikaS1;
	TMenuItem *N29vliaikaT1;
	TMenuItem *N30vliaikaU1;
	TMenuItem *N3;
	TMenuItem *Erokrkeen1;
	TMenuItem *Kirjasin1;
	TMenuItem *Suuri1;
	TMenuItem *Normaali1;
	TMenuItem *Pieni1;
	TMenuItem *N2;
	TMenuItem *Lihavoitu1;
	TMenuItem *oiminto1;
	TMenuItem *Kokoruutu1;
	TMenuItem *Skrollaa1;
	TMenuItem *Lopetaskrollaus1;
	TMenuItem *Pehmeskrollaus1;
	TMenuItem *Liukuva1;
	TMenuItem *Rivittinaskeltava1;
	TMenuItem *Ruutukerrallaan1;
	TMenuItem *aukosarjanptteeksi1;
	TMenuItem *Sarjanminimikesto1;
	TMenuItem *Skrollauksensarjalista1;
	TPanel *Panel1;
	TButton *PaivBtn;
	TComboBox *SarjaVal;
	TLabel *Label1;
	TComboBox *OsVal;
	TEdit *EditAlusta;
	TComboBox *PisteVal;
	TRadioGroup *JarjVal;
	TCheckBox *HylVal;
	TCheckBox *EroVal;
	TCheckBox *Viimeisimmat;
	TCheckBox *CBkaikki;
	TCheckBox *CB1puute;
	TCheckBox *CB2puute;
	TMenuItem *Etunimiensin1;
	TMenuItem *N4;
	TMenuItem *Suljekaavake1;
	TMenuItem *Muumerkkikoko1;
	TMenuItem *N5;
	TMenuItem *allennasarakeleveydet1;
   void __fastcall PaivBtnClick(TObject *Sender);
   void __fastcall OsValChange(TObject *Sender);
   void __fastcall PisteValChange(TObject *Sender);
   void __fastcall JarjValClick(TObject *Sender);
   void __fastcall EroValClick(TObject *Sender);
   void __fastcall ViimeisimmatClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall FormActivate(TObject *Sender);
   void __fastcall SarjaValChange(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
	void __fastcall TulosGridDblClick(TObject *Sender);
	void __fastcall HylValClick(TObject *Sender);
	void __fastcall Suuri1Click(TObject *Sender);
	void __fastcall Normaali1Click(TObject *Sender);
	void __fastcall Pieni1Click(TObject *Sender);
	void __fastcall Lihavoitu1Click(TObject *Sender);
	void __fastcall Kokoruutu1Click(TObject *Sender);
	void __fastcall Skrollaa1Click(TObject *Sender);
	void __fastcall Lopetaskrollaus1Click(TObject *Sender);
	void __fastcall Liukuva1Click(TObject *Sender);
	void __fastcall Rivittinaskeltava1Click(TObject *Sender);
	void __fastcall Ruutukerrallaan1Click(TObject *Sender);
	void __fastcall aukosarjanptteeksi1Click(TObject *Sender);
	void __fastcall Sarjanminimikesto1Click(TObject *Sender);
	void __fastcall Skrollauksensarjalista1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Etunimiensin1Click(TObject *Sender);
	void __fastcall Maatunnus1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Suljekaavake1Click(TObject *Sender);
	void __fastcall Muumerkkikoko1Click(TObject *Sender);
	void __fastcall allennasarakeleveydet1Click(TObject *Sender);
private:	// User declarations
	MESSAGE void __fastcall ScrollMsgHandler(TMyMessage &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYSCROLL, TMyMessage, ScrollMsgHandler);
END_MESSAGE_MAP(TComponent)

	void __fastcall PaivitaGrid(bool scroll);
	void __fastcall FormSetup(TObject *Sender);
	int Ero;
	int nCol;
	int CurrentRow;
	int CurrentCol;
	int CNro;
	int CJoukkue;
	int CMaa;
	int CNimi;
	int CTulos;
	int CEro;
	int lev;
	int ColLj[MAXSEURCOL];
   int ScrollSrjKesto;
   int sc;
   int alkuhetki, loppuhetki;
   int kScrollSarja;
public:		// User declarations
   __fastcall TTilanneForm(TComponent* Owner);
   __fastcall ~TTilanneForm(void);
   int applyParams(void);
   seurIkkParamClass IkkParam;
   TFormJoukkuetiedot *FormJoukktiedot;
   CRITICAL_SECTION tls_CriticalSection;
   bool Aktiivinen;
   int ScrollViive;
   int paivitysflag;
   int Yhd;
   int TilanneFormNo;
   wchar_t Kohde[32];
};
//---------------------------------------------------------------------------
extern PACKAGE TTilanneForm *TilanneForm;
//---------------------------------------------------------------------------
#endif
