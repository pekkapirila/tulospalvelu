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

#ifndef UnitUusintaH
#define UnitUusintaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormUusinta : public TForm
{
__published:	// IDE-managed Components
	TComboBox *CBYhteydet;
	TRadioGroup *RGTapa;
	TLabel *Label1;
	TEdit *EdtRaja;
	TLabel *Label2;
	TButton *Button1;
	TButton *Button2;
	TRadioGroup *RGTila;
	TButton *Button3;
	TPanel *Panel1;
	TButton *Button4;
	TEdit *EdtSulku;
	TLabel *Label3;
	TComboBox *CBSarjat;
	TComboBox *CBOsuudet;
	TLabel *Label4;
	TButton *Button5;
	TRadioGroup *RGRajaus;
	TGroupBox *GBAikavali;
	TEdit *EdtAlku;
	TLabel *Label5;
	TLabel *Label6;
	TEdit *EdtLoppu;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall CBYhteydetChange(TObject *Sender);
	void __fastcall RGTapaClick(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormUusinta(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormUusinta *FormUusinta;
//---------------------------------------------------------------------------
#endif
