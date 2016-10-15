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

#ifndef UnitComfileH
#define UnitComfileH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Grids.hpp>
#include "UnitMain.h"
#include <Vcl.Grids.hpp>

class TestausIkkParamClass {
public:
	int ValiLaji;
	int VertAika;
	double ValiKerroin;
	int LahJono;
	wchar_t Tiedosto[200];
	int TiedJono;
	int LahYhteys;
	TestausIkkParamClass(void);
	void operator=(TestausIkkParamClass&);
	int writeParams(TextFl *outfl, int level);
	int readParams(xml_node *node, int *inode, int nnode);
};

//---------------------------------------------------------------------------
class TFormComfile : public TForm
{
__published:	// IDE-managed Components
	TLabeledEdit *ETietuekoko;
	TLabeledEdit *EJonoja;
	TLabeledEdit *ESanomia;
	TButton *Button1;
	TOpenDialog *OpenDialog1;
	TStringGrid *SGJonot;
	TLabeledEdit *EdtJonoVal;
	TLabeledEdit *EdSanoma;
	TLabeledEdit *EdKilpno;
	TButton *Button2;
	TCheckBox *CheckBox1;
	TStringGrid *SGJono;
	TLabeledEdit *EdPos;
	TButton *Button3;
	TStringGrid *SGSanoma;
	TButton *Button4;
	TButton *Button5;
	TButton *Button6;
	TLabeledEdit *EdtBadge;
	TButton *BtnBadgehaku;
	TButton *Button7;
	TSaveDialog *SaveDialog1;
	TButton *BtnLaheta;
	TGroupBox *GBLahetys;
	TLabeledEdit *EdtLahAlku;
	TLabeledEdit *EdtLahLoppu;
	TLabeledEdit *EdtAloitus;
	TLabeledEdit *EdtValit;
	TRadioGroup *RGValit;
	TButton *BtnLahVal;
	TButton *BtnLahYksi;
	TRadioGroup *RGAloitus;
	TComboBox *CBLahYhteys;
	TLabel *Label1;
	TButton *BtnTallMuutos;
	TCheckBox *CBSalliMuokkaus;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall EdtJonoValExit(TObject *Sender);
	void __fastcall EdSanomaExit(TObject *Sender);
	void __fastcall SGJonoSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall EdtJonoValKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EdSanomaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall EdKilpnoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall EdPosKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall SGJonotDblClick(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall BtnBadgehakuClick(TObject *Sender);
	void __fastcall Button7Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BtnLahetaClick(TObject *Sender);
	void __fastcall RGValitClick(TObject *Sender);
	void __fastcall BtnLahYksiClick(TObject *Sender);
	void __fastcall BtnLahValClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall BtnTallMuutosClick(TObject *Sender);


private:	// User declarations
	MESSAGE void __fastcall PaivHandler(TMyMessage &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYPAIVITA, TMyMessage, PaivHandler);
END_MESSAGE_MAP(TComponent)

	UnicodeString ActComfile;
	combufrec ComBuf;
public:		// User declarations
	__fastcall TFormComfile(TComponent* Owner);
	FILE *avaacomfile(wchar_t *obfilename);
	void __fastcall JonoPituudet(void);
	void __fastcall naytaSanoma(int r);
	void __fastcall lataaJono(int jono);
	void haeParams(void);
	int applyParams(void);
	int RecLen;
	int NRec;
	int NJRec[MAX_LAHPORTTI];
	int NJLah[MAX_LAHPORTTI];
	int NJono;
	int Jono;
	int LahRivi;
	bool inSendCopy;
	FILE *ComFile;
	TestausIkkParamClass IkkParam;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormComfile *FormComfile;
//---------------------------------------------------------------------------
#endif
