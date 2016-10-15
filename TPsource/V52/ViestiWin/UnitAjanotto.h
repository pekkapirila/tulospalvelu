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
#include "VDef.h"

#define NAJANOTOT 4
#define MAXAJCOL 12

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
	int AikaLisays;
	int ColW[MAXAJCOL];
	int ColOn[MAXAJCOL];
	ajatIkkParamClass(void);
	void operator=(ajatIkkParamClass&);
	int writeParams(TextFl *outfl, int level);
	int readParams(xml_node *node, int *inode, int nnode);
};

typedef cellTp rowTp[MAXAJCOL];

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
	TMenuItem *N1;
	TMenuItem *Suljekaavake1;
	TMenuItem *Vaadivahvistus2;
	TPanel *Panel3;
	TLabel *Label12;
	TComboBox *CBOsVal;
	TMenuItem *Kaavakkeenoletuspiste1;
	TPanel *Panel2;
	TLabel *Label9;
	TComboBox *CBJono;
	TBitBtn *BitBtn1;
	TButton *BtnLuoUusi;
	TPanel *Panel1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label10;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TEdit *EdJono;
	TEdit *EdBadge;
	TEdit *EdPiste;
	TEdit *EdOsuus;
	TEdit *EdKno;
	TEdit *EdAika;
	TEdit *EdRivi;
	TEdit *EdPortti;
	TEdit *EdLahde;
	TButton *Button2;
	TButton *Button6;
	TButton *Button4;
	TButton *Button3;
	TRadioGroup *RGSyotto;
	TRadioGroup *RGNayta;
	TButton *BtnPaivita;
	TMenuItem *Salliennakoivanumerosytt1;
	TMenuItem *Kirjoitaajattekstitiedostoon1;
	TMenuItem *SynkronoiSirit1;
	TSaveDialog *SaveDialog1;
	TEdit *EdHaku;
	TButton *BtnHaku;
	TLabel *LblEiLoydy;
	TMenuItem *HaeMySQLkannasta1;
	TMenuItem *Korjaakello;
	TMenuItem *Ajanottojonojenvertailu1;
	TLabel *Label13;
	TEdit *EdDate;
	TMemo *MemoTCP;
	TMenuItem *Lueaikojatekstitiedostosta1;
	TOpenDialog *OpenDialog1;
	TDrawGrid *DGajat1;
	TDrawGrid *DGajat2;
	TMenuItem *Nytmysohitetutajat1;
	TMenuItem *Ajankasvatusmonistettaessa1;
	TMenuItem *Pieninmahdollinen01ms1;
	TMenuItem *Sijoitukseenvaikuttavalisys1;
	TPanel *Panel4;
	TLabel *Label11;
	TMemo *Memo1;
	TButton *Button8;
	TMenuItem *Ajanottolaitteet1;
	TMenuItem *Maalikello1;
	TMenuItem *Vaihdanytetynjonontiedosto1;
	TMenuItem *Nytosuusvalinta1;
	TLabel *Label14;
	TComboBox *CBPisteVal;
	void __fastcall BtnPaivitaClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall RGNaytaClick(TObject *Sender);
	void __fastcall DGajat1SelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall BtnLuoUusiClick(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall EnterPressed(TObject *Sender, System::WideChar &Key);
	void __fastcall EdAikaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EdKnoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Button6Click(TObject *Sender);
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
	void __fastcall RGSyottoClick(TObject *Sender);
	void __fastcall DGajat1KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall Vaaditallennuksillevahvistus1Click(TObject *Sender);
	void __fastcall Ajanottonppin1Click(TObject *Sender);
	void __fastcall DGajat1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Suljekaavake1Click(TObject *Sender);
	void __fastcall Vaadivahvistus2Click(TObject *Sender);
	void __fastcall Kaavakkeenoletuspiste1Click(TObject *Sender);
	void __fastcall Salliennakoivanumerosytt1Click(TObject *Sender);
	void __fastcall Kirjoitaajattekstitiedostoon1Click(TObject *Sender);
	void __fastcall SynkronoiSirit1Click(TObject *Sender);
	void __fastcall Button8Click(TObject *Sender);
	void __fastcall BtnHakuClick(TObject *Sender);
	void __fastcall HaeMySQLkannasta1Click(TObject *Sender);
	void __fastcall KorjaakelloClick(TObject *Sender);
	void __fastcall Ajanottojonojenvertailu1Click(TObject *Sender);
	void __fastcall EdDateKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Memo1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Memo1KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EdHakuKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EdHakuKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall DGajat2KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Lueaikojatekstitiedostosta1Click(TObject *Sender);
	void __fastcall DGajat1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall DGajat2DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall Nytmysohitetutajat1Click(TObject *Sender);
	void __fastcall DGajat1SetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
	void __fastcall DGajat1GetEditText(TObject *Sender, int ACol, int ARow, UnicodeString &Value);
	void __fastcall Pieninmahdollinen01ms1Click(TObject *Sender);
	void __fastcall Sijoitukseenvaikuttavalisys1Click(TObject *Sender);
	void __fastcall Maalikello1Click(TObject *Sender);
	void __fastcall Vaihdanytetynjonontiedosto1Click(TObject *Sender);
	void __fastcall Nytosuusvalinta1Click(TObject *Sender);
	void __fastcall CBPisteValClick(TObject *Sender);
	void __fastcall DGajat1Exit(TObject *Sender);



private:	// User declarations
	void __fastcall GridDrawCell(int grid, TObject *Sender, int ACol,
		int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall lisaaAika(void);
	void __fastcall tallVertRivi(void);
	bool __fastcall vertRiviMuutt(void);
	void __fastcall SarjaLahto(int row, wchar_t val);
	void __fastcall Aktivoi(int row, int status);
	void __fastcall naytaTCPstatus(void);
	void uusintaTallennus(int rivi);
	TDrawGrid *DGajat[2];
	rowTp *Cells[2];
	int CellRows[2];
	UnicodeString VertRivi[5];
	int SyottoTila;
	bool AikaKey;
	aikatp eAika;
	bool Pakota;
	TShiftState SG1KeyShift;
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
	int Jono;
	bool Ennakoi;
	int Ajanotto;
	bool OnLine;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAjanotto *FormAjanotot[NAJANOTOT];
//---------------------------------------------------------------------------
#endif
