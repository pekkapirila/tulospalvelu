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

#include "UnitLisaTekstit.h"
#ifndef MAXOSUUSLUKU
#include "WinHk.h"
#else
#include "UnitMain.h"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormLisaTekstit *FormLisaTekstit;

extern lisatxttp lisatxt[4];

//---------------------------------------------------------------------------
__fastcall TFormLisaTekstit::TFormLisaTekstit(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormLisaTekstit::Poistu1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormLisaTekstit::Luetekstit1Click(TObject *Sender)
{
	wchar_t line[300], tunn, *p;
	txtTp txt, txts[4][50];
	int n[4], i, laji, ya;
	bool par;

	memset(txts, 0, sizeof(txts));
	memset(n, 0, sizeof(n));
	TextFl *inFile;
	OpenDialog1->FileName = FormMain->CurrentDir+UnicodeString(L"\\lisatekstit.csv");
	OpenDialog1->DefaultExt = L"csv";
	OpenDialog1->Filter = L"Csv-tiedostot (*.csv)|*.CSV|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		inFile = new TextFl(OpenDialog1->FileName.c_str(), L"rt");

		while (inFile && !inFile->Feof()) {
			inFile->ReadLine(line, 298);
			p = wcstok(line, L";\t\n");
			if (p) {
				tunn = towupper2(*p);
				laji = wcswcind(tunn, L"PSAL");
				if (laji < 0)
					continue;
				p = wcstok(NULL, L";\t\n");
				}
			if (p) {
				if (*p >= L'A') {
					ya = wcswcind(towupper2(*p), L"ALY") - 1;
					if (ya < -1)
						ya = 0;
					par = true;
					lisatxt[laji].ypos = ya;
					}
				else {
					par = false;
					txt.x = _wtoi(p);
					}
				p = wcstok(NULL, L";\t\n");
				}
			if (p) {
				if (par)
					lisatxt[laji].ytila = _wtoi(p);
				else
					txt.y = _wtoi(p);
				p = wcstok(NULL, L";\t\n");
				}
			if (par)
				continue;
			if (p) {
				txt.size = _wtoi(p);
				p = wcstok(NULL, L";\t\n");
				}
			if (p) {
				wcscpy(txt.txt, p);
				i = wcswcind(tunn, L"PSAL");
				if (i >= 0) {
					txts[i][n[i]++] = txt;
					}
				}
			}
		delete inFile;
		}
	for (int i = 0; i < 4; i++) {
		if (lisatxt[i].txts)
			delete[] lisatxt[i].txts;
		if (n[i])
			lisatxt[i].txts = new txtTp[n[i]];
		lisatxt[i].n_txt = n[i];
		for (int j = 0; j < n[i]; j++)
			lisatxt[i].txts[j] = txts[i][j];
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormLisaTekstit::allennatekstit1Click(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TFormLisaTekstit::CheckBox1Click(TObject *Sender)
{
	lsttulparam.paa_alkuteksti = CheckBox1->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TFormLisaTekstit::CheckBox2Click(TObject *Sender)
{
	lsttulparam.sivu_alkuteksti = CheckBox2->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TFormLisaTekstit::CheckBox3Click(TObject *Sender)
{
	lsttulparam.sivu_alateksti = CheckBox3->Checked;
	lsttulparam.tulmuot.amarg = (CheckBox3->Checked ? lisatxt[2].ytila : 0) + 30;
}
//---------------------------------------------------------------------------
void __fastcall TFormLisaTekstit::CheckBox4Click(TObject *Sender)
{
	lsttulparam.lopputeksti = CheckBox4->Checked;
}
//---------------------------------------------------------------------------
