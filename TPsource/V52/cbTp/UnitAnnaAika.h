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

#ifndef UnitAnnaAikaH
#define UnitAnnaAikaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TFormAnnaAika : public TForm
{
__published:	// IDE-managed Components
	TEdit *EdAika;
	TButton *Button1;
	TButton *Button2;
	TLabel *LblPrompt;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall EdAikaExit(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall EdAikaKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EdAikaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
private:	// User declarations
	int Len;
	int T0;
public:		// User declarations
	__fastcall TFormAnnaAika(TComponent* Owner);
	void __fastcall Kysy(int aika, wchar_t *prompt, int t0, int len);
	int Aika;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAnnaAika *FormAnnaAika;
//---------------------------------------------------------------------------
#endif
