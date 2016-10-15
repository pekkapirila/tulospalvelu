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

#ifndef UnitVaToistoH
#define UnitVaToistoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormVaToisto : public TForm
{
__published:	// IDE-managed Components
	TEdit *EdtSarja;
	TLabel *Label1;
	TLabel *Label2;
	TEdit *EdtVali;
	TLabel *Label3;
	TEdit *EdtMtk1;
	TLabel *Label4;
	TEdit *EdtEnspiste;
	TLabel *Label5;
	TEdit *EdtLkm;
	TLabel *Label6;
	TEdit *EdtAskel;
	TLabel *Label7;
	TComboBox *CBJulkisuus;
	TButton *Button1;
	TButton *Button2;
	TLabel *Label8;
	TComboBox *CBtapa;
	TRadioGroup *RGdes;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormVaToisto(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormVaToisto *FormVaToisto;
//---------------------------------------------------------------------------
#endif
