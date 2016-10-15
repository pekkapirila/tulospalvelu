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

#ifndef UnitSelect2H
#define UnitSelect2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TFormValitse2 : public TForm
{
__published:	// IDE-managed Components
   TMemo *Sanoma;
   TEdit *Optiot;
   TEdit *Valittu;
   TButton *OKBtn;
   TLabel *Label1;
   void __fastcall OKBtnClick(TObject *Sender);
	void __fastcall ValittuKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall ValittuKeyPress(TObject *Sender, System::WideChar &Key);
private:	// User declarations
public:		// User declarations
   __fastcall TFormValitse2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormValitse2 *FormValitse2;

extern wchar_t selected;
extern TFormValitse2 *FormValitse2;
//---------------------------------------------------------------------------
#endif
