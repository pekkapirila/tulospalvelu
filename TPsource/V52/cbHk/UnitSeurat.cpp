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
#include <ctype.h>

#include "WinHk.h"
#include "UnitSeurat.h"
#include "UnitEditDialog.h"
#include "UnitIlm2.h"
#include "UnitEmit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSeurat *FormSeurat;
extern TEditDialog *EditDialog;

ColTp SraGridCols0[] = {
	{L"Piiri", 40, 0, 1, 0, true},
	{L"Maa", 40, 0, 1, 0, false},
	{L"Lyhenne", 80, 0, 1, 0, true},
	{L"Nimi", 200, 0, 1, 0, true}
};

ColTp SraGridCols[sizeof(SraGridCols0)/sizeof(ColTp)];
static wchar_t erotin[8] = L" ;\t";

//---------------------------------------------------------------------------
__fastcall TFormSeurat::TFormSeurat(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	memcpy(SraGridCols, SraGridCols0, sizeof(SraGridCols0));
	SeuraLuett.NSeura = 0;
	SramaxRow = 10000;
	SraFName = UnicodeString(L"");
}
//---------------------------------------------------------------------------
__fastcall TFormSeurat::~TFormSeurat(void)
{
}
//---------------------------------------------------------------------------
bool __fastcall TFormSeurat::SeuraHakuOn(void)
{
	return(SeuraLuett.NSeura > 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormSeurat::KirjSeurat(UnicodeString FName)
{
	if (FName.Length() == 0) {
		SaveDialog1->InitialDir = FormMain->CurrentDir;
		SaveDialog1->FileName = UnicodeString(seuraTiedNm);
		SaveDialog1->DefaultExt = L"csv";
		SaveDialog1->Filter = L"Teksti/csv-tiedosto (*.csv, *.txt)|*.csv;*.txt|Kaikki tiedostot (*.*)|*.*";
		if (SaveDialog1->Execute())
			FName = SaveDialog1->FileName;
		}
	if (FName.Length() > 0) {
		SeuraLuett.maakoodit = Maakooditkytss1->Checked;
		SeuraLuett.kirjSeurat(FName.c_str());
/*
		TextFl *OutFile = new TextFl(FName.c_str(), L"w");
		if (OutFile->IsOpen()) {
			wchar_t Buf[1000], *p;

			for (int i = 0; i < SeuraLuett.NSeura; i++) {
				for (p = SeuraLuett.Seura[i].lyhenne; *p; p++) {
					if (*p == L' ')
						*p = L'_';
					}
				if (wcslen(SeuraLuett.Seura[i].lyhenne) == 0) {
					wcsncpy(SeuraLuett.Seura[i].lyhenne, SeuraLuett.Seura[i].nimi, kilpparam.lseuralyh);
					SeuraLuett.Seura[i].lyhenne[kilpparam.lseuralyh-1] = 0;
					if (wcslen(SeuraLuett.Seura[i].lyhenne) == 0)
						wcscpy(SeuraLuett.Seura[i].lyhenne, L"_");
					}
				if (wcslen(SeuraLuett.Seura[i].nimi) == 0) {
					wcscpy(SeuraLuett.Seura[i].nimi, SeuraLuett.Seura[i].lyhenne);
					}
				if (Maakooditkytss1->Checked)
					swprintf(Buf, L"%d\t%s\t%s\t%s\n", SeuraLuett.Seura[i].piiri, SeuraLuett.Seura[i].maa, SeuraLuett.Seura[i].lyhenne, SeuraLuett.Seura[i].nimi);
				else
					swprintf(Buf, L"%d\t%s\t%s\n", SeuraLuett.Seura[i].piiri, SeuraLuett.Seura[i].lyhenne, SeuraLuett.Seura[i].nimi);
				OutFile->WriteLine(Buf);
				}
			OutFile->Close();
			}
		delete OutFile;
*/
		}
	SetCurrentDir(FormMain->CurrentDir);
}

//---------------------------------------------------------------------------

int __fastcall TFormSeurat::LueSeurat(UnicodeString FName)
{
	bool otsrivi = false;

	SeuraLuett.nollaa();

	if (FName.Length() == 0) {
		OpenDialog1->InitialDir = FormMain->CurrentDir;
		OpenDialog1->FileName = UnicodeString(L"seuraTiedNm");
		OpenDialog1->DefaultExt = L"csv";
		OpenDialog1->Filter = L"Teksti/csv-tiedosto (*.csv, *.txt)|*.csv;*.txt|Kaikki tiedostot (*.*)|*.*";
		if (OpenDialog1->Execute()) {
			FName = OpenDialog1->FileName;
			}
		}
	if (FName.Length() > 0) {
		SeuraLuett.maakoodit = Maakooditkytss1->Checked;
		SeuraLuett.otsrivi = otsrivi;
		SeuraLuett.jarj = kilpparam.seuralyh_on ? 2 : 1;
		wcsncpy(SeuraLuett.erotin, erotin, sizeof(SeuraLuett.erotin)/2-1);
		SeuraLuett.lueSeurat(FName.c_str());
		}
/*
	EnterCriticalSection(&seura_CriticalSection);
	if (FName.Length() > 0) {
		TextFl *InFile = new TextFl(FName.c_str(), L"t");
		if (InFile->IsOpen()) {
			wchar_t Buf[1000];
			int N = 0;

			SraFName = FName;
			while (!InFile->Feof()) {
				if (InFile->ReadLine(Buf, 998) && wcslen(Buf) > 5) {
					N++;
					}
				}
			if (N > 0) {
				bool ensrivi = true;

				NSeuraMax = N+1000;
				Seurat = new seuraTieto[NSeuraMax];
				InFile->Rewind();
				NSeura = 0;
				while (!InFile->Feof()) {
					if (InFile->ReadLine(Buf, 998) && wcslen(Buf) > 5) {
						if (ensrivi && otsrivi) {
							ensrivi = false;
							continue;
							}
						if (!Seurat[NSeura].tulkRivi(Buf, maakoodit))
							NSeura++;
						}
					}
				}
			InFile->Close();
			}
		delete InFile;
		}
	if (NSeura > 1) {
		quicksort((char *)Seurat, NSeura, sizeof(Seurat[0].key), sizeof(Seurat[0]));
		}
	aktSeura = 0;
	LeaveCriticalSection(&seura_CriticalSection);
*/
	aktSeura = 0;
	SetCurrentDir(FormMain->CurrentDir);
	return(0);
}
//---------------------------------------------------------------------------

int __fastcall TFormSeurat::NaytaSeurat(wchar_t *alain, wchar_t *ylain)
{
	wchar_t ch, cs, ch2, ct, line[80], trk;
	int i, n, d, ep, rvali, sj;
	int tl, tlsum;
	char hkey[30];
	seuraTieto *pSeura;
	static int edEnsSeura;
	static wchar_t ala[LSNIMI+1], yla[LSNIMI+6];

	if (SeuraLuett.NSeura == 0)
		return(0);

	EnterCriticalSection(&seura_CriticalSection);
	if (alain) {
		wcsncpy(ala, alain, LSNIMI);
		}
	if (ylain) {
	   wcsncpy(yla, ylain, LSNIMI+4);
	}
	memset(line, 0, sizeof(line));
	SraGrid->RowCount = 2;
	SraGrid->DefaultRowHeight = 20 * Screen->PixelsPerInch / 96;
	SraGrid->ColCount = 2;
	SraGridCols[1].Visible = Maakooditkytss1->Checked;
	n = 0;
	for (int i = 0; i < sizeof(SraGridCols0)/sizeof(ColTp); i++) {
		if (SraGridCols[i].Visible) {
			if (n+1 > SraGrid->ColCount)
				SraGrid->ColCount = n+1;
			SraGridCols[i].Order = n;
			SraGrid->Cells[n][0] = UnicodeString(SraGridCols[i].Caption);
			SraGrid->Cells[n][1] = UnicodeString(L"");
			SraGrid->ColWidths[n] = SraGridCols[i].Width * Screen->PixelsPerInch / 96;
			n += 1;
			}
		}
	n = 0;
	ensSeura = -1;
	pSeura = SeuraLuett.Seura;
	if (ala && *ala) {
		wcscpy(line, ala);
		keyfromwname(hkey, line, sizeof(hkey), 0);
		pSeura = (seuraTieto *) bsrch(hkey, (char *) SeuraLuett.Seura, &SeuraLuett.NSeura, sizeof(SeuraLuett.Seura[0].key), sizeof(SeuraLuett.Seura[0]));
		}
	for (int k = pSeura-SeuraLuett.Seura; k < SeuraLuett.NSeura; k++) {
		if (n > SramaxRow)
			break;
		if (yla && *yla) {
			wcscpy(line, yla);
			keyfromwname(hkey, line, sizeof(hkey), 0);
			if (memcmp(SeuraLuett.Seura[k].key, hkey, sizeof(hkey)) > 0)
				break;
			}
		if (ensSeura < 0) {
			ensSeura = k;
			if (ensSeura != edEnsSeura)
				aktSeura = ensSeura;
			}
		n++;
		if (SraGrid->RowCount < n+1)
			SraGrid->RowCount = n+1;
		if (k == aktSeura)
			SraGrid->Row = n;
		for (int i = 0; i < sizeof(SraGridCols0)/sizeof(ColTp); i++) {
			if (SraGridCols[i].Visible) {
				int col = SraGridCols[i].Order;

				switch (i) {
					case 0:
						SraGrid->Cells[col][n] = UnicodeString(SeuraLuett.Seura[k].piiri);
						break;
					case 1:
						SraGrid->Cells[col][n] = UnicodeString(SeuraLuett.Seura[k].maa);
						break;
					case 2:
						SraGrid->Cells[col][n] = UnicodeString(SeuraLuett.Seura[k].lyhenne);
						break;
					case 3:
						SraGrid->Cells[col][n] = UnicodeString(SeuraLuett.Seura[k].nimi);
						break;
					}
				}
			}
		}
	NEsilla = n;
	edEnsSeura = ensSeura;
	LeaveCriticalSection(&seura_CriticalSection);
	return(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormSeurat::Luetiedot1Click(TObject *Sender)
{
	LueSeurat(L"");
	NaytaSeurat(L"", L"");
}
//---------------------------------------------------------------------------
int __fastcall TFormSeurat::Seuratiedot(int KSeura, int *piiri, wchar_t *maa,
	wchar_t *lyhenne, wchar_t *nimi)
{
	KSeura += aktSeura;
	if (KSeura < 0 || KSeura > SeuraLuett.NSeura || NEsilla < 1) {
		return(-1);
		}
	if (piiri)
		*piiri = SeuraLuett.Seura[KSeura].piiri;
	if (maa)
		wcscpy(maa, SeuraLuett.Seura[KSeura].maa);
	if (lyhenne)
		wcscpy(lyhenne, SeuraLuett.Seura[KSeura].lyhenne);
	if (nimi)
		wcscpy(nimi, SeuraLuett.Seura[KSeura].nimi);
	aktSeura = KSeura;
	NaytaSeurat(NULL, NULL);
	return(KSeura);
}

void __fastcall TFormSeurat::FormResize(TObject *Sender)
{
	SraGrid->Width = FormSeurat->ClientWidth - 8 * Screen->PixelsPerInch / 96;
	SraGrid->Height = FormSeurat->ClientHeight - SraGrid->Top - 4 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------

void __fastcall TFormSeurat::Lyhennejarjestys1Click(TObject *Sender)
{
	Lyhennejarjestys1->Checked = !Lyhennejarjestys1->Checked;
}
//---------------------------------------------------------------------------
void _fastcall TFormSeurat::TallennaSeura(int piiri, wchar_t *maa,
		wchar_t *lyhenne, wchar_t *nimi, int kielto)
{
	SeuraLuett.TallennaSeura(piiri, maa, lyhenne, nimi, kielto);
	if (kielto == 0)
		NaytaSeurat(nimi, L"ööö");
	else
		NaytaSeurat(NULL, NULL);
}

void __fastcall TFormSeurat::Sallimuutokset1Click(TObject *Sender)
{
	SraGrid->Options << goEditing;
}
//---------------------------------------------------------------------------

void __fastcall TFormSeurat::allennamuutokset1Click(TObject *Sender)
{
	int r = SraGrid->Row;

	EnterCriticalSection(&seura_CriticalSection);
	for (int k = 0; k < SeuraLuett.NSeura; k++) {
		for (int i = 0; i < sizeof(SraGridCols0)/sizeof(ColTp); i++) {
			if (SraGridCols[i].Visible) {
				int col = SraGridCols[i].Order;

				switch (i) {
					case 0:
						SeuraLuett.Seura[k].piiri = _wtoi(SraGrid->Cells[col][k+1].c_str());
						break;
					case 1:
						wcsncpy(SeuraLuett.Seura[k].maa, SraGrid->Cells[col][k+1].c_str(), 3);
						break;
					case 2:
						wcsncpy(SeuraLuett.Seura[k].lyhenne, SraGrid->Cells[col][k+1].c_str(), kilpparam.lseuralyh);
						break;
					case 3:
						wcsncpy(SeuraLuett.Seura[k].nimi, SraGrid->Cells[col][k+1].c_str(), kilpparam.lseura);
						break;
					}
				}

			}
		}
	SraGrid->Options >> goEditing;
	LeaveCriticalSection(&seura_CriticalSection);
	KirjSeurat(SraFName);
	LueSeurat(SraFName);
	NaytaSeurat(NULL, NULL);
	if (r <= SraGrid->RowCount) {
		SraGrid->Row = r;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormSeurat::Sulje1Click(TObject *Sender)
{
	SeuraLuett.nollaa();
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormSeurat::Peruutamuutokset1Click(TObject *Sender)
{
	SraGrid->Options >> goEditing;
	LueSeurat(SraFName);
	NaytaSeurat(NULL, NULL);
}
//---------------------------------------------------------------------------


void __fastcall TFormSeurat::Lisseura1Click(TObject *Sender)
{
	SraGrid->RowCount++;
	SeuraLuett.NSeura++;
	SraGrid->Row = SraGrid->RowCount - 1;
	SraGrid->Cells[0][SraGrid->Row] = UnicodeString(0);
	SraGrid->Options << goEditing;
}
//---------------------------------------------------------------------------
void __fastcall TPPGrid::DeleteRow(int ARow)
{
	TStringGrid::DeleteRow(ARow);
}
//---------------------------------------------------------------------------
void __fastcall TFormSeurat::Poistaseura1Click(TObject *Sender)
{
	TPPGrid *PPGrid = (TPPGrid *) SraGrid;

	int r = SraGrid->Row;
	PPGrid->DeleteRow(r);
	SeuraLuett.NSeura--;
	if (r < SraGrid->RowCount) {
		SraGrid->Row = r;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormSeurat::allennanimell1Click(TObject *Sender)
{
	KirjSeurat(L"");
}
//---------------------------------------------------------------------------

void __fastcall TFormSeurat::Maakooditkytss1Click(TObject *Sender)
{
	Maakooditkytss1->Checked = !Maakooditkytss1->Checked;
}

/*
int wcscmpU2(const wchar_t *s1, const wchar_t *s2)
{
	for (; *s1 || *s2; s1++, s2++) {
		wchar_t c1, c2;
		c1 = towupper2(*s1);
		c2 = towupper2(*s2);
		if (c1 != c2)
			return(c1 - c2);
		}
	return(0);
}
*/

//---------------------------------------------------------------------------
bool __fastcall TFormSeurat::HaeTiedot(bool lyhhaku, int *piiri, wchar_t *maa,
	wchar_t *lyhenne, wchar_t *nimi, bool vapaakoko)
{
	_wsetlocale(LC_CTYPE, L"fi-FI");
	for (int i = 0; i < SeuraLuett.NSeura; i++) {
		if (lyhhaku) {
			if (vapaakoko) {
				if (wcscmpU(lyhenne, SeuraLuett.Seura[i].lyhenne))
					continue;
				}
			else  {
				if (wcscmp(lyhenne, SeuraLuett.Seura[i].lyhenne))
					continue;
				}
			}
		else {
			if (vapaakoko) {
				if (wcscmpU(nimi, SeuraLuett.Seura[i].nimi))
					continue;
				}
			else {
				if (wcscmp(nimi, SeuraLuett.Seura[i].nimi))
					continue;
				}
			}
		wcsncpy(nimi, SeuraLuett.Seura[i].nimi, kilpparam.lseura);
		wcsncpy(lyhenne, SeuraLuett.Seura[i].lyhenne, kilpparam.lseuralyh);
		wcsncpy(maa, SeuraLuett.Seura[i].maa, 3);
		*piiri = SeuraLuett.Seura[i].piiri;
		return(false);
		}
	return(true);
}

void __fastcall TFormSeurat::Help1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Seuraluettelo");
}
//---------------------------------------------------------------------------

void __fastcall TFormSeurat::FormShow(TObject *Sender)
{
	if (ilmkentat > 0 && SeuraLuett.NSeura == 0) {
		SetCurrentDir(FormMain->CurrentDir);
		LueSeurat(UnicodeString(seuraTiedNm));
		NaytaSeurat(NULL, NULL);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormSeurat::FormClose(TObject *Sender, TCloseAction &Action)
{
	seurahaku = -2;
}
//---------------------------------------------------------------------------

void __fastcall TFormSeurat::SraGridMouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SraGrid->RowCount - SraGrid->FixedRows >= SraGrid->VisibleRowCount + SraGrid->TopRow)
		SraGrid->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormSeurat::SraGridMouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SraGrid->TopRow > SraGrid->FixedRows)
		SraGrid->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormSeurat::Selaa(int haku)
{
	if (haku > 0 && aktSeura < NSeura)
		aktSeura++;
	if (haku < 0 && aktSeura > 0)
		aktSeura--;
	NaytaSeurat(NULL, NULL);
	SraGrid->Options << goEditing;
	SraGrid->SetFocus();
}

//---------------------------------------------------------------------------

void __fastcall TFormSeurat::SraGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	aktSeura = ensSeura + SraGrid->Row - 1;
	if (Caller == NULL)
		return;
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)
		&& SraGrid->Row > 0) {
		if (Caller == FormIlm2) {
			SraGrid->Options >> goEditing;
			FormIlm2->PalaaSeuraHausta(true);
			}
		if (Caller == FormEmit) {
			SraGrid->Options >> goEditing;
			FormEmit->PalaaSeuraHausta(true);
			}
		return;
		}
	if (Key == VK_ESCAPE) {
		if (Caller == FormIlm2) {
			SraGrid->Options >> goEditing;
			FormIlm2->PalaaSeuraHausta(false);
			}
		if (Caller == FormEmit) {
			SraGrid->Options >> goEditing;
			FormEmit->PalaaSeuraHausta(false);
			}
		return;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSeurat::SraGridDblClick(TObject *Sender)
{
	aktSeura = ensSeura + SraGrid->Row - 1;
	if (Caller == NULL)
		return;
	if (Caller == FormIlm2) {
		SraGrid->Options >> goEditing;
		FormIlm2->PalaaSeuraHausta(true);
		}
	if (Caller == FormEmit) {
		SraGrid->Options >> goEditing;
		FormEmit->PalaaSeuraHausta(true);
		}
	return;
}
//---------------------------------------------------------------------------

