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

#ifndef UnitSeuravalH
#define UnitSeuravalH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "HkDef.h"
//---------------------------------------------------------------------------
class TFormSeuraVal : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label6;
	TListBox *LBSeurat;
	TButton *BtnLisKaikki;
	TButton *BtnLisValitut;
	TButton *BtnPoisValitut;
	TButton *BtnPoisKaikki;
	TListBox *LBSeuraVal;
	TLabel *Label7;
	TLabel *Label1;
	TLabel *Label2;
	TButton *Button1;
	TLabel *Label3;
	TEdit *EdtParhaat;
	TCheckBox *CBMaa;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall BtnLisKaikkiClick(TObject *Sender);
	void __fastcall BtnLisValitutClick(TObject *Sender);
	void __fastcall BtnPoisValitutClick(TObject *Sender);
	void __fastcall BtnPoisKaikkiClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall LBSeuraValDblClick(TObject *Sender);
	void __fastcall LBSeuratDblClick(TObject *Sender);
	void __fastcall CBMaaClick(TObject *Sender);
private:	// User declarations
	int haeseura(wchar_t *snimi);
	void NaytaSeuravalinnat(void);
public:		// User declarations
	__fastcall TFormSeuraVal(TComponent* Owner);
	wchar_t (*seuralista)[LSEURA+5];
	int *SeuraValinnat;
	int sralaji;
	int nsra;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSeuraVal *FormSeuraVal;
//---------------------------------------------------------------------------
#endif
