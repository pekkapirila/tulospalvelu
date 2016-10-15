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

#ifndef UnitLahestyjatH
#define UnitLahestyjatH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>

#include "VDef.h"
#include <Vcl.ExtCtrls.hpp>
#define NLAHEST 4
#define MAXLAHCOL 5

class lahIkkParamClass {
public:
	int FontSize;
	int ColW[MAXLAHCOL];
	int ColOn[MAXLAHCOL];
	lahIkkParamClass(void);
//	void operator=(lahIkkParamClass&);
	int writeParams(TextFl *outfl, int level);
	int readParams(xml_node *node, int *inode, int nnode);
};

typedef cellTp rowTplah[MAXLAHCOL];

//---------------------------------------------------------------------------
class TFormLahestyjat : public TForm
{
__published:	// IDE-managed Components
	TDrawGrid *DG1;
	TLabel *Label1;
	TLabel *Label2;
	TComboBox *CBSarja;
	TLabel *Label3;
	TComboBox *CBOsuus;
	TLabel *Label4;
	TComboBox *CBPiste;
	TRadioGroup *RGFont;
	void __fastcall DG1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall RGFontClick(TObject *Sender);
	void __fastcall CBSarjaClick(TObject *Sender);
	void __fastcall CBOsuusChange(TObject *Sender);
	void __fastcall CBPisteClick(TObject *Sender);

private:	// User declarations
	rowTplah *Cells;
	int CellRows;
public:		// User declarations
	__fastcall TFormLahestyjat(TComponent* Owner);
	void __fastcall Paivita(TObject *Sender);
	lahIkkParamClass IkkParam;
	int applyParams(void);
	int Sarja;
	int Osuus;
	int Piste;
	int maxSeur;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormLahestyjat *FormLahestyjat[NLAHEST];
//---------------------------------------------------------------------------
#endif
