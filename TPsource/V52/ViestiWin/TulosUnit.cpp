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
#include "VDeclare.h"
#include "UnitMain.h"
#include "UnitValSarjat.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTilanneForm *TilanneForm;
extern TTilanneForm *TilFrm[];

extern int ppi0;

//---------------------------------------------------------------------------
__fastcall TTilanneForm::TTilanneForm(TComponent* Owner)
   : TForm(Owner)
{
	Aktiivinen = true;
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
   InitializeCriticalSection(&tls_CriticalSection);

}
//---------------------------------------------------------------------------
__fastcall TTilanneForm::~TTilanneForm(void)
{
   DeleteCriticalSection(&tls_CriticalSection);
}
//---------------------------------------------------------------------------
void __fastcall TTilanneForm::FormCreate(TObject *Sender)
{
	if (kilpparam.maxnosuus > 1) {
	   CBkaikki->Enabled = true;
	   CBkaikki->Visible = true;
	   CBkaikki->Checked = true;
	   CB1puute->Enabled = true;
	   CB1puute->Visible = true;
	   CB1puute->Checked = false;
	   }
	if (kilpparam.maxnosuus > 2) {
	   CB2puute->Enabled = true;
	   CB2puute->Visible = true;
	   CB2puute->Checked = false;
	   }
	CNro = 1;
	CJoukkue = 3;
	CMaa = 4;
	CNimi = 2;
	CTulos = 5;
	CEro = 6;
	applyParams();
}
//---------------------------------------------------------------------------

