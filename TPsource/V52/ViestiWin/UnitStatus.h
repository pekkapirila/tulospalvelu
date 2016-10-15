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

#ifndef UnitStatusH
#define UnitStatusH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include "VDeclare.h"
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TFormStatus : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *LkmGrid;
	TStringGrid *YhteysGrid;
	TGroupBox *GBAutotulostus;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *EdtLaajuus;
	TEdit *EdtKynnys;
	TEdit *EdtKorkein;
	TEdit *EdtAikaaskel;
	TMainMenu *MainMenu1;
	TMenuItem *Nyt1;
	TMenuItem *Help1;
	TMenuItem *Yhteislht1;
	TMenuItem *Hylkysesitykset1;
	TMenuItem *Emittietueet1;
	TMenuItem *Emitkoodeja1;
	TMenuItem *N1;
	TMenuItem *Suljekaavake1;
	TMenuItem *Yhteydeterikaavakkeella1;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Yhteislht1Click(TObject *Sender);
	void __fastcall Hylkysesitykset1Click(TObject *Sender);
	void __fastcall Emittietueet1Click(TObject *Sender);
	void __fastcall Help1Click(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Emitkoodeja1Click(TObject *Sender);
	void __fastcall Suljekaavake1Click(TObject *Sender);
	void __fastcall Yhteydeterikaavakkeella1Click(TObject *Sender);
private:	// User declarations
	void __fastcall FormSetup(void);
	int edaikaNyt;
	bool colMuutos;
	int monisarja;
	int rcnt[MAXSARJALUKU+1];
	int iLisa;
	int edTap[NTAPFORM];
public:		// User declarations
	__fastcall TFormStatus(TComponent* Owner);
	void __fastcall Lukumaarat(void);
	void __fastcall muutos(int srj, int os, int piste);
	int applyParams(void);
	void haeParams(void);
	statusIkkParamClass IkkParam;
	wchar_t Kohde[30];
	bool paivitaAikanaytot;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormStatus *FormStatus;
//---------------------------------------------------------------------------
#endif
