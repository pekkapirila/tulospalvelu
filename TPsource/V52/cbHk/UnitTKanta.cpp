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

#include "WinHk.h"
#include "UnitTKanta.h"
#include "UnitEditDialog.h"
#include "UnitIlm2.h"
#include "UnitEmit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormTKanta *FormTKanta;

#define HENKLISA 100
extern TEditDialog *EditDialog;
extern wchar_t HenkTKantaNm[200];

extern ColTp hTGridCols0[HT_GRIDCOLS];
/* = {
	{L"Id", 50, 0, 1, 0, true},
	{L"Sukunimi", 80, 0, 1, 0, true},
	{L"Etunimi", 80, 0, 1, 0, true},
	{L"Seura", 100, 0, 1, 0, true},
	{L"Maa", 30, 0, 1, 0, false},
	{L"Synt.", 30, 0, 1, 0, true},
	{L"Sukup.", 30, 0, 1, 0, true},
	{L"Sarja", 60, 0, 1, 0, false},
	{L"Bib", 40, 0, 1, 0, false},
	{L"Badge", 45, 0, 1, 0, true},
	{L"Kv. id", 50, 0, 1, 0, true},
	{L"Joukkue", 55, 0, 1, 0, false},
	{L"Ranki", 50, 0, 1, 0, false},
	{L"Maksu", 35, 0, 1, 0, false},
	{L"OsOttoLkm", 60, 0, 1, 0, false},
	{L"ViimOsOtto", 60, 1, 0, 0, false},
	{L"Lisätieto", 60, 1, 0, 0, false}
}; */

ColTp hTGridCols[sizeof(hTGridCols0)/sizeof(ColTp)];
static wchar_t erotin = L';';
static kokonimi = false;
static ColTp FileCols0[HT_FILECOLS]
 = {
	{L"Id", 50, 1, 0, 0, true},
	{L"Sukunimi", 80, 1, 1, 0, true},
	{L"Etunimi", 80, 1, 2, 0, true},
	{L"Seura", 100, 1, 3, 0, true},
	{L"Maa", 30, 1, 4, 0, true},
	{L"Joukkue", 55, 1, 11, 0, true},
	{L"Sarja", 60, 1, 7, 0, true},
	{L"Badge", 45, 1, 9, 0, true},
	{L"Bib", 40, 1, 8, 0, false},
	{L"Sukup.", 30, 1, 6, 0, false},
	{L"Synt.", 30, 1, 5, 0, false},
	{L"Ikä", 50, 1, 0, 0, false},
	{L"OsOttoLkm", 60, 1, 0, 0, false},
	{L"ViimOsOtto", 60, 1, 0, 0, false},
	{L"Lisätieto", 60, 1, 0, 0, false}
};

#define HENKID_KILPID 	1
#define HENKID_INTID 	2
#define HENKID_SUKUNIMI	3
#define HENKID_ETUNIMI	4
#define HENKID_ARVO		5
#define HENKID_SEURA	6
#define HENKID_SEURALYH	7
#define HENKID_YHD		8
#define HENKID_JK		9
#define HENKID_MAA		10
#define HENKID_SARJA	11
#define HENKID_SUKUP	12
#define HENKID_SYNT		13
#define HENKID_IKA		14
#define HENKID_BIB		15
#define HENKID_BADGE	16
#define HENKID_RANKI	17
#define HENKID_OOLKM	18
#define HENKID_VIIMOO	19
#define HENKID_LISA  	20
#define HENKID_NIMI		100

FLD2 henk_kentat[] = {
	{L"Id", HENKID_KILPID, 2, 1, 1},
	{L"IntId", HENKID_INTID, 0, 1, 2},
	{L"Sukunimi", HENKID_SUKUNIMI, 4, 1, 3},
	{L"Etunimi", HENKID_ETUNIMI, 5, 1, 4},
	{L"Arvo", HENKID_ARVO, 0, 0, 5},
	{L"Seura", HENKID_SEURA, 6, 1, 6},
	{L"Seuralyh", HENKID_SEURALYH, 7, 1, 7},
	{L"Yhdistys", HENKID_YHD, 0, 0, 8},
	{L"Joukkue", HENKID_JK, 0, 0, 9},
	{L"Maa", HENKID_MAA, 8, 1, 10},
	{L"Sarja", HENKID_SARJA, 9, 1, 11},
	{L"Sukup", HENKID_SUKUP, 0, 1, 12},
	{L"Synt", HENKID_SYNT, 0, 1, 13},
	{L"Ikä", HENKID_IKA, 0, 1, 14},
	{L"Bib", HENKID_BIB, 1, 1, 15},
	{L"Badge", HENKID_BADGE, 2, 1, 16},
	{L"Ranki", HENKID_RANKI, 2, 1, 17},
	{L"OsOttoLkm", HENKID_OOLKM, 2, 1, 18},
	{L"ViimOsOtto", HENKID_VIIMOO, 2, 1, 19},
	{L"Lisätieto", HENKID_LISA, 2, 1, 20},
	{L"Nimi", HENKID_NIMI, 0, 0, 100}
	};

static int Khenk_kentat = 20;

static struct {
	int fld;
	int ipv;
	int va;
	} fldorder[50];
static int nflds;

//---------------------------------------------------------------------------
__fastcall TFormTKanta::TFormTKanta(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	memcpy(hTGridCols, hTGridCols0, sizeof(hTGridCols0));
	memcpy(FileCols, FileCols0, sizeof(FileCols0));
	nFileCols = 8;
	Henkilot = NULL;
	NHenk = 0;
	hTmaxRow = 200;
	rowHenk = NULL;
}
//---------------------------------------------------------------------------
__fastcall TFormTKanta::~TFormTKanta(void)
{
	if (Henkilot)
		delete[] Henkilot;
	Henkilot = NULL;
	if (rowHenk)
		delete[] rowHenk;
	rowHenk = NULL;
}
//---------------------------------------------------------------------------
bool __fastcall TFormTKanta::HenkiloHakuOn(void)
{
	return(NHenk > 0);
}
//---------------------------------------------------------------------------

__fastcall henkTieto::henkTieto(void)
{
	memset(this, 0, sizeof(henkTieto));
}
//---------------------------------------------------------------------------

__fastcall henkTieto::~henkTieto(void)
{

}
//---------------------------------------------------------------------------

