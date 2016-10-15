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
#include <string>
#pragma hdrstop

#include "TakaaAjoUnit.h"
#include "HkDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern TTakaaAjoForm *TakaaAjoForm;
static int finsrj[1] = {-1};
static int koklkm[1];
static int finlkm[1];
static int finalku[1];
typedef struct {
	UINT32 tls;
	int dkilp;
	} jrjtp;
static jrjtp *jrjarr;
static long tkarki;
static int kerroin = 1, jakaja = 1, tulkerr = 20;
static int ntls1 = 0, ntls2 = 0, nhyl1 = 0, nkesk1 = 0, npois1 = 0;

//---------------------------------------------------------------------------
__fastcall TTakaaAjoForm::TTakaaAjoForm(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
__fastcall TTakaaAjoForm::~TTakaaAjoForm(void)
{
	if (jrjarr) {
	   free(jrjarr);
	   jrjarr = NULL;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTakaaAjoForm::EditMukaanChange(TObject *Sender)
{
	if (EditMukaan->Text.Length()<1)
		return;
	finlkm[0] = _wtoi(EditMukaan->Text.c_str());
}

//---------------------------------------------------------------------------
void __fastcall TTakaaAjoForm::AalkuChange(TObject *Sender)
{
	if (Aalku->Text.Length()>0)
		finalku[0] = Aalku->Text.ToInt();
}
//---------------------------------------------------------------------------
void __fastcall TTakaaAjoForm::CBAfinChange(TObject *Sender)
{
	wchar_t st[14];
	int kesto, lisa, pKarki = 0;

	lisa = 0;
	switch (RGPuolita->ItemIndex) {
		case 0:
			kerroin = 1;
			jakaja = 1;
			break;
		case 1:
			kerroin = 1;
			jakaja = 2;
			break;
		case 2:
			kerroin = _wtoi(EdKerroin->Text.c_str());
			jakaja = _wtoi(EdJakaja->Text.c_str());
			EdKerroin->Text = kerroin;
			EdJakaja->Text = jakaja;
			break;
		}
	if (kilpparam.kilplaji == L'A')
		lisa = SEK/2;
	kesto = EditKesto->Text.ToInt();
	finsrj[0] = CBAfin->ItemIndex - 1;
	if (jrjarr) {
	   free(jrjarr);
	   jrjarr = NULL;
		}
	if (finsrj[0] >= 0) {
		int n = 0;

		jrjarr = (jrjtp *) calloc(nilm[finsrj[0]], sizeof(jrjtp));
		AIKATOWSTRS(st, Sarjat[finsrj[0]].enslahto[k_pv+1], t0);
		st[8] = 0;
		EnsLahtoA->Text = st;
		AIKATOWSTRS(st, SEK*Sarjat[finsrj[0]].lahtovali[k_pv+1], 0);
		st[8] = 0;
		LahtoValiA->Text = st+3;
		EdtAalto->Text = Sarjat[finsrj[0]].pari[k_pv+1];
		Aalku->Text = UnicodeString(Sarjat[finsrj[0]].bibalku[k_pv+1]);
		finalku[0] = Sarjat[finsrj[0]].bibalku[k_pv+1];
		EditIlm->Text = UnicodeString(nilm[finsrj[0]]);
		ntls1 = 0;
		ntls2 = 0;
		nhyl1 = 0;
		nkesk1 = 0;
		npois1 = 0;
/*
		if (Sarjat[finsrj[0]].pisteytulos()) {
			for (int d = 1; d < nrec; d++) {
				kilptietue kilp;
				UINT32 tls;

				kilp.GETREC(d);
				if (kilp.kilpstatus || kilp.Sarja() != finsrj[0])
					continue;
				if (kilp.lasna(k_pv+1) && kilp.pisteet[0] > pKarki)
					pKarki = kilp.pisteet[0];
				}
			pKarki -= Sarjat[kilp.Sarja()].pistekaava[k_pv+1].vpist;
			}
*/
		for (int d = 1; d < nrec; d++) {
			kilptietue kilp;
			UINT32 tls;

			kilp.GETREC(d);
			if (kilp.kilpstatus || kilp.Sarja() != finsrj[0])
				continue;
			if (!kilp.lasna(k_pv+1)) {
				npois1++;
				tls = 0xffffffff;
				}
			else if ((!Sarjat[kilp.Sarja()].pisteytulos() && (tls = kilp.ytulos(k_pv, 2)) > 0) ||
				(Sarjat[kilp.Sarja()].pisteytulos() &&
				 (tls = PisteAika(&Sarjat[kilp.Sarja()].pistekaava[k_pv+1], kilp.pisteet[0]) - pKarki) != -pKarki)) {
			   tls = tulkerr * tls + kilp.id();
			   ntls1++;
			   }
			else {
				switch (kilp.tark(k_pv)) {
					case L'H':
					case L'M':
						nhyl1++;
						tls = 0xffffffffU - 30000U + (UINT32)kilp.id();
						break;
					case L'K':
					case L'O':
					case L'X':
						nkesk1++;
						tls = 0xffffffffU - 20000U + (UINT32)kilp.id();
						break;
					default:
						tls = 0xffffffffU - 10000U + (UINT32)kilp.id();
						break;
					}
				}
			jrjarr[n].tls = tls;
			jrjarr[n].dkilp = d;
			n++;
			}
		koklkm[0] = n;
		if (n) {
			int i;

			qsortint(jrjarr, n, sizeof(jrjtp), 0, 3);
			tkarki = jrjarr[0].tls / tulkerr;
			for (i = 0; i < n; i++) {
				int tls;

				if (jrjarr[i].tls > 0xffffffffU - 30000U)
					break;
				tls = jrjarr[i].tls / tulkerr;
				if (tls - tkarki > MINUUTTI * kesto + lisa)
					break;
				ntls2++;
				}

			}
		EditTls->Text = UnicodeString(ntls1);
		EditHyl->Text = UnicodeString(nhyl1);
		EditKesk->Text = UnicodeString(nkesk1);
		EdtLasna->Text = UnicodeString(n-npois1);
		EditMukaan->Text = UnicodeString(n-npois1);
//		EditTAlkm->Text = UnicodeString(ntls2);
		}
	TakaaAjoLkm();
}

//---------------------------------------------------------------------------
void __fastcall TTakaaAjoForm::EnsLahtoAExit(TObject *Sender)
{
	wchar_t st[14] = L"";

	if (finsrj[0] >= 0) {
		wcsncpy(st, (EnsLahtoA->Text + L",00").c_str(), 13);
		Sarjat[finsrj[0]].enslahto[k_pv+1] = wstrtoaika_vap(st, t0);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTakaaAjoForm::LahtoValiAExit(TObject *Sender)
{
	wchar_t st[14] = L"";

	if (finsrj[0] >= 0) {
		wcsncpy(st, (L"00." + LahtoValiA->Text + L",00").c_str(), 13);
		Sarjat[finsrj[0]].lahtovali[k_pv+1] = wstrtoaika_vap(st, 0) / SEK;
	}
	TakaaAjoLkm();
}
//---------------------------------------------------------------------------
void __fastcall TTakaaAjoForm::EdtAaltoExit(TObject *Sender)
{
	if (finsrj[0] >= 0) {
		Sarjat[finsrj[0]].pari[k_pv+1] = _wtoi(EdtAalto->Text.c_str());
		}
	TakaaAjoLkm();
}
//---------------------------------------------------------------------------
void __fastcall TTakaaAjoForm::Button23Click(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------
int etsivapaakno(void)
{
	int kno;

	kno = finalku[0] + finlkm[0];
	while (getpos(kno) > 0) {
		kno++;
		if (kno > maxkilpno)
			kno = minkilpno;
		}
	return(kno);
}
//---------------------------------------------------------------------------
void __fastcall TTakaaAjoForm::Button22Click(TObject *Sender)
{
	int ntajo, takesto, nmukaan;
	kilptietue kilp;
	bool takajo;
	bool UusiKno;
	int kno, srj, kk, kn;
	int ensl, lvali, aalto;

	switch (RGPuolita->ItemIndex) {
		case 0:
			kerroin = 1;
			jakaja = 1;
			break;
		case 1:
			kerroin = 1;
			jakaja = 2;
			break;
		case 2:
			kerroin = _wtoi(EdKerroin->Text.c_str());
			jakaja = _wtoi(EdJakaja->Text.c_str());
			EdKerroin->Text = kerroin;
			EdJakaja->Text = jakaja;
			break;
		}
	if (finsrj[0] < 0)
		return;
	srj = finsrj[0];
	if (wcswcind(kilpparam.kilplaji, L"NP") >= 0 && Sarjat[srj].kaava[k_pv+1][VALUKU+1].kerroin == 0) {
		Application->MessageBoxW(L"Pistekaavan kerroin virheellinen", L"Virhe", MB_OK);
		return;
		}
	takajo = RGtakajo->ItemIndex == 0;
	UusiKno = RGnumeroi->ItemIndex == 0;
	ntajo = _wtoi(EditTAlkm->Text.c_str());
	takesto = _wtoi(EditKesto->Text.c_str()) * MINUUTTI;
	nmukaan = _wtoi(EditMukaan->Text.c_str());
	kno = finalku[0];
	ensl = Sarjat[finsrj[0]].enslahto[k_pv+1];
	lvali = SEK * Sarjat[finsrj[0]].lahtovali[k_pv+1];
	if (!takajo) {
		ensl += (nmukaan - 1) * lvali;
		kno += nmukaan - 1;
		takesto = 0;
		ntajo = 0;
		lvali = -lvali;
		}
	aalto = Sarjat[finsrj[0]].pari[k_pv+1];
	if (aalto < 1)
		aalto = 1;
	EnterCriticalSection(&tall_CriticalSection);
	kk = 1;
	for (int i = 0; i < koklkm[0]; i++) {
		int td;

		kilp.GETREC(jrjarr[i].dkilp);
		if (i >= finlkm[0]) {
			int kno_lisa;
			if (RGPois->ItemIndex > 0)
				kilp.set_tark(L"VEP"[RGPois->ItemIndex-1], k_pv+1);
			if ((kno_lisa = _wtoi(EdNoLisa->Text.c_str())) != 0)
				kilp.setId(kilp.kilpno + kno_lisa, k_pv+1);
			}
		else {
			int lero, leroa;
			if (UusiKno) {
				kilp.setId(kno, k_pv+1);
				kno += (takajo ? 1 : -1);
				}
			if (wcswcind(kilpparam.kilplaji, L"NP") < 0) {

				lero = (pyoristatls((jrjarr[i].tls)/tulkerr, 4) -
						pyoristatls((jrjarr[0].tls)/tulkerr, 4))*kerroin/jakaja;
				if (CBTallEroSakkona->Checked)
					kilp.pv[k_pv+1].pvpisteet[1] = lero;
				if (kilpparam.kilplaji == L'A') {
					leroa = lero;
					lero =  pyoristatls(((jrjarr[i].tls - jrjarr[0].tls)/tulkerr)*kerroin/jakaja, 4);
					kilp.pv[k_pv].sakko = lero - leroa;
					}
				}
			else {
				lero = (jrjarr[i].tls/tulkerr - jrjarr[0].tls/tulkerr) *
					Sarjat[srj].kaava[k_pv+1][VALUKU+1].jakaja/(-Sarjat[srj].kaava[k_pv+1][VALUKU+1].kerroin)/KSEK;
				}
			if (i >= ntajo) {
				lero = takesto + ((kk + aalto - 1) / aalto) * lvali;
				kk++;
				}
			kilp.tall_lajat_pv(ensl + lero, k_pv+1);
			if (CBVahKarki->Checked) {
				kilp.tall_tulos_pv(lero + pvparam[k_pv].pyor[1], k_pv);
				kilp.pv[k_pv].sakko = 0;
				}
			}
		kilp.tallenna(jrjarr[i].dkilp, 0, 0, 0, NULL);
		}
	LeaveCriticalSection(&tall_CriticalSection);
	Application->MessageBoxW(L"Lähtöjärjestys laadittu", L"Valmis", MB_OK);
}

//---------------------------------------------------------------------------


void __fastcall TTakaaAjoForm::EditKestoExit(TObject *Sender)
{
	TakaaAjoLkm();
/*
	int takesto, i;

	takesto = EditKesto->Text.ToInt() * MINUUTTI;
	for (i = 0; i < ntls1; i++) {
		if (((jrjarr[i].tls / 1000 - jrjarr[0].tls / 1000) / SEK) * SEK > takesto)
			break;
		}
	EditTAlkm->Text = UnicodeString(i);
*/
}
//---------------------------------------------------------------------------

void __fastcall TTakaaAjoForm::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Monivaiheiset kisat");
}
//---------------------------------------------------------------------------

void __fastcall TTakaaAjoForm::RGtakajoClick(TObject *Sender)
{
	RGPuolita->Enabled = RGtakajo->ItemIndex == 0;
	CBVahKarki->Enabled = RGtakajo->ItemIndex == 0;
	Label6->Enabled = RGtakajo->ItemIndex == 0;
	EditKesto->Enabled = RGtakajo->ItemIndex == 0;
	EditTAlkm->Text = L"0";
	EditKesto->Text = L"0";
	TakaaAjoLkm();
}
//---------------------------------------------------------------------------

int __fastcall TTakaaAjoForm::TakaaAjoLkm(void)
{
	int takesto, nmukaan;
	kilptietue kilp;
	int srj, i;

	if (RGtakajo->ItemIndex ==1)
		return(0);
	switch (RGPuolita->ItemIndex) {
		case 0:
			kerroin = 1;
			jakaja = 1;
			break;
		case 1:
			kerroin = 1;
			jakaja = 2;
			break;
		case 2:
			kerroin = _wtoi(EdKerroin->Text.c_str());
			jakaja = _wtoi(EdJakaja->Text.c_str());
			EdKerroin->Text = kerroin;
			EdJakaja->Text = jakaja;
			break;
		}
	if (finsrj[0] < 0)
		return(0);
	srj = finsrj[0];
	if (wcswcind(kilpparam.kilplaji, L"NP") >= 0 && Sarjat[srj].kaava[k_pv+1][VALUKU+1].kerroin == 0) {
		return(0);
		}
	takesto = _wtoi(EditKesto->Text.c_str()) * MINUUTTI;
	nmukaan = _wtoi(EditMukaan->Text.c_str());
	for (i = 0; i < nilm[srj] && i < nmukaan; i++) {
		int lero;
		if (jrjarr[i].dkilp <= 0)
			break;
		kilp.GETREC(jrjarr[i].dkilp);
		if (!kilp.ytulos(k_pv))
			break;
		if (wcswcind(kilpparam.kilplaji, L"NP") < 0) {

			if (kilpparam.kilplaji == L'A') {
				lero =  pyoristatls(((jrjarr[i].tls - jrjarr[0].tls)/tulkerr)*kerroin/jakaja, 4);
				}
			else
				lero = (pyoristatls((int)(jrjarr[i].tls/tulkerr), 4) -
					pyoristatls((int)(jrjarr[0].tls/tulkerr), 4))*kerroin/jakaja;
			}
		else {
			lero = (jrjarr[i].tls/tulkerr - jrjarr[0].tls/tulkerr) *
				Sarjat[srj].kaava[k_pv+1][VALUKU+1].jakaja/(-Sarjat[srj].kaava[k_pv+1][VALUKU+1].kerroin)/KSEK;
			}

		if (lero > takesto)
			break;
		}
	EditTAlkm->Text = i;
	return(i);
}
//---------------------------------------------------------------------------

void __fastcall TTakaaAjoForm::RGPuolitaClick(TObject *Sender)
{
	Label8->Enabled = RGPuolita->ItemIndex == 2;
	Label9->Enabled = Label8->Enabled;
	EdKerroin->Enabled = Label8->Enabled;
	EdJakaja->Enabled = Label8->Enabled;
}
//---------------------------------------------------------------------------

void __fastcall TTakaaAjoForm::RGnumeroiClick(TObject *Sender)
{
	Label4->Enabled = RGnumeroi->ItemIndex == 0;
	Aalku->Enabled = Label4->Enabled;
}
//---------------------------------------------------------------------------

void __fastcall TTakaaAjoForm::FormShow(TObject *Sender)
{
	int itma, itmb, itmc;

	if (wcswcind(kilpparam.kilplaji, L"NP") >= 0) {
		GroupBox1->Visible = false;
		tulkerr = 1;
		}
	if (!kilpparam.bibvaihtelee) {
		RGnumeroi->ItemIndex = 1;
		RGnumeroi->Enabled = false;
		}
	else
		RGnumeroi->Enabled = true;
	itma = CBAfin->ItemIndex;
	CBAfin->Items->Clear();
	CBAfin->Items->Add(L"Ei sarjaa");
	for (int srj = 0; srj < sarjaluku; srj++) {
		CBAfin->Items->Add(Sarjat[srj].sarjanimi);
	}
	CBAfin->ItemIndex = itma;
	finlkm[0] = _wtoi(EditMukaan->Text.c_str());
	finalku[0] = _wtoi(Aalku->Text.c_str());
}
//---------------------------------------------------------------------------


