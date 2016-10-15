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

#ifndef UnitKilpMaaritysH
#define UnitKilpMaaritysH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Buttons.hpp>
#include "VDeclare.h"
//---------------------------------------------------------------------------
class TFormKilpMaaritys : public TForm
{
__published:	// IDE-managed Components
	TEdit *EditKilpailu;
	TLabel *Label1;
	TPanel *Panel1;
	TLabel *Label2;
	TEdit *EditTiedOsluku;
	TLabel *Label3;
	TEdit *EditTiedVa;
	TLabel *Label4;
	TRadioGroup *RGlaji;
	TRadioGroup *RGtasmennys;
	TEdit *EditOsluku;
	TLabel *Label5;
	TEdit *EditKoodi;
	TLabel *Label6;
	TEdit *EditVuosi;
	TLabel *Label7;
	TButton *ButtonOK;
	TButton *Button1;
	TLabel *Label8;
	TEdit *EditHakem;
	TSaveDialog *SaveDialog1;
	TButton *Button2;
	TGroupBox *GBSrj;
	TCheckBox *CBSarjat;
	TEdit *EditSarjoja;
	TLabel *Label9;
	TButton *BtnLueSrj;
	TOpenDialog *OpenDialog1;
	TBitBtn *BitBtn1;
	TEdit *EdtSakko;
	TLabel *Label10;
	TComboBox *CBTunnistin;
	TLabel *Label11;
	TPanel *PanelMuuLaji;
	TRadioGroup *RGMuuLaji;
	TLabel *Label12;
	TComboBox *CBVaihe1;
	TLabel *Label13;
	TComboBox *CBVaihe2;
	TLabel *Label14;
	TComboBox *CBVaihe3;
	TLabel *Label15;
	TComboBox *CBVaihe4;
	TLabel *Label16;
	TComboBox *CBVaihe5;
	TComboBox *CBTarkkuus;
	TLabel *Label17;
	TComboBox *CBJrjTarkkuus;
	TLabel *Label18;
	TComboBox *CBTultarkkuus;
	TLabel *Label19;
	TComboBox *CBVaTarkkuus;
	TLabel *Label20;
	TCheckBox *CBLahtoportti;
	TLabeledEdit *EdtXmlNimi;
	TLabeledEdit *EdtPvm;
	TEdit *EdtMaxRinn;
	TLabel *Label21;
	TCheckBox *CB2badge;
	TEdit *EdtOsLkm;
	TLabel *Label22;
	void __fastcall ButtonOKClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall BtnLueSrjClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall EditTiedOslukuExit(TObject *Sender);
	void __fastcall EditTiedVaExit(TObject *Sender);
	void __fastcall RGlajiClick(TObject *Sender);
	void __fastcall RGtasmennysClick(TObject *Sender);
	void __fastcall RGMuuLajiClick(TObject *Sender);
	void __fastcall EditOslukuExit(TObject *Sender);
	void __fastcall EditSarjojaExit(TObject *Sender);
private:	// User declarations
	TComboBox *CBVaihe[5];
public:		// User declarations
	__fastcall TFormKilpMaaritys(TComponent* Owner);
	void __fastcall NaytaArvot(void);
	void __fastcall NaytaVaiheet(void);
	void __fastcall AlustaKaavat(sarjatietue *Srj);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormKilpMaaritys *FormKilpMaaritys;
//---------------------------------------------------------------------------
#endif
