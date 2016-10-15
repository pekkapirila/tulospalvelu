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
#include <Clipbrd.hpp>
#include <process.h>
#pragma hdrstop

#include "TulosUnit.h"
#include "WinHk.h"
#include "HkDeclare.h"
#include "UnitValSarjat.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTilanneForm *TilanneForm;
extern TTilanneForm *TilFrm[];

//---------------------------------------------------------------------------
__fastcall TTilanneForm::TTilanneForm(TComponent* Owner)
   : TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	NaytaPiste[0] = Maali1;
	NaytaPiste[1] = N1vliaika1;
	NaytaPiste[2] = N2vliaika1;
	NaytaPiste[3] = N3vliaika1;
	NaytaPiste[4] = N4vliaika1;
	NaytaPiste[5] = N5vliaika1;
	NaytaPiste[6] = N6vliaika1;
	NaytaPiste[7] = N7vliaika1;
	NaytaPiste[8] = N8vliaka1;
	NaytaPiste[9] = N9vliaika1;
	NaytaPiste[10] = N10vliaikaA1;
	NaytaPiste[11] = N11vliaikaB1;
	NaytaPiste[12] = N12vliaikaC1;
	NaytaPiste[13] = N13vliaikaD1;
	NaytaPiste[14] = N14vliaikaE1;
	NaytaPiste[15] = N15vliaikaF1;
	NaytaPiste[16] = N16vliaikaG1;
	NaytaPiste[17] = N17vliaikaH1;
	NaytaPiste[18] = N18vliaikaI1;
	NaytaPiste[19] = N19vliaikaJ1;
	NaytaPiste[20] = N20vliaikaK1;
	NaytaPiste[21] = N21vliaikaL1;
	NaytaPiste[22] = N22vliaikaM1;
	NaytaPiste[23] = N23vliaikaN1;
	NaytaPiste[24] = N24vliaikaO1;
	NaytaPiste[25] = N25vliaikaP1;
	NaytaPiste[26] = N26vliaikaQ1;
	NaytaPiste[27] = N27vliaikaR1;
	NaytaPiste[28] = N28vliaikaS1;
	NaytaPiste[29] = N29vliaikaT1;
	NaytaPiste[30] = N30vliaikaU1;
	InitializeCriticalSection(&tls_CriticalSection);
}
//---------------------------------------------------------------------------
__fastcall TTilanneForm::~TTilanneForm(void)
{
	if (Cells) {
		delete[] Cells;
		Cells = NULL;
		nRivi = 0;
		}
	if (sjjarj)
		delete[] sjjarj;
	sjjarj = NULL;
   DeleteCriticalSection(&tls_CriticalSection);
}
//---------------------------------------------------------------------------
void __fastcall TTilanneForm::FormCreate(TObject *Sender)
{
	CNro = 1;
	CNimi = 2;
	CViimeisin = 0;
	CAlisarja = 0;
	CSeura = 3;
	CMaa = 4;
	CTulos = 5;
	CEro = 6;
	IkkParam = seurIkkParam0;
	IkkParam.laika = pvparam[k_pv].laika2;
	N1sek->Checked = IkkParam.laika <= 9;
	N01sek->Checked = IkkParam.laika == 10;
	N001sek->Checked = IkkParam.laika == 11;
	N001sek->Checked = IkkParam.laika >= 12;
	Pieni1->Checked = false;
	Normaali1->Checked = false;
	Suuri1->Checked = false;
	Muumerkkikoko1->Checked = false;
	switch (IkkParam.FontSize) {
		case 8:
			Pieni1->Checked = true;
			break;
		case 10:
			Normaali1->Checked = true;
			break;
		case 12:
			Suuri1->Checked = true;
			break;
		default:
			Muumerkkikoko1->Checked = true;
		}
	Yhteistulos1->Checked = (k_pv > 0);
	Yhteistulos1->Visible = (k_pv > 0);
	memset(&IkkParamEd, 0, sizeof(IkkParamEd));
	InitColors();
}
//---------------------------------------------------------------------------
seurIkkParamClass::seurIkkParamClass(void)
{
	memset(this, 0, sizeof(seurIkkParamClass));
	vainjarj = true;
	seuraTieto = 1;
	FontSize = 10;
	korRaja[0] = MINUUTTI;
	korRaja[1] = 3*MINUUTTI;
	ScrollVali = 2000;
	ScrollRiviKesto = 300;
	ScrollType = 1;
	eroaika = true;
	odRaja = 3 * MINUUTTI;
	ColW[0] = 36;		// Sija
	ColW[1] = 36;		// KilpNo
	ColW[2] = 200;		// Nimi
	ColW[3] = 120;		// Viimeisin
	ColW[4] = 200;		// Seura
	ColW[5] = 100;		// Seuralyh
	ColW[6] = 120;		// Joukkue
	ColW[7] = 40;		// Alisarja
	ColW[8] = 40;		// Maa
	ColW[9] = 65;		// Tulos
	ColW[10] = 65;		// Ero
	ColW[11] = 80;		// Lähtö
	ColW[12] = 80;		// Tulos
	ColW[13] = 45;		// Sakot
	ColW[14] = 45;		// Kierr.
	ColW[15] = 65;		// Y-tulos
	ColW[16] = 65;		// Y-ero
	ColW[17] = 36;		// Y-sija
	ColW[18] = 55;		// Ennätys
	ColW[19] = 55;		// Tavoite
}

//---------------------------------------------------------------------------
void TTilanneForm::InitColors(void)
{
	ColorSet[0].FirstRowBackColor = clBtnFace;
	ColorSet[0].FirstColBackColor = clBtnFace;
	ColorSet[0].WinBackColor = clWindow;
	ColorSet[0].AltBackColor = TColor(0xffffd0);
	ColorSet[0].korBG1 = korVari[0];
	ColorSet[0].korBG2 = korVari[1];
	ColorSet[0].FirstRowColor = clBlack;
	ColorSet[0].FontColor = clBlack;
	ColorSet[0].FontColor2 = clGreen;
	ColorSet[0].FontColor3 = clRed;

	ColorSet[1].FirstRowBackColor = TColor(0x303030);
	ColorSet[1].FirstColBackColor = clBlack;
	ColorSet[1].WinBackColor = clBlack;
	ColorSet[1].AltBackColor = clBlack;
	ColorSet[1].korBG1 = clBlack;
	ColorSet[1].korBG2 = clBlack;
	ColorSet[1].FirstRowColor = clWhite;
	ColorSet[1].FontColor = clYellow;
	ColorSet[1].FontColor2 = clMoneyGreen;
	ColorSet[1].FontColor3 = TColor(0xffc0ff);

	ColorSet[2] = ColorSet[0];
}
//---------------------------------------------------------------------------
void seurIkkParamClass::operator=(seurIkkParamClass& ikkPar)
{
	memcpy(this, &ikkPar, sizeof(seurIkkParamClass));
}
//---------------------------------------------------------------------------
bool seurIkkParamClass::operator==(seurIkkParamClass& ikkPar)
{
	if (memcmp(this, &ikkPar, sizeof(seurIkkParamClass)) == 0)
		return(true);
	return(false);
}
//---------------------------------------------------------------------------
bool seurIkkParamClass::operator!=(seurIkkParamClass& ikkPar)
{
	return(!(*this == ikkPar));
}
//---------------------------------------------------------------------------
int TTilanneForm::applyParams(void)
{
	N1sek->Checked = IkkParam.laika <= 9;
	N01sek->Checked = IkkParam.laika == 10;
	N001sek->Checked = IkkParam.laika == 11;
	N001sek->Checked = IkkParam.laika >= 12;
	switch (IkkParam.FontSize) {
		case 8:
			Pieni1->Checked = true;
			break;
		case 10:
			Normaali1->Checked = true;
			break;
		case 12:
			Suuri1->Checked = true;
			break;
		default:
			Muumerkkikoko1->Checked = true;
		}
	PisteVal->ItemIndex = IkkParam.Va + 1;
	Maatunnus1->Checked = IkkParam.naytaMaa;
	HylVal->Checked = IkkParam.naytaHyl;
	Seuraalhtj1->Checked = IkkParam.seuraaLahtoja;
	switch (IkkParam.minTaso) {
		case 1:
			aso11Click(this);
			break;
		case 2:
			aso21Click(this);
			break;
		case 3:
			aso31Click(this);
			break;
		case 9:
			Vainjulkiset1Click(this);
			break;
		}
	Naytapiilotetut->Checked = IkkParam.naytaPiilotetut;
	Nytodotetut->Checked = IkkParam.naytaOdotetut;
	Omaenntys1->Checked = IkkParam.Ennatys;
	avoiteaika1->Checked = IkkParam.Tavoite;
	if (IkkParam.ScrollOn) {
		IkkParam.ScrollOn = false;
		Skrollaa1Click(this);
		}
	memcpy(ColorSet[2].Colors, IkkParam.Colors, sizeof(ColorSet[2].Colors));
	return(0);
}
//---------------------------------------------------------------------------
unsigned short uswab(unsigned short k)
{
	return(k/256 + 256*(k%256));
}
//---------------------------------------------------------------------------
int __fastcall TTilanneForm::SmartJarj(int rv, int *nRv)
{
	int p = -1, lj = -1, n, nva, aktva[VALUKU+1];
	kilptietue kilp;

	if (rv >= nilm[IkkParam.Sarja])
		return(-1);
	if (rv == 0) {
		if (sjjarj)
			delete[] sjjarj;
		sjjarj = new sjjarjtp[nilm[IkkParam.Sarja]];
		for (int i = 0; i < nilm[IkkParam.Sarja]; i++)
			memset(&sjjarj[i], 0, sizeof(sjjarj[0]));
		nva = 0;
		memset(aktva, 0, sizeof(aktva));
		memset(vavali, 0, sizeof(vavali));
		for (int i = 0; i < npiste; i++) {
			if (naytapiste[i] > 0)
				aktva[nva++] = naytapiste[i];
			}

		// Anna ensin jokaiselle kilpailijalle
		for (n = 0; n  < nilm[IkkParam.Sarja]; n++) {
			while (p < 0 && lj < 4) {
				if (lj == 2)
					lj++;
				p = jalku[IkkParam.Sarja][++lj];
				}
			if (p < 0)
				break;
			kilp.GETREC(p);
			sjjarj[n].d = p;
			if (!kilp.hyv()) {
				sjjarj[n].sijat[0] = 65535U;
				}
			else {
				bool on_va = false;
				for (int iva = 0; iva < nva; iva++) {
					int jva = aktva[nva - iva - 1] - 1;
					int vv = 0, vv1 = 0;
					if (Sarjat[IkkParam.Sarja].luonne[k_pv] == 2 && jva == 0)
						jva = kilpparam.valuku+1;
					if ((vv = kilp.tulos2(jva)) > 0) {
						sjjarj[n].sijat[iva] = uswab((unsigned short)kilp.p_sija(jva, IkkParam.Sarja));
						on_va = true;
						if (iva < nva-1) {
							vv1 = kilp.tulos2(aktva[nva - iva - 2] - 1);
							if (vv1)
								vv = vv - vv1;
							else
								vv = 0;
							}
						if (vv && (vavali[nva-iva-1][2] == 0 || vv < vavali[nva-iva-1][2])) {
							int i;
							for (i = 1; i >= 0 && (vavali[nva-iva-1][i] == 0 || vv < vavali[nva-iva-1][i]); i--) {
								vavali[nva-iva-1][i+1] = vavali[nva-iva-1][i];
								}
							vavali[nva-iva-1][i+1] = vv;
							}
						}
					else {
						sjjarj[n].sijat[iva] = uswab(65279U);
						}
					}
				if (!on_va) {
					if (Sarjat[IkkParam.Sarja].lahtovali[k_pv] != 0) {
						sjjarj[n].sijat[nva] = uswab(65279U -
							(((Sarjat[IkkParam.Sarja].enslahto[k_pv]+28*TUNTI-kilp.TLahto(k_pv))%(24*TUNTI))/SEK));
						}
					else
						sjjarj[n].sijat[nva] = uswab(65279U - maxkilpno + kilp.id());
					}
				}
			p = SeuraavaSrj(IkkParam.Sarja, 0, p);
			}
		if (n > 1) {
			int j, nsij;
			sjjarjtp tsjjarj;

			quicksort((char *)sjjarj, n, sizeof(sjjarj[0])-4, sizeof(sjjarj[0]));
			nsij = ntulos[IkkParam.Sarja][0];
			for (int iva = 1; iva <= nva; iva++) {
				int jpuutt;
				for (int k = nsij; k < n; k++) {
					if (sjjarj[k].sijat[0] == 65535U ||	uswab(sjjarj[k].sijat[iva]) > 65000U)
						break;
					else {
						jpuutt = 0;
						for (j = 0; j < nsij; j++) {
							if (uswab(sjjarj[j].sijat[iva]) > 65000U) {
								if (jpuutt == 0)
									jpuutt = j;
								}
							else {
								jpuutt = 0;
								if (uswab(sjjarj[j].sijat[iva]) > uswab(sjjarj[k].sijat[iva]))
									break;
								}
							}
						if (jpuutt)
							j = jpuutt;
//						if (j < nsij)
//							break;
						}
					if (j < nsij) {
						tsjjarj = sjjarj[k];
						memcpy(tsjjarj.sijat, sjjarj[j].sijat, iva*sizeof(short));
						memmove(sjjarj+j+1, sjjarj+j, (k-j)*sizeof(sjjarj[0]));
						sjjarj[j] = tsjjarj;
						}
					nsij++;
					}
				if (vavali[iva-1][2] && (4*vavali[iva-1][0] < 3*vavali[iva-1][1]))
					vavali[iva-1][0] = vavali[iva-1][1];
				}
			}
		if (nRv)
			*nRv = n;
		}
	return(sjjarj[rv].d);
}
//---------------------------------------------------------------------------

