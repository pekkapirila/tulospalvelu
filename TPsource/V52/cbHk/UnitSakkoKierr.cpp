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
#include <process.h>

#include "UnitSakkoKierr.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSakkoKierr *FormSakkoKierr;

bool haeAvoimetThreadOn = false;
void HaeAvoimetTh(LPVOID lpCn);
CRITICAL_SECTION sakkoKierrCriticalSection;

//---------------------------------------------------------------------------
__fastcall TFormSakkoKierr::TFormSakkoKierr(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormSakkoKierr::Poistakaikkimerkinnt1Click(TObject *Sender)
{
	LBKirjatut->Clear();
	if (KierrFile) {
		KierrFile->Close();
		KierrFile->Open(L"wt");
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSakkoKierr::Vaihdatiedosto1Click(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TFormSakkoKierr::Suljekaavake1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void TFormSakkoKierr::PaivitaAvoimet(void)
{
	int r = 0;
	int d;
	EnterCriticalSection(&sakkoKierrCriticalSection);
	EnterCriticalSection(&idindex_CriticalSection);
//	for (int kno = minkilpno; kno <= maxkilpno; kno++) {
	d = getpos(minkilpno, true);
	while (d > 0) {
		if (Avoimet[d]) {
			wchar_t st[40];
			kilptietue kilp;

			kilp.GETREC(d);
			if (SGAvoimet->RowCount < r+1)
				SGAvoimet->RowCount = r+1;
			SGAvoimet->Cells[0][r] = kilp.id();
			SGAvoimet->Cells[1][r] = kilp.nimi(st, 30, false);
			SGAvoimet->Cells[2][r] = Avoimet[d];
			r++;
			}
		d = nextpos();
		}
	LeaveCriticalSection(&idindex_CriticalSection);
	if (r > 0)
		SGAvoimet->RowCount = r;
	else {
		SGAvoimet->RowCount = 1;
		SGAvoimet->Cells[0][r] = L"";
		SGAvoimet->Cells[1][r] = L"";
		SGAvoimet->Cells[2][r] = L"";
		}
	memcpy(EdAvoimet, Avoimet, (maxrec+1)*sizeof(int));
	LeaveCriticalSection(&sakkoKierrCriticalSection);
}
//---------------------------------------------------------------------------
void TFormSakkoKierr::LaskeAvoimet(void)
{
	EnterCriticalSection(&tall_CriticalSection);
	for (int d = 1; d < nrec && haeAvoimetThreadOn && FormSakkoKierr && FormSakkoKierr->Visible; d++) {
		kilptietue kilp;
		kilp.GETREC(d);
		if (kilp.kilpstatus != 0)
			continue;
		Avoimet[d] = kilp.asakkolkm() - Kierretyt[d];
		}
	LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------
void TFormSakkoKierr::LaskeKierrokset(void)
{
/*
	memset(Kierretyt, 0, (maxrec+1)*sizeof(int));
	for (int r = 0; r < LBKirjatut->Count; r++) {
		int d, kno;
		wchar_t knost[6];
		if (LBKirjatut->Items->Strings[r].Length() > 15) {
			kno = _wtoi(LBKirjatut->Items->Strings[r].SubString(10,4).c_str());
			if ((d = getpos(kno)) > 0)
				Kierretyt[d]++;
			}
		}
*/
}
//---------------------------------------------------------------------------
void __fastcall TFormSakkoKierr::EdtKirjausKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == L'\r') {
		int kno, d;
		Key = 0;
		kno = _wtoi(EdtKirjaus->Text.c_str());
		if ((d = getpos(kno)) > 0) {
			kilptietue kilp;
			wchar_t ln[80], st[40];

			EnterCriticalSection(&tall_CriticalSection);
			kilp.GETREC(d);
			kilp.pv[k_pv].asakot[LSAKKO]++;
			kilp.tallenna(d, 0, 0, 0, 0);
			LeaveCriticalSection(&tall_CriticalSection);
			swprintf(ln, L"%s %04d %-30.30s", wkello(), kno, kilp.nimi(st, 30, false));
			LBKirjatut->Items->Add(ln);
			LBKirjatut->ItemIndex = LBKirjatut->Count-1;
			wcscat(ln, L"\n");
			if (KierrFile)
				KierrFile->WriteLine(ln);
//			Kierretyt[d]++;
			LaskeAvoimet();
			if (memcmp(Avoimet, EdAvoimet, (maxrec+1)*sizeof(int))) {
				PaivitaAvoimet();
				}
			EdtKirjaus->Text = L"";
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSakkoKierr::FormCreate(TObject *Sender)
{
	wcscpy(KierrFlName, L"SakkoKierr.txt");
//	Kierretyt = new int[maxrec+1];
	Avoimet = new int[maxrec+1];
	EdAvoimet = new int[maxrec+1];
//	memset(Kierretyt, 0, (maxrec+1)*sizeof(int));
	memset(Avoimet, 0, (maxrec+1)*sizeof(int));
	memset(EdAvoimet, 0, (maxrec+1)*sizeof(int));
	SGAvoimet->ColWidths[0] = 40 * Screen->PixelsPerInch / 96;
	SGAvoimet->ColWidths[1] = 140 * Screen->PixelsPerInch / 96;
	SGAvoimet->ColWidths[2] = 40 * Screen->PixelsPerInch / 96;
	haeAvoimetThreadOn = true;
	_beginthread(HaeAvoimetTh, 8192, 0);
	InitializeCriticalSection(&sakkoKierrCriticalSection);
}
//---------------------------------------------------------------------------
void __fastcall TFormSakkoKierr::FormDestroy(TObject *Sender)
{
	haeAvoimetThreadOn = false;
	Sleep(1500);
	if (KierrFile)
		delete KierrFile;
	DeleteCriticalSection(&sakkoKierrCriticalSection);
	delete[] EdAvoimet;
	delete[] Avoimet;
//	delete[] Kierretyt;
}
//---------------------------------------------------------------------------
void __fastcall TFormSakkoKierr::FormShow(TObject *Sender)
{
	if (!KierrFile) {
		KierrFile = new TextFl(KierrFlName, L"rt", L'8');
		if (KierrFile) {
			if (KierrFile->IsOpen()) {
				while (!KierrFile->Feof()) {
					wchar_t line[200];
					KierrFile->ReadLine(line, 198);
					wtrimline(line);
					if (wcslen(line) > 15)
						LBKirjatut->Items->Add(line);
					}
				}
			KierrFile->Close();
			KierrFile->Open(L"at");
			}
		}
	LaskeKierrokset();
	LaskeAvoimet();
}
//---------------------------------------------------------------------------
void __fastcall TFormSakkoKierr::FormResize(TObject *Sender)
{
	SGAvoimet->Height = ClientHeight - SGAvoimet->Top - 6 * Screen->PixelsPerInch / 96;
	BtnPeruuta->Top = ClientHeight - BtnPeruuta->Height - 6 * Screen->PixelsPerInch / 96;
	EdtKirjaus->Top = BtnPeruuta->Top;
	Label3->Top = EdtKirjaus->Top - 17 * Screen->PixelsPerInch / 96;
	LBKirjatut->Height = Label3->Top - LBKirjatut->Top - 6 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------

void __fastcall TFormSakkoKierr::BtnPeruutaClick(TObject *Sender)
{
	if (LBKirjatut->Items->Strings[LBKirjatut->ItemIndex].Length() > 15) {
		int kno, d;
		kno = _wtoi(LBKirjatut->Items->Strings[LBKirjatut->ItemIndex].SubString(10, 4).c_str());
		if ((d = getpos(kno)) > 0) {
			kilptietue kilp;
			EnterCriticalSection(&tall_CriticalSection);
			kilp.GETREC(d);
			kilp.pv[k_pv].asakot[LSAKKO]++;
			kilp.tallenna(d, 0, 0, 0, 0);
			LeaveCriticalSection(&tall_CriticalSection);
			}
		LBKirjatut->Items->Delete(LBKirjatut->ItemIndex);
		}
	if (KierrFile) {
		KierrFile->Close();
		KierrFile->Open(L"wt");
		for (int r = 0; r < LBKirjatut->Count; r++)
			if (LBKirjatut->Items->Strings[r].Length() > 15)
				KierrFile->WriteLine((LBKirjatut->Items->Strings[r]+L"\n").c_str());
		}
	LaskeKierrokset();
	LaskeAvoimet();
}
//---------------------------------------------------------------------------

MESSAGE void __fastcall TFormSakkoKierr::HaeAvoimetHandler(TMyMessage &msg)
{
	if (!FormSakkoKierr->Visible)
		return;
	LaskeAvoimet();
	if (memcmp(Avoimet, EdAvoimet, (maxrec+1)*sizeof(int))) {
		PaivitaAvoimet();
		}
}
//---------------------------------------------------------------------------

void HaeAvoimetTh(LPVOID lpCn)
	{
	while (taustaon && !vaiheenvaihto && haeAvoimetThreadOn) {
		Sleep(1000);
		SendMessage(FormSakkoKierr->Handle, WM_MYHAEAVOIMET, 0, 0);
		}
}
//---------------------------------------------------------------------------

