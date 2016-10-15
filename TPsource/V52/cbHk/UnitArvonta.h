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

#ifndef UnitArvontaH
#define UnitArvontaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "HkDeclare.h"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TFormArvonta : public TForm
{
__published:	// IDE-managed Components
	TListBox *LBSarjat;
	TLabel *Label2;
	TLabel *Label6;
	TListBox *LBSarjaVal;
	TButton *BtnLisKaikki;
	TButton *BtnLisValitut;
	TButton *BtnPoisValitut;
	TButton *BtnPoisKaikki;
	TGroupBox *GroupBox2;
	TButton *Button2;
	TButton *Button1;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TRadioGroup *RGToiminto;
	TGroupBox *GroupBox4;
	TLabel *Label1;
	TLabel *Label3;
	TComboBox *CBPaiva;
	TComboBox *CB2pv;
	TRadioGroup *RGPoissa;
	TRadioGroup *RGSeura;
	TButton *BtnToteuta;
	TRadioGroup *RGNumToiminto;
	TRadioGroup *RGRajaus;
	TLabel *LbNumValmis;
	TButton *BtnNumeroi;
	TGroupBox *GBUusiArv;
	TLabel *Label4;
	TEdit *EditAlku;
	TLabel *Label5;
	TEdit *EditLoppu;
	TGroupBox *GroupBox3;
	TComboBox *CBNumPaiva;
	TRadioGroup *RGNumPaivat;
	TBitBtn *BitBtn1;
	TTabSheet *TabSheet3;
	TEdit *Jaettava;
	TLabel *Label7;
	TLabel *Label8;
	TButton *BtnJaettava;
	TButton *BtnMuodostettavat;
	TRadioGroup *RGjakotapa;
	TButton *BtnJaa;
	TCheckBox *CBoxLajat;
	TStringGrid *SGMuodostettavat;
	TEdit *EdtArvottava;
	TLabel *Label10;
	TComboBox *CBJakoVaiheet;
	TLabel *LblJakoVaiheet;
	TPanel *Panel1;
	TCheckBox *CBOhitaVakantit;
	TPanel *Panel2;
	TEdit *EdMuutos;
	TLabel *Label9;
	TGroupBox *GBRValNum;
	TRadioGroup *RGPuuttRNum;
	TRadioGroup *RGPoissaRNum;
	TGroupBox *GBRValArv;
	TRadioGroup *RGPuuttRArv;
	TRadioGroup *RGPoissaRArv;
	TComboBox *CBRankipisteet;
	TLabel *Label11;
	TButton *Button3;
	TRadioGroup *RGRajaustapa;
	TEdit *EdtSprLisa;
	TLabel *LblSprLisa;
	TButton *BtnLisaaLhtSarjat;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall BtnToteutaClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall BtnLisKaikkiClick(TObject *Sender);
	void __fastcall BtnPoisKaikkiClick(TObject *Sender);
	void __fastcall BtnLisValitutClick(TObject *Sender);
	void __fastcall BtnPoisValitutClick(TObject *Sender);
	void __fastcall RGSeuraClick(TObject *Sender);
	void __fastcall BtnNumeroiClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BtnJaettavaClick(TObject *Sender);
	void __fastcall BtnMuodostettavatClick(TObject *Sender);
	void __fastcall BtnJaaClick(TObject *Sender);
	void __fastcall SGMuodostettavatExit(TObject *Sender);
	void __fastcall SGMuodostettavatSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
	void __fastcall RGjakotapaClick(TObject *Sender);
	void __fastcall LBSarjatDblClick(TObject *Sender);
	void __fastcall LBSarjaValDblClick(TObject *Sender);
	void __fastcall RGPoissaClick(TObject *Sender);
	void __fastcall CBPaivaChange(TObject *Sender);
	void __fastcall CBNumPaivaChange(TObject *Sender);
	void __fastcall RGToimintoClick(TObject *Sender);
	void __fastcall RGNumToimintoClick(TObject *Sender);
	void __fastcall TabSheet1Enter(TObject *Sender);
	void __fastcall TabSheet2Enter(TObject *Sender);
	void __fastcall RGPuuttRNumClick(TObject *Sender);
	void __fastcall RGPoissaRNumClick(TObject *Sender);
	void __fastcall RGPuuttRArvClick(TObject *Sender);
	void __fastcall RGPoissaRArvClick(TObject *Sender);
	void __fastcall CBRankipisteetClick(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall RGNumPaivatClick(TObject *Sender);
	void __fastcall BtnLisaaLhtSarjatClick(TObject *Sender);

private:	// User declarations
	void NaytaSarjavalinnat(void);
	int __fastcall SeurPos(kilptietue *kilp, int Jrj, int Srj, int Ed, int ipv);
	bool SarjaValinnat[MAXSARJALUKU];
	void __fastcall ValmisteleJako(int jsrj, int nn, int *msrj, int n, int *nsrj, wchar_t tapa);
	void __fastcall ToteutaJako(int jsrj, int nn, int *msrj, int n, int *nsrj, wchar_t tapa);
	void JaaSeuratKoossa(int jsrj, int *msrj, int n, int *nsrj);
	bool __fastcall TarkVakantit(void);
	int SrjRow;
	int SrjValRow;
	int RPistekentta;
	bool RYlimmasta;
	int RPoissa;
	int RPuuttuu;
	bool OhitaVakantit;
	int SprLisa;
public:		// User declarations
	bool SarjaValittu(int Srj);
	__fastcall TFormArvonta(TComponent* Owner);
   wchar_t Kohde[30];
};
//---------------------------------------------------------------------------
extern PACKAGE TFormArvonta *FormArvonta;
//---------------------------------------------------------------------------
#endif
