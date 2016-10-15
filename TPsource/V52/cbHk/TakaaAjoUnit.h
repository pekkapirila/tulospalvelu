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

#ifndef TakaaAjoUnitH
#define TakaaAjoUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TTakaaAjoForm : public TForm
{
__published:	// IDE-managed Components
	TComboBox *CBAfin;
	TLabel *Label1;
	TEdit *EditMukaan;
	TEdit *Aalku;
	TButton *Button22;
	TButton *Button23;
	TMaskEdit *EnsLahtoA;
	TMaskEdit *LahtoValiA;
	TRadioGroup *RGtakajo;
	TRadioGroup *RGnumeroi;
	TEdit *EditKesto;
	TEdit *EditIlm;
	TMemo *Memo7;
	TEdit *EditTls;
	TMemo *Memo8;
	TEdit *EditHyl;
	TMemo *Memo9;
	TEdit *EditKesk;
	TMemo *Memo10;
	TEdit *EditTAlkm;
	TBitBtn *BitBtn1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TGroupBox *GroupBox1;
	TRadioGroup *RGPuolita;
	TEdit *EdKerroin;
	TEdit *EdJakaja;
	TLabel *Label8;
	TLabel *Label9;
	TMemo *Memo1;
	TEdit *EdtLasna;
	TRadioGroup *RGPois;
	TLabel *Label10;
	TEdit *EdNoLisa;
	TCheckBox *CBVahKarki;
	TLabel *Label11;
	TEdit *EdtAalto;
	TCheckBox *CBTallEroSakkona;
	void __fastcall EditMukaanChange(TObject *Sender);
	void __fastcall AalkuChange(TObject *Sender);
	void __fastcall CBAfinChange(TObject *Sender);
	void __fastcall EnsLahtoAExit(TObject *Sender);
	void __fastcall LahtoValiAExit(TObject *Sender);
	void __fastcall Button23Click(TObject *Sender);
	void __fastcall Button22Click(TObject *Sender);
	void __fastcall EditKestoExit(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall RGtakajoClick(TObject *Sender);
	void __fastcall RGPuolitaClick(TObject *Sender);
	void __fastcall RGnumeroiClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall EdtAaltoExit(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TTakaaAjoForm(TComponent* Owner);
	__fastcall ~TTakaaAjoForm(void);
	int __fastcall TakaaAjoLkm(void);
   wchar_t Kohde[30];
};
//---------------------------------------------------------------------------
extern PACKAGE TTakaaAjoForm *TakaaAjoForm;
//---------------------------------------------------------------------------
#endif
