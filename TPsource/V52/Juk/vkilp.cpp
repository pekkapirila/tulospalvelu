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

#ifdef ILMOITT
#include "ilmoitt.h"
#else
#include "VDeclare.h"
#endif

void vatp::nollaa(void)
{
	sija = 0;
	aika = TMAALI0;
}

void osrec::nollaa(void)
{
	memset(this, 0, kilpparam.osrecsize0);
	keskhyl = '-';
	ylahto = TMAALI0;
	for (int i = 0; i < kilpparam.valuku+1; i++)
		va[i].nollaa();
}

void kilptietue::nollaa(void)
{
	memset(this, 0, kilpparam.kilprecsize0);
	for (int i = 0; i < kilpparam.n_os_akt; i++)
		ostiet[i].nollaa();
}

void vatp::operator=(vatp& va)
{
	aika = va.aika;
	sija = va.sija;
}

void osrec::operator=(osrec& ostiet)
{
	memcpy(this, &ostiet, kilpparam.osrecsize0);
	for (int i = 0; i < kilpparam.valuku+1; i++) {
		 va[i] = ostiet.va[i];
	}
}

void kilptietue::operator=(kilptietue& kilp)
{
	memcpy(this, &kilp, kilpparam.kilprecsize0);
	for (int i = 0; i < kilpparam.osuusluku; i++) {
		 ostiet[i] = kilp.ostiet[i];
	}
}

bool vatp::operator==(vatp& va)
{
	if (aika != va.aika)
		return(false);
	return(true);
}

int osrec::vert0(osrec &ostiet)
{
	int ret = 0;
	int ero = 0;

	if (strcmp(nimi, ostiet.nimi)) {
		ret |= 1;
		ero = 1;
		}
	if (strcmp(osseura, ostiet.osseura)) {
		ret |= 1;
		ero = 1;
		}
	if (strcmp(arvo, ostiet.arvo)) {
		ret |= 1;
		ero = 1;
		}
	if (strcmp(hajonta, ostiet.hajonta)) {
		ret |= 1;
		ero = 1;
		}
	if (lahtolaji != ostiet.lahtolaji) {
		ret |= 1;
		ero = 1;
		}
	if (lisno != ostiet.lisno) {
		ret |= 1;
		ero = 1;
		}
	if (badge[0] != ostiet.badge[0]) {
		ret |= 1;
		ero = 1;
		}
	if (badge[1] != ostiet.badge[1]) {
		ret |= 1;
		ero = 1;
		}
//	if (kone != ostiet.kone) {
//		ret |= 1;
//		ero = 1;
//		}
	if (uusi != ostiet.uusi) {
		ret |= 1;
		ero = 1;
		}
	if (laina[0] != ostiet.laina[0] && (laina[0] > ' ' || ostiet.laina[0] > ' ')) {
		ret |= 1;
		ero = 1;
		}
	if (laina[1] != ostiet.laina[1] && (laina[1] > ' ' || ostiet.laina[1] > ' ')) {
		ret |= 1;
		ero = 1;
		}
	if (memcmp(selitys, ostiet.selitys, sizeof(selitys))) {
		ret |= 1;
		ero = 1;
		}
	if (seuranta != ostiet.seuranta && (seuranta > ' ' || ostiet.seuranta > ' ')) {
		ret |= 1;
		ero = 1;
		}
	if (keskhyl != ostiet.keskhyl) {
		ret |= 2;
		ero = 1;
		}
	if (sakko != ostiet.sakko) {
		ret |= 2;
		ero = 1;
		}
	if (memcmp(ampsakot, ostiet.ampsakot, sizeof(ampsakot))) {
		ret |= 5;
		ero = 1;
		}
	if (ylahto != ostiet.ylahto) {
		ret |= 2;
		ero = 1;
		}
	if (ero)
		return(ret);
	return(0);
}

bool osrec::operator==(osrec& ostiet)
{
	if (vert0(ostiet) != 0)
		return(false);
	for (int i = 0; i < kilpparam.valuku+1; i++) {
		if (!(va[i] == ostiet.va[i]))
			return(false);
		}
	return(true);
}

int kilptietue::vert0(kilptietue &kilp)
{
	int ret = 0;

	ret = memcmp(this, &kilp, kilpparam.kilprecsize0);
	return(ret);
}

bool kilptietue::operator==(kilptietue& kilp)
{
	if (vert0(kilp) != 0)
		return(false);
	for (int i = 0; i < kilpparam.osuusluku; i++) {
		if (!(ostiet[i] == kilp.ostiet[i]))
		  return(false);
	}
	return(true);
}

void kilptietue::getrec(INT d)
{
	GETREC(this, d);
}

void kilptietue::putrec(INT d, bool tiedostoon /* =false */)
{
	PUTREC(this, d);
	if (tiedostoon) {
		if (datatallfl[0])
			putrec_i(&datf2, d, this);
#ifndef _CONSOLE
		if (datatallfl[1])
			kirjKilpSQL(d);
#endif
		}
}

int kilptietue::yOsuus(int osuus)
{
	return(Sarjat[sarja].yosuus[osuus]);
}

int kilptietue::aOsuus(int osuus)
{
	if (osuus == Sarjat[sarja].ntosuus)
		return(Sarjat[sarja].osuusluku);
	return(osuus - Sarjat[sarja].aosuus[Sarjat[sarja].yosuus[osuus]] - 1);
}

int kilptietue::Osuus(int yos, int aos)
{
	return(Sarjat[sarja].aosuus[yos] + aos + 1);
}

int kilptietue::KilpNo(bool siirra /* = false */)
{
	return(kilpno + (siirra ? Sarjat[sarja].knosiirto : 0));
}

void kilptietue::setKilpNo(int kno, bool siirra /* = false */)
{
	kilpno = kno - (siirra ? Sarjat[sarja].knosiirto : 0);
}

INT32 osrec::Maali(int piste)
{
	if (piste >= 0 && piste <= kilpparam.valuku)
		return(va[piste].aika);
	if (piste == -1)
		return(ylahto);
	return(TMAALI0);
}

