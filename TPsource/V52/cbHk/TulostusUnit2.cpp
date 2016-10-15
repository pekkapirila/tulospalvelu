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

#include <vcl.h>
#pragma hdrstop

#include <iostream>
#include <fstream.h>
#include "TulostusUnit2.h"
#include "MuotoiluUnit.h"
#include "HkMuotoilu.h"
#include "UnitSeuraval.h"
#include "UnitPiirival.h"
#include "UnitAutoFile.h"
#include "UnitHtmlEdit.h"
#include "UnitBrowser.h"
#include "UnitLisaTekstit.h"
#include "WinHk.h"
#include "UnitSHLTul.h"
#include "UnitSivuTuloste.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define MAXBUFFERSIZE 500
wchar_t FISMaarFileName[200];
static wchar_t tulostettava[20] = L"TOVYEFP";
static wchar_t OoTulostettava[] = L"KNABCLEFR";
static wchar_t jarj[VALUKU+2] = L"TYANBMLP";
static wchar_t vaiheet[VALUKU+2] = L"TYZUK";
static wchar_t rajaus[] = L"ILTPHKAE-GDSJX", seuralaji[] = L" SLYJTMX";
static wchar_t tiedtyyppi[] = L"RIEHMXOSFYBA";


//TTulosteForm *TulosteForm;
//---------------------------------------------------------------------------
__fastcall TTulosteForm::TTulosteForm(TComponent* Owner)
   : TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
//void __fastcall TTulosteForm::KirjoitinValEnter(TObject *Sender)
void __fastcall TTulosteForm::TabSheetKirjoitinEnter(TObject *Sender)
{
	enumPrinters(KirjoitinVal);
}

