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

#include <stdlib.h>
#include <math.h>
#include "HkDeclare.h"


//  tulos1(piste, tm)       palauttaa maaliajasta lasketun tuloksen
//                               kyseessä on päivän tulos paitsi, 
//                               jos piste == kilpparam.valuku+1
//
//  k_tulos(piste, tm)     kuten tulos1 paitsi, että palauttaa
//                               takaa-ajon väliajat kokonaisaikoina
//
//  tall_tulos(piste, tls) tallentaa annetun tuloksen
//                               kilpailijatietueeseen, ei kirjoita levylle
//
//  set_tulos(piste, tm)   määrää tuloksen funktion tulos1 avulla
//                               ja tallentaa sen funktiolla tall_tulos
//
//  merk_sijat(void)             määrää sijat tietueen kilp tulosten perusteella
//                               ja tallentaa ne samaan tietueeseen, ei kirjoita
//                               levylle
//
//  set_sija(piste, sj)    tallentaa annetun sijoituksen
//                               kilpailijatietueeseen, ei kirjoita levylle
//
//  p_sija(piste)          palauttaa kilpailijan sijan. Kyseessä on
//                               päivän sija paitsi, jos piste == kilpparam.valuku+1
//
//  p_aika(piste)          palauttaa kilpailijan tuloksen. Kyseessä on
//                               päivän tulos paitsi, jos piste == kilpparam.valuku+1
//								 pista == -1 antaa lähtöportin ajan
//
//  tulos2(piste)          kuten p_aika paitsi, että palauttaa
//                               takaa-ajon väliajat kokonaisaikoina
//
//  ytulos_p(piste)          kuten tulos2 paitsi, että palauttaa
//                               aina väliajat kokonaisaikoina
//
//  tulos_pv(i_pv)           palauttaa kilpailijan päiväkohtaisen
//                               lopputuloksen, hylätyille 0
//
//  tall_tulos_pv(tls, i_pv)     mestitsee kilpailijan päiväkohtaisen
//                               lopputuloksen
//
//  ytulos(i_pv)             palauttaa kilpailijan yhteistuloksen
//
//  sija(i_pv)                palauttaa kilpailijan sijoituksen päivänä i_pv
//
//  y_sija(i_pv)              palauttaa kilpailijan yhteissijan
//
//  maali(piste)           palauttaa kilpailijan lähtöporttiajan, maaliajan, tai
//                               väliajan ottohetken
//
//  hyv(void)                    tosi, jos kilpailija hyväksytty
//
//  hyv_pv(i_pv)             tosi, jos hyväksytty päivänä pv
//
//  tark(i_pv)               tarkastusmerkintä päivänä pv
//
//  ytark(i_pv)              hyväksymistilanne päivän i_pv jälkeen
//
//  set_tark(kh, pv)       tallentaa tarkastusmerkinnän kh
//                               kilpailijatietueeseen, ei kirjoita levylle
//

void vatp::operator=(vatp& va)
{
	vatulos = va.vatulos;
	vasija = va.vasija;
	vakoodi = va.vakoodi;
}

void kilppvtp::operator=(kilppvtp& pv1)
{
	memcpy(txt, pv1.txt, sizeof(txt));
	memcpy(rata, pv1.rata, sizeof(rata));
	memcpy(badge, pv1.badge, sizeof(badge));
	memcpy(laina, pv1.laina, sizeof(laina));
	memcpy(selitys, pv1.selitys, sizeof(selitys));
	memcpy(pvpisteet, pv1.pvpisteet, sizeof(pvpisteet));
	enn = pv1.enn;
	tav = pv1.tav;
	uusi = pv1.uusi;
	vac = pv1.vac;
	flags[0] = pv1.flags[0];
	flags[1] = pv1.flags[1];
	sarja = pv1.sarja;
	era = pv1.era;
	bib = pv1.bib;
	tlahto = pv1.tlahto;
	keskhyl = pv1.keskhyl;
	memcpy(asakot, pv1.asakot, sizeof(asakot));
	tasoitus = pv1.tasoitus;
	sakko = pv1.sakko;
	ysija = pv1.ysija;
	for (int i = 0; i < kilpparam.valuku+2; i++) {
		 va[i] = pv1.va[i];
	}
}

void kilptietue::operator=(kilptietue& kilp)
{
	kilpstatus = kilp.kilpstatus;
	kilpno = kilp.kilpno;
	lisno[0] = kilp.lisno[0];
	lisno[1] = kilp.lisno[1];
	ilmlista = kilp.ilmlista;
	piiri = kilp.piiri;
	memcpy(wrkoodi, kilp.wrkoodi, sizeof(kilp.wrkoodi));
	memcpy(pisteet, kilp.pisteet, sizeof(kilp.pisteet));
	memcpy(arvo, kilp.arvo, sizeof(kilp.arvo));
	memcpy(sukunimi, kilp.sukunimi, sizeof(kilp.sukunimi));
	memcpy(etunimi, kilp.etunimi, sizeof(kilp.etunimi));
	memcpy(seura, kilp.seura, sizeof(kilp.seura));
	memcpy(seuralyh, kilp.seuralyh, sizeof(kilp.seuralyh));
	memcpy(yhdistys, kilp.yhdistys, sizeof(kilp.yhdistys));
	memcpy(joukkue, kilp.joukkue, sizeof(kilp.joukkue));
	memcpy(maa, kilp.maa, sizeof(kilp.maa));
	sarja = kilp.sarja;
	sukup = kilp.sukup;
	ikasarja = kilp.ikasarja;
	alisarja = kilp.alisarja;
	synt = kilp.synt;
	arvryhma = kilp.arvryhma;
	for (int i = 0; i < kilpparam.n_pv_akt; i++) {
		 pv[i] = kilp.pv[i];
	}
}

bool vatp::operator==(vatp& va)
{
	if (vatulos != va.vatulos)
		return(false);
	if (vakoodi != va.vakoodi)
		return(false);
	return(true);
}

int kilppvtp::vert0(kilppvtp &pv1)
{
	int ret = 0;
	int ero = 0;

	if (wcscmp(txt, pv1.txt)) {
		ret |= 1;
		ero = 1;
		}
	if (enn != pv1.enn) {
		ret |= 1;
		ero = 1;
		}
	if (tav != pv1.tav) {
		ret |= 1;
		ero = 1;
		}
	if (uusi != pv1.uusi) {
		ret |= 1;
		ero = 1;
		}
	if (vac != pv1.vac) {
		ret |= 1;
		ero = 1;
		}
	if (flags[0] != pv1.flags[0]) {
		ret |= 1;
		ero = 1;
		}
	if (flags[1] != pv1.flags[1]) {
		ret |= 1;
		ero = 1;
		}
	if (wcscmp(rata, pv1.rata)) {
		ret |= 1;
		ero = 1;
		}
	if (badge[0] != pv1.badge[0] || badge[1] != pv1.badge[1]) {
		ret |= 1;
		ero = 1;
		}
	if (laina[0] != pv1.laina[0] || laina[1] != pv1.laina[1]) {
		ret |= 1;
		ero = 1;
		}
	if (wcscmp(selitys, pv1.selitys)) {
		ret |= 1;
		ero = 1;
		}
	if (pvpisteet[0] != pv1.pvpisteet[0] || pvpisteet[1] != pv1.pvpisteet[1]) {
		ret |= 1;
		ero = 1;
		}
	if (sarja != pv1.sarja) {
		ret |= 1;
		ero = 1;
		}
	if (bib != pv1.bib) {
		ret |= 1;
		ero = 1;
		}
	if (era != pv1.era) {
		ret |= 1;
		ero = 1;
		}
	if (tlahto != pv1.tlahto) {
		ret |= 2;
		ero = 1;
		}
	if (keskhyl != pv1.keskhyl) {
		ret |= 4;
		ero = 1;
		}
	if (kilpparam.lsakmax > 0 && memcmp(asakot, pv1.asakot, sizeof(asakot))) {
		ret |= 8;
		ero = 1;
		}
	if (tasoitus != pv1.tasoitus) {
		ret |= 16;
		ero = 1;
		}
	if (sakko != pv1.sakko) {
		ret |= 16;
		ero = 1;
		}
	if (ero)
		return(ret);
	return(0);
}

bool kilppvtp::operator==(kilppvtp& pv1)
{
	if (vert0(pv1) != 0)
		return(false);
	for (int i = 0; i < kilpparam.valuku+2; i++) {
		if (!(va[i] == pv1.va[i]))
			return(false);
	}
	return(true);
}

