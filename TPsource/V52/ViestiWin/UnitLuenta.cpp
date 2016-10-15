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

#include "UnitLuenta.h"
#include "UnitEmiTag.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormLuenta *FormLuenta;

extern INT32 uusi_emit;
extern bool luentaloki;
static FILE *testifl;
static wchar_t testiflnm[] = L"emitrek.lst";
extern luentatesti;

//---------------------------------------------------------------------------
__fastcall TFormLuenta::TFormLuenta(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
	RGkoodi->Visible = kilpparam.kaksibadge != 0;
	RGkoodi->ItemIndex = 0;
}
//---------------------------------------------------------------------------
__fastcall TFormLuenta::~TFormLuenta(void)
{
	if (luentaFl)
		delete luentaFl;
}
//---------------------------------------------------------------------------
void __fastcall TFormLuenta::Nollaa(void)
{
	wchar_t st[80];
	int style;

	MemoMsg->Visible = false;
	DKilp = 0;
	Kilpno = 0;
	Osuus = -1;
	uusi_emit = 0;
	EdtBadge->Text = L"";
	EdtJoukkue->Text = L"";
	EdtKilpno->Text = 0;
	EdtOsuus->Text = 0;
	EdtViivakoodi->Text = L"";
	for (int os = 1; os < DG1->RowCount; os++) {
		style = 0;
		Cells[0][os].text = UnicodeString(os)+L". osuus";
		Cells[1][os].text = L"";
		Cells[2][os].text = L"";
		Cells[3][os].text = L"";
		Cells[4][os].text = L"";
		for (int c = 0; c < 5; c++)
			Cells[c][os+1].style = style;
		}
	DG1->Refresh();
	OdottaaKorttia = false;
}
//---------------------------------------------------------------------------
luentaIkkParamClass::luentaIkkParamClass(void)
{
	memset(this, 0, sizeof(luentaIkkParamClass));
}
//---------------------------------------------------------------------------
void luentaIkkParamClass::operator=(luentaIkkParamClass& ikkPar)
{
	memcpy(this, &ikkPar, sizeof(luentaIkkParamClass));
}
//---------------------------------------------------------------------------
void TFormLuenta::haeParams(void)
{
	IkkParam.Tunnistus = RGTunnistus->ItemIndex;
	IkkParam.Koodi = RGkoodi->ItemIndex;
	IkkParam.Huomautaohitetusta = Huomautaohitetusta->Checked;
	IkkParam.Huomautasuljetusta = Huomautasuljetusta->Checked;
	IkkParam.EhdOsuus = CBEhdOsuus->Checked;
}
//---------------------------------------------------------------------------

