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

#include "UnitKilpSeurat.h"
#include "VDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormKilpSeurat *FormKilpSeurat;

//---------------------------------------------------------------------------
__fastcall TFormKilpSeurat::TFormKilpSeurat(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormKilpSeurat::KirjoitaSeurat(int tyyli)
{
	TextFl *outfile;
	bool lkmfl;

	lkmfl = Sisosotluku->Checked;
	if (tyyli != 2) {
		SaveDialog1->FileName = L"Seuraluettelo.txt";
		SaveDialog1->DefaultExt = L"txt";
		SaveDialog1->Filter = L"Tekstitiedostot (*.txt) | *.TXT|Kaikki tiedostot (*.*)|*.*";
		}
	else {
		SaveDialog1->FileName = L"Seuraluettelo.csv";
		SaveDialog1->DefaultExt = L"csv";
		SaveDialog1->Filter = L"CSV-tiedostot (*.csv) | *.CSV|Kaikki tiedostot (*.*)|*.*";
		}
	if (SaveDialog1->Execute()) {
		outfile = new TextFl(SaveDialog1->FileName.c_str(), L"w");
		if (outfile->IsOpen()) {
			wchar_t ln[2000];

			for (int r = 0; r < nsra + (tyyli == 2 ? 2 : 0); r++) {
				switch (tyyli) {
					case 1:
						if (lkmfl)
							swprintf(ln, L"%s\t%d\n", sra[r+1].seura, sra[r+1].lkm);
						else
							swprintf(ln, L"%s\n", sra[r+1].seura);
						break;
					case 2:
						memset(ln, 0, sizeof(ln));
						for (int c = 0; c < SG1->ColCount; c++) {
							if (wcslen(ln) + SG1->Cells[c][r].Length() > sizeof(ln)/2-3)
								break;
							wcscat(ln, SG1->Cells[c][r].c_str());
							if (c < SG1->ColCount - 1)
								wcscat(ln, L";");
							}
						wcscat(ln, L"\n");
						break;
					}
				if (wcscmp(ln, L"\n") && wcscmp(ln, L"\t\n"))
					outfile->WriteLine(ln);
				}
			}
		delete outfile;
		}
	SetCurrentDir(WorkingDir);

}

//---------------------------------------------------------------------------
void __fastcall TFormKilpSeurat::KirjoitaseuratClick(TObject *Sender)
{
	KirjoitaSeurat(1);
}
//---------------------------------------------------------------------------


void __fastcall TFormKilpSeurat::Kirjoitakaikkitaulukontiedot1Click(TObject *Sender)
{
	KirjoitaSeurat(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::SisosotlukuClick(TObject *Sender)
{
	Sisosotluku->Checked = !Sisosotluku->Checked;
}
//---------------------------------------------------------------------------

int HaeSeuraLkm(sralkmtp *sra, int *nsra, int maxnsra)
{
	kilptietue kilp;
	int isra, i_pv, err = 0;
	wchar_t wkey[LSEURA+2], st[100];

	memset(sra, 0, maxnsra*sizeof(sra[0]));
	*nsra = 0;
	for (int d = 1; d < datf2.numrec; d++) {
		kilp.getrec(d);
		if (kilp.kilpstatus)
			continue;
		if (kilp.wTark(0, 0) != L'P' && kilp.wTark(0, 0) != L'E')
			break;
		for (isra = 1; isra <= *nsra; isra++) {
			 if (!wcscmp(kilp.Seura(st, LSEURA), sra[isra].seura))
				break;
			 }
		if (isra > *nsra) {
			if (*nsra >= maxnsra) {
				err = 1;
				break;
				}
			(*nsra)++;
			wcsncpy(sra[isra].seura, kilp.Seura(st, LSEURA), sizeof(sra[isra].seura)/2-1);
			}
		if (kilp.wTark(0, 0) != L'P' && kilp.wTark(0, 0) != L'E') {
			sra[0].pvlkm++;
			}
		sra[isra].srjlkm[kilp.sarja]++;
		sra[0].srjlkm[kilp.sarja]++;
		sra[isra].lkm++;
		sra[0].lkm++;
		}
	for (isra = 1; isra <= *nsra; isra++) {
		w_aakjarjstr(sra[isra].key, sra[isra].seura, LSEURA, 0, 0);
		}
	quicksort((char *) &sra[1], *nsra, LSEURA+1, sizeof(sra[0]));
	return(err);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::Nayta(void)
{
	int isra, i_pv, srj, clisa, sracol = 0;
	int epv, vpv;

	HaeSeuraLkm(sra, &nsra, maxnseura);

	SG1->RowCount = nsra+2;
	epv = 0;
	vpv = kilpparam.osuusluku - 1;
	clisa = 2 + ((vpv - epv) ? (vpv -epv + 1) : 0);
	SG1->ColCount = sarjaluku + clisa;
	SG1->FixedRows = 2;
	wcscpy(sra[0].seura, L"Yhteensä");
	SG1->Cells[sracol][0] = L"Seura";
	SG1->ColWidths[sracol] = 180 * Screen->PixelsPerInch / 96;
	SG1->Cells[1][0] = L"Kaikki";
	SG1->ColWidths[1] = 45 * Screen->PixelsPerInch / 96;
	if (vpv > epv) {
		for (i_pv = epv; i_pv <= vpv; i_pv++) {
			SG1->Cells[i_pv - epv + 2][0] = UnicodeString(i_pv+1)+L". osuus";
			SG1->ColWidths[i_pv - epv + 2] = 45 * Screen->PixelsPerInch / 96;
			}
		}
	for (srj = 0; srj < sarjaluku; srj++) {
		SG1->Cells[clisa+srj][0] = Sarjat[srj].sarjanimi;
		SG1->ColWidths[clisa+srj] = 50 * Screen->PixelsPerInch / 96;
		}
	for (isra = 0; isra <= nsra; isra++) {
		SG1->Cells[sracol][isra+1] = sra[isra].seura;
		SG1->Cells[1][isra+1] = sra[isra].lkm;
		if (vpv > epv) {
			for (i_pv = epv; i_pv <= vpv; i_pv++)
				SG1->Cells[i_pv - epv + 2][isra+1] = sra[isra].pvlkm;
			}
		for (srj = 0; srj < sarjaluku; srj++) {
			SG1->Cells[clisa+srj][isra+1] = sra[isra].srjlkm[srj];
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::NaytaSarjat(void)
{
	int i_pv, srj;

	SG1->RowCount = sarjaluku+2;
	SG1->ColCount = kilpparam.osuusluku + 2;
	SG1->FixedRows = 1;
	SG1->ColWidths[0] = 70 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[1] = 60 * Screen->PixelsPerInch / 96;
	SG1->Cells[0][0] = L"Sarja";
	SG1->Cells[1][0] = L"Yhteensä";
	SG1->Cells[1][1] = nilmt;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::FormShow(TObject *Sender)
{
	maxnseura = nilmt + 10;
	sra = new sralkmtp[maxnseura];
}
//---------------------------------------------------------------------------


void __fastcall TFormKilpSeurat::FormResize(TObject *Sender)
{
	SG1->Width = ClientWidth - SG1->Left - 4;
	SG1->Height = ClientHeight - SG1->Top - 4;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::FormClose(TObject *Sender, TCloseAction &Action)

{
	delete[] sra;
	sra = NULL;
	maxnseura = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::SG1MouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SG1->RowCount - SG1->FixedRows >= SG1->VisibleRowCount + SG1->TopRow)
		SG1->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::SG1MouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SG1->TopRow > SG1->FixedRows)
		SG1->TopRow--;
}
//---------------------------------------------------------------------------


void __fastcall TFormKilpSeurat::Suljekaavake1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

