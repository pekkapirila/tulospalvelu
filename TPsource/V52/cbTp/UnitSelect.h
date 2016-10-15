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

#ifndef UnitSelectH
#define UnitSelectH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TValitse : public TForm
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
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	wchar_t opts[100];
	wchar_t val;
   __fastcall TValitse(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TValitse *Valitse;

extern wchar_t selected;
extern TValitse *Valitse;
//---------------------------------------------------------------------------
#endif
