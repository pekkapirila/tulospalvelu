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

#ifndef WinHkH
#define WinHkH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Menus.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>
#include "HkDeclare.h"
#include <Vcl.Buttons.hpp>
#include "UnitBrowser.h"
//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
   TMainMenu *MainMenu1;
   TMenuItem *Tiedostot1;
   TMenuItem *Lopeta1;
   TMenuItem *Nytt1;
   TMenuItem *Kilpailija1;
   TMenuItem *Uusi1;
   TMenuItem *N1kilpailija1;
   TMenuItem *N2kilpailija1;
   TMenuItem *N3kilpailija1;
   TMenuItem *N4kilpailija1;
   TMenuItem *N5kilpailija1;
   TMenuItem *N6kilpailija1;
   TMenuItem *Yhteenveto1;
   TMenuItem *Tilanne1;
   TMenuItem *Uusi2;
	TMenuItem *N1tilanne1;
   TMenuItem *N2tilanne1;
   TMenuItem *N3tilanne1;
   TMenuItem *N4tilanne1;
   TMenuItem *N5tilanne1;
   TMenuItem *N6tilanne1;
   TMenuItem *Emitgrafiikka1;
   TMenuItem *Uusi3;
   TMenuItem *N1graafi1;
   TMenuItem *N2graafi1;
   TMenuItem *N3graafi1;
   TMenuItem *N4graafi1;
   TMenuItem *N5graafi1;
   TMenuItem *N6graafi1;
   TMenuItem *Asetukset1;
   TMenuItem *Viimeisettapahtumat1;
   TMenuItem *Toiminnot1;
   TMenuItem *Tulosteet1;
   TMenuItem *Uusiikkuna1;
   TMenuItem *N1ikkuna1;
   TMenuItem *N2ikkuna1;
   TMenuItem *N3ikkuna1;
   TMenuItem *N4ikkuna1;
	TMenuItem *N5ikkuna1;
	TMenuItem *N6ikkuna1;
   TMenuItem *Asetukset2;
	TMenuItem *Tallennaikkunat;
	TMenuItem *Haeikkunat;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TMenuItem *Loppukilpailunmuodostaminen1;
	TMenuItem *akaaajolhtjenmuodostaminen1;
	TMenuItem *Vliaikojenhakupalvelimelta1;
	TButton *BtnTulPalv;
	TOpenDialog *OpenDialog2;
	TMenuItem *Haeerottimellisestatekstitiedostosta1;
	TMenuItem *Status1;
	TListBox *ListBox1;
	TButton *Button2;
	TLabel *Label1;
	TMenuItem *Kil1;
	TMenuItem *Uusi4;
	TMenuItem *N1kilpailija2;
	TMenuItem *N2kilpailija2;
	TMenuItem *N3kilpailija2;
	TMenuItem *N4kilpailija2;
	TMenuItem *N5kilpailija2;
	TMenuItem *N6kilpailija2;
	TButton *BtnIlmTila;
	TMenuItem *Nollaa1;
	TMenuItem *Lisvakantteja1;
	TMenuItem *Numeroi1;
	TMenuItem *Ilmoittautumiset1;
	TEdit *EditKilpailu;
	TLabel *Label2;
	TMenuItem *Seuranimienkopioinnit1;
	TMenuItem *Avausmritykset1;
	TMenuItem *Osanottjat1;
	TMenuItem *Osanottajat1;
	TMenuItem *Ratatiedot1;
	TMenuItem *Sarjatiedot1;
	TLabel *Label3;
	TMenuItem *Help1;
	TMenuItem *iedot1;
	TMenuItem *Kirjoitasiirtotiedostoon1;
	TMenuItem *Help2;
	TBitBtn *BitBtn1;
	TMenuItem *Siirrlistietoja1;
	TMenuItem *Merkitsetarkastuskoodeja1;
	TMenuItem *Analyysit1;
	TMenuItem *COMFILEDAT1;
	TMenuItem *Laskekilpailijoidenpisteet1;
	TLabel *LbVaihe;
	TMenuItem *Tulospalvelu1;
	TMenuItem *Ajanotto1;
	TMenuItem *Emitluenta1;
	TMenuItem *Kirjoitinjatiedostotulosteet1;
	TMenuItem *Sprinthiihdonsiirrot1;
	TMenuItem *Muutasarjanlhtaikoja1;
	TMenuItem *Ajanotto21;
	TEdit *EdtTietueita;
	TLabel *Label4;
	TLabel *Label5;
	TEdit *EdtTuloksia;
	TLabel *Label6;
	TEdit *EdtYhtLkm;
	TEdit *EdtYhtAvattu;
	TLabel *Label7;
	TLabel *Label8;
	TMemo *Memo1;
	TMenuItem *MySQL1;
	TMenuItem *Sanomaikkuna1;
	TMenuItem *Seuratjaosanottajamrt1;
	TMenuItem *Yksiosanottaja1;
	TMenuItem *Aikojenjanumeroidensiirrot1;
	TMenuItem *iedonsiirronuusintalhetys1;
	TLabel *LblLahet;
	TEdit *EdtLahet;
	TMenuItem *Ammunnansakot1;
	TMenuItem *Joukkuepisteet1;
	TPopupMenu *PopupMenu1;
	TMenuItem *Nyttulokset1;
	TMenuItem *Nytavoimet1;
	TMenuItem *Tapahtumasarjansiirrot1;
	TMenuItem *Kilpailuluettelo1;
	TMenuItem *Ajanotto11;
	TMenuItem *Ajanotto31;
	TMenuItem *Ajanotto41;
	TMenuItem *SytEmitkoodeja1;
	TMenuItem *Lhettiedosto1;
	TMenuItem *Lueeratjaradat1;
	TMenuItem *Valitseer1;
	TMenuItem *LueFIStiedostoja1;
	TMenuItem *Vaiheenvaihto1;
	TMenuItem *Sakkokierrostenseuranta1;
	TMenuItem *Ampumapaikat1;
	TMenuItem *Ampumasuunnistuksensakot1;
	TMenuItem *Yhdistettyhiihto1;
	TMenuItem *Tulostaulut1;
	TMenuItem *Luetiedotuudelleen1;
   void __fastcall Lopeta1Click(TObject *Sender);
   void __fastcall KilpailijaU1Click(TObject *Sender);
   void __fastcall TilanneU1Click(TObject *Sender);
   void __fastcall N1kilpailija1Click(TObject *Sender);
	void __fastcall N2kilpailija1Click(TObject *Sender);
   void __fastcall N3kilpailija1Click(TObject *Sender);
   void __fastcall N4kilpailija1Click(TObject *Sender);
   void __fastcall N5kilpailija1Click(TObject *Sender);
   void __fastcall N6kilpailija1Click(TObject *Sender);
   void __fastcall N1tilanne1Click(TObject *Sender);
   void __fastcall N2tilanne1Click(TObject *Sender);
   void __fastcall N3tilanne1Click(TObject *Sender);
   void __fastcall N4tilanne1Click(TObject *Sender);
   void __fastcall N5tilanne1Click(TObject *Sender);
   void __fastcall N6tilanne1Click(TObject *Sender);
   void __fastcall EmitgrafiikkaU1Click(TObject *Sender);
   void __fastcall N1graafi1Click(TObject *Sender);
   void __fastcall N2graafi1Click(TObject *Sender);
   void __fastcall N3graafi1Click(TObject *Sender);
   void __fastcall N4graafi1Click(TObject *Sender);
   void __fastcall N5graafi1Click(TObject *Sender);
   void __fastcall N6graafi1Click(TObject *Sender);
   void __fastcall Asetukset1Click(TObject *Sender);
   void __fastcall Yhteenveto1Click(TObject *Sender);
   void __fastcall ViimTapahtumatU1Click(TObject *Sender);
   void __fastcall N1ikkuna1Click(TObject *Sender);
   void __fastcall N2ikkuna1Click(TObject *Sender);
   void __fastcall N3ikkuna1Click(TObject *Sender);
   void __fastcall N4ikkuna1Click(TObject *Sender);
   void __fastcall N5ikkuna1Click(TObject *Sender);
   void __fastcall N6ikkuna1Click(TObject *Sender);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
   void __fastcall Asetukset2Click(TObject *Sender);
	void __fastcall TallennaikkunatClick(TObject *Sender);
	void __fastcall HaeikkunatClick(TObject *Sender);
	void __fastcall Loppukilpailunmuodostaminen1Click(TObject *Sender);
	void __fastcall akaaajolhtjenmuodostaminen1Click(TObject *Sender);
	void __fastcall Vliaikojenhakupalvelimelta1Click(TObject *Sender);
	void __fastcall BtnTulPalvClick(TObject *Sender);
	void __fastcall Haeerottimellisestatekstitiedostosta1Click(TObject *Sender);
	void __fastcall Status1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall KilpailijaU2Click(TObject *Sender);
	void __fastcall N1kilpailija2Click(TObject *Sender);
	void __fastcall N2kilpailija2Click(TObject *Sender);
	void __fastcall N3kilpailija2Click(TObject *Sender);
	void __fastcall N4kilpailija2Click(TObject *Sender);
	void __fastcall N5kilpailija2Click(TObject *Sender);
	void __fastcall N6kilpailija2Click(TObject *Sender);
	void __fastcall BtnIlmTilaClick(TObject *Sender);
	void __fastcall Nollaa1Click(TObject *Sender);
	void __fastcall Lisvakantteja1Click(TObject *Sender);
	void __fastcall Numeroi1Click(TObject *Sender);
	void __fastcall Ilmoittautumiset1Click(TObject *Sender);
	void __fastcall Seuranimienkopioinnit1Click(TObject *Sender);
	void __fastcall Avausmritykset1Click(TObject *Sender);
	void __fastcall Osanottajat1Click(TObject *Sender);
	void __fastcall Sarjatiedot1Click(TObject *Sender);
	void __fastcall iedot1Click(TObject *Sender);
	void __fastcall ListBox1Click(TObject *Sender);
	void __fastcall Kirjoitasiirtotiedostoon1Click(TObject *Sender);
	void __fastcall Help2Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall Siirrratakoodit1Click(TObject *Sender);
	void __fastcall Siirrlistietoja1Click(TObject *Sender);
	void __fastcall Merkitsetarkastuskoodeja1Click(TObject *Sender);
	void __fastcall COMFILEDAT1Click(TObject *Sender);
	void __fastcall Laskekilpailijoidenpisteet1Click(TObject *Sender);
	void __fastcall Emitluenta1Click(TObject *Sender);
	void __fastcall Ajanotto11Click(TObject *Sender);
	void __fastcall Kirjoitinjatiedostotulosteet1Click(TObject *Sender);
	void __fastcall Sprinthiihdonsiirrot1Click(TObject *Sender);
	void __fastcall Muutasarjanlhtaikoja1Click(TObject *Sender);
	void __fastcall Ajanotto21Click(TObject *Sender);
	void __fastcall MySQL1Click(TObject *Sender);
	void __fastcall Ratatiedot1Click(TObject *Sender);
	void __fastcall dspMsg(wchar_t *txt);
	void __fastcall Memo1DblClick(TObject *Sender);
	void __fastcall Sanomaikkuna1Click(TObject *Sender);
	void __fastcall Seuratjaosanottajamrt1Click(TObject *Sender);
	void __fastcall Yksiosanottaja1Click(TObject *Sender);
	void __fastcall Aikojenjanumeroidensiirrot1Click(TObject *Sender);
	void __fastcall iedonsiirronuusintalhetys1Click(TObject *Sender);
	void __fastcall Ammunnansakot1Click(TObject *Sender);
	void __fastcall Joukkuepisteet1Click(TObject *Sender);
	void __fastcall Nyttulokset1Click(TObject *Sender);
	void __fastcall Nytavoimet1Click(TObject *Sender);
	void __fastcall Tapahtumasarjansiirrot1Click(TObject *Sender);
	void __fastcall Kilpailuluettelo1Click(TObject *Sender);
	void __fastcall Ajanotto31Click(TObject *Sender);
	void __fastcall Ajanotto41Click(TObject *Sender);
	void __fastcall SytEmitkoodeja1Click(TObject *Sender);
	void __fastcall Lhettiedosto1Click(TObject *Sender);
	void __fastcall Lueeratjaradat1Click(TObject *Sender);
	void __fastcall Valitseer1Click(TObject *Sender);
	void __fastcall LueFIStiedostoja1Click(TObject *Sender);
	void __fastcall Vaiheenvaihto1Click(TObject *Sender);
	void __fastcall Sakkokierrostenseuranta1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Ampumapaikat1Click(TObject *Sender);
	void __fastcall Ampumasuunnistuksensakot1Click(TObject *Sender);
	void __fastcall Yhdistettyhiihto1Click(TObject *Sender);
	void __fastcall Tulostaulut1Click(TObject *Sender);

