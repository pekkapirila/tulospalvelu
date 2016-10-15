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

#ifndef UnitIlm2H
#define UnitIlm2H
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ExtCtrls.hpp>
#include "HkDeclare.h"
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Buttons.hpp>
//
struct TFormIlmValues {
	UnicodeString EId;
	UnicodeString ELisno;
	UnicodeString EWRKoodi;
	UnicodeString ESNimi;
	UnicodeString EENimi;
	UnicodeString EArvo;
	UnicodeString ESeura;
	UnicodeString ESeuralyh;
	UnicodeString EMaa;
	UnicodeString EYhdistys;
	UnicodeString EJoukkue;
	UnicodeString ELasna;
	UnicodeString EBadge[5];
	UnicodeString ELA[5];
	UnicodeString ETas[5];
	UnicodeString ERata[5];
	UnicodeString EPiiri;
	UnicodeString ESukup;
	UnicodeString EIkasrj;
	int CSarja;
};

//---------------------------------------------------------------------------
class TFormIlm2 : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *IlmGrid;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TPanel *Panel1;
	TLabel *Label13;
	TCheckBox *CBSarja;
	TCheckBox *CBId;
	TCheckBox *CBLisno;
	TCheckBox *CBWRKoodi;
	TCheckBox *CBSNimi;
	TCheckBox *CBENimi;
	TCheckBox *CBArvo;
	TCheckBox *CBSeura;
	TCheckBox *CBSeuralyh;
	TCheckBox *CBMaa;
	TCheckBox *CBYhdistys;
	TCheckBox *CBJoukkue;
	TCheckBox *CBLasna;
	TCheckBox *CBBadge;
	TCheckBox *CBLA;
	TCheckBox *CBTas;
	TCheckBox *CBRata;
	TCheckBox *CBPiiri;
	TCheckBox *CBJIilm;
	TCheckBox *CBAutoLaika;
	TCheckBox *CBAutoKno;
	TCheckBox *CBHTSarja;
	TCheckBox *CBSeuralisa;
	TCheckBox *CBSeurahaku;
	TCheckBox *CBLisnohaku;
	TCheckBox *CBNimiHThaku;
	TCheckBox *CBNimihaku;
	TCheckBox *CBKnohaku;
	TRadioGroup *RGLisays;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TLabel *Lblpv;
	TComboBox *CBJarjestys;
	TComboBox *CBPaivat;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TCheckBox *CBHTBadge;
	TComboBox *CSarja;
	TEdit *EId;
	TEdit *ELisno;
	TEdit *EWRKoodi;
	TEdit *ESNimi;
	TEdit *EENimi;
	TEdit *EArvo;
	TEdit *ESeura;
	TEdit *ESeuralyh;
	TEdit *EMaa;
	TEdit *EYhdistys;
	TEdit *EJoukkue;
	TEdit *ELasna;
	TEdit *EBadge1;
	TEdit *ELA1;
	TEdit *ETas1;
	TEdit *ERata1;
	TEdit *EPiiri;
	TButton *BtnTallenna;
	TEdit *ERata2;
	TEdit *ETas2;
	TEdit *ELA2;
	TEdit *EBadge2;
	TEdit *EBadge3;
	TEdit *ELA3;
	TEdit *ETas3;
	TEdit *ERata3;
	TEdit *ERata4;
	TEdit *ETas4;
	TEdit *ELA4;
	TEdit *EBadge4;
	TEdit *EBadge5;
	TEdit *ELA5;
	TEdit *ETas5;
	TEdit *ERata5;
	TLabel *LSarja;
	TLabel *LKilpno;
	TLabel *LLisno;
	TLabel *LWRKoodi;
	TLabel *LSNimi;
	TLabel *LENimi;
	TLabel *LArvo;
	TLabel *LSeura;
	TLabel *LSeuralyh;
	TLabel *LMaa;
	TLabel *LYhdistys;
	TLabel *LJoukkue;
	TLabel *LLasna;
	TLabel *LBadge;
	TLabel *LLA;
	TLabel *LTas;
	TLabel *LRata;
	TLabel *LPiiri;
	TEdit *OSarja;
	TLabel *Label2;
	TEdit *OId;
	TEdit *OKilpno;
	TEdit *OLisno;
	TEdit *OWRKoodi;
	TEdit *OSNimi;
	TEdit *OENimi;
	TEdit *OArvo;
	TEdit *OSeura;
	TEdit *OSeuralyh;
	TEdit *OJoukkue;
	TEdit *OYhdistys;
	TEdit *OMaa;
	TEdit *OLasna;
	TEdit *OBadge;
	TEdit *OLA;
	TEdit *OTas;
	TEdit *ORata;
	TEdit *OPiiri;
	TLabel *LBib;
	TEdit *EBib1;
	TEdit *EBib2;
	TEdit *EBib3;
	TEdit *EBib4;
	TEdit *EBib5;
	TCheckBox *CBBib;
	TEdit *OBib;
	TEdit *EKilpno;
	TLabel *Label4;
	TLabel *LPysyva;
	TCheckBox *CBKilpno;
	TCheckBox *CBLisnoTark;
	TButton *BtnPeruuta;
	TBitBtn *BitBtn1;
	TLabel *LblMoodi;
	TButton *BtnEiBadge;
	TCheckBox *CBMerkAvoimeksi;
	TCheckBox *CBSukup;
	TCheckBox *CBIkasrj;
	TLabel *LSukup;
	TEdit *ESukup;
	TLabel *LIkasrj;
	TEdit *EIkasrj;
	TEdit *OSukup;
	TEdit *OIkasrj;
	TEdit *EdtVakBibLisa;
	TLabel *Label3;
	TCheckBox *CBJilm2;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall IlmGridSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall IlmGridExit(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall IlmGridDblClick(TObject *Sender);
	void __fastcall CBPaivatChange(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall CBJarjestysChange(TObject *Sender);
	void __fastcall CBSarjaClick(TObject *Sender);
	void __fastcall CBIdClick(TObject *Sender);
	void __fastcall CBLisnoClick(TObject *Sender);
	void __fastcall CBWRKoodiClick(TObject *Sender);
	void __fastcall CBSNimiClick(TObject *Sender);
	void __fastcall CBENimiClick(TObject *Sender);
	void __fastcall CBArvoClick(TObject *Sender);
	void __fastcall CBSeuraClick(TObject *Sender);
	void __fastcall CBSeuralyhClick(TObject *Sender);
	void __fastcall CBMaaClick(TObject *Sender);
	void __fastcall CBYhdistysClick(TObject *Sender);
	void __fastcall CBJoukkueClick(TObject *Sender);
	void __fastcall CBLasnaClick(TObject *Sender);
	void __fastcall CBBadgeClick(TObject *Sender);
	void __fastcall CBLAClick(TObject *Sender);
	void __fastcall CBTasClick(TObject *Sender);
	void __fastcall CBRataClick(TObject *Sender);
	void __fastcall CBPiiriClick(TObject *Sender);
	void __fastcall CBNimiHThakuClick(TObject *Sender);
	void __fastcall ESNimiChange(TObject *Sender);
	void __fastcall EENimiChange(TObject *Sender);
	void __fastcall ESNimiKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EIdKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall BtnTallennaClick(TObject *Sender);
	void __fastcall CBLisnohakuClick(TObject *Sender);
	void __fastcall ESeuraChange(TObject *Sender);
	void __fastcall CBSeurahakuClick(TObject *Sender);
	void __fastcall ESeuraKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall TabSheet1Show(TObject *Sender);
	void __fastcall RGLisaysClick(TObject *Sender);
	void __fastcall ELisnoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EnterKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall CSarjaChange(TObject *Sender);
	void __fastcall ESeuraEnter(TObject *Sender);
	void __fastcall ESeuraExit(TObject *Sender);
	void __fastcall BtnPeruutaClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall PageControl1Enter(TObject *Sender);
	void __fastcall TabSheet2Show(TObject *Sender);
	void __fastcall CBKilpnoClick(TObject *Sender);
	void __fastcall BtnEiBadgeClick(TObject *Sender);
	void __fastcall ESeuralyhEnter(TObject *Sender);
	void __fastcall IlmGridMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall IlmGridMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall CBSukupClick(TObject *Sender);
	void __fastcall CBIkasrjClick(TObject *Sender);
	void __fastcall EdtVakBibLisaChange(TObject *Sender);
	void __fastcall EditNoEnter(TObject *Sender);

//	void __fastcall BtnLisaaClick(TObject *Sender);
//	void __fastcall BtnPoistaClick(TObject *Sender);

private:	// User declarations
	void __fastcall naytaTiedot(int kno);
	int __fastcall tallennaTiedot(void);
	void __fastcall tyhjennaKentat(void);
	int __fastcall paivitaMuutos(int col, int row);
	void __fastcall naytaKilpailija(void);
	void __fastcall tallennaKilpailija(bool kysy);
	int __fastcall SeurPos(void *);
	void __fastcall SaveEdits(void);
	void __fastcall RestoreEdits(void);
	void __fastcall FormSetSize(int Sheet);
	void __fastcall Initialisoi(void);
	int annavakantinbib(kilptietue *kilp, int ipv);
	bool Initialisoitu;
	int aktrow;
	int aktcol;
	int edkno;
	int ColIx[100];
	int ColPv[100];
	int npv;
	int DKilp[10000];
	int Dk;
	int LSn;
	int LEn;
	int LSe;
	int HTBadge;
	bool NimiKeyHit;
	int SeuraKeyHit;
	kilptietue Kilp;
	kilptietue EKilp;
	kilptietue VKilp;
	int DVKilp;
	UINT16 *AktId;
	int EdKey;
	EdtField Edits[22];
	TWinControl *TabControls[40];
	TEdit *EBib[5];
	TEdit *EBadge[5];
	TEdit *ELA[5];
	TEdit *ETas[5];
	TEdit *ERata[5];
	TEdit *EAktSeura;
	struct TFormIlmValues FormIlmEdits;
	int SheetHeight;
public:		// User declarations
	__fastcall TFormIlm2(TComponent* Owner);
	void __fastcall PalaaHenkHausta(bool tallenna);
	void __fastcall PalaaSeuraHausta(bool tallenna);
	void __fastcall SiirraHTTiedot(int haku);
	void __fastcall SiirraSeuraTiedot(int haku);
	void __fastcall OlEdits(void);
	void __fastcall haeKilp(int d);
	int applyParams(void);
	void haeParams(void);
	ilmIkkParamClass IkkParam;
	TWinControl* __fastcall NextTabControl(TObject *Sender);
	bool sallimuokkaus;
	wchar_t Kohde[30];
};
//---------------------------------------------------------------------------
extern PACKAGE TFormIlm2 *FormIlm2;
//---------------------------------------------------------------------------
#endif