int henkTieto::tulk_HenkRivi(wchar_t *rivi, int lineno, wchar_t erotin)
{
	wchar_t *p, *p1, *pp;
	INT16 i,res = 0;
	INT16 srj;

	wchar_t *fields[40], st1[200] = L"", st2[100] = L"", erottimet[] = L",";
	int nfld = 0, k = 0, ret = 0;
//	rivitulkintatp tlk[40];
	char ast[100];

	erottimet[0] = erotin;
	memset(fields, 0, sizeof(fields));
//	memset(tlk, 0, sizeof(tlk));

	if (wcslen(rivi) < 4)
		return(-1);
	nfld = getfields(rivi, fields, sizeof(fields)/sizeof(fields[0]), erottimet,
		L"\"" , true, false);
	if (lineno <= 0) {
		memset(fldorder, 0, sizeof(fldorder));
		nflds = 0;
		for (int i = 0; i < Khenk_kentat; i++) {
			henk_kentat[i].akt = 0;
			henk_kentat[i].oik = -1;
			}
		for (UINT ifld = 0; ifld < nfld; ifld++) {
			int ipv = 0;
			int va = 0;
			for (int i = 0; i < Khenk_kentat; i++) {
				if (ipv == 0 && wcscmp(henk_kentat[i].name, fields[ifld]) == 0) {
					fldorder[ifld].fld = henk_kentat[i].id;
					henk_kentat[i].akt = 1;
					if (nflds <= ifld)
						nflds = ifld+1;
					break;
					}
				}
			}
		deletefields(fields, sizeof(fields)/sizeof(fields[0]));
		return(1);
		}
	k = 0;
	for (UINT ifld = 0; ifld < nfld; ifld++) {
		if (fldorder[ifld].fld == 0)
			continue;
//		tlk[k].no = k + 1;
//		wcsncpy(tlk[k].teksti, fields[k], sizeof(tlk[k].teksti)/2-1);
		k++;
		}
	for (UINT ifld = 0; res == 0 && ifld < nflds; ifld++) {
		wchar_t tfld[200];
		if (fldorder[ifld].fld == 0)
			continue;
		if (ifld < nfld) {
			wcsncpy(tfld, fields[ifld], 199);
			elimwbl(tfld);
			}
		else
			tfld[0] = 0;
		switch (fldorder[ifld].fld) {
			case HENKID_KILPID:
				id = _wtoi(tfld);
				break;
			case HENKID_INTID:
				intid = _wtoi(tfld);
				break;
			case HENKID_SUKUNIMI:
				wcsncpy(sukunimi, tfld, kilpparam.lsnimi);
				break;
			case HENKID_ETUNIMI:
				wcsncpy(etunimi, tfld, kilpparam.lenimi);
				break;
			case HENKID_ARVO:
				wcsncpy(arvo, tfld, kilpparam.larvo);
				break;
			case HENKID_SEURA:
				wcsncpy(seura, tfld, kilpparam.lseura);
				break;
			case HENKID_SEURALYH:
				wcsncpy(seuralyh, tfld, kilpparam.lseuralyh);
				break;
			case HENKID_JK:
				wcsncpy(joukkue, tfld, kilpparam.ljoukkue);
				break;
			case HENKID_MAA:
				wcsncpy(maa, tfld, sizeof(maa)/2-1);
				break;
			case HENKID_SARJA:
				wcsncpy(sarja, tfld, sizeof(sarja)/2-1);
				break;
			case HENKID_SUKUP:
				sukupuoli = tfld[0];
				break;
			case HENKID_SYNT:
				synt = _wtoi(tfld);
				break;
			case HENKID_IKA:
				ika = _wtoi(tfld);
				break;
			case HENKID_BIB:
				bib = _wtoi(tfld);
				break;
			case HENKID_BADGE:
				badge = _wtoi(tfld);
				break;
			case HENKID_RANKI:
				ranki = StrToDouble(tfld);
				break;
			case HENKID_OOLKM:
				oolkm = _wtoi(tfld);
				break;
			case HENKID_VIIMOO:
				wcsncpy(viimoo, tfld, sizeof(viimoo)/2-1);
				break;
			case HENKID_LISA:
				wcsncpy(lisatieto, tfld, sizeof(lisatieto)/2-1);
				break;
			case HENKID_NIMI:
				wesnimet(tfld, st1, st2);
				wcsncpy(sukunimi, st2, kilpparam.lsnimi);
				wcsncpy(etunimi, st1, kilpparam.lenimi);
				break;
			}
		}
	deletefields(fields, sizeof(fields)/sizeof(fields[0]));
	wcscpy(st1, sukunimi);
	wcscat(st1, L" ");
	wcscat(st1, etunimi);
	keyfromwname(key, st1, 40, 0);
	return(res);
}
//---------------------------------------------------------------------------

int henkTieto::tee_HenkRivi(wchar_t *line, int len, wchar_t erotin)
{
	wchar_t st[40];

	wmemset(line, 0, len);
	for (int i = 0; i < Khenk_kentat; i++) {
		if ((int)wcslen(line) > len-40)
			break;
		if (!henk_kentat[i].akt)
			continue;
		switch (henk_kentat[i].id) {
			case HENKID_KILPID:
				swprintf(line+wcslen(line), L"%d%c", id, erotin);
				break;
			case HENKID_INTID:
				swprintf(line+wcslen(line), L"%d%c", intid, erotin);
				break;
			case HENKID_SUKUNIMI:
				swprintf(line+wcslen(line), L"%s%c", sukunimi, erotin);
				break;
			case HENKID_ETUNIMI:
				swprintf(line+wcslen(line), L"%s%c", etunimi, erotin);
				break;
			case HENKID_ARVO:
				swprintf(line+wcslen(line), L"%s%c", arvo, erotin);
				break;
			case HENKID_SEURA:
				swprintf(line+wcslen(line), L"%s%c", seura, erotin);
				break;
			case HENKID_SEURALYH:
				swprintf(line+wcslen(line), L"%s%c", seuralyh, erotin);
				break;
			case HENKID_JK:
				swprintf(line+wcslen(line), L"%s%c", joukkue, erotin);
				break;
			case HENKID_MAA:
				swprintf(line+wcslen(line), L"%s%c", maa, erotin);
				break;
			case HENKID_SARJA:
				swprintf(line+wcslen(line), L"%s%c", sarja, erotin);
				break;
			case HENKID_SUKUP:
				if (sukupuoli < L' ')
					sukupuoli = L' ';
				swprintf(line+wcslen(line), L"%c%c", sukupuoli, erotin);
				break;
			case HENKID_SYNT:
				swprintf(line+wcslen(line), L"%d%c", synt, erotin);
				break;
			case HENKID_IKA:
				swprintf(line+wcslen(line), L"%d%c", ika, erotin);
				break;
			case HENKID_BIB:
				swprintf(line+wcslen(line), L"%d%c", bib, erotin);
				break;
			case HENKID_BADGE:
				swprintf(line+wcslen(line), L"%d%c", badge, erotin);
				break;
			case HENKID_RANKI:
				swprintf(line+wcslen(line), L"%g%c", ranki, erotin);
				break;
			case HENKID_OOLKM:
				swprintf(line+wcslen(line), L"%d%c", oolkm, erotin);
				break;
			case HENKID_LISA:
				swprintf(line+wcslen(line), L"%s%c", lisatieto, erotin);
				break;
			case HENKID_NIMI:
				swprintf(line+wcslen(line), L"%s %s%c", sukunimi, etunimi, erotin);
				break;
			}
		}
	if (wcslen(line) > 1)
		line[wcslen(line)-1] = L'\n';
	return(0);
}

