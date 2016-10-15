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
#include "VDeclare.h"
#include "UnitMain.h"
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

#ifdef EMIT

void __fastcall TFormVaGr::PaivitaClick(TObject *Sender)
{
   int d, kno, laji = 0, os, aos, lkm, ntls, p, lj;
   kilptietue kilp;
   char line[100], dt[80], st[80];
   int *valitut = 0, edlkm;

   if (Sarja != edSarja)
	  JoukkList->Clear();
   edSarja = Sarja;
   if ((edlkm = JoukkList->SelCount) > 0) {
	  valitut = (int *)calloc(edlkm, sizeof(int));
	  int n = 0;
	  for (int i = 0; i < JoukkList->Count; i++) {
		 if (JoukkList->Selected[i])
			valitut[n++] = JoukkList->Items->Strings[i].SubString(0,4).ToInt();
		 }
	  }
   os = OsVal->ItemIndex-1;
   swprintf(Kohde, L"%.12s-%.8s", SarjaVal->Items->Strings[SarjaVal->ItemIndex].c_str(),
	   OsVal->Items->Strings[OsVal->ItemIndex].c_str());
   FormMain->SetFormKohde(this, Kohde);
   EnterCriticalSection(&tall_CriticalSection);
   if (os >= 0) {
	  if (os >= Sarjat[Sarja].osuusluku) {
		 os = Sarjat[Sarja].osuusluku - 1;
		 OsVal->ItemIndex = os+1;
		 }
	  if (JarjVal->ItemIndex == 0) {
		  laji = kilpparam.valuku+1;
		  p = JosAlku(Sarja, os, &aos);
		  }
	  else {
		  laji = 0;
		  p = jalku[Sarja][os][laji ? laji+4 : 0];
		  }
	  ntls = ntulos[Sarja][os][laji];
	  }
   else {
	  ntls = nilm[Sarja];
	  kno = minkilpno-1;
	  }
   lkm = EdMaxLkm->Text.ToInt();
   if (lkm > ntls)
	  lkm = ntls;
   JoukkList->Clear();
   for (int i = 0; i < lkm; i++) {
	  if (os >= 0) {
		 d = p;
		 p = JarjSeur(0, os, laji, p);
		 }
	  else {
		 kno++;
		 for (; kno < maxkilpno; kno++) {
			if ((d = getpos(kno)) > 0);
			   break;
			}
		 }
	  if (d < 0)
		  break;
	  GETREC(&kilp, d);
	  sprintf(line, "%04d %.22s %s", kilp.kilpno, kilp.Joukkue(st), kilp.maa);
	  JoukkList->Items->Add(AnsiString(line));
	  }
   LeaveCriticalSection(&tall_CriticalSection);
   for (int i = 0; i < JoukkList->Count; i++) {
	  for (int j = 0; j < edlkm; j++) {
		 if (valitut[j] == JoukkList->Items->Strings[i].SubString(0,4).ToInt())
			JoukkList->Selected[i] = True;
		 }
      }
   if (valitut)
      free(valitut);
}
//---------------------------------------------------------------------------
void __fastcall TFormVaGr::PiirraBtnClick(TObject *Sender)
{
   char ch, cs, ch2, ct, line[80], trk, selitys[17];
   int d, os, yos, rvali, sj, valuku, lkm, n, tiedot;
   int (*taul)[MAXEMITVA+1][2], x, y, kilpno, *valitut;
   kilptietue *kilp;
   emittp em;
   emitvaanaltp *eva = 0;

   os = OsVal->ItemIndex-1;
   if (os < 0)
	  os = Osuus;
   if (os < 0)
	  os = 0;
   Osuus = os;
   if (Osuus >= Sarjat[Sarja].osuusluku)
	  Osuus = Sarjat[Sarja].osuusluku-1;
   yos = Sarjat[Sarja].yosuus[Osuus];
   tiedot = TietoVal->ItemIndex;
   if ((lkm = JoukkList->SelCount) < 1)
      return;
   taul = (int (*)[MAXEMITVA+1][2]) calloc(lkm, (MAXEMITVA+1)*2*sizeof(int));
   n = 0;
   if (lkm > MAXEMITVA)
	  lkm = MAXEMITVA;
   valitut = (int *) calloc(lkm, sizeof(int));
   for (int i = 0; i < JoukkList->Count; i++) {
	  if (n >= lkm)
         break;
      if (JoukkList->Selected[i]) {
		 kilpno = JoukkList->Items->Strings[i].SubString(0,4).ToInt();
		 if (kilpno > 0)
			valitut[n++] = kilpno;
		 }
      }
   lkm = n;
   kilp = (kilptietue *) calloc(lkm, sizeof(kilptietue));

   n = 0;
   EnterCriticalSection(&tall_CriticalSection);
   for (int i = 0; i < lkm; i++) {
      if ((kilpno = valitut[i]) > 0 && (d = getpos(kilpno)) > 0) {
         GETREC(kilp+n, d);
		 n++;
         }
      }
   lkm = n;
   LeaveCriticalSection(&tall_CriticalSection);

   n = 0;
   for (int i = 0; i < lkm; i++) {
      if ((kilpno = kilp[i].kilpno) > 0) {
         if (getem(&em, kilpno, os) >= 0 &&
			!laskeemitvaliajat(&em, kilp+i, &eva, 0, &valuku)) {
			for (int j = 0; j <= valuku; j++) {
			   if (j == 0) {
				  x = 0;
				  if (tiedot == 2)
					 y = osuuslahto(kilp+i, os)/SEK;
				  else
					 y = 0;
				  }
			   else {
				  if (eva[j-1].rvno < 0)
					 continue;
				  x += rvalit[eva[j-1].rvno].vertaika[kilp[i].sarja][yos];
				  switch (tiedot) {
					 case 0:
						y += eva[j-1].rvaika - rvalit[eva[j-1].rvno].vertaika[kilp[i].sarja][yos];
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
			free(eva);
			taul[n][MAXEMITVA][0] = kilpno;
			taul[n][MAXEMITVA][1] = valuku+1;
			n++;
			}
		 }
	  }
   DrawTaul(taul, n);
   free(taul);
   free(kilp);
   free(valitut);
}
//---------------------------------------------------------------------------
void __fastcall TFormVaGr::DrawTaul(int (*taul)[MAXEMITVA+1][2], int nrivi)
{
   int xVasT = 170*Screen->PixelsPerInch/96, xVas = 10*Screen->PixelsPerInch/96;
   int xOik, xMin, xMax = -2000000000, x;
   int yYlaT = 10*Screen->PixelsPerInch/96, yYla = 10*Screen->PixelsPerInch/96;
   int yAla, yMin = 2000000000, yMax = -2000000000, y;
   TRect Rect = TRect(0,0,650*Screen->PixelsPerInch/96,650*Screen->PixelsPerInch/96);
   TColor Colors[8];
   int xVali, xLisa = 5, edY = -1000;
   int tiedot, yEro = 0;

   tiedot = TietoVal->ItemIndex;
   if (tiedot) {
	  yEro = taul[0][taul[0][MAXEMITVA][1]-1][1] - taul[0][0][1];
	  }
   xMin = 0;
   for (int i = 0; i < nrivi; i++) {
	  xVali = taul[i][taul[i][MAXEMITVA][1]-1][0] - taul[i][0][0];
	  for (int j = 0; j < taul[i][MAXEMITVA][1]; j++) {
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
   pBitmap->Width = ClientWidth-170*Screen->PixelsPerInch/96;
   pBitmap->Height = ClientHeight-10*Screen->PixelsPerInch/96;
   Rect.Right = pBitmap->Width;
   Rect.Bottom = pBitmap->Height;
   pBitmap->Canvas->Brush->Color = clBtnFace;
   pBitmap->Canvas->FillRect(Rect);
   xOik = pBitmap->Width - 58*Screen->PixelsPerInch/96;
   Rect.Right = pBitmap->Width - 8*Screen->PixelsPerInch/96;
   yAla = pBitmap->Height - 18*Screen->PixelsPerInch/96;
   Rect.Bottom = pBitmap->Height - 8*Screen->PixelsPerInch/96;
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
   pBitmap->Canvas->TextOutW(xVas+10*Screen->PixelsPerInch/96,
		yAla-20*Screen->PixelsPerInch/96,
		AnsiString("Viivaväli ")+ AnsiString(y)+ AnsiString(" s"));
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
		 pBitmap->Canvas->TextOutW(xVas+5, yy-8, AnsiString((i*y)/60));
	  }
   if (tiedot)
	  xVali = taul[0][taul[0][MAXEMITVA][1]-1][0] - taul[0][0][0];
   for (int j = 1; j < taul[0][MAXEMITVA][1]-1; j++) {
	  x = (taul[0][j][0] - xMin)*(xOik-xVas)/xVali+xVas;
	  pBitmap->Canvas->MoveTo(x, yYla);
	  pBitmap->Canvas->LineTo(x, yAla);
	  pBitmap->Canvas->TextOutW(x-5, yYla+5, AnsiString(j));
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
		 xVali = taul[i][taul[i][MAXEMITVA][1]-1][0] - taul[i][0][0];
	  for (int j = 1; j < taul[i][MAXEMITVA][1]; j++) {
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
	  pBitmap->Canvas->TextOutW(xOik+xLisa, y-8, AnsiString(taul[i][MAXEMITVA][0]));
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
   JoukkList->Height = ClientHeight - JoukkList->Top - 31*Screen->PixelsPerInch/96;
   EdMaxLkm->Top = ClientHeight - 27*Screen->PixelsPerInch/96;
   Label1->Top = ClientHeight - 23*Screen->PixelsPerInch/96;
   PiirraBtnClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormVaGr::OsValChange(TObject *Sender)
{
   Osuus = OsVal->ItemIndex - 1;
   PaivitaClick(Sender);
   PiirraBtnClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormVaGr::FormActivate(TObject *Sender)
{
   SarjaVal->Clear();
   for (int srj = 0; srj < sarjaluku; srj++)
      SarjaVal->Items->Add(Sarjat[srj].sarjanimi);
   SarjaVal->ItemIndex = Sarja;
   OsVal->Clear();
   OsVal->Items->Add("Numerot");
   for (int os = 0; os < Sarjat[Sarja].osuusluku; os++)
      OsVal->Items->Add(AnsiString(os+1)+". osuus");
   OsVal->ItemIndex = Osuus + 1;
   PiirraBtnClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormVaGr::SarjaValChange(TObject *Sender)
{
   Sarja = SarjaVal->ItemIndex;
   if (Osuus >= Sarjat[Sarja].osuusluku)
      Osuus = Sarjat[Sarja].osuusluku - 1;
   OsVal->Clear();
   OsVal->Items->Add("Numerot");
   for (int os = 0; os < Sarjat[Sarja].osuusluku; os++)
      OsVal->Items->Add(AnsiString(os+1)+". osuus");
   OsVal->ItemIndex = Osuus + 1;
   OsValChange(Sender);
}
//---------------------------------------------------------------------------

#endif
