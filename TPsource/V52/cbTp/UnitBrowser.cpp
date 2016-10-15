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

#include "UnitBrowser.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormBrowser *FormBrowser;

//---------------------------------------------------------------------------
__fastcall TFormBrowser::TFormBrowser(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormBrowser::FormResize(TObject *Sender)
{
	EditURL->Width = ClientWidth - EditURL->Left - 8;
	WebBrowser1->Height = ClientHeight - WebBrowser1->Top - 8;
	WebBrowser1->Width = ClientWidth - WebBrowser1->Left - 8;
//	WebBrowser1->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormBrowser::EditURLKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN) {
		WebBrowser1->Navigate(EditURL->Text.w_str());
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormBrowser::WebBrowser1NavigateComplete2(TObject *ASender, const IDispatch *pDisp,
		  const OleVariant &URL)
{
	NavigComplete = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormBrowser::WebBrowser1BeforeNavigate2(TObject *ASender, const IDispatch *pDisp,
		  const OleVariant &URL, const OleVariant &Flags, const OleVariant &TargetFrameName,
		  const OleVariant &PostData, const OleVariant &Headers,
		  WordBool &Cancel)
{
/*
	static bool emulSet;
	if (!emulSet)
		setBrowserEmulation();
	emulSet = true;
*/
	NavigComplete = false;
	EditURL->Text = UnicodeString(URL.VOleStr);
}
//---------------------------------------------------------------------------


