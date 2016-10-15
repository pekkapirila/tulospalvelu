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

#ifndef UnitSprintSarjaH
#define UnitSprintSarjaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "HkDeclare.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TFormSprintSarja : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabeledEdit *EdtVaiheLkm;
	TLabeledEdit *EdtEraLkm1;
	TLabeledEdit *EdtEraKoko1;
	TLabel *Label2;
	TLabel *Label3;
	TComboBox *CBS11;
	TComboBox *CBS12;
	TComboBox *CBS13;
	TComboBox *CBS14;
	TComboBox *CBS15;
	TComboBox *CBS16;
	TLabel *Label4;
	TLabeledEdit *EdtEraLkm2;
	TLabeledEdit *EdtEraKoko2;
	TLabel *Label5;
	TComboBox *CBS21;
	TComboBox *CBS22;
	TLabel *Label6;
	TLabeledEdit *EdtEraLkm3;
	TLabeledEdit *EdtEraKoko3;
	TLabel *Label7;
	TComboBox *CBS31;
	TComboBox *CBS32;
	TLabel *Label8;
	TButton *BtnPeruuta;
	TButton *BtnTallenna;
	TEdit *EdtPerusSarja;
	TButton *Button1;
	TBitBtn *BitBtn1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BtnPeruutaClick(TObject *Sender);
	void __fastcall BtnTallennaClick(TObject *Sender);
	void __fastcall EdtVaiheLkmExit(TObject *Sender);
	void __fastcall CBS11Change(TObject *Sender);
	void __fastcall CBS21Change(TObject *Sender);
	void __fastcall CBS31Change(TObject *Sender);
	void __fastcall EdtEraLkm1Change(TObject *Sender);
	void __fastcall EdtEraLkm2Change(TObject *Sender);
	void __fastcall EdtEraLkm3Change(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
	TComboBox *CBS[11];
public:		// User declarations
	__fastcall TFormSprintSarja(TComponent* Owner);
	int pSarja;
	sprintsarja sprSrj;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSprintSarja *FormSprintSarja;
//---------------------------------------------------------------------------
#endif
