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

#ifndef UnitCfgH
#define UnitCfgH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Buttons.hpp>
#include "ScSSHClient.hpp"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdExplicitTLSClientServerBase.hpp>
#include <IdFTP.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
//---------------------------------------------------------------------------
class TFormCfg : public TForm
{
__published:	// IDE-managed Components
	TMemo *Memo1;
	TLabel *Label1;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TTabSheet *TabSheet3;
	TTabSheet *TabSheet4;
	TTabSheet *TabSheet5;
	TMainMenu *MainMenu1;
	TMenuItem *iedosto1;
	TMenuItem *Help1;
	TMenuItem *Luetiedosto1;
	TMenuItem *Kirjoitatiedosto1;
	TMenuItem *Suljekaavake1;
	TLabeledEdit *EdKone;
	TLabeledEdit *EdIkkOts;
	TTabSheet *TabSheet6;
	TCheckBox *CBIkkunat;
	TLabel *Label9;
	TEdit *EdIkkunat;
	TLabel *Label10;
	TCheckBox *CB25rivi;
	TCheckBox *CBKirkas;
	TLabel *Label12;
	TLabel *Label13;
	TCheckBox *CBUudenTulos;
	TComboBox *CBBoot;
	TEdit *EdVarit;
	TStringGrid *SG1;
	TLabel *Label14;
	TComboBox *CBComfile;
	TStringGrid *SGLeimat;
	TLabel *Label18;
	TGroupBox *GroupBox2;
	TGroupBox *GBComAjat;
	TCheckBox *CBComAjat;
	TLabel *Label20;
	TEdit *EdComAjat;
	TLabel *LbMaxjono;
	TEdit *EdMaxjono;
	TGroupBox *GroupBox4;
	TLabel *Label22;
	TComboBox *CBLaite1;
	TLabel *Label23;
	TComboBox *CBLaiteYht1;
	TLabel *Label24;
	TEdit *EdLaitePort1;
	TLabel *LbLaiteIP1;
	TEdit *EdLaiteIP1;
	TLabel *LbLaiteBaud1;
	TRadioGroup *RGJonot;
	TStringGrid *SGAjat;
	TRadioGroup *RGPiste;
	TCheckBox *CBSailAjat;
	TLabel *LbAjat;
	TCheckBox *CBEnnakoi;
	TButton *Button1;
	TLabel *Label31;
	TEdit *EdNappain;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TLabel *Label34;
	TRadioGroup *RGAutorap;
	TRadioGroup *RGComAutorap;
	TRadioGroup *RGLainat;
	TRadioGroup *RGKirjoitin;
	TLabel *Label39;
	TEdit *EdKirjoitin;
	TLabel *Label41;
	TEdit *EdMuotoilu;
	TGroupBox *GBAutoTul;
	TLabel *Label42;
	TComboBox *CBAutoLaajuus;
	TLabel *Label43;
	TEdit *EdAutoParhaat;
	TLabel *Label44;
	TEdit *EdAutoKynnys;
	TLabel *Label45;
	TEdit *EdAutoAskel;
	TLabel *Label46;
	TEdit *EdAutoMaalit;
	TLabel *Label47;
	TComboBox *CBAutoMonivaihe;
	TGroupBox *GBAutoTied;
	TPanel *Panel1;
	TRadioButton *RBYksiHtml;
	TRadioButton *RBXml;
	TLabel *Label48;
	TEdit *EdAutoTiedVali;
	TLabel *Label49;
	TRadioButton *RBSarjatHtml;
	TEdit *EdAutoTiedosto;
	TLabel *Label50;
	TEdit *EdHtmlTitle;
	TLabel *Label51;
	TEdit *EdHtmlOts;
	TLabel *Label52;
	TEdit *EdAutoJakelu;
	TComboBox *CBDesErotin;
	TLabel *Label53;
	TLabel *Label54;
	TComboBox *CBAutoJakelu;
	TCheckBox *CBAutoEtuSuku;
	TCheckBox *CBVaadiTark;
	TEdit *EdLainakortit;
	TLabel *LbLainakortit;
	TButton *Button2;
	TRadioButton *RBEiAutotied;
	TCheckBox *CBAfMolemmat;
	TLabel *Label57;
	TEdit *EdAikaEro1;
	TGroupBox *GBYhtAjat;
	TCheckBox *CBYhteysAjat;
	TLabel *Label58;
	TEdit *EdYhtAikaEro;
	TCheckBox *CBEmitAnal;
	TMenuItem *oiminnot1;
	TMenuItem *ulkitseikkunansislt1;
	TMenuItem *Pivitvalinnatikkunaan1;
	TComboBox *CBLaiteBaud1;
	TEdit *EdYhtPiste;
	TLabel *LbYhtPiste;
	TPopupMenu *PopupMenuYht;
	TMenuItem *UDP1;
	TMenuItem *TCP1;
	TMenuItem *TCPS1;
	TMenuItem *BRO1;
	TMenuItem *COM1;
	TPopupMenu *PopupMenuSan;
	TMenuItem *Perus1;
	TMenuItem *Ajat1;
	TMenuItem *Leimat1;
	TMenuItem *Vainajat1;
	TMenuItem *Vainleimat1;
	TMenuItem *LeimatIn1;
	TMenuItem *LeimatOut1;
	TMenuItem *Rastiva1;
	TMenuItem *ECode1;
	TPopupMenu *PopupMenuVal;
	TPopupMenu *PopupMenuIO;
	TPopupMenu *PopupMenuMalli;
	TMenuItem *Valita1;
	TMenuItem *ValitaEi1;
	TMenuItem *Out1;
	TMenuItem *In1;
	TMenuItem *InOut1;
	TMenuItem *MalliUDPvast;
	TMenuItem *MalliUDPavaaja;
	TMenuItem *MalliUDPvainIn;
	TMenuItem *MalliEmitVA;
	TMenuItem *MalliEmit;
	TPopupMenu *PopupMenuLaite;
	TMenuItem *Lukija1;
	TMenuItem *emiTag1;
	TMenuItem *LaiteMTR1;
	TPopupMenu *PopupMenuLaiteYht;
	TMenuItem *LaiteSarjaportti;
	TMenuItem *LaiteNopeaUSB;
	TMenuItem *LaiteTCPasiakas;
	TMenuItem *LaiteTCPpalvelin;
	TMenuItem *LaiteUDP;
	TPopupMenu *PopupMenuLaiteAjat;
	TMenuItem *Eiaikoja1;
	TMenuItem *Maali1;
	TMenuItem *Valiaika1;
	TMenuItem *Valiaika2;
	TMenuItem *SjBox1;
	TMenuItem *LaiteVuorot;
	TMenuItem *Lahdekoodi1;
	TMenuItem *Tied1;
	TGroupBox *GBVuoro;
	TEdit *EdSamapiste;
	TLabel *Label62;
	TRadioGroup *RGEmittoiminto;
	TMenuItem *AikaPakota;
	TRadioGroup *RGNaytaVal;
	TPanel *Panel2;
	TComboBox *CBAani;
	TLabeledEdit *EdSammSalasana;
	TLabel *Label11;
	TPanel *Panel21;
	TCheckBox *CBJoustoViesti;
	TCheckBox *CBVuorokaudet;
	TCheckBox *CBSailHyl;
	TCheckBox *CBNaytaHaj;
	TPanel *Panel3;
	TLabel *Label17;
	TEdit *EdMaxYhteys;
	TLabel *Label19;
	TEdit *EdPortBase;
	TLabel *Label15;
	TLabel *Label16;
	TCheckBox *CBLahTiedostot;
	TComboBox *CBValita;
	TCheckBox *CBLahKaikkiPolku;
	TPanel *Panel4;
	TCheckBox *CBVainVA;
	TCheckBox *CBKelloNo;
	TLabel *Label3;
	TEdit *EdOlOsuus;
	TLabel *Label4;
	TEdit *EdOsuusraja;
	TPanel *Panel7;
	TCheckBox *CBJalkiSyotto;
	TCheckBox *CBVaadiAika;
	TCheckBox *CBLuentaLoki;
	TCheckBox *CBKaksiTunn;
	TPanel *Panel10;
	TLabel *Label35;
	TEdit *EdKartta;
	TLabel *Label36;
	TEdit *EdEmitAlku;
	TPanel *Panel6;
	TLabel *Label37;
	TComboBox *CBAikaLukija;
	TLabel *Label38;
	TEdit *EdAikaLukija;
	TLabel *Label59;
	TEdit *EdEmitJono;
	TPanel *Panel5;
	TCheckBox *CBEmitAjat;
	TLabel *Label60;
	TEdit *EdEmitAjat;
	TPanel *Panel9;
	TRadioGroup *RGLoki;
	TLabel *Label40;
	TEdit *EdLoki;
	TBitBtn *BitBtn2;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn3;
	void __fastcall Luetiedosto1Click(TObject *Sender);
	void __fastcall Kirjoitatiedosto1Click(TObject *Sender);
	void __fastcall Suljekaavake1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Pivitvalinnatikkunaan1Click(TObject *Sender);
	void __fastcall Poistatulkitutrivit1Click(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall CBIkkunatClick(TObject *Sender);
	void __fastcall RGJonotClick(TObject *Sender);
	void __fastcall CBLaite1Change(TObject *Sender);
	void __fastcall CBYhteysAjatClick(TObject *Sender);
	void __fastcall CBComAjatClick(TObject *Sender);
	void __fastcall CBAikaLukijaClick(TObject *Sender);
	void __fastcall RGLainatClick(TObject *Sender);
	void __fastcall RGKirjoitinClick(TObject *Sender);
	void __fastcall RGLokiClick(TObject *Sender);
	void __fastcall CBAutoLaajuusClick(TObject *Sender);
	void __fastcall CBAutoJakeluClick(TObject *Sender);
	void __fastcall RGPisteClick(TObject *Sender);
	void __fastcall CBLaiteYht1Click(TObject *Sender);
	void __fastcall SG1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall UDP1Click(TObject *Sender);
	void __fastcall SanValClick(TObject *Sender);
	void __fastcall Valita1Click(TObject *Sender);
	void __fastcall InOut1Click(TObject *Sender);
	void __fastcall MalliClick(TObject *Sender);
	void __fastcall SGLeimatMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall LaiteClick(TObject *Sender);
	void __fastcall LaiteYhtClick(TObject *Sender);
	void __fastcall LaiteAjatClick(TObject *Sender);
	void __fastcall Help1Click(TObject *Sender);
	void __fastcall RGNaytaValClick(TObject *Sender);
	void __fastcall RGEmittoimintoClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BitBtn3Click(TObject *Sender);
private:	// User declarations
	void TulkitseTiedot(bool tyhjenna);
	void PoistaTulkitut(void);
	void PaivitaIkkuna(void);
	void ValSetup(void);
	int Tulkitut[2000];
	int Alku;
	int Loppu;
	bool SuVal;
	bool HiVal;
	bool KaVal;
public:		// User declarations
	__fastcall TFormCfg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCfg *FormCfg;
//---------------------------------------------------------------------------
#endif
