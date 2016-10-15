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

#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <process.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <bkeybrd.h>
#include <bstrings.h>
#pragma hdrstop
#include <conio.h>
#include <dos.h>
#include <sys\stat.h>
#include "HkDeclare.h"
#include "HkMuotoilu.h"
#include "TpLaitteet.h"

#ifndef MAXTUNNUS
#define MAXTUNNUS  899
#endif
#define RINNTUNNUS  5

void laheta_emva(emittp *em);
void laheta_emva_pois(int kno);
static void tarkkilpradat(void);
static int TarkKilpailijat(void);

// Leimantarkastuksen pohjana on taulukko kuvio, johon on tallennettu
// luettelo kaikista hyväksyttävistä rastitunnus-leimasinkoodi -yhdistelmistä
// Luettelo on aina järjestettynä rastitunnusten mukaiseen järjestykseen.
// Koodin arvo 1 tarkoittaa, että kaikki leimaukset hyväksytään.

class emitvaclass {
public:
   int tm;
   int tl;
   UINT32 *va[2];
   int *sj[2];
   emitvaclass(void);
   ~emitvaclass(void);
   };

#if defined(EMIT)

void emitva_uusinta(int cn, int tietue);
void editemit(INT32 ep, emittp *em, kilptietue *kilp);
static INT on_kuvio(INT rasti);

ekditp *kuvio;
INT nkuvio;
static INT maxnkuvio;
static INT emitosa, ajat_on_loc;
static int vakvapautus_bdg, emitptr_kno;
extern INT valinelen, vapagelen, vapageskip, tarkrappageskip;
static bdg_kno_tp vapautettu_bdg[5];

rvalitp *rvalit;
int rvluku;
int analvertosuus = 5;
int analvertmax = 10;
emiterotp *EmitErot;
int nEmitErot;
rastivatp *rastiva;
rastikoordtp rastikoord[MAXNPISTE];
maptp Map;

IOFPoint::IOFPoint(void)
{
	memset(this, 0, sizeof(IOFPoint));
}

IOFPoint::~IOFPoint(void)
{
}

void IOFPoint::nollaa(void)
{
	memset(this, 0, sizeof(IOFPoint));
}

void IOFPoint::operator=(IOFPoint &IP)
{
	memcpy(&PointType, &IP.PointType, sizeof(IOFPoint));
}

IOFCourseControl::IOFCourseControl(void)
{
	memset(this, 0, sizeof(IOFCourseControl));
}

IOFCourseControl::~IOFCourseControl(void)
{
}

void IOFCourseControl::nollaa(void)
{
	memset(this, 0, sizeof(IOFCourseControl));
}

void IOFCourseControl::operator=(IOFCourseControl &IC)
{
	memcpy(&iPoint, &IC.iPoint, sizeof(IOFCourseControl));
}

IOFClassAssignment::IOFClassAssignment(void)
{
	nollaa();
}

IOFClassAssignment::~IOFClassAssignment(void)
{
}

void IOFClassAssignment::nollaa(void)
{
	memset(this, 0, sizeof(IOFClassAssignment));
}

void IOFClassAssignment::operator=(IOFClassAssignment &IA)
{
	memcpy(this, &IA, sizeof(IOFClassAssignment));
}

IOFCourseVariation::IOFCourseVariation(void)
{
	memset(Name, 0, sizeof(IOFCourseVariation));
	nollaa();
}

IOFCourseVariation::~IOFCourseVariation(void)
{
	for (int i = 0; i < (int)(sizeof(CourseControl)/sizeof(CourseControl[0])); i++)
		if (CourseControl[i])
			delete CourseControl[i];
}

void IOFCourseVariation::operator=(IOFCourseVariation &CVar)
{
	memcpy(Name, CVar.Name, sizeof(Name));
	memcpy(Id, CVar.Id, sizeof(Id));
	Length = CVar.Length;
	Climb = CVar.Climb;
	StartLeg = CVar.StartLeg;
	FinLeg = CVar.FinLeg;
	ennakko = CVar.ennakko;
	ajanottotapa = CVar.ajanottotapa;
	memcpy(Start, CVar.Start, sizeof(Start));
	memcpy(Finish, CVar.Finish, sizeof(Finish));
	memset(CourseControl, 0, sizeof(CourseControl));
	for (unsigned int ir = 0; ir < sizeof(CourseControl)/sizeof(CourseControl[0]) && CVar.CourseControl[ir]; ir++) {
		CourseControl[ir] = new IOFCourseControl;
		memcpy(CourseControl[ir], CVar.CourseControl[ir], sizeof(IOFCourseControl));
		}
}

void IOFCourseVariation::nollaa(void)
{
	for (unsigned int i = 0; i < sizeof(CourseControl)/sizeof(CourseControl[0]); i++)
		if (CourseControl[i])
			delete CourseControl[i];
	memset(Name, 0, sizeof(IOFCourseVariation));
	Name[0] = L'0';
	Id[0] = L'0';
	ennakko = kuntosuunnmoodi > 0 ? 0 : -1;
	ajanottotapa = 4;
}

IOFCourse::IOFCourse(void)
{
	memset(Name, 0, sizeof(IOFCourse));
}

IOFCourse::~IOFCourse(void)
{
	for (unsigned int i = 0; i < sizeof(CourseVariation)/sizeof(CourseVariation[0]); i++)
		if (CourseVariation[i])
			delete CourseVariation[i];
}

void IOFCourse::nollaa(void)
{
	for (unsigned int i = 0; i < sizeof(CourseVariation)/sizeof(CourseVariation[0]); i++)
		if (CourseVariation[i]) {
			delete CourseVariation[i];
			}
	memset(Name, 0, sizeof(IOFCourse));
}

void IOFCourse::operator=(IOFCourse &CC)
{
	memcpy(Name, CC.Name, sizeof(Name));
	memcpy(Id, CC.Id, sizeof(Id));
	memcpy(ShortName, CC.ShortName, sizeof(ShortName));
	memset(CourseVariation, 0, sizeof(CourseVariation));
	for (unsigned int ir = 0; ir < sizeof(CourseVariation)/sizeof(CourseVariation[0]) && CC.CourseVariation[ir]; ir++) {
		CourseVariation[ir] = new IOFCourseVariation;
		CourseVariation[ir][0] = CC.CourseVariation[ir][0];
		}
}

IOFCourseData::IOFCourseData(void)
{
	memset(DocType, 0, sizeof(IOFCourseData));
}

IOFCourseData::~IOFCourseData(void)
{
	nollaa();
}

void IOFCourseData::nollaa(void)
{
	for (unsigned int i = 0; i < sizeof(Point)/sizeof(Point[0]); i++) {
		if (Point[i])
			delete Point[i];
		}
	for (unsigned int i = 0; i < sizeof(Course)/sizeof(Course[0]); i++) {
		if (Course[i])
			delete Course[i];
		}
	for (unsigned int i = 0; i < sizeof(ClassAssignment)/sizeof(ClassAssignment[0]); i++) {
		if (ClassAssignment[i])
			delete ClassAssignment[i];
		}
	memset(DocType, 0, sizeof(IOFCourseData));
}

void IOFCourseData::CopyMap(IOFCourseData &CD)
{
	MapScale = CD.MapScale;
	MapTY = CD.MapTY;
	MapLX = CD.MapLX;
	MapBY = CD.MapBY;
	MapRX = CD.MapRX;
}

void IOFCourseData::operator=(IOFCourseData &CD)
{
	nollaa();
	memcpy(DocType, CD.DocType, sizeof(DocType));
	memcpy(IOFVersion, CD.IOFVersion, sizeof(IOFVersion));
	memcpy(ModifyDate, CD.ModifyDate, sizeof(ModifyDate));
	memcpy(ModifyTime, CD.ModifyTime, sizeof(ModifyTime));
	CopyMap(CD);
	for (unsigned int i = 0; i < sizeof(Point)/sizeof(Point[0]); i++) {
		if (CD.Point[i]) {
			Point[i] = new IOFPoint;
			Point[i][0] = CD.Point[i][0];
			}
		}
	for (unsigned int i = 0; i < sizeof(Course)/sizeof(Course[0]); i++) {
		if (CD.Course[i]) {
			Course[i] = new IOFCourse;
			Course[i][0] = CD.Course[i][0];
			}
		}
	for (unsigned int i = 0; i < sizeof(ClassAssignment)/sizeof(ClassAssignment[0]); i++) {
		if (CD.ClassAssignment[i]) {
			ClassAssignment[i] = new IOFClassAssignment;
			ClassAssignment[i][0] = CD.ClassAssignment[i][0];
			}
		}
}

int IOFCourseData::siirraXmlKartta(maptp *map)
{
	map->scale = MapScale;
	map->topY = MapTY;
	map->leftX = MapLX;
	map->btmY = MapBY;
	map->rightX = MapRX;
	return(0);
}

int IOFCourseData::siirraXmlKuviot(ekditp **kuvio, int *nkuvio, int *maxnkuvio)
{
	INT k, tn, nr = 0, ikrd = 0, nk = 0;
	wchar_t msg[80];
	ekditp tmpkdi[2000];
	int rastit[2000];
	rastikoordtp koord[sizeof(rastikoord)/sizeof(rastikoord[0])];

	if (kuvio) {
		if (*maxnkuvio && *kuvio) {
			memset(*kuvio, 0, *maxnkuvio*sizeof(ekditp));
			}
		else {
			if (*kuvio)
				free(*kuvio);
			*kuvio = NULL;
			}
		}
	memset(koord, 0, sizeof(koord));
	for (UINT32 i = 0; i < sizeof(Point)/sizeof(Point[0]) && Point[i] != NULL; i++) {
		int krdlisa = 0;

		if (towupper(Point[i]->Code[0]) == L'R' && towupper(Point[i]->Code[1]) == L'R')
			tn = 880 + _wtoi(Point[i]->Code+2);
		else
			tn = _wtoi(Point[i]->Code);
		if (tn > MAXTUNNUS) {
			swprintf(msg,L"Rastitunnus %d on rastitiedoissa liian suuri", tn);
			kirj_err_file(msg, 1);
			continue;
			}
		if (Point[i]->CY) {
			koord[ikrd].Y = Point[i]->CY;
			krdlisa = 1;
			}
		if (Point[i]->CX) {
			koord[ikrd].X = Point[i]->CX;
			krdlisa = 1;
			}
		if (Point[i]->MapY) {
			koord[ikrd].mapY = Point[i]->MapY;
			krdlisa = 1;
			}
		if (Point[i]->MapX) {
			koord[ikrd].mapX = Point[i]->MapX;
			krdlisa = 1;
			}
		if (krdlisa) {
			if (tn)
				_itow(tn, koord[ikrd].tunnus, 10);
			else
				wcsncpy(koord[ikrd].tunnus, Point[i]->Code, sizeof(koord[ikrd].tunnus)/2-1);
			if (ikrd < sizeof(koord)/sizeof(koord[0]) - 1)
				ikrd += krdlisa;
			}

		if (!kuvio || tn == 0)
			continue;
		rastit[nr++] = tn;
		for (UINT32 j = 0; j < sizeof(Point[0]->ECodes)/sizeof(Point[0]->ECodes[0])
			&& (k = Point[i]->ECodes[j]) != 0; j++) {
			tmpkdi[nk].rasti = tn;
			tmpkdi[nk].koodi = k;
			++nk;
			}
		}
	if (ikrd > 3)
		memcpy(rastikoord, koord, sizeof(rastikoord));
	if (!kuvio)
		return(0);
	*nkuvio = nk;
	if (nk == 0) {
		vidspwmsg(20,5,7,0,
			L"EMIT-koodit asetataan samoiksi kuin rastitunnukset");
		if (*maxnkuvio == 0) {
			*kuvio = (ekditp *)calloc(300, sizeof(ekditp));
			*maxnkuvio = 300;
			}
		if (!*kuvio) {
			writeerror_w(L"Ei muistitilaa EMIT-koodeille", 0);
			return(1);
			}
		else {
			if (nr > 0) {
				for (k = 0; k < nr && k < *maxnkuvio; k++) {
					kuvio[0][k].rasti = rastit[k];
					kuvio[0][k].koodi = kuvio[0][k].rasti;
					}
				*nkuvio = nr;
				}
			else {
				for (k = 0; k < 255 && k < *maxnkuvio; k++) {
					kuvio[0][k].rasti = k+1;
					kuvio[0][k].koodi = kuvio[0][k].rasti;
					}
				*nkuvio = 255;
				}
			}
		}
	else {
		qsortint(tmpkdi, *nkuvio, sizeof(ekditp), 0,  sizeof(kuvio[0][k].rasti)==4);
		if (*maxnkuvio == 0) {
			*kuvio = (ekditp *) calloc(*nkuvio+40, sizeof(ekditp));
			if (!*kuvio) {
				writeerror_w(L"Ei muistitilaa EMIT-koodeille", 0);
				return(1);
				}
			*maxnkuvio = *nkuvio+40;
			}
		if (*nkuvio < *maxnkuvio) {
			memcpy(*kuvio, tmpkdi, *nkuvio * sizeof(ekditp));
			}
		}
	return(0);
}

int IOFCourseData::siirraXmlRadat(ratatp **rt, int *nrata, wchar_t nimilaji, int maalikoodi, int ajanottotapa, int ennakko)
{
	UINT32 i, j, ir, kdi[MAXNRASTI], mtk[MAXNRASTI], rno, nr = 0, vapaajarj = 0, ensilm = 1;
	int srj = 0, tn, maxnrata = 0, puute = 0;
	wchar_t msg[80], rnimi[20]= L"";

	if (*nrata) {
		maxnrata = *nrata;
		memset(*rt, 0, maxnrata*sizeof(ratatp));
		}
	else if (*rt) {
		free(*rt);
		}

	*nrata = 0;
	rno = 0;
	tn = 0;
	monirata = 2;
	for (i = 0; sizeof(Course)/sizeof(Course[0]); i++) {
		int iS = 0;
		int iV = 0;
		if (Course[i] == NULL)
			break;
		do {
			if (!Course[i]->CourseVariation[iV])
				break;
			switch (nimilaji) {
				case L'V':
					wcsncpy(rnimi, Course[i]->CourseVariation[iV++]->Name, sizeof(rnimi)/2-1);
					break;
				case L'N':
					wcsncpy(rnimi, Course[i]->Name, sizeof(rnimi)/2-1);
					break;
				case L'Y':
					wcsncpy(rnimi, Course[i]->Name, sizeof(rnimi)/2-1);
					wcsncpy(rnimi+wcslen(rnimi), Course[i]->CourseVariation[iV++]->Name, sizeof(rnimi)/2-wcslen(rnimi)-1);
					break;
				case L'S':
					wcsncpy(rnimi, Course[i]->ShortName[iS++], sizeof(rnimi)/2-1);
					break;
				}
			if (rnimi[0] == 0)
				break;
			elimwbl2(rnimi);
			upcasewstr(rnimi);
			rno++;
			} while(nimilaji != L'N');
		}
	*nrata = rno;

	if (*nrata == 0) {
		vidspwmsg(19, 0, 7, 0, L"Ohjelma ei löytänyt ratoja xml-tiedostosta");
		return(1);
		}
	if (maxnrata == 0)
		*rt = (ratatp *) calloc(*nrata, sizeof(ratatp));
	if (!*rt) {
		writeerror_w(L"Ei tilaa ratatiedoille", 0);
		goto esc;
		}
	rno = 0;
	for (i = 0; sizeof(Course)/sizeof(Course[0]); i++) {
		int iS = 0;
		int iV = -1;
		int mtk1;

		if (Course[i] == NULL)
			break;
		do {
			if (iV < 0 || nimilaji == L'V' || nimilaji == L'Y')
				iV++;
			if ((nimilaji == L'V' || nimilaji == L'Y') && !Course[i]->CourseVariation[iV])
				break;
			switch (nimilaji) {
				case L'V':
					wcsncpy(rnimi, Course[i]->CourseVariation[iV]->Name, sizeof(rnimi)/2-1);
					break;
				case L'N':
					wcsncpy(rnimi, Course[i]->Name, sizeof(rnimi)/2-1);
					break;
				case L'Y':
					wcsncpy(rnimi, Course[i]->Name, sizeof(rnimi)/2-1);
					wcsncpy(rnimi+wcslen(rnimi), Course[i]->CourseVariation[iV]->Name, sizeof(rnimi)/2-wcslen(rnimi)-1);
					rnimi[sizeof(rt[0][0].tunnus)/2-1] = 0;
					break;
				case L'S':
					wcsncpy(rnimi, Course[i]->ShortName[iS++], sizeof(rnimi)/2-1);
					break;
				}
			if (iV < 0)
				iV = 0;
			if (rnimi[0] == 0)
				break;
			elimwbl2(rnimi);
			upcasewstr(rnimi);
			srj = -1;
			memset(mtk, 0, sizeof(mtk));
				{
				unsigned int ii;
				rnimi[sizeof(rt[0][i].tunnus)/2-1] = 0;
				for (ii = 0; ii < rno; ii++)
					if (!wcscmp(rt[0][ii].tunnus, rnimi))
						break;
				if (ii < rno) {
					swprintf(msg, L"Rata %s toistuu ratatiedoissa", rnimi);
					kirj_err_file(msg, 1);
					}
				srj = rno++;
				wcscpy(rt[0][srj].tunnus, rnimi);
				}
			if (ennakko > -90)
				rt[0][srj].ennakko = ennakko;
			else
				rt[0][srj].ennakko = Course[i]->CourseVariation[iV]->ennakko;
			if (ajanottotapa)
				rt[0][srj].maalilaji = ajanottotapa;
			else
				rt[0][srj].maalilaji = Course[i]->CourseVariation[iV]->ajanottotapa;
			wcsncpy(rt[0][srj].lahto, Course[i]->CourseVariation[iV]->Start, sizeof(rt[0][srj].lahto)/2-1);
			wcsncpy(rt[0][srj].maali, Course[i]->CourseVariation[iV]->Finish, sizeof(rt[0][srj].maali)/2-1);
			rt[0][srj].viitmatka = Course[i]->CourseVariation[iV]->FinLeg;
			rt[0][srj].ratapit = Course[i]->CourseVariation[iV]->Length;
			rt[0][srj].nousu = Course[i]->CourseVariation[iV]->Climb;
			if (rt[0][srj].maalilaji == 1) {
				if (maalileimasin == 0)
					maalileimasin = -2;
				}
			else {
				if (maalileimasin != -1)
					maalileimasin = 1;
				}
			ir = 0;
			mtk1 = Course[i]->CourseVariation[iV]->StartLeg;
			for (j = 0; j < sizeof(Course[0]->CourseVariation[iV]->CourseControl)/
				sizeof(Course[0]->CourseVariation[0]->CourseControl[iV]); j++) {
				wchar_t *pCode;

				if (Course[i]->CourseVariation[iV]->CourseControl[j] == NULL ||
					Course[i]->CourseVariation[iV]->CourseControl[j]->Code[0] == 0) {
					if (i == 0 || !vapaajarj)
						break;
					tn = 10000;
					vapaajarj = 0;
					}
				else {
					pCode = Course[i]->CourseVariation[iV]->CourseControl[j]->Code;
					tn = _wtoi(pCode);
					if (tn == 0) {
						if (!wcscmp(pCode, L"V")) {
							if (!vapaajarj) {
								vapaajarj = 1;
								tn = 10001;
								}
							}
						else if (!wcscmp(pCode, L"J")) {
							if (vapaajarj) {
								vapaajarj = 0;
								tn = 10000;
								}
							}
						else if (!wmemcmp(pCode, L"RR", 2)) {
							tn = 880 + _wtoi(pCode+2);
							if (tn > 899)
								tn -= 100;
							}
						else {
							mtk1 += Course[i]->CourseVariation[iV]->CourseControl[j]->LegLength;
							}
						}
					if (tn > 0 && tn <= MAXTUNNUS) {
						mtk[ir] = mtk1 + Course[i]->CourseVariation[iV]->CourseControl[j]->LegLength;
						mtk1 = 0;
						}
					}
				if (tn > 0 && (tn <= MAXTUNNUS || tn == 10000 || tn == 10001)) {
					if (ToimintaTila == 2 && tn <= MAXTUNNUS && !on_kuvio(tn)) {
						swprintf(msg, L"Radan %s rastin %d tunnusta %d ei tiedoissa",
							rnimi, ir+1, tn);
						puute++;
						kirj_err_file(msg, 0);
						}
					if (ir >= MAXNRASTI) {
						swprintf(msg, L"Radalla %s liikaa rasteja (yli %d)",
							rnimi, MAXNRASTI);
						kirj_err_file(msg, 1);
						continue;
						}
					kdi[ir] = tn;
					ir++;
					}
				}
			if (maalikoodi && ir < MAXNRASTI-1) {
				mtk[ir] = Course[i]->CourseVariation[iV]->FinLeg;
				rt[0][srj].maali[0] = 0;
				kdi[ir++] = maalikoodi > 0 ? maalikoodi : -maalikoodi;
				}
			rt[0][srj].rastiluku = ir;
			if (ir) {
				int m = 0;
				memset(rt[0][srj].rastikoodi, 0, sizeof(rt[0][0].rastikoodi));
				if (!rt[0][srj].rastikoodi) {
					writeerror_w(L"Ei tilaa ratatiedoille", 0);
					goto esc;
					}
				for (j = 0; j < ir; j++) {
					rt[0][srj].rastikoodi[j] = kdi[j];
					m += mtk[j];
					rt[0][srj].matka[j] = m;
					}
				nr++;
				}
			} while(nimilaji != L'N');
		}
//	if (ToimintaTila < 2)
//		*nrata = nr;
	if (*rt == rata) {
		tarkkilpradat();
		swprintf(msg, L"Siirretty %d rataa", nr);
		vidspwmsg(18,5,7,0,msg);
		}
	if (ToimintaTila > 0) {
		siirraRataKohdennukset();
		}
	if (ToimintaTila == 2 && *rt == rata)
		TarkKilpailijat();
	if (puute > 0) {
		writeerror_w(L"Kaikille rasteille ei tiedoissa emit-koodia. Käytetään rastien koodeja", 0, true);
		}
	return(0);
esc:
	if (maxnrata == 0 && *rt)
		free(*rt);
	swprintf(msg,L"Virhe ratatietoja käsiteltäessa. Rata %s", rnimi);
	kirj_err_file(msg, 1);
	return(1);
}

void IOFCourseData::siirraRataKohdennukset(void)
{
	for (int srj = 0; srj < sarjaluku; srj++) {
		Sarjat[srj].rata[k_pv][0] = 0;
		for (int iA = 0; iA < sizeof(ClassAssignment)/sizeof(ClassAssignment[0]); iA++) {
			if (ClassAssignment[iA] == NULL)
				break;
			if (!wcscmp(ClassAssignment[iA]->Class, Sarjat[srj].sarjanimi)) {
				wcsncpy(Sarjat[srj].rata[k_pv], ClassAssignment[iA]->Course, sizeof(Sarjat[0].rata[0])/2-1);
				break;
				}
			}
		}
}

int haekoordix(wchar_t *tunnus)
{
	for (int i = 0; i < sizeof(rastikoord)/sizeof(rastikoord[0]); i++) {
		if (rastikoord[i].tunnus[0] == 0)
			break;
		if (!wcscmp(rastikoord[i].tunnus, tunnus))
			return(i);
		}
	return(-1);
}

int IOFCourseData::haeXmlKuviot(ekditp *kdi, rastikoordtp *koord, int nrasti)
{
	ekditp tmpkdi[2000];
	INT16 edrasti;
	int k = -1;

	for (UINT32 ip = 0; ip < sizeof(Point) / sizeof(Point[0]); ip++) {
		if (Point[ip])
			delete Point[ip];
		Point[ip] = NULL;
		}
	for (int ir = 0; ir < nrasti; ir++) {
		tmpkdi[ir].rasti = kdi[ir].rasti;
		tmpkdi[ir].koodi = kdi[ir].koodi;
		}
	qsortint(tmpkdi, nrasti, sizeof(ekditp), 0, sizeof(tmpkdi[0].rasti) == 4);

	edrasti = -1;
	if (koord) {
		for (int i = 0; i < sizeof(rastikoord)/sizeof(rastikoord[0]); i++) {
			if (koord[i].tunnus[0] == 0)
				break;
			if (wcswcind(koord[i].tunnus[0], L"SL") >= 0) {
				k++;
				Point[k] = new IOFPoint;
				Point[k]->PointType = L'S';
				wcsncpy(Point[k]->Code, koord[i].tunnus, sizeof(Point[k]->Code)/2-1);
				Point[k]->CY = koord[i].Y;
				Point[k]->CX = koord[i].X;
				Point[k]->MapY = koord[i].mapY;
				Point[k]->MapX = koord[i].mapX;
				}
			}
		}
	for (int ir = 0; ir < nrasti; ir++) {
		int j;
		if (tmpkdi[ir].rasti != edrasti) {
			edrasti = tmpkdi[ir].rasti;
			k++;
			Point[k] = new IOFPoint;
			Point[k]->PointType = L'C';
			_itow(edrasti, Point[k]->Code, 10);
			if (koord && (j = haekoordix(Point[k]->Code)) >= 0) {
				Point[k]->CY = koord[j].Y;
				Point[k]->CX = koord[j].X;
				Point[k]->MapY = koord[j].mapY;
				Point[k]->MapX = koord[j].mapX;
				}
			}
		for (j = 0; j < (int)(sizeof(Point[k]->ECodes)/sizeof(Point[k]->ECodes[0])); j++) {
			if (Point[k]->ECodes[j] == 0) {
				Point[k]->ECodes[j] = tmpkdi[ir].koodi;
				break;
				}
			}
		}
	if (koord) {
		for (int i = 0; i < (int)(sizeof(rastikoord)/sizeof(rastikoord[0])); i++) {
			if (koord[i].tunnus[0] == 0)
				break;
			if (wcswcind(koord[i].tunnus[0], L"FM") >= 0) {
				k++;
				Point[k] = new IOFPoint;
				Point[k]->PointType = L'F';
				wcsncpy(Point[k]->Code, koord[i].tunnus, sizeof(Point[k]->Code)/2-1);
				Point[k]->CY = koord[i].Y;
				Point[k]->CX = koord[i].X;
				Point[k]->MapY = koord[i].mapY;
				Point[k]->MapX = koord[i].mapX;
				}
			}
		}
	return(0);
}

int IOFCourseData::haeXmlKartta(maptp *map)
{
	MapScale = map->scale;
	MapTY = map->topY;
	MapLX = map->leftX;
	MapBY = map->btmY;
	MapRX = map->rightX;
	return(0);
}

int IOFCourseData::haeXmlRadat(ratatp *rt, int nrata)
{
	for (UINT32 ic = 0; ic < sizeof(Course) / sizeof(Course[0]); ic++) {
		if (Course[ic])
			delete Course[ic];
		Course[ic] = NULL;
		}
	for (int ir = 0; ir < nrata; ir++) {
		int len = 0;
		Course[ir] = new IOFCourse;
		Course[ir]->CourseVariation[0] = new IOFCourseVariation;
		IOFCourseVariation *CVar = Course[ir]->CourseVariation[0];
		wcsncpy(Course[ir]->Name, rt[ir].tunnus, sizeof(Course[0]->Name)/2-1);
		wcsncpy(CVar->Name, rt[ir].tunnus, sizeof(CVar->Name)/2-1);
		if (haesarja_w(rt[ir].tunnus, 0) >= 0)
			wcsncpy(Course[ir]->ShortName[0], rt[ir].tunnus, sizeof(Course[0]->ShortName[0])/2-1);
		wcsncpy(CVar->Start, rt[ir].lahto, sizeof(CVar->Start)/2-1);
		CVar->ennakko = rt[ir].ennakko;
		CVar->ajanottotapa = rt[ir].maalilaji;
//		CVar->Length = rt[ir].matka[rt[ir].rastiluku-1];
		if (rt[ir].maali[0])
			wcsncpy(CVar->Finish, rt[ir].maali, sizeof(CVar->Finish)/2-1);
		else if (rt[ir].maalilaji == 1)
			wcscpy(CVar->Finish, L"F");
//		CVar->Length += rt[ir].viitmatka;
		CVar->Length = rt[ir].ratapit;
		CVar->Climb = rt[ir].nousu;
		CVar->FinLeg = rt[ir].viitmatka;
		for (int ip = 0; ip < rt[ir].rastiluku; ip++) {
			CVar->CourseControl[ip] = new IOFCourseControl;
			switch (rt[ir].rastikoodi[ip]) {
				case 10000:
					wcscpy(CVar->CourseControl[ip]->Code, L"J");
					break;
				case 10001:
					wcscpy(CVar->CourseControl[ip]->Code, L"V");
					break;
				default:
					_itow(rt[ir].rastikoodi[ip], CVar->CourseControl[ip]->Code, 10);
					CVar->CourseControl[ip]->LegLength = rt[ir].matka[ip] - len;
					len = rt[ir].matka[ip];
					break;
				}
			}
		}
	return(0);
}