INT16 osrec::Sija(int piste)
{
	if (piste > kilpparam.valuku)
		return(ossija);
	else if (piste >= 0)
		return(va[piste].sija);
	return(0);
}

INT16 osrec::osSija(void)
{
	return(ossija);
}

void osrec::setMaali(int piste, int aika)
{
	if (piste >= 0 && piste <= kilpparam.valuku)
		va[piste].aika = aika;
	if (piste == -1) {
		ylahto = aika;
		lahtolaji = aika == TMAALI0 ? 0 : 1;
		}
}

void osrec::setSija(int piste, int sija)
{
	if (piste > kilpparam.valuku)
		ossija = sija;
	else if (piste >= 0)
		va[piste].sija = (INT16)sija;
}

int pyoristatls(int tls, int pyorista)
{
	if (!tls || tls == TMAALI0 || pyorista >= (int) sizeof(kilpparam.pyor)/(kilpparam.pyor[0]))
		return(tls);
	if (tls && kilpparam.pyorlahimpaan)
		tls += kilpparam.pyor[pyorista]/2;
	return(kilpparam.pyor[pyorista] * ((tls + 200*TUNTI)/kilpparam.pyor[pyorista]) - 200*TUNTI);
}

INT32 kilptietue::Lahto(int osuus, int *laji /* = NULL */)
{
	__int64 puutelisa;

	if (osuus < 0 || osuus >= Sarjat[sarja].osuusluku)
		return(TMAALI0);
	if (Sarjat[sarja].maxnosuus == 1) {
		if (ostiet[osuus].ylahto != TMAALI0) {
			if (laji)
				*laji = ostiet[osuus].lahtolaji;
			return(ostiet[osuus].ylahto);
			}
		if (laji)
			*laji = 0;
		if (!kilpparam.partio && osuus > 0) {
		   return(Maali(osuus-1, 0));
		   }
		return(Sarjat[sarja].lahto);
		}
	else {
	   TULOS etls;
	   TMAALI oslahto = TMAALI0, tm, edtm[4] = {-TMAALI0, -TMAALI0, -TMAALI0, TMAALI0};
	   int os, os1, yosuus;

	   yosuus = yOsuus(osuus);
	   if (Sarjat[sarja].aosuus[yosuus] < 0)
		  return(Sarjat[sarja].lahto);
	   if (ostiet[osuus].ylahto != TMAALI0)
		  return(NORMKELLO(ostiet[osuus].ylahto));
#ifdef SKMKV
	  if ((etls = tTulos(yosuus-1, 0, 0)) != 0 &&
		 etls < 50L * TUNTI)
		 oslahto = Sarjat[sarja].lahto + etls - SakkoAika(yOsuus, true);
#else // !SKMKV
	   if (Sarjat[sarja].nosuus[yosuus] == 1 ||
		  Sarjat[sarja].nosuus[yosuus-1] == 1) {
		  if ((etls = tTulos(yosuus-1, 0, &puutelisa)) != 0 &&
			   puutelisa == 0)
			   oslahto = Sarjat[sarja].lahto + etls;
		  oslahto -= SakkoAika(yosuus, true);
		  }
	   else {
		  os1 = osuus - Sarjat[sarja].aosuus[yosuus] - 1;
		  for (os = Sarjat[sarja].aosuus[yosuus-1] + 1; os <= Sarjat[sarja].aosuus[yosuus]; os++) {
			 tm = Maali(os, 0);
			 if (tm != TMAALI0) {
				if (tm < edtm[0]) {
				   edtm[3] = edtm[2];
				   edtm[2] = edtm[1];
				   edtm[1] = edtm[0];
				   edtm[0] = tm;
				   }
				else if (tm < edtm[1]) {
				   edtm[3] = edtm[2];
				   edtm[2] = edtm[1];
				   edtm[1] = tm;
				   }
				else if (tm < edtm[2]) {
				   edtm[3] = edtm[2];
				   edtm[2] = tm;
				   }
				else edtm[3] = tm;
				}
			 }
		  if (edtm[os1] != -TMAALI0)
			 oslahto = edtm[os1];
		  }
#endif  // !SKMKV
		oslahto = (oslahto + 36*TUNTI) % (24*TUNTI) - 12*TUNTI;
		return(oslahto);
		}
}

INT32 kilptietue::tTulos(int tosuus, int va, __int64 *tlslisa /* = NULL */)
{
   long tls, etulos = 0;
   int os, yl1 = 0, yl = 0, n_osaika[MAXOSUUSLUKU];
   __int64 lisa;

	if (tosuus < 0 || tosuus >= Sarjat[sarja].ntosuus || va < 0 || va > Sarjat[sarja].valuku[tosuus])
		return(0);

	if (Sarjat[sarja].maxnosuus == 1)
		return(Tulos(tosuus, va));

   tls = 0;
   if (tlslisa)
	   *tlslisa = 0;
   memset(n_osaika, 0, sizeof(n_osaika));
   if (tosuus < 0)
	   return(0);

   if (!tHyv(tosuus) && !kilpparam.sailhyl)
	   return(0);

   // Käydään läpi tosuus ja aiemmat osuudet ja
   // - kasvatetaan muuttujaa tlslisa, jos aika puuttuu
   // - kasvatetaan laskuria n_os, jos aika on otettu ja tulos


   for (int yos = 0; yos <= tosuus; yos++) {
	  if (yos && ostiet[Sarjat[sarja].aosuus[yos+1]].ylahto != TMAALI0)
		  yl1 = 1;
	  for (os = Sarjat[sarja].aosuus[yos]+1; os <= Sarjat[sarja].aosuus[yos+1]; os++) {
		  if (Maali(os, va) != TMAALI0) {
			 n_osaika[yos]++;
			 }
		  else if (tlslisa && Sarjat[sarja].nosuus[yos] > 1) {
			 *tlslisa += Sarjat[sarja].puutelisa[Sarjat[sarja].yosuus[os]];
			 }
		  }
	   if (!yl1 && tlslisa && n_osaika[yos] == Sarjat[sarja].nosuus[yos])
		  *tlslisa = 0;
	   }
   if (n_osaika[tosuus] == 0) {
	   return(0);
	  }
   if (tlslisa == NULL && n_osaika[tosuus] < Sarjat[sarja].nosuus[tosuus]) {
	   return(SEK*n_osaika[tosuus]);
	   }
   if (yl1) {
		if (tosuus > 0) {
			etulos = tTulos(tosuus - 1, 0, &lisa);
			if (etulos && lisa == 0) {
				for (os = Sarjat[sarja].aosuus[tosuus] + 1;
					os <= Sarjat[sarja].aosuus[tosuus+1]; os++)
					tls = max(tls, osTulos(os, va));
				tls += etulos;
				}
			}
		}
   else {
	  tls = aTulos(tosuus, va);
	  }
   if (tls < 0)
	   tls += 24*TUNTI;
   return(tls);
}

