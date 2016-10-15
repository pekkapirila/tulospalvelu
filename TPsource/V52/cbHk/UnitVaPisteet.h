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

#ifndef UnitVaPisteetH
#define UnitVaPisteetH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include "HkWinDef.h"
//---------------------------------------------------------------------------
typedef cellTp cellTpA[VALUKU+5];

class TFormVaPisteet : public TForm
{
__published:	// IDE-managed Components
	TButton *Button1;
	TButton *Button3;
	TButton *Button2;
	TButton *Button4;
	TComboBox *CBPaiva;
	TBitBtn *BitBtn1;
	TMainMenu *MainMenu1;
	TMenuItem *oiminnot1;
	TMenuItem *Kopioi1;
	TMenuItem *Mritteletuloksenlaskentakaava1;
	TMenuItem *Kirjoitatekstitiedostoon1;
	TMenuItem *Luetekstitiedostosta1;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TComboBox *ComboBox1;
	TLabel *Label1;
	TMenuItem *Mrmatkatratatiedoista1;
	TMenuItem *Kopioivaiheentiedotmuillevaiheille1;
	TDrawGrid *VAGrid;
	TPopupMenu *PopupMenu1;
	TMenuItem *Kopioitmsolukaikkiinsarjoihin1;
	TMenuItem *Ohita1;
	TMenuItem *Luotasavlisetvliajatsarjalle1;
	TMenuItem *Poistasolunvliaika1;
	TMenuItem *Kopioikaikkivalitunpisteentiedotmuihinsarjoihin1;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall VAGridSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall VAGridExit(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall Kopioi1Click(TObject *Sender);
	void __fastcall VAGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall VAGridDblClick(TObject *Sender);
	void __fastcall Mritteletuloksenlaskentakaava1Click(TObject *Sender);
	void __fastcall CBPaivaChange(TObject *Sender);
	void __fastcall Kirjoitatekstitiedostoon1Click(TObject *Sender);
	void __fastcall Luetekstitiedostosta1Click(TObject *Sender);
	void __fastcall VAGridMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall VAGridMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall VAGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
		  TGridDrawState State);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall ComboBox1Change(TObject *Sender);
	void __fastcall VAGridSetEditText(TObject *Sender, int ACol, int ARow, const UnicodeString Value);
	void __fastcall Mrmatkatratatiedoista1Click(TObject *Sender);
	void __fastcall Kopioivaiheentiedotmuillevaiheille1Click(TObject *Sender);
	void __fastcall Kopioitmsolukaikkiinsarjoihin1Click(TObject *Sender);
	void __fastcall Luotasavlisetvliajatsarjalle1Click(TObject *Sender);
	void __fastcall Poistasolunvliaika1Click(TObject *Sender);
	void __fastcall VAGridGetEditText(TObject *Sender, int ACol, int ARow, UnicodeString &Value);
	void __fastcall Kopioikaikkivalitunpisteentiedotmuihinsarjoihin1Click(TObject *Sender);




private:	// User declarations
	int __fastcall tallennaTiedot(void);
	int __fastcall paivitaMuutos(int col, int row);
	void __fastcall editKaava(int srj, int pv, int va);
	int __fastcall CRowFromARow(int ARow);
	int aktrow;
	int aktcol;
public:		// User declarations
	__fastcall TFormVaPisteet(TComponent* Owner);
	void __fastcall naytaTiedot(void);
	cellTpA OtsCells;
	cellTpA *Cells;
	int CellsRowCount;
	int nRivi;
	int nRiviEd;
	int NLaji;
	int NNaytaLaji;
	int ValLaji;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormVaPisteet *FormVaPisteet;
//---------------------------------------------------------------------------
#endif
