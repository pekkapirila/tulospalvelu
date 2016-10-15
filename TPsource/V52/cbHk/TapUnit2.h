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
#include <Vcl.Menus.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "HkWinDef.h"
#include "UnitKilpailijaOnline.h"
//---------------------------------------------------------------------------
class TTapForm : public TForm
{
__published:	// IDE-managed Components
   TDrawGrid *TapGrid;
	TMainMenu *MainMenu1;
	TMenuItem *Valinnat1;
	TMenuItem *Pieni1;
	TMenuItem *Keski1;
	TMenuItem *Suuri1;
	TMenuItem *Valinnat2;
	TMenuItem *Nytyhteistulokset1;
	TMenuItem *Nytseuralyhenne1;
	TMenuItem *Etunimiensin1;
	TMenuItem *iedosto1;
	TMenuItem *Sulje1;
	TMenuItem *N1;
	TMenuItem *Aikojennytttarkkuus1;
	TMenuItem *N1sek;
	TMenuItem *N01sek;
	TMenuItem *N001sek;
	TMenuItem *N0001sek;
	TMenuItem *Muumerkkikoko1;
	TMenuItem *N2;
	TMenuItem *allennasarakeleveydet1;
	TMenuItem *Nytmaa1;
	TMenuItem *Piilotavalintatiedot1;
	TPanel *Panel1;
	TLabel *Label1;
	TComboBox *PisteVal;
	TButton *Button1;
	TLabel *LblEra;
	TEdit *EdtEra;
	TEdit *EdtKello;
	TBitBtn *BitBtn1;
	TMenuItem *Nytomaenntys1;
	TMenuItem *Nyttavoite1;
   void __fastcall Button1Click(TObject *Sender);
   void __fastcall PisteValChange(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall TapGridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall TapGridSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall TapGridDblClick(TObject *Sender);
	void __fastcall Pieni1Click(TObject *Sender);
	void __fastcall Keski1Click(TObject *Sender);
	void __fastcall Suuri1Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall Nytyhteistulokset1Click(TObject *Sender);
	void __fastcall Nytseuralyhenne1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Etunimiensin1Click(TObject *Sender);
	void __fastcall Sulje1Click(TObject *Sender);
	void __fastcall N1sekClick(TObject *Sender);
	void __fastcall N01sekClick(TObject *Sender);
	void __fastcall N001sekClick(TObject *Sender);
	void __fastcall N0001sekClick(TObject *Sender);
	void __fastcall Muumerkkikoko1Click(TObject *Sender);
	void __fastcall allennasarakeleveydet1Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Nytmaa1Click(TObject *Sender);
	void __fastcall Piilotavalintatiedot1Click(TObject *Sender);
	void __fastcall Nyttavoite1Click(TObject *Sender);
	void __fastcall Nytomaenntys1Click(TObject *Sender);

private:	// User declarations
	void __fastcall FormSetup(TObject *Sender);
   int Valittu;
	int CurrentRow;
	int CurrentCol;
	int lev;
   int colCount1;
   int colCount2;
public:		// User declarations
	TFormKilpailijaOnline *FormKilpailijaOnlineTap;
	void __fastcall Skaalaa(void);
   __fastcall TTapForm(TComponent* Owner);
   void __fastcall Paivita(TObject *Sender);
	int applyParams(void);
   tapIkkParamClass IkkParam;
	cellTp Cells[22][MAXTAPAHTUMA+1];
   wchar_t Kohde[30];
};
//---------------------------------------------------------------------------
extern PACKAGE TTapForm *TapForm;
//---------------------------------------------------------------------------
#endif