INT32 kilptietue::aTulos(int tosuus, int va)
{
	long tls = 0, tls1, os;

	if (tosuus < 0 || tosuus >= Sarjat[sarja].ntosuus || va < 0 || va > Sarjat[sarja].valuku[tosuus])
		return(0);

	for (os = Sarjat[sarja].aosuus[tosuus] + 1;
		os <= Sarjat[sarja].aosuus[tosuus+1]; os++) {
		if (Maali(os, va) != TMAALI0) {
			tls1 = (Maali(os, va) - Sarjat[sarja].lahto + 48L*TUNTI) % (24L*TUNTI);
			tls1 += SakkoAika(tosuus, true);
			if (tls1 > tls)
				tls = tls1;
			}
		}
	return(tls);
}

INT32 kilptietue::kokTulos(bool sekvajaat /* = false */)
{
	int tls = 0, tls1, os;

	if (!kilpparam.partio)
		return(Tulos(Sarjat[sarja].osuusluku - 1, 0, sekvajaat));
	switch (Sarjat[sarja].partiotls) {
		case 0:
			tls = Tulos(Sarjat[sarja].osuusluku - 1, 0);
			break;
		case -1:
			for (os = 0; os < Sarjat[sarja].osuusluku; os++)
				if ((tls1 = Tulos(os, 0)) > 0 && (tls == 0 || NORMKELLO(tls1-tls) > 0))
					tls = tls1;
			break;
		case -2:
			for (os = 0; os < Sarjat[sarja].osuusluku; os++) {
				if (ostiet[os].badge[0]) {
					if ((tls1 = Tulos(os, 0)) > 0) {
						if (tls == 0 || NORMKELLO(tls1-tls) > 0)
							tls = tls1;
						}
					else {
						tls = 0;
						break;
						}
					}
				}
			break;
		default:
			if (Sarjat[sarja].partiotls > 0)
				tls = Tulos(Sarjat[sarja].partiotls - 1, 0);
		}
	return(tls);
}

INT32 kilptietue::Tulos(int osuus, int va, bool sekvajaat /* = false */)
{
	long tls,tls1,etulos;
	int os, yl = 0, yosuus;
	__int64 lisa;

	if (osuus < 0 || osuus >= Sarjat[sarja].osuusluku || va < 0)
		return(0);

	if (va > kilpparam.valuku)
		return(osTulos(osuus, 0));

	yosuus = Sarjat[sarja].yosuus[osuus];
	if (Sarjat[sarja].nosuus[yosuus] > 1) {
		tls = tTulos(yosuus, va, &lisa);
		if (lisa && sekvajaat) {
			tls = SEK * (Sarjat[sarja].nosuus[yosuus] - (int)(lisa/Sarjat[sarja].puutelisa[yosuus]));
			}
		return(tls);
		}

	if (kierrokset && va == 0 && osuus == Sarjat[sarja].osuusluku-1) {
		for (os = osuus-1; os >= 0; os--) {
			if ((tls = Tulos(os, 0)) != 0) {
				break;
				}
			}
		if (tls) {
			tls += (kilpparam.n_os - os) * 24 * TUNTI;
			}
		return(tls);
		}
	tls = 0;
	if ( osuus >= 0 && Maali(osuus, va) != TMAALI0 &&
		(Hyv(osuus) || kilpparam.sailhyl)) {
		for (os = osuus; os >= 0; os--) {          // os == 0 lisätty 8.3.2010
			if (ostiet[os].ylahto != TMAALI0) {
				yl = 1;
				break;
				}
			}
		if (kilpparam.rogaining) {      // Rogaining
			if (osuus == 0)
				etulos = 1;
			else
				etulos = Tulos(osuus-1, 0);
			if (etulos > 0) {
				tls = Maali(osuus, va) - Lahto(osuus);
				tls = NORMTULOS(tls);
				tls = max(etulos, tls);
				}
			}
		else if (!yl) {       //  Perustilanne
			tls = Maali(osuus, va) - Sarjat[sarja].lahto;
			tls = NORMTULOS(tls);
			tls += SakkoAika(yosuus, true);
			}
		else {                //  yhteislähtö tai partio
			if (kilpparam.partio || osuus == 0) {            // osuus == 0 lisätty 8.3.2010
				tls = Maali(osuus, va) - ostiet[osuus].ylahto;
				if (kilpparam.partio == 2) {
					for (int os = 0; os < Sarjat[sarja].osuusluku; os++) {
						if (ostiet[os].ylahto != TMAALI0 && tls < Maali(os, va) - ostiet[os].ylahto)
							tls = Maali(os, va) - ostiet[os].ylahto;
						}
					tls += SakkoAika(Sarjat[sarja].osuusluku-1, true);
					}
				else
					tls += SakkoAika(osuus, true);
				}
			else {
				etulos = Tulos(osuus-1, 0);
				if (etulos > 0) {
					if (ostiet[osuus].ylahto != TMAALI0) {
						tls = etulos + Maali(osuus, va) - ostiet[osuus].ylahto;
						}
					else {
						tls = etulos + Maali(osuus, va) - Maali(osuus-1, 0);
						}
					if (yosuus > Sarjat[sarja].yosuus[osuus-1])
						tls += SakkoAika(yosuus, false);
					}
				}
			}
		tls = NORMTULOS(tls);
		if (!kilpparam.partio && osuus > 0) {
			tls1 = Tulos(osuus-1, 0);
			if (tls < tls1)
				tls = tls + 24*TUNTI;
			}
		}
	return(tls);
}

