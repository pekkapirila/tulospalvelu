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

#include "UnitSarjatiedot.h"
#include "UnitSarja.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSarjatiedot *FormSarjatiedot;
//extern TFormSarja *FormSarja;
//extern TFormVaPisteet *FormVaPisteet;

ColTp GridCols0[] = {
	{L"Nro", 30, 0, 1, 0, true},
	{L"Sarja", 55, 0, 1, 0, true},
	{L"AlkuNo", 55, 0, 1, 0, true},
	{L"Osuusluku", 55, 0, 1, 0, true},
	{L"Henk.lkm", 55, 0, 1, 0, true},
	{L"Lähtöaika", 60, 0, 1, 0, true},
	{L"Sakollinen", 60, 0, 1, 0, false},
	{L"Lähtöpaikka", 40, 0, 1, 0, false},
	{L"Maali", 40, 0, 1, 0, false},
	{L"Osuuslähtö", 60, 0, MAXOSUUSLUKU, 0, false},
	{L"Lähtöväli", 45, 0, MAXOSUUSLUKU, 0, false},
	{L"Amp.paikat", 60, 0, MAXOSUUSLUKU, 0, false},
	{L"Sakkoyks.", 60, 0, MAXOSUUSLUKU, 0, false},
	{L"Maksu", 60, 0, 1, 0, true},
	{L"Maksu-ale", 60, 0, 1, 0, false},
	{L"Matka", 60, 0, MAXOSUUSLUKU, 0, true},
	{L"Pitkä nimi", 180, 0, 1, 0, true},
	{L"Lukum", 40, 0, 1, 0, false}
};

ColTp *GridCols1;

ColTp GridColsArv[] = {
	{L"Nro", 30, 0, 1, 0, true},
	{L"Sarja", 55, 0, 1, 0, true},
	{L"AlkuNo", 55, 0, 1, 0, true},
	{L"Osuusluku", 55, 0, 1, 0, true},
	{L"Henk.lkm", 55, 0, 1, 0, true},
	{L"Lähtöaika", 60, 0, 1, 0, true},
	{L"Sakollinen", 60, 0, 1, 0, false},
	{L"Lähtöpaikka", 40, 0, 1, 0, false},
	{L"Maali", 40, 0, 1, 0, false},
	{L"Osuuslähtö", 60, 0, MAXOSUUSLUKU, 0, false},
	{L"Lähtöväli", 45, 0, MAXOSUUSLUKU, 0, false},
	{L"Amp.paikat", 60, 0, MAXOSUUSLUKU, 0, false},
	{L"Sakkoyks.", 60, 0, MAXOSUUSLUKU, 0, false},
	{L"Maksu", 60, 0, 1, 0, true},
	{L"Maksu-ale", 60, 0, 1, 0, false},
	{L"Matka", 60, 0, MAXOSUUSLUKU, 0, true},
	{L"Pitkä nimi", 180, 0, 1, 0, true},
	{L"Lukum", 40, 0, 1, 0, false}
};

static int GridCols1Count;
static int GridColsArvCount;
typedef struct {
	char   key[8];
	int    srj;
} srjjrjtp;

static srjjrjtp srjjrj[MAXSARJALUKU];
static int jrjsrj[MAXSARJALUKU];

static void teesarjajarj(int jrj)
{
	switch (jrj) {
		case 0:
			for (int i = 0; i < MAXSARJALUKU; i++) {
				jrjsrj[i] = i;
				srjjrj[i].srj = i;
				}
			break;
		case 1:
			for (int i = 0; i < sarjaluku; i++) {
				memset(srjjrj[i].key, 0, sizeof(srjjrj[i].key));
				*(int *)srjjrj[i].key = Sarjat[i].sarjaalku;
				lswab((long *)srjjrj[i].key);
				srjjrj[i].srj = i;
				}
			quicksort((char *)srjjrj, sarjaluku, sizeof(srjjrj[0].key), sizeof(srjjrj[0]));
			for (int i = 0; i < sarjaluku; i++) {
				jrjsrj[srjjrj[i].srj] = i;
				}
			break;
		case 2:
			for (int i = 0; i < sarjaluku; i++) {
				memset(srjjrj[i].key, 0, sizeof(srjjrj[i].key));
				*(int *)(srjjrj[i].key) = Sarjat[i].lno;
				*(int *)(srjjrj[i].key+4) = Sarjat[i].lahto+t0*TUNTI;
				lswab((long *)srjjrj[i].key);
				lswab((long *)(srjjrj[i].key+4));
				srjjrj[i].srj = i;
				}
			quicksort((char *)srjjrj, sarjaluku, sizeof(srjjrj[0].key), sizeof(srjjrj[0]));
			for (int i = 0; i < sarjaluku; i++) {
				jrjsrj[srjjrj[i].srj] = i;
				}
			break;
		}
 }

