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

#ifndef UnitSelect3H
#define UnitSelect3H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#define WM_MYSELTIMER		(WM_APP + 2207)
struct TMyMessage2 {
	Cardinal Msg;
	int wparam;
	int lparam;
	LRESULT Result;
	};

//---------------------------------------------------------------------------
class TFormSelect3 : public TForm
{
__published:	// IDE-managed Components
   TMemo *Sanoma;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	MESSAGE void __fastcall SelTimerHandler(TMyMessage2 &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYSELTIMER, TMyMessage2, SelTimerHandler);
END_MESSAGE_MAP(TComponent)
public:		// User declarations
   __fastcall TFormSelect3(TComponent* Owner);
	void __fastcall Kysy(int nButton, wchar_t *Teksti, wchar_t *Otsikko,
		wchar_t *Vast1, wchar_t *Vast2, wchar_t *Vast3, int Kesto = 0);
	int selResult;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSelect3 *FormSelect3;
//---------------------------------------------------------------------------
#endif
