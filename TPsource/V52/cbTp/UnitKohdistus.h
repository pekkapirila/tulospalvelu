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

#ifndef UnitKohdistusH
#define UnitKohdistusH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
#ifndef MAXOSUUSLUKU
#include "HkDeclare.h"
#else
#include "VDeclare.h"
#endif
//---------------------------------------------------------------------------
class TFormKohdistus : public TForm
{
__published:	// IDE-managed Components
	TGroupBox *GroupBox1;
	TEdit *EdtKdi1;
	TLabel *Label1;
	TLabel *Label2;
	TEdit *EdtMapX1;
	TEdit *EdtMapY1;
	TLabel *Label3;
	TEdit *EdtX1;
	TEdit *EdtY1;
	TCheckBox *CBHae1;
	TUpDown *UpDownX1;
	TUpDown *UpDownY1;
	TGroupBox *GroupBox2;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TEdit *EdtKdi2;
	TEdit *EdtMapX2;
	TEdit *EdtMapY2;
	TEdit *EdtX2;
	TEdit *EdtY2;
	TCheckBox *CBHae2;
	TUpDown *UpDownX2;
	TGroupBox *GroupBox3;
	TLabel *Label8;
	TLabel *Label9;
	TEdit *EdtMapRight;
	TEdit *EdtMapBottom;
	TEdit *EdtMapWidth;
	TEdit *EdtMapHeight;
	TLabel *Label7;
	TEdit *EdtMapLeft;
	TEdit *EdtMapTop;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TLabel *Label10;
	TUpDown *UpDownMapL;
	TUpDown *UpDownMapT;
	TUpDown *UpDownMapR;
	TLabel *Label11;
	TLabel *Label12;
	TBitBtn *BitBtn1;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall EdtKdi1Exit(TObject *Sender);
	void __fastcall EdtKdi2Exit(TObject *Sender);
	void __fastcall EdtXYExit(TObject *Sender);
	void __fastcall CBHae1Click(TObject *Sender);
	void __fastcall CBHae2Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall UpDownX1Click(TObject *Sender, TUDBtnType Button);
	void __fastcall UpDownY1Click(TObject *Sender, TUDBtnType Button);
	void __fastcall UpDownX2Click(TObject *Sender, TUDBtnType Button);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall UpDownMapLClick(TObject *Sender, TUDBtnType Button);
	void __fastcall UpDownMapTClick(TObject *Sender, TUDBtnType Button);
	void __fastcall UpDownMapRClick(TObject *Sender, TUDBtnType Button);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall EdtKdi1KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EdtKdi2KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EdtX1Exit(TObject *Sender);
	void __fastcall EdtY1Exit(TObject *Sender);
	void __fastcall EdtX2Exit(TObject *Sender);
private:	// User declarations
	void __fastcall PaivitaKohdistus(void);
	void __fastcall PaivitaKohdistus2(void);
	void __fastcall LaskeKohdistus(void);
	double xLeft;
	double xRight;
	double yTop;
	double yBtm;
	int X1, Y1, X2, Y2;
	double MapX1, MapY1, MapX2, MapY2;
public:		// User declarations
	__fastcall TFormKohdistus(TComponent* Owner);
	void __fastcall HiiriKohdista(int x, int y);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormKohdistus *FormKohdistus;
//---------------------------------------------------------------------------
#endif
