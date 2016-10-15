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
#include <stdio.h>

#include "UnitPiikit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

TFormPiikit *FormPiikit;
extern ekditp *kuvio;
extern INT nkuvio;

typedef struct {
   wchar_t x;
   wchar_t y;
   } piikkitp;

static piikkitp piikit[248] = {
   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
   {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, {8, 1}, {9, 1}, {1, 2}, {2, 2},
   {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2}, {8, 2}, {9, 2}, {10, 2}, {1, 3}, {2, 3},
   {3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 3}, {8, 3}, {9, 3}, {10, 3}, {1, 4}, {2, 4},
   {3, 4}, {4, 4}, {5, 4}, {6, 4}, {7, 4}, {0, 0}, {8, 4}, {0, 0}, {9, 4}, {10, 4},
   {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}, {6, 5}, {7, 5}, {8, 5}, {9, 5}, {10, 5},
   {1, 6}, {2, 6}, {3, 6}, {4, 6}, {7, 6}, {0, 0}, {8, 6}, {9, 6}, {0, 0}, {10, 6},
   {1, 7}, {2, 7}, {3, 7}, {4, 7}, {7, 7}, {8, 7}, {9, 7}, {0, 0}, {0, 0}, {10, 7},
   {1, 8}, {2, 8}, {3, 8}, {4, 8}, {7, 8}, {8, 8}, {9, 8}, {10, 8}, {1, 9}, {2, 9},
   {3, 9}, {4, 9}, {5, 9}, {6, 9}, {7, 9}, {8, 9}, {9, 9}, {10, 9}, {1, 10}, {2, 10},
   {3, 10}, {4, 10}, {5, 10}, {6, 10}, {7, 10}, {8, 10}, {9, 10}, {10, 10}, {1, 11}, {2, 11},
   {3, 11}, {4, 11}, {5, 11}, {6, 11}, {7, 11}, {8, 11}, {9, 11}, {10, 11}, {1, 12}, {2, 12},
   {3, 12}, {4, 12}, {5, 12}, {6, 12}, {7, 12}, {8, 12}, {9, 12}, {10, 12}, {1, 13}, {2, 13},
   {3, 13}, {4, 13}, {5, 13}, {6, 13}, {7, 13}, {8, 13}, {9, 13}, {10, 13}, {1, 14}, {2, 14},
   {3, 14}, {4, 14}, {5, 14}, {6, 14}, {7, 14}, {8, 14}, {9, 14}, {10, 14}, {2, 15}, {3, 15},
   {4, 15}, {5, 15}, {6, 15}, {7, 15}, {8, 15}, {9, 15},  {0, 0},   {0, 0},  {0, 0},  {0, 0},
   {1, 7}, {2, 7}, {3, 7}, {4, 7}, {7, 7}, {0, 0}, {8, 7}, {9, 7}, {0, 0}, {10, 7},
   {1, 8}, {2, 8}, {3, 8}, {4, 8}, {7, 8}, {8, 8}, {9, 8}, {10, 8}, {1, 9}, {2, 9},
   {3, 9}, {4, 9}, {5, 9}, {6, 9}, {7, 9}, {8, 9}, {9, 9}, {10, 9}, {1, 10}, {2, 10},
   {3, 10}, {4, 10}, {5, 10}, {6, 10}, {7, 10}, {8, 10}, {9, 10}, {10, 10}, {1, 11}, {2, 11},
   {3, 11}, {4, 11}, {5, 11}, {6, 11}, {7, 11}, {8, 11}, {9, 11}, {10, 11}, {1, 12}, {2, 12},
   {3, 12}, {4, 12}, {5, 12}, {6, 12}, {7, 12}, {8, 12}, {9, 12}, {10, 12}, {1, 13}, {2, 13},
   {3, 13}, {4, 13}, {5, 13}, {6, 13}, {7, 13}, {8, 13}, {9, 13}, {10, 13}};

static int hScale = 20, vScale = 20, marg2 = 0;
static int BMtop = 0, BMleft = 0;

//---------------------------------------------------------------------------
__fastcall TFormPiikit::TFormPiikit(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	CBOsuus->Visible = true;
	Label3->Visible = true;
}
//---------------------------------------------------------------------------
void TFormPiikit::prtpiste(int x, int y, int korostus)
   {
   int pisteSade;
   switch (korostus) {
		case 0:
			pisteSade = 2;
			Image1->Canvas->Pen->Width = 1;
			Image1->Canvas->Pen->Color = clMedGray;
			break;
		case 1:
			pisteSade = hScale / 4;
			Image1->Canvas->Pen->Width = 2;
			Image1->Canvas->Pen->Color = clBlack;
			break;
		case 2:
			pisteSade = hScale / 3;
			Image1->Canvas->Pen->Width = 2;
			Image1->Canvas->Pen->Color = clRed;
			break;
		case 3:
			pisteSade = hScale / 3;
			Image1->Canvas->Pen->Width = 2;
			Image1->Canvas->Pen->Color = clLime;
			break;
		default:
			pisteSade = hScale / 3;
			Image1->Canvas->Pen->Width = 2;
			Image1->Canvas->Pen->Color = clAqua;
			break;
		}
	x = hScale * (x+1) + BMleft;
	y = (vScale * (2*y+5))/2 + BMtop;
	Image1->Canvas->Ellipse(x - pisteSade, y - pisteSade, x + pisteSade, y + pisteSade);
   }

void TFormPiikit::piirrapiikit(void)
   {
   int i, j, k = 0, m, kdi[MAXRINNKOODI], r, nrasti, kdirastit[256][10];
   wchar_t msg[80];
   ratatp *prata;
   TRect Rect = TRect(0,0,10,10);

   Refresh();
   Rect.Right = Rect.Left + 11 * hScale;
   Rect.Bottom = Rect.Top + 22 * vScale;
   Image1->Canvas->Brush->Color = clWhite;
   Image1->Canvas->FillRect(Rect);
   Image1->Canvas->Pen->Style = psSolid;
   Image1->Canvas->Pen->Color = clBlack;
   Image1->Canvas->Pen->Width = 1;
   Image1->Canvas->MoveTo(Rect.Left, Rect.Top + 3*vScale/2);
   Image1->Canvas->LineTo(Rect.Left + 3*hScale/2, Rect.Top);
   Image1->Canvas->LineTo(Rect.Right - 3*hScale/2, Rect.Top);
   Image1->Canvas->LineTo(Rect.Right, Rect.Top + 3*vScale/2);
   Image1->Canvas->LineTo(Rect.Right, Rect.Bottom - 3*vScale/2);
   Image1->Canvas->LineTo(Rect.Right - 3*hScale/2, Rect.Bottom);
   Image1->Canvas->LineTo(Rect.Left + 3*hScale/2, Rect.Bottom);
   Image1->Canvas->LineTo(Rect.Left, Rect.Bottom - 3*vScale/2);
   Image1->Canvas->LineTo(Rect.Left, Rect.Top + 3*vScale/2);
   Image1->Canvas->Pen->Width = 2;
   Image1->Canvas->Pen->Color = clMedGray;
   Image1->Canvas->Brush->Color = clMedGray;
   Image1->Canvas->Ellipse(Rect.Left+5*hScale, Rect.Top+10*vScale, Rect.Left+6*hScale, Rect.Top+11*vScale);
   Image1->Canvas->Brush->Color = clWhite;
   Image1->Canvas->Pen->Color = clBlack;
   for (int r = 0; r < 15; r++) {
		for (int c = 0; c < 10; c++) {
			if (c < 4 || c > 5 || r < 7 || r > 9)
				prtpiste(c, r, 0);
			if ((c == 0 || c == 9) && r == 0)
				SG1->Cells[c][r] = L"noll";
			else
				SG1->Cells[c][r] = L"";
			}
		}

	prtpiste(0, 0, 1);
	prtpiste(9, 0, 1);

	prata = rata + CBRata->ItemIndex;
	nrasti = prata->rastiluku;
	memset(kdirastit, 0, sizeof(kdirastit));
	for (i = 0; i < nrasti; i++) {
		r = prata->rastikoodi[i];
		if (r == 0 || r > MAXTUNNUS)
			continue;
		k++;
		memset(kdi, 0, sizeof(kdi));
		for (j = 0; j < nkuvio; j++) {
			if (kuvio[j].rasti == r) {
				for (m = 0; m < MAXRINNKOODI; m++)
					if (!kdi[m])
						break;
				if (m < MAXRINNKOODI) {
					kdi[m] = kuvio[j].koodi;
					}
				for (m = 0; m < sizeof(kdirastit[0])/sizeof(kdirastit[0][0]); m++)
					if (!kdirastit[kuvio[j].koodi][m])
						break;
				if (m < sizeof(kdirastit[0])/sizeof(kdirastit[0][0])) {
					kdirastit[kuvio[j].koodi][m] = i+1;
					}
				}
			}
		for (j = 0; j < MAXRINNKOODI; j++) {
			if (kdi[j] > 0 && kdi[j] <= (int) (sizeof(piikit)/sizeof(piikkitp)) &&
				piikit[kdi[j]-1].x) {
				int xx, yy, korostus = 1;
				if (RGPuoli->ItemIndex == 0)
					xx = 10 - piikit[kdi[j]-1].x;
				else
					xx = piikit[kdi[j]-1].x - 1;
				yy = 15 - piikit[kdi[j]-1].y;
				for (m = 0; m < sizeof(kdirastit[0])/sizeof(kdirastit[0][0]); m++) {
					if (CBRasti->ItemIndex && kdirastit[kdi[j]][m] == CBRasti->ItemIndex) {
						korostus = 3;
						break;
						}
					else if (tark[kdirastit[kdi[j]][m]-1])
						korostus = 2;
					}
				prtpiste(xx, yy, korostus);
				if (SG1->Cells[xx][yy].Length() == 0)
					SG1->Cells[xx][yy] = UnicodeString(i+1);
				else
					SG1->Cells[xx][yy] = SG1->Cells[xx][yy]+UnicodeString(',')+UnicodeString(i+1);
				}
			}
		}
	r = 0;
	Memo1->Clear();
	for (i = 0; i < 255; i++) {
		if (kdirastit[i][1]) {
			swprintf(msg, L"Koodi %d rasteilla %d", i, kdirastit[i][0]);
			for (j = 1; j < 8 && kdirastit[i][j]; j++) {
				swprintf(msg+wcslen(msg), L"%s %d", (j == 7 || !kdirastit[i][j+1]) ? L" ja" : L",", kdirastit[i][j]);
				}
			Memo1->Lines->Add(msg);
			}
		}
   Image1->Show();
   Panel1->Repaint();
   }

void __fastcall TFormPiikit::Piirra(void)
{
	vScale = (ClientHeight - 24 * Screen->PixelsPerInch / 96) / 22;
	hScale = vScale;
	Panel1->Width = ClientWidth - Panel1->Left - 4 * Screen->PixelsPerInch / 96;
	Panel1->Height = 22 * vScale+2;
	Image1->Width = 11 * hScale+2;
	Image1->Height = 22 * vScale+2;
	Memo1->Height = ClientHeight - Memo1->Top - 6 * Screen->PixelsPerInch / 96;
	piirrapiikit();
}
//---------------------------------------------------------------------------

void __fastcall TFormPiikit::HaeRata(void)
{
int rt = CBRata->ItemIndex;

	kilptietue kilp;
	kilp.nollaa();
	memset(tark, 0, sizeof(tark));
#ifdef MAXOSUUSLUKU
	kilp.getrec(getpos(KilpNo));
	if (!kilp.kilpstatus && kilp.kilpno == KilpNo) {
		int r = getratano(kilp.Hajonta(Osuus));
		wchar_t st[60];
		EdtNimi->Text = kilp.Nimi(st, 50, Osuus);
		if (r >= 0) {
			emittp em;
			CBRata->ItemIndex = r;
			if (getem(&em, KilpNo, Osuus) >= 0) {
				int tulkinta[MAXNLEIMA];
				if (tarkista(&em, &kilp, tulkinta, -1, 0)) {
					int k = 0;
					for (int i = 0; i < rata[r].rastiluku; i++) {
						int j;
						for (j = 0; j < MAXNLEIMA; j++) {
							if (abs(tulkinta[j]) == i+1)
								break;
							}
						if (j == MAXNLEIMA)
							tark[i] = 1;
						}
					}
				}
			}
		}
	else
		EdtNimi->Text = L"";
#else
	kilp.GETREC(getpos(KilpNo));
	if (!kilp.kilpstatus && kilp.id() == KilpNo) {
		wchar_t st[60];
		int r = haeratano(&kilp);
		EdtNimi->Text = kilp.nimi(st, 50, false);
		if (r >= 0) {
			emittp em;
			CBRata->ItemIndex = r;
			if (em.getbykno(KilpNo, 0) >= 0) {
				int tulkinta[MAXNLEIMA];
				if (tarkista(&em, tulkinta, 0, &kilp)) {
					int k = 0;
					for (int i = 0; i < rata[r].rastiluku; i++) {
						int j;
						for (j = 0; j < MAXNLEIMA; j++) {
							if (abs(tulkinta[j]) == i+1)
								break;
							}
						if (j == MAXNLEIMA)
							tark[i] = 1;
						}
					}
				}
			}
		}
	else
		EdtNimi->Text = L"";
#endif
	if (rt != CBRata->ItemIndex)
		PohjRata();
}
//---------------------------------------------------------------------------

void __fastcall TFormPiikit::FormResize(TObject *Sender)
{
	Piirra();
	SG1->Top = 2 * vScale;
	SG1->Left = Image1->Width + 2;
	SG1->Width = Panel1->Width - SG1->Left - 2;
	SG1->DefaultColWidth = SG1->Width / 10 - 1;
	SG1->DefaultRowHeight = vScale - 1;
	SG1->Height = 15*vScale + 2;
	BitBtn1->Left = SG1->Left;
}
//---------------------------------------------------------------------------

void __fastcall TFormPiikit::FormCreate(TObject *Sender)
{
#ifdef MAXOSUUSLUKU
	CBOsuus->Clear();
	for (int i = 0; i < kilpparam.osuusluku; i++) {
		CBOsuus->Items->Add(UnicodeString(i+1)+L". osuus");
		}
	CBOsuus->ItemIndex = 0;
#endif
	CBRata->Clear();
	for (int i = 0; i < nrata; i++) {
		CBRata->Items->Add(rata[i].tunnus);
		}
	CBRata->ItemIndex = 0;
	CBRataChange(Sender);
	Height = 510;
	Width = 750;
}
//---------------------------------------------------------------------------

void __fastcall TFormPiikit::CBRataChange(TObject *Sender)
{
	memset(tark, 0, sizeof(tark));
	PohjRata();
}
//---------------------------------------------------------------------------

void __fastcall TFormPiikit::PohjRata(void)
{
	CBRasti->Clear();
	CBRasti->Items->Add(L"Ei valittu");
	for (int i = 1; i <= rata[CBRata->ItemIndex].rastiluku; i++)
		CBRasti->Items->Add(UnicodeString(i)+L". rasti");
	CBRasti->ItemIndex = 0;
	Piirra();
}
//---------------------------------------------------------------------------

void __fastcall TFormPiikit::RGPuoliClick(TObject *Sender)
{
	Piirra();
}
//---------------------------------------------------------------------------

void __fastcall TFormPiikit::CBRastiClick(TObject *Sender)
{
	Piirra();
}
//---------------------------------------------------------------------------

void __fastcall TFormPiikit::EdtKilpNoExit(TObject *Sender)
{
	KilpNo = _wtoi(EdtKilpNo->Text.c_str());
	HaeRata();
	Piirra();
}
//---------------------------------------------------------------------------

void __fastcall TFormPiikit::CBOsuusChange(TObject *Sender)
{
	Osuus = CBOsuus->ItemIndex;
	HaeRata();
	Piirra();
}
//---------------------------------------------------------------------------

void __fastcall TFormPiikit::FormActivate(TObject *Sender)
{
	EdtKilpNo->Text = KilpNo;
	CBOsuus->ItemIndex = Osuus;
	HaeRata();
	Piirra();
}
//---------------------------------------------------------------------------

void __fastcall TFormPiikit::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Piikkikuvio");
}
//---------------------------------------------------------------------------

void __fastcall TFormPiikit::EdtKilpNoKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		EdtKilpNoExit(Sender);
	}
}
//---------------------------------------------------------------------------