int kilptietue::vert0(kilptietue &kilp)
{
	int ret = 0, ero = 0;

	if (kilpstatus != kilp.kilpstatus) {
		ret |= 1;
		ero = 1;
		}
	if (kilpno != kilp.kilpno) {
		ret |= 1;
		ero = 2;
		}
	if (lisno[0] != kilp.lisno[0]) {
		ret |= 1;
		ero = 3;
		}
	if (lisno[1] != kilp.lisno[1]) {
		ret |= 1;
		ero = 4;
		}
	if (pisteet[0] != kilp.pisteet[0]) {
		ret |= 1;
		ero = 5;
		}
	if (pisteet[1] != kilp.pisteet[1]) {
		ret |= 1;
		ero = 6;
		}
	if (pisteet[2] != kilp.pisteet[2]) {
		ret |= 1;
		ero = 7;
		}
	if (ilmlista != kilp.ilmlista) {
		ret |= 1;
		ero = 8;
		}
	if (piiri != kilp.piiri) {
		ret |= 1;
		ero = 9;
		}
	if (sarja != kilp.sarja) {
		ret |= 1;
		ero = 10;
		}
	if (sukup != kilp.sukup) {
		ret |= 1;
		ero = 11;
		}
	if (ikasarja != kilp.ikasarja) {
		ret |= 1;
		ero = 12;
		}
	if (alisarja != kilp.alisarja) {
		ret |= 1;
		ero = 13;
		}
	if (synt != kilp.synt) {
		ret |= 1;
		ero = 14;
		}
	if (arvryhma != kilp.arvryhma) {
		ret |= 1;
		ero = 15;
		}
	if (wcscmp(wrkoodi, kilp.wrkoodi)) {
		ret |= 1;
		ero = 16;
		}
	if (wcscmp(arvo, kilp.arvo)) {
		ret |= 1;
		ero = 17;
		}
	if (wcscmp(sukunimi, kilp.sukunimi)) {
		ret |= 1;
		ero = 18;
		}
	if (wcscmp(etunimi, kilp.etunimi)) {
		ret |= 1;
		ero = 19;
		}
	if (wcscmp(seura, kilp.seura)) {
		ret |= 1;
		ero = 20;
		}
	if (wcscmp(seuralyh, kilp.seuralyh)) {
		ret |= 1;
		ero = 21;
		}
	if (wcscmp(yhdistys, kilp.yhdistys)) {
		ret |= 1;
		ero = 22;
		}
	if (wcscmp(joukkue, kilp.joukkue)) {
		ret |= 1;
		ero = 23;
		}
	if (wcscmp(maa, kilp.maa)) {
		ret |= 1;
		ero = 24;
		}
	if (ero)
		return(ret);
	return(0);
}


bool kilptietue::operator==(kilptietue& kilp)
{
	if (vert0(kilp) != 0)
		return(false);
	for (int i = 0; i < kilpparam.n_pv_akt; i++) {
		if (!(pv[i] == kilp.pv[i]))
		  return(false);
	}
	return(true);
}

static int kaavatulos(kaavatp *kva, int tlsyla, int tlsala)
{
	int tls = 0;
	if (tlsyla && (!kva->vaadiala || tlsala)) {
		if (kva->ala < 0)
			tls = tlsyla - kva->nollataso;
		else
			tls = tlsyla - tlsala - kva->nollataso;
		if (kva->kerroin)
			tls *= kva->kerroin;
		if (kva->jakaja)
			tls /= kva->jakaja;
		if (tls < kva->minimi)
			tls = kva->minimi;
		if (kva->maksimi && tls > kva->maksimi)
			tls = kva->maksimi;
        }
	return(tls);
}

int kilptietue::tuloskaavat(int kielto)
{
	if (Sarja() < 0)
		return(0);
	for (int va = 0; va < VALUKU + 3; va++) {
		kaavatp *kva = &Sarjat[Sarja()].kaava[k_pv][va];

		if (Sarjat[Sarja()].pistekaava[k_pv].vaiheon && va >= VALUKU)
			break;
		if (kva->on) {
			int tls = 0;

			if (hyv())
				tls = kaavatulos(kva, p_aika(kva->yla, 0, kva->sakotpois ? 1 : 0),
					p_aika(kva->ala, 0, kva->sakotpois ? 1 : 0));

			if (va > VALUKU) {
				pv[k_pv].pvpisteet[va-VALUKU-1] = tls;
				}
			else {
				tall_tulos(va, tls);
				}
			}
		}

	if (Sarjat[Sarja()].pistekaava[k_pv].vaiheon) {
		int karki = pkarki[Sarja()][0], tls;
		if (tulos_pv(k_pv, true, 2) && (karki == TMAALI0 || karki == 0 || karki > p_aika(0, 2, 2)))
			karki = p_aika(0, 2, 2);

		tls = hyv() ? p_aika(0, 2, 2) : 0;
		pv[k_pv].pvpisteet[0] = Pisteet(&Sarjat[Sarja()].pistekaava[k_pv], p_sija(0),
			ntulos[Sarja()][0], tls, karki);
		}
	if (Sarjat[Sarja()].YhtpistOn()) {
		LaskeKilpYhtPisteet(this, &Sarjat[Sarja()].pistekaava[k_pv], Sarja());
		}
	return(0);
}

int kilptietue::id(int i_pv)
{
	if (i_pv >= 0)
		return(pv[i_pv].bib);

	if (ToimintaTila != 2) {
		return(kilpno);
		}
	else {
		return(pv[k_pv].bib);
		}
}

void kilptietue::setIdAll(int id)
{
	kilpno = id;
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		pv[ipv].bib = id;
		}
}

void kilptietue::setId(int id)
{
	if (ToimintaTila != 2 || !kilpparam.bibvaihtelee) {
		setIdAll(id);
		}
	else {
		pv[k_pv].bib = id;
		}
}

void kilptietue::setId(int id, int ipv)
{
	if (!kilpparam.bibvaihtelee) {
		setIdAll(id);
		}
	else if (ipv < 0)
		kilpno = id;
	else
		pv[ipv].bib = id;
}

int kilptietue::Badge(int i_pv, int tn, bool myosPoissa /* = false */)
{
	if (!myosPoissa && pv[i_pv].keskhyl == L'P')
		return(0);
	return(pv[i_pv].badge[tn]);
}

INT16 kilptietue::Sarja(void)
{
	if (ToimintaTila != 2 || !kilpparam.sarjavaihtelee) {
		return(sarja);
		}
	else {
		return(pv[k_pv].sarja);
		}
}

INT16 kilptietue::Sarja(int ipv)
{
	if (ipv < 0 || !kilpparam.sarjavaihtelee) {
		return(sarja);
		}
	else {
		return(pv[ipv].sarja);
		}
}

void kilptietue::setSarjaAll(int srj)
{
	sarja = srj;
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		pv[ipv].sarja = srj;
		}
}

void kilptietue::setSarja(int srj, int ipv)
{
	if (!kilpparam.sarjavaihtelee)
		setSarjaAll(srj);
	else if (ipv < 0)
		sarja = srj;
	else
		pv[ipv].sarja = srj;
}

void kilptietue::setSarja(int srj)
{
	if (ToimintaTila != 2 || !kilpparam.sarjavaihtelee) {
		setSarjaAll(srj);
		}
	else {
		setSarja(srj, k_pv);
		}
}

bool kilptietue::onSarja(wchar_t *snimi, bool hyvYhd)
{
	if (Sarja() < 0)
		return(false);
	if (!wcscmpU(snimi, Sarjat[Sarja()].sarjanimi))
		return(true);
	if (hyvYhd) {
		for (int i = 0; i < nsarjayhd; i++) {
			if (!wcscmpU(snimi, Sarjat[sarjaluku+i].sarjanimi) &&
				yhdsarjat[i][Sarja()])
				return(true);
			}
		}
	return(false);
}

bool kilptietue::onSarja(wchar_t *snimi, int i_pv, bool hyvYhd)
{
	if (Sarja(i_pv) < 0)
		return(false);
	if (!wcscmpU(snimi, Sarjat[Sarja(i_pv)].sarjanimi))
		return(true);
	if (hyvYhd) {
		for (int i = 0; i < nsarjayhd; i++) {
			if (!wcscmpU(snimi, Sarjat[sarjaluku+i].sarjanimi) &&
				yhdsarjat[i][Sarja(i_pv)])
				return(true);
			}
		}
	return(false);
}

wchar_t *kilptietue::SarjaNimi(int ipv, wchar_t *st /* = NULL*/,
	bool pitka /* = false*/)
{
	static wchar_t buf[LPSARJA+1];

	if (st == NULL)
		st = buf;
	if (ipv < 0)
		ipv = k_pv;
	if (pitka && Sarjat[Sarja(ipv)].psarjanimi[0])
		wcsncpy(st, Sarjat[Sarja(ipv)].psarjanimi, LPSARJA+1);
	else
		wcsncpy(st, Sarjat[Sarja(ipv)].sarjanimi, LPSARJA+1);
	return(st);
}

wchar_t *kilptietue::seuratieto(int *len)
{
	if (kilpparam.seuralyh_on) {
		if (len)
			*len = kilpparam.lseuralyh;
		return(seuralyh);
		}
	else {
		if (len)
			*len = kilpparam.lseura;
		return(seura);
		}
}

wchar_t *kilptietue::seuratieto(int *len, bool lyhenne)
{
	if (lyhenne) {
		if (len)
			*len = kilpparam.lseuralyh;
		return(seuralyh);
		}
	else {
		if (len)
			*len = kilpparam.lseura;
		return(seura);
		}
}

