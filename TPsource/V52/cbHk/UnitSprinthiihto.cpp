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

#include "UnitSprinthiihto.h"
#include "WinHk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSprinthiihto *FormSprinthiihto;

static int muodvaihe;
//---------------------------------------------------------------------------
__fastcall TFormSprinthiihto::TFormSprinthiihto(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
	CBL[0] = CBL1;
	CBL[1] = CBL2;
	CBL[2] = CBL3;
	CBL[3] = CBL4;
	CBL[4] = CBL5;
	CBM[0] = CBM1;
	CBM[1] = CBM2;
	CBM[2] = CBM3;
	CBM[3] = CBM4;
	CBM[4] = CBM5;
	EdTul[0] = EdTul1;
	EdTul[1] = EdTul2;
	EdTul[2] = EdTul3;
	EdTul[3] = EdTul4;
	EdTul[4] = EdTul5;
}
//---------------------------------------------------------------------------
void __fastcall TFormSprinthiihto::FormShow(TObject *Sender)
{

	GBPohjustus->Enabled = k_pv == 0;

	if (kilpparam.bibvaihtelee == false) {
		EdtNumAlku->Enabled = false;
		GBPohjustus->Enabled = false;
		EdPalLisays->Text = L"0";
		EdPalLisays->Enabled = false;
		CBpalBib->Checked = false;
		CBpalBib->Enabled = false;
		Label10->Enabled = false;
		}
	EdtMuodVaihe->Text = k_pv+2;
	muodvaihe = k_pv+1;
	CBPerussarja->Clear();
	CBPerussarja->Items->Add(L"Ei sarjaa");
	for (int srj = 0; srj < sarjaluku; srj++)
		if (Sarjat[srj].enspv == 0 && Sarjat[srj].sprintSrj.vaihelkm > k_pv+1)
			CBPerussarja->Items->Add(Sarjat[srj].sarjanimi);
	CBPerussarja->ItemIndex = 0;
	for	(int i = 0; i < 5; i++) {
		CBL[i]->Clear();
		CBM[i]->Clear();
		CBL[i]->Items->Add(L"Ei sarjaa");
		CBM[i]->Items->Add(L"Ei sarjaa");
		EdTul[i]->Text = L"";
		for (int srj = 0; srj < sarjaluku; srj++) {
			CBL[i]->Items->Add(Sarjat[srj].sarjanimi);
			CBM[i]->Items->Add(Sarjat[srj].sarjanimi);
			}
		CBL[i]->ItemIndex = 0;
		CBM[i]->ItemIndex = 0;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSprinthiihto::Karsinnasta(bool suoraan)
{
	int p, k, ml, nl, eria, erakoko, msrj[5], lsrj[5], numAlku = 0;
	int jako5[10] = {1, 4, 5, 2, 3, 3, 2, 5, 4, 1};
	int jako4[8] = {1, 3, 4, 2, 2, 4, 3, 1};
	kilptietue kilp;
	int d[1000], nd = 0, edtl = 0, tasa[1000][2];

	HaeSprintsarjat(lsrj, &nl, msrj, &ml);
	eria = _wtoi(EdLkm->Text.c_str());
	erakoko = _wtoi(EdKoko->Text.c_str());
	if (kilpparam.bibvaihtelee)
		numAlku = _wtoi(EdtNumAlku->Text.c_str());
	if (lsrj[0] < 0 || ml < eria) {
		Application->MessageBoxW(L"M‰‰rittelyt ristiriitaisia", L"Virhe", MB_OK);
		return;
		}
	EnterCriticalSection(&tall_CriticalSection);
	p = jalku[lsrj[0]][0];
	for (int i = 0; i < 1000; i++) {
		if (p < 0)
			break;
		kilp.GETREC(p);
//		if (i >= eria * erakoko && kilp.tulos_pv(k_pv, true, 2) != edtl)
		if (i >= eria * erakoko)
			break;
		edtl = kilp.tulos_pv(k_pv, true, 2);
		d[i] = p;
		nd++;
		p = seuraava(0, 0, p);
		}
/*
	if (nd > eria * erakoko) {
		kilp.GETREC(d[nd-1]);
		edtl = kilp.tulos_pv(k_pv, true, 2);
		k = 1;
		memset(tasa, 0, sizeof(tasa));
		tasa[0][0] = kilp.id();
		tasa[0][1] = nd-1;
		for (int i = nd-2; i >= 0; i--) {
			kilp.GETREC(d[i]);
			if (edtl != kilp.tulos_pv(k_pv, true, 2))
				break;
			tasa[k][0] = kilp.id();
			tasa[k][1] = i;
			k++;
			}
		qsortint(tasa, k, 2*sizeof(int), 0, 1);
		for (int i = 0; i < k; i++) {
			d[nd-k+i] = tasa[i][1];
			}
		}
*/
	if (nd > eria * erakoko)
		nd = eria * erakoko;
	for (int i = 0; i < nd; i++) {
		kilp.GETREC(d[i]);
		if (suoraan) {
			k = i / erakoko;
			kilp.pv[k_pv].qual = 'Q'+k;
			}
		else {
			if (eria == 5)
				k = jako5[i % 10]-1;
			else if (eria == 4)
				k = jako4[i % 8]-1;
			else {
				k = i % (2*eria);
				if (k >= eria)
				   k = 2*eria - k - 1;
				}
			}
		if (suoraan) {
			_itow(i + 1, kilp.pv[muodvaihe].rata, 10);
			}
		else {
			_itow(i / eria + 1, kilp.pv[muodvaihe].rata, 10);
			}
		kilp.setSarja(msrj[k], muodvaihe);
//		kilp.setId(Sarjat[msrj[0]].bibalku[muodvaihe]+i, muodvaihe);
		if (numAlku > 0)
			kilp.setId(numAlku+i, muodvaihe);
		kilp.tall_lajat_pv(Sarjat[msrj[k]].enslahto[muodvaihe], muodvaihe);
		kilp.set_tark(L'-', muodvaihe);
		if (!suoraan)
			kilp.pv[k_pv].qual = 'Q';
		kilp.tallenna(d[i], 0, 0, 0, 0);
		}
	LeaveCriticalSection(&tall_CriticalSection);
	Application->MessageBoxW(L"Toiminto toteutettu", L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TFormSprinthiihto::Semi(void)
{
	int p, lj, k, nl, nm, eria, erakoko, msrj[5], lsrj[5], vd[8], nsj;
	__int64 vtls[8], vtulos;
	kilptietue kilp;
	int vaihe;

	memset(vd, 0, sizeof(vd));
	memset(vtls, 0, sizeof(vtls));
	HaeSprintsarjat(lsrj, &nl, msrj, &nm);
	eria = _wtoi(EdLkm->Text.c_str());
	erakoko = _wtoi(EdKoko->Text.c_str());
	nsj = (eria * erakoko - _wtoi(EdAjalla->Text.c_str())) / nl;
	if (nm < eria) {
		Application->MessageBoxW(L"M‰‰rittelyt ristiriitaisia", L"Virhe", MB_OK);
		return;
		}
	EnterCriticalSection(&tall_CriticalSection);
	for (int il = 0; il < nl; il++) {
		p = jalku[lsrj[il]][0];
		for (int i = 0; i < 4; i++) {
			if (p < 0)
				break;
			kilp.GETREC(p);
			if (i < nsj) {
				if (nl == 5) {
					if (il <= 1 || (il == 2 && i == 0)) {
						k = 0;
						}
					else {
						k = 1;
						}
					}
				else if (nl == 4) {
					if (il <= 1) {
						k = 0;
						}
					else {
						k = 1;
						}
					}
				else {
					k = (nl*i+il) % (2*nm);
					if (k >= nm)
						k = 2*nm-k-1;
					}
				kilp.setSarja(msrj[k], muodvaihe);
				kilp.setId(kilp.id(), muodvaihe);
				kilp.tall_lajat_pv(Sarjat[msrj[k]].enslahto[muodvaihe], muodvaihe);
				kilp.set_tark(L'-', muodvaihe);
				kilp.pv[k_pv].qual = 'Q';
				kilp.tallenna(p, 0, 0, 0, 0);
				}
			else if (nl*nsj < eria*erakoko) {
				for (int j = 0; j < eria*erakoko-nl*nsj && j < sizeof(vd)/sizeof(vd[0]); j++) {
					vtulos = 1000000000LL*kilp.p_aika(0, 1)+10000*kilp.sija(0) + kilp.id(0);
					if (vtls[j] == 0 || vtls[j] > vtulos) {
						if (j < sizeof(vd)/sizeof(vd[0])-1) {
							memmove(&vd[j+1], &vd[j], sizeof(vd)-(j+1)*sizeof(vd[0]));
							memmove(&vtls[j+1], &vtls[j], sizeof(vtls)-(j+1)*sizeof(vtls[0]));
							}
						vd[j] = p;
						vtls[j] = vtulos;
						break;
						}
					}
				}
			p = seuraava(0, 0, p);
			}
		}
	if (nl*nsj < eria*erakoko) {
		for (int j = 0; j < eria*erakoko-nl*nsj; j++) {
			if (vd[j] > 0) {
				kilp.GETREC(vd[j]);
				kilp.setSarja(msrj[(j+1)%nm], muodvaihe);
				kilp.setId(kilp.id(), muodvaihe);
				kilp.tall_lajat_pv(Sarjat[msrj[(j+1)%nm]].enslahto[muodvaihe], muodvaihe);
				kilp.set_tark(L'-', muodvaihe);
				kilp.pv[k_pv].qual = 'q';
				kilp.tallenna(vd[j], 0, 0, 0, 0);
				}
			}
		}
	for (int j = 0; j < eria; j++) {
		int jrj[20][2], n = 0;
		memset(jrj, 0, sizeof(jrj));
		for (int d = 1; d < nrec; d++) {
			kilp.GETREC(d);
			if (kilp.kilpstatus || kilp.Sarja(muodvaihe) != msrj[j] ||
				!kilp.lasna(muodvaihe))
				continue;
			jrj[n][1] = d;
			jrj[n][0] = 10000000*kilp.sija(k_pv)+10000*kilp.sija(0)+kilp.id(0);
			n++;
			}
		qsortint(jrj, n, 8, 0, 1);
		for (int i = 0; i < n; i++) {
			kilp.GETREC(jrj[i][1]);
			kilp.pv[muodvaihe].rata[0] = L'1' + i;
			kilp.pv[muodvaihe].rata[1] = 0;
			kilp.tallenna(jrj[i][1], 0, 0, 0, 0);
			}
		}
	LeaveCriticalSection(&tall_CriticalSection);
	Application->MessageBoxW(L"Toiminto toteutettu", L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TFormSprinthiihto::HaeSprintsarjat(int *lsrj, int *nl, int *msrj, int *nm)
{
	for (*nl = 0; *nl < 5; (*nl)++) {
		lsrj[*nl] = CBL[*nl]->ItemIndex - 1;
		if (lsrj[*nl] < 0)
			break;
		}
	for (*nm = 0; *nm < 5; (*nm)++) {
		msrj[*nm] = CBM[*nm]->ItemIndex - 1;
		if (msrj[*nm] < 0)
			break;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSprinthiihto::Finaalit(void)
{
	int p, k, nm, nl, nsj, eria, erakoko, msrj[5], lsrj[5], vd[8];
	__int64 vtls[8], vtulos;
	int numAlku = 0, ajalla, ntot, rt = 0;
	kilptietue kilp;

	memset(vd, 0, sizeof(vd));
	memset(vtls, 0, sizeof(vtls));
	HaeSprintsarjat(lsrj, &nl, msrj, &nm);
	eria = _wtoi(EdLkm->Text.c_str());
	erakoko = _wtoi(EdKoko->Text.c_str());
	ajalla =  _wtoi(EdAjalla->Text.c_str());
//	numAlku = _wtoi(EdtNumAlku->Text.c_str());
	nsj = (erakoko - ajalla) / nl;
	if (nm < eria) {
		Application->MessageBoxW(L"M‰‰rittelyt ristiriitaisia", L"Virhe", MB_OK);
		return;
		}
	EnterCriticalSection(&tall_CriticalSection);
	ntot = nsj + ajalla;
	if (nl == 1 && ajalla == 0)
		ntot = eria * nsj;
	for (int il = 0; il < nl; il++) {
		p = jalku[lsrj[il]][0];
		for (int i = 0; i < nilm[lsrj[il]] && i < ntot; i++) {
			int isrj;
			if (p < 0)
				break;
			kilp.GETREC(p);
			isrj = i / (nsj+ajalla);
			if (i < nsj || isrj > 0) {
				kilp.setSarja(msrj[isrj], muodvaihe);
				if (numAlku > 0)
					kilp.setId(numAlku+i, muodvaihe);
				else
					kilp.setId(kilp.id(), muodvaihe);
				kilp.tall_lajat_pv(Sarjat[msrj[isrj]].enslahto[muodvaihe], muodvaihe);
				kilp.set_tark(L'-', muodvaihe);
				kilp.pv[muodvaihe].rata[0] = L'1' + rt;
				kilp.pv[muodvaihe].rata[1] = 0;
				kilp.pv[k_pv].qual = isrj == 0 ? 'Q' : 'q';
				kilp.tallenna(p, 0, 0, 0, 0);
				}
			else {
				for (int j = 0; j < sizeof(vd)/sizeof(vd[0]); j++) {
					vtulos = 1000000000LL*kilp.p_aika(0, 1)+10000*kilp.sija(0) + kilp.id(0);
					if (vtls[j] == 0 || vtls[j] > vtulos) {
						if (j < sizeof(vd)/sizeof(vd[0])-1) {
							memmove(&vd[j+1], &vd[j], sizeof(vd)-(j+1)*sizeof(vd[0]));
							memmove(&vtls[j+1], &vtls[j], sizeof(vtls)-(j+1)*sizeof(vtls[0]));
							}
						vd[j] = p;
						vtls[j] = vtulos;
						break;
						}
					}
				}
			p = seuraava(0, 0, p);
			}
		}
	if (ajalla > 0) {
		for (int j = 0; j < ajalla; j++) {
			if (vd[j] > 0) {
				kilp.GETREC(vd[j]);
				kilp.setId(kilp.id(), muodvaihe);
				kilp.setSarja(msrj[0], muodvaihe);
				kilp.tall_lajat_pv(Sarjat[msrj[0]].enslahto[muodvaihe], muodvaihe);
				kilp.set_tark(L'-', muodvaihe);
				kilp.pv[muodvaihe].rata[0] = L'1' + rt;
				kilp.pv[muodvaihe].rata[1] = 0;
				kilp.pv[k_pv].qual = 'q';
				kilp.tallenna(vd[j], 0, 0, 0, 0);
				}
			}
		}
	if (nm > 1 && nl > 1) {
		for (int il = 0; il < nl; il++) {
			p = jalku[lsrj[il]][0];
			for (int i = 0; i < nilm[lsrj[il]]; i++) {
				if (p < 0)
					break;
				kilp.GETREC(p);
				if (kilp.Sarja(muodvaihe) != msrj[0]) {
					kilp.setId(kilp.id(), muodvaihe);
					kilp.setSarja(msrj[1], muodvaihe);
					kilp.tall_lajat_pv(Sarjat[msrj[1]].enslahto[muodvaihe], muodvaihe);
					kilp.set_tark(L'-', muodvaihe);
					kilp.pv[muodvaihe].rata[0] = L'1' + rt;
					kilp.pv[muodvaihe].rata[1] = 0;
					kilp.pv[k_pv].qual = 0;
					kilp.tallenna(p, 0, 0, 0, 0);
					}
				p = seuraava(0, 0, p);
				}
			}
		}
	for (int j = 0; j < eria; j++) {
		int jrj[20][2], n = 0;
		memset(jrj, 0, sizeof(jrj));
		for (int d = 1; d < nrec; d++) {
			kilp.GETREC(d);
			if (kilp.kilpstatus || kilp.Sarja(muodvaihe) != msrj[j] ||
				!kilp.lasna(muodvaihe))
				continue;
			jrj[n][1] = d;
			jrj[n][0] = 10000000*kilp.sija(k_pv)+10000*kilp.sija(0)+kilp.id(0);
			n++;
			}
		qsortint(jrj, n, 8, 0, 1);
		for (int i = 0; i < n; i++) {
			kilp.GETREC(jrj[i][1]);
			kilp.pv[muodvaihe].rata[0] = L'1' + i;
			kilp.pv[muodvaihe].rata[1] = 0;
			kilp.tallenna(jrj[i][1], 0, 0, 0, 0);
			}
		}
	LeaveCriticalSection(&tall_CriticalSection);
	Application->MessageBoxW(L"Toiminto toteutettu", L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TFormSprinthiihto::BtnToteutaClick(TObject *Sender)
{
	if (CBautoPal->Checked) {
		BtnPalautaClick(Sender);
		}
	switch(RGVaihe->ItemIndex) {
		case 0 :
			Karsinnasta(false);
			break;
		case 1 :
			Semi();
			break;
		case 2 :
			Finaalit();
			break;
		case 3 :
			Karsinnasta(true);
			break;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSprinthiihto::RGVaiheClick(TObject *Sender)
{
	switch (RGVaihe->ItemIndex) {
		case 0:
			EdLahto->Text = L"1";
			EdLkm->Text = L"5";
			EdKoko->Text = L"6";
			EdAjalla->Text = L"0";
			LblAjalla->Enabled = false;
			EdAjalla->Enabled = false;
			EdtNumAlku->Enabled = kilpparam.bibvaihtelee;
			break;
		case 1:
			EdLahto->Text = L"5";
			EdLkm->Text = L"2";
			EdKoko->Text = L"6";
			EdAjalla->Text = L"2";
			LblAjalla->Enabled = true;
			EdAjalla->Enabled = true;
			EdtNumAlku->Enabled = false;
			break;
		case 2:
			EdLahto->Text = L"2";
			EdLkm->Text = L"1";
			EdKoko->Text = L"6";
			EdAjalla->Text = L"2";
			LblAjalla->Enabled = true;
			EdAjalla->Enabled = true;
			EdtNumAlku->Enabled = false;
			break;
		case 3:
			EdLahto->Text = L"1";
			EdLkm->Text = L"4";
			EdKoko->Text = L"4";
			EdAjalla->Text = L"0";
			LblAjalla->Enabled = false;
			EdAjalla->Enabled = false;
			EdtNumAlku->Enabled = kilpparam.bibvaihtelee;
			break;
		}
	LblNumAlku->Enabled = EdtNumAlku->Enabled;
	CBPerussarjaChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormSprinthiihto::BtnPohjustaClick(TObject *Sender)
{
	int lisays, epv, vpv;
	int p, lj, k, nm, nl, msrj[5], lsrj[5];
	kilptietue kilp;

	epv = CBPohjVaiheet->ItemIndex;
	if (epv < 1) {
		epv = 1;
		vpv = kilpparam.n_pv_akt - 1;
		}
	else
		vpv = epv;
	HaeSprintsarjat(lsrj, &nl, msrj, &nm);
	if (!nl || epv > kilpparam.n_pv_akt - 1) {
		Application->MessageBoxW(L"Valinnat eiv‰t salli toteutusta", L"Virhe", MB_OK);
		return;
		}
	lisays = _wtoi(EdPohjLisays->Text.c_str());
	EnterCriticalSection(&tall_CriticalSection);
	for (int srj = 0; srj < nl; srj++) {
		lj = -1;
		p = -1;
		for (;;) {
			p = askellasarja(lsrj[srj], 0, p, &lj);
			if (p < 0)
				break;
			kilp.GETREC(p);
			for (int ipv = epv; ipv <= vpv; ipv++) {
				kilp.setSarja(lsrj[srj], ipv);
				kilp.setId(kilp.kilpno + lisays, ipv);
				if (kilp.tark(ipv) != L'P')
					kilp.set_tark(L'V', ipv);
				}
			kilp.tallenna(p, 0, 0, 0, 0);
			}
		}
	LeaveCriticalSection(&tall_CriticalSection);
	Application->MessageBoxW(L"Toiminto toteutettu", L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TFormSprinthiihto::BtnPalautaClick(TObject *Sender)
{
	int lisays;
	int p, lj, k, nm, nl, msrj[5], lsrj[5];
	kilptietue kilp;

	HaeSprintsarjat(lsrj, &nl, msrj, &nm);
	if (!nl) {
		Application->MessageBoxW(L"Sarjoja ei valittu", L"Virhe", MB_OK);
		return;
		}
	lisays = _wtoi(EdPalLisays->Text.c_str());
	EnterCriticalSection(&tall_CriticalSection);
	for (int srj = 0; srj < nl; srj++) {
		lj = -1;
		p = -1;
		for (;;) {
			p = askellasarja(lsrj[srj], 0, p, &lj);
			if (p < 0)
				break;
			kilp.GETREC(p);
			if (CBpalSarja->Checked)
				kilp.setSarja(kilp.sarja, muodvaihe);
			if (CBpalBib->Checked)
				kilp.setId(kilp.kilpno + lisays, muodvaihe);
			if (CBmerkPoissa->Checked && kilp.tark(muodvaihe) != L'P')
				kilp.set_tark(L'V', muodvaihe);
			if (CBpoistaQual->Checked)
				kilp.pv[k_pv].qual = 0;
			memset(kilp.pv[muodvaihe].rata, 0, sizeof(kilp.pv[0].rata));
			kilp.tallenna(p, 0, 0, 0, 0);
			}
		}
	LeaveCriticalSection(&tall_CriticalSection);
	Application->MessageBoxW(L"Toiminto toteutettu", L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TFormSprinthiihto::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Sprint-hiihto");
}
//---------------------------------------------------------------------------

void __fastcall TFormSprinthiihto::CBM1Change(TObject *Sender)
{
	int srj1 = CBM1->ItemIndex-1;
	if (srj1 < 0)
		return;
	CBM2->ItemIndex = 0;
	CBM3->ItemIndex = 0;
	CBM4->ItemIndex = 0;
	CBM5->ItemIndex = 0;
	if (kilpparam.bibvaihtelee && (RGVaihe->ItemIndex == 0 || RGVaihe->ItemIndex == 3))
		EdtNumAlku->Text = Sarjat[srj1].bibalku[1];
	else
		EdtNumAlku->Text = 0;
	if (_wtoi(EdLkm->Text.c_str()) > 1 && srj1 < sarjaluku-1) {
		CBM2->ItemIndex = srj1 + 2;
		}
	if (_wtoi(EdLkm->Text.c_str()) > 2 && srj1 < sarjaluku-2) {
		CBM3->ItemIndex = srj1 + 3;
		}
	if (_wtoi(EdLkm->Text.c_str()) > 3 && srj1 < sarjaluku-3) {
		CBM4->ItemIndex = srj1 + 4;
		}
	if (_wtoi(EdLkm->Text.c_str()) > 4 && srj1 < sarjaluku-4) {
		CBM5->ItemIndex = srj1 + 5;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSprinthiihto::Button4Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormSprinthiihto::EdtMuodVaiheExit(TObject *Sender)
{
	muodvaihe = _wtoi(EdtMuodVaihe->Text.c_str()) - 1;
	if (muodvaihe < k_pv+1 || muodvaihe >= kilpparam.n_pv_akt) {
		Application->MessageBoxW(L"Virheellinen vaihe vaihdettu seuraavaksi vaiheeksi", L"Virhe", MB_OK);
		muodvaihe = k_pv+1;
		EdtMuodVaihe->Text = muodvaihe+1;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormSprinthiihto::CBL1Change(TObject *Sender)
{
	int srj1 = CBL1->ItemIndex-1;
	if (srj1 < 0)
		return;
	EdTul[0]->Text = ntulos[srj1][0];
	for (int i = 1; i < 5; i++) {
		if (_wtoi(EdLahto->Text.c_str()) > i && srj1 < sarjaluku - i) {
			CBL[i]->ItemIndex = srj1 + i + 1;
//			EdTul[i]->Text = ntulos[srj1 + i][0];
			}
		else {
			CBL[i]->ItemIndex = 0;
//			EdTul[i]->Text = L"";
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSprinthiihto::CBLChange(TObject *Sender)
{
	int i;

	for (i = 1; i < 5; i++) {
		if (Sender == CBL[i]) {
			if (CBL[i]->ItemIndex == 0)
				EdTul[i]->Text = L"";
			else
				EdTul[i]->Text = ntulos[CBL[i]->ItemIndex-1][0];
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSprinthiihto::CBPerussarjaChange(TObject *Sender)
{
	int srj, i_pv;

	srj = haesarja_w(CBPerussarja->Text.c_str(), false);
	if (srj < 0)
		return;
	if (Sarjat[srj].sprintSrj.vaihelkm == 2)
		RGVaihe->ItemIndex = 3;
	if (Sarjat[srj].sprintSrj.vaihelkm == 3)
		RGVaihe->ItemIndex = k_pv ? 2 : 0;
	if (Sarjat[srj].sprintSrj.vaihelkm == 4)
		RGVaihe->ItemIndex = k_pv % 3;
	i_pv = k_pv;
	if (Sarjat[srj].sprintSrj.vaihelkm > 1) {
		if (i_pv >= Sarjat[srj].sprintSrj.vaihelkm - 1)
			i_pv >= Sarjat[srj].sprintSrj.vaihelkm - 2;
		for (int i = 0; i < 5; i++) {
			if (i_pv == 0) {
				if (i == 0) {
					CBL[0]->ItemIndex = srj+1;
					EdTul[0]->Text = ntulos[srj][0];
					}
				else
					CBM[i]->ItemIndex = 0;
				}
			else if (i < Sarjat[srj].sprintSrj.eralkm[i_pv-1]) {
				CBL[i]->ItemIndex = Sarjat[srj].sprintSrj.sarjat[i_pv-1][i]+1;
				EdTul[i]->Text = ntulos[Sarjat[srj].sprintSrj.sarjat[i_pv-1][i]][0];
				}
			else {
				CBL[i]->ItemIndex = 0;
				EdTul[i]->Text = L"";
				}
			if (i < Sarjat[srj].sprintSrj.eralkm[i_pv]) {
				CBM[i]->ItemIndex = Sarjat[srj].sprintSrj.sarjat[i_pv][i]+1;
				}
			else {
				CBM[i]->ItemIndex = 0;
				}
			}
		EdtMuodVaihe->Text = i_pv+2;
		EdLahto->Text = UnicodeString(i_pv < 1 ? 1 : Sarjat[srj].sprintSrj.eralkm[i_pv-1]);
		EdLkm->Text = Sarjat[srj].sprintSrj.eralkm[i_pv];
		EdKoko->Text = Sarjat[srj].sprintSrj.erakoko[i_pv];
		if (Sarjat[srj].sprintSrj.erakoko[i_pv] == 6 && i_pv > 0)
			EdAjalla->Text = 2;
		else
			EdAjalla->Text = 0;
		if (kilpparam.bibvaihtelee && (RGVaihe->ItemIndex == 0 || RGVaihe->ItemIndex == 3))
			EdtNumAlku->Text = Sarjat[Sarjat[srj].sprintSrj.sarjat[i_pv][0]].bibalku[i_pv+1];
		else
			EdtNumAlku->Text = 0;
		}
}
//---------------------------------------------------------------------------

