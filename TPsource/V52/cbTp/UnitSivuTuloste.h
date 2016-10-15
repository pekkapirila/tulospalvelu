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

#ifndef UnitSivuTulosteH
#define UnitSivuTulosteH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Menus.hpp>
#include <tputil.h>
#include <xmlparser.h>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TFormSivuTuloste : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *SG1;
	TButton *BtnOtaKayttoon;
	TCheckBox *CBJatkuva;
	TButton *BtnKirjoita;
	TComboBox *CBValinta;
	TEdit *EdtKno;
	TMainMenu *MainMenu1;
	TMenuItem *iedosto1;
	TMenuItem *allennatehdytvalinnat1;
	TMenuItem *Luevalinnattiedostosta1;
	TMenuItem *Suljekaavake1;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TBitBtn *BitBtn1;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall BtnOtaKayttoonClick(TObject *Sender);
	void __fastcall CBJatkuvaClick(TObject *Sender);
	void __fastcall BtnKirjoitaClick(TObject *Sender);
	void __fastcall Suljekaavake1Click(TObject *Sender);
	void __fastcall allennatehdytvalinnat1Click(TObject *Sender);
	void __fastcall Luevalinnattiedostosta1Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
	int __fastcall LueValinnat(wchar_t *flnm);
	void __fastcall KirjoitaValinnat(wchar_t *flnm);
	void __fastcall NaytaValinnat(void);
	int tulkXMLSivuMuotoilu(xml_node *node, int nnode);
	wchar_t PgMaarFlNm[202];
public:		// User declarations
	__fastcall TFormSivuTuloste(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSivuTuloste *FormSivuTuloste;
//---------------------------------------------------------------------------
#endif
