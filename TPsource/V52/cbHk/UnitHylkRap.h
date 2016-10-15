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

#ifndef UnitHylkRapH
#define UnitHylkRapH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>

#include <HkDeclare.h>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Buttons.hpp>

extern wchar_t RaporttiHak[202];
//---------------------------------------------------------------------------
class TFormHylkRap : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *EdtKilpno;
	TLabel *LblSeura;
	TEdit *EdtSeura;
	TLabel *Nimi;
	TEdit *EdtNimi;
	TLabel *Sarja;
	TEdit *EdtSarja;
	TRadioGroup *RGLuonne;
	TEdit *EdtAvausaika;
	TLabel *Label2;
	TLabel *Label3;
	TEdit *EdtTallennusaika;
	TRadioGroup *RGRatk_1;
	TRadioGroup *RGKilpKanta;
	TMemo *Memo1;
	TLabel *Label4;
	TLabel *Label5;
	TEdit *EdtSelitys;
	TRadioGroup *RGTuom;
	TMainMenu *MainMenu1;
	TMenuItem *iedosto1;
	TMenuItem *ValitseHakemisto1;
	TMenuItem *Avaahylkysraportti1;
	TMenuItem *allennanytetyttiedothylkysraporttiin1;
	TMenuItem *Suljekaavake1;
	TOpenDialog *OpenDialog1;
	TButton *BtnTallennaTulosta;
	TMenuItem *Valinnat1;
	TMenuItem *Valitsekirjoitin1;
	TButton *BtnTulosta;
	TButton *BtnTallenna;
	TRadioGroup *RGXmlLuku;
	TRadioGroup *RGPiikit;
	TLabel *Label6;
	TEdit *EdtBadge;
	TBitBtn *BitBtn1;
	TComboBox *CBNimihaku;
	TLabel *Label7;
	TEdit *EdtRata;
	TLabel *Label8;
	TEdit *EdtToimitsija;
	void __fastcall Suljekaavake1Click(TObject *Sender);
	void __fastcall ValitseHakemisto1Click(TObject *Sender);
	void __fastcall Avaahylkysraportti1Click(TObject *Sender);
	void __fastcall allennanytetyttiedothylkysraporttiin1Click(TObject *Sender);
	void __fastcall Valitsekirjoitin1Click(TObject *Sender);
	void __fastcall BtnTallennaClick(TObject *Sender);
	void __fastcall BtnTulostaClick(TObject *Sender);
	void __fastcall BtnTallennaTulostaClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall EdtKilpnoExit(TObject *Sender);
	void __fastcall EdtNimiChange(TObject *Sender);
	void __fastcall EdtNimiExit(TObject *Sender);
	void __fastcall EdtKilpnoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall CBNimihakuChange(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);

private:	// User declarations
	void  kirjoitaXmlRaportti(void);
	int tulkXMLRaportti(xml_node *node, int nnode);
	int  lueXmlRaportti(void);
	void  tulostaRaportti(void);
	void  NaytaTiedot(void);
	wchar_t *RapFile(void);
	void  hylk_rap(tulostusparamtp *tulprm);
	kilptietue Kilp;
	int KilpNo;
	int Badge;
	wchar_t AvausAika[30];
	wchar_t ReportDir[MAX_PATH-20];
	wchar_t RapKirjoitin[200];
	tulostusparamtp RapTulPrm;
public:		// User declarations
	__fastcall TFormHylkRap(TComponent* Owner);
	void NaytaKilpailija(int kno, int bdg);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormHylkRap *FormHylkRap;
//---------------------------------------------------------------------------
#endif
