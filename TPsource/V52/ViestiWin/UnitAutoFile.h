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

#ifndef UnitAutoFileH
#define UnitAutoFileH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TFormAutoFile : public TForm
{
__published:	// IDE-managed Components
	TRadioGroup *RGLaji;
	TGroupBox *GBhtml;
	TGroupBox *GBxml;
	TEdit *EdtVali;
	TLabel *Label1;
	TEdit *EdtHtmlFName;
	TLabel *Label2;
	TSaveDialog *SaveDialog1;
	TButton *Button1;
	TLabel *Label3;
	TEdit *EdtPageTitle;
	TLabel *Label4;
	TEdit *EdtPageHeader;
	TRadioGroup *RGva;
	TButton *BtnOtakayttoon;
	TButton *Button3;
	TMemo *Memo1;
	TLabel *Label5;
	TButton *Button4;
	TRadioGroup *RGPisteet;
	TLabel *Label6;
	TEdit *EdtKomento;
	TButton *BtnValitseKomento;
	TButton *BtnAvaaAFlst;
	TButton *BtnLuoAFlst;
	TButton *Button6;
	TRadioGroup *RGJatko;
	TButton *BtnLahetysVal;
	TCheckBox *CBVaadiMuutos;
	TBitBtn *BitBtn1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BtnOtakayttoonClick(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall BtnValitseKomentoClick(TObject *Sender);
	void __fastcall BtnAvaaAFlstClick(TObject *Sender);
	void __fastcall CBVaadiMuutosClick(TObject *Sender);
	void __fastcall RGJatkoClick(TObject *Sender);
	void __fastcall BtnLahetysValClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BtnLuoAFlstClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormAutoFile(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAutoFile *FormAutoFile;
//---------------------------------------------------------------------------
#endif
