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
#include "WinHk.h"
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
	SaveDialog1->InitialDir = FormMain->CurrentDir;
	if (tyyli != 4) {
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

			for (int r = 0; r < nsra + (tyyli == 4 ? 2 : 0); r++) {
				switch (tyyli) {
					case 1:
						if (lkmfl)
							swprintf(ln, L"%s\t%s\t%d\n", sra[r+1].lyhenne, sra[r+1].seura, sra[r+1].lkm);
						else
							swprintf(ln, L"%s\t%s\n", sra[r+1].lyhenne, sra[r+1].seura);
						break;
					case 2:
						if (lkmfl)
							swprintf(ln, L"%s\t%d\n", sra[r+1].seura, sra[r+1].lkm);
						else
							swprintf(ln, L"%s\n", sra[r+1].seura);
						break;
					case 3:
						if (lkmfl)
							swprintf(ln, L"%s\t%d\n", sra[r+1].lyhenne, sra[r+1].lkm);
						else
							swprintf(ln, L"%s\n", sra[r+1].lyhenne);
						break;
					case 4:
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
void __fastcall TFormKilpSeurat::KirjoitalyhjaseuratClick(TObject *Sender)
{
	KirjoitaSeurat(1);
}

//---------------------------------------------------------------------------
void __fastcall TFormKilpSeurat::KirjoitaseuratClick(TObject *Sender)
{
	KirjoitaSeurat(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::KirjoitalyhenteetClick(TObject *Sender)
{
	KirjoitaSeurat(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::Kirjoitakaikkitaulukontiedot1Click(TObject *Sender)
{
	KirjoitaSeurat(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::SisosotlukuClick(TObject *Sender)
{
	Sisosotluku->Checked = !Sisosotluku->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::Seuranimi1Click(TObject *Sender)
{
	Seuranimi1->Checked = !Seuranimi1->Checked;
	if (!Seuranimi1->Checked) {
		Lyhenne1->Checked = true;
		Lyhennejrj->Checked = true;
		}
	Valitsevaihe(Vaihe);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::Lyhenne1Click(TObject *Sender)
{
	Lyhenne1->Checked = !Lyhenne1->Checked;
	if (!Lyhenne1->Checked) {
		Seuranimi1->Checked = true;
		Lyhennejrj->Checked = false;
		}
	Valitsevaihe(Vaihe);
}
//---------------------------------------------------------------------------

int HaeSeuraLkm(int sralaji, int Vaihe, sralkmtp *sra, int *nsra, int maxnsra, bool lyhjarj)
{
	kilptietue kilp;
	int isra, i_pv, err = 0;
	wchar_t wkey[LSEURA+LLYH+2];

	memset(sra, 0, maxnsra*sizeof(sra[0]));
	*nsra = 0;
	for (int d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (kilp.kilpstatus)
			continue;
		if (Vaihe > 0 && (kilp.tark(Vaihe-1) == L'P' || Sarjat[kilp.Sarja(Vaihe-1)].luonne[Vaihe-1] < 0))
			continue;
		if (Vaihe == 0) {
			for (i_pv = 0; i_pv < kilpparam.n_pv_akt; i_pv++) {
				if (kilp.tark(i_pv) != L'P' && Sarjat[kilp.Sarja(i_pv)].luonne[i_pv] >= 0)
					break;
				}
			if (i_pv >= kilpparam.n_pv_akt)
				continue;
			}
		for (isra = 1; isra <= *nsra; isra++) {
			 if ((sralaji == 2 || !wcscmp(kilp.seura, sra[isra].seura)) &&
				(sralaji == 1 || !wcscmp(kilp.seuralyh, sra[isra].lyhenne)))
				break;
			 }
		if (isra > *nsra) {
			if (*nsra >= maxnsra) {
				err = 1;
				break;
				}
			(*nsra)++;
			wcsncpy(sra[isra].seura, kilp.seura, sizeof(sra[isra].seura)/2-1);
			wcsncpy(sra[isra].lyhenne, kilp.seuralyh, sizeof(sra[isra].lyhenne)/2-1);
			}
		for (i_pv = 0; i_pv < kilpparam.n_pv_akt; i_pv++) {
			if (kilp.tark(i_pv) != L'P' && Sarjat[kilp.Sarja(i_pv)].luonne[i_pv] >= 0) {
				sra[isra].pvlkm[i_pv]++;
				sra[0].pvlkm[i_pv]++;
				}
			}
		sra[isra].srjlkm[kilp.Sarja(Vaihe-1)]++;
		sra[0].srjlkm[kilp.Sarja(Vaihe-1)]++;
		sra[isra].lkm++;
		sra[0].lkm++;
		}
	for (isra = 1; isra <= *nsra; isra++) {
		switch (sralaji) {
			case 1:
				w_aakjarjstr(sra[isra].key, sra[isra].seura, LSEURA, 0, 0);
				break;
			case 2:
				w_aakjarjstr(sra[isra].key, sra[isra].lyhenne, LLYH, 0, 0);
				break;
			case 3:
				if (lyhjarj) {
					w_aakjarjstr(sra[isra].key, sra[isra].lyhenne, LLYH, 0, 0);
					w_aakjarjstr(sra[isra].key+LLYH+1, sra[isra].seura, LSEURA, 0, 0);
					}
				else {
					w_aakjarjstr(sra[isra].key, sra[isra].seura, LSEURA, 0, 0);
					w_aakjarjstr(sra[isra].key+LSEURA+1, sra[isra].lyhenne, LLYH, 0, 0);
					}
				break;
			}
		}
	quicksort((char *) &sra[1], *nsra, LSEURA+LLYH+1, sizeof(sra[0]));
	return(err);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::Nayta(void)
{
	int sralaji, isra, i_pv, srj, clisa, sracol = -1, lyhcol = -1;
	int epv, vpv;

	sralaji = (Seuranimi1->Checked ? 1 : 0) + (Lyhenne1->Checked ? 2 : 0);

	HaeSeuraLkm(sralaji, Vaihe, sra, &nsra, maxnseura, Lyhennejrj->Checked);

	SG1->RowCount = nsra+2;
	epv = Vaihe ? Vaihe - 1 : 0;
	vpv = Vaihe ? Vaihe - 1 : kilpparam.n_pv_akt - 1;
	clisa = 2 + ((vpv - epv) ? (vpv -epv + 1) : 0) + (sralaji-1)/2;
	SG1->ColCount = sarjaluku + clisa;
	SG1->FixedRows = 2;
	wcscpy(sra[0].seura, L"Yhteensä");
	wcscpy(sra[0].lyhenne, L"Yhteensä");
	if (sralaji != 2) {
		sracol = Lyhennejrj->Checked ? 1 : 0;
		SG1->Cells[sracol][0] = L"Seura";
		SG1->ColWidths[sracol] = 180 * Screen->PixelsPerInch / 96;
		}
	if (sralaji != 1) {
		lyhcol = Lyhennejrj->Checked ? 0 : 1;
		SG1->Cells[lyhcol][0] = L"Lyhenne";
		SG1->ColWidths[lyhcol] = 90 * Screen->PixelsPerInch / 96;
		}
	SG1->Cells[(sralaji-1)/2 + 1][0] = L"Kaikki";
	SG1->ColWidths[(sralaji-1)/2 + 1] = 45 * Screen->PixelsPerInch / 96;
	if (vpv > epv) {
		for (i_pv = epv; i_pv <= vpv; i_pv++) {
			SG1->Cells[(sralaji-1)/2 + i_pv - epv + 2][0] = UnicodeString(i_pv+1)+L". vaihe";
			SG1->ColWidths[(sralaji-1)/2 + i_pv - epv + 2] = 45 * Screen->PixelsPerInch / 96;
			}
		}
	for (srj = 0; srj < sarjaluku; srj++) {
		SG1->Cells[clisa+srj][0] = Sarjat[srj].sarjanimi;
		SG1->ColWidths[clisa+srj] = 50 * Screen->PixelsPerInch / 96;
		}
	for (isra = 0; isra <= nsra; isra++) {
		if (sralaji != 2) {
			SG1->Cells[sracol][isra+1] = sra[isra].seura;
			}
		if (sralaji != 1) {
			SG1->Cells[lyhcol][isra+1] = sra[isra].lyhenne;
			}
		SG1->Cells[(sralaji-1)/2 + 1][isra+1] = sra[isra].lkm;
		if (vpv > epv) {
			for (i_pv = epv; i_pv <= vpv; i_pv++)
				SG1->Cells[(sralaji-1)/2 + i_pv - epv + 2][isra+1] = sra[isra].pvlkm[i_pv];
			}
		for (srj = 0; srj < sarjaluku; srj++) {
			SG1->Cells[clisa+srj][isra+1] = sra[isra].srjlkm[srj];
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::NaytaSarjat(void)
{
	int i_pv, srj, npv[N_PV];

	for (i_pv = 0; i_pv < kilpparam.n_pv_akt; i_pv++) {
		npv[i_pv] = 0;
		for (srj = 0; srj < sarjaluku; srj++)
			npv[i_pv] += nilm_pv[i_pv][srj];
	}
	SG1->RowCount = sarjaluku+2;
	SG1->ColCount = kilpparam.n_pv_akt + 2;
	SG1->FixedRows = 1;
	SG1->ColWidths[0] = 70 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[1] = 60 * Screen->PixelsPerInch / 96;
	SG1->Cells[0][0] = L"Sarja";
	SG1->Cells[1][0] = L"Yhteensä";
	SG1->Cells[1][1] = nyilmt;
	for (i_pv = 0; i_pv < kilpparam.n_pv_akt; i_pv++) {
		SG1->Cells[i_pv+2][0] = UnicodeString(i_pv+1) + L". vaihe";
		SG1->ColWidths[i_pv+2] = 60 * Screen->PixelsPerInch / 96;
		SG1->Cells[i_pv+2][1] = npv[i_pv];
		}
	SG1->Cells[0][1] = L"Yhteensä";
	for (srj = 0; srj < sarjaluku; srj++) {
		SG1->Cells[0][srj+2] = Sarjat[srj].sarjanimi;
		SG1->Cells[1][srj+2] = nyilm[srj];
		for (i_pv = 0; i_pv < kilpparam.n_pv_akt; i_pv++) {
			SG1->Cells[i_pv+2][srj+2] = nilm_pv[i_pv][srj];
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::Valitsevaihe(int vaihe)
{
	Vaihe = vaihe;
	Vaiheet1->Checked = Vaihe == 0;
	Sarjat1vaihe1->Checked = Vaihe == 1;
	Sarjat2vaihe1->Checked = Vaihe == 2;
	Sarjat3vaihe1->Checked = Vaihe == 3;
	Sarjat4vaihe1->Checked = Vaihe == 4;
	Sarjat5vaihe1->Checked = Vaihe == 5;
	Nayta();
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::Vaiheet1Click(TObject *Sender)
{
	Valitsevaihe(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::Sarjat1vaihe1Click(TObject *Sender)
{
	Valitsevaihe(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::Sarjat2vaihe1Click(TObject *Sender)
{
	Valitsevaihe(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::Sarjat3vaihe1Click(TObject *Sender)
{
	Valitsevaihe(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::Sarjat4vaihe1Click(TObject *Sender)
{
	Valitsevaihe(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::Sarjat5vaihe1Click(TObject *Sender)
{
	Valitsevaihe(5);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::FormShow(TObject *Sender)
{
	Sarjat2vaihe1->Visible = kilpparam.n_pv_akt > 1;
	Sarjat3vaihe1->Visible = kilpparam.n_pv_akt > 2;
	Sarjat4vaihe1->Visible = kilpparam.n_pv_akt > 3;
	Sarjat5vaihe1->Visible = kilpparam.n_pv_akt > 4;
	maxnseura = nilmt + 10;
	sra = new sralkmtp[maxnseura];
	Valitsevaihe(kilpparam.n_pv_akt > 1 ? 0: 1);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::LyhennejrjClick(TObject *Sender)
{
	Lyhennejrj->Checked = !Lyhennejrj->Checked;
	Valitsevaihe(Vaihe);
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

void __fastcall TFormKilpSeurat::Vainsarjatvaiheittain1Click(TObject *Sender)
{
	Vainsarjatvaiheittain1->Checked = !Vainsarjatvaiheittain1->Checked;
	if (Vainsarjatvaiheittain1->Checked)
		NaytaSarjat();
	else
		Nayta();
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpSeurat::Suljekaavake1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

