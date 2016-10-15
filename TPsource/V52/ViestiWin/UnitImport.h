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

#ifndef UnitImportH
#define UnitImportH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include "VDeclare.h"
#include "UnitTulkinta.h"
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TFormImport : public TForm
{
__published:	// IDE-managed Components
	TRadioGroup *RGLaji;
	TOpenDialog *OpenDialog1;
	TButton *Button1;
	TRadioGroup *RGtiedlaji;
	TEdit *EditLuettu;
	TLabel *Label4;
	TPanel *Panel2;
	TLabel *Label2;
	TLabel *Label8;
	TComboBox *CBerotin;
	TLabel *Label3;
	TLabel *Label1;
	TButton *Button2;
	TEdit *Edit1;
	TLabel *Label6;
	TGroupBox *GBPvVa;
	TLabel *Label9;
	TEdit *EditNpv;
	TLabel *Label10;
	TEdit *EditNva;
	TBitBtn *BitBtn1;
	TOpenDialog *OpenDialog2;
	TEdit *EditLuettuTot;
	TLabel *Label7;
	TRadioGroup *RGtoiminto;
	TRadioGroup *RGNimiJarj;
	TRadioGroup *RGMerkit;
	TLabel *Label5;
	TLabel *Label11;
	TEdit *EdtHaj;
	TLabel *LblXMLtieto;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall RGtiedlajiClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
	FLD2 *exp_kentat_local;
	int lue_kilpcsv(TextFl *afile, kilptietue *kilp, int lineno, int toiminto, int *dKilp, wchar_t erotin);
	int __fastcall lue_SQL(void);
public:		// User declarations
	__fastcall TFormImport(TComponent* Owner);
   wchar_t Kohde[30];
};
//---------------------------------------------------------------------------
extern PACKAGE TFormImport *FormImport;
//---------------------------------------------------------------------------
#endif
