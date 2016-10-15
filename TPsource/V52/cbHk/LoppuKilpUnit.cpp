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
#include <string>

#include "LoppuKilpUnit.h"
#include "WinHk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern TLoppuKilpForm *LoppuKilpForm;
int finsrj[5] = {-1, -1, -1, -1, -1};
int finlkm[5];
int finylkm[5];
int finalku[5];
int karsknolisa, karsemitlisa;
int karssarjat[10] = {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1};

typedef struct {
	int sj;
	int d;
	wchar_t seura[LSEURA+1];
	} srajrjtp;

srajrjtp *srajrj[5];

//---------------------------------------------------------------------------
__fastcall TLoppuKilpForm::TLoppuKilpForm(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	Lkm[0][0] = Alkm1;
	Lkm[0][1] = Alkm2;
	Lkm[0][2] = Alkm3;
	Lkm[0][3] = Alkm4;
	Lkm[0][4] = Alkm5;
	Lkm[0][5] = Alkm6;
	Lkm[0][6] = Alkm7;
	Lkm[0][7] = Alkm8;
	Lkm[0][8] = Alkm9;
	Lkm[0][9] = Alkm10;
	Lkm[1][0] = Blkm1;
	Lkm[1][1] = Blkm2;
	Lkm[1][2] = Blkm3;
	Lkm[1][3] = Blkm4;
	Lkm[1][4] = Blkm5;
	Lkm[1][5] = Blkm6;
	Lkm[1][6] = Blkm7;
	Lkm[1][7] = Blkm8;
	Lkm[1][8] = Blkm9;
	Lkm[1][9] = Blkm10;
	Lkm[2][0] = Clkm1;
	Lkm[2][1] = Clkm2;
	Lkm[2][2] = Clkm3;
	Lkm[2][3] = Clkm4;
	Lkm[2][4] = Clkm5;
	Lkm[2][5] = Clkm6;
	Lkm[2][6] = Clkm7;
	Lkm[2][7] = Clkm8;
	Lkm[2][8] = Clkm9;
	Lkm[2][9] = Clkm10;
	Lkm[3][0] = Dlkm1;
	Lkm[3][1] = Dlkm2;
	Lkm[3][2] = Dlkm3;
	Lkm[3][3] = Dlkm4;
	Lkm[3][4] = Dlkm5;
	Lkm[3][5] = Dlkm6;
	Lkm[3][6] = Dlkm7;
	Lkm[3][7] = Dlkm8;
	Lkm[3][8] = Dlkm9;
	Lkm[3][9] = Dlkm10;
	Lkm[4][0] = Elkm1;
	Lkm[4][1] = Elkm2;
	Lkm[4][2] = Elkm3;
	Lkm[4][3] = Elkm4;
	Lkm[4][4] = Elkm5;
	Lkm[4][5] = Elkm6;
	Lkm[4][6] = Elkm7;
	Lkm[4][7] = Elkm8;
	Lkm[4][8] = Elkm9;
	Lkm[4][9] = Elkm10;
	Kars[0] = Kars1;
	Kars[1] = Kars2;
	Kars[2] = Kars3;
	Kars[3] = Kars4;
	Kars[4] = Kars5;
	Kars[5] = Kars6;
	Kars[6] = Kars7;
	Kars[7] = Kars8;
	Kars[8] = Kars9;
	Kars[9] = Kars10;
	KTls[0] = KTls1;
	KTls[1] = KTls2;
	KTls[2] = KTls3;
	KTls[3] = KTls4;
	KTls[4] = KTls5;
	KTls[5] = KTls6;
	KTls[6] = KTls7;
	KTls[7] = KTls8;
	KTls[8] = KTls9;
	KTls[9] = KTls10;
	KIlm[0] = KIlm1;
	KIlm[1] = KIlm2;
	KIlm[2] = KIlm3;
	KIlm[3] = KIlm4;
	KIlm[4] = KIlm5;
	KIlm[5] = KIlm6;
	KIlm[6] = KIlm7;
	KIlm[7] = KIlm8;
	KIlm[8] = KIlm9;
	KIlm[9] = KIlm10;
	CBfin[0] = CBAfin;
	CBfin[1] = CBBfin;
	CBfin[2] = CBCfin;
	CBfin[3] = CBDfin;
	CBfin[4] = CBEfin;
	CBla[0] = CBlaA;
	CBla[1] = CBlaB;
	CBla[2] = CBlaC;
	CBla[3] = CBlaD;
	CBla[4] = CBlaE;
	Loput[0] = LoputA;
	Loput[1] = LoputB;
	Loput[2] = LoputC;
	Loput[3] = LoputD;
	Loput[4] = LoputE;
	Lkm0[0] = Alkm0;
	Lkm0[1] = Blkm0;
	Lkm0[2] = Clkm0;
	Lkm0[3] = Dlkm0;
	Lkm0[4] = Elkm0;
	LahtoVali[0] = LahtoValiA;
	LahtoVali[1] = LahtoValiB;
	LahtoVali[2] = LahtoValiC;
	LahtoVali[3] = LahtoValiD;
	LahtoVali[4] = LahtoValiE;
	EnsLahto[0] = EnsLahtoA;
	EnsLahto[1] = EnsLahtoB;
	EnsLahto[2] = EnsLahtoC;
	EnsLahto[3] = EnsLahtoD;
	EnsLahto[4] = EnsLahtoE;
	Alku[0] = Aalku;
	Alku[1] = Balku;
	Alku[2] = Calku;
	Alku[3] = Dalku;
	Alku[4] = Ealku;
	Aalto[0] = AaltoA;
	Aalto[1] = AaltoB;
	Aalto[2] = AaltoC;
	Aalto[3] = AaltoD;
	Aalto[4] = AaltoE;
	CBPoimi[0] = CBPoimiA;
	CBPoimi[1] = CBPoimiB;
	CBPoimi[2] = CBPoimiC;
	CBPoimi[3] = CBPoimiD;
	CBPoimi[4] = CBPoimiE;
	CBTakajo[0] = CBtakajoA;
	CBTakajo[1] = CBtakajoB;
	CBTakajo[2] = CBtakajoC;
	CBTakajo[3] = CBtakajoD;
	CBTakajo[4] = CBtakajoE;
}
//---------------------------------------------------------------------------
void __fastcall TLoppuKilpForm::Tyhjenna(void)
{
	for (int ifin = 0; ifin < 5; ifin++) {
		finsrj[ifin] = -1;
		finlkm[ifin] = 0;
		finalku[ifin] = 0;
		CBfin[ifin]->ItemIndex = 0;
		Lkm0[ifin]->Text = L"0";
		Loput[ifin]->Checked = false;
		CBla[ifin]->Checked = true;
		EnsLahto[ifin]->Text = L"12.00.00";
		LahtoVali[ifin]->Text = L"120";
		Aalto[ifin]->Text = L"1";
		Alku[ifin]->Text = L"0";
		CBPoimi[ifin]->ItemIndex = 0;
		CBTakajo[ifin]->Checked = false;
		}
	for (int ikrs = 0; ikrs < 10; ikrs++) {
		karssarjat[ikrs] = -1;
		Kars[ikrs]->Text = L"";
		KIlm[ikrs]->Text = L"0";
		KTls[ikrs]->Text = L"0";
		for (int j = 0; j < 5; j++)
			Lkm[j][ikrs]->Text =  L"0";
		}
}
//---------------------------------------------------------------------------
void tarksrjlkm(int srj, int *lkm)
{
	int p, sj, sj1, lj, poimi = 0;
	kilptietue kilp;
	long tls, tls1 = 0;

	for (int j = 0; j < 5; j++) {
		lkm[j] = 0;
		if (finsrj[j] >= 0 && LoppuKilpForm->CBPoimi[j]->ItemIndex > poimi)
			poimi = LoppuKilpForm->CBPoimi[j]->ItemIndex;
		}
	p = -1;
	lj = -1;
	sj = 0;
	sj1 = 1;
	while (1) {
		p = askellasarja(srj, 0, p, &lj);
		if (p < 0 || (lj > 0 && poimi == 0)
			|| (lj > 2 && poimi == 1)) {
			break;
			}
		kilp.GETREC(p);
		tls = kilp.tulos_pv(k_pv, true, 2);
		sj++;
		if (tls != tls1)
			sj1 = sj;
		tls1 = tls;
		if (!tls)
			sj1 = 90000;
		if (sj1 <= finlkm[0] && (tls ||
			(LoppuKilpForm->CBPoimi[0]->ItemIndex == 1 && kilp.tark(0) != L'E') ||
			LoppuKilpForm->CBPoimi[0]->ItemIndex == 2))
			lkm[0] = sj;
		else if (finlkm[1] && sj1 <= finlkm[0] + finlkm[1] && (tls ||
			(LoppuKilpForm->CBPoimi[1]->ItemIndex == 1 && kilp.tark(0) != L'E') ||
			LoppuKilpForm->CBPoimi[1]->ItemIndex == 2))
			lkm[1] = sj - lkm[0];
		else if (finlkm[2] && sj1 <= finlkm[0] + finlkm[1] + finlkm[2] && (tls ||
			(LoppuKilpForm->CBPoimi[2]->ItemIndex == 1 && kilp.tark(0) != L'E') ||
			LoppuKilpForm->CBPoimi[2]->ItemIndex == 2))
			lkm[2] = sj - lkm[0] - lkm[1];
		else if (finlkm[3] && sj1 <= finlkm[0] + finlkm[1] + finlkm[2] + finlkm[3] && (tls ||
			(LoppuKilpForm->CBPoimi[3]->ItemIndex == 1 && kilp.tark(0) != L'E') ||
			LoppuKilpForm->CBPoimi[3]->ItemIndex == 2))
			lkm[3] = sj - lkm[0] - lkm[1] - lkm[2];
		else if (finlkm[4] && sj1 <= finlkm[0] + finlkm[1] + finlkm[2] + finlkm[3] + finlkm[4])
			lkm[4] = sj - lkm[0] - lkm[1] - lkm[2] - lkm[3];
		}
}
//---------------------------------------------------------------------------
void __fastcall TLoppuKilpForm::setkarslkm(int i)
{
	int lkm[5] = {0, 0, 0, 0, 0}, srj;

	srj = karssarjat[i];
	if (srj >= 0)
		tarksrjlkm(srj, lkm);
	KIlm[i]->Text = UnicodeString(nilm[srj]);
	KTls[i]->Text = UnicodeString(ntulos[srj][0]);
	for (int j = 0; j < 5; j++)
		Lkm[j][i]->Text =  UnicodeString(lkm[j]);
}
//---------------------------------------------------------------------------
void __fastcall TLoppuKilpForm::LisaaKarsSrj(int i)
{
	karssarjat[i] = ListSarjat->ItemIndex;
	Kars[i]->Text = Sarjat[ListSarjat->ItemIndex].sarjanimi;
	setkarslkm(i);
}
//---------------------------------------------------------------------------