bool __fastcall TTilanneForm::OnAlisarja(kilptietue *kilp)
{
	if (Rajaukset->Checked == false)
		return(true);
	if (EdtSp->Text != NULL && EdtSp->Text.Length() > 0 && kilp->sukup != towupper(EdtSp->Text.c_str()[0]))
		return(false);
	if (EdtIkasarja->Text != NULL && EdtIkasarja->Text.Length() > 0) {
		wchar_t *p, asStr[20];
		wcscpy(asStr, EdtIkasarja->Text.UpperCase().SubString(1, 19).c_str());
		p = wcstok(asStr, L" ,;");
		while (p) {
			switch (p[0]) {
				case L'Y':
					if (kilp->ikasarja == 1)
						return(true);
					break;
				case L'A':
					if (kilp->ikasarja == 2)
						return(true);
					break;
				default:
					if (kilp->ikasarja == _wtoi(p))
						return(true);
				}
			p = wcstok(NULL, L" ,;");
			}
		return(false);
		}
	return(true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::PaivBtnClick(TObject *Sender)
{
	FormSetup(true, false);
}
//---------------------------------------------------------------------------

//int teeYPisteJarj(int srj, int (*ypJrj)[4], int tallenna)
int teeYPisteJarj(int srj, int (**ypJrj)[4])
{
	int p = -1, lj = -1, karki = 0x3ffffff, n = 0, sj = 1;
	kilptietue kilp;

	if (*ypJrj)
		free(*ypJrj);
	*ypJrj = (int (*)[4]) calloc(nilm[srj]+1, sizeof((*ypJrj)[0]));
	for (int i = 0; i < nilm[srj]; i++) {
		p = askellasarja(srj, 0, p, &lj);
		if (p <= 0)
			break;
		kilp.GETREC(p);
		(*ypJrj)[n][1] = p;
		if (i == 0)
			karki = kilp.tulos_pv(k_pv, true, 2);
		kilp.pv[k_pv].pvpisteet[0] = 0;
		if (kilp.hyv() && kilp.tulos_pv(k_pv, true, 2))
			kilp.pv[k_pv].pvpisteet[0] = Pisteet(&Sarjat[srj].pistekaava[k_pv], kilp.p_sija(0), ntulos[srj][0],
				kilp.tulos_pv(k_pv, true, 2), karki);
		LaskeKilpYhtPisteet(&kilp, &Sarjat[srj].pistekaava[k_pv], srj);
//		if (tallenna)
//			kilp.PUTREC(p, tallenna > 1);                             0
		if (Sarjat[srj].luonne[k_pv] == 2) {
			(*ypJrj)[n][0] = kilp.ytulos(k_pv);
			}
		else {
			(*ypJrj)[n][0] = kilp.pisteet[0];
			if (Sarjat[srj].pistekaava[k_pv].kerr > 0)
				(*ypJrj)[n][0] = PISTEKATTO - (*ypJrj)[n][0];
			}
		(*ypJrj)[n][3] = kilp.pv[k_pv].pvpisteet[0];
		if ((*ypJrj)[n][0] != 0)
			n++;
		}
	qsortint(*ypJrj, n, sizeof((*ypJrj)[0]), 0, 1);
	for (int i = 0; i < n; i++) {
		if (i > 0 && (*ypJrj)[i][0] != (*ypJrj)[i-1][0])
			sj = i+1;
		if (Sarjat[srj].luonne[k_pv] != 2 && Sarjat[srj].pistekaava[k_pv].kerr > 0)
			(*ypJrj)[i][0] = PISTEKATTO - (*ypJrj)[i][0];
		(*ypJrj)[i][2] = sj;
		}
	return((*ypJrj)[0][0]);
}
//---------------------------------------------------------------------------

int YhtSija(int pst, int srj, int (*ypJrj)[4])
{
	for (int i = 0; i < nilm[srj]; i++)
		if (Sarjat[srj].pistekaava[k_pv].kerr * (pst - ypJrj[i][0]) >= 0)
			return(i+1);
	return(0);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::PaivitaGrid(bool scroll)
{
	int nrivi, nTls, tKarki, pKarki = 0, (*ypJrj)[4] = 0, Sarja, Va;
//   int d, k, edT = 0, t, sj, karki, ykarki, KH = 0;
	int d, k, edT = 0, t, sj, sj1, KH = 0;
	wchar_t ln[80], ln2[20];
	int p, p1, alkuVa, Va1;
	kilptietue kilp;
	int peruutafl = 1;

	if (IkkParam.ScrollOn && !scroll)
		return;
	EnterCriticalSection(&tls_CriticalSection);
	if (!paivitysflag) {
		paivitysflag = 1;
		peruutafl = 0;
		}
	TulosGrid->Color = (TColor) ColorSet[IkkParam.AktColor].WinBackColor;
	LeaveCriticalSection(&tls_CriticalSection);
	if (peruutafl)
		return;
//   Seuraa = 0;
	Sarja = IkkParam.Sarja;
	if (Sarja >= sarjaluku) {
		Yhd = Sarja - sarjaluku + 1;
		}
	else
		Yhd = 0;
	Va = PisteVal->ItemIndex-1;
	if (IkkParam.EraVal < 0)
		IkkParam.Era = era_akt;
	swprintf(Kohde, L"%.12s-%.12s", Sarjat[Sarja].sarjanimi, PisteVal->Text.c_str());
	FormMain->SetFormKohde(this, Kohde);
	if (Va > 0) {
		alkuVa = Va+4;
		LA = 0;
		Caption = UnicodeString(Sarjat[Sarja].sarjanimi) +
			(Va == YTulVal-1 ? UnicodeString(L" - Yhteistulokset") :
			(UnicodeString(L" - ") + Va + L". väliaika " + Sarjat[Sarja].va_matka[k_pv][Va-1] + L" km"));
		}
	else if (Va < 0) {
//		alkuVa = kilpparam.valuku + 5 + 5 * (k_pv > 0);
//		Va = kilpparam.valuku + 1 + (k_pv > 0);
		alkuVa = kilpparam.valuku + 10;
		Va = kilpparam.valuku + 2;
		edEnn = IkkParam.AaniLkm*SEK;
		LA = 1;
		Caption = UnicodeString(Sarjat[Sarja].sarjanimi) + L" - Lähtöajat";
		if (IkkParam.vainjarj)
			OtsCells[CTulos].text = UnicodeString(L"Lähtö");
		}
	else {
		alkuVa = Va;
		if (IkkParam.vainjarj)
			OtsCells[CTulos].text = UnicodeString(L"Tulos");
		Caption = UnicodeString(Sarjat[Sarja].sarjanimi) + L" - Loppuajat";
		LA = 0;
		}
	Va1 = Va;
	if (Va == YTulVal-1) {
		Va = kilpparam.valuku+1;
		alkuVa = Va+4;
		Va1 = 0;
		}
	if (k_pv)
		pKarki = PISTEKATTO - pkarki[Sarja][kilpparam.valuku+1];

//   if (!KH && Seuraa) {
//      nrivi = TulosGrid->Height / TulosGrid->DefaultRowHeight - 2;
//      }
//   else
//      nrivi = 100000;
	if (LA) {
		nTls = nilm[Sarja]-neil[Sarja];
		}
	else {
		if (!HylVal->Checked)
			nTls = ntulos[Sarja][Va];
		else {
//      Seuraa = 0;
			nTls = nhyl[Sarja] + nkesk[Sarja];
			Va = 0;
			KH = 1;
			}
		}
	IkkParam.Va = Va;
//   if (nrivi > nTls+1)
	EnterCriticalSection(&tall_CriticalSection);
//   karki = 0;
//	if (!vainjarj || (k_pv && !LA && !Yhd && yhttulos)) {
	sijat2(Sarja, false, true);
//		}
	if (k_pv && IkkParam.laskeYhtpisteet) {
		pKarki = teeYPisteJarj(Sarja, &ypJrj);
		}
	if (k_pv && IkkParam.laskeYhtpisteet && Va == kilpparam.valuku+1) {
		p = ypJrj[0][1];
		}
	else if (!IkkParam.vainjarj && IkkParam.smartJrj) {
		p = SmartJarj(0, &nTls);
		}
	else {
		if (!KH) {
			p = jalku[Sarja][alkuVa];
			}
		else {
			p = jalku[Sarja][1];
			}
		}
	k = 1;
	if (Cells != NULL && nRivi < nTls + 1) {
		delete[] Cells;
		Cells = NULL;
		nRivi = 0;
		}
	if (Cells == NULL) {
		nRivi = nTls + 1;
		Cells = new cellTpA[nRivi];
		}
	nrivi = nTls+1;
	TulosGrid->RowCount = nrivi > 2 ? nrivi : 2;
//	if (LA && Seuraalhtj1->Checked && nrivi > 1) {
//		for (int c = 0; c < TulosGrid->ColCount; c++)
//			OtsCells[1][c] = L"";
//		}
	TulosGrid->FixedRows = (IkkParam.ScrollOn && IkkParam.ScrollType == 1) ? 0 : 1;
	for (int k1 = 0; k1 < nRivi; k1++) {
		memset(Cells[k1], 0, sizeof(cellTpA));
		if (k1 > 3 && ((k1-4) % 6) < 3) {
			for (int ic = 0; ic < TulosGrid->ColCount; ic++)
				Cells[k1][ic].style = 7;
			}
		}
	if (nrivi == 1) {
		paivitysflag = 0;
		LeaveCriticalSection(&tall_CriticalSection);
		if (nrivi != nRiviEd)
			TulosGrid->Refresh();
		nRiviEd = nrivi;
		return;
		}
	if (!Seuraalhtj1->Checked)
		aikaNyt = t_time_l(biostime(0, 0), t0) * KSEK;
	tKarki = pkarki[Sarja][Va1];
	sj1 = 0;
	for (int j = 0; j < nTls; j++) {
		int tcol = IkkParam.lahtoaika ? CTulos+1 : CTulos;
		int odPiste = -1, odAika = 0;

		if (p < 0 && KH == 1) {
			p = jalku[Sarja][2];
			KH == 2;
			}
		if (p <= 0)
			break;
		p1 = p;
		if (!IkkParam.vainjarj && IkkParam.smartJrj) {
			p = SmartJarj(j+1, 0);
			}
		else if (ypJrj != NULL && Va == kilpparam.valuku+1) {
			p = ypJrj[j+1][1];
			}
		else
			p = JarjSeur(Yhd, Va, p);
//      if (j && Seuraa && j <= nTls - nrivi)
//         continue;
		if (k >= nrivi)
			break;
		if (p1 > 0) {
			int tlsNyt;

			kilp.GETREC(p1);
			if (Rajaukset->Checked) {
				if (!OnAlisarja(&kilp)) {
					TulosGrid->RowCount--;
					continue;
					}
				}
			sj1++;
			if (IkkParam.Era > 0 && kilp.pv[k_pv].era != IkkParam.Era)
				continue;
			if (!KH) {
				if (LA) {
					t = kilp.pv[k_pv].tlahto;
					if (Seuraalhtj1->Checked) {
						int t1;
						t1 = NORMKELLO(t - aikaNyt);
						if (IkkParam.AaniLkm && NORMKELLO(t - edAani) > 0 &&
							t1 < IkkParam.AaniLkm*SEK && t1 >= -2*SEK) {
							if (t1 <= 0) {
								edAani = t;
								Beep(550,400);
								edEnn = IkkParam.AaniLkm*SEK;
								}
							else if (t1 <= edEnn - SEK){
								Beep(650, 150);
								edEnn = t1;
								}
							}
						if (t1 <= -SEK) {
							if (TulosGrid->RowCount > 2)
								TulosGrid->RowCount--;
							continue;
							}
						}
					}
				else
					t = kilp.tulos2(Va1, 2);
//				t = kilp.tulos2(Va1);
				}
			if (IkkParam.laskeYhtpisteet) {
				kilp.pv[k_pv].pvpisteet[0] = Pisteet(&Sarjat[Sarja].pistekaava[k_pv], kilp.p_sija(0), ntulos[Sarja][0],
					kilp.p_aika(0, 2, 2), pkarki[Sarja][0]);
				LaskeKilpYhtPisteet(&kilp, &Sarjat[Sarja].pistekaava[k_pv], Sarja);
				kilp.pv[k_pv].ysija = YhtSija(kilp.pisteet[0], Sarja, ypJrj);
				}
			tlsNyt = NORMTULOS(aikaNyt - kilp.TLahto());
			if (IkkParam.smartJrj) {
				int ip = Sarjat[Sarja].luonne[k_pv] == 2 ? kilpparam.valuku + 1 : 0;
				if (kilp.tulos2(ip) > 0 && kilp.hyv()) {
//					if (yhd)
//						Cells[k][0].text = yhdsija[yhd][ip][kilp.id()];
//					else
						Cells[k][0].text = UnicodeString(kilp.p_sija(ip, Sarja));
					}
				else
					Cells[k][0].text = L" ";
				if (IkkParam.naytaOdotetut && kilp.tulos2(0) == 0) {
					odPiste = 0;
					odAika = vavali[0][0];
					for (int iva = 0; iva < npiste; iva++) {
						if (naytapiste[iva] && kilp.tulos2(naytapiste[iva]-1)) {
							if (vavali[iva+1][0]) {
								odPiste = iva+1;
								odAika = kilp.tulos2(naytapiste[iva]-1) + vavali[iva+1][0];
								}
							else
								odPiste = -1;
							}
						}
					if (odAika > tlsNyt + IkkParam.odRaja) {
						odPiste = -1;
						odAika = 0;
						}
					else
						odAika = tlsNyt - odAika;
					}
				}
			else if (LA || Va == Va1) {
				if (!LA && !KH && t != edT)
					 Cells[k][0].text = UnicodeString(sj1);
				else
					 Cells[k][0].text = UnicodeString(L" ");
				}
			else {
				if (!Rajaukset->Checked)
					sj1 = kilp.p_sija(Va1, Sarja);
				Cells[k][0].text = sj1;
				}
			edT = t;
			if (Rajaukset->Checked && sj1 == 1)
				tKarki = t;
			Cells[k][CNro].text = UnicodeString(kilp.id());
			kilp.nimi(ln, 32, Etunimiensin1->Checked);
			Cells[k][CNimi].text = UnicodeString(ln);
			ln[0] = 0;
			if (Pitkseuranimi1->Checked) {
				swprintf(ln, L"%s", kilp.seura);
			   }
			else if (Yhdistys1->Checked) {
				swprintf(ln, L"%s", kilp.yhdistys);
				 }
			else if (Seuralyhenne1->Checked) {
				swprintf(ln, L"%s", kilp.seuralyh);
				}
			if (ln[0]) {
				Cells[k][CSeura].text = UnicodeString(ln);
				}
			if (CViimeisin) {
				if (kilp.hyv()) {
					int pst = 0;
					do {
						if (kilp.p_aika(pst)) {
							wchar_t st1[5] = L"M", st2[20];
							if (pst > 0)
								_itow(pst, st1, 10);
							AIKATOWSTRS(st2, kilp.p_aika(pst), 0);
							st2[IkkParam.laika] = 0;
							elimwzb1(st2);
							swprintf(ln, L"%s: %s", st1, st2);
							if (kilp.p_sija(pst, Sarja) < 10)
								wcscat(ln, L"  ");
							swprintf(ln+wcslen(ln), L" (%d)", kilp.p_sija(pst, Sarja));
							break;
							}
						if (pst == 0)
							pst = Sarjat[Sarja].valuku[k_pv];
						else
							pst--;
						} while(pst != 0);
					}
				Cells[k][CViimeisin].text = ln;
				}
			if (CAlisarja)
				Cells[k][CAlisarja].text = StrAlisarja(ln, &kilp);
			if (CMaa)
				Cells[k][CMaa].text = UnicodeString(kilp.maa);
			ln2[0] = 0;
			if (!KH && (IkkParam.vainjarj || !IkkParam.smartJrj || kilp.hyv())) {
				if (npiste <= 1 && !IkkParam.lahtoaika) {
					AIKATOWSTRS(ln, t, LA*t0);
					ln[IkkParam.laika] = 0;
					elimwzb1(ln);
					if (!LA && tlsNyt - t > 0 && tlsNyt - t <= IkkParam.korRaja[1]) {
						if (tlsNyt - t <= IkkParam.korRaja[0])
							Cells[k][CTulos].style = 1;
						else
							Cells[k][CTulos].style = 2;
						}
					if (t && !LA && (j > 0 || Va != Va1) && !(k_pv && Va1 && Sarjat[Sarja].luonne[k_pv] == 2 && IkkParam.yhttulos)) {
						t -= tKarki;
						ln2[0] = L'+';
						AIKATOWSTRS(ln2+1, t, 0);
						ln2[IkkParam.laika+1] = 0;
						elimwzb1(ln2+1);
						}
					if (LA) {
						t = NORMKELLO(kilp.TLahto()- aikaNyt);
						if (t >= 0) {
							AIKATOWSTRS(ln2, t, 0);
							ln2[8] = 0;
							elimwzb1(ln2);
							}
						}
					Cells[k][CTulos].text = UnicodeString(ln);
					Cells[k][CEro].text = UnicodeString(ln2);
					}
				else {
					if (IkkParam.lahtoaika) {
						AIKATOWSTRS(ln, kilp.TLahto(), t0);
						ln[IkkParam.laika] = 0;
						elimwzb1(ln);
						Cells[k][CTulos].text = UnicodeString(ln);
						}
					for (int iva = 0; iva < npiste; iva++) {
						if (naytapiste[iva]) {
							t = kilp.tulos2(naytapiste[iva]-1);
							if (t == 0) {
								if (iva != odPiste)
									wcscpy(ln , L"--   ");
								else {
									if (odAika < 0) {
										AIKATOWSTRS(ln, -odAika, 0);
										Cells[k][tcol+iva].style = 16;
										}
									else {
										AIKATOWSTRS(ln, odAika, 0);
										Cells[k][tcol+iva].style = 32;
										}
									ln[8] = 0;
									elimwzb1(ln);
									}
								}
							else {
								if (tlsNyt - t > 0 && tlsNyt - t <= IkkParam.korRaja[1]) {
									if (tlsNyt - t <= IkkParam.korRaja[0])
										Cells[k][tcol+iva].style = 1;
									else
										Cells[k][tcol+iva].style = 2;
									}
								if (IkkParam.kierrosaika > 0 && naytapiste[iva]-1 > IkkParam.kierrosaika) {
									if (kilp.tulos2(naytapiste[iva]-IkkParam.kierrosaika-1))
										t -= kilp.tulos2(naytapiste[iva]-IkkParam.kierrosaika-1);
									else
										t = 0;
									}
								if (IkkParam.kierrosaika > 0 && naytapiste[iva]-1 == 0 &&
									Sarjat[Sarja].valuku[k_pv] >= IkkParam.kierrosaika) {
									if (kilp.tulos2(Sarjat[Sarja].valuku[k_pv]-IkkParam.kierrosaika+1))
										t -= kilp.tulos2(Sarjat[Sarja].valuku[k_pv]-IkkParam.kierrosaika+1);
									else
										t = 0;
									}
								ln[0] = 0;
								if (t)
									AIKATOWSTRS(ln, t, 0);
								ln[IkkParam.laika] = 0;
								elimwzb1(ln);
								if (kilp.p_sija(naytapiste[iva]-1, Sarja) > 1 && IkkParam.eroaika) {
									t -= pkarki[Sarja][naytapiste[iva]-1];
									ln[0] = L'+';
									AIKATOWSTRS(ln+1, t, 0);
									ln[IkkParam.laika+1] = 0;
									elimwzb1(ln+1);
									}
								if (kilp.p_sija(naytapiste[iva]-1, Sarja) < 10)
									wcscat(ln, L"  ");
								swprintf(ln+wcslen(ln), L" (%d)", kilp.p_sija(naytapiste[iva]-1, Sarja));
								}
							Cells[k][tcol+iva].text = UnicodeString(ln);
							}
						}
					}
				}
			else {
				switch (kilp.tark(k_pv)) {
					case L'H' :
						wcscpy(ln, L"Hyl.");
						break;
					case L'M' :
						wcscpy(ln, L"Puut.");
						break;
					case L'K' :
						wcscpy(ln, L"Kesk.");
						break;
					case L'O' :
						wcscpy(ln, L"Ohit.");
						break;
					case L'X' :
						wcscpy(ln, L"EiTul");
						break;
					case L'E' :
					case L'V' :
						wcscpy(ln, L"Eil.");
						break;
					case L'B' :
						wcscpy(ln, L"Hav.");
						break;
					}
				Cells[k][CTulos].text = UnicodeString(ln);
				Cells[k][CEro].text = UnicodeString(ln2);
				}
			if (kilpparam.lsakmax > 0) {
				int scol = CTulos+2;

				if (npiste > 2 || IkkParam.lahtoaika)
					scol = tcol + npiste;
				if (Sarjat[Sarja].lsak[k_pv] > 0) {
					if (kilpparam.kilplaji == L'B') {
						int sk1, sk2, sk3;
						sk1 = kilp.pv[k_pv].asakot[0] - L'0';
						sk2 = kilp.pv[k_pv].asakot[1] - L'0';
						sk3 = kilp.pv[k_pv].asakot[3] - L'0';
						ln[0] = 0;
						if (sk1 < 0)
							wcscat(ln, L"-");
						else
							_itow(sk1, ln, 10);
						if (sk2 >= 0) {
							wcscat(ln, L"+");
							_itow(sk2, ln+wcslen(ln), 10);
							}
						wcscat(ln, L"/");
						if (sk3 < 0)
							wcscat(ln, L"-");
						else
							_itow(sk3, ln+wcslen(ln), 10);
						Cells[k][scol].text = ln;
						}
					else
						Cells[k][scol].text = kilp.asakkoStr();
//						Cells[k][scol].text = UnicodeString(kilp.pv[k_pv].asakot).SubString(1, Sarjat[Sarja].lsak[k_pv]);
					}
				else
					Cells[k][scol].text = UnicodeString(L"");
				}
			if (LA && wcswcind(kilpparam.kilplaji, L"NP") >= 0) {
				Cells[k][CEro].text = kilp.pv[k_pv].era;
				Cells[k][CEro+1].text = kilp.pv[k_pv].rata;
				}
			if (kilpparam.alalaji == L'D' || wcswcind(kilpparam.kilplaji, L"NP") >= 0) {
				int cb = TulosGrid->ColCount - ((k_pv && IkkParam.yhttulos) ? 4 : 1);
				Cells[k][cb].text = kilp.pv[k_pv].pvpisteet[0];
				}
			if (k_pv && IkkParam.yhttulos) {
				int cb = TulosGrid->ColCount - 3;

				ln[0] = 0;
				ln2[0] = 0;
				sj = 0;
				if (!LA && !KH && (t = kilp.ytulos_p(Va1)) != 0) {
					if (Va1)
						{
						if (Sarjat[Sarja].luonne[k_pv] == 2)
							sj = kilp.p_sija(Va1, Sarja);
						else
							sj = 0;
						}
					else
						sj = kilp.p_sija(kilpparam.valuku+1, Sarja);
//						sj = kilp.y_sija(k_pv);
					if (Sarjat[Sarja].luonne[k_pv] != 2 &&
						(IkkParam.laskeYhtpisteet || wcswcind(kilpparam.kilplaji, L"NP") >= 0)) {
						_itow(kilp.pisteet[0], ln, 10);
						if (ypJrj)
							_itow(kilp.pisteet[0]-ypJrj[0][0], ln2, 10);
						}
					else if (Sarjat[Sarja].luonne[k_pv] == 2 || Sarjat[Sarja].pisteytulos() == false) {
						AIKATOWSTRS(ln, t, 0);
						ln[IkkParam.laika] = 0;
						elimwzb1(ln);
						}
					else
						kilp.ytulos_wstr_cols_n(ln, Va1, L',', 0, 0, 8);
//					else if (Sarjat[Sarja].nouspisteet())
//						_itow(t/SEK, ln, 10);
//					else
//						_itow((PISTEKATTO-t)/SEK, ln, 10);
					if (wcswcind(kilpparam.kilplaji, L"NP") >= 0)
						_itow(kilp.pisteet[0] - pKarki/SEK, ln2, 10);
					else if (sj > 1 && (!IkkParam.laskeYhtpisteet || Sarjat[Sarja].luonne[k_pv] == 2)) {
						if (Va1)
							t -= pkarki[Sarja][Va1];
						else
							t -= pkarki[Sarja][kilpparam.valuku+1];
						ln2[0] = L'+';
						if (Sarjat[Sarja].luonne[k_pv] == 2 || Sarjat[Sarja].pisteytulos() == false) {
							AIKATOWSTRS(ln2+1, t, 0);
							ln2[IkkParam.laika+1] = 0;
							elimwzb1(ln2+1);
							}
						else if (Sarjat[Sarja].nouspisteet())
							_itow(t/SEK, ln2+1, 10);
						else {
							_itow(-t/SEK, ln2, 10);
							}
						}
					}
				Cells[k][cb].text = UnicodeString(ln);
				Cells[k][cb+1].text = UnicodeString(ln2);
				if (sj)
					Cells[k][cb+2].text = UnicodeString(sj);
				else
					Cells[k][cb+2].text = UnicodeString(L"");
				}
			if (CEnn) {
				if (kilp.pv[k_pv].enn) {
					aikatowstr_s(ln, kilp.pv[k_pv].enn/SEK, 0);
					elimwzb1(ln);
					}
				else
					ln[0] = 0;
				Cells[k][CEnn].text = UnicodeString(ln);
				if ((t = kilp.p_aika(0,0)) > 0 && t <= kilp.pv[k_pv].enn)
					Cells[k][CEnn].style = 2;
				}
			if (CTav) {
				if (kilp.pv[k_pv].tav) {
					aikatowstr_s(ln, kilp.pv[k_pv].tav/SEK, 0);
					elimwzb1(ln);
					}
				else
					ln[0] = 0;
				Cells[k][CTav].text = UnicodeString(ln);
				if ((t = kilp.p_aika(0,0)) > 0 && t <= kilp.pv[k_pv].tav)
					Cells[k][CTav].style = 2;
				}
			k++;
			}
		}
	LeaveCriticalSection(&tall_CriticalSection);
	if (IkkParam.AktColor > 0 &&
		TulosGrid->RowCount < TulosGrid->Height / (TulosGrid->DefaultRowHeight+1) + 1) {
		TulosGrid->RowCount = TulosGrid->Height / (TulosGrid->DefaultRowHeight+1) + 1;
		for (;k < TulosGrid->RowCount; k++)
			for (int c = 0; c < TulosGrid->ColCount; c++)
				Cells[k][c].text = L"";
		}
	for (int col = 0; col < TulosGrid->ColCount; col++)
		Cells[0][col].text = OtsCells[col].text;
	if (ypJrj)
		free(ypJrj);
	paivitysflag = 0;
	if (ViimeisetRivit) {
		if (k > TulosGrid->Height / (TulosGrid->DefaultRowHeight+1) - 1)
			TulosGrid->TopRow = k - TulosGrid->Height / (TulosGrid->DefaultRowHeight+1) + 1;
		else
			TulosGrid->TopRow = 1;
		}
	TulosGrid->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TTilanneForm::SrjValChange(TObject *Sender)
{
	int srj = haesarja_w(SrjVal->Text.c_str(), true);
	if (srj < 0)
		return;
   IkkParam.Sarja = srj;
   FormSetup(false, true);
}
//---------------------------------------------------------------------------
void __fastcall TTilanneForm::PisteValChange(TObject *Sender)
{
   FormSetup(false, true);
}
//---------------------------------------------------------------------------
void __fastcall TTilanneForm::JarjValClick(TObject *Sender)
{
   FormSetup(false, true);
}
//---------------------------------------------------------------------------
void __fastcall TTilanneForm::ViimeisimmatClick(TObject *Sender)
{
   FormSetup(false, true);
}
//---------------------------------------------------------------------------
void __fastcall TTilanneForm::FormResize(TObject *Sender)
{
	if (!IkkParam.KokoRuutu) {
		TulosGrid->Top = Panel1->Height;
		TulosGrid->Height = ClientHeight - TulosGrid ->Top;
		TulosGrid->Width = ClientWidth - TulosGrid->Left;
		TulosGrid->Refresh();
		Panel1->Visible = true;
		Panel1->Width = ClientWidth;
		}
	else {
		TulosGrid->Top = 0;
		TulosGrid->Height = ClientHeight;
		TulosGrid->Width = ClientWidth;
		Panel1->Visible = false;
		}
	TulosGrid->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TTilanneForm::FormSetup(bool pakota, bool paivitaLeveys)
{
	int wdt, iCol, ii = 0, LeftCol, ensSarja = -1;

	LeftCol = TulosGrid->LeftCol;
	IkkParam.k_pv = k_pv;
	if (!pakota && IkkParam == IkkParamEd && PisteVal->ItemIndex == pVal)
		return;
	IkkParamEd = IkkParam;
	for (int srj = 0; srj < sarjaluku+nsarjayhd; srj++) {
		if (srj >= sarjaluku || (Sarjat[srj].luonne[k_pv] >= 0 &&
			nilm_pv[k_pv][srj] - nvak[k_pv][srj] > 0)) {
			if (ensSarja < 0)
				ensSarja = srj;
			ii++;
			if (wcscmp(SrjVal->Items->Strings[ii-1].c_str(), Sarjat[srj].sarjanimi))
				break;
			}
		}
	if (ii < SrjVal->Items->Count) {
		ii = 0;
		ensSarja = -1;
		SrjVal->Clear();
		for (int srj = 0; srj < sarjaluku+nsarjayhd; srj++) {
			if (srj >= sarjaluku || (Sarjat[srj].luonne[k_pv] >= 0 &&
				nilm_pv[k_pv][srj] - nvak[k_pv][srj] > 0)) {
				if (ensSarja < 0)
					ensSarja = srj;
				SrjVal->Items->Add(Sarjat[srj].sarjanimi);
				if (srj == IkkParam.Sarja)
					ii = SrjVal->Items->Count - 1;
				}
			}
		SrjVal->ItemIndex = ii;
		}
	if (ensSarja < 0)
		ensSarja = 0;
	if (IkkParam.Sarja < ensSarja || IkkParam.Sarja >= sarjaluku+nsarjayhd)
		IkkParam.Sarja = ensSarja;
	npiste = 0;
	iCol = 3;
	if (IkkParam.viimeisin)
		CViimeisin = iCol++;
	else
		CViimeisin = 0;
	if (IkkParam.naytaAlisarja)
		CAlisarja = iCol++;
	else
		CAlisarja = 0;
	if (IkkParam.seuraTieto > 0)
		CSeura = iCol++;
	else
		CSeura = 0;
	if (IkkParam.naytaMaa)
		CMaa = iCol++;
	else
		CMaa = 0;
	CTulos = iCol++;
	CEro = iCol++;
	if (IkkParam.Ennatys)
		CEnn = iCol++;
	else
		CEnn = 0;
	if (IkkParam.Tavoite)
		CTav = iCol++;
	else
		CTav = 0;
	N3->Visible = !IkkParam.vainjarj;
	Erokrkeen1->Visible = !IkkParam.vainjarj;
	Lhtaika1->Visible = !IkkParam.vainjarj;
	CBSmart->Visible = !IkkParam.vainjarj;
	if (IkkParam.vainjarj) {
		IkkParam.lahtoaika = false;
		Lhtaika1->Checked = false;
		}
	if (Sarjat[IkkParam.Sarja].luonne[k_pv] > 0 && k_pv > Sarjat[IkkParam.Sarja].enspv) {
		IkkParam.yhttulos = Yhteistulos1->Checked;
		}
	else {
		IkkParam.yhttulos = false;
		}
	pVal = PisteVal->ItemIndex;
	if (PisteVal->ItemIndex == YTulVal)
		pVal = 9999;
	PisteVal->Items->Clear();
	PisteVal->Items->Add(L"Lähtö");
	PisteVal->Items->Add(L"Maali");
	for (int p = 0; p < Sarjat[IkkParam.Sarja].valuku[k_pv]; p++)
		PisteVal->Items->Add(UnicodeString(p+1)+UnicodeString(L". väliaika"));
//	YTulVal = Sarjat[Sarja].valuku[k_pv] + 2;
	YTulVal = PisteVal->Items->Count;
	if (Sarjat[IkkParam.Sarja].luonne[k_pv] > 0 && k_pv > Sarjat[IkkParam.Sarja].enspv) {
		PisteVal->Items->Add(L"Yhteistulos");
		if (pVal >= YTulVal) {
			pVal = (pVal == 9999 ? YTulVal : YTulVal - 1);
			}
		}
	else {
		if (pVal >= YTulVal) {
			pVal = (pVal == 9999 ? 1 : YTulVal - 1);
			}
		}
	if (IkkParam.smartJrj) {
		if (k_pv > 0 && Sarjat[IkkParam.Sarja].luonne[k_pv] == 2)
			pVal = YTulVal;
		else
			pVal = 1;
		}
	PisteVal->ItemIndex = pVal;
	memset(naytapiste, 0, sizeof(naytapiste));
	for (int iva = 0; iva <= Sarjat[IkkParam.Sarja].valuku[k_pv]; iva++) {
		if (IkkParam.kaikkiajat || IkkParam.minTaso > 0) {
			int minT = IkkParam.minTaso;
			if (IkkParam.minTaso == 0 && !Naytapiilotetut->Checked)
				minT = 1;
			if (iva == 0 || Sarjat[IkkParam.Sarja].va_piilota[k_pv][iva-1] >= minT) {
				if (iva == 0)
					naytapiste[VALUKU] = 1;
				else
					naytapiste[npiste++] = iva+1;
				}
			}
		else if (!IkkParam.vainjarj && (iva == 0 || (Sarjat[IkkParam.Sarja].va_matka[k_pv][iva-1][0] &&
			wcscmp(Sarjat[IkkParam.Sarja].va_matka[k_pv][iva-1], L"0")))
			&& NaytaPiste[iva]->Checked) {
			if (iva == 0)
				naytapiste[VALUKU] = 1;
			else
				naytapiste[npiste++] = iva+1;
			}
		}
	if (naytapiste[VALUKU])
		naytapiste[npiste++] = 1;
	if (IkkParam.Lihavoitu) {
		TulosGrid->Font->Style = TFontStyles() << fsBold;
		lev = 2;
		}
	else {
		lev = 0;
		TulosGrid->Font->Style = TFontStyles();
		}
	TulosGrid->DefaultRowHeight = (2 * IkkParam.FontSize - 2) * Screen->PixelsPerInch/96;
	TulosGrid->Font->Size = IkkParam.FontSize;
	lev += IkkParam.FontSize;
	if (npiste <= 1 && !IkkParam.lahtoaika)
		TulosGrid->ColCount = CTulos + 2 + (kilpparam.lsakmax > 0) + IkkParam.Ennatys + IkkParam.Tavoite;
	else
		TulosGrid->ColCount = CTulos + npiste + (kilpparam.lsakmax > 0) + IkkParam.Ennatys + IkkParam.Tavoite;
	if (FixedCols)
		TulosGrid->FixedCols = (IkkParam.viimeisin ? 4 : 3) + (IkkParam.seuraTieto ? 1 : 0);
	else
		TulosGrid->FixedCols = 0;
	if (LeftCol < TulosGrid->ColCount-1 && LeftCol > TulosGrid->FixedCols)
		TulosGrid->LeftCol = LeftCol;
	else
		TulosGrid->LeftCol = TulosGrid->FixedCols;
	memset(ColLj, 0, sizeof(ColLj));
	TulosGrid->ColWidths[0] = IkkParam.ColW[0] * lev * Screen->PixelsPerInch/960;
	ColLj[0] = 1;
	TulosGrid->ColWidths[CNro] = IkkParam.ColW[1] * lev * Screen->PixelsPerInch/960;
	ColLj[1] = CNro+1;
	TulosGrid->ColWidths[CNimi] = IkkParam.ColW[2] * lev * Screen->PixelsPerInch/960;
	ColLj[2] = CNimi+1;
	if (IkkParam.viimeisin) {
		TulosGrid->ColWidths[CViimeisin] = IkkParam.ColW[3] * lev * Screen->PixelsPerInch/960;
		ColLj[3] = CViimeisin+1;
		}
	switch (IkkParam.seuraTieto) {
		case 1:
			TulosGrid->ColWidths[CSeura] = IkkParam.ColW[4] * lev * Screen->PixelsPerInch/960;
			ColLj[4] = CSeura+1;
			break;
		case 2:
			TulosGrid->ColWidths[CSeura] = IkkParam.ColW[5] * lev * Screen->PixelsPerInch/960;
			ColLj[5] = CSeura+1;
			break;
		case 3:
			TulosGrid->ColWidths[CSeura] = IkkParam.ColW[6] * lev * Screen->PixelsPerInch/960;
			ColLj[6] = CSeura+1;
			break;
		}
	if (CAlisarja) {
		TulosGrid->ColWidths[CAlisarja] = IkkParam.ColW[7] * lev * Screen->PixelsPerInch/960;
		ColLj[7] = CAlisarja+1;
		}
	if (CMaa) {
		TulosGrid->ColWidths[CMaa] = IkkParam.ColW[8] * lev * Screen->PixelsPerInch/960;
		ColLj[8] = CAlisarja+1;
		}
	OtsCells[0].text = UnicodeString(L"Sija");
	OtsCells[0].justify = 1;
	OtsCells[CNro].text = UnicodeString(L"Nro");
	OtsCells[CNro].justify = 1;
	OtsCells[CNimi].text = UnicodeString(L"Nimi");
	OtsCells[CNimi].justify = 0;
	if (CViimeisin) {
		OtsCells[CViimeisin].text = UnicodeString(L"Viimeisin");
		OtsCells[CViimeisin].justify = 1;
		}
	if (CAlisarja) {
		OtsCells[CAlisarja].text = UnicodeString(L"Srj");
		OtsCells[CAlisarja].justify = 0;
		}
	if (CSeura) {
		OtsCells[CSeura].text = UnicodeString(L"Seura");
		OtsCells[CSeura].justify = 0;
		}
	if (CMaa) {
		OtsCells[CMaa].text = UnicodeString(L"Maa");
		OtsCells[CMaa].justify = 0;
		}
	if (CEnn) {
		OtsCells[CEnn].text = UnicodeString(L"Ennätys");
		OtsCells[CEnn].justify = 1;
		TulosGrid->ColWidths[CEnn] = IkkParam.ColW[18] * lev * Screen->PixelsPerInch/960;
		}
	if (CTav) {
		OtsCells[CTav].text = UnicodeString(L"Tavoite");
		OtsCells[CTav].justify = 1;
		TulosGrid->ColWidths[CTav] = IkkParam.ColW[19] * lev * Screen->PixelsPerInch/960;
		}
	if (npiste <= 1 && !IkkParam.lahtoaika) {
		TulosGrid->ColWidths[CTulos] = IkkParam.ColW[9] * lev * Screen->PixelsPerInch/960;
		ColLj[9] = CTulos+1;
		TulosGrid->ColWidths[CEro] = IkkParam.ColW[10] * lev * Screen->PixelsPerInch/960;
		ColLj[10] = CEro+1;
		OtsCells[CTulos].text = UnicodeString(L"Tulos");
		OtsCells[CEro].text = UnicodeString(L"Ero");
		OtsCells[CTulos].justify = 1;
		OtsCells[CEro].justify = 1;
		}
	else {
		int tcol = IkkParam.lahtoaika ? CTulos+1 : CTulos;
		TulosGrid->ColCount = tcol + npiste + (kilpparam.lsakmax > 0);
		if (IkkParam.lahtoaika) {
			TulosGrid->ColWidths[CTulos] = IkkParam.ColW[11] * lev * Screen->PixelsPerInch/960;
			ColLj[11] = CTulos+1;
			OtsCells[CTulos].text = UnicodeString(L"Lähtö");
			OtsCells[CTulos].justify = 1;
			}
		for (int iva = 0; iva < npiste; iva++) {
			TulosGrid->ColWidths[tcol+iva] = IkkParam.ColW[12] * lev * Screen->PixelsPerInch/960;
			ColLj[12] = tcol+iva+1;
			if (naytapiste[iva] == 1)
				OtsCells[tcol+iva].text = UnicodeString(L"Tulos");
			else if (naytapiste[iva] > 1)
				OtsCells[tcol+iva].text = UnicodeString(naytapiste[iva]-1)+L"/"+Sarjat[IkkParam.Sarja].va_matka[k_pv][naytapiste[iva]-2];
			OtsCells[tcol+iva].justify = 1;
			}
		}
	if (kilpparam.lsakmax > 0) {
		TulosGrid->ColWidths[TulosGrid->ColCount-1] = IkkParam.ColW[13] * lev * Screen->PixelsPerInch/960;
		ColLj[13] = TulosGrid->ColCount;
		OtsCells[TulosGrid->ColCount-1].text = UnicodeString(L"Sakot");
		}
	if ((pVal == 0 && IkkParam.EraVal) || wcswcind(kilpparam.kilplaji, L"U") >= 0) {
		OtsCells[CEro].text = UnicodeString(L"Erä");
		OtsCells[CEro+1].text = UnicodeString(L"Rata");
		OtsCells[CEro+1].justify = 1;
		}
	if (kilpparam.alalaji == L'D' || wcswcind(kilpparam.kilplaji, L"NP") >= 0) {
		int cb = TulosGrid->ColCount;
		TulosGrid->ColCount += 1;
		TulosGrid->ColWidths[cb] = IkkParam.ColW[14] * lev * Screen->PixelsPerInch/960;
		ColLj[14] = cb+1;
		OtsCells[cb].text = UnicodeString(L"Kierr");
		OtsCells[cb].justify = 1;
		}
	if (k_pv && IkkParam.yhttulos) {
		int cb = TulosGrid->ColCount;

		TulosGrid->ColCount += 3;
		TulosGrid->ColWidths[cb] = IkkParam.ColW[15] * lev * Screen->PixelsPerInch/960;
		ColLj[15] = cb+1;
		TulosGrid->ColWidths[cb+1] = IkkParam.ColW[16] * lev * Screen->PixelsPerInch/960;
		ColLj[16] = cb+2;
		TulosGrid->ColWidths[cb+2] = IkkParam.ColW[17] * lev * Screen->PixelsPerInch/960;
		ColLj[17] = cb+3;
		OtsCells[cb].text = UnicodeString(L"Y-tulos");
		OtsCells[cb].justify = 1;
		OtsCells[cb+1].text = UnicodeString(L"Y-ero");
		OtsCells[cb+1].justify = 1;
		OtsCells[cb+2].text = UnicodeString(L"Y-sija");
		OtsCells[cb+2].justify = 1;
//		Width += 133 * Screen->PixelsPerInch/96;
		}
	if (paivitaLeveys) {
		wdt = TulosGrid->Left + 24 * Screen->PixelsPerInch/96;
		for (int cb = 0; cb < TulosGrid->ColCount; cb++) {
			wdt += TulosGrid->ColWidths[cb] + 2;
			}
		if (wdt > Screen->Width - 8 * Screen->PixelsPerInch/96)
			wdt = Screen->Width - 8 * Screen->PixelsPerInch/96;
		ClientWidth = wdt;
		}
	PaivitaGrid(false);
	}

//---------------------------------------------------------------------------
void __fastcall TTilanneForm::HylValClick(TObject *Sender)
{
	IkkParam.naytaHyl = HylVal->Checked;
   FormSetup(true, false);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Pitkseuranimi1Click(TObject *Sender)
{
	Pitkseuranimi1->Checked = !Pitkseuranimi1->Checked;
	if (Pitkseuranimi1->Checked) {
		IkkParam.seuraTieto = 1;
		Yhdistys1->Checked = false;
		Seuralyhenne1->Checked = false;
		}
	else if (IkkParam.seuraTieto == 1)
		IkkParam.seuraTieto = 0;
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Seuralyhenne1Click(TObject *Sender)
{
	Seuralyhenne1->Checked = !Seuralyhenne1->Checked;
	if (Seuralyhenne1->Checked) {
		IkkParam.seuraTieto = 2;
		Yhdistys1->Checked = false;
		Pitkseuranimi1->Checked = false;
		}
	else if (IkkParam.seuraTieto == 2)
		IkkParam.seuraTieto = 0;
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Yhdistys1Click(TObject *Sender)
{
	Yhdistys1->Checked = !Yhdistys1->Checked;
	if (Yhdistys1->Checked) {
		IkkParam.seuraTieto = 3;
		Seuralyhenne1->Checked = false;
		Pitkseuranimi1->Checked = false;
		}
	else if (IkkParam.seuraTieto == 3)
		IkkParam.seuraTieto = 0;
	FormSetup(true,true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Maatunnus1Click(TObject *Sender)
{
	Maatunnus1->Checked = !Maatunnus1->Checked;
	IkkParam.naytaMaa = Maatunnus1->Checked;
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Suuri1Click(TObject *Sender)
{
	IkkParam.FontSize = 12;
	Suuri1->Checked = true;
	Normaali1->Checked = false;
	Pieni1->Checked = false;
	Muumerkkikoko1->Checked = false;
	FormSetup(false, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Normaali1Click(TObject *Sender)
{
	IkkParam.FontSize = 10;
	Suuri1->Checked = false;
	Normaali1->Checked = true;
	Pieni1->Checked = false;
	Muumerkkikoko1->Checked = false;
	FormSetup(false, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Pieni1Click(TObject *Sender)
{
	IkkParam.FontSize = 8;
	Suuri1->Checked = false;
	Normaali1->Checked = false;
	Pieni1->Checked = true;
	Muumerkkikoko1->Checked = false;
	FormSetup(false, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Muumerkkikoko1Click(TObject *Sender)
{
	wchar_t ch;
	int fnt;

	fnt = IkkParam.FontSize;
	inputint_prompt(&fnt, L"Valitse koko pisteinä", &ch, this);
	if (ch == ESC)
		return;
	IkkParam.FontSize = fnt;
	Suuri1->Checked = false;
	Normaali1->Checked = false;
	Pieni1->Checked = false;
	Muumerkkikoko1->Checked = true;
	FormSetup(false, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Lihavoitu1Click(TObject *Sender)
{
	Lihavoitu1->Checked = ! Lihavoitu1->Checked;
	IkkParam.Lihavoitu = Lihavoitu1->Checked;
	FormSetup(true, true);
}
//---------------------------------------------------------------------------


void __fastcall TTilanneForm::TulosGridSelectCell(TObject *Sender, int ACol, int ARow,
		  bool &CanSelect)
{
	CurrentRow = ARow;
	CurrentCol = ACol;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::TulosGridDblClick(TObject *Sender)
{
	int KilpNo;
	int row;

	if (TulosGrid->Row > 1)
		row = TulosGrid->Row;
	else
		row = RowClicked;

   if (row > 0 && row < nRivi && Cells[row][1].text.Length() > 0
		&& (KilpNo = _wtoi(Cells[row][1].text.c_str())) > 0) {
		if (!FormKilpailijaOnlineTilanne)
			FormKilpailijaOnlineTilanne = new TFormKilpailijaOnline(this);
		FormKilpailijaOnlineTilanne->sallimuokkaus = false;
		FormKilpailijaOnlineTilanne->naytaKilpailija(getpos(KilpNo));
		FormKilpailijaOnlineTilanne->Show();
		if (FormKilpailijaOnlineTilanne->WindowState == wsMinimized)
			FormKilpailijaOnlineTilanne->WindowState = wsNormal;
		FormKilpailijaOnlineTilanne->BringToFront();
   }
}
//---------------------------------------------------------------------------


void __fastcall TTilanneForm::FormShow(TObject *Sender)
{
	Yhteistulos1->Visible = (k_pv > 0);
	for (int srj = 0; srj < sarjaluku+nsarjayhd; srj++) {
		if (srj >= sarjaluku || (Sarjat[srj].luonne[k_pv] >= 0 &&
			nilm_pv[k_pv][srj] - nvak[k_pv][srj] > 0))
			SrjVal->Items->Add(Sarjat[srj].sarjanimi);
		}
//	for (int yhd = 0; yhd < nsarjayhd; yhd++) {
//		SrjVal->Items->Add(yhdnimet[yhd]);
//		}
	SrjVal->ItemIndex = 0;
	PisteVal->Items->Clear();
	PisteVal->Items->Add(L"Lähtö");
	PisteVal->Items->Add(L"Maali");
	for (int p = 0; p < kilpparam.valuku; p++)
		PisteVal->Items->Add(UnicodeString(p+1)+UnicodeString(L". väliaika"));
	if (k_pv)
		PisteVal->Items->Add(L"Yhteistulos");
	PisteVal->ItemIndex = 1;
//	akaaajonvliaikatilanne1->Checked = takaa_ajo;
//	akaaajonvliaikatilanne1->Visible = k_pv > 0;
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Kilpailun seuranta");
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Yhteistulos1Click(TObject *Sender)
{
	Yhteistulos1->Checked = !Yhteistulos1->Checked;
	IkkParam.yhttulos = Yhteistulos1->Checked;
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	IkkParam.ScrollOn = false;
	Sleep(20);
	if (FormKilpailijaOnlineTilanne) {
		FormKilpailijaOnlineTilanne->Close();
		delete FormKilpailijaOnlineTilanne;
		FormKilpailijaOnlineTilanne = NULL;
		}
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Seuraalhtj1Click(TObject *Sender)
{
	Seuraalhtj1->Checked = !Seuraalhtj1->Checked;
	IkkParam.seuraaLahtoja = Seuraalhtj1->Checked;
	FormSetup(true, false);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Erokrkeen1Click(TObject *Sender)
{
	Erokrkeen1->Checked = !Erokrkeen1->Checked;
	IkkParam.eroaika = Erokrkeen1->Checked;
	if (IkkParam.eroaika) {
		if (IkkParam.kierrosaika > 0)
			IkkParam.kierrosaika = 1 - IkkParam.kierrosaika;
		Nytkierrosaika1->Checked = false;
		}
	if (!IkkParam.eroaika && IkkParam.kierrosaika <= 0) {
		IkkParam.kokotulos = true;
	}
	FormSetup(true, false);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Kaikkiajat1Click(TObject *Sender)
{
	Kaikkiajat1->Checked = !Kaikkiajat1->Checked;
	IkkParam.kaikkiajat = Kaikkiajat1->Checked;
	CBKaikkiPist->Checked = IkkParam.kaikkiajat;
	if (IkkParam.kaikkiajat) {
		Vainjrjestyspiste1->Checked = false;
		IkkParam.vainjarj = false;
		Pisteet1->Checked = true;
		for	(int iva = 0; iva < NAIKASAR && iva <= Sarjat[IkkParam.Sarja].valuku[k_pv]; iva++) {
			NaytaPiste[iva]->Checked = true;
			}
		}
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Vainjrjestyspiste1Click(TObject *Sender)
{
	Vainjrjestyspiste1->Checked = !Vainjrjestyspiste1->Checked;
	IkkParam.vainjarj = Vainjrjestyspiste1->Checked;
	if (IkkParam.vainjarj) {
		Pisteet1->Checked = false;
		Kaikkiajat1->Checked = false;
		IkkParam.kaikkiajat = false;
		for	(int iva = 0; iva < NAIKASAR; iva++) {
			NaytaPiste[iva]->Checked = false;
			}
		}
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Lhtaika1Click(TObject *Sender)
{
	Lhtaika1->Checked = !Lhtaika1->Checked;
	IkkParam.lahtoaika = Lhtaika1->Checked;
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::TulosGridDrawCell(TObject *Sender, int ACol, int ARow,
		  TRect &Rect, TGridDrawState State)
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
   Rect1.Empty();
   if (IkkParam.Lihavoitu)
		pBitmap->Canvas->Font->Style = TFontStyles() << fsBold;
   else
		pBitmap->Canvas->Font->Style = TFontStyles();
   if (Cells && ARow < nRivi) {
	   if (ARow == 0) {
		  pBitmap->Canvas->Brush->Color = TColor(ColorSet[IkkParam.AktColor].FirstRowBackColor);
		  pBitmap->Canvas->Font->Color = TColor(ColorSet[IkkParam.AktColor].FirstRowColor);
		  }
	   else {
		  switch (Cells[ARow][ACol].style & 15) {
			case 0:
				pBitmap->Canvas->Brush->Color = TColor(ColorSet[IkkParam.AktColor].WinBackColor);
				break;
			case 1:
				pBitmap->Canvas->Brush->Color = TColor(ColorSet[IkkParam.AktColor].korBG1);  //clLime;
//            pBitmap->Canvas->Font->Style = TFontStyles() << fsBold;
				break;
			case 2:
				pBitmap->Canvas->Brush->Color = TColor(ColorSet[IkkParam.AktColor].korBG2);  //clYellow;
//            pBitmap->Canvas->Font->Style = TFontStyles() << fsBold;
				break;
			case 3:
				pBitmap->Canvas->Brush->Color = TColor(korVari[2]);  //clFuchsia;
				break;
			case 4:
				pBitmap->Canvas->Brush->Color = TColor(korVari[3]);  //clSkyBlue;
				break;
			case 5:
				pBitmap->Canvas->Brush->Color = TColor(korVari[4]);  //clAqua;
				break;
			case 6:
				pBitmap->Canvas->Brush->Color = TColor(korVari[5]);  //clOlive - light;
				break;
			case 7:
//				pBitmap->Canvas->Brush->Color = TColor(0xffffd0);  //clOlive - light;
				pBitmap->Canvas->Brush->Color = TColor(ColorSet[IkkParam.AktColor].AltBackColor);  //clOlive - light;
				break;
			 }
		  switch (Cells[ARow][ACol].style / 16) {
			case 0:
				pBitmap->Canvas->Font->Color = TColor(ColorSet[IkkParam.AktColor].FontColor);
				break;
			case 1:
				pBitmap->Canvas->Font->Color = TColor(ColorSet[IkkParam.AktColor].FontColor2);
				pBitmap->Canvas->Font->Style = TFontStyles() << fsBold;
				break;
			case 2:
				pBitmap->Canvas->Font->Color = TColor(ColorSet[IkkParam.AktColor].FontColor3);
				break;
			}
		  }
	   pBitmap->Canvas->FillRect(Rect1);
	   pBitmap->Canvas->Font->Size = IkkParam.FontSize;
	   X = 1;
	   if (OtsCells[ACol].justify == 1) {
		   X =  Rect.Width() - pBitmap->Canvas->TextWidth(Cells[ARow][ACol].text) - 2;
		   if (X < 1)
			  X = 1;
		   }
	   pBitmap->Canvas->TextOut(X,1,Cells[ARow][ACol].text);
//   pBitmap->Canvas->TextRect(Rect, X,1,Cells[ACol][ARow].text);
		}
   TulosGrid->Canvas->Draw(Rect.Left, Rect.Top, pBitmap);
   delete pBitmap;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::lyksjrjestys1Click(TObject *Sender)
{
	lyksjrjestys1->Checked = !lyksjrjestys1->Checked;
	IkkParam.smartJrj = lyksjrjestys1->Checked;
	CBSmart->Checked = IkkParam.smartJrj;
	FormSetup(true, false);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::CBKaikkiPistClick(TObject *Sender)
{
	Kaikkiajat1->Checked = CBKaikkiPist->Checked;
	IkkParam.kaikkiajat = Kaikkiajat1->Checked;
	Vainjrjestyspiste1->Checked = !IkkParam.kaikkiajat;
	IkkParam.vainjarj = !IkkParam.kaikkiajat;
	Pisteet1->Checked = IkkParam.kaikkiajat;
	for	(int iva = 0; iva < NAIKASAR &&
		(IkkParam.vainjarj || iva <= Sarjat[IkkParam.Sarja].valuku[k_pv]); iva++) {
		NaytaPiste[iva]->Checked = IkkParam.kaikkiajat;
		}
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::CBSmartClick(TObject *Sender)
{
	lyksjrjestys1->Checked = CBSmart->Checked;
	IkkParam.smartJrj = lyksjrjestys1->Checked;
	FormSetup(true, false);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Korostusaikarajat1Click(TObject *Sender)
{
	int rj;
	wchar_t ch;

	rj = IkkParam.korRaja[0] / SEK;
	inputint_prompt(&rj, L"Anna alempi aikaraja sekunteina", &ch, this);
	IkkParam.korRaja[0] = SEK*rj;
	rj = IkkParam.korRaja[1] / SEK;
	inputint_prompt(&rj, L"Anna ylempi aikaraja sekunteina", &ch, this);
	IkkParam.korRaja[1] = SEK*rj;
}
//---------------------------------------------------------------------------


void __fastcall TTilanneForm::TulosGridFixedCellClick(TObject *Sender, int ACol, int ARow)
{
	int pst;
	if (ARow > 0) {
		RowClicked = ARow;
		TulosGridDblClick(Sender);
		return;
		}
	if (IkkParam.vainjarj || ACol < 5)
		return;
	pst = ACol - 5 - IkkParam.lahtoaika;
	IkkParam.smartJrj = false;
	CBSmart->Checked = false;
	if (pst == -1)
		PisteVal->ItemIndex = 0;
	else if (pst >= npiste) {
		if (k_pv)
			PisteVal->ItemIndex = YTulVal;
		else
			PisteVal->ItemIndex = 1;
		}
	else {
		PisteVal->ItemIndex = naytapiste[pst];
	}
	FormSetup(false, false);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Etunimiensin1Click(TObject *Sender)
{
	Etunimiensin1->Checked = !Etunimiensin1->Checked;
	IkkParam.EtuSuku = Etunimiensin1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::TulosGridMouseWheelDown(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
	if (TulosGrid->RowCount - TulosGrid->FixedRows >= TulosGrid->VisibleRowCount + TulosGrid->TopRow)
		TulosGrid->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::TulosGridMouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (TulosGrid->TopRow > TulosGrid->FixedRows)
		TulosGrid->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::TulosGridClick(TObject *Sender)
{
	RowClicked = TulosGrid->Row;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::akaaajonvliaikatilanne1Click(TObject *Sender)
{
	akaaajonvliaikatilanne1->Checked = !akaaajonvliaikatilanne1->Checked;
//	takaa_ajo = akaaajonvliaikatilanne1->Checked ? 1 : 0;
//	FormSetup(true, false);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::RajauksetClick(TObject *Sender)
{
	Rajaukset->Checked = !Rajaukset->Checked;
	IkkParam.rajaus = Rajaukset->Checked;
	EdtSp->Visible = Rajaukset->Checked;
	LblSp->Visible = Rajaukset->Checked;
	EdtIkasarja->Visible = Rajaukset->Checked;
	LblIkasarja->Visible = Rajaukset->Checked;
	FormSetup(true, false);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Alisarja1Click(TObject *Sender)
{
	Alisarja1->Checked = !Alisarja1->Checked;
	IkkParam.naytaAlisarja = Alisarja1->Checked;
	FormSetup(true, true);
}
//---------------------------------------------------------------------------
void ScrollThread(LPVOID ptr)
{
	TTilanneForm *frm;
	int frmno = *(int *)ptr;

	frm = frmno ? TilFrm[frmno-1] : TilanneForm;
	if (frm->IkkParam.ScrollOn)
	   Sleep(1000);
	frm->IkkParam.ScrollOn = true;
	while(taustaon && !vaiheenvaihto  && lkmThreadOn && frm->IkkParam.ScrollOn && frm->ScrollViive) {
		Sleep(frm->ScrollViive);
		SendMessage(frm->Handle, WM_MYSCROLL, 0, 0);
		}
	frm->IkkParam.ScrollOn = false;
	frm->ScrollViive = 0;
}
//---------------------------------------------------------------------------
void __fastcall TScrollGrid::SInvalidateRow(int ARow)
{
	InvalidateRow(ARow);
}

//---------------------------------------------------------------------------
MESSAGE void __fastcall TTilanneForm::ScrollMsgHandler(TMyMessage &msg)
{
	int rc, rh, n;
	int hetki;
	int srjKesto;

	if (!IkkParam.ScrollOn)
		return;
	rc = TulosGrid->ClientHeight / (TulosGrid->DefaultRowHeight+1)-1;
	if (alkuhetki <= 0) {
		int korj;
		if (IkkParam.ScrollType == 1 && (TulosGrid->ClientHeight+7) % (TulosGrid->DefaultRowHeight+1) != 0) {
			int korj = (TulosGrid->ClientHeight+1) % (TulosGrid->DefaultRowHeight+1) - 3;
			Height -= korj;
			}
		TulosGrid->TopRow = IkkParam.ScrollType == 1 ? 0 : 1;
		sc = 0;
		for (int i = 0; i < sarjaluku; i++) {
			int srj;
			if (IkkParam.ScrollSarjat[kScrollSarja] <= 0)
				kScrollSarja = 0;
			srj = IkkParam.ScrollSarjat[kScrollSarja] - 1;
			if (LA) {
				if (nilm[srj]-neil[srj] > 0)
					break;
				}
			else {
				if (!HylVal->Checked)
					if (ntulos[srj][IkkParam.Va] > 0)
						break;
				else {
					if (nhyl[srj] + nkesk[srj] > 0)
						break;
					}
				}
			kScrollSarja++;
			}
		for (int ii = 0; ii < SrjVal->Items->Count; ii++) {
			if (haesarja_w(SrjVal->Items->Strings[ii].c_str(), true) == IkkParam.ScrollSarjat[kScrollSarja]-1) {
				IkkParam.Sarja = IkkParam.ScrollSarjat[kScrollSarja]-1;
				SrjVal->ItemIndex = ii;
				break;
				}
			}
//		SrjVal->ItemIndex = IkkParam.ScrollSarjat[kScrollSarja]-1;
		PaivitaGrid(true);
		alkuhetki = mstimer();
		loppuhetki = 0;
		if ((n = TulosGrid->RowCount) < rc)
			ScrollSrjKesto = IkkParam.ScrollVali + IkkParam.ScrollRiviKesto * n;
		else
			ScrollSrjKesto = IkkParam.ScrollVali + rc*IkkParam.ScrollRiviKesto;
		return;
		}
	hetki = mstimer();
	if (IkkParam.ScrollType == 0) {
		if (!loppuhetki) {
			if ((n = TulosGrid->RowCount - TulosGrid->TopRow) < rc)
				loppuhetki = hetki + IkkParam.ScrollVali + IkkParam.ScrollRiviKesto * n;
			else
				loppuhetki = hetki + IkkParam.ScrollVali + rc*IkkParam.ScrollRiviKesto;
			}
		if (hetki < loppuhetki)
			return;
		if (TulosGrid->TopRow+rc >= TulosGrid->RowCount) {
			loppuhetki = 0;
			alkuhetki = 0;
			kScrollSarja++;
			return;
			}
		TulosGrid->TopRow += rc-1;
		TulosGrid->Refresh();
		loppuhetki = 0;
		return;
		}
	if (loppuhetki) {
		if (hetki < loppuhetki + IkkParam.ScrollVali || hetki < alkuhetki + ScrollSrjKesto)
			return;
		loppuhetki = 0;
		alkuhetki = 0;
		kScrollSarja++;
		return;
		}
	if (TulosGrid->TopRow+rc >= TulosGrid->RowCount-1) {
		if (IkkParam.ScrollType == 1)
			TulosGrid->Refresh();
		loppuhetki = hetki;
		return;
		}
	if (hetki < alkuhetki + IkkParam.ScrollVali)
		return;
	rh = TulosGrid->DefaultRowHeight;
	if (IkkParam.ScrollType == 1) {
		TulosGrid->ScrollBy(0, -1);
		}
	sc++;
	if (sc > rh) {
		TulosGrid->TopRow++;
		if (IkkParam.ScrollType == 2) {
			TulosGrid->Refresh();
			}
		else {
			TulosGrid->ScrollBy(0, rh);
			TScrollGrid *TulosCGrid = (TScrollGrid *)TulosGrid;
//			TulosCGrid->SInvalidateRow(TulosGrid->TopRow+rc-1);
			TulosCGrid->SInvalidateRow(TulosGrid->TopRow+rc);
//			TulosCGrid->SInvalidateRow(TulosGrid->TopRow+rc+1);
			}
		sc = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Skrollaa1Click(TObject *Sender)
{
	wchar_t ch;

	ScrollViive = IkkParam.ScrollRiviKesto / TulosGrid->DefaultRowHeight;
	int korj = (TulosGrid->ClientHeight+1) % (TulosGrid->DefaultRowHeight+1) - 3;
	Height -= korj;
	if (IkkParam.ScrollOn)
		IkkParam.ScrollOn = false;
	if (ScrollViive > 0) {
		if (IkkParam.ScrollSarjat[0] <= 0)
			IkkParam.ScrollSarjat[0] = IkkParam.Sarja+1;           //  Mikä on tämän tarkoitus ?
		kScrollSarja = 0;
		_beginthread(ScrollThread, 20480, (LPVOID)&TilanneFormNo);
		}
	Skrollaa1->Checked = IkkParam.ScrollOn;
	if (ScrollViive == 0) {
		FormSetup(false, false);
		PaivitaGrid(false);
		}
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Kokoruutu1Click(TObject *Sender)
{
	Kokoruutu1->Checked = !Kokoruutu1->Checked;
	IkkParam.KokoRuutu = Kokoruutu1->Checked;
	FormResize(Sender);
	int korj = (TulosGrid->ClientHeight+1) % (TulosGrid->DefaultRowHeight+1) - 3;
	Height -= korj;
	FormSetup(true, true);
}
//---------------------------------------------------------------------------


void __fastcall TTilanneForm::Lopetaskrollaus1Click(TObject *Sender)
{
	IkkParam.ScrollOn = false;
	Skrollaa1->Checked = IkkParam.ScrollOn;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::aukosarjanptteeksi1Click(TObject *Sender)
{
	wchar_t ch;

	IkkParam.ScrollVali = IkkParam.ScrollVali/1000;
	inputint_prompt(&IkkParam.ScrollVali, L"Tuloksen näytön minimikesto (sek)", &ch, this);
	IkkParam.ScrollVali = IkkParam.ScrollVali*1000;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Sarjanminimikesto1Click(TObject *Sender)
{
	wchar_t ch;

	IkkParam.ScrollRiviKesto = IkkParam.ScrollRiviKesto/100;
	inputint_prompt(&IkkParam.ScrollRiviKesto, L"Näytön lisäkesto 10 riviä kohti (sek)", &ch, this);
	IkkParam.ScrollRiviKesto = IkkParam.ScrollRiviKesto*100;
	ScrollViive = IkkParam.ScrollRiviKesto / TulosGrid->DefaultRowHeight;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Skrollauksensarjalista1Click(TObject *Sender)
{
	FormValSarjat = new TFormValSarjat(this);

	for (int srj = 0; srj < sarjaluku+nsarjayhd; srj++)
		FormValSarjat->ValitutSarjat[srj] = 0;
	for (int srj = 0; srj < sarjaluku+nsarjayhd; srj++) {
		if (IkkParam.ScrollSarjat[srj] > 0)
			FormValSarjat->ValitutSarjat[srj] = IkkParam.ScrollSarjat[srj];
		else
			break;
		}
	FormValSarjat->ShowModal();
	memset(IkkParam.ScrollSarjat, 0, sizeof(IkkParam.ScrollSarjat));
	for (int srj = 0; srj < sarjaluku+nsarjayhd; srj++) {
		if (FormValSarjat->ValitutSarjat[srj] > 0)
			IkkParam.ScrollSarjat[srj] = FormValSarjat->ValitutSarjat[srj];
		else
			break;
		}
	delete FormValSarjat;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Liukuva1Click(TObject *Sender)
{
	IkkParam.ScrollType = 1;
	Liukuva1->Checked = true;
	Rivittinaskeltava1->Checked = false;
	Ruutukerrallaan1->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Rivittinaskeltava1Click(TObject *Sender)
{
	IkkParam.ScrollType = 2;
	Liukuva1->Checked = false;
	Rivittinaskeltava1->Checked = true;
	Ruutukerrallaan1->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Ruutukerrallaan1Click(TObject *Sender)
{
	IkkParam.ScrollType = 0;
	Liukuva1->Checked = false;
	Rivittinaskeltava1->Checked = false;
	Ruutukerrallaan1->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Vainjulkiset1Click(TObject *Sender)
{
	bool chk = !Vainjulkiset1->Checked;
	if (chk)
		poistaPisteValinnat();
	Vainjulkiset1->Checked = chk;
	if (Vainjulkiset1->Checked) {
		IkkParam.minTaso = 9;
		Julkisuustasonmukaan1->Checked = true;
		IkkParam.vainjarj = false;
		}
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::aso11Click(TObject *Sender)
{
	bool chk = !aso11->Checked;
	if (chk)
		poistaPisteValinnat();
	aso11->Checked = chk;
	if (aso11->Checked) {
		IkkParam.minTaso = 1;
		Julkisuustasonmukaan1->Checked = true;
		IkkParam.vainjarj = false;
		}
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::aso21Click(TObject *Sender)
{
	bool chk = !aso21->Checked;
	if (chk)
		poistaPisteValinnat();
	aso21->Checked = chk;
	if (aso21->Checked) {
		IkkParam.minTaso = 2;
		Julkisuustasonmukaan1->Checked = true;
		IkkParam.vainjarj = false;
		}
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::aso31Click(TObject *Sender)
{
	bool chk = !aso31->Checked;
	if (chk)
		poistaPisteValinnat();
	aso31->Checked = chk;
	if (aso31->Checked) {
		IkkParam.minTaso = 3;
		Julkisuustasonmukaan1->Checked = true;
		IkkParam.vainjarj = false;
		}
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::poistaPisteValinnat(void)
{
	Julkisuustasonmukaan1->Checked = false;
	Vainjulkiset1->Checked = false;
	aso11->Checked = false;
	aso21->Checked = false;
	aso31->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::PisteClick(int piste)
{
	NaytaPiste[piste]->Checked = !NaytaPiste[piste]->Checked;
	if (NaytaPiste[piste]->Checked) {
		poistaPisteValinnat();
		Vainjrjestyspiste1->Checked = false;
		Pisteet1->Checked = true;
		IkkParam.vainjarj = false;
		}
	else {
		Kaikkiajat1->Checked = false;
		IkkParam.kaikkiajat = false;
		}
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Maali1Click(TObject *Sender)
{
	PisteClick(0);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N1vliaika1Click(TObject *Sender)
{
	PisteClick(1);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N2vliaika1Click(TObject *Sender)
{
	PisteClick(2);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N3vliaika1Click(TObject *Sender)
{
	PisteClick(3);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N4vliaika1Click(TObject *Sender)
{
	PisteClick(4);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N5vliaika1Click(TObject *Sender)
{
	PisteClick(5);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N6vliaika1Click(TObject *Sender)
{
	PisteClick(6);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N7vliaika1Click(TObject *Sender)
{
	PisteClick(7);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N8vliaka1Click(TObject *Sender)
{
	PisteClick(8);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N9vliaika1Click(TObject *Sender)
{
	PisteClick(9);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N10vliaikaA1Click(TObject *Sender)
{
	PisteClick(10);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N11vliaikaB1Click(TObject *Sender)
{
	PisteClick(11);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N12vliaikaC1Click(TObject *Sender)
{
	PisteClick(12);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N13vliaikaD1Click(TObject *Sender)
{
	PisteClick(13);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N14vliaikaE1Click(TObject *Sender)
{
	PisteClick(14);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N15vliaikaF1Click(TObject *Sender)
{
	PisteClick(15);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N16vliaikaG1Click(TObject *Sender)
{
	PisteClick(16);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N17vliaikaH1Click(TObject *Sender)
{
	PisteClick(17);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N18vliaikaI1Click(TObject *Sender)
{
	PisteClick(18);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N19vliaikaJ1Click(TObject *Sender)
{
	PisteClick(19);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N20vliaikaK1Click(TObject *Sender)
{
	PisteClick(20);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N21vliaikaL1Click(TObject *Sender)
{
	PisteClick(21);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N22vliaikaM1Click(TObject *Sender)
{
	PisteClick(22);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N23vliaikaN1Click(TObject *Sender)
{
	PisteClick(23);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N24vliaikaO1Click(TObject *Sender)
{
	PisteClick(24);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N25vliaikaP1Click(TObject *Sender)
{
	PisteClick(25);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N26vliaikaQ1Click(TObject *Sender)
{
	PisteClick(26);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N27vliaikaR1Click(TObject *Sender)
{
	PisteClick(27);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N28vliaikaS1Click(TObject *Sender)
{
	PisteClick(28);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N29vliaikaT1Click(TObject *Sender)
{
	PisteClick(29);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N30vliaikaU1Click(TObject *Sender)
{
	PisteClick(30);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Sulje1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::NaytapiilotetutClick(TObject *Sender)
{
	Naytapiilotetut->Checked = !Naytapiilotetut->Checked;
	IkkParam.naytaPiilotetut = Naytapiilotetut->Checked;
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::NytodotetutClick(TObject *Sender)
{
	Nytodotetut->Checked = !Nytodotetut->Checked;
	IkkParam.naytaOdotetut = Nytodotetut->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Er1Click(TObject *Sender)
{
	int arvo;
	wchar_t vast;

	arvo = IkkParam.EraVal;
	inputint_prompt(&arvo, L"Anna erän numero, 0: ei rajausta, -1: automaattinen valinta", &vast, this);
	if (vast != ESC)
		IkkParam.EraVal = arvo;
	if (IkkParam.EraVal >= 0)
		IkkParam.Era = IkkParam.EraVal;
	else
		IkkParam.Era = era_akt;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::LaskeyhteispisteetClick(TObject *Sender)
{
	Laskeyhteispisteet->Checked = !Laskeyhteispisteet->Checked;
	IkkParam.laskeYhtpisteet = Laskeyhteispisteet->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N1sekClick(TObject *Sender)
{
	N1sek->Checked = true;
	N01sek->Checked = false;
	N001sek->Checked = false;
	N0001sek->Checked = false;
	IkkParam.laika = 8;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N01sekClick(TObject *Sender)
{
	N1sek->Checked = false;
	N01sek->Checked = true;
	N001sek->Checked = false;
	N0001sek->Checked = false;
	IkkParam.laika = 10;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N001sekClick(TObject *Sender)
{
	N1sek->Checked = false;
	N01sek->Checked = false;
	N001sek->Checked = true;
	N0001sek->Checked = false;
	IkkParam.laika = 11;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::N0001sekClick(TObject *Sender)
{
	N1sek->Checked = false;
	N01sek->Checked = false;
	N001sek->Checked = false;
	N0001sek->Checked = true;
	IkkParam.laika = 12;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::allennamuotoilu1Click(TObject *Sender)
{
	for	(int c = 0; c < sizeof(ColLj)/sizeof(ColLj[0]); c++)
		if (ColLj[c] && ColLj[c] <= TulosGrid->ColCount)
			IkkParam.ColW[c] = (960 * TulosGrid->ColWidths[ColLj[c]-1]) /
					(Screen->PixelsPerInch * lev);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Nytkierrosaika1Click(TObject *Sender)
{
	Nytkierrosaika1->Checked = !Nytkierrosaika1->Checked;
	if (Nytkierrosaika1->Checked) {
		wchar_t ch;
		if (IkkParam.kierrosaika <= 0)
		   IkkParam.kierrosaika = 1 - IkkParam.kierrosaika;
		inputint_prompt(&IkkParam.kierrosaika, L"Kierroksen aikojen lukumäärä", &ch, this);
		}
	else if (IkkParam.kierrosaika > 0) {
		IkkParam.kierrosaika = 1 - IkkParam.kierrosaika;
		}

	if (IkkParam.kierrosaika > 0) {
		Erokrkeen1->Checked = false;
		IkkParam.eroaika = false;
		}
	if (!IkkParam.eroaika && IkkParam.kierrosaika <= 0) {
		IkkParam.kokotulos = true;
		}
	PaivitaGrid(false);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Nytviimeisinaika1Click(TObject *Sender)
{
	Nytviimeisinaika1->Checked = !Nytviimeisinaika1->Checked;
	IkkParam.viimeisin = Nytviimeisinaika1->Checked;
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Omaenntys1Click(TObject *Sender)
{
	Omaenntys1->Checked = !Omaenntys1->Checked;
	IkkParam.Ennatys = Omaenntys1->Checked ? 1 : 0;
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::avoiteaika1Click(TObject *Sender)
{
	avoiteaika1->Checked = !avoiteaika1->Checked;
	IkkParam.Tavoite = avoiteaika1->Checked ? 1 : 0;
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Sallikaikkiensarakkeidenst1Click(TObject *Sender)
{
	Sallikaikkiensarakkeidenst1->Checked = !Sallikaikkiensarakkeidenst1->Checked;
	FixedCols = !Sallikaikkiensarakkeidenst1->Checked;
	FormSetup(true, true);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Lhdnnimerkki1Click(TObject *Sender)
{
	wchar_t ch;
	inputint_prompt(&IkkParam.AaniLkm, L"Anna piippausten lukumäärä, 0: ei piippausta", &ch, this);
}
//---------------------------------------------------------------------------


void __fastcall TTilanneForm::Oletusvrit1Click(TObject *Sender)
{
	IkkParam.AktColor = 0;
	PaivitaGrid(false);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Mustapohja1Click(TObject *Sender)
{
	IkkParam.AktColor = 1;
	PaivitaGrid(false);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Omatvrit1Click(TObject *Sender)
{
	IkkParam.AktColor = 2;
	PaivitaGrid(false);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Lueomatvrit1Click(TObject *Sender)
{
	OpenDialog1->InitialDir = FormMain->CurrentDir;
	OpenDialog1->DefaultExt = L"csv";
	OpenDialog1->Filter = L"Teksti/csv-tiedosto (*.csv, *.txt)|*.csv;*.txt|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		int rv = 0;
		TextFl *InFl;
		InFl = new TextFl(OpenDialog1->FileName.c_str(), L"rt");
		while (InFl->IsOpen() && !InFl->Feof()) {
			wchar_t line[200];
			int i1 = 0, i2 = 0, i3 = 0;
			InFl->ReadLine(line, sizeof(line)/2-1);
			swscanf(line, L"%d;%d;%d;", &i1, &i2, &i3);
			ColorSet[2].Colors[rv] = 65536*i1 + 256 * i2 + i3;
			if (++rv >= sizeof(ColorSet[2].Colors) / sizeof(ColorSet[2].Colors[0]))
				break;
			}
		delete InFl;
		}
	memcpy(IkkParam.Colors, ColorSet[2].Colors, sizeof(IkkParam.Colors));
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Seuraasarjanviimeisisijoja1Click(TObject *Sender)
{
	Seuraasarjanviimeisisijoja1->Checked = !Seuraasarjanviimeisisijoja1->Checked;
	ViimeisetRivit = Seuraasarjanviimeisisijoja1->Checked;
	PaivitaGrid(false);
}
//---------------------------------------------------------------------------

