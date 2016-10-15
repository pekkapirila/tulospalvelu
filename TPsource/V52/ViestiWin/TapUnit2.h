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

#ifndef TapUnit2H
#define TapUnit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>

#include "valinnat.h"
#include "VWinDef.h"
#include "UnitJoukkuetiedot.h"
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------

class tapTp {
public:
   int Kilpno;
   UnicodeString  Joukkue;
   UnicodeString  Nimi;
   UnicodeString  Maa;
   int Sarja;
   int Osuus;
   int Piste;
   int Sija;
   INT32 Tulos;
   UnicodeString  Tark;
   tapTp(void);
   void operator=(tapTp&);
   bool operator==(tapTp&);
   };

//typedef struct {
//	AnsiString  text;
//	int         style;
//	} cellTp;

class TTapForm : public TForm
{
__published:	// IDE-managed Components
   TDrawGrid *TapGrid;
   TButton *Button1;
   TLabel *Label1;
   TComboBox *PisteVal;
	TCheckBox *CBTarkSan;
	TMainMenu *MainMenu1;
	TMenuItem *Valinnat1;
	TMenuItem *Pieni1;
	TMenuItem *Keski1;
	TMenuItem *Suuri1;
	TMenuItem *Valinnat2;
	TMenuItem *Etunimiensin1;
	TMenuItem *N1;
	TMenuItem *Suljekaavake1;
	TMenuItem *Muumerkkikoko1;
	TMenuItem *N2;
	TMenuItem *Lihavoitu1;
	TMenuItem *N3;
	TMenuItem *allennasarakeleveydet1;
   void __fastcall Button1Click(TObject *Sender);
   void __fastcall PisteValChange(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall TapGridDrawCell(TObject *Sender, int ACol, int ARow,
		  TRect &Rect, TGridDrawState State);
	void __fastcall CBTarkSanClick(TObject *Sender);
	void __fastcall Pieni1Click(TObject *Sender);
	void __fastcall Keski1Click(TObject *Sender);
	void __fastcall Suuri1Click(TObject *Sender);
	void __fastcall Etunimiensin1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall TapGridDblClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Suljekaavake1Click(TObject *Sender);
	void __fastcall Muumerkkikoko1Click(TObject *Sender);
	void __fastcall Lihavoitu1Click(TObject *Sender);
	void __fastcall allennasarakeleveydet1Click(TObject *Sender);
private:	// User declarations
	void __fastcall Skaalaa(void);
	int CurrentRow;
	int CurrentCol;
	int lev;
public:		// User declarations
	__fastcall TTapForm(TComponent* Owner);
	void __fastcall Paivita(TObject *Sender);
	int applyParams(void);
	tapIkkParamClass IkkParam;
	TFormJoukkuetiedot *FormJoukktiedot;
	cellTp Cells[20][MAXTAPAHTUMA+1];
   wchar_t Kohde[30];
	int Valittu;
};
//---------------------------------------------------------------------------
extern PACKAGE TTapForm *TapForm;
//---------------------------------------------------------------------------
#endif
