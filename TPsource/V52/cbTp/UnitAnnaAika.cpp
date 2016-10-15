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

#include "UnitAnnaAika.h"
#ifdef MAXOSUUSLUKU
#include "VDeclare.h"
#else
#include "HkDeclare.h"
#endif
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAnnaAika *FormAnnaAika;
//---------------------------------------------------------------------------
__fastcall TFormAnnaAika::TFormAnnaAika(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormAnnaAika::Button1Click(TObject *Sender)
{
	Aika = wstrtoaika_vap(EdAika->Text.c_str(), T0);
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormAnnaAika::EdAikaExit(TObject *Sender)
{
	wchar_t as[20];

	Aika = wstrtoaika_vap(EdAika->Text.c_str(), T0);
	EdAika->Text = aikatowstr_cols_n(as, Aika, T0, 0, Len);
}
//---------------------------------------------------------------------------
void __fastcall TFormAnnaAika::Kysy(int aika, wchar_t *prompt, int t0, int len)
{
	wchar_t as[20];

	LblPrompt->Caption = prompt;
	Len = len;
	T0 = t0;
	EdAika->Text = aikatowstr_cols_n(as, aika, t0, 0, len);
}
//---------------------------------------------------------------------------
void __fastcall TFormAnnaAika::Button2Click(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFormAnnaAika::EdAikaKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key < 32 && Key != 8)
		Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormAnnaAika::EdAikaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN)
		FocusControl(Button1);
	if (Key == VK_ESCAPE)
		FocusControl(Button2);
}
//---------------------------------------------------------------------------