private:	// User declarations
	MESSAGE void __fastcall CloseReqHandler(TMyMessage &msg);
	MESSAGE void __fastcall dspMsgHandler(TMyMessage &msg);
	MESSAGE void __fastcall errorMsgHandler(TMyMessage &msg);
	MESSAGE void __fastcall warnMsgHandler(TMyMessage &msg);
	MESSAGE void __fastcall LukumaaraReqHandler(TMyMessage &msg);
	MESSAGE void __fastcall AutotulostusReqHandler(TMyMessage &msg);
	MESSAGE void __fastcall SelectOptHandler(TMyMessage &msg);
	MESSAGE void __fastcall VaihdaVaiheHandler(TMyMessage &msg);
//	MESSAGE void __fastcall UusiAikaHandler(TMyMessage &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYMSGCLOSEAPPL, TMyMessage, CloseReqHandler);
	MESSAGE_HANDLER(WM_MYMSGDSPMSG, TMyMessage, dspMsgHandler);
	MESSAGE_HANDLER(WM_MYMSGDSPERRORMSG, TMyMessage, errorMsgHandler);
	MESSAGE_HANDLER(WM_MYMSGDSPWARNMSG, TMyMessage, warnMsgHandler);
	MESSAGE_HANDLER(WM_MYLUKUMAARAT, TMyMessage, LukumaaraReqHandler);
	MESSAGE_HANDLER(WM_AUTOTULOSTUS, TMyMessage, AutotulostusReqHandler);
	MESSAGE_HANDLER(WM_MYSELECTOPT, TMyMessage, SelectOptHandler);
	MESSAGE_HANDLER(WM_MYVAIHDAVAIHE, TMyMessage, VaihdaVaiheHandler);
