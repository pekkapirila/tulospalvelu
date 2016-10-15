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

#ifndef UnitMainH
#define UnitMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>
#include "VDeclare.h"
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
   TMenuItem *Toiminnot1;
   TMenuItem *Lopeta1;
   TMenuItem *Kilpailuneteneminen1;
   TMenuItem *Tilannepisteess1;
   TMenuItem *Joukkueentiedot1;
   TMenuItem *Osuudentiedot1;
   TMenuItem *Uusi1;
   TMenuItem *N1ikkuna1;
   TMenuItem *N2ikkuna1;
   TMenuItem *N3ikkna1;
   TMenuItem *N4ikkuna1;
   TMenuItem *N5ikkuna1;
   TMenuItem *N6ikkuna1;
   TMenuItem *Uusi3;
   TMenuItem *N1ikkuna3;
   TMenuItem *N2ikkuna3;
   TMenuItem *N3ikkuna2;
   TMenuItem *N4ikkuna3;
   TMenuItem *N5ikkuna3;
   TMenuItem *N6ikkuna3;
   TMenuItem *N2ikkuna4;
	TMenuItem *N3ikkuna3;
   TMenuItem *N4ikkuna4;
   TMenuItem *N5ikkuna4;
   TMenuItem *N6ikkuna4;
   TMenuItem *N1ikkuna5;
   TMenuItem *N2ikkuna5;
   TMenuItem *N3ikkuna4;
   TMenuItem *N4ikkuna5;
   TMenuItem *N5ikkuna5;
   TMenuItem *N6ikkuna5;
   TMenuItem *Uusi4;
   TMenuItem *Uusi5;
   TMenuItem *N1ikkuna4;
   TMenuItem *Vertailuajanmrittely1;
   TMenuItem *Pidkaavakepllimmisen1;
   TMenuItem *tulosteet1;
   TMenuItem *Laadituloste1;
   TMenuItem *Viimeisettapahtumat1;
   TMenuItem *Uusi6;
   TMenuItem *N11;
   TMenuItem *N2ikkuna6;
   TMenuItem *N3ikkuna5;
   TMenuItem *N4ikkuna6;
	TMenuItem *Tallennaikkunat1;
	TMenuItem *Haeikkunat1;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *LbVaihe;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *LblLahet;
	TButton *BtnTulPalv;
	TListBox *ListBox1;
	TButton *Button2;
	TButton *BtnIlmTila;
	TEdit *EditKilpailu;
	TEdit *EdtTietueita;
	TEdit *EdtTuloksia;
	TEdit *EdtYhtLkm;
	TEdit *EdtYhtAvattu;
	TMemo *Memo1;
	TEdit *EdtLahet;
	TOpenDialog *OpenDialog2;
	TPopupMenu *PopupMenu1;
	TMenuItem *Nyttulokset1;
	TMenuItem *Nytavoimet1;
	TMenuItem *Sanomaikkuna1;
	TMenuItem *Seuranta1;
	TMenuItem *Status1;
	TMenuItem *Valmistelu1;
	TMenuItem *Sarjatiedot1;
	TMenuItem *Numerointi1;
	TMenuItem *Nollaatietoja1;
	TMenuItem *Ratatiedot1;
	TMenuItem *Osanottajat1;
	TMenuItem *Joukkueet1;
	TMenuItem *Yksijoukkue1;
	TMenuItem *Haetiedostosta1;
	TMenuItem *Merkitsetarkastuskoodeja1;
	TMenuItem *Siirrlistietoja1;
	TMenuItem *analyysit1;
	TMenuItem *Emitanalyysit2;
	TMenuItem *N6ikkuna6;
	TMenuItem *N5ikkuna6;
	TMenuItem *N4ikkuna7;
	TMenuItem *N3ikkuna6;
	TMenuItem *N2ikkuna7;
	TMenuItem *N1ikkuna6;
	TMenuItem *Uusi7;
	TMenuItem *Yhteenveto2;
	TMenuItem *Tulospalvelu1;
	TMenuItem *Ajanotto11;
	TMenuItem *Ajanotto21;
	TMenuItem *Emitluenta1;
	TMenuItem *Ammunnansakot1;
	TMenuItem *Yhteislhdt1;
	TMenuItem *Help1;
	TMenuItem *Kirjoitasiirtotiedostoon1;
	TMenuItem *iedonsiirtoasetukset1;
	TMenuItem *Asetuksetjauusinta1;
	TMenuItem *iedonsiirtotiedostontarkastelu1;
	TMenuItem *Help2;
	TMenuItem *iedot1;
	TBitBtn *BitBtn1;
	TMenuItem *Avausmritykset1;
	TMenuItem *SisLuenta;
	TMenuItem *Vliaikatulosteet1;
	TMenuItem *Lhettiedosto1;
	TMenuItem *GAZtaulujenohjaus1;
	TMenuItem *Ajanotto31;
	TMenuItem *Ajanotto41;
	TMenuItem *Ajanotto1;
	TMenuItem *MySQL1;
	TMenuItem *Sakkoseuranta1;
	TMenuItem *Kopioiosuustietoja1;
	TMenuItem *Lhestyjienseuranta1;
	TMenuItem *Lhestyjt11;
	TMenuItem *Lhestyjt21;
	TMenuItem *Lhestyjt31;
	TMenuItem *Lhestyjt41;
   void __fastcall onShow(TObject *Sender);
   void __fastcall Lopeta1Click(TObject *Sender);
   void __fastcall Yhteenveto1Click(TObject *Sender);
   void __fastcall Osuudentiedot1Click(TObject *Sender);
   void __fastcall Emitanalyysit1Click(TObject *Sender);
	void __fastcall Kilpailuneteneminen1Click(TObject *Sender);
   void __fastcall Tilannepisteess1Click(TObject *Sender);
   void __fastcall Joukkueentiedot1Click(TObject *Sender);
   void __fastcall N1ikkuna1Click(TObject *Sender);
   void __fastcall N2ikkuna1Click(TObject *Sender);
   void __fastcall N3ikkna1Click(TObject *Sender);
   void __fastcall N4ikkuna1Click(TObject *Sender);
   void __fastcall N5ikkuna1Click(TObject *Sender);
   void __fastcall N6ikkuna1Click(TObject *Sender);
   void __fastcall N1ikkuna2Click(TObject *Sender);
   void __fastcall N2ikkuna2Click(TObject *Sender);
   void __fastcall N3ikkuna1Click(TObject *Sender);
   void __fastcall N4ikkuna2Click(TObject *Sender);
   void __fastcall N5ikkuna2Click(TObject *Sender);
   void __fastcall N6ikkuna2Click(TObject *Sender);
   void __fastcall N1ikkuna3Click(TObject *Sender);
   void __fastcall N2ikkuna3Click(TObject *Sender);
   void __fastcall N3ikkuna2Click(TObject *Sender);
   void __fastcall N4ikkuna3Click(TObject *Sender);
   void __fastcall N5ikkuna3Click(TObject *Sender);
   void __fastcall N6ikkuna3Click(TObject *Sender);
   void __fastcall N1ikkuna4Click(TObject *Sender);
   void __fastcall N2ikkuna4Click(TObject *Sender);
   void __fastcall N3ikkuna3Click(TObject *Sender);
   void __fastcall N4ikkuna4Click(TObject *Sender);
   void __fastcall N5ikkuna4Click(TObject *Sender);
   void __fastcall N6ikkuna4Click(TObject *Sender);
   void __fastcall N1ikkuna5Click(TObject *Sender);
   void __fastcall N2ikkuna5Click(TObject *Sender);
   void __fastcall N3ikkuna4Click(TObject *Sender);
   void __fastcall N4ikkuna5Click(TObject *Sender);
   void __fastcall N5ikkuna5Click(TObject *Sender);
   void __fastcall N6ikkuna5Click(TObject *Sender);
	void __fastcall Vertailuajanmrittely1Click(TObject *Sender);
   void __fastcall Pidkaavakepllimmisen1Click(TObject *Sender);
   void __fastcall ViimTapahtumat1Click(TObject *Sender);
   void __fastcall N11Click(TObject *Sender);
   void __fastcall N2ikkuna6Click(TObject *Sender);
   void __fastcall N3ikkuna5Click(TObject *Sender);
   void __fastcall N4ikkuna6Click(TObject *Sender);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall Tallennaikkunat1Click(TObject *Sender);
	void __fastcall Haeikkunat1Click(TObject *Sender);
	void __fastcall Laadituloste1Click(TObject *Sender);
	void __fastcall Status1Click(TObject *Sender);
	void __fastcall Yhteenveto2Click(TObject *Sender);
	void __fastcall BtnTulPalvClick(TObject *Sender);
	void __fastcall BtnIlmTilaClick(TObject *Sender);
	void __fastcall ListBox1Click(TObject *Sender);
	void __fastcall iedot1Click(TObject *Sender);
	void __fastcall Help2Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall Nollaatietoja1Click(TObject *Sender);
	void __fastcall Nyttulokset1Click(TObject *Sender);
	void __fastcall Nytavoimet1Click(TObject *Sender);
	void __fastcall Sarjatiedot1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Memo1DblClick(TObject *Sender);
	void __fastcall Kirjoitasiirtotiedostoon1Click(TObject *Sender);
	void __fastcall Haetiedostosta1Click(TObject *Sender);
	void __fastcall Joukkueet1Click(TObject *Sender);
	void __fastcall Yksijoukkue1Click(TObject *Sender);
	void __fastcall Siirrlistietoja1Click(TObject *Sender);
	void __fastcall Ratatiedot1Click(TObject *Sender);
	void __fastcall Avausmritykset1Click(TObject *Sender);
	void __fastcall Emitluenta1Click(TObject *Sender);
	void __fastcall Yhteislhdt1Click(TObject *Sender);
	void __fastcall Merkitsetarkastuskoodeja1Click(TObject *Sender);
	void __fastcall iedonsiirtotiedostontarkastelu1Click(TObject *Sender);
	void __fastcall Asetuksetjauusinta1Click(TObject *Sender);
	void __fastcall Numerointi1Click(TObject *Sender);
	void __fastcall SisLuentaClick(TObject *Sender);
	void __fastcall Ajanotto11Click(TObject *Sender);
	void __fastcall Ajanotto21Click(TObject *Sender);
	void __fastcall Vliaikatulosteet1Click(TObject *Sender);
	void __fastcall Lhettiedosto1Click(TObject *Sender);
	void __fastcall N1GAZ1Click(TObject *Sender);
	void __fastcall N2GAZ1Click(TObject *Sender);
	void __fastcall Ammunnansakot1Click(TObject *Sender);
	void __fastcall Ajanotto31Click(TObject *Sender);
	void __fastcall Ajanotto41Click(TObject *Sender);
	void __fastcall MySQL1Click(TObject *Sender);
	void __fastcall Sakkoseuranta1Click(TObject *Sender);
	void __fastcall Kopioiosuustietoja1Click(TObject *Sender);
	void __fastcall LhestyjienseurantaClick(TObject *Sender);
