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
#include "WinHk.h"
#include "UnitEmiTag.h"
#include "UnitNappain.h"
#include "UnitSarjaVal.h"
#include "UnitEmitSQL.h"
#include "UnitEmitnouto.h"
#include "UnitAikakorjaus.h"
#include "UnitAikavert.h"
#include "TpLaitteet.h"
#include "UnitMaalikello.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAjanotto *FormAjanotot[NAJANOTOT];

void initajat(INT ino, INT vaihda);
static const int cRivi = 0;
static const int cAika = 1;
static const int cBdg = 2;
static const int cKno = 3;
static const int cPst = 4;
static const int cNimi = 5;
static const int cSra = 6;
static const int cSrj = 7;
static const int cTls = 8;
static const int cEro = 9;
static const int cTrk = 10;

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
	IkkParam.NappainPiste = ol_piste;
	applyParams();
#ifdef DBGFILE
	if (dbgtofile)
		{
		Erikoistoiminnot1->Visible = true;
		Avaadebugtiedosto1->Visible = true;
		Suljedebugtiedosto1->Visible = true;
		}
#endif
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
			KynnistSirityhteysuudelleen1->Visible = true;
			Sirityhteydenpollaus1->Visible = true;
			}
		}
	HaeMySQLkannasta1->Visible = (regnly[eTParam.eTrno] == LID_ETSQL);
	HakuEmitinpalvelimelta1->Visible = (regnly[eTParam.eTrno] == LID_ETGPRS);
	DGajat1->ColCount = MAXAJCOL;
	DGajat2->ColCount = MAXAJCOL;
	DGajat1->Row = 1;
	SarakeLeveydet();
	ClientWidth = GridW + Panel2->Width + Panel3->Width;
	aktRivi1 = 1;
	tallVertRivi();
	if (aikajono[0] == NULL || aikajono[0]->atime == NULL) {
		ajanottofl = 1;
		inittimer();
		}
	CBPisteVal->Clear();
	CBPisteVal->Items->Add(L"Auto");
	CBPisteVal->Items->Add(L"Lähtö");
	CBPisteVal->Items->Add(L"Maali");
	for (int ip = 1; ip <= kilpparam.valuku; ip++)
		CBPisteVal->Items->Add(ip);
	if (IkkParam.OletusPiste >= -2 && IkkParam.OletusPiste <= kilpparam.valuku)
		CBPisteVal->ItemIndex = IkkParam.OletusPiste + 2;
	else
		CBPisteVal->ItemIndex = 2;
	CBJono->Clear();
	if (maxjono) {
		for (int r = 0; r <= maxjono; r++) {
			CBJono->Items->Add(L"Jono "+UnicodeString(r+1));
			}
		}
	else if (lajatfl) {
		CBJono->Items->Add(L"Aika");
		CBJono->Items->Add(L"Lähtö");
		}
	else {
		CBJono->Items->Add(L"Jono 1");
		}
	CBJono->ItemIndex = IkkParam.Jono;
	IkkParam.Ennakoi = true;
	if (emitfl)
		CBSyotto->Items->Add(L"Badge");
	FormResize(Sender);
}
//---------------------------------------------------------------------------