static void taydenna_leimat(void)
{
	char rkoodit[250];
	int tmpnkuvio;
	ekditp tmpkdi[2000];

	if (nkuvio > 1999)
		return;
	memset(rkoodit, 0, sizeof(rkoodit));
	for (int r = 0; r < nrata; r++) {
		for (int i = 0; i < rata[r].rastiluku; i++)
			if (rata[r].rastikoodi[i] < 250)
				rkoodit[rata[r].rastikoodi[i]] = 1;
		}
	memset(tmpkdi, 0, sizeof(tmpkdi));
	memcpy(tmpkdi, kuvio, nkuvio * sizeof(ekditp));
	tmpnkuvio = nkuvio;
	for (int i = 0; i < 250; i++) {
		int j;
		if (!rkoodit[i])
			continue;
		for (j = 0; j < nkuvio; j++)
			if (kuvio[j].rasti == i)
				break;
		if (j == nkuvio) {
			tmpkdi[tmpnkuvio].rasti = i;
			tmpkdi[tmpnkuvio].koodi = i;
			tmpnkuvio++;
			if (tmpnkuvio >= 2000)
				break;
			}
		}
	if (tmpnkuvio > nkuvio) {
		free(kuvio);
		kuvio = (ekditp *) calloc(tmpnkuvio+1, sizeof(ekditp));
		nkuvio = tmpnkuvio;
		qsortint(tmpkdi, nkuvio, sizeof(ekditp), 0, sizeof(tmpkdi[0].rasti) == 4);
		memcpy(kuvio, tmpkdi, nkuvio * sizeof(ekditp));
		}
}

int lue_radat_xml(int r)
{
	int er = 0;
	wchar_t msg[200];

   rataxmlfname[5] = k_pv + L'1';
   if ((er = lueEventXml(rataxmlfname, false, true, false)) != 0)
		return(er);
   swprintf(msg, L"Ratatiedot luetaan tiedostosta %s", rataxmlfname);
   vidspwmsg(r, 0, 7, 0, msg);
   CourseData.siirraXmlKuviot(&kuvio, &nkuvio, &maxnkuvio);
   CourseData.siirraXmlRadat(&rata, &nrata, L'V', 0, 0, -99);
   CourseData.siirraXmlKartta(&Map);
   taydenna_leimat();
   ratatiedot = 2;
   return(er);
}

#endif // EMIT

INT32 lukijakoodit[11] = {250, 240, 253, 243};

#ifdef _CONSOLE
static void editlukijakoodit(void)
{
	int i, n;
	wchar_t ch;

	clrln(ySize-3);
	n = sizeof(lukijakoodit)/sizeof(INT);
	vidspwmsg(ySize-3, 0, 7, 0, L"Lukijan koodit (+: hyväksy):");
	for (i = 0; i < n; i++) {
		vidint(ySize-3, 37+4*i, 3, lukijakoodit[i]);
		}
	for (i = 0;;) {
		inputlongw(lukijakoodit+i, 3, 31+4*i, ySize-3, L"+\x1b\r\x09\x0b\x05\x18", &ch);
		if (ch == L'+')
			break;
		switch (ch) {
			case L'\r':
			case KB_TAB:
			case DOWN:
				i = (i+1) % n;
				break;
			case BACKTAB:
			case UP:
				i = (i + n - 1) % n;
				break;
			}
		}
	clrln(ySize-3);
}

void emitLuettu(int badge)
{
}
#endif

int emittp::get(int i_ptr)
	{
	return(lueem(this, i_ptr));
	}

int emittp::getbykno(int kno, int toinen)
	{
	return(getem(this, kno, toinen));
	}

int emittp::put(int i_ptr, bool inCriticalSection)
	{
	return(putem(this, i_ptr, inCriticalSection ? 1 : 0));
	}

void emittp::nollaa(void)
{
	memset(this, 0, sizeof(emittp));
	time = TMAALI0/KSEK;
	maali = TMAALI0/KSEK;
}

bool emittp::LueSeuraava(int *kno)
{
	emitptr = em_HaeSeuraava(emitptr);
	if (emitptr < 0) {
		nollaa();
		*kno = 0;
		return(false);
		}
	get(emitptr);
	*kno = kilpno;
	return(true);
}

bool emittp::LueEdellinen(int *kno)
{
	emitptr = em_HaeEdellinen(emitptr);
	if (emitptr < 0) {
		nollaa();
		*kno = 0;
		return(false);
		}
	get(emitptr);
	*kno = kilpno;
	return(true);
}

INT onlukija(INT koodi)
	{
	unsigned int i;

	for (i = 0; i < sizeof(lukijakoodit)/sizeof(INT); i++)
		if (koodi != 0 && koodi == lukijakoodit[i])
			return(1);
	return(0);
	}

INT haebdg(INT32 badge)
   {
   if (badge <= 0)
	   return(-1);
   return(bsrchint(badge, bdg_kno, nbadge, sizeof(bdg_kno_tp), 0, 1));
   }

INT bdg2kno(INT32 badge)
   {
   INT k;

   k = haebdg(badge);
   if (k >= 0) return(bdg_kno[k].kilpno);
   else return(0);
   }

