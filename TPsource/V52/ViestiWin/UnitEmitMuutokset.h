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

#ifndef UnitEmitMuutoksetH
#define UnitEmitMuutoksetH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TFormEmitMuutokset : public TForm
{
__published:	// IDE-managed Components
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TGroupBox *GroupBox2;
	TButton *BtnPuolita;
	TButton *BtnKohdenna;
	TCheckBox *CBSalliMuutokset;
	TEdit *EdtLuenta;
	TButton *BtnTallennaEm;
	TButton *BtnPeruutaEm;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TEdit *EdtKilpno;
	TEdit *EdtNimi;
	TEdit *EdtBadge;
	TCheckBox *CBLuettu;
	TEdit *EdtSarja;
	TButton *BtnEdell;
	TButton *BtnSeur;
	TButton *BTnSiirra;
	TButton *BtnVaihda;
	TButton *BtnTallenna;
	TButton *BtnPeruuta;
	TEdit *EdtSeura;
	TButton *Button1;
	TButton *Button2;
	TBitBtn *BitBtn1;
	TMemo *MemoOhje;
	TButton *BtnSulje1;
	TLabel *Label3;
	TCheckBox *CBLuenta;
	TComboBox *CBOsuus;
	TLabel *Label4;
	TButton *BtnKasvata;
	TEdit *EdtLisa;
	TLabel *Label8;
	void __fastcall BtnPeruutaClick(TObject *Sender);
	void __fastcall BtnTallennaClick(TObject *Sender);
	void __fastcall CBSalliMuutoksetClick(TObject *Sender);
	void __fastcall EdtKilpnoExit(TObject *Sender);
	void __fastcall EdtKilpnoKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall BtnSeurClick(TObject *Sender);
	void __fastcall BtnKohdennaClick(TObject *Sender);
	void __fastcall EdtLuentaExit(TObject *Sender);
	void __fastcall EdtLuentaKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall BtnPuolitaClick(TObject *Sender);
	void __fastcall BtnVaihdaClick(TObject *Sender);
	void __fastcall BTnSiirraClick(TObject *Sender);
	void __fastcall BtnEdellClick(TObject *Sender);
	void __fastcall EdtKilpnoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall BtnTallennaEmClick(TObject *Sender);
	void __fastcall BtnPeruutaEmClick(TObject *Sender);
	void __fastcall BtnSulje1Click(TObject *Sender);
	void __fastcall EdtSeuraKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall CBOsuusChange(TObject *Sender);
	void __fastcall BtnKasvataClick(TObject *Sender);

private:	// User declarations
public:		// User declarations
	__fastcall TFormEmitMuutokset(TComponent* Owner);
	void __fastcall Nayta(int d, int Osuus);
	void __fastcall naytaMemoOhje(bool nayta);
	int Osuus;
	int DKilp;
	bool NimiHaku;
	bool JoukkueHaku;
	bool OsuusHaku;
	bool joTiedot;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEmitMuutokset *FormEmitMuutokset;
//---------------------------------------------------------------------------
#endif
