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

#include "UnitJakelu.h"
#include "UnitAutoFile.h"
#include "WinHk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAutoFile *FormAutoFile;

//extern wchar_t af_xmlva;

//---------------------------------------------------------------------------
__fastcall TFormAutoFile::TFormAutoFile(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormAutoFile::FormShow(TObject *Sender)
{
	if (autofileparam.afvali == 0)
		RGLaji->ItemIndex = 0;
	else {
		switch (aftulparam.kohde) {
			case L'H':
				RGLaji->ItemIndex = 1;
				break;
			case L'M':
				RGLaji->ItemIndex = 2;
				break;
			case L'X':
				RGLaji->ItemIndex = 3;
				break;
			default:
				RGLaji->ItemIndex = 0;
			}
		}
	EdtVali->Text = autofileparam.afvali/SEK;
	EdtHtmlFName->Text = autofileparam.afname;
	EdtPageTitle->Text = autofileparam.wtitlea;
	EdtPageHeader->Text = autofileparam.wheadera;
	RG1html->ItemIndex = aftulparam.yksihtml ? 0 : 1;
	RGVaiheet->ItemIndex = aftulparam.vaihelkm == 2 ? 1 : 0;
	RGva->ItemIndex = wcswcind(autofileparam.af_xmlva, L"IEM");
	RGJatko->ItemIndex = autofileparam.autokomentolaji;
	EdtKomento->Text = autofileparam.autokomento;
	CBVaadiMuutos->Checked = autofileparam.vainMuutt;
	BtnAvaaAFlstClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormAutoFile::BtnOtakayttoonClick(TObject *Sender)
{
	switch (RGLaji->ItemIndex) {
		case 0:
			autofileparam.afvali = 0;
			break;
		case 1:
			autofileparam.afvali = _wtoi(EdtVali->Text.c_str()) * SEK;
			wcsncpy(autofileparam.afname, EdtHtmlFName->Text.c_str(), sizeof(autofileparam.afname)/2-1);
			wcsncpy(autofileparam.wtitlea, EdtPageTitle->Text.c_str(), sizeof(autofileparam.wtitlea)/2-1);
			wcsncpy(autofileparam.wheadera, EdtPageHeader->Text.c_str(), sizeof(autofileparam.wheadera)/2-1);
			aftulparam.vaihelkm = RGVaiheet->ItemIndex + 1;
			aftulparam.yksihtml = RG1html->ItemIndex == 0;
			aftulparam.kohde = L'H';
			break;
		case 2:
			autofileparam.afvali = _wtoi(EdtVali->Text.c_str()) * SEK;
			wcsncpy(autofileparam.afname, EdtHtmlFName->Text.c_str(), sizeof(autofileparam.afname)/2-1);
			wcsncpy(autofileparam.wtitlea, EdtPageTitle->Text.c_str(), sizeof(autofileparam.wtitlea)/2-1);
			wcsncpy(autofileparam.wheadera, EdtPageHeader->Text.c_str(), sizeof(autofileparam.wheadera)/2-1);
			aftulparam.vaihelkm = RGVaiheet->ItemIndex + 1;
			aftulparam.yksihtml = RG1html->ItemIndex == 0;
			aftulparam.kohde = L'M';
			break;
		case 3:
			autofileparam.afvali = _wtoi(EdtVali->Text.c_str()) * SEK;
			autofileparam.af_xmlva = L"IIEM"[RGva->ItemIndex+1];
			aftulparam.kohde = L'X';
			break;
		}
	wcsncpy(autofileparam.autokomento, EdtKomento->Text.c_str(), sizeof(autofileparam.autokomento)/2-1);
	autofileparam.autokomentolaji = RGJatko->ItemIndex;
	if (FormJakelu)
		FormJakelu->CBAutoJakelu->Checked = autofileparam.autokomentolaji == 2;
//	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormAutoFile::BtnOhitavalinnatClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormAutoFile::Button1Click(TObject *Sender)
{
	SaveDialog1->InitialDir = FormMain->CurrentDir;
	SaveDialog1->FileName = EdtHtmlFName->Text;
	SaveDialog1->DefaultExt = L"html";
	SaveDialog1->Filter = L"Html-tiedostot (*.html)|*.HTML|Kaikki tiedostot (*.*)|*.*";
	if (SaveDialog1->Execute()) {
		EdtHtmlFName->Text = SaveDialog1->FileName;
		}
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------
void __fastcall TFormAutoFile::Button4Click(TObject *Sender)
{
	TextFl *aflst;
	wchar_t buf[100];

	aflst = new TextFl((FormMain->CurrentDir+L"\\"+autofileparam.aflstnm).c_str(), L"wt");
	if (aflst->IsOpen()) {
		for (int r = 0; r < Memo1->Lines->Count; r++) {
			aflst->WriteLine((Memo1->Lines->Strings[r]+L"\n").c_str());
			}
		}
	if (aflst)
		delete aflst;
}
//---------------------------------------------------------------------------
void __fastcall TFormAutoFile::BtnValitseKomentoClick(TObject *Sender)
{
	SaveDialog1->InitialDir = FormMain->CurrentDir;
	SaveDialog1->FileName = EdtKomento->Text;
	SaveDialog1->DefaultExt = L"cmd";
	SaveDialog1->Filter = L"CMD-tiedostot (*.cmd)|*.cmd|BAT-tiedostot (*.bat)|*.bat|Kaikki tiedostot (*.*)|*.*";
	if (SaveDialog1->Execute()) {
		EdtKomento->Text = SaveDialog1->FileName;
		}
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------
void __fastcall TFormAutoFile::Button6Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormAutoFile::BtnLahetysValClick(TObject *Sender)
{
	if (FormJakelu == NULL)
		FormJakelu = new TFormJakelu(this);
	FormJakelu->Show();
	if (FormJakelu->WindowState == wsMinimized)
		FormJakelu->WindowState = wsNormal;
	FormJakelu->BringToFront();
}
//---------------------------------------------------------------------------


void __fastcall TFormAutoFile::RGJatkoClick(TObject *Sender)
{
	BtnLahetysVal->Enabled = RGJatko->ItemIndex != 1;
	EdtKomento->Enabled = RGJatko->ItemIndex != 2;
	BtnValitseKomento->Enabled = EdtKomento->Enabled;
}
//---------------------------------------------------------------------------

void __fastcall TFormAutoFile::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Auto tiedostotulostus");
}
//---------------------------------------------------------------------------

void __fastcall TFormAutoFile::CBVaadiMuutosClick(TObject *Sender)
{
	autofileparam.vainMuutt = CBVaadiMuutos->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormAutoFile::BtnAvaaAFlstClick(TObject *Sender)
{
	TextFl *aflst;
	wchar_t buf[100];

	Memo1->Clear();
	aflst = new TextFl((FormMain->CurrentDir+L"\\"+autofileparam.aflstnm).c_str(), L"rt");
	while (aflst->IsOpen() && !aflst->Feof()) {
		if (aflst->ReadLine(buf, 98) == NULL)
			break;
		wtrimline(buf);
		Memo1->Lines->Add(buf);
		}
	if (aflst)
		delete aflst;
}
//---------------------------------------------------------------------------

void __fastcall TFormAutoFile::BtnLuoAFlstClick(TObject *Sender)
{
	Memo1->Clear();
	for	(int srj = 0; srj < sarjaluku + nsarjayhd; srj++) {
		UnicodeString ln;
		if (Sarjat[srj].vakanttisarja)
			continue;
		ln = UnicodeString(Sarjat[srj].sarjanimi) + L" 0 " +
			UnicodeString(Sarjat[srj].sarjanimi) + L".html " +
			UnicodeString(Sarjat[srj].psarjanimi);
		Memo1->Lines->Add(ln);
		}
	Application->MessageBoxW(L"Tietoja ei ole tallennettu", L"Varoitus", MB_OK);
}
//---------------------------------------------------------------------------


void __fastcall TFormAutoFile::FormCreate(TObject *Sender)
{
	startaf(1);
}
//---------------------------------------------------------------------------

