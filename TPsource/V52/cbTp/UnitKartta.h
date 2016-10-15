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

#ifndef UnitKarttaH
#define UnitKarttaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ExtDlgs.hpp>
#include <Vcl.Menus.hpp>
#ifndef MAXOSUUSLUKU
#include "HkDef.h"
#else
#include "VDef.h"
#endif
//---------------------------------------------------------------------------
class TFormKartta : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *iedosto1;
	TMenuItem *Lataakartta1;
	TOpenPictureDialog *OpenPictureDialog1;
	TScrollBar *ScrHor;
	TScrollBar *ScrVert;
	TPanel *Panel1;
	TImage *Image1;
	TMenuItem *Koko1;
	TMenuItem *Pienet;
	TMenuItem *Keskikok;
	TMenuItem *Suljekaavake1;
	TMenuItem *Help1;
	TMenuItem *Suuretsymbolit1;
	TMenuItem *Tummatvirheet1;
	TMenuItem *Nyt1;
	TMenuItem *Rastit1;
	TMenuItem *Rata1;
	TMenuItem *Kartankohdistus1;
	TMenuItem *Arvaakohdistus1;
	TMenuItem *Kohdistakahdenrastinavulla1;
	void __fastcall Lataakartta1Click(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall ScrHorScroll(TObject *Sender, TScrollCode ScrollCode, int &ScrollPos);
	void __fastcall ScrVertScroll(TObject *Sender, TScrollCode ScrollCode, int &ScrollPos);
	void __fastcall Suljekaavake1Click(TObject *Sender);
	void __fastcall Image1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall Image1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall Panel1MouseMove(TObject *Sender, TShiftState Shift, int X, int Y);
	void __fastcall Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall Help1Click(TObject *Sender);
	void __fastcall ScrVertKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall PienetClick(TObject *Sender);
	void __fastcall KeskikokClick(TObject *Sender);
	void __fastcall Suuretsymbolit1Click(TObject *Sender);
	void __fastcall Tummatvirheet1Click(TObject *Sender);
	void __fastcall Rastit1Click(TObject *Sender);
	void __fastcall Rata1Click(TObject *Sender);
	void __fastcall Kohdistakahdenrastinavulla1Click(TObject *Sender);

private:	// User declarations
	void __fastcall setScrollPos(void);
	int ScrollPosX;
	int ScrollPosY;
	double kerroin;
	double xLeft;
	double yTop;
	bool mouseDown;
	int mouseX;
	int mouseY;
	int Naytetty;
	ratatp *Rata;
	UnicodeString KarttaFlnm;
public:		// User declarations
	__fastcall TFormKartta(TComponent* Owner);
	int __fastcall Lataakartta(UnicodeString Flnm);
	bool __fastcall KarttaLadattu(void);
	void __fastcall naytaLeimat(ratatp *rt, int *tulkinta, char *koodit);
	void __fastcall naytaRastit(void);
	void __fastcall piirraRasti(int y, int x, wchar_t *kdi, int rastilaji);
	void __fastcall piirraLahto(int y, int x);
	void __fastcall piirraMaali(int y, int x);
	void __fastcall piirraViiva(int y1, int x1, int y2, int x2);
	void __fastcall NaytaOtsikko(UnicodeString Txt);
	void __fastcall paivitaKartta(void);
	int __fastcall imgX(double mapX);
	int __fastcall imgY(double mapY);
	bool karttaLadattu;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormKartta *FormKartta;
//---------------------------------------------------------------------------
#endif
