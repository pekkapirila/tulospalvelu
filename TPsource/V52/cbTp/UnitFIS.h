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

#ifndef UnitFISH
#define UnitFISH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
#ifdef MAXOSUUSLUKU
#include "VDeclare.h"
#define LSNIMI (OSNIMIL-5)
#define LENIMI (OSNIMIL-5)
#else
#include "HkDeclare.h"
#endif

typedef struct {
	int		id;
	int		code;
	int		pts;
	wchar_t	sukunimi[LSNIMI+1];
	wchar_t etunimi[LENIMI+1];
	int 	synt;
	wchar_t	maa[4];
} cmptrtp;
//---------------------------------------------------------------------------
class TFormFIS : public TForm
{
__published:	// IDE-managed Components
	TOpenDialog *OpenDialog1;
	TGroupBox *GroupBox1;
	TRadioGroup *RGLaji;
	TButton *BtnLuePisteet;
	TGroupBox *GroupBox2;
	TButton *BtnLueNimet;
	TLabel *Label1;
	TLabel *Label2;
	TBitBtn *BitBtn1;
	TGroupBox *GroupBox3;
	TButton *BtnLue;
	TButton *BtnKirj;
	TLabeledEdit *EdtKoodi;
	TLabeledEdit *EdtNimi;
	TLabeledEdit *EdtPaikka;
	TLabeledEdit *EdtPv;
	TLabeledEdit *EdtKausi;
	TLabeledEdit *EdtLista;
	TLabel *Label3;
	TLabeledEdit *EdtMaa;
	TComboBox *CBFvalue;
	TLabel *Label4;
	TComboBox *CBminpist;
	TLabel *Label5;
	TLabeledEdit *EdtTDid;
	TLabeledEdit *EdtTDsnimi;
	TLabeledEdit *EdtTDenimi;
	TLabeledEdit *EdtTDmaa;
	TLabel *Label6;
	TComboBox *CBcategory;
	TLabeledEdit *EdtLaji;
	TButton *BtnSulje;
	TSaveDialog *SaveDialog1;
	TMemo *Memo1;
	TButton *Button1;
	void __fastcall BtnLuePisteetClick(TObject *Sender);
	void __fastcall BtnLueNimetClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BtnSuljeClick(TObject *Sender);
	void __fastcall BtnLueClick(TObject *Sender);
	void __fastcall BtnKirjClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
private:	// User declarations
	void __fastcall LueKilpailijat(TextFl *InFileCom);
	int __fastcall HaeId(int id);
	int __fastcall HaePisteet(int code);
	int __fastcall HaeCode(int code);
	cmptrtp *cmptr;
	int ncmptr;
public:		// User declarations
	__fastcall TFormFIS(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormFIS *FormFIS;
//---------------------------------------------------------------------------
#endif
