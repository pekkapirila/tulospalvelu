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

#ifndef UnitTarkMerkH
#define UnitTarkMerkH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#define MERKEDITLKM 15
//---------------------------------------------------------------------------
class TFormTarkMerk : public TForm
{
__published:	// IDE-managed Components
	TEdit *EdNo0;
	TEdit *EdNimi0;
	TEdit *EdTrk0;
	TEdit *EdTls0;
	TEdit *EdSrj0;
	TEdit *EdLA0;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TEdit *Edit1;
	TEdit *Edit2;
	TEdit *Edit3;
	TEdit *Edit4;
	TEdit *Edit5;
	TEdit *Edit6;
	TEdit *Edit7;
	TEdit *Edit8;
	TEdit *Edit9;
	TEdit *Edit10;
	TEdit *Edit11;
	TEdit *Edit12;
	TEdit *Edit13;
	TEdit *Edit14;
	TEdit *Edit15;
	TEdit *Edit16;
	TEdit *Edit17;
	TEdit *Edit18;
	TEdit *Edit19;
	TEdit *Edit20;
	TEdit *Edit21;
	TEdit *Edit22;
	TEdit *Edit23;
	TEdit *Edit24;
	TEdit *Edit25;
	TEdit *Edit26;
	TEdit *Edit27;
	TEdit *Edit28;
	TEdit *Edit29;
	TEdit *Edit30;
	TEdit *Edit31;
	TEdit *Edit32;
	TEdit *Edit33;
	TEdit *Edit34;
	TEdit *Edit35;
	TEdit *Edit36;
	TEdit *Edit37;
	TEdit *Edit38;
	TEdit *Edit39;
	TEdit *Edit40;
	TEdit *Edit41;
	TEdit *Edit42;
	TEdit *Edit43;
	TEdit *Edit44;
	TEdit *Edit45;
	TEdit *Edit46;
	TEdit *Edit47;
	TEdit *Edit48;
	TEdit *Edit49;
	TEdit *Edit50;
	TEdit *Edit51;
	TEdit *Edit52;
	TEdit *Edit53;
	TEdit *Edit54;
	TEdit *Edit55;
	TEdit *Edit56;
	TEdit *Edit57;
	TEdit *Edit58;
	TEdit *Edit59;
	TEdit *Edit60;
	TEdit *Edit61;
	TEdit *Edit62;
	TEdit *Edit63;
	TEdit *Edit64;
	TEdit *Edit65;
	TEdit *Edit66;
	TEdit *Edit67;
	TEdit *Edit68;
	TEdit *Edit69;
	TEdit *Edit70;
	TEdit *Edit71;
	TEdit *Edit72;
	TEdit *Edit73;
	TEdit *Edit74;
	TEdit *Edit75;
	TEdit *Edit76;
	TEdit *Edit77;
	TEdit *Edit78;
	TEdit *Edit79;
	TEdit *Edit80;
	TEdit *Edit81;
	TEdit *Edit82;
	TEdit *Edit83;
	TEdit *Edit84;
	TButton *Button1;
	TButton *Button2;
	TComboBox *CBTrk;
	TLabel *Label7;
	TLabel *Label8;
	TComboBox *CBVaihe;
	TButton *Button4;
	void __fastcall EdNo0Change(TObject *Sender);
	void __fastcall Edit1Change(TObject *Sender);
	void __fastcall Edit7Change(TObject *Sender);
	void __fastcall Edit13Change(TObject *Sender);
	void __fastcall Edit19Change(TObject *Sender);
	void __fastcall Edit25Change(TObject *Sender);
	void __fastcall Edit31Change(TObject *Sender);
	void __fastcall Edit37Change(TObject *Sender);
	void __fastcall Edit43Change(TObject *Sender);
	void __fastcall Edit49Change(TObject *Sender);
	void __fastcall Edit55Change(TObject *Sender);
	void __fastcall Edit61Change(TObject *Sender);
	void __fastcall Edit67Change(TObject *Sender);
	void __fastcall Edit73Change(TObject *Sender);
	void __fastcall Edit79Change(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall EdNo0KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit7KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit13KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit19KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit25KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit31KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit37KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit43KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit49KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit55KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit61KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit67KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit73KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Edit79KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EdNo0KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
private:	// User declarations
	TEdit *EdNo[MERKEDITLKM];
	TEdit *EdSrj[MERKEDITLKM];
	TEdit *EdNimi[MERKEDITLKM];
	TEdit *EdLA[MERKEDITLKM];
	TEdit *EdTrk[MERKEDITLKM];
	TEdit *EdTls[MERKEDITLKM];
public:		// User declarations
	void __fastcall EdNoChange(int i);
	__fastcall TFormTarkMerk(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTarkMerk *FormTarkMerk;
//---------------------------------------------------------------------------
#endif
