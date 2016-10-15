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

#ifndef UnitSarjaH
#define UnitSarjaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "VDef.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TFormSarja : public TForm
{
__published:	// IDE-managed Components
	TEdit *EdtSarja;
	TEdit *PNimi;
	TEdit *EOsLkm;
	TEdit *Alku;
	TEdit *La1;
	TEdit *SNimi;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label6;
	TLabel *Label8;
	TLabel *Label10;
	TLabel *Label16;
	TLabel *Label17;
	TEdit *Mks;
	TLabel *Label19;
	TEdit *MksAl;
	TComboBox *CBSak;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TLabel *Label27;
	TEdit *L1;
	TLabel *Label28;
	TEdit *M1;
	TButton *Button5;
	TBitBtn *BitBtn1;
	TButton *BtnSeurSrj;
	TButton *BtnEdSrj;
	TStringGrid *SG1;
	TLabel *Label5;
	TEdit *ERinnMax;
	TMainMenu *MainMenu1;
	TMenuItem *oiminnot1;
	TMenuItem *Kopioi1osuudenmatkakaikilleosuuksille1;
	TMenuItem *Kopioi1osuudenvliajatkaikilleosuuksille1;
	TEdit *EdtKnoSiirto;
	TLabel *Label7;
	TLabel *Label9;
	TComboBox *CBAmpSak;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BtnEdSrjClick(TObject *Sender);
	void __fastcall BtnSeurSrjClick(TObject *Sender);
	void __fastcall MksExit(TObject *Sender);
	void __fastcall MksAlExit(TObject *Sender);
	void __fastcall La1Exit(TObject *Sender);
	void __fastcall SNimiExit(TObject *Sender);
	void __fastcall PNimiExit(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall SG1Exit(TObject *Sender);
	void __fastcall SG1SelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall CBSakClick(TObject *Sender);
	void __fastcall ERinnMaxExit(TObject *Sender);
	void __fastcall EOsLkmExit(TObject *Sender);
	void __fastcall Kopioi1osuudenmatkakaikilleosuuksille1Click(TObject *Sender);
	void __fastcall Kopioi1osuudenvliajatkaikilleosuuksille1Click(TObject *Sender);


private:	// User declarations
	int aktrow;
	int aktcol;
	int  __fastcall paivitaMuutos(int col, int row);
public:		// User declarations
	__fastcall TFormSarja(TComponent* Owner);
	void __fastcall naytaSarja(bool uusi);
	void __fastcall lataaSarja(int Srj);
	void __fastcall tallSarja(void);
	void __fastcall naytaOsuudet(void);
	void __fastcall tallOsuudet(void);
	sarjatietue Sarja1;
	int SarjaNo;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSarja *FormSarja;
//---------------------------------------------------------------------------
#endif
