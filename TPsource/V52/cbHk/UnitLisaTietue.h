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

#ifndef UnitLisaTietueH
#define UnitLisaTietueH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "HkDeclare.h"
//#include "FormEmit.h"
//---------------------------------------------------------------------------
class TFormLisaTietue : public TForm
{
__published:	// IDE-managed Components
	TRadioGroup *RGPohjustus;
	TLabeledEdit *EdAiempi;
	TLabeledEdit *EdAiempiKno;
	TLabeledEdit *EdLuenta;
	TComboBox *CBRata;
	TLabel *Label1;
	TLabeledEdit *EdUusiEmit;
	TButton *Button1;
	TButton *Button2;
	void __fastcall RGPohjustusClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormLisaTietue(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormLisaTietue *FormLisaTietue;
//---------------------------------------------------------------------------
#endif