//	MESSAGE_HANDLER(WM_MYUUSIAIKA, TMyMessage, UusiAikaHandler);
END_MESSAGE_MAP(TComponent)

	void __fastcall KilpailijaEmitClick(int i);
	void __fastcall KilpailijaOnlineClick(int i);
	void __fastcall TilanneClick(int i);
	void __fastcall EmitGrafiikkaClick(int i);
	void __fastcall ViimTapahtumatClick(int i);
	void __fastcall AjanottoClick(int i);

public:		// User declarations
	__fastcall TFormMain(TComponent* Owner);
	__fastcall ~TFormMain(void);
	void __fastcall lueInit(int *TTila, int *KsMoodi, bool *kLuett, bool *avaa, UnicodeString *cfgflnm);
	void __fastcall Aloita(void);
	void __fastcall TallennaIkkunat(UnicodeString FlNm);
	void __fastcall TallennaIkkunatV(UnicodeString FlNm);
	void __fastcall HaeIkkunat(UnicodeString FlNm);
	void __fastcall HaeIkkunatV(UnicodeString FlNm);
	void __fastcall NaytaIkkunalista(void);
	void __fastcall Initialisoi(wchar_t *kilphak, wchar_t *cfgfname);
	void __fastcall SetFormKohde(TForm *Frm, wchar_t *Kohde);
	UnicodeString CfgFileName;
	UnicodeString CurrentDir;
	UnicodeString InitDir;
	wchar_t MessageText[500];
	bool ikkParams;
};
#define NFORMVAGR 6
#define NTILANNEFORM 6
#define NFORMKILPAILIJA 6
#define NTAPFORM 6
#define NAJANOTOT 4
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------
#endif
