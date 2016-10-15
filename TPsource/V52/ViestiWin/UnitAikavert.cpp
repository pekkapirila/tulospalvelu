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

#include "UnitAikavert.h"
#include "UnitAikakorjaus.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAikavert *FormAikavertailut[NAJANOTOT];

ColTp VertGridCols0[MAXAVCOL] = {
	{L"Rivi", 35, 1, 1, 0, true},
	{L"Aika", 70, 0, 1, 0, true},
	{L"KilpNo", 35, 1, 1, 0, true},
	{L"Osuus", 35, 2, 1, 0, true},
	{L"Piste", 35, 2, 1, 0, true},
	{L"Joukkue", 90, 0, 1, 0, true},
	{L"Sarja", 60, 0, 1, 0, false},
	{L"Tulos", 70, 0, 1, 0, true},
	{L"Ero", 50, 1, 1, 0, false},
	{L"Tark", 35, 2, 1, 0, false},
	{L"Jonoero", 55, 1, 1, 0, false}
};

//---------------------------------------------------------------------------
__fastcall TFormAikavert::TFormAikavert(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
	memcpy(&VertGridCols[0], VertGridCols0, sizeof(VertGridCols[0]));
	memcpy(&VertGridCols[1], VertGridCols0, sizeof(VertGridCols[0]));
	applyParams();
}
//---------------------------------------------------------------------------
aikavertIkkParamClass::aikavertIkkParamClass(void)
{
	memset(this, 0, sizeof(aikavertIkkParamClass));
	Jono1 = 0;
	Jono2 = 0;
	FontSize = 8;
	AikaVert = 1;
	NoVert = 1;
	AikaRajat[0] = 2000;
	AikaRajat[1] = 5000;
	for (int c = 0; c < MAXAVCOL; c++) {
		ColW[c] = VertGridCols0[c].Width;
		ColW[MAXAVCOL+c] = ColW[c];
		Visible[c] = VertGridCols0[c].Visible;
		Visible[MAXAVCOL+c] = Visible[c];
		}
}
//---------------------------------------------------------------------------
int TFormAikavert::applyParams(void)
{
	CBJono1->ItemIndex = IkkParam.Jono1;
	CBJono2->ItemIndex = IkkParam.Jono2;
	Jono[0] = IkkParam.Jono1;
	Jono[1] = IkkParam.Jono2;
	DGajat1->Font->Size = IkkParam.FontSize;
	DGajat2->Font->Size = IkkParam.FontSize;
	DGajat1->DefaultRowHeight = (20 * IkkParam.FontSize) / 8;
	DGajat2->DefaultRowHeight = (20 * IkkParam.FontSize) / 8;
	return(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormAikavert::naytaRivi(int grid, int r, aikatp &pt, aikatp &pt1)
{
	int d, tl, td, ero, ero1, c = -1, piste;
	wchar_t as[60], pisteet[] = L"ALM";
	kilptietue kilp;

	for (int ic = 0; ic < MAXAVCOL; ic++) {
		if (!VertGridCols[grid][ic].Visible)
			continue;
		 c++;
		 if (!taustaon)
			break;
		 Cells[grid][c][r].style = pt.status ? 7 : 0;
		 if (pt.t == AIKAJAK*TMAALI0) {
			Cells[grid][c][r].text = L"";
			continue;
			}
		 EnterCriticalSection(&tall_CriticalSection);
		 if (pt.kno > 0 && (d = getpos(pt.kno)) > 0)
			kilp.getrec(d);
		 else
			kilp.nollaa();
		 LeaveCriticalSection(&tall_CriticalSection);
		 piste = va_piste(pt.piste);
		 Cells[grid][c][r].justify = VertGridCols[grid][ic].Justify;
		 switch (ic) {
			case 0:
				Cells[grid][c][r].text = UnicodeString(aRow[grid][r-1]);
				break;
			case 1:
				Cells[grid][c][r].text = AIKATOWSTRS(as, purajak(pt.t), t0);
				if (grid == 1 && IkkParam.AikaVert && pt.t != AIKAJAK*TMAALI0 &&
					pt.t != AIKAJAK*TMAALI0 &&
					abs((td = NORMKELLO(purajak(pt.t) - purajak(pt1.t) - AikaEro))) > IkkParam.AikaRajat[0]) {
					if (td < -IkkParam.AikaRajat[1])
						Cells[grid][c][r].style = 3;
					else if (td < -IkkParam.AikaRajat[0])
						Cells[grid][c][r].style = 4;
					else if (td > IkkParam.AikaRajat[1])
						Cells[grid][c][r].style = 1;
					else
						Cells[grid][c][r].style = 2;
					}
				break;
			case 2:
				Cells[grid][c][r].text = UnicodeString(pt.kno);
				if (grid == 1 && IkkParam.NoVert && pt.kno && pt.kno != pt1.kno)
					Cells[grid][c][r].style = 5;
				break;
			case 3:
				Cells[grid][c][r].text = pt.osuus+1;
				break;
			case 4:
				Cells[grid][c][r].text = str_piste(pt.piste);
				break;
			case 5:
				if (kilp.kilpno)
					Cells[grid][c][r].text = kilp.Joukkue(as, 30, false);
				else {
					swprintf(as, L"Badge: %d, Portti: %d Lähde: %d", pt.badge, (int) pt.kanava, pt.lahde);
					Cells[grid][c][r].text = as;
					}
				break;
			case 6:
				if (kilp.kilpno)
					Cells[grid][c][r].text = Sarjat[kilp.sarja].sarjanimi;
				else
					Cells[grid][c][r].text = L"";
				break;
			case 7:
				if (kilp.kilpno) {
					if (pt.piste > -1 && (tl = kilp.Maali(pt.osuus, pt.piste)) != TMAALI0) {
						Cells[grid][c][r].text = aikatowstr_cols_n(as, kilp.Tulos(pt.osuus, pt.piste), 0, 0, kilpparam.laika);
						}
					else
						Cells[grid][c][r].text = L"";
					}
				else
					Cells[grid][c][r].text = L"";
				break;
			case 8:
				if (kilp.kilpno) {
					if (pt.piste > -1 && (tl = kilp.Maali(pt.osuus, pt.piste)) != TMAALI0) {
						ero = purajak(pt.t) - tl;
						ero1 = ero/kilpparam.pyor[2];
						if (ero1)
							Cells[grid][c][r].text = eroaikatowstr(as, ero, kilpparam.laika);
						else
							Cells[grid][c][r].text = L"";
						}
					}
				else
					Cells[grid][c][r].text = L"";
				break;
			case 9:
				if (kilp.kilpno)
					Cells[grid][c][r].text = (kilp.wTark(pt.osuus) != L'-') ? UnicodeString(kilp.wTark(pt.osuus)) : UnicodeString(L"");
				else
					Cells[grid][c][r].text = L"";
				break;
			case 10:
				if (pt.t != AIKAJAK*TMAALI0 && pt1.t != AIKAJAK*TMAALI0) {
					td = (pt.t - pt1.t)/AIKAJAK;
					swprintf(as, L"%d,%03d", td/SEK, abs(td)%SEK);
					Cells[grid][c][r].text = as;
					}
				else
					Cells[grid][c][r].text = L"";
				break;
			}
		 }
}
//---------------------------------------------------------------------------
void __fastcall TFormAikavert::naytaAjat(void)
{
	int VR[2] = {0,0};

	if (!taustaon)
		return;
	EnterCriticalSection(&tall_CriticalSection);
	ScrollBar1->Min = 1;
	if (aikajono[Jono[0]]->aktrows > 2)
		ScrollBar1->Max = aikajono[Jono[0]]->aktrows;
	else
		ScrollBar1->Max = 2;
	if (Bias >= 0 && Alku1 < 1) {
		Alku1 = 1;
		EdtAlku->Text = Alku1;
		}
	if (Bias < 0 && Alku1 < -Bias+1) {
		Alku1 = -Bias+1;
		EdtAlku->Text = Alku1;
		}
	for (int r = 0; r < MAXAVROW; r++) {
		aRow[0][r] = Alku1 + r;
		aRow[1][r] = Alku1 + Bias + r;
		}
	for (int grid = 0; grid < 2; grid++) {
		for (int r = 0; r < NRivi1-1; r++) {
			aikatp pt, pt1;
			memset(&pt, 0, sizeof(pt));
			memset(&pt1, 0, sizeof(pt));
			pt.t = AIKAJAK*TMAALI0;
			pt1.t = AIKAJAK*TMAALI0;
			if (aRow[grid][r] > 0 && aRow[grid][r] <= aikajono[Jono[grid]]->aktrows)
				aikajono[Jono[grid]]->getAktTime(&pt, aRow[grid][r]-1);
			if (grid == 1 && aRow[0][r] > 0 && aRow[0][r] <= aikajono[Jono[0]]->aktrows)
				aikajono[Jono[0]]->getAktTime(&pt1, aRow[0][r]-1);
			naytaRivi(grid, r+1, pt, pt1);
			VR[grid] = aRow[grid][r];
			}
		DGajat[grid]->Refresh();
		}
	LblLisaA->Caption = UnicodeString(aikajono[Jono[0]]->aktrows - VR[0] > 0 ? aikajono[Jono[0]]->aktrows - VR[0] : 0);
	LblLisaB->Caption = UnicodeString(aikajono[Jono[1]]->aktrows - VR[1] > 0 ? aikajono[Jono[1]]->aktrows - VR[1] : 0);
    LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------
void __fastcall TFormAikavert::BtnPaivitaClick(TObject *Sender)
{
	naytaAjat();
}
//---------------------------------------------------------------------------
void __fastcall TFormAikavert::FormResize(TObject *Sender)
{
	Panel1->Height = ClientHeight - Panel1->Top;
	Panel2->Height = Panel1->Height;
	DGajat1->Height = Panel1->ClientHeight - DGajat1->Top - 19 * Screen->PixelsPerInch / 96;
	DGajat2->Height = DGajat1->Height;
	OnLine = true;
	NRivi1 = DGajat1->Height / (DGajat1->DefaultRowHeight + 1);
	if (NRivi1 >= MAXAVROW)
		NRivi1 = MAXAVROW;
	DGajat1->RowCount = NRivi1;
	DGajat2->RowCount = DGajat1->RowCount;
	ScrollBar1->Height = DGajat1->Height;
	CBViimeiset->Top = Panel1->ClientHeight - 18 * Screen->PixelsPerInch / 96;
	CBViimeiset2->Top = CBViimeiset->Top;
	LblLisaA->Top = CBViimeiset->Top;
	Label4->Top = CBViimeiset->Top;
	LblLisaB->Top = CBViimeiset->Top;
	Label6->Top = CBViimeiset->Top;
	SarakeLeveydet();
}
//---------------------------------------------------------------------------
void __fastcall TFormAikavert::DGajat1SelectCell(TObject *Sender, int ACol, int ARow,
		  bool &CanSelect)
{
	if (ARow < 1 || ARow > DGajat1->RowCount)
		return;
}
//---------------------------------------------------------------------------

void TFormAikavert::Paivita_Aikanaytto(void)
{
	if (OnLine) {
		if (CBViimeiset->Checked || CBViimeiset2->Checked) {
			int a1 = aikajono[Jono[0]]->aktrows - NRivi1 + 2;
			if (CBViimeiset2->Checked) {
				int a2 = aikajono[Jono[1]]->aktrows - NRivi1 + 2 - Bias;
				if (!CBViimeiset->Checked || a2 > a1)
					a1 = a2;
				}
			EdtAlku->Text = a1;
			}
		naytaAjat();
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormAikavert::LuoUusi(int grid)
{
	aikatp pt, vpt, upt;
	int r, t;

	r = DGajat[grid]->Row;
	if (r < 1)
		return;
	r = aRow[grid][r-1] - 1;
	if (r < 0)
		return;
	aikajono[Jono[grid]]->getAktTime(&vpt, r);
	if (vpt.t/AIKAJAK != TMAALI0) {
		if (r == 0)
			t = NORMKELLO_A(vpt.t - 10);
		else {
			t = NORMKELLO_A(vpt.t + 1);
			while (r+1 < aikajono[Jono[grid]]->aktrows) {
				aikajono[Jono[grid]]->getAktTime(&pt, r+1);
				if (NORMKELLO_A(pt.t-t) > 0)
					break;
				t++;
				r++;
				}
			}
		lisaa_aika(Jono[grid], t, 0, vpt.t, vpt.date);
		}
	BtnPaivitaClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::CBJono1Change(TObject *Sender)
{
	IkkParam.Jono1 = CBJono1->ItemIndex;
	Jono[0] = IkkParam.Jono1;
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormAikavert::SarakeLeveydet(void)
{
	for (int grid = 0; grid < 2; grid++) {
		int ncol = 0, icol = 0;
		int wdt = 0;
		for (int c = 0; c < MAXAVCOL; c++) {
			if (VertGridCols[grid][c].Visible) {
				if (DGajat[grid]->ColCount < ncol + 1)
					DGajat[grid]->ColCount = ncol + 1;
				DGajat[grid]->ColWidths[ncol] = IkkParam.FontSize * IkkParam.ColW[c+grid*MAXAVCOL] * Screen->PixelsPerInch / 768;
				Cells[grid][ncol][0].text = VertGridCols[grid][c].Caption;
				wdt += DGajat[grid]->ColWidths[ncol]+2;
				ncol++;
				}
			}
		Panel[grid]->Width = wdt + 2;
		DGajat[grid]->Width = wdt;
		DGajat[grid]->ColCount = ncol;
		}
	ScrollBar1->Left = Panel1->Left + Panel1->Width + 1;
	Panel2->Left = ScrollBar1->Left + ScrollBar1->Width + 2;
	ClientWidth = Panel2->Left + Panel2->Width + 2;
	BtnPaivitaClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::FormCreate(TObject *Sender)
{
	DGajat[0] = DGajat1;
	DGajat[1] = DGajat2;
	Panel[0] = Panel1;
	Panel[1] = Panel2;
	SarakeLeveydet();
	aktRivi1 = 1;
	CBJono1->Clear();
	CBJono2->Clear();
	if (maxjono) {
		for (int r = 0; r <= maxjono; r++) {
			CBJono1->Items->Add(L"Jono "+UnicodeString(r+1));
			CBJono2->Items->Add(L"Jono "+UnicodeString(r+1));
			}
		}
	else {
		CBJono1->Items->Add(L"Jono 1");
		CBJono2->Items->Add(L"Jono 1");
		}
	CBJono1->ItemIndex = IkkParam.Jono1;
	CBJono2->ItemIndex = IkkParam.Jono2;
	for (int r = 0; r < MAXAVROW-1; r++) {
		aRow[0][r] = r+1;
		aRow[1][r] = r+1;
		}
	Numerovertailu1->Checked = IkkParam.NoVert;
	Aikavertailu1->Checked = IkkParam.AikaVert;
	Nytnimi1->Checked = VertGridCols[0][5].Visible;
	Nytsarja1->Checked = VertGridCols[0][6].Visible;
	Nyttulos1->Checked = VertGridCols[0][7].Visible;
	Nyttuloksenero1->Checked = VertGridCols[0][8].Visible;
	Nyttark1->Checked = VertGridCols[0][9].Visible;
	Nytnimi2->Checked = VertGridCols[1][5].Visible;
	Nytsarja2->Checked = VertGridCols[1][6].Visible;
	Nyttulos2->Checked = VertGridCols[1][7].Visible ;
	Nyttuloksenero2->Checked = VertGridCols[1][8].Visible;
	Nyttark2->Checked = VertGridCols[1][9].Visible;
	Jonojenaikaero1->Checked = VertGridCols[1][10].Visible;
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Aikojen vertailu");
}
//---------------------------------------------------------------------------
void __fastcall TFormAikavert::DGajat1MouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	Alku1++;
	EdtAlku->Text = Alku1;
}
//---------------------------------------------------------------------------


void __fastcall TFormAikavert::FormShow(TObject *Sender)
{
	for (int i = 0; i < NAJANOTOT; i++) {
		if (this == FormAikavertailut[i]) {
			Ajanotto = i+1;
			Caption = L"Aikavertailu - " + UnicodeString(Ajanotto);
			break;
			}
		}
	DGajat1->Row = aktRivi1;
//	HaeMySQLkannasta1->Visible = TestiToiminnot;
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormAikavert::Sulje1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::BtnHakuClick(TObject *Sender)
{
	int alkuRivi, kno;

	kno = _wtoi(EdHaku->Text.c_str());
	if (!aikajono[Jono[0]] || aikajono[Jono[0]]->aktrows == 0 || kno < minkilpno || kno > maxkilpno)
		return;
	alkuRivi = aktRivi1;
	if (alkuRivi < 1 || alkuRivi > aikajono[Jono[0]]->aktrows)
		alkuRivi = 1;
	for (int rv = alkuRivi + 1; rv != alkuRivi; rv++) {
		aikatp pt;
		if (rv > aikajono[Jono[0]]->aktrows)
			rv = 0;
		aikajono[Jono[0]]->getAktTime(&pt, rv-1);
		if (pt.kno == kno) {
			DGajat1->Row = rv;
			aktRivi1 = rv;
			return;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::Kirjasinkoko1Click(TObject *Sender)
{
	wchar_t ch;
	int fs = IkkParam.FontSize;
	inputint_prompt(&fs, L"Anna taulukoiden kirjasinkoko", &ch, this);
	if (ch != ESC) {
		IkkParam.FontSize = fs;
		DGajat1->Font->Size = IkkParam.FontSize;
		DGajat2->Font->Size = IkkParam.FontSize;
		DGajat1->DefaultRowHeight = (20 * IkkParam.FontSize) / 8;
		DGajat2->DefaultRowHeight = (20 * IkkParam.FontSize) / 8;
		FormResize(Sender);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormAikavert::GridDrawCell(int grid, TObject *Sender, int ACol,
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
	  switch (Cells[grid][ACol][ARow].style) {
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
			pBitmap->Canvas->Brush->Color = clMedGray;  //; med gray
			break;
		 }
	  }
   pBitmap->Canvas->FillRect(Rect1);
   pBitmap->Canvas->Font->Size = IkkParam.FontSize;
   X = 1;
   if (Cells[grid][ACol][ARow].justify == 1) {
	   X =  Rect.Width() - pBitmap->Canvas->TextWidth(Cells[grid][ACol][ARow].text) - 2;
	   if (X < 1)
		  X = 1;
	   }
   if (Cells[grid][ACol][ARow].justify == 2) {
	   X =  (Rect.Width() - pBitmap->Canvas->TextWidth(Cells[grid][ACol][ARow].text) - 2) / 2;
	   if (X < 1)
		  X = 1;
	   }
   pBitmap->Canvas->TextOut(X,1,Cells[grid][ACol][ARow].text);
//   pBitmap->Canvas->TextRect(Rect, X,1,Cells[ACol][ARow].text);
   DGajat[grid]->Canvas->Draw(Rect.Left, Rect.Top, pBitmap);
   delete pBitmap;
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::DGajat1DrawCell(TObject *Sender, int ACol, int ARow,
		  TRect &Rect, TGridDrawState State)
{
	GridDrawCell(0, Sender, ACol, ARow, Rect, State);
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::DGajat2DrawCell(TObject *Sender, int ACol, int ARow,
		  TRect &Rect, TGridDrawState State)
{
	GridDrawCell(1, Sender, ACol, ARow, Rect, State);
}
//---------------------------------------------------------------------------


void __fastcall TFormAikavert::CBJono2Change(TObject *Sender)
{
	IkkParam.Jono2 = CBJono2->ItemIndex;
	Jono[1] = IkkParam.Jono2;
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::Nytnimi1Click(TObject *Sender)
{
	Nytnimi1->Checked = !Nytnimi1->Checked;
	VertGridCols[0][5].Visible = Nytnimi1->Checked;
	SarakeLeveydet();
}
//---------------------------------------------------------------------------



void __fastcall TFormAikavert::Nytsarja1Click(TObject *Sender)
{
	Nytsarja1->Checked = !Nytsarja1->Checked;
	VertGridCols[0][6].Visible = Nytsarja1->Checked;
	SarakeLeveydet();
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::Nyttulos1Click(TObject *Sender)
{
	Nyttulos1->Checked = !Nyttulos1->Checked;
	VertGridCols[0][7].Visible = Nyttulos1->Checked;
	SarakeLeveydet();
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::Nyttuloksenero1Click(TObject *Sender)
{
	Nyttuloksenero1->Checked = !Nyttuloksenero1->Checked;
	VertGridCols[0][8].Visible = Nyttuloksenero1->Checked;
	SarakeLeveydet();
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::Nytnimi2Click(TObject *Sender)
{
	Nytnimi2->Checked = !Nytnimi2->Checked;
	VertGridCols[1][5].Visible = Nytnimi2->Checked;
	SarakeLeveydet();
}
//---------------------------------------------------------------------------


void __fastcall TFormAikavert::Nytsarja2Click(TObject *Sender)
{
	Nytsarja2->Checked = !Nytsarja2->Checked;
	VertGridCols[1][6].Visible = Nytsarja2->Checked;
	SarakeLeveydet();
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::Nyttulos2Click(TObject *Sender)
{
	Nyttulos2->Checked = !Nyttulos2->Checked;
	VertGridCols[1][7].Visible = Nyttulos2->Checked;
	SarakeLeveydet();
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::Nyttuloksenero2Click(TObject *Sender)
{
	Nyttuloksenero2->Checked = !Nyttuloksenero2->Checked;
	VertGridCols[1][8].Visible = Nyttuloksenero2->Checked;
	SarakeLeveydet();
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::Jonojenaikaero1Click(TObject *Sender)
{
	Jonojenaikaero1->Checked = !Jonojenaikaero1->Checked;
	VertGridCols[1][10].Visible = Jonojenaikaero1->Checked;
	SarakeLeveydet();
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::Help1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Aikojen vertailu");
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::Nyttark1Click(TObject *Sender)
{
	Nyttark1->Checked = !Nyttark1->Checked;
	VertGridCols[0][9].Visible = Nyttark1->Checked;
	SarakeLeveydet();
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::Nyttark2Click(TObject *Sender)
{
	Nyttark2->Checked = !Nyttark2->Checked;
	VertGridCols[1][9].Visible = Nyttark2->Checked;
	SarakeLeveydet();
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::EdtBiasChange(TObject *Sender)
{
	Bias = _wtoi(EdtBias->Text.c_str());
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::UpDownBiasClick(TObject *Sender, TUDBtnType Button)
{
	if (Button == btNext)
		Bias++;
	if (Button == btPrev)
		Bias--;
	EdtBias->Text = Bias;
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::Aktivoi(int grid, int rivi, int status)
{
	aikatp pt, upt;

	if (rivi < 1 || rivi >= DGajat[grid]->RowCount)
		return;
	aikajono[Jono[grid]]->getAktTime(&pt, aRow[grid][rivi-1]-1);
	upt = pt;
	upt.status = status;
	tall_rivi(Jono[grid], &upt, &pt, 0, 0, 0, 0);
	BtnPaivitaClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::DGajat1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == L'T' && !Shift.Contains(ssShift) &&
		!Shift.Contains(ssAlt) && Shift.Contains(ssCtrl)) {
		Aktivoi(0, DGajat1->Row, 0);
		Key = 0;
		return;
		}
	else if (Key == L'O' && !Shift.Contains(ssShift) &&
		!Shift.Contains(ssAlt) && Shift.Contains(ssCtrl)) {
		Aktivoi(0, DGajat1->Row, 1);
		Key = 0;
		return;
		}
	else if (Key == VK_DOWN) {
		Alku1++;
		}
	else if (Key == VK_UP) {
		Alku1--;
		}
	else if (Key == VK_NEXT && !Shift.Contains(ssCtrl)) {
		Alku1 += DGajat1->RowCount - 1;
		}
	else if (Key == VK_PRIOR && !Shift.Contains(ssCtrl)) {
		Alku1 -= DGajat1->RowCount - 1;
		}
	else if (Key == VK_NEXT && Shift.Contains(ssCtrl)) {
		Alku1 = aikajono[Jono[0]]->aktrows - DGajat1->RowCount + 3;
		}
	else if (Key == VK_PRIOR && Shift.Contains(ssCtrl)) {
		Alku1 = 1;
		Bias = 0;
		}
	Key = 0;
	EdtAlku->Text = Alku1;
	EdtBias->Text = Bias;
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::UpDownAlkuClick(TObject *Sender, TUDBtnType Button)
{
	if (Button == btNext)
		Alku1++;
	if (Button == btPrev)
		Alku1--;
	EdtAlku->Text = Alku1;
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::EdtAlkuChange(TObject *Sender)
{
	Scrolling = true;
	Alku1 = _wtoi(EdtAlku->Text.c_str());
	BtnPaivitaClick(Sender);
	ScrollBar1->Position = Alku1;
	Scrolling = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::DGajat2KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == L'T' && !Shift.Contains(ssShift) &&
		!Shift.Contains(ssAlt) && Shift.Contains(ssCtrl)) {
		Aktivoi(1, DGajat2->Row, 0);
		Key = 0;
		return;
		}
	else if (Key == L'O' && !Shift.Contains(ssShift) &&
		!Shift.Contains(ssAlt) && Shift.Contains(ssCtrl)) {
		Aktivoi(1, DGajat2->Row, 1);
		Key = 0;
		return;
		}
	else if (Key == VK_DOWN) {
		Bias++;
		}
	else if (Key == VK_UP) {
		Bias--;
		}
	else if (Key == VK_NEXT && !Shift.Contains(ssCtrl)) {
		Bias += DGajat1->RowCount - 1;
		}
	else if (Key == VK_PRIOR && !Shift.Contains(ssCtrl)) {
		Bias -= DGajat1->RowCount - 1;
		}
	else if (Key == VK_NEXT && Shift.Contains(ssCtrl)) {
		Bias = aikajono[Jono[1]]->aktrows - DGajat2->RowCount - Alku1 + 3;
		}
	else if (Key == VK_PRIOR && Shift.Contains(ssCtrl)) {
		Bias = -Alku1+1;
		}
	Key = 0;
	EdtBias->Text = Bias;
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::Numerovertailu1Click(TObject *Sender)
{
	Numerovertailu1->Checked = !Numerovertailu1->Checked;
	IkkParam.NoVert = Numerovertailu1->Checked;
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::Aikavertailu1Click(TObject *Sender)
{
	wchar_t ch;

	Aikavertailu1->Checked = !Aikavertailu1->Checked;
	IkkParam.AikaVert = Aikavertailu1->Checked;
	if (IkkParam.AikaVert) {
		inputint_prompt(&IkkParam.AikaRajat[0], L"Anna alempi aikaraja (ms)", &ch, this);
		inputint_prompt(&IkkParam.AikaRajat[1], L"Anna ylempi aikaraja (ms)", &ch, this);
		}
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::BtnPoista1Click(TObject *Sender)
{
	if (DGajat[0]->Row > 0 && aRow[DGajat[0]->Row-1] > 0) {
		if (Application->MessageBoxW((UnicodeString(L"Vahvista rivin ")+UnicodeString(aRow[0][DGajat[0]->Row-1])+L" poisto").c_str(),
			L"Vahvista", MB_YESNO) == IDYES) {
			aikatp vpt;
			aikajono[Jono[0]]->getAktTime(&vpt, aRow[0][DGajat[0]->Row-1]-1);
			poista_aika(Jono[0], &vpt);
			}
		}
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::BtnPoista2Click(TObject *Sender)
{
	if (DGajat[1]->Row > 0 && aRow[DGajat[1]->Row-1] > 0) {
		if (Application->MessageBoxW((L"Vahvista rivin "+UnicodeString(aRow[1][DGajat[1]->Row-1])+L" poisto").c_str(),
			L"Vahvista", MB_YESNO) == IDYES) {
			aikatp vpt;
			aikajono[Jono[1]]->getAktTime(&vpt, aRow[1][DGajat[1]->Row-1]-1);
			poista_aika(Jono[1], &vpt);
			}
		}
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::BtnLuoUusi2Click(TObject *Sender)
{
	LuoUusi(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::BtnLuoUusi1Click(TObject *Sender)
{
	LuoUusi(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::DGajat1MouseWheelUp(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
	Alku1--;
	EdtAlku->Text = Alku1;
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::ScrollBar1Change(TObject *Sender)
{
	if (!Scrolling) {
		Alku1 = ScrollBar1->Position;
		EdtAlku->Text = Alku1;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::Aikaero1Click(TObject *Sender)
{
	wchar_t ch;

	inputint_prompt(&AikaEro, L"Anna jonojen aikaero vertailuun (ms)", &ch, this);
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::CBViimeisetClick(TObject *Sender)
{
	if (CBViimeiset->Checked || CBViimeiset2->Checked)
		Paivita_Aikanaytto();
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::Nytmysohitetutajat1Click(TObject *Sender)
{
	Nytmysohitetutajat1->Checked = !Nytmysohitetutajat1->Checked;
	aikajono[Jono[0]]->atimemask = Nytmysohitetutajat1->Checked ? 0 : '\xff';
	aikajono[Jono[0]]->haeAktAjat();
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAikavert::Nytmysohitetutajat2Click(TObject *Sender)
{
	Nytmysohitetutajat2->Checked = !Nytmysohitetutajat2->Checked;
	aikajono[Jono[1]]->atimemask = Nytmysohitetutajat2->Checked ? 0 : '\xff';
	aikajono[Jono[1]]->haeAktAjat();
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------

