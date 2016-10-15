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

#ifndef UnitEmithakuH
#define UnitEmithakuH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "HkDeclare.h"
//---------------------------------------------------------------------------
class TFormEmithaku : public TForm
{
__published:	// IDE-managed Components
	TEdit *Edit1;
	TRadioGroup *RG1;
	TLabel *Label1;
	TButton *Button1;
	TStringGrid *SG1;
	TButton *BtnHyvaksy;
	TButton *Button2;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Edit1Exit(TObject *Sender);
	void __fastcall BtnHyvaksyClick(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall SG1DblClick(TObject *Sender);
	void __fastcall Edit1KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall SG1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// User declarations
public:		// User declarations
	__fastcall TFormEmithaku(TComponent* Owner);
	int Kilpno;
	int Kdi;
	int nLoytyi;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEmithaku *FormEmithaku;
//---------------------------------------------------------------------------
#endif
