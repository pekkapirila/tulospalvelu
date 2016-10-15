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

#ifndef UnitAnnaTekstiH
#define UnitAnnaTekstiH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TFormAnnaTeksti : public TForm
{
__published:	// IDE-managed Components
	TEdit *EdTeksti;
	TButton *Button1;
	TButton *Button2;
	TLabel *LblPrompt;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall EdTekstiExit(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall EdTekstiKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EdTekstiKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);

private:	// User declarations
	int Len;
public:		// User declarations
	__fastcall TFormAnnaTeksti(TComponent* Owner);
	void __fastcall Kysy(wchar_t *teksti, wchar_t *prompt, int len);
	UnicodeString Teksti;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAnnaTeksti *FormAnnaTeksti;
//---------------------------------------------------------------------------
#endif