wchar_t *kilptietue::yhteisotieto(int laji)
{
	static wchar_t st[10];
	switch (laji) {
		case 1:
			return(seura);
		case 2:
			return(seuralyh);
		case 3:
			return(yhdistys);
		case 4:
			return(joukkue);
		case 5:
			_itow(piiri, st, 10);
			return(st);
		default:
			return(seuratieto(0));
		}
}


void kilptietue::set_seuratieto(wchar_t *snimi)
{
	set_seuratieto(snimi, kilpparam.seuralyh_on);
}

void kilptietue::set_seuratieto(wchar_t *snimi, bool lyhenne)
{
	if (lyhenne) {
		wcsncpy(seuralyh, snimi, kilpparam.lseuralyh);
		}
	else {
		wcsncpy(seura, snimi, kilpparam.lseura);
		}
}

int pyoristatls(int tls, int pyorista, int i_pv /* = -1 */)
{
	if (i_pv < 0)
		i_pv = k_pv;
	if (!tls || tls == TMAALI0 || pyorista >= (int) sizeof(pvparam[i_pv].pyor)/(pvparam[i_pv].pyor[0]))
		return(tls);
	if (tls && (kilpparam.pyorlahimpaan || (kilpparam.kilplaji == L'A' && pyorista == 4)))
		tls += pvparam[i_pv].pyor[pyorista]/2;
	return(pvparam[i_pv].pyor[pyorista] * ((tls + 200*TUNTI)/pvparam[i_pv].pyor[pyorista]) - 200*TUNTI);
}

int kilptietue::asakkolkm(int ipv /* = -1*/)
{
	int isak, lkm = 0;

	if (ipv < 0)
		ipv = k_pv;
	for (int i = 0; i < Sarjat[Sarja(ipv)].lsak[ipv]; i++)
		if ((isak = pv[ipv].asakot[i] - L'0') >= 0 && isak <= kilpparam.maxasakko)
			lkm += isak;
	return(lkm);
}

INT32 kilptietue::p_sakko(INT piste)
	{
	INT32 sak;
	INT ns, nsm = 0, isak;

	if (Sarja() < 0)
		return(0);
	piste = va_piste(piste);
	sak = 0;
	if (kilpparam.kilplaji == L'B') {
		nsm = wcslen(Sarjat[Sarja()].asuunnlaji[k_pv]);
		if (piste > 0 && piste < nsm)
			nsm = piste;
		if (piste > nsm)
			nsm = 0;
		for (int i = 0; i < nsm; i++)
			sak += as_sakko(i);
		return(sak);
		}
	if ((nsm = Sarjat[Sarja()].lsak[k_pv]) > 0) {
		if (Sarjat[Sarja()].va_sakot[k_pv][piste] < nsm)
			nsm = Sarjat[Sarja()].va_sakot[k_pv][piste];
		for (ns = 0; ns < nsm; ns++)
			if ((isak = pv[k_pv].asakot[ns] - L'0') >= 0 && isak <= kilpparam.maxasakko)
				sak += isak;
		sak *= SEK*Sarjat[Sarja()].tsak[k_pv];
		}
	sak += pv[k_pv].sakko + SEK*pv[k_pv].tasoitus;
	if (kilpparam.kilplaji == L'P' && pvparam[k_pv].alalaji == sotlajit[0].tunnus)
		sak = SEK*sotlajit[0].vertpiste - sak;
	if (kilpparam.kilplaji == L'P' && pvparam[k_pv].alalaji == sotlajit[3].tunnus)
		sak = SEK*sotlajit[3].vertpiste - sak;
	return(sak);
	}

INT32 kilptietue::sakko_pv(INT i_pv)
	{
	INT32 sak;
	INT ns, nsm, isak;

	if (Sarja(i_pv) < 0)
		return(false);
	sak = 0;
	if (kilpparam.kilplaji == L'B') {
		if ((nsm = Sarjat[Sarja(i_pv)].lsak[i_pv]) > 0) {
			 for (ns = 0; ns < nsm; ns++) {
				if ((isak = pv[i_pv].asakot[ns] - L'0') >= 0) {
					sak += SEK*Sarjat[Sarja(i_pv)].asuunnsakot[i_pv][ns]*isak;
					}
				}
			}
		}
	else {
		if ((nsm = Sarjat[Sarja(i_pv)].lsak[i_pv]) > 0) {
			 for (ns = 0; ns < nsm; ns++)
				if ((isak = pv[i_pv].asakot[ns] - L'0') >= 0 && isak <= kilpparam.maxasakko) {
					sak += isak;
					}
			sak *= SEK*Sarjat[Sarja(i_pv)].tsak[i_pv];
			}
		}
	sak += pv[i_pv].sakko + SEK*pv[i_pv].tasoitus;
	if (kilpparam.kilplaji == L'P' && pvparam[i_pv].alalaji == L'H')
		sak = 300*SEK - sak;
	if (kilpparam.kilplaji == L'P' && pvparam[i_pv].alalaji == L'T')
		sak = 3000*SEK - sak;
	return(sak);
	}

#if defined(_BORLANDC_)
#pragma warn -par
#endif
INT32 kilptietue::tulos1(INT piste, INT32 tm, int pyorista)
	{
	INT32 tls, sak;

#ifdef EBA
	INT i;

	tls = 0;
	piste = va_piste(piste);
	if (piste && piste <= Sarjat[Sarja()].valuku[k_pv])
		tls = pv[0].va[piste+1].vatulos;
	if (!piste) {
		for (i = 0; i < Sarjat[Sarja()].valuku[k_pv]; i++) {
			if (pv[0].va[i+2].vatulos) {
				tls += pv[0].va[i+2].vatulos;
				}
			else {
				tls = 0;
				break;
				}
			}
		}
#else // !EBA
	if (tm == TMAALI0)
		return(0);
	if (piste < -1 || piste > kilpparam.valuku+(kilpparam.n_pv_akt > 1)) {
		if (piste > kilpparam.valuku + (kilpparam.n_pv_akt > 1) + 1)
			parametrivirhe(L"tulos1", piste, 0, 0);
		return(0);
		}
	tm = pyoristatls(tm, pyorista);
	if (piste == -1) {
		if (TLahto(k_pv, false) == TMAALI0)
			return(TMAALI0);
		return(NORMKELLO(tm - TLahto(k_pv, false)));
		}
	if (TLahto(k_pv, true) == TMAALI0)
		return(0);
	sak = p_sakko(piste);
	tls = tm - TLahto(k_pv, true, pyorista) + sak;
	if (piste == kilpparam.valuku+1) {
		if (kilpparam.yhttlslaji == L'L' && Sarjat[Sarja()].luonne[k_pv] == 2) {
			tls = pyoristatls(tm - Sarjat[Sarja()].enslahto[k_pv], pyorista);
			}
		else if (kilpparam.yhttlslaji == L'P') {
			if (((tm = ytulos(k_pv-1, pyorista)) != 0) && tm < tls)
				tls = tm;
			}
		else {
			if (tls && (tm = ytulos(k_pv-1, pyorista)) != 0) {
				if (Sarjat[Sarja()].luonne[k_pv] == 2)
					tm = pyoristatls(tm, 4);
				tls += tm;
				}
			else
				tls = 0;
			}
		}
#endif  // !EBA
	if (tm != 12*TUNTI && kilpparam.alalaji != L'D')
		tls = NORMTULOS(tls);
	return(tls);
	}

INT32 kilptietue::k_tulos(INT piste, INT32 tm, int pyorista)
	{
	INT32 tls, etls;

	if (Sarja() < 0)
		return(0);
	piste = va_piste(piste);
	tls = tulos1(piste, tm, pyorista);
	if (Sarjat[Sarja()].luonne[k_pv] == 2 && piste && piste <= Sarjat[Sarja()].valuku[k_pv] && tls) {
		if (kilpparam.yhttlslaji == L'L') {
			tls = tls + TLahto(k_pv, true, 0) - Sarjat[Sarja()].enslahto[k_pv];
			}
		else {
			if ((etls = ytulos(k_pv-1)) != 0) {
				if (Sarjat[Sarja()].luonne[k_pv] == 2)
					etls = pyoristatls(etls, 4);
				tls += etls;
				}
			else
				tls = 0;
			}
		}
	return(pyoristatls(tls, pyorista));
	}
#if defined(_BORLANDC_)
#pragma warn +par
#endif

INT32 kilptietue::tall_tulos(INT piste, INT32 tls)
	{
	INT32 tls0;

	piste = va_piste(piste);
	if (piste < -1 || piste > kilpparam.valuku+(kilpparam.n_pv_akt > 1)) {
		if (piste > kilpparam.valuku + (kilpparam.n_pv_akt > 1) + 1)
			parametrivirhe(L"tall_tulos", piste, 0, 0);
		return(0);
		}
	tls0 = tls;
	if (piste >= 0 && tls0 < 0) {
		tls0 += 24*TUNTI;
		tls = tls0;
		}
	if (piste >= 0 && kilpparam.alalaji != L'D')
		tls = NORMTULOS(tls0);
	if (piste == -1 && tls != TMAALI0)
		tls = NORMKELLO(tls);
//	tls0 = pv[k_pv].va[piste+1].vatulos;   // Miksi tämä rivi oli mukana?
	if (piste >= -1 && Sarja() >= 0 && piste <= Sarjat[Sarja()].valuku[k_pv])
		pv[k_pv].va[piste+1].vatulos = tls;
	return(tls);
	}

