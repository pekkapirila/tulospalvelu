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
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Vcl.Menus.hpp>
#include "HkDeclare.h"
//---------------------------------------------------------------------------
class TFormStatus : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *LkmGrid;
	TStringGrid *YhteysGrid;
	TGroupBox *GBAutotulostus;
	TLabel *Label1;
	TEdit *EdtLaajuus;
	TLabel *Label2;
	TEdit *EdtKynnys;
	TLabel *Label3;
	TEdit *EdtKorkein;
	TLabel *Label4;
	TEdit *EdtAikaaskel;
	TMainMenu *MainMenu1;
	TMenuItem *Nyt1;
	TMenuItem *VaiheenMahd;
	TMenuItem *YhteisMahd;
	TMenuItem *Palkintojenjako1;
	TMenuItem *Suljekaavake1;
	TMenuItem *Valinnat1;
	TMenuItem *Tuloskirjoitin1;
	TMenuItem *Yhteydeterikaavakkeella1;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall VaiheenMahdClick(TObject *Sender);
	void __fastcall YhteisMahdClick(TObject *Sender);
	void __fastcall Palkintojenjako1Click(TObject *Sender);
	void __fastcall LkmGridDblClick(TObject *Sender);
	void __fastcall Suljekaavake1Click(TObject *Sender);
	void __fastcall Tuloskirjoitin1Click(TObject *Sender);
	void __fastcall Yhteydeterikaavakkeella1Click(TObject *Sender);
private:	// User declarations
	bool palkJaettu[MAXSARJALUKU];
	int jakoCol;
	int mahdCol;
	int edaikaNyt;
	bool colMuutos;
public:		// User declarations
	__fastcall TFormStatus(TComponent* Owner);
	void __fastcall Lukumaarat(void);
	int applyParams(void);
	void haeParams(void);
	statusIkkParamClass IkkParam;
	wchar_t Kohde[30];
	bool paivitaAikanaytot;
	wchar_t AutoLaaj[20];
	int AutoKynnys;
	int AutoKorkein;
	int AutoAikaaskel;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormStatus *FormStatus;
//---------------------------------------------------------------------------
#endif
