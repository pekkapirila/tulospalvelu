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

#ifndef UnitEmitnoutoH
#define UnitEmitnoutoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Buttons.hpp>

#ifndef MAXOSUUSLUKU
#include "WinHk.h"
#else
#include "UnitMain.h"
#endif
#define DEVLKM 4
//---------------------------------------------------------------------------
class TFormEmitNouto : public TForm
{
__published:	// IDE-managed Components
	TLabel *Palvelin;
	TEdit *EdtServer;
	TLabel *Label3;
	TEdit *EdtAika;
	TButton *BtnNyt;
	TLabel *Label4;
	TEdit *EdtLaite1;
	TEdit *EdtLaite2;
	TEdit *EdtLaite3;
	TEdit *EdtLaite4;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label12;
	TCheckBox *CBAvoinna1;
	TButton *BtnClose;
	TEdit *EdtHakuVali;
	TLabel *Label13;
	TLabel *Label8;
	TEdit *EdtPortti;
	TBitBtn *BitBtn1;
	TButton *BtnUusihaku1;
	TLabel *Label1;
	TEdit *EdtLaiteNo1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BtnUusihakuClick(TObject *Sender);
	void __fastcall EdtLaiteChange(TObject *Sender);
	void __fastcall BtnNytClick(TObject *Sender);
	void __fastcall EdtAikaChange(TObject *Sender);
	void __fastcall BtnCloseClick(TObject *Sender);
	void __fastcall CBAvoinna1Click(TObject *Sender);
private:	// User declarations
	void __fastcall EdtLaiteChange(int no);
	void __fastcall EdtAlkuChange(int no);
	void naytaStatus(void);
	TEdit *EdtLaite[DEVLKM];
	bool inNaytaStatus;
public:		// User declarations
	__fastcall TFormEmitNouto(TComponent* Owner);
	int devices[DEVLKM];
	int devCount;
	int HakuVali;
	int hakulkm;
	wchar_t aikaraja[100];
	int Jono;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEmitNouto *FormEmitNouto;
//---------------------------------------------------------------------------
#endif
