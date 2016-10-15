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

#ifndef UnitTietoTulkH
#define UnitTietoTulkH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <tputil.h>
//---------------------------------------------------------------------------
class TFormTietoTulk : public TForm
{
__published:	// IDE-managed Components
	TMemo *Memo1;
	TStringGrid *SG1;
	TMainMenu *MainMenu1;
	TMenuItem *iedosto1;
	TMenuItem *Luetiedottiedostosta1;
	TMenuItem *yhjenntekstialue1;
	TMenuItem *Liittiedotleikepydlt1;
	TPanel *Panel1;
	TComboBox *CBErotin;
	TEdit *EdtCols;
	TCheckBox *CBOtsRivi;
	TButton *Button1;
	TLabel *LblData;
	TEdit *EdtData;
	TLabel *Label3;
	TLabel *Label1;
	TLabel *LblTunn;
	TEdit *EdtTunn;
	TMenuItem *Sulje1;
	TOpenDialog *OpenDialog1;
	TMenuItem *Help1;
	TCheckBox *CBLainMerk;
	void __fastcall Luetiedottiedostosta1Click(TObject *Sender);
	void __fastcall yhjenntekstialue1Click(TObject *Sender);
	void __fastcall Liittiedotleikepydlt1Click(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Sulje1Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall EdtTunnExit(TObject *Sender);
	void __fastcall EdtDataExit(TObject *Sender);
private:	// User declarations
	int tunnCol;
	int dataCol;
public:		// User declarations
	__fastcall TFormTietoTulk(TComponent* Owner);
	wchar_t *tunniste(wchar_t *st, int len, int r);
	wchar_t *tieto(wchar_t *st, int len, int r);
	int tietolkm;
	int NaytaSar;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTietoTulk *FormTietoTulk;
//---------------------------------------------------------------------------
#endif
