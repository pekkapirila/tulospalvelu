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

#ifndef UnitAjanottoH
#define UnitAjanottoH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include "HkWinDef.h"

#define NAJANOTOT 4
#define MAXAJCOL 11

typedef cellTp rowTp[MAXAJCOL];

class ajatIkkParamClass {
public:
	bool Ennakoi;
	bool Suppeat;
	int VaadiVahvistus;
	int Jono;
	int SyottoTila;
	int NayttoTila;
	int KeyCode;
	TShiftState KeyShift;
	int NappainPiste;
	int OletusPiste;
	int AikaLisays;
	int FontSize;
	int ViimOsuus;
	int ColW[MAXAJCOL];
	int ColOn[MAXAJCOL];
	ajatIkkParamClass(void);
	void operator=(ajatIkkParamClass&);
	int writeParams(TextFl *outfl, int level);
	int readParams(xml_node *node, int *inode, int nnode);
};

//---------------------------------------------------------------------------
class TFormAjanotto : public TForm
{
__published:	// IDE-managed Components
	TMainMenu *MainMenu1;
	TMenuItem *Toiminnot1;
	TMenuItem *emiTagohjaus1;
	TMenuItem *Valinnat1;
	TMenuItem *Vaaditallennuksillevahvistus1;
	TMenuItem *Ajanottonppin1;
	TPanel *Panel2;
	TButton *BtnPaivita;
	TRadioGroup *RGNayta;
	TPanel *Panel1;
	TLabel *Label8;
	TLabel *Label7;
	TLabel *Label6;
	TLabel *Label5;
	TLabel *Label4;
	TLabel *Label3;
	TLabel *Label2;
	TLabel *Label1;
	TButton *BtnTall2;
	TButton *BtnPoista;
	TButton *BtnPeruuta;
	TButton *BtnSeur;
	TEdit *EdJono;
	TEdit *EdLaji;
	TEdit *EdLahde;
	TEdit *EdBadge;
	TEdit *EdPiste;
	TEdit *EdKno;
	TEdit *EdAika;
	TEdit *EdRivi;
	TButton *BtnLuoUusi;
	TBitBtn *BitBtn1;
	TComboBox *CBJono;
	TLabel *Label9;
	TMenuItem *Nytsuppeattiedot1;
	TMenuItem *SynkronoiSirit1;
	TMenuItem *Sulje1;
	TMenuItem *Vaihdaer1;
	TMenuItem *Kirjoitaajattekstitiedostoon1;
	TSaveDialog *SaveDialog1;
	TMenuItem *Vaadivahvistus2;
	TPanel *Panel3;
	TButton *Button1;
	TMemo *Memo1;
	TLabel *Label10;
	TButton *Button2;
	TMenuItem *Salliennakoivanumerosytt1;
	TEdit *EdHaku;
	TButton *BtnHaku;
	TLabel *LblEiLoydy;
	TMenuItem *HaeMySQLkannasta1;
	TMenuItem *Kirjasinkoko1;
	TMenuItem *Korjaakello;
	TMenuItem *Ajanottojonojenvertailu1;
	TLabel *Label11;
	TEdit *EdDate;
	TMenuItem *KynnistSirityhteysuudelleen1;
	TMenuItem *Sirityhteydenpollaus1;
	TMemo *MemoTCP;
	TMenuItem *oistuvantiedonvaroitus1;
	TMenuItem *Lueaikojatekstitiedostosta1;
	TOpenDialog *OpenDialog1;
	TMenuItem *Nytmysohitetutajat1;
	TDrawGrid *DGajat1;
	TDrawGrid *DGajat2;
	TLabel *Label12;
	TComboBox *CBSyotto;
	TMenuItem *Ajankasvatusmonistettaessa1;
	TMenuItem *Pieninmahdollinen01ms1;
	TMenuItem *Sijoitukseenvaikuttavalisys1;
	TMenuItem *Erikoistoiminnot1;
	TMenuItem *Avaadebugtiedosto1;
	TMenuItem *Suljedebugtiedosto1;
	TMenuItem *Viimeistenaikojenosuusnytst1;
	TMenuItem *Ajanottolaitteet1;
	TMenuItem *Maalikello1;
	TMenuItem *Vaihdanytetynaikajonontiedosto1;
	TMenuItem *Ert1;
	TMenuItem *Seuraavaer1;
	TMenuItem *Edellinener1;
	TMenuItem *Lhtryhmnkoko1;
	TMenuItem *Pieninajoissankyv1ms1;
	TMenuItem *HakuEmitinpalvelimelta1;
	TMenuItem *Kaavakkeenoletuspiste1;
	TLabel *Label14;
	TComboBox *CBPisteVal;
	TMenuItem *Vritlhdekoodinperusteella1;
	void __fastcall BtnPaivitaClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall RGNaytaClick(TObject *Sender);
	void __fastcall DGajat1SelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall BtnLuoUusiClick(TObject *Sender);
	void __fastcall BtnTall2Click(TObject *Sender);
	void __fastcall BtnPeruutaClick(TObject *Sender);
	void __fastcall BtnPoistaClick(TObject *Sender);
	void __fastcall EnterPressed(TObject *Sender, System::WideChar &Key);
	void __fastcall EdAikaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EdKnoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall BtnSeurClick(TObject *Sender);
	void __fastcall EdPisteKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CBJonoChange(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall DGajat1MouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall DGajat1MouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall emiTagohjaus1Click(TObject *Sender);
	void __fastcall DGajat1KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall Vaaditallennuksillevahvistus1Click(TObject *Sender);
	void __fastcall Ajanottonppin1Click(TObject *Sender);
	void __fastcall DGajat1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Nytsuppeattiedot1Click(TObject *Sender);
	void __fastcall SynkronoiSirit1Click(TObject *Sender);
	void __fastcall Sulje1Click(TObject *Sender);
	void __fastcall Vaihdaer1Click(TObject *Sender);
	void __fastcall Kirjoitaajattekstitiedostoon1Click(TObject *Sender);
	void __fastcall Vaadivahvistus2Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Salliennakoivanumerosytt1Click(TObject *Sender);
	void __fastcall BtnHakuClick(TObject *Sender);
	void __fastcall HaeMySQLkannasta1Click(TObject *Sender);
	void __fastcall Kirjasinkoko1Click(TObject *Sender);
	void __fastcall KorjaakelloClick(TObject *Sender);
	void __fastcall Ajanottojonojenvertailu1Click(TObject *Sender);
	void __fastcall Memo1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EdHakuKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EdHakuKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall Memo1KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EdDateKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall KynnistSirityhteysuudelleen1Click(TObject *Sender);
	void __fastcall Sirityhteydenpollaus1Click(TObject *Sender);
	void __fastcall DGajat2KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall oistuvantiedonvaroitus1Click(TObject *Sender);
	void __fastcall Lueaikojatekstitiedostosta1Click(TObject *Sender);
	void __fastcall Nytmysohitetutajat1Click(TObject *Sender);
	void __fastcall DGajat1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall DGajat2DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall CBSyottoClick(TObject *Sender);
	void __fastcall DGajat1SetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
	void __fastcall DGajat1GetEditText(TObject *Sender, int ACol, int ARow, UnicodeString &Value);
	void __fastcall Pieninmahdollinen01ms1Click(TObject *Sender);
	void __fastcall Sijoitukseenvaikuttavalisys1Click(TObject *Sender);
	void __fastcall Avaadebugtiedosto1Click(TObject *Sender);
	void __fastcall Suljedebugtiedosto1Click(TObject *Sender);
	void __fastcall Viimeistenaikojenosuusnytst1Click(TObject *Sender);
	void __fastcall Maalikello1Click(TObject *Sender);
	void __fastcall Vaihdanytetynaikajonontiedosto1Click(TObject *Sender);
	void __fastcall Seuraavaer1Click(TObject *Sender);
	void __fastcall Edellinener1Click(TObject *Sender);
	void __fastcall Lhtryhmnkoko1Click(TObject *Sender);
	void __fastcall HakuEmitinpalvelimelta1Click(TObject *Sender);
	void __fastcall DGajat1Exit(TObject *Sender);
	void __fastcall Kaavakkeenoletuspiste1Click(TObject *Sender);
	void __fastcall CBPisteValChange(TObject *Sender);
	void __fastcall Vritlhdekoodinperusteella1Click(TObject *Sender);



private:	// User declarations
	void __fastcall GridDrawCell(int grid, TObject *Sender, int ACol,
		int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall lisaaAika(void);
	int __fastcall SarjaLahto(int row, wchar_t val);
	void __fastcall Aktivoi(int row, int status);
	void __fastcall tallVertRivi(void);
	bool __fastcall vertRiviMuutt(void);
	void __fastcall SarakeLeveydet(void);
	void __fastcall naytaTCPstatus(void);
	void uusintaTallennus(int rivi);
	void kasitteleSpecialKey(WORD &Key, TShiftState Shift);
	TDrawGrid *DGajat[2];
	rowTp *Cells[2];
	int CellRows[2];
	int GridW;
	int edRow;
	int edCol;
	UnicodeString VertRivi[4];
	aikatp eAika;
	TShiftState SG1KeyShift;
	bool AikaKey;
	wchar_t varitettavat[52];
public:		// User declarations
	__fastcall TFormAjanotto(TComponent* Owner);
	void __fastcall naytaRivi(int grid, int r, int rw, int jono);
	void __fastcall naytaAjat1(void);
	void __fastcall naytaAjat2(void);
	void __fastcall naytaAktAika(void);
	void Paivita_Aikanaytto(void);
	int applyParams(void);
	ajatIkkParamClass IkkParam;
	int YRivi1;
	int aktRivi1;
	int NRivi1;
	int Ajanotto;
	bool OnLine;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAjanotto *FormAjanotot[NAJANOTOT];
//---------------------------------------------------------------------------
#endif
