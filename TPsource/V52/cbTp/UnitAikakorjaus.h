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

#ifndef UnitAikakorjausH
#define UnitAikakorjausH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#ifdef MAXOSUUSLUKU
#include "VDeclare.h"
#else
#include "HkDeclare.h"
#endif
//---------------------------------------------------------------------------
class TFormAikakorjaus : public TForm
{
__published:	// IDE-managed Components
	TComboBox *CBJono;
	TLabel *Label9;
	TLabel *Label1;
	TEdit *EdtEns;
	TEdit *EdtViim;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TEdit *EdtAika1;
	TEdit *EdtSiirto1;
	TLabel *Label6;
	TLabel *Label7;
	TEdit *EdtAika2;
	TLabel *Label8;
	TEdit *EdtSiirto2;
	TButton *BtnToteuta;
	TButton *BtnSulje;
	TButton *BtnKopioi1;
	TLabel *Label10;
	TLabel *Label11;
	void __fastcall BtnSuljeClick(TObject *Sender);
	void __fastcall BtnKopioi1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall EdtAika1Exit(TObject *Sender);
	void __fastcall EdtAika2Exit(TObject *Sender);
	void __fastcall BtnToteutaClick(TObject *Sender);
private:	// User declarations
	int siirto(int t);
	int T1;
	int T2;
	int S1;
    int S2;
public:		// User declarations
	__fastcall TFormAikakorjaus(TComponent* Owner);
	int Jono;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAikakorjaus *FormAikakorjaus;
//---------------------------------------------------------------------------
#endif
