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

#ifndef UnitEmitH
#define UnitEmitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "VDeclare.h"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------

class TFormEmit : public TForm
{
__published:	// IDE-managed Components
   TEdit *EdtKilpno;
   TLabel *Label1;
   TLabel *Label2;
   TEdit *EdtBadge;
   TLabel *Nimi;
	TEdit *EdtSNimi;
   TLabel *Sarja;
	TLabel *LblSeura;
   TEdit *EdtSeura;
   TEdit *EdtTulos;
   TEdit *EdtMaali;
   TEdit *EdtLahto;
   TLabel *Label3;
   TLabel *Label4;
   TLabel *Label5;
   TLabel *Label6;
   TEdit *EdtLuettu;
   TLabel *Label7;
   TEdit *EdtSelitys;
   TLabel *Label8;
   TComboBox *TarkVal;
   TScrollBar *SBTietue;
   TEdit *EdtTietue;
   TLabel *Label9;
   TCheckBox *CBLue;
   TCheckBox *CBViim;
	TLabel *Label10;
	TBitBtn *BitBtn1;
	TButton *BtnTallenna;
	TComboBox *CSarja;
	TButton *BtnLoppuun;
	TMainMenu *MainMenu1;
	TMenuItem *iedostot1;
	TMenuItem *Lueratatiedotuudelleen1;
	TMenuItem *Nyt1;
	TMenuItem *Kortinkoodi1;
	TEdit *EdtOrigBadge;
	TLabel *LblOrigBadge;
	TButton *BtnEiBadge;
	TEdit *EdtLisno;
	TEdit *EdtJoukkue;
	TComboBox *CRata;
	TPanel *Panel1;
	TLabel *Label13;
	TRadioGroup *RGHakuPeruste;
	TEdit *EdtHakuArvo;
	TButton *BtnHaeSeur;
	TButton *BtnHaeEd;
	TButton *BtnHakuValmis;
	TButton *BtnHaeOo;
	TComboBox *CSarjaVaat;
	TLabel *LblSarjaVaat;
	TButton *BtnPeruuta;
	TButton *BtnLasketulos;
	TButton *BtnMuutokset;
	TLabel *Label12;
	TEdit *EdtEro;
	TMenuItem *Asetukset1;
	TDrawGrid *EmitGrid;
	TCheckBox *CBNaytaLeimat;
	TLabel *LblToimi;
	TButton *BtnPerKirjaus;
	TEdit *EdtMessage;
	TEdit *EdtBdgMsg;
	TMemo *MemoMessage;
	TMenuItem *Valitsekirjoitin1;
	TMenuItem *Kirjoitintulosteet1;
	TMenuItem *Kortintiedot1;
	TLabel *Label14;
	TEdit *EdtSakkoja;
	TMenuItem *Automaattinentulostus1;
	TLabel *Label15;
	TCheckBox *CBHaeArvo;
	TMenuItem *Vliaikatuloste1;
	TButton *BtnLeimRap;
	TButton *BtnVaTuloste;
	TLabel *LblLaina;
	TMenuItem *Automaattinenvliaikatuloste1;
	TMenuItem *Vaadikaksoisluenta1;
	TLabel *Label16;
	TMenuItem *oiminnot1;
	TMenuItem *Etsileimojavastaavarata1;
	TMenuItem *emiTagluennanohjaus1;
	TMenuItem *Maalintietokone1;
	TMenuItem *Merkitsekortitlainakorteiksi1;
	TCheckBox *CBHaeSeurLaskettava;
	TMenuItem *Avaadebugtiedosto1;
	TMenuItem *Suljedebugtiedosto1;
	TMenuItem *Lhdntietokone1;
	TLabel *Label17;
	TComboBox *CBOsuus;
	TComboBox *CBHakuOsuus;
	TLabel *Label11;
	TComboBox *CBVaadi;
	TLabel *Label18;
	TLabel *LblSulj;
	TButton *BtnNaytaKartta;
	TMenuItem *Suljekaavake1;
	TMenuItem *Lhetnytetytleimattiedonsiirtoon1;
	TPopupMenu *PopupMenu1;
	TMenuItem *Lisrivi1;
	TMenuItem *Poistarivi1;
	TMenuItem *Joukkokorjaukset1;
	TMenuItem *Mrluentaajatmallinajoista1;
	TMenuItem *oimimatonkortti1;
	TProgressBar *ProgressBar1;
	TButton *BtnPiikit;
	TMenuItem *Luemysviallisetkortit1;
	TOpenDialog *OpenDialog1;
	TMenuItem *Luetietojatiedostosta1;
	TMenuItem *Onlineaikojenlaskentarastivliajoista1;
	TMenuItem *arkastuksentuloskaavake1;
	TMenuItem *Rastikoodit1;
	TMenuItem *Laitteet1;
	TMenuItem *MTRlaitteenohjaus1;
	TMenuItem *HylkRap;
	TButton *BtnHylkRap;
	TMenuItem *Sail99varoitus;
   void __fastcall SBTietueEnter(TObject *Sender);
   void __fastcall SBTietueChange(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
	void __fastcall EdtTietueExit(TObject *Sender);
	void __fastcall EmitGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BtnTallennaClick(TObject *Sender);
	void __fastcall CSarjaChange(TObject *Sender);
	void __fastcall Kortinkoodi1Click(TObject *Sender);
	void __fastcall EdtKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EdtTietueKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EdtBadgeExit(TObject *Sender);
	void __fastcall EdtBadgeKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EdtRataChange(TObject *Sender);
	void __fastcall TarkValChange(TObject *Sender);
	void __fastcall EdtSelitysChange(TObject *Sender);
	void __fastcall TarkValKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EdtSelitysKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EdtRataKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall RGHakuPerusteClick(TObject *Sender);
	void __fastcall BtnHaeSeurClick(TObject *Sender);
	void __fastcall BtnHaeEdClick(TObject *Sender);
	void __fastcall BtnHakuValmisClick(TObject *Sender);
	void __fastcall EdtHakuArvoChange(TObject *Sender);
	void __fastcall BtnHaeOoClick(TObject *Sender);
	void __fastcall EdtBadgeChange(TObject *Sender);
	void __fastcall EdtBadgeKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall CRataKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CSarjaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EdtKilpnoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall BtnLoppuunClick(TObject *Sender);
	void __fastcall BtnPeruutaClick(TObject *Sender);
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall SBTietueScroll(TObject *Sender, TScrollCode ScrollCode, int &ScrollPos);
	void __fastcall BtnLasketulosClick(TObject *Sender);
	void __fastcall BtnMuutoksetClick(TObject *Sender);
	void __fastcall EmitGridSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall EmitGridExit(TObject *Sender);
	void __fastcall EmitGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall EmitGridSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
	void __fastcall CBNaytaLeimatClick(TObject *Sender);
	void __fastcall BtnPerKirjausClick(TObject *Sender);
	void __fastcall Valitsekirjoitin1Click(TObject *Sender);
	void __fastcall Kortintiedot1Click(TObject *Sender);
	void __fastcall EdtSakkojaChange(TObject *Sender);
	void __fastcall Automaattinentulostus1Click(TObject *Sender);
	void __fastcall CBHaeArvoClick(TObject *Sender);
	void __fastcall Vliaikatuloste1Click(TObject *Sender);
	void __fastcall BtnVaTulosteClick(TObject *Sender);
	void __fastcall BtnLeimRapClick(TObject *Sender);
	void __fastcall Automaattinenvliaikatuloste1Click(TObject *Sender);
	void __fastcall Lueratatiedotuudelleen1Click(TObject *Sender);
	void __fastcall Vaadikaksoisluenta1Click(TObject *Sender);
	void __fastcall EmitGridMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall EmitGridMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall CRataChange(TObject *Sender);
	void __fastcall Etsileimojavastaavarata1Click(TObject *Sender);
	void __fastcall MTRlaitteenohjaus1Click(TObject *Sender);
	void __fastcall emiTagluennanohjaus1Click(TObject *Sender);
	void __fastcall Maalintietokone1Click(TObject *Sender);
	void __fastcall Merkitsekortitlainakorteiksi1Click(TObject *Sender);
	void __fastcall Avaadebugtiedosto1Click(TObject *Sender);
	void __fastcall Suljedebugtiedosto1Click(TObject *Sender);
	void __fastcall Lhdntietokone1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CBOsuusChange(TObject *Sender);
	void __fastcall EdtMaaliChange(TObject *Sender);
	void __fastcall EdtMaaliKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CBHakuOsuusClick(TObject *Sender);
	void __fastcall CBVaadiChange(TObject *Sender);
	void __fastcall BtnNaytaKarttaClick(TObject *Sender);
	void __fastcall Suljekaavake1Click(TObject *Sender);
	void __fastcall Lhetnytetytleimattiedonsiirtoon1Click(TObject *Sender);
	void __fastcall Lisrivi1Click(TObject *Sender);
	void __fastcall Poistarivi1Click(TObject *Sender);
	void __fastcall Mrluentaajatmallinajoista1Click(TObject *Sender);
	void __fastcall oimimatonkortti1Click(TObject *Sender);
	void __fastcall BtnPiikitClick(TObject *Sender);
	void __fastcall Luemysviallisetkortit1Click(TObject *Sender);
	void __fastcall Luetietojatiedostosta1Click(TObject *Sender);
	void __fastcall Onlineaikojenlaskentarastivliajoista1Click(TObject *Sender);
	void __fastcall arkastuksentuloskaavake1Click(TObject *Sender);
	void __fastcall Rastikoodit1Click(TObject *Sender);
	void __fastcall HylkRapClick(TObject *Sender);
	void __fastcall Sail99varoitusClick(TObject *Sender);



private:	// User declarations
   void __fastcall NaytaKilp(void);
   void __fastcall NaytaEmit(void);
   void __fastcall NaytaPartio(void);
   void __fastcall Paivita(emittp *pEm);
	bool __fastcall HakuEhto(bool ylos);
	void __fastcall TallVals(void);
	bool __fastcall OnkoMuutoksia(void);
	void __fastcall tallennaKilpailija(bool kysy);
   void __fastcall BadgeHaku(void);
	void __fastcall lueKilp3(int d);
	void __fastcall paivitaMuutos(void);
	void __fastcall HaeTulostukseen(int badge);
	void __fastcall ProsEmit(void);
	int __fastcall ProsEmitMaali(void);
	int __fastcall HaeKilpailija(void);
	int __fastcall KysyKilpailija(void);
	void __fastcall NollaaMessage(void);
	cellTp Cells[7][52];
   int HTBadge;
   int EdKey;
   int LSn;
   int LEn;
   int LSe;
   UnicodeString ValSNimi;
   UnicodeString ValSeura;
   UnicodeString ValTulos;
   UnicodeString ValMaali;
   UnicodeString ValLahto;
   UnicodeString ValSelitys;
   UnicodeString ValSakkoja;
   int ValTarkVal;
   int ValRata;
   int ValSarja;
   int ValOsuus;
   int aktrow;
   int aktcol;
   bool UusiTietue;
   bool naytaEiAkt;
   bool eiLiitetty;
	MESSAGE void __fastcall NewCardHandler(TMyMessage &msg);
	MESSAGE void __fastcall DisplTimerHandler(TMyMessage &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYMSGNEWCARD, TMyMessage, NewCardHandler);
	MESSAGE_HANDLER(WM_MYDISPLTIMER, TMyMessage, DisplTimerHandler);
END_MESSAGE_MAP(TComponent)
public:		// User declarations
   __fastcall TFormEmit(TComponent* Owner);
   __fastcall ~TFormEmit(void);
//	void __fastcall haeKilp(int d);
   void __fastcall Nayta(void);
	void __fastcall PeruutaMuutokset(void);
	int __fastcall HaeTietue(int pos, int suunta);
	int __fastcall OtaTietue(int pos);
	void __fastcall HaeUusiTietue(void);
	void __fastcall PalautaMuutokset(void);
	void __fastcall TallennaMuutokset(void);
	void __fastcall MerkitseOhitetuksi(void);
	void __fastcall TallennaVaihto(int d, int osuus);
	void __fastcall PalautaEmMuutokset(void);
	void __fastcall TallennaEmMuutokset(void);
	void __fastcall SiirraKilpailijalle(int kno, int osuus);
//	void __fastcall NaytaMuutokset(void);
	int __fastcall LaskeTulos();
	void __fastcall SalliMuutokset(bool salli);
	void __fastcall PuolitaKortinAjat(void);
	void __fastcall KasvataKortinAjat(int lisa);
	void __fastcall Tulosta(int tuloste);
	void __fastcall InitTila(void);
	int applyParams(void);
	void haeParams(void);
	emitIkkParamClass IkkParam;
   wchar_t Kohde[30];
   int TViimRasti;
   emittp Em;
   kilptietue Kilp;
   int Osuus;
   emittp Em_0;
   kilptietue Kilp_0;
   int Osuus_0;
   int DKilp;
   int DKilp_0;
   int Tietue;
   int Tietue_0;
   bool EmitMuutosFlag;
   bool naytaMuutosOhje;
   bool kirjaaEmitMuutokset;
   bool kuntoMaali;
   bool kuntoLahto;
   bool uusiLuenta;
   unsigned int BdgMsgKirkkaus;
	wchar_t EmitKirjoitin[200];
	tulostusparamtp EmitTulPrm;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEmit *FormEmit;
//---------------------------------------------------------------------------
#endif
