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

#include "UnitNappain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormNappain *FormNappain;
//---------------------------------------------------------------------------
__fastcall TFormNappain::TFormNappain(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormNappain::Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	KeyCode = Key;
	KeyShift = Shift;
	Edit2->Text = KeyCode;
	Edit3->Text =
		(KeyShift.Contains(ssShift) ? UnicodeString(L"Shift") : UnicodeString(L"")) +
		(KeyShift.Contains(ssCtrl) ? UnicodeString(L" Ctrl") : UnicodeString(L"")) +
		(KeyShift.Contains(ssAlt) ? UnicodeString(L" Alt") : UnicodeString(L""));
}
//---------------------------------------------------------------------------
void __fastcall TFormNappain::Edit1KeyPress(TObject *Sender, System::WideChar &Key)
{
	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormNappain::Button1Click(TObject *Sender)
{
	KeyCode = 0;
	KeyShift >> ssShift >> ssCtrl >> ssAlt;
	Edit2->Text = L"";
	Edit3->Text = L"";
}
//---------------------------------------------------------------------------
void __fastcall TFormNappain::Button2Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormNappain::FormClose(TObject *Sender, TCloseAction &Action)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormNappain::FormShow(TObject *Sender)
{
	Edit2->Text = KeyCode;
	Edit3->Text =
		(KeyShift.Contains(ssShift) ? UnicodeString(L"Shift") : UnicodeString(L"")) +
		(KeyShift.Contains(ssCtrl) ? UnicodeString(L" Ctrl") : UnicodeString(L"")) +
		(KeyShift.Contains(ssAlt) ? UnicodeString(L" Alt") : UnicodeString(L""));
	CBPiste->Clear();
	CBPiste->Items->Add(L"Automaattisesti määräytyvä");
	CBPiste->Items->Add(L"Lähtö");
	CBPiste->Items->Add(L"Maali");
	for (int i = 1; i <= MaxVa; i++)
		CBPiste->Items->Add(UnicodeString(i)+L". väliaika");
	CBPiste->ItemIndex = Piste + 2;
}
//---------------------------------------------------------------------------
void __fastcall TFormNappain::CBPisteClick(TObject *Sender)
{
	Piste = (int) CBPiste->ItemIndex - 2;
}
//---------------------------------------------------------------------------
void __fastcall TFormNappain::Initiate(int KeyCodeIn, TShiftState KeyShiftIn, int MaxVaIn, int PisteIn)
{
	KeyCode = KeyCodeIn;
	KeyShift = KeyShiftIn;
	MaxVa = MaxVaIn;
	Piste = PisteIn;
}
//---------------------------------------------------------------------------
void __fastcall TFormNappain::GetValues(int &KeyCodeOut, TShiftState &KeyShiftOut, int &PisteOut)
{
	KeyCodeOut = KeyCode;
	KeyShiftOut = KeyShift;
	PisteOut = Piste;
}
//---------------------------------------------------------------------------

