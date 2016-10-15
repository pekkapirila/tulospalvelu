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

#ifndef UnitTulkintaH
#define UnitTulkintaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
typedef struct {
	int	 no;
	wchar_t nimi[30];
	wchar_t teksti[60];
	wchar_t arvo[60];
} rivitulkintatp;

class TFormTulkinta : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *SG1;
	TLabel *Label1;
	TLabeledEdit *EdtTietue;
	TButton *BtnHyvaksy;
	TButton *BtnLueloput;
	TButton *BtnOhita;
	TButton *BtnKeskeyta;
	TLabel *LblSanoma;
	void __fastcall BtnHyvaksyClick(TObject *Sender);
	void __fastcall BtnLueloputClick(TObject *Sender);
	void __fastcall BtnOhitaClick(TObject *Sender);
	void __fastcall BtnKeskeytaClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormTulkinta(TComponent* Owner);
	void __fastcall NaytaTulkinta(rivitulkintatp *RiviTulk, int nRivi, int rNo, int rTot, const wchar_t *sanoma);
	int Vastaus;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTulkinta *FormTulkinta;
//---------------------------------------------------------------------------
#endif