#ifdef TESTBDG
void bdgstatus(void)
   {
   int i, k;
   long bdg;

   for (i = 2; i < ySize-4; i++) {
      clrtxt(i, 0, 64);
      }
   for (i = 0; i < 95 && i < nbadge; i++) {
      vidint((i%(ySize-6)+2, 12*(i/(ySize-6)), 4, bdg_kno[i].kilpno);
      vidint((i%(ySize-6)+2, 12*(i/(ySize-6))+5, 5, bdg_kno[i].badge);
      }
   }
#endif

int getEmitJarr(int kilpno, INT16 *toinen)
	{
	int d;
	if (!emitjarr[0] || (d = getpos(kilpno)) < 1)
		return(-1);
	if (toinen == NULL)
		return(emitjarr[0][d]);
	if (*toinen < 0) {
		for (int i = 0; i < (emitjarr[1] != NULL ? 2 : 1); i++) {
			if (emitjarr[i][d] >= 0) {
				*toinen = i;
				break;
				}
			}
		}
	if (*toinen >= 0 && emitjarr[*toinen])
		return(emitjarr[*toinen][d]);
	return(-1);
	}

void setEmitJarr(int empos, int kilpno, int toinen)
	{
	int d;
	if (!emitjarr[toinen] || (d = getpos(kilpno)) < 1)
		return;
	emitjarr[toinen][d] = empos;
	}

static void autoraptarkistus(INT32 badge, INT kno)
	{
	emittp em;

   if (emittulparam) {
		if (getem(&em, kno, 0) >= 0 && (autorap > 1 || tarkista(&em, 0, 0, 0)))
			emit_rap(emittulparam, kno);
		}
	}

INT addbadge(INT32 badge, INT kno, INT kysyfl)
   {
   INT i;
   INT16 ibdg = 0;
   kilptietue kilp;
   wchar_t msg[50];
   emittp em;

   if (!badge || !kno)
		return(1);
   if ((i = getpos(kno)) <= 0)
		return(1);
   kilp.GETREC(i);
   if (kilp.tark(k_pv) == L'P')
		return(1);
   i = haebdg(badge);
   if (loki && emitloki) {
	  swprintf(msg, L"Lisää e-koodin %ld kilpailijalle %d",
		 badge, kno);
	  wkirjloki(msg);
	  }
   msg[0] = 0;
	if (i >= 0) {
		if (bdg_kno[i].kilpno == kno)
			return(0);
		for (unsigned int ivap = 0; ivap < sizeof(vapautettu_bdg)/sizeof(vapautettu_bdg[0]); ivap++)
			if (vapautettu_bdg[ivap].badge == badge && vapautettu_bdg[ivap].kilpno == kno)
				return(0);
		swprintf(msg, L"Kilpailijoilla %d ja %d sama e-koodi %d",
			bdg_kno[i].kilpno, kno, badge);
		wkirjloki(msg);
		}
	else {
		i = -i - 1;
		memmove((char *)(bdg_kno+i+1), (char *)(bdg_kno+i),
		(nbadge-i) * sizeof(bdg_kno_tp));
		nbadge++;
		bdg_kno[i].kilpno = kno;
		bdg_kno[i].badge = badge;
		}
#ifdef TESTBDG
   bdgstatus();
#endif
   if (msg[0]) {
	  if (loki && emitloki) {
		 wkirjloki(msg);
		 }
	  if (kysyfl)
			writeerror_w(msg, 0, true);
	  return(1);
	  }
	if (badge == vakvapautus_bdg && comautorap && !kysyfl)
		autoraptarkistus(badge, kno);
	if (kilpparam.kaksibadge == 2 && badge == kilp.pv[k_pv].badge[1])
		ibdg = 1;
	if (ibdg == 0 && getEmitJarr(kno, &ibdg)) {
		getem(&em, kno, 0);
		laheta_emva(&em);
		}
   return(0);
   }

INT rembadge(INT32 badge, INT kno, INT kysyfl)
   {
   INT d, kbdg = 0, k;
   INT16 ibdg = 0;
   kilptietue kilp;
   wchar_t msg[50];

   if ((d = getpos(kno)) <= 0)
		return(1);
   kilp.GETREC(d);
   if (loki && emitloki && kilp.tark(k_pv) != L'P') {
	  swprintf(msg, L"Poistaa ekoodin %ld kilpailijalta %d",
		 badge, kno);
	  wkirjloki(msg);
	  }
   for (int k = 0; k < (kilpparam.kaksibadge ? 2 : 1); k++) {
	   if (kilp.pv[k_pv].badge[k] == badge && (!kilpparam.kaksibadge || kilp.pv[k_pv].badge[1-k] != badge)) {
		  kbdg = haebdg(badge);
		  if ((nbadge && (kbdg < 0 || kbdg >= nbadge)) || (!nbadge && kbdg <= 0)) {
			 if (loki && emitloki && kilp.tark(k_pv) != L'P') {
				swprintf(msg, L"E-koodia %ld ei indeksissä", badge);
				wkirjloki(msg);
				}
			 return(1);
			 }
		  ibdg = k;
		  break;
		  }
		}
	if (kbdg < 0)
		return(0);
	if (kilp.pv[k_pv].vac == 'V')
		vakvapautus_bdg = kilp.pv[k_pv].badge[0];
   k = bdg_kno[kbdg].kilpno;
   if (k != kno) {
	  if (loki && emitloki) {
		 swprintf(msg, L"E-koodi %ld liitetty kilpailijaan %d, ei poistettu",
			badge, kilp.id());
		 wkirjloki(msg);
		 }
#ifdef TESTBDG
	  bdgstatus();
#endif
	  return(0);
	  }
	if (ibdg == 0 && getEmitJarr(kno, &ibdg))
		laheta_emva_pois(kno);
	  memmove(vapautettu_bdg+1, vapautettu_bdg, sizeof(vapautettu_bdg)-sizeof(vapautettu_bdg[0]));
	  vapautettu_bdg[0].badge = badge;
	  vapautettu_bdg[0].kilpno = kno;
	  nbadge--;
	  memmove((char *)(bdg_kno+kbdg), (char *)(bdg_kno+kbdg+1),
		 (nbadge-kbdg) * sizeof(bdg_kno_tp));
	  bdg_kno[nbadge].badge = 0;
	  bdg_kno[nbadge].kilpno = 0;
#ifdef TESTBDG
   bdgstatus();
#endif
   return(0);
   }

void vaihda_badge(UINT32 badge)
   {
   INT d, kno, kbg, vbdg;
//   static UINT32 seurbdg = 1;
   kilptietue kilp;
#ifdef EMIT
   INT32 ep;
   emittp em;
#endif
	INT kk = 0;

   kno = bdg2kno(badge);
   if ((d = getpos(kno)) <= 0) return;
   EnterCriticalSection(&tall_CriticalSection);
   kilp.GETREC(d);
	if (kilp.pv[k_pv].badge[0] == (int)badge)
		rembadge(badge, kno, 0);
	if (kilpparam.kaksibadge && kilp.pv[k_pv].badge[1] == (int)badge) {
		rembadge(badge, kno, 0);
		kk = 1;
		}
   vbdg = haeSeurVapaaBadge(badge);
   for (kbg = k_pv; kbg < kilpparam.n_pv_akt; kbg++)
	   kilp.pv[kbg].badge[kk] = vbdg;
   kilp.PUTREC(d, 1);
   if (comfl)
	  laheta(d, kilp.kilpno, -1-k_pv , 0, 0, 0, 0);
   addbadge(vbdg, kno, 1);
   LeaveCriticalSection(&tall_CriticalSection);
#if defined(EMIT)
   ep = getem(&em, kno, 0);
   em.badge = vbdg;
   putem(&em, ep, 0);
#endif
//   if (comfl)
//      laheta(d, kno, 0, 0, 0, 0, 0);
   }

INT add(void *em)
	{
	INT  i, exfl = 0;
	INT  dataf;
	kilptietue kilp;

	do {
#ifdef _CONSOLE
		if (!em) header=L"LISÄYS    ";
		clrscr();
		kehys(1);
		outform();
#endif
		kilp.nollaa();
		for (i = 0; i < kilpparam.n_pv_akt; i++) {
			kilp.set_tark(i < k_pv ? L'P' : L'-', i);
			wmemset(kilp.pv[k_pv].asakot, L'-', sizeof(kilp.pv[k_pv].asakot)/2 - 1);
			}
		kilp.setSarja(-1);
		kilp.setId(haeseurvapaa(seurvapaa));
		seurvapaa = kilp.id();
		if (em) {
			kilp.pv[k_pv].badge[0] = ((emittp *)em)->badge;
			kilp.setSarjaAll(etsiesarja((emittp *)em));
			if (kilp.Sarja() < 0) {
				kilp.setSarjaAll(0);
				}
			kilp.outkilp(0);
			}
#ifdef _CONSOLE
		if (em && luesuoraan) {
			_ltow(((emittp *)em)->badge, kilp.sukunimi, 10);
			if (kilp.Sarja() < 0) {
				kilp.setSarjaAll(0);
				}
			}
		else
			exfl = kilp.inputkilp(1);
		kilp.setSarjaAll(kilp.Sarja());
		if( ! exfl ) {
			if (nrec == maxrec-1)
				writeerror_w(L"Kilpailijataulukko täyttyi. "
					L"Kilpailijamäärää ei voi enää lisätä",3000);
			if (nrec >= maxrec) {
				writeerror_w(
					L"Kilpailijataulukko täynnä - lisäystä ei tallennettu", 0);
#else
			{
			_ltow(((emittp *)em)->badge, kilp.etunimi, 10);
			if (kilp.Sarja() < 0) {
				kilp.setSarjaAll(0);
				}
			if (nrec == maxrec-1)
				vidspwmsg(0,0,0,0,L"Kilpailijataulukko täyttyi. "
					L"Kilpailijamäärää ei voi enää lisätä");
			if (nrec >= maxrec) {
				vidspwmsg(0,0,0,0,
					L"Kilpailijataulukko täynnä - lisäystä ei tallennettu");
#endif
				exfl = 1;
				dataf = 0;
				break;
				}
			if (!kilpparam.bibvaihtelee) {
				kilp.kilpno = kilp.id();
				for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++)
					kilp.pv[ipv].bib = kilp.kilpno;
				}
			EnterCriticalSection(&tall_CriticalSection);
			kilp.addtall(&dataf, 0);
			LeaveCriticalSection(&tall_CriticalSection);
			seurvapaa = kilp.id() + 1;
			}
		} while (!em && !exfl);
	if (exfl)
		dataf = 0;
	return(dataf);
	}

#if defined(EMIT) && defined(TALLEMIT)
INT putem(emittp *em, INT32 ep, int inCrSe)
   {
   int retval = 0;

   if (ep >= 0 && ep <= emithead) {
	  if (!inCrSe)
		 EnterCriticalSection(&emit_CriticalSection);
	  if (_lseek(emitfile, ep * sizeof(emittp), SEEK_SET) < 0) {
		 iowerr(emitfname, 1, 0L);
         retval = 1;
		 }
      else {
		 if (_write(emitfile, em, sizeof(emittp)) < sizeof(emittp)) {
			iowerr(emitfname, 3, ep);
			retval = 1;
			}
		 else {
			if (ep == emitptr)
				emitptr_kno = em->kilpno;
			}
		 }
	  if (!inCrSe)
		 LeaveCriticalSection(&emit_CriticalSection);
	  }
   return(retval);
   }

INT on_emit(emittp *em, int poista)
   {
   static INT32 *emitvaraus;
   static INT nemitvar;
   INT32 kdi, pos;

   if (poista) {
	   if (emitvaraus) {
		   free(emitvaraus);
		   emitvaraus = 0;
		   }
	   return(0);
	   }
   if(!emitvaraus)
	  emitvaraus = (INT32 *)calloc(maxrec+1000, sizeof(INT32));
   if (!emitvaraus)
	  return(0);
   kdi = 4000*em->time + em->badge;
   pos = bsrchint(kdi, emitvaraus, nemitvar, 4, 0, 1);
   if (pos >= 0)
      return(1);
   pos = -pos - 1;
   if (nemitvar < maxrec+1000) {
      memmove(emitvaraus+pos+1, emitvaraus+pos, (nemitvar-pos)*sizeof(INT32));
	  emitvaraus[pos] = kdi;
	  nemitvar++;
      }
   return(0);
   }

INT addem(emittp *em, INT kielto)
   {
   int dh, res = 0;

   EnterCriticalSection(&emit_CriticalSection);
   if (estaemittoisto && on_emit(em, 0))
      res = 1;
   else {
      if ((res = putem(em, emithead, 1)) == 0) {
		 emithead++;
       if (kirjheti & 4)
		  fflush(femitfile);
         if (em->kirjaus != TMAALI0/KSEK) {
			emithead2 = emithead;
			laheta_em(em, 1, kielto);
            if (emitanal_fl)
			   addemitanal(em, 1);
			}
		 }
	  }
   LeaveCriticalSection(&emit_CriticalSection);
   return(res);
   }

INT lueemitfile(void)
   {
   emittp em;
   INT kysy = 0;
   wchar_t ch, msg[81];

   if (!emitfl) return(1);
   if ((emitfile = open(emitfname, O_RDWR | O_CREAT | O_BINARY,
      S_IREAD | S_IWRITE)) < 0) {
	  swprintf(msg, L"Tiedoston %s avaaminen tai luominen ei onnistu",
         emitfname);
	  writeerror_w(msg, 0);
      return(1);
	  }
   femitfile = _wfdopen(emitfile, L"r+b");
   setvbuf(femitfile, NULL, _IONBF, 0);
   emithead = (INT) (filelength(emitfile) / sizeof(emittp));
   emithead2 = emithead;
   return(0);
   }
#endif

static int emitkdit;
static wchar_t kdimask[18] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};
static wchar_t *lainaemit;
INT lainatied;
static INT32 maxlaina;

void merk_ekdi(wchar_t *koodit, long ekdi)
   {
   if (ekdi <= 0 || ekdi > maxlaina)
	  return;
   koodit[(ekdi-1)/16] |= kdimask[(ekdi-1)%16];
   }

int on_ekdi(wchar_t *koodit, long ekdi)
   {
   if (ekdi <= 0 || ekdi > maxlaina)
	  return(0);
   return((koodit[(ekdi-1)/16] & kdimask[(ekdi-1)%16]) != 0);
   }

wchar_t lainafname[200] = L"LAINAEMT.LST";

void lue_lainat(int poista, int varoita)
   {
   wchar_t lainafname2[] = L"LAINAEMIT.LST";
   TextFl * ekdifile;
   wchar_t Buf[200];
   INT32 ekdi;

   if (poista) {
	   if (lainaemit) {
		   free(lainaemit);
		   lainaemit = 0;
		   }
	   return;
	   }
   ekdifile = new TextFl(lainafname, L"rt");
   if (!ekdifile->IsOpen()) {
	   delete ekdifile;
	   ekdifile = new TextFl(lainafname2, L"rt");
	   }
   if (ekdifile->IsOpen()) {
	  while (!ekdifile->Feof()) {
		ekdifile->ReadLine(Buf, 200);
		 swscanf(Buf , L"%ld\n", &ekdi);
		 if (ekdi > maxlaina)
			maxlaina = ekdi;
		 }
	  ekdifile->Rewind();
	  if ((lainaemit = (wchar_t *) calloc(maxlaina/16+1, 2)) != NULL) {
		  while (!ekdifile->Feof()) {
			ekdifile->ReadLine(Buf, 200);
			swscanf(Buf, L"%ld\n", &ekdi);
			merk_ekdi(lainaemit, ekdi);
			}
		 }
	  else {
		 writeerror_w(L"Muisti ei riita lainaemitluettelolle", 0);
		 lainatied = 0;
		 }
	  }
   else {
	  if (varoita)
		  writeerror_w(L"Lainaemitluettelon LAINAEMT.LST avaaminen ei onnistu", 0);
	  lainatied = 0;
	  }
  delete ekdifile;
   }

bool on_lainakortti(int badge)
{
	return(on_ekdi(lainaemit, badge) != 0);
}

INT lainakortti(kilptietue *kilp)
   {
   INT laina = 0;

   if (lainatied < 2 && kilp->pv[k_pv].laina[0] == L'L') laina = 1;
   if (!laina && lainatied) {
	  laina = on_ekdi(lainaemit, kilp->pv[k_pv].badge[0]) ||
		  on_ekdi(lainaemit, kilp->pv[k_pv].badge[1]);
	  }
   return(laina);
   }

void lueemitkoodit(void)
   {
   TextFl *emitkdfile;
   wchar_t fname2[] = L"TUNNKDI.LST";
   wchar_t ch, fname[] = L"EMITKDI.LST", msg[80];
	wchar_t Buf[200];
   INT d, n = 0, nmuut = 0, merklaina;
   INT32 nro;
   INT32 kdi;
   struct strnrot {
	  INT32 lis;
	  INT d;
	  } *nrot = NULL;
   kilptietue kilp, entkilp;

   emitkdfile = new TextFl(fname, L"r");
   if (!emitkdfile->IsOpen()) {
	   delete emitkdfile;
	   emitkdfile = new TextFl(fname2, L"r");
	   }
   if (!emitkdfile->IsOpen()) {
	  delete emitkdfile;
	  writeerror_w(L"Tiedostoa EMITKDI.LST tai TUNNKDI.LST ei löydy", 0, true);
	  return;
	  }
   ch = L' ';
   wselectopt(L"Tunnistus K)ilpailunumerosta vai L)isenssinumerosta", L"KL", &ch);
   if (ch == L'L') {
	  nrot = (struct strnrot *) calloc(nrec, sizeof(INT)+sizeof(INT32));
	  if (!nrot) {
		 writeerror_w(L"Muisti ei riitä", 0);
		 return;
		 }
	  for (d = 1; d < nrec; d++) {
		 kilp.GETREC(d);
		 if (!kilp.kilpstatus && kilp.lisno[0]) {
			nrot[n].lis = kilp.lisno[0];
			nrot[n].d = d;
			n++;
			}
		 }
	  qsortint(nrot, n, sizeof(INT32)+sizeof(INT), 0, 1);
	  }
   ch = L' ';
   wselectopt(L"Merkitäänkö luetut koodit lainakorteiksi (K/E)", L"KE", &ch);
   merklaina = ch == L'K';
   for (;;) {
	  if (!emitkdfile->ReadLine(Buf, 200))
		break;
	  if (swscanf(Buf, L"%ld %ld\n", &nro, &kdi) < 2)
		  break;
	  if (ch == L'L') {
		 if ((d = bsrchint(nro, nrot, n, 6, 0, 1)) >= 0)
			d = nrot[d].d;
			}
	  else if (nro == 0) {
			if ((nro = bdg2kno(kdi)) == 0)
				d = -1;
			else {
				d = getpos((INT) nro);
				kdi = 0;
				}
			}
	  else
		 d = getpos((INT) nro);
	  if (d > 0) {
		 EnterCriticalSection(&tall_CriticalSection);
		 kilp.GETREC(d);
		 entkilp = kilp;
		 for (int k = k_pv; k < kilpparam.n_pv_akt; k++) {
			kilp.pv[k].badge[0] = kdi;
			kilp.pv[k].laina[0] = merklaina ? L'L' : 0;
			}
		 kilp.tallenna(d, 0, 0, 0, &entkilp);
		 LeaveCriticalSection(&tall_CriticalSection);
		 nmuut++;
		 }
	  }
   swprintf(msg, L"%d koodia tallennettu", nmuut);
   writeerror_w(msg, 0, true);
   delete emitkdfile;
   if (nrot) free(nrot);
   }

#if defined(EMIT)
void aikaruutu(void)
   {
   INT ptime;
   aikatp pt;
   INT fore, r;
   wchar_t st[15], st2[40];

   if (!ajat_on || !aikajono[0] || status_on || !seurkrivi) return;
   draw_grchar2(7, 65, 11);
   for (r = 0; r < 16; r++) {
	  wmemset(st, L' ', 14);
	  fore = (r == 8) ? 0 : 7;
	  if (seurkrivi + r >= 8) {
		 ptime = seurkrivi - 8 + r;
		 if (aikajono[0]->aktrows - ptime > 0)
			 aikajono[0]->getAktTime(&pt, ptime);
			swprintf(st, L"%8.8s %4d ", AIKATOWSTRS(st2, pt.t, t0), pt.kno);
		 }
	  viwrrectw(r, 66, r, 79, st, fore, 7-fore, 0);
	  }
   }

INT lueem(emittp *em, INT ep)
   {
   if (ep < 0 || ep >= emithead || !ProsEmitOn) {
	  em->nollaa();
	  return(1);
	  }
   else {
	  EnterCriticalSection(&emit_CriticalSection);
	  _lseek(emitfile, (INT32) ep * sizeof(emittp), SEEK_SET);
	  _read(emitfile, em, sizeof(emittp));
	  LeaveCriticalSection(&emit_CriticalSection);
	  }
   return(0);
   }

INT getem(emittp *em, INT kilpno, INT16 toinen)
   {
   INT ep;

   em->nollaa();
   if (kilpno < minkilpno || kilpno > maxkilpno)
		return(-1);
   ep = getEmitJarr(kilpno, &toinen);
   if (ep >= 0)
		lueem(em, ep);
   return(ep);
   }

INT putem(emittp *em, INT32 ep, int inCrSe)
   {
   INT retval = 0;

   if (ep >= 0 && ep <= emithead) {
      if (!inCrSe)
         EnterCriticalSection(&emit_CriticalSection);
	  if (_lseek(emitfile, ep * sizeof(emittp), SEEK_SET) < 0) {
		 iowerr(emitfname, 1, 0L);
         retval = 1;
         }
	  else {
		 if (_write(emitfile, em, sizeof(emittp)) < (int)sizeof(emittp)) {
			iowerr(emitfname, 3, ep);
			retval = 1;
			}
		 else {
			if (ep == emitptr)
				emitptr_kno = em->kilpno;
			}
		 }
	  if (!inCrSe)
         LeaveCriticalSection(&emit_CriticalSection);
      }
   return(retval);
   }

INT on_emit(emittp *em, int poista)
   {
   static INT32 *emitvaraus;
   static INT nemitvar;
   INT32 kdi, pos;

   if (poista) {
	   if (emitvaraus) {
		   free(emitvaraus);
		   emitvaraus = 0;
		   }
	   return(0);
	   }
   if(!emitvaraus)
      emitvaraus = (INT32 *)calloc(maxrec+1000, sizeof(INT32));
   if (!emitvaraus)
	  return(0);
   kdi = 4000*em->time + em->badge;
   pos = bsrchint(kdi, emitvaraus, nemitvar, 4, 0, 1);
   if (pos >= 0)
      return(1);
   pos = -pos - 1;
   if (nemitvar < maxrec+1000) {
      memmove(emitvaraus+pos+1, emitvaraus+pos, (nemitvar-pos)*sizeof(INT32));
	  emitvaraus[pos] = kdi;
      nemitvar++;
      }
   return(0);
   }

INT addem(emittp *em, INT kielto)
   {
   int res = 0;

#ifdef TUNTURIxxx // Onko tarpeen?
   if (em->osuus)
	  return(1);
#endif
   EnterCriticalSection(&emit_CriticalSection);
   if (estaemittoisto && on_emit(em, 0))
	  res = 1;
   else {
	  if (!putem(em, emithead, 1)) {
		 if (em->kilpno && em->badge) {
			setEmitJarr(emithead, em->kilpno, em->osuus);
			}
		 emithead++;
		 if (!luesuoraan) {
			if (kirjheti & 4)
			   fflush(femitfile);
			}
		 if (KSEK*em->kirjaus != TMAALI0) {
			if (emithead2 == emithead-1)
			   emithead2 = emithead;
			laheta_em(em, 1, kielto);
			if (kielto && comautorap && em->kilpno) {
			   if (emittulparam && (autorap > 1 || tarkista(em, 0, 0, 0)))
				  emit_rap(emittulparam, em->kilpno);
				}
			if (emitanal_fl)
			   addemitanal(em, 1);
			}
		 }
	  }
   LeaveCriticalSection(&emit_CriticalSection);
   return(res);
   }

void haeEmiterot(void)
{
	kilptietue kilp;
	emittp Em;
	long viim;

	if (EmitErot)
		delete[] EmitErot;
	EnterCriticalSection(&emit_CriticalSection);
	nEmitErot = emithead2;
	EmitErot = new emiterotp[nEmitErot+1];
	memset(EmitErot, 0, (nEmitErot+1) * sizeof(EmitErot[0]));
	nEmitErot = 0;
	for (int ep = 0; ep < emithead2; ep++) {
		lueem(&Em, ep);
		if (Em.badge && Em.kilpno && getpos(Em.kilpno) > 0) {
			kilp.GETREC(getpos(Em.kilpno));
			if ((viim = e_maaliaika(&Em, &kilp, 0)) != TMAALI0 && kilp.maali(0) != TMAALI0) {
				viim = ((kilp.maali(0) - viim)/SEK + 36*3600L)%86400L - 43200L;
				EmitErot[nEmitErot].ero = viim;
				EmitErot[nEmitErot].kilpno = Em.kilpno;
				EmitErot[nEmitErot].pos = ep;
				EmitErot[nEmitErot].kh = kilp.tark();
				viim = viim+24*3600+48*3600*(!kilp.hyv());
				lswab(&viim);
				* ((long *)EmitErot[nEmitErot].key) = viim;
				viim = Em.kilpno;
				lswab(&viim);
				* ((long *)(EmitErot[nEmitErot].key+4)) = viim;
				nEmitErot++;
				}
			}
		}
	LeaveCriticalSection(&emit_CriticalSection);
	quicksort((char *)EmitErot, nEmitErot, 8, sizeof(EmitErot[0]));
}

#ifdef EBA
INT eba_ajat(kilptietue *kilp, emittp *em, int uusinta)
   {
   INT i, j, tlkja = 0, n, kdi;
   INT loppu[10];
   INT tulkinta[MAXNLEIMA];
   ratatp *rt;

   memset(loppu, 0, sizeof(loppu));
   n = Sarjat[kilp->Sarja()].valuku[k_pv];
   rt = haerata(kilp);
   if (!rt)
      return(n);
   tarkista(em, tulkinta, 0, 0);
   for (i = MAXNLEIMA-1; i > 0; i--) {
      if (!em->ctrlcode[i] || tulkinta[i] == 0)
		 continue;
      if (tulkinta[i] == rt->rastiluku+1) {
         tlkja = em->ctrltime[i];
		 continue;
         }
      if (tlkja > 0 && i > 0 && !em->ctrltime[i])
         tlkja = -1;
      j = tulkinta[i];
	  if (n > 1 && !em->ctrltime[i] && em->ctrltime[i] != 11)
         continue;
      if (j == rt->rastiluku && tlkja > 0 && KSEK*em->time != TMAALI0)
		 em->maali = em->time - 10L*(tlkja - em->ctrltime[i]);
      if (!j)
         continue;
	  if (j < 0)
         j = -j;
      kdi = rt->koodi[j-1];
      if ((j = kdi - 21) >= 0 && j < 10 && !loppu[j])
         loppu[j] = em->ctrltime[i];
		if ((j = kdi - 11) >= 0 && j < 10) {
			if (loppu[j] > em->ctrltime[i] && (uusinta || !kilp->pv[0].va[Sarjat[j].valuku].vatulos)) {
			kilp->pv[0].va[Sarjat[j].valuku].vatulos = SEK*(loppu[j] - (INT32)em->ctrltime[i]);
			n--;
				}
//         if (tlkja)
//            kilp->valahto[0][j] = em.time - SEK*(tlkja - (INT32)em.ctrltime[j]);
		 }
	  }
   kilp->tall_tulos(0, tulos1(0, 0));
   return(n);
	}
#endif

#ifdef _CONSOLE
static void editkoodit(void)
   {
   TextFl *leima_file;
   wchar_t wch = L' ', Buf[200];
   INT i, j;
   INT16 rasti = 0, kdi[RINNTUNNUS];

   wselectopt(L"L)ue ratatiedot ja leimasinkoodit uudelleen, M)uokkaa koodeja", L"LM\x1b", &wch);
   if (wch == ESC)
	   return;
   if (wch == L'L') {
	  lueRatatiedotUudelleen();
	  return;
	  }
   do {
	  clrln(ySize-3);
	  vidspwmsg(ySize-3,0,7,0,L"Anna rastin tunnus:        999: lukija, Esc: poistu");
	  inputintw(&rasti, 3, 20, ySize-3, L"\r\x1b", &wch);
	  if (wch == ESC) break;
	  if (rasti == 999) {
			editlukijakoodit();
			continue;
			}
	  if (rasti <= 0 || rasti > MAXTUNNUS) {
		 writeerror_w(L"Tunnus liian suuri tai pieni", 0, true);
		 continue;
		 }
	  for (i = 0; i < nkuvio; i++)
		 if (kuvio[i].rasti == rasti) break;
	  if (i >= nkuvio) {
		 writeerror_w(L"Tunnusta ei esiinny ratatiedoissa", 0, true);
		 continue;
		 }
	  vidspwmsg(ySize-3, 27, 7, 0, L"Emit-koodit  ");
	  clrln(ySize-1);
	  vidspwmsg(ySize-1, 0, 7, 0, L"Emit-koodien arvot: 1 kaikki kelpaavat, 2 leimausta ei vaadita");
	  for (j = 0; j < RINNTUNNUS; j++, i++) {
		 if (i < nkuvio && kuvio[i].rasti == rasti)
			kdi[j] = kuvio[i].koodi;
		 else
			kdi[j] = 0;
		 vidint(ySize-3, 44 + 5*j, 3, kdi[j]);
		 }
	  for (j = 0; j < RINNTUNNUS; j++) {
		 inputintw(&kdi[j], 3, 44+5*j, ySize-3, L"\r", &wch);
		 }
	  wch = L' ';
	  wselectopt(L"Vahvista muutokset (K/E), Esc: poistu vahvistamatta", L"KE\x1b", &wch);
	  if (wch == L'K') {
		 i -= RINNTUNNUS;
		 qsortint(kdi, RINNTUNNUS, sizeof(kdi[0]), 0, sizeof(kdi[0])==4);
		 for (j = 0; j < RINNTUNNUS; j++) {
			if (!kdi[j]) continue;
			if (i < nkuvio && kuvio[i].rasti != rasti) {
			   memmove(kuvio+i+1, kuvio+i, (nkuvio-i) * sizeof(ekditp));
			   nkuvio++;
			   }
			if (i == nkuvio)
			   nkuvio++;
            if (nkuvio > maxnkuvio) {
               nkuvio = maxnkuvio;
			   writeerror_w(L"Kooditaulu täynnä. Lisääminen ei onnistu", 0);
               continue;
			   }
			kuvio[i].rasti = rasti;
			kuvio[i].koodi = kdi[j];
			i++;
			}
		 while (i < nkuvio && kuvio[i].rasti == rasti) {
			memmove(kuvio+i, kuvio+i+1, (nkuvio-i-1) * sizeof(ekditp));
			nkuvio--;
			memset(kuvio+nkuvio, 0, sizeof(ekditp));
			}
		 wch = L' ';
		 wselectopt(L"Kirjoitetaanko muutetut tiedot levylle (K/E)", L"KE", &wch);
		 if (wch == L'K') {
			leima_file = new TextFl(leimafname,L"w");
			if (!leima_file->IsOpen()) {
				swprintf(Buf, L"Tiedoston %s avaaminen ei onnistu", leimafname);
			   writeerror_w(Buf, 0, true);
			   }
			else {
			   for (i = 0; i < nkuvio; i++) {
				  swprintf(Buf, L"%d  %d\n",
					 kuvio[i].rasti, (int) kuvio[i].koodi);
				  leima_file->WriteLine(Buf);
				  }
			   }
			delete leima_file;
			}
		 }
	  } while (wch != ESC);
   clrln(ySize-1);
   }
#endif

// Funktio on_kuvio

static INT on_kuvio(INT rasti)
   {
   INT i;

   for (i = 0; i < nkuvio; i++) {
	  if (kuvio[i].rasti >= rasti) break;
	  }
   if (i < nkuvio && kuvio[i].rasti == rasti) return(1);
   return(0);
   }

static INT oikeakoodi(ratatp *rt, INT rasti, INT kdi, INT vapaajarj)
   {
   INT i;

   if (!kdi || rasti >= rt->rastiluku) return(0);
   for (;;) {
	  for (i = 0; i < nkuvio; i++) {
		 if (kuvio[i].rasti > rt->rastikoodi[rasti]) break;
		 if (kuvio[i].rasti < rt->rastikoodi[rasti]) continue;
		 if (kuvio[i].koodi && kuvio[i].koodi <= 2) return(kuvio[i].koodi);
		 if (kdi == kuvio[i].koodi)
            return(1);
         }
	  if (!vapaajarj || !rasti || rt->rastikoodi[rasti-1] == 10000 || rt->rastikoodi[rasti-1] == 10001) {
         break;
         }
      rasti--;
      }
   return(0);
   }

int onrata(wchar_t *tunnus)
{
   ratatp *rt;

   if (rata) {
	  for (rt = rata; rt < rata+nrata; rt++)
		 if (!wcscmp(rt->tunnus, tunnus))
			return(rt-rata+1);
	  }
   return(0);
}

int haeradansarja(wchar_t *tunnus, int *nsarja)
{
	int srj1 = -1;

	if (nsarja)
		*nsarja = 0;
	for (int srj = 0; srj < sarjaluku; srj++) {
		ratatp *rt;
		if ((rt = haerata_sarja(srj, k_pv)) != NULL && !wcscmp(tunnus, rt->tunnus)) {
			if (nsarja)
				++*nsarja;
			if (srj1 < 0)
				srj1 = srj;
			}
		}
	return(srj1);
}

ratatp *haerata_sarja(int srj, int i_pv)
{
	wchar_t tn[60];

	wcsncpy(tn, Sarjat[srj].rata[i_pv], 59);
	for (int i = 0; i < 2; i++) {
		for (int k = 0; k < nrata; k++)
			if (!wcscmp(tn, rata[k].tunnus))
				return(rata+k);
		if (i == 0)
			wcsncpy(tn, Sarjat[srj].sarjanimi, 59);
		}
	return(NULL);
}

ratatp *haerata(kilptietue *kilp)
   {
   ratatp *rt = NULL;
   wchar_t rkoodi[60];

   if (rata) {
	  for (int i = 0; i < 3; i++) {
		  switch (i) {
			 case 0:
				 wcsncpy(rkoodi, Sarjat[kilp->Sarja()].rata[k_pv], 59);
				 break;
			 case 1:
				 wcsncpy(rkoodi, Sarjat[kilp->Sarja()].sarjanimi, 59);
				 break;
			 case 2:
				 wcsncpy(rkoodi, kilp->pv[k_pv].rata, 59);
				 break;
			 }
		  if (!rkoodi[0])
			  continue;
		 for (rt = rata; rt < rata+nrata; rt++)
			if (!wcscmp(rt->tunnus, rkoodi))
				break;
		 if (rt == rata+nrata)
			  rt = NULL;
		  if (rt)
			  break;
		  }
	  }
   return(rt);
   }

int haeratano(kilptietue *kilp)
   {
   ratatp *rt = NULL;

   rt = haerata(kilp);
   if (rt)
		return(rt-rata);
   return(-1);
   }

INT haelukija(emittp *em)
   {
   int i;

   if (!em->ctrlcode[0]) {
      for (i = MAXNLEIMA-1; i >= 0; i--)
		 if (em->ctrlcode[i] && em->ctrlcode[i] != 254)
            break;
      return(i);
      }
   for (i = MAXNLEIMA-1; i >= 0; i--) {
      if (em->ctrlcode[i] == 254)
         return((i+MAXNLEIMA-1)%MAXNLEIMA);
      }
   for (i = 0; i < MAXNLEIMA-1; i++) {
      if (em->ctrltime[i+1] < em->ctrltime[i] && onlukija(em->ctrlcode[i]))
		 return(i);
      if (em->ctrltime[(i+2)%MAXNLEIMA] < em->ctrltime[i] &&
         onlukija(em->ctrlcode[i]) && !onlukija(em->ctrlcode[i+1]))
		 return(i);
      }
   for (i = 0; i <= MAXNLEIMA-1; i++) {
	  if (onlukija(em->ctrlcode[i]) &&
		 em->ctrlcode[(i+1)%MAXNLEIMA] != em->ctrlcode[i] && em->ctrlcode[(i+2)%MAXNLEIMA] != em->ctrlcode[i])
		 break;
	  }
   return(i);
   }


INT tarkista(emittp *em, INT *tulkinta, INT haku, kilptietue *kilp1)
   {
   ratatp *rt = 0;
   INT i, j, k, m, kk = 999, kk2, oikeat[MAXNRASTI], nollattu = 0, vapaajarj = 0, lukija;
   kilptietue kilp;

   if (!haku) {
	  if (tulkinta) memset(tulkinta, 0, MAXNLEIMA*sizeof(INT));
	  if (esitark || esiluenta == 1)
		 return(0);
	  if (kilp1) {
		  kilp = *kilp1;
		  if (em->badge == 0 || (em->badge != kilp.Badge(k_pv, 0) && em->badge != kilp.Badge(k_pv, 1)) || em->kilpno != kilp.id())
			  return(-1);
		  }
	  else {
		  if ((i = getpos(em->kilpno)) <= 0) return(-1);
		  kilp.GETREC(i);
		  }
	  rt = haerata(&kilp);
	  }
   else {
	  // Tätä käytetään funktiossa etsiesarja(em)

	  if (tulkinta && *tulkinta >= 0 && *tulkinta < nrata) {
		 rt = rata + *tulkinta;
		 tulkinta = 0;
		 }
	  }
   if (!rt || !rt->rastiluku) return(-1);

   // Kortille luetut koodit käydään läpi lopusta alkaen.
   // Aluksi ohitetaan nolla-koodit.
//   j = MAXNLEIMA-1;
//   while (!em->ctrlcode[j] && j) j--;
//   if (!j) return(rt->rastiluku);

   lukija = haelukija(em);
   if (lukija > MAXNLEIMA-1) return(rt->rastiluku);
   if (lukija < 0)
		return(-1);
   j = lukija;

   memset(oikeat, 0, sizeof(oikeat));

   // Nyt j osoittaa viimeiseen kirjattuun koodiin, jonka pitäisi olla
   // lukijan koodi (yleensä 250)
   // Jos kortti luetaan toistuvasti voi loppuun tulla uusia rivejä
   // joilla on lukijan koodi, mutta aika nolla. Nämä ohitetaan.

   while (j != (lukija+1)%MAXNLEIMA && em->ctrlcode[j] == em->ctrlcode[(j+MAXNLEIMA-1)%MAXNLEIMA] && !em->ctrltime[j])
	  j = (j+MAXNLEIMA-1)%MAXNLEIMA;

   // Rata käydään läpi alkaen viimeisestä rastista

   i = rt->rastiluku + 1;
   for (;;) {

      if (!vapaajarj) {
          --i;
         }
	  else {
		 while (oikeat[i]) {
			--i;
            }
		 }
      if (i < 0)
		 break;

	  if (rt->rastikoodi[i] == 10000 || rt->rastikoodi[i] == 10001) {
		 vapaajarj = 10001 - rt->rastikoodi[i];
         oikeat[i] = 2;
		 continue;
		 }

      // Jos oikeakoodi palauttaa arvon 2, on rasti merkitty puuttuvaksi

	  if ((m = oikeakoodi(rt, i, 1, 0)) == 2) {
         oikeat[i] = 1;
         continue;
         }

      // Seuraavassa loopissa käsitellään virheelliset koodit. Loopista
      // poistutaan (ennen sen suoritusta), kun
      // -  j == 0 (Tämä vastaa aina nollausleimausta)
      // -  on löydetty vuorossa oleva oikea koodi tai
      // -  i vastaa rataan kuulumatonta lukijalaitetta
      //
      // Vuorossa olevalle koodille annetaan tulkinta (jos pyydetty)
      // seuraavasti.
      // -  tulkinta annetaan negatiivisena (virheen merkkinä)
	  // -  tulkinnan itseisarvo on rastiluku+1, jos lukija
      // -  jos koodi vastaa yhtä tai useampaa radan rastia, on tulkinnan
      //    itseisarvo viimeisen tällaisen rastin järjestysnumero
      // -  muissa tapauksissa tulkinnaksi jää 0
      //
      // Virheellistä koodia käänteisessä käsittelyjärjestyksessä 
	  // seuraavat nolla-koodit ohitetaan (tätä ei pitäisi tapahtua
	  // muulloin kuin, kun on tavattu nollauslaite
      // Jos tässä vaiheessa havaitaan ajan nollaus, asetetaan flag
      // nollattu

		while (j != lukija && j != (lukija+1)%MAXNLEIMA && !oikeakoodi(rt, i, em->ctrlcode[j], vapaajarj) &&
			(i < rt->rastiluku || (em->ctrlcode[(j+1)%MAXNLEIMA] != 0 &&
			em->ctrlcode[(j+1)%MAXNLEIMA] != em->ctrlcode[j]))) {
			if (tulkinta) {
				for (k = rt->rastiluku; k >= 0; k--) {
					if ((k == rt->rastiluku && (em->ctrlcode[(j+1)%MAXNLEIMA] == 0 ||
						em->ctrlcode[(j+1)%MAXNLEIMA] == em->ctrlcode[j])) ||
						oikeakoodi(rt, k, em->ctrlcode[j], 0) == 1) {
						if (k == rt->rastiluku || !tulkinta[j]) {
							tulkinta[j] = -(k+1);
							m = k-1;
							if (kk <= k) {
								for (kk2 = kk; kk2 >= 0; kk2--) {
									if (oikeakoodi(rt, kk2, em->ctrlcode[j], 0) == 1) {
										tulkinta[j] = -(kk2+1);
										m = kk2-1;
										break;
										}
									}
								}
							kk = m;
							for (; m >= 0; m--) {
								if (rt->rastikoodi[m] > 9999 && rt->rastikoodi[m] < 10002 && tulkinta[j] < 0) {
									tulkinta[j]++;
									}
								}
							}
						break;
						}
					}
				}
			do {
				j = (j+MAXNLEIMA-1)%MAXNLEIMA;
				if (i > 0 && i < rt->rastiluku-1 && !em->ctrltime[j])
				nollattu = 1;
				} while (!em->ctrlcode[j] && j != (lukija+1)%MAXNLEIMA);
			}

//      if (j == (lukija+1) % MAXNLEIMA) break;
		if ((j == (lukija+1) % MAXNLEIMA) || (j == lukija && i < rt->rastiluku))
			break;

      // Tähän tullaan vain, kun j vastaa vuorossa olevaa oikeaa rastia
      // tai kohdalla rastiluku+1 olevaa lukijalaitetta

	  // i:n arvoa pienennetään, jos kyseessä lukijalaite, jonka koodi
	  // löytyy radan viimeiseltä ilmoitetulta rastilta eli kun lukija
      // sisältyy rataan

      if (i == rt->rastiluku && oikeakoodi(rt, i-1, em->ctrlcode[j], 0)) i--;

	  if (!vapaajarj) {
//         if ((i == 0 || em->ctrltime[j]) && !nollattu)
		 if ((i == 0 || em->ctrltime[j]))
            oikeat[i] = 1;
         m = i;
         if (tulkinta) {
            tulkinta[j] = i+1;
            kk = i;
			}
         }
	  else {
         for (m = i; m >= 0; m--) {
			if (rt->rastikoodi[m] == 10001) {
               break;
               }
            if (!oikeat[m] && oikeakoodi(rt, m, em->ctrlcode[j], 0)) {
               oikeat[m] = 1;
			   if (tulkinta) {
                  tulkinta[j] = m+1;
                  }
			   break;
               }
            }
         }

      // Korjataan tulkintalaskuri, kun kirjainkoodeja

	  m--;
      if (tulkinta && tulkinta[j] > 0) {
         for (; m >= 0; m--) {
			if (rt->rastikoodi[m] > 9999 && rt->rastikoodi[m] < 10002 && tulkinta[j] > 0) {
               tulkinta[j]--;
               }
            }
         }
   
         // Oikeaa koodia käänteisessä käsittelyjärjestyksessä 
         // seuraavat nolla-koodit ohitetaan (tätä ei pitäisi tapahtua
		 // muulloin kuin, kun on tavattu nollauslaite)
         // Samalla tarkastetaan, esiintyykö radalla haittaavaa kellon
         // nollausta
   
      do {
         j = (j+MAXNLEIMA-1)%MAXNLEIMA;
		 if (i > 0 && (j != lukija && !em->ctrltime[j] && i < rt->rastiluku-1 &&
			(i > 1 || (em->ctrlcode[j] && rt->ennakko >= 0))) &&
				(!vapaajarj || !oikeat[1]))
			nollattu = 1;
		 } while (!em->ctrlcode[j] && j != lukija);
	  }
   for (i = 0; i < rt->rastiluku; i++)
      if (!oikeat[i]) break;
   if (i >= rt->rastiluku) return(-2*nollattu);
   else return(i + 1);
   }

INT etsierata(emittp *em)
   {
   INT i, nr = 0, r = -1;

   for (i = 0; i < nrata; i++) {
	  if (!tarkista(em, &i, 1, 0)) {
		 if (rata[i].rastiluku > nr) {
			r = i;
			nr = rata[i].rastiluku;
			}
		 }
	  }
   return(r);
   }

INT etsiesarja(emittp *em)
   {
   INT r;

   r = etsierata(em);
   if (r >= 0)
	   r = haeradansarja(rata[r].tunnus, NULL);
   return(r);
   }

INT tark_badge(emittp *em)
   {
   kilptietue kilp;
   INT d, er;

   if ((em->kilpno = bdg2kno(em->badge)) == 0) return(1);
   if (esitark)
      return(0);
   if ((d = getpos(em->kilpno)) < 1) return(1);
   EnterCriticalSection(&tall_CriticalSection);
   kilp.GETREC(d);
	if (esiluenta == 1) {
		kilp.set_tark(L'-', k_pv);
		}
	else {
	   er = tarkista(em, NULL, 0, 0);
	   if (er == -1) {
		   LeaveCriticalSection(&tall_CriticalSection);
		   return(1);
		   }
	   if (er <= 0) {
		  if (kilpparam.kaksibadge == 2 && kilp.Badge(k_pv, 1-em->osuus)) {
			  kilp.pv[k_pv].asakot[em->osuus] = L'T';
			  if (kilp.pv[k_pv].asakot[1-em->osuus] == L'T')
				  kilp.set_tark(L'T', k_pv);
			  if (kilp.pv[k_pv].asakot[1-em->osuus] < L'A')
				  kilp.pv[k_pv].asakot[1-em->osuus] = L'-';
			  }
		  else
			  kilp.set_tark(L'T', k_pv);
		  erbeep();
		  }
	   else if (er > 0) {
		  if (kilpparam.kaksibadge == 2 && kilp.Badge(k_pv, 1-em->osuus))
			  kilp.pv[k_pv].asakot[em->osuus] = L'I';
		  kilp.set_tark(L'I', k_pv);
		  if (!luesuoraan) {
			 erbeep();
			 utsleep(2);
			 erbeep();
			 utsleep(2);
			 erbeep();
			 }
		 }
	  }
   kilp.tallenna(d, 1, 0, 0, 0);
   LeaveCriticalSection(&tall_CriticalSection);
   if (er < 0)
	  return(er);
   return(0);
   }

INT maalirasti(ratatp *rt, INT koodi)
   {
   return(oikeakoodi(rt, rt->rastiluku-1, koodi, 0));
   }

void haerastit(int ecode, int *rst, int *nr)
   {
   int r, n = 0;

   for (r = 0; r < nkuvio; r++) {
	  if (kuvio[r].koodi == ecode) {
		 rst[n] = kuvio[r].rasti;
		 n++;
         if (n >= *nr)
            break;
         }
      }
   *nr = n;
   }

INT lue_leimat(INT r)
   {
   TextFl *leima_file = 0;
   INT k, tn;
   wchar_t msg[80], ch, Buf[200];
   ekditp tmpkdi[1000];

	if (!emitfl) return(1);
	if (!ohitaleimat) {
		leima_file = new TextFl(leimafname,L"r");
		if (!leima_file->IsOpen()) {
			delete leima_file;
			wmemmove(leimafname+7, leimafname+6, 5);
			leimafname[6] = L'1'+k_pv;
			leima_file = new TextFl(leimafname,L"r");
			}
		if (!leima_file->IsOpen()) {
			delete leima_file;
			leima_file = 0;
			swprintf(msg, L"Tiedoston LEIMAT.LST tai LEIMAT%c.LST avaaminen ei onnistu", L'1'+k_pv);
			vidspwmsg(r,5,7,0,msg);
			}
		}
   if (!leima_file || !leima_file->IsOpen()) {
	  vidspwmsg(r+1,5,7,0,
		 L"EMIT-koodit asetataan samoiksi kuin rastitunnukset");
	  if (!ohitaleimat) {
		 ch = L' ';
		 wselectopt(L"J)atka ilman tiedostoa LEIMAT.LST, P)oistu ohjelmasta", L"JP", &ch);
		 if (ch == L'P')
			return(1);
		 }
	  kuvio = (ekditp *)calloc(255, sizeof(ekditp));
	  if (!kuvio) {
		 writeerror_w(L"Ei muistitilaa EMIT-koodeille", 0);
			return(1);
		 }
	  else {
		 for (k = 0; k < 255; k++) {
			kuvio[k].rasti = k+1;
			kuvio[k].koodi = kuvio[k].rasti;
			}
		 nkuvio = 255;
		 maxnkuvio = 255;
		 }
	  return(0);
	  }
   while (!leima_file->Feof()) {
	  if (leima_file->ReadLine(Buf, 200) == NULL)
		  break;
	  if (swscanf(Buf, L"%d %d\n", &tn, &k) < 2) break;
	  if (tn > MAXTUNNUS) {
		 swprintf(msg,L"Rastitunnus %d tiedostossa %s on liian suuri",
			tn, leimafname);
		 kirj_err_file(msg, 1);
		 continue;
		 }
	  tmpkdi[nkuvio].rasti = tn;
	  tmpkdi[nkuvio].koodi = k;
	  nkuvio++;
	  }
   if (leima_file)
		delete leima_file;
   qsortint(tmpkdi, nkuvio, sizeof(ekditp), 0,  sizeof(kuvio[0].rasti)==4);
   kuvio = (ekditp *) calloc(nkuvio+20, sizeof(ekditp));
   if (!kuvio) {
	  writeerror_w(L"Ei muistitilaa EMIT-koodeille", 0);
	  return(1);
	  }
   else {
	  memcpy(kuvio, tmpkdi, nkuvio * sizeof(ekditp));
	  maxnkuvio = nkuvio + 20;
	  }
   return(0);
   }

rpistevatp::rpistevatp(void)
{
	memset(this, 0, sizeof(rpistevatp));
}

rpistevatp::~rpistevatp(void)
{
}

ratavatp::ratavatp(void)
{
	memset(this, 0, sizeof(ratavatp));
}

ratavatp::~ratavatp(void)
{
	if (pisteva)
		delete[] pisteva;
}

int ratavatp::lisaa(int kilpno, int sarja, int nva, int aika)
{
	int i, muutt = 0;

	for (i = 0; i < nva; i++) {
		if (pisteva[i].kilpno == kilpno) {
			if (pisteva[i].aika == aika) {
				pisteva[i].sarja = sarja;
				return(0);
				}
			memmove(pisteva+i, pisteva+i+1, (nva-i-1) * sizeof(pisteva[0]));
			nva--;
			muutt--;
			memset(pisteva+nva, 0, sizeof(pisteva[0]));
			break;
			}
		}
	if (aika == 0)
		return(muutt);
	for (i = 0; i < nva; i++)
		if (pisteva[i].aika > aika)
			break;
	muutt++;
	memmove(pisteva+i+1, pisteva+i, (nva-i) * sizeof(pisteva[0]));
	pisteva[i].aika = aika;
	pisteva[i].sarja = sarja;
	pisteva[i].kilpno = kilpno;
	return(muutt);
}

int ratavatp::poista(int kilpno, int nva)
{
	for (int i = 0; i < nva; i++) {
		if (pisteva[i].kilpno == kilpno) {
			memmove(pisteva+i, pisteva+i+1, (nva-i-1) * sizeof(pisteva[0]));
			memset(pisteva+nva-1, 0, sizeof(pisteva[0]));
			return(-1);
			}
		}
	return(0);
}

rastivatp::rastivatp(void)
{
	memset(this, 0, sizeof(rastivatp));
}

rastivatp::~rastivatp(void)
{
	if (ratava)
		delete[] ratava;
}

void rastivatp::poista(int kilpno)
{
	int n = 0;
	for (int r = 0; r < rata->rastiluku+1; r++) {
		n += ratava[r].poista(kilpno, nva);
		}
	if (n < 0)
		nva--;
}

int haervali(ratatp *rt, int ir)
   {
   int a, y, k, rvkdi;

   if (rvalit == NULL || ir > rt->rastiluku - (rt->maalilaji == 4))
	   return(-1);
   rvkdi = 0;
   if (ir) rvkdi = 1000*rt->rastikoodi[ir-1];
   if (ir < rt->rastiluku - (rt->maalilaji == 4))
	  rvkdi += rt->rastikoodi[ir];
   a = 0;
   y = rvluku-1;
   for(;;) {
	  if (rvkdi < rvalit[a].kdi || rvkdi > rvalit[y].kdi)
		 return(-1);
	  if (rvkdi == rvalit[a].kdi)
		 return(a);
	  if (rvkdi == rvalit[y].kdi)
		 return(y);
	  if (y - a < 2)
		 return(-1);
	  k = (a + y) / 2;
	  if (rvkdi > rvalit[k].kdi)
         a = k;
      else
		 y = k;
      }
   }

void analradat(void)
{
	int r, ir, (*rvint)[2], (*p)[2], rkdi, nva, mtk;
	ratatp *rt;

	if (rastiva)
		delete[] rastiva;
	rastiva = new rastivatp[nrata];
	for (r = 0; r < nrata; r++) {
		if (rata[r].rastiluku == 0)
			continue;
		rvluku +=  rata[r].rastiluku+1;
		rastiva[r].rata = rata+r;
		rastiva[r].ratava = new ratavatp[rata[r].rastiluku+1];
		}
	rvint = (int (*)[2]) calloc(rvluku, 2*sizeof(int));
	rvluku = 0;
	for (r = 0; r < nrata; r++) {
		if (rata[r].rastiluku == 0)
			continue;
		rt = rata+r;
		nva = rt->rastiluku  + ((rt->maalilaji > 1) ? 0 : 1);
		for (ir = 0; ir < nva; ir++) {
			rkdi = 0;
			mtk = 0;
			if (ir) {
				rkdi = 1000*rt->rastikoodi[ir-1];
				mtk = -rt->matka[ir-1];
				}
			if (ir < nva - 1) {
				rkdi += rt->rastikoodi[ir];
				mtk += rt->matka[ir];
				}
			else {
				if (nva == rt->rastiluku)
					mtk += rt->matka[ir];
				else
					mtk = rt->viitmatka;
				}
			for (p = rvint; p[0][0]; p++)
				if (p[0][0] == rkdi)
					break;
			if (p[0][0] == 0) {
				rvluku++;
				p[0][0] = rkdi;
				p[0][1] = mtk;
				}
			else if (p[0][1] == 0 && mtk)
				p[0][1] = mtk;
			}
		}
	qsortint(rvint, rvluku, 2*sizeof(int), 0, 1);
	rvalit = (rvalitp *) calloc(rvluku, sizeof(rvalitp));
	for (r = 0; r < rvluku; r++) {
		rvalit[r].kdi = rvint[r][0];
		rvalit[r].matka = rvint[r][1];
		}
	free(rvint);
}

int bSrchInt(int val, int *arr, int n)
   {
   int a, y, k;

   if (n == 0)
	  return(0);
   a = 0;
   y = n-1;
   for(;;) {
      if (val <= arr[a])
         return(a);
      if (val >= arr[y])
         return(y+(val > arr[y]));
      if (y - a < 2)
		 return(y);
      k = (a + y) / 2;
	  if (val > arr[k])
		 a = k+1;
	  else
		 y = k;
	  }
   }

void addIntInArr(int val, int val2, int *arr, int *arr2, int n, int nmax)
   {
   int pos;

   if (n > nmax)
      n = nmax;
   pos = bSrchInt(val, arr, n);
   if (pos < nmax) {
      if (n > nmax -1)
		 n = nmax-1;
	  memmove(arr+pos+1, arr+pos, (n - pos) * sizeof(int));
	  arr[pos] = val;
	  memmove(arr2+pos+1, arr2+pos, (n - pos) * sizeof(int));
      arr2[pos] = val2;
      }
   }

void laskevertaika(int rv, int srj)
   {
   int i, nvert, tvert = 0;

   if (!rvalit)
		return;
   if (rvalit[rv].lkm[srj] > 0) {
	  nvert = (rvalit[rv].lkm[srj] * analvertosuus) / 100;
      if (nvert <= 0)
         nvert = 1;
      if (nvert > analvertmax)
         nvert = analvertmax;
	  for (i = 0; i < nvert; i++)
		 tvert += rvalit[rv].ajat[srj][i];
      tvert /= nvert;
      }
   rvalit[rv].vertaika[srj] = tvert;
   }

void laskevertajat(void)
   {
   int rv, srj;

   for (rv = 0; rv < rvluku; rv++) {
      for (srj = 0; srj < sarjaluku; srj++)
		 laskevertaika(rv, srj);
      }
   }

int laskeemitvaliajat(emittp *em, kilptietue *kilp, emitvaanaltp **eva, rastivaanaltp **rstva, int *valuku)
   {
   int ir, j, k, srj, ret, tulkinta[MAXNLEIMA+1], alku, loppu, ensrasti = -1;
   INT32 enn;
   ratatp *rt;
   static emitvaanaltp *ev;
   static rastivaanaltp *rst;

   enn = haeennakko(em, kilp);
   if ((rt = haerata(kilp)) == 0)
	  return(1);
   ret = tarkista(em, tulkinta, 0, 0);
//   if (tarkista(em, tulkinta, 0, 0)) {
//	  return(1);
//	  }
   for (k = 1; k < 4; k++) {
	  for (j = 0; j < MAXNLEIMA+1; j++) {
		 if (tulkinta[j] == k || -tulkinta[j] == k) {
			ensrasti = j;
			break;
			}
		 }
	  if (ensrasti >= 0) {
		 break;
		 }
		}
	if (ensrasti < 0) {
		return(1);
		}
   srj = kilp->Sarja();
   j = 0;
   alku = 0;
   *valuku = rt->rastiluku  + ((rt->maalilaji & 6) ? 0 : 1);
   if (rastiva && rstva) {
	   rst = (rastivaanaltp *)calloc(*valuku, sizeof(rastivaanaltp));
	   *rstva = rst;
	   }
   ev = (emitvaanaltp *)calloc(*valuku, sizeof(emitvaanaltp));
   *eva = ev;
   for (ir = 0; ir < *valuku; ir++) {
	  int jj;
	  jj = j;
	  while (jj < MAXNLEIMA &&
		 tulkinta[(ensrasti+jj)%MAXNLEIMA] != ir+1 &&
		 -tulkinta[(ensrasti+jj)%MAXNLEIMA] != ir+1)
		 jj++;
	  if (jj == MAXNLEIMA)
		  continue;
	  j = jj;
	  if (ir >= rt->rastiluku) {
		 loppu = kilp->p_aika(0)/SEK;
/*
		 if (ir == rt->rastiluku) {
			if (em->ctrltime[(ensrasti+j)%MAXNLEIMA]-enn < loppu - 10)
			   loppu = em->ctrltime[(ensrasti+j)%MAXNLEIMA]-enn;
			else
			   --*valuku;
			}
*/
		 }
	  else {
		 loppu = em->ctrltime[(ensrasti+j)%MAXNLEIMA]-enn;
		 if (loppu < 0)
			loppu = 0;
		 }
	  ev[ir].rkoodi = rt->rastikoodi[ir];
	  if (loppu >= alku && loppu - alku < 3*3600) {
		 ev[ir].aika = loppu;
		 ev[ir].rvaika = loppu-alku;
		 alku = loppu;
		 ev[ir].rvno = haervali(rt, ir);
		 }
	  else
		 ret = 1;
	  if (rastiva && rstva) {
		 int sj = 0, kaika = 0;
		 rst[ir].aika = ev[ir].aika;
		 for (int i = 0; i < rastiva[rt-rata].nva; i++) {
			 if (rastiva[rt-rata].ratava[ir].pisteva[i].sarja == srj) {
				 sj++;
				 if (kaika == 0)
					kaika = rastiva[rt-rata].ratava[ir].pisteva[i].aika;
				 if (rastiva[rt-rata].ratava[ir].pisteva[i].aika >= rst[ir].aika)
					break;
				 }
			 }
		  rst[ir].kaika = kaika;
		  rst[ir].sj = sj;
		  }
//	  else {
//		 free(ev);
//		 *eva = 0;
//		 return(1);
//		 }
	  }
   return(ret);
   }

void addemitanal(emittp *em, int uusi)
   {
   kilptietue kilp;
   int d, ir, j, tulkinta[MAXNLEIMA+1], nva, srj, rr, lis = 0;
//   int rkdi;
   emitvaanaltp *ev = NULL;
   ratatp *rt;

   if (!em || !em->kilpno || em->osuus > 0)
	  return;
   if ((d = getpos(em->kilpno)) <= 0)
	  return;
   kilp.GETREC(d);
   if ((rt = haerata(&kilp)) == 0)
	  return;
   rr = rt - rata;
   srj = kilp.Sarja();
   j = tarkista(em, tulkinta, 0, 0);
   if (j)
	  return;
	EnterCriticalSection(&rastiva_CriticalSection);
   if (!laskeemitvaliajat(em, &kilp, &ev, 0, &nva)) {
	   for (ir = 0; ir < nva; ir++) {
			if (rastiva[rr].nva < rastiva[rr].maxnva) {
				lis += rastiva[rr].ratava[ir].lisaa(kilp.kilpno, srj, rastiva[rr].nva, ev[ir].aika);
				}
		  d = ev[ir].rvno;
		  if (d >= 0 && d < rvluku) {
			 rvalit[d].sum[srj] += ev[ir].rvaika;
			 addIntInArr(ev[ir].rvaika, em->kilpno,
				rvalit[d].ajat[srj],
				rvalit[d].kilpno[srj],
				rvalit[d].lkm[srj],
				rvalit[d].maxlkm[srj]);
			 rvalit[d].lkm[srj]++;
			 if (uusi)
				laskevertaika(d, srj);
			 }
		  }
		if (lis)
			rastiva[rr].nva += (lis > 0) ? 1 : -1;
	  }
   if (ev)
	  free(ev);
	LeaveCriticalSection(&rastiva_CriticalSection);
   }

static void tarkkilpradat(void)
   {
   int d, srj;
   kilptietue kilp;
   if (emitanal_fl) {
	  analradat();
	  for (d = 0; d < rvluku; d++) {
		 for (srj = 0; srj < sarjaluku; srj++) {

			rvalit[d].maxlkm[srj] = nilm[srj] + nilm[srj]/10 + 5;

			if (rvalit[d].maxlkm[srj]++) {
			   rvalit[d].ajat[srj] =
				  (int *)calloc(rvalit[d].maxlkm[srj]+1, sizeof(int));
			   rvalit[d].kilpno[srj] =
				  (int *)calloc(rvalit[d].maxlkm[srj]+1, sizeof(int));
			   }
			}
		 }
	  }
   }

static int TarkKilpailijat(void)
{
	kilptietue kilp;
	wchar_t msg[100];

	for (int d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (!kilp.kilpstatus && wcswcind(kilp.tark(k_pv), L"PVE") < 0 &&
			Sarjat[kilp.Sarja()].luonne[k_pv] >= 0) {
			if (!Sarjat[kilp.Sarja()].vakanttisarja) {
				ratatp * rt;
				if ((rt = haerata(&kilp)) == NULL) {
					swprintf(msg, L"VIRHE: Kilpailijan %d sarjaa %s tai rataa %s ei radoissa. ",
						kilp.id(), Sarjat[kilp.Sarja()].sarjanimi, kilp.pv[k_pv].rata);
					kirj_err_file(msg, 1);
					}
				else {
					if (emitanal_fl && rastiva && rt == rastiva[rt-rata].rata) {
						rastiva[rt-rata].nva++;
						}
					}
				}
			}
		}
	if (emitanal_fl && rastiva) {
		for (int r = 0; r < nrata; r++) {
			if (rata[r].rastiluku > 0) {
				rastiva[r].maxnva = rastiva[r].nva+kilpparam.analoolisa;
				rastiva[r].nva = 0;
				for (int i = 0; i <= rata[r].rastiluku; i++) {
					rastiva[r].ratava[i].pisteva = new rpistevatp[rastiva[r].maxnva];
					}
				}
			}
		}
	return(0);
}

INT lue_radat(INT r)
   {
   TextFl *rata_file;
   INT i, j, ir, tn, srj, kdi[MAXNRASTI], rno, nr = 0, vapaajarj = 0, ensilm = 1;
   wchar_t line[300], msg[80], *p, rnimi[12];
   static int init = 1;

   if (!emitfl) return(1);
   nrata = 0;
   rno = 0;
   tn = 0;
   if (init) {
	   if (kilpparam.n_pv_akt > 1) {
		  ratafname[5] = k_pv + L'1';
		  }
	   else {
		  wmemmove(ratafname+5, ratafname+6, 5);
		  }
	   init = 0;
	   }
   rata_file = new TextFl(ratafname,L"r");
   if (!rata_file->IsOpen()) {
		delete rata_file;
	  if (emitfl == 2) {
		  emitfl = 0;
		  return(0);
		  }
	  if (emitfl == -1) {
		 return(0);
		 }
	  swprintf(line, L"Tiedoston %s avaaminen ei onnistu",
		 ratafname);
	  writeerror_w(line, 0);
	  emitfl = 0;
	  return(1);
	  }
	if (nkuvio == 0)
		lue_leimat(r-2);
	monirata = 2;
	for (i = 0; ; i++) {
		if (rata_file->ReadLine(line, 298) == NULL || wcslen(line) < 4)
			break;
		if (i % 3 == 0) {
			p = wcstok(line, L" \t");
			rno++;
			}
		}
	nrata = rno;
	rata = (ratatp *) calloc(nrata, sizeof(ratatp));
	if (!rata) {
		writeerror_w(L"Ei tilaa ratatiedoille", 0);
		goto esc1;
		}
	rno = 0;
	rata_file->Rewind();
	for (;;) {
		if (rata_file->ReadLine(line, 298) == NULL || wcslen(line) < 4)
			break;
		p = wcstok(line, L" \t");
		wcsncpy(rnimi, p, 12);
		rnimi[11] = 0;
		elimwbl2(rnimi);
		upcasewstr(rnimi);
		rnimi[sizeof(rata[i].tunnus)/2-1] = 0;
		for (i = 0; i < rno; i++)
			if (!wcscmp(rata[i].tunnus, rnimi))
				break;
		if (i < rno) {
			swprintf(msg, L"Rata %s toistuu ratatiedoissa", rnimi);
			kirj_err_file(msg, 1);
			}
		srj = rno++;
		wcscpy(rata[srj].tunnus, rnimi);
		p = wcstok(NULL, L" \t");
		if (p) {
			rata[srj].ennakko = _wtoi(p);
			p = wcstok(NULL, L" \t");
			if (p)
				rata[srj].maalilaji = _wtoi(p);
			}
	  ir = 0;
	  for (i = 0; i < 2; i++) {
		 if (rata_file->ReadLine(line, 298) == NULL) goto esc;
		 if (emitfl < 0) {
			continue;
			}
		 p = wcstok(line, L" \t");
		 for (j = 0; j < 40; j++) {
			if (!p) {
				if (i == 0 || !vapaajarj)
					break;
				tn = 10000;
				vapaajarj = 0;
				}
			else {
				tn = _wtoi(p);
				if (tn == 0) {
					*p = towupper(*p);
					switch (*p) {
                        case L'V':
							if (!vapaajarj) {
                                vapaajarj = 1;
								tn = 10001;
                                }
                            break;
						case L'J':
                            if (vapaajarj) {
								vapaajarj = 0;
								tn = 10000;
                                }
							break;
                        }
                    }
                if (tn == 0) {
                    break;
                    }
                }
            if (tn <= MAXTUNNUS || tn == 10000 || tn == 10001) {
			   if (tn <= MAXTUNNUS && !on_kuvio(tn)) {
				  swprintf(msg, L"Radan %s rastin %d tunnusta %d ei tiedostossa %s",
					 rnimi, ir+1, tn, leimafname);
				  kirj_err_file(msg, 1);
                  }
               if (ir >= MAXNRASTI) {
                  swprintf(msg, L"Radalla %s liikaa rasteja (yli %d)",
					 rnimi, MAXNRASTI);
				  kirj_err_file(msg, 1);
				  continue;
				  }
			   kdi[ir] = tn;
			   ir++;
			   }
			if (!p)
				break;
			p = wcstok(NULL, L" \t");
			}
		 }
	  rata[srj].rastiluku = ir;
	  if (ir) {
		 memset(rata[srj].rastikoodi, 0, sizeof(rata[0].rastikoodi));
		 if (!rata[srj].rastikoodi) {
			writeerror_w(L"Ei tilaa ratatiedoille", 0);
			goto esc1;
			}
		 for (j = 0; j < ir; j++)
			rata[srj].rastikoodi[j] = kdi[j];
		 nr++;
		 }
	  }
   if (emitfl < 0)
	  swprintf(msg, L"Tiedostosta %s luettu ajanottotavat", ratafname);
   else
	  swprintf(msg, L"Tiedostosta %s luettu %d rataa", ratafname, nr);
   vidspwmsg(r,5,7,0,msg);
   delete rata_file;
   if (emitfl > 0) {
	  tarkkilpradat();
	  TarkKilpailijat();
	  }
   return(0);
   esc1:
   free(rata);
   emitfl = 0;
   esc:
   swprintf(msg,L"Virhe tiedostoa %s luettaessa. Rata %s",
	  ratafname, rnimi);
   kirj_err_file(msg, 1);
   delete rata_file;
   return(1);
   }

INT lueemitfile(void)
   {
   emittp em;
   INT kysy = 0;
   wchar_t ch, msg[81];
   bool uusinta = false;

   if (!emitfl) return(1);
   if (esitark || esiluenta == 1)
	  wcscpy(emitfname, L"ESITARK.DAT");
   ajat_on_loc = ajat_on;
   if (femitfile) {
		_lseek(emitfile, 0, SEEK_SET);
		uusinta = true;
		for (int i = 0; i < maxrec + 1; i++) {
		   emitjarr[0][i] = -1;
		   if (kilpparam.kaksibadge == 2)
			   emitjarr[1][i] = -1;
		   }
		}
   else {
	   if ((emitfile = _wopen(emitfname, O_RDWR | O_CREAT | O_BINARY,
		  S_IREAD | S_IWRITE)) < 0) {
		  swprintf(msg, L"Tiedoston %s avaaminen tai luominen ei onnistu",
			 emitfname);
		  writeerror_w(msg, 0);
		  return(1);
		  }
	   femitfile = _wfdopen(emitfile, L"r+b");
	   setvbuf(femitfile, NULL, _IONBF, 0);
	   }
   emithead = (INT) (_filelength(emitfile) / sizeof(emittp));
   emithead2 = emithead;
   if (emithead) {
	  if (!uusinta && ntulost+nhylt == 0 && !esitark && esiluenta != 1 && KuntoMaali >= 0) {
		 ch = L' ';
		 swprintf(msg, L"Ei tuloksia, mutta %d Emit-tietuetta. J)atka, "
			L"T)uhoa Emit-tiedot, K)eskeytä", emithead);
		 wselectopt(msg,L"JTK", &ch);
		 }
	  else
		 ch = L'J';
	  switch (ch) {
		 case L'K':
			return(1);
		 case L'T':
			_chsize(emitfile, 0);
			emithead = 0;
			emithead2 = 0;
			break;
		 case L'J':
			_lseek(emitfile, 0, SEEK_SET);
			emithead2 = 0;
			for (emitptr = 0; emitptr < emithead; emitptr++) {
			   if (_read(emitfile, &em, sizeof(emittp)) < (int)sizeof(emittp)) {
//                  swprintf(msg, L"Tiedoston %s lukeminen ei onnistu",
//					 emitfname);
//				  writeerror_w(msg, 0);
				  return(1);
				  }
			   if (em.kilpno && em.badge) {
				  if (em.kilpno == bdg2kno(em.badge)) {
					 if (emitanal_fl && getEmitJarr(em.kilpno, 0) < 0)
						addemitanal(&em, 0);
					 setEmitJarr(emitptr, em.kilpno, em.osuus);
					 }
				  else {
					 kysy++;
					 swprintf(msg, L"Emit-koodia %ld vastaa eri kilpailijat "
						L"KILP.DATissa %d ja %sissa %d",
						em.badge, bdg2kno(em.badge), emitfname, em.kilpno);
					 kirj_err_file(msg, 0);
					 }
				  }
			   if (KSEK*em.kirjaus != TMAALI0)
				  emithead2 = emitptr + 1;
			   }
		 }
	  }
   emitptr = emithead-1;
   if (kysy) {
	  swprintf(msg,
		 L"%s ja KILP.DAT eivät yhteensopivia (%d eroa) Kts DATA_ERR.TXT",
			emitfname, kysy);
	  writeerror_w(msg, 0);
	  }
   if (emitanal_fl)
	  laskevertajat();
   return(0);
   }

INT32 haeennakko(emittp *em, kilptietue *kilp)
   {
//   INT i, lkja = 0;
   int lkja, enn;
   ratatp *rt;

   // Jos sarjalla ei ennakkoa (eli ennakko 0) tai ennakko kiinteä ( > 0 )
   // saa ennakko tämän arvon, muuten ennakko lasketaan lukuhetkestä ja
   // viimeisestä lukijalaitteen antamasta emit-väliajasta.

   rt = haerata(kilp);
   if (!rt)
		return(0);
   if (rt->ennakko >= 0)
		return(rt->ennakko);
   if ((lkja = haelukija(em)) < 0)
		return(0);
   enn = (kilp->pv[k_pv].tlahto - KSEK*em->time)/SEK + em->ctrltime[lkja];
   enn = (enn + 51L*3600L) % (24L*3600L) - 3*3600L;
   if (enn > 12L*3600L) enn = 0;
   return(enn);
   }

#ifdef _CONSOLE
INT naytatiedot(emittp *em, kilptietue *kilp, int loppuosa)
   {
   INT32 enn = 0, viim;
   wchar_t line[40], st[16];
   INT fg = 7, bg = 0, cl, i, tulkinta[MAXNLEIMA+1], lisa = 0, code99 = 0, erv = 25;

   memset(tulkinta, 0, sizeof(tulkinta));
   if (ySize < 33)
	  erv = ySize-8;
   clrtxt(3, 52, 64);
   for (i = 4; i < ySize-4; i++)
      clrtxt(i, 0, (i >= erv) ? 64 : 39);
   draw_vline(4, 18, erv);
   draw_vline(4, 38, erv);
   if (kilp) {
	  getem(em, kilp->id(), 0);
	  if (em->kilpno != kilp->id())
         return(0);

	  if (tarkista(em, tulkinta, 0, 0)) {
         fg = 0;
         bg = 7;
         for (i = 4; i < ySize-4; i++) {
			viwrrectw(i,0,i,16, L"                 ", fg, bg, 0);
            viwrrectw(i,20,i,36, L"                 ", fg, bg, 0); 
            }
         }

      enn = haeennakko(em, kilp);
      }
   if (em) {
	  if (kilp && (viim = e_maaliaika(em, kilp, 0)) != TMAALI0 && kilp->maali(0) != TMAALI0) {
		 viim = ((kilp->maali(0) - viim)/SEK + 36*3600L)%86400L - 43200L;
		 swprintf(line, L"V.väli%6d", viim);
		 vidspwmsg(3, 52, 7, 0, line);
         }
      if (loppuosa) {
         for (i = MAXNLEIMA-1; i > 2*erv-1; i--)
			if (em->ctrlcode[i] != 0)
               break;
         lisa = i-2*erv+1;
         }
      for (i = lisa; i < 2*erv+lisa; i++) {
         if (i && !em->ctrlcode[i] && !em->ctrltime[i]) break;
         if (tulkinta[i] < 0) swprintf(st, L"(%2d)", -tulkinta[i]);
         else swprintf(st, L" %2d ", tulkinta[i]);
         swprintf(line, L"%2d %3d%5ld %4s", i, em->ctrlcode[i],
			em->ctrltime[i]-enn, st);
         if (em->ctrlcode[i] == 99 || em->ctrlcode[i+1] == 99) {
            cl = fg;
            fg = bg;
            bg = cl;
			code99 = 1;
			}
		 vidspwmsg((i-lisa) % erv + 4, 20*((i-lisa)/erv), fg, bg, line);
         if (em->ctrlcode[i] == 99 || em->ctrlcode[i+1] == 99) {
            cl = fg;
            fg = bg;
            bg = cl;
            }
         }
      if (code99) {
         viwrrectw(17,42,18,60,L"Leimasin sammumassa"
                              L"(koodi 99)         ",0,7,0);
         }
	  for (i = 0; i < MAXNLEIMA; i++) {
         if (tulkinta[i] > 0) {
            if (em->ctrltime[i] < enn) {
               viwrrectw(17, 42, 20, 64,
                  L"Negatiivisia väliaikoja"
                  L"Varmista kilpailija,   "
				  L"lähtöaika sekä kortin  "
                  L"kellon toiminta.       ", 0,7,0);
                }
            break;
            }
		 }
      }
   return(0);
   }
#endif

void emit_tallenna(kilptietue *kilp, wchar_t trk, wchar_t *selitys, INT cn)
   {
   kilptietue entkilp;
   char ch, che;
   INT d;

   if (!cn && (trk == L'H' || trk == L'K') && kilp->tark(k_pv) != L'I'
	  && kilp->tark(k_pv) != trk) {
	  writeerror_w(L"Hylkäys vain esityksen jälkeen.", 0, true);
      return;
      }
   if (!cn && (kilp->tark(k_pv) == L'H' || kilp->tark(k_pv) == L'K') &&
      trk != kilp->tark(k_pv)) {
      clrln(ySize-3);
      vidspwmsg(ySize-3,20,7,0,L"Hylkäyksen poisto. Vahvista (K/E)");
      mene(56, ySize-3);
      do {
		 ch = readkbd(&che,1,spoolfl);
         ch = toupper(ch);
		 } while (ch != 'K' && ch != 'E');
	  if (ch == 'E') return;
	  }
   entkilp = *kilp;
   kilp->set_tark(trk, k_pv);
   wcscpy(kilp->pv[k_pv].selitys, selitys);
   EnterCriticalSection(&tall_CriticalSection);
   if ((d = getpos(kilp->id())) > 0)
	  kilp->tallenna(d, 1, cn, 0, &entkilp);
   LeaveCriticalSection(&tall_CriticalSection);
   }

int em_HaeSeuraava(int eptr)
	{
	int v_ptr;
	emittp em;

	if (eptr >= (v_ptr = em_HaeViimeinen()))
		return(v_ptr);
	eptr++;
	for (; eptr < v_ptr; eptr++) {
		lueem(&em, eptr);
		if (em.badge && em.kilpno == bdg2kno(em.badge) &&
			getEmitJarr(em.kilpno, &em.osuus) == eptr) {
			break;
			}
		}
	return(eptr);
	}

int em_HaeEdellinen(int eptr)
   {
	int e_ptr;
	emittp em;

	if (eptr <= (e_ptr = em_HaeEnsimmainen()) || e_ptr == -1)
		return(e_ptr);
	eptr--;
	for (; eptr > e_ptr; eptr--) {
		lueem(&em, eptr);
		if (em.badge && em.kilpno == bdg2kno(em.badge) &&
			getEmitJarr(em.kilpno, &em.osuus) == eptr) {
			break;
			}
		}
	return(eptr);
	}

int em_HaeViimeinen(void)
{
	int eptr;
	emittp em;

	if (emithead2 == 0)
		return(-1);
	for (eptr = emithead2-1; eptr >= 0; eptr--) {
		lueem(&em, eptr);
		if (em.badge && em.kilpno == bdg2kno(em.badge) &&
			getEmitJarr(em.kilpno, &em.osuus) == eptr) {
			return(eptr);
			}
		}
   return(-1);
}

int em_HaeEnsimmainen(void)
{
	int eptr;
	emittp em;

	if (emithead2 == 0)
		return(-1);
	for (eptr = 0; eptr < emithead2; eptr++) {
		lueem(&em, eptr);
		if (em.badge && em.kilpno == bdg2kno(em.badge) &&
			getEmitJarr(em.kilpno, &em.osuus) == eptr) {
			return(eptr);
			}
		}
   return(-1);
}

void lueseuraava(INT *kno, emittp *em)
	{
	em->LueSeuraava(kno);
	}

void lueedellinen(INT *kno, emittp *em)
	{
	em->LueEdellinen(kno);
	}

void lueseuraavaong(INT *kilpno, emittp *em)
   {
   INT d, edptr;
   kilptietue kilp;

   for (;;) {
		edptr = emitptr;
	  lueseuraava(kilpno, em);
	  if (emitptr == edptr || emitptr >= emithead2-1 || tarkista(em, 0, 0, 0))
         break;
      if (*kilpno && (d = getpos(*kilpno)) > 0) {
         kilp.GETREC(d);
		 if (kilp.tark(k_pv) != L'T' || !kilp.p_aika(0))
            break;
         }
      }
   }

void poista_emit(emittp *em)
   {
   wchar_t ch = L' ';
   INT32 ep;

   wselectopt(L"Vahvista aiempien tietojen poisto (K/E)", L"KE", &ch);
   if (ch == L'K' && em->kilpno >= minkilpno && em->kilpno <= maxkilpno) {
	  ep = getEmitJarr(em->kilpno, &em->osuus);
	  if (ep >= 0 && ep < emithead2) {
		 setEmitJarr(-1, em->kilpno, em->osuus);
		 em->kilpno = 0;
		 em->badge = 0;
		 putem(em, ep, 0);
		 }
	  }
   clrln(ySize-3);
   }

#ifdef _CONSOLE
void vaihdakilp(INT ekno)
   {
   INT k, d, du, tulkinta[MAXNLEIMA];
   INT16 ibdg = 0;
   INT16 kno = 0;
   INT32 tls = 0;
   kilptietue kilp, ekilp;
   wchar_t ch;
   ratatp *rt;
   emittp em;

   clrln(ySize-3);
   vidspwmsg(ySize-3,0,7,0,
      L"Kilpailijanumero, jolle tiedot siirretään :         Esc : peruuta");
   inputintw(&kno,4, 44, ySize-3,L"\r\x1B",&ch);
   if (ch == ESC) return;
   EnterCriticalSection(&tall_CriticalSection);
   if ((d = getpos(ekno)) <= 0 || (du = getpos(kno)) <= 0) {
	  LeaveCriticalSection(&tall_CriticalSection);
	  writeerror_w(L"Kilpailijaa ei löydy", 0);
	  return;
	  }
   if (getEmitJarr(kno, &ibdg) >= 0) {
	  LeaveCriticalSection(&tall_CriticalSection);
	  writeerror_w(L"Valitulle kilpailijalle jo luettu emit-tiedot - ei muuteta", 0);
	  return;
	  }
   ekilp.GETREC(d);
   kilp.GETREC(du);
   if (ekilp.tark(k_pv) == L'P') {
	  writeerror_w(L"Valittu kilpailija poissaoleva - ei muuteta", 0);
	  return;
	  }
   rt = haerata(&ekilp);
   if (rt && rt->maalilaji == 4) {
		if ((tls = ekilp.p_aika(0)) != 0 && !kilp.p_aika(0)) {
		 if (kilp.pv[k_pv].tlahto == TMAALI0)
			kilp.pv[k_pv].tlahto = ekilp.pv[k_pv].tlahto;
		 kilp.tall_tulos(0, tls);
		 ekilp.tall_tulos(0, 0);
		 }
	  }
	for (k = k_pv; k < kilpparam.n_pv_akt; k++)
	   kilp.pv[k].badge[0] = ekilp.pv[k].badge[0];
   kilp.tallenna(du, 0, -1, 0, 0);
	for (k = k_pv; k < kilpparam.n_pv_akt; k++)
		ekilp.pv[k].badge[0] = 0;
	if (!ekilp.p_aika(0))
		ekilp.set_tark(L'-', k_pv);
   ekilp.tallenna(d, 0, -1, 0, 0);
	if (kilp.tark(k_pv) == L'-') {
		getem(&em, kilp.id(), 0);
		kilp.set_tark(tarkista(&em, tulkinta, 0, 0) == -1 ? L'I' : L'T', k_pv);
		kilp.tallenna(du, 0, -1, 0, 0);
		}
	LeaveCriticalSection(&tall_CriticalSection);
	}
#endif

INT ohj_aikaruutu(char *ch2)
   {
	int i;

   if (autokilp) {
	  switch (*ch2) {
         case 50:
            if (ajat_on_loc) {
               ajat_on_loc = 0;
			   status_on = 1;
               }
            else {
               ajat_on_loc = 1;
			   status_on = 0;
               }
            ajat_on = ajat_on_loc;
            break;
         case 22:
			if (seurkrivi> 0) seurkrivi--;
            break;
         case 36:
			 if (aikajono[0] && seurkrivi - aikajono[0]->aktrows < 0)
				seurkrivi++;
            break;
		 }
      for (i = 7; i < 16; i++)
         clrtxt(i, 66, 79);
      lukumaarat();
      }
   *ch2 = 0;
   return(1);
   }

void clrleimat(void)
   {
   INT i;

   clrtxt(0, 0, 33);
   for (i = 2; i < ySize-4; i++)
      clrtxt(i, 0, 64);
   }

#ifdef  _CONSOLE
wchar_t outleimat(kilptietue *kilp, emittp *em, INT selaus, int loppuosa)
   {
	wchar_t ret = L' ';
	wchar_t line[65], st[40], kh;
   INT i, i_pv, tm;

   clrtxt(0, 0, 33);
   if (kilp) {
      swprintf(line, L"Kilpailija %4d, kortti %6ld", kilp->id(), em->badge);
      vidspwmsg(0,0,7,0, line);
	  }
   if (selaus) vidspwmsg(0,50,7,0,L"SELAUS");
   else clrtxt(0,50,56);
   clrtxt(2,30,36);
   for (i = 15; i < ySize-4; i++) clrtxt(i,42,64);
   vidint(0, 57, 5, emitptr+1);
   if (kilp) {
		kilp->nimi(line, 27, false);
	  line[27] = 0;
      vidspwmsg(2, 0, 7, 0, line);
      if (lainakortti(kilp)) {
         vidspwmsg(2,30, 0, 7, L" LAINA ");
         for (i_pv = k_pv+1; i_pv < kilpparam.n_pv_akt; i_pv++)
            if (kilp->tark(i_pv) != L'P')
               break;
         if (i_pv == kilpparam.n_pv_akt) {
			vidspwmsg(2,37, 0, 7, L"KERÄÄ ");
            }
         }
      if (em->kilpno == kilp->id()) {
		 swprintf(line, L"Lukemisaika %8.8s", aikatowstr_ls(st, em->time, t0));
		 vidspwmsg(3, 10, 7, 0, line);
		 tm = kilp->maali(0);
		 if (tm == TMAALI0)
			tm = KSEK * em->maali;
         if (tm != TMAALI0) {
			swprintf(line, L"Maaliaika %8.8s", AIKATOWSTRS(st, tm, t0));
            vidspwmsg(3, 33, 7, 0, line);
            }
		 if (kilp->p_aika(0)) {
			swprintf(line, L"Tulos %8.8s", AIKATOWSTRS(st, kilp->p_aika(0), 0));
			vidspwmsg(4, 42, 7, 0, line);
            }
         else
            clrtxt(4, 42, 64);
         }
      vidspwmsg(2, 49, 7, 0, (wchar_t *)Sarjat[kilp->Sarja()].sarjanimi);
	  if (kilp->pv[k_pv].rata[0])
		 vidspwmsg(2, 65-wcslen(kilp->pv[k_pv].rata), 7, 0, kilp->pv[k_pv].rata);
		kh = kilp->tark(k_pv);
      viwrrectw(13, 54, 13, 54, &kh, 7, 0, 0);
      vidspwmsg(15, 42, 7, 0, kilp->pv[k_pv].selitys);
      ret = kilp->tark(k_pv);
      }
   vidspwmsg(2, 42, 7, 0, L"Sarja");
	vidspwmsg(6, 42, 7, 0, L"Tarkistuksen tulos");
   vidspwmsg(7, 42, 7, 0, L"T: T)arkistus ok");
   vidspwmsg(8, 42, 7, 0, L"H: H)ylkäys");
	vidspwmsg(9, 42, 7, 0, L"K: K)eskeytys");
   vidspwmsg(10, 42, 7, 0, L"I: hylkäysesI)tys");
   vidspwmsg(11, 42, 7, 0, L"P: P)oissa");
   vidspwmsg(13, 42, 0, 7, L"Tark.tulos:");
   vidspwmsg(14, 42, 0, 7, L"Selitys:");
   naytatiedot(em, kilp,loppuosa);
	if (kilp && Sarjat[kilp->Sarja()].rrtas > 1) {
		int yks = Sarjat[kilp->Sarja()].tsak[k_pv];
		if (yks == 0)
			yks = kilpparam.sakkoyks ? kilpparam.sakkoyks : 1;
	   vidspwmsg(17, 42, 7, 0, L"L)isäaika:");
	   vidint(17, 53, 3, kilp->pv[k_pv].tasoitus/yks);
	   }
   return(ret);
   }

void lisaaika(kilptietue *kilp, int d)
	{
   kilptietue entkilp;
	wchar_t ch;
	INT16 sak;
	INT32 tm;
	int yks;

	yks = Sarjat[kilp->Sarja()].tsak[k_pv];
	if (yks == 0)
		yks = kilpparam.sakkoyks ? kilpparam.sakkoyks : 1;
	EnterCriticalSection(&tall_CriticalSection);
	entkilp = *kilp;
	sak = kilp->pv[k_pv].tasoitus/yks;
	inputintw(&sak, 3, 53, 17, L"\r", &ch);
	if (sak != kilp->pv[k_pv].tasoitus/yks) {
	  if((tm = kilp->p_aika(0)) != 0)
		kilp->tall_tulos(0, tm - SEK*(kilp->pv[k_pv].tasoitus-sak*yks));
		kilp->pv[k_pv].tasoitus = sak*yks;
		kilp->tallenna(d, 0, 0, 0, &entkilp);
		}
	LeaveCriticalSection(&tall_CriticalSection);
	}
#endif  // _CONSOLE

#ifdef  EI_OLE
static void emituudelleen(void)
   {
   INT  eptr, d, emitptr0;
   wchar_t ch, line[16];
   emittp em;
   kilptietue kilp;

   clrln(ySize-3);
   vidspwmsg(ySize-3,0,7,0,
	  L"Uudelleenkäsiteltävä tietue:       Esc : Peruuta");
   eptr = emitptr + 1;
   INPUTINTW(&eptr, 5, 29, ySize-3, L"\r\x1b", &ch);
   if (ch == L'\r' && eptr > 0 && eptr <= emithead) {
	  eptr--;
	  lueem(&em, eptr);
	  swprintf(line, L"Alkuperäinen koodi %6ld, koodi nyt %6ld", em.badge0, em.badge);
	  vidspwmsg(2,0,7,0, line);
	  emitptr0 = emitptr;
	  emitptr = eptr;
	  if (em.kilpno && (d = getpos(em.kilpno)) > 0) {
		 kilp.GETREC(d);
		 outleimat(&kilp, &em, 0, 0);
		 }
	  else {
		 outleimat(0, &em, 0, 0);
         }
	  ch = L' ';
	  wselectopt(L"Haluatko varmasti käsitellä tietueen uudelleen (K/E)", L"KE", &ch);
      if (ch == L'K') {
         emithead2 = eptr;
         emitptr = eptr;
         em.badge = em.badge0;
         em.kirjaus = TMAALI0/KSEK;
         putem(&em, eptr, 0);
         }
      else
         emitptr = emitptr0;
		}
   lueedellinen(&d, &em);
   }
#endif

INT32 e_maaliaika(emittp *em, kilptietue *kilp, INT32 *tlahto)
   {
   ratatp *rt = 0;
   INT32 tm = TMAALI0;
   INT k, l, lk;

   lk = haelukija(em);
   if (lk < 0 || lk > MAXNLEIMA-1)
	  return(TMAALI0);
   if (tlahto) {
      rt = haerata(kilp);
      if (rt) {
         if (rt->ennakko >= 0 && em->ctrltime[lk]) {
			*tlahto =
               KSEK*em->time - SEK*(em->ctrltime[lk] + rt->ennakko);
            if (*tlahto < -12*TUNTI)
               *tlahto += 24*TUNTI;
			}
         }
      else
         *tlahto = TMAALI0;
	  }
   for (k = MAXNLEIMA; k > 1; k--) {
	  if (em->ctrltime[(lk+k)%MAXNLEIMA]) {
         tm = KSEK*em->time - SEK*em->ctrltime[(lk+k)%MAXNLEIMA];
         break;
         }
      }
   if (tm != TMAALI0) {
      if (!rt)
		 rt = haerata(kilp);
	  if (rt) {
		 for (l = k-1; l >= rt->rastiluku-2; l--) {
			if (oikeakoodi(rt, rt->rastiluku-1, em->ctrlcode[(lk+l)%MAXNLEIMA], 0))
               break;
            }
         if (l >= rt->rastiluku-2) {
            if (tlahto && rt->ennakko >= 0) {
               tm = -SEK*rt->ennakko;
               if (*tlahto != TMAALI0)
                  tm += *tlahto;
               }
			tm += SEK*em->ctrltime[(lk+l)%MAXNLEIMA];
			if (tm < -12*TUNTI)
               tm += 24*TUNTI;
            }
			else
            tm = TMAALI0;
		 }
		else
         tm = TMAALI0;
      }
   return(tm);
   }

static void e_maali(emittp *em, kilptietue *kilp, INT d)
   {
   INT32 tm, tlahto = TMAALI0;
   wchar_t ch;
   kilptietue entkilp;

#ifdef EBA
   eba_ajat(kilp, em, 1);
      {
#else

   if (kilp->maali(0) != TMAALI0) {
      ch = L' ';
      wselectopt(L"Kilpailijalla jo aika - muutetaanko (K/E)", L"KE", &ch);
      if (ch == L'E')
         return;
	  }

   tm = e_maaliaika(em, kilp, &tlahto);

   if (tm != TMAALI0) {
      if (tlahto != TMAALI0) {
		 if (kilp->pv[k_pv].tlahto == TMAALI0 || kilp->pv[k_pv].tlahto == 0)
            kilp->pv[k_pv].tlahto = tlahto;
         else {
			tm += kilp->pv[k_pv].tlahto - tlahto;
            tm = (tm + 36*TUNTI) % (24*TUNTI) - 12*TUNTI;
            }
         }
	  entkilp = *kilp;
	  kilp->set_tulos(0, tm);
#endif
	  EnterCriticalSection(&tall_CriticalSection);
	  kilp->tallenna(d, 1, 0, 0, &entkilp);
	  LeaveCriticalSection(&tall_CriticalSection);
	  }
   }

static void e_sarja(emittp *em, kilptietue *kilp, INT d)
   {
   INT ratano, srj, tulkinta[MAXNLEIMA];
   wchar_t ch, snimi[12] = L"";
   kilptietue entkilp;
   ratatp *rt;

   clrln(ySize-3);
   ratano = etsierata(em);
   if (ratano >= 0 && ratano < nrata)
	   wcsncpy(snimi, rata[ratano].tunnus, 11);
   rt = haerata(kilp);
   if ((srj = haeradansarja(snimi, NULL)) < 0) {
#ifdef _CONSOLE
	   vidspwmsg(ySize-3,0,7,0,L"Valitse uusi rata:       "
		  L"<Esc>: Poistu vaihtamatta rataa");
	   do {
		   inputwstr(snimi, 8, ySize-3, 19, L"\x1b\r", &ch, 0);
#else
	   do {
			inputstr_prompt(snimi, 8, L"Valitse uusi rata. "
				L"Peruuta: Poistu vaihtamatta rataa", &ch);
#endif
		   if (onrata(snimi))
			   break;
		   else
			   writeerror_w(L"Virheellinen radan nimi", 0, true);
		   } while (ch != ESC);
//	   INPUTINTW(&srj, 5, ySize-3, 19, L"\x1b\n", &ch);
	   if (ch == ESC)
		  return;
	   ch = L' ';
	   wselectopt(L"Vahvista radan vaihto (K/E)", L"KE", &ch);
	   if (ch == L'E')
		  return;
	   entkilp = *kilp;
//	   kilp->ilmlista = srj;
	   wcscpy(kilp->pv[k_pv].rata, snimi);
	  }
   else {
	   vidspwmsg(ySize-3,0,7,0,L"Valitse uusi sarja:               "
		  L"<Esc>: Poistu vaihtamatta sarjaa");
	   wcscpy(snimi, Sarjat[srj].sarjanimi);
	   srj = luesarja(snimi, &ch);
	   if (ch == ESC)
		  return;
	   ch = L' ';
	   wselectopt(L"Vahvista sarjan vaihto (K/E)", L"KE", &ch);
	   if (ch == L'E')
		  return;
	   entkilp = *kilp;
	   kilp->setSarja(srj);
	   }
   EnterCriticalSection(&tall_CriticalSection);
   kilp->tallenna(d, 1, 0, 0, &entkilp);
   LeaveCriticalSection(&tall_CriticalSection);
   wselectopt(L"Muutetaanko tarkistusmerkintä rataa vastaavaksi (K/E)", L"KE", &ch);
   if (ch == L'K') {
		EnterCriticalSection(&tall_CriticalSection);
		entkilp = *kilp;
		kilp->set_tark(tarkista(em, tulkinta, 0, 0) ? L'I' : L'T', k_pv);
		kilp->tallenna(d, 1, 0, 0, &entkilp);
		LeaveCriticalSection(&tall_CriticalSection);
		}
   }

#ifdef _CONSOLE

INT leimat(void)
	{
   wchar_t ch, cs, ct, ch2, line[80], trk, selitys[17], st[40];
   char che;
   INT i, d, k, key, bdgfl = 0, ep, emptr, loppuosa = 0, res;
   INT aani0, kilpno = 0;
   static INT emithead0;
   INT32 badge = 0;
   kilptietue kilp, entkilp;
   emittp em;
   static wchar_t valtxt[] = L"R)aportti, selaa: eD)ell S)eur A)lku V)iim, "
		L"tU)hoa tietue, kO)odit, Esc: Pois      ";
   static wchar_t valtxt_m[] = L"R)aportti, selaa: eD)ell S)eur A)lku V)iim, "
	  L"tU)hoa tietue, M)TR, kO)odit, Esc: Pois";
	static wchar_t numval[] = L"Numero : kilpailijavalinta, Tab : "
	  L"Leimauskortin valinta, F1 : Muita komentoja";

   if (emitfl != 1) {
	  writeerror_w(L"Leimantarkastustoimintoa ei ole käynnistetty", 0, true);
	  return(0);
      }
   ajat_on = ajat_on_loc;
   if (ajat_on) status_on = 0;
   if (regnly[0] == LID_MTR || regnly[1] == LID_MTR || regnly[NREGNLY-1] == LID_MTR)
	  wcscpy(valtxt, valtxt_m);
   clrscr();
   kehys(1);
   for (k = k_pv; k < kilpparam.n_pv_akt; k++)
	   kilp.pv[k].badge[0] = 0;
   lueseuraava(&kilpno, &em);
   for (;;) {
//      vidspwmsg(ySize-3,0,7,0,valtxt);
//      vidspwmsg(ySize-1,0,7,0,numval);
      clrtxt(2,0,64);
		mene(15, 1);
	  ct = 0;
	  if (!kilpno || (d = getpos(kilpno)) <= 0) {
         badge = em.badge;
         swprintf(line, L"Kilpailija %4d, kortti %6ld", kilpno, badge);
         clrtxt(0, 0, 33);
		 vidspwmsg(0,0,7,0, line);
         if (selaus) vidspwmsg(0,50,7,0,L"SELAUS");
         else clrtxt(0,50,56);
         vidint(0, 57, 5, emitptr+1);
         for (i = 3; i < ySize-4; i++) clrtxt(i,0,64);
         if (em.time != TMAALI0/KSEK) {
			swprintf(line, L"Lukemisaika %8.8s", aikatowstr_ls(st, em.time, t0));
			vidspwmsg(3, 7, 7, 0, line);
			}
			if (em.maali != TMAALI0/KSEK) {
				swprintf(line, L"Maaliaika %8.8s", aikatowstr_ls(st, em.maali, t0));
			vidspwmsg(3, 30, 7, 0, line);
			}
		 if (emithead2) naytatiedot(&em, NULL, 0);
		 }
	  else {
		 kilp.GETREC(d);
		 line[18] = 0;
		 if (kilp.maali(0) != TMAALI0) {
			swprintf(line, L"Maaliaika %8.8s", AIKATOWSTRS(st, kilp.maali(0), t0));
            }
         else
			wmemset(line, L' ', 18);
			vidspwmsg(3, 33, 7, 0, line);
		 if ((ep = getem(&em, kilp.id(), 0)) >= 0)
            emitptr = ep;
         else
			emitptr = emithead2 - 1;
//         badge = kilp.pv[k_pv].badge[0];
         badge = em.badge;
         wcscpy(selitys, kilp.pv[k_pv].selitys);
		 ct = outleimat(&kilp, &em, selaus, loppuosa);
         mene(55, 14);
         }

      trk = 0;

      do {
         vidspwmsg(ySize-3,0,7,0,valtxt);
		 vidspwmsg(ySize-1,0,7,0,numval);

         // Tässä loopissa odotetaan tarkastusnäppäilyä tai uuden tietueen
		 // valintaa. myös M)TR ja tU)hoa, kO)odit sekä Tab tunnistetaan.

			for (;;) {
            key = 0;
            if (!kbready(&che, &key)) {
               che = 0;
				if ((res = pros_emit(0, 0)) <= 0) {
					if (res < 0)
						return(0);
					Sleep(100);
					}

				if (!taustaon || vaiheenvaihto)
					goto loppu;

               // jos selaus ei ole käytössä ja emithead2 muuttuu, haetaan
               // viimeinen tietue

			   if (!selaus && (emithead2 != emithead0 || emitptr < em_HaeSeuraava(emitptr) || kilpno != emitptr_kno)) {
				  emitptr = em_HaeSeuraava(emithead2-1);
				  che = 'V';
				  emitptr_kno = kilpno;
				  }
			   else
				  continue;
			   }
				if (che == 8)
					ch =235;
				else
					ch = che;
			ch = towupper(ch);
			if (ch == 27)
				goto loppu;
			if (!ch) {
			   switch (key) {
				  case 21:
					 ch = 21;
					 break;
				  case 22:      // Alt-U
				  case 36:      // Alt-J
				  case 50:      // Alt-M
					 ch = 1;
					 break;
#ifdef EI_OLE
				  case 25:      // Alt-P
					 ch = 236;
					 break;
#endif
				  case 35:      // Alt-H
				  case 37:      // Alt-K
					 ch = 235;
					 break;
                  case 20:
					 pollaa = 1;   // Alt-T
                     break;
                  case 75:
                  case 77:                         
                     ch = 4;
                     break;
                  case 81:
                     ch = L'S';
                     break;
                  case 73:
					 ch = L'D';
                     break;
                  case 118:
                     ch = L'V';
					 break;
                  case 132:
                     ch = L'A';
                     break;
                  }
               }
            ch = towupper(ch);
            if (ch == 27) goto loppu;
            if (!ch ||
			   wcswcind(ch, L"0123456789THKIPR XSZADEGUMNVOL\x01\x09\x15\x14\x13\x05\x04\xeb\xec\r") >= 0)
               break;
            readkbd(&che,0,spoolfl);
			ch = che;
				}

         // Loopista poistutaan, jos näppäin koskee tietueen vaihtoa,
         // tuhoamista, käsiteltäväksi palauttamista tai MTR-ohjausta.

         if (ch && wcswcind(ch, L"0123456789AGDUMNSVXZ\x09\xec") >= 0)
			break;
         if (ch == L'\r') ch = ct;
         readkbd(&che,0,spoolfl);
		 ch2 = che;
         if (ch == L'O') {
            editkoodit();
            break;
            }
         if (ch == 1) {
            ohj_aikaruutu((char *) &che);
			ch2 = che;
			ch = 0;
			continue;
            }
         if (kilpno) {
            switch (ch) {
               case 19 :
                  e_sarja(&em, &kilp, d);
                  break;
               case 20 :
                  e_maali(&em, &kilp, d);
                  break;
#if !defined(ESITARK)
			   case 21 :         // Ctrl-U
                  emitva_uusinta(-1, ep);
                  break;
#endif
               case L'T':
                  trk = L'T';
				  break;
					case L'H':
                  trk = L'H';
                  break;
               case L'K':
                  trk = L'K';
                  break;
               case L'I':
                  trk = L'I';
				  break;
               case L'P':
                  trk = L'P';
                  break;
               case L' ':
                  trk = L'-';
                  break;
				case L'L':
				  if (Sarjat[kilp.Sarja()].rrtas > 1) {
					  lisaaika(&kilp, d);
					  }
				  ch = L'R';
				  break;
               case L'R':
                  tark_rap(emittulparam, kilpno);
                  break;
               case 235:
                  korjfl = 1;
                  entkilp = kilp;
				  kilp.outkilp(0);
				  korjaa(d, &kilp, 0, &entkilp);
				  for (i = 3; i < ySize-4; i++) clrtxt(i,0,64);
				  outleimat(&kilp, &em, selaus, loppuosa);
                  korjfl = 0;
                  ch = 5;
                  break;
			   case 5 :
                  editemit(ep, &em, &kilp);
                  break;
               case 4 :
                  loppuosa = !loppuosa;
				  break;
               }

				// näppäinpainallus poistetaan, jos tullaan raporttivalinnasta
				// tällöin poistutaan lisäksi loopista

            readkbd(&che,0,spoolfl);
			ch2 = che;
            if (ch == L'R' || ch == 19 || ch == 20 || ch == 5 || ch == 4) break;
            }

         // Jos painallus koskee tarkastusta, luetaan myös selitys sekä
         // pyydetään vahvistusta tietojen tallentamisesta

         if (trk) {
            viwrrectw(13, 54, 13, 54, &trk, 7, 0, 0);
            ct = trk;
			wcscpy(selitys, kilp.pv[k_pv].selitys);
			inputwstr(selitys, 14, 42, 15, L"\r\x1b", &ch, 0);
            if (ch == L'\x1b') break;
            vidspwmsg(16,42,7,0, L"Vahvista kirjaus (K/E)");
            mene(65, 17);
            do {
               ch = readkbd(&che,1,spoolfl);
               ch = towupper(ch);
               if (ch == 27) goto loppu;
               } while (ch != L'K' && ch != L'E');
            clrtxt(16,42,64);
            if (ch == L'E') {
               ch = 0;
               mene(55, 14);
			   }
            else {
               emit_tallenna(&kilp, trk, selitys, 0);
               }
            }
         } while(!ch);

      // Tähän tullaan, kun tarkastustieto on vahvistettu tai näppäily on
		// koskenut uuden tietueen valintaa, tuhoamista, MTR-ohjausta,
      // tietueen palauttamista käsittelyyn, raporttia tai koodien editointia.

      // Palataan pääloopin alkuun, jos tarkastustieto on hyväksytty
      // tai on poistuttu raportti- tai koodieditointitoiminnosta

      if (ch == L'R' || ch == L'K' || ch == L'O' || ch == 19 || ch == 20 ||
		 ch == 5 || ch == 4)
         continue;

      // Tuhoamisen jälkeen siirrytään seuraavaan tietueseen.
      // Jos tuhottu tietue oli viimeinen, siirrtytään viimeiseen
      // jäljelläolevaan.

      if (ch == L'U') {
			poista_emit(&em);
			ch = L'S';
         }

      // MTR-ohjauksen jälkeen siirrytään viimeiseen tietueeseen

      if (ch == L'M') {
         aseta_mtr();
         ch = L'V';
         }

	  if (ch == L'N') {
         seur_mtr();
         ch = L'V';
			}

      if (ch == L'X') {
         vaihdakilp(kilpno);
         aani0 = aani;
         aani = 0;
         tark_badge(&em);
         aani = aani0;
         lueedellinen(&kilpno, &em);
         ch = L'S';
         }
#ifdef EI_OLE
      if (ch == 236) {     // Alt-P
		 emituudelleen();
         ch = L'S';
         }
#endif
      // Tässä käsitellään tietueen vaihto selaamalla tai hyppäämällä
      // viimeiseen

      if (emithead2 && (ch == L'S' || ch == L'D' || ch == L'A' || ch == L'V'
			|| ch == L'G' || ch == L'Z')) {

         // Aluksi poistetaan näppäintieto

         readkbd(&che,0,spoolfl);
		 ch2 = che;

         // Jos tiedot ovat muuttuneet, kysytään vahvistusta tallentamisesta

         if (kilpno && ((trk && trk != kilp.tark(k_pv)) ||
            wcscmp(selitys, kilp.pv[k_pv].selitys))) {
            cs = L' ';
            wselectopt(L"T)allenna näytön tiedot, O)hita muutokset, P)alaa tietoihin",
               L"TOP", &cs);
            switch (cs) {
               case L'T' :
						emit_tallenna(&kilp, trk, selitys, 0);
						break;
               case L'P' :
				  continue;
               }
            }

         switch (ch) {
			case L'G':
               clrln(ySize-3);
               vidspwmsg(ySize-3,0,7,0,L"Anna järjestysnumero");
               INPUTINTW(&emitptr, 5, 22, ySize-3, L"\r", &ch);
               emitptr--;
               ch = L'S';
					if (emitptr < 0)
                  emitptr = 0;
               emitptr--;
            case L'V':
               if ((ch == L'V' && emithead2 > 1) || emitptr >= emithead2)
                  emitptr = emithead2 - 1;
               //  case jatkuu tarkoituksella
            case L'S':
               emptr = emitptr;
               lueseuraava(&kilpno, &em);
               if (emitptr <= emptr) {
                  selaus = 0;
				  emithead0 = emithead2;
                  }
               ch = 0;
               break;
            case L'Z':
               emptr = emitptr;
               lueseuraavaong(&kilpno, &em);
			   if (emitptr <= emptr) {
                  selaus = 0;
                  emithead0 = emithead2;
                  }
               ch = 0;
               break;
            case L'A':
               if (emithead2 > 1)
                  emitptr = 1;
               //  case jatkuu tarkoituksella
            case L'D':
               lueedellinen(&kilpno, &em);
               selaus = 1;
               ch = 0;
               break;
            }

			// Seuraavan pitäisi toteutua aina
			// Päälooppi aloitetaan alusta

         if (ch == 0) {
            loppuosa = 0;
            continue;
			}
         }

      // Tabulaattori vaihtaa kilpno / badge - valinnan

      if (ch == 9) {
         bdgfl = !bdgfl;
         readkbd(&che,0,spoolfl);
		 ch2 = che;
		 }

      // Tähän looppiin tullaan, kun on painettu numeroa tai tabulaattoria
      // Nyt odotetaan numeron näppäilyä

      do {
         clrln(ySize-3);
		 vidspwmsg(ySize-3,0,7,0,L"Esc: poistu numerovalinnasta");
         vidspwmsg(ySize-1,0,7,0,numval);
         clrtxt(2,0,64);
         clrtxt(3,0,64);
		 if (!bdgfl) {
            if (ch != L'\r') kilpno = 0;
            INPUTINTW(&kilpno, 4, 11, 0, L"\r\x1b\x09", &ch);
			}
         else {
            if (ch != L'\r') badge = 0;
            inputlongw(&badge, 6, 24, 0, L"\r\x1b\x09", &ch);
            if (ch == L'\r') kilpno = bdg2kno(badge);
            }
         if (ch == ESC) {
            ch = 0;
            break;
            }
         if (ch == 9) {
            bdgfl = !bdgfl;
            ch = 0;
            continue;
            }
         if ((d = getpos(kilpno)) < 1) {
			vidspwmsg(3,0,7,0, L"Kilpailijaa ei löydy");
			ch = 0;
			}
		 else {
			kilp.GETREC(d);
			if (kilp.tark(k_pv) == L'P') {
				vidspwmsg(3,0,7,0, L"Kilpailijaa poissaoleva");
				ch = 0;
				}
			else
				selaus = 1;
			}

			// luupissa pysytään, jos on painettu tabulaattoria tai
			// syötetty tuntematon numero

			} while (!ch);
		loppuosa = 0;
		}
	loppu:
	clearscreen();
	status_on = 1;
	return(0);
	}
#endif \\ _CONSOLE

void emitlista(tulostusparamtp *tulprm)
   {
   static INT16 elistaalku = 1;
   INT r, d, l, i;
	INT16 elistaloppu;
   INT32 enn;
	wchar_t ch, *p, line[80], as[16], asmaali[16], as1[16], as2[16], st[40];
	emittp em;
	kilptietue kilp;

	if (tulprm->lstf == NULL)
		return;
	clrln(ySize-3);
#ifdef _CONSOLE
   vidspwmsg(ySize-3,0,7,0,L"Anna alku- ja loppurivit (Esc: peruuta):");
   inputintw(&elistaalku, 4, 42, ySize-3, L"\r\x1b", &ch);
#else
   i = elistaalku;
   inputint_prompt(&i,L"Anna alkurivi", &ch);
   elistaalku = i;
#endif
   if (ch == 27) {
	  clrln(ySize-3);
	  return;
	  }
   if (elistaalku < 1) elistaalku = 1;
   elistaloppu = 9999;
#ifdef _CONSOLE
   inputintw(&elistaloppu, 4, 49, ySize-3, L"\r\x1b", &ch);
#else
   i = elistaloppu;
   inputint_prompt(&i,L"Anna loppurivi", &ch);
   elistaloppu = i;
#endif
   clrln(ySize-3);
   if (ch == 27) return;
   elistaloppu = min(elistaloppu, emithead2);
   EnterCriticalSection(&emit_CriticalSection);
   EnterCriticalSection(&print_CriticalSection);
   _lseek(emitfile, sizeof(emittp) * (INT32)(elistaalku - 1), SEEK_SET);
	p = 0;
	if (tulprm->printer == LJETII)
		p = initfont_emrap;
	if (tulprm->printer == GDIPRINTER)
		p = L"C";
	initpage(tulprm, p);
	initline(tulprm);
	l = 0;
   for (r = elistaalku-1; r < elistaloppu; r++) {
	  if (_read(emitfile, &em, sizeof(emittp)) < (int)sizeof(emittp)) break;
      kilp.nollaa();
	  wmemset(asmaali, L' ', 8);
	  if ((d = getpos(em.kilpno)) > 0) {
		 kilp.GETREC(d);
		 if (kilp.p_aika(0))
			AIKATOWSTRS(asmaali, kilp.maali(0), t0);
		 }
	  asmaali[8] = 0;
	  if (em.maali != TMAALI0/KSEK)
		aikatowstr_ls(as, em.maali, t0);
	  else
		aikatowstr_ls(as, em.time, t0);
	  enn = haeennakko(&em, &kilp);
	  for (i = MAXNLEIMA-1; i >= 0; i--) if (em.ctrltime[i]) break;
	  aikatowstr_ls(as1, 10L*(em.ctrltime[i]-enn), 0);
	  as1[8] = 0;
		as2[0] = 0;
		if (i > 1)
			aikatowstr_ls(as2, 10L*(em.ctrltime[i-1] - enn), 0);
	  as2[8] = 0;
		swprintf(line, L"%4d %8.8s %8.8s %4d %6ld %-17.17s %c %8.8s (%8.8s)",
			r+1, as, asmaali, em.kilpno, em.badge, kilp.nimi(st, 20, false), kilp.tark(k_pv),
			as1, as2);
	  putfld(tulprm, line, 1, wcslen(line), 0, 0);
	  endline(tulprm, 0);
	  if (l == 60) {
		 endpage(tulprm);
			initpage(tulprm, p);
		 l = 0;
		 }
	  }
   enddoc(tulprm);
  LeaveCriticalSection(&print_CriticalSection);
   LeaveCriticalSection(&emit_CriticalSection);
   elistaalku = r + 1;
   }

void rastivaots_on(tulostusparamtp *tulprm);
void rastiva_on(tulostusparamtp *tulprm);
void rastiva_pois(tulostusparamtp *tulprm);

void tulostaRastiVa(tulostusparamtp *tulprm, int kilpno)
{
	ratatp *rt;
	kilptietue kilp, kilp1;
	emittp em;
	emitvaanaltp *eva = 0;
	rastivaanaltp *rstva = 0;
	int d, i, r, p, lj, sj1, ntlsrivi = 62;
	int edaika, tls, t1, karki, emvaluku, srj, sj, ero;
	int xva[10] = {0, 57, 80, 160, 190, 250, 330, 340, 395, 475};
	int xsrj[4] = {540, 575, 755, 830};
	wchar_t *vaots[10] = {L"Rasti", L"Kdi", L"Aika", L"Sija", L"Ero", L"Väli", L"Sija", L"Paras", L"Ero", L"M/km"};
	wchar_t *srjots[7] = {L"Sj.", L"Nimi", L"Aika", L"Ero"};
	wchar_t line[121], st[61], as[13];

	if (tulprm->lstf == NULL || (d = getpos(kilpno)) < 1)
		return;
	kilp.GETREC(d);
	rt = haerata(&kilp);
	if (!rt || !rt->rastiluku) return;
	srj = kilp.Sarja();
	sijat(srj);
	EnterCriticalSection(&print_CriticalSection);
	initpage(tulprm, 0);
	initline(tulprm);
	rastivaots_on(tulprm);
	putfld(tulprm, kilpailu,0,70,0,0);
	endline(tulprm, 1);
	swprintf(line, L"%d  %s  -  %s", kilpno, kilp.nimi(st, 30, 0), kilp.seura);
	putfld(tulprm, line, 0, 80, 0, 0);
	endline(tulprm, 1);
	tls = kilp.tulos_pv(k_pv);
	if (tls && kilp.hyv()) {
		AIKATOWSTRS(st, tls, 0);
		st[pvparam[k_pv].laikasj] = 0;
		elimwzb1(st);
		swprintf(line, L"%s  -  %d.   %s", Sarjat[srj].sarjanimi, kilp.sija(k_pv), st);
		}
	else {
		swprintf(line, L"%s  -  %s", Sarjat[srj].sarjanimi, kilp.tarkstr(st, k_pv, false, 10, 0));
		}
	putfld(tulprm, line, 0, 80, 0, 0);
	endline(tulprm, 2);
	rastiva_on(tulprm);
	r = tulprm->vpos;
	tulprm->vpos -= 11*tulprm->lstf->u.wp.GDIparam.Currentfont.line/2;
	swprintf(line, L"Tulostettu %s - %s", wpaiva(), wkello());
	putfld(tulprm, line, xsrj[1], 30, 0, 0);
	endline(tulprm, 0);
	swprintf(line, L"Tuloksia %d, avoinna %d", ntulos[srj][0], n_avoin(srj));
	putfld(tulprm, line, xsrj[1], 27, 0, 0);
	endline(tulprm, 0);
	getem(&em, kilpno, 0);
	if (em.kilpno == kilpno) {
		EnterCriticalSection(&rastiva_CriticalSection);
		laskeemitvaliajat(&em, &kilp, &eva, &rstva, &emvaluku);
		if (eva) {
			swprintf(line, L"Rata %s, väliaikatietoja %d", rt->tunnus, rastiva == NULL ? 0 : rastiva[rt-rata].nva);
			putfld(tulprm, line, xsrj[1], 50, 0, 0);
			endline(tulprm, 0);
			if (rvalit != 0) {
				swprintf(line, L"Rastivälien tietoja %d", rvalit[eva[0].rvno].lkm[srj]);
				putfld(tulprm, line, xsrj[1], 35, 0, 0);
				endline(tulprm, 0);
				}
			tulprm->vpos = r - 4*tulprm->lstf->u.wp.GDIparam.Currentfont.line/3;
			putfld(tulprm, L"Tilanne rasteilla", xva[2]+30, 20, 0, 0);
			putfld(tulprm, L"Rastivälit", xva[5]+40, 12, 0, 0);
			tulprm->vpos = r;
			edaika = 0;
			putfld(tulprm, vaots[0], xva[0], 5, 1, 0);
			putfld(tulprm, vaots[1], xva[1], 3, 1, 0);
			putfld(tulprm, vaots[2], xva[2], 8, 1, 0);
			putfld(tulprm, vaots[3], xva[3], 3, 1, 0);
			putfld(tulprm, vaots[4], xva[4], 8, 1, 0);
			putfld(tulprm, vaots[5], xva[5], 8, 1, 0);
			putfld(tulprm, vaots[6], xva[6], 3, 1, 0);
			putfld(tulprm, vaots[7], xva[7], 8, 1, 0);
			putfld(tulprm, vaots[8], xva[8], 8, 1, 0);
			putfld(tulprm, vaots[9], xva[9], 5, 1, 0);
			endline(tulprm, 1);
			for (i = 0; i < emvaluku; i++) {
				int mtk, kmaika = 0;

				putfld_int(tulprm, i+1, xva[0], 5, 1, 0);
				putfld_int(tulprm, eva[i].rkoodi%1000, xva[1], 3, 1, 0);
				if (maalileimasin > -1 && rt->maalilaji > 1 && i == emvaluku-1 && tls)
					ero = tls / SEK - eva[i].aika;
				else
					ero = 0;
				aikatowstr_s(as, eva[i].aika+ero, 0);
				elimwzb1(as);
				putfld(tulprm, as, xva[2], 8, 1, 0);
				if (rstva) {
					putfld_int(tulprm, rstva[i].sj, xva[3], 3, 1, 0);
					t1 = rstva[i].aika - rstva[i].kaika + ero;
					aikatowstr_s(as+1, abs(t1), 0);
					elimwzb1(as+1);
					as[0] = (tls >= 0) ? L'+' : L'-';
					putfld(tulprm, as, xva[4], 8, 1, 0);
					}
				aikatowstr_s(as, eva[i].rvaika+ero, 0);
				elimwzb1(as);
				putfld(tulprm, as, xva[5], 8, 1, 0);
				if (rvalit != NULL && eva[i].rvno >= 0) {
					sj = bSrchInt(eva[i].rvaika, rvalit[eva[i].rvno].ajat[srj],
						rvalit[eva[i].rvno].lkm[srj]) + 1;
					putfld_int(tulprm, sj, xva[6], 3, 1, 0);
					karki = rvalit[eva[i].rvno].ajat[srj][0];
					aikatowstr_s(as, karki, 0);
					elimwzb1(as);
					putfld(tulprm, as, xva[7], 8, 1, 0);
					t1 = eva[i].rvaika - karki + ero;
					aikatowstr_s(as+1, abs(t1), 0);
					elimwzb1(as+1);
					as[0] = (tls >= 0) ? L'+' : L'-';
					putfld(tulprm, as, xva[8], 8, 1, 0);
					if ((mtk = rvalit[eva[i].rvno].matka) > 10) {
						if (eva[i].rvaika) {
							kmaika = (100000 * (eva[i].rvaika+ero) + 30 * mtk) / (60 * mtk);
							swprintf(as, L"%2d,%02d", kmaika/100, kmaika%100);
							putfld(tulprm, as, xva[9], 5, 1, 0);
							}
						}
					}
				endline(tulprm, 0);
				}
/*
			if (rt->maalilaji < 2 || maalileimasin < 0) {
				putfld(tulprm, st, xva[2], 8, 1, 0);
				if (edaika) {
					aikatowstr_s(as, kilp.tulos_pv(k_pv)/SEK - edaika, 0);
					elimwz(as);
					putfld(tulprm, as, xva[3], 8, 1, 0);
					}
				endline(tulprm, 0);
				}
*/
			free(eva);
			free(rstva);
			}
		LeaveCriticalSection(&rastiva_CriticalSection);
		}
	tulprm->vpos = r;
	putfld(tulprm, L"|", xsrj[0]-10, 1, 0, 0);
	putfld(tulprm, srjots[0], xsrj[0], 3, 1, 0);
	putfld(tulprm, srjots[1], xsrj[1], 5, 0, 0);
	putfld(tulprm, srjots[2], xsrj[2], 8, 1, 0);
	putfld(tulprm, srjots[3], xsrj[3], 8, 1, 0);
	endline(tulprm, 0);
	putfld(tulprm, L"|", xsrj[0]-10, 1, 0, 0);
	endline(tulprm, 0);
	sj = kilp.sija(k_pv);
	p = jalku[srj][0];
	lj = 0;
	sj1 = 1;
	for (r = 0; r < ntlsrivi && lj <= 2 && p > 0; r++) {
		int naskel;

		kilp1.GETREC(p);
		if (kilp1.id() == kilp.id()) {
			bold_on(tulprm);
			}
		putfld(tulprm, L"|", xsrj[0]-10, 1, 0, 0);
		if (kilp1.sija(k_pv) == sj1)
			putfld_int(tulprm, sj1, xsrj[0], 3, 1, 0);
		putfld(tulprm, kilp1.nimi(st,25,false), xsrj[1], 25, 0, 0);
		if (kilp1.hyv()) {
			AIKATOWSTRS(st, kilp1.tulos_pv(k_pv), 0);
			st[8] = 0;
			elimwzb1(st);
			}
		else if (kilp1.tark() == L'K') {
			wcscpy(st, L"Kesk.");
			}
		else if (kilp1.tark() == L'H') {
			wcscpy(st, L"Hyl.");
			}
		putfld(tulprm, st, xsrj[2], 8, 1, 0);
		if (r == 0)
			karki = kilp1.p_aika(0);
		else if (kilp1.hyv() && kilp1.sija(k_pv) > 1) {
			AIKATOWSTRS(st+1, kilp1.tulos_pv(k_pv)-karki, 0);
			st[9] = 0;
			elimwzb1(st+1);
			st[0] = L'+';
			putfld(tulprm, st, xsrj[3], 8, 1, 0);
			}
		if (kilp1.id() == kilp.id()) {
			bold_pois(tulprm);
			}
		endline(tulprm, 0);
		naskel = 0;
		do {
			p = SeuraavaSrj(srj, 0, p);
			while (p < 0 && lj < 2)
				p = jalku[srj][++lj];
			if (lj == 0)
				sj1++;
			else
				sj1 = 0;
			naskel++;
			}
		while (lj == 0 && sj > ntlsrivi && r == ntlsrivi - 7 && sj1 < sj-2);
		if (naskel > 1) {
			newline(tulprm, 1);
			r++;
			}
		}
	LeaveCriticalSection(&print_CriticalSection);
	rastiva_pois(tulprm);
	endpage(tulprm);
	enddoc(tulprm);
}

void prtemitsivu(tulostusparamtp *tulprm)
   {
   if (emitosa) {
	  endpage(tulprm);
	  emitosa = 0;
	  }
   }

void emit_rap(tulostusparamtp *tulprm, INT kilpno)
   {
   ratatp *rt;
   emittp em;
   INT d, i, rlisa = 0, r, tulkinta[MAXNLEIMA+1], kokosivu, rst[4], nr;
   INT32 enn, edaika, tls;
	wchar_t line[81], ln1[81], ln2[81], st[61], as[13], aero[13];
   kilptietue kilp;

   if (tulprm->lstf == NULL)
	   return;
   if ((d = getpos(kilpno)) > 0) {
		kilp.GETREC(d);
	  }
   else
	  return;
   rt = haerata(&kilp);
   if (!rt || !rt->rastiluku) return;
	getem(&em, kilpno, 0);
   i = tarkista(&em, tulkinta, 0, 0);
   EnterCriticalSection(&print_CriticalSection);
   if (emitsivu > 0 && i) {
	  kokosivu = 1;
	  prtemitsivu(tulprm);
	  }
   else
	  kokosivu = (emitsivu == 0);
   if (emitsivu == -1)
	   kokosivu = 1;
   if (!emitosa) {
		initpage(tulprm, tulprm->printer == LJETII ? initfont_emrap : L"C");
		initline(tulprm);
		}
	if (kokosivu && tarkrappageskip)
		endline(tulprm, tarkrappageskip-1);
	if (tulprm->language == EN)
		swprintf(line, L"Printed %s at %s", wpaiva(), wkello());
	else
		swprintf(line, L"Tulostettu %s kello %s", wpaiva(), wkello());
   putfld(tulprm, line, emitsivu == -1 ? 0 : 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
   endline(tulprm, kokosivu);
   if (emitsivu == -1) {
		swprintf(line, L"Nro %d", kilpno);
		if (kilp.pv[k_pv].badge[1]) {
			swprintf(line, L"  Emit %d", kilp.pv[k_pv].badge[1]);
			}
		else if (kilp.pv[k_pv].badge[0]) {
			swprintf(line, L"  Emit %d", kilp.pv[k_pv].badge[0]);
			}
		putfld(tulprm, line, 0, wcslen(line), 0, 0);
		endline(tulprm, 0);
		swprintf(line, L"%s", kilp.nimi(st, 38, false));
		putfld(tulprm, line, 0, wcslen(line), 0, 0);
		endline(tulprm, 0);
		swprintf(line, L"%s", Sarjat[kilp.Sarja()].sarjanimi);
		if (rt->tunnus[0]) {
			swprintf(line+wcslen(line), L" / %s", rt->tunnus);
			}
		putfld(tulprm, line, 0, wcslen(line), 0, 0);
	   }
   else if (kokosivu) {
		if (tulprm->language == EN)
			swprintf(line, L"Participant no     %d", kilpno);
		else
			swprintf(line, L"Kilpailija nro     %d", kilpno);
		putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
		endline(tulprm, 0);
		if (tulprm->language == EN)
			swprintf(line, L"Name               %s", kilp.nimi(st, 38, false));
		else
			swprintf(line, L"Nimi               %s", kilp.nimi(st, 38, false));
		putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
		endline(tulprm, 0);
		if (tulprm->language == EN)
			swprintf(line, L"Class              %s", Sarjat[kilp.Sarja()].sarjanimi);
		else
			swprintf(line, L"Sarja              %s", Sarjat[kilp.Sarja()].sarjanimi);
	  if (rt->tunnus[0]) {
		 swprintf(line+wcslen(line), L" / %s", rt->tunnus);
		 }
	   putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	   }
   else {
		if (tulprm->language == EN)
			swprintf(line, L"Class %s, participant %d, %s",
				Sarjat[kilp.Sarja()].sarjanimi, kilpno, kilp.nimi(st, 38, false));
		else
			swprintf(line, L"Sarja %s, kilpailija %d, %s",
				Sarjat[kilp.Sarja()].sarjanimi, kilpno, kilp.nimi(st, 38, false));
		putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
		}
   endline(tulprm, 0);
   if (emitsivu >= 0 && kokosivu) {
		if (tulprm->language == EN)
			swprintf(line, L"Start time         %8.8s", AIKATOWSTRS(st, kilp.pv[k_pv].tlahto, t0));
		else
			swprintf(line, L"Lähtöaika          %8.8s", AIKATOWSTRS(st, kilp.pv[k_pv].tlahto, t0));
		putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
		endline(tulprm, 0);
		}
	tls = kilp.tulos_pv(k_pv);
	if (emitsivu == -1) {
		swprintf(line, L"Tulos %8.8s", AIKATOWSTRS(st, tls, 0));
		putfld(tulprm, line, 0, wcslen(line), 0, 0);
		endline(tulprm, 0);
		line[0] = 0;
		}
	else {
		if (tulprm->language == EN)
			swprintf(line, L"Result             %8.8s", AIKATOWSTRS(st, tls, 0));
		else
			swprintf(line, L"Tulos              %8.8s", AIKATOWSTRS(st, tls, 0));
	   if (kokosivu) {
			putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
			endline(tulprm, kokosivu);
			if (tulprm->language == EN)
				swprintf(line, L"Emit card no    %ld  manufactured %d-%d  counters %d/%d/%d   %s",
					em.badge, em.badgeyear, em.badgeweek, em.sc, em.pc, em.lc,
					lainakortti(&kilp) ? L"Lent card" : L"");
			else
				swprintf(line, L"Emit-kortti nro    %ld  valmistettu %d-%d  laskurit %d/%d/%d   %s",
					em.badge, em.badgeyear, em.badgeweek, em.sc, em.pc, em.lc,
					lainakortti(&kilp) ? L"Laina" : L"");
			putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
			endline(tulprm, 0);
			if (tulprm->language == EN) {
				if (!em.package)
					swprintf(line, L"Card read %8.8s", aikatowstr_ls(st, em.time,t0));
				else
					swprintf(line, L"Card read %8.8s,   msg no %lu",
						aikatowstr_ls(st, em.time,t0), em.package);
				}
			else {
				if (!em.package)
					swprintf(line, L"Kortti luettu %8.8s", aikatowstr_ls(st,em.time,t0));
				else
					swprintf(line, L"Kortti luettu %8.8s,   sanoma %lu",
						aikatowstr_ls(st,em.time,t0), em.package);
				}
			putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
			endline(tulprm, 0);
			if (tulprm->language == EN)
				wcscpy(line, L"Result of checking : ");
			else
				wcscpy(line, L"Tarkastuksen tulos : ");
			}
		else
			wcscat(line, L",    ");
		}
	if (tulprm->language == EN) {
		switch (kilp.tark(k_pv)) {
			case L'T':
				wcscat(line, L"Accepted");
				break;
			case L'I':
				wcscat(line, L"DQ proposed");
				break;
			case L'H':
				wcscat(line, L"DQ");
				break;
			case L'M':
				wcscat(line, L"Missing");
				break;
			case L'K':
				wcscat(line, L"DNF");
				break;
			case L'X':
				wcscat(line, L"Exclude");
				break;
			default:
				wcscat(line, L"Control result unknown ");
				break;
			}
		}
	else {
		switch (kilp.tark(k_pv)) {
			case L'T':
				wcscat(line, L"Hyväksytty");
				break;
			case L'I':
				wcscat(line, L"Hylkäysesitys");
				break;
			case L'H':
				wcscat(line, L"Hylätty");
				break;
			case L'M':
				wcscat(line, L"Puuttuu");
				break;
			case L'X':
				wcscat(line, L"Ei tulosta");
				break;
			case L'K':
				wcscat(line, L"Keskeyttänyt");
				break;
			default:
				wcscat(line, L"Leimantarkastus avoinna");
				break;
			}
		}
	putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	endline(tulprm, 1);
	edaika = 0;
	enn = haeennakko(&em, &kilp);
	if (emitsivu == -1) {
		wcscpy(line, L"Rasti Koodi Leima  Aika  Rastiväli");
		putfld(tulprm, line, 0, wcslen(line), 0, 0);
		endline(tulprm, 1);
		for (i = 0; i < MAXNLEIMA; i++) {
			if (i && !em.ctrlcode[i] && !em.ctrltime[i])
				break;
			aero[0] = 0;
			if (tulkinta[i] < 0) {
				swprintf(st, L"(%2d)", -tulkinta[i]);
				r = -tulkinta[i] - 1;
				}
			else {
				swprintf(st, L" %2d ", tulkinta[i]);
				r = tulkinta[i] - 1;
				}
			if (r >= 0 && r <= rt->rastiluku) {
				aero[0] = L' ';
				aikatowstr_ls(aero+1,10L*(em.ctrltime[i]-enn-edaika),0);
				aero[9] = 0;
				elimwzb1(aero+1);
				edaika = em.ctrltime[i]-enn;
				}
			if (em.ctrltime[i] - enn < 0) {
				as[0] = L'-';
				aikatowstr_ls(as+1,10L*(enn-em.ctrltime[i]),0);
				}
			else {
				as[0] = L' ';
				aikatowstr_ls(as+1,10L*(em.ctrltime[i]-enn),0);
				}
			as[9] = 0;
			elimwzb1(as+1);
			if (i == 0)
				wcscpy(st, L"Nollaus");
			if (r == rt->rastiluku && em.ctrlcode[i] > 248) {
				wcscpy(st, L"Lukija");
				if (aero[0]) {
					aero[0] = L'(';
					wcscat(aero, L")");
					}
				as[0] = L'(';
				wcscat(as, L")");
				}
			rst[0] = 0;
			nr = 4;
			haerastit(em.ctrlcode[i], rst, &nr);
			if (r >= 0 && r < rt->rastiluku) {
				if (rt->rastikoodi[r+rlisa] > 9999)
					rlisa++;
				r = rt->rastikoodi[r+rlisa];
				swprintf(line, L"%5.5s %3d  %3d %9.9s%9.9s",
					st, r, em.ctrlcode[i], as, aero);
				}
			else {
				swprintf(line, L"%5.5s %3d  %3d %9.9s%9.9s",
					st, rst[0], em.ctrlcode[i], as, aero);
				}
			putfld(tulprm, line, 0, wcslen(line), 0, 0);
			endline(tulprm, 0);
			}
		}
	else if (kokosivu) {
		if (tulprm->language == EN)
			wcscpy(line, L"   Control   Cntrlcode  Emit-code      Time  Leg time ");
		else
			wcscpy(line, L"   Rastinro  Rastikoodi Emitkoodi      Aika  Rastiväli");
		putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
		endline(tulprm, 1);
		for (i = 0; i < MAXNLEIMA; i++) {
			if (i && !em.ctrlcode[i] && !em.ctrltime[i])
				break;
			aero[0] = 0;
			if (tulkinta[i] < 0) {
				swprintf(st, L"(%2d) ", -tulkinta[i]);
				r = -tulkinta[i] - 1;
				}
			else {
				swprintf(st, L" %2d  ", tulkinta[i]);
				r = tulkinta[i] - 1;
				}
			if (r >= 0 && r <= rt->rastiluku) {
				aero[0] = L' ';
				aikatowstr_ls(aero+1,10L*(em.ctrltime[i]-enn-edaika),0);
				aero[9] = 0;
				elimwzb1(aero+1);
				edaika = em.ctrltime[i]-enn;
				}
			if (em.ctrltime[i] - enn < 0) {
				as[0] = L'-';
				aikatowstr_ls(as+1,10L*(enn-em.ctrltime[i]),0);
				}
			else {
				as[0] = L' ';
				aikatowstr_ls(as+1,10L*(em.ctrltime[i]-enn),0);
				}
			as[9] = 0;
			elimwzb1(as+1);
			if (tulprm->language == EN) {
				if (i == 0)
					wcscpy(st, L" Reset ");
				}
			else {
				if (i == 0)
					wcscpy(st, L"Nollaus");
				}
			if (r == rt->rastiluku && em.ctrlcode[i] > 248) {
				if (tulprm->language == EN)
					wcscpy(st, L"Reader");
				else
					wcscpy(st, L"Lukija");
				if (aero[0]) {
					aero[0] = L'(';
					wcscat(aero, L")");
					}
				as[0] = L'(';
				wcscat(as, L")");
				}
			rst[0] = 0;
			nr = 4;
			haerastit(em.ctrlcode[i], rst, &nr);
			if (r >= 0 && r < rt->rastiluku) {
				if (rt->rastikoodi[r+rlisa] > 9999)
					rlisa++;
				r = rt->rastikoodi[r+rlisa];
				swprintf(line, L"  %8s      %3d        %3d    %10.10s%10.10s",
					st, r, em.ctrlcode[i], as, aero);
				}
			else {
				swprintf(line, L"  %8s      %3d         %3d   %10.10s%10.10s",
					st, rst[0], em.ctrlcode[i], as, aero);
				if (nr > 1) {
					st[0] = 0;
					for (r = 0; r < nr; r++)
						swprintf(st+wcslen(st), L" %3d", rst[r]);
					wmemcpy(line+11, st, wcslen(st));
					}
				}
			putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
			endline(tulprm, 0);
			}
		}
	else {
	  line[0] = 0;
	  ln1[0] = 0;
	  ln2[0] = 0;
	  r = 0;
	  for (i = 0; i < MAXNLEIMA; i++) {
		 if (r != rt->rastiluku-1) {
			if (i && !em.ctrlcode[i] && !em.ctrltime[i]) break;
			aero[0] = 0;
			if (tulkinta[i] <= 0)
			   continue;
			swprintf(st, L"     %2d  ", tulkinta[i]);
			r = tulkinta[i] - 1;
			if (r >= rt->rastiluku) break;
			aero[0] = L' ';
			aikatowstr_ls(aero+1,10L*(em.ctrltime[i]-enn-edaika),0);
			aero[9] = 0;
			elimwzb1(aero+1);
			edaika = em.ctrltime[i]-enn;
			if (em.ctrltime[i] - enn < 0) {
			   as[0] = L'-';
			   aikatowstr_ls(as+1,10L*(enn-em.ctrltime[i]),0);
			   }
			else {
			   as[0] = L' ';
			   aikatowstr_ls(as+1,10L*(em.ctrltime[i]-enn),0);
			   }
			as[9] = 0;
			}
		 else {
			if (!tls)
				break;
			if (tulprm->language == EN)
				wcscpy(st, L"  Finish ");
			else
				wcscpy(st, L"  Maali  ");
			aero[0] = L' ';
			AIKATOWSTRS(aero+1,tls - SEK*edaika,0);
			aero[9] = 0;
			elimwzb1(aero+1);
			as[0] = L' ';
			AIKATOWSTRS(as+1, tls,0);
			as[9] = 0;
			r++;
			}
		 if (line[0] && (r % 8 == 0)) {
			putfld(tulprm, line, 2*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
			endline(tulprm, 0);
			putfld(tulprm, ln1, 2*tulprm->tulmuot.numlev, wcslen(ln1), 0, 0);
			endline(tulprm, 0);
			putfld(tulprm, ln2, 2*tulprm->tulmuot.numlev, wcslen(ln2), 0, 0);
			endline(tulprm, 0);
			line[0] = 0;
			ln1[0] = 0;
			ln2[0] = 0;
			}
		 elimwzb1(as+1);
		 wcscat(line, st);
		 swprintf(st,L"%9.9s", as);
		 wcscat(ln1, st);
		 swprintf(st,L"%9.9s", aero);
		 wcscat(ln2, st);
		 if (r == rt->rastiluku) break;
		 }
	  if (line[0]) {
		 putfld(tulprm, line, 2*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
		 endline(tulprm, 0);
		 putfld(tulprm, ln1, 2*tulprm->tulmuot.numlev, wcslen(ln1), 0, 0);
		 endline(tulprm, 0);
		 putfld(tulprm, ln2, 2*tulprm->tulmuot.numlev, wcslen(ln2), 0, 0);
		 endline(tulprm, 0);
		 }
	  }
   if (!kokosivu && emitosa < emitsivu) {
	  emitosa++;
	  newline(tulprm, 3);
	  }
   else {
	  endpage(tulprm);
	  emitosa = 0;
	  }
   enddoc(tulprm);
  LeaveCriticalSection(&print_CriticalSection);
   }

void tark_rap(tulostusparamtp *tulprm, INT kilpno)
	{
	wchar_t ch, *p, line[81], trk[40], l1[66], l2[66], l3[66], st[60];
   wchar_t viiva[61] =
	  L"____________________________________________________________";
   INT d, r;
   kilptietue kilp;

   if (tulprm == NULL)
		return;
   if ((d = getpos(kilpno)) > 0) {
	  kilp.GETREC(d);
	  }
   else
	  return;
   ch = L' ';
   wselectopt(L"K)ortin tiedot, L)uettelo tiedoista, H)ylkäysraportti",
	  L"KLH\x1b", &ch);
   clrln(ySize-3);
   if (ch == ESC) return;
   if (ch == L'K') {
	  emit_rap(tulprm, kilpno);
	  return;
	  }
   if (ch == L'L') {
	  ch = L' ';
	  wselectopt(L"L)ista luetuista korteista, H)ylätyistä, E)sityksistä",
		 L"LHE\x1b", &ch);
	  clrln(ySize-3);
	  if (ch == ESC) return;
	  if (ch == L'L') emitlista(tulprm);
	  else tarkraportit(tulprm, ch == L'E' ? L'I' : ch);
	  return;
	  }
   for (r = 2; r < ySize-4; r++) clrtxt(r,0,64);
   memset(l1, 0, sizeof(l1));
   memset(l2, 0, sizeof(l2));
   memset(l3, 0, sizeof(l3));
   swprintf(line, L"LEIMANTARKASTUSRAPORTTI");
   vidspwmsg(2,0,7,0,line);
   vidint(4,0,4,kilpno);
	vidspwmsg(4,10,7,0, kilp.nimi(st, 38, false));
   vidspwmsg(5,10,7,0, (wchar_t *)Sarjat[kilp.Sarja()].sarjanimi);
	wcscpy(trk, L"Tarkastuksen tulos: ");
   switch (kilp.tark(k_pv)) {
	  case L' ':
	  case L'B':
	  case 0 :
		 wcscat(trk, L"Tarkastamatta");
		 break;
	  case L'T' :
		 wcscat(trk, L"Hyväksytty");
		 break;
	  case L'H' :
		 wcscat(trk, L"Hylätty");
		 break;
	  case L'K' :
		 wcscat(trk, L"Keskeyttänyt");
		 break;
	  case L'M' :
		 wcscat(trk, L"Puuttuu");
		 break;
	  case L'X' :
		 wcscat(trk, L"Ei tulosta");
		 break;
	  case L'P' :
		 wcscat(trk, L"Poissa");
		 break;
	  case L'I' :
         wcscat(trk, L"Hylkäysesitys");
         break;
      }
   vidspwmsg(7,0,7,0,trk);
   vidspwmsg(9,0,7,0,L"Selitys :");
   vidspwmsg(9,12,7,0,kilp.pv[k_pv].selitys);
#ifdef _CONSOLE
   vidspwmsg(11,0,7,0,L"Lisätietoja raporttiin (max 3 riviä)");
   inputwstr(l1, 65, 0, 13, L"\r", &ch, 0);
   inputwstr(l2, 65, 0, 14, L"\r", &ch, 0);
   inputwstr(l3, 65, 0, 15, L"\r", &ch, 0);
#else
	inputstr_prompt(l1, 65, L"Lisätietoja raporttiin (max 65 merkkiä)", &ch);
#endif
   ch = L'K';
   wselectopt(L"Raportti tulostetaan paperille (K/E)", L"KE", &ch);
   clrln(ySize-3);
   if (ch == L'K') {
	   if (tulprm->lstf == NULL)
		   return;
      EnterCriticalSection(&print_CriticalSection);
		p = 0;
		if (tulprm->printer == LJETII)
			p = initfont_emrap;
		if (tulprm->printer == GDIPRINTER)
			p = L"C";
	  initpage(tulprm, p);
	  initline(tulprm);
	  putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm, 1);
	  swprintf(line, L"Tulostettu %s kello %s", wpaiva(), wkello());
	  putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm, 1);
	  swprintf(line, L"Kilpailija nro     %d", kilpno);
	  putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm, 0);
		swprintf(line, L"Nimi               %s", kilp.nimi(st, 38, false));
	  putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm, 0);
	  swprintf(line, L"Sarja              %s", Sarjat[kilp.Sarja()].sarjanimi);
	  putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm, 1);
	  swprintf(line, L"Tarkastuksen tulos %s", trk+20);
	  putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm, 0);
	  swprintf(line, L"Selitys (lyhyt)    %s", kilp.pv[k_pv].selitys);
	  putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm, 1);
	  swprintf(line, L"Lisätietoja");
	  putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm, 0);
	  putfld(tulprm, l1, 4*tulprm->tulmuot.numlev, wcslen(l1), 0, 0);
	  endline(tulprm, 0);
	  putfld(tulprm, l2, 4*tulprm->tulmuot.numlev, wcslen(l2), 0, 0);
	  endline(tulprm, 0);
	  putfld(tulprm, l3, 4*tulprm->tulmuot.numlev, wcslen(l3), 0, 0);
	  endline(tulprm, 2);
	  swprintf(line, L"Käsittelyvaiheet (ratkaisu, aika, käsittelijä)");
	  putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm, 2);
	  putfld(tulprm, viiva, 4*tulprm->tulmuot.numlev, wcslen(viiva), 0, 0);
	  endline(tulprm, 0);
	  swprintf(line, L"Päätösesitys");
	  putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm, 5);
	  putfld(tulprm, viiva, 4*tulprm->tulmuot.numlev, wcslen(viiva), 0, 0);
	  endline(tulprm, 0);
	  swprintf(line, L"Päätös");
	  putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm, 5);
	  putfld(tulprm, viiva, 4*tulprm->tulmuot.numlev, wcslen(viiva), 0, 0);
		endline(tulprm, 0);
	  swprintf(line, L"Vastalause");
	  putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm, 5);
	  putfld(tulprm, viiva, 4*tulprm->tulmuot.numlev, wcslen(viiva), 0, 0);
	  endline(tulprm, 0);
	  swprintf(line, L"Lopullinen päätös");
	  putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm, 5);
	  putfld(tulprm, viiva, 4*tulprm->tulmuot.numlev, wcslen(viiva), 0, 0);
	  endline(tulprm, 0);
	  endpage(tulprm);
	  enddoc(tulprm);
	  LeaveCriticalSection(&print_CriticalSection);
	  }
   if (!ajat_on) status_on = 1;
   }

