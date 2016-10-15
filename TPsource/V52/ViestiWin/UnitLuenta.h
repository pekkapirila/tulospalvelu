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

#ifndef UnitLuentaH
#define UnitLuentaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "VDeclare.h"
#include <Vcl.Menus.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormLuenta : public TForm
{
__published:	// IDE-managed Components
	TEdit *EdtViivakoodi;
	TLabel *Label1;
	TLabel *Label2;
	TEdit *EdtKilpno;
	TLabel *Label3;
	TEdit *EdtOsuus;
	TRadioGroup *RGTunnistus;
	TLabel *Label4;
	TEdit *EdtJoukkue;
	TButton *BtnTallenna;
	TButton *BtnPeruuta;
	TDrawGrid *DG1;
	TLabel *Label5;
	TEdit *EdtBadge;
	TMemo *MemoMsg;
	TEdit *EdtMsg;
	TLabel *Label6;
	TEdit *EdtVahvistus;
	TLabel *Label7;
	TLabel *Label8;
	TRadioGroup *RGkoodi;
	TStringGrid *SG1;
	TLabel *Label9;
	TCheckBox *CBEhdOsuus;
	TMainMenu *MainMenu1;
	TMenuItem *Valinnat1;
	TMenuItem *Huomautaohitetusta;
	TMenuItem *Huomautasuljetusta;
	TMenuItem *Help1;
	TProgressBar *ProgressBar1;
	TMenuItem *Laitteet1;
	TMenuItem *Emitlaitteidenyhteydetjaohjaus1;
	void __fastcall DG1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect, TGridDrawState State);
	void __fastcall EdtViivakoodiKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall RGTunnistusClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall EdtVahvistusKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall EdtVahvistusEnter(TObject *Sender);
	void __fastcall EdtVahvistusExit(TObject *Sender);
	void __fastcall EdtKilpnoKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EdtOsuusKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall BtnTallennaClick(TObject *Sender);
	void __fastcall BtnPeruutaClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall HuomautaohitetustaClick(TObject *Sender);
	void __fastcall HuomautasuljetustaClick(TObject *Sender);
	void __fastcall Help1Click(TObject *Sender);
	void __fastcall EdtViivakoodiEnter(TObject *Sender);
	void __fastcall EdtViivakoodiExit(TObject *Sender);
	void __fastcall EdtKilpnoEnter(TObject *Sender);
	void __fastcall EdtKilpnoExit(TObject *Sender);
	void __fastcall EdtOsuusEnter(TObject *Sender);
	void __fastcall EdtOsuusExit(TObject *Sender);
	void __fastcall Emitlaitteidenyhteydetjaohjaus1Click(TObject *Sender);

private:	// User declarations
	MESSAGE void __fastcall NewCardHandler(TMyMessage &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYMSGNEWCARD, TMyMessage, NewCardHandler);
END_MESSAGE_MAP(TComponent)

	int __fastcall NaytaJoukkue(void);
	void __fastcall Nollaa(void);
	int __fastcall TarkKoodi(int badge);
	cellTp Cells[5][MAXOSUUSLUKU];
	kilptietue Kilp;
	int Kilpno;
	int DKilp;
	int Osuus;
	int Badge;
	int Tietue;
	bool OdottaaKorttia;
	TextFl *luentaFl;
	wchar_t lokiLine[200];
public:		// User declarations
	__fastcall TFormLuenta(TComponent* Owner);
	__fastcall ~TFormLuenta(void);
	void __fastcall HaeUusiTietue(void);
	int applyParams(void);
	void haeParams(void);
	luentaIkkParamClass IkkParam;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormLuenta *FormLuenta;
//---------------------------------------------------------------------------
#endif