INT32 kilptietue::osTulos(int osuus, int va, bool vaadiJoukkueHyv /* = true */)
{
	TULOS ostulos;
	TMAALI ol;
	bool hyv;

	if (osuus < 0 || osuus >= Sarjat[sarja].osuusluku || va < 0)
		return(0);

	ostulos = 0;
	if (Sarjat[sarja].maxnosuus == 1) {
		if (osuus == 0) {
			ostulos = Tulos(0, va);
			}
		else {
			if (vaadiJoukkueHyv)
				hyv = Hyv(osuus) || kilpparam.sailhyl;
			else
				hyv = osHyv(osuus);
			if (Maali(osuus, va) != TMAALI0 && (ol = Lahto(osuus)) != TMAALI0 && hyv) {
				ostulos = Maali(osuus, va) - ol;
				ostulos += SakkoAika(osuus, false);
				ostulos = (ostulos + 48*TUNTI) % (24*TUNTI);
				}
			}
		}

	else {
	   if ((osHyv(osuus) || kilpparam.sailhyl) && Maali(osuus, va) != TMAALI0 &&
		  (!vaadiJoukkueHyv || !tSulj(Sarjat[sarja].yosuus[osuus]))) {
		  if ((ol = Lahto(osuus)) != TMAALI0) {
			 ostulos = Maali(osuus, va) - ol;
			 ostulos += SakkoAika(Sarjat[sarja].yosuus[osuus], false);
			 }
		  ostulos = (ostulos + 36*TUNTI) % (24*TUNTI) - 12*TUNTI;
		  }
		}

	return(ostulos);
}

int sakkoluku(kilptietue *kilp, int osuus)
{
	return(kilp->ASakkoLkm(osuus));
}

int kilptietue::SakkoAika(int tosuus, bool kumul /* = false */)
	{
	int sak = 0;

	if (kumul && tosuus > 0)
		sak = SakkoAika(tosuus-1, true);

	for (int os = Sarjat[sarja].aosuus[tosuus]+1; os <= Sarjat[sarja].aosuus[tosuus+1]; os++) {
		if (Sarjat[sarja].amptsak[tosuus])
			sak += SEK*Sarjat[sarja].amptsak[tosuus] * ASakkoLkm(os);
		sak += ostiet[os].sakko;
		}
	return(sak);
	}

int kilptietue::TSakkoLkm(int osuus)
	{
	int lkm = 0;

	if (Sarjat[sarja].tsak[osuus])
		lkm = ostiet[osuus].sakko/Sarjat[sarja].tsak[osuus]/SEK;
	else
		lkm = ostiet[osuus].sakko;
	return(lkm);
	}

int kilptietue::ASakkoLkm(int osuus)
	{
	int i, lkm = 0;

	for (i = 0; i < Sarjat[sarja].paikat[osuus]; i++) {
		if (ostiet[osuus].ampsakot[i] > '0' && ostiet[osuus].ampsakot[i] <= '9')
			lkm += ostiet[osuus].ampsakot[i] - '0';
		}
	return(lkm);
	}

INT32 kilptietue::Maali(int osuus, int piste)
{
	if (osuus < 0 || osuus >= Sarjat[sarja].osuusluku || piste < -1 || piste > Sarjat[sarja].valuku[Sarjat[sarja].yosuus[osuus]])
		return(TMAALI0);
	return(ostiet[osuus].Maali(piste));
}

INT16 kilptietue::Sija(int osuus, int piste)
{
	if (osuus < 0 || osuus >= Sarjat[sarja].osuusluku || piste < 0 || piste > Sarjat[sarja].valuku[Sarjat[sarja].yosuus[osuus]])
		return(0);
	return(ostiet[osuus].Sija(piste));
}

INT16 kilptietue::osSija(int osuus)
{
	if (osuus < 0 || osuus >= Sarjat[sarja].osuusluku)
		return(0);
	return(ostiet[osuus].osSija());
}

void kilptietue::setMaali(int osuus, int piste, int aika)
{
	if (osuus < 0 || osuus >= Sarjat[sarja].osuusluku || piste < -1 || piste > Sarjat[sarja].valuku[Sarjat[sarja].yosuus[osuus]])
		return;
	ostiet[osuus].setMaali(piste, aika);
}

void kilptietue::setSija(int osuus, int piste, int sija)
{
	if (osuus < 0 || osuus >= Sarjat[sarja].osuusluku || piste < 0)
		return;
	ostiet[osuus].setSija(piste, sija);
}

void kilptietue::settSija(int osuus, int piste, int sija)
{
	if (osuus < 0 || osuus >= Sarjat[sarja].ntosuus || piste < 0)
		return;
	for (int os = Sarjat[sarja].aosuus[osuus]+1; os <= Sarjat[sarja].aosuus[osuus+1]; os++)
		ostiet[os].setSija(piste, sija);
}

char *osrec::Nimi(char *st, int len, int jarj, char chars)
{
	wchar_t *wst = new wchar_t[len+1];

	wmemset(wst, 0, len+1);
	if (chars <= ' ')
		chars = kilpparam.chars;
	Nimi(wst, len, jarj, chars);
	wcstoansi(st, wst, len);
	delete[] wst;
	return(st);
}

wchar_t *osrec::Nimi(wchar_t *st, int len, int jarj, char chars)
{
	wchar_t *ps, *pe = NULL;
	wchar_t wst[OSNIMIL+1] = L"";

	if (chars <= ' ')
		chars = kilpparam.chars;
	if (jarj == 0)
		jarj = kilpparam.etusuku;
	wmemset(st, 0, len+1);
	if (chars == '8')
		MbsToWcs(wst, nimi, OSNIMIL);
	else
		oemtowcs(wst, nimi, OSNIMIL, 0);
//		ansitowcs(wst, nimi, OSNIMIL);
	wst[OSNIMIL] = 0;
	if (len < OSNIMIL)
		wst[len] = 0;
	ps = wcstok(wst, L"|");
	if (ps) {
		pe = wcstok(NULL, L"|");
		}
	if (jarj < 0) {
		if (pe) {
			wcscpy(st, pe);
			if (ps[0])
				wcscat(st, L" ");
			}
		if (ps)
			wcscat(st, ps);
		}
	else {
		if (ps) {
			wcscpy(st, ps);
			if (pe && pe[0]) {
				wcscat(st, L" ");
				wcscat(st, pe);
				}
			}
		}
	return(st);
}

