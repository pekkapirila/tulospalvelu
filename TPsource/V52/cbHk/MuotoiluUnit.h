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
	TTabSheet *TabSheet1;
	TTabSheet *TabSheet2;
	TTabSheet *TabSheet3;
	TTabSheet *TabSheet4;
	TLabel *Label5;
	TEdit *Edtpaaots;
	TLabel *Label6;
	TEdit *Edtalaots;
	TLabel *Label7;
	TEdit *Edtalaots2;
	TCheckBox *CBsnimiflTied;
	TCheckBox *CBtpvaflTied;
	TCheckBox *CBtkloflTied;
	TCheckBox *CBotsikotTied;
	TCheckBox *CBvaihdanimetTied;
	TCheckBox *CBnollakarkiTied;
	TTabSheet *TabSheet6;
	TLabel *Label10;
	TLabel *Label11;
	TEdit *EdtYlaMarg;
	TLabel *Label12;
	TEdit *EdtAlaMarg;
	TLabel *Label13;
	TEdit *EdtVasMarg;
	TLabel *Label14;
	TEdit *Edit11;
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
	TStringGrid *SGllKirj;
	TStringGrid *SGllTied;
	TStringGrid *SGtlsKirj;
	TEdit *EdtSivPit;
	TLabel *Label17;
	TButton *BtnOta1;
	TEdit *Edtesirivit;
	TLabel *Label18;
	TButton *BtnOtaLl1;
	TButton *BtnOtaTul1;
	TStringGrid *SGtlsMKirj;
	TStringGrid *SGtls1Kirj;
	TButton *BtnOtaLl2;
	TLabel *Label19;
	TLabel *Label20;
	TLabel *Label21;
	TLabel *Label22;
	TLabel *Label24;
	TLabel *Label25;
	TButton *BtnKirj1;
	TButton *BtnKirjLl1;
	TButton *BtnKirjTul1;
	TButton *BtnLueLl1;
	TButton *BtnLueLl2;
	TButton *BtnKirjLl2;
	TButton *BtnLueTul1;
	TButton *BtnOtaTul2;
	TButton *BtnLueTul2;
	TButton *BtnKirjTul2;
	TLabel *Label27;
	TEdit *EdtLots0;
	TLabel *Label28;
	TEdit *EdtLots1;
	TLabel *Label29;
	TEdit *EdtLots3;
	TEdit *EdtLots21;
	TEdit *EdtLots22;
	TLabel *Label30;
	TButton *BtnLue1;
	TMenuItem *Help1;
	TLabel *Label31;
	TLabel *Label32;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TBitBtn *BitBtn3;
	TBitBtn *BitBtn4;
	TBitBtn *BitBtn5;
	TLabel *Label26;
	TMenuItem *Luemuotoiluttiedostosta1;
	TMenuItem *allennamuotoiluttiedostoon1;
	TMenuItem *Luevanhalhtluettelomuotoilu1;
	TMenuItem *Luevanhatulosmuotoilu1;
	TCheckBox *CBsnimiflKirj;
	TLabel *Label33;
	TLabel *Label34;
	TCheckBox *CBtpvaflKirj;
	TCheckBox *CBtkloflKirj;
	TCheckBox *CBotsikotKirj;
	TCheckBox *CBvaihdanimetKirj;
	TCheckBox *CBnollakarkiKirj;
	TCheckBox *CBlkmKirj;
	TCheckBox *CBlkmTied;
	TTabSheet *TabSheet5;
	TStringGrid *SGtlsMobil;
	TLabel *Label23;
	TLabel *Label35;
	TStringGrid *SGtlsTied;
	TLabel *Label36;
	TLabel *Label37;
	TButton *BtnOtaTul3;
	TLabel *Label38;
	TCheckBox *CBsnimiflMobil;
	TCheckBox *CBtpvaflMobil;
	TCheckBox *CBtkloflMobil;
	TCheckBox *CBlkmMobil;
	TCheckBox *CBotsikotMobil;
	TCheckBox *CBvaihdanimetMobil;
	TCheckBox *CBnollakarkiMobil;
	TBitBtn *BitBtn6;
	TCheckBox *CBautoOtsMobil;
	TCheckBox *CBautoOtsTied;
	TMenuItem *Lisamaarityksia1;
	TMenuItem *Csseditori1;
	TEdit *EdtPisteDesim;
	TLabel *Label45;
	TComboBox *CBDesSep;
	TLabel *Label46;
	TLabel *Label47;
	TTabSheet *TabSheet7;
	TGroupBox *GroupBox1;
	TLabel *Label40;
	TLabel *Label41;
	TLabel *Label43;
	TComboBox *CBffTied;
	TComboBox *CBfszTied;
	TRadioGroup *RGcssTied;
	TCheckBox *CBtaulHtmlTied;
	TRadioGroup *RGlinkitTied;
	TEdit *EdtVariRivitFile;
	TGroupBox *GroupBox2;
	TLabel *Label42;
	TLabel *Label44;
	TLabel *Label39;
	TComboBox *CBffMobil;
	TComboBox *CBfszMobil;
	TRadioGroup *RGcssMobil;
	TCheckBox *CBtaulHtmlMobil;
	TRadioGroup *RGlinkitMobil;
	TEdit *EdtVariRivitMobil;
	TBitBtn *BitBtn7;
	TButton *BtnOtaHtml;
	TGroupBox *GroupBox3;
	TLabel *Label48;
	TGroupBox *GroupBox4;
	TEdit *EdtClHtml;
	TColorDialog *ColorDialog1;
	TComboBox *CBVariKohdeHtml;
	TLabel *Label49;
	TEdit *EdtPaaOtsKokoHtml;
	TLabel *Label50;
	TEdit *EdtAlaOtsKokoHtml;
	TLabel *Label51;
	TEdit *EdtHPadHtml;
	TLabel *Label52;
	TEdit *EdtVPadHtml;
	TCheckBox *CBSarOtsLihHtml;
	TGroupBox *GroupBox5;
	TLabel *Label53;
	TLabel *Label54;
	TLabel *Label55;
	TLabel *Label56;
	TLabel *Label57;
	TGroupBox *GroupBox6;
	TEdit *EdtClMobil;
	TComboBox *CBVariKohdeMobil;
	TEdit *EdtPaaOtsKokoMobil;
	TEdit *EdtAlaOtsKokoMobil;
	TEdit *EdtHPadMobil;
	TEdit *EdtVPadMobil;
	TCheckBox *CBSarOtsLihMobil;
	TColorBox *ClBBgHtml;
	TColorBox *ClBFgHtml;
	TEdit *EdtViivatHtml;
	TEdit *EdtViivatMobil;
	TColorBox *ClBBgMobil;
	TColorBox *ClBFgMobil;
	TLabel *Label58;
	TLabel *Label59;
	TLabel *Label60;
	TLabel *Label61;
	TEdit *EdtRvaViivatHtml;
	TLabel *Label62;
	TLabel *Label63;
	TEdit *EdtRVaKokoHtml;
	TLabel *Label64;
	TEdit *EdtRvaViivatMobil;
	TLabel *Label65;
	TEdit *EdtRVaKokoMobil;
	TComboBox *CBSarLevTied;
	TComboBox *CBSarLevMobil;
	TEdit *EdMtkKirj;
	TEdit *EdMtkMobil;
	TEdit *EdMtkTied;
	TLabel *Label66;
	void __fastcall NaytaArvot(TObject *Sender);
   void __fastcall Poistu1Click(TObject *Sender);
	void __fastcall BtnOta1Click(TObject *Sender);
	void __fastcall BtnKirj1Click(TObject *Sender);
	void __fastcall BtnOtaLl1Click(TObject *Sender);
	void __fastcall BtnOtaLl2Click(TObject *Sender);
	void __fastcall BtnLue1Click(TObject *Sender);
	void __fastcall BtnLueLl1Click(TObject *Sender);
	void __fastcall BtnKirjLl1Click(TObject *Sender);
	void __fastcall BtnLueLl2Click(TObject *Sender);
	void __fastcall BtnKirjLl2Click(TObject *Sender);
	void __fastcall BtnOtaTul1Click(TObject *Sender);
	void __fastcall BtnOtaTul2Click(TObject *Sender);
	void __fastcall Help1Click(TObject *Sender);
	void __fastcall SGllKirjMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall SGllKirjMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall SGllTiedMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall SGllTiedMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
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
	void __fastcall SGtls1KirjMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall SGtls1KirjMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Luemuotoiluttiedostosta1Click(TObject *Sender);
	void __fastcall allennamuotoiluttiedostoon1Click(TObject *Sender);
	void __fastcall Luevanhatulosmuotoilu1Click(TObject *Sender);
	void __fastcall BtnOtaTul3Click(TObject *Sender);
	void __fastcall SGtlsMobilMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall SGtlsMobilMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
		  bool &Handled);
	void __fastcall CBtaulHtmlTiedClick(TObject *Sender);
	void __fastcall CBtaulHtmlMobilClick(TObject *Sender);
	void __fastcall Csseditori1Click(TObject *Sender);
	void __fastcall BtnOtaHtmlClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ClBBgHtmlClick(TObject *Sender);
	void __fastcall CBVariKohdeHtmlChange(TObject *Sender);
	void __fastcall CBVariKohdeMobilChange(TObject *Sender);
	void __fastcall BitBtn7Click(TObject *Sender);
private:	// User declarations
	void __fastcall naytaTlsKirj(void);
	void __fastcall naytaTlsMKirj(void);
	void __fastcall naytaTls1Kirj(void);
	void __fastcall naytaTlsTied(void);
	void __fastcall naytaTlsMobil(void);
	void __fastcall naytaLlKirj(void);
	void __fastcall naytaLlTied(void);
	void __fastcall naytaTls(int lj);
	void __fastcall naytaLl(int lj);
	void naytaVariEsim(int i);
	int LisaaOmaVari(int vari);
	TStringGrid *SGTls[5];
	TStringGrid *SGLl[2];
	TColorBox *ClBx[2][2];
	TEdit *EdtCl[2];
	TComboBox *CBVariKohde[2];
public:		// User declarations
   __fastcall TFormMuotoilu(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMuotoilu *FormMuotoilu;
//---------------------------------------------------------------------------
#endif