void tarkraportit(tulostusparamtp *tulprm, wchar_t trk)
   {
	wchar_t tc, ch = 0, co, line[81], st[60];
	char che;
	INT r = 0, n = 0, k, p, lj, srj = 0, vsrj = 0;
	kilptietue kilp;

   co = L' ';
   wselectopt(L"N)äytölle, P)aperille", L"NP", &co);
   switch (trk) {
      case L'H':
         wcscpy(st, L"Hylätyt");
         break;
      case L'I':
         wcscpy(st, L"Hylkäysesitykset");
		 break;
	  }
   if (co == L'P') {
      r = 32767;
	  if (tulprm->lstf == NULL)
		  return;
      EnterCriticalSection(&print_CriticalSection);
	  }
   do {
	  if( srj < vsrj ) srj++;
      else {
		 clrln(ySize-3);
         vidspwmsg(ySize-3,0,7,0,L"Ensimmäinen sarja :               "
			L"<F2>: Alusta, <Esc>: Poistu");
		 srj = luesarja(L"", &tc);
			if (tc == ESC) break;
         clrln(ySize-3);
         vidspwmsg(ySize-3,0,7,0,L"Viimeinen sarja :               " 
			L"<Esc>: Poistu");
         vsrj = luesarja((wchar_t *)Sarjat[srj].sarjanimi, &tc);
         clrln(ySize-1);
         if (tc == 202) vsrj = sarjaluku - 1;
         if( tc == ESC ) break;
         }
      if (co == L'N') {
         for (r = 2; r < ySize-4; r++) clrtxt(r,0,64);
         clrln(ySize-3);
		 r = 2;
		 }
	  lj = trk == L'I' ? 0 : 2;
		EnterCriticalSection(&tall_CriticalSection);
      p = jalku[srj][lj];
      while (p >= 0) {
		 k = p;
		 p = JarjSeur(0, 0, p);
         if (p < 0 && lj == 0) {
            lj = 4;
            p = jalku[srj][lj];
			}
         kilp.GETREC(k);
		 if (kilp.tark(k_pv) != trk) continue;
         if (co == L'N') {
			if (r > ySize-5) {
               vidspwmsg(ySize-3,0,0,7,
                  L"Paina näppäintä jatkaaksesi, Esc : Poistu");
			   LeaveCriticalSection(&tall_CriticalSection);
			   if ((ch = readkbd(&che,1,spoolfl)) == ESC) break;
			   EnterCriticalSection(&tall_CriticalSection);
			   clrln(ySize-3);
               for (r = 2; r < ySize-4; r++) clrtxt(r,0,64);
               r = 2;
			   }
			}
		 else {
            if (r > tulprm->tulmuot.sivpit) {
               if (r != 32767)
				  endpage(tulprm);
			   swprintf(line, L"%-16.16s   Tulostettu %s kello %s",
				  st, wpaiva(), wkello());
			   initpage(tulprm, tulprm->printer == LJETII ? initfont_emrap : L"C");
			   initline(tulprm);
				putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
			   endline(tulprm, 1);
			   r = 2;
			   }
            }
         if (co == L'N') {
				swprintf(line, L"%5d %-23.23s %-10.10s  %c %-20.20s",
					kilp.id(), kilp.nimi(st, 25, false), (wchar_t *)Sarjat[srj].sarjanimi,
					kilp.tark(k_pv), kilp.pv[k_pv].selitys);
				vidspwmsg(r, 0, 7, 0, line);
				}
			else {
				swprintf(line, L"%5d %-28.28s %-15.15s  %c %-20.20s",
					kilp.id(), kilp.nimi(st, 28, false), (wchar_t *)Sarjat[srj].sarjanimi,
					kilp.tark(k_pv), kilp.pv[k_pv].selitys);
				putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
			endline(tulprm, 0);
            }
         r++;
         n++;
		 }
      LeaveCriticalSection(&tall_CriticalSection);
      if (co == L'N') {
         if (n > 0 && ch != ESC) {
            vidspwmsg(ySize-3,0,0,7, L"Paina näppäintä jatkaaksesi");
			readkbd(&che,1,0);
            n = 0;
            }
         for (r = 2; r < ySize-4; r++) clrtxt(r,0,64);
		 clrln(ySize-3);
         }
      } while (srj <= sarjaluku);
   if (co == L'P') {
	  endpage(tulprm);
	  enddoc(tulprm);
	  LeaveCriticalSection(&print_CriticalSection);
      clrln(ySize-3);
      swprintf(line, L"Tulostettu %d kilpailijaa     Paina näppäintä", n);
      vidspwmsg(ySize-3,0,7,0,line);
	  readkbd(&che,1,0);
	  clrln(ySize-3);
      }
   clrln(ySize-3);
   }

