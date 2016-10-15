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

#ifndef UnitRadatH
#define UnitRadatH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Menus.hpp>
#ifndef MAXOSUUSLUKU
#include "HkDef.h"
#else
#include "VDef.h"
#endif
#include <Vcl.Menus.hpp>

#define MAXRINNKOODI 10
#define MAXTUNNUS  899
#define MAXNKILP 9999
#define MAXRATA MAXNRATA

//---------------------------------------------------------------------------
class TFormRadat : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *TabSheetRadat;
	TTabSheet *TabSheetLeimat;
	TComboBox *CBRadat;
	TEdit *EdtRNimi;
	TStringGrid *SGRata;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label6;
	TButton *BtnTallRata;
	TStringGrid *SGLeimat;
	TButton *BtnSulje;
	TTabSheet *TabSheetXML;
	TTabSheet *TabSheet1;
	TButton *BtnOtaKayttoon;
	TButton *BtnTarkista;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TTabSheet *TabSheetOngelmat;
	TMemo *Memo1;
	TLabel *Label7;
	TButton *BtnTallKoodit;
	TButton *BtnTyhjKoodit;
	TButton *BtnLeimatLisaaRasti;
	TButton *BtnLeimatPoistaRasti;
	TButton *BtnRataTyhjenna;
	TButton *BtnRataLisaaRasti;
	TButton *BtnRataPoistaRasti;
	TRadioGroup *RGUNimi;
	TButton *BtnPeruutaKoodi;
	TButton *BtnPoistaRata;
	TComboBox *CBTiedLaji;
	TLabel *Label8;
	TGroupBox *GBXML;
	TRadioGroup *RGXMLrata;
	TCheckBox *CBVariaatio;
	TCheckBox *CBXMLLeimat;
	TGroupBox *GBOcad8;
	TRadioGroup *RGOcadRata;
	TGroupBox *GBcp;
	TRadioGroup *RGErotin;
	TEdit *EdtRataSij;
	TLabel *Label9;
	TLabel *Label10;
	TEdit *EdtKoodiSij;
	TButton *BtnImport;
	TCheckBox *CBpilkkuerotin;
	TComboBox *KirjoitinVal;
	TLabel *Label15;
	TLabel *Label16;
	TComboBox *CBTuloste;
	TListBox *LBRadat;
	TButton *BtnPoisKaikki;
	TButton *BtnPoisValitut;
	TButton *BtnLisValitut;
	TButton *BtnLisKaikki;
	TListBox *LBRataVal;
	TLabel *Label17;
	TLabel *Label18;
	TButton *BtnTulosta;
	TBitBtn *BitBtn1;
	TRadioGroup *RGlisaa;
	TButton *BtnOletuskoodit;
	TGroupBox *GBTall;
	TButton *BtnLueXml;
	TButton *BtnKirjXml;
	TLabel *Label11;
	TMainMenu *MainMenu1;
	TMenuItem *iedosto1;
	TMenuItem *Lueradatjarastitxml1;
	TMenuItem *LuevanhaRADATLST1;
	TMenuItem *N1;
	TMenuItem *allennaradatjarastit1;
	TMenuItem *KirjoitavanhaRADATLST1;
	TMenuItem *N2;
	TMenuItem *LuevanhaLEIMATLST1;
	TMenuItem *KirjoitavanhaLEIMATLST1;
	TMenuItem *N3;
	TMenuItem *Suljekaavake1;
	TMenuItem *Ratatoimintoja1;
	TMenuItem *Kopioiradannollausennakkokaikkiinratoihin1;
	TMenuItem *Kopioiradanmaalivalinnatkaikkiinratoihin1;
	TMenuItem *Liskaikkiinratoihinmaalinrastikoodi1;
	TGroupBox *GroupBox2;
	TComboBox *CBLahto;
	TLabel *Label4;
	TEdit *EdtEnnakko;
	TGroupBox *GroupBox3;
	TComboBox *CBMaali;
	TCheckBox *CBAutoTulos;
	TLabel *LblMatkaMaaliin;
	TEdit *EdtMatkaMaaliin;
	TLabel *Label3;
	TEdit *EdtRataLkm;
	TLabel *Label5;
	TLabel *Label13;
	TLabel *Label14;
	TEdit *EdtRastiLkm;
	TBitBtn *BitBtn2;
	TCheckBox *CBNaytaTall;
	TBitBtn *BitBtn3;
	TBitBtn *BitBtn4;
	TBitBtn *BitBtn5;
	TBitBtn *BitBtn6;
	TMenuItem *Help1;
	TOpenDialog *OpenDialog2;
	TTabSheet *TabSheetSarjat;
	TStringGrid *SGSarjat;
	TLabel *Label20;
	TLabel *Label21;
	TLabel *Label22;
	TMenuItem *Kartta1;
	TMenuItem *Avaakartta1;
	TMenuItem *Nytrata1;
	TTabSheet *TabSheetKoord;
	TGroupBox *GroupBox1;
	TLabel *Label23;
	TLabel *Label24;
	TEdit *EdtScale;
	TEdit *EdtMapLeft;
	TLabel *Label25;
	TEdit *EdtMapTop;
	TLabel *Label26;
	TEdit *EdtMapBottom;
	TLabel *Label27;
	TEdit *EdtMapRight;
	TStringGrid *SGKoord;
	TCheckBox *CBKirjKoord;
	TButton *BtnVainKoord;
	TButton *BtnLueKoordCsv;
	TButton *BtnTyhjKoord;
	TButton *BtnPoistaKoord;
	TLabel *Lähtö;
	TLabel *Label28;
	TEdit *EdtLahto;
	TEdit *EdtMaali;
	TButton *BtnKoordKayttoon;
	TButton *BtnKirjKoordCsv;
	TMenuItem *MerkMaalikoodi;
	TMenuItem *MerkLahtokoodi;
	TMenuItem *Laskerastivlienmatkatkoordinaateista1;
	TButton *BtnPoistaKohdennus;
	TButton *BtnTyhjKohdennukset;
	TLabel *Label29;
	TLabel *Label30;
	TLabel *Label31;
	TLabel *Label32;
	TEdit *EdtRatapit;
	TLabel *Label33;
	TEdit *EdtNousu;
	TMenuItem *Merkitsekaikkienratojenpituudetmaalinmukaan1;
	TLabel *LblVaroitus;
	TMenuItem *Nytrastit1;
	void __fastcall CBRadatChange(TObject *Sender);
	void __fastcall BtnTarkistaClick(TObject *Sender);
	void __fastcall SGLeimatClick(TObject *Sender);
	void __fastcall SGRataClick(TObject *Sender);
	void __fastcall BtnRataTyhjennaClick(TObject *Sender);
	void __fastcall BtnRataLisaaRastiClick(TObject *Sender);
	void __fastcall BtnRataPoistaRastiClick(TObject *Sender);
	void __fastcall BtnTallRataClick(TObject *Sender);
	void __fastcall EdtRNimiChange(TObject *Sender);
	void __fastcall BtnSuljeClick(TObject *Sender);
	void __fastcall BtnTallKooditClick(TObject *Sender);
	void __fastcall BtnLeimatLisaaRastiClick(TObject *Sender);
	void __fastcall BtnLeimatPoistaRastiClick(TObject *Sender);
	void __fastcall BtnTyhjKooditClick(TObject *Sender);
	void __fastcall BtnPeruutaKoodiClick(TObject *Sender);
	void __fastcall BtnPoistaRataClick(TObject *Sender);
	void __fastcall CBTiedLajiChange(TObject *Sender);
	void __fastcall BtnImportClick(TObject *Sender);
	void __fastcall TabSheet1Show(TObject *Sender);
	void __fastcall BtnLisKaikkiClick(TObject *Sender);
	void __fastcall BtnLisValitutClick(TObject *Sender);
	void __fastcall BtnPoisValitutClick(TObject *Sender);
	void __fastcall BtnPoisKaikkiClick(TObject *Sender);
	void __fastcall BtnTulostaClick(TObject *Sender);
	void __fastcall BtnOtaKayttoonClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall LBRadatDblClick(TObject *Sender);
	void __fastcall LBRataValDblClick(TObject *Sender);
	void __fastcall CBMaaliChange(TObject *Sender);
	void __fastcall SGRataMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall SGRataMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall SGLeimatMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall SGLeimatMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall BtnLueXmlClick(TObject *Sender);
	void __fastcall BtnKirjXmlClick(TObject *Sender);
	void __fastcall CBLahtoChange(TObject *Sender);
	void __fastcall PageControl1Change(TObject *Sender);
	void __fastcall SGRataSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
	void __fastcall SGLeimatSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
	void __fastcall EdtRNimiExit(TObject *Sender);
	void __fastcall BtnOletuskooditClick(TObject *Sender);
	void __fastcall LuevanhaRADATLST1Click(TObject *Sender);
	void __fastcall KirjoitavanhaRADATLST1Click(TObject *Sender);
	void __fastcall LuevanhaLEIMATLST1Click(TObject *Sender);
	void __fastcall KirjoitavanhaLEIMATLST1Click(TObject *Sender);
	void __fastcall Kopioiradannollausennakkokaikkiinratoihin1Click(TObject *Sender);
	void __fastcall Kopioiradanmaalivalinnatkaikkiinratoihin1Click(TObject *Sender);
	void __fastcall Liskaikkiinratoihinmaalinrastikoodi1Click(TObject *Sender);
	void __fastcall Suljekaavake1Click(TObject *Sender);
	void __fastcall Lueradatjarastitxml1Click(TObject *Sender);
	void __fastcall allennaradatjarastit1Click(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall CBNaytaTallClick(TObject *Sender);
	void __fastcall BitBtn3Click(TObject *Sender);
	void __fastcall BitBtn4Click(TObject *Sender);
	void __fastcall BitBtn5Click(TObject *Sender);
	void __fastcall BitBtn6Click(TObject *Sender);
	void __fastcall Help1Click(TObject *Sender);
	void __fastcall RGUNimiClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall SGSarjatMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall SGSarjatMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall SGSarjatSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
	void __fastcall SGSarjatKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Avaakartta1Click(TObject *Sender);
	void __fastcall Nytrata1Click(TObject *Sender);
	void __fastcall KeyToUpper(TObject *Sender, System::WideChar &Key);
	void __fastcall BtnVainKoordClick(TObject *Sender);
	void __fastcall BtnTyhjKoordClick(TObject *Sender);
	void __fastcall BtnPoistaKoordClick(TObject *Sender);
	void __fastcall SGKoordSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
	void __fastcall SGKoordGetEditText(TObject *Sender, int ACol, int ARow, UnicodeString &Value);
	void __fastcall BtnKoordKayttoonClick(TObject *Sender);
	void __fastcall BtnLueKoordCsvClick(TObject *Sender);
	void __fastcall BtnKirjKoordCsvClick(TObject *Sender);
	void __fastcall MerkLahtokoodiClick(TObject *Sender);
	void __fastcall MerkMaalikoodiClick(TObject *Sender);
	void __fastcall Laskerastivlienmatkatkoordinaateista1Click(TObject *Sender);
	void __fastcall BtnPoistaKohdennusClick(TObject *Sender);
	void __fastcall BtnTyhjKohdennuksetClick(TObject *Sender);
	void __fastcall SGLeimatExit(TObject *Sender);
	void __fastcall Merkitsekaikkienratojenpituudetmaalinmukaan1Click(TObject *Sender);
	void __fastcall Nytrastit1Click(TObject *Sender);





private:	// User declarations
	int vertSarjat(IOFClassAssignment **CA);
	void tallSarjat(IOFClassAssignment **CA);
	int __fastcall TallKoord(rastikoordtp *koord);
	int AktLRow, AktLCol;
	int AktRRow, AktRCol;
	int RataValinnat[MAXRATA];
	bool rataMuutoksia;
	bool xmlLuettu;
	double tmpdbl;
public:		// User declarations
	__fastcall TFormRadat(TComponent* Owner);
	__fastcall ~TFormRadat(void);
	void __fastcall Lue_Xml_File(int lisaa);
	void sortkoodit(void);
	void tarkkoodit(void);
	int tarksarjat(void);
	void prtpiikit(tulostusparamtp *tulprm, int srj, int *n, int graaf);
	void nayta_rata(int krata);
	void __fastcall TallRata(ratatp *rata, bool sailSF);
	void Lue_Radat_Xml(wchar_t *ratafname, int lisaa);
	void lue_radat(wchar_t *fname);
	void kirj_radat(void);
	void nayta_leimat(void);
	void nayta_koordinaatit(void);
	void lue_leimat(wchar_t *fname);
	int kirjXmlTiedot(bool kysy, bool talldialog);
	void kirj_leimat(void);
	void RataItems(void);
	int importradat(void);
	bool RataValittu(int krata);
	void NaytaRatavalinnat(void);
	int tarkrata_re(int k, int varoita);
	void tulostapiikit(tulostusparamtp *tulprm);
	void t_prt(tulostusparamtp *tulprm);
	int tarkkilpailijat(void);
	bool haerata_re(wchar_t *rata);
	int tarkradat(void);
	int haerata(wchar_t *ratanimi);
	void tulostaradat(tulostusparamtp *tulprm);
	void prtrata(tulostusparamtp *tulprm, int srj, int *n);
	void tulostaleimasinkoodit(tulostusparamtp *tulprm);
	void tulostakoodit(tulostusparamtp *tulprm);
	ratatp *rata_re;
	ratatp trata;
	int kRata;
	ekditp koodit[2000];
	int t_piikit[11][16];
	int nrasti, n_erirasti, nrata_re, nkilp;
	INT kdirastit[256][5];
	IOFCourseData ReCourseData;
	wchar_t RataFName[200];
	wchar_t LeimaFName[200];
};
//---------------------------------------------------------------------------
extern PACKAGE TFormRadat *FormRadat;
//---------------------------------------------------------------------------
#endif