INT32 kilptietue::set_tulos(INT piste, INT32 tm, int pyorista)
	{
	tm = pyoristatls(tm, pyorista);
	piste = va_piste(piste);
	return(tall_tulos(piste, piste >= 0 ? tulos1(piste, tm, false) : tm));
	}

INT32 kilptietue::tall_tulos_pv(INT32 tls, int i_pv, int piste /* = 0 */)
	{
	piste = va_piste(piste);
	if (piste == -1) {
		if (tls != TMAALI0)
			tls = NORMKELLO(tls);
		}
	else
		tls = NORMTULOS(tls);
	if (i_pv >= 0 && i_pv < kilpparam.n_pv_akt) {
		if (piste >= -1 && Sarja(i_pv) >= 0 && piste <= Sarjat[Sarja(i_pv)].valuku[i_pv])
			pv[i_pv].va[piste+1].vatulos = tls;
		}
	return(tls);
	}

wchar_t *kilptietue::asakkoStr(int i_pv /* = -1 */)
{
	static wchar_t st[20], *p;
	int i = 0;

	if (i_pv < 0)
		i_pv = k_pv;
	memset(st, 0, sizeof(st));
	if (pvparam[i_pv].asakkolaji > 1 || kilpparam.maxasakko > 9) {
		for (i = 0; i < Sarjat[Sarja(i_pv)].lsak[i_pv]; i++) {
			if (i > 0)
				wcscat(st, L"/");
			if (pv[i_pv].asakot[i] >= L'0') {
				if (kilpparam.kilplaji == L'P' && pvparam[i_pv].alalaji == L'T') {
					if (i == 0)
						_itow((pv[i_pv].asakot[i] - L'0')/10, st+wcslen(st), 10);
					else {
						wchar_t *p = st+wcslen(st);
						_itow((pv[i_pv].asakot[i] - L'0'), p, 10);
						insdes(p, 1, lsttulparam.tulmuot.des_sep);
						}
					}
				else
					_itow(pv[i_pv].asakot[i] - L'0', st+wcslen(st), 10);
				}
			else
				wcscat(st, L"-");
			}
		p = st + wcslen(st) - 2;
		while (p > st && wcscmp(p, L"/-") == 0) {
			*p = 0;
			p -= 2;
			}
		}
	else
		wcsncpy(st, pv[i_pv].asakot, Sarjat[Sarja(i_pv)].lsak[i_pv]);
	return(st);
}
//---------------------------------------------------------------------------

// set_Asakot tulkitsee käyttöliittymän merkkijonon ja kirjoittaa tulkitun jonon kilpailijatietoihin

void kilptietue::set_Asakot(wchar_t *asakstr, int i_pv /* =-1 */)
{
	wchar_t st[20] = L"", *p;

	if (i_pv < 0)
		i_pv = k_pv;
	if (pvparam[i_pv].asakkolaji > 1 || kilpparam.maxasakko > 9) {
		wcsncpy(st, asakstr, 19);
		p = wcstok(st, L"/;");
		}
	for (int i = 0; i < Sarjat[Sarja(i_pv)].lsak[i_pv]; i++) {
		if (pvparam[i_pv].asakkolaji > 1 || kilpparam.maxasakko > 9) {
			if (p != NULL && *p >= L'0') {
				if (kilpparam.kilplaji == L'P' && pvparam[i_pv].alalaji == L'T') {
					if (i == 0)
						pv[i_pv].asakot[i] = L'0' + 10*_wtoi(p);
					else {
						wchar_t *p1 = wcsstr(p, L",");
						if (p1)
							*p1 = L'.';
						pv[i_pv].asakot[i] = L'0' + (int)floor(10*_wtof(p)+0.5);
						}
					}
				else
					pv[i_pv].asakot[i] = L'0' + _wtoi(p);
				}
			else
				pv[i_pv].asakot[i] = L'-';
			if (p)
				p = wcstok(NULL, L"/;");
			}
		else {
			if (asakstr[i] >= L'0' && asakstr[i] <= L'0' + kilpparam.maxasakko)
				pv[i_pv].asakot[i] = asakstr[i];
			else
				pv[i_pv].asakot[i] = L'-';
			}
		}
	pv[i_pv].asakot[Sarjat[Sarja(i_pv)].lsak[i_pv]] = 0;
	if (pvparam[i_pv].asakkolaji == 3) {
		if (p != NULL && *p >= L'0')
			pv[i_pv].sakko = SEK * _wtoi(p) / 20;
		}
}

void kilptietue::tall_asakot(wchar_t *asakot, int i_pv /* =-1 */)
{
	int tm[LSAKKO+1];

	if (i_pv < 0)
		i_pv = k_pv;

	// sotilas-5-ottelun ammunnassa ja kranaatinheitossa ei oteta aikaa
	if (kilpparam.kilplaji == L'P' && (wcswcind(pvparam[k_pv].alalaji, L"TH") >= 0)) {
		set_Asakot(asakot, i_pv);
//		wmemcpy(pv[i_pv].asakot, asakot, Sarjat[Sarja(i_pv)].lsak[i_pv]);
		if (i_pv == k_pv)
			set_tulos(0, p_aika(-1));
		}
	else {
		// hae aiempien tietojen mukaiset sakon aika-arvot eri vaiheissa
		if (i_pv == k_pv) {
			for (int i = 1; i <= Sarjat[Sarja(k_pv)].lsak[k_pv]; i++) {
				tm[i] = p_sakko(i);
				}
			}
		tm[0] = sakko_pv(i_pv);
		// siirrä uudet sakot kilpailijatietoihin
		set_Asakot(asakot, i_pv);
//		wmemcpy(pv[i_pv].asakot, asakot, Sarjat[Sarja(i_pv)].lsak[i_pv]);
//		pv[i_pv].asakot[Sarjat[Sarja()].lsak[i_pv]] = 0;

		// määrää sakkomuutoksen vaikutukset tuloksiin
		if (tulos_pv(i_pv, false, 0) != 0)
			tall_tulos_pv(tulos_pv(i_pv, false, 0) + sakko_pv(i_pv) - tm[0], i_pv, 0);
		if (i_pv == k_pv) {
			for (int i = 1; i <= Sarjat[Sarja(k_pv)].lsak[k_pv]; i++) {
				if (p_aika(i) != 0)
					tall_tulos(i, p_aika(i) + p_sakko(i) - tm[i]);
				}
			}
		}
}

void kilptietue::tall_sakko(int sakko, int i_pv /* =-1 */)
{
	INT32 tm;

	if (i_pv < 0)
		i_pv = k_pv;
	tm = sakko_pv(i_pv);
	pv[i_pv].tasoitus = sakko;
	if (tulos_pv(i_pv, false, 0) != 0)
		tall_tulos_pv(tulos_pv(i_pv, false, 0) + sakko_pv(i_pv) - tm, i_pv, 0);
}

INT32 kilptietue::TLahto(int i_pv /* =-1 */)
{
	if (i_pv < 0)
		i_pv = k_pv;
	return(TLahto(i_pv, pvparam[i_pv].hiihtolahto > 0));
}

INT32 kilptietue::TLahto(int i_pv, bool tot, int pyorista /* = 1 */)
{
	int tl;

	if (i_pv < 0 || i_pv >= kilpparam.n_pv_akt)
		return(TMAALI0);
	if (tot && pvparam[i_pv].hiihtolahto > 0) {
		tl = pv[i_pv].va[0].vatulos;
		tl = pyoristatls(tl, pyorista);
		return(tl);
		}
	else
		return(pv[i_pv].tlahto);
}

INT32 kilptietue::tall_lajat(INT32 tls, bool molemmat)
{
	return(tall_lajat_pv(tls, k_pv, molemmat));
}

INT32 kilptietue::tall_lajat_pv(INT32 tls, int i_pv, bool molemmat)
	{
	if (tls != TMAALI0)
		tls = NORMKELLO(tls);
	if (i_pv >= 0 && i_pv < kilpparam.n_pv_akt) {
		pv[i_pv].tlahto = tls;
		if (!pvparam[i_pv].hiihtolahto || molemmat)
			pv[i_pv].va[0].vatulos = tls;
		}
	return(tls);
	}

INT32 kilptietue::tall_laika(INT32 tls)
	{
	if (tls != TMAALI0)
		tls = NORMKELLO(tls);
	if (!pvparam[k_pv].hiihtolahto)
		pv[k_pv].tlahto = tls;
	pv[k_pv].va[0].vatulos = tls;
	return(tls);
	}