private:	// User declarations
	MESSAGE void __fastcall CloseReqHandler(TMyMessage &msg);
	MESSAGE void __fastcall dspMsgHandler(TMyMessage &msg);
	MESSAGE void __fastcall errorMsgHandler(TMyMessage &msg);
	MESSAGE void __fastcall warnMsgHandler(TMyMessage &msg);
	MESSAGE void __fastcall LukumaaraReqHandler(TMyMessage &msg);
	MESSAGE void __fastcall AutotulostusReqHandler(TMyMessage &msg);
	MESSAGE void __fastcall SelectOptHandler(TMyMessage &msg);
//	MESSAGE void __fastcall UusiAikaHandler(TMyMessage &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYMSGCLOSEAPPL, TMyMessage, CloseReqHandler);
	MESSAGE_HANDLER(WM_MYMSGDSPMSG, TMyMessage, dspMsgHandler);
	MESSAGE_HANDLER(WM_MYMSGDSPERRORMSG, TMyMessage, errorMsgHandler);
	MESSAGE_HANDLER(WM_MYMSGDSPWARNMSG, TMyMessage, warnMsgHandler);
	MESSAGE_HANDLER(WM_MYLUKUMAARAT, TMyMessage, LukumaaraReqHandler);
	MESSAGE_HANDLER(WM_AUTOTULOSTUS, TMyMessage, AutotulostusReqHandler);
	MESSAGE_HANDLER(WM_MYSELECTOPT, TMyMessage, SelectOptHandler);
