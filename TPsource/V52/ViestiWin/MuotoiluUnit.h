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

#ifndef MuotoiluUnitH
#define MuotoiluUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Grids.hpp>
#include <Vcl.Buttons.hpp>
//---------------------------------------------------------------------------
class TFormMuotoilu : public TForm
{
__published:	// IDE-managed Components
   TMainMenu *MainMenu1;
   TMenuItem *Tiedosto1;
   TMenuItem *Poistu1;
   TOpenDialog *OpenDialog1;
   TSaveDialog *SaveDialog1;
	TPageControl *PageControl1;
	TTabSheet *TabSheet3;
	TTabSheet *TabSheet4;
	TLabel *Label5;
	TEdit *Edtpaaots;
	TLabel *Label6;
	TEdit *Edtalaots;
	TLabel *Label7;
	TEdit *Edtalaots2;
	TCheckBox *CBuusisivu;
	TCheckBox *CBsnimifl;
	TCheckBox *CBtpvafl;
	TCheckBox *CBtklofl;
	TCheckBox *CBotsikot;
	TCheckBox *CBvaihdanimet;
	TCheckBox *CBnollakarki;
	TTabSheet *TabSheet6;
	TLabel *Label10;
	TLabel *Label11;
	TEdit *EdtYlaMarg;
	TLabel *Label12;
	TEdit *EdtAlaMarg;
	TLabel *Label13;
	TEdit *EdtVasMarg;
	TLabel *Label14;
	TEdit *EdtOikMarg;
	TLabel *Label1;
	TLabel *Label2;
	TComboBox *CBPOfont;
	TEdit *EdtPOfsize;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *EdtPOrv;
	TComboBox *CBPOlaatu;
	TLabel *Label8;
	TLabel *Label9;
	TComboBox *CBAOfont;
	TEdit *EdtAOfsize;
	TEdit *EdtAOrv;
	TComboBox *CBAOlaatu;
	TLabel *Label15;
	TComboBox *CBDeffont;
	TEdit *EdtDeffsize;
	TEdit *EdtDefrv;
	TComboBox *CBDeflaatu;
	TLabel *Label16;
	TComboBox *CBVakfont;
	TEdit *EdtVakfsize;
	TEdit *EdtVakrv;
	TComboBox *CBVaklaatu;
	TStringGrid *SGtlsKirj;
	TStringGrid *SGtlsTied;
	TEdit *EdtSivPit;
	TLabel *Label17;
	TButton *BtnOta1;
	TEdit *Edtesirivit;
	TLabel *Label18;
	TButton *BtnOtaTul1;
	TStringGrid *SGtlsMKirj;
	TStringGrid *SGtlsMTied;
	TLabel *Label20;
	TLabel *Label21;
	TLabel *Label22;
	TLabel *Label23;
	TLabel *Label24;
	TLabel *Label25;
	TButton *BtnKirj1;
	TButton *BtnOtaTul2;
	TButton *BtnLue1;
	TMenuItem *Help1;
	TBitBtn *BitBtn2;
	TBitBtn *BitBtn4;
	TBitBtn *BitBtn5;
	TLabel *Label31;
	TComboBox *CBLihfont;
	TEdit *EdtLihfsize;
	TEdit *EdtLihrv;
	TComboBox *CBLihlaatu;
	TLabel *Label32;
	TComboBox *CBPienfont;
	TEdit *EdtPienfsize;
	TEdit *EdtPienrv;
	TComboBox *CBPienlaatu;
	TTabSheet *TabSheet1;
	TGroupBox *GroupBox1;
	TRadioGroup *RGTKirjRivit;
	TRadioGroup *RGTKirjFontit;
	TRadioGroup *RGTKirjOsno;
	TGroupBox *GroupBox2;
	TLabel *Label30;
	TEdit *EdtTTiedrivpit;
	TRadioGroup *RGTTiedRivit;
	TRadioGroup *RGTTiedFontit;
	TRadioGroup *RGTTiedOsno;
	TBitBtn *BitBtn1;
	TButton *BtnOtaTul3;
	TMenuItem *Luemuotoiluttiedostosta1;
	TMenuItem *allennamuotoiluttiedostoon1;
	TMenuItem *Luevanhamuotoilutiedosto1;
	TCheckBox *CBTKirjArvo;
	TCheckBox *CBTTiedArvo;
	TLabel *Label19;
	TTabSheet *TabSheet2;
	TGroupBox *GroupBox3;
	TLabel *Label40;
	TLabel *Label41;
	TLabel *Label43;
	TComboBox *CBffTied;
	TComboBox *CBfszTied;
	TRadioGroup *RGcssTied;
	TCheckBox *CBtaulHtmlTied;
	TRadioGroup *RGlinkitTied;
	TEdit *EdtVariRivitFile;
	TGroupBox *GroupBox4;
	TLabel *Label48;
	TLabel *Label49;
	TLabel *Label50;
	TLabel *Label51;
	TLabel *Label52;
	TLabel *Label62;
	TLabel *Label63;
	TGroupBox *GroupBox5;
	TLabel *Label58;
	TLabel *Label59;
	TEdit *EdtClHtml;
	TComboBox *CBVariKohdeHtml;
	TColorBox *ClBBgHtml;
	TColorBox *ClBFgHtml;
	TEdit *EdtPaaOtsKokoHtml;
	TEdit *EdtAlaOtsKokoHtml;
	TEdit *EdtHPadHtml;
	TEdit *EdtVPadHtml;
	TCheckBox *CBSarOtsLihHtml;
	TEdit *EdtViivatHtml;
	TEdit *EdtRvaViivatHtml;
	TEdit *EdtRVaKokoHtml;
	TButton *BtnOta4;
	TCheckBox *CBRvaLeveydet;
	TBitBtn *BitBtn3;
	TComboBox *CBSarLev;
	TEdit *EdLevKerr;
	TLabel *Label26;
	void __fastcall NaytaArvot(TObject *Sender);
   void __fastcall Poistu1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BtnOta1Click(TObject *Sender);
	void __fastcall BtnKirj1Click(TObject *Sender);
	void __fastcall BtnLue1Click(TObject *Sender);
	void __fastcall BtnOtaTul1Click(TObject *Sender);
	void __fastcall BtnOtaTul2Click(TObject *Sender);
	void __fastcall Help1Click(TObject *Sender);
	void __fastcall SGtlsKirjMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall SGtlsKirjMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall SGtlsTiedMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall SGtlsTiedMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall SGtlsMKirjMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall SGtlsMKirjMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall SGtlsMTiedMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall SGtlsMTiedMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall BtnOtaTul3Click(TObject *Sender);
	void __fastcall Luemuotoiluttiedostosta1Click(TObject *Sender);
	void __fastcall allennamuotoiluttiedostoon1Click(TObject *Sender);
	void __fastcall Luevanhamuotoilutiedosto1Click(TObject *Sender);
	void __fastcall BtnOta4Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ClBBgHtmlClick(TObject *Sender);
	void __fastcall CBVariKohdeHtmlChange(TObject *Sender);
	void __fastcall BitBtn3Click(TObject *Sender);
private:	// User declarations
	void __fastcall naytaTlsKirj(void);
	void __fastcall naytaTlsMKirj(void);
	void __fastcall naytaTlsMTied(void);
	void __fastcall naytaTlsTied(void);
	void __fastcall naytaTls(int lj);
	void __fastcall OtaTls(int lj);
	void naytaVariEsim(void);
	int LisaaOmaVari(int vari);
	TColorBox *ClBx[2];
	TStringGrid *SGTls[4];
public:		// User declarations
   __fastcall TFormMuotoilu(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMuotoilu *FormMuotoilu;
//---------------------------------------------------------------------------
#endif
