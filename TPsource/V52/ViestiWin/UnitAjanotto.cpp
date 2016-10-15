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

#include "UnitAjanotto.h"
#include "UnitMain.h"
#include "UnitEmiTag.h"
#include "UnitNappain.h"
#include "UnitSarjaVal.h"
#include "UnitEmitSQL.h"
#include "UnitAikakorjaus.h"
#include "UnitAikavert.h"
#include "TpLaitteet.h"
#include "UnitMaalikello.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAjanotto *FormAjanotot[NAJANOTOT];

void initajat(int ino, int vaihda);
static const int cRivi = 0;
static const int cAika = 1;
static const int cBdg = 2;
static const int cKno = 3;
static const int cOs = 4;
static const int cPst = 5;
static const int cJk = 6;
static const int cNimi = 7;
static const int cSrj = 8;
static const int cTls = 9;
static const int cEro = 10;
static const int cTrk = 11;

//---------------------------------------------------------------------------
__fastcall TFormAjanotto::TFormAjanotto(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	DGajat[0] = DGajat1;
	DGajat[1] = DGajat2;
//	applyParams();
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::FormCreate(TObject *Sender)
{
	for (int i = 0; i < NREGNLY; i++) {
		if (regnly[i] == LID_EMITAG || regnly[i] == LID_LUKIJA) {
			Toiminnot1->Visible = true;
			emiTagohjaus1->Visible = true;
			}
		if (regnly[i] == LID_SIRIT) {
			Toiminnot1->Visible = true;
			SynkronoiSirit1->Visible = true;
//			KynnistSirityhteysuudelleen1->Visible = true;
//			Sirityhteydenpollaus1->Visible = true;
			}
		}
	if (emitfl == 0 && RGSyotto->Items->Count == 4)
		RGSyotto->Items->Delete(3);
	RGSyotto->ItemIndex = 0;
	DGajat1->ColCount = MAXAJCOL;
	DGajat2->ColCount = MAXAJCOL;
	aktRivi1 = 1;
	tallVertRivi();
	if (aikajono[0] == NULL || aikajono[0]->atime == NULL) {
		ajanottofl = 1;
		inittimer();
		}
	CBJono->Clear();
	if (maxjono) {
		for (int r = 0; r <= maxjono; r++) {
			CBJono->Items->Add(L"Jono "+UnicodeString(r+1));
			}
		}
	else {
		CBJono->Items->Add(L"Jono 1");
		}
	CBJono->ItemIndex = 0;
	CBOsVal->Clear();
	CBOsVal->Items->Add(L"Auto");
	CBOsVal->Items->Add(L"Ankkur");
	for (int os = 1; os <= kilpparam.osuusluku; os++)
		CBOsVal->Items->Add(os);
	CBOsVal->ItemIndex = kilpparam.partio != 0 ? 1 : 0;
	CBPisteVal->Clear();
	CBPisteVal->Items->Add(L"Auto");
	CBPisteVal->Items->Add(L"L‰htˆ");
	CBPisteVal->Items->Add(L"Maali");
	for (int ip = 1; ip <= kilpparam.valuku; ip++)
		CBPisteVal->Items->Add(ip);
	if (va_mpiste[Jono] >= -2 && va_mpiste[Jono] <= kilpparam.valuku)
		CBPisteVal->ItemIndex = va_mpiste[Jono] + 2;
	else
		CBPisteVal->ItemIndex = 2;
	Panel4->Visible = false;
	Panel3->Visible = kilpparam.badgelaji == L'S' || kilpparam.partio != 0;
	Nytosuusvalinta1->Checked = kilpparam.partio != 0;
	Width = (865 + IkkParam.ColOn[cBdg] * IkkParam.ColW[cBdg]) * Screen->PixelsPerInch / 96;
	if (Panel3->Visible)
		Width += Panel3->Width;
	FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::FormResize(TObject *Sender)
{
	int nr;

	for (int c = 0; c < MAXAJCOL; c++) {
		DGajat1->ColWidths[c] = IkkParam.ColOn[c] * IkkParam.ColW[c] * Screen->PixelsPerInch / 96;
		DGajat2->ColWidths[c] = IkkParam.ColOn[c] * IkkParam.ColW[c] * Screen->PixelsPerInch / 96;
		}
	if (RGNayta->ItemIndex == 2) {
		DGajat2->Height = 90 * Screen->PixelsPerInch / 96;
		DGajat2->Top = ClientHeight - DGajat2->Height;
		DGajat1->Height = DGajat2->Top - DGajat1->Top - 4 * Screen->PixelsPerInch / 96;
		DGajat1->Visible = true;
		DGajat2->Visible = true;
		}
	if (RGNayta->ItemIndex == 0) {
		DGajat2->Top = ClientHeight - 40 * Screen->PixelsPerInch / 96;
		DGajat2->Height = 30 * Screen->PixelsPerInch / 96;
		DGajat1->Height = ClientHeight - DGajat1->Top;
		DGajat1->Visible = true;
		DGajat2->Visible = false;
		}
	if (RGNayta->ItemIndex == 1) {
		DGajat2->Top = DGajat1->Top;
		DGajat1->Height = 50 * Screen->PixelsPerInch / 96;
		DGajat2->Height = ClientHeight - DGajat2->Top;
		DGajat1->Visible = false;
		DGajat2->Visible = true;
		}
	nr = (DGajat2->ClientHeight) / (DGajat2->DefaultRowHeight+2);
	if (nr < 1)
		nr = 1;
	DGajat2->RowCount = nr;
	OnLine = true;
	Panel2->Top = 0;
	Panel2->Height = ClientHeight;
	Panel3->Top = ClientHeight - Panel3->Height;
	Panel4->Top = 0;
	Panel4->Height = Panel3->Top;
	Button8->Top = Panel4->Height - Button8->Height - 2;
	Memo1->Height = Button8->Top - Memo1->Top - 3;
	Panel3->Left = ClientWidth - Panel3->Width;
	Panel4->Left = Panel3->Left;
	Panel2->Left = ClientWidth - Panel2->Width -
		((Panel3->Visible || Panel4->Visible) ? Panel3->Width : 0);
	DGajat1->Width = Panel2->Left;
	DGajat2->Width = Panel2->Left;
	DGajat2->RowCount = DGajat2->Height / (DGajat2->DefaultRowHeight+2);
//	OnLine = DGajat2->Visible;
}
//---------------------------------------------------------------------------

ajatIkkParamClass::ajatIkkParamClass(void)
{
	memset(this, 0, sizeof(ajatIkkParamClass));
	Ennakoi = false;
	Suppeat = false;
	VaadiVahvistus = 1;
	Jono = 0;
	SyottoTila = 0;
	NayttoTila = 2;
	NappainPiste = -2;
	AikaLisays = 1;
	KeyCode = 0;
	ColW[cRivi] = 40;
	ColW[cAika] = 70;
	ColW[cBdg] = 40;
	ColW[cKno] = 35;
	ColW[cOs] = 30;
	ColW[cPst] = 30;
	ColW[cJk] = 140;
	ColW[cNimi] = 120;
	ColW[cSrj] = 70;
	ColW[cTls] = 70;
	ColW[cEro] = 50;
	ColW[cTrk] = 30;
	for (int c = 0; c < MAXAJCOL; c++) {
		ColOn[c] = (c == cBdg ? 0 : 1);
		}
}
//---------------------------------------------------------------------------
int TFormAjanotto::applyParams(void)
{
	Salliennakoivanumerosytt1->Checked = IkkParam.Ennakoi;
	Vaaditallennuksillevahvistus1->Checked = IkkParam.VaadiVahvistus == 1;
	Vaadivahvistus2->Checked = IkkParam.VaadiVahvistus == 2;
	RGSyotto->ItemIndex = IkkParam.SyottoTila;
	RGNayta->ItemIndex = IkkParam.NayttoTila;
	CBJono->ItemIndex = IkkParam.Jono;
	if (IkkParam.AikaLisays <= 1) {
		IkkParam.AikaLisays = 1;
		Pieninmahdollinen01ms1->Checked = true;
		}
	else {
		IkkParam.AikaLisays = kilpparam.pyor[2]*AIKAJAK;
		Pieninmahdollinen01ms1->Checked = false;
		}
	Sijoitukseenvaikuttavalisys1->Checked = !Pieninmahdollinen01ms1->Checked;
	return(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::tallVertRivi(void)
{
	if (aktRivi1 < 1 || aktRivi1 >= DGajat1->RowCount || aktRivi1 >= CellRows[0])
		return;
	VertRivi[0] = Cells[0][aktRivi1][cAika].text;
	VertRivi[1] = Cells[0][aktRivi1][cBdg].text;
	VertRivi[2] = Cells[0][aktRivi1][cKno].text;
	VertRivi[3] = Cells[0][aktRivi1][cOs].text;
	VertRivi[4] = Cells[0][aktRivi1][cPst].text;
}
//---------------------------------------------------------------------------
bool __fastcall TFormAjanotto::vertRiviMuutt(void)
{
	if (aktRivi1 < 1 || aktRivi1 >= DGajat1->RowCount || aktRivi1 >= CellRows[0])
		return(false);
	if (VertRivi[0] != Cells[0][aktRivi1][cAika].text ||
		(IkkParam.SyottoTila == 3 && VertRivi[1] != Cells[0][aktRivi1][cBdg].text) ||
		(IkkParam.SyottoTila != 3 && (
			VertRivi[2] != Cells[0][aktRivi1][cKno].text ||
			(DGajat1->Col > cAika && VertRivi[3] != Cells[0][aktRivi1][cOs].text) ||
			(DGajat1->Col == cOs && VertRivi[4] != Cells[0][aktRivi1][cPst].text))
			))
//		return(true);
		return(false);
	return(false);
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::naytaRivi(int grid, int r, int rw, int jono)
{
	int d, tl, ero, ero1, srj;
	wchar_t as[60], pisteet[] = L"?ALM";
	kilptietue kilp;
	aikatp pt, upt;

	if (rw > aikajono[jono]->aktrows) {
		memset(&pt, 0, sizeof(pt));
		pt.t = AIKAJAK*TMAALI0;
		}
	else
		aikajono[jono]->getAktTime(&pt, rw);
	for (int c = 0; c < MAXAJCOL; c++)
		Cells[grid][r][c].style = pt.status ? 7 : 0;
	Cells[grid][r][cRivi].justify = 1;
	Cells[grid][r][cAika].justify = 1;
	Cells[grid][r][cBdg].justify = 1;
	Cells[grid][r][cKno].justify = 1;
	Cells[grid][r][cOs].justify = 2;
	Cells[grid][r][cPst].justify = 2;
	Cells[grid][r][cTls].justify = 1;
	Cells[grid][r][cEro].justify = 1;
	Cells[grid][r][cTrk].justify = 2;
	EnterCriticalSection(&tall_CriticalSection);
	if (pt.kno > 0 && (d = getpos(pt.kno)) > 0) {
		kilp.getrec(d);
		srj = kilp.sarja;
		}
	else
		srj = -1;
	LeaveCriticalSection(&tall_CriticalSection);
	Cells[grid][r][cRivi].text = UnicodeString(rw+1);
	Cells[grid][r][cAika].text = AIKATOWSTRS(as, purajak(pt.t), t0);
//	if (IkkParam.SyottoTila && SG == DGajat1 && r == aktRivi1 &&
//		((DGajat1->Col == 2 && pt.kno != _wtoi(EdKno->Text.c_str())) ||
//		 (IkkParam.SyottoTila == 2 && DGajat1->Col == 3 && UnicodeString(pt.piste) != EdPiste->Text)))
	if (IkkParam.SyottoTila == 1 && rw == aikajono[jono]->aktrows-1 && pt.kno == 0 && pt.t != AIKAJAK*TMAALI0) {
		upt = pt;
		while (Memo1->Lines->Count > 0 && upt.kno == 0) {
			int kno = 0;
			if (upt.piste == -2)
				upt.piste = va_mpiste[Jono];
			if (Memo1->Lines->Strings[0].Length() == 0 ||
				(kno = _wtoi(Memo1->Lines->Strings[0].c_str())) == 0) {
				Memo1->Lines->Delete(0);
				continue;
				}
			kno = _wtoi(Memo1->Lines->Strings[0].c_str());
			if (getpos(kno) > 0)
				upt.kno = kno;
			Memo1->Lines->Delete(0);
			}
		if (upt.kno) {
			int uptr, d, os;
			os = CBOsVal->ItemIndex-2;
			if (upt.piste > -2) {
				EnterCriticalSection(&tall_CriticalSection);
				d = getpos(upt.kno);
				kilp.getrec(d);
				if (os == -1)
					os = Sarjat[kilp.sarja].osuusluku - 1;
				else if (os < -1)
					os = haeseurosuus(&kilp);
				if (os >= 0) {
					kilp.setMaali(os, upt.piste, pyoristatls(purajak(upt.t), 1));
					tallenna(&kilp, d, 0, 0, 0, 0);
					if (r == aktRivi1) {
						aktRivi1++;
						DGajat1->Row = aktRivi1;
						aikajono[jono]->getAktTime(&eAika, rw+1);
						}
					}
				LeaveCriticalSection(&tall_CriticalSection);
				}
			if (os < 0)
				os = 0;
			upt.osuus = os;
			tall_rivi(jono, &upt, &pt, &uptr, 0, 0, false);
			pt = upt;
			}
		}
	if (IkkParam.SyottoTila && grid == 0 && r == aktRivi1 && vertRiviMuutt()) {
		if (memcmp(&pt, &eAika, sizeof(aikatp))) {
			Application->MessageBoxW(L"Ylim‰‰r‰inen aika tai vastaanotettu muutos muokattavaan riviin", L"Muutos", MB_OK);
			eAika = pt;
			}
		}
	if (IkkParam.SyottoTila == 3)
		Cells[grid][r][cBdg].text = UnicodeString(pt.badge);
	Cells[grid][r][cKno].text = UnicodeString(pt.kno);
	Cells[grid][r][cOs].text = wosuuskoodi(srj, pt.osuus, 0, 0);
	if (pt.piste < 1)
		Cells[grid][r][cPst].text = UnicodeString(pisteet[pt.piste+3]);
	else
		Cells[grid][r][cPst].text = UnicodeString(pt.piste);
	if (pt.kno > 0 && (d = getpos(pt.kno)) > 0) {
		Cells[grid][r][cJk].text = kilp.Joukkue(as, 35);
		Cells[grid][r][cSrj].text = Sarjat[kilp.sarja].sarjanimi;
		if (pt.osuus >= 0 && pt.osuus < Sarjat[kilp.sarja].osuusluku) {
			Cells[grid][r][cNimi].text = kilp.Nimi(as, 20, pt.osuus, 1);
			if (pt.piste > -1 && (tl = kilp.Maali(pt.osuus, pt.piste)) != TMAALI0) {
				ero = purajak(pt.t) - tl;
				elimwzb1(aikatowstr_cols_n(as, kilp.Tulos(pt.osuus, pt.piste), 0, 0, kilpparam.laikasj));
				Cells[grid][r][cTls].text = as;
				ero1 = ero/kilpparam.pyor[2];
				if (ero1)
					Cells[grid][r][cEro].text = eroaikatowstr(as, ero, kilpparam.laikasj);
				else
					Cells[grid][r][cEro].text = L"";
				}
			else {
				Cells[grid][r][cTls].text = L"";
				Cells[grid][r][cEro].text = L"";
				}
			Cells[grid][r][cTrk].text = (kilp.wTark(pt.osuus) != L'-') ? UnicodeString(kilp.wTark(pt.osuus)) : UnicodeString(L"");
			}
		else {
			Cells[grid][r][cNimi].text = L"";
			Cells[grid][r][cTls].text = L"";
			Cells[grid][r][cEro].text = L"";
			Cells[grid][r][cTrk].text = L"";
			}
		}
	else if (pt.kno > 30000) {
		as[0] = 0;
		if (pt.kno > 30000 && pt.kno <= 30000 + sarjaluku) {
			swprintf(as, L"Sarjan %s l‰htˆ", Sarjat[pt.kno-30001].Sarjanimi());
			}
		if (pt.kno > 31000 && pt.kno < 32000) {
			swprintf(as, L"Er‰n %d l‰htˆ", pt.kno-31000);
			}
		Cells[grid][r][cJk].text = as;
		Cells[grid][r][cNimi].text = L"";
		Cells[grid][r][cSrj].text = L"";
		Cells[grid][r][cTls].text = L"";
		Cells[grid][r][cEro].text = L"";
		Cells[grid][r][cTrk].text = L"";
		}
	else {
		if (rw < aikajono[jono]->aktrows)
			swprintf(as, L"Badge: %d, Portti: %d L‰hde: %d", pt.badge, (int) pt.kanava, pt.lahde);
		else
			as[0] = 0;
		Cells[grid][r][cJk].text = as;
		Cells[grid][r][cNimi].text = L"";
		Cells[grid][r][cSrj].text = L"";
		Cells[grid][r][cTls].text = L"";
		Cells[grid][r][cEro].text = L"";
		Cells[grid][r][cTrk].text = L"";
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::naytaAjat1(void)
{
	int r, r2, n;

	naytaTCPstatus();
	r2 = aikajono[Jono]->aktrows;
	if (r2 > 0)
		DGajat1->RowCount = r2+2;
	else
		DGajat1->RowCount = 2;
	if (CellRows[0] < DGajat1->RowCount) {
		CellRows[0] = DGajat1->RowCount + 200;
		if (Cells[0] != NULL)
			delete[] Cells[0];
		Cells[0] = new rowTp[CellRows[0]];
		Cells[0][0][cRivi].text = L"Rivi";
		Cells[0][0][cAika].text = L"Aika";
		Cells[0][0][cBdg].text = L"Badge";
		Cells[0][0][cKno].text = L"Kilpno";
		Cells[0][0][cOs].text = L"Os";
		Cells[0][0][cPst].text = L"Pst";
		Cells[0][0][cJk].text = L"Joukkue";
		Cells[0][0][cNimi].text = L"Nimi";
		Cells[0][0][cSrj].text = L"Sarja";
		Cells[0][0][cTls].text = L"Tulos";
		Cells[0][0][cEro].text = L"Ero";
		Cells[0][0][cTrk].text = L"Trk";
		}
	for (r = 1; r < DGajat1->RowCount; r++) {
		naytaRivi(0, r, r-1, Jono);
		}
	tallVertRivi();
	DGajat1->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::naytaAjat2(void)
{
	int r, r2, n;

	r2 = aikajono[Jono]->aktrows;
	n = DGajat2->RowCount;
	if (n > r2)
		n = r2;
	if (CellRows[1] < DGajat2->RowCount) {
		CellRows[1] = DGajat2->RowCount + 200;
		if (Cells[1] != NULL)
			delete[] Cells[1];
		Cells[1] = new rowTp[CellRows[1]];
		}
	for (r = 0; r < DGajat2->RowCount; r++) {
		if (r < n) {
			naytaRivi(1, r, r2 - n + r, Jono);
			}
		else {
			for (int c = 0; c < DGajat2->ColCount; c++) {
				Cells[1][r][c].style = 0;
				Cells[1][r][c].text = L"";
				}
			}
		}
	DGajat2->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::naytaTCPstatus(void)
{
	if (aikaTCPstatus == 0) {
		MemoTCP->Visible = false;
		return;
		}
	MemoTCP->Visible = true;
	if (aikaTCPstatus > 0) {
		MemoTCP->Lines->Strings[3] = L"ON";
		MemoTCP->Color = clLime;
		}
	else {
		MemoTCP->Lines->Strings[3] = L"POIKKI";
		MemoTCP->Color = clRed;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::BtnPaivitaClick(TObject *Sender)
{
	naytaAjat1();
	if (DGajat1->RowCount > aktRivi1)
		DGajat1->Row = aktRivi1;
	else
		aktRivi1 = DGajat1->RowCount - 1;
	DGajat1->Row = aktRivi1;
	naytaAjat2();
	naytaAktAika();
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::RGNaytaClick(TObject *Sender)
{
	FormResize(Sender);
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::naytaAktAika(void)
{
	int r;
	wchar_t as[60], pisteet[] = L"?ALM";
	aikatp pt;

	if (aktRivi1 <= 0 || aktRivi1 >= DGajat1->RowCount)
		return;
	r = _wtoi(Cells[0][aktRivi1][cRivi].text.c_str());
	if (r < 1)
		return;
	aikajono[Jono]->getAktTime(&pt, r-1);
	AIKATOWSTRS(as, purajak(pt.t), t0);
//	as[11] = (pt.t+86400*AIKAJAK*SEK) / (AIKAJAK*SEK/1000) % 10 + L'0';
//	as[12] = 0;
	EdRivi->Text = UnicodeString(r);
	EdDate->Text = time_t_date(pt.date, NULL);
	EdAika->Text = as;
	EdKno->Text = UnicodeString(pt.kno);
	EdOsuus->Text = pt.osuus+1;
	if (pt.piste < 1)
		EdPiste->Text = UnicodeString(pisteet[pt.piste+3]);
	else
		EdPiste->Text = UnicodeString(pt.piste);
	EdBadge->Text = UnicodeString(pt.badge);
	EdPortti->Text = UnicodeString((int)pt.kanava);
	EdLahde->Text = UnicodeString(pt.lahde);
	EdJono->Text = UnicodeString(pt.jono);
	tallVertRivi();
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::DGajat1SelectCell(TObject *Sender, int ACol, int ARow,
		  bool &CanSelect)
{
	if (ARow < 1 || ARow >= DGajat1->RowCount)
		return;
	if (ARow != aktRivi1)
		aikajono[Jono]->getAktTime(&eAika, ARow-1);
	aktRivi1 = ARow;
//	naytaAjat1();
	naytaAktAika();
}
//---------------------------------------------------------------------------

void TFormAjanotto::Paivita_Aikanaytto(void)
{
	naytaTCPstatus();
	if (OnLine) {
		naytaAjat1();
		naytaAjat2();
		naytaAktAika();
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::lisaaAika(void)
{
	int t;
	aikatp vpt, upt;

	if (aktRivi1 < 1 || aktRivi1 >= DGajat1->RowCount)
		aktRivi1 = 1;
	aikajono[Jono]->getAktTime(&vpt, aktRivi1-1);
	t = AIKAJAK * Nyt();
	lisaa_aika(Jono, t, 0, AIKAJAK*TMAALI0, 0);
	aikajono[Jono]->getAktTime(&upt, aktRivi1-1);
	if (memcmp(&vpt, &upt, sizeof(aikatp)))
		eAika = upt;
	DGajat1->Row = aktRivi1;
	naytaAjat1();
	naytaAjat2();
	naytaAktAika();
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::BtnLuoUusiClick(TObject *Sender)
{
	aikatp pt, vpt, upt;
	int r, t, dt;

	if (aktRivi1 < 1 || aktRivi1 >= DGajat1->RowCount)
		aktRivi1 = 1;
	r = aktRivi1-2;
	if (r < 0)
		r = 0;
	aikajono[Jono]->getAktTime(&vpt, r);
	if (vpt.t/AIKAJAK != TMAALI0) {
		if (aktRivi1 == 1)
			t = NORMKELLO_A(vpt.t - (IkkParam.AikaLisays > 1 ? IkkParam.AikaLisays : 10));
		else {
			t = NORMKELLO_A(vpt.t + IkkParam.AikaLisays);
			while (r+1 < aikajono[Jono]->aktrows) {
				aikajono[Jono]->getAktTime(&pt, r+1);
				if (NORMKELLO_A(pt.t-t) > 0)
					break;
				t += IkkParam.AikaLisays;
				r++;
				}
			}
		}
	else {
		t = AIKAJAK * Nyt();
		}
	lisaa_aika(Jono, t, 0, vpt.t, vpt.date);
	aikajono[Jono]->getAktTime(&upt, aktRivi1-1);
	if (memcmp(&vpt, &upt, sizeof(aikatp)))
		eAika = upt;
	DGajat1->Row = aktRivi1;
	naytaAjat1();
	naytaAjat2();
	naytaAktAika();
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::Button2Click(TObject *Sender)
{
	aikatp vpt, upt;
	wchar_t cp, *vp = L"Vahvista tallennus";
	int r, t, uptr;
	kilptietue kilp;
	int d;

	if (aktRivi1 < 1)
		return;
	aikajono[Jono]->getAktTime(&vpt, aktRivi1-1);
	upt = vpt;
	upt.t = wstrtoaika_vap(EdAika->Text.c_str(), t0) * AIKAJAK;
	upt.date = date_time_t(EdDate->Text.c_str());
	upt.jono = Jono;
	upt.kno = _wtoi(EdKno->Text.c_str());
	upt.osuus = _wtoi(EdOsuus->Text.c_str()) - 1;
	wcsncpy(&cp, EdPiste->Text.c_str(), 1);
	cp = towupper(cp);
	if (cp == L'A')
		upt.piste = -1;
	if (cp == L'M')
		upt.piste = 0;
	else
		upt.piste = _wtoi(EdPiste->Text.c_str());
	if (upt.kno && (vpt.kno != upt.kno || vpt.osuus != upt.osuus || vpt.piste != upt.piste) &&
		(d = getpos(upt.kno)) > 0) {
		kilp.getrec(d);
		if (kilp.Maali(upt.osuus, upt.piste) != TMAALI0) {
			int vast = select3(3, L"Kilpailijalla jo aika, valitse jatko", L"Valitse", L"Peruuta muutos",
				L"Tallenna numero, mutta ei tulosta", L"Tallenna numero ja uusi tulos", this);
			if (vast == 1)
				return;
			if (vast == 2)
				upt.piste = Sarjat[kilp.sarja].valuku[upt.osuus] + 1;
			}
		}
	else
		if (Application->MessageBoxW(L"Vahvista tallennus", L"Vahvistus", MB_YESNO) != IDYES)
			return;
	tall_rivi(Jono, &upt, &vpt, &uptr, 0, 0, false);
	eAika = upt;
	naytaAjat1();
	naytaAjat2();
	naytaAktAika();
	FocusControl(Button6);
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::Button4Click(TObject *Sender)
{
	naytaAktAika();
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::Button3Click(TObject *Sender)
{
	aikatp vpt, upt;

	if (aktRivi1 > aikajono[Jono]->aktrows)
		return;
	if (aktRivi1 < 1)
		return;
	if (Application->MessageBoxW(L"Vahvista poisto", L"Vahvistus", MB_YESNO) != IDYES)
		return;
	aikajono[Jono]->getAktTime(&vpt, aktRivi1-1);
	poista_aika(Jono, &vpt);
	if (aktRivi1 > aikajono[Jono]->aktrows)
		aktRivi1 = aikajono[Jono]->aktrows;
	DGajat1->Row = aktRivi1;
	aikajono[Jono]->getAktTime(&upt, aktRivi1-1);
	eAika = upt;
	naytaAjat1();
	naytaAjat2();
	naytaAktAika();
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::EnterPressed(TObject *Sender, System::WideChar &Key)
{
	if (AikaKey || Key < 32 && Key != 8)
		Key = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::Aktivoi(int rivi, int status)
{
	aikatp pt, upt;

	if (rivi < 1 || rivi >= DGajat1->RowCount)
		return;
	aikajono[Jono]->getAktTime(&pt, rivi-1);
	upt = pt;
	upt.status = status;
	tall_rivi(Jono, &upt, &pt, 0, 0, 0, 0);
	naytaAjat1();
	naytaAjat2();
	if (DGajat1->Row < DGajat1->RowCount-1)
		DGajat1->Row++;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::EdAikaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN) {
		if (!Shift.Contains(ssShift) &&
			!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt))
			EdKno->SetFocus();
		else if (!Shift.Contains(ssShift) &&
			 Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt))
			 Button2Click(Sender);
		Key = 0;
		}
	if (Key != 0 && Key == IkkParam.KeyCode && Shift == IkkParam.KeyShift) {
		AikaKey = true;
		Key = 0;
		lisaaAika();
		}
	else
		AikaKey = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::EdKnoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN) {
		if (!Shift.Contains(ssShift) &&
			!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt))
			EdPiste->SetFocus();
		else if (!Shift.Contains(ssShift) &&
			 Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt))
			 Button2Click(Sender);
		Key = 0;
		}
	if (Key != 0 && Key == IkkParam.KeyCode && Shift == IkkParam.KeyShift) {
		AikaKey = true;
		Key = 0;
		lisaaAika();
		}
	else
		AikaKey = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::Button6Click(TObject *Sender)
{
	if (aktRivi1 < aikajono[Jono]->aktrows) {
		aktRivi1++;
		naytaAktAika();
		}
	DGajat1->Row = aktRivi1;
	EdAika->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::EdPisteKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN) {
		if (!Shift.Contains(ssShift) &&
			!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt))
			FocusControl(Button2);
		else if (!Shift.Contains(ssShift) &&
			 Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt))
			 Button2Click(Sender);
		Key = 0;
		}
	if (Key != 0 && Key == IkkParam.KeyCode && Shift == IkkParam.KeyShift) {
		AikaKey = true;
		Key = 0;
		lisaaAika();
		}
	else
		AikaKey = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::CBJonoChange(TObject *Sender)
{
	if (CBJono->ItemIndex < 0) {
		CBJono->ItemIndex = Jono;
		return;
		}
	Jono = CBJono->ItemIndex;
	if (aktRivi1 > aikajono[Jono]->aktrows)
		aktRivi1 = aikajono[Jono]->aktrows;
	aikajono[Jono]->getAktTime(&eAika, aktRivi1);
	DGajat1->Row = aktRivi1;
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Ajanottokaavake");
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::DGajat1MouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (DGajat1->RowCount - DGajat1->FixedRows >= DGajat1->VisibleRowCount + DGajat1->TopRow)
		DGajat1->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::DGajat1MouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (DGajat1->TopRow > DGajat1->FixedRows)
		DGajat1->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::FormShow(TObject *Sender)
{
	for (int i = 0; i < NAJANOTOT; i++) {
		if (this == FormAjanotot[i]) {
			Ajanotto = i+1;
			Caption = L"Ajanotto - " + UnicodeString(Ajanotto);
			break;
			}
		}
	if (aktRivi1 > aikajono[Jono]->aktrows)
		aktRivi1 = aikajono[Jono]->aktrows;
	DGajat1->Row = aktRivi1;
	aikajono[Jono]->getAktTime(&eAika, aktRivi1);
	FormResize(Sender);
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::SarjaLahto(int row, wchar_t val)
{
	aikatp pt, upt;
	int r, uptr, vast = 2;

	if (row < 1 || (r = _wtoi(Cells[0][row][cRivi].text.c_str())) == 0)
		return;
	if (val == L'S') {
		vast = select3(3, L"L‰htˆajat voidaan antaa joko sarjan l‰htˆaikana tai "
			L"sarjan kaikkien joukkueiden 1. osuuden henkilˆkohtaisina l‰htˆaikoina",
			L"Valitse menettely", L"Aseta sarjan l‰htˆaika",
			L"Aseta yksilˆlliset l‰htˆajat", L"Peruuta", this);
		if (vast == 3)
			return;
		}
	aikajono[Jono]->getAktTime(&pt, r-1);
	if (pt.t == AIKAJAK*TMAALI0)
		return;
	upt = pt;
	if (val == L'S') {
		FormSarjaVal = new TFormSarjaVal(this);
		FormSarjaVal->ShowModal();
		if (FormSarjaVal->ModalResult == mrOk) {
			if (vast == 2) {
				asetaSrjLahto(FormSarjaVal->Sarja, purajak(pt.t), false);
				}
			else {
				Sarjat[FormSarjaVal->Sarja].lahto = purajak(pt.t);
				kirjEventXml(sarjaxmlfname, true, false, false);
				laheta_yl(-FormSarjaVal->Sarja-1, 0);
				}
			upt.kno = 30001+FormSarjaVal->Sarja;
			upt.piste = -1;
			tall_rivi(Jono, &upt, &pt, &uptr, 0, 0, 0);
			}
		delete FormSarjaVal;
		}
	if (val == L'R') {
		int era = 0;
		wchar_t ch;
		inputint_prompt(&era, L"Anna er‰n numero", &ch, this);
		if (era >= 0 && ch != ESC) {
			asetaEraLahto(era, purajak(pt.t));
			upt.kno = 31000+era;
			upt.piste = -1;
			tall_rivi(Jono, &upt, &pt, &uptr, 0, 0, 0);
			}
		}
	eAika = upt;
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::emiTagohjaus1Click(TObject *Sender)
{
	if (!FormEmiTag)
		FormEmiTag = new TFormEmiTag(this);
	FormEmiTag->Show();
	if (FormEmiTag->WindowState == wsMinimized)
		FormEmiTag->WindowState = wsNormal;
	FormEmiTag->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::RGSyottoClick(TObject *Sender)
{
	if (RGSyotto->ItemIndex == 3 && emitfl == 0) {
		Application->MessageBoxW(L"Tunnistinkoodit eiv‰t ole k‰ytˆss‰", L"Este", MB_OK);
		RGSyotto->ItemIndex = IkkParam.SyottoTila;
		return;
		}
	IkkParam.SyottoTila = RGSyotto->ItemIndex;
	IkkParam.ColOn[cBdg] = (IkkParam.SyottoTila == 3 ? 1 : 0);
	DGajat1->ColWidths[cBdg] = IkkParam.ColOn[cBdg] * IkkParam.ColW[cBdg] * Screen->PixelsPerInch / 96;
	if (DGajat2->ColWidths[cBdg] != DGajat1->ColWidths[cBdg]) {
		Width += DGajat1->ColWidths[cBdg] - DGajat2->ColWidths[cBdg];
		DGajat2->ColWidths[cBdg] = DGajat1->ColWidths[cBdg];
		}
	switch (RGSyotto->ItemIndex) {
		case 1:
		case 2:
		case 3:
			if (RGNayta->ItemIndex == 1) {
				RGNayta->ItemIndex = 2;
				FormResize(Sender);
				}
			DGajat1->Options << goEditing;
			break;
		default:
			DGajat1->Options >> goEditing;
			break;
		}
}
//---------------------------------------------------------------------------

void TFormAjanotto::uusintaTallennus(int rivi)
{
	aikatp pt;
	kilptietue kilp;
	int d;

	if (DGajat1->Row < 1 || DGajat1->Row >= DGajat1->RowCount)
		return;
	aikajono[Jono]->getAktTime(&pt, rivi-1);
	if (pt.kno && pt.t != AIKAJAK*TMAALI0 && pt.piste >=0 && (d = getpos(pt.kno)) > 0) {
		EnterCriticalSection(&tall_CriticalSection);
		kilp.getrec(d);
		if (pt.piste >= 0 && pt.osuus < Sarjat[kilp.sarja].osuusluku && pt.piste <= Sarjat[kilp.sarja].valuku[pt.osuus]) {
			kilp.setMaali(pt.osuus, pt.piste, pyoristatls(purajak(pt.t), 1));
			tallenna(&kilp,d, 0, 0, 1, 0);
			}
		LeaveCriticalSection(&tall_CriticalSection);
		}
	if (DGajat1->Row < DGajat1->RowCount-1)
		DGajat1->Row++;
}
//---------------------------------------------------------------------------
static int haeKnoOs(int badge, int &kno, int &osuus)
{
	kilptietue kilp;
	int d;

	if (badge == 0)
		return(1);
	if ((kno = bdg2kno(badge, 0)) == 0) {
		Application->MessageBoxW(L"Tuntematon koodi", L"", MB_OK);
		return(1);
		}
	if ((d = getpos(kno)) > 0) {
		int os1 = -1;
		kilp.getrec(d);
		osuus = -1;
		for (int os = Sarjat[kilp.sarja].osuusluku-1; os >= 0; os--) {
			if (val_korkein) {
			   if (kilp.ostiet[os].badge[0] == badge ||
				  (kilpparam.kaksibadge && kilp.ostiet[os].badge[1] == badge)) {
				  osuus = os;
				  break;
				  }
			   }
			else if ((kilp.ostiet[os].badge[0] == badge ||
				  (kilpparam.kaksibadge && kilp.ostiet[os].badge[1] == badge) ||
				  (joustoviesti && kilp.ostiet[os].badge[0] == 0)) &&
			   (emitfl < 0 || EmitJarr(kilp.kilpno, os) < 0)) {
			   if (os1 < 0)
				  os1 = os;
			   if (kilp.Maali(os, 0) == TMAALI0)
				  osuus = os;
			   }
			}
		if (osuus < 0)
			osuus = os1;
		}
	return(kno < minkilpno || osuus < 0);
}

void __fastcall TFormAjanotto::DGajat1KeyPress(TObject *Sender, System::WideChar &Key)

{
	int r, uPst, uKno, uOs, srj;
	wchar_t as[60], pisteet[] = L"0ALM";
	int tallFl = 0;
	aikatp pt, upt;
	int t, uptr;
	kilptietue kilp;
	int d, badge = 0, pakLah = 0;
	int vaadiVahvistus;
	bool paivFl = false;

	if (AikaKey || IkkParam.SyottoTila == 0 || aktRivi1 < 1) {
		Key = 0;
		return;
		}
	vaadiVahvistus = Vaaditallennuksillevahvistus1->Checked ? 1 : 0;
	if (Vaadivahvistus2->Checked)
		vaadiVahvistus = 2;
	aikajono[Jono]->getAktTime(&pt, aktRivi1-1);
	upt = pt;

	if (DGajat1->Col == cKno && wcswcind(towupper(Key), L"RS") >= 0) {
		SarjaLahto(DGajat1->Row, towupper(Key));
		return;
		}
	if (Key == ESC) {
		Key = 0;
		switch (DGajat1->Col) {
			case cAika :
				Cells[0][DGajat1->Row][DGajat1->Col].text = VertRivi[0];
				break;
			case cBdg :
				Cells[0][DGajat1->Row][DGajat1->Col].text = VertRivi[1];
				break;
			case cKno :
				Cells[0][DGajat1->Row][DGajat1->Col].text = VertRivi[2];
				break;
			case cOs :
				Cells[0][DGajat1->Row][DGajat1->Col].text = VertRivi[3];
				break;
			case cPst :
				Cells[0][DGajat1->Row][DGajat1->Col].text = VertRivi[4];
				break;
			}
		DGajat1->SetFocus();
		return;
		}
	if (Pakota || Key == L'\r' || Key == L'\n') {
		if (IntInList(IkkParam.SyottoTila, L"1 2 3") >= 0 && DGajat1->Col == cKno && CBOsVal->ItemIndex > 0) {
			if (CBOsVal->ItemIndex == 1) {
				uOs = kilpparam.osuusluku - 1;
				if ((uKno = _wtoi(Cells[0][DGajat1->Row][cKno].text.c_str())) > 0 &&
					(d = getpos(uKno)) > 0) {
					kilptietue kilp1;
					kilp1.getrec(d);
					uOs = Sarjat[kilp1.sarja].osuusluku - 1;
					}
				}
			else
				uOs = CBOsVal->ItemIndex - 2;
			Cells[0][DGajat1->Row][cOs].text = uOs+1;
			}
		if ((IkkParam.SyottoTila == 1 && DGajat1->Col != cKno) ||
			(IkkParam.SyottoTila == 2 && DGajat1->Col != cPst) ||
			(IkkParam.SyottoTila == 3 && DGajat1->Col != cBdg)) {
			if (IkkParam.SyottoTila == 2 && DGajat1->Col >= cKno && DGajat1->Col < cPst)
				DGajat1->Col++;
			else {
				DGajat1->Col = IkkParam.SyottoTila == 3 ? cBdg : cKno;
				if (DGajat1->Row < DGajat1->RowCount - 1) {
					DGajat1->Row++;
					if (_wtoi(Cells[0][DGajat1->Row][cRivi].text.c_str()) == 0)
					   Cells[0][DGajat1->Row][cRivi].text =
						  _wtoi(Cells[0][DGajat1->Row-1][cRivi].text.c_str()) + 1;
					}
				}
			return;
			}
		r = _wtoi(Cells[0][DGajat1->Row][cRivi].text.c_str());
		if (IkkParam.SyottoTila == 3) {
			badge = _wtoi(Cells[0][DGajat1->Row][cBdg].text.c_str());

			if (haeKnoOs(badge, uKno, uOs) != 0) {
				Key = 0;
				return;
				}
			Cells[0][DGajat1->Row][cKno].text = uKno;
			Cells[0][DGajat1->Row][cOs].text = wosuuskoodi(kilp.sarja, uOs, as, 20);
			}
		else
			uKno = _wtoi(Cells[0][DGajat1->Row][cKno].text.c_str());
		if (r > aikajono[Jono]->aktrows) {
			if (IkkParam.Ennakoi && DGajat1->Col == cKno && uKno > 0) {
				Memo1->Lines->Add(uKno);
				}
			Cells[0][DGajat1->Row][cKno].text = L"0";
			Key = 0;
			return;
			}
		uOs = tulkOsuuskoodi(srj, Cells[0][DGajat1->Row][cOs].text.c_str());
		if (uOs < 0)
			uOs = 0;
		if (uKno > 0 && (d = getpos(uKno)) > 0) {
			kilp.getrec(d);
			srj = kilp.sarja;
			if (DGajat1->Col == cKno) {
				uOs = CBOsVal->ItemIndex - 2;
				if (uOs == -1)
					uOs = Sarjat[srj].osuusluku - 1;
				else if (uOs < -1) {
					uOs = haeseurosuus(&kilp);
					if (IkkParam.SyottoTila == 1 && uOs < 0) {
						uOs = Sarjat[srj].osuusluku - 1;
						uPst = Sarjat[srj].valuku[uOs]+1;
						}
					}
				}
			}
		else {
			srj = -1;
			}
		aikajono[Jono]->getAktTime(&pt, r-1);
		upt = pt;
		if (DGajat1->Col < cPst) {
			upt.osuus = uOs;
			}
		if (DGajat1->Col < cPst) {
			if (uKno > 0 && upt.piste == -2)
				upt.piste = va_mpiste[Jono];
			if (uKno > 0 && upt.piste == -2)
				upt.piste = haeseurpiste(&kilp, uOs);
			}
		if (Pakota || uKno != pt.kno) {
			upt.kno = uKno;
			if ((IkkParam.SyottoTila == 1 && DGajat1->Col == cKno) || (IkkParam.SyottoTila == 3 && DGajat1->Col == cBdg)) {
				tallFl = 2;
				}
			}
		if (IkkParam.SyottoTila == 3)
			upt.badge = badge;
		if (IkkParam.SyottoTila == 2 && DGajat1->Col == cPst && Cells[0][DGajat1->Row][cPst].text.Length()>0) {
			uPst = _wtoi(Cells[0][DGajat1->Row][cPst].text.c_str());
			if (uPst == 0)
				uPst = wcswcind(Cells[0][DGajat1->Row][cPst].text.UpperCase().c_str()[0], pisteet) - 3;
			if (uPst <= -2)
				uPst = 0;
			upt.kno = uKno;
			upt.osuus = uOs;
			upt.piste = uPst;
			if (uPst >= -2 && (uPst != pt.piste || uKno != pt.kno || uOs != pt.osuus)) {
				tallFl = 2;
				}
			}
		Key = 0;
		if (Pakota) {
			tallFl = 2;
			if (!memcmp(&pt, &upt, sizeof(aikatp)))
				pakLah = 1;
			}
		if (!tallFl && upt.kno && (d = getpos(upt.kno)) > 0) {
			kilp.getrec(d);
			if (upt.osuus >= 0 && upt.osuus < Sarjat[kilp.sarja].osuusluku &&
				upt.piste > -2 && upt.piste <= Sarjat[kilp.sarja].valuku[upt.osuus] &&
				(purajak(upt.t) - kilp.Maali(upt.osuus, upt.piste) / kilpparam.pyor[2]))
				tallFl = 2;
			}
		}
	if (tallFl) {
		paivFl = true;
		if (upt.kno && (Pakota || pt.kno != upt.kno || pt.osuus != upt.osuus || pt.piste != upt.piste) &&
			(d = getpos(upt.kno)) > 0) {
			int tl1;
			kilp.getrec(d);
			if (upt.piste > Sarjat[kilp.sarja].valuku[upt.osuus])
				tallFl = 1;
			else if ((tl1 = kilp.Maali(upt.osuus, upt.piste)) != TMAALI0) {
				if (upt.piste > -1) {
					wchar_t as[100];
					int ero;
					ero = purajak(upt.t) - tl1;
					if (IkkParam.SyottoTila != 2) {
						Cells[0][DGajat1->Row][cOs].text = wosuuskoodi(kilp.sarja, upt.osuus, as, 10);
						if (pt.piste < 1)
							Cells[0][DGajat1->Row][cPst].text = UnicodeString(pisteet[upt.piste+3]);
						else
							Cells[0][DGajat1->Row][cPst].text = UnicodeString(upt.piste);
						Cells[0][DGajat1->Row][cOs].style = 5;
						Cells[0][DGajat1->Row][cPst].style = 5;
						}
					Cells[0][DGajat1->Row][cJk].text = kilp.Joukkue(as, 35);
					Cells[0][DGajat1->Row][cNimi].text = kilp.Nimi(as, 20, upt.osuus, 1);
					Cells[0][DGajat1->Row][cSrj].text = Sarjat[kilp.sarja].sarjanimi;
					elimwzb1(aikatowstr_cols_n(as, kilp.Tulos(upt.osuus, upt.piste), 0, 0, kilpparam.laikasj));
					Cells[0][DGajat1->Row][cTls].text = as;
					if (ero/kilpparam.pyor[2])
						Cells[0][DGajat1->Row][cEro].text = eroaikatowstr(as, ero, kilpparam.laikasj);
					else
						Cells[0][DGajat1->Row][cTrk].text = L"";
					Cells[0][DGajat1->Row][cJk].style = 5;
					Cells[0][DGajat1->Row][cNimi].style = 5;
					Cells[0][DGajat1->Row][cSrj].style = 5;
					Cells[0][DGajat1->Row][cTls].style = 5;
					Cells[0][DGajat1->Row][cEro].style = 5;
					DGajat1->Refresh();
					}

				int vast = select3(3, L"Kilpailijalla jo rivill‰ n‰ytetty tulos, valitse jatko", L"Valitse", L"Peruuta muutos",
					L"Tallenna numero, mutta ei tulosta", L"Tallenna numero ja uusi tulos", this);
				if (vast == 1) {
					DGajat1->Col = cKno;
					tallFl = 0;
					}
				if (vast == 2)
					upt.piste = Sarjat[kilp.sarja].valuku[upt.osuus] + 1;
				vaadiVahvistus = 0;
				}
			}
		if (tallFl && vaadiVahvistus == 1 || (vaadiVahvistus == 2 && pt.kno)) {
			if (Application->MessageBoxW(L"Vahvista tallennus", L"Vahvistus", MB_YESNO) != IDYES) {
				tallFl = 0;
				}
			}
		}
	if (tallFl)
		tall_rivi(Jono, &upt, &pt, &uptr, 0, pakLah, false);
	if (tallFl == 2) {
		d = -1;
		if (pt.kno || upt.kno) {
			EnterCriticalSection(&tall_CriticalSection);
			if (pt.kno) {
				if ((d = getpos(pt.kno)) > 0) {
					kilp.getrec(d);
					if (kilp.kilpstatus == 0 &&
						abs((int) NORMKELLO(kilp.Maali(pt.osuus, pt.piste) - purajak(pt.t))) < kilpparam.pyor[1]) {
						kilp.setMaali(pt.osuus, pt.piste, TMAALI0);
						kilp.Tallenna(d, 0, 0, 0, 0);
						d = -1;
						}
					}
				}
			if (upt.kno) {
//				if (d < 1) {
				if (d > -10) {
					if ((d = getpos(upt.kno)) > 0)
						kilp.getrec(d);
					if (d > 0 && kilp.kilpstatus == 0) {
						kilp.setMaali(upt.osuus, upt.piste, pyoristatls(purajak(upt.t), 1));
						kilp.Tallenna(d, 0, 0, pakLah, 0);
						}
					}
				}
			}
		if (DGajat1->Row < DGajat1->RowCount-1) {
			DGajat1->Row++;
			}
		DGajat1->Col = IkkParam.SyottoTila == 3 ? cBdg : cKno;
		LeaveCriticalSection(&tall_CriticalSection);
		}
/*
	else if (kilp.Maali(upt.osuus, upt.piste) != wstrtoaika_vap(Cells[0][DGajat1->Row][8].text.c_str(), t0)) {
		if ((d = getpos(upt.kno)) > 0) {
			kilp.getrec(d);
			if (kilp.kilpstatus == 0) {
				EnterCriticalSection(&tall_CriticalSection);
				kilp.setMaali(upt.osuus, upt.piste, pyoristatls(purajak(upt.t), 1));
				kilp.Tallenna(d, 0, 0, 0, 0);
				LeaveCriticalSection(&tall_CriticalSection);
				}
			}
		}
*/
	if ((Key < L' ' || Key > L'z') && (Key != 0 || paivFl) && Key != 8) {
		naytaAjat1();
		naytaAjat2();
		naytaAktAika();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Ajanottonppin1Click(TObject *Sender)
{
	FormNappain = new TFormNappain(this);
	FormNappain->KeyCode = IkkParam.KeyCode;
	FormNappain->KeyShift = IkkParam.KeyShift;
	FormNappain->ShowModal();
	IkkParam.KeyCode = FormNappain->KeyCode;
	IkkParam.KeyShift = FormNappain->KeyShift;
	delete FormNappain;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::DGajat1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	Pakota = false;
	if (Key == VK_RETURN && Shift.Contains(ssCtrl)) {
		Pakota = true;
//		Key = 0;
		}
	else if (Key != 0 && Key == IkkParam.KeyCode && Shift == IkkParam.KeyShift) {
		AikaKey = true;
		Key = 0;
		lisaaAika();
		}
	else if (Key == VK_F11 && !Shift.Contains(ssShift) &&
		!Shift.Contains(ssAlt) && !Shift.Contains(ssCtrl)) {
		AikaKey = true;
		Key = 0;
		BtnLuoUusiClick(Sender);
		}
	else if (Key == L'U' && !Shift.Contains(ssShift) &&
		!Shift.Contains(ssAlt) && Shift.Contains(ssCtrl)) {
		uusintaTallennus(DGajat1->Row);
		Key = 0;
		}
	else if (Key == L'T' && !Shift.Contains(ssShift) &&
		!Shift.Contains(ssAlt) && Shift.Contains(ssCtrl)) {
		Aktivoi(DGajat1->Row, 0);
		Key = 0;
		}
	else if (Key == L'O' && !Shift.Contains(ssShift) &&
		!Shift.Contains(ssAlt) && Shift.Contains(ssCtrl)) {
		Aktivoi(DGajat1->Row, 1);
		Key = 0;
		}
	else
		AikaKey = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Suljekaavake1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Vaaditallennuksillevahvistus1Click(TObject *Sender)
{
	Vaaditallennuksillevahvistus1->Checked = !Vaaditallennuksillevahvistus1->Checked;
	Vaadivahvistus2->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Vaadivahvistus2Click(TObject *Sender)
{
	Vaadivahvistus2->Checked = !Vaadivahvistus2->Checked;
	Vaaditallennuksillevahvistus1->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Kaavakkeenoletuspiste1Click(TObject *Sender)
{
	wchar_t st[20] = L"A", ch = L' ';
	if (va_mpiste[Jono] >= -2 && va_mpiste[Jono] < 1)
		st[0] = L"ALM"[va_mpiste[Jono]+2];
	else
		_itow(va_mpiste[Jono], st, 10);
	inputstr_prompt(st, 3, L"Anna pisteen tunnus M, L tai A tai v‰liaikapisteen numero", &ch, this);
	if (ch != ESC) {
		elimwbl(st);
		switch(towupper(st[0])) {
			case L'A':
				va_mpiste[Jono] = -2;
				break;
			case L'L':
				va_mpiste[Jono] = -1;
				break;
			case L'M':
				va_mpiste[Jono] = 0;
				break;
			default:
				va_mpiste[Jono] = _wtoi(st);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Salliennakoivanumerosytt1Click(TObject *Sender)
{
	Salliennakoivanumerosytt1->Checked = !Salliennakoivanumerosytt1->Checked;
	IkkParam.Ennakoi = Salliennakoivanumerosytt1->Checked;
	Panel4->Visible = IkkParam.Ennakoi;
	Panel3->Visible = Panel4->Visible || aikaTCPstatus != 0;
	if (!IkkParam.Ennakoi)
		Memo1->Clear();
	if (!Panel3->Visible && ClientWidth >= Panel2->Left + Panel2->Width + Panel3->Width-1)
		ClientWidth = Panel2->Left + Panel2->Width;
	if (Panel3->Visible && ClientWidth < Panel2->Left + Panel2->Width + Panel3->Width)
		ClientWidth = Panel2->Left + Panel2->Width + Panel3->Width;
	FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Kirjoitaajattekstitiedostoon1Click(TObject *Sender)

{
	static wchar_t fname[200] = L"ajat.txt";

	SaveDialog1->FileName = fname;
	SaveDialog1->DefaultExt = L"txt";
	SaveDialog1->Filter = L"Tekstitiedostot (*.txt)|*.txt|Kaikki tiedostot (*.*)|*.*";
	if (SaveDialog1->Execute()) {
		wcsncpy(fname, SaveDialog1->FileName.c_str(), sizeof(fname)/2-1);
		AjatTiedostoon(fname, Jono);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::SynkronoiSirit1Click(TObject *Sender)
{
	SiritSync(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Button8Click(TObject *Sender)
{
	Memo1->Clear();
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::BtnHakuClick(TObject *Sender)
{
	int alkuRivi, kno;

	LblEiLoydy->Visible = false;
	kno = _wtoi(EdHaku->Text.c_str());
	if (aikajono[Jono]->aktrows == 0 || kno < minkilpno || kno > maxkilpno)
		return;
	alkuRivi = aktRivi1;
	if (alkuRivi < 1 || alkuRivi > aikajono[Jono]->aktrows)
		alkuRivi = 1;
	for (int rv = alkuRivi + 1; rv != alkuRivi; rv++) {
		aikatp pt;
		if (rv > aikajono[Jono]->aktrows)
			rv = 0;
		aikajono[Jono]->getAktTime(&pt, rv-1);
		if (pt.kno == kno) {
			DGajat1->Row = rv;
			aktRivi1 = rv;
			return;
			}
		}
	LblEiLoydy->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::HaeMySQLkannasta1Click(TObject *Sender)
{
	if (FormEmitSQL == NULL)
		FormEmitSQL = new TFormEmitSQL(this);
	FormEmitSQL->Show();
	if (FormEmitSQL->WindowState == wsMinimized)
		FormEmitSQL->WindowState = wsNormal;
	FormEmitSQL->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::KorjaakelloClick(TObject *Sender)
{
	if (FormAikakorjaus)
		return;
	FormAikakorjaus = new TFormAikakorjaus(this);
	FormAikakorjaus->Jono = CBJono->ItemIndex;
	FormAikakorjaus->ShowModal();
	delete FormAikakorjaus;
	FormAikakorjaus = NULL;
	naytaAjat1();
	naytaAjat2();
	naytaAktAika();
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Ajanottojonojenvertailu1Click(TObject *Sender)
{
	if (!FormAikavertailut[Ajanotto-1])
		FormAikavertailut[Ajanotto-1] = new TFormAikavert(this);
	FormAikavertailut[Ajanotto-1]->Ajanotto = Ajanotto;
	FormAikavertailut[Ajanotto-1]->OnLine = OnLine;
	FormAikavertailut[Ajanotto-1]->Show();
	if (FormAikavertailut[Ajanotto-1]->WindowState == wsMinimized)
		FormAikavertailut[Ajanotto-1]->WindowState = wsNormal;
	FormAikavertailut[Ajanotto-1]->BringToFront();
}
//---------------------------------------------------------------------------


void __fastcall TFormAjanotto::EdDateKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN) {
		if (!Shift.Contains(ssShift) &&
			!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt))
			EdAika->SetFocus();
		else if (!Shift.Contains(ssShift) &&
			 Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt))
			 Button2Click(Sender);
		Key = 0;
		}
	if (Key != 0 && Key == IkkParam.KeyCode && Shift == IkkParam.KeyShift) {
		AikaKey = true;
		Key = 0;
		lisaaAika();
		}
	else
		AikaKey = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Memo1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key != 0 && Key == IkkParam.KeyCode && Shift == IkkParam.KeyShift) {
		AikaKey = true;
		Key = 0;
		lisaaAika();
		}
	else
		AikaKey = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Memo1KeyPress(TObject *Sender, System::WideChar &Key)

{
	if (AikaKey)
		Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::EdHakuKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key != 0 && Key == IkkParam.KeyCode && Shift == IkkParam.KeyShift) {
		AikaKey = true;
		Key = 0;
		lisaaAika();
		}
	else
		AikaKey = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::EdHakuKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (AikaKey)
		Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::DGajat2KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key != 0 && Key == IkkParam.KeyCode && Shift == IkkParam.KeyShift) {
		Key = 0;
		lisaaAika();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Lueaikojatekstitiedostosta1Click(TObject *Sender)
{
	static wchar_t fname[200] = L"ajat.txt";
	int vast;

	vast = Application->MessageBoxW(L"Luetaanko myˆs kilpailijan numero, osuus ja piste (Kyll‰), vai vain ajat (Ei)",
		L"Valitse", MB_YESNOCANCEL);
	if (vast == IDCANCEL)
		return;
	OpenDialog1->FileName = fname;
	OpenDialog1->DefaultExt = L"txt";
	OpenDialog1->Filter = L"Tekstitiedostot (*.txt)|*.txt|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		wcsncpy(fname, OpenDialog1->FileName.c_str(), sizeof(fname)/2-1);
		AjatTiedostosta(fname, Jono, vast == IDNO);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::GridDrawCell(int grid, TObject *Sender, int ACol,
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
	  switch (Cells[grid][ARow][ACol].style) {
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
			pBitmap->Canvas->Brush->Color = clMedGray;  //; light gray
//			pBitmap->Canvas->Brush->Color = TColor(0x00202020);  //; light gray
			break;
		 }
	  }
   pBitmap->Canvas->FillRect(Rect1);
//   pBitmap->Canvas->Font->Size = FontSize;
   X = 1;
   if (Cells[grid][ARow][ACol].justify == 1) {
	   X =  Rect.Width() - pBitmap->Canvas->TextWidth(Cells[grid][ARow][ACol].text) - 2;
	   if (X < 1)
		  X = 1;
	   }
   if (Cells[grid][ARow][ACol].justify == 2) {
	   X =  (Rect.Width() - pBitmap->Canvas->TextWidth(Cells[grid][ARow][ACol].text) - 2) / 2;
	   if (X < 1)
		  X = 1;
	   }
   pBitmap->Canvas->TextOut(X,1,Cells[grid][ARow][ACol].text);
   DGajat[grid]->Canvas->Draw(Rect.Left, Rect.Top, pBitmap);
   delete pBitmap;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::DGajat1DrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
	GridDrawCell(0, Sender, ACol, ARow, Rect, State);
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::DGajat2DrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
	GridDrawCell(1, Sender, ACol, ARow, Rect, State);
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Nytmysohitetutajat1Click(TObject *Sender)
{
	Nytmysohitetutajat1->Checked = !Nytmysohitetutajat1->Checked;
	aikajono[IkkParam.Jono]->atimemask = Nytmysohitetutajat1->Checked ? 0 : '\xff';
	aikajono[IkkParam.Jono]->haeAktAjat();
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::DGajat1SetEditText(TObject *Sender, int ACol, int ARow,
		  const UnicodeString Value)
{
	if (IkkParam.SyottoTila && ARow > 0 && ARow == aktRivi1 &&
		((ACol == cBdg && IkkParam.SyottoTila == 3) ||
		 (ACol == cKno && IkkParam.SyottoTila >= 1 && IkkParam.SyottoTila < 3) ||
		 ((ACol == cOs || ACol == cPst) && IkkParam.SyottoTila == 2))) {
		 Cells[0][ARow][ACol].text = Value;
		 }
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::DGajat1GetEditText(TObject *Sender, int ACol, int ARow,
		  UnicodeString &Value)
{
	if (IkkParam.SyottoTila && ARow > 0 &&
		((ACol == cBdg && IkkParam.SyottoTila == 3) ||
		 (ACol == cKno && IkkParam.SyottoTila >= 1 && IkkParam.SyottoTila < 3) ||
		 ((ACol == cOs || ACol == cPst) && IkkParam.SyottoTila == 2))) {
		 Value = Cells[0][ARow][ACol].text;
		 }
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Pieninmahdollinen01ms1Click(TObject *Sender)
{
	Pieninmahdollinen01ms1->Checked = true;
	Sijoitukseenvaikuttavalisys1->Checked = !Pieninmahdollinen01ms1->Checked;
	IkkParam.AikaLisays = 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Sijoitukseenvaikuttavalisys1Click(TObject *Sender)

{
	Pieninmahdollinen01ms1->Checked = false;
	Sijoitukseenvaikuttavalisys1->Checked = !Pieninmahdollinen01ms1->Checked;
	IkkParam.AikaLisays = kilpparam.pyor[2]*AIKAJAK;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Maalikello1Click(TObject *Sender)
{
	if (!FormMaalikello)
		FormMaalikello = new TFormMaalikello(this);

	FormMaalikello->Show();
	if (FormMaalikello->WindowState == wsMinimized)
		FormMaalikello->WindowState = wsNormal;
	FormMaalikello->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Vaihdanytetynjonontiedosto1Click(TObject *Sender)
{
	wchar_t flnm[102] = L"", ch;
	wcsncpy(flnm, aikajono[Jono]->aikafname, 100);
	inputstr_prompt(flnm, 100, L"Anna uuden tiedoston nimi", &ch, this);
	if (ch != ESC) {
		wcsncpy(aikajono[Jono]->aikafname, flnm, 100);
		suljeajat(Jono, false);
		initajat(Jono, 1);
		}
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Nytosuusvalinta1Click(TObject *Sender)
{
	Nytosuusvalinta1->Checked = !Nytosuusvalinta1->Checked;
	Panel3->Visible = Nytosuusvalinta1->Checked || IkkParam.Ennakoi || aikaTCPstatus != 0;
	if (!Panel3->Visible && ClientWidth >= Panel2->Left + Panel2->Width + Panel3->Width-1)
		ClientWidth = Panel2->Left + Panel2->Width;
	if (Panel3->Visible && ClientWidth < Panel2->Left + Panel2->Width + Panel3->Width)
		ClientWidth = Panel2->Left + Panel2->Width + Panel3->Width;
	FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::CBPisteValClick(TObject *Sender)
{
	va_mpiste[Jono] = CBPisteVal->ItemIndex - 2;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::DGajat1Exit(TObject *Sender)
{
	naytaAjat1();
}
//---------------------------------------------------------------------------