//	MESSAGE_HANDLER(WM_MYUUSIAIKA, TMyMessage, UusiAikaHandler);
END_MESSAGE_MAP(TComponent)

	void __fastcall OsuudentiedotClick(int i);
	void __fastcall EmitanalyysitClick(int i);
	void __fastcall KilpailuneteneminenClick(int i);
	void __fastcall TilannepisteessClick(int i);
	void __fastcall JoukkueentiedotClick(int i);
	void __fastcall ViimTapahtumatClick(int i);
	void __fastcall AjanottoClick(int i);

public:		// User declarations
   __fastcall TFormMain(TComponent* Owner);
   __fastcall ~TFormMain(void);
	void __fastcall Initialisoi(wchar_t *kilphak, wchar_t *cfgfname);
	void __fastcall lueInit(int *TTila, bool *avaa, UnicodeString initflnm, UnicodeString *cfgflnm);
	void __fastcall Aloita(void);
   void __fastcall dspMsg(wchar_t *txt);
//	void __fastcall muutos(int srj, int os, int piste);
	void __fastcall TarkPyynnot(void);
	void __fastcall TallennaIkkunatV(UnicodeString FlNm);
	void __fastcall TallennaIkkunat(UnicodeString FlNm);
	void __fastcall HaeIkkunatV(UnicodeString FlNm);
	void __fastcall HaeIkkunat(UnicodeString FlNm);
	void __fastcall NaytaIkkunalista(void);
	void __fastcall SetFormKohde(TForm *Frm, wchar_t *Kohde);
	UnicodeString CfgFileName;
	UnicodeString CurrentDir;
	UnicodeString InitDir;
	UnicodeString InitFileName;
	wchar_t MessageText[500];
	bool ikkParams;
	};
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
