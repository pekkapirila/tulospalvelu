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

#ifndef UnitSuodatusH
#define UnitSuodatusH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormSuodatus : public TForm
{
__published:	// IDE-managed Components
	TRadioGroup *RGPeruste;
	TEdit *EdtTieto;
	TLabel *Label1;
	TComboBox *CBSarja;
	TComboBox *CBSeura;
	TLabel *Label2;
	TLabel *Label3;
	TButton *BtnSulje;
	TButton *BtnKayta;
	TButton *Button1;
	TLabel *Label4;
	TComboBox *CBTark;
	TComboBox *CBVaihe;
	TLabel *Label5;
	void __fastcall BtnKaytaClick(TObject *Sender);
	void __fastcall BtnSuljeClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall EdtTietoChange(TObject *Sender);
	void __fastcall CBSarjaChange(TObject *Sender);
	void __fastcall CBSeuraChange(TObject *Sender);
	void __fastcall CBTarkChange(TObject *Sender);
	void __fastcall EdtTietoKeyPress(TObject *Sender, System::WideChar &Key);
private:	// User declarations
public:		// User declarations
	__fastcall TFormSuodatus(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSuodatus *FormSuodatus;
//---------------------------------------------------------------------------
#endif
