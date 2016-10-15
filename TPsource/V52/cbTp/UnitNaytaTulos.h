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

#ifndef UnitNaytaTulosH
#define UnitNaytaTulosH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#ifndef MAXOSUUSLUKU
#include "HkDeclare.h"
#else
#include "VDeclare.h"
#endif
//---------------------------------------------------------------------------
class TFormNaytaTulos : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TPanel *Panel1;
	TMemo *Memo1;
	TMainMenu *MainMenu1;
	TMenuItem *Valinnat1;
	TMenuItem *Teksti1;
	TMenuItem *Nytnkesto1;
	TMenuItem *Nyttulosjasija1;
	TLabel *LblTulos;
	TLabel *LblSija;
	TLabel *LblEro;
	TMenuItem *Esiluennanlhtraja1;
	void __fastcall Nytnkesto1Click(TObject *Sender);
	void __fastcall Teksti1Click(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Nyttulosjasija1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Esiluennanlhtraja1Click(TObject *Sender);
private:	// User declarations
	MESSAGE void __fastcall DisplTimerHandler(TMyMessage &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYDISPLTIMER, TMyMessage, DisplTimerHandler);
END_MESSAGE_MAP(TComponent)

	int Kesto[2];
	int Aika;
public:		// User declarations
	__fastcall TFormNaytaTulos(TComponent* Owner);
	void __fastcall NaytaTulos(wchar_t *nimi, wchar_t trk, int tls, int sj, int ntls);
	int applyParams(void);
	void haeParams(void);
	NaytaTulosIkkParamClass IkkParam;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormNaytaTulos *FormNaytaTulos;
//---------------------------------------------------------------------------
#endif
