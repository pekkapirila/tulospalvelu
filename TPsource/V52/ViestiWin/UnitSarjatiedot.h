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

#ifndef UnitSarjatiedotH
#define UnitSarjatiedotH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Dialogs.hpp>
#include "VDef.h"
//---------------------------------------------------------------------------
class TFormSarjatiedot : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *SG1;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TComboBox *CBJarj;
	TComboBox *CBTiedot;
	TMainMenu *MainMenu1;
	TMenuItem *iedosto1;
	TMenuItem *Sulje1;
	TMenuItem *Asetukset1;
	TLabel *Label1;
	TLabel *Label2;
	TBitBtn *BitBtn1;
	TLabel *LblMuokkaus;
	TMenuItem *Sakollisuus1;
	TMenuItem *Lhtvli1;
	TMenuItem *Lhtpaikka1;
	TMenuItem *Maali1;
	TMenuItem *Maksut1;
	TMenuItem *Matka1;
	TMenuItem *Pitknimi1;
	TMenuItem *Ampumapaikat1;
	TMenuItem *Sakkoyksikk1;
	TMenuItem *Kirjoitacsvtiedostoon1;
	TMenuItem *Luecsvtiedostosta1;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TPopupMenu *PopupMenu1;
	TMenuItem *Kopioivalittunaolevasolukaikkiinsarjoihin1;
	TMenuItem *Ohita1;
	TMenuItem *Kiinteitsarakkeita11;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall SG1DblClick(TObject *Sender);
	void __fastcall SG1SelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall SG1Exit(TObject *Sender);
	void __fastcall CBTiedotChange(TObject *Sender);
	void __fastcall CBJarjChange(TObject *Sender);
	void __fastcall Sulje1Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall SG1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall SG1FixedCellClick(TObject *Sender, int ACol, int ARow);
	void __fastcall Sakollisuus1Click(TObject *Sender);
	void __fastcall Lhtvli1Click(TObject *Sender);
	void __fastcall Lhtpaikka1Click(TObject *Sender);
	void __fastcall Maali1Click(TObject *Sender);
	void __fastcall Ampumapaikat1Click(TObject *Sender);
	void __fastcall Sakkoyksikk1Click(TObject *Sender);
	void __fastcall Maksut1Click(TObject *Sender);
	void __fastcall Matka1Click(TObject *Sender);
	void __fastcall Pitknimi1Click(TObject *Sender);
	void __fastcall Kirjoitacsvtiedostoon1Click(TObject *Sender);
	void __fastcall Luecsvtiedostosta1Click(TObject *Sender);
	void __fastcall SG1MouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall SG1MouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall Kopioivalittunaolevasolukaikkiinsarjoihin1Click(TObject *Sender);
	void __fastcall Kiinteitsarakkeita11Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);


private:	// User declarations
	wchar_t *otsikkorivi(wchar_t *line, int len, wchar_t erotin);
	wchar_t *datarivi(sarjatietue *Sarja, wchar_t *line, int len, wchar_t erotin);
	int lue_sarjacsv(TextFl *afile, sarjatietue *Sarja, int lineno, int toiminto, wchar_t erotin);
	int aktrow;
	int aktcol;
	int ColIx[100];
	int ColPv[100];
	int VapNum;
	int VapAika;
    sarjatietue Sarjat_0[MAXSARJALUKU];
public:		// User declarations
	__fastcall TFormSarjatiedot(TComponent* Owner);
	void __fastcall SetColumns(void);
	void __fastcall NaytaSarjat(void);
	int  __fastcall paivitaMuutos(int col, int row);
	void __fastcall TallennaSarjat(void);
   wchar_t Kohde[30];
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSarjatiedot *FormSarjatiedot;
//---------------------------------------------------------------------------
#endif
