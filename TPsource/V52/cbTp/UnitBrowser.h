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

#ifndef UnitBrowserH
#define UnitBrowserH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <OleCtrls.hpp>
#include <SHDocVw.hpp>
//---------------------------------------------------------------------------
class TFormBrowser : public TForm
{
__published:	// IDE-managed Components
	TWebBrowser *WebBrowser1;
	TEdit *EditURL;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall EditURLKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall WebBrowser1NavigateComplete2(TObject *ASender, const IDispatch *pDisp,
          const OleVariant &URL);
	void __fastcall WebBrowser1BeforeNavigate2(TObject *ASender, const IDispatch *pDisp,
          const OleVariant &URL, const OleVariant &Flags, const OleVariant &TargetFrameName,
          const OleVariant &PostData, const OleVariant &Headers,
          WordBool &Cancel);

private:	// User declarations
public:		// User declarations
	__fastcall TFormBrowser(TComponent* Owner);
	bool NavigComplete;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormBrowser *FormBrowser;
//---------------------------------------------------------------------------
#endif
