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
#include "HkDeclare.h"
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TFormKilpMaaritys : public TForm
{
__published:	// IDE-managed Components
	TEdit *EditKilpailu;
	TLabel *Label1;
	TPanel *Panel1;
	TLabel *Label2;
	TEdit *EditTiedPv;
	TLabel *Label3;
	TEdit *EditTiedVa;
	TLabel *Label4;
	TRadioGroup *RGlaji;
	TRadioGroup *RGtasmennys;
	TEdit *EditPv;
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
	TRadioGroup *RGBib;
	TRadioGroup *RGSarja;
	TRadioGroup *RGSeura;
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
	TComboBox *CBTulTarkkuus;
	TLabel *Label19;
	TComboBox *CBVaTarkkuus;
	TLabel *Label20;
	TComboBox *CBTakAjoPyor;
	TLabel *Label21;
	TCheckBox *CBLahtoportti;
	TButton *BtnVaiheMaar;
	TLabel *Label22;
	TComboBox *CBYhtTulos;
	TCheckBox *CB2badge;
	TCheckBox *CBIntPenth;
	TMainMenu *MainMenu1;
	TMenuItem *Lismrityksi1;
	TMenuItem *Kvlisenssinumero1;
	TEdit *EdtMaxSakko;
	TLabel *Label23;
	void __fastcall ButtonOKClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall BtnLueSrjClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall EditTiedPvExit(TObject *Sender);
	void __fastcall EditTiedVaExit(TObject *Sender);
	void __fastcall RGlajiClick(TObject *Sender);
	void __fastcall RGtasmennysClick(TObject *Sender);
	void __fastcall RGMuuLajiClick(TObject *Sender);
	void __fastcall EditPvExit(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall BtnVaiheMaarClick(TObject *Sender);
	void __fastcall EditSarjojaChange(TObject *Sender);
	void __fastcall CBLahtoporttiClick(TObject *Sender);
	void __fastcall Kvlisenssinumero1Click(TObject *Sender);
private:	// User declarations
	TComboBox *CBVaihe[5];
	int hyvaksyMuutokset;
	kilpparamtp kilpparam0;
	pvparamtp pvparam0[N_PV];
	bool inInit;
public:		// User declarations
	__fastcall TFormKilpMaaritys(TComponent* Owner);
	void __fastcall NaytaArvot(void);
	void __fastcall NaytaTarkkuudet(void);
	void __fastcall NaytaVaiheet(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormKilpMaaritys *FormKilpMaaritys;
//---------------------------------------------------------------------------
#endif
