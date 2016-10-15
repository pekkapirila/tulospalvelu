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

#ifndef UnitPartioH
#define UnitPartioH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include "VDeclare.h"
//---------------------------------------------------------------------------
class TFormPartio : public TForm
{
__published:	// IDE-managed Components
	TDrawGrid *DG1;
	TEdit *EdtKilpno;
	TEdit *EdtJoukkue;
	TStringGrid *SGNimet;
	TEdit *EdtSarja;
	TEdit *EdtSija;
	TEdit *EdtTulos;
	TEdit *EdtTark;
	TEdit *EdtRastiPisteet;
	TButton *BtnTallenna;
	TEdit *EdtSakko;
	TEdit *EdtPisteet;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall DG1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall BtnTallennaClick(TObject *Sender);
	void __fastcall EdtSakkoChange(TObject *Sender);

private:	// User declarations
	cellTp Cells[10][52];
public:		// User declarations
	__fastcall TFormPartio(TComponent* Owner);
	void __fastcall Partio(kilptietue *kilp, int d);
	kilptietue Kilp;
	int DKilp;
	int nJas;
	emittp Em[5];
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPartio *FormPartio;
//---------------------------------------------------------------------------
#endif