//---------------------------------------------------------------------------
__fastcall TFormSarjatiedot::TFormSarjatiedot(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	GridCols1 = new ColTp[sizeof(GridCols0)/sizeof(ColTp)];
	memcpy(GridCols1, GridCols0, sizeof(GridCols0));

	switch (kilpparam.kilplaji) {
		case L'S':
//			GridCols1[9].Visible = true;
			GridCols1[11].Visible = false;
			GridCols1[12].Visible = true;
			break;
		case L'H':
//			GridCols1[9].Visible = false;
			GridCols1[11].Visible = false;
			GridCols1[12].Visible = false;
			break;
		case L'A':
//			GridCols1[9].Visible = false;
			GridCols1[11].Visible = true;
			GridCols1[12].Visible = true;
			break;
		}
	GridCols1[4].Visible = kilpparam.maxnosuus > 1;

	Sakollisuus1->Checked = GridCols1[6].Visible;
	Lhtpaikka1->Checked = GridCols1[7].Visible;
	Maali1->Checked = GridCols1[8].Visible;
	Lhtvli1->Checked = GridCols1[10].Visible;
	Ampumapaikat1->Checked = GridCols1[11].Visible;
	Sakkoyksikk1->Checked = GridCols1[12].Visible;
	Maksut1->Checked = GridCols1[13].Visible;
	Matka1->Checked = GridCols1[15].Visible;
	Pitknimi1->Checked = GridCols1[16].Visible;

	teesarjajarj(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::NaytaSarjat(void)
{
	wchar_t St[20];
	ColTp *GridCols;

	LblMuokkaus->Enabled = SG1->Options.Contains(goEditing);
	LblMuokkaus->Visible = LblMuokkaus->Enabled;

	SG1->DefaultRowHeight = 20 * Screen->PixelsPerInch / 96;
	if (CBTiedot->ItemIndex == 0) {
		GridCols = GridCols1;
		if (SG1->FixedCols > GridCols1Count)
			SG1->FixedCols = GridCols1Count;
		SG1->ColCount = GridCols1Count;
		}
	else {
		GridCols = GridColsArv;
		if (SG1->FixedCols > GridColsArvCount)
			SG1->FixedCols = GridColsArvCount;
		SG1->ColCount = GridColsArvCount;
		}
	teesarjajarj(CBJarj->ItemIndex);

	for (int ic = 0; ic < sizeof(GridCols0)/sizeof(ColTp); ic++) {
		if (GridCols[ic].Visible) {
			int n = 1;
			if (GridCols[ic].Count == MAXOSUUSLUKU)
				n = kilpparam.ntosuus;
			for (int i = 0; i < n; i++) {
				SG1->Cells[GridCols[ic].Order+i][0] = UnicodeString(GridCols[ic].Caption);
				SG1->ColWidths[GridCols[ic].Order+i] = GridCols[ic].Width * Screen->PixelsPerInch / 96;
				}
			}
		}

	SG1->RowCount = sarjaluku+1;
	for (int ir = 1; ir <= sarjaluku; ir++) {
		int srj;

		srj = srjjrj[ir-1].srj;
		for (int ic = 0; ic < sizeof(GridCols0)/sizeof(ColTp); ic++) {
			if (GridCols[ic].Visible) {
				int n = 1, col;
				if (GridCols[ic].Count == MAXOSUUSLUKU)
					n = kilpparam.ntosuus;
				col = GridCols[ic].Order;
				ColIx[col] = ic;
				ColPv[col] = 0;
				switch (ic) {
					case 0:
						SG1->Cells[col][ir] = UnicodeString(srj+1);
						break;
					case 1:
						SG1->Cells[col][ir] = UnicodeString(Sarjat[srj].sarjanimi);
						break;
					case 2:
						SG1->Cells[col][ir] = UnicodeString(Sarjat[srj].sarjaalku);
						break;
					case 3:
						SG1->Cells[col][ir] = UnicodeString(Sarjat[srj].ntosuus);
						break;
					case 4:
						SG1->Cells[col][ir] = UnicodeString(Sarjat[srj].osuusluku);
						break;
					case 5:
						AIKATOWSTRS(St, Sarjat[srj].lahto, t0);
						St[8] = 0;
						SG1->Cells[col][ir] = UnicodeString(St);
						break;
					case 6:
						SG1->Cells[col][ir] = UnicodeString(Sarjat[srj].taslaji/10) + L"/" +
							UnicodeString(Sarjat[srj].taslaji%10);
						break;
					case 7:
						SG1->Cells[col][ir] = UnicodeString(Sarjat[srj].lno);
						break;
					case 8:
						SG1->Cells[col][ir] = UnicodeString(Sarjat[srj].lno);
						break;
					case 9:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							AIKATOWSTRS(St, Sarjat[srj].ylahto[i], t0);
							St[8] = 0;
							SG1->Cells[col+i][ir] = UnicodeString(St);
							}
						break;
					case 10:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = UnicodeString(Sarjat[srj].lahtovali[i]);
							}
						break;
					case 11:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = UnicodeString(Sarjat[srj].paikat[i]);
							}
						break;
					case 12:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = UnicodeString(Sarjat[srj].tsak[i]) + L"/" +
								UnicodeString(Sarjat[srj].amptsak[i]);
							}
						break;
					case 13:
						swprintf(St, L"%d,%02d", Sarjat[srj].maksu/100, Sarjat[srj].maksu%100);
						SG1->Cells[col][ir] = UnicodeString(St);
						break;
					case 14:
						swprintf(St, L"%d,%02d", Sarjat[srj].maksual/100, Sarjat[srj].maksual%100);
						SG1->Cells[col][ir] = UnicodeString(St);
						break;
					case 15:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = UnicodeString(Sarjat[srj].matka[i]);
							}
						break;
					case 16:
						SG1->Cells[col][ir] = UnicodeString(Sarjat[srj].psarjanimi);
						break;
					case 17:
						for (int i = 0; i < n; i++) {
							ColIx[col+i] = ic;
							ColPv[col+i] = i;
							SG1->Cells[col+i][ir] = UnicodeString(nilm[srj]);
							}
						break;
					}
				}
			}
		}

}
//---------------------------------------------------------------------------
int __fastcall TFormSarjatiedot::paivitaMuutos(int col, int row)
{
	int i, k, srj, ipv;
	int tl;
	wchar_t line[20], kh;
	sarjatietue Srj;
	ColTp *GridCols;

	if (CBTiedot->ItemIndex == 0) {
		GridCols = GridCols1;
		}
	else {
		GridCols = GridColsArv;
		}
	k = row;
	srj = _wtoi(SG1->Cells[0][k].c_str()) - 1;
	ipv = 0;
	for (i = col-1; i > 0 && ColIx[i] == ColIx[col]; i--)
		ipv++;
	switch (ColIx[col]) {
		case 1:
			SG1->Cells[col][k] = UnicodeString(upcasewstr(SG1->Cells[col][k].SubString(1, LSARJA).c_str()));
			break;
		case 0:
		case 2:
		case 7:
		case 8:
		case 10:
		case 11:
		case 12:
			SG1->Cells[col][k] = UnicodeString(_wtoi(SG1->Cells[col][k].c_str()));
			break;
		case 3:
			if (kilpparam.maxnosuus > 1)
				SG1->Cells[col][k] = Sarjat[srj].ntosuus;
			else
				SG1->Cells[col][k] = UnicodeString(_wtoi(SG1->Cells[col][k].c_str()));
			break;
		case 4:
			SG1->Cells[col][k] = Sarjat[srj].osuusluku;
			break;
		case 5:
		case 9:
			wcsncpy(line,SG1->Cells[col][k].c_str(), 10);
			line[10] = 0;
			elimwbl(line);
			if (line[0] == 0 || line[0] == L':') {
				tl = TMAALI0;
				}
			else
				tl = wstrtoaika_vap(SG1->Cells[col][k].c_str(), t0);
			SG1->Cells[col][k] = UnicodeString(AIKATOWSTRS(line, tl, t0)).SubString(1, 8);
			break;
		case 13:
		case 14:
			tl = wcstomaksu(SG1->Cells[col][k].c_str());
			swprintf(line, L"%d,%02d", tl/100, tl%100);
			SG1->Cells[col][k] = UnicodeString(line);
			break;
		case 15:
			SG1->Cells[col][k] = SG1->Cells[col][k].SubString(1, sizeof(Sarjat[0].matka[0])-1);
			break;
		case 16:
			SG1->Cells[col][k] = SG1->Cells[col][k].SubString(1, LPSARJA);
			break;
		}
	return(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::TallennaSarjat(void)
{
	wchar_t St[20];
	ColTp *GridCols;
	int kk;

	if (CBTiedot->ItemIndex == 0) {
		GridCols = GridCols1;
		}
	else {
		GridCols = GridColsArv;
		}
	SG1->RowCount = sarjaluku+1;
	maaraaOsuusluku(true);
	for (int ir = 1; ir <= sarjaluku; ir++) {
		int srj;

		srj = srjjrj[ir-1].srj;
		for (int ic = 1; ic < sizeof(GridCols0)/sizeof(ColTp); ic++) {
			if (GridCols[ic].Visible) {
				int n = 1, col;
				if (GridCols[ic].Count == MAXOSUUSLUKU)
					n = kilpparam.ntosuus;
				col = GridCols[ic].Order;
				switch (ic) {
					case 1:
						wcstoansi(Sarjat[srj].sarjanimi, SG1->Cells[col][ir].c_str(), LSARJA);
						break;
					case 2:
						Sarjat[srj].sarjaalku = SG1->Cells[col][ir].ToInt();
						break;
					case 3:
						if (kilpparam.maxnosuus == 1) {
							Sarjat[srj].ntosuus = SG1->Cells[col][ir].ToInt();
							if (Sarjat[srj].ntosuus > kilpparam.n_os_akt) {
								Application->MessageBoxW((UnicodeString(L"Kilpailun osuusluvuksi on määritelty ")+
									UnicodeString(kilpparam.n_os_akt)).c_str(), L"Rajoitus", MB_OK);
								Sarjat[srj].ntosuus = kilpparam.n_os_akt;
								}
							Sarjat[srj].osuusluku = Sarjat[srj].ntosuus;
							}
						break;
					case 4:
						break;
					case 5:
						Sarjat[srj].lahto = wstrtoaika_vap(SG1->Cells[col][ir].c_str(), t0);
						break;
					case 6:
						Sarjat[srj].taslaji = 0;
						if (isdigit(SG1->Cells[col][ir][1]))
							Sarjat[srj].taslaji = 10*(SG1->Cells[col][ir][1]-L'0');
						if (isdigit(SG1->Cells[col][ir][3]))
							Sarjat[srj].taslaji += SG1->Cells[col][ir][3]-L'0';
						break;
					case 7:
						Sarjat[srj].lno = SG1->Cells[col][ir].ToInt();
						break;
					case 8:
						Sarjat[srj].mno = SG1->Cells[col][ir].ToInt();
						break;
					case 9:
						for (int i = 0; i < n; i++)
							Sarjat[srj].ylahto[i] = wstrtoaika_vap(SG1->Cells[col+i][ir].c_str(), t0);
						break;
					case 10:
						for (int i = 0; i < n; i++)
							Sarjat[srj].lahtovali[i] = SG1->Cells[col+i][ir].ToInt();
						break;
					case 11:
						for (int i = 0; i < n; i++)
							Sarjat[srj].paikat[i] = SG1->Cells[col+i][ir].ToInt();
						break;
					case 12:
						for (int i = 0; i < n; i++) {
							Sarjat[srj].tsak[i] = _wtoi(SG1->Cells[col+i][ir].c_str());
							if ((kk = SG1->Cells[col+i][ir].Pos(L"/")) > 0)
								Sarjat[srj].amptsak[i] = _wtoi(SG1->Cells[col+i][ir].SubString(kk+1, 4).c_str());
							else
								Sarjat[srj].amptsak[i] = 0;
							}
						break;
					case 13:
						Sarjat[srj].maksu = wcstomaksu(SG1->Cells[col][ir].c_str());
						break;
					case 14:
						Sarjat[srj].maksual = wcstomaksu(SG1->Cells[col][ir].c_str());
						break;
					case 15:
						for (int i = 0; i < n; i++)
							wcstoansi(Sarjat[srj].matka[i], SG1->Cells[col+i][ir].c_str(), sizeof(Sarjat[0].matka[0])-1);
						break;
					case 16:
						wcstoansi(Sarjat[srj].psarjanimi, SG1->Cells[col][ir].c_str(), sizeof(Sarjat[0].psarjanimi)-1);
						break;
					}
				}
			}
		}
	kirjEventXml(sarjaxmlfname, true, false, false);
	maaraaOsuusluku(true);
	if (ToimintaTila == 2) {
		for (int isrj = 0; isrj < sarjaluku; isrj++) {
			if (Sarjat_0[isrj].lahto != Sarjat[isrj].lahto)
				laheta_yl(-isrj-1, 0);
			}
		}
//	kirjsarjat();
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::FormResize(TObject *Sender)
{
	SG1->Width = ClientWidth - 16 * Screen->PixelsPerInch / 96;
	SG1->Height = ClientHeight - 56 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::Button3Click(TObject *Sender)
{
	NaytaSarjat();
	SG1->Options >> (goEditing);
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::Button1Click(TObject *Sender)
{
	SG1->Options << (goEditing);
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::Button2Click(TObject *Sender)
{
	TallennaSarjat();
	SG1->Options >> (goEditing);
	SetColumns();
//	NaytaSarjat();
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::Button4Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::FormClose(TObject *Sender, TCloseAction &Action)

{
	if (SG1->Options.Contains(goEditing)) {
		if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallenna",
			MB_YESNO) == IDYES) {
			TallennaSarjat();
			}
		else
			SG1->Options >> (goEditing);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::SG1DblClick(TObject *Sender)
{

	if (SG1->Options.Contains(goEditing)) {
		if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallenna",
			MB_YESNO) == IDYES) {
			TallennaSarjat();
			}
		}
	if (aktrow > 0 && aktrow <= sarjaluku) {
		if (!FormSarja)
			FormSarja = new TFormSarja(this);

		FormSarja->lataaSarja(SG1->Cells[0][aktrow].ToInt()-1);
		FormSarja->Show();
		if (FormSarja->WindowState == wsMinimized)
			FormSarja->WindowState = wsNormal;
		FormSarja->BringToFront();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::SG1SelectCell(TObject *Sender, int ACol, int ARow,
		  bool &CanSelect)
{
	if (SG1->Options.Contains(goEditing) && aktrow > 0 && aktrow > 0) {
		paivitaMuutos(aktcol, aktrow);
	}
	aktcol = ACol;
	aktrow = ARow;
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::SG1Exit(TObject *Sender)
{
	if (SG1->Options.Contains(goEditing) && aktrow > 0 && aktrow > 0) {
		paivitaMuutos(aktcol, aktrow);
	}
	aktcol = 0;
	aktrow = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::CBTiedotChange(TObject *Sender)
{
	SG1->Options >> (goEditing);
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::CBJarjChange(TObject *Sender)
{
	SG1->Options >> (goEditing);
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Sulje1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------


void __fastcall TFormSarjatiedot::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Sarjat");
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::SG1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	int rr, cc;

	rr = aktrow;
	cc = aktcol;
	if (Key == VK_F9 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (SG1->Options.Contains(goEditing)) {
			if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallenna",
				MB_YESNO) == IDYES) {
				TallennaSarjat();
				}
			SG1->Options >> goEditing;
			}
		else {
			SG1->Options << goEditing;
			}
		NaytaSarjat();
		aktrow = rr;
		aktcol = cc;
		SG1->Row = aktrow;
		SG1->Col = aktcol;
		}
	if (Key == VK_RIGHT && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (SG1->Col < SG1->ColCount-1)
			SG1->Col++;
		}
	if (Key == VK_LEFT && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (SG1->Col > 0)
			SG1->Col--;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::SG1FixedCellClick(TObject *Sender, int ACol, int ARow)

{
	if (ARow > 0)
		aktrow = ARow;
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::SetColumns(void)
{
	int cols = 0;

	GridCols1[4].Visible = kilpparam.maxnosuus > 1;
	GridCols1[6].Visible = Sakollisuus1->Checked;
	GridCols1[7].Visible = Lhtpaikka1->Checked;
	GridCols1[8].Visible = Maali1->Checked;
	GridCols1[10].Visible = Lhtvli1->Checked;
	GridCols1[11].Visible = Ampumapaikat1->Checked;
	GridCols1[12].Visible = Sakkoyksikk1->Checked;
	GridCols1[13].Visible = Maksut1->Checked;
	GridCols1[15].Visible = Matka1->Checked;
	GridCols1[16].Visible = Pitknimi1->Checked;

	for (int ic = 0; ic < sizeof(GridCols0)/sizeof(ColTp); ic++) {
		if (GridCols1[ic].Visible) {
			GridCols1[ic].Order = cols;
			if (GridCols1[ic].Count == MAXOSUUSLUKU)
				cols += kilpparam.ntosuus;
			else
				cols++;
			}
		}

	GridCols1Count = cols;

	cols = 0;
	for (int ic = 0; ic < sizeof(GridCols0)/sizeof(ColTp); ic++) {
		if (GridColsArv[ic].Visible) {
			GridColsArv[ic].Order = cols;
			cols++;
			}
		}

	GridColsArvCount = cols;

	NaytaSarjat();
}
//---------------------------------------------------------------------------


void __fastcall TFormSarjatiedot::Sakollisuus1Click(TObject *Sender)
{
	Sakollisuus1->Checked = !Sakollisuus1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Lhtvli1Click(TObject *Sender)
{
	Lhtvli1->Checked = !Lhtvli1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------


void __fastcall TFormSarjatiedot::Lhtpaikka1Click(TObject *Sender)
{
	Lhtpaikka1->Checked = !Lhtpaikka1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Maali1Click(TObject *Sender)
{
	Maali1->Checked = !Maali1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------


void __fastcall TFormSarjatiedot::Ampumapaikat1Click(TObject *Sender)
{
	Ampumapaikat1->Checked = !Ampumapaikat1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Sakkoyksikk1Click(TObject *Sender)
{
	Sakkoyksikk1->Checked = !Sakkoyksikk1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Maksut1Click(TObject *Sender)
{
	Maksut1->Checked = !Maksut1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Matka1Click(TObject *Sender)
{
	Matka1->Checked = !Matka1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Pitknimi1Click(TObject *Sender)
{
	Pitknimi1->Checked = !Pitknimi1->Checked;
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------
#define FLDID_SRJID 2
#define FLDID_PNIMI 3
#define FLDID_OSLKM 4
#define FLDID_ALKUNO 5
#define FLDID_TLAHTO 6
#define FLDID_LAHTONO 7
#define FLDID_MAALINO 8
#define FLDID_SAKKOLJ 9
#define FLDID_MAKSU 10
#define FLDID_MAKSUAL 11
#define FLDID_MATKA 12
#define FLDID_VALKM 13
#define FLDID_YLAHTO 14
#define FLDID_YLKYNNYS 15
#define FLDID_SAKKOJA 16
#define FLDID_SAKKOYKS 17
#define FLDID_RINNLKM 18
#define FLDID_VAMATKA 19

FLD2 exp_srj_kentat[18] = {
	{L"Nimi", FLDID_SRJID, 1, 1, 2},
	{L"Pitkänimi", FLDID_PNIMI, 1, 1, 3},
	{L"OsLkm", FLDID_OSLKM, 1, 1, 4},
	{L"AlkuNo", FLDID_ALKUNO, 1, 1, 5},
	{L"Lähtöaika", FLDID_TLAHTO, 1, 1, 6},
	{L"LähtöNo", FLDID_LAHTONO, 1, 1, 7},
	{L"MaaliNo", FLDID_MAALINO, 1, 1, 8},
	{L"Sakkolj", FLDID_SAKKOLJ, 1, 1, 9},
	{L"Maksu", FLDID_MAKSU, 1, 1, 10},
	{L"MaksuAl", FLDID_MAKSUAL, 1, 1, 11},
	{L"Matka", FLDID_MATKA, 1, 1, 12},
	{L"VaLkm", FLDID_VALKM, 1, 1, 13},
	{L"YhtLähtö", FLDID_YLAHTO, 1, 1, 14},
	{L"YlKynnys", FLDID_YLKYNNYS, 1, 1, 15},
	{L"Sakkoja", FLDID_SAKKOJA, 1, 1, 16},
	{L"SakYks", FLDID_SAKKOYKS, 1, 1, 17},
	{L"RinnLkm", FLDID_RINNLKM, 1, 1, 18},
	{L"VaMtk", FLDID_VAMATKA, 1, 1, 19}
	};

int Kexp_srj_kentat = 10, Pexp_srj_kentat = 7, Vexp_srj_kentat = 1;

static struct {
	int fld;
	int os;
	int va;
	} fldorder[20+MAXOSUUSLUKU*(8+VAIKALUKU)];
static int nflds;

wchar_t *TFormSarjatiedot::otsikkorivi(wchar_t *line, int len, wchar_t erotin)
{
	wchar_t st[100];

	wmemset(line, 0, len);
	for (int i = 0; i < Kexp_srj_kentat; i++) {
		if (!exp_srj_kentat[i].id || !exp_srj_kentat[i].akt)
			continue;
		swprintf(line+wcslen(line), L"\"%s\"%c", exp_srj_kentat[i].name, erotin);
		}

	for (int ipv = 0; ipv < kilpparam.ntosuus; ipv++) {
		for (int i = Kexp_srj_kentat; i < Kexp_srj_kentat+Pexp_srj_kentat; i++) {
			if (!exp_srj_kentat[i].id || !exp_srj_kentat[i].akt)
				continue;
			swprintf(line+wcslen(line), L"\"%s-%d\"%c", exp_srj_kentat[i].name, ipv+1, erotin);
			}
		for (int iva = 0; iva < kilpparam.maxvaluku; iva++) {
			for (int i = Kexp_srj_kentat+Pexp_srj_kentat; i < Kexp_srj_kentat+Pexp_srj_kentat+Vexp_srj_kentat; i++) {
				if (!exp_srj_kentat[i].id || !exp_srj_kentat[i].akt)
					continue;
				swprintf(line+wcslen(line), L"\"%s-%d-%d\"%c", exp_srj_kentat[i].name, ipv+1, iva+1, erotin);
				}
			}
		}
	line[wcslen(line)-1] = L'\n';
	return(line);
}

//---------------------------------------------------------------------------
wchar_t *TFormSarjatiedot::datarivi(sarjatietue *Sarja, wchar_t *line, int len, wchar_t erotin)
{
	wchar_t st[100];

	wmemset(line, 0, len);
	for (int os = 0; os < kilpparam.ntosuus; os++) {
		for (int i = 0; i < Kexp_kentat+Pexp_kentat; i++) {
			if ((int)wcslen(line) > len-40)
				break;
			if (!exp_srj_kentat[i].id || !exp_srj_kentat[i].akt)
				continue;
			if (os > 0 && i < Kexp_srj_kentat)
				continue;
			memset(st, 0, sizeof(st));
			int fldno = exp_srj_kentat[i].id;
			switch (fldno) {
				case FLDID_SRJID :
					swprintf(line+wcslen(line), L"%s%c", Sarja->Sarjanimi(st), erotin);
					break;
				case FLDID_PNIMI :
					MbsToWcs(st, Sarja->psarjanimi, LPSARJA+1);
					swprintf(line+wcslen(line), L"\"%s\"%c", st, erotin);
					break;
				case FLDID_OSLKM :
					swprintf(line+wcslen(line), L"%d%c", Sarja->ntosuus, erotin);
					break;
				case FLDID_ALKUNO :
					swprintf(line+wcslen(line), L"%d%c", Sarja->sarjaalku, erotin);
					break;
				case FLDID_TLAHTO :
					swprintf(line+wcslen(line), L"%s%c", aikatowstr_cols_n(st, Sarja->lahto, t0, L',', 8), erotin);
					break;
				case FLDID_LAHTONO :
					swprintf(line+wcslen(line), L"%d%c", Sarja->lno, erotin);
					break;
				case FLDID_MAALINO :
					swprintf(line+wcslen(line), L"%d%c", Sarja->mno, erotin);
					break;
				case FLDID_SAKKOLJ :
					swprintf(line+wcslen(line), L"%d%c", Sarja->taslaji, erotin);
					break;
				case FLDID_MAKSU :
					swprintf(line+wcslen(line), L"%d%c", Sarja->maksu, erotin);
					break;
				case FLDID_MAKSUAL :
					swprintf(line+wcslen(line), L"%d%c", Sarja->maksual, erotin);
					break;
				case FLDID_MATKA :
					swprintf(line+wcslen(line), L"%s%c", Sarja->Matka(os, st), erotin);
					break;
				case FLDID_VALKM :
					swprintf(line+wcslen(line), L"%d%c", Sarja->valuku[os], erotin);
					break;
				case FLDID_YLAHTO :
					swprintf(line+wcslen(line), L"%s%c", aikatowstr_cols_n(st, Sarja->ylahto[os], t0, L',', 8), erotin);
					break;
				case FLDID_YLKYNNYS :
					swprintf(line+wcslen(line), L"%s%c", aikatowstr_cols_n(st, Sarja->ylkynnys[os], t0, L',', 8), erotin);
					break;
				case FLDID_SAKKOJA :
					swprintf(line+wcslen(line), L"%d%c", Sarja->paikat[os], erotin);
					break;
				case FLDID_SAKKOYKS :
					swprintf(line+wcslen(line), L"%d%c", Sarja->tsak[os], erotin);
					break;
				case FLDID_RINNLKM :
					swprintf(line+wcslen(line), L"%d%c", Sarja->nosuus[os], erotin);
					break;
				}
			}
		for (int iva = 0; iva < kilpparam.maxvaluku; iva++) {
			for (int i = Kexp_srj_kentat+Pexp_srj_kentat; i < Kexp_srj_kentat+Pexp_srj_kentat+Vexp_srj_kentat; i++) {
				if (!exp_srj_kentat[i].id || !exp_srj_kentat[i].akt)
					continue;
				switch (exp_srj_kentat[i].id) {
					case FLDID_VAMATKA :
						swprintf(line+wcslen(line), L"%s%c", Sarja->vaMatka(os, iva, st), erotin);
						break;
					}
				}
			}
		}
	line[wcslen(line)-1] = L'\n';
	return(line);
}
//---------------------------------------------------------------------------
void __fastcall TFormSarjatiedot::Kirjoitacsvtiedostoon1Click(TObject *Sender)
{
	TextFl *OutFl;
	wchar_t erotin = L';';

	SaveDialog1->FileName = UnicodeString(L"sarjat.csv");
	SaveDialog1->DefaultExt = L"csv";
	SaveDialog1->Filter = L"Csv-tiedostot (*.csv)|*.CSV|Kaikki tiedostot (*.*)|*.*";
	if (SaveDialog1->Execute()) {
		OutFl = new TextFl(SaveDialog1->FileName.c_str(), L"wt");
		if (OutFl->IsOpen()) {
			wchar_t line[400+15*MAXOSUUSLUKU*(8+VAIKALUKU)];

			maaraaOsuusluku(true);
			exp_srj_kentat[FLDID_RINNLKM].akt = kilpparam.maxnosuus > 1;
			otsikkorivi(line, sizeof(line)/2, erotin);
			OutFl->WriteLine(line);
			for (int srj = 0; srj < sarjaluku; srj++) {
				datarivi(Sarjat+srj, line, sizeof(line)/2, erotin);
				OutFl->WriteLine(line);
				}
			}
		delete OutFl;
		}
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------
int TFormSarjatiedot::lue_sarjacsv(TextFl *afile, sarjatietue *Sarja, int lineno, int toiminto, wchar_t erotin)
{
	wchar_t rivi[400+15*MAXOSUUSLUKU*(8+VAIKALUKU)],s[40], *p, *p1, *pp;
	INT16 i,il,res = 0;
	INT16 srj, l, kbg;

	wchar_t *fields[20+MAXOSUUSLUKU*(8+VAIKALUKU)], st[100] = L"", st1[100] = L"", st2[100] = L"", erottimet[] = L",";
	int nfld = 0, k = 0, ret = 0, nRivi = 9;
	char ast[100];

	erottimet[0] = erotin;
	memset(fields, 0, sizeof(fields));

	if (afile->ReadLine(rivi, sizeof(rivi)/2-1) == NULL)
		return(-1);
	if (wcslen(rivi) < 4)
		return(-1);
	Sarja->nollaa();
	nfld = getfields(rivi, fields, sizeof(fields)/sizeof(fields[0]), erottimet,
		L"\"" , true, false);
	if (lineno < 0) {
		memset(fldorder, 0, sizeof(fldorder));
		nflds = 0;
		for (UINT ifld = 0; ifld < nfld; ifld++) {
			int i = 0;
			int os = 0;
			int va = 0;
			for (i = 0; i < Kexp_srj_kentat+Pexp_srj_kentat+Vexp_srj_kentat; i++) {
				if (i < Kexp_srj_kentat && wcscmp(exp_srj_kentat[i].name, fields[ifld]) == 0) {
					fldorder[ifld].fld = exp_srj_kentat[i].id;
					if (nflds <= ifld)
						nflds = ifld+1;
					break;
					}
				if (i >= Kexp_srj_kentat) {
					if (i == Kexp_srj_kentat) {
						p1 = NULL;
						pp = NULL;
						p = wcstok(fields[ifld], L"-");
						if (p)
							p1 = wcstok(NULL, L"-");
						if (!p1)
							continue;
						if ((os = _wtoi(p1)) < 1 || os > kilpparam.n_os_akt)
							continue;
						pp = wcstok(NULL, L"-");
						if (pp)
							va = _wtoi(pp);
						}
					if (os < 1 || os > kilpparam.n_os_akt)
						continue;
					if (va == 0 && i < Kexp_srj_kentat+Pexp_srj_kentat &&
						wcscmp(exp_srj_kentat[i].name, p) == 0) {
						fldorder[ifld].fld = exp_srj_kentat[i].id;
						fldorder[ifld].os = os;
						if (nflds <= ifld)
							nflds = ifld+1;
						break;
						}
					if (i < Kexp_srj_kentat+Pexp_srj_kentat || va < 1 || va > kilpparam.valuku)
						continue;
					if (wcscmp(exp_srj_kentat[i].name, p) == 0) {
						fldorder[ifld].fld = exp_srj_kentat[i].id;
						fldorder[ifld].os = os;
						fldorder[ifld].va = va;
						if (nflds <= ifld)
							nflds = ifld+1;
						break;
						}
					}
				}
			}
		deletefields(fields, sizeof(fields)/sizeof(fields[0]));
		return(1);
		}
	memset(Sarja->valuku, 0, sizeof(Sarja->valuku));
	for (UINT ifld = 0; res == 0 && ifld < nflds; ifld++) {
		wchar_t tfld[200];
		if (fldorder[ifld].fld == 0)
			continue;
		if (ifld < nfld) {
			wcsncpy(tfld, fields[ifld], 199);
			tfld[199] = 0;
			elimwbl(tfld);
			}
		else
			tfld[0] = 0;
		switch (fldorder[ifld].fld) {
			case FLDID_SRJID :
				wcstoansi(Sarja->sarjanimi, tfld, LSARJA);
				break;
			case FLDID_PNIMI :
				wcstoansi(Sarja->psarjanimi, tfld, LPSARJA);
				break;
			case FLDID_OSLKM :
				Sarja->ntosuus = _wtoi(tfld);
				break;
			case FLDID_ALKUNO :
				Sarja->sarjaalku = _wtoi(tfld);
				break;
			case FLDID_TLAHTO :
				Sarja->lahto = wstrtoaika_vap(tfld, t0);
				break;
			case FLDID_LAHTONO :
				Sarja->lno = _wtoi(tfld);
				break;
			case FLDID_MAALINO :
				Sarja->mno = _wtoi(tfld);
				break;
			case FLDID_SAKKOLJ :
				Sarja->taslaji = _wtoi(tfld);
				break;
			case FLDID_MAKSU :
				Sarja->maksu = _wtoi(tfld);
				break;
			case FLDID_MAKSUAL :
				Sarja->maksual = _wtoi(tfld);
				break;
			case FLDID_MATKA :
				WcsToMbs(Sarja->matka[fldorder[ifld].os-1], tfld, sizeof(Sarja->matka[0])-1);
				break;
			case FLDID_VALKM :
				Sarja->valuku[fldorder[ifld].os-1] = _wtoi(tfld);
				break;
			case FLDID_YLAHTO :
				Sarja->ylahto[fldorder[ifld].os-1] = wstrtoaika_vap(tfld, t0);
				break;
			case FLDID_YLKYNNYS :
				Sarja->ylkynnys[fldorder[ifld].os-1] = wstrtoaika_vap(tfld, t0);
				break;
			case FLDID_SAKKOJA :
				Sarja->paikat[fldorder[ifld].os-1] = _wtoi(tfld);
				break;
			case FLDID_SAKKOYKS :
				Sarja->tsak[fldorder[ifld].os-1] = _wtoi(tfld);
				break;
			case FLDID_RINNLKM :
				Sarja->nosuus[fldorder[ifld].os-1] = _wtoi(tfld);
				break;
			case FLDID_VAMATKA :
				WcsToMbs(Sarja->va_matka[fldorder[ifld].os-1][fldorder[ifld].va-1], tfld, sizeof(Sarja->va_matka[0][0])-1);
				if (Sarja->valuku[fldorder[ifld].os-1] < fldorder[ifld].va)
					Sarja->valuku[fldorder[ifld].os-1] = fldorder[ifld].va;
				break;
			}
		}
	deletefields(fields, sizeof(fields)/sizeof(fields[0]));
	return(res);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Luecsvtiedostosta1Click(TObject *Sender)
{
	TextFl *InFl;
	int srj = 0, lineno = -1;
	wchar_t erotin = L';';
	sarjatietue Srj;

	OpenDialog1->FileName = UnicodeString(L"sarjat.csv");
	OpenDialog1->DefaultExt = L"csv";
	OpenDialog1->Filter = L"Csv-tiedostot (*.csv)|*.CSV|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		InFl = new TextFl(OpenDialog1->FileName.c_str(), L"rt");
		while (InFl->IsOpen() && !InFl->Feof()) {

			if (lue_sarjacsv(InFl, &Srj, lineno, 0, erotin) == 0)
				Sarjat[srj++] = Srj;
			if (srj > sarjaluku)
				sarjaluku = srj;
			lineno++;
			}
		delete InFl;
		}
	SetCurrentDir(FormMain->CurrentDir);
	maaraaOsuusluku(true);
	SetColumns();
	NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::SG1MouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SG1->RowCount - SG1->FixedRows >= SG1->VisibleRowCount + SG1->TopRow)
		SG1->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::SG1MouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SG1->TopRow > SG1->FixedRows)
		SG1->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Kopioivalittunaolevasolukaikkiinsarjoihin1Click(TObject *Sender)

{
	UnicodeString Val;

	if (SG1->Options.Contains(goEditing)) {
		if (SG1->Row > 0 && SG1->Col > 0) {
			Val = SG1->Cells[SG1->Col][SG1->Row];
			for (int srj = 0; srj < sarjaluku; srj++)
			   SG1->Cells[SG1->Col][srj+1] = Val;
			}
		SG1->Refresh();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSarjatiedot::Kiinteitsarakkeita11Click(TObject *Sender)
{
	int FC = SG1->FixedCols;
	wchar_t ret;
	inputint_prompt(&FC, L"Anna kiinteiden sarakkeiden luku (1..8)", &ret, this);
	if (ret != ESC) {
		if (FC < 1)
			FC = 1;
		if (FC > 8)
			FC = 8;
		if (FC > SG1->ColCount)
			FC = SG1->ColCount;
		SG1->FixedCols = FC;
		Kiinteitsarakkeita11->Caption = L"Kiinteitä sarakkeita "+UnicodeString(FC);
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormSarjatiedot::FormShow(TObject *Sender)
{
	SetColumns();
	for (int srj = 0; srj < sarjaluku; srj++)
		Sarjat_0[srj] = Sarjat[srj];
}
//---------------------------------------------------------------------------

