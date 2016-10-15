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

#include "UnitSelect.h"
//#include "WinHk.h"
#include <bstrings.h>
#include <wchar.h>
#include "tputil.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TValitse::TValitse(TComponent* Owner)
   : TForm(Owner)
{
   selected = 0;
}
//---------------------------------------------------------------------------
void __fastcall TValitse::OKBtnClick(TObject *Sender)
{
   wchar_t ch, st[100] = L"";
   wchar_t wc[2] = L"";

   if (Valittu->Text.Length() > 0) {
	  wcsncpy(st, Valittu->Text.c_str(), 99);
	  elimwbl2(st);
	  if (st[0]) {
		  ch = upcasewsc(st[0]);
		  if (wcswcind(ch, opts) >= 0) {
			  selected = ch;
			  ModalResult = mrOk;
			  }
		  }
	  }
   if (selected == 0)
	   Valittu->Text = L"";
}
//---------------------------------------------------------------------------

void __fastcall TValitse::ValittuKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN)
		OKBtnClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TValitse::ValittuKeyPress(TObject *Sender, System::WideChar &Key)

{
	wchar_t ch;
	if (Key == L'\r') {
		ch = towupper(val);
		Key = 0;
		}
	else
		ch = towupper(Key);
	if (wcswcind(ch, opts) >= 0) {
		  selected = ch;
		  ModalResult = mrOk;
		  }
	else {
		Key = 0;
	   Valittu->Text = L"";
	   }
	val = 0;
}
//---------------------------------------------------------------------------

void __fastcall TValitse::FormShow(TObject *Sender)
{
	wcsncpy(opts, Optiot->Text.c_str(), 99);
	if (Valittu->Text.Length() > 0)
		val = Valittu->Text.c_str()[0];
}
//---------------------------------------------------------------------------

