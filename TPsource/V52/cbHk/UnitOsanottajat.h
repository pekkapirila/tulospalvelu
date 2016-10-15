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

#ifndef UnitOsanottajatH
#define UnitOsanottajatH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include "HkDeclare.h"
#include <Vcl.Buttons.hpp>
#include <Vcl.Menus.hpp>

typedef struct {
	char key[80];
	int Dk;
} OoJarjTp;

//---------------------------------------------------------------------------
class TFormOsanottajat : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *OoGrid;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TComboBox *CBPaivat;
	TLabel *Lblpv;
	TLabel *Label1;
	TComboBox *CBJarjestys;
	TButton *BtnPaivita;
	TLabel *LblMuokkaus;
	TBitBtn *BitBtn1;
	TMainMenu *MainMenu1;
	TMenuItem *iedosto1;
	TMenuItem *Jrjestys1;
	TMenuItem *Suodata1;
	TMenuItem *Kentt1;
	TMenuItem *Help1;
	TMenuItem *Pivittiedot1;
	TMenuItem *MuokkaussallittuF91;
	TMenuItem *allennamuutokset1;
	TMenuItem *Peruutamuutokset1;
	TMenuItem *Kysymuutostenmr1;
	TMenuItem *Sulje1;
	TMenuItem *Perusnumero2;
	TMenuItem *Rintanumero1;
	TMenuItem *Sarja1;
	TMenuItem *Lisenssinumero1;
	TMenuItem *Kvidnumero1;
	TMenuItem *Nimi1;
	TMenuItem *Arvo1;
	TMenuItem *Seura1;
	TMenuItem *Seuralyhenne1;
	TMenuItem *Yhdistys1;
	TMenuItem *Joukkue1;
	TMenuItem *Maa1;
	TMenuItem *Aluepiiri1;
	TMenuItem *Emitkoodi1;
	TMenuItem *Lsn1;
	TMenuItem *Lhtaika1;
	TMenuItem *Tulos1;
	TMenuItem *Sakot1;
	TMenuItem *Vaiheensarja1;
	TMenuItem *Sukupx1;
	TMenuItem *Iksarjag1;
	TMenuItem *Rata1;
	TMenuItem *oiminnot1;
	TMenuItem *Lueemitkoodeja1;
	TMenuItem *Qual1;
	TMenuItem *GSM1;
	TMenuItem *Kiinteitsarakkeita11;
	TMenuItem *Er1;
	TMenuItem *Pisteet1;
	TMenuItem *Pisteet2;
	TMenuItem *Pisteet3;
	TMenuItem *Ilmoittautumislista1;
	TMenuItem *Vaihepist11;
	TMenuItem *Vaihepist21;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall OoGridSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall OoGridExit(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall OoGridDblClick(TObject *Sender);
	void __fastcall CBPaivatChange(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CBJarjestysChange(TObject *Sender);
	void __fastcall OoGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall OoGridKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall BtnPaivitaClick(TObject *Sender);
	void __fastcall OoGridFixedCellClick(TObject *Sender, int ACol, int ARow);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall Perusnumero2Click(TObject *Sender);
	void __fastcall Rintanumero1Click(TObject *Sender);
	void __fastcall Sarja1Click(TObject *Sender);
	void __fastcall Lisenssinumero1Click(TObject *Sender);
	void __fastcall Kvidnumero1Click(TObject *Sender);
	void __fastcall Nimi1Click(TObject *Sender);
	void __fastcall Arvo1Click(TObject *Sender);
	void __fastcall Seura1Click(TObject *Sender);
	void __fastcall Seuralyhenne1Click(TObject *Sender);
	void __fastcall Yhdistys1Click(TObject *Sender);
	void __fastcall Joukkue1Click(TObject *Sender);
	void __fastcall Maa1Click(TObject *Sender);
	void __fastcall Aluepiiri1Click(TObject *Sender);
	void __fastcall Emitkoodi1Click(TObject *Sender);
	void __fastcall Lsn1Click(TObject *Sender);
	void __fastcall Lhtaika1Click(TObject *Sender);
	void __fastcall Tulos1Click(TObject *Sender);
	void __fastcall Sakot1Click(TObject *Sender);
	void __fastcall MuokkaussallittuF91Click(TObject *Sender);
	void __fastcall Suodata1Click(TObject *Sender);
	void __fastcall Vaiheensarja1Click(TObject *Sender);
	void __fastcall OoGridMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall OoGridMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall Sukupx1Click(TObject *Sender);
	void __fastcall Iksarjag1Click(TObject *Sender);
	void __fastcall Rata1Click(TObject *Sender);
	void __fastcall Lueemitkoodeja1Click(TObject *Sender);
	void __fastcall Qual1Click(TObject *Sender);
	void __fastcall GSM1Click(TObject *Sender);
	void __fastcall Kiinteitsarakkeita11Click(TObject *Sender);
	void __fastcall Er1Click(TObject *Sender);
	void __fastcall Sulje1Click(TObject *Sender);
	void __fastcall Pisteet1Click(TObject *Sender);
	void __fastcall Pisteet2Click(TObject *Sender);
	void __fastcall Pisteet3Click(TObject *Sender);
	void __fastcall Ilmoittautumislista1Click(TObject *Sender);
	void __fastcall Vaihepist11Click(TObject *Sender);
	void __fastcall Vaihepist21Click(TObject *Sender);
//	void __fastcall BtnLisaaClick(TObject *Sender);
//	void __fastcall BtnPoistaClick(TObject *Sender);

private:	// User declarations
private:	// User declarations
	MESSAGE void __fastcall ECodeHandler(TMyMessage &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYECODE, TMyMessage, ECodeHandler);
END_MESSAGE_MAP(TComponent)
	void __fastcall initOoGrid(void);
	void __fastcall setOoGrid(void);
	void __fastcall teeOoJarj(void);
	int __fastcall tallennaTiedot(void);
	int __fastcall paivitaMuutos(int col, int row);
	bool __fastcall Mukana(int d);
	void __fastcall KilpKey(int d, char * key);
	void __fastcall hae_puutt_badge(void);
	int aktrow;
	int aktcol;
	int ColIx[100];
	int ColPv[100];
	int npv;
	int epv;
	int Oo_pv;
	int Oo_Jrjpv;
	int DKilp[10000];
	kilpindex ooindex;
	kilptietue *KilpA;
	int NKilp;
	bool initialisoitu;
	OoJarjTp * OoJarj;
public:		// User declarations
	void __fastcall naytaTiedot(void);
	__fastcall TFormOsanottajat(TComponent* Owner);
	bool sallimuokkaus;
	bool merklaina;
	int Oo_r_no;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormOsanottajat *FormOsanottajat;
//---------------------------------------------------------------------------
#endif
