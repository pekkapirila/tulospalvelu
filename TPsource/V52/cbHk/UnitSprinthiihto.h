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

#ifndef UnitSprinthiihtoH
#define UnitSprinthiihtoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TFormSprinthiihto : public TForm
{
__published:	// IDE-managed Components
	TRadioGroup *RGVaihe;
	TComboBox *CBL1;
	TLabel *Label1;
	TComboBox *CBL2;
	TComboBox *CBL3;
	TComboBox *CBL4;
	TComboBox *CBL5;
	TLabel *Label2;
	TComboBox *CBM1;
	TComboBox *CBM2;
	TComboBox *CBM3;
	TComboBox *CBM5;
	TButton *BtnToteuta;
	TComboBox *CBM4;
	TGroupBox *GBMuu;
	TLabel *Label4;
	TEdit *EdKoko;
	TLabel *LblAjalla;
	TEdit *EdAjalla;
	TLabel *Label3;
	TEdit *EdLkm;
	TGroupBox *GBPohjustus;
	TLabel *Label6;
	TEdit *EdPohjLisays;
	TComboBox *CBPohjVaiheet;
	TLabel *Label7;
	TLabel *Label8;
	TGroupBox *GBPalautus;
	TButton *BtnPohjusta;
	TLabel *Label9;
	TLabel *Label10;
	TEdit *EdPalLisays;
	TButton *BtnPalauta;
	TBitBtn *BitBtn1;
	TEdit *EdtNumAlku;
	TLabel *LblNumAlku;
	TButton *Button4;
	TEdit *EdtMuodVaihe;
	TLabel *Label5;
	TLabel *Label11;
	TEdit *EdLahto;
	TLabel *Label12;
	TCheckBox *CBmerkPoissa;
	TCheckBox *CBpoistaQual;
	TCheckBox *CBpalSarja;
	TCheckBox *CBpalBib;
	TCheckBox *CBautoPal;
	TLabel *Label13;
	TComboBox *CBPerussarja;
	TLabel *Label14;
	TEdit *EdTul1;
	TEdit *EdTul2;
	TEdit *EdTul3;
	TEdit *EdTul4;
	TEdit *EdTul5;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BtnToteutaClick(TObject *Sender);
	void __fastcall RGVaiheClick(TObject *Sender);
	void __fastcall BtnPohjustaClick(TObject *Sender);
	void __fastcall BtnPalautaClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall CBM1Change(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall EdtMuodVaiheExit(TObject *Sender);
	void __fastcall CBL1Change(TObject *Sender);
	void __fastcall CBPerussarjaChange(TObject *Sender);
	void __fastcall CBLChange(TObject *Sender);
private:	// User declarations
	TComboBox *CBL[5];
	TComboBox *CBM[5];
	TEdit *EdTul[5];
	void __fastcall Karsinnasta(bool suoraan);
	void __fastcall Semi(void);
	void __fastcall Finaalit(void);
	void __fastcall HaeSprintsarjat(int *lsrj, int *nl, int *msrj, int *ml);
public:		// User declarations
	__fastcall TFormSprinthiihto(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSprinthiihto *FormSprinthiihto;
//---------------------------------------------------------------------------
#endif
