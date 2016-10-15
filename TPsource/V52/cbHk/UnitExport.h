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

#ifndef UnitExportH
#define UnitExportH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include "HkDeclare.h"
//---------------------------------------------------------------------------
class TFormExport : public TForm
{
__published:	// IDE-managed Components
	TRadioGroup *RGLaji;
	TGroupBox *GBXML;
	TCheckBox *CBXMLSarjat;
	TCheckBox *CBXMLKilpailijat;
	TCheckBox *CBXMLRadat;
	TGroupBox *GBcsv;
	TComboBox *CBErotin;
	TLabel *Label1;
	TButton *Button1;
	TSaveDialog *SaveDialog1;
	TComboBox *CBJarj;
	TLabel *Label3;
	TCheckBox *CBPisteet;
	TCheckBox *CBLainMerk;
	TLabel *Label4;
	TComboBox *CBMerkisto;
	TCheckBox *CBKaikkiKentat;
	TCheckBox *CBValiajat;
	TLabel *Label2;
	TButton *Button2;
	TGroupBox *GBSQL;
	TLabel *Label5;
	TEdit *EdtSQLlkm;
	TLabel *Label6;
	TLabel *Label7;
	TCheckBox *CBOhitaEil;
	TLabel *Label8;
	TLabel *Label9;
	TButton *BtnLisKaikki;
	TButton *BtnLisValitut;
	TButton *BtnPoisValitut;
	TButton *BtnPoisKaikki;
	TListBox *LBSarjat;
	TListBox *LBSarjaVal;
	TCheckBox *CBLaajennettu;
	TRadioGroup *RGdatRajaus;
	void __fastcall RGLajiClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall LBSarjatDblClick(TObject *Sender);
	void __fastcall LBSarjaValDblClick(TObject *Sender);
	void __fastcall BtnLisKaikkiClick(TObject *Sender);
	void __fastcall BtnLisValitutClick(TObject *Sender);
	void __fastcall BtnPoisValitutClick(TObject *Sender);
	void __fastcall BtnPoisKaikkiClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
	FLD2 *exp_kentat_local;
	wchar_t *otsikkorivi(wchar_t *line, int len, wchar_t erotin);
	wchar_t *datarivi(kilptietue *kilp, wchar_t *line, int len, wchar_t erotin);
	void NaytaSarjavalinnat(void);
	void kirjCsv(wchar_t *flname);
	void kirjSQL(void);
	int kirjKilpDat(wchar_t *fname, int rajaus);
public:		// User declarations
	__fastcall TFormExport(TComponent* Owner);
	bool SarjaValinnat[MAXSARJALUKU];
};
//---------------------------------------------------------------------------
extern PACKAGE TFormExport *FormExport;
//---------------------------------------------------------------------------
#endif
