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

#ifndef UnitEmiTagH
#define UnitEmiTagH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Buttons.hpp>
#ifdef MAXOSUUSLUKU
#include "VDeclare.h"
#else
#include "HkDeclare.h"
#endif
#include <Vcl.ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TFormEmiTag : public TForm
{
__published:	// IDE-managed Components
	TSaveDialog *SaveDialog1;
	TPageControl *PageControl1;
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TPanel *Panel1;
	TLabel *Label1;
	TComboBox *CBAsema;
	TLabel *Label2;
	TEdit *EdtVersio;
	TLabel *Label3;
	TEdit *EdtMoodi;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TLabel *Label10;
	TGroupBox *GroupBox1;
	TLabel *Label11;
	TLabel *Label12;
	TButton *Button2;
	TRadioGroup *RGSanomat;
	TEdit *EdtFirst;
	TRadioGroup *RGkohde;
	TEdit *EdtTallennettu;
	TEdit *EdtCode;
	TEdit *EdtAlku;
	TEdit *EdtLoppu;
	TEdit *EdtTime;
	TEdit *EdtRcvTime;
	TEdit *EdtBattery;
	TEdit *EdtComm;
	TButton *BtnMoodi;
	TButton *BtnCode;
	TButton *Button3;
	TCheckBox *CBPaivTiedot;
	TLabel *Label4;
	TButton *BtnTime;
	TButton *Button1;
	TButton *BtnSulje;
	TPanel *Panel2;
	TStringGrid *SGLeimat;
	TLabel *Label13;
	TLabel *Label14;
	TButton *BtnAvaa;
	TButton *BtnSuljeYhteys;
	TPopupMenu *PopupMenuLaite;
	TMenuItem *Lukija1;
	TMenuItem *emiTag1;
	TMenuItem *LaiteMTR1;
	TPopupMenu *PopupMenuLaiteYht;
	TMenuItem *LaiteSarjaportti;
	TMenuItem *LaiteNopeaUSB;
	TMenuItem *LaiteTCPasiakas;
	TMenuItem *LaiteTCPpalvelin;
	TMenuItem *LaiteUDP;
	TPopupMenu *PopupMenuLaiteAjat;
	TMenuItem *Eiaikoja1;
	TMenuItem *Maali1;
	TMenuItem *Valiaika1;
	TMenuItem *Valiaika2;
	TMenuItem *SjBox1;
	TMenuItem *LaiteVuorot;
	TMenuItem *Lahdekoodi1;
	TBitBtn *BitBtn1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall BtnTimeClick(TObject *Sender);
	void __fastcall BtnMoodiClick(TObject *Sender);
	void __fastcall BtnCodeClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall BtnSuljeClick(TObject *Sender);
	void __fastcall CBPaivTiedotClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall BtnAvaaClick(TObject *Sender);
	void __fastcall SGLeimatMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
          int X, int Y);
	void __fastcall LaiteClick(TObject *Sender);
	void __fastcall LaiteYhtClick(TObject *Sender);
	void __fastcall LaiteAjatClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall BtnSuljeYhteysClick(TObject *Sender);
private:	// User declarations
	void PaivitaYhteydet(void);
	void alustaEmiTag(void);
	MESSAGE void __fastcall PaivHandler(TMyMessage &msg);
	MESSAGE void __fastcall TiedotHandler(TMyMessage &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYPAIVITA, TMyMessage, PaivHandler);
	MESSAGE_HANDLER(WM_MYTIEDOT, TMyMessage, TiedotHandler);
END_MESSAGE_MAP(TComponent)
public:		// User declarations
	__fastcall TFormEmiTag(TComponent* Owner);
	void __fastcall LueEmiTagTiedot(void);
	int r_no;
	bool TiedotOn;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEmiTag *FormEmiTag;
//---------------------------------------------------------------------------
#endif
