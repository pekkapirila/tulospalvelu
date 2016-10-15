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

#include "UnitEmitFile.h"
#include <stdio.h>
#ifndef MAXOSUUSLUKU
#include "HkDeclare.h"
#else
#include "VDeclare.h"
#endif
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEmitFile *FormEmitFile;
static FILE *emitinfile;
static int nemit;
static emittp *emarr;

//---------------------------------------------------------------------------
__fastcall TFormEmitFile::TFormEmitFile(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitFile::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (emarr)
		delete[] emarr;
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitFile::Button1Click(TObject *Sender)
{

	OpenDialog1->DefaultExt = "dat";
	OpenDialog1->Filter = "Datatiedostot (*.dat) |*.dat|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		emitinfile = _wfopen(OpenDialog1->FileName.c_str(), L"rb");
		if (emitinfile) {
			int len;
			wchar_t st[40];
			fseek(emitinfile, 0, SEEK_END);
			len = ftell(emitinfile);
			nemit = len/sizeof(emittp);
			emarr = new emittp[len+1];
			fseek(emitinfile, 0, SEEK_SET);
			fread(emarr, sizeof(emittp), nemit, emitinfile);
			fclose(emitinfile);
			SG1->RowCount = nemit+1;
			for (int r = 1; r <= nemit; r++) {
				SG1->Cells[0][r] = r;
				SG1->Cells[1][r] = emarr[r-1].badge;
				SG1->Cells[2][r] = emarr[r-1].badge0;
				SG1->Cells[3][r] = emarr[r-1].kilpno;
				SG1->Cells[4][r] = emarr[r-1].osuus;
				SG1->Cells[5][r] = aikatowstr_ls(st, emarr[r-1].time, t0);
				SG1->Cells[6][r] = aikatowstr_ls(st, emarr[r-1].maali, t0);
				for (int c = 0; c < 50; c++) {
					SG1->Cells[2*c+7][r] = (int)emarr[r-1].ctrlcode[c];
					SG1->Cells[2*c+8][r] = emarr[r-1].ctrltime[c];
					}
				}
			Edit1->Text = 1;
			Edit2->Text = nemit;
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitFile::FormResize(TObject *Sender)
{
	SG1->Width = ClientWidth - SG1->Left;
	SG1->Height = ClientHeight - SG1->Top;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitFile::FormShow(TObject *Sender)
{
	SG1->RowCount = 2;
	SG1->ColWidths[1] = 52;
	SG1->ColWidths[2] = 52;
	SG1->ColWidths[5] = 64;
	SG1->ColWidths[6] = 64;
	SG1->Cells[0][0] = L"Rivi";
	SG1->Cells[1][0] = L"Badge";
	SG1->Cells[2][0] = L"OrigBdg";
	SG1->Cells[3][0] = L"Kno";
	SG1->Cells[4][0] = L"Os";
	SG1->Cells[5][0] = L"Luettu";
	SG1->Cells[6][0] = L"Maali";
	for (int c = 0; c < 50; c++) {
		SG1->Cells[2*c+7][0] = L"C-"+UnicodeString(c);
		SG1->Cells[2*c+8][0] = L"T-"+UnicodeString(c);
		}
	for (int c = 0; c < 107; c++)
		SG1->Cells[c][1] = L"";
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitFile::Button2Click(TObject *Sender)
{
	int ens, viim;

	ens = Edit1->Text.ToInt();
	viim = Edit2->Text.ToInt();
	for (int r = ens-1; r < viim; r++) {
		emittp em;
		memcpy(&em, emarr+r, sizeof(em));
		em.maali = TMAALI0/KSEK;
		em.kirjaus = TMAALI0/KSEK;
		em.kilpno = 0;
		em.osuus = 0;
		switch (RGEmit->ItemIndex) {
			case 0:
				em.badge = em.badge0;
				break;
			case 1:
				em.badge0 = em.badge;
				break;
			}
		addem(&em, 0);
		}
	Application->MessageBoxW(L"Valmis", L"", MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitFile::Button3Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

