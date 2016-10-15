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

#include "UnitAnnaTeksti.h"
//#include "HkDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAnnaTeksti *FormAnnaTeksti;
//---------------------------------------------------------------------------
__fastcall TFormAnnaTeksti::TFormAnnaTeksti(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormAnnaTeksti::Button1Click(TObject *Sender)
{
	Teksti = EdTeksti->Text;
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormAnnaTeksti::EdTekstiExit(TObject *Sender)
{
	EdTeksti->Text = EdTeksti->Text.SubString(1, Len);
}
//---------------------------------------------------------------------------
void __fastcall TFormAnnaTeksti::Kysy(wchar_t *line, wchar_t *prompt, int len)
{
	LblPrompt->Caption = prompt;
	Len = len;
	EdTeksti->Text = line;
	Teksti = EdTeksti->Text;
}
//---------------------------------------------------------------------------
void __fastcall TFormAnnaTeksti::Button2Click(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFormAnnaTeksti::EdTekstiKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key < 32 && Key != 8)
		Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormAnnaTeksti::EdTekstiKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN)
		FocusControl(Button1);
	if (Key == VK_ESCAPE)
		FocusControl(Button2);
}
//---------------------------------------------------------------------------
