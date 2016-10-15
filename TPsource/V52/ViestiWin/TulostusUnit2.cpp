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
#include "UnitSeuraval.h"
#include "UnitPiirival.h"
#include "UnitAutoFile.h"
#include "UnitHtmlEdit.h"
#include "UnitBrowser.h"
#include "UnitLisaTekstit.h"
#include "UnitMain.h"
#include "UnitSHLTul.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

#define MAXBUFFERSIZE 500
wchar_t FISMaarFileName[200];
static wchar_t tulostettava[20] = L"TLHOVYEP";
static wchar_t OoTulostettava[] = L"KNABCLE";
static wchar_t jarj[VAIKALUKU+2] = L"TLOANBMLP";
static wchar_t vaiheet[VAIKALUKU+2] = L"TYZUK";
static wchar_t kohde[] = L"PI", tiedtyyppi[] = L"RIEHXSB";
static wchar_t rajaus[] = L"ILHPYKAE-GDSRZM", seuralaji[] = L" SLYJTMX";
void uusiemitanalyysi(bool uusi);
int list(wchar_t kohde, wchar_t tiedlaji, wchar_t tulostettava, wchar_t jarjestys,
	wchar_t pisteval, wchar_t rajaus, int alaraja, int ylaraja, INT16 *sarjat,
	int osuus, wchar_t *kohdenimi, wchar_t merkisto, wchar_t erotin,
	wchar_t *PisteLista, wchar_t (*seuralista)[LSEURA+5], wchar_t *Lisarivit, int options);

