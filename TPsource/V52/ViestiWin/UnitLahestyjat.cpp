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

#include "UnitMain.h"
#include "UnitLahestyjat.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormLahestyjat *FormLahestyjat[NLAHEST];

static const int cAika = 0;
static const int cSj = 1;
static const int cKno = 2;
static const int cJk = 3;
static const int cNimi = 4;

//---------------------------------------------------------------------------
__fastcall TFormLahestyjat::TFormLahestyjat(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	RGFont->ItemIndex = 1;
	maxSeur = 30;
	Cells = new rowTplah[maxSeur+1];
}

//---------------------------------------------------------------------------

lahIkkParamClass::lahIkkParamClass(void)
{
	FontSize = 10;
	ColW[cAika] = 80;
	ColW[cSj] = 35;
	ColW[cKno] = 45;
	ColW[cJk] = 150;
	ColW[cNimi] = 115;
}

//---------------------------------------------------------------------------
int TFormLahestyjat::applyParams(void)
{
	DG1->Font->Size = IkkParam.FontSize;
	DG1->DefaultRowHeight = 2 * IkkParam.FontSize;
	return(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormLahestyjat::Paivita(TObject *Sender)
{
	int os, va, r, sj, t, p;
	wchar_t was[20], wst[100];
	kilptietue kilp;

	if (Piste == 1 && Osuus == 0)
		return;
	if (Piste == 1) {
		os = Osuus - 1;
		if (os < 0)
			os = 0;
		va = 0;
		}
	else {
		os = Osuus;
		for (va = Piste ? Piste - 1 : kilpparam.maxvaluku; va > 0 && ntulosix[Sarja][os][va] == 0; va--);
		if (va == 0 && os > 0)
			os--;
		}
	sj = 0;
	t = KSEK*T_TIME(biostime(0,0));
	p = jalku[Sarja][os][va ? va+4 : 0];
	for (r = 0; p > 0 && r < maxSeur;) {
		sj++;
		GETREC(&kilp, p);
		p = JarjSeur(0, os, va, p);
		if (kilp.Maali(Osuus, Piste) != TMAALI0)
			continue;
		if (DG1->RowCount < r+2)
			DG1->RowCount = r+2;
		aikatowstr_cols_n(was, NORMTULOS(t-kilp.Maali(os, va)), 0, 0, 8);
		Cells[r+1][0].text = was;
		Cells[r+1][1].text = sj;
		Cells[r+1][2].text = kilp.kilpno;
		kilp.Joukkue(wst, 23);
		Cells[r+1][3].text = wst;
		kilp.Nimi(wst, 20, os);
		Cells[r+1][4].text = wst;
		Cells[r+1][0].justify = 1;
		Cells[r+1][1].justify = 1;
		Cells[r+1][2].justify = 1;
		r++;
		}
	if (r == 0) {
		DG1->RowCount = 2;
		for (int c = 0; c < DG1->ColCount; c++)
			Cells[1][c].text = L"";
		}
	else
		DG1->RowCount = r+1;
	DG1->Refresh();
}

//---------------------------------------------------------------------------
void __fastcall TFormLahestyjat::DG1DrawCell(TObject *Sender, int ACol, int ARow,
		  TRect &Rect, TGridDrawState State)
{
	int X;
	Graphics::TBitmap *pBitmap = new Graphics::TBitmap();
	TRect Rect1;
	pBitmap->Width = Rect.Width();
	pBitmap->Height = Rect.Height();
	Rect1.Left = 0;
	Rect1.Top = 0;
	Rect1.Right = pBitmap->Width;
	Rect1.Bottom = pBitmap->Height;
	pBitmap->Canvas->Font->Size = IkkParam.FontSize;
	if (ARow == 0)
		pBitmap->Canvas->Brush->Color = clBtnFace;
	else {
	  switch (Cells[ARow][ACol].style) {
		case 0:
			pBitmap->Canvas->Brush->Color = clWindow;
			pBitmap->Canvas->Font->Style = TFontStyles();
			break;
		case 1:
			pBitmap->Canvas->Brush->Color = TColor(0x0020ff20);  //Green
//            pBitmap->Canvas->Font->Style = TFontStyles() << fsBold;
			break;
		case 2:
			pBitmap->Canvas->Brush->Color = TColor(0x00c0ffc0);  //light green;
//            pBitmap->Canvas->Font->Style = TFontStyles() << fsBold;
			break;
		case 3:
			pBitmap->Canvas->Brush->Color = TColor(0x004040ff);  //red;
			break;
		case 4:
			pBitmap->Canvas->Brush->Color = TColor(0x00a0a0ff);  //light red;
			break;
		case 5:
			pBitmap->Canvas->Brush->Color = TColor(0x0000ffff);  //Yellow;
			break;
		case 6:
			pBitmap->Canvas->Brush->Color = TColor(0x00ffff00);  //;
			break;
		case 7:
			pBitmap->Canvas->Brush->Color = clMedGray;  //; light gray
//			pBitmap->Canvas->Brush->Color = TColor(0x00202020);  //; light gray
			break;
		 }
	  }
	pBitmap->Canvas->FillRect(Rect1);
	X = 1;
	if (Cells[ARow][ACol].justify == 1) {
		X =  Rect.Width() - pBitmap->Canvas->TextWidth(Cells[ARow][ACol].text) - 2;
		if (X < 1)
			X = 1;
		}
	if (Cells[ARow][ACol].justify == 2) {
		X =  (Rect.Width() - pBitmap->Canvas->TextWidth(Cells[ARow][ACol].text) - 2) / 2;
		if (X < 1)
			X = 1;
		}
	pBitmap->Canvas->TextOut(X,1,Cells[ARow][ACol].text);
	DG1->Canvas->Draw(Rect.Left, Rect.Top, pBitmap);
	delete pBitmap;
}
//---------------------------------------------------------------------------
void __fastcall TFormLahestyjat::FormResize(TObject *Sender)
{
	int wdth = 0;

	DG1->Font->Size = IkkParam.FontSize;
	DG1->DefaultRowHeight = 2 * IkkParam.FontSize;
	for (int c = 0; c < DG1->ColCount; c++) {
		DG1->ColWidths[c] = IkkParam.ColW[c] * IkkParam.FontSize * Screen->PixelsPerInch / 960;
		wdth += DG1->ColWidths[c] + 2;
		}
	ClientWidth = wdth + 20 * Screen->PixelsPerInch / 96;
	DG1->Left = 0;
	DG1->Width = ClientWidth;
	DG1->Height = ClientHeight - DG1->Top;
}
//---------------------------------------------------------------------------
void __fastcall TFormLahestyjat::FormShow(TObject *Sender)
{
	Cells[0][0].text = L"Ed. pisteestä";
	Cells[0][1].text = L"Järj.";
	Cells[0][2].text = L"KilpNo";
	Cells[0][3].text = L"Joukkue";
	Cells[0][4].text = L"Nimi";
	FormResize(Sender);
	CBSarja->Clear();
	CBSarja->ItemIndex = -1;
	for (int srj = 0; srj < sarjaluku+nsarjayhd; srj++) {
		CBSarja->Items->Add(Sarjat[srj].sarjanimi);
		if (srj == Sarja)
			CBSarja->ItemIndex = srj;
		}
	if (CBSarja->ItemIndex < 0) {
		CBSarja->ItemIndex = 0;
		Sarja = 0;
		}
	CBOsuus->Clear();
	for (int os = 1; os <= Sarjat[Sarja].ntosuus; os++)
		CBOsuus->Items->Add(UnicodeString(os)+L". os.");
	if (Osuus >= Sarjat[Sarja].ntosuus)
		Osuus = Sarjat[Sarja].ntosuus - 1;
	CBOsuus->ItemIndex = Osuus;
	CBPiste->Clear();
	CBPiste->Items->Add(L"Vaihto");
	for (int va = 1; va <= kilpparam.maxvaluku; va++)
		CBPiste->Items->Add(UnicodeString(va)+L". va");
	if (Osuus == 0 && Piste == 1)
		Piste = 2;
	CBPiste->ItemIndex = Piste;
}
//---------------------------------------------------------------------------
void __fastcall TFormLahestyjat::RGFontClick(TObject *Sender)
{
	IkkParam.FontSize = 8 + 2 * RGFont->ItemIndex;
	FormResize(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormLahestyjat::CBSarjaClick(TObject *Sender)
{
	Sarja = CBSarja->ItemIndex;
	CBOsuus->Clear();
	for (int os = 1; os <= Sarjat[Sarja].ntosuus; os++)
		CBOsuus->Items->Add(UnicodeString(os)+L". os.");
	if (Osuus >= Sarjat[Sarja].ntosuus)
		Osuus = Sarjat[Sarja].ntosuus - 1;
	CBOsuus->ItemIndex = Osuus;
	Paivita(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormLahestyjat::CBOsuusChange(TObject *Sender)
{
	Osuus = CBOsuus->ItemIndex;
	Paivita(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormLahestyjat::CBPisteClick(TObject *Sender)
{
	Piste = CBPiste->ItemIndex;
	Paivita(Sender);
}
//---------------------------------------------------------------------------
