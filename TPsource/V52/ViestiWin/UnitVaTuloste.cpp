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

#include "UnitVaTuloste.h"
#include "UnitKirjoitinVal.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormVaTuloste *FormVaTuloste;

void tulostaRastiVa(tulostusparamtp *tulprm, int kilpno, int osuus);
extern INT32 uusi_emit;

//---------------------------------------------------------------------------
__fastcall TFormVaTuloste::TFormVaTuloste(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::AinoaSuoraanClick(TObject *Sender)
{
	AinoaSuoraan->Checked = !AinoaSuoraan->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::AinaViimeinenClick(TObject *Sender)
{
	AinaViimeinen->Checked = !AinaViimeinen->Checked;
	Button1->Visible = !AinaViimeinen->Checked;
	Button2->Visible = Button1->Visible;
	Button3->Visible = Button1->Visible;
	Label1->Visible = Button1->Visible;
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::Nollaa(void)
{
	wchar_t st[80];
	int style;

	DKilp = 0;
	Kilpno = 0;
	Osuus = -1;
	memset(KilpnoA, 0, sizeof(KilpnoA));
	memset(OsuusA, 0, sizeof(OsuusA));
	EdtBadge->Text = L"";
	EdtJoukkue->Text = L"";
	EdtKilpno->Text = 0;
	EdtOsuus->Text = 0;
	for (int os = 1; os < DG1->RowCount; os++) {
		style = 0;
		Cells[0][os].text = L"";
		Cells[1][os].text = L"";
		Cells[2][os].text = L"";
		Cells[3][os].text = L"";
		Cells[4][os].text = L"";
		for (int c = 0; c < 5; c++)
			Cells[c][os+1].style = style;
		}
	DG1->Row = 1;
	DG1->RowCount = 2;
	DG1->Refresh();
}
//---------------------------------------------------------------------------
int __fastcall TFormVaTuloste::NaytaJoukkue(void)
{
	wchar_t st[80];
	int style;

	Kilp.getrec(DKilp);
	if (Osuus >= Sarjat[Kilp.sarja].osuusluku)
		return(1);
	EdtJoukkue->Text = Kilp.Joukkue(st, 60);
	EdtKilpno->Text = Kilpno;
	EdtOsuus->Text = Osuus+1;
	return(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::NaytaKilpailijat(int badge)
{
	wchar_t st[80];
	int style, kno, r;
	kilptietue kilp;

	Nollaa();
	r = 0;
	EdtBadge->Text = badge;
	for (int os = 1; os < kilpparam.n_os_akt; os++) {
		if (AinaViimeinen->Checked || (kno = bdg2kno(badge+os*BADGEASKEL, 0)) < minkilpno) {
			os = 0;
			if ((kno = bdg2kno(badge, 0)) < minkilpno)
				break;
			}
		kilp.getrec(getpos(kno));
		for (Osuus = 0; Osuus < Sarjat[kilp.sarja].osuusluku; Osuus++){
			if (kilp.ostiet[Osuus].badge[0] == badge+os*BADGEASKEL) {
				if (r >= DG1->RowCount-1)
					DG1->RowCount++;
				r++;
				DG1->Row = DG1->RowCount-1;
				Kilpno = kno;
				Kilp = kilp;
				EdtJoukkue->Text = Kilp.Joukkue(st, 60);
				EdtKilpno->Text = Kilpno;
				EdtOsuus->Text = Osuus+1;
				KilpnoA[DG1->Row-1] = Kilpno;
				OsuusA[DG1->Row-1] = Osuus;
				JoukkueA[DG1->Row-1] = EdtJoukkue->Text;
				Cells[0][DG1->Row].text = UnicodeString(Osuus+1)+L". osuus";
				Cells[1][DG1->Row].text = Kilp.Nimi(st, kilpparam.lnimi, Osuus, -1);
				Cells[2][DG1->Row].text = Kilp.ostiet[Osuus].badge[0];
				Cells[3][DG1->Row].text = aikatowstr_cols_n(st, Kilp.Maali(Osuus, 0), t0, 0, 8);
				Cells[4][DG1->Row].text = Kilp.TarkStr(Osuus, true);
				for (int c = 0; c < 5; c++) {
					if (DG1->Row > 1)
						Cells[c][DG1->Row-1].style = 0;
					Cells[c][DG1->Row].style = 1;
					}
				break;
				}
			}
		if (os == 0)
			break;
		}
	DG1->Refresh();
	if (AinaViimeinen->Checked || (AinoaSuoraan->Checked && DG1->RowCount == 2))
		Tulosta();
	DG1->Col = 1;
	FocusControl(DG1);
}
//---------------------------------------------------------------------------

MESSAGE void __fastcall TFormVaTuloste::NewCardHandler(TMyMessage &msg)
{
	if (uusi_emit && uusi_emit != _wtoi(EdtBadge->Text.c_str())) {
		EdtBadge->Text = uusi_emit;
		NaytaKilpailijat(uusi_emit);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormVaTuloste::HaeUusiTietue(void)
{
	SendMessage(Handle,WM_MYMSGNEWCARD,0,0);
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::EdtKilpnoKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r' || Key == L'\t') {
		if (EdtKilpno->Text.Length() > 0 && (Kilpno = _wtoi(EdtKilpno->Text.c_str())) > 0 &&
			(DKilp = getpos(Kilpno)) > 0)
			{
			FocusControl(EdtOsuus);
			}
		else {
			DKilp = 0;
			}
		Key = 0;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::EdtOsuusKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		if (EdtKilpno->Text.Length() > 0 && (Kilpno = _wtoi(EdtKilpno->Text.c_str())) > 0 &&
			(DKilp = getpos(Kilpno)) > 0 && EdtOsuus->Text.Length() > 0 &&
			(Osuus = _wtoi(EdtOsuus->Text.c_str())-1) >= 0 && Osuus < kilpparam.osuusluku) {
			FocusControl(EdtVahvistus);
			}
		Key = 0;
		}
	if (Key == ESC) {
		Key = 0;
		Nollaa();
		FocusControl(EdtKilpno);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::Button1Click(TObject *Sender)
{
	if (DG1->Row > 1) {
		for (int c = 0; c < 5; c++) {
			Cells[c][DG1->Row].style = 0;
			Cells[c][DG1->Row-1].style = 1;
			}
		DG1->Row--;
		EdtJoukkue->Text = JoukkueA[DG1->Row-1];
		EdtKilpno->Text = KilpnoA[DG1->Row-1];
		EdtOsuus->Text = OsuusA[DG1->Row-1]+1;
		}
	DG1->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::Button2Click(TObject *Sender)
{
	if (DG1->Row < DG1->RowCount-1) {
		for (int c = 0; c < 5; c++) {
			Cells[c][DG1->Row].style = 0;
			Cells[c][DG1->Row+1].style = 1;
			}
		DG1->Row++;
		EdtJoukkue->Text = JoukkueA[DG1->Row-1];
		EdtKilpno->Text = KilpnoA[DG1->Row-1];
		EdtOsuus->Text = OsuusA[DG1->Row-1]+1;
		}
	DG1->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::Button3Click(TObject *Sender)
{
	Tulosta();
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::DG1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
          TGridDrawState State)
{
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
				break;
			case 1:
				pBitmap->Canvas->Brush->Color = clYellow;
				break;
			case 2:
				pBitmap->Canvas->Brush->Color = clFuchsia;
				break;
			}
		pBitmap->Canvas->Font->Style = TFontStyles();
		}
   pBitmap->Canvas->FillRect(Rect1);
   pBitmap->Canvas->Font->Size = 10;
   pBitmap->Canvas->TextOut(1,1,Cells[ACol][ARow].text);
   DG1->Canvas->Draw(Rect.Left, Rect.Top, pBitmap);
   delete pBitmap;
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::DG1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && !Shift.Contains(ssAlt))
		Tulosta();
	if (Key == VK_UP)
		Button1Click(Sender);
	if (Key == VK_DOWN)
		Button2Click(Sender);
	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::Kirjoittimenvalinta1Click(TObject *Sender)
{
	FormKirjVal = new TFormKirjVal(this);
	FormKirjVal->ShowModal();
	wcsncpy(EmitKirjoitin,
		FormKirjVal->KirjoitinVal->Items->Strings[FormKirjVal->KirjoitinVal->ItemIndex].c_str(), 198);
	delete FormKirjVal;
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::Tulosta(void)
{
	emittp Em;

	if (DG1->Row > 0 && KilpnoA[DG1->Row-1] >= minkilpno) {
		Kilpno = KilpnoA[DG1->Row-1];
		Osuus = OsuusA[DG1->Row-1];
		}
	getem(&Em, Kilpno, Osuus);
	if (Em.kilpno != Kilpno)
		return;
	if (wcslen(EmitKirjoitin) == 0) {
		FormKirjVal = new TFormKirjVal(this);
		FormKirjVal->FormShow(this);
		wcsncpy(EmitKirjoitin,
			FormKirjVal->KirjoitinVal->Items->Strings[FormKirjVal->KirjoitinVal->ItemIndex].c_str(), 198);
		delete FormKirjVal;
		}
	EmitTulPrm.printer = GDIPRINTER;
	if (!EmitTulPrm.lstf) {
		if ((EmitTulPrm.lstf = openprfile(EmitKirjoitin, 1, TRUE, FALSE, 0, FALSE))
			== NULL) {
			Application->MessageBoxW(L"Kirjoittimen käyttö ei onnistu", L"Virhe", MB_OK);
			return;
			}
		if (EmitTulPrm.lstf && EmitTulPrm.lstf->prno == 2)
			initgdiprinter(&EmitTulPrm);
		}
	tulostaRastiVa(&EmitTulPrm, Em.kilpno, Osuus);
	closeprfile(EmitTulPrm.lstf);
	EmitTulPrm.lstf = NULL;
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::DG1KeyPress(TObject *Sender, System::WideChar &Key)

{
	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::EdtVahvistusEnter(TObject *Sender)
{
	EdtVahvistus->Color = clYellow;
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::EdtVahvistusExit(TObject *Sender)
{
	EdtVahvistus->Color = clBtnFace;
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::EdtVahvistusKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		Tulosta();
		Nollaa();
		}
	if (Key == ESC) {
		Nollaa();
		}
	Key = 0;
	FocusControl(EdtKilpno);
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::FormCreate(TObject *Sender)
{
	Cells[0][0].text = L"Osuus";
	Cells[1][0].text = L"Nimi";
	Cells[2][0].text = L"Badge";
	Cells[3][0].text = L"Maali";
	Cells[4][0].text = L"Tark";
	DG1->ColWidths[1] = 200 * Screen->PixelsPerInch / 96;
	DG1->ColWidths[3] = 75 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::FormResize(TObject *Sender)
{
	DG1->Height = ClientHeight - DG1->Top;
}
//---------------------------------------------------------------------------
void __fastcall TFormVaTuloste::Suljekaavake1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

