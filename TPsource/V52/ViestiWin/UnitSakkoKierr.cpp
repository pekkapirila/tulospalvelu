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
	int r = 0, av;
	EnterCriticalSection(&sakkoKierrCriticalSection);
	for (int kno = minkilpno; kno <= maxkilpno; kno++) {
		int d;
		av = 0;
		d = getpos(kno);
		if (d <= 0)
			continue;
		for (int os = 0; os < kilpparam.osuusluku; os++)
			av += Avoimet[d][os];
		if (av) {
			wchar_t st[40];
			kilptietue kilp;

			kilp.getrec(d);
			for (int os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
				if (Avoimet[d][os]) {
					if (SGAvoimet->RowCount < r+1)
						SGAvoimet->RowCount = r+1;
					SGAvoimet->Cells[0][r] = kno;
					SGAvoimet->Cells[1][r] = os + 1;
					SGAvoimet->Cells[2][r] = kilp.Joukkue(st, 30, false);
					SGAvoimet->Cells[3][r] = Avoimet[d][os];
					r++;
					}
				}
			}
		}
	if (r > 0)
		SGAvoimet->RowCount = r;
	else {
		SGAvoimet->RowCount = 1;
		SGAvoimet->Cells[0][r] = L"";
		SGAvoimet->Cells[1][r] = L"";
		SGAvoimet->Cells[2][r] = L"";
		SGAvoimet->Cells[3][r] = L"";
		}
	memcpy(EdAvoimet, Avoimet, (maxrec+1)*sizeof(Avoimet[0]));
	LeaveCriticalSection(&sakkoKierrCriticalSection);
}
//---------------------------------------------------------------------------
void TFormSakkoKierr::LaskeAvoimet(void)
{
	if (inLopetus)
		return;
	EnterCriticalSection(&tall_CriticalSection);
	for (int d = 1; d < datf2.numrec && haeAvoimetThreadOn && FormSakkoKierr && FormSakkoKierr->Visible; d++) {
		kilptietue kilp;
		kilp.getrec(d);
		if (kilp.kilpstatus != 0)
			continue;
		for (int os = 0; os < Sarjat[kilp.sarja].osuusluku; os++)
			Avoimet[d][os] = kilp.ASakkoLkm(os) - kilp.ostiet[os].ampsakot[LSAKOT];
		}
	LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------
void TFormSakkoKierr::LaskeKierrokset(void)
{
/*
	memset(Kierretyt, 0, (maxrec+1)*sizeof(Kierretyt[0]));
	for (int r = 0; r < LBKirjatut->Count; r++) {
		int d, kno, os;
		wchar_t knost[6];
		if (LBKirjatut->Items->Strings[r].Length() > 15) {
			kno = _wtoi(LBKirjatut->Items->Strings[r].SubString(10,4).c_str());
			os = _wtoi(LBKirjatut->Items->Strings[r].SubString(15,2).c_str()) - 1;
			if ((d = getpos(kno)) > 0)
				Kierretyt[d][os]++;
			}
		}
*/
}
//---------------------------------------------------------------------------
void __fastcall TFormSakkoKierr::EdtKirjausKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		Key = 0;
		FocusControl(EdtOsuus);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSakkoKierr::EdtOsuusKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == L'\r') {
		int kno, d, os;
		Key = 0;
		kno = _wtoi(EdtKirjaus->Text.c_str());
		os = _wtoi(EdtOsuus->Text.c_str()) - 1;
		if ((d = getpos(kno)) > 0) {
			kilptietue kilp;
			wchar_t ln[80], st[40];

			EnterCriticalSection(&tall_CriticalSection);
			kilp.getrec(d);
			swprintf(ln, L"%s %04d-%2d %-28.28s", wkello(), kno, os+1, kilp.Joukkue(st, 26, false));
			LBKirjatut->Items->Add(ln);
			LBKirjatut->ItemIndex = LBKirjatut->Count-1;
			wcscat(ln, L"\n");
			if (KierrFile)
				KierrFile->WriteLine(ln);
//			Kierretyt[d][os]++;
			kilp.ostiet[os].ampsakot[LSAKOT]++;
			kilp.Tallenna(d, 0, 0, 0, 0);
			LeaveCriticalSection(&tall_CriticalSection);
			LaskeAvoimet();
			if (memcmp(Avoimet, EdAvoimet, (maxrec+1)*sizeof(Avoimet[0]))) {
				PaivitaAvoimet();
				}
			EdtKirjaus->Text = L"";
			EdtOsuus->Text = L"";
			FocusControl(EdtKirjaus);
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSakkoKierr::FormCreate(TObject *Sender)
{
	wcscpy(KierrFlName, L"SakkoKierr.txt");
//	Kierretyt = new intA_OSLUKU[maxrec+1];
	Avoimet = new intA_OSLUKU[maxrec+1];
	EdAvoimet = new intA_OSLUKU[maxrec+1];
//	memset(Kierretyt, 0, (maxrec+1)*sizeof(Kierretyt[0]));
	memset(Avoimet, 0, (maxrec+1)*sizeof(Avoimet[0]));
	memset(EdAvoimet, 0, (maxrec+1)*sizeof(EdAvoimet[0]));
	SGAvoimet->ColWidths[0] = 40 * Screen->PixelsPerInch / 96;
	SGAvoimet->ColWidths[1] = 30 * Screen->PixelsPerInch / 96;
	SGAvoimet->ColWidths[2] = 150 * Screen->PixelsPerInch / 96;
	SGAvoimet->ColWidths[3] = 30 * Screen->PixelsPerInch / 96;
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
	SGAvoimet->Width = ClientWidth - SGAvoimet->Left - 6 * Screen->PixelsPerInch / 96;
	SGAvoimet->ColWidths[2] = SGAvoimet->Width - 125 * Screen->PixelsPerInch / 96;
	BtnPeruuta->Top = ClientHeight - BtnPeruuta->Height - 6 * Screen->PixelsPerInch / 96;
	EdtKirjaus->Top = BtnPeruuta->Top;
	EdtOsuus->Top = BtnPeruuta->Top;
	Label3->Top = EdtKirjaus->Top - 17 * Screen->PixelsPerInch / 96;
	LBKirjatut->Height = Label3->Top - LBKirjatut->Top - 6 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------

void __fastcall TFormSakkoKierr::BtnPeruutaClick(TObject *Sender)
{
	if (LBKirjatut->Items->Strings[LBKirjatut->ItemIndex].Length() > 15) {
		int kno, d, os;
		kno = _wtoi(LBKirjatut->Items->Strings[LBKirjatut->ItemIndex].SubString(10,4).c_str());
		os = _wtoi(LBKirjatut->Items->Strings[LBKirjatut->ItemIndex].SubString(15,2).c_str()) - 1;
		if ((d = getpos(kno)) > 0) {
			kilptietue kilp;
			EnterCriticalSection(&tall_CriticalSection);
			kilp.getrec(d);
			kilp.ostiet[os].ampsakot[LSAKOT]--;
			kilp.Tallenna(d, 0, 0, 0, 0);
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
	if (memcmp(Avoimet, EdAvoimet, (maxrec+1)*sizeof(Avoimet[0]))) {
		PaivitaAvoimet();
		}
}
//---------------------------------------------------------------------------

void HaeAvoimetTh(LPVOID lpCn)
	{
	while (taustaon && haeAvoimetThreadOn) {
		Sleep(1000);
		SendMessage(FormSakkoKierr->Handle, WM_MYHAEAVOIMET, 0, 0);
		}
}
//---------------------------------------------------------------------------


