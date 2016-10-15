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

#include "UnitKohdistus.h"
#include "UnitKartta.h"
#include "UnitRadat.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormKohdistus *FormKohdistus;
bool rastiKoord(wchar_t *tunnus, double *y, double *x);
//---------------------------------------------------------------------------
__fastcall TFormKohdistus::TFormKohdistus(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::PaivitaKohdistus(void)
{
	EdtMapLeft->Text = Map.leftX;
	EdtMapRight->Text = Map.rightX;
	EdtMapTop->Text = Map.topY;
	EdtMapBottom->Text = Map.btmY;
	EdtMapWidth->Text = FormKartta->Image1->Width;
	EdtMapHeight->Text = FormKartta->Image1->Height;
	Y2 = (Map.topY - MapY2) * FormKartta->Image1->Width / (Map.rightX - Map.leftX);
	EdtY2->Text = Y2;
	if (Map.leftX < Map.rightX) {
		FormKartta->paivitaKartta();
		}
	Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::PaivitaKohdistus2(void)
{
	if (MapX1 || MapY1) {
		X1 = (MapX1 - Map.leftX) * FormKartta->Image1->Width / (Map.rightX - Map.leftX);
		Y1 = (Map.topY - MapY1) * FormKartta->Image1->Width / (Map.rightX - Map.leftX);
		EdtX1->Text = X1;
		EdtY1->Text = Y1;
		}
	if (MapX2) {
		X2 = (MapX2 - Map.leftX) * FormKartta->Image1->Width / (Map.rightX - Map.leftX);
		EdtX2->Text = X2;
		EdtY2->Text = Y2;
		}
	PaivitaKohdistus();
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::LaskeKohdistus(void)
{
	if (X2 - X1 < 50)
		return;
	Map.leftX = MapX1 - X1 * (MapX2-MapX1) / (X2-X1);
	Map.rightX = MapX2 + (FormKartta->Image1->Width - X2) * (MapX2-MapX1) / (X2-X1);
	Map.topY = MapY1 + Y1 * (MapX2-MapX1) / (X2-X1);
	Map.btmY = MapY1 - (FormKartta->Image1->Height - Y1) * (MapX2-MapX1) / (X2-X1);
	Map.leftX = floor(100*Map.leftX+0.5)*0.01;
	Map.rightX = floor(100*Map.rightX+0.5)*0.01;
	Map.topY = floor(100*Map.topY+0.5)*0.01;
	Map.btmY = floor(100*Map.btmY+0.5)*0.01;
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::HiiriKohdista(int x, int y)
{
	if (CBHae1->Checked) {
		EdtX1->Text = x;
		EdtY1->Text = y;
		if (X1 != 0 || Y1 != 0) {
			if (X2 != 0) {
				X1 = x;
				Y1 = y;
				LaskeKohdistus();
				PaivitaKohdistus();
				}
			}
		}
	else if (CBHae2->Checked) {
		EdtX2->Text = x;
		X2 = x;
		if (X1 != 0 || Y1 != 0) {
			LaskeKohdistus();
			PaivitaKohdistus();
			}
		}
}

//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::Button1Click(TObject *Sender)
{
	double minX = 999999, maxX = -999999, minY = 999999, maxY = -999999;
	double kx, ky;

	for (int i = 0; i < sizeof(rastikoord)/sizeof(rastikoord[0]); i++) {
		if (rastikoord[i].tunnus[0] != 0) {
			if (minX > rastikoord[i].mapX)
				minX = rastikoord[i].mapX;
			if (maxX < rastikoord[i].mapX)
				maxX = rastikoord[i].mapX;
			if (minY > rastikoord[i].mapY)
				minY = rastikoord[i].mapY;
			if (maxY < rastikoord[i].mapY)
				maxY = rastikoord[i].mapY;
			}
		}
	if (minX >= maxX || minY >= maxY)
		return;
	kx = maxX - minX;
	ky = maxY - minY;
	if (ky / FormKartta->Image1->Height > kx / FormKartta->Image1->Width)
		kx = ky * FormKartta->Image1->Width / FormKartta->Image1->Height;
	else
		ky = kx * FormKartta->Image1->Height / FormKartta->Image1->Width;
	Map.leftX = minX - 0.2*kx;
	Map.rightX = minX + 1.2*kx;
	Map.topY = maxY + 0.2*ky;
	Map.btmY = maxY - 1.2*ky;
	Map.leftX = floor(100*Map.leftX+0.5)*0.01;
	Map.rightX = floor(100*Map.rightX+0.5)*0.01;
	Map.topY = floor(100*Map.topY+0.5)*0.01;
	Map.btmY = floor(100*Map.btmY+0.5)*0.01;
	PaivitaKohdistus();
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::EdtKdi1Exit(TObject *Sender)
{
	double x = 0, y = 0;
	if (rastiKoord(EdtKdi1->Text.c_str(), &y, &x)) {
		EdtMapX1->Text = x;
		EdtMapY1->Text = y;
		MapX1 = x;
		MapY1 = y;
		if (Map.rightX > Map.leftX) {
			X1 = FormKartta->imgX(x);
			Y1 = FormKartta->imgY(y);
			EdtX1->Text = X1;
			EdtY1->Text = Y1;
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::EdtKdi2Exit(TObject *Sender)
{
	double x = 0, y = 0;
	if (rastiKoord(EdtKdi2->Text.c_str(), &y, &x)) {
		EdtMapX2->Text = x;
		EdtMapY2->Text = y;
		MapX2 = x;
		MapY2 = y;
		if (Map.rightX > Map.leftX) {
			X2 = FormKartta->imgX(x);
			Y2 = FormKartta->imgY(y);
			EdtX2->Text = X2;
			EdtY2->Text = Y2;
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::EdtXYExit(TObject *Sender)
{
	if (Map.leftX < Map.rightX) {
		FormKartta->paivitaKartta();
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::CBHae1Click(TObject *Sender)
{
	if (CBHae1->Checked)
		CBHae2->Checked = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::CBHae2Click(TObject *Sender)
{
	if (CBHae2->Checked)
		CBHae1->Checked = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::Button4Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::FormShow(TObject *Sender)
{
	if (!FormKartta->Image1 || FormKartta->Image1->Width < 10 || !rastikoord[0].tunnus[0]) {
		Application->MessageBoxW(L"Karttaa tai rastikoordinaatteja ei ladattuna", L"Este", MB_OK);
		Close();
		return;
		}
	xLeft = Map.leftX;
	xRight = Map.rightX;
	yTop = Map.topY;
	yBtm = Map.btmY;
	UpDownX1->Min = -1000;
	UpDownX2->Min = -1000;
	UpDownY1->Min = -1000;
	UpDownX1->Max = 1000;
	UpDownX2->Max = 1000;
	UpDownY1->Max = 1000;
//	UpDownX1->Position = 0;
//	UpDownX2->Position = 0;
//	UpDownY1->Position = 0;
	UpDownMapL->Min = -1000;
	UpDownMapL->Max = 1000;
	UpDownMapR->Min = -1000;
	UpDownMapR->Max = 1000;
	UpDownMapT->Min = -1000;
	UpDownMapT->Max = 1000;
//	UpDownMapL->Position = 0;
//	UpDownMapR->Position = 0;
//	UpDownMapT->Position = 0;
	PaivitaKohdistus();
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::Button3Click(TObject *Sender)
{
	Map.leftX = xLeft;
	Map.rightX = xRight;
	Map.topY = yTop;
	Map.btmY = yBtm;
	PaivitaKohdistus2();
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::UpDownX1Click(TObject *Sender, TUDBtnType Button)

{
	int D;

	D = (Button == btNext) ? 1 : -1;
	X1 += D;
	EdtX1->Text = X1;
	if (X1 != 0 && Y1 != 0 && X2 != 0) {
		LaskeKohdistus();
		PaivitaKohdistus();
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::UpDownY1Click(TObject *Sender, TUDBtnType Button)

{
	int D;

	D = (Button == btNext) ? 1 : -1;
	Y1 += D;
	EdtY1->Text = Y1;
	if (X1 != 0 && Y1 != 0 && X2 != 0) {
		LaskeKohdistus();
		PaivitaKohdistus();
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::UpDownX2Click(TObject *Sender, TUDBtnType Button)

{
	int D;

	D = (Button == btNext) ? +1 : -1;
	X2 += D;
	EdtX2->Text = X2;
	if (X1 != 0 && Y1 != 0 && X2 != 0) {
		LaskeKohdistus();
		PaivitaKohdistus();
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::Button2Click(TObject *Sender)
{
	if (FormRadat) {
		FormRadat->nayta_koordinaatit();
		Application->MessageBoxW(L"Muutosten tallentaminen tiedostoon on tehtävä kaavakkella Ratatiedot",
			L"Ohje", MB_OK);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::UpDownMapLClick(TObject *Sender, TUDBtnType Button)
{
	int D;

	D = (Button == btNext) ? +1 : -1;
	Map.leftX += 0.01 * D;
	Map.leftX = floor(100*Map.leftX+0.5)*0.01;
	PaivitaKohdistus2();
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::UpDownMapTClick(TObject *Sender, TUDBtnType Button)
{
	int D;

	D = (Button == btNext) ? +1 : -1;
	Map.topY += 0.01 * D;
	Map.topY = floor(100*Map.topY+0.5)*0.01;
	PaivitaKohdistus2();
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::UpDownMapRClick(TObject *Sender, TUDBtnType Button)
{
	int D;

	D = (Button == btNext) ? +1 : -1;
	Map.rightX += 0.01 * D;
	Map.rightX = floor(100*Map.rightX+0.5)*0.01;
	PaivitaKohdistus2();
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Kartta");
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::EdtKdi1KeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		Key = 0;
		FocusControl(EdtKdi2);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::EdtKdi2KeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		EdtKdi2Exit(Sender);
		Key = 0;
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::EdtX1Exit(TObject *Sender)
{
	X1 = _wtoi(EdtX1->Text.c_str());

	if (X1 && X2 && Y1) {
		LaskeKohdistus();
		PaivitaKohdistus();
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::EdtY1Exit(TObject *Sender)
{
	Y1 = _wtoi(EdtY1->Text.c_str());

	if (X1 && X2 && Y1) {
		LaskeKohdistus();
		PaivitaKohdistus();
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormKohdistus::EdtX2Exit(TObject *Sender)
{
	X2 = _wtoi(EdtX2->Text.c_str());

	if (X1 && X2 && Y1) {
		LaskeKohdistus();
		PaivitaKohdistus();
		}
}
//---------------------------------------------------------------------------