void kirjvaliajat(wchar_t *flnm)
   {
   TextFl *vafile;
	wchar_t as[16], as2[16], ast[16], ase[16], st[60], line[400], vafilename[] = L"EMITAJAT.LST";
	emittp em;
	kilptietue kilp;
	INT d, i, rastiluku, tulkinta[MAXNLEIMA+1];
	UINT Va[MAXNLEIMA+1];
	INT32 enn;
   ratatp *rt;

   vafile = new TextFl(flnm != NULL ? flnm : vafilename, L"wb");
   if (!vafile->IsOpen()) {
	  delete vafile;
	  writeerror_w(L"Virhe väliaikatiedostoa avattaessa", 0);
	  return;
	  }
   vafile->WriteLine(L"Klpno Nimi                        Sarja      Tulos    T NR Lahto"
	  L"    Mlikello"
	  L" Luku       Enn Aika1 Aika2 Aika3 Aika4 Aika5 Aika6 Aika7 Aika8 "
	  L"Aika9 Aik10 Aik11 Aik12 Aik13 Aik14 Aik15 Aik16 Aik17 Aik18 Aik19 "
	  L"Aik20 Aik21 Aik22 Aik23 Aik24 Aik25 Aik26 Aik27 Aik28 Aik29 Aik30 "
	  L"Aik31 Aik32 Aik33 Aik34 Aik35 Aik36 Aik37 Aik38 Aik39 Aik40\r\n");
   EnterCriticalSection(&emit_CriticalSection);
   _lseek(emitfile, 0, SEEK_SET);
   while (1) {
	  if (_read(emitfile, &em, sizeof(emittp)) < (int)sizeof(emittp))
         break;
	  if (em.kilpno <= 0 || (d = getpos(em.kilpno)) <= 0)
         continue;
      kilp.GETREC(d);
	  tarkista(&em, tulkinta, 0, 0);
      memset(Va, 0, sizeof(Va));
      rt = haerata(&kilp);
	  rastiluku = 0;
	  if (rt && (rastiluku = rt->rastiluku) != 0)
         enn = haeennakko(&em, &kilp);
      else
         enn = 0;
	  if (em.maali != TMAALI0/KSEK)
		aikatowstr_ls(as, em.maali, t0);
	  else
		wcscpy(as, L"00.00.00,0");
      as[8] = 0;
	  if (em.time != TMAALI0/KSEK)
		aikatowstr_ls(ase, em.time, t0);
	  else
		wcscpy(ase, L"00.00.00,0");
      ase[8] = 0;
	  AIKATOWSTRS(ast, kilp.p_aika(0), 0);
	  ast[pvparam[k_pv].laikasj] = 0;
	  swprintf(line, L"%05.5d %-27.27s %-10.10s %s %c %2.2d %8.8s %s %s %5ld",
			em.kilpno, kilp.nimi(st, 28, false), Sarjat[kilp.Sarja()].sarjanimi, ast, kilp.tark(k_pv),
		 rastiluku, AIKATOWSTRS(as2, kilp.pv[k_pv].tlahto, t0), as, ase, enn);
	  for (i = 0; i < MAXNLEIMA; i++) {
		 if (i && !em.ctrlcode[i] && !em.ctrltime[i]) break;
		 if (tulkinta[i] < 0) Va[-tulkinta[i]-1] = (UINT) (em.ctrltime[i] - enn);
         else if (tulkinta[i] > 0) Va[tulkinta[i]-1] = (UINT) (em.ctrltime[i] - enn);
		 }
	  for (i = 0; i < 40; i++) {
         swprintf(line+wcslen(line), L" %5.5u", Va[i]);
		 }
	  wcscat(line, L"\r\n");
	  vafile->WriteLine(line);
	  }
   LeaveCriticalSection(&emit_CriticalSection);
   delete vafile;
   }

