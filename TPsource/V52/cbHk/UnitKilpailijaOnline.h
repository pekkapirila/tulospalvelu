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

#ifndef UnitKilpailijaOnlineH
#define UnitKilpailijaOnlineH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include "HkWinDef.h"
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Buttons.hpp>;

//---------------------------------------------------------------------------
class TFormKilpailijaOnline : public TForm
{
__published:	// IDE-managed Components
   TEdit *EdtKilpno;
   TLabel *Label1;
	TEdit *EdtSukunimi;
	TEdit *EdtMaa;
   TButton *SrjKarki;
   TButton *SeurBtn;
   TButton *EdBtn;
   TLabel *Label10;
   TEdit *EdtSeura;
   TStringGrid *VaGrid;
   TComboBox *SrjVal;
	TButton *BtnSalli;
	TButton *BtnTallenna;
	TButton *BtnPeruuta;
	TButton *Button4;
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
	TRadioGroup *RGSelaus;
	TLabel *Label5;
	TEdit *EdtNimihaku;
	TLabel *Label6;
	TLabel *LblJoukkue;
	TEdit *EdtJoukkue;
	TCheckBox *CBSeuranta;
	TComboBox *CBSeurPiste;
	TLabel *LblSeurPiste;
	TDrawGrid *SeurGrid;
	TButton *BtnTietoa;
	TOpenDialog *OpenDialog1;
	TBitBtn *BitBtn1;
	TButton *BtnPaivita;
	TEdit *EdtEnn;
	TLabel *Label4;
	TEdit *EdtTav;
	TLabel *Label7;
   void __fastcall EdtKilpnoChanged(TObject *Sender);
   void __fastcall naytaTiedot(void);
   void __fastcall SrjKarkiBtn(TObject *Sender);
   void __fastcall SeurBtnClick(TObject *Sender);
   void __fastcall EdBtnClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall SrjValChange(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
	void __fastcall BtnSalliClick(TObject *Sender);
	void __fastcall BtnPeruutaClick(TObject *Sender);
	void __fastcall BtnTallennaClick(TObject *Sender);
	void __fastcall VaGridExit(TObject *Sender);
	void __fastcall PvGridExit(TObject *Sender);
	void __fastcall EdtNimihakuChange(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall CBSeurantaClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall SeurGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State);
	void __fastcall BtnTietoaClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall EdtKilpnoKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EdtKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall VaGridMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall VaGridMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall BtnPaivitaClick(TObject *Sender);

private:	// User declarations
   int __fastcall tallennaTiedot(void);
	void __fastcall haeTiedot(kilptietue *Kilp1);
	void FormSetup(void);
   int Sarja;
   int dKilp;
   kilptietue Kilp;
   ColTp *PvGridCols;
   ColTp *VaGridCols;
   bool nimihaku;
   cellTp Cells[4][51];
   int SeurRow;
   int NKarki;
   int PvGridWidth;
   int PvGridHeight;
   int SeurGridWidth;
public:		// User declarations
   bool sallimuokkaus;
   bool Seuranta;
   void __fastcall naytaSeuranta(void);
	void __fastcall naytaKilpailija(int d);
   __fastcall TFormKilpailijaOnline(TComponent* Owner);
   wchar_t Kohde[30];
};
//---------------------------------------------------------------------------
extern TFormKilpailijaOnline *FormKilpailijaOnline;
//---------------------------------------------------------------------------
#endif
