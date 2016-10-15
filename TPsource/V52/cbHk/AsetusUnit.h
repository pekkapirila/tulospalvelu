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

#ifndef AsetusUnitH
#define AsetusUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
//---------------------------------------------------------------------------
class TAsetusForm : public TForm
{
__published:	// IDE-managed Components
   TButton *BtnOK;
   TButton *BtnPeruuta;
   TPageControl *PageControl1;
   TTabSheet *TabSheet1;
   TTabSheet *TabSheet2;
   TLabel *Label5;
   TEdit *Edit3;
   TLabel *Label6;
   TEdit *Edit4;
   TLabel *Label7;
   TLabel *Label8;
	TMainMenu *MainMenu1;
	TMenuItem *Nytt1;
	TMenuItem *Luekorostustiedot1;
	TCheckBox *CheckBox1;
   void __fastcall BtnPeruutaClick(TObject *Sender);
   void __fastcall BtnOKClick(TObject *Sender);
	void __fastcall Luekorostustiedot1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CheckBox1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TAsetusForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TAsetusForm *AsetusForm;
//---------------------------------------------------------------------------
#endif