//---------------------------------------------------------------------------
int henkTieto::tulkRivi(wchar_t *iBuf)
{
	wchar_t *p, *fields[50], st[100] = L"", erottimet[4] = L";\n";
	int nfld = 0, k = 0;

	memset(fields, 0, sizeof(fields));
	erottimet[0] = erotin;
	if (FormTKanta->IRMAtiedosto1->Checked) {
		nfld = getfields(iBuf, fields, sizeof(fields)/sizeof(fields[0]), L";", NULL, true, false);
		if (nfld < ++k)
			goto pois;
		id =  _wtoi(fields[0]);
		if (nfld < ++k)
			goto pois;
		wcsncpy(etunimi, fields[1], kilpparam.lenimi);
		elimwbl(etunimi);
		if (nfld < ++k)
			goto pois;
		wcsncpy(sukunimi, fields[2], kilpparam.lsnimi);
		elimwbl(sukunimi);
		if (nfld < ++k)
			goto pois;
		sukupuoli = towupper(fields[3][0]);
		if (nfld < ++k)
			goto pois;
		synt = _wtoi(fields[4]);
		if (nfld < ++k)
			goto pois;
		wcsncpy(seura, fields[5], kilpparam.lseura);
		wcsncpy(seuralyh, fields[5], kilpparam.lseuralyh);
		ranki = StrToDouble(fields[5]);
		if (nfld < ++k)
			goto pois;
		maksettu = towupper(fields[6][0]);
		if (nfld < ++k)
			goto pois;
		badge = _wtoi(fields[7]);
		++k;
		}
	else if (FormTKanta->Hiihtoliitontiedosto1->Checked) {
		nfld = getfields(iBuf, fields, sizeof(fields)/sizeof(fields[0]), L";", L"\"", true, false);
		if (nfld < ++k)
			goto pois;
		id =  _wtoi(fields[0]+1);
		if (nfld < ++k)
			goto pois;
		wcsncpy(sarja, fields[1], 10);
		sukupuoli = sarja[0] == L'N' ? L'L' : L'M';
		if (nfld < ++k)
			goto pois;
		synt = _wtoi(fields[2]);
		if (nfld < ++k)
			goto pois;
		wcsncpy(st, fields[3], 99);
		st[99] = 0;
		if ((p = wcsstr(st+2, L" Van ")) == NULL
			&& (p = wcsstr(st+2, L" Von ")) == NULL
			&& (p = wcsstr(st+2, L" De ")) == NULL
			&& (p = wcsstr(st+2, L" Af ")) == NULL
			&& (p = wcsstr(st+2, L" Av ")) == NULL
			&& (p = wcsstr(st+2, L" Mc ")) == NULL
			&& (p = wcsstr(st+2, L" Ben ")) == NULL)
			for (p = st + wcslen(st) - 1; p > st && *p != L' '; p--) ;
		wcsncpy(sukunimi, p+1, kilpparam.lsnimi);
		*p = 0;
		wcsncpy(etunimi, st, kilpparam.lenimi);
		if (nfld < ++k)
			goto pois;
		wcsncpy(seura, fields[4], kilpparam.lseura);
		id =  _wtoi(fields[0]+1);
		++k;
		}
	else if (FormTKanta->FIStiedosto1->Checked) {
		erottimet[0] = erotin;
		if (*iBuf >= L'A')
			goto pois;
		nfld = getfields(iBuf, fields, sizeof(fields)/sizeof(fields[0]), erottimet, NULL, true, false);
		if (nfld < ++k)
			goto pois;
		k = nfld+1;
		intid =  _wtoi(fields[0]);
		if (nfld > 15)
			id =  _wtoi(fields[15]);
		if (nfld > 12)
			wcsncpy(sukunimi, fields[12], kilpparam.lsnimi);
		elimwbl(sukunimi);
		if (nfld > 1)
			wcsncpy(st, fields[1], 99);
		st[99] = 0;
		if (wcslen(st) > wcslen(sukunimi)) {
			elimwbl(st + wcslen(sukunimi));
			wcsncpy(etunimi, st+wcslen(sukunimi), kilpparam.lenimi);
			}
		if (nfld > 17) {
			synt = _wtoi(fields[17])+1980;
			synt = synt % 100 + 1920;
			}
		if (nfld > 3)
			wcsncpy(maa, fields[3], 3);
		if (nfld > 13)
			sukupuoli = fields[13][0];
		if (nfld > 14)
			wcsncpy(seura, fields[14], kilpparam.lseura);
		if (nfld > 4) {
			if (fields[4][0] == L'-')
				ranki = 9999.99;
			else
				ranki = StrToDouble(fields[4]);
			}
		if (nfld > 8) {
			if (fields[8][0] == L'-')
				badge = 999999;
			else
				badge = 100*StrToDouble(fields[8]);
			}
		}
	else {
		erottimet[0] = erotin;
		nfld = getfields(iBuf, fields, sizeof(fields)/sizeof(fields[0]), erottimet, NULL, true, false);
		if (nfld < ++k)
			goto pois;
		id =  _wtoi(fields[0]);
		if (id == 0 && fields[0][0] > L'9')
			goto pois;
		if (nfld < ++k)
			goto pois;
		if (kokonimi) {
			elimwbl(fields[1]);
			wesnimet(fields[1], etunimi, sukunimi);
			}
		else {
			elimwbl(fields[1]);
			wcsncpy(sukunimi, fields[1], kilpparam.lsnimi);
			if (nfld < ++k)
				goto pois;
			elimwbl(fields[2]);
			wcsncpy(etunimi, fields[2], kilpparam.lenimi);
			}
		if (nfld < ++k)
			goto pois;
		wcsncpy(seura, fields[k-1], kilpparam.lseura);
		if (nfld < ++k)
			goto pois;
		wcsncpy(maa, fields[k-1], 3);
		if (nfld < ++k)
			goto pois;
		wcsncpy(joukkue, fields[k-1], kilpparam.ljoukkue);
		if (nfld < ++k)
			goto pois;
		wcsncpy(sarja, fields[k-1], 11);
		if (nfld < ++k)
			goto pois;
		badge = _wtoi(fields[k-1]);
		if (nfld < ++k)
			goto pois;
		bib = _wtoi(fields[k-1]);
		if (nfld < ++k)
			goto pois;
		sukupuoli = towupper(fields[k-1][0]);
		if (nfld < ++k)
			goto pois;
		synt =  _wtoi(fields[k-1]);
		if (nfld < ++k)
			goto pois;
		ika =  _wtoi(fields[k-1]);
		++k;
		}
	pois:
	wcscpy(st, sukunimi);
	wcscat(st, L" ");
	wcscat(st, etunimi);
	keyfromwname(key, st, 40, 0);
	deletefields(fields, sizeof(fields)/sizeof(fields[0]));
	return(k-1);
}

