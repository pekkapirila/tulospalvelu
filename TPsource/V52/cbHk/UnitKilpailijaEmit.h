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

#ifndef UnitKilpailijaEmitH
#define UnitKilpailijaEmitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include "HkDef.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.Menus.hpp>;
//---------------------------------------------------------------------------
class TFormKilpailijaEmit : public TForm
{
__published:	// IDE-managed Components
   TEdit *EdtKilpno;
   TLabel *Label1;
   TEdit *Nimi;
   TEdit *Maa;
   TLabel *Label3;
   TEdit *TLahto;
   TEdit *TMaali;
   TLabel *Label4;
   TEdit *Tulos;
   TLabel *Label5;
   TEdit *Sija;
   TLabel *Label6;
   TStringGrid *EmitGrid;
   TButton *SrjKarki;
   TButton *SeurBtn;
   TButton *EdBtn;
   TLabel *Label10;
   TEdit *EdtSeura;
   TStringGrid *VaGrid;
   TComboBox *SrjVal;
	TBitBtn *BitBtn1;
	TMainMenu *MainMenu1;
	TMenuItem *iedosto1;
	TMenuItem *Sulje1;
   void __fastcall EdtKilpnoChanged(TObject *Sender);
   void __fastcall naytaTiedot(void);
   void __fastcall SrjKarkiBtn(TObject *Sender);
   void __fastcall SeurBtnClick(TObject *Sender);
   void __fastcall EdBtnClick(TObject *Sender);
   void __fastcall EdtKilpnoKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall FormCreate(TObject *Sender);
   void __fastcall SrjValChange(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall EmitGridMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall EmitGridMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall Sulje1Click(TObject *Sender);
private:	// User declarations
   int Sarja;
   int dKilp;
   kilptietue *Kilp;
public:		// User declarations
	void __fastcall naytaKilpailija(int d);
   __fastcall TFormKilpailijaEmit(TComponent* Owner);
   wchar_t Kohde[30];
};
//---------------------------------------------------------------------------
extern TFormKilpailijaEmit *FormKilpailijaEmit;
//---------------------------------------------------------------------------
#endif
