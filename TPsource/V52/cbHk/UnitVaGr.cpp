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

#include "UnitVaGr.h"
#include "HkDeclare.h"
#include "WinHk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormVaGr *FormVaGr;
//---------------------------------------------------------------------------
__fastcall TFormVaGr::TFormVaGr(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
__fastcall TFormVaGr::~TFormVaGr(void)
{
}
//---------------------------------------------------------------------------


void __fastcall TFormVaGr::PaivitaClick(TObject *Sender)
{
   int kno, laji = 0, piste = 0, lkm, ntls;
   int p;
	kilptietue kilp;
	wchar_t line[60], st[40];
	int *valitut = 0, edlkm;

	if ((edlkm = KilpailijaList->SelCount) > 0) {
		valitut = (int *)calloc(edlkm, sizeof(int));
		int n = 0;
		for (int i = 0; i < KilpailijaList->Count; i++) {
		 if (KilpailijaList->Selected[i])
			valitut[n++] = KilpailijaList->Items->Strings[i].SubString(0,4).ToInt();
			}
		}
	Sarja = SrjVal->ItemIndex;
	wcsncpy(Kohde, SrjVal->Items->Strings[SrjVal->ItemIndex].c_str(), sizeof(Kohde)/2-1);
	FormMain->SetFormKohde(this, Kohde);
	EnterCriticalSection(&tall_CriticalSection);
	switch (JarjVal->ItemIndex) {
		case 0:
//		 laji = kilpparam.valuku + 5 + 5 * (k_pv > 0);
//		 piste = kilpparam.valuku + 1 + (k_pv > 0);
		 laji = kilpparam.valuku + 10;
		 piste = kilpparam.valuku + 2;
		 ntls = nilm[Sarja];
			break;
		case 1:
			laji = 0;
		 piste = 0;
		 ntls = ntulos[Sarja][0];
			break;
		}
	p = jalku[Sarja][laji];
	lkm = EdMaxLkm->Text.ToInt();
	if (lkm > ntls)
		lkm = ntls;
	KilpailijaList->Clear();
	for (int i = 0; i < lkm; i++) {
		if (p < 0)
			break;
		kilp.GETREC(p);
	  p = JarjSeur(0, piste, p);
		swprintf(line, L"%04d %.20s", kilp.id(), kilp.nimi(st, 20, false));
	  KilpailijaList->Items->Add(UnicodeString(line));
		}
	LeaveCriticalSection(&tall_CriticalSection);
	for (int i = 0; i < KilpailijaList->Count; i++) {
	  for (int j = 0; j < edlkm; j++) {
			if (valitut[j] == KilpailijaList->Items->Strings[i].SubString(0,4).ToInt())
			KilpailijaList->Selected[i] = True;
			}
		}
	if (valitut)
		free(valitut);
}
//---------------------------------------------------------------------------
void __fastcall TFormVaGr::PiirraBtnClick(TObject *Sender)
{
   char ch, cs, ch2, ct, line[80], trk, selitys[17];
   int d, rvali, sj, valuku, lkm, n, tiedot;
   int (*taul)[51][2], x, y, kilpno, *valitut;
   emittp em;
   emitvaanaltp *eva = 0;
   wchar_t wcs[80];

   tiedot = TietoVal->ItemIndex;
   if ((lkm = KilpailijaList->SelCount) < 1)
	  return;
	taul = (int (*)[51][2]) calloc(lkm, 51*2*sizeof(int));
	n = 0;
   if (lkm > 50)
	  lkm = 50;
   valitut = (int *) calloc(lkm, sizeof(int));
   for (int i = 0; i < KilpailijaList->Count; i++) {
	  if (n >= lkm)
			break;
	  if (KilpailijaList->Selected[i]) {
//		 memset(wcs, 0, sizeof(wcs));
//		 wcsncpy(wcs, KilpailijaList->Items->Strings[i].c_str(), 4);
//		 kilpno = _wtoi(wcs);
		 kilpno = KilpailijaList->Items->Strings[i].SubString(0,4).ToInt();
		 if (kilpno > 0)
			valitut[n++] = kilpno;
		 }
	  }
	lkm = n;
	kilptietue *kilp = new kilptietue[lkm];

   n = 0;
   EnterCriticalSection(&tall_CriticalSection);
   for (int i = 0; i < lkm; i++) {
		if ((kilpno = valitut[i]) > 0 && (d = getpos(kilpno)) > 0) {
		 kilp[n].GETREC(d);
		 n++;
		 }
	  }
   lkm = n;
   LeaveCriticalSection(&tall_CriticalSection);

   n = 0;
   for (int i = 0; i < lkm; i++) {
	  if ((kilpno = kilp[i].id()) > 0) {
		 if (getem(&em, kilpno, 0) >= 0 &&
			!laskeemitvaliajat(&em, &kilp[i], &eva, 0, &valuku)) {
			for (int j = 0; j <= valuku; j++) {
			   if (j == 0) {
				  x = 0;
				  if (tiedot == 2)
							y = kilp[i].pv[k_pv].tlahto/SEK;
						else
					 y = 0;
				  }
               else {
                  if (eva[j-1].rvno < 0) {
                     valuku = j-1;
							break;
                     }
				  x += rvalit[eva[j-1].rvno].vertaika[kilp[i].Sarja()];
                  switch (tiedot) {
                     case 0:
                        y += eva[j-1].rvaika - rvalit[eva[j-1].rvno].vertaika[kilp[i].Sarja()];
                        break;
                     case 1:
							case 2:
								y += eva[j-1].rvaika;
                        break;
                     }
                  }
               taul[n][j][0] = x;
					taul[n][j][1] = y;
			   }
//			free(eva);
            taul[n][50][0] = kilpno;
            taul[n][50][1] = valuku+1;
            n++;
			}
		 if (eva)
			free(eva);
		 eva = NULL;
		 }
	  }
   DrawTaul(taul, n);
   free(taul);
   delete[] kilp;
   free(valitut);
}
//---------------------------------------------------------------------------
void __fastcall TFormVaGr::DrawTaul(int (*taul)[51][2], int nrivi)
{
	int xVasT, xVas, xOik, xMin, xMax = -2000000000, x;
	int yYlaT, yYla, yAla, yMin = 2000000000, yMax = -2000000000, y;
	TRect Rect = TRect(0,0,650,650);
	TColor Colors[8];
	int xVali, xLisa = 5, edY = -1000;
	int tiedot, yEro = 0;

	xVasT = 170 * Screen->PixelsPerInch / 96;
	xVas = 10 * Screen->PixelsPerInch / 96;
//	xOik = 650 * Screen->PixelsPerInch / 96;
	yYlaT = 10 * Screen->PixelsPerInch / 96;
	yYla = 10 * Screen->PixelsPerInch / 96;
//	yAla = 620 * Screen->PixelsPerInch / 96;
	tiedot = TietoVal->ItemIndex;
	if (tiedot) {
		yEro = taul[0][taul[0][50][1]-1][1] - taul[0][0][1];
		}
	xMin = 0;
	for (int i = 0; i < nrivi; i++) {
		xVali = taul[i][taul[i][50][1]-1][0] - taul[i][0][0];
		for (int j = 0; j < taul[i][50][1]; j++) {
			if (taul[i][j][0] > xMax)
				xMax = taul[i][j][0];
			y = taul[i][j][1];
			if (yEro && xVali)
				y = y - (taul[i][j][0] - taul[i][0][0])*yEro/xVali;
			if (y > yMax)
				yMax = y;
			if (y < yMin)
				yMin = y;
			}
		}
	xVali = xMax - xMin;
	if (xVali == 0 || yMax == yMin)
		return;
	Graphics::TBitmap *pBitmap = new Graphics::TBitmap();
	pBitmap->Width = ClientWidth-xVasT;
	pBitmap->Height = ClientHeight-yYlaT;
	Rect.Right = pBitmap->Width;
	Rect.Bottom = pBitmap->Height;
	pBitmap->Canvas->Brush->Color = clBtnFace;
	pBitmap->Canvas->FillRect(Rect);
	xOik = pBitmap->Width - 58 * Screen->PixelsPerInch / 96;
	Rect.Right = pBitmap->Width - 8 * Screen->PixelsPerInch / 96;
	yAla = pBitmap->Height - 18 * Screen->PixelsPerInch / 96;
	Rect.Bottom = pBitmap->Height - 8 * Screen->PixelsPerInch / 96;
	pBitmap->Canvas->Brush->Color = clBtnFace;
	pBitmap->Canvas->FillRect(Rect);
	pBitmap->Canvas->Brush->Color = clWhite;
	Colors[0] = clBlue;
	Colors[1] = clRed;
	Colors[2] = clGreen;
	Colors[3] = clBlack;
	Colors[4] = clSkyBlue;
	Colors[5] = clLime;
	Colors[6] = clMaroon;
	Colors[7] = clGray;
	pBitmap->Canvas->FillRect(Rect);
	pBitmap->Canvas->Pen->Style = psSolid;
	pBitmap->Canvas->Pen->Color = clDkGray;
	pBitmap->Canvas->Pen->Width = 1;
	pBitmap->Canvas->Rectangle(xVas, yYla, xOik, yAla);
	y = (yMax-yMin)/8;
	if (y == 0)
		y = 1;
	if (y > 600)
		y = (y / 600) * 600;
	else if (y > 300)
		y = 200;
	else if (y > 60)
		y = (y /60) * 60;
	else if (y > 30)
		y = 30;
	else if (y > 10)
		y = (y / 10) * 10;
	else if (y > 5)
		y = 5;
	pBitmap->Canvas->Font->Size = 10;
	pBitmap->Canvas->Font->Color = clBlack;
	pBitmap->Canvas->TextOut(xVas+10, yAla-20, UnicodeString(L"Viivaväli ")+ UnicodeString(y)+ UnicodeString(L" s"));
	pBitmap->Canvas->Pen->Color = clLtGray;
	for (int i = -20; ; i++) {
		int yy = taul[0][0][1] + i*y;
		if (yy < yMin)
			continue;
		if (yy > yMax)
			break;
		yy = (yy - yMin)*(yAla-yYla)/(yMax-yMin)+yYla;
		pBitmap->Canvas->MoveTo(xVas, yy);
		pBitmap->Canvas->LineTo(xOik, yy);
		if ((i*y)%60 == 0)
			pBitmap->Canvas->TextOut(xVas+5, yy-8, UnicodeString((i*y)/60));
		}
	if (tiedot)
		xVali = taul[0][taul[0][50][1]-1][0] - taul[0][0][0];
	for (int j = 1; j < taul[0][50][1]-1; j++) {
      x = (taul[0][j][0] - xMin)*(xOik-xVas)/xVali+xVas;
      pBitmap->Canvas->MoveTo(x, yYla);
		pBitmap->Canvas->LineTo(x, yAla);
	  pBitmap->Canvas->TextOut(x-5, yYla+5, UnicodeString(j));
      }
   pBitmap->Canvas->Pen->Width = ViivaVal->ItemIndex+1;
   for (int i = 0; i < nrivi; i++) {
		pBitmap->Canvas->Pen->Color = Colors[i%8];
      if (i > 7) {
         if (i > 15)
            pBitmap->Canvas->Pen->Style = psDash;
         else
            pBitmap->Canvas->Pen->Style = psDot;
         }
      pBitmap->Canvas->MoveTo((taul[i][0][0] - xMin)*(xOik-xVas)/(xMax-xMin)+xVas,
         (taul[i][0][1] - yMin)*(yAla-yYla)/(yMax-yMin)+yYla);
	  if (tiedot)
         xVali = taul[i][taul[i][50][1]-1][0] - taul[i][0][0];
      for (int j = 1; j < taul[i][50][1]; j++) {
			x = (taul[i][j][0] - xMin)*(xOik-xVas)/xVali+xVas;
         y = taul[i][j][1];
         if (yEro && xVali)
            y = y - (taul[i][j][0] - taul[i][0][0])*yEro/xVali;
         y = (y - yMin)*(yAla-yYla)/(yMax-yMin)+yYla;
         pBitmap->Canvas->LineTo(x, y);
         }
      pBitmap->Canvas->Font->Color = Colors[i%8];
      if (abs(y-edY) < 15)
         xLisa = 30 - xLisa;
      else
			xLisa = 5;
      pBitmap->Canvas->TextOut(xOik+xLisa, y-8, UnicodeString(taul[i][50][0]));
      edY = y;
      }
   try {
        Canvas->Draw(xVasT, yYlaT, pBitmap);
		}
	catch (...) {
        }
   delete pBitmap;
}
//---------------------------------------------------------------------------

void __fastcall TFormVaGr::ViivaValChange(TObject *Sender)
{
	PiirraBtnClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormVaGr::FormResize(TObject *Sender)
{
   static int init;

	if (!init) {
      init = 1;
      return;
      }
	KilpailijaList->Height = ClientHeight - 369 * Screen->PixelsPerInch / 96;
	EdMaxLkm->Top = ClientHeight - 27 * Screen->PixelsPerInch / 96;
	Label1->Top = ClientHeight - 23 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------

void __fastcall TFormVaGr::SrjValChange(TObject *Sender)
{
   PaivitaClick(Sender);
   PiirraBtnClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormVaGr::FormActivate(TObject *Sender)
{
   PiirraBtnClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormVaGr::FormCreate(TObject *Sender)
{
	for (int srj = 0; srj < sarjaluku; srj++)
      SrjVal->Items->Add(Sarjat[srj].sarjanimi);
   SrjVal->ItemIndex = 0;
}
//---------------------------------------------------------------------------

