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

#ifndef LoppuKilpUnitH
#define LoppuKilpUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Mask.hpp>
#include <Dialogs.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TLoppuKilpForm : public TForm
{
__published:	// IDE-managed Components
	TListBox *ListSarjat;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *Kars1;
	TEdit *KTls1;
	TEdit *KIlm1;
	TEdit *Alkm1;
	TEdit *Kars2;
	TEdit *KTls2;
	TEdit *KIlm2;
	TEdit *Alkm2;
	TEdit *Kars10;
	TEdit *KTls10;
	TEdit *KIlm10;
	TEdit *Alkm10;
	TEdit *Kars3;
	TEdit *KTls3;
	TEdit *KIlm3;
	TEdit *Alkm3;
	TEdit *Kars4;
	TEdit *KTls4;
	TEdit *KIlm4;
	TEdit *Alkm4;
	TEdit *Kars5;
	TEdit *KTls5;
	TEdit *KIlm5;
	TEdit *Alkm5;
	TEdit *Kars6;
	TEdit *KTls6;
	TEdit *KIlm6;
	TEdit *Alkm6;
	TEdit *Kars7;
	TEdit *KTls7;
	TEdit *KIlm7;
	TEdit *Alkm7;
	TEdit *Kars8;
	TEdit *KTls8;
	TEdit *KIlm8;
	TEdit *Alkm8;
	TEdit *Kars9;
	TEdit *KTls9;
	TEdit *KIlm9;
	TEdit *Alkm9;
	TButton *BtnLisa1;
	TButton *BtnPois1;
	TButton *BtnPois2;
	TButton *BtnLisa2;
	TButton *BtnPois3;
	TButton *BtnLisa3;
	TButton *BtnPois4;
	TButton *BtnLisa4;
	TButton *BtnPois5;
	TButton *BtnLisa5;
	TButton *BtnPois6;
	TButton *BtnLisa6;
	TButton *BtnPois7;
	TButton *BtnLisa7;
	TButton *BtnPois8;
	TButton *BtnLisa8;
	TButton *BtnPois9;
	TButton *BtnLisa9;
	TButton *BtnPois10;
	TButton *BtnLisa10;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label5;
	TButton *BtnToteuta;
	TButton *Button23;
	TEdit *Blkm1;
	TEdit *Blkm2;
	TEdit *Blkm3;
	TEdit *Blkm4;
	TEdit *Blkm5;
	TEdit *Blkm6;
	TEdit *Blkm7;
	TEdit *Blkm8;
	TEdit *Blkm9;
	TEdit *Blkm10;
	TLabel *Label9;
	TEdit *Clkm1;
	TEdit *Clkm2;
	TEdit *Clkm3;
	TEdit *Clkm4;
	TEdit *Clkm5;
	TEdit *Clkm6;
	TEdit *Clkm7;
	TEdit *Clkm8;
	TEdit *Clkm9;
	TEdit *Clkm10;
	TLabel *Label10;
	TButton *Button1;
	TBitBtn *BitBtn1;
	TComboBox *CBAfin;
	TComboBox *CBBfin;
	TComboBox *CBCfin;
	TComboBox *CBDfin;
	TComboBox *CBEfin;
	TEdit *Alkm0;
	TEdit *Blkm0;
	TEdit *Clkm0;
	TEdit *Dlkm0;
	TEdit *Elkm0;
	TCheckBox *CBlaA;
	TCheckBox *CBlaB;
	TCheckBox *CBlaC;
	TCheckBox *CBlaD;
	TCheckBox *CBlaE;
	TEdit *EnsLahtoA;
	TEdit *EnsLahtoB;
	TEdit *EnsLahtoC;
	TEdit *EnsLahtoD;
	TEdit *EnsLahtoE;
	TEdit *LahtoValiA;
	TEdit *LahtoValiB;
	TEdit *LahtoValiC;
	TEdit *LahtoValiD;
	TEdit *LahtoValiE;
	TCheckBox *LoputA;
	TCheckBox *LoputB;
	TCheckBox *LoputC;
	TCheckBox *LoputD;
	TCheckBox *LoputE;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label15;
	TLabel *Label16;
	TEdit *Aalku;
	TEdit *Balku;
	TEdit *Calku;
	TEdit *Dalku;
	TEdit *Ealku;
	TEdit *AaltoA;
	TEdit *AaltoB;
	TEdit *AaltoC;
	TEdit *AaltoD;
	TEdit *AaltoE;
	TComboBox *CBPoimiA;
	TComboBox *CBPoimiB;
	TComboBox *CBPoimiC;
	TComboBox *CBPoimiD;
	TComboBox *CBPoimiE;
	TCheckBox *CBtakajoA;
	TCheckBox *CBtakajoB;
	TCheckBox *CBtakajoC;
	TCheckBox *CBtakajoD;
	TCheckBox *CBtakajoE;
	TLabel *Label17;
	TLabel *Label18;
	TLabel *Label19;
	TButton *Button2;
	TLabel *Label20;
	TEdit *Dlkm1;
	TEdit *Dlkm2;
	TEdit *Dlkm3;
	TEdit *Dlkm4;
	TEdit *Dlkm5;
	TEdit *Dlkm6;
	TEdit *Dlkm7;
	TEdit *Dlkm8;
	TEdit *Dlkm9;
	TEdit *Dlkm10;
	TLabel *Label21;
	TEdit *Elkm1;
	TEdit *Elkm2;
	TEdit *Elkm3;
	TEdit *Elkm4;
	TEdit *Elkm5;
	TEdit *Elkm6;
	TEdit *Elkm7;
	TEdit *Elkm8;
	TEdit *Elkm9;
	TEdit *Elkm10;
	TOpenDialog *OpenDialog1;
	TLabel *Label22;
	TLabel *Label23;
	TMainMenu *MainMenu1;
	TMenuItem *iedosto1;
	TMenuItem *Lisskriptitiedostoon1;
	TSaveDialog *SaveDialog1;
	TMenuItem *Sulje1;
	TButton *BtnPuraSeurat;
	TEdit *EdtKnoLisa;
	TLabel *Label24;
	TComboBox *CBMerkEilah;
	TLabel *Label25;
	void __fastcall BtnLisa1Click(TObject *Sender);
	void __fastcall BtnLisa2Click(TObject *Sender);
	void __fastcall BtnLisa3Click(TObject *Sender);
	void __fastcall BtnLisa4Click(TObject *Sender);
	void __fastcall BtnLisa5Click(TObject *Sender);
	void __fastcall BtnLisa6Click(TObject *Sender);
	void __fastcall BtnLisa7Click(TObject *Sender);
	void __fastcall BtnLisa8Click(TObject *Sender);
	void __fastcall BtnLisa9Click(TObject *Sender);
	void __fastcall BtnLisa10Click(TObject *Sender);
	void __fastcall BtnPois1Click(TObject *Sender);
	void __fastcall BtnPois2Click(TObject *Sender);
	void __fastcall BtnPois3Click(TObject *Sender);
	void __fastcall BtnPois4Click(TObject *Sender);
	void __fastcall BtnPois5Click(TObject *Sender);
	void __fastcall BtnPois6Click(TObject *Sender);
	void __fastcall BtnPois7Click(TObject *Sender);
	void __fastcall BtnPois8Click(TObject *Sender);
	void __fastcall BtnPois9Click(TObject *Sender);
	void __fastcall BtnPois10Click(TObject *Sender);
	void __fastcall Alkm0Change(TObject *Sender);
	void __fastcall Blkm0Change(TObject *Sender);
	void __fastcall Clkm0Change(TObject *Sender);
	void __fastcall Dlkm0Change(TObject *Sender);
	void __fastcall Elkm0Change(TObject *Sender);
	void __fastcall AalkuChange(TObject *Sender);
	void __fastcall BalkuChange(TObject *Sender);
	void __fastcall CalkuChange(TObject *Sender);
	void __fastcall DalkuChange(TObject *Sender);
	void __fastcall EalkuChange(TObject *Sender);
	void __fastcall CBAfinChange(TObject *Sender);
	void __fastcall CBBfinChange(TObject *Sender);
	void __fastcall CBCfinChange(TObject *Sender);
	void __fastcall CBDfinChange(TObject *Sender);
	void __fastcall CBEfinChange(TObject *Sender);
	void __fastcall EnsLahtoAExit(TObject *Sender);
	void __fastcall EnsLahtoBExit(TObject *Sender);
	void __fastcall EnsLahtoCExit(TObject *Sender);
	void __fastcall EnsLahtoDExit(TObject *Sender);
	void __fastcall EnsLahtoEExit(TObject *Sender);
	void __fastcall LahtoValiAExit(TObject *Sender);
	void __fastcall LahtoValiBExit(TObject *Sender);
	void __fastcall LahtoValiCExit(TObject *Sender);
	void __fastcall LahtoValiDExit(TObject *Sender);
	void __fastcall LahtoValiEExit(TObject *Sender);
	void __fastcall LoputAClick(TObject *Sender);
	void __fastcall LoputBClick(TObject *Sender);
	void __fastcall LoputCClick(TObject *Sender);
	void __fastcall LoputDClick(TObject *Sender);
	void __fastcall LoputEClick(TObject *Sender);
	void __fastcall CBlaAClick(TObject *Sender);
	void __fastcall CBlaBClick(TObject *Sender);
	void __fastcall CBlaCClick(TObject *Sender);
	void __fastcall CBlaDClick(TObject *Sender);
	void __fastcall CBlaEClick(TObject *Sender);
	void __fastcall Button23Click(TObject *Sender);
	void __fastcall BtnToteutaClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CBPoimiClick(TObject *Sender);
	void __fastcall Lisskriptitiedostoon1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Sulje1Click(TObject *Sender);
	void __fastcall BtnPuraSeuratClick(TObject *Sender);
private:	// User declarations
	TEdit *Lkm[5][10];
	TEdit *Kars[10];
	TEdit *KTls[10];
	TEdit *KIlm[10];
	TComboBox *CBfin[5];
	TCheckBox *CBla[5];
	TCheckBox *Loput[5];
	TEdit *Lkm0[5];
	TEdit *LahtoVali[5];
	TEdit *EnsLahto[5];
	TEdit *Alku[5];
	TEdit *Aalto[5];
	TCheckBox *CBTakajo[5];
	void __fastcall LisaaKarsSrj(int i);
	void __fastcall PoisKarsSrj(int i);
	void __fastcall EnsLahtoExit(int ifin);
	void __fastcall CBfinChange(int ifin);
	void __fastcall Lkm0Change(int ifin);
	void __fastcall LoputClick(int ifin);
	void __fastcall AlkuChange(int ifin);
	void __fastcall LahtoValiExit(int ifin);
	void __fastcall Tyhjenna(void);
public:		// User declarations
	__fastcall TLoppuKilpForm(TComponent* Owner);
	void __fastcall setkarslkm(int i);
   wchar_t Kohde[30];
	TComboBox *CBPoimi[5];
};
//---------------------------------------------------------------------------
extern PACKAGE TLoppuKilpForm *LoppuKilpForm;
//---------------------------------------------------------------------------
#endif