ajatIkkParamClass::ajatIkkParamClass(void)
{
	memset(this, 0, sizeof(ajatIkkParamClass));
	Ennakoi = false;
	Suppeat = false;
	VaadiVahvistus = 1;
	Jono = 0;
	OletusPiste = 0;
	SyottoTila = 0;
	NayttoTila = 2;
	NappainPiste = -2;
	AikaLisays = 1;
	KeyCode = 0;
	FontSize = 8;
	ViimOsuus = 20;
	ColW[cRivi] = 35;
	ColW[cAika] = 75;
	ColW[cBdg] = 40;
	ColW[cKno] = 40;
	ColW[cPst] = 25;
	ColW[cNimi] = 140;
	ColW[cSra] = 80;
	ColW[cSrj] = 80;
	ColW[cTls] = 75;
	ColW[cEro] = 50;
	ColW[cTrk] = 30;
	for (int c = 0; c < MAXAJCOL; c++)
		ColOn[c] = (c == cBdg ? 0 : 1);
}
//---------------------------------------------------------------------------
int TFormAjanotto::applyParams(void)
{
	Salliennakoivanumerosytt1->Checked = IkkParam.Ennakoi;
	Vaaditallennuksillevahvistus1->Checked = IkkParam.VaadiVahvistus == 1;
	Vaadivahvistus2->Checked = IkkParam.VaadiVahvistus == 2;
	Nytsuppeattiedot1->Checked = IkkParam.Suppeat;
	CBSyotto->ItemIndex = IkkParam.SyottoTila;
	RGNayta->ItemIndex = IkkParam.NayttoTila;
	CBJono->ItemIndex = IkkParam.Jono;
	DGajat1->Font->Size = IkkParam.FontSize;
	DGajat2->Font->Size = IkkParam.FontSize;
	DGajat1->DefaultRowHeight = (20 * IkkParam.FontSize) / 8;
	DGajat2->DefaultRowHeight = (20 * IkkParam.FontSize) / 8;
	if (IkkParam.AikaLisays <= 1) {
		IkkParam.AikaLisays = 1;
		Pieninmahdollinen01ms1->Checked = true;
		}
	else {
		IkkParam.AikaLisays = pvparam[k_pv].pyor[2]*AIKAJAK;
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
	VertRivi[3] = Cells[0][aktRivi1][cPst].text;
}
//---------------------------------------------------------------------------
bool __fastcall TFormAjanotto::vertRiviMuutt(void)
{
	if (aktRivi1 < 1 || aktRivi1 >= DGajat1->RowCount || aktRivi1 >= CellRows[0])
		return(false);
	if ((CBSyotto->ItemIndex == 3 && VertRivi[0] != Cells[0][aktRivi1][cAika].text) ||
		(CBSyotto->ItemIndex == 6 && VertRivi[1] != Cells[0][aktRivi1][cBdg].text) ||
		(CBSyotto->ItemIndex != 3 && CBSyotto->ItemIndex != 6 &&
			(VertRivi[2] != Cells[0][aktRivi1][cKno].text
			|| (DGajat1->Col == cPst && VertRivi[3] != Cells[0][aktRivi1][cPst].text))
			))
		return(true);
	return(false);
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::naytaRivi(int grid, int r, int rw, int jono)
{
	int d, tl, ero, ero1, piste;
	wchar_t as[60], pisteet[] = L"ALM";
	kilptietue kilp;
	aikatp pt, upt;

	if (rw > aikajono[jono]->aktrows) {
		memset(&pt, 0, sizeof(pt));
		pt.t = AIKAJAK*TMAALI0;
		}
	else
		aikajono[jono]->getAktTime(&pt, rw);
	for (int c = 0; c < MAXAJCOL; c++) {
		Cells[grid][r][c].style = pt.status ? 7 : 0;
		if (!pt.status && wcslen(varitettavat) > 0 && IntInList(pt.kanava, varitettavat) >= 0) {
			Cells[grid][r][c].style = 5;
			}
		}
	Cells[grid][r][cRivi].justify = 1;
	Cells[grid][r][cAika].justify = 1;
	Cells[grid][r][cBdg].justify = 1;
	Cells[grid][r][cKno].justify = 1;
	Cells[grid][r][cPst].justify = 2;
	Cells[grid][r][cTls].justify = 1;
	Cells[grid][r][cEro].justify = 1;
	Cells[grid][r][cTrk].justify = 2;
	Cells[grid][r][cRivi].text = UnicodeString(rw+1);
	Cells[grid][r][cAika].text = AIKATOWSTRS(as, purajak(pt.t), t0);
	if (IkkParam.SyottoTila == 1 && rw == aikajono[jono]->aktrows-1 && pt.kno == 0 && pt.t != AIKAJAK*TMAALI0) {
		upt = pt;
		while (Memo1->Lines->Count > 0 && upt.kno == 0) {
			int kno = 0;
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
			int uptr, d;
			tall_rivi(jono, &upt, &pt, &uptr, 0, 0, 0, false);
			pt = upt;
			piste = va_piste(upt.piste);
			if (piste > -2) {
				EnterCriticalSection(&tall_CriticalSection);
				d = getpos(upt.kno);
				kilp.GETREC(d);
				kilp.set_tulos(piste, purajak(upt.t));
				kilp.tallenna(d, 0, 0, 0, 0);
				LeaveCriticalSection(&tall_CriticalSection);
				if (r == aktRivi1) {
					aktRivi1++;
					DGajat1->Row = aktRivi1;
					aikajono[jono]->getAktTime(&eAika, rw+1);
					}
				}
			}
		}
	if (grid == 0 && IkkParam.SyottoTila && r == aktRivi1 && vertRiviMuutt() &&
		(IkkParam.SyottoTila < 3 || IkkParam.SyottoTila == 6)) {
		if (memcmp(&pt, &eAika, sizeof(aikatp))) {
			Application->MessageBoxW(L"Vastaanotettu muutos muokattavaan riviin", L"Muutos", MB_OK);
			eAika = pt;
			}
		}
	if (IkkParam.ColOn[cBdg])
		Cells[grid][r][cBdg].text = pt.badge;
	Cells[grid][r][cKno].text = UnicodeString(pt.kno);
	Cells[grid][r][cPst].text = str_piste(pt.piste);
	EnterCriticalSection(&tall_CriticalSection);
	if (pt.kno > 0 && (d = getpos(pt.kno)) > 0) {
		kilp.GETREC(d);
		LeaveCriticalSection(&tall_CriticalSection);
		Cells[grid][r][cNimi].text = kilp.nimi(as, 35, false);
		Cells[grid][r][cSra].text = kilp.seuratieto(0);
		Cells[grid][r][cSrj].text = Sarjat[kilp.Sarja()].sarjanimi;
		piste = va_piste(pt.piste);
		if (piste > -2 && (tl = kilp.p_aika(piste)) != 0) {
			ero = purajak(pt.t) - kilp.maali(piste);
			if (piste == -1) {
				tl -= kilp.pv[k_pv].tlahto;
				Cells[grid][r][cTls].text = eroaikatowstr(as, tl, pvparam[k_pv].laikasj);
				}
			else {
				elimwzb1(aikatowstr_cols_n(as, tl, 0, 0, pvparam[k_pv].laikasj));
				Cells[grid][r][cTls].text = as;
				}
			ero1 = ero/pvparam[k_pv].pyor[2];
			if (ero1)
				Cells[grid][r][cEro].text = eroaikatowstr(as, ero, pvparam[k_pv].laikasj);
			else
				Cells[grid][r][cEro].text = L"";
			}
		else {
			Cells[grid][r][cTls].text = L"";
			Cells[grid][r][cEro].text = L"";
			}
		if (kilp.tark(k_pv) == L'B')
			Cells[grid][r][cTrk].text = L"Hav";
		else
			Cells[grid][r][cTrk].text = (kilp.tark(k_pv) != L'-') ? UnicodeString(kilp.tark(k_pv)) : UnicodeString(L"");
		}
	else if (pt.kno > 10000) {
		LeaveCriticalSection(&tall_CriticalSection);
		as[0] = 0;
		if (pt.kno == 10000) {
			wcscpy(as, L"Kohdistusaika");
			}
		if (pt.kno == 19998) {
			wcscpy(as, L"Nollaus - ylimääräinen aika");
			}
		if (pt.kno == 19999) {
			wcscpy(as, L"Nollauksen loppu");
			}
		if (pt.kno > 30000 && pt.kno <= 30000 + sarjaluku) {
			swprintf(as, L"Sarjan %s lähtö", Sarjat[pt.kno-30001].sarjanimi);
			}
		if (pt.kno > 31000 && pt.kno < 32000 + sarjaluku) {
			swprintf(as, L"Erän %d lähtö", pt.kno-31000);
			}
		Cells[grid][r][cNimi].text = as;
		Cells[grid][r][cSra].text = L"";
		Cells[grid][r][cSrj].text = L"";
		Cells[grid][r][cTls].text = L"";
		Cells[grid][r][cEro].text = L"";
		Cells[grid][r][cTrk].text = L"";
		}
	else {
		LeaveCriticalSection(&tall_CriticalSection);
		if (rw < aikajono[jono]->aktrows)
			swprintf(as, L"Badge: %d, Lähde: %3d%c", pt.badge, (int) pt.kanava, pt.tapahtuma ? L'D' : L' ');
		else
			as[0] = 0;
		Cells[grid][r][cNimi].text = as;
		Cells[grid][r][cSra].text = L"";
		Cells[grid][r][cSrj].text = L"";
		Cells[grid][r][cTls].text = L"";
		Cells[grid][r][cEro].text = L"";
		Cells[grid][r][cTrk].text = L"";
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::naytaTCPstatus(void)
{
	int rno;

	for (rno = 0; rno < NREGNLY; rno++)
		if (regnly[rno] == LID_SIRIT)
			break;
	if (rno >= NREGNLY)
		return;
	if (aikaTCPstatus == 0) {
		MemoTCP->Visible = false;
		return;
		}
	MemoTCP->Visible = true;
	if (aikaTCPstatus > 0) {
		if (SiritEiVastaa[rno] < 5000) {
			MemoTCP->Lines->Strings[3] = L"ON";
			MemoTCP->Lines->Strings[4] = L"";
			MemoTCP->Color = clLime;
			}
		else {
			MemoTCP->Lines->Strings[3] = L"Ei";
			MemoTCP->Lines->Strings[4] = L"vastaa";
			MemoTCP->Color = clYellow;
			}
		}
	else {
		MemoTCP->Lines->Strings[3] = L"POIKKI";
		MemoTCP->Lines->Strings[4] = L"";
		MemoTCP->Color = clRed;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::naytaAjat1(void)
{
	int r, r2, n;

	naytaTCPstatus();
	r2 = aikajono[IkkParam.Jono]->aktrows;
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
		Cells[0][0][cPst].text = L"Pst";
		Cells[0][0][cNimi].text = L"Nimi";
		Cells[0][0][cSra].text = L"Seura";
		Cells[0][0][cSrj].text = L"Sarja";
		Cells[0][0][cTls].text = L"Tulos";
		Cells[0][0][cEro].text = L"Ero";
		Cells[0][0][cTrk].text = L"Trk";
		}
	for (r = 1; r < DGajat1->RowCount; r++) {
		naytaRivi(0, r, r-1, IkkParam.Jono);
		}
	tallVertRivi();
	if (DGajat1->TopRow == 0)
		DGajat1->TopRow = 1;
	DGajat1->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::naytaAjat2(void)
{
	int r, r2, n;

	r2 = aikajono[IkkParam.Jono]->aktrows;
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
			naytaRivi(1, r, r2 - n + r, IkkParam.Jono);
			}
		else {
			for (int c = 0; c < DGajat2->ColCount; c++)
				Cells[1][r][c].text = L"";
			}
		}
	DGajat2->Refresh();
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
void __fastcall TFormAjanotto::FormResize(TObject *Sender)
{
	Panel2->Top = 0;
	DGajat1->Top = 0;
	Panel3->Top = 0;
	if (RGNayta->ItemIndex == 2) {
		DGajat2->Height = IkkParam.ViimOsuus * ClientHeight / 100;
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
	OnLine = true;
	Panel2->Height = ClientHeight;
	Panel3->Height = ClientHeight;
	Panel3->Left = ClientWidth - Panel3->Width;
	Panel2->Left = ClientWidth - Panel2->Width - (IkkParam.Ennakoi ? Panel3->Width : 0);
	DGajat1->Width = Panel2->Left;
	DGajat2->Width = Panel2->Left;
	DGajat2->RowCount = DGajat2->Height / (DGajat2->DefaultRowHeight+2);
//	OnLine = DGajat2->Visible;
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::RGNaytaClick(TObject *Sender)
{
	IkkParam.NayttoTila = RGNayta->ItemIndex;
	FormResize(Sender);
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::naytaAktAika(void)
{
	int r;
	wchar_t as[60], pisteet[] = L"ALM";
	aikatp pt;

	if (aktRivi1 == 0 || aktRivi1 >= DGajat1->RowCount)
		return;
	r = _wtoi(Cells[0][aktRivi1][cRivi].text.c_str());
	aikajono[IkkParam.Jono]->getAktTime(&pt, r-1);
	AIKATOWSTRS(as, purajak(pt.t), t0);
//	as[11] = (pt.t+86400*AIKAJAK*SEK) / (AIKAJAK*SEK/1000) % 10 + L'0';
//	as[12] = 0;
	EdRivi->Text = UnicodeString(r);
	EdAika->Text = as;
	EdDate->Text = time_t_date(pt.date, NULL);
	EdKno->Text = UnicodeString(pt.kno);
	EdPiste->Text = str_piste(pt.piste);
	EdBadge->Text = UnicodeString(pt.badge);
	EdLahde->Text = UnicodeString((int)pt.kanava);
	EdLaji->Text = UnicodeString(pt.tapahtuma);
	EdJono->Text = UnicodeString(pt.jono);
	tallVertRivi();
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::DGajat1SelectCell(TObject *Sender, int ACol, int ARow,
		  bool &CanSelect)
{
	if (ARow < 1 || ARow > DGajat1->RowCount)
		return;
//	if (ARow != edRow)
	naytaAjat1();
	if (ARow != aktRivi1) {
		aikajono[IkkParam.Jono]->getAktTime(&eAika, ARow-1);
		aktRivi1 = ARow;
		}
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

	if (aktRivi1 < 1 || aktRivi1 >= DGajat1->RowCount) {
		aktRivi1 = 1;
		}
	aikajono[IkkParam.Jono]->getAktTime(&vpt, aktRivi1-1);
	t = AIKAJAK * Nyt();
	lisaa_aika(IkkParam.Jono, t, IkkParam.NappainPiste, AIKAJAK*TMAALI0, 0);
	aikajono[IkkParam.Jono]->getAktTime(&upt, aktRivi1-1);
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
	int r, t;

	if (aktRivi1 < 1 || aktRivi1 >= DGajat1->RowCount) {
		aktRivi1 = 1;
		}
	r = aktRivi1-2;
	if (r < 0)
		r = 0;
	aikajono[IkkParam.Jono]->getAktTime(&vpt, r);
	if (vpt.t/AIKAJAK != TMAALI0) {
		if (aktRivi1 == 1)
			t = NORMKELLO_A(vpt.t - (IkkParam.AikaLisays > 1 ? IkkParam.AikaLisays : 10));
		else {
			t = NORMKELLO_A(vpt.t + IkkParam.AikaLisays);
			while (r+1 < aikajono[IkkParam.Jono]->aktrows) {
				aikajono[IkkParam.Jono]->getAktTime(&pt, r+1);
				if (NORMKELLO_A(pt.t-t) > 0)
					break;
				t += IkkParam.AikaLisays;
				r++;
				}
			}
		}
	else
		t = AIKAJAK * Nyt();
	lisaa_aika(IkkParam.Jono, t, IkkParam.NappainPiste, vpt.t, vpt.date);
	aikajono[IkkParam.Jono]->getAktTime(&upt, aktRivi1-1);
	if (memcmp(&vpt, &upt, sizeof(aikatp)))
		eAika = upt;
	DGajat1->Row = aktRivi1;
	naytaAjat1();
	naytaAjat2();
	naytaAktAika();
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::BtnTall2Click(TObject *Sender)
{
	aikatp vpt, upt;
	wchar_t cp, *vp = L"Vahvista tallennus";
	int r, t, uptr;
	kilptietue kilp;
	int d;

	aikajono[IkkParam.Jono]->getAktTime(&vpt, aktRivi1-1);
	upt = vpt;
	upt.t = wstrtoaika_vap(EdAika->Text.c_str(), t0) * AIKAJAK;
	upt.date = date_time_t(EdDate->Text.c_str());
	upt.jono = IkkParam.Jono;
	upt.kno = _wtoi(EdKno->Text.c_str());
	wcsncpy(&cp, EdPiste->Text.c_str(), 1);
	cp = towupper(cp);
	if (cp == L'A')
		upt.piste = -2;
	if (cp == L'L')
		upt.piste = -1;
	if (cp == L'M')
		upt.piste = 0;
	else
		upt.piste = (char) _wtoi(EdPiste->Text.c_str());
	d = 0;
	if (upt.kno && (d = getpos(upt.kno)) > 0) {
		kilp.GETREC(d);
		}
	if ((vpt.kno != upt.kno || vpt.piste != upt.piste) && d > 0) {
		if (kilp.p_aika(va_piste(upt.piste))) {
			vp = L"Kilpailijalla jo aika, tallennetaanko muutos?";
			}
		}
	if (Application->MessageBoxW(vp, L"Vahvistus", MB_YESNO) != IDYES) {
		return;
		}
	tall_rivi(IkkParam.Jono, &upt, &vpt, &uptr, 0, 0, 0, false);
	eAika = upt;
	if (vpt.kno || upt.kno) {
		EnterCriticalSection(&tall_CriticalSection);
		if ((d = getpos(vpt.kno)) > 0) {
			kilp.GETREC(d);
			if (abs((double) NORMKELLO(vpt.t/AIKAJAK-kilp.maali(va_piste(vpt.piste)))) < 2*pvparam[k_pv].pyor[2]) {
				kilp.tall_tulos(va_piste(vpt.piste), 0);
				if (upt.kno != vpt.kno)
					kilp.tallenna(d, 0, 0, 0, 0);
				}
			if (upt.kno != vpt.kno) {
				d = -1;
				}
			}
		if (d < 1 && upt.kno && (d = getpos(upt.kno)) > 0) {
			kilp.GETREC(d);
			}
		if (d > 0) {
			if (upt.kno && va_piste(upt.piste) >= -1 && va_piste(upt.piste) <= Sarjat[kilp.Sarja()].valuku[k_pv])
				kilp.set_tulos(va_piste(upt.piste), upt.t/AIKAJAK, true);
			kilp.tallenna(d, 0, 0, 0, 0);
			}
		LeaveCriticalSection(&tall_CriticalSection);
		}
	naytaAjat1();
	naytaAjat2();
	naytaAktAika();
	FocusControl(BtnSeur);
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::BtnPeruutaClick(TObject *Sender)
{
	naytaAktAika();
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::BtnPoistaClick(TObject *Sender)
{
	aikatp vpt, upt;

	if (aktRivi1 > aikajono[IkkParam.Jono]->aktrows)
		return;
	if (Application->MessageBoxW(L"Vahvista poisto", L"Vahvistus", MB_YESNO) != IDYES)
		return;
	aikajono[IkkParam.Jono]->getAktTime(&vpt, aktRivi1-1);
	poista_aika(IkkParam.Jono, &vpt);
	if (aktRivi1 > aikajono[IkkParam.Jono]->aktrows) {
		aktRivi1 = aikajono[IkkParam.Jono]->aktrows;
		}
	DGajat1->Row = aktRivi1;
	aikajono[IkkParam.Jono]->getAktTime(&upt, aktRivi1-1);
	eAika = upt;
	naytaAjat1();
	naytaAjat2();
	naytaAktAika();
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::EnterPressed(TObject *Sender, System::WideChar &Key)
{
	if (AikaKey || (Key < 32 && Key != 8))
		Key = 0;
}

//---------------------------------------------------------------------------
void TFormAjanotto::kasitteleSpecialKey(WORD &Key, TShiftState Shift)
{
	if (
//		(Key == L'E' && Shift.Contains(ssCtrl)) ||
		(Key != 0 && Key == IkkParam.KeyCode && Shift == IkkParam.KeyShift)) {
		AikaKey = true;
		Key = 0;
		if (Key == L'E' && Shift.Contains(ssCtrl))
			Vaihdaer1Click(this);
		else
			lisaaAika();
		}
	else
		AikaKey = false;
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
			 BtnTall2Click(Sender);
		Key = 0;
		}
	kasitteleSpecialKey(Key, Shift);
/*
	if (
//		(Key == L'E' && Shift.Contains(ssCtrl)) ||
		(Key != 0 && Key == IkkParam.KeyCode && Shift == IkkParam.KeyShift)) {
		AikaKey = true;
		Key = 0;
		if (Key == L'E' && Shift.Contains(ssCtrl))
			Vaihdaer1Click(Sender);
		else
			lisaaAika();
		}
	else
		AikaKey = false;
*/
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
			 BtnTall2Click(Sender);
		Key = 0;
		}
	kasitteleSpecialKey(Key, Shift);
/*
	if (Key != 0 && Key == IkkParam.KeyCode && Shift == IkkParam.KeyShift) {
		AikaKey = true;
		Key = 0;
		lisaaAika();
		}
	else
		AikaKey = false;
*/
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::BtnSeurClick(TObject *Sender)
{
	if (aktRivi1 < aikajono[IkkParam.Jono]->aktrows) {
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
			FocusControl(BtnTall2);
		else if (!Shift.Contains(ssShift) &&
			 Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt))
			 BtnTall2Click(Sender);
		Key = 0;
		}
	kasitteleSpecialKey(Key, Shift);
/*
	if (Key != 0 && Key == IkkParam.KeyCode && Shift == IkkParam.KeyShift) {
		AikaKey = true;
		Key = 0;
		lisaaAika();
		}
	else
		AikaKey = false;
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::CBJonoChange(TObject *Sender)
{
	if (CBJono->ItemIndex < 0) {
		CBJono->ItemIndex = IkkParam.Jono;
		return;
		}
	IkkParam.Jono = CBJono->ItemIndex;
	if (aktRivi1 > aikajono[IkkParam.Jono]->aktrows) {
		aktRivi1 = aikajono[IkkParam.Jono]->aktrows;
		}
	DGajat1->Row = aktRivi1;
	aikajono[IkkParam.Jono]->getAktTime(&eAika, aktRivi1);
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormAjanotto::SarakeLeveydet(void)
{
	DGajat1->ColCount = IkkParam.Suppeat ? cPst + 1 : MAXAJCOL;
	DGajat2->ColCount = DGajat1->ColCount;
	GridW = 20 * Screen->PixelsPerInch / 96;
	for (int c = 0; c < DGajat1->ColCount; c++) {
		DGajat1->ColWidths[c] = IkkParam.FontSize * IkkParam.ColOn[c] *
			IkkParam.ColW[c] * Screen->PixelsPerInch / 768;
		DGajat2->ColWidths[c] = DGajat1->ColWidths[c];
		GridW += DGajat1->ColWidths[c] + 2;
		}
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
//	HaeMySQLkannasta1->Visible = TestiToiminnot;
	BtnPaivitaClick(Sender);
	if (aktRivi1 > aikajono[IkkParam.Jono]->aktrows)
		aktRivi1 = aikajono[IkkParam.Jono]->aktrows;
	if (aktRivi1 < 1)
		aktRivi1 = 1;
	DGajat1->Row = aktRivi1;
	aikajono[IkkParam.Jono]->getAktTime(&eAika, aktRivi1);
}
//---------------------------------------------------------------------------
int __fastcall TFormAjanotto::SarjaLahto(int row, wchar_t val)
{
	aikatp pt, upt;
	int r, uptr, ret = 1;

	if (row < 1 || (r = _wtoi(Cells[0][row][cRivi].text.c_str())) == 0)
		return(ret);
	aikajono[IkkParam.Jono]->getAktTime(&pt, r-1);
	if (pt.t == AIKAJAK*TMAALI0)
		return(ret);
	upt = pt;
	if (val == L'S') {
		FormSarjaVal = new TFormSarjaVal(this);
		FormSarjaVal->ShowModal();
		if (FormSarjaVal->ModalResult == mrOk) {
			asetaSrjLahto(FormSarjaVal->Sarja, purajak(pt.t), false, true, false);
			upt.kno = 30001+FormSarjaVal->Sarja;
			upt.piste = -1;
			tall_rivi(IkkParam.Jono, &upt, &pt, &uptr, 0, 0, 0, false);
			ret = 0;
			}
		delete FormSarjaVal;
		}
	if (val == L'R') {
		int era = 0;
		wchar_t ch;
		inputint_prompt(&era, L"Anna erän numero", &ch, this);
		if (era >= 0 && ch != ESC) {
			asetaEraLahto(era, purajak(pt.t), true, false);
			upt.kno = 31000+era;
			upt.piste = -1;
			tall_rivi(IkkParam.Jono, &upt, &pt, &uptr, 0, 0, 0, false);
			ret = 0;
			}
		}
	return(ret);
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

void __fastcall TFormAjanotto::CBSyottoClick(TObject *Sender)
{
	IkkParam.SyottoTila = CBSyotto->ItemIndex;
	switch (CBSyotto->ItemIndex) {
		case 1:
		case 2:
		case 3:
		case 4:
		case 5:
		case 6:
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
	if (IkkParam.ColOn[cBdg] && IkkParam.SyottoTila != 6) {
		IkkParam.ColOn[cBdg] = 0;
		SarakeLeveydet();
		ClientWidth = GridW + Panel2->Width + (Panel3->Visible ? Panel3->Width : 0);
		}
	if (IkkParam.ColOn[cBdg] == 0 && IkkParam.SyottoTila == 6) {
		IkkParam.ColOn[cBdg] = 1;
		SarakeLeveydet();
		ClientWidth = GridW + Panel2->Width + (Panel3->Visible ? Panel3->Width : 0);
		}
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------

void TFormAjanotto::uusintaTallennus(int rivi)
{
	aikatp pt;
	kilptietue kilp;
	int d;

	if (DGajat1->Row < 1 || DGajat1->Row >= DGajat1->RowCount)
		return;
	aikajono[IkkParam.Jono]->getAktTime(&pt, rivi-1);
	tall_rivi(IkkParam.Jono, &pt, &pt, 0, 0, 0, 1, 0);
	if (pt.kno && pt.t != AIKAJAK*TMAALI0 && va_piste(pt.piste) > -2 && (d = getpos(pt.kno)) > 0) {
		kilp.GETREC(d);
		if ((va_piste(pt.piste) >= 0 && va_piste(pt.piste) <= Sarjat[kilp.Sarja()].valuku[k_pv]) ||
			(pt.piste == -1 && pvparam[k_pv].hiihtolahto)) {
			kilp.set_tulos(va_piste(pt.piste), purajak(pt.t), 1);
			kilp.tallenna(d, 0, 0, 1, 0);
			}
		}
	if (DGajat1->Row < DGajat1->RowCount-1)
		DGajat1->Row++;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Aktivoi(int rivi, int status)
{
	aikatp pt, upt;

	if (rivi < 1 || rivi >= DGajat1->RowCount)
		return;
	aikajono[IkkParam.Jono]->getAktTime(&pt, rivi-1);
	upt = pt;
	upt.status = status;
	tall_rivi(IkkParam.Jono, &upt, &pt, 0, 0, 0, 0, 0);
	naytaAjat1();
	naytaAjat2();
	if (DGajat1->Row < DGajat1->RowCount-1)
		DGajat1->Row++;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::DGajat1KeyPress(TObject *Sender, System::WideChar &Key)

{
	int r, uPst, uKno;
	wchar_t as[60], pisteet[] = L"ALM";
	bool tallFl = false;
	aikatp pt, upt;
	int t, uptr, badge = 0;
	kilptietue kilp;
	int d, pakLah = 0;
	aikajono[IkkParam.Jono]->getAktTime(&pt, aktRivi1-1);
	upt = pt;
	UnicodeString CTxt;

	if (AikaKey || IkkParam.SyottoTila == 0 || aktRivi1 < 1) {
		Key = 0;
		return;
		}
	if (DGajat1->Col == cKno && wcswcind(towupper(Key), L"RS") >= 0) {
		if (SarjaLahto(DGajat1->Row, towupper(Key)) == 0) {
			naytaAjat1();
			naytaAjat2();
			naytaAktAika();
			if (DGajat1->Row < DGajat1->RowCount-1) {
				DGajat1->Row++;
				}
			}
		Key = 0;
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
			case cPst :
				Cells[0][DGajat1->Row][DGajat1->Col].text = VertRivi[3];
				break;
			}
		DGajat1->SetFocus();
		return;
		}
	if (Key == L'\r' || Key == L'\n') {
		t = TMAALI0;
		if (IkkParam.SyottoTila >= 3 && IkkParam.SyottoTila <= 5 && DGajat1->Col >= cKno && DGajat1->Col <= IkkParam.SyottoTila - 1 &&
			Cells[0][DGajat1->Row][cAika].text.Length() > 6) {
			t = wstrtoaika_vap(Cells[0][DGajat1->Row][cAika].text.c_str(), t0);
			Cells[0][DGajat1->Row][cAika].text = aikatowstr_ts(as, t, t0);
			if (DGajat1->Col == cAika)
				DGajat1->Refresh();
			}
		if (IkkParam.SyottoTila == 3 && DGajat1->Col != cAika || Cells[0][DGajat1->Row][cAika].text.Length() != 12) {
			naytaAjat1();
			DGajat1->Col = cAika;
			return;
			}
		if ((IkkParam.SyottoTila == 1 && DGajat1->Col != cKno) ||
			(IkkParam.SyottoTila == 2 && DGajat1->Col != cPst) ||
			(IkkParam.SyottoTila == 3 && DGajat1->Col != cAika) ||
			(IkkParam.SyottoTila == 4 && DGajat1->Col != cKno) ||
			(IkkParam.SyottoTila == 5 && DGajat1->Col != cPst) ||
			(IkkParam.SyottoTila == 6 && DGajat1->Col != cBdg)) {
			if (IkkParam.SyottoTila % 3 == 2 && DGajat1->Col == cKno)
				DGajat1->Col = cPst;
			else if (IkkParam.SyottoTila > 3 && IkkParam.SyottoTila <= 5 && DGajat1->Col == cAika)
				DGajat1->Col = cKno;
			else {
				DGajat1->Col = (IkkParam.SyottoTila < 3 ? cKno : (IkkParam.SyottoTila == 6 ? cBdg : cAika));
				if (DGajat1->Row < DGajat1->RowCount - 1) {
					DGajat1->Row++;
					if (_wtoi(Cells[0][DGajat1->Row][cRivi].text.c_str()) == 0)
					   Cells[0][DGajat1->Row][cRivi].text =
						  _wtoi(Cells[0][DGajat1->Row-1][cRivi].text.c_str()) + 1;
					}
				}
//			naytaAjat1();
			return;
			}
		r = _wtoi(Cells[0][DGajat1->Row][cRivi].text.c_str());
		if (r > aikajono[IkkParam.Jono]->aktrows && (IkkParam.SyottoTila < 3 || t == TMAALI0)) {
			if (IkkParam.Ennakoi && DGajat1->Col == cKno && (uKno = _wtoi(Cells[0][DGajat1->Row][cKno].text.c_str())) > 0) {
				Memo1->Lines->Add(uKno);
				}
			Cells[0][DGajat1->Row][cKno].text = L"0";
			return;
			}
		aikajono[IkkParam.Jono]->getAktTime(&pt, r-1);
		upt = pt;
		if (IkkParam.SyottoTila >= 3 && IkkParam.SyottoTila <= 5) {
			int uT = wstrtoaika_vap(Cells[0][DGajat1->Row][cAika].text.c_str(), t0) * AIKAJAK;
			if (uT/AIKAJAK != TMAALI0 && uT != pt.t) {
				upt.t = uT;
				if (r > aikajono[IkkParam.Jono]->aktrows)
					upt.date = tm_date(upt.t);
				else
					upt.date = tm_copydate(upt.t, pt.t, pt.date);
				tallFl = true;
				}
			}
		if (IkkParam.SyottoTila == 6) {
			badge = _wtoi(Cells[0][DGajat1->Row][cBdg].text.c_str());
			if ((uKno = bdg2kno(badge)) < minkilpno) {
				Key = 0;
				Application->MessageBoxW(L"Tuntematon koodi", L"", MB_OK);
				return;
				}
			Cells[0][DGajat1->Row][cKno].text = uKno;
			upt.kno = uKno;
			upt.badge = badge;
			if (upt.piste < -1 && IkkParam.OletusPiste > -2)
				upt.piste = IkkParam.OletusPiste;
			tallFl = true;
			}
		else if (IkkParam.SyottoTila != 3) {
			uKno = _wtoi(Cells[0][DGajat1->Row][cKno].text.c_str());
//			if (getpos(uKno) <= 0) {
//				Cells[0][2][DGajat1->Row] = L"0";
//				return;
//				}
			if (upt.piste < -1 && IkkParam.OletusPiste > -2) {
				upt.piste = IkkParam.OletusPiste;
				uPst = upt.piste;
				}
			if (uKno != pt.kno) {
				upt.kno = uKno;
				if (IkkParam.SyottoTila % 3 == 1 && DGajat1->Col == cKno) {
					tallFl = true;
					}
				}
			}
		if (IkkParam.SyottoTila % 3 == 2 && DGajat1->Col == cPst && Cells[0][DGajat1->Row][cPst].text.Length()>0) {
			uPst = _wtoi(Cells[0][DGajat1->Row][cPst].text.c_str());
			if (uPst == 0)
				uPst = wcswcind(Cells[0][DGajat1->Row][cPst].text.UpperCase().c_str()[0], L"0ALM") - 3;
			if (uPst < -2)
				uPst = 0;
			upt.kno = uKno;
			upt.piste = uPst;
			if (uPst >= -2 && (uPst != va_piste(pt.piste) || uKno != pt.kno)) {
				tallFl = true;
				}
			}
		Key = 0;
		if (SG1KeyShift.Contains(ssCtrl)) {
			tallFl = true;
			if (!memcmp(&pt, &upt, sizeof(aikatp)))
				pakLah = 1;
			}
		if (!tallFl && upt.kno && (d = getpos(upt.kno)) > 0) {
			kilp.GETREC(d);
			if (va_piste(upt.piste) > -2 && va_piste(upt.piste) <= Sarjat[kilp.Sarja()].valuku[k_pv]) {
				if ((purajak(upt.t) - kilp.maali(va_piste(upt.piste))) / pvparam[k_pv].pyor[2])
					tallFl = true;
				}
			}
		}
	if (tallFl) {
		rowTp edRow;
		int tl, ero, ero1;

		d = 0;
		for (int c = 1; c < DGajat1->ColCount; c++)
			edRow[c].text = Cells[0][DGajat1->Row][c].text;
		if (upt.kno && (d = getpos(upt.kno)) > 0) {
			kilp.GETREC(d);
			Cells[0][DGajat1->Row][cNimi].text = kilp.nimi(as, 35, false);
			Cells[0][DGajat1->Row][cSra].text = kilp.seuratieto(0);
			Cells[0][DGajat1->Row][cSrj].text = Sarjat[kilp.Sarja()].sarjanimi;
			Cells[0][DGajat1->Row][cPst].text = upt.piste;
			if (va_piste(upt.piste) > -2 && (tl = kilp.p_aika(va_piste(upt.piste))) != 0) {
				ero = purajak(upt.t) - kilp.maali(va_piste(upt.piste));
				if (va_piste(upt.piste) == -1) {
					tl -= kilp.pv[k_pv].tlahto;
					Cells[0][DGajat1->Row][cTls].text = eroaikatowstr(as, tl, pvparam[k_pv].laikasj);
					}
				else {
					elimwzb1(aikatowstr_cols_n(as, tl, 0, 0, pvparam[k_pv].laikasj));
					Cells[0][DGajat1->Row][cTls].text = as;
					}
				ero1 = ero/pvparam[k_pv].pyor[2];
				if (ero1)
					Cells[0][DGajat1->Row][cEro].text = eroaikatowstr(as, ero, pvparam[k_pv].laikasj);
				else
					Cells[0][DGajat1->Row][cEro].text = L"";
				}
			DGajat1->Refresh();
			}
		if ((pt.kno != upt.kno || pt.piste != upt.piste) && d > 0) {
			if (kilp.p_aika(va_piste(upt.piste))) {
				if (Application->MessageBoxW(L"Kilpailijalla jo aika, tallennetaanko muutos?", L"Vahvistus", MB_YESNO) != IDYES)
					tallFl = false;
				}
			}
		if (tallFl && IkkParam.VaadiVahvistus == 1 || (IkkParam.VaadiVahvistus == 2 && pt.kno)) {
			if (Application->MessageBoxW(L"Vahvista tallennus", L"Vahvistus", MB_YESNO) != IDYES)
				tallFl = false;
			}
//		if (!tallFl) {
//			for (int c = 1; c < DGajat1->ColCount; c++)
//				Cells[0][DGajat1->Row][c].text = edRow[c].text;
//			}
		}
	if (tallFl) {
		if (r > aikajono[IkkParam.Jono]->aktrows) {
			tall_rivi(IkkParam.Jono, &upt, 0, &uptr, 0, 0, 0, false);
			eAika = upt;
			}
		else
			tall_rivi(IkkParam.Jono, &upt, &pt, &uptr, 0, 0, pakLah, false);
		tallVertRivi();
		d = -1;
		if (pt.kno || upt.kno) {
			EnterCriticalSection(&tall_CriticalSection);
			if (pt.kno && (d = getpos(pt.kno)) > 0) {
				kilp.GETREC(d);
				if (kilp.kilpstatus == 0 &&
					NORMTULOS(kilp.maali(va_piste(pt.piste)) - purajak(pt.t) + 2*pvparam[k_pv].pyor[3]) < 4*pvparam[k_pv].pyor[3]) {
					kilp.tall_tulos(va_piste(pt.piste), 0);
					if (pt.kno != upt.kno) {
						kilp.tallenna(d, 0, 0, 0, 0);
						d = -1;
						}
					}
				else
					d = -1;
				}
			if (upt.kno) {
				if (d < 1) {
					if ((d = getpos(upt.kno)) > 0)
						kilp.GETREC(d);
					}
				if (d > 0 && kilp.kilpstatus == 0) {
					kilp.set_tulos(va_piste(upt.piste), purajak(upt.t));
					kilp.tallenna(d, 0, 0, pakLah, 0);
					}
				}
			LeaveCriticalSection(&tall_CriticalSection);
			if (upt.kno && pvparam[k_pv].hiihtolahto && upt.piste == -1 && era_akt && kilp.p_aika(-1) != TMAALI0 &&
				purajak(eralahto[kilp.pv[k_pv].era]) != purajak(upt.t)) {
				eralahto[kilp.pv[k_pv].era] = upt.t;
				lahetaeralahto(kilp.pv[k_pv].era, 0);
				}
			}
		}
/*
	else if (kilp.p_aika(upt.piste) != wstrtoaika_vap(Cells[0][7][DGajat1->Row].c_str(), 0)) {
		if ((d = getpos(upt.kno)) > 0) {
			kilp.GETREC(d);
			if (kilp.kilpstatus == 0) {
				EnterCriticalSection(&tall_CriticalSection);
				kilp.set_tulos(upt.piste, purajak(upt.t));
				kilp.tallenna(d, 0, 0, 0, 0);
				LeaveCriticalSection(&tall_CriticalSection);
				}
			}
		}
*/
	if ((Key < L' ' || Key > L'z') && Key != 8) {
		naytaAjat1();
		naytaAjat2();
		naytaAktAika();
		}
	if (tallFl) {
		if (DGajat1->Row < DGajat1->RowCount-1) {
			DGajat1->Row++;
			}
		DGajat1->Col = (IkkParam.SyottoTila < 3 ? cKno : (IkkParam.SyottoTila == 6 ? cBdg : cAika));
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Ajanottonppin1Click(TObject *Sender)
{
	FormNappain = new TFormNappain(this);
	FormNappain->Initiate(IkkParam.KeyCode, IkkParam.KeyShift, kilpparam.maxvaluku, IkkParam.NappainPiste);
	FormNappain->ShowModal();
	FormNappain->GetValues(IkkParam.KeyCode, IkkParam.KeyShift, IkkParam.NappainPiste);
	delete FormNappain;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::DGajat1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	SG1KeyShift = Shift;
 /*
	if (Key != 0 && Key == IkkParam.KeyCode && Shift == IkkParam.KeyShift) {
		AikaKey = true;
		Key = 0;
		lisaaAika();
		}
	else
*/
	if (Key == VK_F11 && !Shift.Contains(ssShift) &&
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
	kasitteleSpecialKey(Key, Shift);
//		AikaKey = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Nytsuppeattiedot1Click(TObject *Sender)
{
	int w;
	Nytsuppeattiedot1->Checked = !Nytsuppeattiedot1->Checked;
	IkkParam.Suppeat = Nytsuppeattiedot1->Checked;
//	for (int col = cPst + 1; col < DGajat1->ColCount; col++)
//		IkkParam.ColOn[col] = IkkParam.Suppeat ? 0 : 1;
	SarakeLeveydet();
	ClientWidth = GridW + Panel2->Width + (Panel3->Visible ? Panel3->Width : 0);
;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::SynkronoiSirit1Click(TObject *Sender)
{
	SiritSync(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Sulje1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Vaihdaer1Click(TObject *Sender)
{
	wchar_t ch;
	int era;

	era = era_akt;
	inputint_prompt(&era, L"Anna erän numero", &ch, this);
	if (era >= 0 && era < MAXERA) {
		era_akt = era;
		Caption = L"Ajanotto - " + UnicodeString(Ajanotto) + L" - Erä " + era_akt;
		}
	else
		Caption = L"Ajanotto - " + UnicodeString(Ajanotto);
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
		AjatTiedostoon(fname, IkkParam.Jono);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Lueaikojatekstitiedostosta1Click(TObject *Sender)
{
	static wchar_t fname[200] = L"ajat.txt";
	int vast;

	vast = Application->MessageBoxW(L"Luetaanko myös kilpailijan numero ja piste (Kyllä), vai vain ajat (Ei)",
		L"Valitse", MB_YESNOCANCEL);
	if (vast == IDCANCEL)
		return;
	OpenDialog1->FileName = fname;
	OpenDialog1->DefaultExt = L"txt";
	OpenDialog1->Filter = L"Tekstitiedostot (*.txt)|*.txt|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		wcsncpy(fname, OpenDialog1->FileName.c_str(), sizeof(fname)/2-1);
		AjatTiedostosta(fname, IkkParam.Jono, vast == IDNO);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Vaaditallennuksillevahvistus1Click(TObject *Sender)
{
	Vaaditallennuksillevahvistus1->Checked = !Vaaditallennuksillevahvistus1->Checked;
	IkkParam.VaadiVahvistus = Vaaditallennuksillevahvistus1->Checked ? 1 : 0;
	Vaadivahvistus2->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Vaadivahvistus2Click(TObject *Sender)
{
	Vaadivahvistus2->Checked = !Vaadivahvistus2->Checked;
	IkkParam.VaadiVahvistus = Vaadivahvistus2->Checked ? 2 : 0;
	Vaaditallennuksillevahvistus1->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Button2Click(TObject *Sender)
{
	Memo1->Clear();
}
//---------------------------------------------------------------------------


void __fastcall TFormAjanotto::Salliennakoivanumerosytt1Click(TObject *Sender)
{
	Salliennakoivanumerosytt1->Checked = !Salliennakoivanumerosytt1->Checked;
	IkkParam.Ennakoi = Salliennakoivanumerosytt1->Checked;
//	Panel3->Visible = IkkParam.Ennakoi;
	if (!IkkParam.Ennakoi)
		Memo1->Clear();
//	Width += (IkkParam.Ennakoi ? Panel3->Width : -Panel3->Width);
	FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::BtnHakuClick(TObject *Sender)
{
	int alkuRivi, kno;

	LblEiLoydy->Visible = false;
	kno = _wtoi(EdHaku->Text.c_str());
	if (aikajono[IkkParam.Jono]->aktrows == 0 || kno < minkilpno || kno > maxkilpno)
		return;
	alkuRivi = aktRivi1;
	if (alkuRivi < 1 || alkuRivi > aikajono[IkkParam.Jono]->aktrows)
		alkuRivi = 1;
	for (int rv = alkuRivi + 1; rv != alkuRivi; rv++) {
		aikatp pt;
		if (rv > aikajono[IkkParam.Jono]->aktrows)
			rv = 0;
		aikajono[IkkParam.Jono]->getAktTime(&pt, rv-1);
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
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Kirjasinkoko1Click(TObject *Sender)
{
	wchar_t ch;
	int fs = IkkParam.FontSize;
	inputint_prompt(&fs, L"Anna taulukoiden kirjasinkoko", &ch, this);
	if (ch != ESC) {
		IkkParam.FontSize = fs;
		DGajat1->Font->Size = IkkParam.FontSize;
		DGajat2->Font->Size = IkkParam.FontSize;
		DGajat1->DefaultRowHeight = (18 * IkkParam.FontSize) / 8;
		DGajat2->DefaultRowHeight = (18 * IkkParam.FontSize) / 8;
		SarakeLeveydet();
		BtnPaivitaClick(Sender);
		}
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

void __fastcall TFormAjanotto::Memo1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	kasitteleSpecialKey(Key, Shift);
/*
	if (Key != 0 && Key == IkkParam.KeyCode && Shift == IkkParam.KeyShift) {
		AikaKey = true;
		Key = 0;
		lisaaAika();
		}
	else
		AikaKey = false;
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::EdHakuKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	kasitteleSpecialKey(Key, Shift);
/*
	if (Key != 0 && Key == IkkParam.KeyCode && Shift == IkkParam.KeyShift) {
		AikaKey = true;
		Key = 0;
		lisaaAika();
		}
	else
		AikaKey = false;
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::EdHakuKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (AikaKey)
		Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Memo1KeyPress(TObject *Sender, System::WideChar &Key)

{
	if (AikaKey)
		Key = 0;
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
	kasitteleSpecialKey(Key, Shift);
/*
	if (Key != 0 && Key == IkkParam.KeyCode && Shift == IkkParam.KeyShift) {
		AikaKey = true;
		Key = 0;
		lisaaAika();
		}
	else
		AikaKey = false;
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::KynnistSirityhteysuudelleen1Click(TObject *Sender)

{
	int sirits[5], nsirit = 0;
	wchar_t ch, msg[100] = L"Valitse laitenumeroista";

	memset(sirits, 0, sizeof(sirits));
	for (int rno = 0; rno < NREGNLY && nsirit < 5; rno++) {
		if (regnly[rno] == LID_SIRIT) {
			sirits[nsirit++] = rno+1;
			wcscat(msg, L" ");
			_itow(rno+1, msg+wcslen(msg), 10);
			}
		}
	if (nsirit == 0)
		return;
	if (nsirit > 1) {
		inputint_prompt(sirits, msg, &ch);
		if (ch == ESC)
			return;
		}
	if (sirits[0] > 0 && sirits[0] <= NREGNLY && regnly[sirits[0]-1] == LID_SIRIT)
		reconnectSirit[sirits[0]-1] = 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Sirityhteydenpollaus1Click(TObject *Sender)
{
	int sirits[5], nsirit = 0;
	wchar_t ch, msg[100] = L"Valitse laitenumeroista";

	memset(sirits, 0, sizeof(sirits));
	for (int rno = 0; rno < NREGNLY && nsirit < 5; rno++) {
		if (regnly[rno] == LID_SIRIT) {
			sirits[nsirit++] = rno+1;
			wcscat(msg, L" ");
			_itow(rno+1, msg+wcslen(msg), 10);
			}
		}
	if (nsirit == 0)
		return;
	if (nsirit > 1) {
		inputint_prompt(sirits, msg, &ch);
		if (ch == ESC)
			return;
		}

	int k = SiritPoll[sirits[0]-1] / 1000;
	inputint_prompt(&k, L"Annan pollausväli sekunteina. 0 : ei pollausta", &ch, this);
	if (ch != ESC)
		SiritPoll[sirits[0]-1] = 1000 * k;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::DGajat2KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	kasitteleSpecialKey(Key, Shift);
/*
	if (Key != 0 && Key == IkkParam.KeyCode && Shift == IkkParam.KeyShift) {
		AikaKey = true;
		Key = 0;
		lisaaAika();
		}
	else
		AikaKey = false;
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::oistuvantiedonvaroitus1Click(TObject *Sender)
{
	int kesto;
	wchar_t ch;

	kesto = JoKirjattuKesto / 1000;
	inputint_prompt(&kesto, L"Anna varoitusilmoituksen kesto (sek). 0: ei varoiteta", &ch, this);
	if (ch != ESC)
		JoKirjattuKesto = 1000*kesto;
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
   pBitmap->Canvas->Font->Size = IkkParam.FontSize;
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


void __fastcall TFormAjanotto::DGajat1SetEditText(TObject *Sender, int ACol, int ARow,
		  const UnicodeString Value)
{
	if (IkkParam.SyottoTila && ARow > 0 && ARow == edRow &&
		((ACol == cAika && IkkParam.SyottoTila >= 3 && IkkParam.SyottoTila <= 5) ||
		 (ACol == cKno && IkkParam.SyottoTila % 3 >= 1) ||
		 (ACol == cPst && IkkParam.SyottoTila % 3 == 2) ||
		 (ACol == cBdg && IkkParam.SyottoTila == 6))) {
		 Cells[0][ARow][ACol].text = Value;
		 }
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::DGajat1GetEditText(TObject *Sender, int ACol, int ARow,
		  UnicodeString &Value)
{
	if (IkkParam.SyottoTila && ARow > 0 &&
		((ACol == cAika && IkkParam.SyottoTila >= 3 && IkkParam.SyottoTila <= 5) ||
		 (ACol == cKno && IkkParam.SyottoTila % 3 >= 1) ||
		 (ACol == cPst && IkkParam.SyottoTila % 3 == 2) ||
		 (ACol == cBdg && IkkParam.SyottoTila == 6))) {
		 Value = Cells[0][ARow][ACol].text;
		 edRow = ARow;
		 edCol = ACol;
		 }
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Pieninmahdollinen01ms1Click(TObject *Sender)
{
	if (Sender == (TObject *)Pieninajoissankyv1ms1) {
		Pieninajoissankyv1ms1->Checked = true;
		Pieninmahdollinen01ms1->Checked = false;
		Sijoitukseenvaikuttavalisys1->Checked = false;
		IkkParam.AikaLisays = 10;
		}
	else {
		Pieninmahdollinen01ms1->Checked = true;
		Pieninajoissankyv1ms1->Checked = false;
		Sijoitukseenvaikuttavalisys1->Checked = false;
		IkkParam.AikaLisays = 1;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Sijoitukseenvaikuttavalisys1Click(TObject *Sender)
{
	Pieninmahdollinen01ms1->Checked = false;
	Pieninajoissankyv1ms1->Checked = false;
	Sijoitukseenvaikuttavalisys1->Checked = true;
	IkkParam.AikaLisays = pvparam[k_pv].pyor[2]*AIKAJAK;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Avaadebugtiedosto1Click(TObject *Sender)
{
#ifdef DBGFILE
	opendbgfile();
#endif
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Suljedebugtiedosto1Click(TObject *Sender)
{
#ifdef DBGFILE
	closedbgfile();
#endif
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Viimeistenaikojenosuusnytst1Click(TObject *Sender)

{
	wchar_t ch;

	inputint_prompt(&IkkParam.ViimOsuus, L"Anna osuus prosentteina", &ch, this);
	if (IkkParam.ViimOsuus < 5)
	   IkkParam.ViimOsuus == 5;
	if (IkkParam.ViimOsuus > 95)
	   IkkParam.ViimOsuus == 95;
	FormResize(Sender);
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

void __fastcall TFormAjanotto::Vaihdanytetynaikajonontiedosto1Click(TObject *Sender)

{
	wchar_t flnm[102] = L"", ch;
	wcsncpy(flnm, aikajono[IkkParam.Jono]->aikafname, 100);
	inputstr_prompt(flnm, 100, L"Anna uuden tiedoston nimi", &ch, this);
	if (ch != ESC) {
		wcsncpy(aikajono[IkkParam.Jono]->aikafname, flnm, 100);
		suljeajat(IkkParam.Jono, false);
		initajat(IkkParam.Jono, 1);
		}
	BtnPaivitaClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Seuraavaer1Click(TObject *Sender)
{
	if (era_akt < MAXERA-1) {
		Caption = L"Ajanotto - " + UnicodeString(Ajanotto) + L" - Erä " + (++era_akt);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Edellinener1Click(TObject *Sender)
{
	if (era_akt > 1) {
		Caption = L"Ajanotto - " + UnicodeString(Ajanotto) + L" - Erä " + (--era_akt);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Lhtryhmnkoko1Click(TObject *Sender)
{
	inputint_prompt(&kilpparam.lahtoryhma, L"Lähtöryhmän koko", 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::HakuEmitinpalvelimelta1Click(TObject *Sender)
{
	if (FormEmitNouto == NULL)
		FormEmitNouto = new TFormEmitNouto(this);
	FormEmitNouto->Show();
	if (FormEmitNouto->WindowState == wsMinimized)
		FormEmitNouto->WindowState = wsNormal;
	FormEmitNouto->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::DGajat1Exit(TObject *Sender)
{
	naytaAjat1();
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Kaavakkeenoletuspiste1Click(TObject *Sender)
{
	wchar_t st[20] = L"A", ch = L' ';
	if (IkkParam.OletusPiste >= -2 && IkkParam.OletusPiste < 1)
		st[0] = L"ALM"[IkkParam.OletusPiste+2];
	else
		_itow(IkkParam.OletusPiste, st, 10);
	inputstr_prompt(st, 3, L"Anna pisteen tunnus M, L tai A tai väliaikapisteen numero", &ch, this);
	if (ch != ESC) {
		elimwbl(st);
		switch(towupper(st[0])) {
			case L'A':
				IkkParam.OletusPiste = -2;
				break;
			case L'L':
				IkkParam.OletusPiste = -1;
				break;
			case L'M':
				IkkParam.OletusPiste = 0;
				break;
			default:
				IkkParam.OletusPiste = _wtoi(st);
			}
		}
	CBPisteVal->ItemIndex = IkkParam.OletusPiste + 2;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::CBPisteValChange(TObject *Sender)
{
	IkkParam.OletusPiste = CBPisteVal->ItemIndex - 2;
}
//---------------------------------------------------------------------------

void __fastcall TFormAjanotto::Vritlhdekoodinperusteella1Click(TObject *Sender)
{
	wchar_t ch;
	inputstr_prompt(varitettavat, 50, L"Luettele väritettävät koodit välilyöntien erottamina", &ch, this);
}
//---------------------------------------------------------------------------