wchar_t *osrec::SukuNimi(wchar_t *st, int len, char chars)
{
	wchar_t *ps;
	wchar_t wst[OSNIMIL+1] = L"";

	if (chars <= ' ')
		chars = kilpparam.chars;
	if (chars == '8')
		MbsToWcs(wst, nimi, OSNIMIL);
	else
		oemtowcs(wst, nimi, OSNIMIL, 0);
//		ansitowcs(wst, nimi, OSNIMIL);
	wst[OSNIMIL] = 0;
	if (len < OSNIMIL)
		wst[len] = 0;
	wmemset(st, 0, len+1);
	if (wst[0] != L'|') {
		ps = wcstok(wst, L"|");
		if (ps)
			wcscpy(st, ps);
		}
	return(st);
}

wchar_t *osrec::EtuNimi(wchar_t *st, int len, char chars)
{
	wchar_t *ps;
	wchar_t wst[OSNIMIL+1] = L"";

	if (chars <= ' ')
		chars = kilpparam.chars;
	if (chars == '8')
		MbsToWcs(wst, nimi, OSNIMIL);
	else
		oemtowcs(wst, nimi, OSNIMIL, 0);
//		ansitowcs(wst, nimi, OSNIMIL);
	wst[OSNIMIL] = 0;
	if (len < OSNIMIL)
		wst[len] = 0;
	wmemset(st, 0, len+1);
	if (wst[0] == L'|') {
		ps = wst+1;
		}
	else {
		ps = wcstok(wst, L"|");
		if (ps)
			ps = wcstok(NULL, L"|");
		}
	if (ps)
		wcscpy(st, ps);
	return(st);
}

void osrec::setNimi(char *etunimi, char *sukunimi, char chars)
{
	wchar_t en[OSNIMIL+1] = L"", sn[OSNIMIL+1] = L"";

	if (chars <= ' ')
		chars = kilpparam.chars;
	ansitowcs(en, etunimi, OSNIMIL);
	ansitowcs(sn, sukunimi, OSNIMIL);
	en[OSNIMIL] = 0;
	sn[OSNIMIL] = 0;
	setNimi(en, sn, chars);
}

void osrec::setNimi(wchar_t *etunimi, wchar_t *sukunimi, char chars)
{
	int l, le, ls;
	wchar_t st[OSNIMIL+1] = L"";
	char en[OSNIMIL+1] = "", sn[OSNIMIL+1] = "";

	if (chars <= ' ')
		chars = kilpparam.chars;
	l = OSNIMIL;
	ls = -1;
	wcsncpy(st, sukunimi, l);
	st[l] = 0;
	if (chars != '8') {
		wcstoansi(nimi, st, l);
		ls = strlen(nimi);
		if (ls < l - 1 && wcslen(etunimi) > 0) {
			strcat(nimi, "|");
			l = OSNIMIL - ls - 1;
			wcstoansi(nimi+strlen(nimi), etunimi, l);
			}
		}
	else {
		do {
			if ((ls = WcsToMbs(sn, st, l)) < 0)
				st[wcslen(st)-1] = 0;
			} while (st[0] != 0 && ls < 0);
		strcpy(nimi, sn);
		if (ls < l - 1 && wcslen(etunimi) > 0) {
			strcat(nimi, "|");
			l = OSNIMIL - ls - 1;
			le = -1;
			wcsncpy(st, etunimi, l);
			st[l] = 0;
			do {
				if ((le = WcsToMbs(en, st, l)) < 0)
					st[wcslen(st)-1] = 0;
				} while (st[0] != 0 && le < 0);
			strcat(nimi, en);
			}
		}
	nimi[OSNIMIL] = 0;
}

void osrec::setNimi(wchar_t *nimistr, int jarj, char chars)
{
	wchar_t *p, ste[200] = L"", sts[200] = L"";

	if (chars <= ' ')
		chars = kilpparam.chars;
	if (jarj == 0)
		jarj = kilpparam.etusuku;
	if ((p = wcsstr(nimistr, L"|")) != NULL) {
		wmemcpy(ste, nimistr, p-nimistr);
		wcsncpy(sts, p+1, 198);
		while ((p = wcsstr(sts, L"|")) != NULL)
			*p = ' ';
		}
	else {
		wesnimet(nimistr, ste, sts);
		}
	if (jarj > 0)
		setNimi(ste, sts, chars);
	else
		setNimi(sts, ste, chars);
}

void osrec::setNimi(char *nimistr, int jarj, char chars)
{
	wchar_t wstr[200] = L"";

	oemtowcs(wstr, nimistr, 199, 0);
	setNimi(wstr, jarj, chars);
}

void osrec::setEtuNimi(wchar_t *etunimi, char chars)
{
	wchar_t st[60];

	setNimi(etunimi, SukuNimi(st, 58, chars), chars);
}

void osrec::setSukuNimi(wchar_t *sukunimi, char chars)
{
	wchar_t st[60];

	setNimi(EtuNimi(st, 58, chars), sukunimi, chars);
}

char *kilptietue::Nimi(char *st, int len, int osuus, int jarj, char chars)
{
	ostiet[osuus].Nimi(st, len, jarj, chars);
	return(st);
}

void kilptietue::setNimi(char *etunimi, char *sukunimi, int osuus, char chars)
{
	ostiet[osuus].setNimi(etunimi, sukunimi, chars);
}

wchar_t *kilptietue::Nimi(wchar_t *st, int len, int osuus, int jarj, char chars)
{
	ostiet[osuus].Nimi(st, len, jarj, chars);
	return(st);
}

wchar_t *kilptietue::SukuNimi(wchar_t *st, int len, int osuus, char chars)
{
	ostiet[osuus].SukuNimi(st, len, chars);
	return(st);
}

