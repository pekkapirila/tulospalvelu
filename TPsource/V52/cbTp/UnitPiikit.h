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

#ifndef UnitPiikitH
#define UnitPiikitH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Buttons.hpp>
#ifndef MAXOSUUSLUKU
#include "WinHk.h"
#else
#include "UnitMain.h"
#endif

#define MAXTUNNUS  899
#define MAXRINNKOODI 10

//---------------------------------------------------------------------------
class TFormPiikit : public TForm
{
__published:	// IDE-managed Components
	TLabeledEdit *EdtNimi;
	TLabeledEdit *EdtKilpNo;
	TComboBox *CBRata;
	TLabel *Label1;
	TRadioGroup *RGPuoli;
	TMemo *Memo1;
	TComboBox *CBRasti;
	TLabel *Label2;
	TPanel *Panel1;
	TImage *Image1;
	TStringGrid *SG1;
	TComboBox *CBOsuus;
	TLabel *Label3;
	TBitBtn *BitBtn1;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall CBRataChange(TObject *Sender);
	void __fastcall RGPuoliClick(TObject *Sender);
	void __fastcall CBRastiClick(TObject *Sender);
	void __fastcall EdtKilpNoExit(TObject *Sender);
	void __fastcall CBOsuusChange(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall EdtKilpNoKeyPress(TObject *Sender, System::WideChar &Key);
private:	// User declarations
	void __fastcall HaeRata(void);
	void __fastcall PohjRata(void);
	void piirrapiikit(void);
	void prtpiste(int x, int y, int korostus);
public:		// User declarations
	__fastcall TFormPiikit(TComponent* Owner);
	void __fastcall Piirra(void);
	int KilpNo;
	int Osuus;
	int iRata;
	int tark[MAXNLEIMA];
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPiikit *FormPiikit;
//---------------------------------------------------------------------------
#endif
