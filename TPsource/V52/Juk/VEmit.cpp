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
#include <process.h>
#include <dos.h>
#include <conio.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
//#include <stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#ifdef _CONSOLE
#include <bkeybrd.h>
#endif
#include <bstrings.h>
#pragma hdrstop
#include "VDeclare.h"

//#define DBG_1
//   int emitloki = 1;

#ifndef MAXTUNNUS
#define MAXTUNNUS  899
#endif
#define RINNTUNNUS  5

// Leimantarkastuksen pohjana on taulukko kuvio, johon on tallennettu
// luettelo kaikista hyv‰ksytt‰vist‰ rastitunnus-leimasinkoodi -yhdistelmist‰
// Luettelo on aina j‰rjestettyn‰ rastitunnusten mukaiseen j‰rjestykseen.
// Koodin arvo 1 tarkoittaa, ett‰ kaikki leimaukset hyv‰ksyt‰‰n.

//#pragma pack(push)
//#pragma pack(1)

int onrata(wchar_t *tunnus);
int onrata(char *tunnus);
int em_HaeSeuraava(int eptr);
int em_HaeEdellinen(int eptr);
int em_HaeViimeinen(void);
int em_HaeEnsimmainen(void);
INT haelukija(emittp *em);
INT32 ensvapaa(void);
static INT oikeakoodi(ratatp *rt, INT rasti, INT kdi, INT vapaajarj);
void emitva_uusinta(int cn, int tietue);
void lueRatatiedotUudelleen(void);
void addemitanal(emittp *em, int uusi);
int monirata = 2;
int maalileimasin;
static INT on_kuvio(INT rasti);
void tarkkilpradat(void);
static int TarkKilpailijat(void);
IOFCourseData CourseData;
int ratatiedot;
#ifndef _CONSOLE
rastivatp *rastiva;
#endif
rastikoordtp rastikoord[MAXNPISTE];
maptp Map;

typedef struct {
   INT32 tm;
   INT32 tl;
   UINT *va;
   INT *sj;
   } emitvatype;
/*
typedef struct {
   INT rasti;
   char koodi;
   int pisteet;
   } ekditp;
*/

//#pragma pack(pop)

INT32 vapaaemit = 201;

ekditp *kuvio;
INT nkuvio;
static INT maxnkuvio;
static INT emitosa, partiorap;
extern INT valinelen, vapagelen;

static int emitkdit;
static INT32 maxlaina;
INT lainatied;
static wchar_t *lainaemit;
static wchar_t kdimask[18] = {1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768};

