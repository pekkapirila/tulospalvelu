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

#ifndef UnitValitseKilpH
#define UnitValitseKilpH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TFormValitseKilp : public TForm
{
__published:	// IDE-managed Components
	TComboBox *CBKilpailu;
	TButton *BtnValittu;
	TButton *BtnMuu;
	TButton *BtnPeruuta;
	void __fastcall BtnValittuClick(TObject *Sender);
	void __fastcall BtnMuuClick(TObject *Sender);
	void __fastcall BtnPeruutaClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormValitseKilp(TComponent* Owner);
	void __fastcall ValitseKilp(void);
	UnicodeString cfgFName;
	UnicodeString Path;
	UnicodeString Title;
	bool ilmTila;
	bool Avaa;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormValitseKilp *FormValitseKilp;
//---------------------------------------------------------------------------
#endif
