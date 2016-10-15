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

#ifndef OsuusTiedotH
#define OsuusTiedotH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TFormOsuus : public TForm
{
__published:	// IDE-managed Components
   TEdit *EdtJoukkue;
   TLabel *Label1;
   TEdit *JoukkNimi;
   TEdit *OsNimi;
   TEdit *Maa;
   TLabel *Label3;
   TEdit *TLahto;
   TEdit *TMaali;
   TLabel *Label4;
   TEdit *Tulos;
   TLabel *Label5;
   TEdit *Sija;
   TLabel *Label6;
   TEdit *OsTls;
   TLabel *Label7;
   TEdit *OsSija;
   TLabel *Label8;
   TLabel *Label11;
   TEdit *Ekoodi;
   TStringGrid *EmitGrid;
   TEdit *LahtoSija;
   TLabel *Label9;
   TButton *OsKarkiBtn;
   TButton *OsSeur;
   TButton *OsEd;
   TButton *VaihdKarki;
   TButton *Button1;
   TButton *Button2;
   TLabel *Label10;
   TEdit *EdHajonta;
   TLabel *Label12;
   TComboBox *ValSarja;
	TComboBox *ValOsuus;
	TMainMenu *MainMenu1;
	TMenuItem *Haku1;
	TMenuItem *Haenimell1;
	TMenuItem *Rastivliaikaanalyysi1;
	TMenuItem *eeanalyysi1;
   void __fastcall EdtJoukkueChanged(TObject *Sender);
   void __fastcall naytaTiedot(void);
   void __fastcall OsKarkiBtnClick(TObject *Sender);
   void __fastcall OsSeurClick(TObject *Sender);
   void __fastcall OsEdClick(TObject *Sender);
   void __fastcall VaihdKarkiBtn(TObject *Sender);
   void __fastcall VaihdSeurBtn(TObject *Sender);
   void __fastcall VaihdEdBtn(TObject *Sender);
   void __fastcall EdtJoukkueKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall ValSarjaChange(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ValOsuusChange(TObject *Sender);
	void __fastcall Haenimell1Click(TObject *Sender);
	void __fastcall eeanalyysi1Click(TObject *Sender);
private:	// User declarations
   int Yhd;
   int Sarja;
   int Osuus;
   int KilpNo;
   int dKilp;
   int inNaytaTiedot;
   int laika;
public:		// User declarations
   __fastcall TFormOsuus(TComponent* Owner);
   void __fastcall naytaOsuus(int KilpNo, int osuus);
   wchar_t Kohde[30];
};
//---------------------------------------------------------------------------
extern PACKAGE TFormOsuus *FormOsuus;
//---------------------------------------------------------------------------
#endif
