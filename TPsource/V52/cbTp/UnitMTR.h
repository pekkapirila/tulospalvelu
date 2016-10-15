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

#ifndef UnitMTRH
#define UnitMTRH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#ifdef MAXOSUUSLUKU
#include "VDeclare.h"
#else
#include "HkDeclare.h"
#endif
//---------------------------------------------------------------------------
class TFormMTR : public TForm
{
__published:	// IDE-managed Components
	TComboBox *CBAsema;
	TLabel *Label1;
	TEdit *EdtId;
	TLabel *Label2;
	TLabel *Label3;
	TEdit *EdtKello;
	TLabel *Label4;
	TEdit *EdtEro;
	TLabel *Label5;
	TEdit *EdtParisto;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TStringGrid *SG1;
	TLabel *Label6;
	TGroupBox *GroupBox1;
	TButton *Button1;
	TRadioGroup *RGToisto;
	TRadioGroup *RGRajaus;
	TComboBox *CBIstunto;
	TEdit *EdtLkm;
	TEdit *EdtEns;
	TEdit *EdtViim;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TButton *BtnSeur;
	TButton *BtnSulje;
	TRadioGroup *RGkohde;
	TSaveDialog *SaveDialog1;
	TLabel *Label12;
	TEdit *EdtTallennettu;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall RGRajausClick(TObject *Sender);
	void __fastcall RGToistoClick(TObject *Sender);
	void __fastcall BtnSeurClick(TObject *Sender);
	void __fastcall BtnSuljeClick(TObject *Sender);
private:	// User declarations
	MESSAGE void __fastcall PaivHandler(TMyMessage &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYPAIVITA, TMyMessage, PaivHandler);
END_MESSAGE_MAP(TComponent)
public:		// User declarations
	__fastcall TFormMTR(TComponent* Owner);
	void __fastcall LueMTRTiedot(void);
	san_type vastaus;
	int r_no;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMTR *FormMTR;
//---------------------------------------------------------------------------
#endif
