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

#ifndef UnitSeurGrH
#define UnitSeurGrH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#define TAULDIM (MAXOSUUSLUKU+1)*(VAIKALUKU+2)
//---------------------------------------------------------------------------
class TSeurantaForm : public TForm
{
__published:	// IDE-managed Components
   TListBox *JoukkList;
   TLabel *Label1;
   TEdit *EdMaxLkm;
   TButton *PaivitaButton;
   TButton *PiirraBtn;
   TComboBox *OsVal;
   TGroupBox *GroupBox1;
   TComboBox *AlinOs;
   TComboBox *YlinOs;
   TLabel *Label3;
   TLabel *Label4;
   TComboBox *ViivaVal;
   TLabel *Label5;
   TComboBox *SarjaVal;
   void __fastcall PaivitaButtonClick(TObject *Sender);
   void __fastcall PiirraBtnClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall JoukkListClick(TObject *Sender);
   void __fastcall YlinOsChange(TObject *Sender);
   void __fastcall AlinOsChange(TObject *Sender);
   void __fastcall ViivaValChange(TObject *Sender);
   void __fastcall OsValChange(TObject *Sender);
   void __fastcall FormActivate(TObject *Sender);
   void __fastcall SarjaValChange(TObject *Sender);
private:	// User declarations
   int Sarja, edSarja, Osuus;
   void __fastcall DrawTaul(int (*taul)[TAULDIM][2], int nrivi);
public:		// User declarations
   __fastcall TSeurantaForm(TComponent* Owner);
   __fastcall ~TSeurantaForm(void);
   CRITICAL_SECTION seur_CriticalSection;
   int paivitysflag;
   int listaPyynto;
   int piirraPyynto;
   wchar_t Kohde[30];
};
//---------------------------------------------------------------------------
extern PACKAGE TSeurantaForm *SeurantaForm;
//---------------------------------------------------------------------------
#endif
