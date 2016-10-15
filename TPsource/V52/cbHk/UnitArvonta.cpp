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

#include "UnitArvonta.h"
#include "UnitSarjatiedot.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormArvonta *FormArvonta;
extern TFormSarjatiedot *FormSarjatiedot;

void arvo(int Toiminto);
void arvonumerot(INT srj, bool yhtlahtoajat, bool kaikkipaivat, kilpindex *aktindex);

//---------------------------------------------------------------------------
__fastcall TFormArvonta::TFormArvonta(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	SGMuodostettavat->ColWidths[0] = 70 * Screen->PixelsPerInch / 96;
	SGMuodostettavat->Cells[0][0] = L"Sarja";
	SGMuodostettavat->Cells[1][0] = L"Ennalta";
	SGMuodostettavat->Cells[2][0] = L"Jaosta";
	GroupBox4->Visible = kilpparam.n_pv_akt >= 2;
	CB2pv->Enabled = (kilpparam.n_pv_akt == 2);
	CBJakoVaiheet->Visible = kilpparam.n_pv_akt >= 2;
	LblJakoVaiheet->Visible = kilpparam.n_pv_akt >= 2;
	if (kilpparam.n_pv_akt > 1) {
		CBPaiva->Clear();
		CBNumPaiva->Clear();
		CBJakoVaiheet->Clear();
		CBPaiva->Items->Add(L"Kaikkien vaiheiden arvonta kerralla");
		CBJakoVaiheet->Items->Add(L"Kaikki vaiheet");
		for (int i = 0;i < kilpparam.n_pv_akt; i++) {
		   CBPaiva->Items->Add(UnicodeString(i+1)+L". vaihe");
		   CBNumPaiva->Items->Add(UnicodeString(i+1)+L". vaihe");
		   CBJakoVaiheet->Items->Add(UnicodeString(i+1)+L". vaihe");
			}
		CBPaiva->ItemIndex = 0;
		CBNumPaiva->ItemIndex = 0;
		CBJakoVaiheet->ItemIndex = 0;
		}
	if (!kilpparam.bibvaihtelee) {
		RGNumPaivat->ItemIndex = 0;
		RGNumPaivat->Enabled = false;
		}
	else {
		if (kilpparam.kilplaji == L'H' && kilpparam.alalaji == L'S') {
			RGNumPaivat->ItemIndex = 1;
			}
		else
			RGNumPaivat->ItemIndex = 0;
		}
	if (ToimintaTila == 2) {
		RGNumPaivat->ItemIndex = 2;
		RGNumPaivat->Enabled = false;
		CBNumPaiva->ItemIndex = k_pv + 1;
		CBPaiva->ItemIndex = k_pv + 2;
		Label3->Visible = false;
		}
	if (kilpparam.kilplaji != L'S')
		RGToiminto->ItemIndex = 1;
	if (ToimintaTila == 1 && kilpparam.kilplaji == L'H' && kilpparam.alalaji == L'S')
		CBPaiva->ItemIndex = 1;
	PageControl1->ActivePageIndex = 0;
}
//---------------------------------------------------------------------------
bool __fastcall TFormArvonta::TarkVakantit(void)
{
	kilptietue kilp;
	int nvac = 0;

	if (kilpparam.n_pv_akt > 1)
		return(false);
	for (int d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (kilp.kilpstatus != 0 || kilp.tark(0) != L'V' || Sarjat[kilp.Sarja(0)].vakanttisarja)
			continue;
		if (kilp.id(0) == 0 || kilp.id(0) >= Sarjat[kilp.Sarja(0)].bibalku[0] ||
			kilp.TLahto(0) != Sarjat[kilp.Sarja(0)].enslahto[0] +
				SEK*Sarjat[kilp.Sarja(0)].lahtovali[0]*(kilp.id(0)-Sarjat[kilp.Sarja(0)].bibalku[0]))
			return(false);
		nvac++;
		}
	return(nvac > 1);
}
//---------------------------------------------------------------------------
void __fastcall TFormArvonta::FormShow(TObject *Sender)
{
	RGSeura->ItemIndex = kilpparam.seuralyh_on ? 1 : 0;
	EdtSprLisa->Visible = RGNumPaivat->ItemIndex == 1;
	LblSprLisa->Visible = EdtSprLisa->Visible;
	kilpparam.poissamukaan = RGPoissa->ItemIndex == 0;
	CBOhitaVakantit->Checked = TarkVakantit();
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------
void TFormArvonta::NaytaSarjavalinnat(void)
{
	LBSarjat->Clear();
	LBSarjaVal->Clear();
	for (int srj = 0; srj < sarjaluku; srj++) {
		if (SarjaValinnat[srj])
			LBSarjaVal->Items->Add(UnicodeString(Sarjat[srj].sarjanimi));
		else
			LBSarjat->Items->Add(UnicodeString(Sarjat[srj].sarjanimi));
		}
}
//---------------------------------------------------------------------------
int tarkNumerotila(bool kerro)
{
	wchar_t msg[5000] = L"";
	int nPaall = 0;

	for (int i = 0; i < sarjaluku; i++) {
//		int nn = nilm[i];
//		if (FormArvonta->RGNumPaivat->ItemIndex == 2) {
//			nn = nilm_pv[FormArvonta->CBNumPaiva->ItemIndex][i];
//			}
		int sa;
		int sl;
		int ipv;

		if (!FormArvonta->SarjaValittu(i))
			continue;
		ipv = FormArvonta->CBNumPaiva->ItemIndex;
		if (FormArvonta->RGNumPaivat->ItemIndex == 1 && Sarjat[i].luonne[ipv] < 0)
			continue;
		sa = Sarjat[i].sarjaalku;
		if (kilpparam.n_pv_akt == 1 || FormArvonta->RGNumPaivat->ItemIndex == 2) {
			if (kilpparam.bibvaihtelee)
				sa = Sarjat[i].bibalku[ipv];
			sl = sa + nilm_pv[ipv][i] - (FormArvonta->CBOhitaVakantit->Checked ? nvak[ipv][i] : 0) - 1;
			}
		else {
			sl = sa + nilm[i] - 1;
			}

		if (sl < sa)
			continue;
		for (int j = 0; j < sarjaluku; j++) {
			int sa1, n1;

			if (j == i)
				continue;
			if (FormArvonta->RGNumPaivat->ItemIndex == 1 && Sarjat[j].luonne[ipv] < 0)
				continue;
			sa1 = Sarjat[j].sarjaalku;
			if (kilpparam.n_pv_akt == 1 || FormArvonta->RGNumPaivat->ItemIndex == 2) {
				n1 = nilm_pv[ipv][j] - (FormArvonta->CBOhitaVakantit->Checked ? nvak[ipv][j] : 0);
				if (kilpparam.bibvaihtelee)
					sa1 = Sarjat[j].bibalku[ipv];
				}
			else {
				n1 = nilm[j];
				}
			if (n1 <= 0)
				continue;
			if (sa <= sa1 && sa1 <= sl) {
				nPaall++;
				if (nPaall <= 20) {
					swprintf(msg+wcslen(msg), L"Sarjojen %s ja %s numeroalueet p‰‰llekk‰iset\n",
						Sarjat[i].sarjanimi, Sarjat[j].sarjanimi);
					}
				}
			if (wcslen(msg) > sizeof(msg)/2 - 100)
				break;
			}
		if (wcslen(msg) > sizeof(msg)/2 - 100)
			break;
		}
	if (wcslen(msg) > 20) {
		int vast;
		if (nPaall > 20) {
			swprintf(msg+wcslen(msg), L"..\n..\nP‰‰llekk‰isyyksi‰ yhteens‰ %d\n", nPaall);
			}
		vast = Application->MessageBox((UnicodeString(msg)+L" Numeroidaanko t‰st‰ huolimatta?").c_str(), L"Virhe", MB_YESNO);
		return(vast == IDYES ? 0 : 1);
		}
	else if(kerro)
		Application->MessageBoxW(L"Ei p‰‰llekk‰isyyksi‰", L"OK", MB_OK);
	return(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormArvonta::Button1Click(TObject *Sender)
{
	FormSarjatiedot->Show();
	if (FormSarjatiedot->WindowState == wsMinimized)
		FormSarjatiedot->WindowState = wsNormal;
	FormSarjatiedot->BringToFront();
}
//---------------------------------------------------------------------------
int __fastcall TFormArvonta::SeurPos(kilptietue *kilp, int Jrj, int Srj, int Ed, int ipv)
{
	int d = -1, arv_pv0, ok = 1;
	char keyn[100], keyv[2];

	switch (Jrj) {
		case 0:
			if (Ed < 0)
				Ed = 0;
			EnterCriticalSection(&idindex_CriticalSection);
			d = getpos(Ed+1, true);
//			for (int i = Ed+1; i <= maxkilpno; i++) {
//				if ((d = getpos(i)) > 0 && d < nrec) {
			while (d > 0) {
				kilp->GETREC(d);
				if (kilp->Sarja(ipv) == Srj) {
					break;
					}
				d = nextpos();
				}
			LeaveCriticalSection(&idindex_CriticalSection);
/*
						return(d);
						}
					}
				}
			d = -1;
*/
			break;
		case 1:
			if (Ed < 0) {
				keyfromwname(keyn, L"", nameindex.keyl-2, 0);
				ok = !searchix(&nameindex, keyn, &d);
				}
			else {
				ok = !nextix(&nameindex, keyn, &d);
				}
			while (ok) {
				kilp->GETREC(d);
				if (kilp->Sarja(ipv) == Srj)
					return(d);
				ok = !nextix(&nameindex, keyn, &d);
				}
			if (!ok) {
				d = -1;
				}
			break;
		case 4:
			_swab((char *) &Srj, keyv, 2);
			if (Ed < 0) {
				if (CBOhitaVakantit->Checked && !SprLisa)
					ljarjindex->flags = 0x4000;
				ok = !teeindeksi(ljarjindex, true);
				ljarjindex->flags = 0;
				memset(keyn, 0, sizeof(keyn));
				memcpy(keyn, keyv, 2);
				if (ok)
					ok = !searchix(ljarjindex, keyn, &d);
				if (!ok || memcmp(keyn, keyv, 2))
					d = -1;
				}
			else {
				ok = !nextix(ljarjindex, keyn, &d);
				if (!ok || memcmp(keyn, keyv, 2))
					d = -1;
				}
			break;
		case 6:
			if (Ed < 0)
				Ed = 0;
			for (int d = Ed+1; d < nrec; d++) {
				kilp->GETREC(d);
				if (kilp->kilpstatus == 0 && kilp->Sarja(ipv) == Srj) {
					return(d);
					}
				}
			d = -1;
			break;
		case 7:

		//  Pistej‰rjestys m‰‰r‰t‰‰n seuraavasti:
		//
		//  flags & 1:  0: alhaiset pisteet parempia, 1: korkeat pisteet parempia
		//  flags & 6:  kahdella jaettuna indeksi pistetietoihin
		//  flags & 8:  0: kokonaispisteet, 8: vaiheen pisteet
		//  flags & 16: 0: 0 pistett‰ k‰sitell‰‰n nollana, 16: 0 pistett‰ muutetaan suurimmaksi.
		//  flags & 32: yhdistelm‰ 1000*piste[2]+piste[1]/100
		//  flags & 64: poissaolevalle pisteet 0
		//  flags & 128: poissaolevalle pistelis‰ys 2^31

			_swab((char *) &Srj, keyv, 2);
			if (Ed < 0) {
				pstindex.flags = (RYlimmasta ? 0 : 1);
				pstindex.flags |= (RPoissa * 64);
				pstindex.flags |= (RPuuttuu ? 16 : 0);
				pstindex.flags |= RPistekentta;
				ok = !teeindeksi(&pstindex, true);
				memset(keyn, 0, sizeof(keyn));
				memcpy(keyn, keyv, 2);
				if (ok)
					ok = !searchix(&pstindex, keyn, &d);
				if (!ok || memcmp(keyn, keyv, 2))
					d = -1;
				}
			else {
				ok = !nextix(&pstindex, keyn, &d);
				if (!ok || memcmp(keyn, keyv, 2))
					d = -1;
				}
			break;
		}
	if (d > 0)
		kilp->GETREC(d);
	return(d);
}
//---------------------------------------------------------------------------
bool TFormArvonta::SarjaValittu(int Srj)
{
	for (int rv = 0; rv < LBSarjaVal->Items->Count; rv++) {
		if (haesarja_w(LBSarjaVal->Items->Strings[rv].c_str(), false) == Srj)
			return(true);
		}
	return(false);
}
//---------------------------------------------------------------------------
void __fastcall TFormArvonta::BtnToteutaClick(TObject *Sender)
{
	int Toiminto, Pv, n, d, kno, epv, vpv;
	int jarj[8] = {0,0,6,0,7,7};
	kilptietue kilp;

	Sleep(100);
	Toiminto = RGToiminto->ItemIndex;
	Pv = CBPaiva->ItemIndex;
	RYlimmasta = Toiminto == 4;
	RPuuttuu = RGPuuttRArv->ItemIndex == 1;
	RPoissa = RGPoissaRArv->ItemIndex;
	switch (CBRankipisteet->ItemIndex) {
		case 0:
			RPistekentta = 4;
			break;
		case 1:
			RPistekentta = 2;
			break;
		case 2:
			RPistekentta = 32;
			break;
		}

	if (ToimintaTila ==2 && Pv <= k_pv+1) {
		Application->MessageBoxW(L"Tulospalvelutilassa voidaan arpoa vain myˆhempi‰ vaiheita", L"Rajoitus", MB_OK);
		return;
		}
	if (Pv > 0) {
		int n = 0;
		for (int srj = 0; srj < sarjaluku; srj++)
			if (SarjaValittu(srj) && Sarjat[srj].luonne[Pv-1] < 0)
				n++;
		if (n > 0) {
			if (Application->MessageBoxW(L"Kaikki valitut sarjat eiv‰t osallistu valittuun vaiheeseen. Arvotaanko osallistuvat sarjat?", L"Varoitus", MB_YESNO)
				!= IDYES)
				return;
			}
		}
	if (CB2pv->ItemIndex > 0) {
		int n = 0;
		for (int srj = 0; srj < sarjaluku; srj++)
			if (SarjaValittu(srj) && (Sarjat[srj].luonne[0] < 0 || Sarjat[srj].luonne[1] < 0))
				n++;
		if (n > 0) {
			Application->MessageBoxW(L"Kaikki valitut sarjat eiv‰t osallistu kahteen ensimm‰iseen p‰iv‰‰n. Valittua arvontaa ei suoriteta.", L"Rajoitus", MB_OK);
			return;
			}
		}
	if (Toiminto == 0 || Toiminto == 1 || Toiminto == 3) {
		if (Toiminto == 3) {
			int n = 0;
			for (int srj = 0; srj < sarjaluku; srj++)
				if (SarjaValittu(srj))
					n++;
			if (RGRajaustapa->ItemIndex == 0 && n != 1) {
				Application->MessageBoxW(L"Osittainen arvonta tehd‰‰n yhdelle sarjalle kerrallaan", L"Rajoitus", MB_OK);
				return;
				}
			}
		arvo(Toiminto);
		}
	else {
		for (int srj = 0; srj < sarjaluku; srj++) {
			int alkuno;

			if (!SarjaValittu(srj))
				continue;
			alkuno = Sarjat[srj].sarjaalku;
			if (Pv > 0) {
				arv_pv = Pv-1;
				epv = Pv-1;
				vpv = Pv-1;
				}
			else {
				arv_pv = 0;
				epv = Sarjat[srj].enspv;
				vpv = Sarjat[srj].viimpv;
				}
			d = -1;
			n = 0;
			EnterCriticalSection(&tall_CriticalSection);
			do {
				d = SeurPos(&kilp, jarj[Toiminto], srj, d, kilpparam.sarjavaihtelee ? epv : -1);
				if (d > 0) {
					if (Toiminto != 2 && CBOhitaVakantit->Checked && !SprLisa && kilp.tark(epv) == L'V')
						continue;
					for (int i = epv; i <= vpv; i++) {
						if (Toiminto == 2) {
							if (Pv > 0)
								kno = kilp.id(i);
							else
								kno = kilp.id();
							if (kilpparam.bibvaihtelee)
								alkuno = Sarjat[srj].bibalku[i];
							kilp.tall_lajat_pv(Sarjat[srj].enslahto[i] +
								((kno - alkuno)/Sarjat[srj].pari[i])*Sarjat[srj].lahtovali[i]*SEK, i);
							}
						else {
							kilp.tall_lajat_pv(Sarjat[srj].enslahto[i] +
								(n/Sarjat[srj].pari[i])*Sarjat[srj].lahtovali[i]*SEK, i);
							}
						}
					n++;
					kilp.tallenna(d, 0, 0, 0, NULL);
					}
				} while(d > 0);
			LeaveCriticalSection(&tall_CriticalSection);
			}
		}
	Application->MessageBoxW(L"Toiminto toteutettu", L"Valmis", MB_OK);
	EdtArvottava->Text = L"";
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::FormResize(TObject *Sender)
{
	LBSarjat->Height = ClientHeight - LBSarjat->Top - 37 * Screen->PixelsPerInch / 96;
	LBSarjaVal->Height = ClientHeight - LBSarjaVal->Top - 37 * Screen->PixelsPerInch / 96;
	Button3->Top = ClientHeight - 30 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::Button2Click(TObject *Sender)
{
	tarkNumerotila(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::BtnLisKaikkiClick(TObject *Sender)
{
	for (int srj = 0; srj < sarjaluku; srj++)
		SarjaValinnat[srj] = true;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::BtnPoisKaikkiClick(TObject *Sender)
{
	for (int srj = 0; srj < sarjaluku; srj++)
		SarjaValinnat[srj] = false;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::BtnLisValitutClick(TObject *Sender)
{
	for (int rv = 0; rv < LBSarjat->Items->Count; rv++) {
		if (LBSarjat->Selected[rv]) {
			int srj;
			if ((srj = haesarja_w(LBSarjat->Items->Strings[rv].c_str(), false)) >= 0)
				SarjaValinnat[srj] = true;
			}
		}
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::BtnPoisValitutClick(TObject *Sender)
{
	for (int rv = 0; rv < LBSarjaVal->Items->Count; rv++) {
		if (LBSarjaVal->Selected[rv]) {
			int srj;
			if ((srj = haesarja_w(LBSarjaVal->Items->Strings[rv].c_str(), false)) >= 0)
				SarjaValinnat[srj] = false;
			}
		}
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::RGSeuraClick(TObject *Sender)
{
	kilpparam.seuralyh_on = RGSeura->ItemIndex == 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::BtnNumeroiClick(TObject *Sender)
{
	bool NumKaikki, keskeyta = false;
	int NumKaikkiPaivat = 0;
	int Toiminto, Pv, d, kno, epv, vpv, muutos;
	int jarj[9] = {4,1,6,6,0,7,7,6,0};
	kilptietue kilp;

	Sleep(100);
	NumKaikki = RGRajaus->ItemIndex == 0;
	Toiminto = RGNumToiminto->ItemIndex;
	Pv = CBNumPaiva->ItemIndex;
	RYlimmasta = Toiminto == 6;
	RPuuttuu = RGPuuttRNum->ItemIndex == 1;
	RPoissa = RGPoissaRNum->ItemIndex;
	switch (CBRankipisteet->ItemIndex) {
		case 0:
			RPistekentta = 4;
			break;
		case 1:
			RPistekentta = 2;
			break;
		case 2:
			RPistekentta = 32;
			break;
		}
	SprLisa = 0;
	if (!kilpparam.bibvaihtelee || RGNumPaivat->ItemIndex == 0)
		NumKaikkiPaivat = 1;
	else if (RGNumPaivat->ItemIndex == 1) {
		NumKaikkiPaivat = 2;
		SprLisa = _wtoi(EdtSprLisa->Text.c_str());
		}
	else
		NumKaikkiPaivat = 0;
	if (ToimintaTila ==2 && Pv <= k_pv) {
		Application->MessageBoxW(L"Tulospalvelutilassa voidaan numeroida vain myˆhempi‰ vaiheita", L"Rajoitus", MB_OK);
		return;
		}
	if (NumKaikki && Toiminto <= 2 && tarkNumerotila(false))
		return;

	if (kilpparam.bibvaihtelee) {
		int vast;
		if (NumKaikkiPaivat)
			vast = Application->MessageBoxW(L"Numerointi perustuu perusnumeroiden alkunumeroihin. Toteuta numerointi?", L"Vahvista", MB_YESNO);
		else
			vast = Application->MessageBoxW(L"Numerointi perustuu vaihekohtaisiin alkunumeroihin. Toteuta numerointi?", L"Vahvista", MB_YESNO);
		if (vast != IDYES)
			return;
		}
	SprLisa = 0;
	if (NumKaikkiPaivat == 2)
		SprLisa = _wtoi(EdtSprLisa->Text.c_str());
	OhitaVakantit = CBOhitaVakantit->Checked;
	arv_pv = Pv;
	if (Toiminto == 4) {
		if (CBOhitaVakantit->Checked && !SprLisa)
			sarjaindex->flags = 0x4000;
		teeindeksi(sarjaindex, true);
		sarjaindex->flags = 0;
		}
	for (int srj = 0; srj < sarjaluku; srj++) {
		if (!SarjaValittu(srj))
			continue;
		switch (Toiminto) {
			case 0:                    // Numerointi sarjoittain l‰htˆj‰rjestykss‰
				if (NumKaikkiPaivat)
					kno = Sarjat[srj].sarjaalku;
				else
					kno = Sarjat[srj].bibalku[arv_pv];
				d = -1;
				EnterCriticalSection(&tall_CriticalSection);
				do {
					d = SeurPos(&kilp, 4, srj, d, arv_pv);
					if (d > 0) {
						kilp.GETREC(d);
						if (OhitaVakantit && kilp.tark(arv_pv) == L'V') {
							int kk = kilp.id(arv_pv);
							kilp.setIdAll(kk+SprLisa);
							kilp.setId(kk, arv_pv);
							kilp.setId(kk, -1);
							kilp.tallenna(d, 0, 0, 0, NULL);
							}
						else if (NumKaikki || kilp.id() == 0) {
							if (NumKaikkiPaivat || kno != kilp.id()) {
								if (NumKaikkiPaivat) {
									kilp.setIdAll(kno+SprLisa);
									if (SprLisa) {
										kilp.setId(kno, arv_pv);
										kilp.setId(kno, -1);
										for (int ipv = arv_pv+1;
											ipv <= Sarjat[kilp.Sarja(arv_pv)].viimpv; ipv++)
											kilp.set_tark(L'V', ipv);
										}
									}
								else
									kilp.setId(kno, arv_pv);
								kilp.tallenna(d, 0, 0, 0, NULL);
								}
							if (NumKaikki)
								kno++;
							else
								kno = haeseurvapaa(kno);
							}
						}
					} while(d > 0);
				LeaveCriticalSection(&tall_CriticalSection);
				break;
			case 1:                    // Numerointi sarjoittain aakkosj‰rjestykss‰
				if (NumKaikkiPaivat)
					kno = Sarjat[srj].sarjaalku;
				else
					kno = Sarjat[srj].bibalku[arv_pv];
				d = -1;
				EnterCriticalSection(&tall_CriticalSection);
				do {
					d = SeurPos(&kilp, 1, srj, d, arv_pv);
					if (d > 0) {
						if (OhitaVakantit && kilp.tark(arv_pv) == L'V') {
							int kk = kilp.id(arv_pv);
							kilp.setIdAll(kk+SprLisa);
							kilp.setId(kk, arv_pv);
							kilp.setId(kk, -1);
							kilp.tallenna(d, 0, 0, 0, NULL);
							}
						else {
							if ((NumKaikkiPaivat || kno != kilp.id())
								&& (NumKaikki || kilp.id() == 0)) {
								if (NumKaikkiPaivat) {
									kilp.setIdAll(kno+SprLisa);
									if (SprLisa) {
										kilp.setId(kno, arv_pv);
										kilp.setId(kno, -1);
										for (int ipv = arv_pv+1;
											ipv <= Sarjat[kilp.Sarja(arv_pv)].viimpv; ipv++)
											kilp.set_tark(L'V', ipv);
										}
									}
								else
									kilp.setId(kno, arv_pv);
								kilp.tallenna(d, 0, 0, 0, NULL);
								}
							if (NumKaikki)
								kno ++;
							else
								kno = haeseurvapaa(kno);
							}
						}
					} while(d > 0);
				LeaveCriticalSection(&tall_CriticalSection);
				break;
			case 2:                    // Numerointi sarjoittain tallennusj‰rjestykss‰
			case 3:                    // Kopioi perusnumerot muille vaiheille
			case 5:					   // Numeroi rankin mukaan alkaen alimmasta
			case 6:                    // Numeroi rankin mukaan alkaen korkeimmasta
			case 7:                    // Numeroiden muuttaminen
				if (NumKaikkiPaivat)
					kno = Sarjat[srj].sarjaalku;
				else
					kno = Sarjat[srj].bibalku[arv_pv];
				d = -1;
				if (Toiminto == 7) {
					muutos = _wtoi(EdMuutos->Text.c_str());
					}
				EnterCriticalSection(&tall_CriticalSection);
				do {
					d = SeurPos(&kilp, jarj[Toiminto], srj, d, arv_pv);
					if (d > 0) {
						if (OhitaVakantit && kilp.tark(arv_pv) == L'V') {
							int kk = kilp.id(arv_pv);
							kilp.setIdAll(kk+SprLisa);
							kilp.setId(kk, arv_pv);
							kilp.setId(kk, -1);
							kilp.tallenna(d, 0, 0, 0, NULL);
							}
						else {
							if (Toiminto == 3) {
								if (NumKaikkiPaivat) {
									int kk = kilp.kilpno;
									kilp.setIdAll(kk+SprLisa);
									if (SprLisa) {
										kilp.setId(kk, arv_pv);
										kilp.setId(kk, -1);
										for (int ipv = arv_pv+1;
											ipv <= Sarjat[kilp.Sarja(arv_pv)].viimpv; ipv++)
											kilp.set_tark(L'V', ipv);
										}
									}
								else
									kilp.setId(kilp.kilpno, arv_pv);
								kilp.tallenna(d, 0, 0, 0, NULL);
								}
							else if (Toiminto == 7) {
								if (NumKaikkiPaivat) {
									int kk = kilp.kilpno + muutos;
									kilp.setIdAll(kk+SprLisa);
									if (SprLisa) {
										kilp.setId(kk, arv_pv);
										kilp.setId(kk, -1);
										}
									}
								else
									kilp.setId(kilp.id() + muutos, arv_pv);
								kilp.tallenna(d, 0, 0, 0, NULL);
								}
							else {
								if (NumKaikki || kilp.kilpno == 0) {
									if (NumKaikkiPaivat || kno != kilp.id()) {
										if (NumKaikkiPaivat) {
											kilp.setIdAll(kno+SprLisa);
											if (SprLisa) {
												kilp.setId(kno, arv_pv);
												kilp.setId(kno, -1);
												for (int ipv = arv_pv+1;
													ipv <= Sarjat[kilp.Sarja(arv_pv)].viimpv; ipv++)
													kilp.set_tark(L'V', ipv);
												}
											}
										else
											kilp.setId(kno, arv_pv);
										kilp.tallenna(d, 0, 0, 0, NULL);
										}
									if (NumKaikki)
										kno++;
									else
										kno = haeseurvapaa(kno);
									}
								}
							}
						}
					} while(d > 0);
				LeaveCriticalSection(&tall_CriticalSection);
				break;
			case 4:					// Arvo numerot
				arvonumerot(srj, false, NumKaikkiPaivat, sarjaindex);
				break;
			}
		if (keskeyta)
			break;
		}
	SprLisa = 0;
	if (ToimintaTila == 2)
		arv_pv = k_pv;
	else
		arv_pv = 0;
	luo_posarr();
	if (!keskeyta)
		Application->MessageBoxW(L"Toiminto toteutettu", L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Arvonta");
}
//---------------------------------------------------------------------------


void __fastcall TFormArvonta::BtnJaettavaClick(TObject *Sender)
{
	for	(int srj = 0; srj < sarjaluku; srj++) {
		if (SarjaValittu(srj)) {
			Jaettava->Text = Sarjat[srj].sarjanimi;
			break;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::BtnMuodostettavatClick(TObject *Sender)
{
	int n = 0;
	SGMuodostettavat->RowCount = 2;
	arv_pv = CBJakoVaiheet->ItemIndex-1;
	for (int col = 0; col < 3; col++)
		SGMuodostettavat->Cells[col][1] = L"";
	for	(int srj = 0; srj < sarjaluku; srj++) {
		if (SarjaValittu(srj)) {
			n++;
			if (SGMuodostettavat->RowCount < n+1)
				SGMuodostettavat->RowCount = n+1;
			SGMuodostettavat->Cells[0][n] = Sarjat[srj].sarjanimi;
			SGMuodostettavat->Cells[1][n] = arv_pv < 0 ? nilm[srj] : nilm_pv[arv_pv][srj];
			SGMuodostettavat->Cells[2][n] = L"";
			}
		}
}

static wchar_t jakotapa[] = L"RPATY";
//---------------------------------------------------------------------------
void __fastcall TFormArvonta::ValmisteleJako(int jsrj, int nn, int *msrj, int n, int *ns, wchar_t tapa)
{
	int n0, a, y, nv = 0;
	for (int i = 0; i < n; i++) {
		if (msrj[i] != jsrj)
			nv += arv_pv < 0 ? nilm[msrj[i]] : nilm_pv[arv_pv][msrj[i]];
		}
	n0 = (nv + nn) / n;
	if (tapa != L'P' && tapa != L'Y' && n0 % 2 == 1) {
		y = n - 1;
		a = y - (nv + nn) % n + 1;
		}
	else  {
		a = 0;
		y = (nv + nn) % n - 1;
		}
	for (int i = 0; i < n; i++) {
		if (msrj[i] != jsrj)
			ns[i] = n0 - (arv_pv < 0 ? nilm[msrj[i]] : nilm_pv[arv_pv][msrj[i]]) + (i >= a && i <= y);
		else
			ns[i] = n0 + (i >= a && i <= y);
		SGMuodostettavat->Cells[2][i+1] = UnicodeString(ns[i]);
		}
}
//---------------------------------------------------------------------------
int arvovaihe(int srj, int nn, int ipv, int sailpo, int hiihtoarv, bool osasarja, wchar_t tyyli, wchar_t cpv, kilpindex *aktindex);

void TFormArvonta::JaaSeuratKoossa(int jsrj, int *msrj, int n, int *nsrj)
{
	char key[MAXINDL], skey[2];
	struct {
		int lkm;
		char key[MAXINDL];
	} seuralkm[500];
	int d, isra, ok;
	int ns1[20], nvap, k = 0;

	memset(ns1, 0, sizeof(ns1));
	memset(seuralkm, 0, sizeof(seuralkm));
	if (teeindeksi(sarjaindex, true))
		return;
	_swab((char *)&jsrj, skey, 2);
	memset(key, 0, sizeof(key));
	memcpy(key, skey, 2);
	ok = !searchix(sarjaindex, key, &d);
	isra = 0;
	while (ok && !memcmp(key, skey, 2)) {
		if (isra == 0 || memcmp(seuralkm[isra-1].key, key, 23)) {
			if (isra > 499)
				return;
			memcpy(seuralkm[isra++].key, key, 23);
			}
		seuralkm[isra-1].lkm++;
		ok = !nextix(sarjaindex, key, &d);
		}
	if (!isra)
		return;
	quicksort((char *)seuralkm, isra, 4, sizeof(seuralkm[0]));
	isra--;
	EnterCriticalSection(&tall_CriticalSection);
	for ( ; isra >= 0; isra--) {
		memcpy(key, seuralkm[isra].key, MAXINDL);
		searchix(sarjaindex, key, &d);
		while (seuralkm[isra].lkm) {
			kilptietue kilp;

			nvap = 0;
			k = -1;
			for (int i = 0; i < n; i++) {
				if (nvap < nsrj[i] - ns1[i]) {
					k = i;
					nvap = nsrj[i] - ns1[i];
					}
				}
			if (k == -1)
				break;
			while (d > 0) {
				kilp.GETREC(d);
				kilp.setSarjaAll(msrj[k]);
				kilp.tallenna(d, 0, 0, 0, 0);
				ns1[k]++;
				seuralkm[isra].lkm--;
				if (seuralkm[isra].lkm == 0 || nsrj[k] - ns1[k] == 0)
					break;
				ok = !nextix(sarjaindex, key, &d);
				if (!ok || memcmp(key, seuralkm[isra].key, 23))
					break;
				}
			}
		if (k == -1)
			break;
		}
	LeaveCriticalSection(&tall_CriticalSection);
}

void __fastcall TFormArvonta::ToteutaJako(int jsrj, int nn, int *msrj, int n, int *nsrj, wchar_t tapa)
{
	int *d, nv[20], nu[20], alkuno;
	kilptietue kilp;

	if (tapa == L'A' || tapa == L'T') {
		if (teeindeksi(sarjaindex, true))
			return;
		if (arvovaihe(jsrj, nn, arv_pv, 1, tapa == L'A', 0, L'A', L'1', sarjaindex))
			return;
		}
	if (tapa == L'P') {
		for (int rs = 0; rs < SGMuodostettavat->RowCount-1; rs++) {
			int n1;
			nv[rs] = _wtoi(SGMuodostettavat->Cells[1][rs+1].c_str());
			n1 = _wtoi(SGMuodostettavat->Cells[2][rs+1].c_str());
			d = new int[n1+1];
			d[n1] = -1;
			EnterCriticalSection(&tall_CriticalSection);
			for (int i = n1 - 1; i >= 0; i--) {
				if ((d[i] = SeurPos(&kilp, tapa == L'P' ? 7 : 4, jsrj, d[i+1], 0)) < 0)
					break;
				}
			for (int i = 0; i < n1; i++) {
				if (d[i] < 0)
					break;
				kilp.GETREC(d[i]);
				if (CBJakoVaiheet->ItemIndex <= 0) {
					kilp.setSarjaAll(msrj[rs]);
					}
				else {
					kilp.setSarja(msrj[rs], arv_pv);
					}
				if (!kilpparam.bibvaihtelee || CBJakoVaiheet->ItemIndex <= 0) {
					alkuno = Sarjat[msrj[rs]].sarjaalku;
					}
				else {
					alkuno = Sarjat[msrj[rs]].bibalku[arv_pv];
					}
				if (CBoxLajat->Checked) {
					kilp.setIdAll(alkuno + nv[rs] + i);
					kilp.pv[0].tlahto = Sarjat[msrj[rs]].enslahto[arv_pv] + SEK * Sarjat[msrj[rs]].lahtovali[arv_pv] * (nv[rs] + i);
					}
				kilp.tallenna(d[i], 0, 0, 0 ,0);
				}
			LeaveCriticalSection(&tall_CriticalSection);
			delete[] d;
			}
		}
	else if (tapa == L'Y') {
		JaaSeuratKoossa(jsrj, msrj, n, nsrj);
		}
	else {
		int rs, suunta = 1, ntot;

		for (int rs = 0; rs < SGMuodostettavat->RowCount-1; rs++) {
			nv[rs] = _wtoi(SGMuodostettavat->Cells[1][rs+1].c_str());
			nu[rs] = _wtoi(SGMuodostettavat->Cells[2][rs+1].c_str());
			}
		EnterCriticalSection(&tall_CriticalSection);
		ntot = nn;
		rs = -1;
		d = new int[nn+1];
		d[0] = -1;
		for (int i = 0; i < ntot; i++) {
			if ((d[i+1] = SeurPos(&kilp, tapa == L'R' ? 7 : 4, jsrj, d[i], 0)) < 0) {
				break;
				}
			}
		for (int i = 0; i < ntot; i++) {
			if (d[i+1] < 0)
				break;
			for (int j = 0; j < 2 * n - 1; j++) {
				if (rs == 0 && suunta == -1)
					suunta = 1;
				else if (rs == n-1 && suunta == 1)
					suunta = -1;
				else
					rs += suunta;
				if (nu[rs] > 0)
					break;
				}
			if (nu[rs] <= 0) {
//				LeaveCriticalSection(&tall_CriticalSection);
				break;
				}
			nu[rs]--;
			kilp.GETREC(d[i+1]);
			if (!kilpparam.bibvaihtelee || CBJakoVaiheet->ItemIndex <= 0) {
				alkuno = Sarjat[msrj[rs]].sarjaalku;
				}
			else {
				alkuno = Sarjat[msrj[rs]].bibalku[arv_pv];
				}
			if (CBJakoVaiheet->ItemIndex <= 0) {
				kilp.setSarjaAll(msrj[rs]);
				if (CBoxLajat->Checked) {
					kilp.setIdAll(alkuno + nv[rs] + nu[rs]);
					kilp.tall_lajat_pv(Sarjat[msrj[rs]].enslahto[0] +
						SEK * (nv[rs] + nu[rs]) * Sarjat[msrj[rs]].lahtovali[0], 0);
					}
				}
			else {
				kilp.setSarja(msrj[rs], arv_pv);
				if (CBoxLajat->Checked) {
					kilp.setId(alkuno + nv[rs] + nu[rs], arv_pv);
					kilp.tall_lajat_pv(Sarjat[msrj[rs]].enslahto[arv_pv] +
						SEK * (nv[rs] + nu[rs]) * Sarjat[msrj[rs]].lahtovali[arv_pv], 0);
					}
				}
			kilp.tallenna(d[i+1], 0, 0, 0 ,0);
			}
		delete[] d;
		LeaveCriticalSection(&tall_CriticalSection);
		}
	reindex = 1;
}
//---------------------------------------------------------------------------
void __fastcall TFormArvonta::BtnJaaClick(TObject *Sender)
{
	int jsrj, msrj[20], nsrj[20], n, nn;
	wchar_t tapa;

	RYlimmasta = true;
	switch (CBRankipisteet->ItemIndex) {
		case 0:
			RPistekentta = 4;
			break;
		case 1:
			RPistekentta = 2;
			break;
		case 2:
			RPistekentta = 32;
			break;
		}
	jsrj = haesarja_w(Jaettava->Text.c_str(), false);
	n = 0;
	arv_pv = CBJakoVaiheet->ItemIndex-1;
	nn = arv_pv < 0 ? nilm[jsrj] : nilm_pv[arv_pv][jsrj];
	if (jsrj < 0 || RGjakotapa->ItemIndex < 0 || nn < 2) {
		if (arv_pv >= 0 && jsrj >= 0 && Sarjat[jsrj].luonne[arv_pv] < 0)
			Application->MessageBoxW(L"Jaettavan sarjan on oltava aktiivinen k‰sitelt‰v‰ss‰ vaiheessa", L"Virhe", MB_OK);
		else
			Application->MessageBoxW(L"Puutteellisen valinnat tai jaettavassa sarjassa ei kilpailijoita", L"Virhe", MB_OK);
		return;
		}
	tapa = jakotapa[RGjakotapa->ItemIndex];
	for (int i = 1; i < SGMuodostettavat->RowCount; i++) {
		wchar_t ln[40];
		wcsncpy(ln, SGMuodostettavat->Cells[0][i].c_str(), 39);
		if (wcslen(ln) > 0) {
			if ((msrj[n] = haesarja_w(ln, false)) >= 0) {
				if (arv_pv >= 0 && Sarjat[msrj[n]].luonne[arv_pv] < 0) {
					Application->MessageBoxW(L"Kaikkien muodostettavien sarjojen on oltava aktiivisia k‰sitelt‰v‰ss‰ vaiheessa", L"Virhe", MB_OK);
					return;
					}
				n++;
				}
			}
		}
	if (n < 1) {
		Application->MessageBoxW(L"Ei muodostettavia sarjoja", L"Virhe", MB_OK);
		return;
		}
	ValmisteleJako(jsrj, nn, msrj, n, nsrj, tapa);
	if (arv_pv < 0)
		arv_pv = 0;
	if (Application->MessageBoxW(L"Toteuta jako", L"Vahvistus", MB_YESNO) == IDYES)
		ToteutaJako(jsrj, nn, msrj, n, nsrj, tapa);
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::SGMuodostettavatExit(TObject *Sender)
{
	int n, jsrj;

	jsrj = haesarja_w(Jaettava->Text.c_str(), false);
	n = nilm[jsrj];
	for	(int r = 1; r < SGMuodostettavat->RowCount-1; r++) {
		n -= _wtoi(SGMuodostettavat->Cells[2][r].c_str());
		}
	SGMuodostettavat->Cells[2][SGMuodostettavat->RowCount-1] = UnicodeString(n);
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::SGMuodostettavatSelectCell(TObject *Sender, int ACol,
          int ARow, bool &CanSelect)
{
	SGMuodostettavatExit(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::RGjakotapaClick(TObject *Sender)
{
/*
	if (RGjakotapa->ItemIndex < 2) {
		CBoxLajat->Enabled = true;
		}
	else {
		CBoxLajat->Checked = true;
		CBoxLajat->Enabled = false;
		}
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::LBSarjatDblClick(TObject *Sender)
{
	int srj;

	srj = haesarja_w(LBSarjat->Items->Strings[LBSarjat->ItemIndex].c_str(), false);
	if (srj >= 0)
		SarjaValinnat[srj] = true;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------


void __fastcall TFormArvonta::LBSarjaValDblClick(TObject *Sender)
{
	int srj;

	srj = haesarja_w(LBSarjaVal->Items->Strings[LBSarjaVal->ItemIndex].c_str(), false);
	if (srj >= 0)
		SarjaValinnat[srj] = false;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------



void __fastcall TFormArvonta::RGPoissaClick(TObject *Sender)
{
	kilpparam.poissamukaan = RGPoissa->ItemIndex == 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::CBPaivaChange(TObject *Sender)
{
	if (ToimintaTila == 2 && CBPaiva->ItemIndex <= k_pv+1) {
		CBPaiva->ItemIndex = k_pv + 2;
		Application->MessageBoxW(L"Tulospalvelutilassa voi arpoa vain tulevia p‰ivi‰", L"Rajoitus", MB_OK);
	}
	if (kilpparam.n_pv_akt > 2)
		CB2pv->Enabled = (CBPaiva->ItemIndex == 2);
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::CBNumPaivaChange(TObject *Sender)
{
	if (ToimintaTila == 2 && CBNumPaiva->ItemIndex <= k_pv) {
		CBNumPaiva->ItemIndex = k_pv + 1;
		Application->MessageBoxW(L"Tulospalvelutilassa voi numeroida vain tulevia p‰ivi‰", L"Rajoitus", MB_OK);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::RGToimintoClick(TObject *Sender)
{
	GBRValArv->Visible = RGToiminto->ItemIndex >= 4;
	GBUusiArv->Visible = RGToiminto->ItemIndex == 3;
	RGSeura->Visible = RGToiminto->ItemIndex == 0 || RGToiminto->ItemIndex == 3;
	EditAlku->Visible = GBUusiArv->Visible;
	EditLoppu->Visible = GBUusiArv->Visible;
	if (kilpparam.n_pv_akt >= 2) {
		if (RGToiminto->ItemIndex == 3)
			CB2pv->ItemIndex = 0;
			CB2pv->Enabled = RGToiminto->ItemIndex != 3 && (kilpparam.n_pv_akt == 2 || CBPaiva->ItemIndex == 2);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::RGNumToimintoClick(TObject *Sender)
{
	GBRValNum->Visible = RGNumToiminto->ItemIndex == 5 || RGNumToiminto->ItemIndex == 6;
	Panel2->Visible = !GBRValNum->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::TabSheet1Enter(TObject *Sender)
{
	RGPuuttRArv->ItemIndex = RPuuttuu ? 1 : 0;
	RGPoissaRArv->ItemIndex = RPoissa;
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::TabSheet2Enter(TObject *Sender)
{
	RGPuuttRNum->ItemIndex = RPuuttuu ? 1 : 0;
	RGPoissaRNum->ItemIndex = RPoissa;
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::RGPuuttRNumClick(TObject *Sender)
{
	RPuuttuu = RGPuuttRNum->ItemIndex == 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::RGPoissaRNumClick(TObject *Sender)
{
	RPoissa = RGPoissaRNum->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::RGPuuttRArvClick(TObject *Sender)
{
	RPuuttuu = RGPuuttRArv->ItemIndex == 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::RGPoissaRArvClick(TObject *Sender)
{
	RPoissa = RGPoissaRArv->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::CBRankipisteetClick(TObject *Sender)
{
	switch (CBRankipisteet->ItemIndex) {
		case 0:
			RPistekentta = 4;
			break;
		case 1:
			RPistekentta = 2;
			break;
		case 2:
			RPistekentta = 32;
			break;
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormArvonta::Button3Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::RGNumPaivatClick(TObject *Sender)
{
	EdtSprLisa->Visible = RGNumPaivat->ItemIndex == 1;
	LblSprLisa->Visible = EdtSprLisa->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::BtnLisaaLhtSarjatClick(TObject *Sender)
{
	static int i_pv = 0, lno = 0;
	wchar_t ch;

	if (kilpparam.n_pv_akt > 1) {
		i_pv++;
		inputint_prompt(&i_pv, L"Anna vaiheen j‰rjestysnumero", &ch, this);
		if (ch == ESC || i_pv < 1 || i_pv > kilpparam.n_pv_akt)
			return;
		i_pv--;
		}
	inputint_prompt(&lno, L"Anna l‰hdˆn numero", &ch, this);
	if (ch == ESC)
		return;
	for (int srj = 0; srj < sarjaluku; srj++) {
		if (!SarjaValinnat[srj] && Sarjat[srj].lno[i_pv] == lno)
			SarjaValinnat[srj] = true;
		}
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