void kilptietue::merk_sijat(void)
	{
	INT p, piste;
	UINT32 tl[2];

	if (Sarja() < 0)
		return;
	tl[1] = 0;
	for (piste = 0; piste <= Sarjat[Sarja()].valuku[k_pv]; piste++) {
		if (tulos2(piste+1)) {
			p = jalku[Sarja()][piste ? piste+4 : 0];
			tl[0] = tulos2(piste);
			pv[k_pv].va[piste+1].vasija =
				intsija(tl, &p, piste, 0, NULL);
			}
		}
	if (k_pv) {
		if (ytulos(k_pv)) {
			p = jalku[Sarja()][kilpparam.valuku+5];
			tl[0] = ytulos(k_pv);
			pv[k_pv].ysija = intsija(tl, &p, kilpparam.valuku+1, 0, NULL);
			}
		else
			pv[k_pv].ysija = 0;
		}
	}

void kilptietue::set_sija(INT piste, INT sj)
	{
   if (piste == kilpparam.valuku+1)
	  pv[k_pv].ysija = sj;
   else if (piste >= 0 && piste < kilpparam.valuku+1)
	  pv[k_pv].va[piste+1].vasija = sj;
   }

void kilptietue::set_tark(wchar_t kh, int i_pv /* = -1 */)
{
	if (i_pv == -1)
		i_pv = k_pv;
	if (i_pv >= 0 && i_pv < kilpparam.n_pv && wcswcind(kh, L"-TIHKOEVPXMB") >= 0)
		pv[i_pv].keskhyl = kh;
	else {
		parametrivirhe(L"set_tark", i_pv, NULL, kh);
		}
}

INT32 kilptietue::p_aika(INT piste, int pyorista /* = 0 */, int sakotpois /* = 0 */ )
	{
	piste = va_piste(piste);
	if (piste == kilpparam.valuku+1)
		return(ytulos(k_pv, pyorista));
	if (piste == -1)
		return(TLahto(k_pv, true, pyorista));
	if (piste >= 0 && piste <= kilpparam.valuku) {
		int tls = pv[k_pv].va[piste+1].vatulos;
		if (tls) {
			if (sakotpois == 1)
				tls -= p_sakko(piste);
//			if (sakotpois != 2 && pvparam[k_pv].takaaAjoLisa)
//				tls += pv[k_pv].pvpisteet[1];
			}
		tls = pyoristatls(tls, pyorista);
		return(tls);
		}
	if (piste > kilpparam.valuku + (kilpparam.n_pv_akt > 1) + 1)
		parametrivirhe(L"p_aika", piste, 0, 0);
	return(0);
	}

INT kilptietue::p_sija(INT piste, int sarja /* = -1 */)
   {
	piste = va_piste(piste);
   if (sarja >= sarjaluku && sarja < sarjaluku+nsarjayhd) {
	   int d = getpos(id());
	   return(d <= 0 ? 0 : yhdsija[sarja - sarjaluku][piste][d]);
	   }
   if (piste == kilpparam.valuku+1)
		return(y_hyv(k_pv) ? pv[k_pv].ysija : 0);
   if (!hyv() || p_aika(piste) == 0)
		return(0);
   if (piste >= 0 && piste <= kilpparam.valuku)
	  return(pv[k_pv].va[piste+1].vasija);
	parametrivirhe(L"p_sija", piste, 0, 0);
   return(0);
   }

INT kilptietue::sija(INT i_pv)
   {
   if (!hyv_pv(i_pv) || pv[i_pv].va[1].vatulos == 0)
		return(0);
   return(pv[i_pv].va[1].vasija);
   }

INT32 kilptietue::maali(INT piste)
   {
	piste = va_piste(piste);
   if (piste == kilpparam.valuku+1)
		piste = 0;
   if (piste >= -1 && piste <= kilpparam.valuku)
	  return(tmpmaali(piste, pv[k_pv].va[piste+1].vatulos));
   if (piste > kilpparam.valuku + 1)
	   parametrivirhe(L"maali", piste, 0, 0);
   return(0);
   }

#if defined(_BORLANDC_)
#pragma warn -par
#endif
INT32 kilptietue::tmpmaali(INT piste, INT32 tls)
   {
   INT32 tm, sak = 0;

	piste = va_piste(piste);
	if (piste > kilpparam.valuku+(kilpparam.n_pv_akt>1))
		return(0);
	if (!tls)
		return(TMAALI0);
	if (piste == -1)
		return(NORMKELLO(tls));
	if (Sarjat[Sarja()].rrtas != 6)
		sak = p_sakko(piste);
	tm = tls + TLahto(k_pv, true, false) - sak;
	tm = NORMKELLO(tm);
	return(tm);
	}
#if defined(_BORLANDC_)
#pragma warn +par
#endif

wchar_t kilptietue::tark(int ipv /* = -1 */)
	{
	if (ipv == -1)
		ipv = k_pv;
	if (Sarjat[Sarja(ipv)].luonne[ipv] < 0)
		return(L'P');
	return(pv[ipv].keskhyl);
	}

INT kilptietue::hyv(void)
   {
   return(hyv_pv(k_pv));
   }

INT kilptietue::hyv_pv(INT i_pv)
	{
	return(tark(i_pv) == L'-' || tark(i_pv) == L'I' || tark(i_pv) == L'T' || (kilpparam.hyvHavaittu && tark(i_pv) == L'B'));
   }

INT kilptietue::y_hyv(INT i_pv)
	{
	if (Sarja(i_pv) < 0 || Sarjat[Sarja(i_pv)].luonne[i_pv] < 0)
		return(0);
	if (Sarjat[Sarja(i_pv)].luonne[i_pv] != 2 && Sarjat[Sarja(i_pv)].pisteytulos()) {
		return(ypistetulos(i_pv) != 0);
		}
	if (Sarjat[Sarja(i_pv)].luonne[i_pv] == 2 && Sarjat[Sarja(i_pv)].pisteytulos()) {
		return(hyv_pv(i_pv));
		}
	for (int ip = Sarjat[Sarja(i_pv)].enspv; ip <= i_pv; ip++)
		if (Sarjat[Sarja(ip)].luonne[ip] >= 0 &&
			((kilpparam.yhttlslaji != L'V' && Sarja(ip) != Sarja(i_pv)) || !hyv_pv(ip)))
			return(0);
	return(1);
	}

bool kilptietue::lasna(void)
   {
   return(lasna(k_pv));
   }

bool kilptietue::lasna(int i_pv)
	{
	return(tark(i_pv) != L'P' && tark(i_pv) != L'E' && tark(i_pv) != L'V' && !(kilpparam.hyvHavaittu && tark(i_pv) == L'B'));
   }

INT32 kilptietue::tulos_pv(INT i_pv, bool vaadi_hyv /* = false */, int pyorista /* = 1 */)
	{
	int tls;
	if (!kilpparam.sarjavaihtelee && (Sarja() < 0 || Sarjat[Sarja()].luonne[i_pv] < 0))
		return(0);
	if (vaadi_hyv && !hyv_pv(i_pv))
		return(0);
	if (i_pv == k_pv)
		return(p_aika(0, pyorista));
	tls = pv[i_pv].va[1].vatulos;
	tls = pyoristatls(tls, pyorista, i_pv);
	return(tls);
   }

INT32 kilptietue::ytulos_p(INT piste, int pyorista)
	{
	INT32 tls, etls;

	piste = va_piste(piste);
	if (k_pv < Sarjat[Sarja()].enspv)
		return(0);
	if (k_pv == Sarjat[Sarja()].enspv)
		return(p_aika(piste, pyorista));
	if (piste == 0 || piste == kilpparam.valuku+1)
		tls = ytulos(k_pv, pyorista);
	else
		{
		if (Sarjat[Sarja()].luonne[k_pv] < 0)
			return(0);
		tls = p_aika(piste, pyorista);
		if (piste && Sarja() >= 0 && piste <= Sarjat[Sarja()].valuku[k_pv] && tls) {
			if (Sarjat[Sarja()].luonne[k_pv] == 2 && Sarjat[Sarja()].pisteytulos()) {
				tls = pisteet[0] != 0 ? tls + TakaaAjoLisa(k_pv) : 0;
				}
			else if ((etls = ytulos(k_pv-1, pyorista)) != 0 &&
				(!kilpparam.sarjavaihtelee || kilpparam.yhttlslaji == L'V' || Sarja() == Sarja(k_pv-1))) {
				if (Sarjat[Sarja()].luonne[k_pv] == 2)
					etls = pyoristatls(etls, 4);
				tls += etls;
				}
			else
				tls = 0;
			}
		else
			tls = 0;
		}
	return(tls);
	}

INT32 kilptietue::tulos2(INT piste, int pyorista)
	{
	INT32 tls;

	piste = va_piste(piste);
	if (!k_pv || piste <= 0)
		return(p_aika(piste, pyorista));
	if (piste == kilpparam.valuku+1)
		tls = ytulos(k_pv, pyorista);
	else
		{
//		if (Sarjat[Sarja()].luonne[k_pv] == 2 && !Sarjat[Sarja()].pisteytulos())
		if (Sarjat[Sarja()].luonne[k_pv] == 2)
			tls = ytulos_p(piste, pyorista);
		else
			tls = p_aika(piste, pyorista);
		}
	return(tls);
   }