emitvaclass::emitvaclass(void)
{
	memset(this, 0, sizeof(emitvaclass));
	va[0] = new UINT32[MAXNLEIMA+2];
	va[1] = new UINT32[MAXNLEIMA+2];
	sj[0] = new int[MAXNLEIMA+2];
	sj[1] = new int[MAXNLEIMA+2];
	memset(va[0], 0, (MAXNLEIMA+2)*sizeof(UINT32));
	memset(va[1], 0, (MAXNLEIMA+2)*sizeof(UINT32));
	memset(sj[0], 0, (MAXNLEIMA+2)*sizeof(int));
	memset(sj[1], 0, (MAXNLEIMA+2)*sizeof(int));
}

emitvaclass::~emitvaclass(void)
{
	delete[] va[0];
	delete[] va[1];
	delete[] sj[0];
	delete[] sj[1];
}

INT luo_emitva(INT srj, emitvaclass *vaptr, INT kaik, INT *mxnrasti)
   {
   INT i, n, p, lj, k, t, viive;
   INT32 enn;
   INT tulkinta[MAXNLEIMA+1];
   emittp em;
   kilptietue kilp;
   ratatp *rt;

   p = -1;
   lj = 0;
   n = 0;
   EnterCriticalSection(&tall_CriticalSection);
   EnterCriticalSection(&emit_CriticalSection);
   while (1) {

	  // Ohitetaan tyhjät lajit. Tämän jälkeen
	  // lj osoittaa seuraavaan vuorossa olevaan lajiin, ei tarkasteltavaan

	  while (p < 0 && lj < 1+2*kaik) p = jalku[srj][lj++];

	  // Poistutaan, kun tarkasteltava laji on 1 tai 3 eli ei-lähteneet

	  if (p < 0 || lj > 1+2*kaik) break;
	  k = p;               // k osoittaa tarkasteltavaan kilpailijaan
	  p = JarjSeur(0, 0, p);  // osoittaa seuraavaan kilpailijaan
	  kilp.GETREC(k);
	  if (kilp.tark() == L'X' || getem(&em, kilp.id(), 0) < 0) continue;

	  // Varataan tila tarkasteltavan kilpailijan väliajoille ja
	  // väliaikasijoituksille

	  rt = haerata(&kilp);
	  if (!rt || !rt->rastiluku) continue;
	  if (rt->rastiluku > *mxnrasti) *mxnrasti = rt->rastiluku;

	  // t on aika lähdöstä maaliin sekunteina, jos tulos on olemassa

	  if (kilp.p_aika(0)) {
		 t = (INT)(((kilp.maali(0) - kilp.pv[k_pv].tlahto)/SEK + 86400L)
            % 86400L);
		 }
      else
		 t = 0;

	  enn = haeennakko(&em, &kilp);

	  vaptr[n].tm = em.maali;
	  vaptr[n].tl = em.time;

      // Tulkitse, mitä rastia mikin aika vastaa
	  if (rt) {
			tarkista(&em, tulkinta, 0, 0);

      // Tallenna ennakolla korjatut väliajat kullekin rastille
	  // Väärässä vaiheessa leimattu aika tallennetaan vain, jos ei vielä
      // aikaa

		  for (i = 1; i < MAXNLEIMA; i++) {
			 if (i && !em.ctrlcode[i] && !em.ctrltime[i])
				continue;
			 if (tulkinta[i] > 0)
				vaptr[n].va[0][tulkinta[i]-1] = (UINT) (em.ctrltime[i] - enn);
			 else if (tulkinta[i] < 0 && !vaptr[n].va[0][-tulkinta[i]-1])
				vaptr[n].va[0][-tulkinta[i]-1] = (UINT) (em.ctrltime[i] - enn);
			 }

	  // Jos maalilaji == 2, lasketaan viive maalista kortin lukemiseen
	  // ja maalia vastaava Emit-aika lasketaan lukuhetken ajasta korjaten
	  // viiveellä.
	  // Muuten maalia vastaava Emit-aika on tulos sekunteina

		  if (rt->maalilaji == 2) {
			 if (em.maali != TMAALI0/KSEK)
				viive = (INT)(((em.time - em.maali)/10 + 86400L) % 86400L);
			 else {
				if (t)
				   viive = (INT)(((KSEK*em.time - kilp.pv[k_pv].tlahto)/SEK + 86400L)
					  % 86400L) - t;
				else
				   viive = 0;
				}
			 vaptr[n].va[0][rt->rastiluku] -= viive;
			 }
		  else if (t) vaptr[n].va[0][rt->rastiluku] = t;
		  }
		if (++n >= nilm[srj]) break;
		}
   LeaveCriticalSection(&emit_CriticalSection);
   LeaveCriticalSection(&tall_CriticalSection);
   return(n);
   }

