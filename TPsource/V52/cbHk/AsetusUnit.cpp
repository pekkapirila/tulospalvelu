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

#include "HkDeclare.h"
#include "AsetusUnit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TAsetusForm *AsetusForm;
//---------------------------------------------------------------------------
__fastcall TAsetusForm::TAsetusForm(TComponent* Owner)
   : TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TAsetusForm::BtnPeruutaClick(TObject *Sender)
{
//  ModalResult = mrCancel;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TAsetusForm::BtnOKClick(TObject *Sender)
{
  nakviive = Edit3->Text.ToInt();
  UDPCliWait = Edit4->Text.ToInt();
//  ModalResult = mrOk;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TAsetusForm::Luekorostustiedot1Click(TObject *Sender)
{
	lueKorRajat();
}
//---------------------------------------------------------------------------

void __fastcall TAsetusForm::FormShow(TObject *Sender)
{
   Edit3->Text = UnicodeString(nakviive);
   Edit4->Text = UnicodeString(UDPCliWait);
}
//---------------------------------------------------------------------------

void __fastcall TAsetusForm::CheckBox1Click(TObject *Sender)
{
	TestiToiminnot = CheckBox1->Checked;
}
//---------------------------------------------------------------------------

