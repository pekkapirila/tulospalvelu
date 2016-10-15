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

#include "UnitAvausMaar.h"
#include "UnitMain.h"
#include "UnitCfg.h"
#include "VDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAvausMaar *FormAvausMaar;
//---------------------------------------------------------------------------
__fastcall TFormAvausMaar::TFormAvausMaar(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormAvausMaar::LueCfgFile(void)
{
	TextFl *cfgfile;

	Memo1->Clear();
	cfgfile = new TextFl(Edit1->Text.c_str(), L"r");
	if (cfgfile->IsOpen()) {
		while (!cfgfile->Feof()) {
			wchar_t line[200];

			if (!cfgfile->ReadLine(line, 198))
				break;
			wtrimline(line);
			if (wcslen(line) > 0)
				Memo1->Lines->Add(line);
			}
		}
	delete cfgfile;

}
//---------------------------------------------------------------------------
void __fastcall TFormAvausMaar::Button1Click(TObject *Sender)
{
	TextFl *cfgfile;

	if (RadioGroup1->ItemIndex == 1)
		OpenDialog1->FileName = UnicodeString(L"ilmoitt.cfg");
	else
		OpenDialog1->FileName = UnicodeString(L"laskenta.cfg");
	OpenDialog1->Filter = L"Konfiguraatiot (*.cfg) | *.CFG";
	if (OpenDialog1->Execute()) {
		Edit1->Text = OpenDialog1->FileName;
		LueCfgFile();
		}
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------
void __fastcall TFormAvausMaar::Button2Click(TObject *Sender)
{
	SetCurrentDir(FormMain->InitDir);
	if (FormMain->InitFileName.Length() == 0)
		FormMain->InitFileName = FormMain->InitDir+L"\\Init.cfg";
	SaveDialog1->FileName = FormMain->InitFileName;
	SaveDialog1->Filter = L"Konfiguraatiot (*.cfg) | *.CFG";
	if (!SaveDialog1->Execute()) {
		SetCurrentDir(FormMain->CurrentDir);
		return;
		}
	FormMain->InitFileName = SaveDialog1->FileName;
	SetCurrentDir(FormMain->CurrentDir);
	TextFl *InitFile = new TextFl(FormMain->InitFileName.c_str(), L"w");
	if (InitFile->IsOpen()) {
		wchar_t line[200];

		switch (RadioGroup1->ItemIndex) {
			case 0:
				InitFile->WriteLine(L"PÄÄVALIKKOON\n");
				if (!CBLuettValinta->Checked)
					InitFile->WriteLine((UnicodeString(L"CFG=")+Edit1->Text+UnicodeString(L"\n")).c_str());
				break;
			case 1:
				InitFile->WriteLine(L"ILMOITTAUTUMISTILA\n");
				if (!CBLuettValinta->Checked)
					InitFile->WriteLine((UnicodeString(L"CFG=")+Edit1->Text+UnicodeString(L"\n")).c_str());
				InitFile->WriteLine(L"AVAA\n");
				break;
			case 2:
				InitFile->WriteLine(L"TULOSPALVELUTILA\n");
				if (!CBLuettValinta->Checked)
					InitFile->WriteLine((UnicodeString(L"CFG=")+Edit1->Text+UnicodeString(L"\n")).c_str());
				InitFile->WriteLine(L"AVAA\n");
				break;
			}
		if (CBKuntoSuunn->Checked)
			InitFile->WriteLine(L"KUNTOSUUNN\n");
		if (CBLuettValinta->Checked)
			InitFile->WriteLine(L"LUETTELOVALINTA\n");
		FormMain->CfgFileName = FormMain->InitDir+UnicodeString(L"\\Init.cfg");
		InitFile->Close();
		}
	delete InitFile;
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------
void __fastcall TFormAvausMaar::Button3Click(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormAvausMaar::FormShow(TObject *Sender)
{
	UnicodeString cfgflnm = L"";
	int TTila = 0, KsMoodi = 0;
	bool KLuett = false, avaa = false;

	FormMain->lueInit(&TTila, &avaa, FormMain->InitFileName, &cfgflnm);
	Edit1->Text = cfgflnm;
	RadioGroup1->ItemIndex = avaa ? TTila : 0;
	CBKuntoSuunn->Checked = KsMoodi != 0;
	CBLuettValinta->Checked = KLuett;
	if (&KLuett && cfgflnm.Length() > 1) {
		LueCfgFile();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormAvausMaar::Button4Click(TObject *Sender)
{
	TextFl *cfgfile;

	SaveDialog1->FileName = Edit1->Text;
	SaveDialog1->Filter = L"Konfiguraatiot (*.cfg) | *.CFG";
	if (SaveDialog1->Execute()) {
		cfgfile = new TextFl(SaveDialog1->FileName.c_str(), L"w");
		if (cfgfile->IsOpen()) {
			for (int r = 0; r < Memo1->Lines->Count; r++) {
				cfgfile->WriteLine((Memo1->Lines->Strings[r]+L"\n").c_str());
				}
			}
		delete cfgfile;
		}
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------

void __fastcall TFormAvausMaar::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Asentaminen");
}
//---------------------------------------------------------------------------

void __fastcall TFormAvausMaar::Button5Click(TObject *Sender)
{
	if (FormCfg == NULL)
		FormCfg = new TFormCfg(this);
	FormCfg->Show();
}
//---------------------------------------------------------------------------