static void va_sijat(emitvaclass *emptr, INT k, INT n)
   {
   typedef struct{
	  INT32 tls;
	  INT r;
	  } rstp;
   rstp *rs;
   INT i, r, nr;
   UINT edtls;

   rs = (rstp *) malloc(n*sizeof(rstp));
   if (!rs)
	   return;
   for (int ii = 0; ii < 2; ii++) {
	   memset(rs, 0, n*sizeof(rstp));
	   for (r = 0; r < n; r++) {
		  emptr[r].sj[ii][k] = 0;
		  }
	   i = 0;
	   for (r = 0; r < n; r++) {
		  if (emptr[r].va[0][k]) {
			 rs[i].r = r;
			 edtls = emptr[r].va[0][k];
			 if (ii && k) {
				if (edtls && emptr[r].va[0][k-1])
					edtls -= emptr[r].va[0][k-1];
				else
					edtls = 0;
				}
			 if (edtls) {
				rs[i].tls = edtls;
				i++;
				}
			 if (ii == 1)
				emptr[r].va[1][k] = edtls;
			 }
		  }
	   nr = i;
		qsortint((void *)rs, nr, sizeof(INT32)+sizeof(INT), 0, 1);
	   i = 1;
	   edtls = 0;
	   int rr = 0;
	   for (r = 0; r < nr; r++) {
		  if (rs[r].tls == 0)
			  continue;
		  rr++;
		  if ((UINT)rs[r].tls != edtls) i = rr;
		  edtls = (UINT)rs[r].tls;
		  emptr[rs[r].r].sj[ii][k] = i;
		  }
	   }
   free(rs);
   }

static wchar_t **vapg;
static INT valn;

INT initvapage(INT lopfl)
   {
   INT i;

   if (!vapg) {
	  if (lopfl) return(0);
	  vapg = (wchar_t **) calloc(vapagelen+5, sizeof(wchar_t *));
	  if (!vapg) return(1);
	  }
   else {
	  for (i = 0; i < vapagelen+5; i++) {
		 if (vapg[i]) free(vapg[i]);
		 vapg[i] = 0;
		 }
	  if (lopfl) {
		 free(vapg);
		 vapg = 0;
		 }
	  }
   valn = 0;
   return(0);
   }

INT addvaline(wchar_t *line)
   {
   if ((vapg[valn] = (wchar_t *) calloc(wcslen(line)+1, sizeof(wchar_t))) == 0)
   		return(1);
   wcscpy(vapg[valn++], line);
   return(0);
   }

void printvapage(tulostusparamtp *tulprm)
   {
   INT i, tl = 1, l;
   INT allev = 0;
   wchar_t *p, line[250];
   PRFILE *lstf;

   lstf = tulprm->lstf;
   while (tl) {
	  tl = 0;
      EnterCriticalSection(&print_CriticalSection);
	  initpage(tulprm, tulprm->printer == LJETII ? initfont_emva : L"C");
      for (i = 0; i < valn; i++) {
		 for (p = vapg[i]; *p && *p != L'\n'; p++);
		 l = (INT) (p - vapg[i]);
         if (l > valinelen) {
            l = valinelen;
            tl = 1;
            }
		 wmemcpy(line, vapg[i], l);
		 if (*p) {
            line[l] = L'\r';
			wcscpy(line+l+1, p);
            }
         else line[l] = 0;
		 wmemmove(vapg[i], vapg[i]+l, wcslen(vapg[i]+l)+1);
			if (tulprm->printer == LJETII && i > 5 && !((i-1) % 5)) {
			sendchars("\x1b&d0D", 5, lstf);
			allev = 1;
            }
		 sendwline(line, lstf);
         if (allev) {
			sendchars("\x1b&d@", 4, lstf);
			allev = 0;
			}
		 }
	  endpage(tulprm);
	  enddoc(tulprm);
	  LeaveCriticalSection(&print_CriticalSection);
      }
   }

INT tulostusparamtp::puts1(wchar_t *line)
   {
   switch (kohde) {
      case L'P':
		 return(addvaline(line));
	  case L'H':
	  case L'M':
		 writehtml(line);
		 break;
	  case L'I':
	  case L'J':
		 puts_f(line);
			break;
	  }
   return(0);
	}

/*
INT puts1(wchar_t *line, wchar_t co)
   {
   switch (co) {
	  case L'P':
		 return(addvaline(line));
	  case L'H':
		 writehtml(line);
		 break;
	  case L'I':
	  case L'J':
		 puts_f(line);
			break;
	  }
   return(0);
	}
*/

