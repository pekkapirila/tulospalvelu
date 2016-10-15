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

#include "UnitAnnaArvo.h"
//#include "HkDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAnnaArvo *FormAnnaArvo;
//---------------------------------------------------------------------------
__fastcall TFormAnnaArvo::TFormAnnaArvo(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormAnnaArvo::Button1Click(TObject *Sender)
{
	Arvo = _wtoi(EdArvo->Text.c_str());
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormAnnaArvo::EdArvoExit(TObject *Sender)
{
	EdArvo->Text = UnicodeString(_wtoi(EdArvo->Text.c_str()));
}
//---------------------------------------------------------------------------
void __fastcall TFormAnnaArvo::Kysy(int arvo, wchar_t *prompt)
{
	LblPrompt->Caption = prompt;
	EdArvo->Text = UnicodeString(arvo);
}
//---------------------------------------------------------------------------
void __fastcall TFormAnnaArvo::Button2Click(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFormAnnaArvo::EdArvoKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key < 32 && Key != 8)
		Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormAnnaArvo::EdArvoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN)
		FocusControl(Button1);
	if (Key == VK_ESCAPE)
		FocusControl(Button2);
}
//---------------------------------------------------------------------------

