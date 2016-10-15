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

#include "UnitSeurGr.h"
#include "VDeclare.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TSeurantaForm *SeurantaForm;
//---------------------------------------------------------------------------
__fastcall TSeurantaForm::TSeurantaForm(TComponent* Owner)
   : TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
#ifdef EMIT
	int os;

	InitializeCriticalSection(&seur_CriticalSection);
	Sarja = 0;
	piirraPyynto = 1;
//   YlinOs->ItemIndex = Sarjat[Sarja].osuusluku - 1;
	for (os = Sarjat[Sarja].osuusluku-1; os >= 0; os--)
		if (ntulos[Sarja][os][0] > 1)
			break;
//   OsVal->ItemIndex = os+1;
#endif
}
//---------------------------------------------------------------------------

__fastcall TSeurantaForm::~TSeurantaForm(void)
{
   DeleteCriticalSection(&seur_CriticalSection);
}
//---------------------------------------------------------------------------
void __fastcall TSeurantaForm::PaivitaButtonClick(TObject *Sender)
{
   int d, kno, laji = 0, os, aos, lkm, ntls, p;
   kilptietue kilp;
   char line[60];
   wchar_t wline[80], wst[80], wmaa[8];
   int *valitut = 0, edlkm;
   int peruutafl = 1;

   EnterCriticalSection(&seur_CriticalSection);
   if (!paivitysflag) {
	  paivitysflag = 1;
	  peruutafl = 0;
      }
   LeaveCriticalSection(&seur_CriticalSection);
   if (peruutafl)
      return;

   if (Sarja != edSarja)
      JoukkList->Clear();
   edSarja = Sarja;
   listaPyynto = 0;
   if ((edlkm = JoukkList->SelCount) > 0) {
      valitut = (int *)calloc(edlkm, sizeof(int));
      int n = 0;
      for (int i = 0; i < JoukkList->Count; i++) {
         if (JoukkList->Selected[i])
				valitut[n++] = JoukkList->Items->Strings[i].SubString(0,4).ToInt();
			}
      }
   os = OsVal->ItemIndex - 1;
   swprintf(Kohde, L"%.12s-%.8s", SarjaVal->Items->Strings[SarjaVal->ItemIndex].c_str(),
	   OsVal->Items->Strings[OsVal->ItemIndex].c_str());
   FormMain->SetFormKohde(this, Kohde);
   EnterCriticalSection(&tall_CriticalSection);
   if (os >= 0) {
      if (os >= Sarjat[Sarja].osuusluku) {
         os = Sarjat[Sarja].osuusluku - 1;
         OsVal->ItemIndex = os + 1;
         }
	  laji = 0;
      while ((ntls = ntulos[Sarja][os][laji]) < 3 && laji != 1) {
         if (!laji)
			laji = kilpparam.valuku;
         else
            laji--;
		 }
	  if (laji < kilpparam.valuku)
		  p = jalku[Sarja][os][laji ? laji+4 : 0];
	  else
		  p = JosAlku(Sarja, os, &aos);
	  d = p;
	  }
   else {
	  ntls = nilm[Sarja];
	  for (kno = minkilpno; kno < maxkilpno; kno++) {
		 if ((d = getpos(kno)) > 0);
			break;
		 }
	  }
   lkm = EdMaxLkm->Text.ToInt();
   if (lkm > ntls)
	  lkm = ntls;
   JoukkList->Clear();
   for (int i = 0; i < lkm; i++) {
	  GETREC(&kilp, d);
		swprintf(wline, L"%04d %.20s %s", kilp.kilpno, kilp.Joukkue(wst), kilp.Maa());
	  JoukkList->Items->Add(AnsiString(wline));
	  if (os >= 0) {
		 p = JarjSeur(0, os, laji, p);
		 d = p;
		 }
	  else {
		 kno++;
		 for (; kno < maxkilpno; kno++) {
			if ((d = getpos(kno)) > 0);
			   break;
			}
		 }
	  if (d <= 0)
		 break;
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
   paivitysflag = 0;
}
//---------------------------------------------------------------------------

void __fastcall TSeurantaForm::PiirraBtnClick(TObject *Sender)
{
   char ch, cs, ch2, ct, line[80], trk, selitys[17];
	int d, osA, osY, rvali, sj, va , k, lkm, n, aos;
	int (*taul)[TAULDIM][2], x, y, kilpno, *valitut;
   kilptietue *kilp;
   int peruutafl = 1;

   EnterCriticalSection(&seur_CriticalSection);
   if (!paivitysflag) {
	  paivitysflag = 1;
      peruutafl = 0;
      }
   LeaveCriticalSection(&seur_CriticalSection);
   if (peruutafl)
      return;
   if ((lkm = JoukkList->SelCount) < 1) {
      paivitysflag = 0;
      return;
      }
   piirraPyynto = 0;
   osA = AlinOs->ItemIndex;
   osY = YlinOs->ItemIndex;
   if (lkm > 50)
      lkm = 50;
   taul = (int (*)[TAULDIM][2]) calloc(lkm+1, TAULDIM*2*sizeof(int));
   valitut = (int *) calloc(lkm, sizeof(int));
   for (int i = 0, n = 0; i < JoukkList->Count; i++) {
      if (n >= lkm)
         break;
      if (JoukkList->Selected[i]) {
		 kilpno = JoukkList->Items->Strings[i].SubString(0,4).ToInt();
		 if (kilpno > 0)
            valitut[n++] = kilpno;
			}
		}
   kilp = (kilptietue *) calloc(lkm, sizeof(kilptietue));

   EnterCriticalSection(&tall_CriticalSection);
   if (osA > 0 && ntulos[Sarja][osA-1][0] > 1 &&
	  (d = jalku[Sarja][osA-1][0]) > 0) {
	  GETREC(kilp, d);
	  taul[lkm][0][0] = tulos(kilp, osA-1, 0)/KSEK;
      }
   taul[lkm][0][1] = osA;

   for (int os = osA; os <= osY; os++) {
      for (int j = 0; j <= kilpparam.valuku; j++) {
		 n = (j+1)%(kilpparam.valuku+1);
		 if (ntulos[Sarja][os][n] >= 1) {
			if (n == kilpparam.valuku)
				d = JosAlku(Sarja, os, &aos);
			else
				d = jalku[Sarja][os][n ? n+4 : 0];
			if (d > 0) {
			   GETREC(kilp, d);
			   va = (os-osA)*(kilpparam.valuku+1)+j+1;
			   if (!n)
				  taul[lkm][va][1] = os+1;
			   taul[lkm][va][0] = tulos(kilp, os, n)/KSEK;
			   taul[lkm][TAULDIM-1][1] = va+1;
			   }
            }
         }
      }
   n = 0;
   for (int i = 0; i < lkm; i++) {
      if ((kilpno = valitut[i]) > 0 && (d = getpos(kilpno)) > 0) {
         GETREC(kilp+n, d);
         n++;
         }
		}
	LeaveCriticalSection(&tall_CriticalSection);

   n = 0;
   for (int i = 0; i < lkm; i++) {
      if (kilp[i].kilpno) {
         if (osA)
            taul[n][0][1] = tulos(kilp+i, osA-1, 0)/KSEK;
         taul[n][0][0] = taul[lkm][0][0];
         k = 1;
         for (int os = osA; os <= osY; os++) {
            for (int j = 0; j <= kilpparam.valuku; j++) {
               va = (os-osA)*(kilpparam.valuku+1)+j+1;
               int tl;
			   if ((tl = tulos(kilp+i, os, (j+1)%(kilpparam.valuku+1))/KSEK) > 0) {
                  taul[n][k][1] = tl;
                  taul[n][k][0] = taul[lkm][va][0];
                  k++;
                  }
               }
            }
		 taul[n][TAULDIM-1][0] = kilp[i].kilpno;
		 taul[n][TAULDIM-1][1] = k;
         n++;
         }
      }

   if (n < lkm) {
	  memcpy(taul+n, taul+lkm, sizeof(taul[0]));
	  }
   DrawTaul(taul, n);
   free(taul);
	free(kilp);
	free(valitut);
   paivitysflag = 0;
}
//---------------------------------------------------------------------------
void __fastcall TSeurantaForm::DrawTaul(int (*taul)[TAULDIM][2], int nrivi)
{
   int xVasT = 170, xVas = 10, xOik, xMin, xMax = -2000000000, x;
   int yYlaT = 10, yYla = 10, yAla, yMin = 2000000000, yMax = -2000000000, y;
   TRect Rect = TRect(0,0,650,650);
   TColor Colors[8];
   int xVali, xLisa = 5, edY = -1000;

   xMin = taul[nrivi][0][0];
   xMax = taul[nrivi][taul[nrivi][TAULDIM-1][1]-1][0];
   for (int i = 0; i < nrivi; i++) {
	  for (int j = 0; j < taul[i][TAULDIM-1][1]; j++) {
		 y = taul[i][j][1] - taul[i][j][0];
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
   pBitmap->Width = ClientWidth-170;
   pBitmap->Height = ClientHeight-10;
   Rect.Right = pBitmap->Width;
   Rect.Bottom = pBitmap->Height;
   pBitmap->Canvas->Brush->Color = clBtnFace;
	pBitmap->Canvas->FillRect(Rect);
	xOik = pBitmap->Width - 58;
   Rect.Right = pBitmap->Width - 8;
   yAla = pBitmap->Height - 18;
   Rect.Bottom = pBitmap->Height - 8;
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
   if (y > 6000)
	  y = (y / 6000) * 6000;
   else if (y > 3000)
	  y = 2000;
   else if (y > 600)
	  y = (y /600) * 600;
   else if (y > 300)
	  y = 300;
   else if (y > 100)
	  y = (y / 100) * 100;
   else if (y > 50)
		y = 50;
	pBitmap->Canvas->Font->Size = 10;
   pBitmap->Canvas->Font->Color = clBlack;
   pBitmap->Canvas->TextOutW(xVas+10, yAla-20, AnsiString("Viivaväli ")+ AnsiString(y/10)+ AnsiString(" s"));
   pBitmap->Canvas->Pen->Color = clLtGray;
   for (int i = -20; ; i++) {
	  int yy = taul[0][0][1]-taul[0][0][0] + i*y;
	  if (yy < yMin)
		 continue;
	  if (yy > yMax)
		 break;
	  yy = (yy - yMin)*(yAla-yYla)/(yMax-yMin)+yYla;
	  pBitmap->Canvas->MoveTo(xVas, yy);
	  pBitmap->Canvas->LineTo(xOik, yy);
	  if ((i*y)%600 == 0)
		 pBitmap->Canvas->TextOut(xVas+5, yy-8, AnsiString((i*y)/600));
	  }
   for (int j = 1; j < taul[nrivi][TAULDIM-1][1]-1; j++) {
	  if (taul[nrivi][j][0]) {
		 x = (taul[nrivi][j][0] - xMin)*(xOik-xVas)/xVali+xVas;
		 if (taul[nrivi][j][1])
			pBitmap->Canvas->Pen->Width = 2;
		 else
			pBitmap->Canvas->Pen->Width = 1;
		 pBitmap->Canvas->MoveTo(x, yYla);
		 pBitmap->Canvas->LineTo(x, yAla);
		 if (taul[nrivi][j][1])
			pBitmap->Canvas->TextOut(x-5, yYla+5, AnsiString(taul[nrivi][j][1]));
		 }
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
	  pBitmap->Canvas->MoveTo((taul[i][0][0] - xMin)*(xOik-xVas)/xVali+xVas,
		 (taul[i][0][1] - taul[i][0][0]- yMin)*(yAla-yYla)/(yMax-yMin)+yYla);
	  for (int j = 1; j < taul[i][TAULDIM-1][1]; j++) {
		 x = (taul[i][j][0] - xMin)*(xOik-xVas)/xVali+xVas;
		 y = taul[i][j][1] - taul[i][j][0];
		 y = (y - yMin)*(yAla-yYla)/(yMax-yMin)+yYla;
		 pBitmap->Canvas->LineTo(x, y);
		 }
	  pBitmap->Canvas->Font->Color = Colors[i%8];
	  if (abs(y-edY) < 15)
		 xLisa = 30 - xLisa;
	  else
		 xLisa = 5;
	  pBitmap->Canvas->TextOut(xOik+xLisa, y-8, AnsiString(taul[i][TAULDIM-1][0]));
	  edY = y;
	  }
   try {
		Canvas->Draw(xVasT, yYlaT, pBitmap);
		}
   catch (...) {
		}
   delete pBitmap;
}
#ifdef EI_OLE
//---------------------------------------------------------------------------
void __fastcall TSeurantaForm::DrawTaul(int (*taul)[TAULDIM][2], int nrivi)
{
	int xVas = 180, xOik = 800, xMin = 2000000000, xMax = -2000000000, x;
	int yYla = 20, yAla = 640, yMin = 2000000000, yMax = -2000000000, y;
   TRect Rect = TRect(170,10,820,660);
   TColor Colors[8];
   int xVali, xLisa = 5, edY = -1000;


   Rect.Right = ClientWidth;
   Rect.Bottom = ClientHeight;
   Canvas->Brush->Color = clBtnFace;
   Canvas->FillRect(Rect);
   xOik = ClientWidth - 58;
   Rect.Right = ClientWidth - 8;
   yAla = ClientHeight - 18;
   Rect.Bottom = ClientHeight - 8;
   Canvas->Brush->Color = clWhite;
   Colors[0] = clBlue;
   Colors[1] = clRed;
   Colors[2] = clGreen;
   Colors[3] = clBlack;
   Colors[4] = clSkyBlue;
   Colors[5] = clLime;
   Colors[6] = clMaroon;
   Colors[7] = clGray;
   Canvas->FillRect(Rect);
   xMin = taul[nrivi][0][0];
   xMax = taul[nrivi][taul[nrivi][TAULDIM-1][1]-1][0];
   for (int i = 0; i < nrivi; i++) {
	  for (int j = 0; j < taul[i][TAULDIM-1][1]; j++) {
		 y = taul[i][j][1] - taul[i][j][0];
		 if (y > yMax)
			yMax = y;
		 if (y < yMin)
				yMin = y;
			}
	  }
   xVali = xMax - xMin;
   if (xVali == 0 || yMax == yMin)
	  return;
   Canvas->Pen->Style = psSolid;
   Canvas->Pen->Color = clDkGray;
   Canvas->Pen->Width = 1;
   Canvas->Rectangle(xVas, yYla, xOik, yAla);
   y = (yMax-yMin)/8;
   if (y == 0)
	  y = 1;
   if (y > 6000)
	  y = (y / 6000) * 6000;
   else if (y > 3000)
	  y = 2000;
   else if (y > 600)
	  y = (y /600) * 600;
   else if (y > 300)
	  y = 300;
   else if (y > 100)
	  y = (y / 100) * 100;
   else if (y > 50)
	  y = 50;
   Canvas->Font->Size = 10;
   Canvas->Font->Color = clBlack;
   Canvas->TextOutA(xVas+10, yAla-20, AnsiString("Viivaväli ")+ AnsiString(y/10)+ AnsiString(" s"));
   Canvas->Pen->Color = clLtGray;
   for (int i = -20; ; i++) {
	  int yy = taul[0][0][1]-taul[0][0][0] + i*y;
	  if (yy < yMin)
		 continue;
		if (yy > yMax)
			break;
	  yy = (yy - yMin)*(yAla-yYla)/(yMax-yMin)+yYla;
	  Canvas->MoveTo(xVas, yy);
	  Canvas->LineTo(xOik, yy);
	  if ((i*y)%600 == 0)
		 Canvas->TextOutA(xVas+5, yy-8, AnsiString((i*y)/600));
	  }
   for (int j = 1; j < taul[nrivi][TAULDIM-1][1]-1; j++) {
	  if (taul[nrivi][j][0]) {
		 x = (taul[nrivi][j][0] - xMin)*(xOik-xVas)/xVali+xVas;
		 if (taul[nrivi][j][1])
			Canvas->Pen->Width = 2;
		 else
			Canvas->Pen->Width = 1;
		 Canvas->MoveTo(x, yYla);
		 Canvas->LineTo(x, yAla);
		 if (taul[nrivi][j][1])
			Canvas->TextOutA(x-5, yYla+5, AnsiString(taul[nrivi][j][1]));
		 }
	  }
   Canvas->Pen->Width = ViivaVal->ItemIndex+1;
   for (int i = 0; i < nrivi; i++) {
	  Canvas->Pen->Color = Colors[i%8];
	  if (i > 7) {
		 if (i > 15)
			Canvas->Pen->Style = psDash;
		 else
			Canvas->Pen->Style = psDot;
		 }
	  Canvas->MoveTo((taul[i][0][0] - xMin)*(xOik-xVas)/xVali+xVas,
		 (taul[i][0][1] - taul[i][0][0]- yMin)*(yAla-yYla)/(yMax-yMin)+yYla);
	  for (int j = 1; j < taul[i][TAULDIM-1][1]; j++) {
			x = (taul[i][j][0] - xMin)*(xOik-xVas)/xVali+xVas;
			y = taul[i][j][1] - taul[i][j][0];
		 y = (y - yMin)*(yAla-yYla)/(yMax-yMin)+yYla;
		 Canvas->LineTo(x, y);
		 }
	  Canvas->Font->Color = Colors[i%8];
	  if (abs(y-edY) < 15)
		 xLisa = 30 - xLisa;
	  else
		 xLisa = 5;
	  Canvas->TextOutA(xOik+xLisa, y-8, AnsiString(taul[i][TAULDIM-1][0]));
	  edY = y;
	  }
}
#endif
//---------------------------------------------------------------------------

void __fastcall TSeurantaForm::FormResize(TObject *Sender)
{
/*
   static int init;

   if (!init) {
      init = 1;
      return;
      }
*/
   JoukkList->Height = ClientHeight - 275;
   EdMaxLkm->Top = ClientHeight - 27;
   Label1->Top = ClientHeight - 23;
   Refresh();
}
//---------------------------------------------------------------------------


void __fastcall TSeurantaForm::JoukkListClick(TObject *Sender)
{
   PiirraBtnClick(Sender);
        piirraPyynto = 1;
}
//---------------------------------------------------------------------------

void __fastcall TSeurantaForm::YlinOsChange(TObject *Sender)
{
   PiirraBtnClick(Sender);
        piirraPyynto = 1;
}
//---------------------------------------------------------------------------

void __fastcall TSeurantaForm::AlinOsChange(TObject *Sender)
{
   PiirraBtnClick(Sender);
        piirraPyynto = 1;
}
//---------------------------------------------------------------------------

void __fastcall TSeurantaForm::ViivaValChange(TObject *Sender)
{
   PiirraBtnClick(Sender);
        piirraPyynto = 1;
}
//---------------------------------------------------------------------------

void __fastcall TSeurantaForm::OsValChange(TObject *Sender)
{
   Osuus = OsVal->ItemIndex - 1;
    PaivitaButtonClick(Sender);
		  listaPyynto = 1;
}
//---------------------------------------------------------------------------

void __fastcall TSeurantaForm::FormActivate(TObject *Sender)
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
        piirraPyynto = 1;
}
//---------------------------------------------------------------------------

void __fastcall TSeurantaForm::SarjaValChange(TObject *Sender)
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

