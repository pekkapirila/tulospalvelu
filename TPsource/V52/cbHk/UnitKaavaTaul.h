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

#ifndef UnitKaavaTaulH
#define UnitKaavaTaulH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include "HkDeclare.h"
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TFormKaavaTaul : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *SG1;
	TComboBox *CBSarja;
	TLabel *Label1;
	TLabel *Label2;
	TComboBox *CBVaihe;
	TCheckBox *CBSij;
	TCheckBox *CBTls;
	TBitBtn *BitBtn1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CBSarjaChange(TObject *Sender);
	void __fastcall CBVaiheChange(TObject *Sender);
	void __fastcall CBSijClick(TObject *Sender);
	void __fastcall CBTlsClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormKaavaTaul(TComponent* Owner);
	void __fastcall NaytaTaul(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormKaavaTaul *FormKaavaTaul;
//---------------------------------------------------------------------------
#endif
