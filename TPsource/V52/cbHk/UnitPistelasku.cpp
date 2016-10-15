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

#include "UnitPistelasku.h"
#include "UnitKaavaTaul.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormPistelasku *FormPistelasku;
//---------------------------------------------------------------------------
__fastcall TFormPistelasku::TFormPistelasku(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::BtnLisKaikkiClick(TObject *Sender)
{
	for (int srj = 0; srj < sarjaluku + nsarjayhd; srj++)
		SarjaValinnat[srj] = true;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::BtnPoisKaikkiClick(TObject *Sender)
{
	for (int srj = 0; srj < sarjaluku + nsarjayhd; srj++)
		SarjaValinnat[srj] = false;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::BtnLisValitutClick(TObject *Sender)
{
	for (int rv = 0; rv < LBSarjat->Items->Count; rv++) {
		if (LBSarjat->Selected[rv]) {
			int srj;
			if ((srj = haesarja_w(LBSarjat->Items->Strings[rv].c_str(), true)) >= 0)
				SarjaValinnat[srj] = true;
			}
		}
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::BtnPoisValitutClick(TObject *Sender)
{
	for (int rv = 0; rv < LBSarjaVal->Items->Count; rv++) {
		if (LBSarjaVal->Selected[rv]) {
			int srj;
			if ((srj = haesarja_w(LBSarjaVal->Items->Strings[rv].c_str(), true)) >= 0)
				SarjaValinnat[srj] = false;
			}
		}
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------
bool TFormPistelasku::SarjaValittu(int Srj)
{
	for (int rv = 0; rv < LBSarjaVal->Items->Count; rv++) {
		if (haesarja_w(LBSarjaVal->Items->Strings[rv].c_str(), true) == Srj)
			return(true);
		}
	return(false);
}
//---------------------------------------------------------------------------
void TFormPistelasku::NaytaSarjavalinnat(void)
{
	LBSarjat->Clear();
	LBSarjaVal->Clear();
	for (int srj = 0; srj < sarjaluku + nsarjayhd; srj++) {
		if (SarjaValinnat[srj])
			LBSarjaVal->Items->Add(UnicodeString(Sarjat[srj].sarjanimi));
		else
			LBSarjat->Items->Add(UnicodeString(Sarjat[srj].sarjanimi));
		}
}
//---------------------------------------------------------------------------
void TFormPistelasku::HaeKertoimet(pistekaavatp *kaava)
{
	if (nytulost > 0 && CBYPisteTls->Checked != ((kaava->yhtpiston & 4) != 0)) {
		if (Application->MessageBoxW(L"Yhteistuloksen m‰‰r‰ytymistavan muutos, kun yhteistuloksia jo on, voi synnytt‰‰ indeksointivirheit‰ ja vaatii ohjelman uudelleenk‰ynnistyksen. Muutetaanko m‰‰r‰ytymistapa",
			L"Vahvista valinta", MB_YESNO) != IDYES) {
			CBYPisteTls->Checked = ((kaava->yhtpiston & 4) != 0);
			return;
			}
		}
	kaava->a[0] = _wtoi(EdA1->Text.c_str());
	kaava->a[1] = _wtoi(EdA2->Text.c_str());
	kaava->a[2] = _wtoi(EdA3->Text.c_str());
	kaava->a[3] = _wtoi(EdA4->Text.c_str());
	kaava->a[4] = _wtoi(EdA5->Text.c_str());
	kaava->y[0] = _wtoi(EdL1->Text.c_str());
	kaava->y[1] = _wtoi(EdL2->Text.c_str());
	kaava->y[2] = _wtoi(EdL3->Text.c_str());
	kaava->y[3] = _wtoi(EdL4->Text.c_str());
	kaava->y[4] = _wtoi(EdL5->Text.c_str());
	kaava->p[0] = _wtoi(EdP1->Text.c_str());
	kaava->p[1] = _wtoi(EdP2->Text.c_str());
	kaava->p[2] = _wtoi(EdP3->Text.c_str());
	kaava->p[3] = _wtoi(EdP4->Text.c_str());
	kaava->p[4] = _wtoi(EdP5->Text.c_str());
	kaava->d[0] = _wtoi(EdD1->Text.c_str());
	kaava->d[1] = _wtoi(EdD2->Text.c_str());
	kaava->d[2] = _wtoi(EdD3->Text.c_str());
	kaava->d[3] = _wtoi(EdD4->Text.c_str());
	kaava->d[4] = _wtoi(EdD5->Text.c_str());
	kaava->laji = CBMolemmat->Checked ? 3 : (ComboBox1->ItemIndex == 0 ? 1 : 2);
	kaava->plaji = ComboBox1->ItemIndex;
	kaava->vaika = CBVertAika->Checked ? 0 : wstrtoaika_vap(EdtVertAika->Text.c_str(), 0);
//	if (kaava->vaika = CBVertAika->Checked)
		EdtVertAika->Text = aikatowstr_cols_n(0, kaava->vaika, 0, L',', pvparam[k_pv].laika);
	kaava->vpist = StrToDouble(EdtVoittopist->Text.c_str());
	kaava->kerr = _wtoi(EditA->Text.c_str());
	kaava->jak = _wtoi(EditB->Text.c_str());
	kaava->optiot = RGPyor->ItemIndex + (CBEiNeg->Checked ? 4 : 0);
	kaava->yhtpistvaihelkm = _wtoi(EdVaiheLkm->Text.c_str());
	kaava->yhtpistosottomin = _wtoi(EdtOsOttoMin->Text.c_str());
	kaava->yhtpisthyvmin = _wtoi(EdtHyvMin->Text.c_str());
	kaava->vaiheon = CBpisteetOn->Checked ? 1 : 0;
	kaava->yhtpiston = CByhteispisteetOn->Checked ? 1 : 0;
	if (CByhtpkaikille->Checked)
		kaava->yhtpiston |= 2;
	if (CBYPisteTls->Checked)
		kaava->yhtpiston |= 4;
}
//---------------------------------------------------------------------------
void TFormPistelasku::NaytaKertoimet(pistekaavatp *kaava)
{
	wchar_t as[20];

	EdA1->Text = kaava->a[0];
	EdA2->Text = kaava->a[1];
	EdA3->Text = kaava->a[2];
	EdA4->Text = kaava->a[3];
	EdA5->Text = kaava->a[4];
	EdL1->Text = kaava->y[0];
	EdL2->Text = kaava->y[1];
	EdL3->Text = kaava->y[2];
	EdL4->Text = kaava->y[3];
	EdL5->Text = kaava->y[4];
	EdP1->Text = kaava->p[0];
	EdP2->Text = kaava->p[1];
	EdP3->Text = kaava->p[2];
	EdP4->Text = kaava->p[3];
	EdP5->Text = kaava->p[4];
	EdD1->Text = kaava->d[0];
	EdD2->Text = kaava->d[1];
	EdD3->Text = kaava->d[2];
	EdD4->Text = kaava->d[3];
	EdD5->Text = kaava->d[4];
	if (kaava->laji & 2)
		ComboBox1->ItemIndex = kaava->plaji;
	else
		ComboBox1->ItemIndex = 0;
	CBMolemmat->Checked = (kaava->laji & 3) == 3;
	CBVertAika->Checked = kaava->vaika == 0;
	EdtVertAika->Text = aikatowstr_cols_n(0, kaava->vaika, 0, L',', pvparam[k_pv].laika);
	EdtVoittopist->Text = kaava->vpist;
	EditA->Text = kaava->kerr;
	EditB->Text = kaava->jak;
	RGPyor->ItemIndex = (kaava->optiot & 3);
	CBEiNeg->Checked = (kaava->optiot & 4) == 4;
	EdVaiheLkm->Text = kaava->yhtpistvaihelkm;
	EdtOsOttoMin->Text = kaava->yhtpistosottomin;
	EdtHyvMin->Text = kaava->yhtpisthyvmin;
	CBpisteetOn->Checked = kaava->vaiheon;
	CByhteispisteetOn->Checked = kaava->yhtpiston & 1;
	CByhtpkaikille->Checked = kaava->yhtpiston & 2;
	CBYPisteTls->Checked = kaava->yhtpiston & 4;
	Label17->Enabled = CBVertAika->Enabled && !CBVertAika->Checked;
	EdtVertAika->Enabled = CBVertAika->Enabled && !CBVertAika->Checked;
	ComboBox1Change(this);
	CBVertAikaClick(this);
}

//---------------------------------------------------------------------------
//int TFormPistelasku::Pisteet(pistekaavatp *kva, int sj, int n, int tls, int karki)

//void __fastcall TFormPistelasku::LaskeVaiheSarja(pistekaavatp *kva, int srj)
void LaskeVaiheSarja(pistekaavatp *kva, int srj, int i_pv)
{
	int lj = 0, p, sj = 0, sj1 = 0, pst, edtls = -99999999, karki;
	kilptietue kilp;

	if (i_pv == k_pv) {
		p = jalku[srj][0];
		EnterCriticalSection(&tall_CriticalSection);
		while (lj <= 4 && p > 0) {
			kilp.GETREC(p);
			if (lj > 0 || (FormPistelasku->CBJoukkNimi->Checked && !kilp.joukkue[0]))
				pst = 0;
			else {
				int tls;
				sj1++;
				tls = kilp.hyv() ? kilp.p_aika(0, 2, 2) : 0;
				if (tls != edtls) {
					edtls = tls;
					sj = sj1;
					}
				if (sj1 == 1)
					karki = tls;
				pst = Pisteet(kva, sj, ntulos[srj][0], edtls, karki);
				if (pst == -99999999)
					pst = 0;
				}
			kilp.pv[k_pv].pvpisteet[0] = pst;
			kilp.tallenna(p, 0, 0, 0, 0);
			p = SeuraavaSrj(srj, 0, p);
			while (p < 0 && lj < 4)
				p = jalku[srj][++lj];
			}
		LeaveCriticalSection(&tall_CriticalSection);
		}
	else {
		char key[10], vkey[10];
		int ok = 0;

//		vaihetlsindex.flags = i_pv;
		arv_pv = i_pv;
		teeindeksi(&vaihetlsindex, true);
		memset(key, 0, sizeof(key));
		swab((char *)&srj, key, 2);
		memcpy(vkey, key, sizeof(vkey));
		ok = !searchix(&vaihetlsindex, key, &p);
		while (ok && p > 0 && !memcmp(key, vkey, 2)) {
			kilp.GETREC(p);
			sj1++;
			if (kilp.tulos_pv(i_pv, true) != edtls) {
				edtls = kilp.tulos_pv(i_pv, true);
				sj = sj1;
				}
			if (sj1 == 1)
				karki = edtls;
			pst = Pisteet(kva, sj, ntulos[srj][0], edtls, karki);
			if (pst == -99999999)
				pst = 0;
			kilp.pv[i_pv].pvpisteet[0] = pst;
			kilp.tallenna(p, 0, 0, 0, 0);
			ok = !nextix(&vaihetlsindex, key, &p);
			}
		poistaindeksi(&vaihetlsindex);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::BtnLaskeVaiheClick(TObject *Sender)
{
	pistekaavatp Kv;

	for (int srj = 0; srj < sarjaluku+nsarjayhd; srj++) {
		if (SarjaValittu(srj))
			Kv = Sarjat[srj].pistekaava[k_pv];
		}
	Panel1->Color = clYellow;
	HaeKertoimet(&Kv);
	for (int srj = 0; srj < sarjaluku + nsarjayhd; srj++) {
		if (!SarjaValittu(srj))
			continue;
		LaskeVaiheSarja(&Kv, srj, k_pv);
		}
	Sleep(1000);
	Panel1->Color = clBtnFace;
}
//---------------------------------------------------------------------------

//void __fastcall TFormPistelasku::LaskeYhtPisteet(pistekaavatp *kva, int srj)
void LaskeYhtPisteet(pistekaavatp *kva, int srj)
{
	int d;
	kilptietue kilp;

	if (Sarjat[srj].luonne[k_pv] < 0 || nilm_pv[k_pv][srj] == 0)
		return;
	EnterCriticalSection(&tall_CriticalSection);
	for (d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (kilp.kilpstatus != 0 || kilp.tark() == L'P')
			continue;
		if (!((srj < sarjaluku && kilp.Sarja() == srj) ||
			(srj >= sarjaluku && srj < sarjaluku + nsarjayhd && !yhdsarjat[srj-sarjaluku, kilp.Sarja()])))
			continue;
//		if (!Sarjat[kilp.Sarja()].YhtpistOn())
			{
//		if (Sarjat[kilp.Sarja()].YhtpistOn()) {
			LaskeKilpYhtPisteet(&kilp, kva, srj);
			}
		kilp.tallenna(d, 0, 0, 0, 0);
		}
	LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::BtnLaskeYhtPisteetClick(TObject *Sender)
{
	pistekaavatp Kv;

	for (int srj = 0; srj < sarjaluku+nsarjayhd; srj++) {
		if (SarjaValittu(srj))
			Kv = Sarjat[srj].pistekaava[k_pv];
		}
	Panel2->Color = clYellow;
	HaeKertoimet(&Kv);
	for (int srj = 0; srj < sarjaluku+nsarjayhd; srj++) {
		if (!SarjaValittu(srj))
			continue;
		LaskeYhtPisteet(&Kv, srj);
		}
	Sleep(1000);
	Panel2->Color = clBtnFace;
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::FormShow(TObject *Sender)
{
	 NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Pistelaskenta");
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::LBSarjatDblClick(TObject *Sender)
{
	int srj;

	srj = haesarja_w(LBSarjat->Items->Strings[LBSarjat->ItemIndex].c_str(), true);
	if (srj >= 0)
		SarjaValinnat[srj] = true;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::LBSarjaValDblClick(TObject *Sender)
{
	int srj;

	srj = haesarja_w(LBSarjaVal->Items->Strings[LBSarjaVal->ItemIndex].c_str(), true);
	if (srj >= 0)
		SarjaValinnat[srj] = false;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::allennasnnttiedostoon1Click(TObject *Sender)
{
	for (int srj = 0; srj < sarjaluku+nsarjayhd; srj++) {
		if (SarjaValittu(srj))
			kv = Sarjat[srj].pistekaava[k_pv];
		}
	HaeKertoimet(&kv);
	for (int is = 0; is < sarjaluku; is++) {
		if (SarjaValittu(is)) {
			Sarjat[is].pistekaava[k_pv] = kv;
			}
		}
	kirjEventXml(sarjaxmlfname, true, false, false);
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::Luesnnttiedostosta1Click(TObject *Sender)
{
	int is;
	for (is = 0; is < sarjaluku; is++) {
		if (SarjaValittu(is)) {
			kv = Sarjat[is].pistekaava[k_pv];
			break;
			}
		}
	if (is == sarjaluku) {
		Application->MessageBoxW(L"Sarjaa ei ole valittu, haettu vain aiemmin m‰‰ritelty‰ kaavaa vastaavat sarjat",
			L"Varoitus", MB_OK);
		}
	for (is = 0; is < sarjaluku; is++) {
		SarjaValinnat[is] = memcmp(&kv, &Sarjat[is].pistekaava[k_pv], sizeof(kv)) == 0;
		}
	NaytaKertoimet(&kv);
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::Help1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Pistelaskenta");
}
//---------------------------------------------------------------------------

//void __fastcall TFormPistelasku::BtnLaskeKaikkiClick(TObject *Sender)
void LaskeKaikkiPisteet(void)
{
	for (int srj = 0; srj < sarjaluku; srj++) {
		if (Sarjat[srj].pistekaava[k_pv].laji == 0)
			continue;
		LaskeVaiheSarja(&Sarjat[srj].pistekaava[k_pv], srj, k_pv);
		if (k_pv && Sarjat[srj].pistekaava[k_pv].yhtpistvaihelkm)
			LaskeYhtPisteet(&Sarjat[srj].pistekaava[k_pv], srj);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::BtnLaskeKaikkiClick(TObject *Sender)
{
	Panel3->Color = clYellow;
	LaskeKaikkiPisteet();
	Sleep(1000);
	Panel3->Color = clBtnFace;
}

void __fastcall TFormPistelasku::Haeedellisenvaiheenkaavat1Click(TObject *Sender)
{
	memset(&kv, 0, sizeof(kv));
	if (!k_pv)
		return;
	for (int is = 0; is < sarjaluku; is++) {
		Sarjat[is].pistekaava[k_pv] = Sarjat[is].pistekaava[k_pv-1];
		}
	for (int is = 0; is < sarjaluku; is++) {
		if (SarjaValittu(is)) {
			kv = Sarjat[is].pistekaava[k_pv];
			break;
			}
		}
	for (int is = 0; is < sarjaluku; is++) {
		SarjaValinnat[is] = memcmp(&kv, &Sarjat[is].pistekaava[k_pv], sizeof(kv)) == 0;
		}
	NaytaKertoimet(&kv);
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::ComboBox1Change(TObject *Sender)
{
	CBVertAika->Enabled = ComboBox1->ItemIndex > 0;
	EditA->Enabled = ComboBox1->ItemIndex > 1;
	EditB->Enabled = ComboBox1->ItemIndex > 1;
	CBEiNeg->Enabled = ComboBox1->ItemIndex > 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::CBVertAikaClick(TObject *Sender)
{
	Label17->Enabled = !CBVertAika->Checked;
	EdtVertAika->Enabled = !CBVertAika->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::aulukkonytt1Click(TObject *Sender)
{
	if (FormKaavaTaul == NULL)
		FormKaavaTaul = new TFormKaavaTaul(this);
	FormKaavaTaul->Show();
	if (FormKaavaTaul->WindowState == wsMinimized)
		FormKaavaTaul->WindowState = wsNormal;
	FormKaavaTaul->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::Kopioitmnvaiheenkaavattoisellevaiheelle1Click(TObject *Sender)

{
	int ipv = k_pv + 2;
	wchar_t ch;
	if (ipv == kilpparam.n_pv_akt)
		ipv = 1;
	inputint_prompt(&ipv, L"Anna vaihe jolle kopioidaan. Kopiointi koskee valittuja sarjoja.", &ch, this);
	if (ch != ESC) {
		ipv--;
		for (int srj = 0; srj < sarjaluku+nsarjayhd; srj++) {
			if (SarjaValittu(srj)) {
				memcpy(&Sarjat[srj].pistekaava[ipv], &Sarjat[srj].pistekaava[k_pv],
					sizeof(Sarjat[0].pistekaava[0]));
				}
			}
		kirjEventXml(sarjaxmlfname, true, false, false);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormPistelasku::Laskeaiempivaihe1Click(TObject *Sender)
{
	int ipv = k_pv;
	wchar_t ch;

	inputint_prompt(&ipv, L"Valittujen sarjojen laskenta tapahtuu tallennetun kaavan mukaisesti. "
		L"Anna vaihe. (Esc peruuttaa laskennan)", &ch, this);
	ipv--;
	if (ch != ESC && ipv >= 0 && ipv < k_pv) {
		for (int srj = 0; srj < sarjaluku + nsarjayhd; srj++) {
			if (!SarjaValittu(srj))
				continue;
			LaskeVaiheSarja(&Sarjat[srj].pistekaava[ipv], srj, ipv);
			}
		}
}
//---------------------------------------------------------------------------


