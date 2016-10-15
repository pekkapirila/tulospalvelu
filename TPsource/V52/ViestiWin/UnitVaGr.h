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

#ifndef UnitVaGrH
#define UnitVaGrH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "VDef.h"
//---------------------------------------------------------------------------
class TFormVaGr : public TForm
{
__published:	// IDE-managed Components
   TListBox *JoukkList;
   TButton *PiirraBtn;
   TComboBox *OsVal;
   TRadioGroup *JarjVal;
   TButton *PaivitaButton;
   TEdit *EdMaxLkm;
   TLabel *Label1;
   TRadioGroup *TietoVal;
   TComboBox *ViivaVal;
   TLabel *Label5;
   TComboBox *SarjaVal;
   void __fastcall PaivitaClick(TObject *Sender);
   void __fastcall PiirraBtnClick(TObject *Sender);
   void __fastcall ViivaValChange(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall OsValChange(TObject *Sender);
   void __fastcall FormActivate(TObject *Sender);
   void __fastcall SarjaValChange(TObject *Sender);
private:	// User declarations
   void __fastcall DrawTaul(int (*taul)[MAXEMITVA+1][2], int nrivi);
public:		// User declarations
   __fastcall TFormVaGr(TComponent* Owner);
   __fastcall ~TFormVaGr(void);
   int Osuus;
   int Sarja;
   int edSarja;
   wchar_t Kohde[30];
   };
//---------------------------------------------------------------------------
extern PACKAGE TFormVaGr *FormVaGr;
//---------------------------------------------------------------------------
#endif
