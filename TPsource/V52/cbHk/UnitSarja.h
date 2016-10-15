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

#ifndef UnitSarjaH
#define UnitSarjaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include "HkDef.h"
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TFormSarja : public TForm
{
__published:	// IDE-managed Components
	TEdit *EdtSarja;
	TEdit *PNimi;
	TEdit *EPv;
	TEdit *VPv;
	TEdit *Alku;
	TEdit *Bib1;
	TEdit *La1;
	TEdit *JLa1;
	TEdit *LV1;
	TEdit *Pari1;
	TEdit *LS1;
	TEdit *TS1;
	TEdit *SNimi;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label15;
	TEdit *Bib2;
	TEdit *La2;
	TEdit *JLa2;
	TEdit *LV2;
	TEdit *Pari2;
	TEdit *LS2;
	TEdit *TS2;
	TEdit *Mtk2;
	TEdit *Bib3;
	TEdit *La3;
	TEdit *JLa3;
	TEdit *LV3;
	TEdit *Pari3;
	TEdit *LS3;
	TEdit *TS3;
	TEdit *Mtk3;
	TEdit *Bib4;
	TEdit *La4;
	TEdit *JLa4;
	TEdit *LV4;
	TEdit *Pari4;
	TEdit *LS4;
	TEdit *TS4;
	TEdit *Mtk4;
	TEdit *Bib5;
	TEdit *La5;
	TEdit *JLa5;
	TEdit *LV5;
	TEdit *Pari5;
	TEdit *LS5;
	TEdit *TS5;
	TEdit *Mtk5;
	TLabel *Label16;
	TLabel *Label17;
	TEdit *Mks;
	TLabel *Label18;
	TLabel *Label19;
	TEdit *MksAl;
	TLabel *Label20;
	TEdit *Mks1;
	TComboBox *CBSak;
	TLabel *LbV1;
	TLabel *LbV2;
	TLabel *LbV3;
	TLabel *LbV4;
	TLabel *LbV5;
	TEdit *Mtk1;
	TButton *BtnTallenna;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TLabel *Label27;
	TEdit *L1;
	TEdit *L2;
	TEdit *L3;
	TEdit *L4;
	TEdit *L5;
	TLabel *Label28;
	TEdit *M1;
	TEdit *M2;
	TEdit *M3;
	TEdit *M4;
	TEdit *M5;
	TButton *Button5;
	TBitBtn *BitBtn1;
	TButton *BtnSeurSrj;
	TButton *BtnEdSrj;
	TCheckBox *CBVakanttisarja;
	TLabel *Label22;
	TEdit *Port5;
	TEdit *Port4;
	TEdit *Port3;
	TEdit *Port2;
	TEdit *Port1;
	TComboBox *CBLuonne1;
	TComboBox *CBLuonne2;
	TComboBox *CBLuonne3;
	TComboBox *CBLuonne4;
	TComboBox *CBLuonne5;
	TLabel *Label29;
	TEdit *EdtFlags1;
	TEdit *EdtFlags2;
	TEdit *EdtFlags3;
	TEdit *EdtFlags4;
	TEdit *EdtFlags5;
	TComboBox *CBSprintSrj;
	TLabel *Label31;
	TGroupBox *GBAmpSuunn;
	TComboBox *CBVaiheAS;
	TLabel *Label32;
	TLabel *Label33;
	TComboBox *CBAsLaji1;
	TEdit *EdtAsSakko1;
	TLabel *Label34;
	TLabel *Label35;
	TComboBox *CBAsLaji2;
	TEdit *EdtAsSakko2;
	TComboBox *CBAsLaji3;
	TEdit *EdtAsSakko3;
	TComboBox *CBAsLaji4;
	TEdit *EdtAsSakko4;
	TComboBox *CBAsLaji5;
	TEdit *EdtAsSakko5;
	TLabel *Label21;
	TEdit *Tapa1;
	TEdit *Tapa2;
	TEdit *Tapa3;
	TEdit *Tapa4;
	TEdit *Tapa5;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall BtnTallennaClick(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BtnEdSrjClick(TObject *Sender);
	void __fastcall BtnSeurSrjClick(TObject *Sender);
	void __fastcall MksExit(TObject *Sender);
	void __fastcall MksAlExit(TObject *Sender);
	void __fastcall Mks1Exit(TObject *Sender);
	void __fastcall La1Exit(TObject *Sender);
	void __fastcall La2Exit(TObject *Sender);
	void __fastcall La3Exit(TObject *Sender);
	void __fastcall La4Exit(TObject *Sender);
	void __fastcall La5Exit(TObject *Sender);
	void __fastcall JLa1Exit(TObject *Sender);
	void __fastcall JLa2Exit(TObject *Sender);
	void __fastcall JLa3Exit(TObject *Sender);
	void __fastcall JLa4Exit(TObject *Sender);
	void __fastcall JLa5Exit(TObject *Sender);
	void __fastcall Mtk1Exit(TObject *Sender);
	void __fastcall Mtk2Exit(TObject *Sender);
	void __fastcall Mtk3Exit(TObject *Sender);
	void __fastcall Mtk4Exit(TObject *Sender);
	void __fastcall Mtk5Exit(TObject *Sender);
	void __fastcall CBSakClick(TObject *Sender);
	void __fastcall SNimiExit(TObject *Sender);
	void __fastcall PNimiExit(TObject *Sender);
	void __fastcall VPvExit(TObject *Sender);
	void __fastcall EPvExit(TObject *Sender);
	void __fastcall CBSprintSrjClick(TObject *Sender);
	void __fastcall CBVaiheASChange(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall TapaExit(TObject *Sender);
private:	// User declarations
	TComboBox *CBAsLaji[5];
	TEdit *EdtAsSakko[5];
	int as_pv;
public:		// User declarations
	__fastcall TFormSarja(TComponent* Owner);
	void __fastcall naytaSarja(void);
	void __fastcall lataaSarja(int Srj);
	void __fastcall tallSarja(void);
	void __fastcall naytaPaivat(void);
	void __fastcall LaExit(int ipv);
	void __fastcall JLaExit(int ipv);
	void __fastcall MtkExit(int ipv);
	sarjatietue Sarja1;
	int SarjaNo;
	TEdit *EL[5];
	TEdit *EPort[5];
	TEdit *EM[5];
	TEdit *ELa[5];
	TEdit *EJLa[5];
	TEdit *ELS[5];
	TEdit *ETS[5];
	TEdit *EBib[5];
	TEdit *EMtk[5];
	TEdit *ETapa[5];
	TEdit *EPari[5];
	TEdit *ELV[5];
	TEdit *EdtFlags[5];
	TLabel *LbV[5];
	TComboBox *CBLuonne[5];
	bool pakotaIsot;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSarja *FormSarja;
//---------------------------------------------------------------------------
#endif