//---------------------------------------------------------------------------
int __fastcall TFormTKanta::LueHenkilot(wchar_t *flName, bool NaytaDialogi)
{
	bool otsrivi = false;
	TextFl *InFile = NULL;

	if (Henkilot) {
		delete[] Henkilot;
		Henkilot = NULL;
		NHenk = 0;
		maxNHenk = 0;
	}
	if (rowHenk) {
		delete[] rowHenk;
		rowHenk = NULL;
	}
	if (IRMAtiedosto1->Checked) {
		otsrivi = true;
		}
	if (!FormTKanta->IRMAtiedosto1->Checked && !FormTKanta->Hiihtoliitontiedosto1->Checked &&
		!FormTKanta->FIStiedosto1->Checked) {
		nFileCols = ilmkentat > 1 ? 8 : 7;
		FileCols[ilmkentat].Visible = false;
		if (Osanottolaskuri1->Checked) {
			nFileCols += 2;
			FileCols[12].Visible = true;
			}
		else
			FileCols[12].Visible = false;
		FileCols[13].Visible = FileCols[12].Visible;
		}
	kokonimi = Sukujaetunimierikseen1->Checked == false;
	if (flName) {
		InFile = new TextFl(flName, L"t");
		if (!InFile->IsOpen()) {
			delete InFile;
			InFile = NULL;
			}
		else
			wcsncpy(HenkTKantaNm, flName, sizeof(HenkTKantaNm)/2-1);
		}
	if (InFile == NULL && (NaytaDialogi || wcslen(flName) < 2)) {
		OpenDialog1->InitialDir = FormMain->CurrentDir;
		OpenDialog1->FileName = UnicodeString(HenkTKantaNm);
		OpenDialog1->DefaultExt = L"csv";
		OpenDialog1->Filter = L"Teksti/csv-tiedosto (*.csv, *.txt)|*.csv;*.txt|Kaikki tiedostot (*.*)|*.*";
		if (OpenDialog1->Execute()) {
			InFile = new TextFl(OpenDialog1->FileName.c_str(), L"t");
			if (InFile->IsOpen())
				wcsncpy(HenkTKantaNm, OpenDialog1->FileName.c_str(), sizeof(HenkTKantaNm)/2-1);
			}
		}
	if (InFile != NULL) {
		if (InFile->IsOpen()) {
			wchar_t Buf[1000];
			int N = 0;

			while (!InFile->Feof()) {
				if (InFile->ReadLine(Buf, 998) && wcslen(Buf) > 20) {
					if (N == 0) {
						wchar_t *p1, *p2;

						p1 = wcsstr(Buf, L";");
						p2 = wcsstr(Buf, L"\t");
						if ((p1 && !p2) || (p1 && p2 && p1 < p2 )) {
							erotin = L';';
							}
						else {
							erotin = L'\t';
							 }
						}
					N++;
					}
				}
			if (N > 0) {
				bool ensrivi = true;
				int rv = 0;

				Henkilot = new henkTieto[N+HENKLISA];
				maxNHenk = N+HENKLISA;
				if (rowHenk == NULL)
					rowHenk = new int[maxNHenk+1];
				InFile->Rewind();
				NHenk = 0;
				while (!InFile->Feof()) {
					if (InFile->ReadLine(Buf, 998) && wcslen(Buf) > 20) {
						if (Otsikkorivinmukaan1->Checked) {
							if (!Henkilot[NHenk].tulk_HenkRivi(Buf, rv++, erotin))
								NHenk++;
							continue;
							}
						if (ensrivi && otsrivi) {
							ensrivi = false;
							continue;
							}
						if (Henkilot[NHenk].tulkRivi(Buf) > 2)
							NHenk++;
						else
							continue;
						}
					}
				}
			InFile->Close();
			}
		delete InFile;
		}
	if (NHenk > 1) {
		quicksort((char *)Henkilot, NHenk, sizeof(Henkilot[0].key), sizeof(Henkilot[0]));
	}
	aktHenk = 0;
	SetCurrentDir(FormMain->CurrentDir);
	return(0);
}
//---------------------------------------------------------------------------

int __fastcall TFormTKanta::NaytaHenkilot(wchar_t *alain, wchar_t *ylain, wchar_t *etualain, wchar_t *etuylain, int badge)
{
	wchar_t ch, cs, ch2, ct, line[80], trk;
	int i, n, d, ep, rvali, sj;
	int tl, tlsum;
	char hkey[40];
	henkTieto *pHenk;
	static int edEnsHenk;
	static wchar_t ala[LSNIMI+1], yla[LSNIMI+6], etuala[LENIMI+1], etuyla[LENIMI+6];

	if (NHenk == 0)
		return(0);
	if (alain == NULL && ylain == NULL && etualain == NULL && etuylain == NULL && badge == 0) {
		badge = ValBadge;
		}
	ValBadge = badge;
	if (!badge) {
		if (alain) {
			wcsncpy(ala, alain, LSNIMI);
			}
		if (ylain) {
		   wcsncpy(yla, ylain, LSNIMI+4);
			}
		if (etualain) {
		   wcsncpy(etuala, etualain, LENIMI);
			}
		if (etuylain) {
		   wcsncpy(etuyla, etuylain, LENIMI+4);
			}
		}
	memset(line, 0, sizeof(line));
	memset(rowHenk, 0, (NHenk+1)*sizeof(int));
	hTGrid->RowCount = 2;
	hTGrid->DefaultRowHeight = 20 * Screen->PixelsPerInch / 96;
	n = 0;
	if ((hTGridCols[0].Visible = IdLisenssino1->Checked) == true) n++;
	if ((hTGridCols[1].Visible = Nimi1->Checked) == true) n++;
	if ((hTGridCols[2].Visible = Nimi1->Checked) == true) n++;
	if ((hTGridCols[3].Visible = Seura1->Checked) == true) n++;
	if ((hTGridCols[4].Visible = Maa1->Checked) == true) n++;
	if ((hTGridCols[5].Visible = Syntymvuosi1->Checked) == true) n++;
	if ((hTGridCols[6].Visible = Sukupuoli1->Checked) == true) n++;
	if ((hTGridCols[7].Visible = Sarja1->Checked) == true) n++;
	if ((hTGridCols[8].Visible = Rintanumero1->Checked) == true) n++;
	if ((hTGridCols[9].Visible = Emit1->Checked) == true) n++;
	if ((hTGridCols[10].Visible = Kvid1->Checked) == true) n++;
	if ((hTGridCols[11].Visible = Joukkue1->Checked) == true) n++;
	if ((hTGridCols[12].Visible = Rankipisteet1->Checked) == true) n++;
	if ((hTGridCols[13].Visible = Maksumerkint1->Checked) == true) n++;
	if ((hTGridCols[14].Visible = Osanotot1->Checked) == true) n++;
	if ((hTGridCols[15].Visible = Osanotot1->Checked) == true) n++;
	hTGrid->ColCount = n;
	n = 0;
	memset(ColContent, 0, sizeof(ColContent));
	for (int i = 0; i < sizeof(hTGridCols0)/sizeof(ColTp); i++) {
		if (hTGridCols[i].Visible) {
			if (n+1 > hTGrid->ColCount)
				hTGrid->ColCount = n+1;
			ColContent[n] = i;
			hTGridCols[i].Order = n;
			hTGrid->Cells[n][0] = UnicodeString(hTGridCols[i].Caption);
			hTGrid->Cells[n][1] = UnicodeString(L"");
			hTGrid->ColWidths[n] = hTGridCols[i].Width * Screen->PixelsPerInch / 96;
			n += 1;
			}
		}
	n = 0;
	ensHenk = -1;
	pHenk = Henkilot;
	if (!badge && ala && *ala) {
		wcscpy(line, ala);
		if (!wcscmp(ala, yla) && etuala && *etuala) {
			wcscat(line, L" ");
			wcscat(line, etuala);
			}
		keyfromwname(hkey, line, 40, 0);
		pHenk = (henkTieto *) bsrch(hkey, (char *) Henkilot, &NHenk, sizeof(Henkilot[0].key), sizeof(Henkilot[0]));
		}
	for (int k = pHenk-Henkilot; k < NHenk; k++) {
		if (!Nytkaikkirivit1->Checked && n > hTmaxRow)
			break;
/*		if (ala && *ala) {
			wcscpy(line, ala);
			if (!wcscmp(ala, yla) && etuala && *etuala) {
				wcscat(line, L" ");
				wcscat(line, etuala);
				}
			keyfromwname(hkey, line, 40);
			if (memcmp(Henkilot[k].key, hkey, 40) < 0)
				continue;
			}
*/
		if (!badge && yla && *yla) {
			wcscpy(line, yla);
			if (!wcscmp(ala, yla) && etuyla && *etuyla) {
				wcscat(line, L" ");
				wcscat(line, etuyla);
				}
			keyfromwname(hkey, line, 40, 0);
			if (memcmp(Henkilot[k].key, hkey, 40) > 0)
				break;
			}
		if (badge && badge != Henkilot[k].badge)
			continue;
		if (ensHenk < 0) {
			ensHenk = k;
			if (ensHenk != edEnsHenk)
				aktHenk = ensHenk;
			}
		n++;
		if (hTGrid->RowCount < n+1)
			hTGrid->RowCount = n+1;
		if (n == 1 && aktHenk < k)
			aktHenk = k;
		if (k == aktHenk)
			hTGrid->Row = n;
		rowHenk[n] = k;
		for (int i = 0; i < sizeof(hTGridCols0)/sizeof(ColTp); i++) {
			if (hTGridCols[i].Visible) {
				int col = hTGridCols[i].Order;

				switch (i) {
					case 0:
						hTGrid->Cells[col][n] = UnicodeString(Henkilot[k].id);
						break;
					case 1:
						hTGrid->Cells[col][n] = UnicodeString(Henkilot[k].sukunimi);
						break;
					case 2:
						hTGrid->Cells[col][n] = UnicodeString(Henkilot[k].etunimi);
						break;
					case 3:
						hTGrid->Cells[col][n] = UnicodeString(Henkilot[k].seura);
						break;
					case 4:
						hTGrid->Cells[col][n] = UnicodeString(Henkilot[k].maa);
						break;
					case 5:
						hTGrid->Cells[col][n] = UnicodeString(Henkilot[k].synt);
						break;
					case 6:
						hTGrid->Cells[col][n] = UnicodeString(Henkilot[k].sukupuoli);
						break;
					case 7:
						hTGrid->Cells[col][n] = UnicodeString(Henkilot[k].sarja);
						break;
					case 8:
						hTGrid->Cells[col][n] = UnicodeString(Henkilot[k].bib);
						break;
					case 9:
						hTGrid->Cells[col][n] = UnicodeString(Henkilot[k].badge);
						break;
					case 10:
						hTGrid->Cells[col][n] = UnicodeString(Henkilot[k].intid);
						break;
					case 11:
						hTGrid->Cells[col][n] = UnicodeString(Henkilot[k].joukkue);
						break;
					case 12:
						hTGrid->Cells[col][n] = UnicodeString(Henkilot[k].ranki);
						break;
					case 13:
						hTGrid->Cells[col][n] = UnicodeString(Henkilot[k].maksettu);
						break;
					case 14:
						hTGrid->Cells[col][n] = UnicodeString(Henkilot[k].oolkm);
						break;
					case 15:
						hTGrid->Cells[col][n] = UnicodeString(Henkilot[k].viimoo);
						break;
					case 16:
						hTGrid->Cells[col][n] = UnicodeString(Henkilot[k].lisatieto);
						break;
					}
				}
			}
		}
	NEsilla = n;
	if (hTGrid->Row < 1)
		hTGrid->Row = 1;
	aktHenk = rowHenk[hTGrid->Row];
	edEnsHenk = ensHenk;
	return(n);
}

