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

#ifndef UnitJoukkuetiedotH
#define UnitJoukkuetiedotH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Menus.hpp>;
#include "VWinDef.h"
#include "UnitJjVaihdot.h"
#include "OsuusTiedot.h"
//---------------------------------------------------------------------------
class TFormJoukkuetiedot : public TForm
{
__published:	// IDE-managed Components
   TEdit *EdtKilpno;
   TLabel *Label1;
	TEdit *EdtSeura;
	TEdit *EdtMaa;
   TButton *SrjKarki;
   TButton *SeurBtn;
   TButton *EdBtn;
   TLabel *Label10;
   TComboBox *SrjVal;
	TButton *BtnSalli;
	TButton *BtnTallenna;
	TButton *BtnPeruuta;
	TButton *BtnSulje;
	TLabel *Tila;
	TStringGrid *OsGrid;
	TEdit *EdtJoukkue;
	TEdit *EdtJoukkNimi;
	TLabel *Label2;
	TLabel *LblSeuralyh;
	TLabel *LblMaa;
	TRadioGroup *RGSelaus;
	TEdit *EdtNimihaku;
	TLabel *Label6;
	TLabel *LblJoukkue;
	TLabel *Label7;
	TEdit *EdtJoukkId;
	TEdit *EdPisteet0;
	TLabel *Label4;
	TEdit *EdPisteet1;
	TBitBtn *BitBtn1;
	TEdit *EdPisteet2;
	TMainMenu *MainMenu1;
	TLabel *Label11;
	TEdit *EdtPiiri;
	TMenuItem *Haku1;
	TMenuItem *Emitkoodihaku1;
	TMenuItem *iedosto1;
	TMenuItem *Liskilpailija1;
	TMenuItem *Poistakilpailija1;
	TComboBox *CBSelOsuus;
	TLabel *Label3;
	TStringGrid *TlsGrid;
	TMenuItem *Henkilhaku1;
	TRadioGroup *RGVaTieto;
	TButton *BtnJjVaihto;
	TMenuItem *oistuvienlisystentila1;
	TMenuItem *N1;
	TMenuItem *Suljekaavake1;
	TMenuItem *Nyt1;
	TMenuItem *Lisenssinumero1;
	TMenuItem *Badge1;
	TButton *BtnPaivita;
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
	void __fastcall OsGridExit(TObject *Sender);
	void __fastcall EdtNimihakuChange(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall BtnSuljeClick(TObject *Sender);
	void __fastcall OsGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall OsGridSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall EdtKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EdtKilpnoKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EdtSeuraChange(TObject *Sender);
	void __fastcall EdtSeuraEnter(TObject *Sender);
	void __fastcall EdtJoukkNimiEnter(TObject *Sender);
	void __fastcall Emitkoodihaku1Click(TObject *Sender);
	void __fastcall Perusnumerohaku1Click(TObject *Sender);
	void __fastcall Liskilpailija1Click(TObject *Sender);
	void __fastcall Poistakilpailija1Click(TObject *Sender);
	void __fastcall EdtSeuraExit(TObject *Sender);
	void __fastcall EdtJoukkNimiExit(TObject *Sender);
	void __fastcall EdtJoukkueExit(TObject *Sender);
	void __fastcall EdtMaaExit(TObject *Sender);
	void __fastcall SrjValExit(TObject *Sender);
	void __fastcall EdtJoukkIdExit(TObject *Sender);
	void __fastcall EdPisteet0Exit(TObject *Sender);
	void __fastcall EdPisteet1Exit(TObject *Sender);
	void __fastcall EdPisteet2Exit(TObject *Sender);
	void __fastcall EdtPiiriExit(TObject *Sender);
	void __fastcall OsGridClick(TObject *Sender);
	void __fastcall OsGridKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall RGVaTietoClick(TObject *Sender);
	void __fastcall BtnJjVaihtoClick(TObject *Sender);
	void __fastcall TlsGridSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall TlsGridExit(TObject *Sender);
	void __fastcall TlsGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall TlsGridKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall TlsGridClick(TObject *Sender);
	void __fastcall OsGridDblClick(TObject *Sender);
	void __fastcall oistuvienlisystentila1Click(TObject *Sender);
	void __fastcall Suljekaavake1Click(TObject *Sender);
	void __fastcall Lisenssinumero1Click(TObject *Sender);
	void __fastcall Badge1Click(TObject *Sender);
	void __fastcall Henkilhaku1Click(TObject *Sender);
	void __fastcall BtnPaivitaClick(TObject *Sender);


private:	// User declarations
   int __fastcall tallennaTiedot(void);
//   void __fastcall haeTiedot(kilptietue *Kilp1);
	void __fastcall SiirraSeuraTiedot(int haku);
	void __fastcall GridDims(void);
	void __fastcall tyhjennaKaavake(void);
   int Sarja;
   int dKilp;
   bool Lisays;
   kilptietue Kilp, Kilp1;
   ColTp *OsGridCols;
   bool nimihaku;
   bool salliNumeromuutos;
   int vOs;
   int SelOsuus;
	int aktrow;
	int aktcol;
	int aktvarow;
	int aktvacol;
	int ColIx[100];
	int LSe;
	int EdKey;
	int laika;
public:		// User declarations
	TFormJjVaihdot *FormJjVaihdot;
	TFormOsuus *OsuusTiedot;
   bool sallimuokkausvalinta;
   bool sallimuokkaus;
	void __fastcall naytaKilpailija(int d);
	int __fastcall paivitaMuutos(int col, int row);
	int __fastcall paivitaVaMuutos(int col, int row);
   __fastcall TFormJoukkuetiedot(TComponent* Owner);
   wchar_t Kohde[30];
};
//---------------------------------------------------------------------------
extern TFormJoukkuetiedot *FormJoukkuetiedot;
//---------------------------------------------------------------------------
#endif
