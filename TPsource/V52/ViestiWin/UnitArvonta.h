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
#include "VDeclare.h"
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
	TTabSheet *TabSheet2;
	TRadioGroup *RGNumToiminto;
	TLabel *LbNumValmis;
	TButton *BtnNumeroi;
	TBitBtn *BitBtn1;
	TPanel *Panel1;
	TCheckBox *CBOhitaVakantit;
	TPanel *Panel2;
	TEdit *EdMuutos;
	TLabel *Label9;
	TTabSheet *TabSheet1;
	TButton *BtnLahtoajat;
	TLabel *Label1;
	TLabel *Label3;
	TLabel *LbAjatValmis;
	TRadioGroup *RGAikatoiminto;
	TEdit *EdtAikasiirto;
	TLabel *LblAikaSiirto;
	TButton *Button3;
	TComboBox *CBLAOsuudet;
	TLabel *Label4;
	TLabel *Label5;
	TButton *BtnFISpisteet;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall BtnLisKaikkiClick(TObject *Sender);
	void __fastcall BtnPoisKaikkiClick(TObject *Sender);
	void __fastcall BtnLisValitutClick(TObject *Sender);
	void __fastcall BtnPoisValitutClick(TObject *Sender);
	void __fastcall BtnNumeroiClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall LBSarjatDblClick(TObject *Sender);
	void __fastcall LBSarjaValDblClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall BtnLahtoajatClick(TObject *Sender);
	void __fastcall RGAikatoimintoClick(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall BtnFISpisteetClick(TObject *Sender);
	void __fastcall RGNumToimintoClick(TObject *Sender);

private:	// User declarations
	void NaytaSarjavalinnat(void);
	int __fastcall SeurPos(kilptietue *kilp, int Jrj, int Srj, int Ed);
	int __fastcall varmista_vapaa(int kno, int d);
	bool SarjaValinnat[MAXSARJALUKU];
	int SrjRow;
	int SrjValRow;
	int RPoissa;
	int RPuuttuu;
public:		// User declarations
	bool SarjaValittu(int Srj);
	__fastcall TFormArvonta(TComponent* Owner);
   wchar_t Kohde[30];
};
//---------------------------------------------------------------------------
extern PACKAGE TFormArvonta *FormArvonta;
//---------------------------------------------------------------------------
#endif