wchar_t *kilptietue::EtuNimi(wchar_t *st, int len, int osuus, char chars)
{
	ostiet[osuus].EtuNimi(st, len, chars);
	return(st);
}

void kilptietue::setNimi(wchar_t *etunimi, wchar_t *sukunimi, int osuus, char chars)
{
	ostiet[osuus].setNimi(etunimi, sukunimi, chars);
}

void kilptietue::setNimi(wchar_t *nimistr, int osuus, int jarj, char chars)
{
	ostiet[osuus].setNimi(nimistr, jarj, chars);
}

void kilptietue::setNimi(char *nimistr, int osuus, int jarj, char chars)
{
	ostiet[osuus].setNimi(nimistr, jarj, chars);
}

void kilptietue::setEtuNimi(wchar_t *etunimi, int osuus, char chars)
{
	ostiet[osuus].setEtuNimi(etunimi, chars);
}

void kilptietue::setSukuNimi(wchar_t *sukunimi, int osuus, char chars)
{
	ostiet[osuus].setSukuNimi(sukunimi, chars);
}

char kilptietue::tTark(int osuus)
{
	char kh = 'T';

	if (Sarjat[sarja].nosuus[osuus] == 1)
		return(Tark(Sarjat[sarja].aosuus[osuus+1], 1));

	if (tSulj(osuus))
		return('S');
	for (int os = Sarjat[sarja].aosuus[osuus] + 1; os <= Sarjat[sarja].aosuus[osuus+1]; os++) {
		char kh1 = ostiet[os].keskhyl;
		if (kh1 == 'E')
			return('E');
		if (kh == 'T' || kh1 == 'K' ||
			(kh1 == 'H' && kh != 'K') ||
			(kh1 == 'I' && kh != '-') 
			) {
			kh = kh1;
			continue;
			}
		}
	return(kh);
}

char kilptietue::Tark(int osuus, int naytasulj /* = 0 */)
{
	char kh;

	if (naytasulj && Sarjat[sarja].nosuus[Sarjat[sarja].yosuus[osuus]] > 1)
		return(tTark(Sarjat[sarja].yosuus[osuus]));

	kh = ostiet[osuus].keskhyl;
	if (localparam.koodiEil && naytasulj && kh == '-')
		return('E');
	if (osuus == -1) {
		if (!Hyv(0))
			kh = Tark(0, 0);
		else if (!Hyv(Sarjat[sarja].osuusluku-1)) {
			kh = 'K';
			for (int os = 0; os < Sarjat[sarja].osuusluku; os++) {
				if (Tark(os, 0) == 'H') {
					kh = 'H';
					break;
					}
				}
			}
		else {
			kh = 'T';
			for (int os = 0; os < Sarjat[sarja].osuusluku; os++) {
				if (Tark(os, 0) != 'T') {
					kh = '-';
					break;
					}
				}
			}
		}
	else {
		if (wcswcind(ostiet[osuus].keskhyl, L"EP") < 0 && osuus > 0 && naytasulj && (Sulj(osuus) ||
			(osuus == Sarjat[sarja].osuusluku - 1 && kh == 'E'))) {
			if (naytasulj == 1 && Sulj(osuus))
				kh = 'S';
			else {
				for (int os = 0; os <= osuus; os++) {
					if (os == osuus || !Hyv(os)) {
						kh = ostiet[os].keskhyl;
						if (os > 0 && kh == 'E')
							kh = 'K';
						break;
						}
					}
				}
			}
		}
	return(kh);
}

wchar_t *kilptietue::TarkStr(int osuus, bool naytasulj /* = false */, int lang /* = 0 */)
{
	wchar_t kh;

	if (osuus == -1) {
		kh = wTark(0, 0);
		if (wcswcind(kh, L"EP") < 0) {
			if (!Hyv(Sarjat[sarja].osuusluku-1)) {
				kh = L'K';
				for (int os = 0; os < Sarjat[sarja].osuusluku; os++) {
					if (Tark(os, 0) == 'H') {
						kh = L'H';
						break;
						}
					}
				}
			else {
				kh = L'T';
				for (int os = 0; os < Sarjat[sarja].osuusluku; os++) {
					if (Tark(os, 0) != 'T') {
						kh = L'-';
						break;
						}
					}
				}
			}
		}
	else {
		kh = wTark(osuus, 0);
		if (wcswcind(kh, L"EP") < 0 && naytasulj && Sulj(osuus)) {
			if (naytasulj == 1)
				kh = L'S';
			else {
				for (int os = 0; os < osuus; os++) {
					if (!Hyv(os)) {
						kh = wTark(os, 0);
						if (os > 0 && kh == L'E')
							kh = L'K';
						break;
						}
					}
				}
			}
		}
	if (lang == 0) {
		switch (kh) {
			case L'T' :
				return(L"Tark.");
			case L'H' :
				return(L"Hyl.");
			case L'I' :
				return(L"H-esit.");
			case L'K' :
				return(L"Kesk.");
			case L'E' :
				return(L"Ei läht.");
			case L'P' :
				return(L"Poissa");
			case L'S' :
				return(L"Sulj.");
			default:
				return(L"Avoinna");
			}
		}
	else {
		switch (kh) {
			case L'T' :
				return(L"OK");
			case L'H' :
				return(L"DQ");
			case L'I' :
				return(L"Recheck");
			case L'K' :
				return(L"DNF");
			case L'E' :
				return(L"DNS");
			case L'P' :
				return(L"Absent");
			case L'S' :
				return(L"Closed");
			default:
				return(L"Open");
			}
		}
}