INT32 HSprintYTulos(kilptietue *kilp)
{
	int tls;
	int vaiheluku;
	int epv;

	epv = Sarjat[kilp->sarja].enspv;
	if (k_pv != epv)
		return(0);
	if (k_pv == 0 && Sarjat[kilp->sarja].sprintSrj.vaihelkm > 1)
		vaiheluku = Sarjat[kilp->sarja].sprintSrj.vaihelkm;
	else if (Sarjat[kilp->sarja].viimpv == 0)
		vaiheluku = kilpparam.n_pv_akt - epv;
	else
		vaiheluku = Sarjat[kilp->sarja].viimpv+1-epv;
	if (vaiheluku == 0)
		return(0);
	if ((tls = kilp->tulos_pv(epv, true)) == 0) {
		if (kilp->lasna(epv) && !kilp->hyv_pv(epv))
			return(0x1f00000);
		else
			return(0x1ffffff);
		}
	tls = kilp->sija(epv) * KSEK;
	if (vaiheluku == 1)
		return(tls);
	if (vaiheluku == 2) {
		if (kilp->tulos_pv(epv+1, true) > 0)
			tls += 500 * kilp->sija(epv+1) * KSEK;
		else
			tls += 500 * 20 * KSEK;
		if (toupper(kilp->pv[epv].qual) < 'Q')
			tls += 500*20*160 * KSEK;
		else
			tls += 500*20*20*(toupper(kilp->pv[epv].qual) - 'Q')* KSEK;
		}
	if (vaiheluku == 3) {
		if (kilp->tulos_pv(epv+1, true) > 0)
			tls += 500 * kilp->sija(epv+1) * KSEK;
		else
			tls += 500 * 20 * KSEK;
		if (toupper(kilp->pv[epv+1].qual) < 'Q')
			tls += 500*20*100 * KSEK;
		else
			tls += 500*20*20*(toupper(kilp->pv[epv+1].qual) - 'Q')* KSEK;
		if (kilp->tulos_pv(epv+2, true) > 0) {
			tls += 500*20 * kilp->sija(epv+2) * KSEK;
			}
		else
			tls += 500*20*10 * KSEK;
		}
	if (vaiheluku > 3) {                      // Finaaleihin päässeet
		if (kilp->tulos_pv(epv+3, true) > 0) {             // Finaalissa tulksen saaneet
			tls = 500*40*20 * kilp->sija(epv+3) * KSEK;    // Kaikille finalisteille sijoitus
			if (vaiheluku == 4 && kilp->pv[epv+2].qual < 'A')
				tls += 500*40*20*10 * KSEK;            // B-finalisteille 10 sijaa lisää
			}
		else {
			if (kilp->tulos_pv(epv+2, true) > 0) {         // Semifinaalissa tuloksen saaneet
				tls += 500*40 * kilp->sija(epv+2) * KSEK;  // Semifinaalisija (+ karsinta)
				}
			else {
				tls += 500*40 * 10 * KSEK;           // Ei semifinaalitulosta
				if (kilp->pv[epv+1].qual < 'A')
					tls += 500*40*5 * KSEK;          // Karsiutui semeistä
				if (kilp->tulos_pv(epv+1, true) > 0)
					tls += 500 * kilp->sija(epv+1) * KSEK;  // Alkuerä semesitä karsiutuneille
				else
					tls += 500 * 20 * KSEK;           // Ei alkuerätulosta
				}
			tls += 500*40*20*19 * KSEK;              // Lisä kaikille, joilla ei finaalitulosta
			}
		}
	return(tls);
}

INT32 kilptietue::ypistetulos(int i_pv)
	{
	INT16 srj;
	int tls;

	if (kilpparam.sarjavaihtelee && kilpparam.yhttlslaji != L'V')
		srj = Sarja(i_pv);
	else
		srj = Sarja();
	if (i_pv == k_pv && Sarjat[srj].luonne[i_pv] == 2) {
		tls = ytulos(i_pv);
		}
	else if (Sarjat[srj].nouspisteet()) {
		tls = SEK * pisteet[0];
		}
	else {
		tls = PISTEKATTO - SEK*pisteet[0];
		}
	return(tls);
	}

int findYp(int (*ypJrj)[4], int kilpno)
{
	int d = getpos(kilpno);
	if (!ypJrj || d <= 0)
		return(-1);
	for (int i = 0; ypJrj[i][1] > 0; i++)
		if (ypJrj[i][1] == d)
			return(i);
	return(-1);
}

wchar_t *kilptietue::ytulos_wstr_cols_n(wchar_t *as, int i_pv, wchar_t sep, int pistedesim,
	int (*ypJrj)[4], int len, int pyorista)
{
	static wchar_t as0[20];

	if (as == NULL)
		as = as0;
	if (Sarjat[Sarja(i_pv)].pisteytulos() && Sarjat[Sarja(i_pv)].luonne[i_pv] != 2) {
		if (ypJrj) {
			int ii = findYp(ypJrj, id());
			if (ii >= 0)
				_itow(ypJrj[ii][0], as, 10);
			else
				as[0] = 0;
			}
		else
			_itow(pisteet[0], as, 10);
		if (pistedesim)
			insdes(as, pistedesim, sep);
		}
	else if (ytulos(i_pv, pyorista))
		aikatowstr_cols_n(as, ytulos(i_pv, pyorista), 0, sep, len);
	else
		wcscpy(as, L"-----");
	return(as);
}

INT32 kilptietue::ytulos(int i_pv, int pyorista)
	{
	INT32 ytls = 0, tls;
	INT16 srj;

	if (kilpparam.kilplaji == L'P' || kilpparam.kilplaji == L'N')
		return(meriytulos(this, i_pv));
	if (kilpparam.kilplaji == L'H' && kilpparam.alalaji == L'S')
		return(HSprintYTulos(this));
	if (kilpparam.sarjavaihtelee && kilpparam.yhttlslaji != L'V')
		srj = Sarja(i_pv);
	else
		srj = Sarja();
	if (srj < 0 || i_pv < Sarjat[srj].enspv)
		return(0);
	if (i_pv > Sarjat[srj].enspv && Sarjat[srj].luonne[i_pv] < 0)
		return(ytulos(i_pv-1, pyorista));
	if (Sarjat[srj].pisteytulos() && Sarjat[srj].luonne[i_pv] != 2) {
		return(ypistetulos(i_pv));
		}
	if (!y_hyv(i_pv))
		return(0);
	if (i_pv > Sarjat[srj].viimpv)
		return(ytulos(Sarjat[srj].viimpv, pyorista));
	if (i_pv == Sarjat[srj].enspv)
		return(pyoristatls(tulos_pv(i_pv, true, 2)*Sarjat[srj].tlskerroin[i_pv]/Sarjat[srj].tlsjakaja[i_pv], pyorista));
	if (kilpparam.yhttlslaji == L'L' && Sarjat[srj].luonne[i_pv] == 2) {
		ytls =  p_aika(0, pyorista) + TLahto(i_pv, true, pyorista) - Sarjat[Sarja()].enslahto[i_pv];
		}
	else if (Sarjat[srj].pisteytulos() && Sarjat[srj].luonne[i_pv] == 2) {
		ytls = pisteet[0] != 0 ? p_aika(0, pyorista) + TakaaAjoLisa(i_pv) : 0;
		}
	else if (kilpparam.yhttlslaji == L'P') {
		ytls = p_aika(0, pyorista);
		if (((tls = ytulos(i_pv-1, pyorista)) != 0) && tls < ytls)
			ytls = tls;
		}
	else {
		if (pv[i_pv].va[1].vatulos && (ytls = ytulos(i_pv-1)) != 0) {
			if (kilpparam.sarjavaihtelee && kilpparam.yhttlslaji != L'V' && srj != Sarja(i_pv-1))
				return(0);
			if (i_pv == k_pv && Sarjat[srj].luonne[k_pv] == 2)
				ytls = pyoristatls(ytls, 4);
			ytls += (pv[i_pv].va[1].vatulos*Sarjat[srj].tlskerroin[i_pv])/Sarjat[srj].tlsjakaja[i_pv];
			}
		}
	ytls = pyoristatls(ytls, pyorista, i_pv);
	return(ytls);
	}

INT kilptietue::y_sija(int i_pv)
	{
	if (i_pv == 0)
		return(pv[i_pv].va[1].vasija);
	if (i_pv > 0)
		return(pv[i_pv].ysija);
	return(0);
	}

wchar_t kilptietue::ytark(int i_pv)
	{
	wchar_t kh;

	if (i_pv < Sarjat[sarja].enspv)
		return(L'P');
	if (i_pv > Sarjat[sarja].enspv && Sarjat[sarja].luonne[i_pv] < 0)
		return(ytark(i_pv-1));
	kh = tark(Sarjat[sarja].enspv);
	for (int i = Sarjat[sarja].enspv+1; i <= i_pv ; i++) {
		if (Sarjat[sarja].luonne[i] < 0)
			continue;
		if (wcswcind(kh, L"HKOXM") >= 0)
			break;
		if (kh == L'I')
			kh = L'-';
		if (tark(i) == L'T')
			continue;
		if (kh == L'T') {
			kh = tark(i);
			continue;
			}
		if (kh == L'-') {
			if (tark(i) == L'K' || tark(i) == L'O' || tark(i) == L'X' || !lasna(i))
				kh = L'K';
			if (tark(i) == L'H' || tark(i) == L'M')
				kh = L'H';
			}
		if ((kh == L'E' || kh == L'B' || kh == L'P') && lasna(i))
			kh = L'K';
		}
	if (kh == L'I' || (kh == L'T' && ytulos(i_pv) == 0))
		kh = L'-';
	return(kh);
	}

