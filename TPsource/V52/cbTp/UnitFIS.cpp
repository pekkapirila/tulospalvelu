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

#include "UnitFIS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormFIS *FormFIS;
//---------------------------------------------------------------------------
__fastcall TFormFIS::TFormFIS(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
#ifdef MAXOSUUSLUKU
	ClientWidth = BtnSulje->Left + BtnSulje->Width + 8;
	ClientHeight = BtnSulje->Top + BtnSulje->Height + 8;
	GroupBox3->Visible = false;
	Button1->Visible = false;
	Memo1->Visible = false;
#endif
}
//---------------------------------------------------------------------------
void __fastcall TFormFIS::LueKilpailijat(TextFl *InFileCom)
{
	int n = 0;
	while (!InFileCom->Feof()) {
		wchar_t line[200];

		if (InFileCom->ReadLine(line, sizeof(line)/2-1) != NULL && wcslen(line) > 10 && _wtoi(line) > 0)
			n++;
		}
	ncmptr = 0;
	InFileCom->Rewind();
	if (cmptr != NULL)
		delete[] cmptr;
	cmptr = new cmptrtp[n+1];
	while (!InFileCom->Feof()) {
		wchar_t *p, line[200];

		memset(&cmptr[ncmptr], 0, sizeof(cmptr[0]));
		if (InFileCom->ReadLine(line, sizeof(line)/2-1) != NULL && wcslen(line) > 10 && _wtoi(line) > 0) {
			if ((p = wcstokch(line, L'\t')) == NULL)
				continue;
			cmptr[ncmptr].id = _wtoi(p);
			if ((p = wcstokch(NULL, L'\t')) == NULL)
				continue;
			if ((p = wcstokch(NULL, L'\t')) == NULL)
				continue;
			cmptr[ncmptr].code = _wtoi(p);
			if ((p = wcstokch(NULL, L'\t')) == NULL)
				continue;
			wcsncpy(cmptr[ncmptr].sukunimi, p, LSNIMI);
			if ((p = wcstokch(NULL, L'\t')) == NULL)
				continue;
			wcsncpy(cmptr[ncmptr].etunimi, p, LSNIMI);
			if ((p = wcstokch(NULL, L'\t')) == NULL)
				continue;
			if ((p = wcstokch(NULL, L'\t')) == NULL)
				continue;
			cmptr[ncmptr].synt = _wtoi(p);
			if ((p = wcstokch(NULL, L'\t')) == NULL)
				continue;
			wcsncpy(cmptr[ncmptr].maa, p, LSNIMI);
			cmptr[ncmptr].pts = 9999999;
			ncmptr++;
			}
		}
}
//---------------------------------------------------------------------------
int __fastcall TFormFIS::HaeId(int id)
{
	for (int i = 0; i < ncmptr; i++) {
		if (id == cmptr[i].id) {
			return(i);
			}
		}
	return(-1);
}
//---------------------------------------------------------------------------
int __fastcall TFormFIS::HaePisteet(int code)
{
	for (int i = 0; code != 0 && i < ncmptr; i++) {
		if (code == cmptr[i].code) {
			return(cmptr[i].pts);
			}
		}
	return(9999999);
}
//---------------------------------------------------------------------------
int __fastcall TFormFIS::HaeCode(int code)
{
	for (int i = 0; code != 0 && i < ncmptr; i++) {
		if (code == cmptr[i].code) {
			return(i);
			}
		}
	return(-1);
}
//---------------------------------------------------------------------------
void __fastcall TFormFIS::BtnLuePisteetClick(TObject *Sender)
{
	wchar_t laji[3];

	Label1->Visible = false;
	switch (RGLaji->ItemIndex) {
		case 0:
			wcscpy(laji, L"DI");
			break;
		case 1:
			wcscpy(laji, L"SP");
			break;
		default:
			Application->MessageBoxW(L"Valitse laji", L"Valitse", MB_OK);
			return;
		}
	OpenDialog1->DefaultExt = L"csv";
	OpenDialog1->Filter = L"Csv-tiedostot (*.csv)|*.CSV|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		wchar_t *p, fnamepts[200] = L"", fnamecom[200];
		TextFl *InFilePts = NULL, *InFileCom = NULL;

		wcsncpy(fnamepts, OpenDialog1->FileName.c_str(), sizeof(fnamepts)/2-1);
		if (!wcscmpU(fnamepts+wcslen(fnamepts)-7, L"pts.csv")) {
			wcscpy(fnamecom, fnamepts);
			wcscpy(fnamecom+wcslen(fnamecom)-7, L"com.csv");
			InFilePts = new TextFl(fnamepts, L"rt");
			if (InFilePts && InFilePts->IsOpen())
				InFileCom = new TextFl(fnamecom, L"rt");
			if (!InFileCom || !InFileCom->IsOpen()) {
				Application->MessageBoxW(
					(UnicodeString(L"Ohjelma ei onnistunut avaamaan molempia tarvittavia tiedostoja: ")+fnamepts+
					L" ja "+fnamecom).c_str(), L"Este", MB_OK);
				}
			else {
				LueKilpailijat(InFileCom);
				while (!InFilePts->Feof()) {
					wchar_t *p1, line[200];
					int id, kk, pts = 0;

					if (InFilePts->ReadLine(line, sizeof(line)/2-1) != NULL && wcslen(line) > 10 && _wtoi(line) > 0) {
						if ((p = wcstokch(line, L'\t')) == NULL)
							continue;
						if ((p = wcstokch(NULL, L'\t')) == NULL)
							continue;
						if ((p = wcstokch(NULL, L'\t')) == NULL)
							continue;
						id = _wtoi(p);
						kk = HaeId(id);
						if (kk < 0)
							continue;
						if ((p = wcstokch(NULL, L'\t')) == NULL)
							continue;
						if (wcscmp(laji, p))
							continue;
						if ((p = wcstokch(NULL, L'\t')) == NULL ||
							*p < L'0' || *p > L'9') {
							continue;
							}
						for (p1 = p; *p1 != 0; p1++)
							if (*p1 == L',')
								*p1 = L'.';
						cmptr[kk].pts = (int) floor(1000 * _wtof(p) + 0.5);
						}
					}
				EnterCriticalSection(&tall_CriticalSection);
#ifndef MAXOSUUSLUKU
				for (int d = 1; d < nrec; d++) {
					kilptietue kilp;

					kilp.GETREC(d);
					if (kilp.kilpstatus != 0)
						continue;
					kilp.pisteet[2] = HaePisteet(kilp.lisno[1]);
					kilp.tallenna(d, 0, 0, 0, 0);
					}
#else
				for (int d = 1; d < datf2.numrec; d++) {
					kilptietue kilp;

					kilp.getrec(d);
					if (kilp.kilpstatus != 0)
						continue;
					for (int os = 0; os < Sarjat[kilp.sarja].osuusluku; os++)
						kilp.ostiet[os].pisteet[2] = HaePisteet(kilp.ostiet[os].pisteet[0]);
					kilp.Tallenna(d, 0, 0, 0, 0);
					}
#endif
				LeaveCriticalSection(&tall_CriticalSection);
				Label1->Visible = true;
				}
			}
		if (InFilePts)
			delete InFilePts;
		if (InFileCom)
			delete InFileCom;
		}
	if (cmptr != NULL)
		delete[] cmptr;
	cmptr = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TFormFIS::BtnLueNimetClick(TObject *Sender)
{
	Label2->Visible = false;
	OpenDialog1->DefaultExt = L"csv";
	OpenDialog1->Filter = L"Csv-tiedostot (*.csv)|*.CSV|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		wchar_t *p, fnamecom[200];
		TextFl *InFileCom = NULL;

		wcsncpy(fnamecom, OpenDialog1->FileName.c_str(), sizeof(fnamecom)/2-1);
		InFileCom = new TextFl(fnamecom, L"rt");
		if (!InFileCom || !InFileCom->IsOpen()) {
			Application->MessageBoxW(
				(UnicodeString(L"Ohjelma ei onnistunut avaamaan tarvittavaa tiedostoa: ")+
				fnamecom).c_str(), L"Este", MB_OK);
				}
		else {
			LueKilpailijat(InFileCom);
			EnterCriticalSection(&tall_CriticalSection);
#ifndef MAXOSUUSLUKU
			for (int d = 1; d < nrec; d++) {
				kilptietue kilp;
				int kk;

				kilp.GETREC(d);
				if (kilp.kilpstatus != 0)
					continue;
				if ((kk = HaeCode(kilp.lisno[1])) < 0)
					continue;
				wcsncpy(kilp.sukunimi, cmptr[kk].sukunimi, kilpparam.lsnimi);
				wcsncpy(kilp.etunimi, cmptr[kk].etunimi, kilpparam.lenimi);
				wcsncpy(kilp.maa, cmptr[kk].maa, 3);
				kilp.tallenna(d, 0, 0, 0, 0);
				}
#else
			for (int d = 1; d < datf2.numrec; d++) {
				kilptietue kilp;

				kilp.getrec(d);
				if (kilp.kilpstatus != 0)
					continue;
				for (int os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
					int kk;
					if ((kk = HaeCode(kilp.ostiet[os].pisteet[0])) < 0)
						continue;
					kilp.setNimi(cmptr[kk].etunimi, cmptr[kk].sukunimi, os);
					}
				kilp.Tallenna(d, 0, 0, 0, 0);
				}
#endif
			LeaveCriticalSection(&tall_CriticalSection);
			Label2->Visible = true;
			}
		if (InFileCom)
			delete InFileCom;
		}
	if (cmptr != NULL)
		delete[] cmptr;
	cmptr = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TFormFIS::FormShow(TObject *Sender)
{
	Label1->Visible = false;
	Label2->Visible = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormFIS::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"FIS-kilpailut");
}
//---------------------------------------------------------------------------
void __fastcall TFormFIS::BtnSuljeClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormFIS::BtnLueClick(TObject *Sender)
{
	wchar_t *tags[] = {L"FVALUE", L"MINPENALTY", L"FISLIST", L"SEASON",
		L"CODEX", L"NATION", L"DISCIPLINE", L"CATEGORY", L"DATE",
		L"EVENTNAME", L"PLACE", L"TDIDCODE", L"TDLASTNAME", L"TDFIRSTNAME",
		L"TDNATION"};
	wchar_t *laji[] = {L"FIS", L"JUN", L"NC", L"NJC", L"SCAN", L"CISM", L"ML"};

	TextFl *FISmaarfl;
	OpenDialog1->DefaultExt = L"txt";
	OpenDialog1->Filter = L"Txt-tiedostot (*.txt)|*.TXT|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		if ((FISmaarfl = new TextFl(OpenDialog1->FileName.c_str(), L"r")) != NULL) {
			Memo1->Clear();
			while (FISmaarfl->IsOpen() && !FISmaarfl->Feof()) {
				wchar_t ln[200], *p, *p1;
				int k;
				FISmaarfl->ReadLine(ln, 198);
				if (wcslen(ln) > 6) {
					Memo1->Lines->Add(ln);
					p = wcstok(ln, L" =");
					p1 = NULL;
					if (p)
						p1 = wcstok(NULL, L" \n");
					if (p1 == NULL)
						continue;
					for (UINT i = 0; i < sizeof(tags)/sizeof(tags[0]); i++) {
						if (wcscmpU(tags[i], p) == 0) {
							switch (i) {
								case 0:
									if ((k = _wtoi(p1)) == 0)
										CBFvalue->ItemIndex = 3;
									else if ((k = _wtoi(p1)) == 1400)
										CBFvalue->ItemIndex = 2;
									else if ((k = _wtoi(p1)) == 1200)
										CBFvalue->ItemIndex = 1;
									else
										CBFvalue->ItemIndex = 0;
									break;
								case 1:
									if ((k = _wtoi(p1)) == 35)
										CBminpist->ItemIndex = 2;
									else if ((k = _wtoi(p1)) == 25)
										CBminpist->ItemIndex = 1;
									else
										CBminpist->ItemIndex = 0;
									break;
								case 2:
									EdtLista->Text = p1;
									break;
								case 3:
									EdtKausi->Text = p1;
									break;
								case 4:
									EdtKoodi->Text = p1;
									break;
								case 5:
									EdtMaa->Text = p1;
									break;
								case 6:
									EdtLaji->Text = p1;
									break;
								case 7:
									for (UINT j = 0; j < sizeof(laji)/sizeof(laji[0]); j++)
										if (wcscmpU(laji[j], p1) == 0) {
											CBcategory->ItemIndex = j;
											break;
											}
									 break;
								case 8:
									EdtPv->Text = p1;
									break;
								case 9:
									EdtNimi->Text = p1;
									break;
								case 10:
									EdtPaikka->Text = p1;
									break;
								case 11:
									EdtTDid->Text = p1;
									break;
								case 12:
									EdtTDsnimi->Text = p1;
									break;
								case 13:
									EdtTDenimi->Text = p1;
									break;
								case 14:
									EdtTDmaa->Text = p1;
									break;
								}
							break;
							}
						}
					}
				}
			delete FISmaarfl;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormFIS::BtnKirjClick(TObject *Sender)
{
	wchar_t *tags[] = {L"Fvalue", L"Minpenalty", L"Fislist", L"Season",
		L"Codex", L"Nation", L"Discipline", L"Category", L"Date",
		L"Eventname", L"Place", L"TDidcode", L"TDlastname", L"TDfirstname",
		L"TDnation"};
	wchar_t *laji[] = {L"FIS", L"JUN", L"NC", L"NJC", L"SCAN", L"CISM", L"ML"};
	int Fvals[] = {800, 1200, 1400, 0};
	int Mvals[] = {15, 25, 35, 35, 35, 0, 25, 35};

	TextFl *FISmaarfl;
	SaveDialog1->DefaultExt = L"txt";
	SaveDialog1->Filter = L"Txt-tiedostot (*.txt)|*.TXT|Kaikki tiedostot (*.*)|*.*";
	if (SaveDialog1->Execute()) {
		if ((FISmaarfl = new TextFl(SaveDialog1->FileName.c_str(), L"wt")) != NULL) {
			Memo1->Clear();
			for (int i = 0; i < 15; i++) {
				wchar_t ln[100];
				switch (i) {
					case 0:
						swprintf(ln, L"%s=%d",tags[i], Fvals[CBFvalue->ItemIndex]);
						break;
					case 1:
						swprintf(ln, L"%s=%d",tags[i], Mvals[CBminpist->ItemIndex]);
						if (CBminpist->ItemIndex > 4)
							wmemcpy(ln, L"Max", 3);
						break;
					case 2:
						swprintf(ln, L"%s=%d",tags[i], _wtoi(EdtLista->Text.c_str()));
						break;
					case 3:
						swprintf(ln, L"%s=%d",tags[i], _wtoi(EdtKausi->Text.c_str()));
						break;
					case 4:
						swprintf(ln, L"%s=%d",tags[i], _wtoi(EdtKoodi->Text.c_str()));
						break;
					case 5:
						swprintf(ln, L"%s=%s",tags[i], EdtMaa->Text.c_str());
						break;
					case 6:
						swprintf(ln, L"%s=%s",tags[i], EdtLaji->Text.c_str());
						break;
					case 7:
						swprintf(ln, L"%s=%s",tags[i], laji[CBcategory->ItemIndex]);
						 break;
					case 8:
						swprintf(ln, L"%s=%s",tags[i], EdtPv->Text.c_str());
						break;
					case 9:
						swprintf(ln, L"%s=%s",tags[i], EdtNimi->Text.c_str());
						break;
					case 10:
						swprintf(ln, L"%s=%s",tags[i], EdtPaikka->Text.c_str());
						break;
					case 11:
						swprintf(ln, L"%s=%d",tags[i], _wtoi(EdtMaa->Text.c_str()));
						break;
					case 12:
						swprintf(ln, L"%s=%s",tags[i], EdtTDsnimi->Text.c_str());
						break;
					case 13:
						swprintf(ln, L"%s=%s",tags[i], EdtTDenimi->Text.c_str());
						break;
					case 14:
						swprintf(ln, L"%s=%s",tags[i], EdtTDmaa->Text.c_str());
						break;
					case 15:
						wcscpy(ln, L"Variable");
						break;
					}
				Memo1->Lines->Add(ln);
				wcscat(ln, L"\n");
				FISmaarfl->WriteLine(ln);
				}
			delete FISmaarfl;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormFIS::Button1Click(TObject *Sender)
{
	TextFl *FISmaarfl;
	SaveDialog1->DefaultExt = L"txt";
	SaveDialog1->Filter = L"Txt-tiedostot (*.txt)|*.TXT|Kaikki tiedostot (*.*)|*.*";
	if (SaveDialog1->Execute()) {
		if ((FISmaarfl = new TextFl(SaveDialog1->FileName.c_str(), L"w")) != NULL) {
			for (int i = 0; i < Memo1->Lines->Count; i++) {
				if (Memo1->Lines->Strings[i].Length() > 2)
					FISmaarfl->WriteLine((Memo1->Lines->Strings[i]+L"\n").c_str());
				}
			delete FISmaarfl;
			}
		}
}
//---------------------------------------------------------------------------

