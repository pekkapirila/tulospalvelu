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

#include "UnitEditDialog.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TEditDialog *EditDialog;
//---------------------------------------------------------------------------
__fastcall TEditDialog::TEditDialog(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TEditDialog::KysyArvo(UnicodeString Arvo, UnicodeString Nimi)
{
	Label1->Caption = Nimi;
	Edit1->Text = Arvo;
}
//---------------------------------------------------------------------------
void __fastcall TEditDialog::Edit1KeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		Arvo = Edit1->Text;
		Close();
	}
}
//---------------------------------------------------------------------------
void __fastcall TEditDialog::FormClose(TObject *Sender, TCloseAction &Action)
{
  ModalResult = mrOk;
}
//---------------------------------------------------------------------------
