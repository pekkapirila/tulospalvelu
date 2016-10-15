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

#ifndef UnitKisaLuettH
#define UnitKisaLuettH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>

typedef struct {
	int date;
	wchar_t koodi[12];
	wchar_t nimi[40];
	wchar_t polku[100];
	wchar_t cfgfile[20];
} kisaLuettTp;

//---------------------------------------------------------------------------
class TFormKisaLuett : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *SG1;
	TMainMenu *MainMenu1;
	TMenuItem *Tiedosto1;
	TMenuItem *Luecsv1;
	TMenuItem *Kirjoitacsv1;
	TMenuItem *allenna1;
	TButton *Button1;
	TButton *Button2;
	TGroupBox *GroupBox1;
	TEdit *EdtRunko;
	TLabel *Label1;
	TButton *BtnRunko;
	TCheckBox *CBLuo;
	TCheckBox *CBSarjat;
	TCheckBox *CBData;
	TCheckBox *CBKonfig;
	TButton *BtnToteuta;
	TButton *Button3;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TMenuItem *Luekilpailuluettelo1;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall SG1SelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall BtnToteutaClick(TObject *Sender);
	void __fastcall BtnRunkoClick(TObject *Sender);
	void __fastcall Luekilpailuluettelo1Click(TObject *Sender);

private:	// User declarations
	kisaLuettTp kisaLuett[100];
public:		// User declarations
	__fastcall TFormKisaLuett(TComponent* Owner);
	void kirjKilpLuett(wchar_t *outfname, kisaLuettTp *kisat, int lkm);
	int lueKilpLuett(wchar_t *infname, kisaLuettTp *kisat, int *lkm);
	void naytaLista(void);
	void haeLista(void);
	int kilpLkm;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormKisaLuett *FormKisaLuett;
//---------------------------------------------------------------------------
#endif
