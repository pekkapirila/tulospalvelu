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

#include "UnitMsgs.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMsgs *FormMsgs;
//---------------------------------------------------------------------------
__fastcall TFormMsgs::TFormMsgs(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormMsgs::FormResize(TObject *Sender)
{
	Memo1->Width = ClientWidth - Memo1->Left - 4;
	Memo1->Height = ClientHeight - Memo1->Top - 4;
}
//---------------------------------------------------------------------------
void __fastcall TFormMsgs::FormClose(TObject *Sender, TCloseAction &Action)
{
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormMsgs::Button1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
