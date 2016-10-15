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
#include "HkDeclare.h"
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
	TTabSheet *TabSheetOottajat;
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
	TComboBox *CBOoSisalto;
	TLabel *Label9;
	TGroupBox *GBLlVaihe;
	TComboBox *CBLlPaivat;
	TComboBox *CBLlJarjPv;
	TLabel *Label10;
	TLabel *Label11;
	TGroupBox *GroupBox4;
	TRadioGroup *RGLahdot;
	TEdit *EditLahdot;
	TLabel *Label12;
	TGroupBox *GroupBox5;
	TRadioGroup *RGOoSeurat;
	TRadioGroup *RGOoSra;
	TComboBox *CBSeura;
	TGroupBox *GroupBox6;
	TCheckBox *TulYV;
	TComboBox *CBYv;
	TLabel *Sisältö;
	TLabel *Label14;
	TGroupBox *GBhtml;
	TRadioGroup *RGSarjatErikseen;
	TButton *Button1;
	TMemo *Memo1;
	TMenuItem *Help1;
	TOpenDialog *OpenDialog1;
	TGroupBox *GBFISMaar;
	TEdit *EdtFISMaar;
	TButton *Button2;
	TRadioGroup *RGPisteet;
	TMenuItem *Vakiotulosteet1;
	TMenuItem *Kaikkilopettaneethtmltiedostoon1;
	TMenuItem *Kaikkilopettaneetkirjoittimelle1;
	TMenuItem *Kaikkilhtajatsarjoittainhtmltiedostoon1;
	TCheckBox *CBKaikkiSijoitukset;
	TCheckBox *CBSarjayhd;
	TGroupBox *GBJoukkue;
	TComboBox *CBJkTunnus;
	TEdit *EdtJkKoko;
	TLabel *Label3;
	TLabel *Label15;
	TRadioGroup *RGJkTls;
	TCheckBox *CBVaadiJoukkue;
	TCheckBox *CBSeuraJoukkueena;
	TMenuItem *Kirjoittimenasetukset1;
	TCheckBox *CBUusiSivu;
	TMenuItem *Automaattinentulostus1;
	TCheckBox *CBVapaarivit;
	TCheckBox *CBViimLeima;
	TEdit *EdtPistelista;
	TCheckBox *CBVaadiLahtija;
	TComboBox *CBEmitRajaus;
	TLabel *Label13;
	TEdit *EdtJonoja;
	TLabel *Label16;
	TCheckBox *CBLainat;
	TMenuItem *Automaattinentiedostotulostus1;
	TGroupBox *GBAlisarja;
	TLabel *LblSukup;
	TEdit *EdtSukup;
	TEdit *EdtIkasarjat;
	TLabel *Label17;
	TCheckBox *CBAlisarjat;
	TCheckBox *CBLisaTeksti;
	TGroupBox *GBSHLVal;
	TCheckBox *CBSHLyhdsrj;
	TCheckBox *CBSHLohita;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TLabel *LblEste;
	TLabel *Label1;
	TEdit *EdtPisteJak;
	TMenuItem *TuloksetJaHtml;
	TMenuItem *Reittihrvelintiedostoxml1;
	TComboBox *CBSijoitukset;
	TMenuItem *Englanninkielisetotsikot1;
	TComboBox *CBxmlStand;
	TMenuItem *Listekstit1;
	TMenuItem *Mahdollisetmetsssolijat1;
	TGroupBox *GBLehdVal;
	TCheckBox *CBLehdErot;
	TCheckBox *CBLehdEtuensin;
	TCheckBox *CBLehdPitkat;
	TCheckBox *CBTulVak;
	TCheckBox *CBNaytaheti;
	TCheckBox *CBVoittLisa;
	TCheckBox *CBpisteMax;
	TCheckBox *CBvaadiKvId;
	TMenuItem *Kilpailijansivutuloste1;
	TButton *BtnLisaaLhtSarjat;
	TCheckBox *CBEiAikaa;
	TRadioGroup *RGlahtokello;
	TCheckBox *CBkierr;
	TLabel *Label4;
	TEdit *EdtKopioita;
	TLabel *Label18;
	TEdit *EdtJkMin;
	TLabel *Label19;
	TLabel *Label20;
	TComboBox *CBVaadiTark;
	TCheckBox *CBOtsikkorivi;
	TCheckBox *CBLehdPilkut;
	TLabeledEdit *EdXslFilename;
	TComboBox *CBJkPstJrj;
	TLabel *LblHtmlvaroitus;
	TMenuItem *Oletusmuotoilu11;
	TMenuItem *Oletusmuotoilu21;
	TMenuItem *Oletusmuotoilu3vanha1;
	TCheckBox *CBVAKesk;
	TCheckBox *CBTakAjo;
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
	void __fastcall Kaikkilhtajatsarjoittainhtmltiedostoon1Click(TObject *Sender);
	void __fastcall CBSarjayhdClick(TObject *Sender);
	void __fastcall PageControlKohdeChange(TObject *Sender);
	void __fastcall PageControlSisaltoChange(TObject *Sender);
	void __fastcall RGOoSeuratClick(TObject *Sender);
	void __fastcall Kirjoittimenasetukset1Click(TObject *Sender);
	void __fastcall LBSarjatDblClick(TObject *Sender);
	void __fastcall LBSarjaValDblClick(TObject *Sender);
	void __fastcall Automaattinentulostus1Click(TObject *Sender);
	void __fastcall CBOoSisaltoChange(TObject *Sender);
	void __fastcall CBLlPaivatChange(TObject *Sender);
	void __fastcall Automaattinentiedostotulostus1Click(TObject *Sender);
	void __fastcall CBAlisarjatClick(TObject *Sender);
	void __fastcall CBLisaTekstiClick(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall TuloksetJaHtmlClick(TObject *Sender);
	void __fastcall Reittihrvelintiedostoxml1Click(TObject *Sender);
	void __fastcall Englanninkielisetotsikot1Click(TObject *Sender);
	void __fastcall Listekstit1Click(TObject *Sender);
	void __fastcall Mahdollisetmetsssolijat1Click(TObject *Sender);
	void __fastcall Kilpailijansivutuloste1Click(TObject *Sender);
	void __fastcall BtnLisaaLhtSarjatClick(TObject *Sender);
	void __fastcall PisteValChange(TObject *Sender);
	void __fastcall RGJkTlsClick(TObject *Sender);
	void __fastcall CBJarjClick(TObject *Sender);
	void __fastcall CBLlJarjPvChange(TObject *Sender);
	void __fastcall CBxmlStandChange(TObject *Sender);
	void __fastcall Oletusmuotoilu11Click(TObject *Sender);
	void __fastcall Oletusmuotoilu21Click(TObject *Sender);
	void __fastcall Oletusmuotoilu3vanha1Click(TObject *Sender);
private:	// User declarations
	void NaytaSarjavalinnat(void);
	void __fastcall PaivitaTulossivu(TObject *Sender);
	bool SarjaValinnat[MAXSARJALUKU+MAXYHD];
	wchar_t TiedDefault[6];
	int SrjLkm;
	bool Tyyppivaihdettu;
	bool initialisoitu;
public:		// User declarations
	void enumPrinters(TComboBox *CBkirj);
	bool SarjaValittu(int Srj);
   __fastcall TTulosteForm(TComponent* Owner);
   wchar_t Kohde[30];
   wchar_t LisaFile[200];
   wchar_t SHLkilpailu[20];
   wchar_t SHLSarja[MAXSARJALUKU+MAXYHD][12];
   wchar_t SHLMatka[MAXSARJALUKU+MAXYHD][10];
   wchar_t SHLTyyli[MAXSARJALUKU+MAXYHD][2];
};
//---------------------------------------------------------------------------
extern PACKAGE TTulosteForm *TulosteForm;
//---------------------------------------------------------------------------
#endif