void __fastcall TLoppuKilpForm::BtnLisa1Click(TObject *Sender)
{
	LisaaKarsSrj(0);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnLisa2Click(TObject *Sender)
{
	LisaaKarsSrj(1);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnLisa3Click(TObject *Sender)
{
	LisaaKarsSrj(2);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnLisa4Click(TObject *Sender)
{
	LisaaKarsSrj(3);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnLisa5Click(TObject *Sender)
{
	LisaaKarsSrj(4);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnLisa6Click(TObject *Sender)
{
	LisaaKarsSrj(5);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnLisa7Click(TObject *Sender)
{
	LisaaKarsSrj(6);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnLisa8Click(TObject *Sender)
{
	LisaaKarsSrj(7);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnLisa9Click(TObject *Sender)
{
	LisaaKarsSrj(8);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnLisa10Click(TObject *Sender)
{
	LisaaKarsSrj(9);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::PoisKarsSrj(int i)
{
	karssarjat[i] = -1;
	Kars[i]->Text = L"";
	KIlm[i]->Text = L"0";
	KTls[i]->Text = L"0";
	for (int j = 0; j < 5; j++)
		Lkm[j][i]->Text = L"0";
}
//---------------------------------------------------------------------------
void __fastcall TLoppuKilpForm::BtnPois1Click(TObject *Sender)
{
	PoisKarsSrj(0);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnPois2Click(TObject *Sender)
{
	PoisKarsSrj(1);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnPois3Click(TObject *Sender)
{
	PoisKarsSrj(2);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnPois4Click(TObject *Sender)
{
	PoisKarsSrj(3);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnPois5Click(TObject *Sender)
{
	PoisKarsSrj(4);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnPois6Click(TObject *Sender)
{
	PoisKarsSrj(5);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnPois7Click(TObject *Sender)
{
	PoisKarsSrj(6);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnPois8Click(TObject *Sender)
{
	PoisKarsSrj(7);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnPois9Click(TObject *Sender)
{
	PoisKarsSrj(8);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnPois10Click(TObject *Sender)
{
	PoisKarsSrj(9);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::Lkm0Change(int ifin)
{
	if (Lkm0[ifin]->Text.Length()<1)
		return;
	finlkm[ifin] = _wtoi(Lkm0[ifin]->Text.c_str());
	for (int i = 0; i < 10; i++)
		setkarslkm(i);
}
//---------------------------------------------------------------------------
void __fastcall TLoppuKilpForm::Alkm0Change(TObject *Sender)
{
	Lkm0Change(0);
}
//---------------------------------------------------------------------------
void __fastcall TLoppuKilpForm::Blkm0Change(TObject *Sender)
{
	Lkm0Change(1);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::Clkm0Change(TObject *Sender)
{
	Lkm0Change(2);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::Dlkm0Change(TObject *Sender)
{
	Lkm0Change(3);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::Elkm0Change(TObject *Sender)
{
	Lkm0Change(4);
}
//---------------------------------------------------------------------------
void __fastcall TLoppuKilpForm::AlkuChange(int ifin)
{
	if (Alku[ifin]->Text.Length()>0)
		finalku[ifin] = _wtoi(Alku[ifin]->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::AalkuChange(TObject *Sender)
{
	AlkuChange(0);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BalkuChange(TObject *Sender)
{
	AlkuChange(1);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::CalkuChange(TObject *Sender)
{
	AlkuChange(2);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::DalkuChange(TObject *Sender)
{
	AlkuChange(3);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::EalkuChange(TObject *Sender)
{
	AlkuChange(4);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::CBfinChange(int ifin)
{
	wchar_t st[14];

	finsrj[ifin] = CBfin[ifin]->ItemIndex - 1;
	if (finsrj[ifin] >= 0) {
		AIKATOWSTRS(st, Sarjat[finsrj[ifin]].enslahto[k_pv+1], t0);
		st[8] = 0;
		EnsLahto[ifin]->Text = st;
		LahtoVali[ifin]->Text = Sarjat[finsrj[ifin]].lahtovali[k_pv+1];
		if (kilpparam.bibvaihtelee) {
			finalku[ifin] = Sarjat[finsrj[ifin]].bibalku[k_pv+1];
			Alku[ifin]->Text = UnicodeString(finalku[ifin]);
			}
		else
			finalku[ifin] = 0;
		Aalto[ifin]->Text = Sarjat[finsrj[ifin]].pari[k_pv+1];
	}
}
//---------------------------------------------------------------------------
void __fastcall TLoppuKilpForm::CBAfinChange(TObject *Sender)
{
	CBfinChange(0);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::CBBfinChange(TObject *Sender)
{
	CBfinChange(1);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::CBCfinChange(TObject *Sender)
{
	CBfinChange(2);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::CBDfinChange(TObject *Sender)
{
	CBfinChange(3);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::CBEfinChange(TObject *Sender)
{
	CBfinChange(4);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::EnsLahtoExit(int ifin)
{
	int t;
	wchar_t as[16];

	t = wstrtoaika_vap(EnsLahto[ifin]->Text.c_str(), t0);
	Sarjat[finsrj[ifin]].enslahto[k_pv+1] = t;
	EnsLahto[ifin]->Text = aikatowstr_cols_n(as, t, t0, 0, 8);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::EnsLahtoAExit(TObject *Sender)
{
	EnsLahtoExit(0);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::EnsLahtoBExit(TObject *Sender)
{
	EnsLahtoExit(1);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::EnsLahtoCExit(TObject *Sender)
{
	EnsLahtoExit(2);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::EnsLahtoDExit(TObject *Sender)
{
	EnsLahtoExit(3);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::EnsLahtoEExit(TObject *Sender)
{
	EnsLahtoExit(4);
}
//---------------------------------------------------------------------------
void __fastcall TLoppuKilpForm::LahtoValiExit(int ifin)
{
	if (finsrj[ifin] >= 0) {
		Sarjat[finsrj[ifin]].lahtovali[k_pv+1] = _wtoi(LahtoVali[ifin]->Text.c_str());
	}
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::LahtoValiAExit(TObject *Sender)
{
	LahtoValiExit(0);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::LahtoValiBExit(TObject *Sender)
{
	LahtoValiExit(1);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::LahtoValiCExit(TObject *Sender)
{
	LahtoValiExit(2);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::LahtoValiDExit(TObject *Sender)
{
	LahtoValiExit(3);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::LahtoValiEExit(TObject *Sender)
{
	LahtoValiExit(4);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::Button23Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------


void __fastcall TLoppuKilpForm::BtnToteutaClick(TObject *Sender)
{
	int p[10], dk, *df[5] = {0, 0, 0, 0, 0}, nksrj, nf[5][10], nft[5] = {0, 0, 0, 0, 0};
	int maxsj[5] = {0, 0, 0, 0, 0};
	int kf[5] = {0, 0, 0, 0, 0}, tllj[10], nkierros;
	kilptietue kilp;

	memset(finylkm, 0, sizeof(finylkm));
	for (int i = 0; i < 10; i++)
		for (int j = 0; j < 5; j++)
			nf[j][i] = _wtoi(Lkm[j][i]->Text.c_str());

	for (int i = 0; i < 10; i++) {
		int nsj = 0;

		if (karssarjat[i] < 0)
			continue;
		if (CBMerkEilah->ItemIndex > 0 || _wtoi(EdtKnoLisa->Text.c_str())) {
			int p = -1, lj = -1, srj;
			srj = karssarjat[i];
			EnterCriticalSection(&tall_CriticalSection);
			while (1) {
				p = askellasarja(srj, 0, p, &lj);
				if (p < 0)
					break;
				kilp.GETREC(p);
				if (CBMerkEilah->ItemIndex > 0)
					kilp.set_tark(L"VEP"[CBMerkEilah->ItemIndex-1], k_pv+1);
				if (_wtoi(EdtKnoLisa->Text.c_str()))
					kilp.setId(kilp.id(k_pv+1) + _wtoi(EdtKnoLisa->Text.c_str()), k_pv+1);
				kilp.tallenna(p, 0, 0, 0, 0);
				}
			LeaveCriticalSection(&tall_CriticalSection);
			}
		p[i] = -1;
		tllj[i] = -1;
		for (int j = 0; j < 5; j++) {
			nft[j] += nf[j][i];
			nsj += nf[j][i];
			if (nsj > maxsj[j])
				maxsj[j] = nsj;
			}
		}
	for (int j = 0; j < 5; j++) {
		if (nft[j]) {
			df[j] = (int *) calloc(nft[j], sizeof(int));
			}
		}
	nkierros = 0;
	for (int i = 9; ;) {
		i = (i + 1) % 10;
		if (i == 0) {
			nkierros++;
			}
		if (nkierros > maxsj[4])
			break;
		if (karssarjat[i] >= 0) {
			if (nkierros > nf[0][i] +  nf[1][i] +  nf[2][i] +  nf[3][i] +  nf[4][i])
				continue;
			while (p[i] < 0 && tllj[i] < 4) {
				tllj[i]++;
				p[i] = jalku[karssarjat[i]][tllj[i]];
				}
			if (p[i] < 0)
				continue;
			if (df[0] && nkierros <= nf[0][i])
				df[0][kf[0]++] = p[i];
			else if (df[1] && nkierros <= nf[0][i] + nf[1][i])
				df[1][kf[1]++] = p[i];
			else if (df[2] && nkierros <= nf[0][i] + nf[1][i] + nf[2][i])
				df[2][kf[2]++] = p[i];
			else if (df[3] && nkierros <= nf[0][i] + nf[1][i] + nf[2][i] + nf[3][i])
				df[3][kf[3]++] = p[i];
			else if (df[4] && nkierros <= nf[0][i] + nf[1][i] + nf[2][i] + nf[3][i] + nf[4][i])
				df[4][kf[4]++] = p[i];
			p[i] = JarjSeur(0, 0, p[i]);
			}
		}
	for (int j = 0; j < 5; j++) {
		int kno, dd;
		long ensl, lvali, aalto;
		bool UusiLA;

		if (finsrj[j] < 0) {
			break;
			}
		if (srajrj[j])
			free(srajrj[j]);
		srajrj[j] = (srajrjtp *) calloc(kf[j]+1, sizeof(srajrjtp));
		UusiLA = CBla[j]->Checked;
		aalto = _wtoi(Aalto[j]->Text.c_str());
		kno = finalku[j];
		ensl = Sarjat[finsrj[j]].enslahto[k_pv+1];
		lvali = SEK * Sarjat[finsrj[j]].lahtovali[k_pv+1];
		EnterCriticalSection(&tall_CriticalSection);
		for (int i = 0; i < kf[j]; i++) {
			int tkarki;
//			long tero;
			int len;

			if (CBTakajo[j]->Checked)
				dk = df[j][i];
			else
				dk = df[j][kf[j] - i - 1];
			kilp.GETREC(dk);
			kilp.setSarja(finsrj[j], k_pv+1);
			if (kno) {
				kilp.setId(kno++, k_pv+1);
				}
			if (CBTakajo[j]->Checked) {
				if (i == 0)
					tkarki = kilp.ytulos(k_pv, 4);
				kilp.pv[k_pv+1].tlahto = ensl + kilp.ytulos(k_pv, 4) - tkarki;
				}
			else if (UusiLA) {
				kilp.pv[k_pv+1].tlahto = ensl + (i / aalto) * lvali;
				}
			kilp.pv[k_pv+1].va[0].vatulos = kilp.pv[k_pv+1].tlahto;
			kilp.set_tark(L'-', k_pv+1);
			kilp.tallenna(dk, 0, 0, 0, NULL);
			srajrj[j][i].d = dk;
			srajrj[j][i].sj = i;
			wcsncpy(srajrj[j][i].seura, kilp.seuratieto(&len), LSEURA);
			finylkm[j]++;
			}
		LeaveCriticalSection(&tall_CriticalSection);
		}
	for (int j = 0; j < 5; j++)
		if (df[j])
			free(df[j]);
	Application->MessageBoxW(L"Loppukilpailut laadittu", L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::Button1Click(TObject *Sender)
{
	TextFl *lafile;

	OpenDialog1->InitialDir = FormMain->CurrentDir;
	if (OpenDialog1->Execute()) {
		wchar_t line[200], st[20], srjst[20];
		int kno, d, nn = 0, kntlkm, srj;
		kilptietue kilp;

		lafile = new TextFl(OpenDialog1->FileName.c_str(), L"rt");

		if (lafile->IsOpen()) {
			EnterCriticalSection(&tall_CriticalSection);
			while (!lafile->Feof()) {
				lafile->ReadLine(line, 200);
				if (wcslen(line) > 10) {
					if ((kntlkm = swscanf(line, L"%d %s %s\n", &kno, st, srjst)) >= 2) {
						if ((d = getpos(kno)) > 0) {
							kilp.GETREC(d);
							if (kilp.sarja == finsrj[1]) {
								if (wcslen(st) == 8) {
									wcscat(st, L",00");
									}
								else if (wcslen(st) == 10) {
									wcscat(st, L"0");
									}
								else if (wcslen(st) == 5) {
									wcscat(st, L".00,00");
									}
								if (wcslen(st) == 11) {
									kilp.pv[k_pv+1].tlahto = WSTRTOAIKA(st, t0);
									kilp.pv[k_pv+1].va[0].vatulos = kilp.pv[k_pv+1].tlahto;
									}
								if (kntlkm > 2 && wcscmp(srjst, Sarjat[kilp.Sarja()].sarjanimi)) {
									if ((srj = haesarja_w(srjst, false)) >= 0)
										kilp.setSarja(srj, k_pv+1);
									}
								kilp.tallenna(d, 0, -1, 0, 0);
								nn++;
								}
							}
						}
					}
				}
			LeaveCriticalSection(&tall_CriticalSection);
			swprintf(line, L"Siirretty %d lähtöaikaa", nn);
			Application->MessageBoxW(line, L"Valmis", MB_OK);
			}
		delete lafile;
		}
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Loppukilpailu");
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::LoputClick(int ifin)
{
	if (Loput[ifin]->Checked) {
		finlkm[ifin] = 99998;
		Lkm0[ifin]->Visible = false;
		}
	else {
		Lkm0[ifin]->Visible = true;
		finlkm[ifin] = _wtoi(Lkm0[ifin]->Text.c_str());
		}
	for (int i = 0; i < 10; i++)
		setkarslkm(i);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::LoputAClick(TObject *Sender)
{
	LoputClick(0);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::LoputBClick(TObject *Sender)
{
	LoputClick(1);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::LoputCClick(TObject *Sender)
{
	LoputClick(2);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::LoputDClick(TObject *Sender)
{
	LoputClick(3);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::LoputEClick(TObject *Sender)
{
	LoputClick(4);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::CBlaAClick(TObject *Sender)
{
	EnsLahtoA->Visible = CBlaA->Checked;
	LahtoValiA->Visible = CBlaA->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::CBlaBClick(TObject *Sender)
{
	EnsLahtoB->Visible = CBlaB->Checked;
	LahtoValiB->Visible = CBlaB->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::CBlaCClick(TObject *Sender)
{
	EnsLahtoC->Visible = CBlaC->Checked;
	LahtoValiC->Visible = CBlaC->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::CBlaDClick(TObject *Sender)
{
	EnsLahtoD->Visible = CBlaD->Checked;
	LahtoValiD->Visible = CBlaD->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::CBlaEClick(TObject *Sender)
{
	EnsLahtoE->Visible = CBlaE->Checked;
	LahtoValiE->Visible = CBlaE->Checked;
}
//---------------------------------------------------------------------------

TextFl *SkriptiFl;
TextFl *SkriptiOFl;

void __fastcall TLoppuKilpForm::Button2Click(TObject *Sender)
{
	if (!SkriptiFl) {
		OpenDialog1->FileName = L"FinSaannot.txt";
		OpenDialog1->DefaultExt = L"txt";
		OpenDialog1->Filter = L"Tekstitiedostot (*.txt) | *.TXT|Kaikki tiedostot (*.*)|*.*";
		if (OpenDialog1->Execute()) {
			SkriptiFl = new TextFl(OpenDialog1->FileName.c_str(), L"rt");
			if (!SkriptiFl->IsOpen()) {
				delete SkriptiFl;
				SkriptiFl = NULL;
				}
			if (!SkriptiFl)
				return;
		   }
		}
	if (SkriptiFl) {
		wchar_t line[200], st[20];
		int srj = -1, ikrs = 0;
		bool uusisaanto = true;

		while (SkriptiFl->IsOpen() && !SkriptiFl->Feof()) {
			wchar_t *p, erotin;
			if (uusisaanto)
				Tyhjenna();
			uusisaanto = false;
			SkriptiFl->ReadLine(line, 198);
			wcsncpy(st, line, 19);
			st[19] = 0;
			upcasewstr(st);
/*
			if (!wmemcmp(st, L"PERUS:", 6)) {
				if ((p = wcstok(line+6, L" ;\t\n")) == NULL)
					continue;
				CBtakajo->Checked = _wtoi(p);
				if ((p = wcstok(NULL, L" ;\t\n")) == NULL)
					continue;
				RGPoimi->ItemIndex = _wtoi(p)-1;
				continue;
				}
*/
			if (!wmemcmp(st+1, L"-FINAALI:", 9)) {
				int ifin;

				ifin = st[0] - L'A';
				for (p = line+10; *p && *p != L';' && *p != L'\t'; p++) ;
				if (*p == 0)
					break;
				erotin = *p;
				if ((p = wcstokch(line+10, erotin)) == NULL)
					break;
				elimwbl(p);
				if (*p == 0) {
					if ((p = wcstokch(NULL, erotin)) == NULL)
						break;
					elimwbl(p);
					}
				srj = haesarja_w(p, false);
				if (srj < 0)
					break;
				finsrj[ifin] = srj;
				CBfin[ifin]->ItemIndex = srj + 1;
				CBfinChange(ifin);
				if ((p = wcstokch(NULL, erotin)) != NULL) {
					elimwbl(p);
					if (_wtoi(p))
						Lkm0[ifin]->Text = p;
					}
				if (p != NULL && (p = wcstokch(NULL, erotin)) != NULL) {
					elimwbl(p);
					if (p[0] >= L'0')
						Loput[ifin]->Checked = _wtoi(p);
					}
				if (p != NULL && (p = wcstokch(NULL, erotin)) != NULL) {
					elimwbl(p);
					if (p[0] >= L'0')
						CBla[ifin]->Checked = _wtoi(p);
					}
				if (p != NULL && (p = wcstokch(NULL, erotin)) != NULL) {
					elimwbl(p);
					if (p[0] >= L'0')
						EnsLahto[ifin]->Text = p;
					}
				if (p != NULL && (p = wcstokch(NULL, erotin)) != NULL) {
					elimwbl(p);
					if (p[0] >= L'0')
						LahtoVali[ifin]->Text = p;
					}
				if (p != NULL && (p = wcstokch(NULL, erotin)) != NULL) {
					elimwbl(p);
					if (p[0] >= L'0')
						Aalto[ifin]->Text = p;
					}
				if (p != NULL && (p = wcstokch(NULL, erotin)) != NULL) {
					elimwbl(p);
					if (kilpparam.bibvaihtelee && p[0] >= L'0')
						Alku[ifin]->Text = p;
					}
				if (p != NULL && (p = wcstokch(NULL, erotin)) != NULL) {
					elimwbl(p);
					if (p[0] >= L'0')
						CBPoimi[ifin]->ItemIndex = _wtoi(p)-1;
					}
				if (p != NULL && (p = wcstokch(NULL, erotin)) != NULL) {
					elimwbl(p);
					if (p[0] >= L'0')
						CBTakajo[ifin]->Checked = _wtoi(p);
					}
				LoputClick(ifin);
				EnsLahtoExit(ifin);
				LahtoValiExit(ifin);
				continue;
				}
			if (!wmemcmp(st, L"LOPPU", 5)) {
				uusisaanto = true;
				if (srj >= 0) {
					int lkm[5] = {0, 0, 0, 0, 0};
					tarksrjlkm(srj, lkm);
					}
				return;
				}
			else {
				if ((p = wcstok(st, L" ;\t\n")) == NULL)
					break;
				srj = haesarja_w(p, false);
				if (srj < 0)
					break;
				karssarjat[ikrs] = srj;
				Kars[ikrs]->Text = Sarjat[srj].sarjanimi;
				setkarslkm(ikrs);
				ikrs++;
				}
			}
		delete SkriptiFl;
		SkriptiFl = NULL;
		}
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	for (int k = 0; k < 5; k++) {
		if (srajrj[k]) {
			free(srajrj[k]);
			srajrj[k] = NULL;
			}
		}
	if (SkriptiFl)
		delete SkriptiFl;
	SkriptiFl = NULL;
	if (SkriptiOFl)
		delete SkriptiOFl;
	SkriptiOFl = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::CBPoimiClick(TObject *Sender)
{
	for (int i = 0; i < 10; i++)
		setkarslkm(i);
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::Lisskriptitiedostoon1Click(TObject *Sender)
{
	static wchar_t flnm[200] = L"FinSaannot.txt";

	if (!SkriptiOFl) {
		SaveDialog1->InitialDir = FormMain->CurrentDir;
		SaveDialog1->FileName = flnm;
		SaveDialog1->DefaultExt = L"txt";
		SaveDialog1->Filter = L"Tekstitiedostot (*.txt) | *.TXT|Kaikki tiedostot (*.*)|*.*";
		if (SaveDialog1->Execute()) {
			SkriptiOFl = new TextFl(SaveDialog1->FileName.c_str(), L"at");
			if (!SkriptiOFl->IsOpen()) {
				delete SkriptiOFl;
				SkriptiOFl = NULL;
				}
			if (!SkriptiOFl)
				return;
			}
		}
	if (SkriptiOFl) {
		wchar_t line[200], st[20];

		for (int ifin = 0; ifin < 5; ifin++) {
			if (finsrj[ifin] < 0)
				break;
			swprintf(line, L"%c-FINAALI:; %s; %s; %d; %d; %s; %s; %s; %s; %d; %d\n",
				L'A'+ifin,
				Sarjat[finsrj[ifin]].sarjanimi,
				Lkm0[ifin]->Text.c_str(),
				Loput[ifin]->Checked ? 1 : 0,
				CBla[ifin]->Checked ? 1 : 0,
				EnsLahto[ifin]->Text.c_str()[0] >= L'0' ? EnsLahto[ifin]->Text.c_str() : L"",
				LahtoVali[ifin]->Text.c_str(),
				Aalto[ifin]->Text.c_str(),
				Alku[ifin]->Text.c_str(),
				CBPoimi[ifin]->ItemIndex + 1,
				CBTakajo[ifin]->Checked ? 1 : 0);
			SkriptiOFl->WriteLine(line);
			}
		for (int ikrs = 0; ikrs < 10; ikrs++) {
			if (karssarjat[ikrs] < 0)
				break;
			swprintf(line, L"%s\n",	Sarjat[karssarjat[ikrs]].sarjanimi);
			SkriptiOFl->WriteLine(line);
			}
		SkriptiOFl->WriteLine(L"LOPPU\n");
		}
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::FormShow(TObject *Sender)
{
	int itm;

	ListSarjat->Items->Clear();
	for (int srj = 0; srj < sarjaluku; srj++)
		ListSarjat->Items->Add(Sarjat[srj].sarjanimi);
	for (int ifin = 0; ifin < 5; ifin++) {
		itm = CBfin[ifin]->ItemIndex;
		CBfin[ifin]->Items->Clear();
		CBfin[ifin]->Items->Add(L"Ei sarjaa");
		for (int srj = 0; srj < sarjaluku; srj++)
			CBfin[ifin]->Items->Add(Sarjat[srj].sarjanimi);
		CBfin[ifin]->ItemIndex = itm;
		finlkm[ifin] = _wtoi(Lkm0[ifin]->Text.c_str());
		if (!kilpparam.bibvaihtelee) {
			Alku[ifin]->Enabled = false;
			Alku[ifin]->Text = L"0";
			finalku[ifin] = 0;
			}
		else
			finalku[ifin] = _wtoi(Alku[ifin]->Text.c_str());
		}
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::Sulje1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TLoppuKilpForm::BtnPuraSeuratClick(TObject *Sender)
{
	for (int k = 0; k < 5; k++) {
		if (finsrj[k] < 0 || finylkm[k] == 0 || !srajrj[k])
			break;
		for (int i = finylkm[k] - 1; i > 1; i--) {
			srajrjtp sraj;
			int j;
			if (wcscmp(srajrj[k][i].seura, srajrj[k][i-1].seura))
				continue;
			for (j = i-2; j >= 0; j--) {
				if (wcscmp(srajrj[k][j].seura, srajrj[k][i-1].seura))
					break;
				}
			if (wcscmp(srajrj[k][j].seura, srajrj[k][i-1].seura)) {
				sraj = srajrj[k][j];
				memmove(srajrj[k]+j, srajrj[k]+j+1, (i-j-1)*sizeof(srajrjtp));
				srajrj[k][i-1] = sraj;
				}
			}
		for (int i = 0; i < finylkm[k] - 2; i++) {
			srajrjtp sraj;
			int j;
			if (wcscmp(srajrj[k][i].seura, srajrj[k][i+1].seura))
				continue;
			for (j = i+2; j < finylkm[k]; j++) {
				if (wcscmp(srajrj[k][j].seura, srajrj[k][i+1].seura))
					break;
				}
			if (wcscmp(srajrj[k][j].seura, srajrj[k][i+1].seura)) {
				sraj = srajrj[k][j];
				memmove(srajrj[k]+i+2, srajrj[k]+i+1, (j-i-1)*sizeof(srajrjtp));
				srajrj[k][i+1] = sraj;
				}
			}
		for (int i = 0; i < finylkm[k]; i++) {
			int ero;
			kilptietue kilp;
			if ((ero = i - srajrj[k][i].sj) != 0) {
				kilp.GETREC(srajrj[k][i].d);
				kilp.setId(kilp.id(k_pv+1)+ero, k_pv+1);
				kilp.tall_lajat_pv(kilp.pv[k_pv+1].tlahto+ero*Sarjat[finsrj[k]].lahtovali[k_pv+1]*SEK, k_pv+1, true);
				kilp.tallenna(srajrj[k][i].d, 0, 0, 0, 0);
				}
			}
		free(srajrj[k]);
		srajrj[k] = NULL;
		}
}
//---------------------------------------------------------------------------