//---------------------------------------------------------------------------
void __fastcall TFormTKanta::Selaa(int haku)
{
	if (haku > 0 && aktHenk < NHenk)
		aktHenk++;
	if (haku < 0 && aktHenk > 0)
		aktHenk--;
	NaytaHenkilot(NULL, NULL, NULL, NULL, 0);
	hTGrid->Options << goEditing;
	hTGrid->SetFocus();
}

//---------------------------------------------------------------------------

int __fastcall TFormTKanta::EtsiHenkilo(int lisno)
{
	if (lisno < 1 || NHenk == 0)
		return(0);
	for (int i = 0; i < NHenk; i++) {
		if (lisno == Henkilot[i].id) {
			NaytaHenkilot(Henkilot[i].sukunimi, Henkilot[i].sukunimi, Henkilot[i].etunimi, Henkilot[i].etunimi, 0);
			aktHenk = i;
			return(1);
			}
		}
	return(0);
}

//---------------------------------------------------------------------------
void __fastcall TFormTKanta::Luetiedot1Click(TObject *Sender)
{
	LueHenkilot(NULL, true);
	NaytaHenkilot(L"", L"", L"", L"", 0);
}
//---------------------------------------------------------------------------
int __fastcall TFormTKanta::THenktiedot(int KHenk, wchar_t *snimi, wchar_t *enimi,
	wchar_t *seura, wchar_t *maa, wchar_t *sarja, wchar_t *joukkue, int *id,
	int *intid, int *badge, int *bib, wchar_t *sukup, int *ika, int *synt)
{
	if (NEsilla < 1)
		return(-1);
	if (ValBadge) {
		if (NEsilla == 1 || KHenk == 0) {
			KHenk = aktHenk;
			}
		else {
			int r, k, n;

			for (r = 1; r <= NEsilla; r++)
				if (hTGrid->Cells[hTGridCols[0].Order][r].ToInt() == UnicodeString(Henkilot[aktHenk].id))
					break;
			if (KHenk < 0 && r > 1)
				r--;
			else if (KHenk > 0 && r < NEsilla)
				r++;
			k = hTGrid->Cells[hTGridCols[0].Order][r].ToInt();
			for (n = 0; n < NHenk; n++)
				if (Henkilot[n].id == k)
					break;
			if (n < NHenk)
				KHenk = n;
			else
				KHenk = aktHenk;
			}
		}
	else
		KHenk += aktHenk;
	if (KHenk < 0 || KHenk > NHenk || NEsilla < 1 ) {
		return(-1);
		}
	if (snimi)
		wcscpy(snimi, Henkilot[KHenk].sukunimi);
	if (enimi)
		wcscpy(enimi, Henkilot[KHenk].etunimi);
	if (seura)
		wcscpy(seura, Henkilot[KHenk].seura);
	if (maa)
		wcscpy(maa, Henkilot[KHenk].maa);
	if (sarja)
		wcscpy(sarja, Henkilot[KHenk].sarja);
	if (joukkue)
		wcscpy(joukkue, Henkilot[KHenk].joukkue);
	if (id)
		*id = Henkilot[KHenk].id;
	if (intid)
		*intid = Henkilot[KHenk].intid;
	if (badge)
		*badge = Henkilot[KHenk].badge;
	if (bib)
		*bib = Henkilot[KHenk].bib;
	if (ika)
		*ika = Henkilot[KHenk].ika;
	if (synt)
		*synt = Henkilot[KHenk].synt;
	if (sukup)
		*sukup = Henkilot[KHenk].sukupuoli;
	aktHenk = KHenk;
	NaytaHenkilot(NULL, NULL, NULL, NULL, ValBadge);
	return(KHenk);
}

//---------------------------------------------------------------------------


