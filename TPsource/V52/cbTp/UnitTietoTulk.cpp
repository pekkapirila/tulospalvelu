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

#include "UnitTietoTulk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormTietoTulk *FormTietoTulk;

wchar_t *wcstokch(wchar_t *st, wchar_t erotin);
#define max(x,y) ((x) < (y) ? (y) : (x))

//---------------------------------------------------------------------------
__fastcall TFormTietoTulk::TFormTietoTulk(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	tunnCol = 1;
	dataCol = 2;
	EdtTunn->Text = tunnCol;
	EdtData->Text = dataCol;
}
//---------------------------------------------------------------------------
void __fastcall TFormTietoTulk::Luetiedottiedostosta1Click(TObject *Sender)
{
	if (OpenDialog1->Execute()) {
		TextFl *InFile = new TextFl(OpenDialog1->FileName.c_str(), L"rt");
		if (InFile->IsOpen()) {
			while (!InFile->Feof()) {
				wchar_t buf[1000];
				buf[sizeof(buf)/2-1] = 0;
				InFile->ReadLine(buf, sizeof(buf)/2-1);
				if (wcslen(buf) > 1)
					Memo1->Lines->Add(buf);
				}
			}
		delete InFile;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormTietoTulk::yhjenntekstialue1Click(TObject *Sender)
{
	Memo1->Clear();
}
//---------------------------------------------------------------------------
void __fastcall TFormTietoTulk::Liittiedotleikepydlt1Click(TObject *Sender)
{
//
}
//---------------------------------------------------------------------------
void __fastcall TFormTietoTulk::FormResize(TObject *Sender)
{
	Memo1->Top = 0;
	Memo1->Left = 0;
	Memo1->Width = ClientWidth;
	Memo1->Height = (ClientHeight - Panel1->Height) /2;
	Panel1->Left = 0;
	Panel1->Width = ClientWidth;
	Panel1->Top = Memo1->Top + Memo1->Height;
	SG1->Top = Panel1->Top + Panel1->Height;
	SG1->Left = 0;
	SG1->Width = ClientWidth;
	SG1->Height = ClientHeight - SG1->Top;
}
//---------------------------------------------------------------------------
void __fastcall TFormTietoTulk::FormShow(TObject *Sender)
{
	EdtTunn->Visible = NaytaSar;
	LblTunn->Visible = NaytaSar;
	EdtData->Visible = NaytaSar;
	LblData->Visible = NaytaSar;
	CBOtsRivi->Checked = !NaytaSar;
	FormResize(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormTietoTulk::Sulje1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormTietoTulk::Button1Click(TObject *Sender)
{
	wchar_t erotin[2] = L";", erottimet[] = L" \t;,", *sep, *qt, tietost[200], *fields[200];
	int r, r1, cc, merkkeja[50], nflds = 0;
	bool eityhja;

	erotin[0] = erottimet[CBErotin->ItemIndex];
	memset(merkkeja, 0, sizeof(merkkeja));
	memset(fields, 0, sizeof(fields));
	cc = _wtoi(EdtCols->Text.c_str()) + 1;
	SG1->ColCount = max(cc, 2);
	r = Memo1->Lines->Count + 1;
	r1 = CBOtsRivi->Checked ? 3 : 2;
	SG1->RowCount = max(r, r1);
	SG1->FixedRows = r1 - 1;
	SG1->Cells[0][0] = L"Rivi";
	for (int c = 1; c < SG1->ColCount; c++) {
		SG1->Cells[c][0] = c;
		}
	r1 = 1;
	for (r = 0; r < Memo1->Lines->Count; r++) {
		wchar_t *p, line[1000];
		SG1->Cells[0][r1] = r1;
		wcsncpy(line, Memo1->Lines->Strings[r].c_str(), sizeof(line)/2-1);
		if (erotin[0] == L' ')
			sep = L" \t\n";
		else
			sep = erotin;
		qt =  CBLainMerk->Checked ? L"\"" : L"";
		nflds = getfields(line, fields, sizeof(fields)/sizeof(fields[0]),
			sep, qt, erotin[0] != L' ', true);
		if (nflds == 0)
			continue;
		eityhja = false;
		for (int i = 0; i < SG1->ColCount-1; i++) {
			memset(tietost, 0, sizeof(tietost));
			if (i < nflds) {
				wcsncpy(tietost, fields[i], sizeof(tietost)/2-1);
				elimwbl2(tietost);
				if (wcslen(tietost) > 0)
					eityhja = true;
				}
			SG1->Cells[i+1][r1] = tietost;
			if (i < 50 && wcslen(tietost) > merkkeja[i])
				merkkeja[i] = wcslen(tietost);
			}
		if (eityhja)
			r1++;
		}
	if (fields[0] != NULL)
		deletefields(fields, sizeof(fields)/sizeof(fields[0]));
	tietolkm = r1 - SG1->FixedRows;
	SG1->RowCount = max(SG1->FixedRows + 1, r1);
	r1 = 4;
	for (int c = 0; c < 50 && c < SG1->ColCount - 1; c++)
		r1 += merkkeja[c];
	SG1->ColWidths[0] = (4 * (SG1->ClientWidth - 2 * SG1->ColCount - 20)) / r1;
	for (int c = 0; c < 50 && c < SG1->ColCount - 1; c++) {
		SG1->ColWidths[c+1] = (merkkeja[c] * (SG1->ClientWidth - 2 * SG1->ColCount - 20)) / r1;
		}
}
//---------------------------------------------------------------------------
wchar_t *TFormTietoTulk::tunniste(wchar_t *st, int len, int r)
{
	tunnCol = _wtoi(EdtTunn->Text.c_str());
	wcsncpy(st, SG1->Cells[tunnCol][SG1->FixedRows+r].c_str(), len);
	return(st);
}
//---------------------------------------------------------------------------

wchar_t *TFormTietoTulk::tieto(wchar_t *st, int len, int r)
{
	dataCol = _wtoi(EdtData->Text.c_str());
	wcsncpy(st, SG1->Cells[dataCol][SG1->FixedRows+r].c_str(), len);
	return(st);
}
//---------------------------------------------------------------------------
void __fastcall TFormTietoTulk::EdtTunnExit(TObject *Sender)
{
	tunnCol = _wtoi(EdtTunn->Text.c_str());
	EdtTunn->Text = tunnCol;
}
//---------------------------------------------------------------------------
void __fastcall TFormTietoTulk::EdtDataExit(TObject *Sender)
{
	dataCol = _wtoi(EdtData->Text.c_str());
	EdtData->Text = dataCol;
}
//---------------------------------------------------------------------------