void emitvaliajat(INT *srj, tulostusparamtp *tulprm)
	{
	INT r, i, j, d, n, p, lj, sj = 0, sj1, html = 0, nvakilp;
	INT mxnrasti = 0;
	INT32 tls0 = 0, tls, aika, edaika;
	kilptietue kilp;
	wchar_t oline[4000], sjstr[8], st[30], st1[16], st2[16];
	emitvaclass *emptr = NULL;
	emittp em;
	ratatp *rt;
	wchar_t co, cr;
	INT nparas, moniriviva = 0, viimpois = 0;

	co = tulprm->kohde;
	if (co == L'M')
		co = L'H';
	if (co == L'J') {
		co = L'I';
		moniriviva = 1;
		}

#ifndef _CONSOLE
	if (tulprm->optiot & 0x00100000)
		viimpois = 1;
#else
	if (maalileimasin > -1)
		viimpois = 1;
#endif

	cr = L"KRMN"[tulprm->piste];

	switch (tulprm->rajaus) {
		case L'L' :
		case L'I' :
			nparas = -1;
			break;
		case L'P' :
			nparas = tulprm->viimsija;
			break;
		default :
			nparas = ntulos[*srj][0];
			break;
		}

	switch (co) {
		case L'H' :
			html = 1;
			break;
		}
	emptr = new emitvaclass[nilm[*srj]+1];
	if (!emptr) {
		writeerror_w(L"Muisti ei riitä", 0);
		*srj = sarjaluku;
		goto loppu;
		}
	if ((nvakilp = luo_emitva(*srj, emptr, nparas < 0, &mxnrasti)) > 0) {
		for (i = 0; i < mxnrasti - viimpois + 1; i++)
			va_sijat(emptr, i, nvakilp);
		for (int lj1 = 0; lj1 < 2; lj1++) {
			int laji = lj1;
			if (cr == L'N')
				laji = 2;
			if (laji == 0 && cr == L'R')
				continue;
			if (laji == 1 && cr == L'K')
				break;
			r = 0;
			if (laji == 1 && cr == L'M') {
				switch (co) {
					case L'H' :
						tulprm->writehtml(L"<p>\n");
						break;
					case L'I' :
					case L'J' :
						tulprm->puts1(L"\n\n");
						break;
					case L'P' :
						if (r > 2)
							printvapage(tulprm);
						break;
					}
				}

			p = -1;
			lj = 0;
			n = 0;
			EnterCriticalSection(&tall_CriticalSection);
			for (sj1 = 1;; sj1++) {
				edaika = 0;
				oline[0] = 0;
				if (co == L'P' && r >= vapagelen) {
					printvapage(tulprm);
					r = 0;
					}
				if (r == 0) {
					if (co == L'P')
						initvapage(0);
					if (html)
						tulprm->writehtml(L"<H3>");
					if (laji == 0) {
						if (tulprm->language == EN)
							swprintf(oline, L"%s   Times at controls\n\n", Sarjat[*srj].sarjanimi);
						else
							swprintf(oline, L"%s   Tilanne rasteilla\n\n", Sarjat[*srj].sarjanimi);
						}
					else if (laji == 1) {
						if (tulprm->language == EN)
							swprintf(oline, L"%s Control to control\n\n", Sarjat[*srj].sarjanimi);
						else
							swprintf(oline, L"%s   Rastivälien ajat\n\n", Sarjat[*srj].sarjanimi);
						}
					else {
						if (tulprm->language == EN)
							swprintf(oline, L"%s Split times\n\n", Sarjat[*srj].sarjanimi);
						else
							swprintf(oline, L"%s   Rastiväliajat\n\n", Sarjat[*srj].sarjanimi);
						}
					tulprm->puts1(oline);
					if (html) {
						tulprm->writehtml(L"</H3>");
						if (tulprm->tulmuot.tauluhtml) {
/*							int lev = (colW[0] + colW[1] + colW[4] +
								(mxnrasti - viimpois + laji) * (colW[2] + 4*colW[3]/3 + 4 * tulprm->tulmuot.hpad + 12)
								+ 6 * tulprm->tulmuot.hpad + 18) * fsz / fsz0;
							swprintf(oline, L"<table width=\"%dpx\" class='rvatbl'>\n<colgroup>\n", lev);
							tulprm->writehtml(oline);
							swprintf(oline, L"<col width=\"%dpx\">\n", colW[0]*fsz/fsz0);
							tulprm->writehtml(oline);
							swprintf(oline, L"<col width=\"%dpx\">\n", colW[1]*fsz/fsz0);
							for (i = 0; i < mxnrasti - viimpois + laji; i++) {
								swprintf(oline+wcslen(oline), L"<col width=\"%dpx\">\n", colW[2]*fsz/fsz0);
								swprintf(oline+wcslen(oline), L"<col width=\"%dpx\">\n", colW[3]*fsz/fsz0);
								}
							tulprm->writehtml(oline);
							swprintf(oline, L"<col width=\"%dpx\">\n</colgroup>\n", colW[4]*fsz/fsz0);
							tulprm->writehtml(oline);
*/
							tulprm->writehtml(L"<table class='rvatbl'>\n");
							}
						else
							tulprm->writehtml(L"<PRE>\n");
						}
					if (html && tulprm->tulmuot.tauluhtml) {
						if (tulprm->language == EN)
							wcscpy(oline, L"<tr><th>Pos</th><th>Name</th>");
						else
							wcscpy(oline, L"<tr><th>Sija</th><th>Nimi</th>");
						for (i = 0; i < mxnrasti - viimpois + (laji ? 1 : 0) ; i++) {
//							swprintf(oline+wcslen(oline), L"<th style=\"border-right-style:none;\"></th><th style=\"border-left-style:none;\">%d.</th>", i+1);
//							swprintf(oline+wcslen(oline), L"<th class='norbd'></th><th class='nolbd'>%d.</th>", i+1);
							swprintf(oline+wcslen(oline), L"<th></th><th>%d.</th>", i+1);
							}
						if (tulprm->language == EN) {
							if (!onrata(Sarjat[*srj].sarjanimi))
								wcscat(oline, L"<th>Course</th><th>Result</th></tr>\n");
							else
								wcscat(oline, L"<th>Result</th></tr>\n");
							}
						else {
							if (!onrata(Sarjat[*srj].sarjanimi))
								wcscat(oline, L"<th>Rata</th><th>Tulos</th></tr>\n");
							else
								wcscat(oline, L"<th>Tulos</th></tr>\n");
							}
						}
					else if (!moniriviva) {
						if (tulprm->language == EN)
							wcscpy(oline, L"Pos   Name                 ");
						else
							wcscpy(oline, L"Sija  Nimi                 ");
						for (i = 0; i < mxnrasti - viimpois + (laji ? 1 : 0); i++) {
							if (laji == 0)
								swprintf(st, L"%d.", i+1);
							else
								swprintf(st, L"%d-%d", i, i+1);
							swprintf(oline+wcslen(oline), L" %10s  ", st);
							}
						if (laji == 0) {
							if (tulprm->language == EN)
								swprintf(oline+wcslen(oline), L"   Result\n\n");
							else
								swprintf(oline+wcslen(oline), L"    Tulos\n\n");
							}
						else {
							swprintf(st, L"%d-M", i);
							if (tulprm->language == EN)
								swprintf(oline+wcslen(oline), L" %10s     Result\n\n", st);
							else
								swprintf(oline+wcslen(oline), L" %10s      Tulos\n\n", st);
							}
						r += 2;
						}
					else {
						wcscpy(oline, L"       1/6/11       2/7/12       3/8/13 "
							L"      4/9/14      5/10/15\n\n");
						}
					tulprm->puts1(oline);
					memset(oline, 0, sizeof(oline));
					}
				while (p < 0 && lj < 1+2*(nparas < 0))
					p = jalku[*srj][lj++];
				if (p < 0 || lj > 1+2*(nparas < 0))
					break;
				d = p;
				p = JarjSeur(0, 0, p);
				kilp.GETREC(d);
				if (lj > 1) {
					sj = 0;
					tls = 0;
					}
				else if ((tls = kilp.p_aika(0)/KSEK) != tls0)
					sj = sj1;
				if (nparas > 0 && sj > nparas)
					break;
				if (kilp.tark() == L'X')
					continue;
				tls0 = tls;
				if (lj == 1) {
					swprintf(sjstr, L"%3d.", sj);
					aikatowstr_cols_n(st1, KSEK*tls, 0, 0, 8);
					st1[8] = 0;
					elimwzb1(st1);
					}
				else {
					sjstr[0] = 0;
					if (tulprm->language == EN) {
						if (kilp.tark(k_pv) == L'K')
							wcscpy(sjstr, L" DNF");
						else if (kilp.tark(k_pv) == L'H')
							wcscpy(sjstr, L" DQ ");
						else if (kilp.tark(k_pv) == L'M')
							wcscpy(sjstr, L"MISS");
						}
					else {
						if (kilp.tark(k_pv) == L'K')
							wcscpy(sjstr, L"KESK");
						else if (kilp.tark(k_pv) == L'H')
							wcscpy(sjstr, L" HYL");
						else if (kilp.tark(k_pv) == L'M')
							wcscpy(sjstr, L"VIRH");
						}
					st1[0] = 0;
					}
				if (html && tulprm->tulmuot.tauluhtml) {
/*
					if ((sj1+tulprm->tulmuot.riviryhma-1)%(2*tulprm->tulmuot.riviryhma) >= tulprm->tulmuot.riviryhma)
						wcscpy(oline, L"<tr class='aa rva'>");
					else
						wcscpy(oline, L"<tr class='ab rva'>");
					swprintf(oline+wcslen(oline), L"<td>%s</td><td align=left>%s</td>", sjstr, kilp.nimi(st, 25, false));
//					swprintf(oline+wcslen(oline), L"<td>%s</td><td style=\"text-align: left;\">%s</td>", sjstr, kilp.nimi(st, 25, false));
*/
					swprintf(oline+wcslen(oline), L"<tr><td>%s</td><td>%s</td>", sjstr, kilp.nimi(st, 25, false));
					}
				else if (!moniriviva)
					swprintf(oline, L"%4.4s %-21.21s", sjstr, kilp.nimi(st, 21, false));
				else {
					if (kilp.pv[k_pv].rata[0])
						swprintf(oline, L"%4.4s %-25.25s %-10.10s %5s %8s", sjstr,
							kilp.nimi(st, 25, false), kilp.seuratieto(0), kilp.pv[k_pv].rata, st1);
					else
						swprintf(oline, L"%4.4s %-25.25s %-10.10s  %8s", sjstr,
							kilp.nimi(st, 25, false), kilp.seuratieto(0), st1);
					}
				if (getem(&em, kilp.id(), 0) >= 0 && (rt = haerata(&kilp)) != NULL && rt->rastiluku > 0) {
					for (int laji2 = (laji < 2 ? laji : 0); laji2 <= (laji < 2 ? laji : 1); laji2++) {
						if (laji > 1 && laji2 > 0) {
							if (html && tulprm->tulmuot.tauluhtml)
								wcscpy(oline, L"<tr><td></td><td></td>");
							else
								swprintf(oline, L"%26.26s", L"");
							}
	//					for (i = 0; i < mxnrasti - viimpois + laji; i++)
						for (i = 0; i < mxnrasti - viimpois + 1; i++) {
							if (laji2 == 0) {
								aikatowstr_cols_n(st2, SEK*emptr[n].va[0][i], 0, 0, 8);
								st2[8] = 0;
								elimwzb1(st2);
								if (!wcscmp(st2, L"0"))
									st2[0] = L'-';
								if (html && tulprm->tulmuot.tauluhtml) {
									if (i < mxnrasti - viimpois) {
										if (emptr[n].sj[0][i])
											swprintf(oline+wcslen(oline), L"<td>%d.</td><td>", emptr[n].sj[0][i]);
	//										swprintf(oline+wcslen(oline), L"<td class='norbd'>%d.</td><td class='nolbd'>", emptr[n].sj[i]);
	//										swprintf(oline+wcslen(oline), L"<td style=\"border-right-style:none;\">%d.</td><td style=\"border-left-style:none;\">", emptr[n].sj[i]);
										else
											wcscat(oline, L"<td></td><td>");
	//										wcscat(oline, L"<td class='norbd'></td><td class='nolbd'>");
	//										wcscat(oline, L"<td style=\"border-right-style: none;\"></td><td style=\"border-left-style:none;\">");
										wcscat(oline, st2);
										wcscat(oline, L"</td>");
										}
									}
								else {
									if (emptr[n].sj[0][i])
										swprintf(st, L"%d-%s", emptr[n].sj[0][i], st2);
									else
										wcscpy(st, st2);
									if (!moniriviva) {
										if (i < mxnrasti - viimpois)
											swprintf(oline+wcslen(oline), L" %12s", st);
										}
									else {
										if ((i % 5) == 0) {
											wcscat(oline, L"\n");
											for (j = i/5; j; j--)
												wcscat(oline, L" ");
											}
										swprintf(oline+wcslen(oline), L" %12s", st);
										}
									}
								}
							else {
								edaika = 0;
								aika = emptr[n].va[1][i];
								if (aika > edaika) {
									aikatowstr_cols_n(st2, SEK*(aika-edaika), 0, 0, 8);
									st2[8] = 0;
	//								if (i < mxnrasti - viimpois)
	//									edaika = aika;
									elimwzb1(st2);
									}
								else
									wcscpy(st2, L"-");
								if (html && tulprm->tulmuot.tauluhtml) {
									if (emptr[n].sj[1][i])
										swprintf(oline+wcslen(oline), L"<td>%d.</td><td>", emptr[n].sj[1][i]);
	//									swprintf(oline+wcslen(oline), L"<td  class='norbd'>%d.</td><td  class='nolbd'>", emptr[n].sj[i]);
									else
										wcscat(oline, L"<td></td><td>");
	//									wcscat(oline, L"<td  class='norbd'></td><td  class='nolbd'>");
									wcscat(oline, st2);
									wcscat(oline, L"</td>");
									}
								else {
									if (emptr[n].sj[1][i])
										swprintf(st, L"%d-%s", emptr[n].sj[1][i], st2);
									else
										wcscpy(st, st2);
									if (!moniriviva)
										swprintf(oline+wcslen(oline), L"   %10s", st);
									else {
										if ((i % 5) == 0) {
											wcscat(oline, L"\n");
											for (j = i/5; j; j--)
												wcscat(oline, L" ");
											}
										swprintf(oline+wcslen(oline), L"   %10s", st);
										}
									}
								}
							}
						if (html && tulprm->tulmuot.tauluhtml) {
							if (laji2 == 0 && laji > 1) {
								if (tls) {
									swprintf(oline+wcslen(oline), L"<td>%s</td><td>%s</td>", sjstr, st1);
									}
								else
									wcscat(oline, L"<td></td><td></td>");
								}
							if (!onrata(Sarjat[*srj].sarjanimi)) {
								if (laji2 == 0 || laji == 1)
									swprintf(oline+wcslen(oline), L"<td>%s</td>", kilp.pv[k_pv].rata);
								else
									wcscat(oline, L"<td></td>");
								if (tls && laji2 == 0 || laji == 1)
									swprintf(oline+wcslen(oline), L"<td>%s</td>", st1);
								else
									wcscat(oline, L"<td></td>");
								}
							else if ((laji2 == 0 || laji2 == laji) && tls)
								swprintf(oline+wcslen(oline), L"<td>%s</td>", st1);
							else
								wcscat(oline, L"<td></td>");
/*
							if (laji2 == 1 && laji > 1) {
								wcscat(oline, L"<td></td>");
								}
							else if (tls == 0) {
								wcscat(oline, L"<td></td>");
								}
*/
							wcscat(oline, L"</tr>");
							}
						else if (laji2 == 1) {
							if (laji == 1 && tls) {
	//							aika = tls/10;
								aika = emptr[n].va[1][mxnrasti - viimpois + 1];
								st[0] = 0;
								if (aika > edaika) {
									aikatowstr_cols_n(st+1, SEK*(aika-edaika), 0, 0, 8);
									st[9] = 0;
		//                     			if (i < mxnrasti - viimpois) edaika = aika;
									elimwzb1(st+1);
									st[0] = L'(';
									wcscat(st, L")");
									}
								if (!moniriviva) {
									aikatowstr_cols_n(st2, KSEK*tls, 0, 0, 8);
									st2[8] = 0;
									elimwzb1(st2);
									if (html && tulprm->tulmuot.tauluhtml)
										swprintf(oline+wcslen(oline), L"<td>%s</td>", st2);
									else
										swprintf(oline+wcslen(oline), L"   %8s", st2);
									}
								}
							}
						else if (!moniriviva) {
							if (laji2 == 0) {
								if (tls)
									swprintf(oline+wcslen(oline), L"  %8s", st1);
								if (kilp.pv[k_pv].rata[0])
									swprintf(oline+wcslen(oline), L" %5s", kilp.pv[k_pv].rata);
								}
							if (wcslen(oline) > 100) {
								wcscat(oline, L"  ");
								kilp.nimi(oline+wcslen(oline), 32, false);
								}
							}
						wcscat(oline, L"\n");
						tulprm->puts1(oline);
						memset(oline, 0, sizeof(oline));
						r++;
						}
					n++;
					}
				}
			LeaveCriticalSection(&tall_CriticalSection);
			if (html) {
				if (tulprm->tulmuot.tauluhtml)
					tulprm->writehtml(L"</table>\n");
				else
					tulprm->writehtml(L"</PRE>\n");
				}
			if (cr == L'K' || cr == L'N')
				break;
			}
		if (co == L'P' && r > 2)
			printvapage(tulprm);
		}
	else
		if (nvakilp < 0)
			*srj = sarjaluku;
loppu:
	initvapage(1);
	if (emptr)
		delete[] emptr;
	}

int tee_emva(emitvatp *emva, emittp *em)
	{
	kilptietue kilp;
	int d, i, l, r, tulkinta[MAXNLEIMA+1];
   INT32 enn;
   ratatp *rt;

	if (em->kilpno <= 0 || (d = getpos(em->kilpno)) <= 0)
		return(1);
   kilp.GETREC(d);
   tarkista(em, tulkinta, 0, 0);
   memset(emva, 0, sizeof(emitvatp));
   rt = haerata(&kilp);
   if (!rt)
	   return(1);
   if (rt && rt->rastiluku != 0)
	  enn = haeennakko(em, &kilp);
   else
	  enn = 0;
	emva->badge = em->badge;
	if (origemvabadge)
		emva->badge = em->badge0;
	else
		emva->badge = em->badge;
	emva->kilpno = em->kilpno;
	emva->osuus = k_pv;
	emva->tulos = kilp.tulos2(0);
	emva->ok = kilp.tark(k_pv) == L'T' ? 0 : kilp.tark(k_pv);
	emva->rastiluku = rt->rastiluku-((maalileimasin > -1 && rt->maalilaji > 1) ? 1 : 0);
	wcsncpy(emva->rata, rt->tunnus, 7);
//	_itoa(rt->tunnus, (char *)emva->rata, 10);
	l = haelukija(em);
	if (l < 0 || l > MAXNLEIMA-1)
		return(1);
	for (i = 1; i < MAXNLEIMA; i++) {
		if ((r = tulkinta[(l+i)%MAXNLEIMA]) != 0) {
			if (r < 0) {
				int ii;
				for (ii = 0; ii < MAXNLEIMA; ii++)
					if (r == -tulkinta[ii])
						break;
				if (ii < MAXNLEIMA)
					continue;
				}
			r = abs(r);
			if (r > emva->rastiluku || r > maxemvalah)
				continue;
			if (tulkinta[(l+i)%MAXNLEIMA] > 0 || !emva->rastit[r-1][0]) {
				emva->rastit[r-1][0] = rt->rastikoodi[r-1];
				emva->rastit[r-1][1] = em->ctrltime[(l+i)%MAXNLEIMA]-enn;
				}
			}
		}
	return(0);
	}

void kirjemvaloki(combufrec *ibuf)
{
   wchar_t msg[400];
   emitvatp *emva;

   emva = &ibuf->d.emva;
   swprintf(msg, L"%d: Vaihe %d Bdg %d, Aika %d, Trk %d, Rata %s, Rasteja %d",
	  emva->kilpno, emva->osuus+1, emva->badge, emva->tulos/SEK, emva->ok,
		emva->rata, emva->rastiluku);
   wkirjloki(msg);
   msg[0] = 0;
   for (int i = 0; i < emva->rastiluku && i < MAXNLEIMA; i++) {
	   swprintf(msg+wcslen(msg), L"%d %d ", emva->rastit[i][0], emva->rastit[i][1]);
	   }
   wkirjloki(msg);
}

static int rastiVA(int rno, int *tulkinta, emittp *em, int enn)
{
	for (int i = 0; i < MAXNLEIMA+1; i++)
		if (tulkinta[i] == rno) {
			return(em->ctrltime[i]-enn);
			}
	for (int i = 0; i < MAXNLEIMA+1; i++)
		if (tulkinta[i] == -rno) {
			return(em->ctrltime[i]-enn);
			}
	return(0);
}
//---------------------------------------------------------------------------
void laskeEm_VA(int d, bool sail)
{
	emittp em;
	int enn, tulkinta[MAXNLEIMA+1];
	bool muutos = false;
	kilptietue kilp;

	if (d < 1 || d >= nrec)
		return;
	kilp.GETREC(d);
	if (kilp.kilpstatus)
		return;
	if (em.getbykno(kilp.kilpno, 0) < 0)
		return;
	tarkista(&em, tulkinta, 0, &kilp);
	enn = haeennakko(&em, &kilp);
	for (int i = 0; i <= Sarjat[kilp.Sarja()].valuku[k_pv]; i++) {
		int ta, tl;
		if (Sarjat[kilp.Sarja()].mtb[k_pv][i].alkukdi + Sarjat[kilp.Sarja()].mtb[k_pv][i].loppukdi == 0)
			continue;
		if (sail && kilp.p_aika(Sarjat[kilp.Sarja()].mtb[k_pv][i].piste))
			continue;
		if (Sarjat[kilp.Sarja()].mtb[k_pv][i].alkukdi == 0)
			ta = 0;
		else
			ta = SEK * rastiVA(Sarjat[kilp.Sarja()].mtb[k_pv][i].alkukdi, tulkinta, &em, enn);
		if (Sarjat[kilp.Sarja()].mtb[k_pv][i].loppukdi == 0)
			tl = kilp.p_aika(0);
		else {
			tl = SEK * rastiVA(Sarjat[kilp.Sarja()].mtb[k_pv][i].loppukdi, tulkinta, &em, enn);
			if (tl == 0)
				tl = 5 * TUNTI;
			}
		if (Sarjat[kilp.Sarja()].mtb[k_pv][i].pohja != 0)
			tl += kilp.p_aika(Sarjat[kilp.Sarja()].mtb[k_pv][i].pohja);
		kilp.tall_tulos(Sarjat[kilp.Sarja()].mtb[k_pv][i].piste, tl-ta);
		muutos = true;
		}
	if (muutos) {
		kilp.tallenna(d, 0, 0, 0, 0);
	}
}

#if defined(EMIT) && defined(_CONSOLE)
void editemit(INT32 ep, emittp *em, kilptietue *kilp)
   {
   INT16 cd[MAXNLEIMA];
   INT i, j, r, x;
   UINT tm[MAXNLEIMA];
   INT32 tl, etm, emt;
   wchar_t line[20], wch;

   for (r = 4; r < ySize-4; r++) {
	  clrtxt(r, 0, 63);
	  }
   for (i = 0; i < MAXNLEIMA; i++) {
      etm = em->time;
      emt = em->maali;
      cd[i] = em->ctrlcode[i];
	  tm[i] = em->ctrltime[i];
      }
   i = -1;
   j = 0;
   do {
	  clrln(ySize-3);
	  clrln(ySize-1);
	  vidspwmsg(ySize-1,0,7,0,
               L"P)uolita ajat, L)isää aikoihin vakiolisäys, +: Tallenna, Esc: Poistu");
	  vidspwmsg(5,45,7,0,L"Luetun kortin koodi");
      vidint(6, 50, 7, em->badge0);
      vidspwmsg(8,45,7,0,L"Emit-maaliaika");
	  vidspaika_l(9,50,em->maali, t0, 8);
	  vidspwmsg(10,45,7,0,L"Tulos-maaliaika");
	  VIDSPAIKA(11,50,kilp->maali(0), t0);
	  for (r = 0; r < MAXNLEIMA; r++) {
		 swprintf(line, L"%2d %3d %5d |", r, cd[r], tm[r]);
		 x = r / (ySize-8);
		 vidspwmsg(r + 4 - (ySize-8)*x, 15*x, 7, 0, line);
		 }
	  if (i == -1) {
		 inputaika_lw(&etm, t0, 10, 22, 3, L"+lLpP\x1b\r\x05\x18\t", &wch);
		 if (wch == L'\r' || wch == KB_TAB)
			inputaika_lw(&emt, t0, 10, 43, 3, L"+lLpP\x1b\r\x05\x18\t", &wch);
		 i = 0;
		 }
	  else {
		 x = (i / (ySize-8)) * 15;
		 r = i % (ySize-8) + 4;
		 switch (j) {
			case 0 :
			   inputintw(&cd[i], 3, x+3, r, L"+lLpP\x1b\r\x05\x18\t", &wch);
			   break;
			case 1 :
			   tl = tm[i];
			   inputlongw(&tl, 5, x+7, r, L"+lLpP\x1b\r\x05\x18\t", &wch);
			   tm[i] = (UINT) tl;
			   break;
            }
		 wch = towupper(wch);
		 switch (wch) {
			case ESC :
			   wch = L' ';
			   wselectopt(L"Peruutetaanko kaikki muutokset (K/E)", L"KE", &wch);
			   if (wch == L'K') {
				  wch = ESC;
				  }
			   break;
			case KB_TAB :
			   j = 1-j;
			   break;
			case L'P' :
			   wch = L' ';
			   wselectopt(L"Jaetaanko kaikki leimausajat kahdella (K/E)", L"KE", &wch);
			   if (wch == L'K') {
				  for (r = 0; r < MAXNLEIMA; r++) {
					 tm[r] /= 2;
					 }
				  }
			   break;
			case L'L' :
			   tl = 0;
			   clrln(ySize-3);
			   vidspwmsg(ySize-3, 0, 7, 0, L"Anna kaikkiin aikoihin tuleva lisäys (sek)");
			   inputlongw(&tl, 6, 45, ySize-3, L"\r", &wch);
			   for (r = 1; r < MAXNLEIMA; r++) {
				  if ((r == 1 || tm[r]) && (tl + (INT32)tm[r]) >= 0 && tl + tm[r] < 32768L) {
					 tm[r] = (UINT) (tl + tm[r]);
					 }
				  }
			case L'\r' :
			case DOWN :
			   i++;
			   break;
			case UP :
			   i--;
			   break;
			case L'+' :
			   wch = L' ';
			   wselectopt(L"V)ahvista muutosten tallennus, J)atka muokkausta", L"VJ", &wch);
			   if (wch == L'V') {
				  wch = L'+';
				  }
			   break;
			}
		 if (i > MAXNLEIMA-1) {
			i = 0;
			}
		 }
	  }
   while (wch != ESC && wch != L'+');
   if (wch == '+') {
	  for (i = 0; i < MAXNLEIMA; i++) {
		 em->ctrlcode[i] = (char) cd[i];
		 em->ctrltime[i] = tm[i];
		 em->time = etm;
		 em->maali = emt;
		 }
	  putem(em, ep, 0);
	  }
   for (r = 4; r < ySize-4; r++) {
	  clrtxt(r, 0, 63);
	  }
   }
#endif

#endif // EMIT

void vapautaemitmuisti(bool vapautarata, bool vapautaleimat, bool vapautalainaemit, bool vapautaanal)
   {
   on_emit(0, 1);
   if (vapautalainaemit && lainaemit) {
		free(lainaemit);
		lainaemit = 0;
		}
   if (vapautaleimat && kuvio) {
	   free(kuvio);
	   kuvio = 0;
	   maxnkuvio = 0;
	   }
   if (vapautarata || vapautaanal) {
	   if (rvalit) {
		   for (int d = 0; d < rvluku; d++) {
			  for (int srj = 0; srj < sarjaluku; srj++) {
				 if (rvalit[d].ajat[srj])
					free(rvalit[d].ajat[srj]);
				 if (rvalit[d].kilpno[srj])
					free(rvalit[d].kilpno[srj]);
				 }
			  }
		  free(rvalit);
		  }
	   rvalit = 0;
	   if (vapautarata) {
		   if (rata) {
			  free(rata);
			  }
		   rata = 0;
		   nrata = 0;
		   }
	   if (rastiva)
		   delete[] rastiva;
	   rastiva = NULL;
	   }
   }

void lueRatatiedotUudelleen(void)
{
   EnterCriticalSection(&emit_CriticalSection);
   EnterCriticalSection(&rastiva_CriticalSection);
   vapautaemitmuisti(true, true, false, true);
   if (ratatiedot == 2) {
		CourseData.nollaa();
		lue_radat_xml(ySize-6);
		if (emitanal_fl)
			lueemitfile();
		}
   else {
		lue_leimat(ySize-6);
		lue_radat(ySize-5);
		}
   LeaveCriticalSection(&rastiva_CriticalSection);
   LeaveCriticalSection(&emit_CriticalSection);
}

void uusiemitanalyysi(void)
{
   if (!emitfl || !emitanal_fl)
		return;
   EnterCriticalSection(&rastiva_CriticalSection);
   vapautaemitmuisti(false, false, false, true);
   tarkkilpradat();
   TarkKilpailijat();
   lueemitfile();
   LeaveCriticalSection(&rastiva_CriticalSection);
}
