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

#ifndef UnitSarjatiedotH
#define UnitSarjatiedotH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Dialogs.hpp>
//---------------------------------------------------------------------------
class TFormSarjatiedot : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *SG1;
	TButton *BtnMuokkaus;
	TButton *BtnTallenna;
	TButton *BtnPeruuta;
	TButton *Button4;
	TButton *Button5;
	TComboBox *CBJarj;
	TComboBox *CBTiedot;
	TMainMenu *MainMenu1;
	TMenuItem *iedosto1;
	TMenuItem *Sulje1;
	TMenuItem *Asetukset1;
	TMenuItem *Nytlhtajat1;
	TLabel *Label1;
	TLabel *Label2;
	TBitBtn *BitBtn1;
	TLabel *LblMuokkaus;
	TMenuItem *Vakanttinumeroitasarjojenvliss1;
	TMenuItem *Vapaataaikaasarjojenvliss1;
	TMenuItem *Automaattisetmuutokset1;
	TMenuItem *Mrsarjojenalkunumerot1;
	TMenuItem *Mrsarjojenlhtajat1;
	TMenuItem *Alkunumero1;
	TMenuItem *Sakollisuus1;
	TMenuItem *Lhtvli1;
	TMenuItem *Jlkiilmlht1;
	TMenuItem *Lhtpaikka1;
	TMenuItem *Maali1;
	TMenuItem *Maksut1;
	TMenuItem *Matka1;
	TMenuItem *Pitknimi1;
	TMenuItem *Ampumapaikat1;
	TMenuItem *Sakkoyksikk1;
	TMenuItem *N1vaihe1;
	TMenuItem *Viimvaihe1;
	TMenuItem *Ryhmkoko1;
	TMenuItem *Siiraikoja1;
	TMenuItem *Listoiminnot1;
	TMenuItem *Pisteidenlaskenta1;
	TMenuItem *Kirjoitacsvtiedostoon1;
	TMenuItem *Luecsvtiedostosta1;
	TOpenDialog *OpenDialog1;
	TSaveDialog *SaveDialog1;
	TPopupMenu *PopupMenu1;
	TMenuItem *Kopioivalittunaolevasolukaikkiinsarjoihin1;
	TMenuItem *Ohita1;
	TMenuItem *Kiinteitsarakkeita11;
	TMenuItem *Hiihtokilpailunperusmalli1;
	TMenuItem *Lissarja1;
	TMenuItem *Otalhthuomioon1;
	TMenuItem *Lisvakantitsarjojenedelle1;
	TLabel *LblLVal;
	TComboBox *CBLVal;
	TMenuItem *Haematkatratatiedoista1;
	TMenuItem *Kopioivaiheenalkunumerotperustietoihin1;
	TMenuItem *Kopioiperustietojenalkunumerotvaiheelle1;
	TMenuItem *Bibalku1;
	TMenuItem *Pakotasarjanimetisoillakirjaimilla1;
	TLabel *Label3;
	TMenuItem *Vaiheenluonne1;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall BtnPeruutaClick(TObject *Sender);
	void __fastcall BtnMuokkausClick(TObject *Sender);
	void __fastcall BtnTallennaClick(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall SG1DblClick(TObject *Sender);
	void __fastcall SG1SelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall SG1Exit(TObject *Sender);
	void __fastcall CBTiedotChange(TObject *Sender);
	void __fastcall CBJarjChange(TObject *Sender);
	void __fastcall Sulje1Click(TObject *Sender);
	void __fastcall Nytlhtajat1Click(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall SG1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall SG1FixedCellClick(TObject *Sender, int ACol, int ARow);
	void __fastcall Vakanttinumeroitasarjojenvliss1Click(TObject *Sender);
	void __fastcall Vapaataaikaasarjojenvliss1Click(TObject *Sender);
	void __fastcall Mrsarjojenalkunumerot1Click(TObject *Sender);
	void __fastcall Mrsarjojenlhtajat1Click(TObject *Sender);
	void __fastcall Alkunumero1Click(TObject *Sender);
	void __fastcall Sakollisuus1Click(TObject *Sender);
	void __fastcall N1vaihe1Click(TObject *Sender);
	void __fastcall Viimvaihe1Click(TObject *Sender);
	void __fastcall Lhtvli1Click(TObject *Sender);
	void __fastcall Jlkiilmlht1Click(TObject *Sender);
	void __fastcall Lhtpaikka1Click(TObject *Sender);
	void __fastcall Maali1Click(TObject *Sender);
	void __fastcall Ryhmkoko1Click(TObject *Sender);
	void __fastcall Ampumapaikat1Click(TObject *Sender);
	void __fastcall Sakkoyksikk1Click(TObject *Sender);
	void __fastcall Maksut1Click(TObject *Sender);
	void __fastcall Matka1Click(TObject *Sender);
	void __fastcall Pitknimi1Click(TObject *Sender);
	void __fastcall Siiraikoja1Click(TObject *Sender);
	void __fastcall Pisteidenlaskenta1Click(TObject *Sender);
	void __fastcall Kirjoitacsvtiedostoon1Click(TObject *Sender);
	void __fastcall Luecsvtiedostosta1Click(TObject *Sender);
	void __fastcall SG1MouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall SG1MouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall Kopioivalittunaolevasolukaikkiinsarjoihin1Click(TObject *Sender);
	void __fastcall Kiinteitsarakkeita11Click(TObject *Sender);
	void __fastcall Hiihtokilpailunperusmalli1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Lissarja1Click(TObject *Sender);
	void __fastcall Otalhthuomioon1Click(TObject *Sender);
	void __fastcall Lisvakantitsarjojenedelle1Click(TObject *Sender);
	void __fastcall Haematkatratatiedoista1Click(TObject *Sender);
	void __fastcall Kopioivaiheenalkunumerotperustietoihin1Click(TObject *Sender);
	void __fastcall Kopioiperustietojenalkunumerotvaiheelle1Click(TObject *Sender);
	void __fastcall Bibalku1Click(TObject *Sender);
	void __fastcall Vaiheenluonne1Click(TObject *Sender);



private:	// User declarations
	int aktrow;
	int aktcol;
	int ColIx[100];
	int ColPv[100];
	int VapNum;
	int VapAika;
public:		// User declarations
	__fastcall TFormSarjatiedot(TComponent* Owner);
	void __fastcall SetColumns(void);
	void __fastcall NaytaSarjat(void);
	int  __fastcall paivitaMuutos(int col, int row);
	void __fastcall TallennaSarjat(void);
   wchar_t Kohde[30];
   bool pakotaIsot;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormSarjatiedot *FormSarjatiedot;
//---------------------------------------------------------------------------
#endif
