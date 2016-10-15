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

#ifndef UnitEmit_VAH
#define UnitEmit_VAH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include "VDeclare.h"
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TFormEmit_VA : public TForm
{
__published:	// IDE-managed Components
	TComboBox *CBSarja;
	TLabel *Label1;
	TStringGrid *SG1;
	TButton *BtnTallSarja;
	TButton *BtnTallKaikki;
	TCheckBox *CBAuto;
	TButton *BtnLaskeSrj;
	TButton *BtnLaskeKaikki;
	TCheckBox *CBSail;
	TButton *BtnSulje;
	TLabel *Label2;
	TComboBox *CBOsuus;
	TBitBtn *BitBtn1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall CBSarjaChange(TObject *Sender);
	void __fastcall BtnTallSarjaClick(TObject *Sender);
	void __fastcall BtnTallKaikkiClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall BtnSuljeClick(TObject *Sender);
	void __fastcall BtnLaskeSrjClick(TObject *Sender);
	void __fastcall BtnLaskeKaikkiClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
	void TallSarja(int Srj);
	int Sarja;
	int Osuus;
public:		// User declarations
	__fastcall TFormEmit_VA(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEmit_VA *FormEmit_VA;
//---------------------------------------------------------------------------
#endif
