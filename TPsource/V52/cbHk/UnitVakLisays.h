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

#ifndef UnitVakLisaysH
#define UnitVakLisaysH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TFormVakLisays : public TForm
{
__published:	// IDE-managed Components
	TComboBox *CBSarja;
	TLabel *Sarja;
	TLabel *Label1;
	TEdit *EditAlku;
	TLabel *Label2;
	TEdit *EditLkm;
	TLabel *Label3;
	TEdit *EditNimi;
	TLabel *Label4;
	TEdit *EditLahto;
	TButton *Button1;
	TLabel *Label5;
	TLabel *Label6;
	TEdit *EditSeura;
	TRadioGroup *RGEil;
	TEdit *EditLuotu;
	TLabel *Label7;
	TBitBtn *BitBtn1;
	TCheckBox *CBAukot;
	TButton *Button4;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormVakLisays(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormVakLisays *FormVakLisays;
//---------------------------------------------------------------------------
#endif