#ifndef _CONSOLE
rvalitp *rvalit;
int rvluku;
int analvertosuus = 5;
int analvertmax = 10;
#endif
emiterotp *EmitErot;
int nEmitErot;

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
//	ennakko = -1;
//	ajanottotapa = 4;
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
	ennakko = -1;
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

	memset(tmpkdi, 0, sizeof(tmpkdi));
	if (kuvio) {
		if (*maxnkuvio && *kuvio) {
			memset(*kuvio, 0, *maxnkuvio*sizeof(ekditp));
			}
		else {
			if (*kuvio)
				free(*kuvio);
			*kuvio = NULL;
			*maxnkuvio = 0;
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
			kirj_err_file_w(msg, 1);
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
			if (k < 1000) {
				tmpkdi[nk].rasti = tn;
				tmpkdi[nk].koodi = k;
#ifdef AMPUSUUNN
				if (tn > 20 && tn < 21+kilpparam.valuku)
				vakoodi[tn-20] = k;
#endif
				++nk;
				}
			else if (kilpparam.rogaining) {
				if (nk == 0 || tmpkdi[nk-1].rasti != tn) {
					tmpkdi[nk].rasti = tn;
					tmpkdi[nk].koodi = tn;
					++nk;
					}
				tmpkdi[nk-1].pisteet = k/1000;
				}
			}
		}
	if (ikrd > 3)
		memcpy(rastikoord, koord, sizeof(rastikoord));
	if (!kuvio)
		return(0);
	*nkuvio = nk;
	if (nk == 0) {
		vidspwmsg(ySize-20,5,7,0,
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
	UINT32 i, j, ir, kdi[MAXNRASTI], mtk[MAXNRASTI], rno, nr = 0, ensilm = 1;
	int srj = 0, tn, maxnrata = 0, vapaajarj = 0, puute = 0;
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
	if (monirata) {
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
		}
	*nrata = (monirata < 2 ? sarjaluku : 0 ) + rno;

	if (*nrata == 0) {
		vidspwmsg(19, 0, 7, 0, L"Ohjelma ei lˆyt‰nyt ratoja xml-tiedostosta");
		return(1);
		}
	if (maxnrata == 0)
		*rt = (ratatp *) calloc(*nrata, sizeof(ratatp));
	if (!*rt) {
		writeerror_w(L"Ei tilaa ratatiedoille", 0);
		goto esc;
		}
	rno = (ToimintaTila == 2 && monirata < 2) ? sarjaluku : 0;
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
			if (srj < 0) {
				if (monirata) {
					unsigned int ii;
					rnimi[sizeof(rt[0][i].tunnus)/2-1] = 0;
					for (ii = 0; ii < rno; ii++)
						if (!wcscmpU(rt[0][ii].tunnus, rnimi))
							break;
					if (ii < rno) {
						swprintf(msg, L"Rata %s toistuu ratatiedoissa", rnimi);
						kirj_err_file_w(msg, 1);
						}
					srj = rno++;
					wcscpy(rt[0][srj].tunnus, rnimi);
					}
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
			mtk1 = 0;
			for (j = 0; j < sizeof(Course[0]->CourseVariation[iV]->CourseControl)/
				sizeof(Course[0]->CourseVariation[0]->CourseControl[iV]); j++) {
				wchar_t *pCode;

				if (Course[i]->CourseVariation[iV]->CourseControl[j] == NULL ||
					Course[i]->CourseVariation[iV]->CourseControl[j]->Code[0] == 0) {
					if (i == 0 || !vapaajarj)
						break;
					}
				else {
					pCode = Course[i]->CourseVariation[iV]->CourseControl[j]->Code;
					tn = _wtoi(pCode);
					if (tn == 0) {
						int tn1;

						tn1 = wcswcind(towupper(pCode[0]), L"JPV");
						if (tn1 >= 0) {
							if (vapaajarj >= 0) {
								tn = vapaajarj + 10000;
								}
							vapaajarj = tn1;
							}
						else if (!wmemcmpU(pCode, L"RR", 2)) {
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
				if (tn > 0 && (tn <= MAXTUNNUS || tn >= 10000)) {
					if (ToimintaTila == 2 && tn <= MAXTUNNUS && !on_kuvio(tn)) {
						swprintf(msg, L"Radan %s rastin %d tunnusta %d ei tiedoissa",
							rnimi, ir+1, tn);
						puute++;
						kirj_err_file_w(msg, 0);
						}
					if (ir >= MAXNRASTI) {
						swprintf(msg, L"Radalla %s liikaa rasteja (yli %d)",
							rnimi, MAXNRASTI);
						kirj_err_file_w(msg, 1);
						break;
						}
					kdi[ir] = tn;
					tn = 0;
					ir++;
					}
				if (Course[i]->CourseVariation[iV]->CourseControl[j] == NULL)
					break;
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
	clrln(ySize - 6);
	swprintf(msg, L"Siirretty %d rataa", nr);
	vidspwmsg(ySize-6,5,7,0,msg);
	if (puute > 0) {
		writeerror_w(L"Kaikille rasteille ei tiedoissa emit-koodia. K‰ytet‰‰n rastien koodeja", 0);
		}
	return(0);
esc:
	if (maxnrata == 0 && *rt)
		free(*rt);
	swprintf(msg,L"Virhe ratatietoja k‰sitelt‰essa. Rata %s", rnimi);
	kirj_err_file_w(msg, 1);
	return(1);
}

int haekoordix(wchar_t *tunnus)
{
	for (int i = 0; i < sizeof(rastikoord)/sizeof(rastikoord[0]); i++) {
		if (rastikoord[i].tunnus[0] == 0)
			break;
		if (!wcscmpU(rastikoord[i].tunnus, tunnus))
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
		if (kilpparam.rogaining)
			tmpkdi[ir].pisteet = kdi[ir].pisteet;
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
		if (kilpparam.rogaining && tmpkdi[ir].pisteet) {
			for (j = 0; j < (int)(sizeof(Point[k]->ECodes)/sizeof(Point[k]->ECodes[0])); j++) {
				if (Point[k]->ECodes[j] == 0) {
					Point[k]->ECodes[j] = 1000*tmpkdi[ir].pisteet;
					break;
					}
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
		int len = 0, pkdi;
		int partiokoodit[100];

		memset(partiokoodit, 0, sizeof(partiokoodit));
		Course[ir] = new IOFCourse;
		Course[ir]->CourseVariation[0] = new IOFCourseVariation;
		IOFCourseVariation *CVar = Course[ir]->CourseVariation[0];
		wcsncpy(Course[ir]->Name, rt[ir].tunnus, sizeof(Course[0]->Name)/2-1);
		wcsncpy(CVar->Name, rt[ir].tunnus, sizeof(CVar->Name)/2-1);
//		if (haesarja_w(rt[ir].tunnus, 0) >= 0)
//			wcsncpy(Course[ir]->ShortName[0], rt[ir].tunnus, sizeof(Course[0]->ShortName[0])/2-1);
		wcsncpy(CVar->Start, rt[ir].lahto, sizeof(CVar->Start)/2-1);
		CVar->ennakko = rt[ir].ennakko;
		CVar->ajanottotapa = rt[ir].maalilaji;
//		CVar->Length = rt[ir].matka[rt[ir].rastiluku-1];
//		CVar->Length += rt[ir].viitmatka;
		CVar->FinLeg = rt[ir].viitmatka;
		CVar->Length = rt[ir].ratapit;
		CVar->Climb = rt[ir].nousu;
		if (rt[ir].maali[0])
			wcsncpy(CVar->Finish, rt[ir].maali, sizeof(CVar->Finish)/2-1);
		else if (rt[ir].maalilaji == 1)
			wcscpy(CVar->Finish, L"F");
		pkdi = 0;
		for (int i = 0; i < rt[ir].rastiluku; i++) {
			if (rt[ir].rastikoodi[i] >= 10000 && rt[ir].rastikoodi[i] <= 10002)
				partiokoodit[pkdi++] = rt[ir].rastikoodi[i]-10000;
			}
		pkdi = 1;
		for (int ip = 0; ip < rt[ir].rastiluku; ip++) {
			CVar->CourseControl[ip] = new IOFCourseControl;
			if (rt[ir].rastikoodi[ip] >= 10000 && rt[ir].rastikoodi[ip] <= 10002) {
				CVar->CourseControl[ip]->Code[0] = L"JPV"[partiokoodit[pkdi++]];
				CVar->CourseControl[ip]->Code[1] = 0;
				}
			else {
				_itow(rt[ir].rastikoodi[ip], CVar->CourseControl[ip]->Code, 10);
				CVar->CourseControl[ip]->LegLength = rt[ir].matka[ip] - len;
				len = rt[ir].matka[ip];
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
	ratatp *xrata = NULL;

   if ((er = lueEventXml(rataxmlfname, false, true, false)) != 0)
		return(er);

   swprintf(msg, L"Ratatiedot luetaan tiedostosta %s", rataxmlfname);
   vidspwmsg(r, 0, 7, 0, msg);
   CourseData.siirraXmlKuviot(&kuvio, &nkuvio, &maxnkuvio);
   CourseData.siirraXmlRadat(&xrata, &nrata, L'V', 0, 0, -99);
   CourseData.siirraXmlKartta(&Map);
   if (rata)
		free(rata);
   rata = (ratatp *) calloc(nrata+1, sizeof(ratatp));
   for (int i = 0; i < nrata; i++) {
	   wcstoansi(xrata[i].rata, xrata[i].tunnus, sizeof(rata[i].rata)-1);
	   memcpy(rata+i, xrata+i, sizeof(rata[0]));
/*
	   wcstoansi(rata[i].rata, xrata[i].tunnus, sizeof(rata[i].rata)-1);
	   rata[i].rastiluku = xrata[i].rastiluku;
	   rata[i].ennakko = xrata[i].ennakko;
	   rata[i].maalilaji = xrata[i].maalilaji;
	   rata[i].koodi = (int *) calloc(rata[i].rastiluku, sizeof(int));
	   for (int j = 0; j < rata[i].rastiluku; j++) {
		   rata[i].koodi[j] = xrata[i].rastikoodi[j];
		   }
*/
	   }
   taydenna_leimat();
   ratatiedot = 2;
   tarkkilpradat();
   TarkKilpailijat();
   return(er);
}

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

INT lainakortti(kilptietue *kilp, INT os)
   {
   INT laina = 0;

   if (lainatied < 2 && kilp->ostiet[os].laina[0] == 'L') laina = 1;
   if (!laina && lainatied) {
	  laina = on_ekdi(lainaemit, kilp->ostiet[os].badge[0]);
	  }
   return(laina);
   }

INT haebdg(INT32 badge, int *toinen)
   {
   int ibdg = -1;

	// bsrchint palauttaa sijainnin kertovan negatiivisen arvon, ellei tarkkaa osumaa lˆydy
	// haebdg palauttaa t‰m‰n arvon ellei *toinen == -1, jolloin tarkkaa osumaa haetaan kummastakin 
	// jonosta ja palautettava arvo on >= 0 tai -1.

   if (toinen == NULL || *toinen <= 0) {
		ibdg = bsrchint(badge, bdg_kno[0], nbadge[0], sizeof(bdg_kno_tp), 0, 1);

		if (toinen == NULL || *toinen == 0 || ibdg >= 0) {
			if (toinen)
				*toinen = 0;
			return(ibdg);
			}
		}
   if (kilpparam.kaksibadge && (toinen != NULL || *toinen != 0)) {
		ibdg = bsrchint(badge, bdg_kno[1], nbadge[1], sizeof(bdg_kno_tp), 0, 1);
		if (*toinen == 1 || ibdg >= 0) {
			if (toinen)
				*toinen = 1;
			return(ibdg);
			}
		}
   return(-1);
   }

INT bdg2kno(INT32 badge, int *toinen)
   {
   INT k;

   k = haebdg(badge, toinen);
   if (k >= 0)
	   return(bdg_kno[toinen == 0 ? 0 : *toinen][k].kilpno);
   return(0);
   }

#ifdef TESTBDG
void bdgstatus(void)
   {
   int i, k;
   long bdg;
   INT em_pg[256];

   for (i = 2; i < ySize-4; i++) {
      clrtxt(i, 0, 64);
      }
   if (ems_fl) {
      bdg = 0;
      for (i = 0; i < 95; i++) {
         while (bdg < maxekoodi) {
            bdg++;
            getgrec_ems(em_pg, 2, bdg/256);
            k = em_pg[bdg & 255];
            if (k > 0) {
               vidint(i%(ySize-6)+2, 12*(i/(ySize-6)), 4, k);
               vidint(i%(ySize-6)+2, 12*(i/(ySize-6))+5, 5, bdg);
               break;
               }
            }
         if (bdg >= maxekoodi)
            break;
         }
      }
   else {
      for (i = 0; i < 95 && i < nbadge[0]; i++) {
         vidint(i%(ySize-6)+2, 12*(i/(ySize-6)), 4, bdg_kno[0][i].kilpno);
         vidint(i%(ySize-6)+2, 12*(i/(ySize-6))+5, 5, bdg_kno[0][i].badge);
         }
	  }
   }
#endif

int haeVapaaKoodi(int badge)
{
	int askel;

	if (badge == 200) {
		return(ensvapaa());
		}
	else {
		if (!esta_uusikaytto)
			return(badge);
		askel = BADGEASKEL;
		}
	for (int i = 1; i < kilpparam.n_os_akt; i++) {
		if (bdg2kno(badge+i*askel, 0) <= 0)
			return(badge+i*askel);
		}
	return(0);
}

INT addbadge(INT32 badge, INT kno, INT d, int toinen, INT msgfl)
   {
   INT i;
   char msg[50];

   if (!d && (i = getpos(kno)) <= 0)
      return(1);
   i = haebdg(badge, &toinen);
   if (i >= 0) {
      if (bdg_kno[toinen][i].kilpno == kno) return(0);
      if (msgfl) {
         sprintf(msg, "Joukkueella %d jo Emit-koodi %ld",
            bdg_kno[toinen][i].kilpno, badge);
         writeerror(msg, 0);
         }
      return(2);
      }
   i = -i - 1;
   memmove((char *)(bdg_kno[toinen]+i+1), (char *)(bdg_kno[toinen]+i),
      (nbadge[toinen]-i) * sizeof(bdg_kno_tp));
   nbadge[toinen]++;
   bdg_kno[toinen][i].kilpno = kno;
   bdg_kno[toinen][i].badge = badge;
#ifdef DBG_1
   if (loki && emitloki) {
	  sprintf(msg, "addbadge %ld, kno %d, d %d\n",
         badge, kno, d);
      sendline(msg, lokifile);
      }
#endif
#ifdef TESTBDG
   bdgstatus();
#endif
   return(0);
   }

INT rembadge(INT kno, INT os, int toinen, INT d)
   {
   INT i;
   INT32 bdg;
   kilptietue kilp;
#ifdef DBG_1
   char msg[80];
#endif

   if (d)
      i = d;
   else
      if ((i = getpos(kno)) <= 0)
         return(1);
   GETREC(&kilp, i);
      {
      if ((bdg = kilp.ostiet[os].badge[toinen]) != 0) {
         i = haebdg(bdg, &toinen);
         if (i < 0 || i >= nbadge[toinen]) {
/*
            if (loki && emitloki) {
               sprintf(msg, "Ekoodia %ld ei indeksiss‰\n", kilp.badge[0]);
               sendline(msg, lokifile);
               }
*/
#ifdef TESTBDG
            bdgstatus();
#endif
            return(1);
			}
         }
      if (bdg_kno[toinen][i].kilpno != kno) {
/*
         if (loki && emitloki) {
            sprintf(msg, "Ekoodi %ld liitetty joukkueeseen %d, ei poistettu\n",
               kilp.badge[0], kilp.kilpno);
            sendline(msg, lokifile);
            }
*/
         return(0);
         }
//      for (j = 0; j < Sarjat[kilp.sarja].osuusluku; j++)
//         if (j != os && kilp.ostiet[j].badge[0] == bdg) return(0);
      nbadge[toinen]--;
      memmove((char *)(bdg_kno[toinen]+i), (char *)(bdg_kno[toinen]+i+1),
         (nbadge[toinen]-i) * sizeof(bdg_kno_tp));
      bdg_kno[toinen][nbadge[toinen]].badge = 0;
      bdg_kno[toinen][nbadge[toinen]].kilpno = 0;
#ifdef DBG_1
      if (loki && emitloki) {
         sprintf(msg, "rembadge %ld, kno %d, os %d, d %d\n",
            kilp.ostiet[os].badge[toinen], kno, os, d);
         sendline(msg, lokifile);
         }
#endif
      }
#ifdef TESTBDG
   bdgstatus();
#endif
   return(0);
   }

INT32 ensvapaa(void)
   {
   INT32 kdi;

   for (kdi = vapaaemit; ; kdi++) {
      if (bdg2kno(kdi, 0) < 1) break;
      }
   vapaaemit = kdi + 1;
   return(kdi);
   }

INT remjkbadge(INT kno, INT32 bdg)
   {
   INT i, d;
   INT32 eno;
   kilptietue kilp, entkilp;
#ifdef DBG_1
   char msg[80];
#endif

   if ((d = getpos(kno)) <= 0)
      return(1);
	EnterCriticalSection(&tall_CriticalSection);
   GETREC(&kilp, d);
   entkilp = kilp;
   eno = ensvapaa();
   for (i = 0; i < Sarjat[kilp.sarja].osuusluku; i++) {
	  if (kilp.ostiet[i].badge[0] == bdg)
		 kilp.ostiet[i].badge[0] = eno;
	  }
   tallenna(&kilp, d, 0, 0, 0, &entkilp);
   LeaveCriticalSection(&tall_CriticalSection);
   return(0);
   }

#ifdef EI_OLE
void vaihda_badge(UINT32 badge)
   {
   INT d, kno, os, os0 = -1;
   INT32 ep;
   static UINT32 seurbdg = 1;
   kilptietue kilp;
   emittp em;

   kno = bdg2kno(badge);
   if ((d = getpos(kno)) <= 0) return;
   GETREC(&kilp, d);
   for (; haebdg(seurbdg, 0) >= 0; seurbdg++) ;
   for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
      if (kilp.ostiet[os].badge[0] == badge) {
         if (os0 < 0)
            os0 = os;
		 else
            kilp.ostiet[os].badge[0] = seurbdg;
         }
      }
   if (os0 < 0) return;
   PUTREC(&kilp,d);
   rembadge(kno, os0, 0, d);
   GETREC(&kilp, d);
   kilp.ostiet[os0].badge[0] = seurbdg;
   PUTREC(&kilp,d);
   putrec_i(&datf2, d, &kilp);
   addbadge(seurbdg, kno, d, 0, 1);
#if defined(MAALI)
   for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
	  if (kilp.ostiet[os].badge[0] == seurbdg) {
		 if ((ep = getem(&em, kno, os)) >= 0) {
			em.badge = seurbdg;
			putem(&em, ep, 0);
			}
		 }
	  }
#endif
   }
#endif  // EI_OLE

int onrata(wchar_t *wtunnus)
{
	char tunnus[30];

	wcstoansi(tunnus, wtunnus, 28);
	return(onrata(tunnus));
}

int onrata(char *tunnus)
{
   ratatp *rt;

   if (rata) {
	  for (rt = rata ; rt < rata+nrata; rt++)
		if (!strcmpU(rt->rata, tunnus))
			return(rt-rata+1);
	  }
   return(0);
}

INT32 e_maaliaika(emittp *em, kilptietue *kilp, INT os)
   {
   ratatp *rt;
//   INT tulkinta[51];
   INT k, l, ok, lk;
   INT32 tm = TMAALI0;

   lk = haelukija(em);
   if (lk > 49)
	  return(TMAALI0);
   for (k = 50; k > 1; k--) {
	  if (em->ctrltime[(lk+k)%50]) {
		 tm = em->time*KSEK - SEK*em->ctrltime[(lk+k)%50];
		 break;
		 }
	  }
   if (tm != TMAALI0 && (rt = getrata(kilp->ostiet[os].hajonta)) != NULL) {
	  for (l = k-1; l >= rt->rastiluku-2 && l >= 0; l--) {
		 if ((ok = oikeakoodi(rt, rt->rastiluku-1, em->ctrlcode[(lk+l)%50], 0)) != 0)
			break;
		 }
	  if (ok) {
		 if (rt->ennakko >= 0) {
			tm = oslahto(kilp, os) + SEK*(em->ctrltime[(lk+l)%50] - rt->ennakko);
			}
		 else {
			tm += SEK*em->ctrltime[(lk+l)%50];
			}
		 tm = (tm+36*TUNTI)%(24*TUNTI) - 12*TUNTI;
		 }
	  else
		 tm = TMAALI0;
	  }
   return(tm);
   }

#if defined(MAALI) && !defined(LUENTA)
 
void aikaruutu(int ino)
   {
   int ptime;
   INT fore, r;
   char st[20], as[20];
   aikatp ttm;

   if (!ajat_on || !aikajono[ino] || status_on || !seurkrivi) return;
   draw_hline2(0, 60, 16);
   draw_vline2(16, 61, 4);
   draw_grchar(1, 60, 14);
   draw_grchar(16, 60, 3);
   draw_grchar(16, 65, 13);
//   viwrrect(0,60,16,60,"∫π∫∫∫∫∫∫∫∫∫∫∫∫∫∫»",7,0,0);
//   viwrrect(16,61,16,65,"ÕÕÕÕÀ",7,0,0);
   for (r = 0; r < 16; r++) {
      memset(st, ' ', 19);
      fore = (r == 8) ? 0 : 7;
      if (seurkrivi + r >= 8) {
         ptime = seurkrivi - 8 + r;
		 if (ptime - aikajono[ino]->aktrows < 0) {
			 aikajono[ino]->getAktTime(&ttm, ptime);
            sprintf(st, "%4d %8.8s %4d ", ptime+1,
               AIKATOSTRS(as, purajak(ttm.t), t0), ttm.kno);
            }
         }
	  viwrrect(r, 61, r, 79, st, fore, 7-fore, 0);
      }
   }

INT etsios(INT kno, INT32 bdg)
   {
   INT d, os = 0;
   kilptietue kilp;

   if ((d = getpos(kno)) > 0) {
      GETREC(&kilp, d);
      for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++)
         if (kilp.ostiet[os].badge[0] == bdg) break;
      if (kilp.ostiet[os].badge[0] != bdg) os = 0;
      }
   return(os);
   }

int emittp::get(int i_ptr)
	{
	return(lueem(this, i_ptr));
	}

int emittp::getbykno(int kno, int os)
	{
	return(getem(this, kno, os));
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

INT lueem(emittp *em, INT ep)
   {
   if (ep < 0 || ep >= emithead || !ProsEmitOn) {
	  em->nollaa();
	  ep = -1;
	  }
   else {
	  EnterCriticalSection(&emit_CriticalSection);
	  _lseek(emitfile, (INT32) ep * sizeof(emittp), SEEK_SET);
	  _read(emitfile, em, sizeof(emittp));
	  LeaveCriticalSection(&emit_CriticalSection);
	  }
   return(ep);
   }

INT getem(emittp *em, INT kilpno, INT os)
   {
   INT ep;

   em->nollaa();
   if (kilpno < minkilpno || kilpno > maxkilpno) return(-1);
   ep = EmitJarr(kilpno, os);
   return(lueem(em, ep));
   }

int EmitJarr(int kilpno, int os)
{
	int d;
	if (emitfl < 1 || (d = getpos(kilpno)) < 1 || os < 0 || os >= kilpparam.n_os_akt) {
#ifdef _DEBUG
		if (kilpno)
			parametrivirhe(L"EmitJarr", kilpno, L"", L'0'+os);
#endif
		return(-1);
		}
	return(emitjarr[os][d]);
}

void setEmitJarr(int kilpno, int os, int ep)
{
	int d;
	if (emitfl < 1 || (d = getpos(kilpno)) < 1 || os < 0 || os >= kilpparam.n_os_akt) {
#ifdef _DEBUG
		parametrivirhe(L"setEmitJarr", kilpno, L"", L'0'+os);
#endif
		return;
		}
	emitjarr[os][d] = ep;
}

INT putem(emittp *em, INT32 ep, int inCrSe)
   {
   int err = 0;

   if (ep >= 0 && ep <= emithead) {
      if (!inCrSe)
         EnterCriticalSection(&emit_CriticalSection);
      if (_lseek(emitfile, ep * sizeof(emittp), SEEK_SET) < 0) {
		 writeerror_w(L"Virhe rutiinissa 'putem', koodi 1", 0);
		 err = 1;
		 }
	  else {
		 if (_write(emitfile, em, sizeof(emittp)) < sizeof(emittp)) {
		    writeerror_w(L"Virhe rutiinissa 'putem', koodi 2", 0);
			err = 1;
            }
         }
      if (!inCrSe)
         LeaveCriticalSection(&emit_CriticalSection);
      }
   return(err);
   }

static INT on_emit(emittp *em, int poista)
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
      emitvaraus = (INT32 *)calloc(kilpparam.n_os_akt*(maxrec+100), sizeof(INT32));
   if (!emitvaraus)
	  return(0);
   kdi = 4000*em->time + em->badge;
   pos = bsrchint(kdi, emitvaraus, nemitvar, 4, 0, 1);
   if (pos >= 0)
      return(1);
   pos = -pos - 1;
   if (nemitvar < kilpparam.n_os_akt*(maxrec+100)) {
      memmove(emitvaraus+pos+1, emitvaraus+pos, (nemitvar-pos)*sizeof(INT32));
      emitvaraus[pos] = kdi;
      nemitvar++;
      }
   return(0);
   }

INT addem(emittp *em, INT kielto)
	{
	int res = 0, d;
	kilptietue kilp;

	EnterCriticalSection(&emit_CriticalSection);
	if (estaemittoisto && on_emit(em, 0))
		res = 1;
	else {
		if ((res = putem(em, emithead, 1)) == 0) {
			if (em->kilpno && em->badge)
				setEmitJarr(em->kilpno, em->osuus, emithead);
			emithead++;
			fflush(femitfile);
			if (em->kirjaus != TMAALI0/KSEK) {
				if (emithead2 == emithead-1)
					emithead2 = emithead;
#ifndef _CONSOLE
				if (emitanal_fl)
				   addemitanal(em, 1);
#endif
				laheta_em(em, 1, kielto);
				if (kielto && comautorap && em->kilpno && (d = getpos(em->kilpno)) > 0) {
					GETREC(&kilp, d);
					if (!(autorap & 1) || autorapsrj[kilp.sarja] || tarkista(em, &kilp, 0, -1, 0)) {
#ifdef PARTIO
						if (kilpparam.partio)
							tark_partio(emittulparam, &kilp, 1, 0);
						if (!(autorap & 8))
#endif
							if (emittulparam)
								emit_rap(emittulparam, em->kilpno, em->osuus);
						}
					}
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
			kilp.getrec(getpos(Em.kilpno));
			if ((viim = e_maaliaika(&Em, &kilp, Em.osuus)) != TMAALI0 && kilp.Maali(Em.osuus, 0) != TMAALI0) {
				viim = ((kilp.Maali(Em.osuus, 0) - viim)/SEK + 36*3600L)%86400L - 43200L;
				EmitErot[nEmitErot].ero = viim;
				EmitErot[nEmitErot].kilpno = Em.kilpno;
				EmitErot[nEmitErot].pos = ep;
				EmitErot[nEmitErot].osuus = Em.osuus;
				EmitErot[nEmitErot].kh = kilp.wTark(Em.osuus, false);
				viim = viim+24*3600+48*3600*(!kilp.Hyv(Em.osuus));
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

#ifdef _CONSOLE
static void editkoodit(void)
   {
   FILE *leima_file;
   char leimafname[] = "leimat.lst";
   char ch;
   INT i, j, rasti = 0, kdi[RINNTUNNUS];

   selectopt("L)ue ratatiedot ja leimasinkoodit uudelleen, M)uokkaa koodeja", "LM\x1b", &ch);
   if (ch == ESC)
	   return;
   if (ch == 'L') {
	  lueRatatiedotUudelleen();
	  return;
	  }
   do {
	  clrln(ySize-3);
      vidspmsg(ySize-3,0,7,0,"Anna rastin tunnus:        Esc: poistu");
      INPUTINT(&rasti, 3, 20, ySize-3, "\r\x1b", &ch);
      if (ch == ESC) break;
      if (rasti <= 0 || rasti > MAXTUNNUS) {
         writeerror("Tunnus liian suuri tai pieni", 0);
         continue;
         }
      for (i = 0; i < nkuvio; i++)
         if (kuvio[i].rasti == rasti) break;
	  if (i >= nkuvio) {
         writeerror("Tunnusta ei esiinny ratatiedoissa", 0);
         continue;
         } 
      vidspmsg(ySize-3, 27, 7, 0, "Emit-koodit  ");
      clrln(ySize-1);
      vidspmsg(ySize-1, 0, 7, 0, "Emit-koodien arvot: 1 kaikki kelpaavat, 2 leimausta ei vaadita");
      for (j = 0; j < RINNTUNNUS; j++, i++) {
         if (i < nkuvio && kuvio[i].rasti == rasti)
            kdi[j] = kuvio[i].koodi;
         else
            kdi[j] = 0;
         vidint(ySize-3, 40 + 5*j, 3, kdi[j]);
         }
	  for (j = 0; j < RINNTUNNUS; j++) {
         INPUTINT(&kdi[j], 3, 40+5*j, ySize-3, "\r", &ch);
         }
      ch = ' ';
      selectopt("Vahvista muutokset (K/E), Esc: poistu vahvistamatta", "KE\x1b", &ch);
      if (ch == 'K') {
         i -= RINNTUNNUS;
		 qsortint(kdi, RINNTUNNUS, sizeof(INT), 0, sizeof(INT)==4);
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
               writeerror("Kooditaulu t‰ynn‰. Lis‰‰minen ei onnistu", 0);
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
         ch = ' ';
         selectopt("Kirjoitetaanko muutetut tiedot levylle (K/E)", "KE", &ch);
         if (ch == 'K') {
			if ((leima_file = fopen(leimafname,"w")) == NULL) {
               writeerror("Tiedoston LEIMAT.LST avaaminen ei onnistu", 0);
               }
            else {
               for (i = 0; i < nkuvio; i++)
                  fprintf(leima_file, "%d  %d\n",
                     kuvio[i].rasti, (int) kuvio[i].koodi);
               fclose(leima_file);
               }
            }
         }
      } while (ch != ESC);
   clrln(ySize-1);
   }
#endif // __CONSOLE

void lueemitkoodit(void)
   {
   FILE *emitkdfile;
   char ch, fname[] = "EMITKDI.LST", msg[80];
   INT d, os, nmuut = 0, merklaina;
   INT32 nro;
   UINT kdi;
   kilptietue kilp;

   if ((emitkdfile = fopen(fname, "r")) == NULL) {
      writeerror("Tiedostoa EMITKDI.LST ei lˆydy", 0);
      return;
      }
   ch = ' ';
   selectopt("Merkit‰‰nkˆ luetut koodit lainakorteiksi (K/E)", "KE", &ch);
   merklaina = ch == 'K';
   for (;;) {
      if (fscanf(emitkdfile, "%ld %d %u\n", &nro, &os, &kdi) < 2) break;
      d = getpos((INT) nro);
      if (d > 0) {
		 EnterCriticalSection(&tall_CriticalSection);
         GETREC(&kilp, d);
		 if (os <= kilpparam.n_os_akt) {
			 kilp.ostiet[os-1].badge[0] = kdi;
			 addbadge(kdi, kilp.kilpno, 0, 0, 1);
			 kilp.ostiet[os-1].laina[0] = merklaina ? 'L' : 0;
			 PUTREC(&kilp, d);
			 putrec_i(&datf2,d,&kilp);
			 }
         LeaveCriticalSection(&tall_CriticalSection);
         nmuut++;
		 }
      }
   sprintf(msg, "%d koodia tallennettu", nmuut);
   writeerror(msg, 0);
   fclose(emitkdfile);
   }

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

   if (!kdi || !rt || rasti >= rt->rastiluku) return(0);
   for (;;) {
	  for (i = 0; i < nkuvio; i++) {
		 if (kuvio[i].rasti > rt->rastikoodi[rasti]) break;
		 if (kuvio[i].rasti < rt->rastikoodi[rasti]) continue;
		 if (kuvio[i].koodi && kuvio[i].koodi <= 2) return(kuvio[i].koodi);
		 if (kdi == kuvio[i].koodi)
			return(1);
		 }
	  if (!vapaajarj || !rasti || rt->rastikoodi[rasti-1] == 10000) {
         break;
         }
      rasti--;
      }
   return(0);
   }

ratatp *getrata(char *tunnus)
   {
   ratatp *rt = NULL;
   INT i;

   for (i = 0; i < nrata; i++) {
	  if (!strcmpU(tunnus, rata[i].rata)) {
         rt = rata+i;
		 break;
         }
      }
   return(rt);
   }

ratatp *getrata(wchar_t *tunnus)
   {
   ratatp *rt = NULL;
   INT i;

   for (i = 0; i < nrata; i++) {
	  if (!wcscmpU(tunnus, rata[i].tunnus)) {
		 rt = rata+i;
		 break;
		 }
	  }
   return(rt);
   }

int getratano(char *tunnus)
   {
   INT i;

   for (i = 0; i < nrata; i++) {
	  if (!strcmpU(tunnus, rata[i].rata)) {
		 return(i);
		 }
	  }
   return(-1);
   }

int getratano(wchar_t *tunnus)
   {
   INT i;

   for (i = 0; i < nrata; i++) {
	  if (!wcscmpU(tunnus, rata[i].tunnus)) {
		 return(i);
		 }
	  }
   return(-1);
   }

INT haelukija(emittp *em)
   {
   int i;

   if (!em->ctrlcode[0]) {
      for (i = 49; i >= 0; i--)
         if (em->ctrlcode[i] && em->ctrlcode[i] != 254)
			break;
      return(i);
      }
   for (i = 49; i >= 0; i--) {
      if (em->ctrlcode[i] == 254)
         return((i+49)%50);
      }
   for (i = 0; i < 49; i++) {
      if (em->ctrltime[i+1] < em->ctrltime[i] && 
         (em->ctrlcode[i] == 250 || (em->ctrlcode[i] > 121 && em->ctrlcode[i] < 126)))
         return(i);
      if (em->ctrltime[(i+2)%50] < em->ctrltime[i] && 
         em->ctrlcode[i] == 250 && em->ctrlcode[i+1] != 250)
         return(i);
	  }
   for (i = 0; i <= 49; i++) {
      if ((em->ctrlcode[i] == 250 || (em->ctrlcode[i] > 121 && em->ctrlcode[i] < 126)) &&
         em->ctrlcode[(i+1)%50] != em->ctrlcode[i] && em->ctrlcode[(i+2)%50] != em->ctrlcode[i])
         break;
      }
   return(i);
   }

int vatulkinta(emittp *em, int *valiajat)
{
	int i, j, k, n, m, enn, rst, tulkinta[50], ala = 0, yla = 49, r1, r2, r3;
	int puuttuu, ylim, vaara, arvio[50], alku[50], lukijat[50];
	ratatp *rt = 0;
	kilptietue kilp;

	memset(valiajat, 0, MAXEMITVA*sizeof(int));
    if ((i = getpos(em->kilpno)) <= 0) 
		return(-1);
    GETREC(&kilp, i);
	enn = haeennakko(em, &kilp, em->osuus);
    rt = getrata(kilp.ostiet[em->osuus].hajonta);
	if (!rt)
		return(1);
	memset(arvio, 0, sizeof(arvio));
	memset(alku, 0, sizeof(alku));
	memset(lukijat, 0, sizeof(lukijat));
	n = 0;
	for (i = 0; i < 50; i++) {
		if (em->ctrlcode[i] == 254) {
			ala = i-1;
			yla = i-1;
			break;
			}
		}
	for (m = ala; m <= yla; m++) {
		if (em->ctrlcode[m] != 250)
			continue;
		lukijat[n] = m;
		i = m;
		if (tarkista(em, &kilp, tulkinta, i, 0) < 0)
			return(1);
		puuttuu = rt->rastiluku + 1;
		ylim = 0;
		vaara = 0;
		rst = rt->rastiluku + 1;
		while (1) {
			if (i == 0 && em->ctrlcode[0] == 0)
				break;
			if (i > 0 && em->ctrlcode[i] == 0 && em->ctrlcode[i-1] == 0)
				break;
			j = (i + MAXEMITVA - 1) % MAXEMITVA;
			k = (i + MAXEMITVA - 2) % MAXEMITVA;
			if (tulkinta[i] == rst || tulkinta[i] == -rst) {
				--puuttuu;
				rst--;
				}
			else if (tulkinta[j] == rst || tulkinta[j] == -rst) {
				++ylim;
				--puuttuu;
				rst--;
				i = j;
				j = (i + MAXEMITVA - 1) % MAXEMITVA;
				k = (i + MAXEMITVA - 2) % MAXEMITVA;
				}
			else if (tulkinta[k] == rst || tulkinta[k] == -rst) {
				ylim += 2;
				--puuttuu;
				rst--;
				i = k;
				j = (i + MAXEMITVA - 1) % MAXEMITVA;
				k = (i + MAXEMITVA - 2) % MAXEMITVA;
				}
			else if (rst > 1 && tulkinta[i] == -(rst-1)) {
				--puuttuu;
				rst -= 2;
				}
			else if (rst > 1 && tulkinta[j] == -(rst-1)) {
				++vaara;
				rst -= 2;
				--puuttuu;
				i = j;
				j = (i + MAXEMITVA - 1) % MAXEMITVA;
				k = (i + MAXEMITVA - 2) % MAXEMITVA;
				}
			else if (rst > 1 && tulkinta[k] == -(rst-1)) {
				vaara += 2;
				rst -= 2;
				--puuttuu;
				i = k;
				j = (i + MAXEMITVA - 1) % MAXEMITVA;
				k = (i + MAXEMITVA - 2) % MAXEMITVA;
				}
			else if (rst > 2 && tulkinta[i] == -(rst-2)) {
				rst -= 3;
				--puuttuu;
				}
			else if (rst > 2 && tulkinta[j] == -(rst-2)) {
				++vaara;
				rst -= 3;
				--puuttuu;
				i = j;
				j = (i + MAXEMITVA - 1) % MAXEMITVA;
				k = (i + MAXEMITVA - 2) % MAXEMITVA;
				}
			else if (rst > 2 && tulkinta[k] == -(rst-2)) {
				vaara += 2;
				rst -= 3;
				--puuttuu;
				i = k;
				j = (i + MAXEMITVA - 1) % MAXEMITVA;
				k = (i + MAXEMITVA - 2) % MAXEMITVA;
				}
			if (rst <= 0)
				break;
			if (em->ctrlcode[j] == 250 || em->ctrlcode[j] == 254)
				break;
			i = (i + MAXEMITVA - 1) % MAXEMITVA;
			}
		alku[n] = i;
		arvio[n] = 5*(puuttuu + vaara) + ylim;
		n++;
		}
	if (n < 1)
		return(-1);
	k = n-1;
	for (i = n-2; i >= 0; i--) {
		if (arvio[i] < arvio[k])
			k = i;
		}
	if (tarkista(em, &kilp, tulkinta, lukijat[k], 0) < 0)
		return(1);
	n = 24*TUNTI;
	memset(arvio, 0, sizeof(arvio));
	for (i = lukijat[k]; i != (alku[k] + MAXEMITVA - 1) % MAXEMITVA;) {
		if (tulkinta[i] > 0 && valiajat[tulkinta[i]-1] == 0 && em->ctrltime[i] < n) {
			valiajat[tulkinta[i]-1] = em->ctrltime[i] - enn;
			arvio[i] = tulkinta[i];
			n = em->ctrltime[i];
			}
		i = (i + MAXEMITVA - 1) % MAXEMITVA;
		}
	n = 0;
	for (i = alku[k]; i != (lukijat[k] + 1) % MAXEMITVA; ) {
		if (arvio[i] > n)
			n = arvio[i];
		else
			arvio[i] = n;
		i = (i + 1) % MAXEMITVA;
		}
	n = 0;
	for (i = lukijat[k]; i != (alku[k] + MAXEMITVA - 1) % MAXEMITVA;) {
		if (tulkinta[i] != 0) {
			m = abs(tulkinta[i]);
			if (valiajat[m-1] == em->ctrltime[i] - enn)
				n = m;
			else {
				for (j = n-1; j > arvio[i]; j--) {
					if (oikeakoodi(rt, j-1, em->ctrlcode[i], 0)) {
						if (n - j > 1 && (
							((r1 = abs(tulkinta[(i + MAXEMITVA - 1) % MAXEMITVA])) > j && r1 < n) ||
							((r2 = abs(tulkinta[(i + MAXEMITVA - 2) % MAXEMITVA])) > j && r2 < n) ||
							((r3 = abs(tulkinta[(i + MAXEMITVA - 3) % MAXEMITVA])) > j && r3 < n)))
							break;
						valiajat[j-1] = em->ctrltime[i] - enn;
						n = j;
						break;
						}
					}
				}
			}
		i = (i + MAXEMITVA - 1) % MAXEMITVA;
		}
	return(0);
}

// tarkista palauttaa
// -1 virhetilanteissa
// 0  kun leimat kunnossa
// >0 kun leimoissa virhe palautettavan arvon kohdalla

INT tarkista(emittp *em, kilptietue *pkilp, INT *tulkinta, int lukija, INT haku)
   {
   ratatp *rt = 0;
   INT i, j, k, kk = 999, m, oikeat[MAXNRASTI], vapaajarj = 0;
   kilptietue kilp;

   if (!haku) {
	  if (tulkinta) memset(tulkinta, 0, 50*sizeof(INT));
	  if (pkilp)
			kilp = *pkilp;
	  else {
			if ((i = getpos(em->kilpno)) <= 0)
				return(-1);
			GETREC(&kilp, i);
			}
	  rt = getrata(kilp.ostiet[em->osuus].hajonta);
	  }
   else {
	  // T‰t‰ k‰ytet‰‰n funktiossa etsiesarja(em)

	  if (tulkinta && *tulkinta >= 0 && *tulkinta < nrata) {
		 rt = rata + *tulkinta;
		 tulkinta = 0;
		 }
	  }
   if (!rt || !rt->rastiluku)
	   return(-1);

   // Kortille luetut koodit k‰yd‰‰n l‰pi lopusta alkaen.
   // Aluksi ohitetaan nolla-koodit.
//   j = 49;
//   while (!em->ctrlcode[j] && j) j--;
//   if (!j) return(rt->rastiluku);

   if (lukija < 0)
		lukija = haelukija(em);
   if (lukija < 0)
	   return(-1);
   if (lukija > 49) return(rt->rastiluku);
   j = lukija;

   memset(oikeat, 0, sizeof(oikeat));
   
   // Nyt j osoittaa viimeiseen kirjattuun koodiin, jonka pit‰isi olla 
   // lukijan koodi (yleens‰ 250)
   // Jos kortti luetaan toistuvasti voi loppuun tulla uusia rivej‰
   // joilla on lukijan koodi, mutta aika nolla. N‰m‰ ohitetaan.

   while (j > (lukija+1)%50 && em->ctrlcode[j] == em->ctrlcode[(j+49)%50] && !em->ctrltime[j])
      j = (j+49)%50;
   m = j;

   // Rata k‰yd‰‰n l‰pi alkaen viimeisest‰ rastista

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

	  if (rt->rastikoodi[i] >= 10000 && rt->rastikoodi[i] <= 10002) {
		 vapaajarj = rt->rastikoodi[i] - 10000;
         oikeat[i] = 2;
		 continue;
         }

      // Jos oikeakoodi palauttaa arvon 2, on rasti merkitty puuttuvaksi

      if (oikeakoodi(rt, i, 1, 0) == 2) {
         oikeat[i] = 1;
         continue;
         }

	  // Seuraavassa loopissa k‰sitell‰‰n virheelliset koodit. Loopista
      // poistutaan (ennen sen suoritusta), kun
      // -  j == 0 (T‰m‰ vastaa aina nollausleimausta) tai
      // -  on lˆydetty vuorossa oleva oikea koodi tai
      // -  i vastaa rataan kuulumatonta lukijalaitetta
      //
      // Vuorossa olevalle koodille annetaan tulkinta (jos pyydetty)
      // seuraavasti.
      // -  tulkinta annetaan negatiivisena (virheen merkkin‰)
      // -  tulkinnan itseisarvo on rastiluku+1, jos lukija
	  // -  jos koodi vastaa yht‰ tai useampaa radan rastia, on tulkinnan
      //    itseisarvo viimeisen t‰llaisen rastin j‰rjestysnumero
      // -  muissa tapauksissa tulkinnaksi j‰‰ 0
      //
	  // Virheellist‰ koodia k‰‰nteisess‰ k‰sittelyj‰rjestyksess‰
      // seuraavat nolla-koodit ohitetaan (t‰t‰ ei pit‰isi tapahtua
      // muulloin kuin, kun on tavattu nollauslaite

      while (j != lukija && j != (lukija+1)%50 && !oikeakoodi(rt, i, em->ctrlcode[j], vapaajarj) &&
         (i < rt->rastiluku || (em->ctrlcode[(j+1)%50] != 0 &&
         em->ctrlcode[(j+1)%50] != em->ctrlcode[j]))) {
         if (tulkinta) {
            for (k = rt->rastiluku; k >= 0; k--) {
               if ((k == rt->rastiluku && (em->ctrlcode[(j+1)%50] == 0 ||
                  em->ctrlcode[(j+1)%50] == em->ctrlcode[j])) ||
                  oikeakoodi(rt, k, em->ctrlcode[j], 0) == 1) {
                  if (k == rt->rastiluku || !tulkinta[j]) {
                     tulkinta[j] = -(k+1);
                     if (kk <= k) {
                        for (m = kk;m >= 0; m--) {
                           if (oikeakoodi(rt, m, em->ctrlcode[j], 0) == 1) {
                              tulkinta[j] = -(m+1);
                              break;
                              }
                           }
                        }
							kk = -tulkinta[j]-1;
                     for (m = -tulkinta[j]-1; m >= 0; m--) {
						if (rt->rastikoodi[m] > 9999 && rt->rastikoodi[m] <= 10002 && tulkinta[j] < 0) {
						   tulkinta[j]++;
                           }
                        }
                     }
                  break;
                  }
               }
            }
         do {
            j = (j+49)%50;
			} while (!em->ctrlcode[j] && j != (lukija+1)%50);
         }
//      if (j == (lukija+1) % 50) break;
      if ((j == (lukija+1) % 50) || (j == lukija && i < rt->rastiluku)) break;

      // T‰h‰n tullaan vain, kun j vastaa vuorossa olevaa oikeaa rastia
      // tai kohdalla rastiluku+1 olevaa lukijalaitetta

	  // i:n arvoa pienennet‰‰n, jos kyseess‰ lukijalaite, jonka koodi
      // lˆytyy radan viimeiselt‰ ilmoitetulta rastilta eli kun lukija
      // sis‰ltyy rataan

      if (i == rt->rastiluku && oikeakoodi(rt, i-1, em->ctrlcode[j], 0)) i--;

      if (!vapaajarj) {
         oikeat[i] = 1;
         if (tulkinta) {
            tulkinta[j] = i+1;
            kk = i;
            }
         }
      else {
         for (m = i; m >= 0; m--) {
			if (rt->rastikoodi[m] == 10000) {
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

      if (tulkinta && tulkinta[j] > 0) {
		 m = (tulkinta[j] < rt->rastiluku) ? tulkinta[j] - 1 : rt->rastiluku - 1;
         for (; m >= 0; m--) {
			if (rt->rastikoodi[m] > 9999 && rt->rastikoodi[m] <= 10002 && tulkinta[j] > 0) {
               tulkinta[j]--;
               }
            }
         }

      // Oikeaa koodia k‰‰nteisess‰ k‰sittelyj‰rjestyksess‰ 
      // seuraavat nolla-koodit ohitetaan (t‰t‰ ei pit‰isi tapahtua
	  // muulloin kuin, kun on tavattu nollauslaite)

      do {
         j = (j+49)%50;
         } while (!em->ctrlcode[j] && j != lukija);
      }
	if (tulkinta) {
	   for (j = 0; j < 49; j++) {
		   if (tulkinta[j] > 0 && tulkinta[(j+49)%50] < 0 && tulkinta[(j+1)%50] == tulkinta[(j+49)%50] - 2) {
			   if (oikeakoodi(rt, -tulkinta[(j+49)%50], em->ctrlcode[j], vapaajarj))
				   tulkinta[j] = tulkinta[(j+49)%50] - 1;
			  }
			}
		}
	if (kilpparam.partio) {
	   m = 0;
	   for (i = rt->rastiluku-1; i >= 0; i--) {
		   if (rt->rastikoodi[i] == 10002) {
			   m = 1;
			   oikeat[i] = 2;
				}
		   else if (rt->rastikoodi[i] > 9999) {
			   m = 0;
			   oikeat[i] = 2;
				}
		   else {
			   if (m)
				   oikeat[i] = 1;
				}
			}
		}
   for (i = 0; i < rt->rastiluku; i++)
      if (!oikeat[i]) break;
   if (i >= rt->rastiluku) return(0);
   else return(i + 1);
   }

INT etsiesarja(emittp *em)
   {
   INT i, nr = 0, r = -1;

   for (i = 0; i < nrata; i++) {
	  if (!tarkista(em, NULL, &i, -1, 1)) {
         if (rata[i].rastiluku > nr) {
            r = i;
            nr = rata[i].rastiluku;
            }
         }
      }
   return(r);
   }

#ifdef PARTIO

INT seur_laji(ratatp *rt, INT r)
   {
   r++;
   for (; r < rt->rastiluku; r++) {
	  if (rt->rastikoodi[r] > 9999 && rt->rastikoodi[r] <= 10002) {
		 return(rt->rastikoodi[r]);
		 }
      }
   return(10000);
   }

int rogpisteet(ratatp *rt, int rasti)
	{
	int i, r;

	for (r = 0; r < rt->rastiluku; r++) {
		if (rt->rastikoodi[r] < 9999)
			rasti--;
		if (rasti == 0)
			break;
		}
	for (i = 0; i < nkuvio; i++)
		if (kuvio[i].rasti == rt->rastikoodi[r])
			return(kuvio[i].pisteet);
	return(0);
	}

INT tark_partio(tulostusparamtp *tulprm, kilptietue *kilp, INT partiorap, int vajaat)
   {
   emittp em;
   INT tulkinta[50], p_tulkinta[MAXNRASTI][MAXOSUUSLUKU];
   int pisteet[MAXOSUUSLUKU], jpisteet = 0, entpisteet = -1000000;
   ratatp *rt;
   INT i, ii, j, jj, jjj, lj, k, os, r, kaikki, ll, rluku;
   INT32 tl, tls;
   char *p, ch, kh;
   wchar_t *wp, line[100], as[20], st[80];

   for (os = 0; os < Sarjat[kilp->sarja].osuusluku; os++) {
	  if (EmitJarr(kilp->kilpno, os) < 0)
		 break;
	  }
	if (os < Sarjat[kilp->sarja].osuusluku) {
		if (!vajaat)
			return(-1);
		}
	else if (vajaat)
		entpisteet = kilp->pisteet[0];
   rt = getrata(kilp->ostiet[0].hajonta);
   if (!rt || !rt->rastiluku) return(-1);
	rluku = 0;
	for (i = 0; i < rt->rastiluku; i++)
		if (rt->rastikoodi[i] < 9999)
			rluku++;
   memset(p_tulkinta, 0, sizeof(p_tulkinta));
   memset(pisteet, 0, sizeof(pisteet));
   for (os = 0; os < Sarjat[kilp->sarja].osuusluku; os++) {
      if (getem(&em, kilp->kilpno, os) < 0)
		  continue;
	  if (tarkista(&em, kilp, tulkinta, -1, 0) < 0)
		  continue;
      for (i = 0; i < 50; i++) {
		 if ((k = abs(tulkinta[i])) != 0) {
            if (!p_tulkinta[k-1][os]) {
               p_tulkinta[k-1][os] = i+1;
			   if (kilpparam.rogaining) {
				   pisteet[os] += rogpisteet(rt, k);
				   }
			   }
            }
		 }
	  }
   memset(tulkinta, 0, sizeof(tulkinta));
   if (kilpparam.rogaining) {
	   for (i = 0; i < rt->rastiluku; i++) {
		   for (os = 0; os < Sarjat[kilp->sarja].osuusluku; os++)
				if (p_tulkinta[i][os] > 0)
					tulkinta[i] = 1;
			}
	   for (i = 0; i < rt->rastiluku; i++) {
		   if (tulkinta[i])
			   jpisteet += rogpisteet(rt, i+1);
		   }
	   for (os = 0; os < Sarjat[kilp->sarja].osuusluku; os++)
		   kilp->ostiet[os].sakko = pisteet[os];
	   kilp->pisteet[0] = jpisteet;
	   kilp->pisteet[1] = jpisteet;
	   if (maxaika[kilp->sarja] && (tls = tulos(kilp, Sarjat[kilp->sarja].osuusluku-1,0)) > 0 
		   && tls > maxaika[kilp->sarja]) {
		   if (sakkoaskel[kilp->sarja]) {
			   kilp->pisteet[0] -= 
				   ((tls - maxaika[kilp->sarja] + sakkoaskel[kilp->sarja] - SEK) / sakkoaskel[kilp->sarja])
				       * askelsakko[kilp->sarja];
			  }
	       }   
	   os = Sarjat[kilp->sarja].osuusluku - 1;
		}
   else {
	   os = Sarjat[kilp->sarja].osuusluku - 1;
	   kaikki = os + 1;
	   k = 0;
	   ii = 0;
	   lj = seur_laji(rt, 0);
	   for (i = 0;i < rt->rastiluku; i++) {
		  if (rt->rastikoodi[i] >= 10000 && rt->rastikoodi[i] <= 10002) {
			 lj = seur_laji(rt, i);
			 kaikki = (lj <= 10001) ? os+1 : 1;
			 tulkinta[i] = -1;
			 continue;
			 }
		  jj = kaikki;
		  for (j = 0; j <= os; j++) {
			 if (p_tulkinta[ii][j]) {
				if (lj == 10000) {
				   for (jjj = 0; jjj < rluku; jjj++) {
					  if (p_tulkinta[ii][j] && (
						  (jjj < ii &&
						   p_tulkinta[jjj][j] > p_tulkinta[ii][j]) ||
						  (jjj > ii && p_tulkinta[jjj][j] &&
						   p_tulkinta[jjj][j] < p_tulkinta[ii][j])))
						 break;
					  }
				   if (jjj == rluku)
					  jj--;
				   }
				else
				   jj--;
				}
			 }
		  if (jj > 0)
			 k++;
		  ii++;
		  }
	   kh = kilp->Tark(os, 1);
	   if (stschind(kh, "ITHSK") < 0) {
		  kilp->SetTark(os, k ? 'I' : 'T');
		  }
	   }
	if (tulprm && tulprm->lstf && partiorap || (
		k >= 0 && autorap && 
		(!(autorap & 1 && k == 0) || autorapsrj[kilp->sarja]))
		) {
		if (partiorap == 2) {
			for (i = 2; i < ySize-4; i++)
				clrtxt(i, 0, 63);
			}
		else {
			p = 0;
//			if (printer == LJETII) 
//				p = initfont_emva;
#ifdef WIN32
			if (tulprm->printer == GDIPRINTER)
				wp = L"C";
#endif
			initpage(tulprm, wp);
			initline(tulprm);
#ifdef WIN32
			if (tulprm->printer == GDIPRINTER)
				tulprm->tulmuot.numlev = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
#endif
			if (tulprm->language == 0)
				swprintf(line, L"Tulostettu %s kello %s", wpaiva(), wkello());
			else
				swprintf(line, L"Printed %s at %s", wpaiva(), wkello());
			putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
			endline(tulprm, 1);
			}
		swprintf(line, L"%d %-26.26s", kilp->kilpno, kilp->Joukkue(st, 26));
		if (partiorap == 2) {
			r = 0;
			vidspwmsg(2,0,7,0,line);
			if (kilpparam.rogaining) {
				tls = tulos(kilp, Sarjat[kilp->sarja].osuusluku-1, 0);
				if (tulprm->language == 0)
					swprintf(line, L"Pisteet: %d (%d)/%8.8s", kilp->pisteet[0],
						kilp->pisteet[1]-kilp->pisteet[0], aikatowstr_ls(as, tls, 0));
				else
					swprintf(line, L"Points: %d (%d)/%8.8s", kilp->pisteet[0],
						kilp->pisteet[1]-kilp->pisteet[0], aikatowstr_ls(as, tls, 0));
				vidspwmsg(2,35,7,0,line);
				if (entpisteet > -1000000 && entpisteet != kilp->pisteet[0]) {
					if (tulprm->language == 0)
						swprintf(line, L"Tallennetut pisteet: %d", entpisteet);
					else
						swprintf(line, L"Stored points: %d", entpisteet);
					vidspwmsg(3,35,7,0,line);
					r++;
					}
				}
			}
		else {
			putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
			endline(tulprm, 0);
			line[0] = 0;
			for (i = 0; i <= os; i++) {
				swprintf(line, L"   %-24.24s   Emit: %ld %c", kilp->Nimi(st, OSNIMIL, i), 
					kilp->ostiet[i].badge[0], kilp->ostiet[i].laina[0]);
				putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
				endline(tulprm, 0);
				}
			endline(tulprm, 0);
			if (tulprm->language == 0)
				swprintf(line, L"Sarja              %s", Sarjat[kilp->sarja].Sarjanimi(st));
			else
				swprintf(line, L"Class              %s", Sarjat[kilp->sarja].Sarjanimi(st));
			putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
			endline(tulprm, 0);
			tl = osuuslahto(kilp, os);
			if (tl > VRK)
				tl = NORMKELLO(tl);
			if (tulprm->language == 0)
				swprintf(line, L"L‰htˆaika          %8.8s", aikatowstr_ls(as, tl, t0));
			else
				swprintf(line, L"Start time         %8.8s", aikatowstr_ls(as, tl, t0));
			putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
			endline(tulprm, 0);
//          tls = 0;
//          if (kilp->Maali(os, 0) != TMAALI0)
//             tls = kilp->Maali(os, 0) - tl;
			tls = tulos(kilp, Sarjat[kilp->sarja].osuusluku-1, 0);
			if (!kilpparam.rogaining) {
				if (tulprm->language == 0)
					swprintf(line, L"Tulos              %8.8s", aikatowstr_ls(as, tls, 0));
				else
					swprintf(line, L"Result             %8.8s", aikatowstr_ls(as, tls, 0));
				}
			else {
				if (tulprm->language == 0)
					swprintf(line, L"Lasketut pisteet: %d, Sis. sakon %d,  Aika: %8.8s",
						kilp->pisteet[0], kilp->pisteet[1]-kilp->pisteet[0], aikatowstr_ls(as, tls, 0));
				else
					swprintf(line, L"Calculated points:%d, Incl. plty %d,  Time: %8.8s",
						kilp->pisteet[0], kilp->pisteet[1]-kilp->pisteet[0], aikatowstr_ls(as, tls, 0));
				if (entpisteet > -1000000 && entpisteet != kilp->pisteet[0]) {
					putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
					endline(tulprm, 0);
					if (tulprm->language == 0)
						swprintf(line, L"Tallennetut pisteet: %d", entpisteet);
					else
						swprintf(line, L"Stored points: %d", entpisteet);
					}
				}
			putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
			endline(tulprm, 0);
			}
		wcscpy(line, L"Tarkastuksen tulos : ");
		if (k > 0)
			wcscat(line, L"Virheit‰");
		else
			wcscat(line, L"Ei virheit‰");
		if (partiorap == 2)
			wcscat(line, L"  Tallennus: ");
		else
			wcscat(line, L"  Kilpailijatietoihin : ");
		switch (kilp->Tark(os, 1)) {
			case 'T':
				if (tulprm->language > 0)
					wcscat(line, L"Accepted");
				else
					wcscat(line, L"Hyv‰ksytty");
				break;
			case 'I':
				if (tulprm->language > 0)
					wcscat(line, L"DQ proposed");
				else
					wcscat(line, L"Hylk‰ysesitys");
				break;
			case 'H':
				if (tulprm->language > 0)
					wcscat(line, L"DQ");
				else
					wcscat(line, L"Hyl‰tty");
				break;
			case 'K':
				if (tulprm->language > 0)
					wcscat(line, L"DNF");
				else
					wcscat(line, L"Keskeytt‰nyt");
				break;
			default:
				if (tulprm->language > 0) {
					if (partiorap == 2)
						wcscat(line, L"Open");
					else
						wcscat(line, L"Control unknown ");
					}
				else {
					if (partiorap == 2)
						wcscat(line, L"Avoinna");
					else
						wcscat(line, L"Tark. avoinna");
					}
				break;
			}
		if (partiorap == 2) {
			vidspwmsg(r+3,0,7,0,line);
			}
		else {
			putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
			endline(tulprm, 1);
			}
		ii = 0;
		ll = rt->rastiluku < 2 * (ySize-9) ? 3 : 2;
		for (i = -1; i < rt->rastiluku; i++) {
			if (i == -1 || (rt->rastikoodi[i] > 9999 && rt->rastikoodi[i] <= 10002)) {
				if (i == rt->rastiluku-1 || rt->rastikoodi[i+1] > 9999)
					continue;
				j = seur_laji(rt, i) - 10000;
				if (tulprm->language > 0) {
					switch (j) {
						case 0 :
							if (partiorap == 2)
								wcscpy(line, L"Obligatory order");
							else
								wcscpy(line, L"Common to all, obligatory order");
							break;
						case 1 :
							if (partiorap == 2)
								wcscpy(line, L"Common, free order");
							else
								wcscpy(line, L"Common to all, free order");
							break;
						case 2 :
							wcscpy(line, L"Selectable");
							break;
						}
					}
				else {
					switch (j) {
						case 0 :
							if (partiorap == 2)
								wcscpy(line, L"Yhteisi‰ pak. j‰rj.");
							else
								wcscpy(line, L"Yhteisi‰ rasteja pakollisessa j‰rjestyksess‰");
							break;
						case 1 :
							if (partiorap == 2)
								wcscpy(line, L"Yhteisi‰ vap. j‰rj.");
							else
								wcscpy(line, L"Yhteisi‰ rasteja vapaassa j‰rjestyksess‰");
							break;
						case 2 :
							wcscpy(line, L"Valinnaisia rasteja");
							break;
						}
					}
				}
			else if (!kilpparam.rogaining) {
				if (partiorap == 2) {
					if (tulprm->language > 0)
						swprintf(line, L"%2d %3d Ctrl", ii+1, rt->rastikoodi[i]);
					else
						swprintf(line, L"%2d %3d Leim", ii+1, rt->rastikoodi[i]);
					}
				else {
					if (tulprm->language > 0)
						swprintf(line, L"    %2d  %3d  Ordinals of stamps :", ii+1, rt->rastikoodi[i]);
					else
						swprintf(line, L"    %2d  %3d  Leimausten j‰rjestysnumerot :", ii+1, rt->rastikoodi[i]);
					}
				for (j = 0; j <= os; j++) {
					if (partiorap == 2 && ll == 2)
						wcscat(line, L"  ");
					else
						wcscat(line, L"   ");
					if (p_tulkinta[ii][j])
						putintrightw(p_tulkinta[ii][j], line+wcslen(line)-1);
					}
				ii++;
				}
			else {
				ii++;
				for (j = 0; j <= os; j++)
					if (p_tulkinta[ii-1][j])
						break;
				if (p_tulkinta[ii-1][j]) {
					if (partiorap == 2)
						swprintf(line, L"%2d %3d %3d", ii, rt->rastikoodi[i], rogpisteet(rt, i));
					else {
						if (tulprm->language > 0)
							swprintf(line, L"    %2d  %3d  %3d Ordinals of stamps :", ii+1, rt->rastikoodi[i], rogpisteet(rt,i));
						else
							swprintf(line, L"    %2d  %3d  %3d Leimausten j‰rjestysnumerot :", ii, rt->rastikoodi[i], rogpisteet(rt,i));
						}
					for (j = 0; j <= os; j++) {
						if (partiorap == 2 && ll == 2)
							wcscat(line, L"  ");
						else
							wcscat(line, L"   ");
						if (p_tulkinta[ii-1][j])
							putintrightw(p_tulkinta[ii-1][j]-1, line+wcslen(line)-1);
						}
					}
				else
					continue;
				}
			if (partiorap == 2) {
				if (r < 3*(ySize-9))
					vidspwmsg(5+r%(ySize-9), (8*ll+4)*(r/(ySize-9)), 7, 0, line);
				r++;
				}
			else {
				putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
				endline(tulprm, 0);
				}
			}
		if (partiorap == 2) {
			clrln(ySize-3);
			vidspmsg(ySize-3,0,7,0,"Paina jotain n‰pp‰int‰ jatkaaksesi");
			readkbd(&ch, 1, spoolfl);
			clrln(3);
			}
		else
			endpage(tulprm);
		}
	if (kh == kilp->Tark(os, 1)) {
		k = -1;
		}
	return(k);
	}
#endif

INT tark_badge(emittp *em, kilptietue *pkilp, bool tallfl)
   {
   kilptietue kilp;
   INT d, os, osuus, er, muut = 0;
   INT osfl[MAXOSUUSLUKU];

   memset(osfl, 0, sizeof(osfl));
   if (!pkilp) {
		if ((em->kilpno = bdg2kno(em->badge, 0)) == 0)
			return(-1);
		if ((d = getpos(em->kilpno)) < 1)
			return(-1);
		GETREC(&kilp, d);
		pkilp = &kilp;
		}
   osuus = -1;
   for (os = Sarjat[pkilp->sarja].osuusluku-1; os >= 0; os--) {
	  if (pkilp->ostiet[os].badge[0] == em->badge &&
		 (esitark || EmitJarr(pkilp->kilpno, os) < 0)) {
		 if (esitark && osuus >= 0 &&
			(pkilp->Maali(os, 0) != TMAALI0 ||
			stschind(pkilp->Tark(os, 1), "TIHKE") >= 0))
			break;
		 osuus = os;
		 osfl[os] = 1;
		 em->osuus = os;
		 if (val_korkein)
			break;
		 }
	  }
   osuus = em->osuus;
   if (em->osuus < 0)
		return(-1);
   if (esitark < 0)
		return(0);
   for (;;) {
	  er = tarkista(em, NULL, NULL, -1, 0);
	  if (er < 0) return(-1);
		 {
		 if (!er) {
			if (!kilpparam.partio && pkilp->Tark(em->osuus, 0) == '-') {
			   pkilp->SetTark(em->osuus, 'T');
			   muut = 1;
			   }
			attnbeep();
			osuus = em->osuus;
			break;
			}
		 else {
			for (os = em->osuus+1; os < Sarjat[pkilp->sarja].osuusluku; os++) {
			   if (osfl[os]) break;
			   }
			if (os < Sarjat[pkilp->sarja].osuusluku) {
			   em->osuus = os;
			   }
			else {
			   em->osuus = osuus;
			   break;
               }
            }
         }
      }
   if (!kilpparam.partio && er > 0) {
	  if (stschind(pkilp->Tark(em->osuus, 0), "-T") >= 0) {
		 pkilp->SetTark(em->osuus, 'I');
		 muut = 2;
         }
      if (!esitark) {
         erbeep();
		 Sleep(100);
		 erbeep();
		 Sleep(100);
         erbeep();
         }
      }
   if (muut && tallfl && !esitark)
	  tallenna(pkilp, d, 1, 0, 0, 0);
   return(muut);
   }

INT maalirasti(char *tunnus, INT koodi)
   {
   ratatp *rt;

   if ((rt = getrata(tunnus)) == NULL)
      return(0);
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
   INT k, tn, pt;
   char msg[80], ch;
   wchar_t Buf[200];
   ekditp tmpkdi[1000];

   if (!emitfl || esitark < 0) return(!emitfl);
   if (!ohitaleimat) {
		leima_file = new TextFl(leimafname,L"r");
		if (!leima_file->IsOpen()) {
			delete leima_file;
			leima_file = NULL;
			 vidspmsg(r,5,7,0,"Tiedoston Radat.xml tai LEIMAT.LST avaaminen ei onnistu");
			}
		}
   if (!leima_file || !leima_file->IsOpen()) {
	  vidspmsg(r+1,5,7,0,
		 "EMIT-koodit asetataan samoiksi kuin rastitunnukset");
	  if (!ohitaleimat) {
		 ch = ' ';
		 selectopt("J)atka ilman tiedostoa LEIMAT.LST, P)oistu ohjelmasta", "JP", &ch);
		 if (ch == 'P')
			return(1);
		 }
	  kuvio = (ekditp *) calloc(255, sizeof(ekditp));
	  if (!kuvio) {
		 writeerror("Ei muistitilaa EMIT-koodeille", 0);
			return(1);
		 }
	  else {
		 for (k = 0; k < 255; k++) {
			kuvio[k].rasti = k+1;
			kuvio[k].koodi = (char) kuvio[k].rasti;
			}
		 nkuvio = 255;
		 maxnkuvio = 255;
		 }
	  return(0);
	  }
   while (!leima_file->Feof()) {
	  if (leima_file->ReadLine(Buf, 200) == NULL)
		  break;
	   if (kilpparam.rogaining) {
		  if (swscanf(Buf, L"%d %d %d\n", &tn, &k, &pt) < 3)
			  break;
		  }
	  else {
		  if (swscanf(Buf, L"%d %d\n", &tn, &k) < 2)
			  break;
		  }
	  if (tn > MAXTUNNUS) {
		 sprintf(msg,"Rastitunnus %d tiedostossa LEIMAT.LST on liian suuri",
			tn);
		 writeerror(msg, 0);
		 continue;
		 }
	  tmpkdi[nkuvio].rasti = tn;
	  tmpkdi[nkuvio].koodi = (char) k;
	  if (kilpparam.rogaining)
		  tmpkdi[nkuvio].pisteet = pt;
	  nkuvio++;
	  }
   delete leima_file;
   qsortint(tmpkdi, nkuvio, sizeof(ekditp), 0,  sizeof(tmpkdi[0].rasti)==4);
   kuvio = (ekditp *) calloc(nkuvio+20, sizeof(ekditp));
   if (!kuvio) {
	  writeerror("Ei muistitilaa EMIT-koodeille", 0);
	  return(1);
	  }
   else {
	  memcpy(kuvio, tmpkdi, nkuvio * sizeof(ekditp));
	  maxnkuvio = nkuvio + 20;
	  }
   return(0);
   }

#ifndef _CONSOLE
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

int ratavatp::lisaa(int kilpno, int sarja, int osuus, int nva, int aika)
{
	int i, muutt = 0;

	for (i = 0; i < nva; i++) {
		if (pisteva[i].kilpno == kilpno && pisteva[i].osuus == osuus) {
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
	pisteva[i].osuus = osuus;
	return(muutt);
}

int ratavatp::poista(int kilpno, int osuus, int nva)
{
	for (int i = 0; i < nva; i++) {
		if (pisteva[i].kilpno == kilpno && pisteva[i].osuus == osuus) {
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

void rastivatp::poista(int kilpno, int osuus)
{
	int n = 0;
	for (int r = 0; r < rata->rastiluku+1; r++) {
		n += ratava[r].poista(kilpno, osuus, nva);
		}
	if (n < 0)
		nva--;
}

int haervali(ratatp *rt, int ir)
   {

   int a, y, k, rvkdi;

   if (ir > rt->rastiluku - maalirastipois)
	   return(-1);
   rvkdi = 0;
   if (ir) rvkdi = 1000*rt->rastikoodi[ir-1];
   if (ir < rt->rastiluku - maalirastipois)
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

/*
void analradat(void)
   {
   int r, ir, *rvint, *p, rkdi;

   if (!emitanal_fl)
	   return;
   for (r = 0; r < nrata; r++)
	  rvluku +=  rata[r].rastiluku+1;
   rvint = (int *) calloc(rvluku, sizeof(int));
   rvluku = 0;
   for (r =  0; r < nrata; r++) {
	  for (ir = 0; ir < rata[r].rastiluku + 1; ir++) {
		 if (ir == rata[r].rastiluku && maalirastipois)
			break;
		 rkdi = 0;
		 if (ir) rkdi = 1000*rata[r].rastikoodi[ir-1];
		 if (ir < rata[r].rastiluku) rkdi += rata[r].rastikoodi[ir];
		 for (p = rvint; *p; p++)
			if (*p == rkdi) break;
		 if (*p == 0) {
			rvluku++;
			*p = rkdi;
			}
		 }
	  }
	qsortint(rvint, rvluku, 4, 0, 1);
   rvalit = (rvalitp *) calloc(rvluku, sizeof(rvalitp));
   for (r = 0; r < rvluku; r++) {
		rvalit[r].kdi = rvint[r][0];
		rvalit[r].matka = rvint[r][1];
		}
   free(rvint);
   }
*/

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
		maalirastipois = ((rt->maalilaji > 1) ? 1 : 0);
		nva = rt->rastiluku  + 1 - maalirastipois;
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

void laskevertaika(int rv, int srj, int os)
   {
   int i, nvert, tvert = 0;

   if (!emitanal_fl)
	   return;
   if (rvalit[rv].lkm[srj][os] > 0) {
	  nvert = (rvalit[rv].lkm[srj][os] * analvertosuus) / 100;
	  if (nvert <= 0)
		 nvert = 1;
	  if (nvert > analvertmax)
         nvert = analvertmax;
      for (i = 0; i < nvert; i++)
         tvert += rvalit[rv].ajat[srj][os][i];
      tvert /= nvert;
      }
   rvalit[rv].vertaika[srj][os] = tvert;
   }

void laskevertajat(void)
   {
	int rv, srj, os;

   if (!emitanal_fl)
	   return;
   for (rv = 0; rv < rvluku; rv++) {
	  for (srj = 0; srj < sarjaluku; srj++) {
		 for (os = 0; os < Sarjat[srj].ntosuus; os++)
			laskevertaika(rv, srj, os);
		 }
	  }
   }

int laskeemitvaliajat(emittp *em, kilptietue *kilp, emitvaanaltp **eva, rastivaanaltp **rstva, int *valuku)
   {
	int ir, j, k, tulkinta[MAXEMITVA+1], alku, loppu, ensrasti = -1;
   INT32 enn;
   ratatp *rt;
   static emitvaanaltp *ev;
   static rastivaanaltp *rst;

   if (!emitanal_fl)
	   return(1);
	enn = haeennakko(em, kilp, em->osuus);
   if ((rt = getrata(kilp->ostiet[em->osuus].hajonta)) == 0)
	  return(1);
	if (tarkista(em, kilp, tulkinta, -1, 0) < 0)
		return(1);
	ir = 0;
	for (j = 0; j < MAXEMITVA+1; j++)
	  if (tulkinta[j])
			ir++;
   if (ir < 3)
	  return(1);
//   if (tarkista(em, kilp, tulkinta, -1, 0))
//      return(1);
	for (k = 1; k < 4; k++) {
		for (j = 0; j <= MAXEMITVA; j++) {
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
	j = 0;
	alku = 0;
	*valuku = rt->rastiluku  + 1 - maalirastipois;
	if (*valuku > MAXEMITVA) {
	   *valuku = MAXEMITVA;
	}
   if (rstva) {
	   rst = (rastivaanaltp *)calloc(*valuku, sizeof(rastivaanaltp));
	   *rstva = rst;
	   }
	ev = (emitvaanaltp *)calloc(*valuku, sizeof(emitvaanaltp));
	*eva = ev;
	for (ir = 0; ir < *valuku; ir++) {
		while (j < MAXEMITVA &&
			tulkinta[(ensrasti+j)%MAXEMITVA] != ir+1 &&
			-tulkinta[(ensrasti+j)%MAXEMITVA] != ir+1)
			j++;
		if (j == MAXEMITVA) break;
		if (ir >= *valuku - 2) {
			loppu = osuustulos(kilp, em->osuus, 0)/SEK;
			if (ir == *valuku - 2) {
				if (em->ctrltime[(ensrasti+j)%MAXEMITVA]-enn < loppu - 10)
					loppu = em->ctrltime[(ensrasti+j)%MAXEMITVA]-enn;
				else
					--*valuku;
				}
			}
		else
			loppu = em->ctrltime[(ensrasti+j)%MAXEMITVA]-enn;
		ev[ir].rkoodi = rt->rastikoodi[ir];
		if (loppu >= alku && loppu - alku < analmaxosuuskesto) {
			ev[ir].aika = loppu;
			ev[ir].rvaika = loppu-alku;
			}
		else {
			free(ev);
			*eva = 0;
			return(1);
			}
		ev[ir].rvno = haervali(rt, ir);
		alku = loppu;
		if (rstva) {
		   int sj = 0, kaika = 0;
			rst[ir].aika = ev[ir].aika;
			for (int i = 0; i < rastiva[rt-rata].nva; i++) {
				if (rastiva[rt-rata].ratava[ir].pisteva[i].sarja == kilp->sarja &&
					rastiva[rt-rata].ratava[ir].pisteva[i].osuus == em->osuus) {
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
		}
	return(0);
	}

void addemitanal(emittp *em, int uusi)
	{
   kilptietue kilp;
   int d, ir, j, tulkinta[MAXEMITVA+1], rkdi, nva, err, rr, lis = 0, yos;
   ratatp *rt;
   emitvaanaltp *ev = 0;

   if (!em || !em->kilpno || !emitanal_fl)
	  return;
   if ((d = getpos(em->kilpno)) <= 0)
	  return;
   GETREC(&kilp, d);
   if ((rt = getrata(kilp.ostiet[em->osuus].hajonta)) == 0)
	  return;
   rr = rt - rata;
   err = tarkista(em, &kilp, tulkinta, -1, 0);
   if (err)
	  return;
   yos = Sarjat[kilp.sarja].yosuus[em->osuus];
	EnterCriticalSection(&rastiva_CriticalSection);
   err = laskeemitvaliajat(em, &kilp, &ev, 0, &nva);
   if (!err) {
		j = 0;
	  for (ir = 0; ir < nva; ir++) {
		if (rastiva[rr].nva < rastiva[rr].maxnva) {
			lis += rastiva[rr].ratava[ir].lisaa(kilp.kilpno, kilp.sarja, em->osuus, rastiva[rr].nva, ev[ir].aika);
			}
		 rkdi = 0;
		 if (ir) rkdi = 1000*rt->rastikoodi[ir-1];
		 if (ir < nva-1) rkdi += rt->rastikoodi[ir];
		 d = ev[ir].rvno;
		 if (d >= 0) {
			rvalit[d].sum[kilp.sarja][em->osuus] += ev[ir].rvaika;
			addIntInArr(ev[ir].rvaika, em->kilpno,
			   rvalit[d].ajat[kilp.sarja][yos],
			   rvalit[d].kilpno[kilp.sarja][yos],
			   rvalit[d].lkm[kilp.sarja][yos],
			   rvalit[d].maxlkm[kilp.sarja][yos]);
			rvalit[d].lkm[kilp.sarja][yos]++;
			if (uusi)
			   laskevertaika(d, kilp.sarja, yos);
			}
		 }
		if (lis)
			rastiva[rr].nva += (lis > 0) ? 1 : -1;
	  }
   if (ev)
	  free(ev);
	LeaveCriticalSection(&rastiva_CriticalSection);
   }

void tarkkilpradat(void)
   {
   int d, os, ir, rv, srj;
   kilptietue kilp;
   char msg[82];
   ratatp *rt;

   if (emitanal_fl) {
	  analradat();
	  for (d = 0; d < rvluku; d++) {
		 for (srj = 0; srj < sarjaluku; srj++) {
			for (os = 0; os < Sarjat[srj].ntosuus; os++) {
				rvalit[d].maxlkm[srj][os] = Sarjat[srj].nosuus[os]*(nilm[srj] + nilm[srj]/10) + 5;

			   if (rvalit[d].maxlkm[srj][os]++) {
				  rvalit[d].ajat[srj][os] =
					 (int *)calloc(rvalit[d].maxlkm[srj][os]+1, sizeof(int));
				  rvalit[d].kilpno[srj][os] =
					 (int *)calloc(rvalit[d].maxlkm[srj][os]+1, sizeof(int));
				  }
			   }
			}
		 }
	  }
}

static int TarkKilpailijat(void)
{
	kilptietue kilp;
	wchar_t msg[100], st[40];
	ratatp *rt;
//   int (*rtsrjos)[MAXSARJALUKU][MAXOSUUSLUKU];

//   if (emitanal_fl) {
//		rtsrjos = (int (*)[MAXSARJALUKU][MAXOSUUSLUKU])calloc(nrata, sizeof(int)*MAXSARJALUKU*MAXOSUUSLUKU);
//		}
   for (int d = 1; d < datf2.numrec; d++) {
	  GETREC(&kilp, d);
	  if (kilp.kilpstatus)
		  continue;
	  for (int os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
		 if (kilp.ostiet[os].hajonta[0]) {
			if ((rt = getrata(kilp.ostiet[os].hajonta)) == 0) {
			   swprintf(msg, L"Joukkueen %d osuudella %s tuntematon rata %s",
				  kilp.kilpno, wosuuskoodi(kilp.sarja, os, 0, 0), kilp.Hajonta(os, st));
			   kirj_err_file_w(msg, 1);
			   }
			else {
				if (emitanal_fl && rastiva && rt == rastiva[rt-rata].rata) {
					rastiva[rt-rata].nva++;
					}
				}
//			 if (emitanal_fl
//			   rtsrjos[rt-rata][kilp.sarja][os]++;
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

//   if (emitanal_fl)
//	  free(rtsrjos);
	return(0);
   }

#else // _CONSOLE

void tarkkilpradat(void)
   {
   UINT32 d;
   int os;
   kilptietue kilp;
   char msg[82];

   for (d = 1; d < filelen(&datf2); d++) {
	  GETREC(&kilp, d);
	  if (kilp.kilpstatus) continue;
	  for (os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
		 if (kilp.ostiet[os].hajonta[0]) {
			if (!getrata(kilp.ostiet[os].hajonta)) {
			   sprintf(msg, "Joukkueen %d osuudella %d tuntematon rata %s",
				  kilp.kilpno, os+1, kilp.ostiet[os].hajonta);
			   kirj_err_file(msg, 1);
			   }
			}
		 else {
			sprintf(msg, "Joukkueen %d osuudella %d ei rataa",
			   kilp.kilpno, os+1);
			kirj_err_file(msg, 1);
			}
		 }
	  }
   }

static int TarkKilpailijat(void)
{
	return(0);
}

#endif // _CONSOLE

static void lue_rogsakot(void)
	{
   FILE *sak_file;
   char sakfname[] = "AIKARAJAT.TXT";
   int srj, j, raja, askel, sakko, asrj, ysrj;
   char line[300], *p;

   if ((sak_file = fopen(sakfname,"rt")) == NULL) {
	  writeerror("Tiedoston AIKARAJAT.TXT avaaminen ei onnistu", 0);
      return;
      }
   for (srj = 0; !feof(sak_file); srj++) {
      if (fgets(line, 298, sak_file) == NULL) break;
	  if (strlen(line) < 5)
		  continue;
	  upcasestr(line);
	  if ((p = strtok(line, " ;\t")) != NULL) {
		  if (!strcmpU(p, "KAIKKI")) {
			  asrj = 0;
			  ysrj = sarjaluku-1;
			  }
		  else {
			  ysrj = haesarja(p, false);
			  asrj = ysrj;
			  }
		  if (ysrj >= 0) {
			  if ((p = strtok(NULL, " ;\t")) != NULL)
				  raja = MINUUTTI * atoi(p);
			  if (p && (p = strtok(NULL, " ;\t")) != NULL)
				  askel = SEK * atoi(p);
			  if (p && (p = strtok(NULL, " ;\t")) != NULL)
				  sakko = atoi(p);
			  if (p && raja > 0 && askel > 0 && sakko > 0) {
				  for (j = asrj; j <= ysrj; j++) {
					  maxaika[j] = raja;
					  sakkoaskel[j] = askel;
					  askelsakko[j] = sakko;
					  }
				  }
			  }
		  }
	   }
	fclose(sak_file);
	}

INT lue_radat(INT r)
   {
   TextFl *rata_file;
   INT i, j, ir, tn, tn1, kdi[MAXNRASTI], rno, nr = 0, vapaajarj = 0, ensilm = 1;
   wchar_t line[300], *p;
   char rnimi[40], msg[80];

   if (!emitfl || esitark < 0)
		return(!emitfl);
   nrata = 0;
   rno = 0;
   tn = 0;
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

	nrata = 0;
	for (i = 0; ; i++) {
		if (rata_file->ReadLine(line, 298) == NULL || wcslen(line) < 4)
			break;
		if (i % 3 == 0)
			nrata++;
		}

   rata = (ratatp *) calloc(nrata+1, sizeof(ratatp));

   if (!rata) {
	  writeerror_w(L"Ei tilaa ratatiedoille", 0);
	  goto esc1;
	  }
   rno = 0;
   nrata = 0;
   rata_file->Rewind();
   for (;;) {
	  if (rata_file->ReadLine(line, 298) == NULL || wcslen(line) < 4)
		  break;
	  p = wcstok(line, L" \t");
	  wcstoansi(rnimi, p, 39);
	  rnimi[39] = 0;
	  elimbl2(rnimi);
	  upcasestr(rnimi);
	  if (strlen(rnimi) >= sizeof(rata[i].rata)) {
		  sprintf(msg, "Ratanimi %s liian pitk‰", rnimi);
		  kirj_err_file(msg, 1);
		  }

		rnimi[sizeof(rata[i].rata)-1] = 0;
		for (i = 0; i < rno; i++)
			if (!strcmpU(rata[i].rata, rnimi))
				break;
		if (i < rno) {
			sprintf(msg, "Rata %s toistuu ratatiedoissa", rnimi);
			kirj_err_file(msg, 1);
			}
		nrata = rno++;
		strcpy(rata[nrata].rata, rnimi);

	  p = wcstok(NULL, L" \t");
	  if (p) {
		 rata[nrata].ennakko = _wtoi(p);
		 p = wcstok(NULL, L" \t");
		 if (p) rata[nrata].maalilaji = _wtoi(p);
		 }
	  ir = 0;


	  for (i = 0; i < 2; i++) {
		 if (rata_file->ReadLine(line, 298) == NULL) goto esc;
		 if (emitfl < 0)
			continue;
		 p = wcstok(line, L" \t");
		 for (j = 0; j < 40; j++) {
			tn = 0;
			if (p) {
			   tn = _wtoi(p);
			   if (tn == 0) {
				  *p = towupper(*p);
				  tn1 = wcswcind(*p, L"JPV");
				  if (tn1 >= 0) {
					 if (vapaajarj >= 0) {
						tn = vapaajarj + 10000;
						}
					 vapaajarj = tn1;
					 }
				  }
			   }
			if (tn == 0 && i && vapaajarj > 0) {
			   tn = vapaajarj + 10000;
			   }
			if (tn == 0)
			   break;
			if (tn <= MAXTUNNUS || tn >= 10000) {
			   if (tn <= MAXTUNNUS && emitfl > 0 && !on_kuvio(tn)) {
				  sprintf(msg, "Radan %s rastin %d tunnusta %d ei tiedostossa"
					 " LEIMAT.LST", rata[nrata].rata, ir+1, tn);
				  kirj_err_file(msg, 1);
				  }
			   if (ir >= MAXNRASTI) {
				  sprintf(msg, "Radalla %s liikaa rasteja (yli %d)",
					  rata[nrata].rata, MAXNRASTI);
				  kirj_err_file(msg, 1);
				  }
			   kdi[ir] = tn;
			   ir++;
			   }
			p = wcstok(NULL, L" \t");
			}
		 }
	  rata[nrata].rastiluku = ir;
	  if (ir) {
//		 rata[nrata].rastikoodi = (int *) calloc(ir+1, sizeof(INT));
//		 if (!rata[nrata].rastikoodi) {
//			writeerror_w(L"Ei tilaa ratatiedoille", 0);
//			goto esc1;
//			}
		 for (j = 0; j < ir; j++)
			rata[nrata].rastikoodi[j] = kdi[j];
		 }
	  nrata++;
	  }
   if (emitfl < 0)
	  sprintf(msg, "Tiedostosta RADAT.LST luettu ajanottotavat");
   else
	  sprintf(msg, "Tiedostosta RADAT.LST luettu %d rataa", nrata);
   vidspmsg(r,5,7,0,msg);
   delete rata_file;
   if (emitfl > 0) {
	  tarkkilpradat();
	  TarkKilpailijat();
	  }
   if (kilpparam.rogaining)
	   lue_rogsakot();
   return(0);
   esc1:
//   for (j = 0; j < nrata; j++)
//	  if (rata[j].koodi) free(rata[j].koodi);
   free(rata);
   emitfl = 0;
   esc:
   sprintf(msg,"Virhe tiedostoa RADAT.LST luettaessa. Rata %s",
	  rata[nrata].rata);
   writeerror(msg, 0);
   delete rata_file;
   return(1);
   }

INT lueemitfile(void)
   {
   emittp em;
   INT kysy = 0;
   wchar_t ch, msg[81];

   if (!emitfl) return(1);
   if (esitark) {
	  wcscpy(emitfname, L"ESITARK.DAT");
	  }
   if ((emitfile = _wopen(emitfname, O_RDWR | O_CREAT | O_BINARY,
	  S_IREAD | S_IWRITE)) < 0) {
	  swprintf(msg, L"Tiedoston %s avaaminen tai luominen ei onnistu", emitfname);
	  writeerror_w(msg, 0);
      return(1);
	  }
   femitfile = _fdopen(emitfile, "r+b");
   setvbuf(femitfile, NULL, _IONBF, 0);
   emithead = (INT) (_filelength(emitfile) / sizeof(emittp));
   emithead2 = emithead;
   if (emithead) {
      if (ntulost[0]+nhylt[0]+ntulost[1]+nhylt[1]+ntulost[2]+nhylt[2]
         == 0) {
         ch = ' ';
		 swprintf(msg, L"Ei tuloksia, mutta %d Emit-tietuetta. J)atka, "
			L"T)uhoa Emit-tiedot, P)oistu", emithead);
		 wselectopt(msg,L"JTP", &ch);
         }
      else
		 ch = L'J';
	  switch (ch) {
		 case L'P':
			return(1);
		 case L'T':
			_chsize(emitfile, 0);
			break;
		 case L'J':
			_lseek(emitfile, 0, SEEK_SET);
			emithead2 = 0;
			for (emitptr = 0; emitptr < emithead; emitptr++) {
			   if (_read(emitfile, &em, sizeof(emittp)) < sizeof(emittp)) {
				  swprintf(msg, L"Tiedoston %s lukeminen ei onnistu", emitfname);
				  writeerror_w(msg, 0);
				  return(1);
				  }
			   if (em.kilpno && em.badge) {
				  if (em.kilpno == bdg2kno(em.badge, 0) &&
					  em.osuus < kilpparam.n_os_akt) {
#ifndef _CONSOLE
					 if (emitanal_fl && EmitJarr(em.kilpno, em.osuus) < 0)
						addemitanal(&em, 0);
#endif
					 setEmitJarr(em.kilpno, em.osuus, emitptr);
					 if (estaemittoisto)
						on_emit(&em, 0);
					 }
				  else {
					 kysy++;
					 swprintf(msg, L"Emit-koodia %ld vastaa eri kilpailijat KILP.DATissa %d ja %sissa %d",
						em.badge, bdg2kno(em.badge, 0), emitfname, em.kilpno);
					 kirj_err_file_w(msg, 1);
					 }
				  }
			   if (em.kirjaus != TMAALI0/KSEK)
				  emithead2 = emitptr + 1;
			   }
		 }
	  }
   emitptr = emithead-1;
   if (kysy) {
	  swprintf(msg,
		 L"%s ja KILP.DAT eiv‰t yhteensopivia (%d eroa)", emitfname, kysy);
	  writeerror_w(msg, 0);
	  }
#ifndef _CONSOLE
   if (emitanal_fl)
	  laskevertajat();
#endif
   return(0);
   }

INT32 haeennakko(emittp *em, kilptietue *kilp, INT osuus)
   {
//   INT i;
   int lkja, enn = 0;

   // Jos sarjalla ei ennakkoa (eli ennakko 0) tai ennakko kiinte‰ ( > 0 )
   // saa ennakko t‰m‰n arvon, muuten ennakko lasketaan lukuhetkest‰ ja
   // viimeisest‰ lukijalaitteen antamasta emit-v‰liajasta.

   if ((lkja = haelukija(em)) < 0)
	   return(0);
   enn = osuuslahto(kilp, osuus)/KSEK;
   if (enn != TMAALI0/KSEK) {
	  enn = (24*36000L + 9 + enn - em->time)/10 - 24*3600L + em->ctrltime[lkja];
	  enn = (enn + 51L*3600L) % (24L*3600L) - 3*3600L;
      if (enn > 12L*3600L) enn = 0;
      }
   else {
      enn = 0;
      }
   return(enn);
   }
#ifdef _CONSOLE
INT naytatiedot(emittp *em, kilptietue *kilp, INT os, int loppuosa)
   {
   INT32 viim, enn = 0;
   char line[120], st[80];
   wchar_t wline[120], wst[80];
   INT fg = 7, i, tulkinta[51], code99 = 0, lisa = 0, nr1 = 17, rm;

#if defined(ESITARK)
   int r;
   if (esitark && esitark != 2) {
	  for (i = 4; i < ySize-4; i++)
         clrtxt(i, 0, 58);
      if (!kilp || kilp->kilpstatus)
         return(0);
      swprintf(wline, L"%4d %s -%d", kilp->kilpno, kilp->Seura(wst), kilp->joukkue);
      line[55] = 0;
      r = (Sarjat[kilp->sarja].osuusluku < 13) ? 5 : 4;
      vidspwmsg(r, 2, 7, 0, wline);
      sprintf(line, "Koodi %d osuuksilla", kilp->ostiet[os].badge[0]);
      for (i = 0; i < Sarjat[kilp->sarja].osuusluku; i++) {
         if (kilp->ostiet[i].badge[0] == kilp->ostiet[os].badge[0]) {
            sprintf(line+strlen(line), " %d", i+1); 
            }
         }
      line[55] = 0;
      r = r + 1 + r/5;
      vidspmsg(r, 2, 7, 0, line);
      r = r + 1 + r/7;
      for (i = 0; i < Sarjat[kilp->sarja].osuusluku; i++) {
         swprintf(wline, L"%2d. os.%7d %s", i+1, kilp->ostiet[i].badge[0],
            kilp->Nimi(wst, OSNIMIL, i));
         wline[55] = 0;
         if (i == os)
            vidspwmsg(r+i, 2, 0, 7, wline);
         else {
            if (kilp->ostiet[i].badge[0] == kilp->ostiet[os].badge[0]) {
               viwrrect(r+i, 2, r+i, 8, line, 0, 7, 0);
               vidspmsg(r+i, 9, 7, 0, line+7);
               }
			else
               vidspmsg(r+i, 2, 7, 0, line);
            }
         }
      return(0);
      }
#endif
   nr1 = min(25, (ySize-8));
//   for (i = 4; i < ySize-4; i++)
   rm = ySize == 50 ? 41 : ySize-4;
   for (i = 4; i < rm; i++)
	  clrtxt(i, 0, 39);
   clrtxt(3, 52, 64);
	draw_vline(4, 18, nr1);
	draw_vline(4, 38, nr1);
   if (kilp) {
	  getem(em, kilp->kilpno, os);
      if (em) {
		 if ((viim = e_maaliaika(em, kilp, os)) != TMAALI0 &&
            kilp->Maali(os, 0) != TMAALI0) {
			viim = ((kilp->Maali(os, 0) - viim)/SEK + 36*3600L)%86400L - 43200L;
            sprintf(line, "V.v‰li%6d", viim);
            vidspmsg(3, 52, 7, 0, line);
            }
         }
	  if (tarkista(em, kilp, tulkinta, -1, 0))
		 fg = 0;
      enn = haeennakko(em, kilp, os);
      }
   else
      memset(tulkinta, 0, sizeof(tulkinta));
//   if (enn) {
//      sprintf(line, "Ennakko %5ld sek", enn);
//      vidspmsg(3, 33, 7, 0, line);
//      }
   if (loppuosa) {
      for (i = 49; i > 2*nr1-1; i--)
         if (em->ctrlcode[i] != 0)
            break;
      lisa = i-2*nr1+1;
	  }
   for (i = lisa; i < 2*nr1+lisa; i++) {
      if (i && !em->ctrlcode[i] && !em->ctrltime[i]) break;
      if (tulkinta[i] < 0) sprintf(st, "(%2d)", -tulkinta[i]);
      else sprintf(st, " %2d ", tulkinta[i]);
      sprintf(line, "%2d %3d%5ld %4s", i, em->ctrlcode[i],
         em->ctrltime[i]-enn, st);
      if (em->ctrlcode[i] == 99 || em->ctrlcode[i+1] == 99) {
         fg = 7-fg;
		 code99 = 1;
         }
      vidspmsg((i-lisa) % nr1 + 4, 20*((i-lisa)/nr1), fg, 7-fg, line);
      if (em->ctrlcode[i] == 99 || em->ctrlcode[i+1] == 99)
         fg = 7-fg;
      }
   if (code99) {
      viwrrect(17,42,18,60,"Leimasin sammumassa"
                           "(koodi 99)         ",0,7,0);
      }
   return(0);
   }
#endif // _CONSOLE

void emit_tallenna(kilptietue *kilp, INT os, char trk, char *selitys, INT cn)
   {
   char ch, che;
   INT d;
   kilptietue entkilp;

   if (!cn && (trk == 'H' || trk == 'K') && kilp->Tark(os, 0) != trk
	  && kilp->Tark(os, 0) != 'I') {
	  writeerror("Hylk‰ys vain esityksen j‰lkeen.", 0);
	  return;
	  }
   if (!cn && (stschind(kilp->Tark(os, 0), "HK") >= 0) &&
	  trk != kilp->Tark(os, 0)) {
	  clrln(ySize-3);
	  vidspmsg(ySize-3,20,7,0,"Hylk‰yksen poisto. Vahvista (K/E)");
	  mene(56, ySize-3);
	  do {
		 ch = readkbd(&che,1,spoolfl);
		 ch = toupper(ch);
		 } while (ch != 'K' && ch != 'E');
	  if (ch == 'E') return;
	  }
   entkilp = *kilp;
   kilp->SetTark(os, trk);
   strcpy(kilp->ostiet[os].selitys, selitys);
   if ((d = getpos(kilp->kilpno)) > 0)
	  tallenna(kilp, d, 1, cn, 0, &entkilp);
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
		if (em.badge && em.kilpno == bdg2kno(em.badge, 0)) {
			for (int os = 0; os < kilpparam.osuusluku; os++) {
				if (EmitJarr(em.kilpno, os) == eptr)
					return(eptr);
				}
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
		if (em.badge && em.kilpno && em.kilpno == bdg2kno(em.badge, 0)) {
			for (int os = 0; os < kilpparam.osuusluku; os++) {
				if (EmitJarr(em.kilpno, os) == eptr)
					return(eptr);
				}
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
		if (em.badge && em.kilpno == bdg2kno(em.badge, 0)) {
			for (int os = 0; os < kilpparam.osuusluku; os++) {
				if (EmitJarr(em.kilpno, os) == eptr)
					return(eptr);
				}
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
		if (em.badge && em.kilpno == bdg2kno(em.badge, 0)) {
			for (int os = 0; os < kilpparam.osuusluku; os++) {
				if (EmitJarr(em.kilpno, os) == eptr)
					return(eptr);
				}
			}
		}
   return(-1);
}

void lueseuraava(INT *kilpno, INT *os, emittp *em)
   {
   INT i, palaa = 0;

   em->nollaa();
   if (emithead2 == 0) return;
   *kilpno = 0;
   if (emitptr < emithead2-1) emitptr++;
   else emitptr = emithead2-1;
   do {
	  lueem(em, emitptr);
	  if (em->badge && em->kilpno == bdg2kno(em->badge, 0)) {
		 for (i = 0; i < kilpparam.n_os_akt; i++)
			if (EmitJarr(em->kilpno, i) == emitptr)
			break;
		 if (i < kilpparam.n_os_akt)
			break;
		 }
	  if (emitptr == emithead2-1) palaa = 1;
	  if (!palaa) emitptr++;
	  else {
		 if (emitptr > 0) emitptr--;
		 else break;
		 }
	  } while (emitptr <= emithead2-1);
//   if (palaa) emithead = emitptr+1;
   *kilpno = em->kilpno;
   *os = em->osuus;
   }

void lueedellinen(INT *kilpno, INT *os, emittp *em)
   {
   INT i, palaa = 0;

   em->nollaa();
   if (emithead2 == 0) return;
   *kilpno = 0;
   if (emitptr > 0) emitptr--;
   do {
	  lueem(em, emitptr);
	  if (em->badge && em->kilpno == bdg2kno(em->badge, 0)) {
		 for (i = 0; i < kilpparam.n_os_akt; i++)
			if (EmitJarr(em->kilpno, i) == emitptr)
			break;
		 if (i < kilpparam.n_os_akt)
			break;
		 }
	  if (emitptr == 0) palaa = 1;
	  if (!palaa) emitptr--;
	  else {
		 if (emitptr < emithead2-1) emitptr++;
		 else break;
		 }
	  } while (emitptr >= 0);
   *kilpno = em->kilpno;
   *os = em->osuus;
   }

void lueseuraavaong(INT *kilpno, INT *os, emittp *em)
   {
   INT d, edptr;
   kilptietue kilp;

   for (;;) {
		edptr = emitptr;
	  lueseuraava(kilpno, os, em);
	  if (edptr == emitptr || emitptr >= emithead2-1 || tarkista(em, NULL, 0, -1, 0))
		 break;
	  if (*kilpno && (d = getpos(*kilpno)) > 0) {
		 GETREC(&kilp, d);
		 if (kilp.ostiet[*os].keskhyl != 'T' || kilp.Maali(*os, 0) == TMAALI0)
			break;
		 }
	  }
   }

void poista_emit(emittp *em)
   {
   char ch = ' ';
   INT32 ep;

   selectopt("Vahvista aiempien tietojen poisto (K/E)", "KE", &ch);
   if (ch == 'K' && em->kilpno >= minkilpno && em->kilpno <= maxkilpno) {
	  ep = EmitJarr(em->kilpno, em->osuus);
	  if (ep >= 0 && ep < emithead2) {
		 setEmitJarr(em->kilpno, em->osuus, -1);
		 em->kilpno = 0;
		 em->badge = 0;
		 em->osuus = 0;
		 putem(em, ep, 0);
		 }
	  }
   clrln(ySize-3);
   }

void tallkvaihto(INT ekno, INT eos, INT kno, INT os, INT kielto)
   {
   INT32 ep;
   emittp em;

   if ((ep = getem(&em, ekno, eos)) >= 0 && EmitJarr(kno, os) < 0) {
	  setEmitJarr(ekno, eos, -1);
	  em.osuus = os;
	  em.kilpno = kno;
	  putem(&em, ep, 0);
	  if (kno >= minkilpno)
		  setEmitJarr(kno, os, ep);
	  lahkvaihto(ekno, eos, kno, os, kielto);
	  }
   }

#ifdef _CONSOLE
void vaihdakilp(INT ekno, INT eos, INT vainos)
   {
   INT d, du, os, kno = 0;
   kilptietue kilp, ekilp, entkilp;
   char ch;
   INT32 tm = TMAALI0;
   ratatp *rt;

   if (vainos)
      ch = 'O';
   else {
      ch = ' ';
      selectopt("Vaihda O)suus, kilpailijaN)umero", "ON", &ch);
	  }
   if (ch == 'O') {
	  os = eos;
      os++;
      clrln(ySize-3);
      vidspmsg(ySize-3,0,7,0,"Anna uusi osuus");
      INPUTINT(&os, 2, 16, ySize-3, "\r\x1b", &ch);
      if (ch == 27 || os < 1 || os > kilpparam.n_os_akt)
         return;
      os--;
      if ((du = getpos(ekno)) <= 0) {
         writeerror("Kilpailijaa ei lˆydy", 0);
         return;
         }
      GETREC(&kilp, du);
	  if (EmitJarr(ekno, os) >= 0) {
		 writeerror("Osuudelle jo luettu emit-tiedot, muutos perutaan", 0);
		 return;
         }
      if (os != eos && os < Sarjat[kilp.sarja].osuusluku) {
         ekilp = kilp;
         kilp.ostiet[os].badge[0] = kilp.ostiet[eos].badge[0];
         if (esitark >= 0 && kilp.Maali(os, 0) == TMAALI0 &&
            kilp.Maali(eos, 0) != TMAALI0) {
            rt = getrata(kilp.ostiet[os].hajonta);
            if (rt && (rt->maalilaji == 4 || rt->maalilaji == 10)) {
               kilp.setMaali(os, 0, kilp.Maali(eos, 0));
               kilp.setMaali(eos, 0, TMAALI0);
               }
            }
         tallkvaihto(ekno, eos, ekno, os, 0);
		 tallenna(&kilp, du, 0, -1, 0, &ekilp);
		 }
      }
   else {
      if ((d = getpos(ekno)) <= 0) {
		 return;
         }
      GETREC(&ekilp, d);
      for (os = 0; os < Sarjat[ekilp.sarja].osuusluku; os++) {
         if (os != eos && ekilp.ostiet[os].badge[0] == ekilp.ostiet[eos].badge[0]) {
            writeerror("Sama emitkoodi toisellakin osuudella. Vaihda ensin "
               "koodi", 0);
			return;
            }
         }
      clrln(ySize-3);
	  vidspmsg(ySize-3,0,7,0,
         "Kilpailijanumero, jolle tiedot siirret‰‰n :         Esc : peruuta");
      INPUTINT(&kno,4, 44, ySize-3,"\r\x1B",&ch);
      if (ch == ESC || kno == ekno) return;
      if ((du = getpos(kno)) <= 0) {
         writeerror("Kilpailijaa ei lˆydy", 0);
         return;
         }
      GETREC(&kilp, du);
      os = eos + 1;
      clrln(ySize-3);
      vidspmsg(ySize-3,0,7,0,"Anna uusi osuus");
      INPUTINT(&os, 2, 16, ySize-3, "\r\x1b", &ch);
      if (ch == 27 || os < 1 || os > Sarjat[kilp.sarja].osuusluku)
         return;
      os--;
      if (os < Sarjat[kilp.sarja].osuusluku) {
		 if (EmitJarr(kno, os) >= 0) {
			writeerror("Valitulle kilpailijalle jo luettu emit-tiedot - ei muuteta", 0);
            return;
            }
         entkilp = ekilp;
         ekilp.ostiet[eos].badge[0] = 0;
         if (esitark >= 0 && kilp.Maali(os, 0) == TMAALI0 &&
            ekilp.Maali(eos, 0) != TMAALI0) {
            rt = getrata(kilp.ostiet[os].hajonta);
            if (rt && (rt->maalilaji == 4 || rt->maalilaji == 10)) {
               tm = ekilp.Maali(eos, 0);
               ekilp.setMaali(eos, 0, TMAALI0);
               }
            }
		 tallenna(&ekilp, d, 0, -1, 0, &entkilp);
         ekilp = kilp;
         kilp.ostiet[os].badge[0] = entkilp.ostiet[eos].badge[0];
         if (tm != TMAALI0)
			 kilp.setMaali(os, 0, pyoristatls(tm, 1));
		 tallkvaihto(ekno, eos, kno, os, 0);
		 tallenna(&kilp, du, 0, -1, 0, &ekilp);
		 }
      }
   }

INT ohj_aikaruutu(char *ch2)
   {
   INT i;

   if (autokilp) {
	  switch (*ch2) {
         case 50:
            if (ajat_on) {
               ajat_on = 0;
               status_on = 1;
               }
            else {
               ajat_on = 1;
               status_on = 0;
               }
            break;
         case 22:
            if (seurkrivi > 0) seurkrivi--;
            break;
         case 36:
			if (aikajono[akttm] && seurkrivi - aikajono[akttm]->aktrows < 0) seurkrivi++;
            break;
         }
      for (i = 0; i < 17; i++)
         clrtxt(i, 60, 79);
      lukumaarat();
      }
   *ch2 = 0;
   return(1);
   }

void outleimat(kilptietue *kilp, INT os, emittp *em, INT selaus, int loppuosa)
   {
   char line[65], as[20];
   wchar_t wline[65];
   int i, os1, rm;

   if (kilpparam.partio)
      os1 = Sarjat[kilp->sarja].osuusluku - 1;
   else
      os1 = os;  
   sprintf(line, "Kilpailija %4d-%-2d, kortti %d", kilp->kilpno, os+1,
      kilp->ostiet[os].badge[0]);
   clrtxt(0, 0, 33);
   rm = ySize == 50 ? 41 : ySize-4;
   for (i = 15; i < rm; i++)
      clrtxt(i, 40, 64);
   vidspmsg(0,0,7,0, line);
   if (selaus) vidspmsg(0,50,7,0,"SELAUS");
   else clrtxt(0,50,56);
   vidint(0, 57, 5, emitptr+1);
   wmemset(wline, L' ', 27);
   kilp->Seura(wline);
   wline[25] = 0;
   if (kilp->joukkue >= 10) {
	  wline[24] = L'-';
      wline[25] = kilp->joukkue / 10 + L'0';
      wline[26] = kilp->joukkue % 10 + L'0';
      }
   else {
	  wline[25] = L'-';
      wline[26] = kilp->joukkue + L'0';
      }
   wline[27] = 0;
   vidspwmsg(2, 0, 7, 0, wline);
   if (lainakortti(kilp, os)) vidspmsg(2,27, 0, 7, " LAINA ");
   else clrtxt(2,30,36);
   sprintf(line, "Lukemisaika %8.8s", aikatostr_ls(as, em->time, t0));
   vidspmsg(3, 7, 7, 0, line);
   if (em->maali != TMAALI0/KSEK) {
      sprintf(line, "Maaliaika %8.8s", aikatostr_ls(as, em->maali, t0));
      vidspmsg(3, 30, 7, 0, line);
      }
   vidspmsg(2, 34, 7, 0, "Sarja");
   vidspmsg(2, 41, 7, 0, (char *)Sarjat[kilp->sarja].sarjanimi);
   if (!esitark || esitark == 2) {
      vidspmsg(2, 51, 7, 0, "Rata");
      vidspmsg(2, 56, 7, 0, kilp->ostiet[os1].hajonta);
      vidspmsg(6, 41, 7, 0, "Tarkistuksen tulos");
      vidspmsg(7, 41, 7, 0, "T: T)arkistus ok");
      vidspmsg(8, 41, 7, 0, "H: H)ylk‰ys");
      vidspmsg(9, 41, 7, 0, "K: K)eskeytys");
      vidspmsg(10, 41, 7, 0, "I: hylk‰ysesI)tys");
      vidspmsg(11, 41, 7, 0, "P: P)oissa");
	  vidspmsg(13, 41, 0, 7, "Tark.tulos:");
      mene(54, 14);
	  strcpy(line, "      ");
	  line[0] = kilp->Tark(os1, 0);
	  if (kilp->Tark(os1, 1) == 'S')
		  strcpy(line+2, "Sulj");
      vidspmsg(13, 53, 7, 0, line);
      vidspmsg(14, 41, 0, 7, "Selitys:");
      vidspmsg(15, 41, 7, 0, kilp->ostiet[os1].selitys);
		if (Sarjat[kilp->sarja].tsak[os1]) {
		  vidspmsg(17, 41, 7, 0, "L)is‰aikalkm:");
		   vidint(17, 55, 2, kilp->ostiet[os1].sakko/Sarjat[kilp->sarja].tsak[os1]/SEK);
			}
	  }
   naytatiedot(em, kilp, os, loppuosa);
   }

void lisaaika(kilptietue *kilp, int d, int os)
{
	kilptietue entkilp;
	char ch;
	int lkm;

	entkilp = *kilp;
	lkm = kilp->ostiet[os].sakko/Sarjat[kilp->sarja].tsak[os]/SEK;
	inputlong(&lkm, 2, 55, 17, "\r", &ch);
	kilp->ostiet[os].sakko = lkm * Sarjat[kilp->sarja].tsak[os] * SEK;
	if (kilp->ostiet[os].sakko != entkilp.ostiet[os].sakko)
		tallenna(kilp, d, 1, 0, 0, &entkilp);
}

static void editemit(INT32 ep, emittp *em, kilptietue *kilp, int osuus)
   {
   INT16 cd[50];
   INT i, j, r, x;
   UINT tm[50];
   INT32 tl, etm, emt;
   char line[20], ch;

   for (r = 4; r < ySize-4; r++) {
      clrtxt(r, 0, 63);
      }
   for (i = 0; i < 50; i++) {
      etm = em->time;
      emt = em->maali;
      cd[i] = em->ctrlcode[i];
      tm[i] = em->ctrltime[i];
      }
   i = -1;
   j = 0;
   vidspmsg(5,45,7,0,"Luetun kortin koodi");
   vidint(6, 50, 7, em->badge0);
   vidspmsg(8,45,7,0,"Emit-maaliaika");
   vidspaika_l(9,50,em->maali, t0, 8);
   vidspmsg(10,45,7,0,"Tulos-maaliaika");
   VIDSPAIKA(11,50,kilp->Maali(osuus, 0), t0);
   do {
      clrln(ySize-3);
      clrln(ySize-1);
      vidspmsg(ySize-1,0,7,0, 
               "P)uolita ajat, L)is‰‰ aikoihin vakiolis‰ys, +: Tallenna, Esc: Poistu");
      for (r = 0; r < 50; r++) {
		 sprintf(line, "%2d %3d %5d |", r, cd[r], tm[r]);
         x = r / (ySize-8);
         vidspmsg(r + 4 - (ySize-8)*x, 15*x, 7, 0, line);
         }
      if (i == -1) {
		 inputaika_l(&etm, t0, 10, 19, 3, "+lLpP\x1b\r\x05\x18\t", &ch);
		 if (ch == KB_CR || ch == KB_TAB)
			inputaika_l(&emt, t0, 10, 40, 3, "+lLpP\x1b\r\x05\x18\t", &ch);
		 i = 0;
         }
      else {
         x = (i / (ySize-8)) * 15;
		 r = i % (ySize-8) + 4;
         switch (j) {
            case 0 :
               inputint(&cd[i], 3, x+3, r, "+lLpP\x1b\r\x05\x18\t", &ch);
               break;
            case 1 :
               tl = tm[i];
               inputlong(&tl, 5, x+7, r, "+lLpP\x1b\r\x05\x18\t", &ch);
               tm[i] = (UINT) tl;
               break;
            }
		 ch = toupper(ch);
         switch (ch) {
            case ESC :
               ch = ' ';
               selectopt("Peruutetaanko kaikki muutokset (K/E)", "KE", &ch);
               if (ch == 'K') {
                  ch = ESC;
                  }
               break;
			case KB_TAB :
               j = 1-j;
               break;
            case 'P' :
               ch = ' ';
               selectopt("Jaetaanko kaikki leimausajat kahdella (K/E)", "KE", &ch);
               if (ch == 'K') {
                  for (r = 0; r < 50; r++) {
                     tm[r] /= 2;
                     }
				  }
               break;
            case 'L' :
               tl = 0;
               clrln(ySize-3);
               vidspmsg(ySize-3, 0, 7, 0, "Anna kaikkiin aikoihin tuleva lis‰ys (sek)");
               inputlong(&tl, 6, 45, ySize-3, "\r", &ch);
               for (r = 1; r < 50; r++) {
                  if ((r == 1 || tm[r]) && (tl + (INT32)tm[r]) >= 0 && tl + tm[r] < 32768L) {
                     tm[r] = (UINT) (tl + tm[r]);
                     }
                  }
			case KB_CR :
            case DOWN :
               i++;
               break;
            case UP :
               i--;
               break;
			case '+' :
               ch = ' ';
               selectopt("V)ahvista muutosten tallennus, J)atka muokkausta", "VJ", &ch);
               if (ch == 'V') {
                  ch = '+';
                  }
               break;
            }
         if (i > 49) {
            i = 0;
            }
         }
      }
   while (ch != ESC && ch != '+');
   if (ch == '+') {
      for (i = 0; i < 50; i++) {
         em->ctrlcode[i] = (UCHAR) cd[i];
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

#endif // _CONSOLE
static void e_maali(emittp *em, INT ep, kilptietue *kilp, INT os, INT d)
   {
   INT32 tm;
   char ch;
   kilptietue entkilp;

   if (kilp->Maali(os, 0) != TMAALI0) {
	  ch = ' ';
	  selectopt("Kilpailijalla jo aika - muutetaanko (K/E)", "KE", &ch);
	  if (ch == 'E')
		 return;
	  }
   tm = e_maaliaika(em, kilp, os);
   if (tm != TMAALI0) {
	  entkilp = *kilp;
	  kilp->setMaali(os, 0, pyoristatls(tm, 1));
	  tallenna(kilp, d, 1, 0, 0, &entkilp);
	  if (em->maali != tm/KSEK) {
		 em->maali = tm/KSEK;
		 putem(em, ep, 0);
		 }
	  }
   }

static void e_maalitoisto(int laskeajat)
   {
   INT eptr, d;
   kilptietue kilp, entkilp;
   emittp em;

   for (eptr = 0; eptr < emithead2; eptr++) {
      lueem(&em, eptr);
      if (em.kilpno && (d = getpos(em.kilpno)) > 0) {
         GETREC(&kilp, d);
         if (!kilp.ostiet[em.osuus].badge[0]) {
            kilp.ostiet[em.osuus].badge[0] = em.badge;
			tallenna(&kilp, d, 1, 0, 0, &entkilp);
            }
         entkilp = kilp;
         if (kilp.ostiet[em.osuus].badge[0] == em.badge && kilp.Maali(em.osuus, 0) == TMAALI0) {
            if (laskeajat && em.maali == TMAALI0/KSEK) {
               e_maali(&em, eptr, &kilp, em.osuus, d);
               }
            else {
               if (em.maali != TMAALI0/KSEK) {
                  kilp.setMaali(em.osuus, 0, pyoristatls(em.maali*KSEK, 1));
				  tallenna(&kilp, d, 1, 0, 0, &entkilp);
                  }
               }
            }
         }
      }
   }

#ifdef _CONSOLE
static void e_sarja(emittp *em, kilptietue *kilp, INT osuus, INT d)
   {
   INT i, os;
   char ch, snimi[HAJONTA+1] = "", msg[80];
   kilptietue entkilp;

   i = etsiesarja(em);
   if (i >= 0)
      strcpy(snimi, rata[i].rata);
   for (os = 0; os < Sarjat[kilp->sarja].osuusluku; os++) {
      if (!strcmpU(rata[i].rata, kilp->ostiet[os].hajonta)) {
         clrln(ySize-1);
         sprintf(msg, "Rata vastaa osuutta %d", os+1);
         vidspmsg(ySize-1,0,7,0,msg);
		 break;
         }
      }
   clrln(ySize-3);
   vidspmsg(ySize-3,0,7,0,"Anna uusi rata :               "
      "<Esc>: Poistu vaihtamatta rataa");
   inputstr(snimi, HAJONTA, 18, ySize-3, "\r\x1b", &ch, 0);
   if (ch == ESC)
      return;
   ch = ' ';
   selectopt("Vahvista radan vaihto (K/E)", "KE", &ch);
   if (ch == 'E')
      return;
   entkilp = *kilp;
   strcpy(kilp->ostiet[osuus].hajonta, snimi);
   tallenna(kilp, d, 1, 0, 0, &entkilp);
   }

INT leimat(void)
   {
   char ch, cs, ch2, ct, line[80], trk, selitys[SELITYS+1], as[20];
   INT i, d, key, os = 0, os1, kilpno = 0, bdgfl = 0, ep, emptr, erv = 25, loppuosa = 0, rm;
   static INT selaus, emithead0;
   INT32 badge = 0;
   kilptietue kilp;
   emittp em;
   static char valtxt[] =   "R)aportti, selaa: eD)ell S)eur V)iim, "
      "tU)hoa tietue, kO)odit, Esc: Pois      ";
   static char valtxt_m[] = "R)aportti, selaa: eD)ell S)eur V)iim, "
      "tU)hoa tietue, M)TR, kO)odit, Esc: Pois";
   static char numval[] = "Numero : kilpailijavalinta, Tab : "
      "Leimauskortin valinta, X : Kilpailijan vaihto";

   if (emitfl <= 0) {
      writeerror("Leimantarkastustoimintoa ei ole k‰ynnistetty", 0);
      return(0);
      }
   if (ySize < 33)
      erv = ySize-8;
   if (ajat_on) status_on = 0;
   if (regnly[0] == 13 || regnly[1] == 13 || regnly[NREGNLY-1] == 13)
      strcpy(valtxt, valtxt_m);
   clrscr();
   kehys(1);
   rm = ySize == 50 ? 41 : ySize-4;
   kilp.ostiet[os].badge[0] = 0;
   lueseuraava(&kilpno, &os, &em);
   for (;;) {
      clrtxt(2,0,59*status_on);
	  mene(15, 1);
      ct = 0;
      trk = 0;
      if (!kilpno || (d = getpos(kilpno)) <= 0) {
         badge = em.badge;
         sprintf(line, "Kilpailija %4d-%-2d kortti %7ld", kilpno, os+1,
            badge);
         clrtxt(0, 0, 33);
         vidspmsg(0,0,7,0, line);
         if (selaus) vidspmsg(0,50,7,0,"SELAUS");
         else clrtxt(0,50,56);
         for (i = 3; i < rm; i++) clrtxt(i,0,59+5*status_on);
         vidint(0, 57, 5, emitptr+1);
         if (em.time != TMAALI0/KSEK) {
            sprintf(line, "Lukemisaika %8.8s", aikatostr_ls(as, em.time, t0));
            vidspmsg(3, 7, 7, 0, line);
            }
         if (em.maali != TMAALI0/KSEK) {
            sprintf(line, "Maaliaika %8.8s", aikatostr_ls(as, em.maali, t0));
            vidspmsg(3, 30, 7, 0, line);
            }
         if (emithead2) naytatiedot(&em, NULL, 0, loppuosa);
         }
      else {
         GETREC(&kilp, d);
         if (os >= Sarjat[kilp.sarja].osuusluku)
            os = Sarjat[kilp.sarja].osuusluku - 1;
		 if (kilpparam.partio)
            os1 = Sarjat[kilp.sarja].osuusluku - 1;
         else
			os1 = os;
         line[18] = 0;
         if (kilp.Maali(os, 0) != TMAALI0) {
            sprintf(line, "Maaliaika %8.8s",
               aikatostr_ls(as, kilp.Maali(os, 0), t0));
            }
         else
            memset(line, ' ', 18);
         vidspmsg(3, 30, 7, 0, line);
         if ((ep = getem(&em, kilp.kilpno, os)) >= 0)
            emitptr = ep;
         else
			emitptr = emithead2 - 1;
         badge = kilp.ostiet[os].badge[0];
         outleimat(&kilp, os, &em, selaus, loppuosa);
		 ct = kilp.Tark(os1, 0);
         strcpy(selitys, kilp.ostiet[os1].selitys);
         mene(53, 13);
         }

      trk = 0;

      do {
         vidspmsg(ySize-3,0,7,0,valtxt);
         vidspmsg(ySize-1,0,7,0,numval);

         // T‰ss‰ loopissa odotetaan tarkastusn‰pp‰ily‰ tai uuden tietueen
         // valintaa. myˆs M)TR ja tU)hoa, kO)odit sek‰ Tab tunnistetaan.

         for (;;) {
            if (!kbready(&ch, &key)) {
               ch = 0;
               if (pros_emit(NULL)) {
                  Sleep(100);
                  }

               // jos selaus ei ole k‰ytˆss‰ ja emithead muuttuu, haetaan
               // viimeinen tietue

               if (!selaus && emithead2 != emithead0)
                  ch = 'V';
               else
                  continue;
               }
            
            if (!ch) {
               switch (key) {
				  case 75:
                  case 77:                         
                     ch = 4;
                     break;
                  case 81:
                     ch = 'S';
                     break;
                  case 21:
                     ch = 21;
                     break;
                  case 73:
                     ch = 'D';
                     break;
                  case 118:
					 ch = 'V';
                     break;
                  case 132:
                     ch = 'A';
                     break;
                  }
               }
			ch = toupper(ch);
            if (ch == 27) goto loppu;
#ifdef ESITARK
            if (esitark) {
               if (!ch || stschind(ch, "0123456789RSDLMNVAG\x09") >= 0)
                  break;
               }
            else
#endif
	            if (!ch || stschind(ch, "0123456789THKIPR XSZAGDEUMNVOL\x04\x05\x07\x09\x13\x14\x15\r") >= 0)
		            break;
            readkbd(&ch,0,spoolfl);
            }

         // Loopista poistutaan, jos n‰pp‰in koskee tietueen vaihtoa,
         // tuhoamista tai MTR-ohjausta.

#ifdef ESITARK
         if (ch == 'L' && esitark > 0) {
            esitark = 3-esitark;
            ch = 0;
            break;
            }
#endif
         if (ch && stschind(ch, "0123456789AGDUMNSZVX\x09") >= 0)
            break;
		 if (ch == '\r') ch = ct;
         readkbd(&ch2,0,spoolfl);
         if (ch == 'O') {
            editkoodit();
            break;
            }
         if (ch == 0) {
            ohj_aikaruutu(&ch2);
            continue;
            }
         if (ch == 7) {
            ch = ' ';
            selectopt("Vahvista koodien ja tulosten siirto Emit-tiedoista (KE)", "KE", &ch);
            if (ch == 'K') {
               ch = ' ';
               selectopt("Lasketaanko aika, jos ei jo laskettuna Emit-tiedoissa (KE)", "KE", &ch);
			   e_maalitoisto(ch == 'K');
               ch = 'V';
               }
            else {
               ch = 0;
               continue;
               }
			}
         if (kilpno) {
            switch (ch) {
               case 19 :
                  e_sarja(&em, &kilp, os, d);
                  break;
               case 20 :
                  e_maali(&em, ep, &kilp, os, d);
                  break;
					case 21 :
                  emitva_uusinta(-1, ep);
                  break;
               case 'T':
                  trk = 'T';
                  break;
               case 'H':
                  trk = 'H';
                  break;
               case 'K':
                  trk = 'K';
                  break;
               case 'I':
                  trk = 'I';
                  break;
			   case 'P':
                  trk = 'P';
                  break;
               case ' ':
                  trk = '-';
                  break;
               case 'R':
                  tark_rap(emittulparam, kilpno, os);
                  break;
			   case 'L':
				  if (Sarjat[kilp.sarja].tsak[os]) {
					  lisaaika(&kilp, d, os);
					  }
				  ch = 'R';
				  break;
               case 5 :
                  editemit(ep, &em, &kilp, os);
                  break;
			   case 4 :
                  loppuosa = !loppuosa;
                  break;
               }

            // n‰pp‰inpainallus poistetaan, jos tullaan raporttivalinnasta
			// tai ajan kirjauksesta, poistutaan lis‰ksi loopista

            readkbd(&ch2,0,spoolfl);
            if (ch == 'R' || ch == 19 || ch == 20 || ch == 22) break;
            }

         // Jos painallus koskee tarkastusta, luetaan myˆs selitys sek‰
         // pyydet‰‰n vahvistusta tietojen tallentamisesta

         if (trk) {
            viwrrect(13, 53, 13, 53, &trk, 7, 0, 0);
            ct = trk;
            strcpy(selitys, kilp.ostiet[os].selitys);
            inputstr(selitys, SELITYS, 41, 15, "\r\x1b", &ch, 0);
            if (ch == '\x1b') break;
            vidspmsg(16,42,7,0, "Vahvista kirjaus (K/E)");
            mene(65, 17);
            do {
               ch = readkbd(&ch,1,spoolfl);
               ch = toupper(ch);
               if (ch == 27) goto loppu;
               } while (ch != 'K' && ch != 'E');
            clrtxt(16,42,59+5*status_on);
			if (ch == 'E') {
               ch = 0;
               mene(54, 14);
               }
            else {
               emit_tallenna(&kilp, os1, trk, selitys, 0);
               }
            }
         } while(!ch);

      // T‰h‰n tullaan, kun tarkastustieto on vahvistettu tai n‰pp‰ily on
      // koskenut uuden tietueen valintaa, tuhoamista, MTR-ohjausta,
      // raporttia tai koodien editointia.

      // Palataan p‰‰loopin alkuun, jos tarakastustieto on hyv‰ksytty
      // tai on poistuttu raportti- tai koodieditointitoiminnosta

      if (ch == 'R' || ch == 'K' || ch == 'O' || ch == 19 || ch == 20) continue;

      // Tuhoamisen j‰lkeen siirryt‰‰n seuraavaan tietueseen.
	  // Jos tuhottu tietue oli viimeinen, siirrtyt‰‰n viimeiseen
      // j‰ljell‰olevaan.

      if (ch == 'U') {
         poista_emit(&em);
         ch = 'S';
		 }

      // MTR-ohjauksen j‰lkeen siirryt‰‰n viimeiseen tietueeseen

      if (ch == 'M') {
         aseta_mtr();
         ch = 'V';
         }

      if (ch == 'N') {
#ifdef EMITFILE
         seur_mtr();
         Sleep(200);
#endif
         ch = 'V';
         }

      if (ch == 'X') {
         vaihdakilp(kilpno, os, 0);
         lueedellinen(&kilpno, &os, &em);
         ch = 'S';
		 }

      // T‰ss‰ k‰sitell‰‰n tietueen vaihto selaamalla tai hypp‰‰m‰ll‰
      // viimeiseen

      if (emithead2 && (ch == 'S' || ch == 'D' || ch == 'V' || ch == 'L' || 
         ch == 'G' || ch == 'A' || ch == 'Z')) {

         // Aluksi poistetaan n‰pp‰intieto

         readkbd(&ch2,0,spoolfl);

         // Jos tiedot ovat muuttuneet, kysyt‰‰n vahvistusta tallentamisesta

		 if (kilpno && ((trk && trk != kilp.Tark(os1, 0)) ||
			strcmp(selitys, kilp.ostiet[os1].selitys))) {
            cs = ' ';
            selectopt("T)allenna n‰ytˆn tiedot, O)hita muutokset, P)alaa tietoihin",
               "TOP", &cs);
            switch (cs) {
               case 'T' :
                  emit_tallenna(&kilp, os1, trk, selitys, 0);
				  break;
               case 'P' :
                  continue;
               }
            }

		 switch (ch) {
			case 'G':
               clrln(ySize-3);
               vidspmsg(ySize-3,0,7,0,"Anna j‰rjestysnumero");
               INPUTINT(&emitptr, 5, 22, ySize-3, "\r", &ch);
               emitptr--;
               ch = 'S';
               if (emitptr < 0)
                  emitptr = 0;
               emitptr--;
            case 'V':
               if ((ch == 'V' && emithead2 > 1) || emitptr >= emithead2)
                  emitptr = emithead2 - 1;
               //  case jatkuu tarkoituksella
            case 'S':
               emptr = emitptr;
               lueseuraava(&kilpno, &os, &em);
               if (emitptr <= emptr) {
                  selaus = 0;
				  emithead0 = emithead2;
                  }
               ch = 0;
               break;
            case 'Z':
               emptr = emitptr;
               lueseuraavaong(&kilpno, &os, &em);
               if (emitptr <= emptr) {
                  selaus = 0;
                  emithead0 = emithead2;
                  }
               ch = 0;
               break;
            case 'A':
               if (emithead2 > 1)
                  emitptr = 1;
               //  case jatkuu tarkoituksella
            case 'D':
               lueedellinen(&kilpno, &os, &em);
               selaus = 1;
               ch = 0;
               break;
            }

		 // Seuraavan pit‰isi toteutua aina
         // P‰‰looppi aloitetaan alusta

         if (ch == 0) {
            continue;
            }
		 }

      // Tabulaattori vaihtaa kilpno / badge - valinnan

      if (ch == 9) {
         bdgfl = !bdgfl;
         readkbd(&ch2,0,spoolfl);
         }

      // T‰h‰n looppiin tullaan, kun on painettu numeroa tai tabulaattoria
      // Nyt odotetaan numeron n‰pp‰ily‰

      do {
         clrln(ySize-3);
         vidspmsg(ySize-3,0,7,0,"Esc: poistu numerovalinnasta");
         vidspmsg(ySize-1,0,7,0,numval);
         clrtxt(2,0,59+5*status_on);
		 clrtxt(3,0,59+5*status_on);
         if (!bdgfl) {
            if (ch != '\r') kilpno = 0;
            INPUTINT(&kilpno, 4, 11, 0, "\r\x1b\x09", &ch);
            while (ch == '\r') {
               os++;
               INPUTINT(&os, 2, 16, 0, "\r", &ch);
               os--;
               if (os >= 0 && os < kilpparam.n_os_akt)
                  break;
               else
                  erbeep();
               }
            }
         else {
            if (ch != '\r') badge = 0;
            inputlong(&badge, 7, 27, 0, "\r\x1b\x09", &ch);
            if (ch == '\r') {
               kilpno = bdg2kno(badge, 0);
               os = etsios(kilpno, badge);
               }
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
			vidspmsg(3,0,7,0, "Kilpailijaa ei lˆydy");
			ch = 0;
            }
         else {
            GETREC(&kilp, d);
			if (kilpparam.partio)
               os1 = Sarjat[kilp.sarja].osuusluku - 1;
            else
               os1 = os;  
            selaus = 1;
            }

		 // luupissa pysyt‰‰n, jos on painettu tabulaattoria tai
		 // syˆtetty tuntematon numero

		 } while (!ch);
	  }
   loppu:
   clrscr();
   status_on = 1;
   return(0);
   }
#endif // _CONSOLE

void emitlista(tulostusparamtp *tulprm)
   {
   static INT elistaalku = 1;
   INT r, d, l, i, elistaloppu;
   INT32 enn;
   wchar_t *p, ch, line[80], as[20], as1[20], as2[20], st[80];
   emittp em;
   kilptietue kilp;

   if (tulprm->lstf == NULL)
	   return;
#ifdef _CONSOLE
   clrln(ySize-3);
   vidspmsg(ySize-3,0,7,0,"Anna alku- ja loppurivit (Esc: peruuta):");
   INPUTINTW(&elistaalku, 4, 42, ySize-3, L"\r\x1b", &ch);
#else
   i = elistaalku;
   inputint_prompt(&i,L"Anna alkurivi", &ch);
   elistaalku = i;
#endif // _CONSOLE
   if (ch == 27) {
	  clrln(ySize-3);
	  return;
	  }
   if (elistaalku < 1) elistaalku = 1;
   elistaloppu = 9999;
#ifdef _CONSOLE
   INPUTINTW(&elistaloppu, 4, 49, ySize-3, L"\r\x1b", &ch);
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
	if (tulprm->printer == GDIPRINTER)
		tulprm->tulmuot.numlev = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
   l = 0;
   for (r = elistaalku-1; r < elistaloppu; r++) {
	  if (_read(emitfile, &em, sizeof(emittp)) < sizeof(emittp)) break;
      memset(&kilp, 0, sizeof(kilptietue));
      if ((d = getpos(em.kilpno)) > 0) {
         GETREC(&kilp, d);
		 }
	  if (em.maali != TMAALI0/KSEK)
		  aikatowstr_ls(as, em.maali, t0);
	  else
		  aikatowstr_ls(as, em.time, t0);
	  enn = haeennakko(&em, &kilp, em.osuus);
	  for (i = 49; i >= 0; i--) if (em.ctrltime[i]) break;
	  aikatowstr_ls(as1, 10L*em.ctrltime[i]-enn, 0);
	  as2[0] = 0;
	  if (i > 1)
		 aikatowstr_ls(as2, 10L*(em.ctrltime[i-1] - enn), 0);
	  swprintf(line, L"%4d %8.8s %4d-%-2d %8d %-23.23s %c %8.8s (%8.8s)",
		 r+1, as, em.kilpno, em.osuus+1, em.badge, kilp.Joukkue(st, 23),
		 kilp.wTark(em.osuus, 0), as1, as2);
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
   endpage(tulprm);
   elistaalku = r + 1;
   }

ratatp *haerata(kilptietue *kilp, int osuus)
   {
   ratatp *rt = NULL;

   if (rata) {
	  if (kilp->ostiet[osuus].hajonta[0]) {
		 for (rt = rata; rt < rata+nrata; rt++)
			if (!strcmpU(rt->rata, kilp->ostiet[osuus].hajonta))
				break;
		 if (rt == rata+nrata) rt = NULL;
		 }
	  }
   return(rt);
   }

#ifndef _CONSOLE
void paaots_on(tulostusparamtp *tulprm);
void paaots_pois(tulostusparamtp *tulprm);
void bold_on(tulostusparamtp *tulprm);
void bold_pois(tulostusparamtp *tulprm);
void putfld_int(tulostusparamtp *tulprm, int k, INT pos, INT len, INT oik, INT r);

void tulostaRastiVa(tulostusparamtp *tulprm, int kilpno, int osuus)
{
	ratatp *rt;
	kilptietue kilp, kilp1;
	emittp em;
	emitvaanaltp *eva = 0;
	rastivaanaltp *rstva = 0;
	int d, i, r, p, lj, sj1, ntlsrivi = 50, aos, yos;
	int edaika, tls, t1, karki, emvaluku, srj, sj, ero;
	int xva[8] = {0, 60, 100, 160, 250, 265, 330, 430};
	int xsrj[4] = {510, 560, 770, 855};
	wchar_t *vaots[2][8] = {{L"Rasti", L"Kdi", L"Aika", L"V‰li", L"Sija", L"Paras", L"Ero", L"M/km"},
						{L"Cntrl", L"Code", L"Time", L"Spilt", L"Rank", L"Best", L"Diff", L"M/km"}};
	wchar_t *srjots[2][4] = {{L"Sj.", L"Joukkue", L"Aika", L"Ero"},
							{L"Rank", L"Team", L"Time", L"Diff"}};
	wchar_t line[121], st[61], st2[61], as[13];

	if (tulprm->lstf == NULL || (d = getpos(kilpno)) < 1)
		return;
	kilp.getrec(d);
	rt = haerata(&kilp, osuus);
	if (!rt || !rt->rastiluku) return;
	srj = kilp.sarja;
	yos = Sarjat[srj].yosuus[osuus];
	merk_sijat(srj, osuus, kilpparam.valuku+1, 1);
	kilp.getrec(d);
	EnterCriticalSection(&print_CriticalSection);
	initpage(tulprm, 0);
	initline(tulprm);
	paaots_on(tulprm);
	putfld(tulprm, kilpailu,0,70,0,0);
	endline(tulprm, 1);
	swprintf(line, L"%d  %s  -  %s", kilpno, kilp.Joukkue(st, 30, 0), kilp.Nimi(st2, 30, osuus, -1));
	putfld(tulprm, line, 0, 80, 0, 0);
	endline(tulprm, 1);
	tls = kilp.osTulos(osuus, 0);
	if (tls && kilp.osHyv(osuus)) {
		AIKATOWSTRS(st, tls, 0);
		st[kilpparam.laika2] = 0;
		elimwzb1(st);
		if (tulprm->language == 0)
			swprintf(line, L"%s - osuus %s -  %d.   %s", Sarjat[srj].Sarjanimi(), wosuuskoodi(srj, osuus, 0, 0), kilp.osSija(osuus), st);
		else
			swprintf(line, L"%s -  Leg %s -  %d.     %s", Sarjat[srj].Sarjanimi(), wosuuskoodi(srj, osuus, 0, 0), kilp.osSija(osuus), st);
		}
	else {
		if (tulprm->language == 0)
			swprintf(line, L"%s - osuus %s -  %s", Sarjat[srj].Sarjanimi(), wosuuskoodi(srj, osuus, 0, 0), kilp.TarkStr(osuus, false, tulprm->language));
		else
			swprintf(line, L"%s -  Leg %s  -   %s", Sarjat[srj].Sarjanimi(), wosuuskoodi(srj, osuus, 0, 0), kilp.TarkStr(osuus, false, tulprm->language));
		}
	putfld(tulprm, line, 0, 80, 0, 0);
	endline(tulprm, 2);
	paaots_pois(tulprm);
	r = tulprm->vpos;
	tulprm->vpos -= 4*tulprm->lstf->u.wp.GDIparam.Currentfont.line;
	if (tulprm->language == 0)
		swprintf(line, L"Tulostettu %s - %s", wpaiva(), wkello());
	else
		swprintf(line, L"Printed %s - %s", wpaiva(), wkello());
	putfld(tulprm, line, xsrj[1], 30, 0, 0);
	endline(tulprm, 0);
	if (tulprm->language == 0)
		swprintf(line, L"Tuloksia %d", ntulos[srj][yos][0]);
	else
		swprintf(line, L"Results %d", ntulos[srj][yos][0]);
	putfld(tulprm, line, xsrj[1], 27, 0, 0);
	endline(tulprm, 0);
	getem(&em, kilpno, osuus);
	if (emitanal_fl && em.kilpno == kilpno) {
		EnterCriticalSection(&rastiva_CriticalSection);
		laskeemitvaliajat(&em, &kilp, &eva, &rstva, &emvaluku);
		if (eva) {
			if (tulprm->language == 0)
				swprintf(line, L"V‰liaikatietoja %d", rvalit[eva[0].rvno].lkm[srj][yos]);
			else
				swprintf(line, L"Split times %d", rvalit[eva[0].rvno].lkm[srj][yos]);
			putfld(tulprm, line, xsrj[1], 27, 0, 0);
			endline(tulprm, 0);
			tulprm->vpos = r;
			edaika = 0;
			putfld(tulprm, vaots[tulprm->language][0], xva[0], 5, 1, 0);
			putfld(tulprm, vaots[tulprm->language][1], xva[1], 4, 1, 0);
			putfld(tulprm, vaots[tulprm->language][2], xva[2], 8, 1, 0);
			putfld(tulprm, vaots[tulprm->language][3], xva[3], 8, 1, 0);
			putfld(tulprm, vaots[tulprm->language][4], xva[4], 4, 1, 0);
			putfld(tulprm, vaots[tulprm->language][5], xva[5], 8, 1, 0);
			putfld(tulprm, vaots[tulprm->language][6], xva[6], 8, 1, 0);
			putfld(tulprm, vaots[tulprm->language][7], xva[7], 5, 1, 0);
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
				aikatowstr_s(as, eva[i].rvaika+ero, 0);
				elimwzb1(as);
				putfld(tulprm, as, xva[3], 8, 1, 0);
				if (eva[i].rvno >= 0) {
					sj = bSrchInt(eva[i].rvaika, rvalit[eva[i].rvno].ajat[srj][yos],
						rvalit[eva[i].rvno].lkm[srj][yos]) + 1;
					putfld_int(tulprm, sj, xva[4], 4, 1, 0);
					karki = rvalit[eva[i].rvno].ajat[srj][yos][0];
					aikatowstr_s(as, karki, 0);
					elimwzb1(as);
					putfld(tulprm, as, xva[5], 8, 1, 0);
					t1 = eva[i].rvaika - karki + ero;
					aikatowstr_s(as+1, abs(t1), 0);
					elimwzb1(as+1);
					as[0] = (tls >= 0) ? L'+' : L'-';
					putfld(tulprm, as, xva[6], 8, 1, 0);
					if ((mtk = rvalit[eva[i].rvno].matka) > 10) {
						if (eva[i].rvaika) {
							kmaika = (100000 * (eva[i].rvaika+ero) + 30 * mtk) / (60 * mtk);
							swprintf(as, L"%2d,%02d", kmaika/100, kmaika%100);
							putfld(tulprm, as, xva[7], 5, 1, 0);
							}
						}
					}
				endline(tulprm, 0);
				}
			if (rt->maalilaji < 2 || maalileimasin < 0) {
				putfld(tulprm, st, xva[2], 8, 1, 0);
				if (edaika) {
					aikatowstr_s(as, kilp.osTulos(osuus, 0)/SEK - edaika, 0);
					elimwz(as);
					putfld(tulprm, as, xva[3], 8, 1, 0);
					}
				endline(tulprm, 0);
				}
			free(eva);
			free(rstva);
			}
		LeaveCriticalSection(&rastiva_CriticalSection);
		}
	tulprm->vpos = r;
	putfld(tulprm, L"|", xsrj[0]-10, 1, 0, 0);
	putfld(tulprm, srjots[tulprm->language][0], xsrj[0], 4, 1, 0);
	putfld(tulprm, srjots[tulprm->language][1], xsrj[1], 5, 0, 0);
	putfld(tulprm, srjots[tulprm->language][2], xsrj[2], 8, 1, 0);
	putfld(tulprm, srjots[tulprm->language][3], xsrj[3], 8, 1, 0);
	endline(tulprm, 0);
	putfld(tulprm, L"|", xsrj[0]-10, 1, 0, 0);
	endline(tulprm, 0);
	sj = kilp.osSija(osuus);
	lj = kilpparam.valuku+5;
	p = JosAlku(srj, yos, &aos);
	sj1 = 1;
	for (r = 0; r < ntlsrivi && p > 0; r++) {
		kilp1.getrec(p);
		if (kilp1.kilpno == kilp.kilpno) {
			bold_on(tulprm);
			}
		putfld(tulprm, L"|", xsrj[0]-10, 1, 0, 0);
		if (kilp1.osSija(aos) == sj1)
			putfld_int(tulprm, sj1, xsrj[0], 4, 1, 0);
		putfld(tulprm, kilp1.Joukkue(st,25,false), xsrj[1], 25, 0, 0);
		if (kilp1.osHyv(aos)) {
			AIKATOWSTRS(st, kilp1.osTulos(aos, 0), 0);
			st[8] = 0;
			elimwzb1(st);
			}
		else
			wcscpy(st, kilp1.TarkStr(aos, false, tulprm->language));
		putfld(tulprm, st, xsrj[2], 8, 1, 0);
		if (r == 0)
			karki = kilp1.osTulos(aos, 0);
		else if (kilp1.osHyv(aos) && kilp1.osSija(aos) > 1) {
			AIKATOWSTRS(st+1, kilp1.osTulos(aos, 0)-karki, 0);
			st[9] = 0;
			elimwzb1(st+1);
			st[0] = L'+';
			putfld(tulprm, st, xsrj[3], 8, 1, 0);
			}
		if (kilp1.kilpno == kilp.kilpno) {
			bold_pois(tulprm);
			}
		endline(tulprm, 0);
		do {
			p = askellasarja(srj, yos, &aos, kilpparam.valuku+1, p, &lj);
			sj1++;
			} while (p > 0 && sj > ntlsrivi && r == ntlsrivi - 8 && sj1 < sj-4);
		}
	LeaveCriticalSection(&print_CriticalSection);
	endpage(tulprm);
	enddoc(tulprm);
}
#endif // !_CONSOLE

void prtemitsivu(tulostusparamtp *tulprm)
   {
   if (emitosa) {
	  endpage(tulprm);
	  emitosa = 0;
	  }
   }

void emit_rap(tulostusparamtp *tulprm, INT kilpno, INT os)
   {
   ratatp *rt;
   emittp em;
   INT d, i, r, r1, os1, tulkinta[51], kokosivu, rst[4], nr;
   INT32 enn, edaika, tl, tls;
   wchar_t *p, line[81], ln1[81], ln2[81], st[17], as[13], aero[13];
   kilptietue kilp;

   if (tulprm->lstf == NULL)
	   return;
   if (esitark >= 0 && (d = getpos(kilpno)) > 0) {
	  GETREC(&kilp, d);
	  }
   else
	  return;
   rt = getrata(kilp.ostiet[os].hajonta);
   if (!rt || !rt->rastiluku) return;
   getem(&em, kilpno, os);
   i = tarkista(&em, NULL, tulkinta, -1, 0);
   EnterCriticalSection(&print_CriticalSection);
   if (emitsivu && i) {
	  kokosivu = 1;
	  prtemitsivu(tulprm);
	  }
   else
	  kokosivu = (emitsivu == 0);
   if (!emitosa) {
		p = 0;
		initpage(tulprm, tulprm->printer == LJETII ? initfont_emrap : L"C");
		initline(tulprm);
		if (tulprm->printer == GDIPRINTER)
			tulprm->tulmuot.numlev = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
	  }
	if (tulprm->language > 0)
		swprintf(line, L"Printed %s at %s", wpaiva(), wkello());
	else
		swprintf(line, L"Tulostettu %s kello %s", wpaiva(), wkello());
	putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	endline(tulprm,kokosivu);
	if (kokosivu) {
		if (tulprm->language > 0)
			swprintf(line, L"Team and leg   %d -%s", kilpno, wosuuskoodi(kilp.sarja, os, 0, 0));
		else
			swprintf(line, L"Joukkue ja osuus   %d -%s", kilpno, wosuuskoodi(kilp.sarja, os, 0, 0));
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,0);
	  swprintf(line, L"%-24.24s-%-2d  %s", kilp.Seura(ln2), (int)kilp.joukkue,
		 kilp.Nimi(ln1, OSNIMIL, os));
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,0);
	if (tulprm->language > 0)
		swprintf(line, L"Class and route    %s, %s", Sarjat[kilp.sarja].Sarjanimi(),
			kilp.ostiet[os].Hajonta());
	else
		swprintf(line, L"Sarja ja rata      %s %s", Sarjat[kilp.sarja].Sarjanimi(),
			kilp.ostiet[os].Hajonta());
	  }
   else {
		if (tulprm->language > 0)
			swprintf(line, L"Class %s, team and leg %d -%s, %s-%d",
				Sarjat[kilp.sarja].sarjanimi, kilpno, wosuuskoodi(kilp.sarja, os, 0, 0), kilp.Seura(ln1), (int)kilp.joukkue);
		else
			swprintf(line, L"Sarja %s, joukkue ja osuus %d -%s, %s-%d",
				Sarjat[kilp.sarja].sarjanimi, kilpno, wosuuskoodi(kilp.sarja, os, 0, 0), kilp.Seura(ln1), (int)kilp.joukkue);
	  }
   putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
   endline(tulprm,0);
   tl = Sarjat[kilp.sarja].lahto;
   if (os) {
	  if (kilp.ostiet[os].ylahto != TMAALI0)
		 tl = NORMKELLO(kilp.ostiet[os].ylahto);
	  else {
		 if (kilp.Maali(os-1, 0) != TMAALI0)
			tl = kilp.Maali(os-1, 0);
		 }
	  }
   if (kokosivu) {
		if (tulprm->language > 0)
			swprintf(line, L"Start time         %8.8s", AIKATOWSTRS(as, tl, t0));
		else
			swprintf(line, L"L‰htˆaika          %8.8s", AIKATOWSTRS(as, tl, t0));
		putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
		endline(tulprm,0);
		}
   tls = 0;
   if (kilp.Maali(os, 0) != TMAALI0)
	  tls = kilp.Maali(os, 0) - tl;
		if (tulprm->language > 0)
			swprintf(line, L"Result             %8.8s", AIKATOWSTRS(as, tls, 0));
		else
			swprintf(line, L"Tulos              %8.8s", AIKATOWSTRS(as, tls, 0));
   if (kokosivu) {
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,1);
		if (tulprm->language > 0)
			swprintf(line, L"Emit card no    %ld  manufactured %d-%d  %s",
				em.badge, em.badgeyear, em.badgeweek,
				lainakortti(&kilp, os) ? L"Lent card" : L"");
		else
			swprintf(line, L"Emit-kortti nro    %ld  valmistettu %d-%d  %s",
				em.badge, em.badgeyear, em.badgeweek,
				lainakortti(&kilp, os) ? L"Laina" : L"");
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,0);
		if (tulprm->language > 0)
			swprintf(line, L"Card read %8.8s", aikatowstr_ls(as, em.time,t0));
		else
			swprintf(line, L"Kortti luettu %8.8s", aikatowstr_ls(as, em.time,t0));
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,0);
		if (tulprm->language > 0)
			wcscpy(line, L"Result of checking : ");
		else
			wcscpy(line, L"Tarkastuksen tulos : ");
	  }
   else
	  wcscat(line, L",    ");
   if (kilpparam.partio)
	  os1 = Sarjat[kilp.sarja].osuusluku - 1;
   else
	  os1 = os;
   switch (kilp.Tark(os1, 0)) {
	  case L'T':
		if (tulprm->language > 0)
			wcscat(line, L"Accepted");
		else
			wcscat(line, L"Hyv‰ksytty");
		 break;
	  case L'I':
		if (tulprm->language > 0)
			wcscat(line, L"DQ proposed");
		else
			wcscat(line, L"Hylk‰ysesitys");
		 break;
	  case L'H':
		if (tulprm->language > 0)
			wcscat(line, L"DQ");
		else
			wcscat(line, L"Hyl‰tty");
		 break;
	  case L'K':
		if (tulprm->language > 0)
			wcscat(line, L"DNF");
		else
			wcscat(line, L"Keskeytt‰nyt");
		 break;
	  default:
		if (tulprm->language > 0)
			wcscat(line, L"Control result unknown ");
		else
			wcscat(line, L"Leimantarkastus avoinna");
		 break;
	  }
   putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
   endline(tulprm,1);
   edaika = 0;
   enn = haeennakko(&em, &kilp, os);
   if (kokosivu) {
		if (tulprm->language > 0)
			wcscpy(line, L"   Control   Cntrlcode  Emit-code      Time  Leg time ");
		else
			wcscpy(line, L"   Rastinro  Rastikoodi Emitkoodi      Aika  Rastiv‰li");
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,1);
	  for (i = 0; i < 50; i++) {
		 if (i && !em.ctrlcode[i] && !em.ctrltime[i]) break;
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
			r1 = 0;
			while (r1 <= r) {
			   if (rt->rastikoodi[r1] > 9999 && rt->rastikoodi[r1] <= 10002) {
				  r++;
				  }
			   r1++;
			   }
			}
		 if (r >= 0 && r <= rt->rastiluku) {
			aero[0] = L' ';
			aikatowstr_ls(aero+1, 10L*(em.ctrltime[i]-enn-edaika),0);
			aero[9] = 0;
			elimwzb1(aero+1);
			edaika = em.ctrltime[i]-enn;
			}
		 if (em.ctrltime[i] - enn < 0) {
			as[0] = L'-';
			aikatowstr_ls(as+1, 10L*(enn-em.ctrltime[i]),0);
			}
		 else {
			as[0] = L' ';
			aikatowstr_ls(as+1, 10L*(em.ctrltime[i]-enn),0);
			}
		 as[9] = 0;
		 elimwzb1(as+1);
		 if (i == 0) {
			 if (tulprm->language > 0)
				wcscpy(st, L" Reset ");
			 else
				wcscpy(st, L"Nollaus");
			}
		 if (r == rt->rastiluku && em.ctrlcode[i] > 248) {
			if (tulprm->language > 0)
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
		 if (r >= 0 && r < rt->rastiluku) {
			r = rt->rastikoodi[r];
			swprintf(line, L"  %8s      %3d         %3d   %10.10s%10.10s",
			   st, r, em.ctrlcode[i], as, aero);
			}
		 else {
			rst[0] = 0;
			nr = 4;
			haerastit(em.ctrlcode[i], rst, &nr);
			swprintf(line, L"  %8s      %3d         %3d   %10.10s%10.10s",
			   st, rst[0], em.ctrlcode[i], as, aero);
			if (nr > 1) {
			   st[0] = 0;
			   for (r = 0; r < nr; r++)
				  swprintf(st+wcslen(st), L" %3d", rst[r]);
			   wmemcpy(line+11, st, wcslen(st));
			   }
			}
		 putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
		 endline(tulprm,0);
		 }
	  }
   else {
	  r = 0;
	  line[0] = 0;
	  ln1[0] = 0;
	  ln2[0] = 0;
	  for (i = 0; i < 50; i++) {
		 if (r != rt->rastiluku-1) {
			if (i && !em.ctrlcode[i] && !em.ctrltime[i]) break;
			aero[0] = 0;
			if (tulkinta[i] <= 0)
			   continue;
			swprintf(st, L"     %2d  ", tulkinta[i]);
			r = tulkinta[i] - 1;
			if (r >= 0 && r <= rt->rastiluku) {
			   r1 = 0;
			   while (r1 <= r) {
				  if (rt->rastikoodi[r1] > 9999 && rt->rastikoodi[r1] <= 10002) {
					 r++;
					 }
				  r1++;
				  }
			   }
			if (r >= rt->rastiluku) break;
			aero[0] = L' ';
			aikatowstr_ls(aero+1, 10L*(em.ctrltime[i]-enn-edaika),0);
			aero[9] = 0;
			elimwzb1(aero+1);
			edaika = em.ctrltime[i]-enn;
			if (em.ctrltime[i] - enn < 0) {
			   as[0] = L'-';
			   aikatowstr_ls(as+1, 10L*(enn-em.ctrltime[i]),0);
			   }
			else {
			   as[0] = L' ';
			   aikatowstr_ls(as+1, 10L*(em.ctrltime[i]-enn),0);
			   }
			as[9] = 0;
			}
		 else {
			if (!tls)
				break;
			if (tulprm->language > 0)
				wcscpy(st, L"  Finish ");
			else
				wcscpy(st, L"  Maali  ");
			aero[0] = L' ';
			aikatowstr_ls(aero+1, tls - 10L*edaika,0);
			aero[9] = 0;
			elimwzb1(aero+1);
			as[0] = L' ';
			aikatowstr_ls(as+1,tls,0);
			as[9] = 0;
			r++;
			}
		 if (line[0] && (r % 8 == 0)) {
			putfld(tulprm,line, 2*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
			endline(tulprm,0);
			putfld(tulprm,ln1, 2*tulprm->tulmuot.numlev, wcslen(ln1), 0, 0);
			endline(tulprm,0);
			putfld(tulprm,ln2, 2*tulprm->tulmuot.numlev, wcslen(ln2), 0, 0);
			endline(tulprm,0);
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
		 putfld(tulprm,line, 2*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
		 endline(tulprm,0);
		 putfld(tulprm,ln1, 2*tulprm->tulmuot.numlev, wcslen(ln1), 0, 0);
		 endline(tulprm,0);
		 putfld(tulprm,ln2, 2*tulprm->tulmuot.numlev, wcslen(ln2), 0, 0);
		 endline(tulprm,0);
		 }
	  }
   if (!kokosivu && emitosa < emitsivu) {
	  emitosa++;
	  newline(tulprm,3);
	  }
   else {
	  endpage(tulprm);
	  emitosa = 0;
	  }
   enddoc(tulprm);
   LeaveCriticalSection(&print_CriticalSection);
   }

void tark_rap(tulostusparamtp *tulprm, INT kilpno, INT os)
   {
#ifdef _CONSOLE
   wchar_t ch, *p, line[81], st[80], st1[80], trk[40], l1[66], l2[66], l3[66], cr = L'N';
   char cc;
   wchar_t viiva[61] =
	  L"____________________________________________________________";
   INT d, r;
   kilptietue kilp;

   if (tulprm == NULL)
	   return;

   if ((d = getpos(kilpno)) > 0) {
	  GETREC(&kilp, d);
	  }
   else
	  return;
   ch = L' ';
#ifdef PARTIO
   if (kilpparam.partio) {
      wselectopt(L"K)ortin tiedot, P)artio, L)uettelo tiedoista, T)iedostoon, H)ylk‰ysraportti",
         L"KLPHT\x1b", &ch);
      }
   else
#endif
	  wselectopt(L"K)ortin tiedot, L)uettelo tiedoista, T)iedostoon tulostus, H)ylk‰ysraportti",
         L"KLHT\x1b", &ch);
   clrln(ySize-3);
   switch (ch) {
	  case ESC :
		 return;
      case L'K' :
         if (esitark >= 0)
            emit_rap(tulprm, kilpno, os);
         return;
#ifdef PARTIO
      case L'P' :
         if (!esitark || esitark == 2) {
            wselectopt(L"N)‰ytˆlle, P)aperille", L"NP", &cr);
            tark_partio(tulprm, &kilp, cr == L'N' ? 2 : 1, 1);
            }
         return;
#endif
      case L'T' :
         if (!esitark || esitark == 2)
            kirjvaliajat(NULL);
         return;
      case L'L' :
         ch = L' ';
         wselectopt(L"L)ista luetuista korteista, H)yl‰tyist‰, E)sityksist‰",
            L"LHE\x1b", &ch);
		 clrln(ySize-3);
		 cc = (char) ch;
		 if (ch == ESC) return;
		 if (ch == L'L') emitlista(tulprm);
		 else tarkraportit(tulprm, cc == 'E' ? 'I' : cc);
		 return;
	  }
   for (r = 2; r < ySize-4; r++) clrtxt(r,0,59+5*status_on);
   memset(l1, 0, sizeof(l1));
   memset(l2, 0, sizeof(l2));
   memset(l3, 0, sizeof(l3));
	if (tulprm->language == 0)
	   swprintf(line, L"LEIMANTARKASTUSRAPORTTI");
	else
	   swprintf(line, L"LEIMANTARKASTUSRAPORTTI");
   vidspwmsg(2,0,7,0,line);
   vidint(4,0,4,kilpno);
   vidint(4,6,2,os+1);
   vidspwmsg(4,10,7,0, kilp.Joukkue(line));
   vidspmsg(5,10,7,0, (char *)Sarjat[kilp.sarja].sarjanimi);
   wcscpy(trk, L"Tarkastuksen tulos: ");
   switch (kilp.Tark(os, 0)) {
	  case L' ':
	  case 0 :
		 wcscat(trk, L"Tarkastamatta");
		 break;
	  case L'T' :
		 wcscat(trk, L"Hyv‰ksytty");
		 break;
	  case L'H' :
		 wcscat(trk, L"Hyl‰tty");
		 break;
	  case L'K' :
		 wcscat(trk, L"Keskeytt‰nyt");
		 break;
	  case L'P' :
		 wcscat(trk, L"Poissa");
		 break;
	  case L'I' :
		 wcscat(trk, L"Hylk‰ysesitys");
		 break;
	  }
   vidspwmsg(7,0,7,0,trk);
   vidspwmsg(9,0,7,0,L"Selitys :");
   vidspmsg(9,12,7,0,kilp.ostiet[os].selitys);
   vidspwmsg(11,0,7,0,L"Lis‰tietoja raporttiin (max 3 rivi‰)");
   inputwstr(l1, 65, 0, 13, L"\r", &ch, 0);
   inputwstr(l2, 65, 0, 14, L"\r", &ch, 0);
   inputwstr(l3, 65, 0, 15, L"\r", &ch, 0);
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
		if (tulprm->printer == GDIPRINTER)
			tulprm->tulmuot.numlev = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,1);
	  if (tulprm->language == 0)
		  swprintf(line, L"Tulostettu %s kello %s", paiva(), kello());
	  else
		  swprintf(line, L"Printed %s at %s", paiva(), kello());
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,1);
	  if (tulprm->language == 0)
		  swprintf(line, L"Joukkue ja osuus   %d -%d", kilpno, os+1);
	  else
		  swprintf(line, L"Team and leg       %d -%d", kilpno, os+1);
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,0);
	  swprintf(line, L"%-24.24s-%-2d  %s", kilp.Seura(st1), (int)kilp.joukkue,
		 kilp.Nimi(st, OSNIMIL, os));
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,0);
	  if (tulprm->language == 0)
		  swprintf(line, L"Sarja              %s", (char *)Sarjat[kilp.sarja].sarjanimi);
	  else
		  swprintf(line, L"Class              %s", (char *)Sarjat[kilp.sarja].sarjanimi);
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,1);
	  if (tulprm->language == 0)
		  swprintf(line, L"Tarkastuksen tulos %s", trk+20);
	  else
		  swprintf(line, L"Original outcome   %s", trk+20);
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,0);
	  if (tulprm->language == 0)
		  swprintf(line, L"Selitys (lyhyt)    %s", kilp.ostiet[os].selitys);
	  else
		  swprintf(line, L"Explanation        %s", kilp.ostiet[os].selitys);
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,1);
	  if (tulprm->language == 0)
		  swprintf(line, L"Lis‰tietoja");
	  else
		  swprintf(line, L"Additional");
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,0);
	  putfld(tulprm,l1, 4*tulprm->tulmuot.numlev, wcslen(l1), 0, 0);
	  endline(tulprm,0);
	  putfld(tulprm,l2, 4*tulprm->tulmuot.numlev, wcslen(l2), 0, 0);
	  endline(tulprm,0);
	  putfld(tulprm,l3, 4*tulprm->tulmuot.numlev, wcslen(l3), 0, 0);
	  endline(tulprm,2);
	  if (tulprm->language == 0)
		  swprintf(line, L"K‰sittelyvaiheet (ratkaisu, aika, k‰sittelij‰)");
	  else
		  swprintf(line, L"Processing steps (decision, time, official)");
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,2);
	  putfld(tulprm,viiva, 4*tulprm->tulmuot.numlev, wcslen(viiva), 0, 0);
	  endline(tulprm,0);
	  if (tulprm->language == 0)
		  swprintf(line, L"P‰‰tˆsesitys");
	  else
		  swprintf(line, L"Proposed decision");
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,5);
	  putfld(tulprm,viiva, 4*tulprm->tulmuot.numlev, wcslen(viiva), 0, 0);
	  endline(tulprm,0);
	  if (tulprm->language == 0)
		  swprintf(line, L"P‰‰tˆs");
	  else
		  swprintf(line, L"Decision");
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,5);
	  putfld(tulprm,viiva, 4*tulprm->tulmuot.numlev, wcslen(viiva), 0, 0);
	  endline(tulprm,0);
	  if (tulprm->language == 0)
		  swprintf(line, L"Vastalause");
	  else
		  swprintf(line, L"Protest");
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,5);
	  putfld(tulprm,viiva, 4*tulprm->tulmuot.numlev, wcslen(viiva), 0, 0);
	  endline(tulprm,0);
	  if (tulprm->language == 0)
		  swprintf(line, L"Lopullinen p‰‰tˆs");
	  else
		  swprintf(line, L"Final decision");
	  putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
	  endline(tulprm,5);
	  putfld(tulprm,viiva, 4*tulprm->tulmuot.numlev, wcslen(viiva), 0, 0);
	  endline(tulprm,0);
	  endpage(tulprm);
	  enddoc(tulprm);
	  LeaveCriticalSection(&print_CriticalSection);
	  }
   if (!ajat_on) status_on = 1;
#endif // _CONSOLE
   }

void tarkraportit(tulostusparamtp *tulprm, char trk)
   {
   wchar_t *pp, tc, ch = 0, che, co, line[81], st[40], st1[40];
   INT os = 0, r = 0, n = 0, srj = 0, vsrj = 0;
   kilptietue kilp;
   char cc;
   int p, lj;

   co = L' ';
   wselectopt(L"N)‰ytˆlle, P)aperille", L"NP", &co);
   switch (trk) {
      case L'H':
         wcscpy(st, L"Hyl‰tyt");
         break;
	  case L'I':
		 wcscpy(st, L"Hylk‰ysesitykset");
         break;
	  }
   ch = os + L'1';
   wselectopt(L"Anna osuus", L"123456789", &ch);
   os = ch - L'1';
   ch = 0;
   if (co == L'P') {
	   if (tulprm->lstf == NULL)
		   return;
	   r = 32767;
	  EnterCriticalSection(&print_CriticalSection);
	  }
   do {
      if( srj < vsrj ) srj++;
      else {
         clrln(ySize-3);
		 vidspwmsg(ySize-3,0,7,0,L"Ensimm‰inen sarja :               "
			L"<F2>: Alusta, <Esc>: Poistu");
		 srj = luesarja("", &cc);
		 if (cc == ESC) break;
		 clrln(ySize-3);
		 vidspwmsg(ySize-3,0,7,0,L"Viimeinen sarja :               "
			L"<Esc>: Poistu");
		 vsrj = luesarja((char *)Sarjat[srj].sarjanimi, &cc);
		 tc = cc;
		 clrln(ySize-1);
		 if (tc == 202) vsrj = sarjaluku - 1;
		 if( tc == ESC ) break;
		 }
	  if (co == L'N') {
		 for (r = 2; r < ySize-4; r++) clrtxt(r,0,59+5*status_on);
		 clrln(ySize-3);
		 r = 2;
		 }
	  else {
		 r = 0;
		  if (tulprm->language == 0)
			 swprintf(line, L"%-15.15s  %-16.16s   Tulostettu %s kello %s",
				ansitowcs(st1, Sarjat[srj].sarjanimi, 20), st, wpaiva(), wkello());
		  else
			  swprintf(line, L"%-15.15s  %-16.16s   Printed %s at %s",
				ansitowcs(st1, Sarjat[srj].sarjanimi, 20), st, wpaiva(), wkello());
			pp = 0;
			if (tulprm->printer == LJETII)
				pp = initfont_emrap;
			if (tulprm->printer == GDIPRINTER)
				pp = L"C";
		   initpage(tulprm, pp);
			initline(tulprm);
			if (tulprm->printer == GDIPRINTER)
				tulprm->tulmuot.numlev = tulprm->lstf->u.wp.GDIparam.Currentfont.numwidth;
		 initline(0);
		 putfld(tulprm,line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
		 endline(tulprm,1);
		 r += 2;
		 }
	  switch (trk) {
		 case L'K' :
			lj = 0;
			break;
		 case L'E' :
		 case L'S' :
			trk = L'E';
			lj = 2;
			break;
		 case L'H' :
			lj = 1;
			break;
		 default:
			lj = -1;
		 }
	  EnterCriticalSection(&tall_CriticalSection);
	  p = -1;
	  while (1) {
		  p = askellasarja(srj, os, 0, 0, p, &lj);
		  if (p < 0)
			  break;
		 GETREC(&kilp, p);
		 if (trk != L'K' && kilp.Tark(os, 0) == L'K') break;
		 if (trk != L'H' && kilp.Tark(os, 0) == L'H') break;
		 if (trk == L'H' && kilp.Tark(os, 0) != L'H') break;
		 if (trk == L'E' && kilp.Tark(os, 0) != L'E') break;
		 if (kilp.Tark(os, 0) != trk) continue;
		 if (co == L'N') {
			if (r > ySize-5) {
			   vidspwmsg(ySize-3,0,0,7,
				  L"Paina n‰pp‰int‰ jatkaaksesi, Esc : Poistu");
			   LeaveCriticalSection(&tall_CriticalSection);
			   ch = readkbd_w(&che,1);
               EnterCriticalSection(&tall_CriticalSection);
			   if (ch == ESC) break;
               clrln(ySize-3);
               for (r = 2; r < ySize-4; r++) clrtxt(r,0,59+5*status_on);
               r = 2;
               }
			}
         else {
			if (!mahtuusivulle(r, 1, tulprm)) {
			   endpage(tulprm);
				  initpage(tulprm, pp);
			   initline(tulprm);
			   Sarjat[srj].Sarjanimi(st1);
			   putfld(tulprm, st1, 4*tulprm->tulmuot.numlev, wcslen(st1), 0, 0);
			   endline(tulprm,1);
			   r = 2;
			   }
			}
		 if (co == L'N') {
			swprintf(line, L"%4d-%-2d %-22.22s %-10.10s  %c %-20.20s",
			   kilp.kilpno, os+1, kilp.Joukkue(st, 22), Sarjat[srj].Sarjanimi(),
			   kilp.wTark(os, 0), kilp.Selitys(os));
			vidspwmsg(r, 0, 7, 0, line);
            }
         else {
			swprintf(line, L"%4d-%-2d %-26.26s %-15.15s  %c %-20.20s",
               kilp.kilpno, os+1, kilp.Joukkue(st, 26), Sarjat[srj].Sarjanimi(),
			   kilp.Tark(os, 0), kilp.ostiet[os].selitys);
			putfld(tulprm, line, 4*tulprm->tulmuot.numlev, wcslen(line), 0, 0);
			endline(tulprm,0);
            }
         r++;
         n++;
		 }
      LeaveCriticalSection(&tall_CriticalSection);
	  if (co == L'P')
			endpage(tulprm);
	  if (co == L'N') {
		 if (n > 0 && ch != ESC) {
			vidspwmsg(ySize-3,0,0,7, L"Paina n‰pp‰int‰ jatkaaksesi");
			ch = readkbd_w(&che,1);
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
	  swprintf(line, L"Tulostettu %d kilpailijaa     Paina n‰pp‰int‰", n);
	  vidspwmsg(ySize-3,0,7,0,line);
	  readkbd_w(&che,1);
	  clrln(ySize-3);
      }
   clrln(ySize-3);
   }

static void writevafile(TextFl *vafile)
   {
   wchar_t as[20], ast[20], ase[20], as2[20], line[400], st[80];
   emittp em;
   kilptietue kilp;
   INT d, i, rastiluku, tulkinta[51];
   INT va[51], oikva[51];
   INT32 enn, tl;
   ratatp *rt;

   vafile->WriteLine(L"Klpno Os Seura                       Jk Sarja      Tulos      T"
	  L" NR Lahto      Maalikello"
	  L" Luku        Enn   Aika1  Aika2  Aika3  Aika4  Aika5  Aika6  Aika7  Aika8  "
	  L"Aika9  Aik10  Aik11  Aik12  Aik13  Aik14  Aik15  Aik16  Aik17  Aik18  Aik19  "
	  L"Aik20  Aik21  Aik22  Aik23  Aik24  Aik25  Aik26  Aik27  Aik28  Aik29  Aik30  "
	  L"Aik31  Aik32  Aik33  Aik34  Aik35  Aik36  Aik37  Aik38  Aik39  Aik40\n");
   _lseek(emitfile, 0, SEEK_SET);
   while (1) {
      if (_read(emitfile, &em, sizeof(emittp)) < sizeof(emittp))
		 break;
	  if (em.kilpno <= 0 || (d = getpos(em.kilpno)) < 0)
         continue;
      GETREC(&kilp, d);
	  if (tarkista(&em, &kilp, tulkinta, -1, 0) < 0)
		  continue;
      rt = getrata(kilp.ostiet[em.osuus].hajonta);
      if (rt && (rastiluku = rt->rastiluku) != 0)
         enn = haeennakko(&em, &kilp, em.osuus);
	  memset(va, 0, sizeof(va));
      memset(oikva, 0, sizeof(oikva));
      if (em.maali != TMAALI0/KSEK) 
		  aikatowstr_ls(as, em.maali, t0);
      else 
		  wcscpy(as, L"00.00.00,0");
	  if (em.time != TMAALI0/KSEK)
		  aikatowstr_ls(ase, em.time, t0);
	  else
		  wcscpy(ase, L"00.00.00,0");
	  AIKATOWSTRS(ast, kilp.osTulos(em.osuus, 0, false), 0);
	  ast[10] = 0;
      tl = osuuslahto(&kilp, em.osuus);
/*
	  tl = Sarjat[kilp.sarja].lahto;
      if (em.osuus) {
         if (kilp.ostiet[em.osuus].ylahto != TMAALI0)
            tl = kilp.ostiet[em.osuus].ylahto;
         else {
            if (kilp.Maali(em.osuus-1, 0) != TMAALI0)
               tl = kilp.Maali(em.osuus-1, 0);
			}
		 }
*/
      swprintf(line, L"%05.5d %02.2d %-27.27s %2.2d %-10.10s %s %c %2.2d %10.10s %s"
		 L" %s %5ld", em.kilpno, em.osuus+1, kilp.Seura(st), (int)kilp.joukkue,
         Sarjat[kilp.sarja].Sarjanimi(),
		 ast, kilp.Tark(em.osuus, 0), rastiluku,
		 AIKATOWSTRS(as2, tl, t0), as, ase, enn);
      for (i = 0; i < 50; i++) {
         if (i && !em.ctrlcode[i] && !em.ctrltime[i]) break;
         if (tulkinta[i] < 0 && !oikva[-tulkinta[i]-1]) va[-tulkinta[i]-1] = (INT) (em.ctrltime[i] - enn);
		 else if (tulkinta[i] > 0) {
			va[tulkinta[i]-1] = (INT) (em.ctrltime[i] - enn);
			oikva[tulkinta[i]-1] = 1;
			}
		 }
	  for (i = 0; i < 40; i++) {
		 swprintf(line+wcslen(line), L" %6.5d", va[i]);
		 }
	  wcscat(line, L"\n");
	  vafile->WriteLine(line);
	  }
   }

void kirjvaliajat(wchar_t *flnm)
   {
   TextFl *vafile;
   wchar_t ch = L'T';
   INT xml = 0;

   if (esitark < 0) return;
#ifdef _CONSOLE
   ch = 0;
   wselectopt(L"X)ML vai T)ekstitiedosto", L"XT", &ch);
#endif
   if (ch == L'X') {
	  if (!flnm)
		  flnm = L"EMITAJAT.XML";
	  xml = 1;
	  }
   else {
	  if (!flnm)
		  flnm = L"EMITAJAT.LST";
	  xml = 0;
	  }
   vafile = new TextFl(flnm, L"wb");
   if (!vafile->IsOpen()) {
	  delete vafile;
	  writeerror_w(L"Virhe v‰liaikatiedostoa avattaessa", 0, true);
	  return;
	  }
   if (!xml) {
	  writevafile(vafile);
	  }
   else {
//	  writexml(vafile, 1, 0);
	  }
   delete vafile;
   }

int tee_emva(emitvatp *emva, emittp *em)
	{
	kilptietue kilp;
	int d, i, l, r, rastiluku, tulkinta[51];
   UINT va[51];
   INT32 enn;
   ratatp *rt;

	if (em->kilpno <= 0 || (d = getpos(em->kilpno)) < 0)
		return(1);
   GETREC(&kilp, d);
   if (tarkista(em, &kilp, tulkinta, -1, 0) < 0)
		return(1);
   memset(va, 0, sizeof(va));
   memset(emva, 0, sizeof(emitvatp));
	rt = getrata(kilp.ostiet[em->osuus].hajonta);
   rastiluku = 0;
   if (rt && (rastiluku = rt->rastiluku) != 0)
		enn = haeennakko(em, &kilp, em->osuus);
   else
	  enn = 0;
	emva->badge = em->badge;
	emva->kilpno = em->kilpno;
	emva->osuus = em->osuus;
	emva->tulos = osuustulos(&kilp, em->osuus, 0);
	emva->ok = kilp.wTark(em->osuus, 0) == L'T' ? 0 : kilp.wTark(em->osuus, 0);
	emva->rastiluku = rt->rastiluku - (rt->maalilaji > 1 ? 1 : 0);
	strncpy(emva->rata, kilp.ostiet[em->osuus].hajonta, sizeof(emva->rata)-1);
	l = haelukija(em);
	if (l > 49)
		return(1);
	for (i = 1; i < 50; i++) {
		if ((r = tulkinta[(l+i)%50]) != 0) {
			r = abs(r);
			if (r > rt->rastiluku || r > MAXEMVALAH)
				continue;
			if (tulkinta[(l+i)%50] > 0 || !emva->rastit[r-1][0]) {
				emva->rastit[r-1][0] = rt->rastikoodi[r-1];
				emva->rastit[r-1][1] = em->ctrltime[(l+i)%50]-enn;
				}
			}
		}
	return(0);

	}

void kirjemvaloki(combufrec *ibuf)
{
   wchar_t msg[400], st[40];
   emitvatp *emva;

   emva = &ibuf->d.emva;
   swprintf(msg, L"%d-%d: Bdg %d, Aika %d, Trk %d, Rata %s, Rasteja %d",
	  emva->kilpno, emva->osuus+1, emva->badge, emva->tulos/SEK, emva->ok,
		ansitowcs(st, emva->rata, 39), emva->rastiluku);
   wkirjloki(msg);
   msg[0] = 0;
   for (int i = 0; i < emva->rastiluku && i < MAXEMVALAH; i++) {
	   swprintf(msg+wcslen(msg), L"%d %d ", emva->rastit[i][0], emva->rastit[i][1]);
	   }
   wkirjloki(msg);
}

#endif // MAALI

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
#ifndef _CONSOLE
	   if (rvalit) {
		   for (int d = 0; d < rvluku; d++) {
			  for (int srj = 0; srj < sarjaluku; srj++) {
				  for (int os = 0; os < Sarjat[srj].ntosuus; os++) {
					 if (rvalit[d].ajat[srj][os])
						free(rvalit[d].ajat[srj][os]);
					 if (rvalit[d].kilpno[srj][os])
						free(rvalit[d].kilpno[srj][os]);
					 }
				 }
			  }
		  free(rvalit);
		  }
	   rvalit = 0;
	   if (rastiva)
		   delete[] rastiva;
	   rastiva = NULL;
#endif
	   if (vapautarata) {
		   if (rata) {
			  free(rata);
			  }
		   rata = 0;
		   nrata = 0;
		   }
	   }
   }

void lueRatatiedotUudelleen(void)
{
   EnterCriticalSection(&emit_CriticalSection);
   EnterCriticalSection(&rastiva_CriticalSection);
   vapautaemitmuisti(true, true, 0, true);
   if (ratatiedot == 2) {
		CourseData.nollaa();
		lue_radat_xml(19);
		}
   else {
		lue_leimat(19);
		lue_radat(20);
		}
   LeaveCriticalSection(&rastiva_CriticalSection);
   LeaveCriticalSection(&emit_CriticalSection);
}

void uusiemitanalyysi(bool uusi)
{
#ifndef _CONSOLE
   if (!emitfl || (!emitanal_fl && !uusi))
		return;
   EnterCriticalSection(&rastiva_CriticalSection);
   if (emitanal_fl)
	   vapautaemitmuisti(false, false, false, true);
   emitanal_fl = true;
   tarkkilpradat();
   TarkKilpailijat();
   laskevertajat();
   for (int i = 0; i < emithead2; i++) {
		emittp em;
		em.get(i);
		if (em.badge && em.kilpno && bdg2kno(em.badge, 0) == em.kilpno)
			addemitanal(&em, 0);
		}
   LeaveCriticalSection(&rastiva_CriticalSection);
#endif
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
void laskeEm_VA(int d, int osuus, bool sail)
{
	emittp em;
	int enn, tulkinta[MAXNLEIMA+1];
	bool muutos = false;
	kilptietue kilp;

	if (d < 1 || d >= (int) datf2.numrec)
		return;
	kilp.getrec(d);
	if (kilp.kilpstatus)
		return;
	if (em.getbykno(kilp.kilpno, osuus) < 0)
		return;
	if (tarkista(&em, &kilp, tulkinta, -1, 0) < 0)
		return;
	enn = haeennakko(&em, &kilp, osuus);
	for (int i = 0; i <= Sarjat[kilp.sarja].valuku[osuus]; i++) {
		int ta, tl;
		if (Sarjat[kilp.sarja].mtb[osuus][i].alkukdi + Sarjat[kilp.sarja].mtb[osuus][i].loppukdi == 0)
			continue;
		if (sail && kilp.Maali(osuus, Sarjat[kilp.sarja].mtb[osuus][i].piste) != TMAALI0)
			continue;
		if (Sarjat[kilp.sarja].mtb[osuus][i].alkukdi == 0)
			ta = 0;
		else
			ta = SEK * rastiVA(Sarjat[kilp.sarja].mtb[osuus][i].alkukdi, tulkinta, &em, enn);
		if (Sarjat[kilp.sarja].mtb[osuus][i].loppukdi == 0)
			tl = kilp.Maali(osuus,0);
		else {
			tl = SEK * rastiVA(Sarjat[kilp.sarja].mtb[osuus][i].loppukdi, tulkinta, &em, enn);
			if (tl == 0)
				tl = 5 * TUNTI;
			}
		if (Sarjat[kilp.sarja].mtb[osuus][i].pohja != 0)
			tl += kilp.Tulos(osuus, Sarjat[kilp.sarja].mtb[osuus][i].pohja);
		kilp.setMaali(osuus, Sarjat[kilp.sarja].mtb[osuus][i].piste, tl-ta+kilp.Lahto(osuus));
		muutos = true;
		}
	if (muutos) {
		tallenna(&kilp, d, 0, 0, 0, 0);
	}
}