int TFormLuenta::applyParams(void)
{
	RGTunnistus->ItemIndex = IkkParam.Tunnistus;
	RGkoodi->ItemIndex = IkkParam.Koodi;
	Huomautaohitetusta->Checked = IkkParam.Huomautaohitetusta;
	Huomautasuljetusta->Checked = IkkParam.Huomautasuljetusta;
	CBEhdOsuus->Checked = IkkParam.EhdOsuus;
	return(0);
}
//---------------------------------------------------------------------------
int __fastcall TFormLuenta::NaytaJoukkue(void)
{
	wchar_t st[80];
	int style, ret = 0;
	bool huomautettu = false;

	MemoMsg->Visible = false;
	Kilp.getrec(DKilp);
	if (Osuus >= Sarjat[Kilp.sarja].osuusluku)
		return(1);
	EdtJoukkue->Text = Kilp.Joukkue(st, 60);
	EdtKilpno->Text = Kilpno;
	EdtOsuus->Text = wosuuskoodi(Kilp.sarja, Osuus, 0, 0);
	DG1->RowCount = Sarjat[Kilp.sarja].osuusluku+1;
	DG1->FixedRows = 0;
	for (int os = 0; os < Sarjat[Kilp.sarja].osuusluku; os++) {
		style = 0;
		Cells[0][os+1].text = UnicodeString("Osuus ") + wosuuskoodi(Kilp.sarja, os, 0, 0);
		Cells[1][os+1].text = Kilp.Nimi(st, kilpparam.lnimi, os, -1);
		Cells[2][os+1].text = Kilp.ostiet[os].badge[RGkoodi->ItemIndex];
		Cells[3][os+1].text = aikatowstr_cols_n(st, Kilp.Maali(os, 0), t0, 0, 8);
		Cells[4][os+1].text = Kilp.TarkStr(os, true);
		if (os == Osuus) {
			style = ((Kilp.wTark(os, false) == L'-' && Kilp.ostiet[os].badge[RGkoodi->ItemIndex] == 0 &&
				Kilp.Maali(os, 0) == TMAALI0) ? 1 : 2);
			if (!huomautettu && Huomautasuljetusta->Checked && Kilp.wTark(os, 1) != L'-') {
				if (Application->MessageBoxW(L"Joukkue suljettu tai kilpailijalla tarkastusmerkintä. Kirjataanko silti",
					L"Poikkeama", MB_YESNO) != IDYES)
					ret = 1;
				huomautettu = true;
				}
			}
		if (!huomautettu && Huomautaohitetusta->Checked && Kilp.yOsuus(os) < Kilp.yOsuus(Osuus) &&
			Kilp.ostiet[os].badge[RGkoodi->ItemIndex] == 0) {
			if (Application->MessageBoxW(L"Aiemmalla osuudella ei emit-koodia. Kirjataanko silti",
				L"Poikkeama", MB_YESNO) != IDYES)
				ret = 1;
			huomautettu = true;
			}
		for (int c = 0; c < 5; c++)
			Cells[c][os+1].style = style;
		}
	DG1->Refresh();
	if (toupper(Kilp.ostiet[Osuus].seuranta) == 'G') {
		MemoMsg->Text = L"  Kilpailija osallisena GPS-seurantaan. Lähetin haettava.  ";
		MemoMsg->Color = clYellow;
		MemoMsg->Visible = true;
		}
	OdottaaKorttia = true;
	return(ret);
}
//---------------------------------------------------------------------------
void __fastcall TFormLuenta::DG1DrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
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
void __fastcall TFormLuenta::EdtViivakoodiKeyPress(TObject *Sender, System::WideChar &Key)
{
	int alios = 0, yos = 0, srj = -1;

	if (Key == L'\r') {
		if (EdtViivakoodi->Text.Length() > 2 && wcswcind(EdtViivakoodi->Text.c_str()[0], L"*%") >= 0) {
			wchar_t st[20];
			wcsncpy(st, EdtViivakoodi->Text.c_str(), 19);
			st[19] = 0;
			Osuus = -1;
			Kilpno = 0;
			if (st[0] == L'*') {
				wchar_t *p;
				for (p = st+wcslen(st)-1; p > st + 1; p--) {
					if (*p >= L'A')
						alios = towupper(*p) - L'A';
					if (*p > L'0' && *p <= L'9') {
						yos = *p - L'1';
						*p = 0;
						break;
						}
					}
				Osuus = _wtoi(st+wcslen(st)-1) - 1;
				st[wcslen(st)-1] = 0;
				Kilpno = _wtoi(st+1);
				}
			else if (wcslen(st) > 5) {
				if (st[wcslen(st)-1] >= L'A')
					alios = towupper(st[wcslen(st)-1]) - L'A';
				yos = _wtoi(st+5) - 1;
				st[5] = 0;
				Kilpno = _wtoi(st+1);
				}
			Osuus = yos;
			if (kilpparam.maxnosuus > 1 && (DKilp = getpos(Kilpno)) > 0) {
				kilptietue kilp;
				kilp.getrec(DKilp);
				srj = kilp.sarja;
				if (yos < Sarjat[srj].ntosuus)
					Osuus = Sarjat[srj].aosuus[yos] + 1 + alios;
				}
			if (Osuus >= 0 && Osuus < kilpparam.osuusluku && (DKilp = getpos(Kilpno)) > 0) {
				if (!NaytaJoukkue()) {
					Key = 0;
					EdtMsg->Text = L"Lue Emit-kortti";
					EdtMsg->Color = clLime;
					FocusControl(EdtVahvistus);
					}
				else
					Key = 1;
				}
			else
				Key = ESC;
			}
		else
			Key = ESC;
		if (Key == ESC) {
			EdtMsg->Text = L"Viivakoodi virheellinen";
			EdtMsg->Color = clFuchsia;
			}
		}
	if (Key == ESC || Key == 1) {
		EdtViivakoodi->Text = L"";
		Key = 0;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormLuenta::RGTunnistusClick(TObject *Sender)
{
	EdtViivakoodi->Enabled = RGTunnistus->ItemIndex == 0;
	EdtViivakoodi->TabStop = EdtViivakoodi->Enabled;
	EdtViivakoodi->Color = EdtViivakoodi->Enabled ? clWindow : clBtnFace;
	EdtKilpno->TabStop = !EdtViivakoodi->Enabled;
	EdtOsuus->TabStop = !EdtViivakoodi->Enabled;
	EdtKilpno->ReadOnly = EdtViivakoodi->Enabled;
	EdtOsuus->ReadOnly = EdtViivakoodi->Enabled;
	CBEhdOsuus->Visible = !EdtViivakoodi->Enabled;
}
//---------------------------------------------------------------------------
void __fastcall TFormLuenta::FormCreate(TObject *Sender)
{
	wchar_t flname[] = L"luentaloki.txt";

	Cells[0][0].text = L"Osuus";
	Cells[1][0].text = L"Nimi";
	Cells[2][0].text = L"Badge";
	Cells[3][0].text = L"Maali";
	Cells[4][0].text = L"Tark";
	DG1->ColWidths[1] = 160 * Screen->PixelsPerInch / 96;
	DG1->ColWidths[3] = 70 * Screen->PixelsPerInch / 96;
	SG1->Cells[0][0] = L"Badge";
	SG1->Cells[1][0] = L"Joukkue";
	SG1->Cells[2][0] = L"Osuus";
	if (luentaloki) {
		if (!luentaFl) {
			luentaFl = new TextFl(flname, L"a");
			}
		if (luentaFl && !luentaFl->IsOpen()) {
			delete luentaFl;
			luentaFl = NULL;
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormLuenta::EdtVahvistusKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'%') {
		Application->MessageBoxW(L"Viivakoodi luettu hyväksymiskenttään ennen edellisen kilpailijan tallentamista,", L"Poikkeama", MB_OK);
		if (uusi_emit && Application->MessageBoxW(L"Viivakoodi luettu hyväksymiskenttään ennen edellisen kilpailijan tallentamista. Tallennetaanko aiemmat tiedot?", L"Poikkeama", MB_YESNO) == IDYES)
			BtnTallennaClick(Sender);
		else
			BtnPeruutaClick(Sender);
		}
	if (Key == L'\r') {
		BtnTallennaClick(Sender);
		}
	if (Key == ESC) {
		BtnPeruutaClick(Sender);
		}
	Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormLuenta::FormResize(TObject *Sender)
{
	DG1->Height = ClientHeight - DG1->Top - 4 * Screen->PixelsPerInch / 96;
	SG1->Height = ClientHeight - SG1->Top - 4 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------
int __fastcall TFormLuenta::TarkKoodi(int badge)
{
	int kno, ibdg;
	if (badge == 200) {
		MemoMsg->Text = L"Viallinen Emit-kortti (koodi 200). Kortti on vaihdettava. Paina Esc.";
		MemoMsg->Color = clFuchsia;
		MemoMsg->Visible = true;
		FocusControl(EdtVahvistus);
		}
	ibdg = RGkoodi->ItemIndex;
	if ((kno = bdg2kno(badge, &ibdg)) > 0) {
		int d, vap = 0, os;
		kilptietue kilp;

		d = getpos(kno);
		EnterCriticalSection(&tall_CriticalSection);
		kilp.getrec(d);
		if (badge == kilp.ostiet[Osuus].badge[ibdg]) {
			MemoMsg->Text = L"Kortti jo luettu samalle kilpailijalle. Ei muutettavaa. Paina Esc.";
			MemoMsg->Color = clFuchsia;
			MemoMsg->Visible = true;
			FocusControl(EdtVahvistus);
			vap = 1;
			}
		else {
			for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
				if (badge == kilp.ostiet[os].badge[ibdg]) {
					if (wcswcind(kilp.wTark(os, false), L"HKEP") >= 0) {
						kilp.ostiet[os].badge[ibdg] = haeVapaaKoodi(badge);
						kilp.Tallenna(d, 0, 0, 0, 0);
						vap = 1;
						}
					else
						vap = 0;
					break;
					}
				}
			}
		LeaveCriticalSection(&tall_CriticalSection);
		if (!vap) {
			MemoMsg->Text = L"Kortti on varattuna joukkueen " + UnicodeString(kno) + L" osuudelle " +
				UnicodeString(os+1) + L". Kortti on vaihdettava. Paina Esc.";
			MemoMsg->Color = clFuchsia;
			MemoMsg->Visible = true;
			FocusControl(EdtVahvistus);
			}
		}
	return(0);
}
//---------------------------------------------------------------------------

MESSAGE void __fastcall TFormLuenta::NewCardHandler(TMyMessage &msg)
{
	if (uusi_emit && uusi_emit != _wtoi(EdtBadge->Text.c_str())) {
		if (luentatesti) {
			if (testifl) {
				if (feof(testifl)) {
					Application->MessageBoxW(L"Tiedosto emitrek.lst käsitelty", L"Valmis", MB_OK);
					fclose(testifl);
					testifl = NULL;
					return;
					}
				do {
					wchar_t *p, ln[100] = L"";
					int kno = 0, os = 0, bdg = 0, d;
					fgetws(ln, 99, testifl);
					if (wcslen(ln) > 5) {
						p = wcstok(ln, L",;\t\n");
						if (p) {
							kno = _wtoi(p);
							p = wcstok(NULL, L",;\t\n");
							}
						if (p) {
							os = _wtoi(p);
							p = wcstok(NULL, L",;\t\n");
							}
						if (p) {
							bdg = _wtoi(p);
							}
						if (bdg == uusi_emit && os > 0 && (d = getpos(kno)) > 0) {
							kilptietue kilp;
							EnterCriticalSection(&tall_CriticalSection);
							kilp.getrec(d);
							if (os <= Sarjat[kilp.sarja].osuusluku) {
								kilp.ostiet[os-1].badge[0] = uusi_emit;
								kilp.Tallenna(d, 0, 0, 0, 0);
								}
							LeaveCriticalSection(&tall_CriticalSection);
							if (os <= Sarjat[kilp.sarja].osuusluku) {
								SG1->RowCount++;
								SG1->Row = SG1->RowCount-2;
								SG1->Cells[0][SG1->Row] = uusi_emit;
								SG1->Cells[1][SG1->Row] = kno;
								SG1->Cells[2][SG1->Row] = os;
								}
							break;
							}
						}
					} while(!feof(testifl));
				}
			}
		else if (!OdottaaKorttia) {
			EdtMsg->Text = L"Kortti luettu väärässä vaiheessa";
			EdtMsg->Color = clFuchsia;
			}
		else {
			EdtMsg->Text = L"Kortti luettu. Tallenna tai peruuta";
			EdtMsg->Color = clWindow;
			}
		EdtBadge->Text = uusi_emit;
		TarkKoodi(uusi_emit);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormLuenta::HaeUusiTietue(void)
{
	SendMessage(Handle,WM_MYMSGNEWCARD,0,0);
}
//---------------------------------------------------------------------------

void __fastcall TFormLuenta::EdtVahvistusEnter(TObject *Sender)
{
	EdtVahvistus->Color = clYellow;
}
//---------------------------------------------------------------------------
void __fastcall TFormLuenta::EdtVahvistusExit(TObject *Sender)
{
	EdtVahvistus->Color = clBtnFace;
}
//---------------------------------------------------------------------------
void __fastcall TFormLuenta::EdtKilpnoKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r' || Key == L'\t') {
		if (EdtKilpno->Text.Length() > 0 && (Kilpno = _wtoi(EdtKilpno->Text.c_str())) > 0 &&
			(DKilp = getpos(Kilpno)) > 0)
			{
			if (CBEhdOsuus->Checked) {
				int os;
				kilptietue kilp;
				kilp.getrec(DKilp);
				for (os = 0; os < Sarjat[kilp.sarja].osuusluku-1; os++)
					if (kilp.ostiet[os].badge[0] == 0)
						break;
				EdtOsuus->Text = os+1;
				}
			EdtMsg->Text = L"Anna osuus";
			FocusControl(EdtOsuus);
			EdtMsg->Color = clAqua;
			}
		else {
			DKilp = 0;
			EdtMsg->Text = L"Numero virheellinen";
			EdtMsg->Color = clFuchsia;
			}
		Key = 0;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormLuenta::EdtOsuusKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == L'\r') {
		if (EdtKilpno->Text.Length() > 0 && (Kilpno = _wtoi(EdtKilpno->Text.c_str())) > 0 &&
			(DKilp = getpos(Kilpno)) > 0 && EdtOsuus->Text.Length() > 0  &&
			(Osuus = tulkOsuuskoodi(sarjaKno(Kilpno), EdtOsuus->Text.c_str())) >= 0 && Osuus < kilpparam.osuusluku) {
			if (!NaytaJoukkue()) {
				EdtMsg->Text = L"Lue Emit-kortti";
				EdtMsg->Color = clLime;
				FocusControl(EdtVahvistus);
            	}
			}
		else {
			EdtMsg->Text = L"Numero tai osuus virheellinen";
			EdtMsg->Color = clFuchsia;
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
void __fastcall TFormLuenta::BtnTallennaClick(TObject *Sender)
{
	if (!uusi_emit) {
		EdtMsg->Text = L"Emitkoodi puuttuu, ei tallennettu";
		EdtMsg->Color = clYellow;
		if (luentaFl) {
			swprintf(lokiLine, L"%s\tTallennusyritys\t%4d-%d\tEmit\tpuuttuu\n", wkello(),
				Kilpno, Osuus + 1);
			luentaFl->WriteLine(lokiLine);
			}
		}
	else {
		EnterCriticalSection(&tall_CriticalSection);
		Kilp.getrec(DKilp);
		Kilp.ostiet[Osuus].badge[RGkoodi->ItemIndex] = uusi_emit;
		Kilp.Tallenna(DKilp, 0, 0, 0, 0);
		LeaveCriticalSection(&tall_CriticalSection);
		SG1->RowCount++;
		SG1->Row = SG1->RowCount-2;
		SG1->Cells[0][SG1->Row] = uusi_emit;
		SG1->Cells[1][SG1->Row] = Kilp.kilpno;
		SG1->Cells[2][SG1->Row] = wosuuskoodi(Kilp.sarja, Osuus, 0, 0);
		if (luentaFl) {
			swprintf(lokiLine, L"%s\tTallennus\t%4d-%d\tEmit\t%d\n", wkello(),
				Kilp.kilpno, Osuus + 1, uusi_emit);
			luentaFl->WriteLine(lokiLine);
			luentaFl->Flush();
			}
		if (RGTunnistus->ItemIndex == 0) {
			EdtMsg->Text = L"Tallennettu. Lue seuraava viivakoodi";
			}
		else {
			EdtMsg->Text = L"Tallennettu. Syötä seuraava numero";
			}
		EdtMsg->Color = clAqua;
		Nollaa();
		}
	if (RGTunnistus->ItemIndex == 0)
		FocusControl(EdtViivakoodi);
	else
		FocusControl(EdtKilpno);
}
//---------------------------------------------------------------------------
void __fastcall TFormLuenta::BtnPeruutaClick(TObject *Sender)
{
	if (luentaFl) {
		swprintf(lokiLine, L"%s\tPeruutus\t%4d-%d\tEmit\t%d\n", wkello(),
			Kilpno, Osuus + 1, uusi_emit);
		luentaFl->WriteLine(lokiLine);
		}
	if (RGTunnistus->ItemIndex == 0) {
		EdtMsg->Text = L"Peruutettu. Lue seuraava viivakoodi";
		}
	else {
		EdtMsg->Text = L"Peruutettu. Syötä seuraava numero";
		}
	EdtMsg->Color = clAqua;
	Nollaa();
	if (RGTunnistus->ItemIndex == 0)
		FocusControl(EdtViivakoodi);
	else
		FocusControl(EdtKilpno);
}
//---------------------------------------------------------------------------
void __fastcall TFormLuenta::FormShow(TObject *Sender)
{
	if (luentatesti) {
		if (testifl == NULL)
			testifl = _wfopen(testiflnm, L"rt");
		if (testifl == NULL)
			Application->MessageBoxW(L"Tiedoston emitrek.lst avaaminen ei onnistunut", L"Virhe", MB_OK);
		else
			EdtMsg->Text = L"Testi odottaa Emit-koodeja";
		return;
	}
	EdtMsg->Text = L"Lue viivakoodi tai syötä joukkueen numero";
	EdtMsg->Color = clAqua;
	Nollaa();
}
//---------------------------------------------------------------------------

void __fastcall TFormLuenta::HuomautaohitetustaClick(TObject *Sender)
{
	Huomautaohitetusta->Checked = !Huomautaohitetusta->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormLuenta::HuomautasuljetustaClick(TObject *Sender)
{
	Huomautasuljetusta->Checked = !Huomautasuljetusta->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormLuenta::Help1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Emitkoodien luenta");
}
//---------------------------------------------------------------------------

void __fastcall TFormLuenta::EdtViivakoodiEnter(TObject *Sender)
{
	EdtViivakoodi->Color = clYellow;
}
//---------------------------------------------------------------------------

void __fastcall TFormLuenta::EdtViivakoodiExit(TObject *Sender)
{
	EdtViivakoodi->Color = clWindow;
}
//---------------------------------------------------------------------------

void __fastcall TFormLuenta::EdtKilpnoEnter(TObject *Sender)
{
	EdtKilpno->Color = clYellow;
}
//---------------------------------------------------------------------------

void __fastcall TFormLuenta::EdtKilpnoExit(TObject *Sender)
{
	EdtKilpno->Color = clWindow;
}
//---------------------------------------------------------------------------

void __fastcall TFormLuenta::EdtOsuusEnter(TObject *Sender)
{
	EdtOsuus->Color = clYellow;
}
//---------------------------------------------------------------------------

void __fastcall TFormLuenta::EdtOsuusExit(TObject *Sender)
{
	EdtOsuus->Color = clWindow;
}
//---------------------------------------------------------------------------

void __fastcall TFormLuenta::Emitlaitteidenyhteydetjaohjaus1Click(TObject *Sender)

{
	if (!FormEmiTag)
		FormEmiTag = new TFormEmiTag(this);
	FormEmiTag->Show();
	if (FormEmiTag->WindowState == wsMinimized)
		FormEmiTag->WindowState = wsNormal;
	FormEmiTag->BringToFront();
}
//---------------------------------------------------------------------------

