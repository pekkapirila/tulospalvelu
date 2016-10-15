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

#ifndef UnitKilpailijatiedotH
#define UnitKilpailijatiedotH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include "HkWinDef.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Menus.hpp>;
//---------------------------------------------------------------------------
class TFormKilpailijatiedot : public TForm
{
__published:	// IDE-managed Components
   TEdit *EdtKilpno;
   TLabel *Label1;
	TEdit *EdtSukunimi;
	TEdit *EdtMaa;
   TLabel *Label10;
   TEdit *EdtSeura;
   TComboBox *SrjVal;
	TButton *BtnSalli;
	TButton *BtnTallenna;
	TButton *BtnPeruuta;
	TButton *BtnSulje;
	TLabel *Tila;
	TStringGrid *PvGrid;
	TEdit *EdtEtunimi;
	TEdit *EdtArvo;
	TEdit *EdtSeuralyh;
	TEdit *EdtYhd;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *LblArvo;
	TLabel *LblSeuralyh;
	TLabel *LblYhd;
	TLabel *LblMaa;
	TLabel *Label5;
	TLabel *LblJoukkue;
	TEdit *EdtJoukkue;
	TEdit *EdtPerusNo;
	TLabel *LblPerusNo;
	TLabel *Label7;
	TEdit *EdtLisno;
	TEdit *EdtKvId;
	TLabel *Label8;
	TButton *BtnOnline;
	TButton *BtnEmit;
	TLabel *Label4;
	TEdit *EdPisteet;
	TBitBtn *BitBtn1;
	TLabel *Label9;
	TEdit *EdRankipisteet;
	TMainMenu *MainMenu1;
	TMenuItem *Valinnat1;
	TMenuItem *Pivitseura1;
	TMenuItem *Nytseuraluettelo1;
	TLabel *Label11;
	TEdit *EdtPiiri;
	TMenuItem *Haku1;
	TMenuItem *Emitkoodihaku1;
	TMenuItem *Lisenssinumerohaku1;
	TMenuItem *Perusnumerohaku1;
	TMenuItem *iedosto1;
	TMenuItem *Liskilpailija1;
	TMenuItem *Poistakilpailija1;
	TEdit *EdtSp;
	TLabel *Label12;
	TEdit *EdtIkasrj;
	TLabel *Label13;
	TEdit *EdtSynt;
	TLabel *Label14;
	TMenuItem *Vaihemuutokset1;
	TMenuItem *Suljekaavake1;
	TGroupBox *GBHaku;
	TRadioGroup *RGSelaus;
	TLabel *Label6;
	TEdit *EdtNimihaku;
	TButton *SrjKarki;
	TButton *EdBtn;
	TButton *SeurBtn;
	TButton *BtnPaivita;
	TMenuItem *Nytomaenntys1;
	TMenuItem *Nyttavoitetulos1;
	TMenuItem *Pisteet2ei1;
	TMenuItem *Vaihepisteet2ei1;
	TMenuItem *Sakkoinamsekkorjaus;
	TComboBox *CBsalanimi;
	TLabel *Label15;
   void __fastcall EdtKilpnoChanged(TObject *Sender);
   void __fastcall naytaTiedot(void);
   void __fastcall SrjKarkiBtn(TObject *Sender);
   void __fastcall SeurBtnClick(TObject *Sender);
   void __fastcall EdBtnClick(TObject *Sender);
   void __fastcall EdtKilpnoKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift);
   void __fastcall SrjValChange(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
	void __fastcall BtnSalliClick(TObject *Sender);
	void __fastcall BtnPeruutaClick(TObject *Sender);
	void __fastcall BtnTallennaClick(TObject *Sender);
	void __fastcall PvGridExit(TObject *Sender);
	void __fastcall EdtNimihakuChange(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall BtnSuljeClick(TObject *Sender);
	void __fastcall BtnOnlineClick(TObject *Sender);
	void __fastcall BtnEmitClick(TObject *Sender);
	void __fastcall PvGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall PvGridSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall EdtKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EdtKilpnoKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall Nytseuraluettelo1Click(TObject *Sender);
	void __fastcall Pivitseura1Click(TObject *Sender);
	void __fastcall EdtSeuraKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EdtSeuraChange(TObject *Sender);
	void __fastcall EdtSeuraEnter(TObject *Sender);
	void __fastcall EdtSeuralyhEnter(TObject *Sender);
	void __fastcall Emitkoodihaku1Click(TObject *Sender);
	void __fastcall Lisenssinumerohaku1Click(TObject *Sender);
	void __fastcall Perusnumerohaku1Click(TObject *Sender);
	void __fastcall Liskilpailija1Click(TObject *Sender);
	void __fastcall Poistakilpailija1Click(TObject *Sender);
	void __fastcall EdtPerusNoExit(TObject *Sender);
	void __fastcall EdtSukunimiExit(TObject *Sender);
	void __fastcall EdtEtunimiExit(TObject *Sender);
	void __fastcall EdtArvoExit(TObject *Sender);
	void __fastcall EdtSeuraExit(TObject *Sender);
	void __fastcall EdtSeuralyhExit(TObject *Sender);
	void __fastcall EdtYhdExit(TObject *Sender);
	void __fastcall EdtJoukkueExit(TObject *Sender);
	void __fastcall EdtMaaExit(TObject *Sender);
	void __fastcall SrjValExit(TObject *Sender);
	void __fastcall EdtLisnoExit(TObject *Sender);
	void __fastcall EdtKvIdExit(TObject *Sender);
	void __fastcall EdPisteetExit(TObject *Sender);
	void __fastcall EdRankipisteetExit(TObject *Sender);
	void __fastcall EdtPiiriExit(TObject *Sender);
	void __fastcall PvGridClick(TObject *Sender);
	void __fastcall PvGridKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EdtSpChange(TObject *Sender);
	void __fastcall EdtIkasrjExit(TObject *Sender);
	void __fastcall EdtSyntExit(TObject *Sender);
	void __fastcall Vaihemuutokset1Click(TObject *Sender);
	void __fastcall Suljekaavake1Click(TObject *Sender);
	void __fastcall BtnPaivitaClick(TObject *Sender);
	void __fastcall Nytomaenntys1Click(TObject *Sender);
	void __fastcall Nyttavoitetulos1Click(TObject *Sender);
	void __fastcall Pisteet2ei1Click(TObject *Sender);
	void __fastcall Vaihepisteet2ei1Click(TObject *Sender);
	void __fastcall SakkoinamsekkorjausClick(TObject *Sender);
	void __fastcall CBsalanimiClick(TObject *Sender);

private:	// User declarations
   int __fastcall tallennaTiedot(void);
//   void __fastcall haeTiedot(kilptietue *Kilp1);
	void __fastcall SiirraSeuraTiedot(int haku);
   int Sarja;
   int dKilp;
   bool Lisays;
   kilptietue Kilp, Kilp1;
   ColTp *PvGridCols;
   bool nimihaku;
   int epv;
   int vpv;
	int aktrow;
	int aktcol;
	int ColIx[100];
	int LSe;
	int EdKey;
	int PvPist;
	int ValPist;
public:		// User declarations
   bool sallimuokkausvalinta;
   bool sallimuokkaus;
	void __fastcall naytaKilpailija(int d);
	int __fastcall paivitaMuutos(int col, int row);
   __fastcall TFormKilpailijatiedot(TComponent* Owner);
   wchar_t Kohde[30];
};
//---------------------------------------------------------------------------
extern TFormKilpailijatiedot *FormKilpailijatiedot;
//---------------------------------------------------------------------------
#endif
