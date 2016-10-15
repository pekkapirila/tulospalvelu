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

#ifndef UnitSeuratH
#define UnitSeuratH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include "HkDeclare.h"

//---------------------------------------------------------------------------
class TFormSeurat : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *SraGrid;
	TMainMenu *MainMenu1;
	TMenuItem *iedosto1;
	TMenuItem *Luetiedot1;
	TOpenDialog *OpenDialog1;
	TMenuItem *Asetukset1;
	TMenuItem *Lyhennejarjestys1;
	TSaveDialog *SaveDialog1;
	TMenuItem *Muokkaa1;
	TMenuItem *Sallimuutokset1;
	TMenuItem *allennamuutokset1;
	TMenuItem *Sulje1;
	TMenuItem *Peruutamuutokset1;
	TMenuItem *Lisseura1;
	TMenuItem *Poistaseura1;
	TMenuItem *allennanimell1;
	TMenuItem *N1;
	TMenuItem *Maakooditkytss1;
	TMenuItem *Help1;
	void __fastcall Luetiedot1Click(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Lyhennejarjestys1Click(TObject *Sender);
	void __fastcall Sallimuutokset1Click(TObject *Sender);
	void __fastcall allennamuutokset1Click(TObject *Sender);
	void __fastcall Sulje1Click(TObject *Sender);
	void __fastcall Peruutamuutokset1Click(TObject *Sender);
	void __fastcall Lisseura1Click(TObject *Sender);
	void __fastcall Poistaseura1Click(TObject *Sender);
	void __fastcall allennanimell1Click(TObject *Sender);
	void __fastcall Maakooditkytss1Click(TObject *Sender);
	void __fastcall Help1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall SraGridMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall SraGridMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall SraGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall SraGridDblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormSeurat(TComponent* Owner);
	__fastcall ~TFormSeurat(void);
	int __fastcall LueSeurat(UnicodeString FName);
	void __fastcall KirjSeurat(UnicodeString FName);
	int __fastcall NaytaSeurat(wchar_t *ala, wchar_t *yla);
	int __fastcall Seuratiedot(int KSeura, int *piiri, wchar_t *maa,
		wchar_t *lyhenne, wchar_t *nimi);
	void _fastcall TallennaSeura(int piiri, wchar_t *maa,
		wchar_t *lyhenne, wchar_t *nimi, int kielto);
	bool __fastcall SeuraHakuOn(void);
	bool __fastcall HaeTiedot(bool lyhhaku, int *piiri, wchar_t *maa,
		wchar_t *lyhenne, wchar_t *nimi, bool vapaakoko);
	void __fastcall Selaa(int haku);

//	seuraTieto *Seurat;
	seuraLuettelo SeuraLuett;
	int NSeura;
	int NEsilla;
	int NSeuraMax;
	int ensSeura;
	int aktSeura;
	int SramaxRow;
	TComponent *Caller;
	UnicodeString SraFName;
};
class TPPGrid : public TStringGrid
{
public:
	virtual void __fastcall DeleteRow(int ARow);
   wchar_t Kohde[30];
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSeurat *FormSeurat;
//---------------------------------------------------------------------------
#endif
