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

#ifndef UnitJjVaihdotH
#define UnitJjVaihdotH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TFormJjVaihdot : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *SG1;
	TButton *BtnEtuSuku;
	TButton *BtnToteuta;
	TCheckBox *CBEmitkoodit;
	TButton *BtnJaaNimi;
	TButton *BtnPeruuta;
	TLabel *Label1;
	TCheckBox *CBHajonnat;
	TCheckBox *CBLisenssi;
	TButton *BtnIsoalku;
	void __fastcall BtnPeruutaClick(TObject *Sender);
	void __fastcall BtnToteutaClick(TObject *Sender);
	void __fastcall BtnEtuSukuClick(TObject *Sender);
	void __fastcall BtnJaaNimiClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall BtnIsoalkuClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormJjVaihdot(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormJjVaihdot *FormJjVaihdot;
//---------------------------------------------------------------------------
#endif
