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

#ifndef UnitYvH
#define UnitYvH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TFormYhtveto : public TForm
{
__published:	// IDE-managed Components
   TStringGrid *YvGrid;
   TButton *BtnSulje;
   TButton *BtnPaivita;
   TMainMenu *MainMenu1;
   TMenuItem *Tiedosto1;
   TMenuItem *Kirjoitatiedostoon1;
   TSaveDialog *SaveDialog1;
	TMenuItem *Suljekaavake1;
   void __fastcall YvGridClick(TObject *Sender);
   void __fastcall BtnSuljeClick(TObject *Sender);
   void __fastcall BtnPaivitaClick(TObject *Sender);
   void __fastcall FormActivate(TObject *Sender);
   void __fastcall Kirjoitatiedostoon1Click(TObject *Sender);
	void __fastcall Suljekaavake1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TFormYhtveto(TComponent* Owner);
   void __fastcall LaadiYhtveto(void);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormYhtveto *FormYhtveto;
//---------------------------------------------------------------------------
#endif