void __fastcall TFormTKanta::FormResize(TObject *Sender)
{
	hTGrid->Width = FormTKanta->ClientWidth;
	hTGrid->Height = FormTKanta->ClientHeight;
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Sukujaetunimierikseen1Click(TObject *Sender)
{
    Sukujaetunimierikseen1->Checked = !Sukujaetunimierikseen1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::aulukonrivimr1Click(TObject *Sender)
{
	EditDialog = new TEditDialog(FormTKanta);
	EditDialog->KysyArvo(UnicodeString(hTmaxRow), UnicodeString(L"Rivien maksimimäärä"));
	EditDialog->ShowModal();
	hTmaxRow = EditDialog->Arvo.ToInt();
	delete EditDialog;
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Sulje1Click(TObject *Sender)
{
	hTGrid->RowCount = 2;
	if (rowHenk)
		delete[] rowHenk;
	rowHenk = NULL;
	delete[] Henkilot;
	Henkilot = NULL;
	NHenk = 0;
	maxNHenk = 0;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::FormShow(TObject *Sender)
{
	if (ilmkentat >= 1) {
		Muokkaus1->Visible = true;
		Asetukset1->Visible = false;
		}
	if (ilmkentat > 0 && NHenk == 0) {
		SetCurrentDir(FormMain->CurrentDir);
		LueHenkilot(HenkTKantaNm, true);
		NaytaHenkilot(L"", L"", L"", L"", 0);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Puolipiste1Click(TObject *Sender)
{
	Puolipiste1->Checked = !Puolipiste1->Checked;
	if (Puolipiste1->Checked) {
		abulaattori1->Checked = false;
		Pilkku1->Checked = false;
		erotin = L';';
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::abulaattori1Click(TObject *Sender)
{
	abulaattori1->Checked = !abulaattori1->Checked;
	if (abulaattori1->Checked) {
		Puolipiste1->Checked = false;
		Pilkku1->Checked = false;
		erotin = L'\t';
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Pilkku1Click(TObject *Sender)
{
	Pilkku1->Checked = !Pilkku1->Checked;
	if (Pilkku1->Checked) {
		abulaattori1->Checked = false;
		Puolipiste1->Checked = false;
		erotin = L',';
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Otsikkorivinmukaan1Click(TObject *Sender)
{
	Otsikkorivinmukaan1->Checked = !Otsikkorivinmukaan1->Checked;
	Kytoletusmuotoa1->Checked = !Otsikkorivinmukaan1->Checked;
	Ohjelmanoletusmuoto1->Checked = !Otsikkorivinmukaan1->Checked;
	Hiihtoliitontiedosto1->Checked = false;
	IRMAtiedosto1->Checked = false;
	FIStiedosto1->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Kytoletusmuotoa1Click(TObject *Sender)
{
	Kytoletusmuotoa1->Checked = !Kytoletusmuotoa1->Checked;
	Ohjelmanoletusmuoto1->Checked = Kytoletusmuotoa1->Checked;
	Otsikkorivinmukaan1->Checked = !Kytoletusmuotoa1->Checked;
	Hiihtoliitontiedosto1->Checked = false;
	IRMAtiedosto1->Checked = false;
	FIStiedosto1->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Hiihtoliitontiedosto1Click(TObject *Sender)
{
	Hiihtoliitontiedosto1->Checked = !Hiihtoliitontiedosto1->Checked;
	Otsikkorivinmukaan1->Checked = !Hiihtoliitontiedosto1->Checked;
	Kytoletusmuotoa1->Checked = false;
	Ohjelmanoletusmuoto1->Checked = Kytoletusmuotoa1->Checked;
	IRMAtiedosto1->Checked = false;
	FIStiedosto1->Checked = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormTKanta::IRMAtiedosto1Click(TObject *Sender)
{
	IRMAtiedosto1->Checked = !IRMAtiedosto1->Checked;
	Otsikkorivinmukaan1->Checked = !IRMAtiedosto1->Checked;
	Kytoletusmuotoa1->Checked = false;
	Ohjelmanoletusmuoto1->Checked = Kytoletusmuotoa1->Checked;
	Hiihtoliitontiedosto1->Checked = false;
	FIStiedosto1->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::FIStiedosto1Click(TObject *Sender)
{
	FIStiedosto1->Checked = !FIStiedosto1->Checked;
	Otsikkorivinmukaan1->Checked = !FIStiedosto1->Checked;
	Kytoletusmuotoa1->Checked = false;
	Ohjelmanoletusmuoto1->Checked = Kytoletusmuotoa1->Checked;
	Hiihtoliitontiedosto1->Checked = false;
	IRMAtiedosto1->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::IdLisenssino1Click(TObject *Sender)
{
	IdLisenssino1->Checked = !IdLisenssino1->Checked;
	NaytaHenkilot(NULL, NULL, NULL, NULL, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Nimi1Click(TObject *Sender)
{
	Nimi1->Checked = !Nimi1->Checked;
	NaytaHenkilot(NULL, NULL, NULL, NULL, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Seura1Click(TObject *Sender)
{
	Seura1->Checked = !Seura1->Checked;
	NaytaHenkilot(NULL, NULL, NULL, NULL, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Maa1Click(TObject *Sender)
{
	Maa1->Checked = !Maa1->Checked;
	NaytaHenkilot(NULL, NULL, NULL, NULL, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Syntymvuosi1Click(TObject *Sender)
{
	Syntymvuosi1->Checked = !Syntymvuosi1->Checked;
	NaytaHenkilot(NULL, NULL, NULL, NULL, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Sukupuoli1Click(TObject *Sender)
{
	Sukupuoli1->Checked = !Sukupuoli1->Checked;
	NaytaHenkilot(NULL, NULL, NULL, NULL, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Sarja1Click(TObject *Sender)
{
	Sarja1->Checked = !Sarja1->Checked;
	NaytaHenkilot(NULL, NULL, NULL, NULL,0);
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Rintanumero1Click(TObject *Sender)
{
	Rintanumero1->Checked = !Rintanumero1->Checked;
	NaytaHenkilot(NULL, NULL, NULL, NULL, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Emit1Click(TObject *Sender)
{
	Emit1->Checked = !Emit1->Checked;
	NaytaHenkilot(NULL, NULL, NULL, NULL, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Joukkue1Click(TObject *Sender)
{
	Joukkue1->Checked = !Joukkue1->Checked;
	NaytaHenkilot(NULL, NULL, NULL, NULL, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Rankipisteet1Click(TObject *Sender)
{
	Rankipisteet1->Checked = !Rankipisteet1->Checked;
	NaytaHenkilot(NULL, NULL, NULL, NULL, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Maksumerkint1Click(TObject *Sender)
{
	Maksumerkint1->Checked = !Maksumerkint1->Checked;
	NaytaHenkilot(NULL, NULL, NULL, NULL, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Kvid1Click(TObject *Sender)
{
	Kvid1->Checked = !Kvid1->Checked;
	NaytaHenkilot(NULL, NULL, NULL, NULL, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Osanotot1Click(TObject *Sender)
{
	Osanotot1->Checked = !Osanotot1->Checked;
	NaytaHenkilot(NULL, NULL, NULL, NULL, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Nytkaikkirivit1Click(TObject *Sender)
{
	Nytkaikkirivit1->Checked = !Nytkaikkirivit1->Checked;
	NaytaHenkilot(NULL, NULL, NULL, NULL, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Help1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Henkilötietokanta");
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Sallimuokkaustaulukossa1Click(TObject *Sender)
{
	Nytkaikkirivit1->Checked = true;
	NaytaHenkilot(L"", L"", L"", L"", 0);
	hTGrid->Options << goEditing;
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Peruutamuutokset1Click(TObject *Sender)
{
	hTGrid->Options >> goEditing;
	NaytaHenkilot(L"", L"", L"", L"", 0);
}
//---------------------------------------------------------------------------

void TFormTKanta::paivitaHenkilot(void)
{
	wchar_t st[200];

	hTGrid->Options >> goEditing;
	for (int r = 0; r < hTGrid->RowCount-1; r++) {
		for (UINT c = 0; c < sizeof(hTGridCols0)/sizeof(ColTp); c++) {
			if (!hTGridCols[c].Visible)
				continue;
			switch (c) {
				case 0:
					Henkilot[r].id = _wtoi(hTGrid->Cells[hTGridCols[c].Order][r+1].c_str());
					break;
				case 1:
					wcsncpy(Henkilot[r].sukunimi, hTGrid->Cells[hTGridCols[c].Order][r+1].c_str(), sizeof(Henkilot[r].sukunimi)/2-1);
					break;
				case 2:
					wcsncpy(Henkilot[r].etunimi, hTGrid->Cells[hTGridCols[c].Order][r+1].c_str(), sizeof(Henkilot[r].etunimi)/2-1);
					break;
				case 3:
					wcsncpy(Henkilot[r].seura, hTGrid->Cells[hTGridCols[c].Order][r+1].c_str(), sizeof(Henkilot[r].seura)/2-1);
					break;
				case 4:
					wcsncpy(Henkilot[r].maa, hTGrid->Cells[hTGridCols[c].Order][r+1].c_str(), sizeof(Henkilot[r].maa)/2-1);
					break;
				case 5:
					Henkilot[r].synt = _wtoi(hTGrid->Cells[hTGridCols[c].Order][r+1].c_str());
					break;
				case 6:
					Henkilot[r].sukupuoli = hTGrid->Cells[hTGridCols[c].Order][r+1].c_str()[0];
					break;
				case 7:
					wcsncpy(Henkilot[r].sarja, hTGrid->Cells[hTGridCols[c].Order][r+1].c_str(), sizeof(Henkilot[r].sarja)/2-1);
					break;
				case 8:
					Henkilot[r].bib = _wtoi(hTGrid->Cells[hTGridCols[c].Order][r+1].c_str());
					break;
				case 9:
					Henkilot[r].badge = _wtoi(hTGrid->Cells[hTGridCols[c].Order][r+1].c_str());
					break;
				case 10:
					Henkilot[r].intid = _wtoi(hTGrid->Cells[hTGridCols[c].Order][r+1].c_str());
					break;
				case 11:
					wcsncpy(Henkilot[r].joukkue, hTGrid->Cells[hTGridCols[c].Order][r+1].c_str(), sizeof(Henkilot[r].joukkue)/2-1);
					break;
				case 12:
					Henkilot[r].ranki = StrToDouble(hTGrid->Cells[hTGridCols[c].Order][r+1].c_str());
					break;
				case 13:
					Henkilot[r].maksettu = hTGrid->Cells[hTGridCols[c].Order][r+1].c_str()[0];
					break;
				case 14:
					Henkilot[r].oolkm = _wtoi(hTGrid->Cells[hTGridCols[c].Order][r+1].c_str());
					break;
				case 15:
					wcsncpy(Henkilot[r].viimoo, hTGrid->Cells[hTGridCols[c].Order][r+1].c_str(), sizeof(Henkilot[r].viimoo)/2-1);
					break;
				case 16:
					wcsncpy(Henkilot[r].lisatieto, hTGrid->Cells[hTGridCols[c].Order][r+1].c_str(), sizeof(Henkilot[r].lisatieto)/2-1);
					break;
				}
			}
		wcscpy(st, Henkilot[r].sukunimi);
		wcscat(st, L" ");
		wcscat(st, Henkilot[r].etunimi);
		keyfromwname(Henkilot[r].key, st, 40, 0);
		}

	if (NHenk > 1)
		quicksort((char *)Henkilot, NHenk, sizeof(Henkilot[0].key), sizeof(Henkilot[0]));
	NaytaHenkilot(L"", L"", L"", L"", 0);
}
//---------------------------------------------------------------------------

void TFormTKanta::laadiRivi(wchar_t *oBuf, int len, int r, bool kokonimi)
{
	wmemset(oBuf, 0, len);
	if (Otsikkorivinmukaan1->Checked) {
		if (r == 0) {
			for (int c = 0; c < nflds; c++) {
				for (int i = 0; i < Khenk_kentat; i++) {
					if (fldorder[c].fld == henk_kentat[i].id) {
						wcscat(oBuf, henk_kentat[i].name);
						oBuf[wcslen(oBuf)] = erotin;
						break;
						}
					}
				}
			oBuf[wcslen(oBuf)-1] = L'\n';
			}
		else {
			Henkilot[r-1].tee_HenkRivi(oBuf, len, erotin);
			}
		}
	else  {
		if (r == 0) {
			for (int c = 0; c < nFileCols; c++) {
				wcscat(oBuf, hTGridCols[FileCols[c].Order].Caption);
				if (c < nFileCols-1)
					oBuf[wcslen(oBuf)] = erotin;
				}
			}
		else {
			for (int c = 0; c < nFileCols; c++) {
				if (hTGridCols[FileCols[c].Order].Visible &&
					hTGrid->Cells[hTGridCols[FileCols[c].Order].Order][r].Length() > 0)
					wcscat(oBuf, hTGrid->Cells[hTGridCols[FileCols[c].Order].Order][r].c_str());
				if (c < nFileCols-1)
					oBuf[wcslen(oBuf)] = erotin;
				}
			}
		oBuf[wcslen(oBuf)] = L'\n';
		}
}

//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Tallennatehdytmuutokset1Click(TObject *Sender)
{
	hTGrid->Options >> goEditing;
	paivitaHenkilot();
	kirjoitaTiedostoon(0);
	NaytaHenkilot(L"", L"", L"", L"", 0);
}
//---------------------------------------------------------------------------

int TFormTKanta::TallennaMuutokset(void)
{
	paivitaHenkilot();
	kirjoitaTiedostoon(2);
	return(0);
}
//---------------------------------------------------------------------------

void TFormTKanta::kirjoitaTiedostoon(int aiempaan)
{
	bool otsrivi = false;
	TextFl *OutFile = NULL;
	int vast;
	UnicodeString FName = L"";

	if (IRMAtiedosto1->Checked) {
		otsrivi = true;
		}
	kokonimi = Sukujaetunimierikseen1->Checked == false;
	if (aiempaan) {
		vast = aiempaan;
		}
	else {
		vast = select3(3, L"Tallennetaanko muutokset aiempaan tiedostoon?", L"Kohdetiedosto",
			L"Tallenna aimpaan tehden backup", L"Tallenna samaan ilman backup-tiedostoa", L"Tallenna uudella nimellä", this);
		}
	if (vast == 3) {
		SaveDialog1->InitialDir = FormMain->CurrentDir;
		SaveDialog1->DefaultExt = L"csv";
		SaveDialog1->Filter = L"Teksti/csv-tiedosto (*.csv, *.txt)|*.csv;*.txt|Kaikki tiedostot (*.*)|*.*";
		if (SaveDialog1->Execute()) {
			FName = SaveDialog1->FileName;
			}
		}
	else {
		if (vast == 1) {
			teeBackupkopio(HenkTKantaNm);
			}
		FName = HenkTKantaNm;
		}
	if (FName.Length() > 1)
		OutFile = new TextFl(FName.c_str(), L"wt");
	if (OutFile != NULL) {
		if (OutFile->IsOpen()) {
			wchar_t Buf[1000];
			int N = otsrivi ? 1 : 0;

			for (; N <= NHenk ; N++) {
				laadiRivi(Buf, sizeof(Buf)/2, N, kokonimi);
				OutFile->WriteLine(Buf);
				}
			OutFile->Close();
			Application->MessageBoxW((UnicodeString(L"Tiedosto ") + FName +
				L" kirjoitettu. Kirjoitetut tiedot luetaan uudelleen ohjelmaan").c_str(), L"Valmis", MB_OK);
			LueHenkilot(FName.c_str(), false);
			}
		delete OutFile;
		if (ToimintaTila == 2 && comfl && maxyhteys && memnullcmp((char *)lah_tiedostot, sizeof(lah_tiedostot))) {
			if (Application->MessageBoxW(L"Lähetetäänkö tiedosto verkon muille tietokoneille?",
				L"Lähetys", MB_YESNO) == IDYES) {
				if (lah_tiedosto(FName.c_str(), 0, 2))
					Application->MessageBoxW(L"Tiedoston lähettäminen ei onnistunut.",
						L"Ei lähetetty", MB_OK);
				}
			}
		}
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Liskilpailijattietokantaan1Click(TObject *Sender)
{
	kilptietue kilp;
	int vapLisno = 1;
	wchar_t datestr[20] = L"";

	if (pvparam[k_pv].Date)
		stDateNo(datestr,  pvparam[k_pv].Date);
	else {
		stDateNo(datestr, floor(Date()));
		}
	Nytkaikkirivit1->Checked = true;
	NaytaHenkilot(L"", L"", L"", L"", 0);
	Sleep(2000);
	for (int k = 0; k < NHenk; k++) {
		if (vapLisno <= Henkilot[k].id)
			vapLisno = Henkilot[k].id + 1;
		}
	for (int d = 1; d < nrec; d++) {
		int k;
		kilp.GETREC(d);
		if (kilp.kilpstatus != 0 || !kilp.lasna() || (ilmkentat > 1 && kilp.pv[k_pv].badge[1] == 0))
			continue;
		if (kilp.lisno[0] == 0) {
			while (1) {

				for (k = 0; k < NHenk; k++) {
					if (vapLisno == Henkilot[k].id)
						break;
					}
				if (k == NHenk)
					break;
				vapLisno++;
				}
			EnterCriticalSection(&tall_CriticalSection);
			kilp.lisno[0] = vapLisno;
			kilp.tallenna(d, 0, 0, 0, 0);
			LeaveCriticalSection(&tall_CriticalSection);
//			k = hTGrid->RowCount;
//			hTGrid->RowCount++;
			k = NHenk;
			NHenk++;
			memset(&Henkilot[k], 0, sizeof(Henkilot[k]));
/*
			hTGrid->Cells[hTGridCols[0].Order][k] = UnicodeString(vapLisno++);
			hTGrid->Cells[hTGridCols[1].Order][k] = UnicodeString(kilp.sukunimi);
			hTGrid->Cells[hTGridCols[2].Order][k] = UnicodeString(kilp.etunimi);
			hTGrid->Cells[hTGridCols[3].Order][k] = UnicodeString(kilp.seura);
			hTGrid->Cells[hTGridCols[7].Order][k] = UnicodeString(Sarjat[kilp.Sarja()].sarjanimi);
			if (ilmkentat > 1 && kilp.pv[k_pv].laina[1] != 'L')
				hTGrid->Cells[hTGridCols[9].Order][k] = UnicodeString(kilp.pv[k_pv].badge[1]);
*/
			}
		else {
			for (k = 0; k < NHenk; k++) {
				if (kilp.lisno[0] == Henkilot[k].id)
					break;
				}
			if (k == NHenk) {
//				k = hTGrid->RowCount;
//				hTGrid->RowCount++;
				NHenk++;
				memset(&Henkilot[k], 0, sizeof(Henkilot[k]));
				}
/*
			hTGrid->Cells[hTGridCols[0].Order][k] = UnicodeString(kilp.lisno[0]);
			hTGrid->Cells[hTGridCols[1].Order][k] = UnicodeString(kilp.sukunimi);
			hTGrid->Cells[hTGridCols[2].Order][k] = UnicodeString(kilp.etunimi);
			hTGrid->Cells[hTGridCols[3].Order][k] = UnicodeString(kilp.seura);
			hTGrid->Cells[hTGridCols[7].Order][k] = UnicodeString(Sarjat[kilp.Sarja()].sarjanimi);
			if (ilmkentat > 1 && kilp.pv[k_pv].laina[1] != 'L')
				hTGrid->Cells[hTGridCols[9].Order][k] = UnicodeString(kilp.pv[k_pv].badge[1]);
*/
			}
		if (k < maxNHenk) {
			wchar_t st[200];

			Henkilot[k].id = kilp.lisno[0];
			wcscpy(Henkilot[k].sukunimi, kilp.sukunimi);
			wcscpy(Henkilot[k].etunimi, kilp.etunimi);
			wcscpy(Henkilot[k].seura, kilp.seura);
			wcscpy(Henkilot[k].sarja, Sarjat[kilp.Sarja()].sarjanimi);
			if (ilmkentat > 1 && kilp.pv[k_pv].laina[1] != 'L')
				Henkilot[k].badge = kilp.pv[k_pv].badge[1];
			if (kilp.tulos_pv(k_pv, false) || wcswcind(kilp.tark(), L"KHM") >= 0) {
				Henkilot[k].oolkm++;
				wcsncpy(Henkilot[k].viimoo, datestr, sizeof(Henkilot[k].viimoo)/2-1);
				}
			wcscpy(st, Henkilot[k].sukunimi);
			wcscat(st, L" ");
			wcscat(st, Henkilot[k].etunimi);
			keyfromwname(Henkilot[k].key, st, 40, 0);
			}
		}
	if (NHenk > 1)
		quicksort((char *)Henkilot, NHenk, sizeof(Henkilot[0].key), sizeof(Henkilot[0]));
	kirjoitaTiedostoon(0);
	NaytaHenkilot(L"", L"", L"", L"", 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::FormClose(TObject *Sender, TCloseAction &Action)
{
	NimiHTHaku = -2;
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::hTGridMouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (hTGrid->RowCount - hTGrid->FixedRows >= hTGrid->VisibleRowCount + hTGrid->TopRow)
		hTGrid->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::hTGridMouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (hTGrid->TopRow > hTGrid->FixedRows)
		hTGrid->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::hTGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
//	aktHenk = ensHenk + hTGrid->Row - 1;
	aktHenk = rowHenk[hTGrid->Row];
	if (Caller == NULL)
		return;
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)
		&& hTGrid->Row > 0) {
		if (Caller == FormIlm2) {
			hTGrid->Options >> goEditing;
			FormIlm2->PalaaHenkHausta(true);
			}
		if (Caller == FormEmit) {
			hTGrid->Options >> goEditing;
			FormEmit->PalaaHenkHausta(true);
			}
		return;
		}
	if (Key == VK_ESCAPE) {
		if (Caller == FormIlm2) {
			hTGrid->Options >> goEditing;
			FormIlm2->PalaaHenkHausta(false);
			}
		if (Caller == FormEmit) {
			hTGrid->Options >> goEditing;
			FormEmit->PalaaHenkHausta(false);
			}
		return;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::hTGridDblClick(TObject *Sender)
{
//	aktHenk = ensHenk + hTGrid->Row - 1;
	aktHenk = rowHenk[hTGrid->Row];
	if (Caller == NULL)
		return;
	if (Caller == FormIlm2) {
		hTGrid->Options >> goEditing;
		FormIlm2->PalaaHenkHausta(true);
		}
	if (Caller == FormEmit) {
		hTGrid->Options >> goEditing;
		FormEmit->PalaaHenkHausta(true);
		}
	return;
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Osanottolaskuri1Click(TObject *Sender)
{
	Osanottolaskuri1->Checked = !Osanottolaskuri1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Nytlisvalinnat1Click(TObject *Sender)
{
	Nytlisvalinnat1->Checked = !Nytlisvalinnat1->Checked;
	Asetukset1->Visible = Nytlisvalinnat1->Checked;
/*
	Sallimuokkaustaulukossa1->Visible = Nytlisvalinnat1->Checked;
	Tallennatehdytmuutokset1->Visible = Nytlisvalinnat1->Checked;
	Peruutamuutokset1->Visible = Nytlisvalinnat1->Checked;
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormTKanta::Lisatieto1Click(TObject *Sender)
{
	Lisatieto1->Checked = !Lisatieto1->Checked;
	NaytaHenkilot(NULL, NULL, NULL, NULL, 0);
}
//---------------------------------------------------------------------------

