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

#ifndef UnitLisaTekstitH
#define UnitLisaTekstitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormLisaTekstit : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *iedosto1;
	TMenuItem *Luetekstit1;
	TMenuItem *allennatekstit1;
	TMenuItem *Poistu1;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TCheckBox *CheckBox1;
	TCheckBox *CheckBox2;
	TCheckBox *CheckBox3;
	TCheckBox *CheckBox4;
	TLabel *Label1;
	void __fastcall Poistu1Click(TObject *Sender);
	void __fastcall Luetekstit1Click(TObject *Sender);
	void __fastcall allennatekstit1Click(TObject *Sender);
	void __fastcall CheckBox1Click(TObject *Sender);
	void __fastcall CheckBox2Click(TObject *Sender);
	void __fastcall CheckBox3Click(TObject *Sender);
	void __fastcall CheckBox4Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormLisaTekstit(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormLisaTekstit *FormLisaTekstit;
//---------------------------------------------------------------------------
#endif
