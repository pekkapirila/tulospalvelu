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

#include "UnitKartta.h"
#include "UnitKohdistus.h"
#ifndef MAXOSUUSLUKU
#include "WinHk.h"
#else
#include "UnitMain.h"
#endif
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormKartta *FormKartta;
#ifdef MAXOSUUSLUKU
int onrata(wchar_t *tunnus);
#endif
static TColor clRata = clFuchsia;
static TColor clVirhe = clRed;
static int wdthRata = 3;
static int rastiHalk = 40;
static int fntSize = 20;
//---------------------------------------------------------------------------
__fastcall TFormKartta::TFormKartta(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
bool __fastcall TFormKartta::KarttaLadattu(void)
{
	return(karttaLadattu);
}
//---------------------------------------------------------------------------
int __fastcall TFormKartta::Lataakartta(UnicodeString Flnm)
{
	int err = 0;
	karttaLadattu = false;
	try {
		Image1->Picture->LoadFromFile(Flnm);
		} catch (...) {
		err = 1;
		}
	if (!err) {
		KarttaFlnm = Flnm;
		Image1->Height = Image1->Picture->Height;
		Image1->Width = Image1->Picture->Width;
		Image1->Left = -(Image1->Width - Panel1->ClientWidth) / 2;
		Image1->Top = -(Image1->Height - Panel1->ClientHeight) / 2;
		setScrollPos();
		if (Image1->Height && Image1->Width)
			karttaLadattu = true;
		Image1->Refresh();
		}
	else
		KarttaFlnm = L"";
	return(err);
}
//---------------------------------------------------------------------------
void __fastcall TFormKartta::Lataakartta1Click(TObject *Sender)
{
	OpenPictureDialog1->InitialDir = FormMain->CurrentDir;
	if (OpenPictureDialog1->Execute()) {
		Lataakartta(OpenPictureDialog1->FileName);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormKartta::piirraRasti(int y, int x, wchar_t *kdi, int rastilaji)
{
	if (rastilaji) {
		Image1->Canvas->Pen->Color = clVirhe;
		Image1->Canvas->Font->Color = clVirhe;
		}
	Image1->Canvas->Ellipse(x - rastiHalk/2, y - rastiHalk/2, x + rastiHalk/2, y + rastiHalk/2);
	if (rastilaji == 1) {
		Image1->Canvas->MoveTo(x - rastiHalk/2, y - rastiHalk/2);
		Image1->Canvas->LineTo(x + rastiHalk/2, y + rastiHalk/2);
		Image1->Canvas->MoveTo(x - rastiHalk/2, y + rastiHalk/2);
		Image1->Canvas->LineTo(x + rastiHalk/2, y - rastiHalk/2);
		}
	Image1->Canvas->TextOutW(x + 8*rastiHalk/10, y-fntSize/2, kdi);
	Image1->Canvas->Pen->Color = clRata;
	Image1->Canvas->Font->Color = clRata;
}
//---------------------------------------------------------------------------
void __fastcall TFormKartta::piirraLahto(int y, int x)
{
	Image1->Canvas->MoveTo(x, y + 2*rastiHalk/3);
	Image1->Canvas->LineTo(x + 15*rastiHalk/26, y - rastiHalk/3);
	Image1->Canvas->LineTo(x - 15*rastiHalk/26, y - rastiHalk/3);
	Image1->Canvas->LineTo(x, y + 2*rastiHalk/3);
}
//---------------------------------------------------------------------------
void __fastcall TFormKartta::piirraMaali(int y, int x)
{
	Image1->Canvas->Ellipse(x - rastiHalk/2, y - rastiHalk/2, x + rastiHalk/2, y + rastiHalk/2);
	Image1->Canvas->Ellipse(x - rastiHalk/3, y - rastiHalk/3, x + rastiHalk/3, y + rastiHalk/3);
}
//---------------------------------------------------------------------------
bool rastiKoord(wchar_t *tunnus, double *y, double *x)
{
	*x = 0;
	*y = 0;
	for (int i = 0; i < sizeof(rastikoord)/sizeof(rastikoord[0]); i++) {
		if (rastikoord[i].tunnus[0] == 0)
			break;
		if (!wcscmpU(rastikoord[i].tunnus, tunnus)) {
			*y = rastikoord[i].mapY;
			*x = rastikoord[i].mapX;
			return(true);
			}
		}
	return(false);
}
//---------------------------------------------------------------------------
void __fastcall TFormKartta::piirraViiva(int y1, int x1, int y2, int x2)
{
	double x, y, l;
	if ((y1-y2)*(y1-y2) + (x1-x2)*(x1-x2) < 6*rastiHalk*rastiHalk/5)
		return;
	x = x2-x1;
	y = y2-y1;
	l = sqrt(x*x+y*y);
	x = x * rastiHalk / l;
	y = y * rastiHalk / l;
	Image1->Canvas->MoveTo(x1 + (int)(x/2), y1 + (int)(y/2));
	Image1->Canvas->LineTo(x2 - (int)(x/2), y2 - (int)(y/2));
}
//---------------------------------------------------------------------------
void __fastcall TFormKartta::naytaRastit(void)
{
	wchar_t st[20];

	if (KarttaFlnm.Length() == 0)
		return;
	if (Map.rightX <= Map.leftX) {
		Application->MessageBoxW(L"Kartan oikean laidan koordinaatti ratatiedoissa "
			L"sama tai pienempi kuin vasemman laidan. Rataa ei voida näyttää", L"Este", MB_OK);
		return;
		}
	Image1->Picture->LoadFromFile(KarttaFlnm);
	Image1->Canvas->Pen->Color = clRata;
	Image1->Canvas->Pen->Width = wdthRata;
	Image1->Canvas->Font->Color = clRata;
	Image1->Canvas->Font->Name = L"Arial";
	Image1->Canvas->Font->Size = fntSize;
	Image1->Canvas->Brush->Style = bsClear;
	xLeft = Map.leftX;
	yTop = Map.topY;
	if (Map.rightX > Map.leftX)
		kerroin = Image1->Width / (Map.rightX - Map.leftX);
	for (int i = 0; i < sizeof(rastikoord)/sizeof(rastikoord[0]); i++) {
		if (rastikoord[i].tunnus[0] != 0) {
			if (wcswcind(rastikoord[i].tunnus[0], L"SL") >= 0) {
				piirraLahto(imgY(rastikoord[i].mapY), imgX(rastikoord[i].mapX));
				}
			else
				piirraRasti(imgY(rastikoord[i].mapY), imgX(rastikoord[i].mapX), rastikoord[i].tunnus, 0);
			}
		}
	Naytetty = 1;
}
//---------------------------------------------------------------------------
void __fastcall TFormKartta::naytaLeimat(ratatp *rt, int *tulkinta, char *koodit)
{
	int n, l = -1, m = -1, ix;
	double ex = 0, ey = 0, x1 = -999999, y1 = -999999, x2, y2;
	wchar_t st[20];
	int tlk[MAXNLEIMA];

	if (!rt || KarttaFlnm.Length() == 0)
		return;
	if (Map.rightX <= Map.leftX) {
		Application->MessageBoxW(L"Kartan oikean laidan koordinaatti ratatiedoissa "
			L"sama tai pienempi kuin vasemman laidan. Rataa ei voida näyttää", L"Este", MB_OK);
		return;
		}
	Image1->Picture->LoadFromFile(KarttaFlnm);
	Image1->Canvas->Pen->Color = clRata;
	Image1->Canvas->Pen->Width = wdthRata;
	Image1->Canvas->Font->Color = clRata;
	Image1->Canvas->Font->Name = L"Arial";
	Image1->Canvas->Font->Size = fntSize;
	Image1->Canvas->Brush->Style = bsClear;
	memset(tlk, 0, sizeof(tlk));
	xLeft = Map.leftX;
	yTop = Map.topY;
	if (Map.rightX > Map.leftX)
		kerroin = Image1->Width / (Map.rightX - Map.leftX);
	if (rt->lahto[0] && (ix = haekoordix(rt->lahto)) >= 0) {
		x1 = rastikoord[ix].mapX;
		y1 = rastikoord[ix].mapY;
		piirraLahto(imgY(y1), imgX(x1));
		}
	if (tulkinta && koodit) {
		int k, viimoikea = -1;
		for (k = 0; k < rt->rastiluku; k++) {
			for (int j = 0; j < MAXNLEIMA; j++) {
				if (tulkinta[j] == k+1) {
					tlk[k] = 1;
					viimoikea = j;
					}
				if (tulkinta[j] == -k-1)
					tlk[k] = -1;
				}
			}
		for (k = 0; k < rt->rastiluku; k++) {
			if (tlk[k] == -1 && (k == 0 || tlk[k-1] == 1 || (k > 1 && tlk[k-2] == 1))) {
				tlk[k] = 1;
				}
			}
		if (viimoikea >= 0) {
			k = (viimoikea+2) % MAXNLEIMA;
			for (; k != viimoikea; ) {
				if (tulkinta[k] != 0)
					break;
				k = (k+1) % MAXNLEIMA;
				}
			m = 1;
			for (; k != (viimoikea+1)%MAXNLEIMA; ) {
				if (tulkinta[k] == m || tulkinta[k] == -m)
					m++;
				k = (k+1) % MAXNLEIMA;
				}
			if (m <= rt->rastiluku)
				tlk[m-1] = 0;
			}
		}
	for (int k = 0; k < rt->rastiluku; k++) {
		_itow(rt->rastikoodi[k], st, 10);
		if (rastiKoord(st, &y2, &x2)) {
			if (y1 > -999999) {
				piirraViiva(imgY(y1), imgX(x1), imgY(y2), imgX(x2));
				}
			_itow(k+1, st, 10);
			if (!koodit)
				tlk[k] = 1;
			piirraRasti(imgY(y2), imgX(x2), st, tlk[k] == 1 ? 0 : 1);
			if (tlk[k] != 1 && ex == 0 && ey == 0) {
				ex = x2;
				ey = y2;
				}
			y1 = y2;
			x1 = x2;
			}
		else {
			x1 = -999999;
			y1 = -999999;
			}
		}
	if (rt->maali[0] && (ix = haekoordix(rt->maali)) >= 0) {
		x2 = rastikoord[ix].mapX;
		y2 = rastikoord[ix].mapY;
		if (y1 > -999999) {
			piirraViiva(imgY(y1), imgX(x1), imgY(y2), imgX(x2));
			}
		piirraMaali(imgY(y2), imgX(x2));
		}
	if (tulkinta && koodit) {
		for (int j = 0; j < MAXNLEIMA; j++) {
			if (koodit[j] && tulkinta[j] == 0) {
				haerastit(koodit[j], tlk, &n);
				for (int i = 0; i < n; i++) {
					_itow(tlk[i], st, 10);
					ix = haekoordix(st);
					if (ix >= 0) {
						_itow(koodit[j], st, 10);
						piirraRasti(imgY(rastikoord[ix].mapY), imgX(rastikoord[ix].mapX), st, 2);
						}
					}
				}
			}
		if (ex || ey) {
			l = (ex - xLeft) * kerroin - Panel1->ClientWidth/2;
			if (l < 0)
				l = 0;
			m = (yTop - ey) * kerroin - Panel1->ClientHeight/2;
			if (m < 0)
				m = 0;
			Image1->Left = -l;
			Image1->Top = -m;
			setScrollPos();
			}
		}
	Naytetty = 2;
	Rata = rt;
}
//---------------------------------------------------------------------------
void __fastcall TFormKartta::FormResize(TObject *Sender)
{
	ScrHor->Top = ClientHeight - ScrHor->Height;
	ScrVert->Left = ClientWidth - ScrVert->Width;
	Panel1->Height = ScrHor->Top - Panel1->Top - 2;
	Panel1->Width = ScrVert->Left - 2;
	ScrVert->Height = Panel1->Height;
	ScrHor->Width = Panel1->Width;
	Image1->Left = -(Image1->Width - Panel1->ClientWidth) * ScrollPosX / ScrHor->Max;
	Image1->Top = -(Image1->Height - Panel1->ClientHeight) * ScrollPosY / ScrVert->Max;
}
//---------------------------------------------------------------------------
int __fastcall TFormKartta::imgX(double mapX)
{
	return((int)(kerroin*(mapX-xLeft)));
}
//---------------------------------------------------------------------------
int __fastcall TFormKartta::imgY(double mapY)
{
	return((int)(kerroin*(yTop-mapY)));
}
//---------------------------------------------------------------------------
void __fastcall TFormKartta::ScrHorScroll(TObject *Sender, TScrollCode ScrollCode,
		  int &ScrollPos)
{
	ScrollPosX = ScrollPos;
	Image1->Left = -(Image1->Width - Panel1->ClientWidth) * ScrollPosX / ScrHor->Max;
}
//---------------------------------------------------------------------------
void __fastcall TFormKartta::ScrVertScroll(TObject *Sender, TScrollCode ScrollCode,
		  int &ScrollPos)
{
	ScrollPosY = ScrollPos;
	Image1->Top = -(Image1->Height - Panel1->ClientHeight) * ScrollPosY / ScrVert->Max;
}
//---------------------------------------------------------------------------


void __fastcall TFormKartta::Suljekaavake1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormKartta::NaytaOtsikko(UnicodeString Txt)
{
	Caption = Txt;
}
//---------------------------------------------------------------------------
void __fastcall TFormKartta::setScrollPos(void)
{
	if (Panel1->ClientWidth < Image1->Width) {
		ScrollPosX = - ScrHor->Max * Image1->Left / (Image1->Width - Panel1->ClientWidth);
		ScrHor->Position = ScrollPosX;
		}
	if (Panel1->ClientHeight < Image1->Height) {
		ScrollPosY = - ScrVert->Max * Image1->Top / (Image1->Height - Panel1->ClientHeight);
		ScrVert->Position = ScrollPosY;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKartta::Image1MouseMove(TObject *Sender, TShiftState Shift, int X,
		  int Y)
{
	if (Shift.Contains(ssLeft) && (X != mouseX || Y != mouseY)) {
		Image1->Left += X-mouseX;
		Image1->Top += Y-mouseY;
		mouseX = X;
		mouseY = Y;
		setScrollPos();
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormKartta::Image1MouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	if (Button == mbLeft) {
		mouseX = X;
		mouseY = Y;
		Image1->Cursor = crSizeAll;
		Image1->DragCursor = crSizeAll;
		}
	if (Button == mbRight && FormKohdistus) {
		FormKohdistus->HiiriKohdista(X, Y);
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormKartta::Panel1MouseMove(TObject *Sender, TShiftState Shift, int X,
		  int Y)
{
/*
	if (Shift.Contains(ssLeft) && (X != mouseX || Y != mouseY)) {
		Image1->Left += X-mouseX;
		Image1->Top += Y-mouseY;
		mouseX = X;
		mouseY = Y;
		setScrollPos();
		}
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormKartta::Image1MouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	if (Button == mbLeft) {
		Image1->Cursor = crArrow;
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormKartta::Help1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Kartta");
}
//---------------------------------------------------------------------------

void __fastcall TFormKartta::ScrVertKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	bool muut = false;
	if (Key == VK_NEXT) {
		ScrVert->Position += 10;;
		muut = true;
	}
	if (Key == VK_PRIOR) {
		ScrVert->Position -= 10;;
		muut = true;
	}
	if (Key == VK_UP) {
		ScrVert->Position -= Shift.Contains(ssCtrl) ? 10 : 1;
		muut = true;
		}
	if (Key == VK_DOWN) {
		ScrVert->Position += Shift.Contains(ssCtrl) ? 10 : 1;
		muut = true;
		}
	if (Key == VK_LEFT) {
		ScrHor->Position -= Shift.Contains(ssCtrl) ? 10 : 1;
		muut = true;
		}
	if (Key == VK_RIGHT) {
		ScrHor->Position += Shift.Contains(ssCtrl) ? 10 : 1;
		muut = true;
		}
	Key = 0;
	if (muut) {
		ScrollPosX = ScrHor->Position;
		Image1->Left = -(Image1->Width - Panel1->ClientWidth) * ScrollPosX / ScrHor->Max;
		ScrollPosY = ScrVert->Position;
		Image1->Top = -(Image1->Height - Panel1->ClientHeight) * ScrollPosY / ScrVert->Max;
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormKartta::PienetClick(TObject *Sender)
{
	Pienet->Checked = true;
	Keskikok->Checked = false;
	Suuretsymbolit1->Checked = false;
	rastiHalk = 30;
	fntSize = 15;
	paivitaKartta();
}
//---------------------------------------------------------------------------

void __fastcall TFormKartta::KeskikokClick(TObject *Sender)
{
	Pienet->Checked = false;
	Keskikok->Checked = true;
	Suuretsymbolit1->Checked = false;
	rastiHalk = 40;
	fntSize = 20;
	paivitaKartta();
}
//---------------------------------------------------------------------------

void __fastcall TFormKartta::Suuretsymbolit1Click(TObject *Sender)
{
	Pienet->Checked = false;
	Keskikok->Checked = false;
	Suuretsymbolit1->Checked = true;
	rastiHalk = 60;
	fntSize = 30;
	paivitaKartta();
}
//---------------------------------------------------------------------------

void __fastcall TFormKartta::Tummatvirheet1Click(TObject *Sender)
{
	Tummatvirheet1->Checked = !Tummatvirheet1->Checked;
	clVirhe = Tummatvirheet1->Checked ? clMaroon : clRed;
	paivitaKartta();
}
//---------------------------------------------------------------------------

void __fastcall TFormKartta::Rastit1Click(TObject *Sender)
{
	naytaRastit();
}
//---------------------------------------------------------------------------

void __fastcall TFormKartta::Rata1Click(TObject *Sender)
{
	wchar_t ch, st[20] = L"";
	int r;

	if (ToimintaTila != 2 || emitfl < 1) {
		Application->MessageBoxW(L"Tämä valinta on käytettävissä vain tulospalvelutilassa. Käytä ratamäärittelykaavakkeen valintaa.",
			L"Rajoitus", MB_OK);
		return;
		}
	inputstr_prompt(st, 18, L"Anna radan nimi", &ch, this);
	if (ch != ESC && (r = onrata(st)-1) >= 0) {
		naytaLeimat(rata+r, NULL, NULL);
		NaytaOtsikko(UnicodeString(L"Rata ")+rata[r].tunnus);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKartta::paivitaKartta(void)
{
	switch (Naytetty) {
		case 1:
			naytaRastit();
			break;
		case 2:
			naytaLeimat(Rata, NULL, NULL);
			break;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKartta::Kohdistakahdenrastinavulla1Click(TObject *Sender)
{
	if (!FormKohdistus)
		FormKohdistus = new TFormKohdistus(this);
	FormKohdistus->Show();
}
//---------------------------------------------------------------------------

