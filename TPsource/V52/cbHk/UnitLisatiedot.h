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
#include "HkDef.h"
//---------------------------------------------------------------------------
class TFormLisatiedot : public TForm
{
__published:	// IDE-managed Components
	TRadioGroup *RGTunniste;
	TGroupBox *GBTunnvaihe;
	TComboBox *CBTunnPv;
	TRadioGroup *RGTieto;
	TGroupBox *GBKohdevaihe;
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
	TEdit *EdtEhto;
	TLabel *Label4;
	TSaveDialog *SaveDialog1;
	TComboBox *CBEhtoVaihe;
	TLabel *Label5;
	TComboBox *CBEhtoSrj;
	TComboBox *CBEhto;
	TLabel *Label6;
	TGroupBox *GroupBox6;
	TEdit *EdtLisays;
	TEdit *EdtKerroin;
	TLabel *Label7;
	TLabel *LblKerroin;
	TGroupBox *GroupBox7;
	TButton *Button3;
	TComboBox *CBKirjDesLuku;
	TLabel *Label8;
	TLabel *Label9;
	TButton *Button4;
	TButton *Button5;
	TCheckBox *CBDesLuku;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall CBEhtoVaiheClick(TObject *Sender);
	void __fastcall CBEhtoClick(TObject *Sender);
	void __fastcall EdtLisaysExit(TObject *Sender);
	void __fastcall EdtKerroinExit(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall RGTietoClick(TObject *Sender);
	void __fastcall RGTunnisteClick(TObject *Sender);
private:	// User declarations
	void __fastcall SiirraTiedot(int sellaisinaan);
	void __fastcall LataaSarjat(void);
	bool __fastcall Ehto(kilptietue *kilp);
	void __fastcall TulkEhdot(void);
	int Ehtolaji;
	int EhtoArvo;
	wchar_t EhtoSt[60];
	int EhtoPv;
	int Lisays;
public:		// User declarations
	__fastcall TFormLisatiedot(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormLisatiedot *FormLisatiedot;
//---------------------------------------------------------------------------
#endif
