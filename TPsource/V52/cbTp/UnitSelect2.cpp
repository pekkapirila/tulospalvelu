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

#include <vcl.h>
#pragma hdrstop

#include "UnitSelect2.h"
//#include "WinHk.h"
#include <bstrings.h>
#include <wchar.h>
#include "tputil.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TFormValitse2::TFormValitse2(TComponent* Owner)
   : TForm(Owner)
{
   selected = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormValitse2::OKBtnClick(TObject *Sender)
{
   wchar_t ch, st[100] = L"";
   wchar_t wc[2] = L"";

   if (Valittu->Text.Length() > 0) {
	  wc[0] = Valittu->Text.c_str()[0];
	  wcsncpy(st, wc, 2);
	  ch = upcasewsc(st[0]);
	  wcsncpy(st, Optiot->Text.c_str(), 99);
	  if (wcswcind(ch, st) >= 0) {
		 selected = ch;
         ModalResult = mrOk;
         }
      }
}
//---------------------------------------------------------------------------

void __fastcall TFormValitse2::ValittuKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN)
		OKBtnClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormValitse2::ValittuKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r')
		Key = 0;
}
//---------------------------------------------------------------------------

