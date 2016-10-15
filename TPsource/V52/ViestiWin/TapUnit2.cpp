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
#include "VDeclare.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TTapForm *TapForm;
extern tapTp Tapahtuma[MAXTAPAHTUMA];
extern int viimTap;
extern bool TapEtunimiensin;

//---------------------------------------------------------------------------
__fastcall TTapForm::TTapForm(TComponent* Owner)
   : TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
}
//---------------------------------------------------------------------------
tapTp::tapTp(void)
{
	Kilpno = 0;
	Joukkue = L"";
	Nimi = L"";
	Maa = L"";
	Sarja = 0;
	Osuus = 0;
	Piste = 0;
	Sija = 0;
	Tulos = 0;
	Tark = L"";
}
//---------------------------------------------------------------------------
void tapTp::operator=(tapTp& Tap)
{
	Kilpno = Tap.Kilpno;
	Joukkue = Tap.Joukkue;
	Nimi = Tap.Nimi;
	Maa = Tap.Maa;
	Sarja = Tap.Sarja;
	Osuus = Tap.Osuus;
	Piste = Tap.Piste;
	Sija = Tap.Sija;
	Tulos = Tap.Tulos;
	Tark = Tap.Tark;
}
//---------------------------------------------------------------------------
bool tapTp::operator==(tapTp& Tap)
{
	return(
		Kilpno == Tap.Kilpno &&
		Joukkue == Tap.Joukkue &&
		Nimi == Tap.Nimi &&
		Maa == Tap.Maa &&
		Sarja == Tap.Sarja &&
		Osuus == Tap.Osuus &&
		Piste == Tap.Piste &&
		Sija == Tap.Sija &&
		Tulos == Tap.Tulos &&
		Tark == Tap.Tark);
}