wchar_t *kilptietue::tTarkStr(int osuus, int lang /* = 0 */)
{
	wchar_t kh;

	if (osuus == -1) {
		if (!tHyv(Sarjat[sarja].ntosuus-1)) {
			kh = 'K';
			for (int os = 0; os < Sarjat[sarja].ntosuus; os++) {
				if (tTark(os) == 'H') {
					kh = 'H';
					break;
					}
				}
			}
		else {
			kh = 'T';
			for (int os = 0; os < Sarjat[sarja].ntosuus; os++) {
				if (tTark(os) != 'T') {
					kh = '-';
					break;
					}
				}
			}
		}
	else {
		kh = wtTark(osuus);
		if (wcswcind(kh, L"EP") < 0 && Sulj(osuus)) {
			kh = L'S';
			}
		}
	if (lang == 0) {
		switch (kh) {
			case L'T' :
				return(L"Tark.");
			case L'H' :
				return(L"Hyl.");
			case L'I' :
				return(L"H-esit.");
			case L'K' :
				return(L"Kesk.");
			case L'E' :
				return(L"Ei läht.");
			case L'P' :
				return(L"Poissa");
			case L'S' :
				return(L"Sulj.");
			default:
				return(L"Avoinna");
			}
		}
	else {
		switch (kh) {
			case L'T' :
				return(L"OK");
			case L'H' :
				return(L"DQ");
			case L'I' :
				return(L"Recheck");
			case L'K' :
				return(L"DNF");
			case L'E' :
				return(L"DNS");
			case L'P' :
				return(L"Absent");
			case L'S' :
				return(L"Closed");
			default:
				return(L"Open");
			}
		}
}

wchar_t kilptietue::wtTark(int osuus)
{
	char ch = tTark(osuus);
	return(ansitowchar(ch));
}

wchar_t kilptietue::wTark(int osuus, int naytasulj)
{
	char ch = Tark(osuus, naytasulj);
	return(ansitowchar(ch));
}

void kilptietue::SetTark(int osuus, wchar_t trk)
{
	SetTark(osuus, wchartoansi(trk));
}

char *kilptietue::Seura(char *st, int len, char chars)
{
	wchar_t wst[LSEURA+1] = L"";

	if (len > LSEURA)
		len = LSEURA;
	Seura(wst, len, chars);
	wst[len] = 0;
	return(wcstoansi(st, wst, len));
}

char *kilptietue::Joukkue(char *st, int len, char chars)
{
	char jk[12] = "";

	memset(st, 0, len+1);
	if (joukkue)
		sprintf(jk, "-%d", joukkue);
	Seura(st, len-strlen(jk), chars);
	strcat(st, jk);
	return(st);
}

void kilptietue::setSeura(char *sra, char chars)
{
	wchar_t wst[LSEURA+1] = L"";

	ansitowcs(wst, sra, LSEURA);
	wst[LSEURA] = 0;
	setSeura(wst, chars);
}

wchar_t *kilptietue::Seura(wchar_t *st, int len, char chars)
{
	if (chars <= ' ')
		chars = kilpparam.chars;
	if (chars == '8')
		MbsToWcs(st, seura, len);
	else
		oemtowcs(st, seura, len, 0);
//		ansitowcs(st, seura, len);
	st[len] = 0;
	return(st);
}

wchar_t *kilptietue::Joukkue(wchar_t *st, int len, char chars/* = ' ' */)
{
	wchar_t jk[12] = L"";

	wmemset(st, 0, len+1);
	if (joukkue)
		swprintf(jk, L"-%d", joukkue);
	Seura(st, len-wcslen(jk), chars);
	wcscat(st, jk);
	return(st);
}

void kilptietue::setSeura(wchar_t *sra, char chars /* = ' ' */)
{
	wchar_t wst[LSEURA+1] = L"";

	if (chars <= ' ')
		chars = kilpparam.chars;
	wcsncpy(wst, sra, LSEURA);
	wst[LSEURA] = 0;
	if (chars != '8') {
		wcstoansi(seura, wst, LSEURA);
		}
	else {
		while (wst[0] && WcsToMbs(seura, wst, LSEURA) < 0)
			wst[wcslen(wst)-1] = 0;
		}
}

wchar_t *kilptietue::Joukknimi(wchar_t *st, int len, char chars)
{
	if (chars <= ' ')
		chars = kilpparam.chars;
	if (chars == '8')
		MbsToWcs(st, joukknimi, len);
	else
		oemtowcs(st, joukknimi, len, 0);
//		ansitowcs(st, seura, len);
	st[len] = 0;
	return(st);
}

void kilptietue::setJoukknimi(wchar_t *st, char chars /* = ' ' */)
{
	wchar_t wst[LJK+1] = L"";

	if (chars <= ' ')
		chars = kilpparam.chars;
	wcsncpy(wst, st, LJK);
	wst[LJK] = 0;
	if (chars != '8') {
		wcstoansi(joukknimi, wst, LJK);
		}
	else {
		while (wst[0] && WcsToMbs(joukknimi, wst, LJK) < 0)
			wst[wcslen(wst)-1] = 0;
		}
}

bool kilptietue::osHyv(int osuus)
{
	char ch;

	ch = Tark(osuus, 0);
	return(stschind(ch, "TI-") >= 0);
}

bool kilptietue::tHyv(int osuus /* =-1 */)
{
	if (osuus == -1)
		osuus = Sarjat[sarja].osuusluku-1;
	else
		osuus = Sarjat[sarja].aosuus[osuus+1];
	for (int os = 0; os <= osuus; os++)
		if (stschind(ostiet[os].keskhyl, "TI-") < 0)
			return(false);
	return(true);
}

bool kilptietue::Hyv(int osuus /* =-1 */)
{
	if (osuus == -1)
		osuus = Sarjat[sarja].osuusluku-1;
	for (int os = 0; os <= osuus; os++)
		if (stschind(ostiet[os].keskhyl, "TI-") < 0)
			return(false);
	return(true);
}

bool kilptietue::tSulj(int osuus)
{
	if (osuus > 0 && !tHyv(osuus-1))
		return(true);
	return(false);
}

bool kilptietue::Sulj(int osuus)
{
	if (osuus > 0 && !Hyv(osuus-1))
		return(true);
	return(false);
}

void kilptietue::SetTark(int osuus, char trk)
{
	if (stschind(trk, "-TIKHEP") >= 0)
		ostiet[osuus].keskhyl = trk;
}

wchar_t osrec::Laina(int kortti /* = 0 */)
{
	wchar_t ch = towupper(ansitowchar(laina[kortti]));
	if (ch != L'L')
		ch = L' ';
	return(ch);
}

