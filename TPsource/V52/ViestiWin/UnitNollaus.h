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

#ifndef UnitNollausH
#define UnitNollausH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
//---------------------------------------------------------------------------
class TFormNollaus : public TForm
{
__published:	// IDE-managed Components
	TCheckBox *CBTulokset;
	TCheckBox *CBEil;
	TCheckBox *CBPoissa;
	TCheckBox *CBEmit;
	TCheckBox *CBRadat;
	TCheckBox *CBVa;
	TLabel *Label1;
	TComboBox *CBOsuudet;
	TRadioGroup *RGSarjat;
	TListBox *LBSarjat;
	TLabel *Label2;
	TButton *Button1;
	TMainMenu *MainMenu1;
	TMenuItem *Erikoistoiminnot1;
	TMenuItem *Kerrokaikkiajatkymmenell1;
	TMenuItem *Jaakaikkiajatkymmenell1;
	TLabel *LblTila;
	TMenuItem *N1;
	TMenuItem *Suljekaavake1;
	TMenuItem *Poistamiljoonat;
	TCheckBox *CBPisteet;
	TCheckBox *CBFlags;
	TMenuItem *Vaihdaetusuku;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Kerrokaikkiajatkymmenell1Click(TObject *Sender);
	void __fastcall Jaakaikkiajatkymmenell1Click(TObject *Sender);
	void __fastcall Suljekaavake1Click(TObject *Sender);
	void __fastcall PoistamiljoonatClick(TObject *Sender);
	void __fastcall VaihdaetusukuClick(TObject *Sender);
private:	// User declarations
	bool vaihdaetusuku;
public:		// User declarations
	__fastcall TFormNollaus(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormNollaus *FormNollaus;
//---------------------------------------------------------------------------
#endif
