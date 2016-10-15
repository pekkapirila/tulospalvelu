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

#ifndef UnitTuloskaavaH
#define UnitTuloskaavaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TFormTuloskaava : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TEdit *EdtYla;
	TEdit *EdtAla;
	TLabel *Label9;
	TEdit *EdtAlaraja;
	TLabel *Label10;
	TEdit *EdtKerroin;
	TLabel *Label11;
	TLabel *Label12;
	TEdit *EdtJakaja;
	TLabel *Label13;
	TEdit *EdtYlaraja;
	TLabel *Label14;
	TLabel *Label15;
	TLabel *Label16;
	TLabel *Label17;
	TButton *BtnTallenna;
	TButton *BtnPoista;
	TButton *BtnKaikille;
	TCheckBox *CBVaadiAla;
	TButton *BtnPoistaKaikilta;
	TButton *Button2;
	TLabel *Label18;
	TEdit *EdtNollataso;
	TLabel *Label19;
	TLabel *Label20;
	TComboBox *CBSarja;
	TComboBox *CBVaihe;
	TComboBox *CBLaskettava;
	TEdit *EdtKaavaOn;
	TCheckBox *CBSakotPois;
	TLabel *Label21;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BtnKaikilleClick(TObject *Sender);
	void __fastcall BtnTallennaClick(TObject *Sender);
	void __fastcall BtnPoistaClick(TObject *Sender);
	void __fastcall BtnPoistaKaikiltaClick(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall CBSarjaChange(TObject *Sender);
	void __fastcall CBVaiheChange(TObject *Sender);
	void __fastcall CBLaskettavaChange(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormTuloskaava(TComponent* Owner);
	void __fastcall AsetaPiste(int srj, int pv, int va);
	int Srj;
	int Pv;
	int Va;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTuloskaava *FormTuloskaava;
//---------------------------------------------------------------------------
#endif
