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

#include "TapUnit2.h"
#include "WinHk.h"
#include "HkDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TTapForm *TapForm;
//static int nRows;

//---------------------------------------------------------------------------
__fastcall TTapForm::TTapForm(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	Valittu = 0;
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::FormCreate(TObject *Sender)
{
	IkkParam = tapIkkParam0;
	IkkParam.laika = pvparam[k_pv].laika2;
	applyParams();
	FormSetup(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TTapForm::FormSetup(TObject *Sender)
{
	lev = IkkParam.FontSize + (IkkParam.Lihavoitu ? 2 : 0);
	TapGrid->DefaultRowHeight = 18 * Screen->PixelsPerInch * IkkParam.FontSize / 960;
	colCount1 = (k_pv > 0 ? 13 : 10) +
		(wcswcind(kilpparam.kilplaji, L"NP") >= 0 ? 1 : 0);
	colCount2 = colCount1;
	TapGrid->ColCount = colCount1 +
		(IkkParam.Ennatys > 0 ? 1 : 0) +
		(IkkParam.Tavoite > 0 ? 1 : 0);
	TapGrid->ColWidths[0] = IkkParam.ColW[0] * Screen->PixelsPerInch * lev / 960;
	TapGrid->ColWidths[1] = IkkParam.ColW[1] * Screen->PixelsPerInch * lev / 960;
	TapGrid->ColWidths[2] = IkkParam.ColW[2] * Screen->PixelsPerInch * lev / 960;
	TapGrid->ColWidths[3] = IkkParam.ColW[3] * Screen->PixelsPerInch * lev / 960;
	TapGrid->ColWidths[4] = IkkParam.ColW[4] * Screen->PixelsPerInch * lev / 960;
	TapGrid->ColWidths[5] = IkkParam.ColW[5] * Screen->PixelsPerInch * lev / 960;
	TapGrid->ColWidths[6] = IkkParam.ColW[6] * Screen->PixelsPerInch * lev / 960;
	TapGrid->ColWidths[7] = IkkParam.ColW[7] * Screen->PixelsPerInch * lev / 960;
	TapGrid->ColWidths[8] = IkkParam.ColW[8] * Screen->PixelsPerInch * lev / 960;
	TapGrid->ColWidths[9] = IkkParam.ColW[9] * Screen->PixelsPerInch * lev / 960;
	if (k_pv > 0) {
		TapGrid->ColWidths[10] = IkkParam.ColW[10] * Screen->PixelsPerInch * lev / 960;
		TapGrid->ColWidths[11] = IkkParam.ColW[11] * Screen->PixelsPerInch * lev / 960;
		TapGrid->ColWidths[12] = IkkParam.ColW[12] * Screen->PixelsPerInch * lev / 960;
		}
	if (wcswcind(kilpparam.kilplaji, L"NP") >= 0)
		TapGrid->ColWidths[TapGrid->ColCount-1] = IkkParam.ColW[13] * Screen->PixelsPerInch * lev / 960;
	if (IkkParam.Ennatys) {
		IkkParam.Ennatys = colCount2++;
		if (IkkParam.ColW[14])
			TapGrid->ColWidths[IkkParam.Ennatys] = IkkParam.ColW[14] * Screen->PixelsPerInch * lev / 960;
		Cells[IkkParam.Ennatys][0].text = L"Ennätys";
		}
	if (IkkParam.Tavoite) {
		IkkParam.Tavoite = colCount2++;
		if (IkkParam.ColW[15])
			TapGrid->ColWidths[IkkParam.Tavoite] = IkkParam.ColW[15] * Screen->PixelsPerInch * lev / 960;
		Cells[IkkParam.Tavoite][0].text = L"Tavoite";
		}
}
//---------------------------------------------------------------------------
tapIkkParamClass::tapIkkParamClass(void)
{
	memset(this, 0, sizeof(tapIkkParamClass));
	seuraTieto = 1;
	FontSize = 10;
	yhttulos = true;
	laika = 10;
//	korRaja[0] = MINUUTTI;
//	korRaja[1] = 3*MINUUTTI;
	naytaMaa = true;
	KokoRuutu = false;
	ColW[0] = 38;
	ColW[1] = 200;
	ColW[2] = 55+(seuraTieto == 1 ? 120 : 0);
	ColW[3] = 35;
	ColW[4] = 55;
	ColW[5] = 35;
	ColW[6] = 35;
	ColW[7] = 55;
	ColW[8] = 50;
	if (kilpparam.kilplaji == L'A')
		ColW[9] = 40;
	else
		ColW[9] = 30;
	ColW[10] = 35;
	ColW[11] = 55;
	ColW[12] = 50;
	ColW[13] = 35;
	ColW[14] = 55;
	ColW[15] = 55;
}
//---------------------------------------------------------------------------
void tapIkkParamClass::operator=(tapIkkParamClass& ikkPar)
{
	memcpy(this, &ikkPar, sizeof(tapIkkParamClass));
}
//---------------------------------------------------------------------------
int TTapForm::applyParams(void)
{
	N1sek->Checked = IkkParam.laika <= 9;
	N01sek->Checked = IkkParam.laika == 10;
	N001sek->Checked = IkkParam.laika == 11;
	N001sek->Checked = IkkParam.laika >= 12;
	switch (IkkParam.FontSize) {
		case 8:
			Pieni1->Checked = true;
			Keski1->Checked = false;
			break;
		case 10:
			Pieni1->Checked = false;
			Keski1->Checked = true;
			break;
		case 12:
			Pieni1->Checked = false;
			Keski1->Checked = false;
			Suuri1->Checked = true;
			break;
		default:
			Pieni1->Checked = false;
			Keski1->Checked = false;
			Muumerkkikoko1->Checked = true;
		}
	Valittu = IkkParam.Va;
	PisteVal->ItemIndex = Valittu;
	Piilotavalintatiedot1->Checked = IkkParam.KokoRuutu;
	Panel1->Visible = !IkkParam.KokoRuutu;
	Nytmaa1->Checked = IkkParam.naytaMaa;
	Etunimiensin1->Checked = IkkParam.EtuSuku;
	Nytyhteistulokset1->Checked = IkkParam.yhttulos;
	return(0);
}
//---------------------------------------------------------------------------
void __fastcall TTapForm::Paivita(TObject *Sender)
{
	int k = 1, lkm = 0, valinta, pst, ii ;
	tapTp *T;
	wchar_t ln[120];

	int viim = viimTap + MAXTAPAHTUMA;
	Valittu = PisteVal->ItemIndex;
	IkkParam.Va = Valittu;
	Etunimiensin1->Checked = IkkParam.EtuSuku;
	wcsncpy(Kohde, PisteVal->Items->Strings[PisteVal->ItemIndex].c_str(), sizeof(Kohde)/2-1);
	FormMain->SetFormKohde(this, Kohde);
	valinta = Valittu-1;
	if (valinta == 1)
		valinta = -2;
	if (valinta > 1)
		valinta--;
	if (valinta > kilpparam.maxvaluku)
		valinta = kilpparam.valuku + 2;
	for (int i = viim; i > viim - MAXTAPAHTUMA + 1; i--) {
		T = &Tapahtuma[i%MAXTAPAHTUMA];
		if (T->Kilpno) {
			if (valinta == -1 ||
				(valinta == -2 && T->Piste > 0 && T->Piste <= Sarjat[T->Sarja].valuku[k_pv]) ||
				 valinta == T->Piste)
				lkm++;
			}
		else
			break;
		}
	if (EdtEra->Visible)
		EdtEra->Text = era_akt;
	TapGrid->RowCount = (lkm > 0 ? lkm+1 : 2);
	TapGrid->FixedRows = 1;
	if (lkm == 0) {
		for (int i = 0; i < TapGrid->ColCount; i++) {
			Cells[i][1].text = L"";
			Cells[i][1].style = 0;
			}
		}
	else {
		for (int i = viim; i > viim - MAXTAPAHTUMA + 1; i--) {
			kilptietue kilp;
			int d;
			T = &Tapahtuma[i%MAXTAPAHTUMA];
			if (T->Kilpno == 0)
				break;
			if ((valinta >= 0 && T->Piste != valinta) ||
				(valinta == -2 && (T->Piste == 0 || T->Piste > Sarjat[T->Sarja].valuku[k_pv])))
				continue;
			if ((d = getpos(T->Kilpno)) > 0)
				kilp.GETREC(getpos(T->Kilpno));
			else
				kilp.nollaa();
			Cells[0][k].text = T->Kilpno;
			kilp.nimi(ln, 99, IkkParam.EtuSuku);
			Cells[1][k].text = ln;
			wcsncpy(ln, kilp.seuratieto(0, IkkParam.seuraTieto == 0), 99);
			Cells[2][k].text = ln;
			if (IkkParam.naytaMaa)
				Cells[3][k].text = T->Maa;
			wcsncpy(ln, Sarjat[T->Sarja].sarjanimi, 39);
			ln[39] = 0;
			Cells[4][k].text = UnicodeString(ln);
			wcscpy(ln, L"M");
			pst = T->Piste;
			if (pst == Sarjat[T->Sarja].valuku[k_pv]+1)
				{
				pst = 0;
				wcscpy(ln, L"KH");
				}
			else if (pst == kilpparam.valuku+2)
				{
				wcscpy(ln, L"Sakko");
				}
			else if (T->Piste)
				_itow(T->Piste, ln, 10);
			Cells[5][k].text = UnicodeString(ln);
			if (pst == kilpparam.valuku+2) {
				Cells[6][k].text = L" ";
				Cells[7][k].text = L" ";
				Cells[8][k].text = L" ";
				}
			else {
				AIKATOWSTRS(ln, T->Tulos, 0);
				ln[IkkParam.laika] = 0;
				elimwzb1(ln);
				Cells[7][k].text = UnicodeString(ln);
				if (!k_pv || !pst || Sarjat[T->Sarja].luonne[k_pv] != 2 || !Nytyhteistulokset1->Checked) {
					Cells[6][k].text = T->Sija;
					AIKATOWSTRS(ln, T->Tulos - pkarki[T->Sarja][pst], 0);
					ln[IkkParam.laika] = 0;
					}
				else {
					wcscpy(ln, L" ");
					Cells[6][k].text = L" ";
					}
				elimwzb1(ln);
				Cells[8][k].text = UnicodeString(ln);
				}
			Cells[9][k].text = T->Tark;
			int style = 0;
			if (T->Tark == L"H" || T->Tark == L"K")
				style = 3;
			else if ((ii = getpos(T->Kilpno)) > 0 && vipLaji[ii] != 0) {
				switch (vipLaji[ii]) {
					case 2:
						style = 5;
						break;
					case 3:
						style = 6;
						break;
					default:
						style = 4;
					}
				 }
			else {
				if (T->Sija > 0 && T->Sija <= korRaja[T->Sarja][0])
					style = 1;
				else
					if (T->Sija > 0 && T->Sija <= korRaja[T->Sarja][1])
						style = 2;
				}
			if (wcswcind(kilpparam.kilplaji, L"NP") >= 0) {
				Cells[TapGrid->ColCount-1][k].text = T->pisteet;
				}
			if (k_pv && Nytyhteistulokset1->Checked) {
				if (Sarjat[T->Sarja].pisteytulos() == false && wcswcind(kilpparam.kilplaji, L"NP") < 0) {
					AIKATOWSTRS(ln, T->yTulos, 0);
					ln[IkkParam.laika] = 0;
					elimwzb1(ln);
					}
				else if (!Sarjat[T->Sarja].nouspisteet() || wcswcind(kilpparam.kilplaji, L"NP") >= 0) {
					_itow((PISTEKATTO - T->yTulos)/SEK, ln, 10);
					}
				else
					_itow(T->yTulos/SEK, ln, 10);
				Cells[11][k].text = UnicodeString(ln);
				Cells[10][k].text = L" ";
				ln[0] = 0;
				if (!pst && T->yTulos) {
					Cells[10][k].text = T->ySija;
					if (Sarjat[T->Sarja].pisteytulos() == false && wcswcind(kilpparam.kilplaji, L"NP") < 0) {
						AIKATOWSTRS(ln, T->yTulos - pkarki[T->Sarja][kilpparam.valuku+1], 0);
						ln[IkkParam.laika] = 0;
						elimwzb1(ln);
						}
					else if (!Sarjat[T->Sarja].nouspisteet() || wcswcind(kilpparam.kilplaji, L"NP") >= 0)
						_itow(-(T->yTulos - pkarki[T->Sarja][kilpparam.valuku+1])/SEK, ln, 10);
					else
						_itow((T->yTulos - pkarki[T->Sarja][kilpparam.valuku+1])/SEK, ln, 10);
					}
				else if (T->yTulos && Sarjat[T->Sarja].luonne[k_pv] && pst <= Sarjat[T->Sarja].valuku[k_pv]) {
					AIKATOWSTRS(ln, T->yTulos - pkarki[T->Sarja][pst], 0);
					Cells[10][k].text = T->Sija;
					ln[IkkParam.laika] = 0;
					elimwzb1(ln);
					}
				else
					wcscpy(ln, L" ");
				Cells[12][k].text = UnicodeString(ln);
				if (style < 3 && T->ySija > 0 && T->ySija <= korRaja[T->Sarja][0])
					style = 1;
				else
					if (style == 0 && T->ySija > 0 && T->ySija <= korRaja[T->Sarja][1])
						style = 2;
				}
			if (IkkParam.Ennatys) {
				if (kilp.pv[k_pv].enn) {
					aikatowstr_s(ln, kilp.pv[k_pv].enn/SEK, 0);
					elimwzb1(ln);
					}
				else
					ln[0] = 0;
				Cells[IkkParam.Ennatys][k].text = UnicodeString(ln);
				if (T->Tulos > 0 && T->Tulos <= kilp.pv[k_pv].enn)
					Cells[IkkParam.Ennatys][k].style = 6;
				else
					Cells[IkkParam.Ennatys][k].style = style;
				}
			if (IkkParam.Tavoite) {
				if (kilp.pv[k_pv].tav) {
					aikatowstr_s(ln, kilp.pv[k_pv].tav/SEK, 0);
					elimwzb1(ln);
					}
				else
					ln[0] = 0;
				Cells[IkkParam.Tavoite][k].text = UnicodeString(ln);
				if (T->Tulos > 0 && T->Tulos <= kilp.pv[k_pv].tav)
					Cells[IkkParam.Tavoite][k].style = 6;
				else
					Cells[IkkParam.Tavoite][k].style = style;
				}
			for (int j = 0; j < TapGrid->ColCount; j++) {
				if (j == 0 || (j != IkkParam.Ennatys && j != IkkParam.Tavoite))
					Cells[j][k].style = style;
				Cells[j][k].justify = (j > 0 && j < 5) ? 0 : 1;
				}
			k++;
		 }
	  }
	TapGrid->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TTapForm::Button1Click(TObject *Sender)
{
	 Paivita(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TTapForm::PisteValChange(TObject *Sender)
{
	Valittu = PisteVal->ItemIndex;
}
//---------------------------------------------------------------------------
void __fastcall TTapForm::Skaalaa(void)
{
	int GrdWidth;

	colCount1 = (k_pv > 0 ? 13 : 10) +
		(wcswcind(kilpparam.kilplaji, L"NP") >= 0 ? 1 : 0);
	TapGrid->ColCount = colCount1 +
		(IkkParam.Ennatys > 0 ? 1 : 0) +
		(IkkParam.Tavoite > 0 ? 1 : 0);
	TapGrid->FixedRows = 1;
	lev = IkkParam.FontSize + (IkkParam.Lihavoitu ? 2 : 0);
	TapGrid->DefaultRowHeight = 18 * Screen->PixelsPerInch * IkkParam.FontSize / 960;
	TapGrid->ColWidths[0] = IkkParam.ColW[0] * Screen->PixelsPerInch * lev / 960;
	TapGrid->ColWidths[1] = ((k_pv && Nytyhteistulokset1->Checked) ? 130 : 200)
								* Screen->PixelsPerInch * IkkParam.FontSize / 960;
//	TapGrid->ColWidths[2] = (55+(IkkParam.seuraTieto == 1 ? 120 : 0)) * Screen->PixelsPerInch * lev / 960;
	TapGrid->ColWidths[1] = IkkParam.ColW[1] * Screen->PixelsPerInch * lev / 960;
	TapGrid->ColWidths[2] = IkkParam.ColW[2] * Screen->PixelsPerInch * lev / 960;
	TapGrid->ColWidths[3] = IkkParam.naytaMaa ? IkkParam.ColW[3] * Screen->PixelsPerInch * lev / 960 : 0;
	TapGrid->ColWidths[4] = IkkParam.ColW[4] * Screen->PixelsPerInch * lev / 960;
	TapGrid->ColWidths[5] = IkkParam.ColW[5] * Screen->PixelsPerInch * lev / 960;
	TapGrid->ColWidths[6] = IkkParam.ColW[6] * Screen->PixelsPerInch * lev / 960;
	TapGrid->ColWidths[7] = IkkParam.ColW[7] * Screen->PixelsPerInch * lev / 960;
	TapGrid->ColWidths[8] = IkkParam.ColW[8] * Screen->PixelsPerInch * lev / 960;
	TapGrid->ColWidths[9] = IkkParam.ColW[9] * Screen->PixelsPerInch * lev / 960;
	Cells[0][0].text = L"Kilpno";
	Cells[1][0].text = L"Nimi";
	Cells[2][0].text = L"Seura";
	Cells[3][0].text = L"Maa";
	Cells[4][0].text = L"Sarja";
	Cells[5][0].text = L"Piste";
	Cells[6][0].text = L"Sija";
	Cells[7][0].text = L"Tulos";
	Cells[8][0].text = L"Ero";
	if (kilpparam.kilplaji == L'A') {
		Cells[9][0].text = L"Sakot";
		}
	else
		Cells[9][0].text = L"Trk";
	if (k_pv && Nytyhteistulokset1->Checked) {
		TapGrid->ColWidths[10] = IkkParam.ColW[10] * Screen->PixelsPerInch * lev / 960;
		TapGrid->ColWidths[11] = IkkParam.ColW[11] * Screen->PixelsPerInch * lev / 960;
		TapGrid->ColWidths[12] = IkkParam.ColW[12] * Screen->PixelsPerInch * lev / 960;
		Cells[10][0].text = L"Y-sija";
		Cells[11][0].text = L"Y-tulos";
		Cells[12][0].text = L"Y-ero";
		}
	if (IkkParam.Ennatys) {
		TapGrid->ColWidths[IkkParam.Ennatys] = IkkParam.ColW[14] * Screen->PixelsPerInch * lev / 960;
		Cells[IkkParam.Ennatys][0].text = L"Ennätys";
		}
	if (IkkParam.Tavoite) {
		TapGrid->ColWidths[IkkParam.Tavoite] = IkkParam.ColW[15] * Screen->PixelsPerInch * lev / 960;
		Cells[IkkParam.Tavoite][0].text = L"Tavoite";
		}
	if (wcswcind(kilpparam.kilplaji, L"NP") >= 0) {
		TapGrid->ColWidths[colCount1-1] = IkkParam.ColW[13] * Screen->PixelsPerInch * lev / 960;
		Cells[colCount1-1][0].text = L"Vaihe";
		}
	GrdWidth = 16 * Screen->PixelsPerInch / 96;
	for (int i = 0; i < TapGrid->ColCount; i++) {
		GrdWidth += TapGrid->ColWidths[i] + 2;
	}
	Width = GrdWidth + 8 * Screen->PixelsPerInch / 96;
	ClientWidth = Width - 8 * Screen->PixelsPerInch / 96;
	TapGrid->Width = GrdWidth;
}
//---------------------------------------------------------------------------
void __fastcall TTapForm::FormShow(TObject *Sender)
{
	EdtEra->Visible = (wcswcind(kilpparam.kilplaji, L"NPU") >= 0);
	LblEra->Visible = EdtEra->Visible;
	Valittu = PisteVal->ItemIndex;
	Etunimiensin1->Checked = IkkParam.EtuSuku;
	PisteVal->Clear();
	PisteVal->Items->Add(L"Kaikki");
	PisteVal->Items->Add(L"Maali");
	if (kilpparam.maxvaluku > 0) {
		PisteVal->Items->Add(L"Kaikki väliajat");
		for (int i = 1; i <= kilpparam.maxvaluku; i++) {
			PisteVal->Items->Add(UnicodeString(i)+L". väliaika");
			}
		}
	if (kilpparam.kilplaji == L'A')
		PisteVal->Items->Add(L"Ammunnat");
	PisteVal->ItemIndex = Valittu;
	Skaalaa();
}
//---------------------------------------------------------------------------
void __fastcall TTapForm::FormResize(TObject *Sender)
{
	TapGrid->Top = Panel1->Visible ? Panel1->Height : 0;
	Panel1->Width = ClientWidth;
	TapGrid->Width = ClientWidth;
	TapGrid->Height = ClientHeight - TapGrid->Top;
}
//---------------------------------------------------------------------------
void __fastcall TTapForm::TapGridDrawCell(TObject *Sender, int ACol,
	  int ARow, TRect &Rect, TGridDrawState State)
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
   if (ARow == 0)
      pBitmap->Canvas->Brush->Color = clBtnFace;
   else {
	  switch (Cells[ACol][ARow].style) {
		case 0:
			pBitmap->Canvas->Brush->Color = clWindow;
			pBitmap->Canvas->Font->Style = TFontStyles();
			break;
		case 1:
			pBitmap->Canvas->Brush->Color = TColor(korVari[0]);  //clLime;
//            pBitmap->Canvas->Font->Style = TFontStyles() << fsBold;
			break;
		case 2:
			pBitmap->Canvas->Brush->Color = TColor(korVari[1]);  //clYellow;
//            pBitmap->Canvas->Font->Style = TFontStyles() << fsBold;
			break;
		case 3:
			pBitmap->Canvas->Brush->Color = TColor(korVari[2]);  //clFuchsia;
			break;
		case 4:
			pBitmap->Canvas->Brush->Color = TColor(korVari[3]);  //clSkyBlue;
			break;
		case 5:
			pBitmap->Canvas->Brush->Color = TColor(korVari[4]);  //clAqua;
			break;
		case 6:
			pBitmap->Canvas->Brush->Color = TColor(korVari[5]);  //clOlive - light;
			break;
		 }
	  }
   pBitmap->Canvas->FillRect(Rect1);
   pBitmap->Canvas->Font->Size = IkkParam.FontSize;
   X = 1;
   if (Cells[ACol][ARow].justify == 1) {
	   X =  Rect.Width() - pBitmap->Canvas->TextWidth(Cells[ACol][ARow].text) - 2;
	   if (X < 1)
		  X = 1;
	   }
   pBitmap->Canvas->TextOut(X,1,Cells[ACol][ARow].text);
//   pBitmap->Canvas->TextRect(Rect, X,1,Cells[ACol][ARow].text);
   TapGrid->Canvas->Draw(Rect.Left, Rect.Top, pBitmap);
   delete pBitmap;
}
//---------------------------------------------------------------------------
void __fastcall TTapForm::TapGridSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect)

{
	CurrentRow = ARow;
	CurrentCol = ACol;
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::TapGridDblClick(TObject *Sender)
{
	int KilpNo;

   if (TapGrid->Row > 0 && Cells[0][TapGrid->Row].text.Length() > 0
		&& (KilpNo = Cells[0][TapGrid->Row].text.ToInt()) > 0) {
		if (!FormKilpailijaOnlineTap)
			FormKilpailijaOnlineTap = new TFormKilpailijaOnline(this);
		FormKilpailijaOnlineTap->sallimuokkaus = false;
		FormKilpailijaOnlineTap->naytaKilpailija(getpos(KilpNo));
		FormKilpailijaOnlineTap->Show();
		if (FormKilpailijaOnlineTap->WindowState == wsMinimized)
			FormKilpailijaOnlineTap->WindowState = wsNormal;
		FormKilpailijaOnlineTap->BringToFront();
   }
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::Pieni1Click(TObject *Sender)
{
	Pieni1->Checked = true;
	IkkParam.FontSize = 8;
	Keski1->Checked = false;
	Suuri1->Checked = false;
	Muumerkkikoko1->Checked = false;
	Skaalaa();
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::Keski1Click(TObject *Sender)
{
	Keski1->Checked = true;
	IkkParam.FontSize = 10;
	Pieni1->Checked = false;
	Suuri1->Checked = false;
	Muumerkkikoko1->Checked = false;
	Skaalaa();
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::Suuri1Click(TObject *Sender)
{
	Suuri1->Checked = true;
	IkkParam.FontSize = 12;
	Keski1->Checked = false;
	Pieni1->Checked = false;
	Muumerkkikoko1->Checked = false;
	Skaalaa();
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::Muumerkkikoko1Click(TObject *Sender)
{
	wchar_t ch;
	int fnt;

	fnt = IkkParam.FontSize;
	inputint_prompt(&fnt, L"Valitse koko pisteinä", &ch, this);
	if (ch == ESC)
		return;
	IkkParam.FontSize = fnt;
	Suuri1->Checked = false;
	Keski1->Checked = false;
	Pieni1->Checked = false;
	Muumerkkikoko1->Checked = true;
	Skaalaa();
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Kilpailun seuranta");
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::Nytyhteistulokset1Click(TObject *Sender)
{
	Nytyhteistulokset1->Checked = !Nytyhteistulokset1->Checked;
	IkkParam.yhttulos = Nytyhteistulokset1->Checked;
	Skaalaa();
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::Nytseuralyhenne1Click(TObject *Sender)
{
	Nytseuralyhenne1->Checked = !Nytseuralyhenne1->Checked;
	IkkParam.seuraTieto = !Nytseuralyhenne1->Checked;
	Skaalaa();
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (FormKilpailijaOnlineTap) {
		FormKilpailijaOnlineTap->Close();
		delete FormKilpailijaOnlineTap;
		FormKilpailijaOnlineTap = NULL;
		}
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::Etunimiensin1Click(TObject *Sender)
{
	Etunimiensin1->Checked = !Etunimiensin1->Checked;
	IkkParam.EtuSuku = Etunimiensin1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::Sulje1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::N1sekClick(TObject *Sender)
{
	N1sek->Checked = true;
	N01sek->Checked = false;
	N001sek->Checked = false;
	N0001sek->Checked = false;
	IkkParam.laika = 8;
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::N01sekClick(TObject *Sender)
{
	N1sek->Checked = false;
	N01sek->Checked = true;
	N001sek->Checked = false;
	N0001sek->Checked = false;
	IkkParam.laika = 10;
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::N001sekClick(TObject *Sender)
{
	N1sek->Checked = false;
	N01sek->Checked = false;
	N001sek->Checked = true;
	N0001sek->Checked = false;
	IkkParam.laika = 11;
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::N0001sekClick(TObject *Sender)
{
	N1sek->Checked = false;
	N01sek->Checked = false;
	N001sek->Checked = false;
	N0001sek->Checked = true;
	IkkParam.laika = 12;
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::allennasarakeleveydet1Click(TObject *Sender)
{
	for	(int c = 0; c < TapGrid->ColCount; c++)
		IkkParam.ColW[c] = (960 * TapGrid->ColWidths[c]) /
			(Screen->PixelsPerInch * lev);
	if (TapGrid->ColCount == 11)
		IkkParam.ColW[13] = IkkParam.ColW[10];
	if (!IkkParam.naytaMaa && IkkParam.ColW[3] < 20)
		IkkParam.ColW[3] = 35;
}
//---------------------------------------------------------------------------


void __fastcall TTapForm::Nytmaa1Click(TObject *Sender)
{
	Nytmaa1->Checked = !Nytmaa1->Checked;
	IkkParam.naytaMaa = Nytmaa1->Checked;
	Skaalaa();
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::Piilotavalintatiedot1Click(TObject *Sender)
{
	Piilotavalintatiedot1->Checked = !Piilotavalintatiedot1->Checked;
	IkkParam.KokoRuutu = Piilotavalintatiedot1->Checked;
	Panel1->Visible = !IkkParam.KokoRuutu;
	FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::Nyttavoite1Click(TObject *Sender)
{
	Nyttavoite1->Checked = !Nyttavoite1->Checked;
	IkkParam.Tavoite = Nyttavoite1->Checked ? colCount1 + 2 : 0;
	FormSetup(Sender);
	FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::Nytomaenntys1Click(TObject *Sender)
{
	Nytomaenntys1->Checked = !Nytomaenntys1->Checked;
	IkkParam.Ennatys = Nytomaenntys1->Checked ? colCount1 + 1 : 0;
	FormSetup(Sender);
	FormResize(Sender);
}
//---------------------------------------------------------------------------

