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

#include <time.h>
#include "mtrand.h"
#include "UnitArvonta.h"
#include "UnitSarjatiedot.h"
#include "UnitFIS.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormArvonta *FormArvonta;
extern TFormSarjatiedot *FormSarjatiedot;

void arvo(int Toiminto);
void arvonumerot(INT srj, bool yhtlahtoajat, bool kaikkipaivat, kilpindex *aktindex);
static int n_ix_jrj, ix_jrj[1000];


//---------------------------------------------------------------------------
__fastcall TFormArvonta::TFormArvonta(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormArvonta::FormShow(TObject *Sender)
{
	CBLAOsuudet->Clear();
	CBLAOsuudet->Clear();
	CBLAOsuudet->Items->Add(L"Kaikki osuudet");
	for (int os = 0; os < kilpparam.osuusluku; os++)
		CBLAOsuudet->Items->Add(UnicodeString(os-1)+L". osuus");
	CBLAOsuudet->ItemIndex = 0;
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
int tarkNumeroVapaus(int srj, int siirto)
{
	if (siirto == 0)
		return(1);
	for (int d = 1; d < datf2.numrec; d++) {
		kilptietue kilp;
		int kno, dd;
		kilp.getrec(d);
		if (kilp.kilpstatus != 0 || kilp.sarja != srj)
			continue;
		if ((kno = kilp.kilpno + siirto) < minkilpno || kno > maxkilpno) {
			Application->MessageBoxW(L"Siirto johtaisi liian pieneen tai liian suureen numeroon. Ei siirret‰",
				L"Este", MB_OK);
			return(1);
			}
		if ((dd = getpos(kno)) > 0) {
			kilp.getrec(dd);
			if (kilp.kilpstatus == 0 && kilp.sarja != srj) {
				Application->MessageBoxW(L"Siirto johtaisi samaan numeroon kahdella kilpailijalla. Ei siirret‰",
					L"Este", MB_OK);
				return(1);
				}
			}
		}
	return(0);
}
//---------------------------------------------------------------------------
int tarkNumerotila(bool kerro)
{
	wchar_t msg[5000] = L"";

	for (int i = 0; i < sarjaluku; i++) {
//		int nn = nilm[i];
//		if (FormArvonta->RGNumPaivat->ItemIndex == 1) {
//			nn = nilm_pv[FormArvonta->CBNumPaiva->ItemIndex][i];
//			}
		int sa;
		int sl;
		int ipv;

		if (!FormArvonta->SarjaValittu(i))
			continue;
		sa = Sarjat[i].sarjaalku;
		sl = sa + nilm[i] - 1;
		if (sl < sa)
			continue;
		for (int j = 0; j < sarjaluku; j++) {
			int sa1, n1;

			if (j == i)
				continue;
			n1 = nilm[j];
			sa1 = Sarjat[j].sarjaalku;
			if (n1 == 0)
				continue;
			if (sa <= sa1 && sa1 <= sl) {
				swprintf(msg+wcslen(msg), L"Sarjojen %s ja %s numeroalueet p‰‰llekk‰iset\n",
					Sarjat[i].sarjanimi, Sarjat[j].sarjanimi);
				}
			if (wcslen(msg) > sizeof(msg)/2 - 100)
				break;
			}
		if (wcslen(msg) > sizeof(msg)/2 - 100)
			break;
		}
	if (wcslen(msg) > 20) {
		Application->MessageBox(UnicodeString(msg).c_str(), L"Virhe", MB_OK);
		return(1);
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

static int teeRankiJarj(int Srj)
{
	typedef int int_2[2];
	int_2 *rjrj;
	kilptietue kilp;
	int d, n = 0;

	rjrj = new int_2[nilm[Srj]];
	for (d = 1; d < datf2.numrec; d++) {
		kilp.getrec(d);
		if (kilp.kilpstatus == 0 && kilp.sarja == Srj) {
			rjrj[n][0] = 0;
			for (int os = 0; os < Sarjat[Srj].osuusluku; os++)
				rjrj[n][0] += kilp.ostiet[os].pisteet[2];
			rjrj[n++][1] = d;
			}
		}
	qsortint(rjrj, n, 8, 0, 1);
	if (n > 1000)
		n = 1000;
	n_ix_jrj = n;
	for (int i = 0; i < n; i++)
		ix_jrj[i] = rjrj[i][1];
	delete[] rjrj;
	return(0);
}
//---------------------------------------------------------------------------
int __fastcall TFormArvonta::SeurPos(kilptietue *kilp, int Jrj, int Srj, int Ed)
{
	int d = -1, arv_pv0, ok = 1;
	char keyn[100], keyv[2];
	static kk;

	switch (Jrj) {
		case 0:
			if (Ed < 0)
				Ed = minkilpno;
			for (int i = Ed+1; i <= maxkilpno; i++) {
				if ((d = getpos(i)) > 0 && d < datf2.numrec) {
					kilp->getrec(d);
					if (kilp->sarja == Srj) {
						return(d);
						}
					}
				}
			d = -1;
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
				kilp->getrec(d);
				if (kilp->sarja == Srj)
					return(d);
				ok = !nextix(&nameindex, keyn, &d);
				}
			if (!ok) {
				d = -1;
				}
			break;
		case 2:
			if (Ed < 0)
				Ed = 0;
			for (int d = Ed+1; d < datf2.numrec; d++) {
				kilp->getrec(d);
				if (kilp->kilpstatus == 0 && kilp->sarja == Srj) {
					return(d);
					}
				}
			d = -1;
			break;
		case 3:
			if (Ed < 0) {
				teeRankiJarj(Srj);
				kk = 0;
				Ed = 0;
				}
			if (kk < n_ix_jrj)
				d = ix_jrj[kk++];
			else {
				d = -1;
				}
			break;
		case 4:
			if (Ed < 0)
				Ed = maxkilpno+1;
			for (int kno = Ed-1; kno >= minkilpno; kno--) {
				d = getpos(kno);
				if (d < 1)
					continue;
				kilp->getrec(d);
				if (kilp->sarja == Srj) {
					return(d);
					}
				}
			d = -1;
			break;
		}
	if (d > 0)
		kilp->getrec(d);
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

void __fastcall TFormArvonta::FormResize(TObject *Sender)
{
	LBSarjat->Height = ClientHeight - LBSarjat->Top - 8;
	LBSarjaVal->Height = ClientHeight - LBSarjaVal->Top - 8;
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
#define RNDDIVIDOR 131072L

void arvonumerot(UINT16 *numarr, int lkm)
   {
   MTRand_int32 irand;
   int (*sat)[2];

   sat = (int (*)[2]) calloc(lkm, 2*sizeof(int));
   for (int i = 0; i < lkm; i++) {
	  sat[i][0] = irand()/RNDDIVIDOR;
	  sat[i][1] = i;
	  }
   qsortint(sat, lkm, 2*sizeof(int), 0, 1);
   for (int i = 0; i < lkm; i++) {
	   numarr[i] = (UINT16) sat[i][1];
	   }
   free(sat);
   }
//---------------------------------------------------------------------------

int haeseurvapaa(int kno)
{
	return(kno+1);
}

int __fastcall TFormArvonta::varmista_vapaa(int kno, int d) {
	int ok = 0;
	if (kno < minkilpno || kno >= maxkilpno) {
		Application->MessageBoxW((UnicodeString(L"Siirto johtaisi liian pieneen tai suureen numeroon ")
			+ UnicodeString(kno) + L". Toiminto keskeytetty.").c_str(),
			L"Virhe", MB_OK);
		return(1);
		}
	if (ToimintaTila != 1) {
		int dd;
		if ((dd = getpos(kno)) > 0 && dd != d) {
			Application->MessageBoxW((UnicodeString(L"Siirto johtaisi numeroon ")
				+ UnicodeString(kno) + L"kahdella kilpailijalla. Toiminto keskeytetty.").c_str(),
				L"Virhe", MB_OK);
			return(1);
			}
		}
	return(ok);
}
//---------------------------------------------------------------------------
void __fastcall TFormArvonta::BtnNumeroiClick(TObject *Sender)
{
	bool NumKaikki, keskeyta = false;
	int Toiminto, d, d_in, k, kno, muutos;
	int JarjNo, jarj[9] = {1, 2, 2, 3, 2};
	kilptietue kilp;

	Sleep(100);
	UINT32 kirjheti0 = kirjheti;
	kirjheti = 0;
	Toiminto = RGNumToiminto->ItemIndex;
	if (Toiminto == 4) {
		muutos = _wtoi(EdMuutos->Text.c_str());
		}
	if (ToimintaTila != 1) {
		int este = 0;
		jarj[4] = 0;
		if (Toiminto == 4) {
			for (int srj = 0; srj < sarjaluku; srj++) {
				if (SarjaValittu(srj))
					if ((este = tarkNumeroVapaus(srj, muutos)) != 0)
						return;
				}
			}
		else {
			if (Application->MessageBoxW(L"Varoitus: Toteuta muutos vain, jos olet varma, "
				L"ett‰ siirto ei johda tilap‰isestik‰‰n kahdelle joukkueelle samaan numeroon. Toteutetaanko?",
				L"Varoitus", MB_YESNO) != IDYES)
				return;
			}
		}
	for (int srj = 0; srj < sarjaluku; srj++) {
		if (!SarjaValittu(srj))
			continue;
		switch (Toiminto) {
			case 0:                    // Numerointi sarjoittain aakkosj‰rjestykss‰
				kno = Sarjat[srj].sarjaalku;
				d = -1;
				EnterCriticalSection(&tall_CriticalSection);
				do {
					d = SeurPos(&kilp, 1, srj, d);
					if (d > 0) {
						kilp.getrec(d);
						if (varmista_vapaa(kno, d))
							break;
						kilp.kilpno = kno;
						tallenna(&kilp, d, 0, 0, 0, NULL);
						if (NumKaikki)
							kno ++;
						else
							kno = haeseurvapaa(kno);
						}
					} while(d > 0);
				LeaveCriticalSection(&tall_CriticalSection);
				break;
			case 1:                    // Numerointi sarjoittain tallennusj‰rjestykss‰
			case 2:                    // Arvo numerot
			case 3:                    // Rankisummaj‰rjestys
			case 4:                    // Numeroiden muuttaminen
				UINT16 nums[10000];
				if (Toiminto == 2) {
					arvonumerot(nums, nilm[srj]);
					kno = Sarjat[srj].sarjaalku+nums[0];
					k = 1;
					}
				else
					kno = Sarjat[srj].sarjaalku;
				d = -1;
				d_in = -1;
				JarjNo = jarj[Toiminto];
				if (Toiminto == 4 && muutos > 0 && JarjNo == 0)
					JarjNo = 4;
				EnterCriticalSection(&tall_CriticalSection);
				do {
					d = SeurPos(&kilp, JarjNo, srj, d_in);
					if (d > 0) {
						if (Toiminto == 4) {
							d_in = kilp.kilpno;
							kilp.kilpno += muutos;
							if (varmista_vapaa(kilp.kilpno, d))
								break;
							tallenna(&kilp, d, 0, 0, 0, NULL);
							}
						else {
							d_in = d;
							kilp.kilpno = kno;
							if (varmista_vapaa(kilp.kilpno, d))
								break;
							tallenna(&kilp, d, 0, 0, 0, NULL);
							if (Toiminto == 2) {
								kno = Sarjat[srj].sarjaalku+nums[k++];
								}
							else {
								if (NumKaikki)
									kno++;
								else
									kno = haeseurvapaa(kno);
								}
							}
						}
					} while(d > 0);
				LeaveCriticalSection(&tall_CriticalSection);
				break;
			}
		if (keskeyta)
			break;
		}
	kirjheti = kirjheti0;
	if (!keskeyta)
		Application->MessageBoxW(L"Toiminto toteutettu", L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Arvonta");
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

void __fastcall TFormArvonta::FormCreate(TObject *Sender)
{
	time_t  tl;
	unsigned long sd;

	sd = time(&tl);
	MTRand_int32 irand(sd);
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::BtnLahtoajatClick(TObject *Sender)
{
	int d, tlahto, toiminto, siirto, os_ala, os_yla;
	kilptietue kilp;

	LbAjatValmis->Visible = false;
	switch (RGAikatoiminto->ItemIndex) {
		case 0:
			toiminto = 0;
			break;
		case 1:
			toiminto = 1;
			siirto = SEK * _wtoi(EdtAikasiirto->Text.c_str());
			break;
		default:
			Application->MessageBoxW(L"Valitse toiminto", L"Este", MB_OK);
			return;
		}
	UINT32 kirjheti0 = kirjheti;
	kirjheti = 0;
	for	(d = 1; d < datf2.numrec; d++) {
		EnterCriticalSection(&tall_CriticalSection);
		kilp.getrec(d);
		if (kilp.kilpstatus || !SarjaValinnat[kilp.sarja])
			continue;
		if (CBLAOsuudet->ItemIndex == 0) {
			os_ala = 0;
			os_yla = Sarjat[kilp.sarja].osuusluku - 1;
			}
		else {
			os_ala = CBLAOsuudet->ItemIndex - 1;
			os_yla = os_ala;
			}
		for (int os = os_ala; os <= os_yla; os++) {
			if (toiminto == 0) {
				if (Sarjat[kilp.sarja].ylahto[os] != TMAALI0) {
					kilp.ostiet[os].ylahto = Sarjat[kilp.sarja].ylahto[os] +
						SEK * Sarjat[kilp.sarja].lahtovali[os] * (kilp.kilpno - Sarjat[kilp.sarja].sarjaalku);
					kilp.ostiet[os].lahtolaji = 2;
					}
				else if ((kilpparam.partio || kilpparam.rogaining) &&
					(tlahto = Sarjat[kilp.sarja].ylahto[0]) != TMAALI0 || (tlahto = Sarjat[kilp.sarja].lahto) != TMAALI0) {
					kilp.ostiet[os].ylahto = tlahto +
						SEK * Sarjat[kilp.sarja].lahtovali[0] * (kilp.kilpno - Sarjat[kilp.sarja].sarjaalku);
					kilp.ostiet[os].lahtolaji = 2;
					}
				}
			if (toiminto == 1) {
				if (kilp.ostiet[os].ylahto != TMAALI0)
					kilp.ostiet[os].ylahto += siirto;
				}
			}
		kilp.Tallenna(d, 0, 0, 0, 0);
		LeaveCriticalSection(&tall_CriticalSection);
		}
	kirjheti = kirjheti0;
	LbAjatValmis->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::RGAikatoimintoClick(TObject *Sender)
{
	EdtAikasiirto->Enabled = RGAikatoiminto->ItemIndex == 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::Button3Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::BtnFISpisteetClick(TObject *Sender)
{
   if (!FormFIS)
		FormFIS = new TFormFIS(this);
	FormFIS->Show();
	if (FormFIS->WindowState == wsMinimized)
		FormFIS->WindowState = wsNormal;
	FormFIS->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormArvonta::RGNumToimintoClick(TObject *Sender)
{
	BtnFISpisteet->Visible = RGNumToiminto->ItemIndex == 3;
}
//---------------------------------------------------------------------------

