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

#ifndef UnitTKantaH
#define UnitTKantaH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Menus.hpp>
#include "HkDeclare.h"

#define HT_FILECOLS 15

//---------------------------------------------------------------------------
class TFormTKanta : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *hTGrid;
	TMainMenu *MainMenu1;
	TMenuItem *iedosto1;
	TMenuItem *Luetiedot1;
	TOpenDialog *OpenDialog1;
	TMenuItem *Asetukset1;
	TMenuItem *Sukujaetunimierikseen1;
	TMenuItem *IRMAtiedosto1;
	TMenuItem *aulukonrivimr1;
	TMenuItem *Sulje1;
	TMenuItem *Nytettvtsarakkeet1;
	TMenuItem *IdLisenssino1;
	TMenuItem *Nimi1;
	TMenuItem *Seura1;
	TMenuItem *Maa1;
	TMenuItem *Sarja1;
	TMenuItem *Rintanumero1;
	TMenuItem *Emit1;
	TMenuItem *Joukkue1;
	TMenuItem *Rankipisteet1;
	TMenuItem *Maksumerkint1;
	TMenuItem *Syntymvuosi1;
	TMenuItem *Sukupuoli1;
	TMenuItem *Erotin1;
	TMenuItem *Puolipiste1;
	TMenuItem *abulaattori1;
	TMenuItem *Pilkku1;
	TMenuItem *Hiihtoliitontiedosto1;
	TMenuItem *FIStiedosto1;
	TMenuItem *Ohjelmanoletusmuoto1;
	TMenuItem *Kytoletusmuotoa1;
	TMenuItem *Luettavantiedostonmuoto1;
	TMenuItem *Kvid1;
	TMenuItem *Nytkaikkirivit1;
	TMenuItem *Help1;
	TMenuItem *Muokkaus1;
	TMenuItem *Liskilpailijattietokantaan1;
	TMenuItem *Sallimuokkaustaulukossa1;
	TMenuItem *Tallennatehdytmuutokset1;
	TMenuItem *Peruutamuutokset1;
	TSaveDialog *SaveDialog1;
	TMenuItem *Osanotot1;
	TMenuItem *Osanottolaskuri1;
	TMenuItem *Otsikkorivinmukaan1;
	TMenuItem *Nytlisvalinnat1;
	TMenuItem *Lisatieto1;
	void __fastcall Luetiedot1Click(TObject *Sender);
	void __fastcall IRMAtiedosto1Click(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall Sukujaetunimierikseen1Click(TObject *Sender);
	void __fastcall aulukonrivimr1Click(TObject *Sender);
	void __fastcall Sulje1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Puolipiste1Click(TObject *Sender);
	void __fastcall abulaattori1Click(TObject *Sender);
	void __fastcall Pilkku1Click(TObject *Sender);
	void __fastcall Kytoletusmuotoa1Click(TObject *Sender);
	void __fastcall Hiihtoliitontiedosto1Click(TObject *Sender);
	void __fastcall FIStiedosto1Click(TObject *Sender);
	void __fastcall IdLisenssino1Click(TObject *Sender);
	void __fastcall Nimi1Click(TObject *Sender);
	void __fastcall Seura1Click(TObject *Sender);
	void __fastcall Maa1Click(TObject *Sender);
	void __fastcall Syntymvuosi1Click(TObject *Sender);
	void __fastcall Sukupuoli1Click(TObject *Sender);
	void __fastcall Sarja1Click(TObject *Sender);
	void __fastcall Rintanumero1Click(TObject *Sender);
	void __fastcall Emit1Click(TObject *Sender);
	void __fastcall Joukkue1Click(TObject *Sender);
	void __fastcall Rankipisteet1Click(TObject *Sender);
	void __fastcall Maksumerkint1Click(TObject *Sender);
	void __fastcall Kvid1Click(TObject *Sender);
	void __fastcall Nytkaikkirivit1Click(TObject *Sender);
	void __fastcall Help1Click(TObject *Sender);
	void __fastcall Sallimuokkaustaulukossa1Click(TObject *Sender);
	void __fastcall Peruutamuutokset1Click(TObject *Sender);
	void __fastcall Tallennatehdytmuutokset1Click(TObject *Sender);
	void __fastcall Liskilpailijattietokantaan1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall hTGridMouseWheelDown(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall hTGridMouseWheelUp(TObject *Sender, TShiftState Shift, TPoint &MousePos,
          bool &Handled);
	void __fastcall hTGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall hTGridDblClick(TObject *Sender);
	void __fastcall Osanotot1Click(TObject *Sender);
	void __fastcall Osanottolaskuri1Click(TObject *Sender);
	void __fastcall Otsikkorivinmukaan1Click(TObject *Sender);
	void __fastcall Nytlisvalinnat1Click(TObject *Sender);
	void __fastcall Lisatieto1Click(TObject *Sender);
private:	// User declarations
	void  laadiRivi(wchar_t *iBuf, int len, int r, bool kokonimi);
	void  kirjoitaTiedostoon(int aiempaan);
	void  paivitaHenkilot(void);
public:		// User declarations
	__fastcall TFormTKanta(TComponent* Owner);
	__fastcall ~TFormTKanta();
	int __fastcall LueHenkilot(wchar_t *flName, bool NaytaDialogi);
	int __fastcall EtsiHenkilo(int lisno);
	void __fastcall Selaa(int haku);
	int __fastcall NaytaHenkilot(wchar_t *ala, wchar_t *yla, wchar_t *etuala, wchar_t *etuyla, int badge);
	int __fastcall THenktiedot(int KHenk, wchar_t *snimi, wchar_t *enimi,
		wchar_t *seura, wchar_t *maa, wchar_t *sarja, wchar_t *joukkue, int *id, int *intid,
		int *badge, int *bib, wchar_t *sukup, int *ika, int *synt);
	bool __fastcall HenkiloHakuOn(void);
	int TallennaMuutokset(void);

	henkTieto *Henkilot;
	int NHenk;
	int maxNHenk;
	int NEsilla;
	int ensHenk;
	int aktHenk;
	int aktRow;
	int hTmaxRow;
	int ValBadge;
	int ColContent[80];
	ColTp FileCols[HT_FILECOLS];
	int nFileCols;
	TComponent *Caller;
	int *rowHenk;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormTKanta *FormTKanta;
//---------------------------------------------------------------------------
#endif
