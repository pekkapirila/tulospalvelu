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

#ifndef UnitPiirivalH
#define UnitPiirivalH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "VDef.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormPiiriVal : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label6;
	TListBox *LBPiirit;
	TButton *BtnLisKaikki;
	TButton *BtnLisValitut;
	TButton *BtnPoisValitut;
	TButton *BtnPoisKaikki;
	TListBox *LBPiiriVal;
	TLabel *Label7;
	TLabel *Label1;
	TLabel *Label2;
	TButton *Button1;
	TRadioGroup *RGPiirisijat;
	TEdit *EdtParhaat;
	TLabel *Label3;
	TButton *Button2;
	TOpenDialog *OpenDialog1;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BtnLisKaikkiClick(TObject *Sender);
	void __fastcall BtnLisValitutClick(TObject *Sender);
	void __fastcall BtnPoisValitutClick(TObject *Sender);
	void __fastcall BtnPoisKaikkiClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall LBPiiriValDblClick(TObject *Sender);
	void __fastcall LBPiiritDblClick(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
	int haepiiri(wchar_t *snimi);
	void NaytaPiirivalinnat(void);
public:		// User declarations
	__fastcall TFormPiiriVal(TComponent* Owner);
	wchar_t piirilista[200][60];
};
//---------------------------------------------------------------------------
extern PACKAGE TFormPiiriVal *FormPiiriVal;
//---------------------------------------------------------------------------
#endif
