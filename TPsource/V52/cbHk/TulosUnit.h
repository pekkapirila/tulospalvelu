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

#ifndef TulosUnitH
#define TulosUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <Vcl.Buttons.hpp>
#include "UnitKilpailijaOnline.h"
#include "HkWinDef.h"
#include <Vcl.Dialogs.hpp>
#define NAIKASAR 31

typedef cellTp cellTpA[VALUKU+13];
typedef struct {
   unsigned short sijat[VALUKU+2];
   int d;
   }sjjarjtp;

class TScrollGrid : public TCustomGrid
{
public:
	void __fastcall SInvalidateRow(int ARow);
};

typedef union {
	UINT32 Colors[10];
	struct {
		UINT32 FirstRowBackColor;
		UINT32 FirstColBackColor;
		UINT32 WinBackColor;
		UINT32 AltBackColor;
		UINT32 korBG1;
		UINT32 korBG2;
		UINT32 FirstRowColor;
		UINT32 FontColor;
		UINT32 FontColor2;
		UINT32 FontColor3;
		};
} colorsettp;

//---------------------------------------------------------------------------
class TTilanneForm : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *Nyt1;
	TMenuItem *Seuralyhenne1;
	TMenuItem *Pitkseuranimi1;
	TMenuItem *Yhdistys1;
	TMenuItem *Maatunnus1;
	TMenuItem *N1;
	TMenuItem *Kirjasin1;
	TMenuItem *Suuri1;
	TMenuItem *Normaali1;
	TMenuItem *Pieni1;
	TMenuItem *N2;
	TMenuItem *Lihavoitu1;
	TMenuItem *Yhteistulos1;
	TMenuItem *Valinnat1;
	TMenuItem *Seuraalhtj1;
	TMenuItem *Pisteet1;
	TMenuItem *Kaikkiajat1;
	TMenuItem *Maali1;
	TMenuItem *N1vliaika1;
	TMenuItem *N2vliaika1;
	TMenuItem *N3vliaika1;
	TMenuItem *N4vliaika1;
	TMenuItem *N5vliaika1;
	TMenuItem *N6vliaika1;
	TMenuItem *N7vliaika1;
	TMenuItem *N8vliaka1;
	TMenuItem *N9vliaika1;
	TMenuItem *N10vliaikaA1;
	TMenuItem *N11vliaikaB1;
	TMenuItem *N12vliaikaC1;
	TMenuItem *N13vliaikaD1;
	TMenuItem *N14vliaikaE1;
	TMenuItem *N15vliaikaF1;
	TMenuItem *N16vliaikaG1;
	TMenuItem *Lhtaika1;
	TMenuItem *Vainjrjestyspiste1;
	TMenuItem *N3;
	TMenuItem *Erokrkeen1;
	TMenuItem *lyksjrjestys1;
	TDrawGrid *TulosGrid;
	TMenuItem *Korostusaikarajat1;
	TMenuItem *Etunimiensin1;
	TMenuItem *N17vliaikaH1;
	TMenuItem *N18vliaikaI1;
	TMenuItem *N19vliaikaJ1;
	TMenuItem *N20vliaikaK1;
	TMenuItem *N21vliaikaL1;
	TMenuItem *N22vliaikaM1;
	TMenuItem *N23vliaikaN1;
	TMenuItem *N24vliaikaO1;
	TMenuItem *N25vliaikaP1;
	TMenuItem *N26vliaikaQ1;
	TMenuItem *N27vliaikaR1;
	TMenuItem *N28vliaikaS1;
	TMenuItem *N29vliaikaT1;
	TMenuItem *N30vliaikaU1;
	TMenuItem *akaaajonvliaikatilanne1;
	TMenuItem *Rajaukset;
	TMenuItem *Alisarja1;
	TMenuItem *oiminto1;
	TMenuItem *Skrollaa1;
	TMenuItem *Kokoruutu1;
	TPanel *Panel1;
	TButton *PaivBtn;
	TComboBox *SrjVal;
	TComboBox *PisteVal;
	TCheckBox *HylVal;
	TEdit *EditKello;
	TCheckBox *CBKaikkiPist;
	TCheckBox *CBSmart;
	TLabel *LblSp;
	TEdit *EdtSp;
	TLabel *LblIkasarja;
	TEdit *EdtIkasarja;
	TBitBtn *BitBtn1;
	TMenuItem *Lopetaskrollaus1;
	TMenuItem *Pehmeskrollaus1;
	TMenuItem *aukosarjanptteeksi1;
	TMenuItem *Sarjanminimikesto1;
	TMenuItem *Skrollauksensarjalista1;
	TMenuItem *Liukuva1;
	TMenuItem *Rivittinaskeltava1;
	TMenuItem *Ruutukerrallaan1;
	TMenuItem *Julkisuustasonmukaan1;
	TMenuItem *Vainjulkiset1;
	TMenuItem *aso11;
	TMenuItem *aso21;
	TMenuItem *aso31;
	TMenuItem *Sulje1;
	TMenuItem *Naytapiilotetut;
	TMenuItem *Nytodotetut;
	TMenuItem *Er1;
	TMenuItem *Laskeyhteispisteet;
	TMenuItem *N4;
	TMenuItem *Aikojennytttarkkuus1;
	TMenuItem *N1sek;
	TMenuItem *N01sek;
	TMenuItem *N001sek;
	TMenuItem *N0001sek;
	TMenuItem *Muumerkkikoko1;
	TMenuItem *N5;
	TMenuItem *allennamuotoilu1;
	TMenuItem *Nytkierrosaika1;
	TMenuItem *Nytviimeisinaika1;
	TMenuItem *Omaenntys1;
	TMenuItem *avoiteaika1;
	TMenuItem *Sallikaikkiensarakkeidenst1;
	TMenuItem *Lhdnnimerkki1;
	TMenuItem *Variskeema1;
	TMenuItem *Oletusvrit1;
	TMenuItem *Mustapohja1;
	TMenuItem *Omatvrit1;
	TMenuItem *Lueomatvrit1;
	TOpenDialog *OpenDialog1;
	TMenuItem *Seuraasarjanviimeisisijoja1;
   void __fastcall PaivBtnClick(TObject *Sender);
   void __fastcall SrjValChange(TObject *Sender);
   void __fastcall PisteValChange(TObject *Sender);
   void __fastcall JarjValClick(TObject *Sender);
   void __fastcall ViimeisimmatClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
	void __fastcall HylValClick(TObject *Sender);
	void __fastcall Pitkseuranimi1Click(TObject *Sender);
	void __fastcall Seuralyhenne1Click(TObject *Sender);
	void __fastcall Yhdistys1Click(TObject *Sender);
	void __fastcall Maatunnus1Click(TObject *Sender);
	void __fastcall Suuri1Click(TObject *Sender);
	void __fastcall Normaali1Click(TObject *Sender);
	void __fastcall Pieni1Click(TObject *Sender);
	void __fastcall Lihavoitu1Click(TObject *Sender);
	void __fastcall TulosGridSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall TulosGridDblClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall Yhteistulos1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Seuraalhtj1Click(TObject *Sender);
	void __fastcall Erokrkeen1Click(TObject *Sender);
	void __fastcall Maali1Click(TObject *Sender);
	void __fastcall N1vliaika1Click(TObject *Sender);
	void __fastcall N2vliaika1Click(TObject *Sender);
	void __fastcall N3vliaika1Click(TObject *Sender);
	void __fastcall N4vliaika1Click(TObject *Sender);
	void __fastcall N5vliaika1Click(TObject *Sender);
	void __fastcall N6vliaika1Click(TObject *Sender);
	void __fastcall N7vliaika1Click(TObject *Sender);
	void __fastcall N8vliaka1Click(TObject *Sender);
	void __fastcall N9vliaika1Click(TObject *Sender);
	void __fastcall N10vliaikaA1Click(TObject *Sender);
	void __fastcall N11vliaikaB1Click(TObject *Sender);
	void __fastcall N12vliaikaC1Click(TObject *Sender);
	void __fastcall N13vliaikaD1Click(TObject *Sender);
	void __fastcall N14vliaikaE1Click(TObject *Sender);
	void __fastcall N15vliaikaF1Click(TObject *Sender);
	void __fastcall N16vliaikaG1Click(TObject *Sender);
	void __fastcall N17vliaikaH1Click(TObject *Sender);
	void __fastcall N18vliaikaI1Click(TObject *Sender);
	void __fastcall N19vliaikaJ1Click(TObject *Sender);
	void __fastcall N20vliaikaK1Click(TObject *Sender);
	void __fastcall N21vliaikaL1Click(TObject *Sender);
	void __fastcall N22vliaikaM1Click(TObject *Sender);
	void __fastcall N23vliaikaN1Click(TObject *Sender);
	void __fastcall N24vliaikaO1Click(TObject *Sender);
	void __fastcall N25vliaikaP1Click(TObject *Sender);
	void __fastcall N26vliaikaQ1Click(TObject *Sender);
	void __fastcall N27vliaikaR1Click(TObject *Sender);
	void __fastcall N28vliaikaS1Click(TObject *Sender);
	void __fastcall N29vliaikaT1Click(TObject *Sender);
	void __fastcall N30vliaikaU1Click(TObject *Sender);
	void __fastcall Kaikkiajat1Click(TObject *Sender);
	void __fastcall Vainjrjestyspiste1Click(TObject *Sender);
	void __fastcall Lhtaika1Click(TObject *Sender);
	void __fastcall TulosGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
		  TGridDrawState State);
	void __fastcall lyksjrjestys1Click(TObject *Sender);
	void __fastcall CBKaikkiPistClick(TObject *Sender);
	void __fastcall CBSmartClick(TObject *Sender);
	void __fastcall Korostusaikarajat1Click(TObject *Sender);
	void __fastcall TulosGridFixedCellClick(TObject *Sender, int ACol, int ARow);
	void __fastcall Etunimiensin1Click(TObject *Sender);
	void __fastcall TulosGridMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall TulosGridMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall TulosGridClick(TObject *Sender);
	void __fastcall akaaajonvliaikatilanne1Click(TObject *Sender);
	void __fastcall RajauksetClick(TObject *Sender);
	void __fastcall Alisarja1Click(TObject *Sender);
	void __fastcall Skrollaa1Click(TObject *Sender);
	void __fastcall Kokoruutu1Click(TObject *Sender);
	void __fastcall Lopetaskrollaus1Click(TObject *Sender);
	void __fastcall aukosarjanptteeksi1Click(TObject *Sender);
	void __fastcall Sarjanminimikesto1Click(TObject *Sender);
	void __fastcall Skrollauksensarjalista1Click(TObject *Sender);
	void __fastcall Liukuva1Click(TObject *Sender);
	void __fastcall Rivittinaskeltava1Click(TObject *Sender);
	void __fastcall Ruutukerrallaan1Click(TObject *Sender);
	void __fastcall Vainjulkiset1Click(TObject *Sender);
	void __fastcall aso11Click(TObject *Sender);
	void __fastcall aso21Click(TObject *Sender);
	void __fastcall aso31Click(TObject *Sender);
	void __fastcall Sulje1Click(TObject *Sender);
	void __fastcall NaytapiilotetutClick(TObject *Sender);
	void __fastcall NytodotetutClick(TObject *Sender);
	void __fastcall Er1Click(TObject *Sender);
	void __fastcall LaskeyhteispisteetClick(TObject *Sender);
	void __fastcall N1sekClick(TObject *Sender);
	void __fastcall N01sekClick(TObject *Sender);
	void __fastcall N001sekClick(TObject *Sender);
	void __fastcall N0001sekClick(TObject *Sender);
	void __fastcall Muumerkkikoko1Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall allennamuotoilu1Click(TObject *Sender);
	void __fastcall Nytkierrosaika1Click(TObject *Sender);
	void __fastcall Nytviimeisinaika1Click(TObject *Sender);
	void __fastcall Omaenntys1Click(TObject *Sender);
	void __fastcall avoiteaika1Click(TObject *Sender);
	void __fastcall Sallikaikkiensarakkeidenst1Click(TObject *Sender);
	void __fastcall Lhdnnimerkki1Click(TObject *Sender);
	void __fastcall Oletusvrit1Click(TObject *Sender);
	void __fastcall Mustapohja1Click(TObject *Sender);
	void __fastcall Omatvrit1Click(TObject *Sender);
	void __fastcall Lueomatvrit1Click(TObject *Sender);
	void __fastcall Seuraasarjanviimeisisijoja1Click(TObject *Sender);