wchar_t osrec::Seuranta(void)
{
	if (seuranta >= 'A')
		return(toupper(seuranta));
	else
		return(L' ');
}

wchar_t *osrec::Selitys(wchar_t *st)
{
	static wchar_t wst[40];
	if (!st) {
		st = wst;
		}
	ansitowcs(st, selitys, sizeof(selitys));
	return(st);
}

wchar_t *osrec::Hajonta(wchar_t *st)
{
	static wchar_t wst[40];
	if (!st) {
		st = wst;
		}
	ansitowcs(st, hajonta, sizeof(hajonta));
	return(st);
}

wchar_t *osrec::Arvo(wchar_t *st)
{
	static wchar_t wst[LARVO+1];
	if (!st) {
		st = wst;
		}
	MbsToWcs(st, arvo, LARVO);
	return(st);
}

wchar_t *osrec::OsSeura(wchar_t *st)
{
	static wchar_t wst[OSSEURAL+1];
	if (!st) {
		st = wst;
		}
	MbsToWcs(st, osseura, OSSEURAL);
	return(st);
}

wchar_t *osrec::Ampsakot(wchar_t *st)
{
	static wchar_t wst[40];
	memset(wst, 0, sizeof(wst));
	if (!st) {
		st = wst;
		}
	ansitowcs(st, ampsakot, sizeof(ampsakot));
	return(st);
}

void osrec::setLaina(bool lainafl, int kortti /* = 0 */)
{
	laina[kortti] = lainafl ? 'L' : ' ';
}

void osrec::setLaina(wchar_t wlaina, int kortti /* = 0 */)
{
	laina[kortti] = toupper(wchartoansi(wlaina));
}

void osrec::setSeuranta(wchar_t ch)
{
	if (ch >= L'A')
		seuranta = wchartoansi(towupper(ch));
	else
		seuranta = 0;
}

void osrec::setSelitys(wchar_t *st)
{
	wcstoansi(selitys, st, sizeof(selitys)-1);
}

int osrec::setHajonta(wchar_t *st)
{
	wcstoansi(hajonta, st, sizeof(hajonta)-1);
	return(wcslen(st) >= sizeof(hajonta));
}

void osrec::setAmpsakot(int i, int ohi)
{
	if (ohi < 0)
		ampsakot[i] = '-';
	ampsakot[i] = '0'+ohi;
}

void osrec::setAmpsakot(wchar_t *st)
{
	wcstoansi(ampsakot, st, sizeof(ampsakot));
}

wchar_t *kilptietue::Maa(wchar_t *st)
{
	static wchar_t wst[8];

	if (st == NULL)
		st = wst;
	return(ansitowcs(st, maa, sizeof(maa)));
}

char *kilptietue::Maa(char *st)
{
	st = maa;
	return(maa);
}

void kilptietue::setMaa(wchar_t *st)
{
	wcstoansi(maa, st, sizeof(maa));
}

wchar_t kilptietue::Laina(int os, int kortti /* = 0 */)
{
	return(ostiet[os].Laina(kortti));
}

wchar_t kilptietue::Seuranta(int os)
{
	return(ostiet[os].Seuranta());
}

wchar_t *kilptietue::Arvo(int os, wchar_t *st)
{
	return(ostiet[os].Arvo(st));
}

wchar_t *kilptietue::OsSeura(int os, wchar_t *st)
{
	return(ostiet[os].OsSeura(st));
}

wchar_t *kilptietue::Selitys(int os, wchar_t *st)
{
	return(ostiet[os].Selitys(st));
}

wchar_t *kilptietue::Hajonta(int os, wchar_t *st)
{
	return(ostiet[os].Hajonta(st));
}

wchar_t *kilptietue::Ampsakot(int os, wchar_t *st)
{

	return(ostiet[os].Ampsakot(st));
}

void kilptietue::setLaina(int os, bool lainafl, int kortti /* = 0 */)
{
	ostiet[os].setLaina(lainafl, kortti);
}

void kilptietue::setLaina(int os, wchar_t wlaina, int kortti /* = 0 */)
{
	ostiet[os].setLaina(wlaina, kortti);
}

void kilptietue::setSeuranta(int os, wchar_t ch)
{
	ostiet[os].setSeuranta(ch);
}

void kilptietue::setSelitys(int os, wchar_t *st)
{
	ostiet[os].setSelitys(st);
}

int kilptietue::setHajonta(int os, wchar_t *st)
{
	return(ostiet[os].setHajonta(st));
}

void kilptietue::setAmpsakot(int os, int i, int ohi)
{
	ostiet[os].setAmpsakot(i, ohi);
}

void kilptietue::setAmpsakot(int os, wchar_t *st)
{
	ostiet[os].setAmpsakot(st);
}

void kilptietue::Tallenna(int d, int tarbuffl, int kielto, int pakota_os, kilptietue *entkilp)
{
	tallenna(this, d, tarbuffl, kielto, pakota_os, entkilp);
}

INT32 osuuslahto(kilptietue *kilp, int osuus)
{
   return(kilp->Lahto(osuus));
}

INT32 osuustulos(kilptietue *kilp, int osuus, int va)
{
   return(kilp->osTulos(osuus, va));
}

INT32 ttulos(kilptietue *kilp, int osuus, int va, int lisafl)
{
   return(tulos(kilp, osuus, va));
}

INT32 tulos(kilptietue *kilp, int osuus, int va)
{
	return(kilp->Tulos(osuus, va));
}
//---------------------------------------------------------------------------

void kilptietue::laskeKierrokset(void)
{
	int viimos = 0, edtls = 0, tls = 0, vrk = 0;

	for (int i = 0; i < Sarjat[sarja].osuusluku-1; i++) {
		if ((tls = Tulos(i, 0)) > 0) {
			viimos = i+1;
			if (tls < edtls - 12*TUNTI)
				vrk++;
			edtls = tls;
			}
		}
	pisteet[0] = viimos;
	pisteet[1] = vrk;
	setMaali(Sarjat[sarja].osuusluku-1, 0, Lahto(0)+edtls);
}
//---------------------------------------------------------------------------

