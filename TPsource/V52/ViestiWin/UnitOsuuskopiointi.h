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

#ifndef UnitOsuuskopiointiH
#define UnitOsuuskopiointiH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TFormOsuuskopiointi : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TComboBox *CBOsuudet;
	TRadioGroup *RGSarjat;
	TListBox *LBSarjat;
	TLabel *Label2;
	TButton *Button1;
	TMainMenu *MainMenu1;
	TMenuItem *Erikoistoiminnot1;
	TLabel *LblTila;
	TMenuItem *Suljekaavake1;
	TLabel *Label3;
	TEdit *EdtKohdeos;
	TGroupBox *GroupBox1;
	TCheckBox *CBNimi;
	TCheckBox *CBTunnKdi;
	TCheckBox *CBSeura;
	TCheckBox *CBId;
	TCheckBox *CBHajonta;
	TCheckBox *CBArvo;
	TBitBtn *BitBtn1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Suljekaavake1Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormOsuuskopiointi(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormOsuuskopiointi *FormOsuuskopiointi;
//---------------------------------------------------------------------------
#endif