INT32 meripiste(kilptietue *kilp, int i_pv)
   {
   INT32 tt;

	tt = kilp->tulos_pv(i_pv);
	if (tt) {
//		if (meri3 || Sarjat[kilp->sarja].viimpv - Sarjat[kilp->sarja].enspv == 2)
//			kilp->pv[i_pv].pvpisteet[0] = 1500 - (500 * tt + m3vert[i_pv][kilp->sarja]/2) / m3vert[i_pv][kilp->sarja];

		tt = kilp->pv[i_pv].pvpisteet[0];
//			tt = (Sarjat[kilp->sarja].tlskerroin[i_pv] - tt/KSEK)/Sarjat[kilp->sarja].tlsjakaja[i_pv];
		}
	if (tt < 0)
		tt = 0;
	return(tt);
	}

INT32 meriytulos(kilptietue *kilp, int i_pv)
   {
   INT32 tt, ytls;

   kilp->tuloskaavat(0);
   if (i_pv < Sarjat[kilp->sarja].enspv)
		return(0);
   if (Sarjat[kilp->sarja].luonne[i_pv] < 0)
		return(meriytulos(kilp, i_pv-1));
   if (!kilp->hyv_pv(i_pv))
		return(0);
//   if (i_pv > Sarjat[kilp->sarja].viimpv)
//		return(meriytulos(kilp, Sarjat[kilp->sarja].viimpv));
	tt = meripiste(kilp, i_pv);
	if (i_pv == Sarjat[kilp->sarja].enspv) {
		ytls = PISTEKATTO;
		if (tt > 0)
			ytls = ytls - tt*SEK;
		}
	else {
		if ((ytls = meriytulos(kilp, i_pv-1)) != 0 && tt) {
			if (tt > 0)
				ytls = ytls - tt*SEK;
			}
		}
   return(ytls);
   }

#ifdef _CONSOLE
void takaa_ajolahdot(void)
	{
	int i, n = 0, n1, srj = 0, lvali = 60, nr = 2, kr = 0;
	wchar_t ch;
	INT32 p, enslahto = 0, vlahto = 0, karki;
	kilptietue kilp;

	for (;;) {
		vidspwmsg(ySize-3, 0, 7, 0, L"Anna sarja                      Esc: lopeta");
		srj = luesarja(Sarjat[srj].sarjanimi, &ch);
		if (srj < 0 || ch == ESC)
			break;
		clrln(ySize-3);
		vidspwmsg(ySize-3, 0, 7, 0, L"Takaa-ajovaiheen osanottajamäärä");
		INPUTINTW(&n, 3, 34, ySize-3, L"\r\x1b", &ch);
		if (n > ntulos[srj][VALUKU+1])
			n = ntulos[srj][VALUKU+1];
		p = jalku[srj][VALUKU+5];
		if (n > 0) {
			clrln(ySize-3);
			vidspwmsg(ySize-3, 0, 7, 0, L"Ensimmäinen lähtöaika");
			INPUTAIKAW(&enslahto, t0, 8, 34, ySize-3, L"\r\x1b", &ch);
			vlahto = enslahto;
			for (i = 0; i < n && p >= 0; i++) {
				kilp.GETREC(p);
				if (i == 0) {
					karki = kilp.ytulos(k_pv);
					vlahto = enslahto;
					}
				else {
					vlahto = enslahto - ((kilp.ytulos(k_pv) - karki) * Sarjat[srj].kaava[k_pv+1][VALUKU+1].jakaja)/Sarjat[srj].kaava[k_pv+1][VALUKU+1].kerroin;
					}
				kilp.tall_lajat_pv(vlahto, k_pv+1);
				kilp.tallenna(p, 0, 0, 0, 0);
				p = seuraava(0, VALUKU+1, p);
				}
			}
		if (n < ntulos[srj][VALUKU+1]) {
			clrln(ySize-3);
			vidspwmsg(ySize-3, 0, 7, 0, L"Muiden osanottajien määrä");
			n1 = ntulos[srj][VALUKU+1] - n;
			INPUTINTW(&n1, 3, 34, ySize-3, L"\r\x1b", &ch);
			if (n1 > 0 && ch != ESC) {
				if (n1 > ntulos[srj][VALUKU+1]-n)
					n1 = ntulos[srj][VALUKU+1]-n;
				clrln(ySize-3);
				enslahto = vlahto + MINUUTTI;
				enslahto = ((enslahto + 24*TUNTI)/SEK)*SEK-24*TUNTI;
				vidspwmsg(ySize-3, 0, 7, 0, L"Ensimmäinen jälkiosan lähtöaika");
				INPUTAIKAW(&enslahto, t0, 8, 44, ySize-3, L"\r\x1b", &ch);
				vlahto = enslahto;
				clrln(ySize-3);
				vidspwmsg(ySize-3, 0, 7, 0, L"Jälkiosan lähtövälit sekunteina");
				INPUTINTW(&lvali, 4, 44, ySize-3, L"\r\x1b", &ch);
				clrln(ySize-3);
				vidspwmsg(ySize-3, 0, 7, 0, L"Jälkiosan ryhmäkoko");
				for (i = 0; i < n1 && p >= 0; i++) {
					kilp.GETREC(p);
					vlahto = enslahto + (i/nr) * lvali * SEK;
					kilp.tall_lajat_pv(vlahto, k_pv+1);
					kilp.tallenna(p, 0, 0, 0, 0);
					p = seuraava(0, VALUKU+1, p);
					}
				}
			}
		}
	}
#endif

INT32 kilptietue::as_sakko(int piste)
	{
   INT32 sak = 0;

   piste = va_piste(piste);
   if (Sarjat[Sarja()].asuunnlaji[k_pv][piste] == L'S') {
	   wchar_t *p = wcsstr(Sarjat[Sarja()].asuunnlaji[k_pv], L"S");
	   int k = piste > p - Sarjat[Sarja()].asuunnlaji[k_pv] ? 1 : 0;
	   sak = SEK * Sarjat[Sarja()].asuunnsakot[k_pv][k] *
		  (pv[k_pv].asakot[k] <= L'0' ? 0 : (int)(pv[k_pv].asakot[k] - L'0'));
	   if (k == 0)
		   sak += MINUUTTI * (pv[k_pv].asakot[2] <= L'0' ? 0 : (int)(pv[k_pv].asakot[2] - L'0'));
	   }
   if (Sarjat[Sarja()].asuunnlaji[k_pv][piste] == L'L') {
	   sak = SEK * Sarjat[Sarja()].asuunnsakot[k_pv][3] * (
		  (pv[k_pv].asakot[3] <= L'0' ? 0 : (int)(pv[k_pv].asakot[3] - L'0'))) +
		  MINUUTTI * (pv[k_pv].asakot[4] <= L'0' ? 0 : (int)(pv[k_pv].asakot[4] - L'0'));
	   }
   return(sak);
   }