//---------------------------------------------------------------------------
tapIkkParamClass::tapIkkParamClass(void)
{
	memset(this, 0, sizeof(tapIkkParamClass));
	FontSize = 10;
	laika = 10;
	naytaMaa = true;
	KokoRuutu = false;
	EtuSuku = true;
	Lihavoitu = false;
	OhitaTarkSan = true;
	Va = 0;
	ColW[0] = 38;
	ColW[1] = 38;
	ColW[2] = 150;
	ColW[3] = 150;
	ColW[4] = 35;
	ColW[5] = 55;
	ColW[6] = 35;
	ColW[7] = 35;
	ColW[8] = vuorokaudet ? 75 : 56;
	ColW[9] = 56;
	ColW[10] = 36;
}
//---------------------------------------------------------------------------
void tapIkkParamClass::operator=(tapIkkParamClass& ikkPar)
{
	memcpy(this, &ikkPar, sizeof(tapIkkParamClass));
}
//---------------------------------------------------------------------------
int TTapForm::applyParams(void)
{
/*
	N1sek->Checked = IkkParam.laika <= 9;
	N01sek->Checked = IkkParam.laika == 10;
	N001sek->Checked = IkkParam.laika == 11;
	N001sek->Checked = IkkParam.laika >= 12;
*/
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
	Lihavoitu1->Checked = IkkParam.Lihavoitu;
	Valittu = IkkParam.Va;
	PisteVal->ItemIndex = Valittu;
//	Piilotavalintatiedot1->Checked = IkkParam.KokoRuutu;
//	Panel1->Visible = !IkkParam.KokoRuutu;
//	Nytmaa1->Checked = IkkParam.naytaMaa;
	Etunimiensin1->Checked = IkkParam.EtuSuku;
	TapEtunimiensin = IkkParam.EtuSuku;
	CBTarkSan->Checked = !IkkParam.OhitaTarkSan;
	tarktapahtumat = !IkkParam.OhitaTarkSan;
	return(0);
}
//---------------------------------------------------------------------------
void __fastcall TTapForm::Paivita(TObject *Sender)
{
   int k = 1, lkm = 0, valinta, yosuus, d;
   tapTp *T;
   char ln[40];
   wchar_t wln[100], wst[40];

	int viim = viimTap + MAXTAPAHTUMA;
	valinta = PisteVal->ItemIndex - 1 - kierrokset;
	wcsncpy(Kohde, PisteVal->Items->Strings[PisteVal->ItemIndex].c_str(), sizeof(Kohde)/2-1);
	FormMain->SetFormKohde(this, Kohde);
	for (int i = viim; i > viim - MAXTAPAHTUMA-1; i--) {
		if (Tapahtuma[i%MAXTAPAHTUMA].Kilpno) {
			if (valinta == -1 || valinta == Tapahtuma[i%MAXTAPAHTUMA].Piste
				|| (valinta == -2 &&
				Tapahtuma[i%MAXTAPAHTUMA].Osuus ==
					Sarjat[Tapahtuma[i%MAXTAPAHTUMA].Sarja].osuusluku-1))
				lkm++;
		 }
		else
         break;
	  }
	TapGrid->RowCount = lkm+1;
	TapGrid->FixedRows = (lkm ? 1 : 0);
	if (lkm == 0) {
		for (int i = 0; i < TapGrid->ColCount; i++) {
			Cells[i][k].text = "";
			Cells[i][k].style = 0;
			}
		k++;
		}
	else {
		for (int i = viim; i > viim - MAXTAPAHTUMA + 1; i--) {
			kilptietue kilp;
			int d;
			T = &Tapahtuma[i%MAXTAPAHTUMA];
			if (T->Kilpno == 0)
				break;
			if (valinta >= 0 && T->Piste != valinta)
				continue;
			if (valinta == -2 && T->Osuus != Sarjat[T->Sarja].osuusluku-1)
				continue;
			yosuus = Sarjat[T->Sarja].yosuus[T->Osuus];
			d = getpos(T->Kilpno);
			if (d > 0)
				kilp.getrec(d);
			else
				kilp.nollaa();
			Cells[0][k].text = T->Kilpno;
			Cells[0][k].justify = 1;
			Cells[1][k].text = wosuuskoodi(T->Sarja, T->Osuus, 0, 0);
			Cells[1][k].justify = 2;
			Cells[2][k].text = T->Joukkue;
			if (d > 0) {
				Cells[3][k].text = kilp.Nimi(wln, 99, T->Osuus, IkkParam.EtuSuku ? -1 : 1);
				}
			else
				Cells[3][k].text = T->Nimi;
			Cells[4][k].text = T->Maa;
			strncpy(ln, Sarjat[T->Sarja].sarjanimi, 39);
			ln[39] = 0;
			Cells[5][k].text = AnsiString(ln);
			strcpy(ln, "M");
			if (T->Piste)
				ln[0] = '0'+T->Piste;
			Cells[6][k].text = AnsiString(ln);
			Cells[6][k].justify = 2;
			Cells[7][k].text = T->Sija;
			Cells[7][k].justify = 1;
			if (vuorokaudet && T->Tulos > 24*TUNTI) {
				int vrk, tls;

				vrk = T->Tulos / (24*TUNTI);
				tls = T->Tulos - 24*TUNTI*vrk;
				swprintf(wln, L"%02d %s", kilpparam.n_os+1-vrk, aikatowstr_cols_n(wst, tls, 0, 0, kilpparam.laika2));
				Cells[8][k].text = wln;
				if ((tls = (vrk - pkarki[T->Sarja][yosuus][0] / (24*TUNTI))) == 0) {
					aikatowstr_cols_n(wln, T->Tulos - pkarki[T->Sarja][yosuus][T->Piste], 0, 0, kilpparam.laika2);
					}
				else {
					swprintf(wln, L"%d kierr.", tls);
					}
				}
			else {
				if (T->Tulos > 5*SEK) {
					aikatowstr_cols_n(wln, T->Tulos, 0, 0, kilpparam.laika2);
					Cells[8][k].text = wln;
					aikatowstr_cols_n(wln+1, T->Tulos - pkarki[T->Sarja][yosuus][T->Piste], 0, 0, kilpparam.laika2);
					elimwzb1(wln+1);
					wln[0] = L'+';
					}
				else {
					Cells[8][k].text = L"     "+UnicodeString(T->Tulos/SEK);
					wln[0] = 0;
					}
				}
			Cells[9][k].text = wln;
			Cells[9][k].justify = 1;
			Cells[10][k].text = T->Tark;
			Cells[10][k].justify = 2;
			int style = 0;
			if (T->Tark == "H" || T->Tark == "K")
				style = 3;
			else if ((d = getpos(T->Kilpno)) > 0 && vipLaji[d] != 0) {
				switch (vipLaji[d]) {
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
			for (int j = 0; j < TapGrid->ColCount; j++)
				Cells[j][k].style = style;
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
	Paivita(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TTapForm::FormShow(TObject *Sender)
{
	Skaalaa();
	Paivita(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TTapForm::FormResize(TObject *Sender)
{
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
   pBitmap->Canvas->Font->Style = TapGrid->Font->Style;
   pBitmap->Canvas->Font->Size = IkkParam.FontSize;
   if (ARow == 0)
	  pBitmap->Canvas->Brush->Color = clBtnFace;
	else {
		switch (Cells[ACol][ARow].style) {
			case 0:
				pBitmap->Canvas->Brush->Color = clWindow;
				break;
			case 1:
				pBitmap->Canvas->Brush->Color = (TColor) korVari[0];  //clLime;
//            pBitmap->Canvas->Font->Style = TFontStyles() << fsBold;
				break;
			case 2:
				pBitmap->Canvas->Brush->Color = (TColor) korVari[1];  //clYellow;
//            pBitmap->Canvas->Font->Style = TFontStyles() << fsBold;
				break;
			case 3:
				pBitmap->Canvas->Brush->Color = (TColor) korVari[2];  //clFuchsia;
				break;
			case 4:
				pBitmap->Canvas->Brush->Color = (TColor) korVari[3];  //clSkyBlue;
				break;
			case 5:
				pBitmap->Canvas->Brush->Color = (TColor) korVari[4];  //clAqua;
				break;
			case 6:
				pBitmap->Canvas->Brush->Color = (TColor) korVari[5];  //clOlive - light;
				break;
			}
		}
	pBitmap->Canvas->FillRect(Rect1);
   X = 1;
   if (Cells[ACol][ARow].justify == 1) {
	   X =  Rect.Width() - pBitmap->Canvas->TextWidth(Cells[ACol][ARow].text) - 2;
	   if (X < 1)
		  X = 1;
	   }
   if (Cells[ACol][ARow].justify == 2) {
	   X =  (Rect.Width() - pBitmap->Canvas->TextWidth(Cells[ACol][ARow].text) - 2) / 2;
	   if (X < 1)
		  X = 1;
	   }
	pBitmap->Canvas->TextOut(X,1,Cells[ACol][ARow].text);
   TapGrid->Canvas->Draw(Rect.Left, Rect.Top, pBitmap);
   delete pBitmap;
}
//---------------------------------------------------------------------------
void __fastcall TTapForm::CBTarkSanClick(TObject *Sender)
{
	 IkkParam.OhitaTarkSan = !CBTarkSan->Checked;
	 tarktapahtumat = !IkkParam.OhitaTarkSan;
}
//---------------------------------------------------------------------------
void __fastcall TTapForm::Skaalaa(void)
{
	int GrdWidth;

	if (IkkParam.Lihavoitu) {
		TapGrid->Font->Style = TFontStyles() << fsBold;
		lev = IkkParam.FontSize + 2;
		}
	else {
		TapGrid->Font->Style = TFontStyles();
		lev = IkkParam.FontSize;
		}
	TapGrid->ColCount = 11;
	TapGrid->FixedRows = TapGrid->RowCount > 1 ? 1 : 0;
//	TapGrid->DefaultRowHeight = 20 * Screen->PixelsPerInch * IkkParam.FontSize / 960;
	TapGrid->DefaultRowHeight = 2 * (IkkParam.FontSize - 1) * Screen->PixelsPerInch/96;
	for (int cc = 0; cc < TapGrid->ColCount; cc++)
		TapGrid->ColWidths[cc] = IkkParam.ColW[cc] * Screen->PixelsPerInch * lev / 960;

	Cells[0][0].text = L"Kilpno";
	Cells[1][0].text = L"Os.";
	Cells[2][0].text = L"Joukkue";
	Cells[3][0].text = L"Nimi";
	Cells[4][0].text = L"Maa";
	Cells[5][0].text = L"Sarja";
	Cells[6][0].text = L"Pst";
	Cells[7][0].text = L"Sija";
	Cells[8][0].text = L"Tulos";
	Cells[9][0].text = L"Ero";
	if (kilpparam.lsakmax) {
		Cells[10][0].text = L"Sakot";
		}
	else
		Cells[10][0].text = L"Trk";

	GrdWidth = 16 * Screen->PixelsPerInch * IkkParam.FontSize / 960;
	for (int i = 0; i < TapGrid->ColCount; i++) {
		GrdWidth += TapGrid->ColWidths[i] + 2;
		}
	ClientWidth = GrdWidth;
	TapGrid->Width = GrdWidth;
}
//---------------------------------------------------------------------------


void __fastcall TTapForm::Pieni1Click(TObject *Sender)
{
	Pieni1->Checked = true;;
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
void __fastcall TTapForm::Etunimiensin1Click(TObject *Sender)
{
	Etunimiensin1->Checked = !Etunimiensin1->Checked;
	IkkParam.EtuSuku = Etunimiensin1->Checked;
	TapEtunimiensin = IkkParam.EtuSuku;
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (FormJoukktiedot) {
		FormJoukktiedot->Close();
		delete FormJoukktiedot;
		FormJoukktiedot = NULL;
		}
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::TapGridDblClick(TObject *Sender)
{
	int d;

	if (TapGrid->Row == 0)
		return;
	d = getpos(_wtoi(Cells[2][TapGrid->Row].text.c_str()));
	if (!FormJoukktiedot)
		FormJoukktiedot = new TFormJoukkuetiedot(this);
	FormJoukktiedot->sallimuokkausvalinta = false;
	FormJoukktiedot->naytaKilpailija(d);
	FormJoukktiedot->Show();
	if (FormJoukktiedot->WindowState == wsMinimized)
		FormJoukktiedot->WindowState = wsNormal;
	FormJoukktiedot->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::FormCreate(TObject *Sender)
{
//	IkkParam.FontSize = 10;
	applyParams();
	PisteVal->Clear();
	if (kierrokset)
		PisteVal->Items->Add("Yhteensä");
	PisteVal->Items->Add("Kaikki");
	PisteVal->Items->Add("Maali");
	for (int i = 1; i <= kilpparam.maxvaluku; i++) {
		PisteVal->Items->Add(AnsiString(i)+". väliaika");
		}
	PisteVal->ItemIndex = 0;
	Valittu = 0;
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::Suljekaavake1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------


void __fastcall TTapForm::Lihavoitu1Click(TObject *Sender)
{
	Lihavoitu1->Checked = ! Lihavoitu1->Checked;
	IkkParam.Lihavoitu = Lihavoitu1->Checked;
	Skaalaa();
}
//---------------------------------------------------------------------------

void __fastcall TTapForm::allennasarakeleveydet1Click(TObject *Sender)
{
	for	(int c = 0; c < TapGrid->ColCount; c++)
		IkkParam.ColW[c] = (960 * TapGrid->ColWidths[c]) /
			(Screen->PixelsPerInch * lev);
}
//---------------------------------------------------------------------------

