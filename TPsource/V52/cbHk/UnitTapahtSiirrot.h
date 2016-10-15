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

#ifndef UnitTapahtSiirrotH
#define UnitTapahtSiirrotH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TFormTapahtSiirrot : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TComboBox *CBVaihe;
	TLabel *Label2;
	TEdit *EdtAlaraja;
	TLabel *Label3;
	TEdit *EdtYlaraja;
	TLabel *Label4;
	TEdit *EdtAlku;
	TLabel *Label5;
	TCheckBox *CBNumKaikki;
	TCheckBox *CBTulSarjat;
	TButton *Button1;
	TCheckBox *CBPalautaVakantit;
	TLabel *Label6;
	TEdit *EdtVakBibLisa;
	TButton *Button4;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall EdtVakBibLisaChange(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormTapahtSiirrot(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTapahtSiirrot *FormTapahtSiirrot;
//---------------------------------------------------------------------------
#endif
