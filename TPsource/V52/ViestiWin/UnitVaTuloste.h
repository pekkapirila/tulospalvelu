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

#ifndef UnitVaTulosteH
#define UnitVaTulosteH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include "VDeclare.h"
//---------------------------------------------------------------------------
class TFormVaTuloste : public TForm
{
__published:	// IDE-managed Components
	TDrawGrid *DG1;
	TLabel *Label1;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TMainMenu *MainMenu1;
	TMenuItem *Valinnat1;
	TMenuItem *AinoaSuoraan;
	TMenuItem *AinaViimeinen;
	TMenuItem *Kirjoittimenvalinta1;
	TLabel *Label2;
	TEdit *EdtKilpno;
	TLabel *Label3;
	TEdit *EdtOsuus;
	TLabel *Label4;
	TEdit *EdtJoukkue;
	TLabel *Label5;
	TEdit *EdtBadge;
	TLabel *Label6;
	TEdit *EdtVahvistus;
	TLabel *Label7;
	TLabel *Label8;
	TMenuItem *N1;
	TMenuItem *Suljekaavake1;
	void __fastcall AinoaSuoraanClick(TObject *Sender);
	void __fastcall AinaViimeinenClick(TObject *Sender);
	void __fastcall EdtKilpnoKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EdtOsuusKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall DG1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall DG1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Kirjoittimenvalinta1Click(TObject *Sender);
	void __fastcall DG1KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EdtVahvistusEnter(TObject *Sender);
	void __fastcall EdtVahvistusExit(TObject *Sender);
	void __fastcall EdtVahvistusKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Suljekaavake1Click(TObject *Sender);

private:	// User declarations
	void __fastcall NaytaKilpailijat(int badge);
	int __fastcall NaytaJoukkue(void);
	void __fastcall Nollaa(void);
	void __fastcall Tulosta(void);
	MESSAGE void __fastcall NewCardHandler(TMyMessage &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYMSGNEWCARD, TMyMessage, NewCardHandler);
END_MESSAGE_MAP(TComponent)

	cellTp Cells[5][MAXOSUUSLUKU];
	kilptietue Kilp;
	int Kilpno;
	int DKilp;
	int Osuus;
	int KilpnoA[MAXOSUUSLUKU];
	int OsuusA[MAXOSUUSLUKU];
	UnicodeString JoukkueA[MAXOSUUSLUKU];
	int Badge;
	int Tietue;
	bool OdottaaKorttia;
	wchar_t EmitKirjoitin[200];
	tulostusparamtp EmitTulPrm;
public:		// User declarations
	__fastcall TFormVaTuloste(TComponent* Owner);
	void __fastcall HaeUusiTietue(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormVaTuloste *FormVaTuloste;
//---------------------------------------------------------------------------
#endif
