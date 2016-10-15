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

#ifndef UnitTauluH
#define UnitTauluH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "HkDeclare.h"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>

#define TAULU_NRIVI 5
//#define TAULU_RIVPIT 23
#define tauluqsize 400
#define LED_NRIVI 4
#define LED_LKM 12

//---------------------------------------------------------------------------
class TFormTaulu : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TLabel *Label10;
	TEdit *EdtPort;
	TLabel *Label1;
	TEdit *Osuus1;
	TEdit *Osuus2;
	TEdit *Osuus3;
	TEdit *Osuus4;
	TEdit *Osuus5;
	TLabel *Label4;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label8;
	TCheckBox *CheckBox1;
	TCheckBox *CheckBox2;
	TCheckBox *CheckBox3;
	TCheckBox *CheckBox4;
	TCheckBox *CheckBox5;
	TCheckBox *CheckBox10;
	TCheckBox *CheckBox9;
	TCheckBox *CheckBox8;
	TCheckBox *CheckBox7;
	TCheckBox *CheckBox6;
	TEdit *Karki1;
	TEdit *Karki2;
	TEdit *Karki3;
	TEdit *Karki4;
	TEdit *Karki5;
	TEdit *Edit6;
	TEdit *Edit16;
	TLabel *Label9;
	TLabel *Label5;
	TEdit *Edit1;
	TEdit *Edit2;
	TEdit *Edit3;
	TEdit *Edit4;
	TEdit *Edit5;
	TButton *Button1;
	TLabel *Label6;
	TLabel *Label7;
	TCheckBox *CheckBox11;
	TLabel *LblT1;
	TLabel *Label16;
	TLabel *Label17;
	TComboBox *CBCPTieto1;
	TEdit *EdCPNaytto1;
	TLabel *Label18;
	TLabel *Label19;
	TEdit *EdLaite1;
	TButton *BtnAsKello1;
	TEdit *EdTeksti1;
	TTabSheet *TabSheet3;
	TPanel *Panel2;
	TLabel *Label39;
	TLabel *Label40;
	TStringGrid *SGTaulut;
	TButton *BtnAvaaYht;
	TButton *BtnSuljeYht;
	TBitBtn *BitBtn1;
	TPopupMenu *PopupMenuLaiteYht;
	TMenuItem *LaiteSarjaportti;
	TMenuItem *LaiteTCPasiakas;
	TMenuItem *LaiteUDP;
	TPopupMenu *PopupMenuLaite;
	TMenuItem *miGAZ;
	TMenuItem *CPower;
	TLabel *Label12;
	TEdit *EdRiveja1;
	TLabel *Label13;
	TLabel *Label14;
	TEdit *EdKirk1;
	TUpDown *UpDown1;
	TPanel *PlCP1;
	TRadioButton *RBOn1;
	TRadioButton *RBTyhja1;
	TRadioButton *RBOletus1;
	TTabSheet *TabSheet4;
	TComboBox *CBSarja;
	TMenuItem *RTNM;
	TLabel *Label11;
	TLabel *Label15;
	TMemo *Memo1;
	TButton *Button2;
	TEdit *EditAlku;
	TLabel *Label20;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CheckBox11Click(TObject *Sender);
	void __fastcall BtnAsKelloClick(TObject *Sender);
	void __fastcall SGTaulutMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y);
	void __fastcall LaiteYhtClick(TObject *Sender);
	void __fastcall LaiteClick(TObject *Sender);
	void __fastcall BtnAvaaYhtClick(TObject *Sender);
	void __fastcall BtnSuljeYhtClick(TObject *Sender);
	void __fastcall RBTilaClick(TObject *Sender);
	void __fastcall TabSheet2Show(TObject *Sender);
	void __fastcall UpDownClick(TObject *Sender, TUDBtnType Button);
	void __fastcall EdKirkExit(TObject *Sender);
	void __fastcall EdRivejaChange(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
	void PaivitaYhteydet(void);
	void PaivLEDsivu(void);
	void __fastcall Naytaoletus(int laite);
	void __fastcall TyhjTaulu(int laite);
	void __fastcall TauluAkt(int laite, bool on);
	void __fastcall TauluJako(int laite, int lkm);
	void __fastcall TauluKirkkaus(int laite, int val);
/*
	MESSAGE void __fastcall PaivHandler(TMyMessage &msg);
	MESSAGE void __fastcall TiedotHandler(TMyMessage &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYPAIVITA, TMyMessage, PaivHandler);
	MESSAGE_HANDLER(WM_MYTIEDOT, TMyMessage, TiedotHandler);
END_MESSAGE_MAP(TComponent)
*/
	TEdit *EdCPOs[LED_LKM][LED_NRIVI];
	TComboBox *CBCPTieto[LED_LKM][LED_NRIVI];
	TEdit *EdCPNaytto[LED_LKM][LED_NRIVI];
	TEdit *EdTeksti[LED_LKM][LED_NRIVI];
	TEdit *EdLaite[LED_LKM];
	TButton *BtnAsKello[LED_LKM];
	TEdit *EdRiveja[LED_LKM];
	TEdit *EdKirk[LED_LKM];
	TUpDown *UpDown[LED_LKM];
	TRadioButton *RBOn[LED_LKM];
	TRadioButton *RBTyhja[LED_LKM];
	TRadioButton *RBOletus[LED_LKM];
	TPanel *PlCP[LED_LKM];
	int ledNRivi[LED_LKM];
	TLabel *LblT[LED_LKM];
	int NLed;
	int CPos[LED_LKM];
	int CPrno[LED_LKM];
	int CPavoin[LED_LKM];
	TEdit *Edt[5];
	TEdit *Karki[5];
	TEdit *Osuus[5];
	TEdit *GAZ1[5][6];
	TEdit *GAZ2[5][6];
	TCheckBox *CBak[5];
	TCheckBox *CBauto[5];
	int taulusrj, GAZavoin, in_taulu;
	int nosuus;
	int eronaytto, autokarki[MAXSARJALUKU];
	int tkarki[MAXSARJALUKU], kviive[MAXSARJALUKU];
	int htaulu, tosuus[TAULU_NRIVI], lkmaikana[TAULU_NRIVI];
public:		// User declarations
	__fastcall TFormTaulu(TComponent* Owner);
	void opentaulu(void);
	int taulu_tied(char *text1[], int nrv);
	int taulu_lah(char *text1[], int nrv);
	void paiv_taulu(void);
	void closetaulu(void);
	void algetaulu(void);
	void nayta_valinnat(void);

	int t_com;
	int t_baud;
	int cn_gaz;
	bool hidasta;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTaulu *FormTaulu;
//---------------------------------------------------------------------------
#endif