int Pisteet(pistekaavatp *kva, int sj, int n, int tls, int karki)
{
//	int a[5], y[5], p[5], d[5];
	int taite = -99999999, pst = 0;
	double pst1 = 0;

	if (kva->vaika)
		karki = kva->vaika;

	if (kva->laji & 1) {
		pst = (kva->d[0]+kva->d[1]) > 0 ? 99999999 : -99999999;

		for (int i = 0; i < 5; i++) {
			if (kva->a[i] > 0 && sj >= kva->a[i] && sj <= kva->y[i]) {
				pst = kva->p[i] + (sj - kva->a[i]) * kva->d[i];
				break;
				}
			if (kva->a[i] >= 9999) {
				if (sj >= kva->y[i]) {
					pst = kva->p[i] - (n - sj) * kva->d[i];
					break;
					}
				taite = kva->p[i] - (n - kva->y[i]) * kva->d[i];
				}
			if (taite != -99999999 && kva->a[i] == kva->y[i-1]) {
				if (sj >= kva->y[i]) {
					pst = taite - (kva->a[i] - sj) * kva->d[i];
					break;
					}
				taite = taite - (kva->a[i] - kva->y[i]) * kva->d[i];
				}
			}
		}

	if (kva->plaji == 1) {
		if (tls > 0)
			pst1 = (karki * kva->vpist) / tls;
		else
			pst1 = 0;
		if ((kva->optiot & 3) == 0)
			pst += int(pst1+0.000001);
		else
			pst += int(floor(pst1+0.500001));
		}
	if (kva->plaji == 2) {
		if (karki <= 0 || kva->jak == 0 || tls == 0)
			return(0);
		pst1 = tls - karki;
		pst1 = pst1 / karki * kva->kerr / kva->jak;
		pst1 = kva->vpist * (1.0 - pst1);
		if ((kva->optiot & 4) && pst1 < 0)
			pst1 = 0;
		if ((kva->optiot & 3) == 0)
			pst += int(pst1+0.000001);
		else
			pst += int(floor(pst1+0.500001));
		}
	if (kva->plaji == 3) {
		if (karki <= 0 || kva->jak == 0 || tls == 0)
			return(0);
		pst1 = tls - karki;
		pst1 = pst1 / karki * kva->kerr / kva->jak;
		pst1 = kva->vpist - pst1;
		if ((kva->optiot & 4) && pst1 < 0)
			pst1 = 0;
		if ((kva->optiot & 3) == 0)
			pst += int(pst1+0.000001);
		else
			pst += int(floor(pst1+0.500001));
		}
	if (kva->plaji == 4) {
		if (kva->jak == 0 || tls == 0)
			return(0);
		pst1 = tls - karki;
		pst1 = pst1 * kva->kerr / kva->jak;
		pst1 = kva->vpist - pst1;
		if ((kva->optiot & 4) && pst1 < 0)
			pst1 = 0;
		if ((kva->optiot & 3) == 0)
			pst += int(pst1+0.000001);
		else
			pst += int(floor(pst1+0.500001));
		}
	if (kva->plaji == 5) {
		if (karki <= 0 || kva->jak == 0 || tls <= 0)
			return(0);
		pst1 = karki;
		pst1 = tls/pst1;
		pst1 = log10(pst1) * kva->kerr / kva->jak;
		pst1 = kva->vpist - pst1;
		if ((kva->optiot & 4) && pst1 < 0)
			pst1 = 0;
		if ((kva->optiot & 3) == 0)
			pst += int(pst1+0.000001);
		else
			pst += int(floor(pst1+0.500001));
		}
	return(pst);
}
//---------------------------------------------------------------------------
int PisteAika(pistekaavatp *kva, int pst)
{
	int tls = 0;
	double karki;

	if (kva->laji != 2 || kva->vaika <= 0 || kva->vpist == 0)
		return(0);

	karki = kva->vaika;

	if (kva->plaji == 1) {
		if (pst > 0) {
			tls = (int) ((karki * kva->vpist) / pst);
			}
		}
	if (kva->plaji == 2) {
		if (kva->kerr == 0)
			return(0);
		tls = (int) (kva->vaika * ((1 + ((k_pv+1)*kva->vpist - pst) * kva->jak) / (kva->kerr * kva->vpist)));
		}
	if (kva->plaji == 3) {
		if (kva->kerr == 0)
			return(0);
		tls = (int) (kva->vaika * ((1 + ((k_pv+1)*kva->vpist - pst) * kva->jak) / kva->kerr));
		}
	if (kva->plaji == 4) {
		if (kva->kerr == 0)
			return(0);
		tls = (int) (kva->vaika + (((k_pv+1)*kva->vpist - pst) * kva->jak) / kva->kerr);
		}
	if (kva->plaji == 5) {
		if (kva->kerr == 0)
			return(0);
		tls = (int) (kva->vaika * exp(log((double)10.0) * (((k_pv+1)*kva->vpist - pst) * kva->jak) / kva->kerr));
		}
	return(tls);
}
//---------------------------------------------------------------------------
void LaskeKilpYhtPisteet(kilptietue *kilp, pistekaavatp *kva, int srj)
{
	int pst[N_PV], ip, srj1 = -1, maxlkm, npv, epv, minosotto, minhyv, nosotto, nhyv;

	maxlkm = kva->yhtpistvaihelkm;
	minosotto = kva->yhtpistosottomin;
	minhyv = kva->yhtpisthyvmin;
	npv = Sarjat[srj].viimpv;
	if (npv > k_pv)
		npv = k_pv;
	if ((kva->yhtpiston & 2) == 0 && ((Sarjat[srj].luonne[npv] == 2 && kilp->tulos_pv(npv, true) == 0) ||
		((!kilp->lasna(npv) || (kilp->tulos_pv(npv) == 0 && kilp->hyv_pv(npv)))))) {
		kilp->pisteet[0] = 0;
		return;
		}
	epv = Sarjat[srj].enspv;
	for (int ipv = epv; ipv <= npv; ipv++) {
		if (Sarjat[srj].luonne[ipv] == 0)
			epv = ipv;
		}
	npv = npv - epv + 1;
	memset(pst, 0, sizeof(pst));
	srj1 = -1;
	nosotto = 0;
	nhyv = 0;
	for (int ipv = 0; ipv < npv; ipv++) {
		if (Sarjat[srj].luonne[ipv] < 0)
			continue;
		if (kilp->lasna(epv + ipv))
			nosotto++;
		if (!kilp->hyv_pv(epv + ipv)
			|| !kilp->tulos_pv(epv + ipv, true)) {
			continue;
			}
		nhyv++;
		ip = kilp->pv[epv + ipv].pvpisteet[0];
		if (ip == 0)
			continue;
		if (kilpparam.sarjavaihtelee && kilpparam.yhttlslaji != L'V') {
			if (srj1 < 0)
				srj1 = kilp->Sarja(epv+ipv);
			else if (srj1 != kilp->Sarja(epv+ipv)) {
				memset(pst, 0, sizeof(pst));
				break;
				}
			}
		if (ipv == 0)
			pst[0] = ip;
		else {
			for (int i = ipv; i >= 0; i--) {
				if (i == 0 || (ip < pst[i-1] && !(epv + ipv == k_pv && (kva->yhtpiston & 2) == 0))) {
					pst[i] = ip;
					break;
					}
				else {
					pst[i] = pst[i-1];
					}
				}
			}
		}
	kilp->pisteet[0] = 0;
	if (nosotto >= minosotto && nhyv >= minhyv) {
		for (int ipv = 0; ipv < npv && ipv < maxlkm; ipv++) {
			kilp->pisteet[0] += pst[ipv];
			}
		}
}
//---------------------------------------------------------------------------

void kilptietue::laskeKierrokset(void)
{
	int viimva = 0, edtls = 0, tls = 0, vrk = 0;

	for (int i = 1; i <= Sarjat[Sarja()].valuku[k_pv]; i++) {
		if ((tls = p_aika(i)) > 0) {
			viimva = i;
			if (tls < edtls - 12*TUNTI)
				vrk++;
			edtls = tls;
			}
		}
	pv[k_pv].pvpisteet[0] = viimva;
	tall_tulos(0, edtls + vrk * 24 * TUNTI);
}
//---------------------------------------------------------------------------

int PisteKarki(int srj, int i_pv)
{
	int pst = 0, kk;

	kk = Sarjat[srj].pistekaava[i_pv].kerr >= 0 ? 1 : -1;
	if (kk < 0)
		pst = 0x3fffffff;
	for (int d = 1; d < nrec; d++) {
		kilptietue kilp;
		kilp.GETREC(d);
		if (!kilp.kilpstatus && kilp.Sarja(i_pv) == srj &&
			kk * (kilp.pv[i_pv-1].pvpisteet[0] - pst) > 0)
			pst = kilp.pv[i_pv-1].pvpisteet[0];
		}
	Sarjat[srj].pistekarki[i_pv] = pst;
	return(pst);
}
//---------------------------------------------------------------------------

void PisteKarjet(void)
{
	for (int srj = 0; srj < sarjaluku; srj++) {
		if (Sarjat[srj].luonne[k_pv] == 2 && Sarjat[srj].pisteytulos())
			PisteKarki(srj, k_pv - 1);
		}
}
//---------------------------------------------------------------------------

int kilptietue::TakaaAjoLisa(int i_pv /* = -1 */)
{
	int tls = TMAALI0;

	if (i_pv < 0)
		i_pv = k_pv;

	int srj = Sarja(i_pv);
	if (Sarjat[srj].luonne[i_pv] != 2)
		return(TMAALI0);
	if (Sarjat[srj].pisteytulos()) {
		tls = pv[i_pv].pvpisteet[1];
/*
		pistekaavatp *kva = &Sarjat[srj].pistekaava[i_pv];

		if (i_pv < 1 || kva->laji != 2 || kva->plaji < 2 || kva->plaji > 2 || kva->vaika <= 0)
			return(TMAALI0);

		if (kva->plaji == 2) {
			if (kva->kerr == 0 || kva->vpist == 0)
				return(TMAALI0);
			tls = (int) (kva->vaika * ((1 + (Sarjat[srj].pistekarki[i_pv-1] - pv[i_pv-1].pvpisteet[1]) * kva->jak) / (kva->kerr * kva->vpist)));
			}
		if (kva->plaji == 3) {
			if (kva->kerr == 0)
				return(TMAALI0);
			tls = (int) (kva->vaika * ((1 + (Sarjat[srj].pistekarki[i_pv-1] - pv[i_pv-1].pvpisteet[1]) * kva->jak) / kva->kerr));
			}
		if (kva->plaji == 4) {
			if (kva->kerr == 0)
				return(TMAALI0);
			tls = (int) (((Sarjat[srj].pistekarki[i_pv-1] - pv[i_pv-1].pvpisteet[1]) * kva->jak) / kva->kerr);
			}
*/
		}
	else
		tls = ytulos(i_pv-1);
	return(tls);
}

