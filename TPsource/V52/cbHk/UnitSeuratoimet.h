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

#ifndef UnitSeuratoimetH
#define UnitSeuratoimetH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "HkDeclare.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TFormSeuratoimet : public TForm
{
__published:	// IDE-managed Components
	TRadioGroup *RGToiminto;
	TCheckBox *CBPiiri;
	TButton *Button1;
	TCheckBox *CBMaa;
	TButton *Button2;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TComboBox *CBsrj1;
	TComboBox *CBsrj2;
	TLabel *Label2;
	TGroupBox *GroupBox2;
	TLabel *Label3;
	TLabel *Label4;
	TComboBox *CBLahto;
	TComboBox *CBUusisarja;
	TBitBtn *BitBtn1;
	TButton *Button3;
	TCheckBox *CBVapaaKoko;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall RGToimintoClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CBsrj2Change(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormSeuratoimet(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSeuratoimet *FormSeuratoimet;
//---------------------------------------------------------------------------
#endif
