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

#ifndef UnitLisatiedotH
#define UnitLisatiedotH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TFormLisatiedot : public TForm
{
__published:	// IDE-managed Components
	TRadioGroup *RGTunniste;
	TGroupBox *GroupBox1;
	TComboBox *CBTunnPv;
	TRadioGroup *RGTieto;
	TGroupBox *GroupBox2;
	TComboBox *CBTietoPv;
	TGroupBox *GroupBox3;
	TLabel *Label1;
	TEdit *EditTunniste;
	TEdit *EditTieto;
	TLabel *Label2;
	TButton *Button1;
	TOpenDialog *OpenDialog1;
	TComboBox *CBErotin;
	TLabel *Label3;
	TBitBtn *BitBtn1;
	TButton *Button2;
	TGroupBox *GroupBox4;
	TEdit *EdtPohjustus;
	TGroupBox *GroupBox5;
	TRadioGroup *RGEhto;
	TEdit *EdtEhto;
	TLabel *Label4;
	TSaveDialog *SaveDialog1;
	TButton *Button3;
	TLabel *Label5;
	TEdit *EdtOhjelkm;
	TLabel *Label6;
	TEdit *EdtTietolkm;
	TCheckBox *CBDes1000;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall RGTunnisteClick(TObject *Sender);
	void __fastcall RGTietoClick(TObject *Sender);
private:	// User declarations
	void __fastcall SiirraTiedot(bool sellaisinaan);
	void __fastcall KopioiTiedot(void);
public:		// User declarations
	__fastcall TFormLisatiedot(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormLisatiedot *FormLisatiedot;
//---------------------------------------------------------------------------
#endif