void TTulosteForm::enumPrinters(TComboBox *CBkirj)
{
   PRINTER_INFO_1 PrinterEnum[400];
   wchar_t wPrinterName[120];
   DWORD cbNeeded, cReturned, cLisa;
   TCHAR cBuffer[MAXBUFFERSIZE];
   wchar_t wcBuffer[MAXBUFFERSIZE];
   DWORD bufSz = 120, err;
   HANDLE     hPrinter;
//   OSVERSIONINFO osvi;
   PRINTER_INFO_1 PrtInfo;
   int oletuskirjoitin = 0;

   if (!EnumPrinters(
	  PRINTER_ENUM_LOCAL,        // printer object types
	  NULL,                      // name of printer object
	  1,                         // information level
	  (char *) PrinterEnum,      // printer information buffer
	  sizeof(PrinterEnum),       // size of printer information buffer
	  &cbNeeded,                 // bytes received or required
	  &cReturned                 // number of printers enumerated
	  )) {
	  wchar_t msg[80];
	  DWORD ec = GetLastError();
	  swprintf(msg, L"Kirjoitinten luettelointi epäonnistui, koodi %d", ec);
	  writeerror_w(msg, 0);
	  return;
	  }
   if (cReturned) {
	  CBkirj->Clear();
	  for (int i = 0; i < (int) cReturned; i++) {
		 CBkirj->AddItem(PrinterEnum[i].pName, 0);
		 }

	  memset(wPrinterName, 0, sizeof(wPrinterName));
//	  osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
//	  GetVersionEx(&osvi);
		 if (EnumPrinters(
			PRINTER_ENUM_NETWORK | PRINTER_ENUM_CONNECTIONS,      // printer object types
			NULL,                      // name of printer object
			1,                         // information level
			(char *) PrinterEnum,     // printer information buffer
			sizeof(PrinterEnum),       // size of printer information buffer
			&cbNeeded,                 // bytes received or required
			&cLisa                     // number of printers enumerated
			) && cLisa) {
			for (int i = 0; i < (int) cLisa; i++) {
			   CBkirj->AddItem(PrinterEnum[i].pName, 0);
			   }
			cReturned += cLisa;
			}
//		 if (osvi.dwMajorVersion > 4) {
		 // Windows 2000, XP
			if (!GetDefaultPrinterW(wPrinterName, &bufSz)) {
			   err = GetLastError();
			   if (err == ERROR_INSUFFICIENT_BUFFER)
				  writeerror_w(L"Oletuskirjoittimen nimi liian pitkä (yli 119 merkkiä)", 0, true);
			   }
			else
			   oletuskirjoitin = 1;
/*
			}
		 else {
		 // Windows NT4
		 // Retrieve the default string from Win.ini (the registry).
		 // String will be in form L"printername,drivername,portname".
			if (!GetProfileStringW(L"windows", L"device", L",,,", wcBuffer, MAXBUFFERSIZE) <= 0) {
			   // Printer name precedes first L"," character...
			   wcstok(wcBuffer, L",");

			   // If given buffer too small, set required size and fail...
			   if ((DWORD)wcslen(wcBuffer) >= 120) {
				  writeerror_w(L"Oletuskirjoittimen nimi liian pitkä (yli 119 merkkiä)", 0, true);
				  }
			   else {
				  wcscpy(wPrinterName, wcBuffer);
				  oletuskirjoitin = 1;
				  }
			   }
			}
*/
	  if (oletuskirjoitin) {
		 for (int i = 0; i < (int) cReturned; i++) {
			if (!wcscmpU(wPrinterName, CBkirj->Items->Strings[i].c_str())) {
			   CBkirj->ItemIndex = i;
			   break;
			   }
			}
		 }
	  }

}
//---------------------------------------------------------------------------
void __fastcall TTulosteForm::ButtonTiedValClick(TObject *Sender)
{
	SaveDialog1->InitialDir = FormMain->CurrentDir;
	if (SaveDialog1->FileName.Length() == 0 || Tyyppivaihdettu) {
		SaveDialog1->FileName = UnicodeString(PageControlSisalto->ActivePageIndex == 0 ? L"Luett." : L"Tulokset.")+TiedDefault;
		}
	Tyyppivaihdettu = false;
	SaveDialog1->DefaultExt = TiedDefault;
	if (!wcscmp(TiedDefault, L"txt"))
		SaveDialog1->Filter = L"Tekstitiedostot (*.txt)|*.TXT|Kaikki tiedostot (*.*)|*.*";
	if (!wcscmp(TiedDefault, L"csv"))
		SaveDialog1->Filter = L"Erotteelliset tiedostot (*.csv, *.txt)|*.CSV;*.TXT|Kaikki tiedostot (*.*)|*.*";
	if (!wcscmp(TiedDefault, L"html"))
		SaveDialog1->Filter = L"Html-tiedostot (*html, *.htm)|*.HTML;*.HTM|Kaikki tiedostot (*.*)|*.*";
	if (!wcscmp(TiedDefault, L"xml"))
		SaveDialog1->Filter = L"XML-tiedostot (*.xml)|*.XML|Kaikki tiedostot (*.*)|*.*";
   if (SaveDialog1->Execute()) {
	  TiedNimiEdit->Text = SaveDialog1->FileName;
	  }
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------
void __fastcall TTulosteForm::PoistuBtnClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::FormCreate(TObject *Sender)
{
	CBTiedLaji->ItemIndex = 3;
	TabSheetKirjoitinEnter(Sender);
	PageControlSisalto->ActivePageIndex = 0;
	PageControlKohde->ActivePageIndex = 1;
	CBLlPaivat->Clear();
	CBLlJarjPv->Clear();
	CBLlPaivat->Items->Add(L"Kaikki vaiheet");
	CBLlJarjPv->Items->Add(L"Perustiedot");
	for (int i = 0;i < kilpparam.n_pv_akt; i++) {
	   CBLlPaivat->Items->Add(UnicodeString(i+1)+L". vaihe");
	   CBLlJarjPv->Items->Add(UnicodeString(i+1)+L". vaihe");
		}
	if (kilpparam.n_pv_akt == 1) {
		CBLlPaivat->ItemIndex = 1;
		CBLlJarjPv->ItemIndex = 1;
		GBLlVaihe->Enabled = false;
		GBLlVaihe->Visible = false;
		}
	else {
		CBLlPaivat->ItemIndex = 0;
		CBLlJarjPv->ItemIndex = (ToimintaTila == 1 ? 0 : k_pv+1);
		GBLlVaihe->Enabled = true;
		GBLlVaihe->Visible = true;
		}
	if (emitfl < 1) {
		CBSisalto->Clear();
		CBSisalto->Items->Add(L"Tulokset");
		CBSisalto->Items->Add(L"Osanottajat ilman tulostietoa");
		CBSisalto->Items->Add(L"Väliajat");
		CBSisalto->Items->Add(L"Tulokset ja väliaikoja");
		CBSisalto->Items->Add(L"Pisteet (tuloksista lasketut)");
		wcscpy(tulostettava, L"TOVYP");
		CBSisalto->ItemIndex = 0;
		}
	RGOoSra->ItemIndex = (kilpparam.seuralyh_on ? 1 : 0);
	if (ToimintaTila != 2) {
		TabSheetTulokset->Enabled = false;
		GroupBox1->Visible = false;
		LblEste->Visible = true;
		}
	if (ToimintaTila == 2) {
		PageControlSisalto->ActivePageIndex = 1;
		TuloksetJaHtml->Visible = (kuntosuunnmoodi == 1 || kuntosuunnmoodi == 2);
		CBViimLeima->Checked = maalileimasin >= 0;
		CBSisaltoChange(Sender);
		CBSarjayhd->Visible = nsarjayhd > 0;
		}
	if (kilpparam.kilplaji == L'S')
		CBVapaarivit->Checked = true;
	if (EnvData.IEEmul < 9000) {
        CBNaytaheti->Checked = false;
		}
	PageControlKohdeChange(Sender);
}
//---------------------------------------------------------------------------
void lahtoluettelo(wchar_t kohde, wchar_t tiedlaji, wchar_t luetlaji, int paiva, int jarjpv,
	int njono, wchar_t emitrajaus, wchar_t sranimi, wchar_t *listflnm, wchar_t merkisto,
	wchar_t erotin, short *sarjat, wchar_t (*seuralista)[LSEURA+5], wchar_t *LisaRivit, UINT32 Options,
	int pistejakaja);

void yhtveto(wchar_t kohde, wchar_t tiedlaji, wchar_t yvlaji, wchar_t *listflnm, wchar_t merkit,
	wchar_t erotin);

/* ---------- Optioiden käyttö

0x00000001	LL Vapaarivit tai vakantit
0x00000004	LL Lainakortit
0x00000008	LL Vaadilahtijä
0x00000010	LL Lähtökello lajiin A
0x00000060 	LL VaadiTark
0x00400000	VaadiKvId
0x00200000	XML oma
0x08000000	LL Maa seuralistan osana
0x10000000	LangUK
0x80000000	Uusisivu

0x00000001	TL SarjatErikseen
0x00000004	TL pisteet
0x00000018  TL osasijoitusten näyttö
0x00000038	TL Sijoitukset
0x000000
0x00001000	TL Piirisijat tai EiAikaa
0x0002E000	TL kärjestä
0x00100000	LehdErot
0x00200000	LehdEtuEnsin
0x00040000	LehdPitkat
0x00080000	LehdPilkut
0x00400000	VaadiKvId
0x00100000	EmitVA: ViimeLeima
0x00800000	CSV sarjaotsikk omalle riville
0x01000000	TL Alisarjat
0x00200000	XML oma
0x00200000	kierrostulos
0x08000000	Maa seuralistan osana
0x10000000	LangUK
0x20000000	SHLohita
0x40000000	SHL Yhdistäsarjat
0x80000000	Uusisivu

0x00000003	Pistedesimaalit
0x01000000  VA keskeyttäneille  Options2

Joukkuetul (P)
0x000001C0	koko
0x00000200	voittolisa
0x00000400	pisteMax
0x00000010 * koko  Options2
0x00001000 * minkoko  Options2

Joukkuetul (J)
0x0000000C	laji
0x00000010	vaadiJk
0x00000020	kirj seura
0x000000C0	tunnus
0x00000010 * koko  Options2
0x00001000 * minkoko  Options2

-------------- Optioien käyttö */

void __fastcall TTulosteForm::ButtonTulostaClick(TObject *Sender)
{
	int alaraja = 0, ylaraja = 999999, KohdeIndex, SisaltoIndex, err = 0;
	int RiviLuku = 0, kopioita = 1;
	UINT32 Options = 0, Options2 = 0;
#if MAXSARJALUKU+MAXYHD > 255
	INT16 sarjat[MAXSARJALUKU+MAXYHD+1];
#else
	INT16 sarjat[256];
#endif
	wchar_t kohde[] = L"PI";
	wchar_t kohdenimi[200] = L"", merkisto[] = L"U8AOW", Tulostettava;
	wchar_t erottimet[] = L";\t,", **Seura = 0, *SeuraLst[2] = {L"", L""}, St[80];
	wchar_t YvLaji[] = L"OTSE", pisteval;
	wchar_t emitrajaus[] = L"KLO";
	wchar_t PisteLista[VALUKU+3] = L"";
	wchar_t (*SeuraLista)[LSEURA+5] = NULL;
	wchar_t SpRajaus[20] = L"";
	wchar_t *LisaRivit = NULL;
	wchar_t tiedTyyppi;
	INT16 IkaSrj[40];

	memset(IkaSrj, 0, sizeof(IkaSrj));
	KohdeIndex = PageControlKohde->ActivePageIndex;
	SisaltoIndex = PageControlSisalto->ActivePageIndex;
	if (CBUusiSivu->Checked)
		Options |= 0x80000000;
	if (Englanninkielisetotsikot1->Checked)
		Options |= 0x10000000;
	switch (KohdeIndex) {
		case 0:
			if (KirjoitinVal->ItemIndex == -1 ||
				KirjoitinVal->Items->Strings[KirjoitinVal->ItemIndex].Length() == 0) {
				Application->MessageBoxW(L"Kirjoitinta ei valittu", L"Valitse kirjoitin", MB_OK);
				return;
				}
			wcsncpy(kohdenimi,
				KirjoitinVal->Items->Strings[KirjoitinVal->ItemIndex].c_str(), 199);
			break;
		case 1:
			if (TiedNimiEdit->Text.Length() == 0) {
				Application->MessageBoxW(L"Tiedostoa ei valittu", L"Valitse tiedosto", MB_OK);
				return;
				}
			wcsncpy(kohdenimi, TiedNimiEdit->Text.c_str(), 199);
			break;
		}
	memset(sarjat, 0, sizeof(sarjat));
	for (int srj = 0; srj < SrjLkm; srj++) {
		sarjat[srj] = SarjaValittu(srj);
		}
	tiedTyyppi = tiedtyyppi[CBTiedLaji->ItemIndex];

	if (KohdeIndex == 1 && tiedTyyppi == L'X') {
		tiedTyyppi = L"VVXW"[CBxmlStand->ItemIndex+1];
		if (CBvaadiKvId->Checked)
			Options |= 0x00400000;
		}

	if (KohdeIndex == 1 && tiedTyyppi == L'Y') {
		Options |= 0x00400000;
		}

	if (TulYV->Checked) {
		yhtveto(kohde[KohdeIndex],
			tiedTyyppi,
			YvLaji[CBYv->ItemIndex],
			kohdenimi,
			merkisto[CBMerkisto->ItemIndex],
			erottimet[CBErotin->ItemIndex]);
		}
	else {

		if ((SisaltoIndex == 1 && rajaus[CBRajVal->ItemIndex] == L'S') ||
			(SisaltoIndex == 0 && RGOoSeurat->ItemIndex == 1 && OoTulostettava[CBOoSisalto->ItemIndex])) {
			int ns, is = 0;
			if (FormSeuraVal && FormSeuraVal->LBSeuraVal &&
				(ns = FormSeuraVal->LBSeuraVal->Items->Count) > 0) {
				SeuraLista = new wchar_t[ns+1][LSEURA+5];
				if (!SeuraLista)
					return;
				memset(SeuraLista, 0, (ns+1)*sizeof(SeuraLista[0]));
				for (int i = 0; i < FormSeuraVal->nsra; i++) {
					if (FormSeuraVal->SeuraValinnat[i]) {
						if (FormSeuraVal->CBMaa->Checked) {
							wmemcpy(SeuraLista[is], FormSeuraVal->seuralista[i], 3);
							wcsncpy(SeuraLista[is]+3, FormSeuraVal->seuralista[i]+4, LSEURA);
							}
						else
							wcsncpy(SeuraLista[is], FormSeuraVal->seuralista[i], LSEURA);
						if (++is == ns)
							break;
						}
					}
				}
			else {
				Application->MessageBoxW(L"Seuroja ei ole valittu tai valintaikkuna on suljettu",
					L"Este", MB_OK);
				return;
				}
			}
		if (CBLisaTeksti->Checked && kohde[KohdeIndex] == L'I' && tiedTyyppi == L'H') {
			LisaRivit = LisaFile;
			}

		while (SisaltoIndex == 0) {
			int Paivat, jonot = 1;

			Paivat = CBLlPaivat->ItemIndex;
			if (KohdeIndex == 1 && wcswcind(tiedTyyppi, L"RIHBA") < 0) {
				Application->MessageBoxW(L"Lähtöluettelon sallitut tiedostotyypit ovat"
					L" 'kentät erotettuina', 'kentät kohdistettuina', HTML, 'WebScorer'"
					L" ja 'Takaa-ajon ajat kellolle'", L"Rajoitus", MB_OK);
				break;
				}
			if (wcswcind(OoTulostettava[CBOoSisalto->ItemIndex], L"LR") >= 0) {
				if (Paivat < 1) {
					Application->MessageBoxW(L"Valitse yksi vaihe", L"Virhe", MB_OK);
					err = 1;
					break;
					}
				CBLlJarjPv->ItemIndex = Paivat;
				}
			if (OoTulostettava[CBOoSisalto->ItemIndex] == L'L') {
				if (CBVapaarivit->Checked)
					Options |= 1;
				Options |= 32*CBVaadiTark->ItemIndex;
				memset(sarjat, 0, sizeof(sarjat));
				if (RGLahdot->ItemIndex == 0) {
					for (int i = 0; i < 256; i++) {
						sarjat[i] = 1;
						}
					}
				else {
					wchar_t ln[100], *p;
					wcsncpy(ln, EditLahdot->Text.c_str(), 99);
					ln[99] = 0;
					p = wcstok(ln, L" ';/");
					while (p) {
						int i;
						if ((i = _wtoi(p)) > 0) {
							if (i < 256)
								sarjat[i] = 1;
							else {
								Application->MessageBoxW(L"Lähtönumeron on oltava välillä 1 - 255", L"Rajoitus", MB_OK);
								err = 1;
								break;
								}
							}
						p = wcstok(NULL, L" ';/");
						}
					}
				}
/*
			else if (ToimintaTila == 2 && kilpparam.n_pv_akt > 1 && CBLlJarjPv->ItemIndex == 0) {
				if (Application->MessageBoxW(L"Tulospalvelutilassa voi jossain vaiheessa poissaolevaksi"
					L" merkittyjä jäädä pois kaikkien vaiheiden tulosteesta. Laaditaanko tuloste silti?",
					L"Varoitus", MB_YESNO) != IDYES)
					return;
				}
*/
			if (wcswcind(OoTulostettava[CBOoSisalto->ItemIndex], L"NBC") >= 0 && CBTulVak->Checked)
				Options |= 1;
			if (CBLainat->Checked)
				Options |= 4;
			if (CBVaadiLahtija->Checked)
					Options |= 8;
			if (tiedTyyppi == L'A' && RGlahtokello->ItemIndex == 1)
				Options |= 16;
			if (SeuraLista && FormSeuraVal && FormSeuraVal->CBMaa->Checked)
				Options |= 0x08000000;
			if (CBTakAjo->Visible && CBTakAjo->Checked)
				Options |= 0x01000000;
			jonot = _wtoi(EdtJonoja->Text.c_str());
			if (jonot < 1)
				jonot = 1;
			if (err)
				break;
			if (KohdeIndex == 0) {
				kopioita = _wtoi(EdtKopioita->Text.c_str());
				EdtKopioita->Text = 1;
				if (kopioita > 5) {
					if (Application->MessageBoxW((UnicodeString(L"Vahvista ")+UnicodeString(kopioita)+L" kopion tulostus").c_str(),
						L"Vahvista", MB_YESNO) != IDYES)
						return;
					}
				}
			for (int ik = 0; ik < kopioita; ik++) {
				lahtoluettelo(kohde[KohdeIndex],
					tiedTyyppi,
					OoTulostettava[CBOoSisalto->ItemIndex],
					Paivat,
					CBLlJarjPv->ItemIndex-1,
					jonot, emitrajaus[CBEmitRajaus->ItemIndex],
					seuralaji[RGOoSra->ItemIndex+1],
					kohdenimi,
					merkisto[CBMerkisto->ItemIndex],
					erottimet[CBErotin->ItemIndex],
					sarjat,
					SeuraLista,
					LisaRivit,
					Options,
					_wtoi(EdtPisteJak->Text.c_str()));
					Application->MessageBoxW(L"Pyydetty tuloste laadittu", L"Valmis", MB_OK);
				}
			break;
			}

		while (SisaltoIndex == 1) {
			Tulostettava = tulostettava[CBSisalto->ItemIndex];
			pisteval = vaiheet[PisteVal->ItemIndex];
			if (rajaus[CBRajVal->ItemIndex] == L'P') {
				alaraja = Alaraja->Text.ToInt();
				ylaraja = Ylaraja->Text.ToInt();
				}
			if (RGSarjatErikseen->ItemIndex == 1)
				Options |= 1;
			if (RGPisteet->ItemIndex == 1)
				Options |= 6;
			if (CBSijoitukset->ItemIndex > 0)
				Options |= (8*CBSijoitukset->ItemIndex);
			if (rajaus[CBRajVal->ItemIndex] == L'D') {
				if (!FormPiiriVal)
					return;
				if (FormPiiriVal->RGPiirisijat->ItemIndex == 1)
					Options |= 0x1000;
				Options |= 0x2000*_wtoi(FormPiiriVal->EdtParhaat->Text.c_str());
				}
			if (rajaus[CBRajVal->ItemIndex] == L'S') {
				if (!FormSeuraVal)
					return;
				Options |= 0x2000*_wtoi(FormSeuraVal->EdtParhaat->Text.c_str());
				}
			if (rajaus[CBRajVal->ItemIndex] == L'L') {
				if (CBEiAikaa->Checked)
					Options |= 0x1000;
				}
			if (tulostettava[CBSisalto->ItemIndex] == L'Y') {
				wchar_t *p, buf[500];
				buf[499] = 0;
				if (kohde[KohdeIndex] == L'P') {
					Application->MessageBoxW(L"Tulostettavan tiedon vaihtoehto ei käytettävissä kirjoitintulosteissa",
						L"Rajoitus", MB_OK);
						break;
					}
				wcsncpy(buf, EdtPistelista->Text.c_str(), sizeof(buf)/2-1);
				p = wcstok(buf, L" ,;");
				for (int i = 0; i < VALUKU+2 && p != NULL;) {
					if (towupper(*p) == L'L')
						PisteLista[i] = 1;
					else if (towupper(*p) == L'M')
						PisteLista[i] = 2;
					else if (towupper(*p) == L'Y')
						PisteLista[i] = kilpparam.valuku + 3;
					else
						PisteLista[i] = _wtoi(p)+2;
					if (PisteLista[i])
						i++;
					p = wcstok(NULL, L" ,");
					}
				if (PisteLista[0] == 0)
					break;
				}
			if (tulostettava[CBSisalto->ItemIndex] == L'F' &&
				(k_pv > 0 || kohde[KohdeIndex] != L'I' || (tiedTyyppi != L'H' && tiedTyyppi != L'M'))) {
				Application->MessageBoxW(L"Pyydetty tulosteyhdistelmä ei käytettävissä tähän tulosteeseen",
					L"Rajoitus", MB_OK);
				break;
				}
			if (tulostettava[CBSisalto->ItemIndex] == L'E' || tulostettava[CBSisalto->ItemIndex] == L'F') {
				if (pisteval == L'S' &&
					(kohde[KohdeIndex] != L'I' || wcswcind(tiedTyyppi, L"IXYVW") < 0)) {
					Application->MessageBoxW(L"Emit-tietojen siirtotiedosto on kohdistettu tekstitiedosto tai XML-tiedosto",
						L"Rajoitus", MB_OK);
					break;
					}
				if (kohde[KohdeIndex] != L'I' || wcswcind(tiedTyyppi, L"IHMXYVW") < 0) {
					Application->MessageBoxW(L"Emit-tulosteen tiedostomuodon on oltava html, XML tai kohdistettu tekstitiedosto",
						L"Rajoitus", MB_OK);
					break;
					}
				}
			if (rajaus[CBRajVal->ItemIndex] == L'J') {
				if (kohde[KohdeIndex] == L'I' && tiedTyyppi != L'H' && tiedTyyppi != L'M') {
					Application->MessageBoxW(L"Joukkuetulokset voi tulostaa vain kirjoittimelle ja html-tiedostoon",
						L"Rajoitus", MB_OK);
					break;
					}
				if (RGJkTls->ItemIndex == 3) {
					Tulostettava = L'P';
					Options = 0x40 * CBJkTunnus->ItemIndex + (CBVoittLisa->Checked ? 0x200 : 0) +
						(CBpisteMax->Checked ? 0x400 : 0);
					Options2 |= 16*_wtoi(EdtJkKoko->Text.c_str()) + 4096*_wtoi(EdtJkMin->Text.c_str());
					}
				else {
					Tulostettava = L'J';
					if (CBJkTunnus->ItemIndex > 3) {
						Application->MessageBoxW(L"Piiriä ei voi valita tunnukseksi näille pisteille",
							L"Rajoitus", MB_OK);
						break;
						}
					Options = 4 * RGJkTls->ItemIndex + (CBVaadiJoukkue->Checked ? 0x10 : 0) +
						(CBSeuraJoukkueena->Checked ? 0x20 : 0) +
						0x40 * CBJkTunnus->ItemIndex;
					if (RGJkTls->ItemIndex == 1 && CBJkPstJrj->ItemIndex == 1)
						Options |= 2;
					Options2 |= 16*_wtoi(EdtJkKoko->Text.c_str()) + 4096*_wtoi(EdtJkMin->Text.c_str());
					}
				}
			if (CBAlisarjat->Checked && (EdtSukup->Text.Length() > 0 || EdtIkasarjat->Text.Length() > 0)) {
				wchar_t *p;
				int is = 0;
				Options |= 0x01000000;
				for (int ip = 0; is < 19 && ip < EdtSukup->Text.Length(); ip++) {
					wchar_t csp = towupper(EdtSukup->Text.c_str()[ip]);
					if (csp > L' ')
						SpRajaus[is++] = csp;
					}
				memset(St, 0, sizeof(St));
				wcsncpy(St, EdtIkasarjat->Text.UpperCase().c_str(), sizeof(St)/2-1);
				p = wcstok(St, L" ,;/");
				is = 0;
				while (p && is < 40) {
					if (_wtoi(p) > 0)
						IkaSrj[is++] = _wtoi(p);
					else {
						switch(*p) {
							case L'Y':
								IkaSrj[is++] = 1;
								break;
							case L'A':
								IkaSrj[is++] = 2;
								break;
							}
						}
					p = wcstok(NULL, L" ,;/");
					}
				}
			if (kohde[KohdeIndex] == L'I' && tiedTyyppi == L'S') {
				FormSHLtul = new TFormSHLtul(this);
				FormSHLtul->ShowModal();
				if (FormSHLtul->ModalResult == mrCancel) {
					delete FormSHLtul;
					return;
					}
				delete FormSHLtul;
				}
			if (kohde[KohdeIndex] == L'I' && tiedTyyppi == L'E') {
				if (CBLehdErot->Checked)
					Options |= 0x100000;
				if (CBLehdEtuensin->Checked)
					Options |= 0x200000;
				if (CBLehdPitkat->Checked)
					Options |= 0x40000;
				if (CBLehdPilkut->Checked)
					Options |= 0x80000;
				}
			else if (CBViimLeima->Checked)
				Options |= 0x00100000;
			if (kohde[KohdeIndex] == L'I' && wcswcind(tiedTyyppi, L"R") >= 0
				&& CBOtsikkorivi->Checked)
				Options |= 0x00800000;
			if (wcswcind(tulostettava[CBSisalto->ItemIndex], L"VY") >= 0 &&	CBVAKesk->Checked)
				Options2 |= 0x01000000;
			if (SeuraLista && FormSeuraVal && FormSeuraVal->CBMaa->Checked)
				Options |= 0x08000000;
			if (CBSHLohita->Checked)
				Options |= 0x20000000;
			if (CBSHLyhdsrj->Checked)
				Options |= 0x40000000;
			if (CBkierr->Checked)
				Options |= 0x00200000;
			if (KohdeIndex == 0) {
				kopioita = _wtoi(EdtKopioita->Text.c_str());
				EdtKopioita->Text = 1;
				if (kopioita > 5) {
					if (Application->MessageBoxW((UnicodeString(L"Vahvista ")+UnicodeString(kopioita)+L" kopion tulostus").c_str(),
						L"Vahvista", MB_YESNO) != IDYES)
						return;
					}
				}
			wcsncpy(lsttulparam.muottied, EdXslFilename->Text.c_str(), sizeof(lsttulparam.muottied)/2-1);

			for (int ik = 0; ik < kopioita; ik++) {
				RiviLuku =
				list(kohde[KohdeIndex],
					tiedTyyppi,
					Tulostettava,
					jarj[CBJarj->ItemIndex],
					pisteval,
					rajaus[CBRajVal->ItemIndex],
					alaraja, ylaraja, sarjat,
					kohdenimi,
					seuralaji[CBSeura->ItemIndex],
					merkisto[CBMerkisto->ItemIndex],
					erottimet[CBErotin->ItemIndex],
					PisteLista,
					SeuraLista,
					SpRajaus,
					IkaSrj,
					LisaRivit,
					Options,
					Options2);
				}
			if (RiviLuku)
				Application->MessageBoxW((UnicodeString(L"Pyydetty tuloste laadittu. ")+
				UnicodeString(RiviLuku)+ " riviä kirjoitettu").c_str(), L"Valmis", MB_OK);
			else
				Application->MessageBoxW(L"Pyydetty tuloste laadittu", L"Valmis", MB_OK);
			break;
			}
		if (SeuraLista)
			delete[] SeuraLista;
		}
	if (CBNaytaheti->Checked && kohde[KohdeIndex] == L'I' &&
		wcswcind(tiedTyyppi, L"RIEHMOSBA") >= 0) {
		UnicodeString HtmlFl = L"file:///";
		if (FormBrowser == NULL)
			FormBrowser = new TFormBrowser(this);
		if (kohdenimi[1] != L':')
			HtmlFl += FormMain->CurrentDir + L"\\";
		HtmlFl += kohdenimi;
		FormBrowser->WebBrowser1->Navigate(HtmlFl.c_str());
		FormBrowser->Show();
		if (FormBrowser->WindowState == wsMinimized)
			FormBrowser->WindowState = wsNormal;
		FormBrowser->BringToFront();
		}
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Luetaimuokkaa1Click(TObject *Sender)
{
	if (!FormMuotoilu)
		FormMuotoilu = new TFormMuotoilu(TulosteForm);
	FormMuotoilu->Show();
	if (FormMuotoilu->WindowState == wsMinimized)
		FormMuotoilu->WindowState = wsNormal;
	FormMuotoilu->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Sulje1Click(TObject *Sender)
{
   PoistuBtnClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::CBRajValChange(TObject *Sender)
{
	PaivitaTulossivu(Sender);
	CBSisaltoChange(Sender);
	if (rajaus[CBRajVal->ItemIndex] == L'S') {
		if (!FormSeuraVal)
			FormSeuraVal = new TFormSeuraVal(this);
		FormSeuraVal->Show();
		if (FormSeuraVal->WindowState == wsMinimized)
			FormSeuraVal->WindowState = wsNormal;
		FormSeuraVal->BringToFront();
		}
	if (rajaus[CBRajVal->ItemIndex] == L'D') {
		if (!FormPiiriVal)
			FormPiiriVal = new TFormPiiriVal(this);
		FormPiiriVal->Show();
		if (FormPiiriVal->WindowState == wsMinimized)
			FormPiiriVal->WindowState = wsNormal;
		FormPiiriVal->BringToFront();
		}
	LblSijat->Visible = rajaus[CBRajVal->ItemIndex] == L'P';
	LblSjVali->Visible = LblSijat->Visible;
	Alaraja->Visible = LblSijat->Visible;
	Ylaraja->Visible = LblSijat->Visible;
	CBEiAikaa->Visible = rajaus[CBRajVal->ItemIndex] == L'L';
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::BtnLisValitutClick(TObject *Sender)
{
	for (int rv = 0; rv < LBSarjat->Items->Count; rv++) {
		if (LBSarjat->Selected[rv]) {
			int srj;
			if ((srj = haesarja_w(LBSarjat->Items->Strings[rv].c_str(), true)) >= 0)
				SarjaValinnat[srj] = true;
			}
		}
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::BtnLisaaLhtSarjatClick(TObject *Sender)
{
	static int i_pv = 0, lno = 0;
	wchar_t ch;

	if (kilpparam.n_pv_akt > 1) {
		i_pv++;
		inputint_prompt(&i_pv, L"Anna vaiheen järjestysnumero", &ch, this);
		if (ch == ESC || i_pv < 1 || i_pv > kilpparam.n_pv_akt)
			return;
		i_pv--;
		}
	inputint_prompt(&lno, L"Anna lähdön numero", &ch, this);
	if (ch == ESC)
		return;
	for (int srj = 0; srj < sarjaluku; srj++) {
		if (!SarjaValinnat[srj] && Sarjat[srj].lno[i_pv] == lno)
			SarjaValinnat[srj] = true;
		}
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::BtnPoisValitutClick(TObject *Sender)
{
	for (int rv = 0; rv < LBSarjaVal->Items->Count; rv++) {
		if (LBSarjaVal->Selected[rv]) {
			int srj;
			if ((srj = haesarja_w(LBSarjaVal->Items->Strings[rv].c_str(), true)) >= 0)
				SarjaValinnat[srj] = false;
			}
		}
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::BtnLisKaikkiClick(TObject *Sender)
{
	for (int srj = 0; srj < SrjLkm; srj++)
		SarjaValinnat[srj] = true;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::BtnPoisKaikkiClick(TObject *Sender)
{
	for (int srj = 0; srj < SrjLkm; srj++)
		SarjaValinnat[srj] = false;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::FormShow(TObject *Sender)
{
	Automaattinentulostus1->Checked = autofl;
	CBkierr->Visible = kilpparam.alalaji == L'D';
	EdXslFilename->Text = lsttulparam.muottied;
	NaytaSarjavalinnat();
	PaivitaTulossivu(Sender);
	CBSisaltoChange(Sender);
}
//---------------------------------------------------------------------------
bool TTulosteForm::SarjaValittu(int Srj)
{
	for (int rv = 0; rv < LBSarjaVal->Items->Count; rv++) {
		if (haesarja_w(LBSarjaVal->Items->Strings[rv].c_str(), true) == Srj)
			return(true);
		}
	return(false);
}
//---------------------------------------------------------------------------
void TTulosteForm::NaytaSarjavalinnat(void)
{
	SrjLkm = (PageControlSisalto->ActivePageIndex == 1 && CBSarjayhd->Checked)
		? sarjaluku + nsarjayhd : sarjaluku;

	LBSarjat->Clear();
	LBSarjaVal->Clear();
/*
	if (PageControlSisalto->ActivePageIndex == 1 && RGSrjYhd->ItemIndex == 1) {
		for (int yhd = 0; yhd < nsarjayhd; yhd++) {
			if (SarjaValinnat[yhd])
				LBSarjaVal->Items->Add(yhdnimet[yhd]);
			else
				LBSarjat->Items->Add(yhdnimet[yhd]);
			}
		}
	else
*/
		{
		for (int srj = 0; srj < SrjLkm; srj++) {
			if (SarjaValinnat[srj])
				LBSarjaVal->Items->Add(UnicodeString(Sarjat[srj].sarjanimi));
			else
				LBSarjat->Items->Add(UnicodeString(Sarjat[srj].sarjanimi));
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::CBSisaltoChange(TObject *Sender)
{
	static int edSisalto = -1, edIx, edIx2;

	PisteVal->Visible = true;
	LblPistevalinta->Visible = true;
	EdtPistelista->Visible = false;
//	CBKaikkiSijoitukset->Caption = L"Tulosta kaikki sijoitukset";
	CBSijoitukset->Visible = false;
	CBJarj->Enabled = true;
	CBViimLeima->Visible = false;
	CBVAKesk->Visible = false;
	edIx = PisteVal->ItemIndex;
	edIx2 = CBJarj->ItemIndex;
	switch (tulostettava[CBSisalto->ItemIndex]) {
		case L'T':
		case L'O':
			CBJarj->Clear();
			if (k_pv == 0) {
				if (kilpparam.kilplaji == L'H' && kilpparam.alalaji == L'S') {
					LblPistevalinta->Caption = L"Tulostettavat vaiheet";
					PisteVal->Clear();
					PisteVal->Items->Add(L"Tämän vaiheen tulos");
					PisteVal->Items->Add(L"Yhteissijat");
					PisteVal->Items->Add(L"Tulosyhteenveto");
					wcscpy(vaiheet, L"TYK");
					}
				else {
					PisteVal->Visible = false;
					LblPistevalinta->Visible = false;
					wcscpy(vaiheet, L"TYZUK");
					}
				CBJarj->Items->Add(L"Paremmuus");
//				wcscpy(tulostettava, L"TOVYEP");
				wcscpy(jarj, L"TAN");
				}
			else {
				LblPistevalinta->Caption = L"Tulostettavat vaiheet";
				PisteVal->Clear();
				PisteVal->Items->Add(L"Tämän vaiheen tulos");
				PisteVal->Items->Add(L"Yhteistulos");
				PisteVal->Items->Add(L"Yhteistulos ja vaiheen tulos");
				PisteVal->Items->Add(L"Vaiheen tulos ja yhteistulos");
				PisteVal->Items->Add(L"Kaikki vaiheet");
				CBJarj->Items->Add(L"Paremmuus - tämä vaihe");
				CBJarj->Items->Add(L"Paremmuus - yhteistilanne");
				wcscpy(jarj, L"TYAN");
				wcscpy(vaiheet, L"TYZUK");
				}
			CBJarj->Items->Add(L"Sarjoittain aakkosjärjestyksessä");
			CBJarj->Items->Add(L"Sarjoittain numerojärjestyksessä");

			PisteVal->ItemIndex = 0;
			CBJarj->ItemIndex = 0;
			break;
		case L'V':
			CBSijoitukset->Visible = true;
			LblPistevalinta->Caption = L"Tulostettava väliaika";
			PisteVal->Clear();
			PisteVal->Items->Add(L"Kaikki väliajat");
			vaiheet[0] = L'0';
			for (int Va = 1; Va <= kilpparam.valuku; Va++) {
				PisteVal->Items->Add(UnicodeString(Va)+L". väliaika");
				vaiheet[Va] = L'0'+Va;
				}
			vaiheet[kilpparam.valuku+1] = 0;
			PisteVal->ItemIndex = 0;
			CBJarj->Clear();
			CBJarj->Items->Add(L"Paremmuus");
			CBJarj->Enabled = false;
			CBJarj->ItemIndex = 0;
			wcscpy(jarj, L"T");
			CBVAKesk->Visible = true;
			break;
		case L'Y':
			LblPistevalinta->Caption = L"Luettele tulostettavat pisteet";
			CBSijoitukset->Visible = true;
			PisteVal->Visible = false;
			EdtPistelista->Visible = true;
			PisteVal->ItemIndex = 0;
			CBJarj->Clear();
			CBJarj->Items->Add(L"Lopputulos");
			jarj[0] = L'0';
			for (int Va = 1; Va <= kilpparam.valuku; Va++) {
				CBJarj->Items->Add(UnicodeString(Va)+L". väliaika");
				jarj[Va] = L'0'+Va;
				}
			jarj[kilpparam.valuku+1] = 0;
			CBJarj->ItemIndex = 0;
			wcscpy(vaiheet, L"TYZUK");
			CBVAKesk->Visible = true;
			break;
		case L'E':
		case L'F':
			if (PageControlKohde->ActivePageIndex == 1 && tiedtyyppi[CBTiedLaji->ItemIndex] == L'X') {
				LblPistevalinta->Visible = false;
				PisteVal->Visible = false;
				}
			LblPistevalinta->Caption = L"Tulostettavat taulukot";
			PisteVal->Clear();
			PisteVal->Items->Add(L"Ajat rasteille");
			PisteVal->Items->Add(L"Rastivälien ajat");
			PisteVal->Items->Add(L"Molemmat eri taulukkoon");
			PisteVal->Items->Add(L"Molemmat samaaan taulukkoon");
			PisteVal->Items->Add(L"Siirtotiedosto");
			PisteVal->ItemIndex = 0;
			CBJarj->Clear();
			CBJarj->Items->Add(L"Paremmuus - tämä vaihe");
			CBJarj->Enabled = false;
			CBJarj->ItemIndex = 0;
			wcscpy(jarj, L"TANBMLP");
			wcscpy(vaiheet, L"KRMNS");
			CBViimLeima->Visible = true;
			break;
		case L'P':
			LblPistevalinta->Caption = L"Tulostettavat pisteet";
//			CBKaikkiSijoitukset->Visible = true;
//			CBKaikkiSijoitukset->Caption = L"Jaa pisteet sadalla";
			PisteVal->Clear();
			PisteVal->Items->Add(L"Tämän vaiheen pisteet");
			PisteVal->Items->Add(L"Kokonaispisteet");
			PisteVal->Items->Add(L"Kaikki vaiheet ja kok.pisteet");
			PisteVal->Items->Add(L"Lähtötietojen rankipisteet");
			PisteVal->ItemIndex = 0;
			CBJarj->Clear();
			CBJarj->Items->Add(L"Sarjoittain aakkosjärjestyksessä");
			CBJarj->Items->Add(L"Sarjoittain numerojärjestyksessä");
			CBJarj->Items->Add(L"Pisteet (laskevat, suurin paras)");
			CBJarj->Items->Add(L"Pisteet (nousevat, 0p: ei mukana)");
			CBJarj->Items->Add(L"Pisteet (nousevat, 0p: paras)");
			wcscpy(jarj, L"ANLPO");
			wcscpy(vaiheet, L"TYZR");
			break;
		}
	if (CBSisalto->ItemIndex == edSisalto) {
	   PisteVal->ItemIndex = edIx;
	   CBJarj->ItemIndex = edIx2;
	   }
	else {
	   PisteVal->ItemIndex = 0;
	   CBJarj->ItemIndex = 0;
	   }
	edSisalto = CBSisalto->ItemIndex;
	EdtPistelista->ShowHint = EdtPistelista->Visible;
	PaivitaTulossivu(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Button1Click(TObject *Sender)
{
	wchar_t fontit[400][30], listfname[200];
	PRFILE* lstf;

	if (KirjoitinVal->ItemIndex <= 0)
		return;
	memset(fontit, 0, sizeof(fontit));
	fontit[0][0] = 399;
	fontit[0][2] = 29;
	wcsncpy(listfname,
		KirjoitinVal->Items->Strings[KirjoitinVal->ItemIndex].c_str(), 199);
	if ((lstf = openprfile(listfname, 1, TRUE, FALSE, trlate, FALSE))
		!= NULL) {
		enumfontsGDI(lstf, L"", fontit);
		Memo1->Enabled = true;
		Memo1->Visible = true;
		Memo1->Clear();
		for (int i = 1; i <= fontit[0][1]; i++) {
			Memo1->Lines->Add(UnicodeString(fontit[i]));
			}
		closeprfile(lstf);
		}
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Help1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Tulostus");
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::CBTiedLajiChange(TObject *Sender)
{
	switch (tiedtyyppi[CBTiedLaji->ItemIndex]) {
		case L'R':								// R    csv
			wcscpy(TiedDefault, L"csv");
			break;
		case L'I':								// I    teksti
			wcscpy(TiedDefault, L"txt");
			break;
		case L'E':								// E    lehdistö
			wcscpy(TiedDefault, L"txt");
			break;
		case L'H':								// H    html
		case L'M':								// M
			wcscpy(TiedDefault, L"html");
			break;
		case L'Y':								// Y    WRE-tulokset
			CBMerkisto->ItemIndex = 0;
			CBxmlStand->ItemIndex = 1;
		case L'X':								// X    XML
			wcscpy(TiedDefault, L"xml");
			break;
		case L'O':								// O    SSL
			wcscpy(TiedDefault, L"txt");
			CBMerkisto->ItemIndex = 2;
			break;
		case L'S':								// S    Irma
			CBMerkisto->ItemIndex = 2;
			wcscpy(TiedDefault, L"csv");
			break;
		case L'F':								// F    FIS-tulokset
			CBMerkisto->ItemIndex = 0;
			wcscpy(TiedDefault, L"xml");
			break;
		case L'B':								// B    WebScorer
			CBMerkisto->ItemIndex = 1;
			wcscpy(TiedDefault, L"txt");
			break;
		case L'A':							// A   Algen lähtökellolle
			CBMerkisto->ItemIndex = 2;
			wcscpy(TiedDefault, L"csv");
			break;
		}
	CBErotin->Visible = (CBTiedLaji->ItemIndex == 0);
	CBxmlStand->Visible = wcswcind(tiedtyyppi[CBTiedLaji->ItemIndex], L"XY") >= 0;
	CBxmlStand->Enabled = wcswcind(tiedtyyppi[CBTiedLaji->ItemIndex], L"X") >= 0;
	Label8->Visible = CBErotin->Visible || CBxmlStand->Visible;
	CBvaadiKvId->Visible = wcswcind(tiedtyyppi[CBTiedLaji->ItemIndex], L"X") >= 0;
	Label8->Caption = CBErotin->Visible ? UnicodeString(L"Kenttäerotin") : UnicodeString(L"XML-standardi");
	CBMerkisto->Enabled = wcswcind(tiedtyyppi[CBTiedLaji->ItemIndex], L"RIEHMX") >= 0 &&
		CBTiedLaji->ItemIndex != 9;
	GBhtml->Visible = wcswcind(tiedtyyppi[CBTiedLaji->ItemIndex], L"IHM") >= 0;
 //	GBSHLVal->Visible = (CBTiedLaji->ItemIndex == 6);
	GBFISMaar->Visible = (tiedtyyppi[CBTiedLaji->ItemIndex] == L'F');
	GBLehdVal->Visible = (tiedtyyppi[CBTiedLaji->ItemIndex] == L'E');
	CBOtsikkorivi->Visible = wcswcind(tiedtyyppi[CBTiedLaji->ItemIndex], L"R") >= 0;
	CBNaytaheti->Visible = wcswcind(tiedtyyppi[CBTiedLaji->ItemIndex], L"RIHMOSBA") >= 0;
	LblHtmlvaroitus->Visible = wcswcind(tiedtyyppi[CBTiedLaji->ItemIndex], L"HM") >= 0
		&& CBNaytaheti->Checked && EnvData.IEEmul < 9000;
	RGlahtokello->Visible = tiedtyyppi[CBTiedLaji->ItemIndex] == L'A';
	Tyyppivaihdettu = true;
	EdXslFilename->Visible = tiedtyyppi[CBTiedLaji->ItemIndex] == L'X' && CBxmlStand->ItemIndex == 2;
	if (PageControlSisalto->ActivePageIndex == 1)
		CBSisaltoChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Button2Click(TObject *Sender)
{
	OpenDialog1->InitialDir = FormMain->CurrentDir;
	OpenDialog1->FileName = UnicodeString(L"fismaar.txt");
	OpenDialog1->DefaultExt = L"txt";
	OpenDialog1->Filter = L"Tekstitiedostot (*.txt)|*.TXT|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		EdtFISMaar->Text = OpenDialog1->FileName;
		wcsncpy(FISMaarFileName, OpenDialog1->FileName.c_str(), sizeof(FISMaarFileName)/2-1);
		}
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------


void __fastcall TTulosteForm::Kaikkilopettaneethtmltiedostoon1Click(TObject *Sender)

{
	PageControlKohde->ActivePageIndex = 1;
	PageControlSisalto->ActivePageIndex = 1;
	CBTiedLaji->ItemIndex = 3;
	CBTiedLajiChange(Sender);
	TiedNimiEdit->Text = UnicodeString(L"Tulokset_")+kilpparam.kilpkoodi+L".html";
	SetCurrentDir(FormMain->CurrentDir);
	BtnLisKaikkiClick(Sender);
	CBRajVal->ItemIndex = 1;
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Kaikkilopettaneetkirjoittimelle1Click(TObject *Sender)

{
	PageControlKohde->ActivePageIndex = 0;
	PageControlSisalto->ActivePageIndex = 1;
	BtnLisKaikkiClick(Sender);
	CBRajVal->ItemIndex = 1;
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Kaikkilhtajatsarjoittainhtmltiedostoon1Click(TObject *Sender)

{
	PageControlKohde->ActivePageIndex = 1;
	PageControlSisalto->ActivePageIndex = 0;
	CBTiedLaji->ItemIndex = 3;
	CBTiedLajiChange(Sender);
	TiedNimiEdit->Text = UnicodeString(L"Lahtoajat_")+kilpparam.kilpkoodi+L".html";
	SetCurrentDir(FormMain->CurrentDir);
	BtnLisKaikkiClick(Sender);
	CBOoSisalto->ItemIndex = wcswcind(L'B', OoTulostettava);
	CBOoSisaltoChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::CBSarjayhdClick(TObject *Sender)
{
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::PageControlKohdeChange(TObject *Sender)
{
	if (PageControlKohde->ActivePageIndex == 1) {
		CBTiedLajiChange(Sender);
		}
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::PageControlSisaltoChange(TObject *Sender)
{
	if (PageControlSisalto->ActivePageIndex == 1) {
		PaivitaTulossivu(Sender);
		CBSisaltoChange(Sender);
		}
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::PaivitaTulossivu(TObject *Sender)
{
	if (PageControlSisalto->ActivePageIndex == 1) {
		CBJkPstJrj->Visible = false;
		switch (rajaus[CBRajVal->ItemIndex]) {
			case L'J':
				GBJoukkue->Visible = true;
				RGJkTls->Visible = true;
				if (RGJkTls->ItemIndex == 1)
					CBJkPstJrj->Visible = true;
				Label15->Caption = (RGJkTls->ItemIndex != 3 ? L"Joukkueen koko" : L"Max. sijoitus");
				CBVaadiJoukkue->Visible =  RGJkTls->ItemIndex != 3;
				CBSeuraJoukkueena->Visible = CBVaadiJoukkue->Visible;
				CBVoittLisa->Visible = !CBVaadiJoukkue->Visible;
				CBpisteMax->Visible = !CBVaadiJoukkue->Visible;
				PisteVal->Visible = RGJkTls->Visible;
				EdtJkMin->Visible = !CBpisteMax->Visible;
				Label19->Visible = !CBpisteMax->Visible;
				break;
			default:
				GBJoukkue->Visible = false;
//				PisteVal->Visible = true;
			}
		Alaraja->Enabled = CBRajVal->ItemIndex == 3;
		Ylaraja->Enabled = Alaraja->Enabled;
		CBAlisarjat->Visible = !GBJoukkue->Visible;
		GroupBox2->Visible = !GBJoukkue->Visible;
		Label5->Visible = !GBJoukkue->Visible;
		CBJarj->Visible = !GBJoukkue->Visible;
		if (CBJarj->Visible && k_pv > 0 && wcswcind(jarj[CBJarj->ItemIndex], L"LPO") >= 0 && kilpparam.sarjavaihtelee) {
		   RGPisteet->Visible = true;
		   }
		else {
		   RGPisteet->Visible = false;
		   RGPisteet->ItemIndex = 0;
		   }
		}
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::RGOoSeuratClick(TObject *Sender)
{
	if (RGOoSeurat->ItemIndex == 1) {
		if (!FormSeuraVal)
			FormSeuraVal = new TFormSeuraVal(this);
		FormSeuraVal->Show();
		if (FormSeuraVal->WindowState == wsMinimized)
			FormSeuraVal->WindowState = wsNormal;
		FormSeuraVal->BringToFront();
		}
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Kirjoittimenasetukset1Click(TObject *Sender)
{
	PrinterSetupDialog1->Execute();
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::LBSarjatDblClick(TObject *Sender)
{
	int srj;

	srj = haesarja_w(LBSarjat->Items->Strings[LBSarjat->ItemIndex].c_str(), true);
	if (srj >= 0)
		SarjaValinnat[srj] = true;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------


void __fastcall TTulosteForm::LBSarjaValDblClick(TObject *Sender)
{
	int srj;

	srj = haesarja_w(LBSarjaVal->Items->Strings[LBSarjaVal->ItemIndex].c_str(), true);
	if (srj >= 0)
		SarjaValinnat[srj] = false;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Automaattinentulostus1Click(TObject *Sender)
{
	int vast, sek;
	wchar_t co, *p, maalistr[12] = L"";

	if (autofl) {
		autofl = 0;
		Automaattinentulostus1->Checked = false;
		return;
		}
	if (autotulostusparam.rajaus != 0) {
		vast = select3(3, L"Käynnistä automaattinen tulostus", L"Automaattinen tulostus",
			L"Aiempien valintojen mukaan", L"Kaavakkeen valintojen mukaan", L"Ei käynnistetä", this);
		}
	else {
		vast = select3(2, L"Käynnistä automaattinen tulostus", L"Automaattinen tulostus",
			L"Kaavakkeen valintojen mukaan", L"Ei käynnistetä", L"", this) + 1;
		}
	if (vast == 2) {
		autotulostusparam.tulmuot = tulosmuot;
		if (k_pv > 0) {
			autotulostusparam.yhttuljarj = jarj[CBJarj->ItemIndex] == L'Y';
			autotulostusparam.yhttulpaino = PisteVal->ItemIndex > 0 && PisteVal->ItemIndex < 3;
			autotulostusparam.vaihelkm = PisteVal->ItemIndex / 2 + 1;
			}
		else {
			autotulostusparam.yhttuljarj = false;
			autotulostusparam.yhttulpaino = false;
			autotulostusparam.vaihelkm = 1;
			}
		autotulostusparam.rajaus = rajaus[CBRajVal->ItemIndex];
		autotulostusparam.enssija = _wtoi(Alaraja->Text.c_str());
		autotulostusparam.viimsija = _wtoi(Ylaraja->Text.c_str());
		inputint_prompt(&kynnys, L"Anna tulostuskynnys", &co, this);
		kynnys = max(kynnys,1);
		sek = intv[0] / 18;
		inputint_prompt(&sek, L"Anna aika-askel (sek)", &co, this);
		intv[0] = 18*sek;
		p = maalistr;
		for (int i = 0; i < 9; i++) {
			if (autotulostusparam.automaali[i]) *(p++) = i + L'1';
			autotulostusparam.automaali[i] = 0;
			}
		*p = 0;
		inputstr_prompt(maalistr, 9, L"Anna tulostettavat maalinumerot yhtenä numerosarjana", &co, this);
		for (p = maalistr; *p; p++)
			if (*p > L'0' && *p <= L'9') autotulostusparam.automaali[*p - L'1'] = 1;
		}
	autofl = vast < 3;
	if (autofl && autotulostusparam.lstf == NULL) {
		autotulostusparam.printer = GDIPRINTER;
		merkisto(L'G');
		if ((autotulostusparam.lstf = openprfile(KirjoitinVal->Items->Strings[KirjoitinVal->ItemIndex].c_str(),
			1, TRUE, FALSE, trlate, FALSE))	== NULL) {
			return;
			}
		if (autotulostusparam.lstf && autotulostusparam.lstf->prno == 2)
			initgdiprinter(&autotulostusparam);
		}
	if (autotulostusparam.lstf == NULL)
		autofl = 0;
	if (autofl)
		autotulostusparam.kohde = L'P';
	Automaattinentulostus1->Checked = autofl;
}
//---------------------------------------------------------------------------


void __fastcall TTulosteForm::CBOoSisaltoChange(TObject *Sender)
{
	CBVaadiLahtija->Visible = wcswcind(OoTulostettava[CBOoSisalto->ItemIndex], L"ABC") >= 0;
	CBTulVak->Visible = wcswcind(OoTulostettava[CBOoSisalto->ItemIndex], L"NBC") >= 0;
	CBTakAjo->Visible = wcswcind(OoTulostettava[CBOoSisalto->ItemIndex], L"BC") >= 0 &&
		CBLlPaivat->ItemIndex > 0;
	GroupBox4->Visible  = OoTulostettava[CBOoSisalto->ItemIndex] == L'L';
	Label11->Visible = wcswcind(OoTulostettava[CBOoSisalto->ItemIndex], L"LR") < 0;
	CBLlJarjPv->Visible = Label11->Visible;
	GroupBox5->Visible  = wcswcind(OoTulostettava[CBOoSisalto->ItemIndex], L"EF") >= 0;
	if (OoTulostettava[CBOoSisalto->ItemIndex] == L'R') {
		int ipv = 2;
		if (k_pv > 2)
			ipv = k_pv;
		CBLlJarjPv->ItemIndex = ipv;
		CBLlPaivat->ItemIndex = ipv;
		}
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::CBLlPaivatChange(TObject *Sender)
{
	CBLlJarjPv->Enabled = CBLlPaivat->ItemIndex == 0;
//	if (CBLlPaivat->ItemIndex > 0)
	CBLlJarjPv->ItemIndex = CBLlPaivat->ItemIndex;
	CBOoSisaltoChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Automaattinentiedostotulostus1Click(TObject *Sender)

{
	if (!FormAutoFile)
		FormAutoFile = new TFormAutoFile(this);
	FormAutoFile->Show();
	if (FormAutoFile->WindowState == wsMinimized)
		FormAutoFile->WindowState = wsNormal;
	FormAutoFile->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::CBAlisarjatClick(TObject *Sender)
{
	GBAlisarja->Visible = CBAlisarjat->Checked;
	if (GBAlisarja->Visible) {
		LBSarjat->Height = GBAlisarja->Top - LBSarjat->Top - 4 * Screen->PixelsPerInch / 96;
		}
	else {
		LBSarjat->Height = ClientHeight - LBSarjat->Top - 8 * Screen->PixelsPerInch / 96;
		}
	LBSarjaVal->Height = LBSarjat->Height;
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::CBLisaTekstiClick(TObject *Sender)
{
	if (CBLisaTeksti->Checked) {
		if (!FormHtmlEdit)
			FormHtmlEdit = new TFormHtmlEdit(this);
		FormHtmlEdit->ShowModal();
		if (FormHtmlEdit->ModalResult == mrOk)
			wcsncpy(LisaFile, FormHtmlEdit->FileName.c_str(), sizeof(LisaFile)/2-1);
		delete FormHtmlEdit;
		FormHtmlEdit = NULL;
		}
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::BitBtn2Click(TObject *Sender)
{
	Application->HelpKeyword(L"Lähtöluettelot");
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Tulosluettelot");
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::TuloksetJaHtmlClick(TObject *Sender)
{
	PageControlKohde->ActivePageIndex = 1;
	PageControlSisalto->ActivePageIndex = 1;
	CBTiedLaji->ItemIndex = 3;
	CBTiedLajiChange(Sender);
	TiedNimiEdit->Text = UnicodeString(L"Tulokset_")+kilpparam.kilpkoodi+L".html";
	SetCurrentDir(FormMain->CurrentDir);
	BtnLisKaikkiClick(Sender);
	CBSisalto->ItemIndex = wcswcind(L'F', tulostettava);
	CBSisaltoChange(Sender);
	CBRajVal->ItemIndex = wcswcind(L'L', rajaus);
	PisteVal->ItemIndex = wcswcind(L'M', vaiheet);
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Reittihrvelintiedostoxml1Click(TObject *Sender)
{
	PageControlKohde->ActivePageIndex = 1;
	PageControlSisalto->ActivePageIndex = 1;
	CBTiedLaji->ItemIndex = 4;
	CBMerkisto->ItemIndex = 2;
	CBTiedLajiChange(Sender);
	CBSisaltoChange(Sender);
	TiedNimiEdit->Text = UnicodeString(L"rastiva_")+kilpparam.kilpkoodi+L".xml";
	SetCurrentDir(FormMain->CurrentDir);
	BtnLisKaikkiClick(Sender);
	CBSisalto->ItemIndex = wcswcind(L'E', tulostettava);
	CBSisaltoChange(Sender);
	CBRajVal->ItemIndex = wcswcind(L'L', rajaus);
	PisteVal->ItemIndex = wcswcind(L'M', vaiheet);
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Englanninkielisetotsikot1Click(TObject *Sender)
{
	Englanninkielisetotsikot1->Checked = !Englanninkielisetotsikot1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Listekstit1Click(TObject *Sender)
{
	if (!FormLisaTekstit)
		FormLisaTekstit = new TFormLisaTekstit(this);
	FormLisaTekstit->Show();
	if (FormLisaTekstit->WindowState == wsMinimized)
		FormLisaTekstit->WindowState = wsNormal;
	FormLisaTekstit->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Mahdollisetmetsssolijat1Click(TObject *Sender)
{
	PageControlKohde->ActivePageIndex = 1;
	PageControlSisalto->ActivePageIndex = 1;
	CBTiedLaji->ItemIndex = 3;
	CBMerkisto->ItemIndex = 1;
	CBSisaltoChange(Sender);
	TiedNimiEdit->Text = UnicodeString(L"TulosAvoinna.html");
	SetCurrentDir(FormMain->CurrentDir);
	BtnLisKaikkiClick(Sender);
	CBSisalto->ItemIndex = 0;
	CBSisaltoChange(Sender);
	CBRajVal->ItemIndex = wcswcind(L'A', rajaus);
	PisteVal->ItemIndex = 0;
}
//---------------------------------------------------------------------------



void __fastcall TTulosteForm::Kilpailijansivutuloste1Click(TObject *Sender)
{
	if (FormSivuTuloste == NULL)
		FormSivuTuloste = new TFormSivuTuloste(this);
	FormSivuTuloste->Show();
	if (FormSivuTuloste->WindowState == wsMinimized)
		FormSivuTuloste->WindowState = wsNormal;
	FormSivuTuloste->BringToFront();
}
//---------------------------------------------------------------------------


void __fastcall TTulosteForm::PisteValChange(TObject *Sender)
{
	if (wcswcind(vaiheet[PisteVal->ItemIndex], L"YZ") >= 0 && CBJarj->ItemIndex < wcswcind(L'Y', jarj))
		CBJarj->ItemIndex = wcswcind(L'Y', jarj);
	if (k_pv > 0 && wcswcind(vaiheet[PisteVal->ItemIndex], L"TU") >= 0 && jarj[CBJarj->ItemIndex] == L'Y')
		CBJarj->ItemIndex = wcswcind(L'T', jarj);
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::RGJkTlsClick(TObject *Sender)
{
	PaivitaTulossivu(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TTulosteForm::CBJarjClick(TObject *Sender)
{
	PaivitaTulossivu(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::CBLlJarjPvChange(TObject *Sender)
{
	if (CBLlPaivat->ItemIndex == 0 && CBLlJarjPv->ItemIndex > 0)
		Application->MessageBoxW(L"Kun järjestys on yhden vaiheen mukainen, ei tästä vaiheesta poissaolevia tulosteta.",
			L"Varoitus", MB_OK);
}
//---------------------------------------------------------------------------


void __fastcall TTulosteForm::CBxmlStandChange(TObject *Sender)
{
	EdXslFilename->Visible = tiedtyyppi[CBTiedLaji->ItemIndex] == L'X' && CBxmlStand->ItemIndex == 2;
}
//---------------------------------------------------------------------------


void __fastcall TTulosteForm::Oletusmuotoilu11Click(TObject *Sender)
{
	filetulosmuot.tauluhtml = 3;
	filetulosmuot.td2color = 0xffffff;
	filetulosmuot.border = 0;
	filetulosmuot.otsikot = 0;
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Oletusmuotoilu21Click(TObject *Sender)
{
	filetulosmuot.tauluhtml = 3;
	filetulosmuot.thcolor = 0xd0ffff;
	filetulosmuot.td2color = 0xe8ffff;
	filetulosmuot.otsikot = 1;
	filetulosmuot.border = 1;
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Oletusmuotoilu3vanha1Click(TObject *Sender)
{
	filetulosmuot.tauluhtml = 0;
	filetulosmuot.otsikot = 0;
}
//---------------------------------------------------------------------------

