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

#include "UnitMessages.h"
#ifdef MAXOSUUSLUKU
#include "UnitMain.h"
#else
#include "HkDeclare.h"
#include "WinHk.h"
#endif
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMessages *FormMessages;
//---------------------------------------------------------------------------
__fastcall TFormMessages::TFormMessages(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormMessages::FormShow(TObject *Sender)
{
	Memo1->Clear();
#ifdef MAXOSUUSLUKU
	for (int r = 0; r < FormMain->Memo1->Lines->Count; r++) {
		if (!FormMain->Memo1->Lines->Strings[r].IsEmpty() && FormMain->Memo1->Lines->Strings[r].Length() > 0)
			Memo1->Lines->Add(FormMain->Memo1->Lines->Strings[r]);
	}
	FormMain->Sanomaikkuna1->Checked = true;
#else
	for (int r = 0; r < FormMain->Memo1->Lines->Count; r++) {
		if (!FormMain->Memo1->Lines->Strings[r].IsEmpty() && FormMain->Memo1->Lines->Strings[r].Length() > 0)
			Memo1->Lines->Add(FormMain->Memo1->Lines->Strings[r]);
	}
	FormMain->Sanomaikkuna1->Checked = true;
#endif
}
//---------------------------------------------------------------------------
void __fastcall TFormMessages::FormResize(TObject *Sender)
{
	Memo1->Width = ClientWidth - Memo1->Left - 4;
	Memo1->Height = ClientHeight - Memo1->Top - 4;
}
//---------------------------------------------------------------------------
void __fastcall TFormMessages::FormClose(TObject *Sender, TCloseAction &Action)
{
#ifdef MAXOSUUSLUKU
	FormMain->Sanomaikkuna1->Checked = false;
#else
	FormMain->Sanomaikkuna1->Checked = false;
#endif}
}
//---------------------------------------------------------------------------
