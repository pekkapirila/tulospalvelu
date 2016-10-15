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

#ifndef TulostusUnit2H
#define TulostusUnit2H
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Menus.hpp>
#include <Vcl.ComCtrls.hpp>
#include "VDeclare.h"
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TTulosteForm : public TForm
{
__published:	// IDE-managed Components
   TSaveDialog *SaveDialog1;
   TButton *PoistuBtn;
   TButton *ButtonTulosta;
   TMainMenu *MainMenu1;
   TMenuItem *Tiedosto1;
   TMenuItem *Sulje1;
   TMenuItem *Muotoilu1;
   TMenuItem *Luetaimuokkaa1;
	TPrinterSetupDialog *PrinterSetupDialog1;
	TPageControl *PageControlKohde;
	TTabSheet *TabSheetKirjoitin;
	TTabSheet *TabSheetTiedosto;
	TEdit *TiedNimiEdit;
	TButton *ButtonTiedVal;
	TComboBox *KirjoitinVal;
	TPageControl *PageControlSisalto;
	TTabSheet *TabSheetTulokset;
	TGroupBox *GroupBox1;
	TComboBox *CBRajVal;
	TLabel *LblSijat;
	TEdit *Alaraja;
	TEdit *Ylaraja;
	TLabel *LblSjVali;
	TComboBox *CBJarj;
	TLabel *Label5;
	TGroupBox *GroupBox2;
	TComboBox *CBSisalto;
	TComboBox *PisteVal;
	TLabel *LblPistevalinta;
	TListBox *LBSarjat;
	TLabel *Label6;
	TLabel *Label7;
	TButton *BtnLisKaikki;
	TButton *BtnLisValitut;
	TButton *BtnPoisValitut;
	TButton *BtnPoisKaikki;
	TListBox *LBSarjaVal;
	TComboBox *CBTiedLaji;
	TLabel *Tiedostotyyppi;
	TLabel *Label2;
	TComboBox *CBMerkisto;
	TLabel *Label8;
	TComboBox *CBErotin;
	TGroupBox *GroupBox6;
	TCheckBox *TulYV;
	TComboBox *CBYv;
	TLabel *Sisältö;
	TGroupBox *GBhtml;
	TRadioGroup *RGSarjatErikseen;
	TButton *Button1;
	TMemo *Memo1;
	TMenuItem *Help1;
	TOpenDialog *OpenDialog1;
	TGroupBox *GBFISMaar;
	TEdit *EdtFISMaar;
	TButton *Button2;
	TMenuItem *Vakiotulosteet1;
	TMenuItem *Kaikkilopettaneethtmltiedostoon1;
	TMenuItem *Kaikkilopettaneetkirjoittimelle1;
	TMenuItem *JuoksujHtml;
	TCheckBox *CBKaikkiSijoitukset;
	TCheckBox *CBSarjayhd;
	TMenuItem *Kirjoittimenasetukset1;
	TCheckBox *CBUusiSivu;
	TMenuItem *Automaattinentulostus1;
	TCheckBox *CBViimLeima;
	TMenuItem *Automaattinentiedostotulostus1;
	TCheckBox *CBLisaTeksti;
	TBitBtn *BitBtn1;
	TRadioGroup *RGMoniOs;
	TComboBox *CBOsuusVal;
	TLabel *Label1;
	TCheckBox *CBBadge;
	TMenuItem *YksiOsuusKirjoittimelle;
	TMenuItem *JuoksujKirj;
	TMenuItem *YksiOsuusHtml;
	TCheckBox *CBJoukkHyv;
	TCheckBox *CBTagPit;
	TMenuItem *Englanninkielisetotsikot1;
	TComboBox *CBxmlStand;
	TMenuItem *Listekstit1;
	TGroupBox *GBJatkTxt;
	TCheckBox *CBVaihtoajat;
	TCheckBox *CBOsuusajat;
	TCheckBox *CBVaihtosijat;
	TCheckBox *CBOsuussijat;
	TGroupBox *GBSHLVal;
	TCheckBox *CBSHLohita;
	TCheckBox *CBNaytaheti;
   void __fastcall ButtonTiedValClick(TObject *Sender);
   void __fastcall PoistuBtnClick(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall ButtonTulostaClick(TObject *Sender);
   void __fastcall Luetaimuokkaa1Click(TObject *Sender);
   void __fastcall Sulje1Click(TObject *Sender);
	void __fastcall CBRajValChange(TObject *Sender);
	void __fastcall BtnLisValitutClick(TObject *Sender);
	void __fastcall BtnPoisValitutClick(TObject *Sender);
	void __fastcall BtnLisKaikkiClick(TObject *Sender);
	void __fastcall BtnPoisKaikkiClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CBSisaltoChange(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Help1Click(TObject *Sender);
	void __fastcall CBTiedLajiChange(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall TabSheetKirjoitinEnter(TObject *Sender);
	void __fastcall Kaikkilopettaneethtmltiedostoon1Click(TObject *Sender);
	void __fastcall Kaikkilopettaneetkirjoittimelle1Click(TObject *Sender);
	void __fastcall JuoksujHtmlClick(TObject *Sender);
	void __fastcall CBSarjayhdClick(TObject *Sender);
	void __fastcall PageControlKohdeChange(TObject *Sender);
	void __fastcall PageControlSisaltoChange(TObject *Sender);
	void __fastcall Kirjoittimenasetukset1Click(TObject *Sender);
	void __fastcall LBSarjatDblClick(TObject *Sender);
	void __fastcall LBSarjaValDblClick(TObject *Sender);
	void __fastcall Automaattinentulostus1Click(TObject *Sender);
	void __fastcall Automaattinentiedostotulostus1Click(TObject *Sender);
	void __fastcall CBLisaTekstiClick(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall CBOsuusValClick(TObject *Sender);
	void __fastcall JuoksujKirjClick(TObject *Sender);
	void __fastcall YksiOsuusHtmlClick(TObject *Sender);
	void __fastcall YksiOsuusKirjoittimelleClick(TObject *Sender);
	void __fastcall CBJoukkHyvClick(TObject *Sender);
	void __fastcall Englanninkielisetotsikot1Click(TObject *Sender);
	void __fastcall Listekstit1Click(TObject *Sender);
	void __fastcall PisteValChange(TObject *Sender);
	void __fastcall CBVaihtoajatClick(TObject *Sender);
	void __fastcall CBOsuusajatClick(TObject *Sender);
	void __fastcall CBVaihtosijatClick(TObject *Sender);
	void __fastcall CBOsuussijatClick(TObject *Sender);
	void __fastcall CBJarjChange(TObject *Sender);
private:	// User declarations
	void NaytaSarjavalinnat(void);
	void __fastcall PaivitaTulossivu(TObject *Sender);
	bool SarjaValinnat[MAXSARJALUKU+MAXYHD];
	wchar_t TiedDefault[6];
	int SrjLkm;
	bool Tyyppivaihdettu;
public:		// User declarations
	void enumPrinters(TComboBox *CBkirj);
	bool SarjaValittu(int Srj);
   __fastcall TTulosteForm(TComponent* Owner);
   wchar_t Kohde[30];
   wchar_t LisaFile[200];
   wchar_t SHLkilpailu[20];
   wchar_t SHLSarja[MAXSARJALUKU+MAXYHD][12];
   wchar_t SHLMatka[MAXSARJALUKU+MAXYHD][MAXOSUUSLUKU][10];
   wchar_t SHLTyyli[MAXSARJALUKU+MAXYHD][MAXOSUUSLUKU][2];
};
//---------------------------------------------------------------------------
extern PACKAGE TTulosteForm *TulosteForm;
//---------------------------------------------------------------------------
#endif
