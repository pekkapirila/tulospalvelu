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

#include "UnitAmpPaikat.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAmpPaikat *FormAmpPaikat;
//---------------------------------------------------------------------------
__fastcall TFormAmpPaikat::TFormAmpPaikat(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormAmpPaikat::FormResize(TObject *Sender)
{
	DG1->Width = ClientWidth - DG1->Left;
	DG1->Height = ClientHeight - DG1->Top;
}
//---------------------------------------------------------------------------
void __fastcall TFormAmpPaikat::DG1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
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
//			pBitmap->Canvas->Brush->Color = TColor(0xc0ffc0);  //vihreä
			pBitmap->Canvas->Brush->Color = clGreen;
			pBitmap->Canvas->Font->Style << fsBold;
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
		case 8:
			pBitmap->Canvas->Brush->Color = clWindow;
			pBitmap->Canvas->Font->Style << fsBold;
			break;
		case 9:
			pBitmap->Canvas->Brush->Color = TColor(0xc0ffc0);  //vihreä;
			break;
		 }
	  }
   pBitmap->Canvas->FillRect(Rect1);
   pBitmap->Canvas->Font->Name = L"Arial";
   pBitmap->Canvas->Font->Size = FontSize;
   w = pBitmap->Canvas->TextWidth(Cells[ACol][ARow].text);
   if (ACol == 0 || ACol == 2)
		w = pBitmap->Width - w - 2;
   else if (ACol == 1)
		w = (pBitmap->Width - w - 6) / 2;
   else
		w = 1;
   pBitmap->Canvas->TextOut(w,1,Cells[ACol][ARow].text);
   DG1->Canvas->Draw(Rect.Left, Rect.Top, pBitmap);
   delete pBitmap;
}
//---------------------------------------------------------------------------
void __fastcall TFormAmpPaikat::FormCreate(TObject *Sender)
{
	tauluLkm = 30;
	Vara = 0;
	DG1->RowCount = tauluLkm + 1;
	DG1->ColCount = (Vara > 0) ? 12 : 11;
	Cells[0][0].text = L"Taulu";
	Cells[1][0].text = L"Pka";
	Cells[2][0].text = L"Nro";
	Cells[3][0].text = L"Nimi";
	Cells[4][0].text = L"Sakot";
	Cells[5][0].text = L"1";
	Cells[6][0].text = L"2";
	Cells[7][0].text = L"3";
	Cells[8][0].text = L"4";
	Cells[9][0].text = L"5";
	Cells[10][0].text = L"Ohi";
	Cells[11][0].text = L"Vara";
	DG1->ColWidths[0] = 30 * Screen->PixelsPerInch / 96;
	DG1->ColWidths[1] = 30 * Screen->PixelsPerInch / 96;
	DG1->ColWidths[2] = 40 * Screen->PixelsPerInch / 96;
	DG1->ColWidths[3] = 150 * Screen->PixelsPerInch / 96;
	DG1->ColWidths[4] = 40 * Screen->PixelsPerInch / 96;
	DG1->ColWidths[5] = 18 * Screen->PixelsPerInch / 96;
	DG1->ColWidths[6] = 18 * Screen->PixelsPerInch / 96;
	DG1->ColWidths[7] = 18 * Screen->PixelsPerInch / 96;
	DG1->ColWidths[8] = 18 * Screen->PixelsPerInch / 96;
	DG1->ColWidths[9] = 18 * Screen->PixelsPerInch / 96;
	DG1->ColWidths[10] = 40 * Screen->PixelsPerInch / 96;
	if (Vara)
		DG1->ColWidths[11] = 40 * Screen->PixelsPerInch / 96;
	for (int r = 1; r <= tauluLkm; r++) {
		pohjustaTaulu(r);
		}
	DG1->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFormAmpPaikat::pohjustaTaulu(int paikka)
{
	for (int c = 0; c < DG1->ColCount; c++) {
		Cells[c][paikka].style = 0;
		Cells[c][paikka].text = (c == 0 ? UnicodeString(paikka) : UnicodeString(L""));
		}
}
//---------------------------------------------------------------------------

void ampPaikka(int paikka)
{
	if (!FormAmpPaikat)
		return;
	FormAmpPaikat->pohjustaTaulu(paikka);
}

void ampujaPaikalle(int paikka, int kno)
{
	kilptietue kilp;
	wchar_t st[60] = L"", sak[5] = L"";
	int kierr = 1;

	if (!FormAmpPaikat)
		return;
#ifndef MAXOSUUSLUKU
	kilp.GETREC(getpos(kno));
	if (kilp.id() != kno)
		return;
	kilp.nimi(st, 30, false);
	wcsncpy(sak, kilp.pv[k_pv].asakot, 4);
	for (int i = 0; i < Sarjat[kilp.Sarja()].lsak[k_pv]; i++) {
		if (kilp.pv[k_pv].asakot[i] == L'-') {
			kierr = i+1;
			break;
			}
		}
#endif
	FormAmpPaikat->pohjustaTaulu(paikka);
	FormAmpPaikat->Cells[1][paikka].text = kierr;
	FormAmpPaikat->Cells[2][paikka].text = kno;
	FormAmpPaikat->Cells[3][paikka].text = st;
	FormAmpPaikat->Cells[4][paikka].text = sak;
	if (FormAmpPaikat->Vara) {
		FormAmpPaikat->Cells[11][paikka].style = 8;
		for (int i = 0; i < FormAmpPaikat->Vara; i++)
			FormAmpPaikat->Cells[11][paikka].text = FormAmpPaikat->Cells[11][paikka].text + L"I";
		}
	FormAmpPaikat->DG1->Refresh();
}
//---------------------------------------------------------------------------

void Laukaus(int paikka, bool osuma, char *taulut)
{
	if (!FormAmpPaikat)
		return;
	if (osuma) {
		for (int i = 0; i < 5; i++) {
			if (taulut[i] == '1') {
				FormAmpPaikat->Cells[5+i][paikka].style = 3;
//				FormAmpPaikat->Cells[5+i][paikka].text = L'\u26ab';
				}
			}
		}
	else {
		if (FormAmpPaikat->Vara && FormAmpPaikat->Cells[11][paikka].text.Length() > 0)
			FormAmpPaikat->Cells[11][paikka].text =
				FormAmpPaikat->Cells[11][paikka].text.SubString(1, FormAmpPaikat->Cells[11][paikka].text.Length()-1);
		else
			FormAmpPaikat->Cells[10][paikka].text = FormAmpPaikat->Cells[10][paikka].text + L"#";
	}
	FormAmpPaikat->DG1->Refresh();
}
//---------------------------------------------------------------------------