seurIkkParamClass::seurIkkParamClass(void)
{
	memset(this, 0, sizeof(seurIkkParamClass));
	FontSize = 10;
	ScrollVali = 2000;
	ScrollRiviKesto = 300;
	ScrollType = 1;
	ColW[0] = 36;		// Sija
	ColW[1] = 36;		// Ed.sija
	ColW[2] = 36;		// Kilpno
	ColW[3] = 200;		// Joukkue
	ColW[4] = 35;		// Maa
	ColW[5] = 60 + 25*vuorokaudet;		// Tulos
	ColW[6] = (kilpparam.maxnosuus == 1 ? 140 : 320);		// Nimi
	ColW[7] = 55;		// Os.tulos
	ColW[8] = 40;		// Sakot
	ColW[9] = 36;		// Os.sija
	ColW[10] = 140;		// Seur.nimi
}
//---------------------------------------------------------------------------
void seurIkkParamClass::operator=(seurIkkParamClass& ikkPar)
{
	memcpy(this, &ikkPar, sizeof(seurIkkParamClass));
}
//---------------------------------------------------------------------------
int TTilanneForm::applyParams(void)
{
/*
	N1sek->Checked = IkkParam.laika <= 9;
	N01sek->Checked = IkkParam.laika == 10;
	N001sek->Checked = IkkParam.laika == 11;
	N001sek->Checked = IkkParam.laika >= 12;
*/
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
	Lihavoitu1->Checked = IkkParam.Lihavoitu;
	OsVal->ItemIndex = IkkParam.Osuus;
	PisteVal->ItemIndex = IkkParam.Va + 1;
	Viimeisimmat->Checked = IkkParam.Seuraa;
	EroVal->Checked = IkkParam.Ero;
	if (IkkParam.OsJarj)
		JarjVal->ItemIndex = 1;
	return(0);
}
//---------------------------------------------------------------------------
void __fastcall TTilanneForm::FormSetup(TObject *Sender)
{
	int iCol, wdt;

	EditAlusta->Text = " ";
	if (IkkParam.Sarja < 0 || IkkParam.Sarja >= SarjaVal->Items->Count)
		IkkParam.Sarja = SarjaVal->ItemIndex;
	IkkParam.Osuus = OsVal->ItemIndex;
	IkkParam.Va = PisteVal->ItemIndex-1;
	IkkParam.Seuraa = Viimeisimmat->Checked;
	IkkParam.Ero = EroVal->Checked;
	IkkParam.OsJarj = JarjVal->ItemIndex == 1 && IkkParam.Va == 0;

	nCol = 9;
	if (Sarjat[IkkParam.Sarja].maxpaikat) {
		nCol = 10;
		}
	if (IkkParam.Lihavoitu) {
		TulosGrid->Font->Style = TFontStyles() << fsBold;
		lev = 2;
		}
	else {
		TulosGrid->Font->Style = TFontStyles();
		lev = 0;
		}
	TulosGrid->DefaultRowHeight = 2 * (IkkParam.FontSize - 1) * Screen->PixelsPerInch/96;
	TulosGrid->Font->Size = IkkParam.FontSize;
	lev += IkkParam.FontSize;
	memset(ColLj, 0, sizeof(ColLj));

	if (kilpparam.maxnosuus == 1 && IkkParam.Osuus < Sarjat[IkkParam.Sarja].osuusluku-1) {
		TulosGrid->ColCount = ++nCol;
		TulosGrid->ColWidths[nCol-1] = IkkParam.ColW[10] * Screen->PixelsPerInch / ppi0;
		TulosGrid->Cells[nCol-1][0] = L"Seuraava";
		ColLj[nCol-1] = 11;
		}
	else
		TulosGrid->ColCount = nCol;
	iCol = 0;
	ColLj[iCol] = 1;
	TulosGrid->ColWidths[iCol++] = IkkParam.ColW[0] * Screen->PixelsPerInch * lev / 10 / ppi0;
	ColLj[iCol] = 2;
	TulosGrid->ColWidths[iCol++] = IkkParam.ColW[1] * Screen->PixelsPerInch * lev / 10 / ppi0;
	ColLj[iCol] = 3;
	TulosGrid->ColWidths[iCol++] = IkkParam.ColW[2] * Screen->PixelsPerInch * lev / 10 / ppi0;
	ColLj[iCol] = 4;
	TulosGrid->ColWidths[iCol++] = IkkParam.ColW[3] * Screen->PixelsPerInch * lev / 10 / ppi0;
	if (Maatunnus1->Checked) {
		ColLj[iCol] = 5;
		TulosGrid->ColWidths[iCol++] = IkkParam.ColW[4] * Screen->PixelsPerInch * lev / 10 / ppi0;
		}
	else
		TulosGrid->ColWidths[iCol++] = 0;
	ColLj[iCol] = 6;
	TulosGrid->ColWidths[iCol++] = IkkParam.ColW[5] * Screen->PixelsPerInch * lev / 10 / ppi0;
	ColLj[iCol] = 7;
	TulosGrid->ColWidths[iCol++] = IkkParam.ColW[6] * Screen->PixelsPerInch * lev / 10 / ppi0;
	ColLj[iCol] = 8;
	TulosGrid->ColWidths[iCol++] = IkkParam.ColW[7] * Screen->PixelsPerInch * lev / 10 / ppi0;
	if (Sarjat[IkkParam.Sarja].maxpaikat) {
		ColLj[iCol] = 9;
		TulosGrid->ColWidths[iCol++] = IkkParam.ColW[8] * Screen->PixelsPerInch * lev / 10 / ppi0;
		}
	ColLj[iCol] = 10;
	TulosGrid->ColWidths[iCol++] = IkkParam.ColW[9] * Screen->PixelsPerInch * lev / 10 / ppi0;
	iCol = 0;
	if (IkkParam.Va < 0) {
		TulosGrid->Cells[iCol++][0] = L"Nro";
		}
	else
		TulosGrid->Cells[iCol++][0] = L"Sija";
	TulosGrid->Cells[iCol++][0] = L"Ed.sj";
	if (IkkParam.Va < 0) {
		TulosGrid->Cells[iCol++][0] = L"Sija";
		}
	else
		TulosGrid->Cells[iCol++][0] = L"Nro";
	TulosGrid->Cells[iCol++][0] = L"Joukkue";
	if (Maatunnus1->Checked)
		TulosGrid->Cells[iCol++][0] = L"Maa";
	else
		TulosGrid->Cells[iCol++][0] = L"";
	TulosGrid->Cells[iCol++][0] = L"Tulos";
	TulosGrid->Cells[iCol++][0] = L"Nimi";
	if (kilpparam.alalaji == L'D' && IkkParam.Osuus == Sarjat[IkkParam.Sarja].osuusluku -1)
		TulosGrid->Cells[iCol++][0] = L"Kierr";
	else
		TulosGrid->Cells[iCol++][0] = L"Ostls";
	if (Sarjat[IkkParam.Sarja].maxpaikat) {
		TulosGrid->Cells[iCol++][0] = L"Sakot";
		}
	if (kilpparam.alalaji == L'D' && IkkParam.Osuus == Sarjat[IkkParam.Sarja].osuusluku -1)
		TulosGrid->ColCount--;
	else
		TulosGrid->Cells[iCol++][0] = UnicodeString((IkkParam.OsJarj ? L"Jk.sj" : L"Os.sj"));

	wdt = TulosGrid->Left + 24 * Screen->PixelsPerInch/96;
	for (int cb = 0; cb < TulosGrid->ColCount; cb++) {
		wdt += TulosGrid->ColWidths[cb] + 2;
		}
	ClientWidth = wdt;
}
//---------------------------------------------------------------------------
void __fastcall TTilanneForm::PaivBtnClick(TObject *Sender)
{
	PaivitaGrid(false);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::PaivitaGrid(bool scroll)
{
	int kno, nrivi, nTls, NimiJarj, aos, Osuus, Va, Sarja;
	int d, k, edT = 0, t, karki, lisa, KH = 0, iCol, lj;
//	char ln[80], st[40];
	wchar_t wst[200], wln[80];
	int p;
	kilptietue kilp;
	int peruutafl = 1;
	bool ensrivi = true;

	if (HylVal->Checked && (IkkParam.OsJarj || IkkParam.Va))
		return;
	EnterCriticalSection(&tls_CriticalSection);
	if (!paivitysflag) {
		paivitysflag = 1;
		peruutafl = 0;
		}
	LeaveCriticalSection(&tls_CriticalSection);
	if (peruutafl)
		return;
	FormSetup(this);
	Osuus = IkkParam.Osuus;
	Va = IkkParam.Va;
	Sarja = IkkParam.Sarja;

	aos = Sarjat[Sarja].aosuus[IkkParam.Osuus] + 1;
	NimiJarj = Etunimiensin1->Checked ? -1 : 1;

	swprintf(Kohde, L"%.10s-%.5s-%.12s", Sarjat[Sarja].Sarjanimi(wst),
		OsVal->Items->Strings[OsVal->ItemIndex].c_str(),
		PisteVal->Items->Strings[PisteVal->ItemIndex].c_str());
	FormMain->SetFormKohde(this, Kohde);

	if (Va == 0) {
		Caption = UnicodeString(Sarjat[Sarja].Sarjanimi(wst)) + L" - " + (Osuus == Sarjat[Sarja].osuusluku-1 ?
			UnicodeString(L"Maali") : UnicodeString(Osuus+1) + L". osuus - vaihto");
		}
	else if (Va > 0) {
		Caption = UnicodeString(Sarjat[Sarja].Sarjanimi(wst)) + L" - " + UnicodeString(Osuus+1) + L". osuus "
			+ Va + L". väliaika " + Sarjat[Sarja].vaMatka(Osuus, Va-1, wln) + L"km";
		}
	else
		Caption = UnicodeString(Sarjat[Sarja].Sarjanimi(wst)) + L" - " + UnicodeString(Osuus+1) + L". osuus";
	EnterCriticalSection(&tall_CriticalSection);
	if (Va < 0) {
		IkkParam.Seuraa = 0;
		nTls = nilm[Sarja];
		}
	else if (!HylVal->Checked) {
		if (IkkParam.OsJarj) {
			nTls = ntulosix[Sarja][Osuus][kilpparam.valuku+1];
			}
		else {
			if (Sarjat[Sarja].nosuus[Osuus] == 1)
				nTls = ntulosix[Sarja][Osuus][Va];
			else {
				lj = Va == 0 ? 0 : Va+4;
				p = jalku[Sarja][Osuus][lj];
				nTls = 0;
				for (int j = 0; j < ntulosix[Sarja][Osuus][Va]; j++) {
					if (p > 0) {
						GETREC(&kilp, p);
						t = kilp.Tulos(Sarjat[Sarja].aosuus[Osuus+1], Va, true);
						if (CBkaikki->Checked && t > 5 * SEK)
							nTls++;
						else if (CB1puute->Checked && t == SEK * (Sarjat[Sarja].nosuus[Osuus]-1))
							nTls++;
						else if (CB2puute->Checked && t == SEK * (Sarjat[Sarja].nosuus[Osuus]-2))
							nTls++;
						p = JarjSeur(0, Osuus, Va, p);
						}
					}
				}
			}
		}
	else {
		IkkParam.Seuraa = 0;
		nTls = nhyl[Sarja][Osuus] + nkesk[Sarja][Osuus];
		Va = 0;
		KH = 1;
		}

	if (!KH && IkkParam.Seuraa) {
		nrivi = TulosGrid->Height / (TulosGrid->DefaultRowHeight+1) - 1;
		}
	else
		nrivi = 100000;

	if (nrivi > nTls+1)
		nrivi = nTls+1;
	TulosGrid->RowCount = nrivi;

	if (nrivi == 1) {
		paivitysflag = 0;
		LeaveCriticalSection(&tall_CriticalSection);
		return;
	  }
	TulosGrid->FixedRows = (IkkParam.ScrollOn && IkkParam.ScrollType == 1) ? 0 : 1;
//	TulosGrid->FixedRows = 1;
	if (IkkParam.OsJarj) {
		merk_sijat(Sarja, Osuus, 0, 0);
		p = JosAlku(Sarja, Osuus, &aos);
		}
	else if (Va >= 0) {
		if (Va == 0 && Sarjat[Sarja].nosuus[Osuus] == 1)
			merk_sijat(Sarja, Osuus, kilpparam.valuku+1, 0);
		if (KH)
			lj = 1;
		else
			lj = Va == 0 ? 0 : Va+4;
		p = jalku[Sarja][Osuus][lj];
		}
	else
		kno = minkilpno-1;
	k = 1;

	for (int j = 0; j < nTls;) {
		int ok = 0;
		if (j && IkkParam.Seuraa && j <= nTls - nrivi) {
			j++;
			if (!IkkParam.OsJarj)
				p = JarjSeur(Yhd, Osuus, Va, p);
			else {
				p = OsJarjSeur(Yhd, Sarja, &aos, p);
				}
			continue;
			}
		if (k >= nrivi)
			break;
		if (Va < 0) {
			int dk;
			do {
				if (++kno > maxkilpno) {
					break;
					}
				if ((dk = getpos(kno)) > 0)
					GETREC(&kilp, dk);
				} while (dk <= 0 || kilp.sarja != Sarja);
			if (kno > maxkilpno) {
				break;
				}
			ok = 1;
			}
		else {
			if (!ensrivi || (KH && p < 0))
				p = askellasarja(Sarja, Osuus, &aos, IkkParam.OsJarj ? kilpparam.valuku+1 : Va, p, &lj);
			ensrivi = false;
			if (p > 0) {
				GETREC(&kilp, p);
//				p = JarjSeur(Yhd, Osuus, Va, p);
				ok = 1;
				}
			else
				break;
			}
		if (ok) {
			if (Va < 0 || !KH) {
				if (kilpparam.maxnosuus == 1) {
					if (!IkkParam.OsJarj)
						t = kilp.Tulos(Osuus, Va < 0 ? 0 : Va);
					else
						t = kilp.osTulos(Osuus, 0);
					}
				else {
					if (!IkkParam.OsJarj)
						t = kilp.Tulos(aos, Va < 0 ? 0 : Va, true);
					else
						t = kilp.osTulos(aos, 0);
					if (!IkkParam.OsJarj && (Sarjat[Sarja].nosuus[Osuus] > 1) &&
						 !(CBkaikki->Checked && t > 5 * SEK) &&
						 !(CB1puute->Checked && t == SEK * (Sarjat[Sarja].nosuus[Osuus]-1)) &&
						 !(CB2puute->Checked && t == SEK * (Sarjat[Sarja].nosuus[Osuus]-2)))
						 continue;
//					if ((Va < 0 || !IkkParam.OsJarj) && Sarjat[Sarja].nosuus[Osuus] > 1 && t < 6*SEK)
//						t = kilp.aTulos(Osuus, Va < 0 ? 0 : Va);
					}
				if (Va >= 0 && !j) {
					karki = t;
					if (IkkParam.Seuraa && j <= nTls - nrivi) {
						j++;
						continue;
						}
					}
				}
			if (Va < 0) {
				TulosGrid->Cells[0][k] = kilp.kilpno;
				TulosGrid->Cells[2][k] = kilp.Sija(Osuus, 0);
				}
			else {
				if (!KH && t != edT)
					TulosGrid->Cells[0][k] = j+1;
				else
					TulosGrid->Cells[0][k] = L" ";
				TulosGrid->Cells[2][k] = kilp.kilpno;
				}
			edT = t;
			if (Osuus > 0)
				TulosGrid->Cells[1][k] = psija(kilp.kilpno, Sarja, Osuus-1, 0);
			else
				TulosGrid->Cells[1][k] = L" ";
			TulosGrid->Cells[3][k] = kilp.Joukkue(wst, LSEURA+4);
			if (Maatunnus1->Checked)
				TulosGrid->Cells[4][k] = kilp.Maa(wst);
			else
				TulosGrid->Cells[4][k] = L"";
			if (!kilpparam.partio || Osuus == Sarjat[Sarja].ntosuus -1) {
				wln[0] = 0;
				if (Va < 0 || !KH) {
					lisa = 0;
					if (Va >= 0 && !IkkParam.OsJarj) {
						if (IkkParam.Ero && j > 0) {
							t -= karki;
							lisa = 1;
							wln[0] = L'+';
							}
						}
					else
						t = kilp.Tulos(aos, 0);
					if (vuorokaudet && t > 24*TUNTI) {
						if ((lisa)) {
							swprintf(wln, L"+%d %s", t / (24*TUNTI),
								aikatowstr_cols_n(wst, t % (24*TUNTI), 0, 0, kilpparam.laika2));
							}
						else {
							swprintf(wln, L"%02d %s", kilpparam.n_os+1 - t / (24*TUNTI),
								aikatowstr_cols_n(wst, t % (24*TUNTI), 0, 0, kilpparam.laika2));
							}
						wln[kilpparam.laika2+3] = 0;
						}
					else  {
						aikatowstr_cols_n(wln+lisa, t, 0, 0, kilpparam.laika2);
						elimwzb1(wln+lisa);
						}
					}
				if (Va < 0 || KH) {
					switch (kilp.wtTark(Osuus)) {
						case L'H' :
							wcscpy(wln, L"Hyl.");
							break;
						case L'K' :
							wcscpy(wln, L"Kesk.");
							break;
						case L'E' :
							wcscpy(wln, L"Eil.");
							break;
						case L'S' :
							wcscpy(wln, L"Sulj.");
							break;
						}
					}
				TulosGrid->Cells[5][k] = wln;
				}
			if (kilpparam.maxnosuus == 1 || IkkParam.OsJarj) {
				TulosGrid->Cells[6][k] = kilp.Nimi(wln, OSNIMIL, aos, NimiJarj);
				if (kilpparam.maxnosuus == 1 && Osuus < Sarjat[Sarja].osuusluku-1) {
					TulosGrid->Cells[nCol-1][k] = kilp.Nimi(wln, OSNIMIL, Osuus+1, NimiJarj);
					}
				if (kilpparam.alalaji == L'D' && Osuus == Sarjat[Sarja].osuusluku -1) {
					TulosGrid->Cells[7][k] = kilp.pisteet[0];
					}
				else {
					if (Va < 0 || !KH) {
						lisa = 0;
						if (Va < 0 || IkkParam.OsJarj) {
							t = kilp.osTulos(aos, 0, false);
							if (Va >= 0 && IkkParam.Ero && j > 0) {
								t -= karki;
								lisa = 1;
								wln[0] = L'+';
								}
							}
						else
							t = kilp.osTulos(Osuus, Va, false);
						aikatowstr_cols_n(wln+lisa, t, 0, 0, kilpparam.laika2);
						elimwzb1(wln+lisa);
						TulosGrid->Cells[7][k] = wln;
						iCol = 8;
						if (Sarjat[Sarja].maxpaikat) {
							TulosGrid->Cells[iCol++][k] = kilp.Ampsakot(Osuus, wst);
							}
						if (IkkParam.OsJarj) {
							TulosGrid->Cells[iCol++][k] =
								psija(kilp.kilpno, Sarja, Osuus, 0);
							}
						else {
							if (Va <= 0) {
								TulosGrid->Cells[iCol++][k] =
									psija(kilp.kilpno, Sarja, Osuus, kilpparam.valuku+1);
								}
							else
								TulosGrid->Cells[iCol++][k] = L" ";
							}
						}
					else {
						TulosGrid->Cells[7][k] = L" ";
						iCol = 8;
						if (Sarjat[Sarja].maxpaikat) {
							TulosGrid->Cells[iCol++][k] = L" ";
							}
						TulosGrid->Cells[iCol++][k] = L" ";
						}
					}
				}
			else  { // kilpparam.maxnosuus > 1 && !IkkParam.OsJarj
				int stlen = 0;
				for (int os = Sarjat[kilp.sarja].aosuus[Osuus] + 1;
					os <= Sarjat[kilp.sarja].aosuus[Osuus+1]; os++) {
					stlen += wcslen(kilp.Nimi(wst, OSNIMIL, os, -1))+1;
					}
				if (stlen > 50)
					stlen = (stlen - 50) / Sarjat[kilp.sarja].nosuus[Osuus];
				else
					stlen = 0;
				memset(wln, 0, sizeof(wln));
				for (int os = Sarjat[kilp.sarja].aosuus[Osuus] + 1;
					os <= Sarjat[kilp.sarja].aosuus[Osuus+1]; os++) {
					kilp.Nimi(wln+wcslen(wln), wcslen(kilp.Nimi(wst, OSNIMIL, os, -1)) - stlen, os, -1);
					wcscat(wln, L"/");
					}
				wln[wcslen(wln)-1] = 0;
				TulosGrid->Cells[6][k] = UnicodeString(wln);
				if (!KH) {
					lisa = 0;
					if (Sarjat[Sarja].nosuus[Osuus] > 1) {
						t = kilp.aTulos(Osuus, Va);
						if (t > 0 && Osuus > 0)
							t -= kilp.aTulos(Osuus-1, 0);
						}
					else {
						if (IkkParam.OsJarj) {
							t = kilp.osTulos(aos, 0);
							if (IkkParam.Ero && j > 0) {
								t -= karki;
								lisa = 1;
								wln[0] = L'+';
								}
							}
						else
							t = kilp.osTulos(aos, Va);
						}
					aikatowstr_cols_n(wln+lisa, t, 0, 0, kilpparam.laika2);
					elimwzb1(wln+lisa);
					TulosGrid->Cells[7][k] = UnicodeString(wln);
					if (Sarjat[Sarja].nosuus[Osuus] == 1) {
						TulosGrid->Cells[8][k] =
							UnicodeString(psija(kilp.kilpno, Sarja, aos, kilpparam.valuku+1));
						}
					else
						TulosGrid->Cells[8][k] = L" ";
/*
					else {
						if (!Va) {
							TulosGrid->Cells[8][k] =
								UnicodeString(psija(kilp.kilpno, Sarja, Osuus, kilpparam.valuku+1));
							}
						else
							TulosGrid->Cells[8][k] = UnicodeString(L" ");
						}
*/
					}
				else {
					TulosGrid->Cells[7][k] = UnicodeString(L" ");
					TulosGrid->Cells[8][k] = UnicodeString(L" ");
					}
				}
			j++;
			k++;
			}
		}
	TulosGrid->RowCount = k;
	LeaveCriticalSection(&tall_CriticalSection);
	paivitysflag = 0;
}
//---------------------------------------------------------------------------
void __fastcall TTilanneForm::OsValChange(TObject *Sender)
{
	PaivBtnClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TTilanneForm::PisteValChange(TObject *Sender)
{
	JarjVal->Visible = PisteVal->ItemIndex == 1;
	PaivBtnClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TTilanneForm::JarjValClick(TObject *Sender)
{
   PaivBtnClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TTilanneForm::EroValClick(TObject *Sender)
{
   PaivBtnClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TTilanneForm::ViimeisimmatClick(TObject *Sender)
{
   PaivBtnClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TTilanneForm::FormResize(TObject *Sender)
{
	if (!Kokoruutu1->Checked) {
		TulosGrid->Top = Panel1->Height;
		TulosGrid->Height = ClientHeight - TulosGrid->Top;
		TulosGrid->Width = ClientWidth;
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

void __fastcall TTilanneForm::FormActivate(TObject *Sender)
{
   SarjaVal->Clear();
	for (int srj = 0; srj < sarjaluku+nsarjayhd; srj++)
	  SarjaVal->Items->Add(Sarjat[srj].sarjanimi);
	if (IkkParam.Sarja < 0 || IkkParam.Sarja >= SarjaVal->Items->Count)
		IkkParam.Sarja = 0;
   SarjaVal->ItemIndex = IkkParam.Sarja;
   OsVal->Clear();
   for (int os = 1; os <= Sarjat[IkkParam.Sarja].ntosuus; os++)
	  OsVal->Items->Add(UnicodeString(os)+L". osuus");
	if (IkkParam.Osuus >= Sarjat[IkkParam.Sarja].ntosuus)
		IkkParam.Osuus = Sarjat[IkkParam.Sarja].ntosuus-1;
   OsVal->ItemIndex = IkkParam.Osuus;
   PisteVal->Clear();
	PisteVal->Items->Add(L"Osanottajat");
	PisteVal->Items->Add(L"Vaihto/Maali");
	for (int va = 1; va <= kilpparam.maxvaluku; va++)
		PisteVal->Items->Add(UnicodeString(va)+L". väliaika");
	PisteVal->ItemIndex = IkkParam.Va+1;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::SarjaValChange(TObject *Sender)
{
	IkkParam.Sarja = SarjaVal->ItemIndex;
	OsVal->Clear();
	if (IkkParam.Osuus >= Sarjat[IkkParam.Sarja].ntosuus)
		IkkParam.Osuus = Sarjat[IkkParam.Sarja].ntosuus-1;
	for (int os = 1; os <= Sarjat[IkkParam.Sarja].ntosuus; os++)
		OsVal->Items->Add(UnicodeString(os)+L". osuus");
	OsVal->ItemIndex = IkkParam.Osuus;
	PaivBtnClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::TulosGridDblClick(TObject *Sender)
{
	int d;

	if (TulosGrid->Row == 0)
		return;
	d = getpos(_wtoi(TulosGrid->Cells[2][TulosGrid->Row].c_str()));
	if (!FormJoukktiedot)
		FormJoukktiedot = new TFormJoukkuetiedot(this);
	FormJoukktiedot->sallimuokkausvalinta = false;
	FormJoukktiedot->naytaKilpailija(d);
	FormJoukktiedot->Show();
	if (FormJoukktiedot->WindowState == wsMinimized)
		FormJoukktiedot->WindowState = wsNormal;
	FormJoukktiedot->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::HylValClick(TObject *Sender)
{
   PaivBtnClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Suuri1Click(TObject *Sender)
{
	IkkParam.FontSize = 12;
	Suuri1->Checked = true;
	Normaali1->Checked = false;
	Pieni1->Checked = false;
	Muumerkkikoko1->Checked = false;
	FormSetup(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Normaali1Click(TObject *Sender)
{
	IkkParam.FontSize = 10;
	Suuri1->Checked = false;
	Normaali1->Checked = true;
	Pieni1->Checked = false;
	Muumerkkikoko1->Checked = false;
	FormSetup(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Pieni1Click(TObject *Sender)
{
	IkkParam.FontSize = 8;
	Suuri1->Checked = false;
	Normaali1->Checked = false;
	Pieni1->Checked = true;
	Muumerkkikoko1->Checked = false;
	FormSetup(Sender);
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
	FormSetup(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TTilanneForm::Lihavoitu1Click(TObject *Sender)
{
	Lihavoitu1->Checked = ! Lihavoitu1->Checked;
	IkkParam.Lihavoitu = Lihavoitu1->Checked;
	FormSetup(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Kokoruutu1Click(TObject *Sender)
{
	Kokoruutu1->Checked = !Kokoruutu1->Checked;
	FormResize(Sender);
	int korj = (TulosGrid->ClientHeight+1) % (TulosGrid->DefaultRowHeight+1) - 3;
	Height -= korj;
	FormSetup(Sender);
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
	while(lkmThreadOn && frm->IkkParam.ScrollOn && frm->ScrollViive) {
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
		for (int i = 0; i < sarjaluku+nsarjayhd; i++) {
			int srj;
			if (IkkParam.ScrollSarjat[kScrollSarja] <= 0)
				kScrollSarja = 0;
			srj = IkkParam.ScrollSarjat[kScrollSarja] - 1;
			if (!HylVal->Checked)
				if (ntulosix[srj][IkkParam.Osuus][IkkParam.Va] > 0)
					break;
			else {
				if (nhyl[srj][IkkParam.Osuus] + nkesk[srj][IkkParam.Osuus] > 0)
					break;
				}
			kScrollSarja++;
			}
		IkkParam.Sarja = IkkParam.ScrollSarjat[kScrollSarja]-1;
		if (IkkParam.Sarja < 0)
			IkkParam.Sarja = 0;
		SarjaVal->ItemIndex = IkkParam.Sarja;
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
			IkkParam.ScrollSarjat[0] = IkkParam.Sarja+1;
		kScrollSarja = 0;
		_beginthread(ScrollThread, 20480, (LPVOID)&TilanneFormNo);
		}
	Skrollaa1->Checked = IkkParam.ScrollOn;
	if (ScrollViive == 0) {
		FormSetup(Sender);
		PaivitaGrid(false);
		}
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Lopetaskrollaus1Click(TObject *Sender)
{
	IkkParam.ScrollOn = false;
	Skrollaa1->Checked = IkkParam.ScrollOn;
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

void __fastcall TTilanneForm::aukosarjanptteeksi1Click(TObject *Sender)
{
	wchar_t ch;
	int sV;

	sV = IkkParam.ScrollVali/1000;
	inputint_prompt(&sV, L"Tuloksen näytön minimikesto (sek)", &ch, this);
	IkkParam.ScrollVali = sV*1000;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Sarjanminimikesto1Click(TObject *Sender)
{
	wchar_t ch;
	int sK;

	sK = IkkParam.ScrollRiviKesto/100;
	inputint_prompt(&sK, L"Näytön lisäkesto 10 riviä kohti (sek)", &ch, this);
	IkkParam.ScrollRiviKesto = sK*100;
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

void __fastcall TTilanneForm::FormClose(TObject *Sender, TCloseAction &Action)
{
	IkkParam.ScrollOn = false;
	IkkParam.Seuraa = false;
	Aktiivinen = false;
	Sleep(20);
	if (FormJoukktiedot) {
		FormJoukktiedot->Close();
		delete FormJoukktiedot;
		FormJoukktiedot = NULL;
		}
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Etunimiensin1Click(TObject *Sender)
{
	Etunimiensin1->Checked = !Etunimiensin1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Maatunnus1Click(TObject *Sender)
{
	Maatunnus1->Checked = !Maatunnus1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::FormShow(TObject *Sender)
{
	Aktiivinen = true;
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::Suljekaavake1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TTilanneForm::allennasarakeleveydet1Click(TObject *Sender)
{
	for	(int c = 0; c < TulosGrid->ColCount; c++)
		if (ColLj[c])
			IkkParam.ColW[ColLj[c]-1] = (10 * ppi0 * TulosGrid->ColWidths[c]) /
					(Screen->PixelsPerInch * lev);
}
//---------------------------------------------------------------------------