private:	// User declarations
	MESSAGE void __fastcall ScrollMsgHandler(TMyMessage &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYSCROLL, TMyMessage, ScrollMsgHandler);
END_MESSAGE_MAP(TComponent)

	void __fastcall PisteClick(int piste);
	bool __fastcall OnAlisarja(kilptietue *kilp);
	void __fastcall poistaPisteValinnat(void);
	void InitColors(void);
	int CurrentRow;
	int CurrentCol;
	TMenuItem *NaytaPiste[NAIKASAR];
	int CNro;
	int CNimi;
	int CViimeisin;
	int CAlisarja;
	int CSeura;
	int CMaa;
	int CTulos;
	int CEro;
	int CEnn;
	int CTav;
	int vavali[VALUKU+1][3];
	int ColLj[20];
	int lev;
	int pVal;
	bool ViimeisetRivit;
public:		// User declarations
	TFormKilpailijaOnline *FormKilpailijaOnlineTilanne;
   __fastcall TTilanneForm(TComponent* Owner);
   __fastcall ~TTilanneForm(void);
	void __fastcall PaivitaGrid(bool scroll);
//	void ScrollThread(LPVOID);
	void __fastcall FormSetup(bool pakota, bool paivitaLeveys);
	int __fastcall SmartJarj(int ed, int *nTls);
	int applyParams(void);
   seurIkkParamClass IkkParam;
   seurIkkParamClass IkkParamEd;
   int Yhd;
   CRITICAL_SECTION tls_CriticalSection;
   int paivitysflag;
   wchar_t Kohde[30];
   int LA;
   int naytapiste[VALUKU+1];
   int npiste;
   int YTulVal;
   cellTpA OtsCells;
   cellTpA *Cells;
   int nRivi;
   int nRiviEd;
   sjjarjtp *sjjarj;
   int RowClicked;
   int ScrollViive;
   int ScrollSrjKesto;
   int sc;
   int alkuhetki, loppuhetki;
   int TilanneFormNo;
   int kScrollSarja;
   bool FixedCols;
   int edAani;
   int edEnn;
   int aikaNyt;
   colorsettp ColorSet[3];
};
//---------------------------------------------------------------------------
extern PACKAGE TTilanneForm *TilanneForm;
//---------------------------------------------------------------------------
#endif