TTulosteForm *TulosteForm;
//---------------------------------------------------------------------------
__fastcall TTulosteForm::TTulosteForm(TComponent* Owner)
   : TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
	CBRajVal->ItemIndex = 1;
	CBOsuusVal->Clear();
	CBOsuusVal->Items->Add(L"Lopputulos");
	for (int i = 1; i <= kilpparam.ntosuus; i++)
		CBOsuusVal->Items->Add(UnicodeString(i)+L". osuus");
	if (sarjaluku == 1 && kilpparam.osuusluku == 7 && tulostettava[CBSisalto->ItemIndex] == L'E' &&
		tiedtyyppi[CBTiedLaji->ItemIndex] == L'X') {
		CBOsuusVal->Items->Add(L"Osuudet 1-3");
		CBOsuusVal->Items->Add(L"Osuudet 4-7");
		}
	CBOsuusVal->ItemIndex = 0;
	if (emitfl < 1) {
		CBSisalto->Clear();
		CBSisalto->Items->Add(L"Tulokset");
		CBSisalto->Items->Add(L"Lähtöluettelo");
		CBSisalto->Items->Add(L"Osuuskohtaiset tulokset");
		CBSisalto->Items->Add(L"Osanottajat ilman tulostietoa");
		CBSisalto->Items->Add(L"Väliajat");
		CBSisalto->Items->Add(L"Kaikki tulokset ja väliajat");
		CBSisalto->Items->Add(L"Pisteet (tuloksista lasketut)");
		wcscpy(tulostettava, L"TLHOVYP");
		CBSisalto->ItemIndex = 0;
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
				  writeerror_w(L"Oletuskirjoittimen nimi liian pitkä (yli 119 merkkiä)", 0);
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
				  writeerror_w(L"Oletuskirjoittimen nimi liian pitkä (yli 119 merkkiä)", 0);
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
			if (!wcscmp(wPrinterName, CBkirj->Items->Strings[i].c_str())) {
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
	if (SaveDialog1->FileName.Length() == 0 || Tyyppivaihdettu) {
		SaveDialog1->FileName = FormMain->CurrentDir+L"\\"+L"Luett."+TiedDefault;
		}
	Tyyppivaihdettu = false;
	SaveDialog1->DefaultExt = TiedDefault;
	if (!wcscmp(TiedDefault, L"txt"))
		SaveDialog1->Filter = L"Tekstitiedostot (*.txt)|*.txt|Kaikki tiedostot (*.*)|*.*";
	if (!wcscmp(TiedDefault, L"csv"))
		SaveDialog1->Filter = L"Erotteelliset tiedostot (*.csv, *.txt)|*.csv;*.txt|Kaikki tiedostot (*.*)|*.*";
	if (!wcscmp(TiedDefault, L"html"))
		SaveDialog1->Filter = L"Html-tiedostot (*html, *.htm)|*.html;*.htm|Kaikki tiedostot (*.*)|*.*";
	if (!wcscmp(TiedDefault, L"xml"))
		SaveDialog1->Filter = L"XML-tiedostot (*.xml)|*.xml|Kaikki tiedostot (*.*)|*.*";
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
//	CBViimLeima->Checked = maalileimasin < 0;
	TabSheetKirjoitinEnter(Sender);
	Englanninkielisetotsikot1->Checked = (lsttulparam.language == 1);
}
//---------------------------------------------------------------------------
void yhtveto(wchar_t kohde, wchar_t tiedlaji, wchar_t yvlaji, wchar_t *listflnm, wchar_t merkit,
	wchar_t erotin);

// Options
// 0x00000001  Sarjat erikseen
// 0x00000004  CBBadge->Checked
// 0x00000008  KaikkiSijoitukset
// 0x00000380  RGMoniOs->ItemIndex
// 0x00001000  piirisijat
// 0x00002000  CBJoukkHyv->Checked
// 0x0007e000  kärkeä mukaan
// 0x00100000  CBViimLeima->Checked
// 0x00200000  CBTagPit->Checked
// 0x00600000  Xml-standardi
// Lehdistötuloksiin:
// 0x01000000  CBVaihtoajat->Checked
// 0x02000000  CBOsuusajat->Checked
// 0x04000000  CBVaihtosijat->Checked
// 0x08000000  CBOsuussijat->Checked
// Lehdistötulokset loppu
// 0x10000000  LangUK
// 0x20000000  CBSHLohita->Checked
// 0x80000000  UusiSivu

void __fastcall TTulosteForm::ButtonTulostaClick(TObject *Sender)
{
	int alaraja = 0, ylaraja = 999999, KohdeIndex, SisaltoIndex, Options = 0, err = 0, osuus = -1;
	int RiviLuku = 0;
#if MAXSARJALUKU+MAXYHD > 255
	INT16 sarjat[MAXSARJALUKU+MAXYHD+1];
#else
	INT16 sarjat[256];
#endif
	wchar_t kohdenimi[200] = L"", merkisto[] = L"U8AOW";
	wchar_t erottimet[] = L";\t,", **Seura = 0, *SeuraLst[2] = {L"", L""}, St[80];
	wchar_t YvLaji[] = L"OTE", pisteval;
	wchar_t emitrajaus[] = L"KLO";
	wchar_t PisteLista[VAIKALUKU+3] = L"";
	wchar_t (*SeuraLista)[LSEURA+5] = NULL;
	wchar_t SpRajaus[20] = L"";
	wchar_t *LisaRivit = NULL;
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

	if (KohdeIndex == 1 && tiedtyyppi[CBTiedLaji->ItemIndex] == L'X')
		Options |= CBxmlStand->ItemIndex * 0x00200000;

	if (TulYV->Checked) {
		yhtveto(kohde[KohdeIndex],
			tiedtyyppi[CBTiedLaji->ItemIndex],
			YvLaji[CBYv->ItemIndex],
			kohdenimi,
			merkisto[CBMerkisto->ItemIndex],
			erottimet[CBErotin->ItemIndex]);
		}
	else {

		if (rajaus[CBRajVal->ItemIndex] == L'S') {

			int ns, is = 0;
			if (FormSeuraVal && FormSeuraVal->LBSeuraVal &&
				(ns = FormSeuraVal->LBSeuraVal->Items->Count) > 0) {
				SeuraLista = new wchar_t[ns+1][LSEURA+5];
				if (!SeuraLista)
					return;
				memset(SeuraLista, 0, (ns+1)*sizeof(SeuraLista[0]));
				for (int i = 0; i < FormSeuraVal->nsra; i++) {
					if (FormSeuraVal->SeuraValinnat[i]) {
						wmemcpy(SeuraLista[is], FormSeuraVal->seuralista[i], 3);
						wcsncpy(SeuraLista[is]+3, FormSeuraVal->seuralista[i]+4, LSEURA);
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
		if (CBLisaTeksti->Checked && kohde[KohdeIndex] == L'I' && tiedtyyppi[CBTiedLaji->ItemIndex] == L'H') {
			LisaRivit = LisaFile;
			}

		while (SisaltoIndex == 0) {
			pisteval = vaiheet[PisteVal->ItemIndex];
			if (rajaus[CBRajVal->ItemIndex] == L'P') {
				alaraja = Alaraja->Text.ToInt();
				ylaraja = Ylaraja->Text.ToInt();
				}
			if (RGSarjatErikseen->ItemIndex == 1)
				Options |= 1;
			if (CBKaikkiSijoitukset->Checked)
				Options |= 8;
			if (rajaus[CBRajVal->ItemIndex] == L'D') {
				if (!FormPiiriVal)
					return;
				if (FormPiiriVal->RGPiirisijat->ItemIndex == 1)
					Options |= 4096;
				Options |= 8192*_wtoi(FormPiiriVal->EdtParhaat->Text.c_str());
				}
			if (tulostettava[CBSisalto->ItemIndex] == L'Y') {
				if (kohde[KohdeIndex] == L'P' || wcswcind(tiedtyyppi[CBTiedLaji->ItemIndex], L"RIX") < 0) {
					Application->MessageBoxW(L"Tulostettavan tiedon vaihtoehto käytettävissä vain teksti- ja xml-tulosteissa",
						L"Rajoitus", MB_OK);
						break;
					}
				}
			if (tulostettava[CBSisalto->ItemIndex] == L'F' &&
				(kohde[KohdeIndex] != L'I' || tiedtyyppi[CBTiedLaji->ItemIndex] != L'H')) {
				Application->MessageBoxW(L"Pyydetty tulosteyhdistelmä ei käytettävissä tähän tulosteeseen",
					L"Rajoitus", MB_OK);
				break;
				}
			if (tulostettava[CBSisalto->ItemIndex] == L'E' || tulostettava[CBSisalto->ItemIndex] == L'F') {
				if (pisteval == L'S' &&	(kohde[KohdeIndex] != L'I' ||
					(tiedtyyppi[CBTiedLaji->ItemIndex] != L'I' && tiedtyyppi[CBTiedLaji->ItemIndex] != L'X'))) {
					Application->MessageBoxW(L"Emit-tietojen siirtotiedosto on kohdistettu tekstitiedosto tai XML-tiedosto",
						L"Rajoitus", MB_OK);
					break;
					}
				if (kohde[KohdeIndex] != L'I' || (tiedtyyppi[CBTiedLaji->ItemIndex] != L'I' &&
					tiedtyyppi[CBTiedLaji->ItemIndex] != L'H' && tiedtyyppi[CBTiedLaji->ItemIndex] != L'X')) {
					Application->MessageBoxW(L"Emit-tulosteen tiedostomuodon on oltava html, XML tai kohdistettu tekstitiedosto",
						L"Rajoitus", MB_OK);
					break;
					}
				}
			if (tulostettava[CBSisalto->ItemIndex] == L'E' && tiedtyyppi[CBTiedLaji->ItemIndex] == L'X') {
				if (CBTagPit->Checked)
					Options |= 0x00200000;
				}
			if (rajaus[CBRajVal->ItemIndex] == L'J') {
				if (kohde[KohdeIndex] == L'I' && tiedtyyppi[CBTiedLaji->ItemIndex] != L'H') {
					Application->MessageBoxW(L"Joukkuetulokset voi tulostaa vain kirjoittimelle ja html-tiedostoon",
						L"Rajoitus", MB_OK);
					break;
					}
				}
			if (rajaus[CBRajVal->ItemIndex] == L'R') {
				if (CBJoukkHyv->Checked)
					Options |= 0x00002000;
				if (tulostettava[CBSisalto->ItemIndex] == L'E' && !emitanal_fl)
					uusiemitanalyysi(true);
				}
			if (tulostettava[CBSisalto->ItemIndex] == L'L') {
				if (CBBadge->Checked)
					Options |= 4;
				}
			if (CBOsuusVal->ItemIndex == 0 && pisteval == L'K') {
				Options |= (128*RGMoniOs->ItemIndex);
				}
			if (kohde[KohdeIndex] == L'I' && tiedtyyppi[CBTiedLaji->ItemIndex] == L'S') {
				FormSHLtul = new TFormSHLtul(this);
				FormSHLtul->ShowModal();
				if (FormSHLtul->ModalResult == mrCancel) {
					delete FormSHLtul;
					return;
					}
				delete FormSHLtul;
				}
			if (CBViimLeima->Checked)
				Options |= 0x00100000;
			if (CBSHLohita->Checked)
				Options |= 0x20000000;
			if (tiedtyyppi[CBTiedLaji->ItemIndex] == L'E' &&
				tulostettava[CBSisalto->ItemIndex] == L'T' && PisteVal->ItemIndex == 1) {
				if (CBVaihtoajat->Checked)
					Options |= 0x01000000;
				if (CBOsuusajat->Checked)
					Options |= 0x02000000;
				if (CBVaihtosijat->Checked)
					Options |= 0x04000000;
				if (CBOsuussijat->Checked)
					Options |= 0x08000000;
				}
			RiviLuku =
			list(kohde[KohdeIndex],
				tiedtyyppi[CBTiedLaji->ItemIndex],
				tulostettava[CBSisalto->ItemIndex],
				jarj[CBJarj->ItemIndex],
				pisteval,
				rajaus[CBRajVal->ItemIndex],
				alaraja, ylaraja, sarjat,
				CBOsuusVal->ItemIndex,
				kohdenimi,
				merkisto[CBMerkisto->ItemIndex],
				erottimet[CBErotin->ItemIndex],
				PisteLista,
				SeuraLista,
				LisaRivit,
				Options);
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
		wcswcind(tiedtyyppi[CBTiedLaji->ItemIndex], L"RIEHS") >= 0) {
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
	CBJoukkHyv->Visible = rajaus[CBRajVal->ItemIndex] == L'R';
	if (rajaus[CBRajVal->ItemIndex] == L'R') {
		CBSisalto->Clear();
		CBSisalto->Items->Add(L"Osuuskohtaiset tulokset");
		if (emitfl >= 1)
			CBSisalto->Items->Add(L"Emit-väliajat");
		wcscpy(tulostettava, L"HE");
		CBSisalto->ItemIndex = 0;
		}
	else if (CBSisalto->Items->Count < 7) {
		CBSisalto->Clear();
		CBSisalto->Items->Add(L"Tulokset");
		CBSisalto->Items->Add(L"Lähtöluettelo");
		CBSisalto->Items->Add(L"Osuuskohtaiset tulokset");
		CBSisalto->Items->Add(L"Osanottajat ilman tulostietoa");
		CBSisalto->Items->Add(L"Väliajat");
		CBSisalto->Items->Add(L"Kaikki tulokset ja väliajat");
		if (emitfl >= 1) {
			CBSisalto->Items->Add(L"Emit-väliajat");
			wcscpy(tulostettava, L"TLHOVYEP");
			}
		else
			wcscpy(tulostettava, L"TLHOVYP");
		CBSisalto->Items->Add(L"Pisteet (tuloksista lasketut)");
		CBSisalto->ItemIndex = 0;
		}
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
//	RGOoSra->ItemIndex = kilpparam.seuralyh_on ? 1 : 0;
	CBSarjayhd->Visible = nsarjayhd > 0;
	Automaattinentulostus1->Checked = autofl;
	PageControlKohdeChange(Sender);
	CBSisaltoChange(Sender);
	NaytaSarjavalinnat();
	PaivitaTulossivu(Sender);
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
	SrjLkm = CBSarjayhd->Checked ? sarjaluku + nsarjayhd : sarjaluku;

	LBSarjat->Clear();
	LBSarjaVal->Clear();
	if (SrjLkm == 1)
		SarjaValinnat[0] = true;
/*
	if (RGSrjYhd->ItemIndex == 1) {
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
	PisteVal->Visible = true;
	LblPistevalinta->Visible = true;
	CBKaikkiSijoitukset->Caption = L"Tulosta kaikki sijoitukset";
	CBKaikkiSijoitukset->Visible = false;
	CBJarj->Enabled = true;
	CBViimLeima->Visible = false;
	CBBadge->Visible = false;
	Label1->Visible = true;
	CBOsuusVal->Visible = true;
	CBTagPit->Visible = false;
	switch (tulostettava[CBSisalto->ItemIndex]) {
		case L'T':
		case L'H':
		case L'O':
			CBJarj->Clear();
			LblPistevalinta->Caption = L"Tulostettavat osuudet";
			PisteVal->Clear();
			PisteVal->Items->Add(L"Yksi osuus");
			PisteVal->Items->Add(L"Kaikki osuudet");
			CBJarj->Items->Add(L"Paremmuus");
			CBJarj->Items->Add(L"Sarjoittain aakkosjärjestyksessä");
			CBJarj->Items->Add(L"Sarjoittain numerojärjestyksessä");
			CBJarj->Items->Add(L"Kaikki aakkosjärjestyksessä");
			CBJarj->Items->Add(L"Kaikki numerojärjestyksessä");
			wcscpy(jarj, L"TANBM");
			wcscpy(vaiheet, L"YK");

			PisteVal->ItemIndex = 0;
			CBJarj->ItemIndex = 0;
			break;
		case L'L':
			CBJarj->Clear();
			CBBadge->Visible = kilpparam.badgelaji != 0;
			LblPistevalinta->Caption = L"Tulostettavat osuudet";
			PisteVal->Clear();
			PisteVal->Items->Add(L"Ei osuustietoja");
//			PisteVal->Items->Add(L"Yksi osuus");
			PisteVal->Items->Add(L"Kaikki osuudet");
			CBJarj->Items->Add(L"Sarjoittain aakkosjärjestyksessä");
			CBJarj->Items->Add(L"Sarjoittain numerojärjestyksessä");
			CBJarj->Items->Add(L"Kaikki aakkosjärjestyksessä");
			CBJarj->Items->Add(L"Kaikki numerojärjestyksessä");
			wcscpy(jarj, L"ANBM");
			wcscpy(vaiheet, L"YK");
			Label1->Visible = false;
			CBOsuusVal->Visible = false;
			PisteVal->ItemIndex = 0;
			CBJarj->ItemIndex = 0;
			break;
		case L'V':
			CBKaikkiSijoitukset->Visible = true;
			LblPistevalinta->Caption = L"Tulostettava väliaika";
			PisteVal->Clear();
			PisteVal->Items->Add(L"Kaikki väliajat");
			vaiheet[0] = L'0';
			for (int Va = 1; Va <= kilpparam.maxvaluku; Va++) {
				PisteVal->Items->Add(UnicodeString(Va)+L". väliaika");
				vaiheet[Va] = L'0'+Va;
				}
			vaiheet[kilpparam.maxvaluku+1] = 0;
			PisteVal->ItemIndex = 0;
			CBJarj->Clear();
			CBJarj->Items->Add(L"Paremmuus");
			CBJarj->Enabled = false;
			CBJarj->ItemIndex = 0;
			wcscpy(jarj, L"T");
			break;
		case L'Y':
			LblPistevalinta->Caption = L" ";
			CBKaikkiSijoitukset->Visible = true;
			PisteVal->Visible = false;
			PisteVal->ItemIndex = 0;
			CBJarj->Clear();
			CBJarj->Items->Add(L"Lopputulos");
			CBJarj->Items->Add(L"Sarjoittain aakkosjärjestyksessä");
			CBJarj->Items->Add(L"Sarjoittain numerojärjestyksessä");
			CBJarj->Items->Add(L"Kaikki aakkosjärjestyksessä");
			CBJarj->Items->Add(L"Kaikki numerojärjestyksessä");
			wcscpy(jarj, L"TANBM");
			CBJarj->ItemIndex = 0;
			wcscpy(vaiheet, L"TYZUK");
			break;
		case L'E':
		case L'F':
			LblPistevalinta->Caption = L"Tulostettavat taulukot";
			if (kohde[PageControlKohde->ActivePageIndex] == L'I' &&
				tiedtyyppi[CBTiedLaji->ItemIndex] == L'X') {
				PisteVal->Visible = false;
				CBTagPit->Visible = true;
				}
			else {
				PisteVal->Clear();
				PisteVal->Items->Add(L"Ajat rasteille");
				PisteVal->Items->Add(L"Rastivälien ajat");
				PisteVal->Items->Add(L"Molemmat");
//				PisteVal->Items->Add(L"Siirtotiedosto");
				CBViimLeima->Visible = true;
				}
			PisteVal->ItemIndex = 0;
			CBJarj->Clear();
			CBJarj->Items->Add(L"Paremmuus - tämä vaihe");
			CBJarj->Enabled = false;
			CBJarj->ItemIndex = 0;
			wcscpy(jarj, L"TANBMLP");
			wcscpy(vaiheet, L"KRMS");
			break;
		case L'P':
			LblPistevalinta->Caption = L"Tulostettavat pisteet";
			CBKaikkiSijoitukset->Visible = true;
			CBKaikkiSijoitukset->Caption = L"Jaa pisteet sadalla";
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
	LblPistevalinta->Visible = PisteVal->Visible;
	GBJatkTxt->Visible = (tiedtyyppi[CBTiedLaji->ItemIndex] == L'E' &&
		tulostettava[CBSisalto->ItemIndex] == L'T' && PisteVal->ItemIndex == 1);
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
	switch (CBTiedLaji->ItemIndex) {
		case 0:								// R
			wcscpy(TiedDefault, L"csv");
			break;
		case 1:								// I
			wcscpy(TiedDefault, L"txt");
			break;
		case 2:								// E
			wcscpy(TiedDefault, L"txt");
			break;
		case 3:								// H
			wcscpy(TiedDefault, L"html");
			break;
		case 4:								// X
			PisteVal->ItemIndex = 0;
			wcscpy(TiedDefault, L"xml");
			break;
		case 5:								// S
			wcscpy(TiedDefault, L"txt");
			CBMerkisto->ItemIndex = 2;
			break;
		case 6:								//
			CBMerkisto->ItemIndex = 2;
			wcscpy(TiedDefault, L"txt");
			break;
/*		case 7:								//
			CBMerkisto->ItemIndex = 1;
			wcscpy(TiedDefault, L"xml");
			break;
*/
		case 7:								//
			CBMerkisto->ItemIndex = 2;
			wcscpy(TiedDefault, L"txt");
			break;
		case 8:								//
			CBMerkisto->ItemIndex = 1;
			wcscpy(TiedDefault, L"txt");
			break;
		}
	GBJatkTxt->Visible = (tiedtyyppi[CBTiedLaji->ItemIndex] == L'E' &&
		tulostettava[CBSisalto->ItemIndex] == L'T' && PisteVal->ItemIndex == 1);
	CBErotin->Visible = (tiedtyyppi[CBTiedLaji->ItemIndex] == L'R');
	CBxmlStand->Visible = (tiedtyyppi[CBTiedLaji->ItemIndex] == L'X');
	Label8->Visible = CBErotin->Visible || CBxmlStand->Visible;
	Label8->Caption = CBErotin->Visible ? UnicodeString(L"Kenttäerotin") : UnicodeString(L"XML-standardi");
	CBMerkisto->Enabled = wcswcind(tiedtyyppi[CBTiedLaji->ItemIndex], L"RIEHX") >= 0;
	GBhtml->Visible = (tiedtyyppi[CBTiedLaji->ItemIndex] == L'H');
	PisteVal->Visible = wcswcind(tiedtyyppi[CBTiedLaji->ItemIndex], L"X") < 0;
	LblPistevalinta->Visible = PisteVal->Visible;
	CBTagPit->Visible = !PisteVal->Visible && tulostettava[CBSisalto->ItemIndex] == L'E';
//	GBSHLVal->Visible = (CBTiedLaji->ItemIndex == 5);
	CBNaytaheti->Visible = wcswcind(tiedtyyppi[CBTiedLaji->ItemIndex], L"RIHS") >= 0;
	Tyyppivaihdettu = true;
	CBOsuusVal->Clear();
	CBOsuusVal->Items->Add(L"Lopputulos");
	for (int i = 1; i <= kilpparam.ntosuus; i++)
		CBOsuusVal->Items->Add(UnicodeString(i)+L". osuus");
	if (sarjaluku == 1 && kilpparam.osuusluku == 7 && tulostettava[CBSisalto->ItemIndex] == L'E' &&
		tiedtyyppi[CBTiedLaji->ItemIndex] == L'X') {
		CBOsuusVal->Items->Add(L"Osuudet 1-3");
		CBOsuusVal->Items->Add(L"Osuudet 4-7");
		}
	CBOsuusVal->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Button2Click(TObject *Sender)
{
	OpenDialog1->FileName = FormMain->CurrentDir+UnicodeString(L"\\fismaar.txt");
	OpenDialog1->DefaultExt = L"txt";
	OpenDialog1->Filter = L"Tekstitiedostot (*.txt)|*.TXT|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		EdtFISMaar->Text = OpenDialog1->FileName;
		wcsncpy(FISMaarFileName, OpenDialog1->FileName.c_str(), sizeof(FISMaarFileName)/2-1);
		}
	SetCurrentDir(FormMain->CurrentDir);
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
	PaivitaTulossivu(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::PaivitaTulossivu(TObject *Sender)
{
	if (PageControlSisalto->ActivePageIndex == 0) {
		CBSisaltoChange(Sender);
		Alaraja->Enabled = CBRajVal->ItemIndex == 3;
		Ylaraja->Enabled = Alaraja->Enabled;
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
	wchar_t co, *p, maalistr[12];

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
		if (0) {
			autotulostusparam.vaihelkm = PisteVal->ItemIndex / 2 + 1;
			}
		else {
			autotulostusparam.vaihelkm = 1;
			}
		autotulostusparam.rajaus = rajaus[CBRajVal->ItemIndex];
		autotulostusparam.enssija = _wtoi(Alaraja->Text.c_str());
		autotulostusparam.viimsija = _wtoi(Ylaraja->Text.c_str());
		inputint_prompt(&kynnys, L"Anna tulostuskynnys", &co, this);
		kynnys = max(kynnys,1);
//		sek = intv[0] / 20;
		inputint_prompt(&sek, L"Anna aika-askel (sek)", &co, this);
//		intv[0] = 20*sek;
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

void __fastcall TTulosteForm::CBOsuusValClick(TObject *Sender)
{
	RGMoniOs->Visible = CBOsuusVal->ItemIndex == 0;
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::JuoksujKirjClick(TObject *Sender)
{
	PageControlKohde->ActivePageIndex = 0;
	PageControlSisalto->ActivePageIndex = 0;
	CBRajVal->ItemIndex = 0;
	CBSisalto->ItemIndex = wcswcind(L'L', tulostettava);
	CBOsuusVal->ItemIndex = 0;
	PisteVal->ItemIndex = 1;
	BtnLisKaikkiClick(Sender);
	CBJarj->ItemIndex = 2;
	CBBadge->Visible = kilpparam.badgelaji != 0;
	CBSisaltoChange(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TTulosteForm::JuoksujHtmlClick(TObject *Sender)
{
	PageControlKohde->ActivePageIndex = 1;
	PageControlSisalto->ActivePageIndex = 0;
	CBTiedLaji->ItemIndex = 3;
	TiedNimiEdit->Text = UnicodeString(L"Juoksujarj_")+kilpparam.kilpkoodi+L".html";
	SetCurrentDir(FormMain->CurrentDir);
	BtnLisKaikkiClick(Sender);
	CBRajVal->ItemIndex = 0;
	CBSisalto->ItemIndex = wcswcind(L'L', tulostettava);
	CBOsuusVal->ItemIndex = 0;
	PisteVal->ItemIndex = 1;
	CBJarj->ItemIndex = 2;
	CBBadge->Visible = kilpparam.badgelaji != 0;
	CBSisaltoChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Kaikkilopettaneethtmltiedostoon1Click(TObject *Sender)

{
	PageControlKohde->ActivePageIndex = 1;
	PageControlSisalto->ActivePageIndex = 0;
	CBTiedLaji->ItemIndex = 3;
	TiedNimiEdit->Text = UnicodeString(L"Tulokset_")+kilpparam.kilpkoodi+L".html";
	SetCurrentDir(FormMain->CurrentDir);
	BtnLisKaikkiClick(Sender);
	CBSisalto->ItemIndex = wcswcind(L'T', tulostettava);
	CBRajVal->ItemIndex = 1;
	CBOsuusVal->ItemIndex = 0;
	CBJarj->ItemIndex = 0;
	CBBadge->Visible = false;
	CBSisaltoChange(Sender);
	PisteVal->ItemIndex = 1;
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::Kaikkilopettaneetkirjoittimelle1Click(TObject *Sender)

{
	PageControlKohde->ActivePageIndex = 0;
	PageControlSisalto->ActivePageIndex = 0;
	BtnLisKaikkiClick(Sender);
	CBSisalto->ItemIndex = wcswcind(L'T', tulostettava);
	CBRajVal->ItemIndex = 1;
	CBOsuusVal->ItemIndex = 0;
	CBJarj->ItemIndex = 0;
	CBBadge->Visible = false;
	CBSisaltoChange(Sender);
	PisteVal->ItemIndex = 1;
}
//---------------------------------------------------------------------------


void __fastcall TTulosteForm::YksiOsuusHtmlClick(TObject *Sender)
{
	PageControlKohde->ActivePageIndex = 1;
	PageControlSisalto->ActivePageIndex = 0;
	TiedNimiEdit->Text = UnicodeString(L"Osuus_1.html");
	SetCurrentDir(FormMain->CurrentDir);
	CBSisalto->ItemIndex = wcswcind(L'T', tulostettava);
	CBRajVal->ItemIndex = 1;
	CBTiedLaji->ItemIndex = 3;
	CBOsuusVal->ItemIndex = 1;
	PisteVal->ItemIndex = 0;
	CBJarj->ItemIndex = 0;
	CBBadge->Visible = false;
	CBSisaltoChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::YksiOsuusKirjoittimelleClick(TObject *Sender)
{
	PageControlKohde->ActivePageIndex = 0;
	PageControlSisalto->ActivePageIndex = 0;
	CBSisalto->ItemIndex = wcswcind(L'T', tulostettava);
	CBRajVal->ItemIndex = 1;
	CBOsuusVal->ItemIndex = 1;
	PisteVal->ItemIndex = 0;
	CBJarj->ItemIndex = 0;
	CBBadge->Visible = false;
	CBSisaltoChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::CBJoukkHyvClick(TObject *Sender)
{
	reindex = 1;
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

void __fastcall TTulosteForm::PisteValChange(TObject *Sender)
{
	GBJatkTxt->Visible = (tiedtyyppi[CBTiedLaji->ItemIndex] == L'E' &&
		tulostettava[CBSisalto->ItemIndex] == L'T' && PisteVal->ItemIndex == 1);
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::CBVaihtoajatClick(TObject *Sender)
{
	if (CBVaihtoajat->Checked)
	   CBOsuusajat->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::CBOsuusajatClick(TObject *Sender)
{
	if (CBOsuusajat->Checked)
	   CBVaihtoajat->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::CBVaihtosijatClick(TObject *Sender)
{
	if (CBVaihtosijat->Checked)
	   CBOsuussijat->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::CBOsuussijatClick(TObject *Sender)
{
	if (CBOsuussijat->Checked)
	   CBVaihtosijat->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TTulosteForm::CBJarjChange(TObject *Sender)
{
/*
	if (ToimintaTila < 2 && wcswcind(jarj[CBJarj->ItemIndex], L"NM") >= 0) {
		Application->MessageBoxW(L"Numerojärjestys käytettävissä vain tulospalvelutilassa",
			L"Este", MB_OK);
	}
*/
}
//---------------------------------------------------------------------------

