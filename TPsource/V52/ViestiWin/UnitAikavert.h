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

#ifndef UnitAikavertH
#define UnitAikavertH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include "VWinDef.h"
#include <Vcl.ComCtrls.hpp>

#define NAJANOTOT 4
#define MAXAVCOL 11
#define MAXAVROW 50
//---------------------------------------------------------------------------
class TFormAikavert : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *Toiminnot1;
	TMenuItem *Valinnat1;
	TMenuItem *Sulje1;
	TMenuItem *Kirjasinkoko1;
	TBitBtn *BitBtn1;
	TEdit *EdHaku;
	TPanel *Panel1;
	TDrawGrid *DGajat1;
	TLabel *Label9;
	TComboBox *CBJono1;
	TButton *BtnLuoUusi1;
	TPanel *Panel2;
	TDrawGrid *DGajat2;
	TLabel *Label1;
	TComboBox *CBJono2;
	TButton *BtnLuoUusi2;
	TButton *BtnPoista2;
	TButton *BtnPoista1;
	TUpDown *UpDownBias;
	TEdit *EdtBias;
	TLabel *Label2;
	TScrollBar *ScrollBar1;
	TMenuItem *Help1;
	TMenuItem *Pivit1;
	TMenuItem *Vasentaulukko1;
	TMenuItem *Oikeataulukko1;
	TMenuItem *Nytnimi1;
	TMenuItem *Nytsarja1;
	TMenuItem *Nyttulos1;
	TMenuItem *Nytnimi2;
	TMenuItem *Nytsarja2;
	TMenuItem *Nyttulos2;
	TMenuItem *Nyttuloksenero1;
	TMenuItem *Nyttuloksenero2;
	TMenuItem *Jonojenaikaero1;
	TMenuItem *Nyttark1;
	TMenuItem *Nyttark2;
	TLabel *Label3;
	TEdit *EdtAlku;
	TUpDown *UpDownAlku;
	TMenuItem *Numerovertailu1;
	TMenuItem *Aikavertailu1;
	TMenuItem *Aikaero1;
	TLabel *Label4;
	TLabel *LblLisaA;
	TCheckBox *CBViimeiset;
	TLabel *Label6;
	TLabel *LblLisaB;
	TMenuItem *Nytmysohitetutajat1;
	TMenuItem *Nytmysohitetutajat2;
	TCheckBox *CBViimeiset2;
	void __fastcall BtnPaivitaClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall CBJono1Change(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Sulje1Click(TObject *Sender);
	void __fastcall BtnHakuClick(TObject *Sender);
	void __fastcall Kirjasinkoko1Click(TObject *Sender);
	void __fastcall DGajat1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall DGajat2DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall DGajat1MouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall DGajat1SelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall CBJono2Change(TObject *Sender);
	void __fastcall Nytnimi1Click(TObject *Sender);
	void __fastcall Nytsarja1Click(TObject *Sender);
	void __fastcall Nyttulos1Click(TObject *Sender);
	void __fastcall Nyttuloksenero1Click(TObject *Sender);
	void __fastcall Nytnimi2Click(TObject *Sender);
	void __fastcall Nytsarja2Click(TObject *Sender);
	void __fastcall Nyttulos2Click(TObject *Sender);
	void __fastcall Nyttuloksenero2Click(TObject *Sender);
	void __fastcall Jonojenaikaero1Click(TObject *Sender);
	void __fastcall Help1Click(TObject *Sender);
	void __fastcall Nyttark1Click(TObject *Sender);
	void __fastcall Nyttark2Click(TObject *Sender);
	void __fastcall EdtBiasChange(TObject *Sender);
	void __fastcall UpDownBiasClick(TObject *Sender, TUDBtnType Button);
	void __fastcall DGajat1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall UpDownAlkuClick(TObject *Sender, TUDBtnType Button);
	void __fastcall EdtAlkuChange(TObject *Sender);
	void __fastcall DGajat2KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Numerovertailu1Click(TObject *Sender);
	void __fastcall Aikavertailu1Click(TObject *Sender);
	void __fastcall BtnPoista1Click(TObject *Sender);
	void __fastcall BtnPoista2Click(TObject *Sender);
	void __fastcall BtnLuoUusi2Click(TObject *Sender);
	void __fastcall BtnLuoUusi1Click(TObject *Sender);
	void __fastcall DGajat1MouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall ScrollBar1Change(TObject *Sender);
	void __fastcall Aikaero1Click(TObject *Sender);
	void __fastcall CBViimeisetClick(TObject *Sender);
	void __fastcall Nytmysohitetutajat1Click(TObject *Sender);
	void __fastcall Nytmysohitetutajat2Click(TObject *Sender);




private:	// User declarations
	void __fastcall GridDrawCell(int grid, TObject *Sender, int ACol,
		int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall Aktivoi(int grid, int rivi, int status);
	void __fastcall LuoUusi(int grid);
	void __fastcall SarakeLeveydet(void);
	TDrawGrid *DGajat[2];
	TPanel *Panel[2];
	cellTp Cells[2][MAXAVCOL][MAXAVROW];
	ColTp VertGridCols[2][MAXAVCOL];
	int aRow[2][MAXAVROW];
	int AikaEro;
	bool Scrolling;
public:		// User declarations
	__fastcall TFormAikavert(TComponent* Owner);
	void __fastcall naytaRivi(int grid, int r, aikatp &pt, aikatp &pt1);
	void __fastcall naytaAjat(void);
	void Paivita_Aikanaytto(void);
	int applyParams(void);
	aikavertIkkParamClass IkkParam;
	int YRivi1;
	int aktRivi1;
	int Alku1;
	int Bias;
	int NRivi1;
	int Ajanotto;
	bool OnLine;
	int Jono[2];
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAikavert *FormAikavertailut[NAJANOTOT];
//---------------------------------------------------------------------------
#endif
