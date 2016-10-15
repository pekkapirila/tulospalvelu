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

#include "UnitPartio.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPartio *FormPartio;
int rogpisteet(ratatp *rt, int rasti);
INT seur_laji(ratatp *rt, INT r);

//---------------------------------------------------------------------------
__fastcall TFormPartio::TFormPartio(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormPartio::FormShow(TObject *Sender)
{
	Left = Screen->Width - Width;
	Top = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormPartio::Partio(kilptietue *kilp, int d)
{
	emittp em;
	INT tulkinta[50], p_tulkinta[50][MAXOSUUSLUKU+1];
	int pisteet[MAXOSUUSLUKU], jpisteet = 0, entpisteet = -1000000;
	ratatp *rt;
	INT i, ii, j, jj, jjj, lj, k, os, r, kaikki, ll, OsLuku, rluku;
	INT32 tl, tls;
	char *p, ch, kh;
	wchar_t *wp, line[100], as[20], st[80], *ljcode;
	bool vajaat = true;
	int ljstyle = 0, hyvstyle = 0, min_k;

	if (kilp->kilpno < minkilpno)
		return;
	Kilp = *kilp;
	DKilp = d;
	OsLuku = Sarjat[Kilp.sarja].osuusluku;
	for (os = 0; os < OsLuku; os++) {
		if (EmitJarr(Kilp.kilpno, os) < 0)
			break;
		}
	if (os < OsLuku) {
		if (!vajaat)
			return;
		}
	else if (vajaat)
		entpisteet = Kilp.pisteet[0];
	rt = getrata(Kilp.ostiet[0].hajonta);
	if (!rt || !rt->rastiluku)
		return;
	rluku = 0;
	for (i = 0; i < rt->rastiluku; i++)
		if (rt->rastikoodi[i] < 9999)
			rluku++;
	memset(Cells, 0, sizeof(Cells));
	SGNimet->RowCount = OsLuku;
	DG1->ColCount = OsLuku + 4;
	SGNimet->ColWidths[0] = 35 * Screen->PixelsPerInch / 96;
	SGNimet->ColWidths[1] = 140 * Screen->PixelsPerInch / 96;
	SGNimet->ColWidths[2] = 60 * Screen->PixelsPerInch / 96;
	DG1->ColWidths[0] = 35 * Screen->PixelsPerInch / 96;
	DG1->ColWidths[1] = 35 * Screen->PixelsPerInch / 96;
	DG1->ColWidths[2] = 45 * Screen->PixelsPerInch / 96;
	DG1->ColWidths[3] = 35 * Screen->PixelsPerInch / 96;
	Cells[0][0].text = L"No";
	Cells[1][0].text = L"Rasti";
	if (kilpparam.rogaining) {
		Cells[2][0].text = L"Arvo";
		Cells[3][0].text = L"Pist";
		}
	else {
		Cells[2][0].text = L"Laji";
		Cells[3][0].text = L"Hyv";
		}
	EdtKilpno->Text = Kilp.kilpno;
	EdtJoukkue->Text = Kilp.Joukkue(st, 50);
	EdtSarja->Text = Sarjat[Kilp.sarja].Sarjanimi(st);
	EdtSija->Text = Kilp.Sija(OsLuku-1, 0);
	EdtTulos->Text = aikatowstr_cols_n(st, Kilp.Tulos(OsLuku-1, 0), 0, L':', kilpparam.laika2);
	EdtTark->Text = Kilp.TarkStr(OsLuku-1, false);
	for (i = 0; i < 51; i++) {
		for (j = 0; j < OsLuku + 4; j++) {
			Cells[j][i+1].text = L"";
			Cells[j][i+1].style = 0;
			}
		}
	for (os = 0; os < OsLuku; os++) {
		DG1->ColWidths[os+4] = 35 * Screen->PixelsPerInch / 96;
		Cells[os+4][0].text = UnicodeString(os+1);
		SGNimet->Cells[0][os] = os+1;
		SGNimet->Cells[1][os] = Kilp.Nimi(st, 30, os, 1);
		SGNimet->Cells[2][os] = Kilp.ostiet[os].badge[0];
		}
	memset(p_tulkinta, 0, sizeof(p_tulkinta));
	memset(pisteet, 0, sizeof(pisteet));
	nJas = 0;
	for (os = 0; os < OsLuku; os++) {
		if (getem(&em, Kilp.kilpno, os) < 0)
			continue;
		nJas++;
		tarkista(&em, kilp, tulkinta, -1, 0);
		for (i = 0; i < 50; i++) {
			if ((k = abs(tulkinta[i])) != 0) {
				if (!p_tulkinta[k-1][os+1]) {
					p_tulkinta[k-1][os+1] = i;
					if (kilpparam.rogaining) {
						pisteet[os] += rogpisteet(rt, k);
						}
					}
				}
			}
		}
	memset(tulkinta, 0, sizeof(tulkinta));
	if (kilpparam.rogaining) {
		int aikasakko = 0;
		for (i = 0; i < rt->rastiluku; i++) {
			for (os = 0; os < OsLuku; os++)
				if (p_tulkinta[i][os+1] > 0) {
					p_tulkinta[i][0] = 1;
					tulkinta[i] = 1;
					}
			}
		for (i = 0; i < rt->rastiluku; i++) {
			if (tulkinta[i])
				jpisteet += rogpisteet(rt, i+1);
			}
		for (os = 0; os < Sarjat[Kilp.sarja].osuusluku; os++)
			Kilp.ostiet[os].sakko = pisteet[os];
		if (maxaika[Kilp.sarja] && (tls = tulos(kilp, Sarjat[Kilp.sarja].osuusluku-1,0)) > 0
			&& tls > maxaika[Kilp.sarja]) {
			if (sakkoaskel[Kilp.sarja]) {
				aikasakko =
					((tls - maxaika[Kilp.sarja] + sakkoaskel[Kilp.sarja] - SEK) / sakkoaskel[Kilp.sarja])
						* askelsakko[Kilp.sarja];
				}
			}
		os = Sarjat[Kilp.sarja].osuusluku - 1;
		EdtRastiPisteet->Color = (Kilp.pisteet[1] != jpisteet ? clYellow : clWindow);
		EdtPisteet->Color = (Kilp.pisteet[0] != jpisteet - aikasakko ? clYellow : clWindow);
		EdtSakko->Color = (Kilp.pisteet[1] - Kilp.pisteet[0] !=  aikasakko ? clYellow : clWindow);
		EdtRastiPisteet->ShowHint = EdtRastiPisteet->Color == clYellow;
		EdtPisteet->ShowHint = EdtPisteet->Color == clYellow;
		EdtSakko->ShowHint = EdtSakko->Color == clYellow;
		BtnTallenna->Visible  = EdtRastiPisteet->Color == clYellow ||
			EdtPisteet->Color == clYellow || EdtSakko->Color == clYellow;
		Kilp.pisteet[0] = jpisteet - aikasakko;
		Kilp.pisteet[1] = jpisteet;
		if (nJas > (int) Kilp.flag[0]) {
			Kilp.flag[0] = (UCHAR) nJas;
			Kilp.Tallenna(DKilp, 0, 0, 0, 0);
			EdtRastiPisteet->Color = clWindow;
			EdtPisteet->Color = clWindow;
			EdtSakko->Color = clWindow;
			}
		EdtPisteet->Text = Kilp.pisteet[0];
		EdtRastiPisteet->Text = Kilp.pisteet[1];
		EdtSakko->Text = Kilp.pisteet[1] - Kilp.pisteet[0];
		}
	else {
		os = Sarjat[Kilp.sarja].osuusluku - 1;
		kaikki = os + 1;
		k = 0;
		ii = 0;
		lj = seur_laji(rt, 0);
		for (i = 0;i < rt->rastiluku; i++) {
			if (rt->rastikoodi[i] >= 10000 && rt->rastikoodi[i] <= 10002) {
				lj = seur_laji(rt, i);
				kaikki = (lj <= 10001) ? os+1 : 1;
				tulkinta[i] = -1;
				continue;
				}
			jj = kaikki;
			for (j = 1; j <= os; j++) {
				if (p_tulkinta[ii][j]) {
					if (lj == 10000) {
						for (jjj = 0; jjj < rluku; jjj++) {
							if (p_tulkinta[ii][j] && (
								(jjj < ii &&
								p_tulkinta[jjj][j] > p_tulkinta[ii][j]) ||
								(jjj > ii && p_tulkinta[jjj][j] &&
								p_tulkinta[jjj][j] < p_tulkinta[ii][j]))) {
								p_tulkinta[ii][j] = -p_tulkinta[ii][j];
								}
							}
						if (jjj == rluku)
							jj--;
						}
					else
						jj--;
					}
				}
			ii++;
			}
		}
	if (kilpparam.rogaining) {
		swprintf(line, L"Lasketut pisteet: %d, Sis. sakon %d,  Aika: %8.8s",
			Kilp.pisteet[0], Kilp.pisteet[1]-Kilp.pisteet[0], aikatowstr_ls(as, tls, 0));
		if (entpisteet > -1000000 && entpisteet != Kilp.pisteet[0]) {
			swprintf(line, L"Tallennetut pisteet: %d", entpisteet);
			}
		}
	hyvstyle = 0;
	ii = 0;
	min_k = 1;
	for (i = -1; i < rt->rastiluku; i++) {

		if (i == -1 || (rt->rastikoodi[i] > 9999 && rt->rastikoodi[i] <= 10002)) {
			if (i == rt->rastiluku-1 || rt->rastikoodi[i+1] > 9999)
				continue;
			j = seur_laji(rt, i) - 10000;
			switch (j) {
				case 0 :
					ljstyle = 4;
					min_k = OsLuku;
					ljcode = L"PakJrj";
					break;
				case 1 :
					ljstyle = 6;
					min_k = OsLuku;
					ljcode = L"VapJrj";
					break;
				case 2 :
					ljstyle = 0;
					ljcode = L"Valinn";
					min_k = 1;
					break;
				}
			}
		else if (!kilpparam.rogaining) {
			k = 0;
//			swprintf(line, L"    %2d  %3d  Leimausten järjestysnumerot :", ii+1, rt->rastikoodi[i]);
			Cells[0][ii+1].text = ii+1;
			Cells[1][ii+1].text = rt->rastikoodi[i];
			Cells[2][ii+1].text = ljcode;
			Cells[2][ii+1].style = ljstyle;
			for (j = 0; j < os; j++) {
				if (p_tulkinta[ii][j+1]) {
					Cells[j+4][ii+1].text = p_tulkinta[ii][j+1];
					if (p_tulkinta[ii][j+1] > 0)
						k++;
					}
				else
					Cells[j+4][ii+1].text = L"";
				}
			Cells[3][ii+1].text = (k >= min_k ? UnicodeString(L"Ok") : UnicodeString(L"Vaj"));
			if (k < min_k)
				hyvstyle = 2;
			for (j = -1; j < os; j++) {
				Cells[j+4][ii+1].style = (k >= min_k ? 0 : 5);
				}
			ii++;
			}
		else {
			ii++;
			Cells[0][ii].text = ii;
			Cells[1][ii].text = rt->rastikoodi[i];
			Cells[2][ii].text = rogpisteet(rt, i);
//			for (j = 0; j <= os; j++)
//				if (p_tulkinta[ii-1][j])
//					break;
			if (p_tulkinta[ii-1][0]) {
//				swprintf(line, L"    %2d  %3d  %3d Leimausten järjestysnumerot :", ii, rt->rastikoodi[i], rogpisteet(rt,i));
//				Cells[3][ii+1].text = Cells[2][ii+1].text;
				for (j = 0; j <= OsLuku; j++) {
					if (p_tulkinta[ii-1][j])
						Cells[3+j][ii].text = Cells[2][ii].text;
					else
						Cells[3+j][ii].text = L"";
					}
				}
			else {
				for (j = 0; j <= os; j++) {
					Cells[3+j][ii].text = L"";
					}
//				continue;
				}
			}
		}
	for (; ii < 51; ii++) {
		for (j = 0; j < OsLuku + 4; j++) {
			Cells[j][ii+1].text = L"";
			Cells[j][ii+1].style = hyvstyle;
			}
		}
	DG1->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFormPartio::DG1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
		  TGridDrawState State)
{
	int FontSize = 9, w;

   Graphics::TBitmap *pBitmap = new Graphics::TBitmap();
	TRect Rect1;
   pBitmap->Width = Rect.Width();
	pBitmap->Height = Rect.Height();
   Rect1.Left = 0;
   Rect1.Top = 0;
   Rect1.Right = pBitmap->Width;
   Rect1.Bottom = pBitmap->Height;
   if (ARow == 0)
	  pBitmap->Canvas->Brush->Color = clBtnFace;
   else
	{
	  switch (Cells[ACol][ARow].style) {
		case 0:
			pBitmap->Canvas->Brush->Color = clWindow;
			pBitmap->Canvas->Font->Style = TFontStyles();
			break;
		case 1:
			pBitmap->Canvas->Brush->Color = TColor(0xd0d0d0);  //vaaleanharmaa
			break;
		case 2:
			pBitmap->Canvas->Brush->Color = clYellow;  //clYellow;
			break;
		case 3:
			pBitmap->Canvas->Brush->Color = TColor(0xc0ffc0);  //vihreä
			break;
		case 4:
			pBitmap->Canvas->Brush->Color = clLime;
			break;
		case 5:
			pBitmap->Canvas->Brush->Color = clFuchsia;
			break;
		case 6:
			pBitmap->Canvas->Brush->Color = clAqua;
			break;
		 }
	  }
   pBitmap->Canvas->FillRect(Rect1);
   pBitmap->Canvas->Font->Name = L"Arial";
   pBitmap->Canvas->Font->Size = FontSize;
   w = pBitmap->Canvas->TextWidth(Cells[ACol][ARow].text);
   if (w < pBitmap->Width - 3)
		w = pBitmap->Width - w - 2;
   else
		w = 1;
   pBitmap->Canvas->TextOut(w,1,Cells[ACol][ARow].text);
   DG1->Canvas->Draw(Rect.Left, Rect.Top, pBitmap);
   delete pBitmap;
}
//---------------------------------------------------------------------------

void __fastcall TFormPartio::FormResize(TObject *Sender)
{
	DG1->Height = ClientHeight - DG1->Top - 4;
	DG1->Width = ClientWidth - DG1->Left - 4;
	if (kilpparam.rogaining) {
		BtnTallenna->Left = ClientWidth - BtnTallenna->Width - 4;
		SGNimet->Width = BtnTallenna->Left - SGNimet->Left - 4;
		}
	else
		SGNimet->Width = ClientWidth - SGNimet->Left - 4;
}
//---------------------------------------------------------------------------

void __fastcall TFormPartio::FormCreate(TObject *Sender)
{
	if(kilpparam.rogaining) {
		BtnTallenna->Visible = true;
		Label1->Visible = true;
		Label2->Visible = true;
		Label3->Visible = true;
		EdtPisteet->Visible = true;
		EdtRastiPisteet->Visible = true;
		EdtSakko->Visible = true;
		}
	else {
		SGNimet->Top = EdtSarja->Top + EdtSarja->Height + 4;
		DG1->Top = SGNimet->Top + SGNimet->Height + 4;
		DG1->Height = ClientHeight - DG1->Top - 4;
		}
	if (kilpparam.osuusluku > 3) {
		DG1->Width = (37 * (kilpparam.osuusluku + 4) + 24) * Screen->PixelsPerInch / 96;
		ClientWidth = DG1->Left + DG1->Width + 4;
		}
	FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormPartio::BtnTallennaClick(TObject *Sender)
{
	Kilp.pisteet[0] = _wtoi(EdtPisteet->Text.c_str());
	Kilp.pisteet[1] = _wtoi(EdtRastiPisteet->Text.c_str());
	Kilp.Tallenna(DKilp, 0, 0, 0, 0);
	Partio(&Kilp, DKilp);
}
//---------------------------------------------------------------------------

void __fastcall TFormPartio::EdtSakkoChange(TObject *Sender)
{
	int sak = _wtoi(EdtSakko->Text.c_str());
	EdtPisteet->Text = EdtRastiPisteet->Text.ToInt() - sak;
	EdtSakko->Text = sak;
}
//---------------------------------------------------------------------------

