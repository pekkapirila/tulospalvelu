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
#pragma hdrstop

#include "UnitKilpailijatiedot.h"
#include "UnitKilpailijaOnline.h"
#include "UnitKilpailijaEmit.h"
#include "UnitSeurat.h"
#include "UnitEmithaku.h"
#include "WinHk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormKilpailijatiedot *FormKilpailijatiedot;

static const int VaiheCol =    0;
static const int BibCol =      1;
static const int SarjaCol =    2;
static const int BadgeCol =    3;
static const int LCol =        4;
static const int Badge2Col =   5;
static const int L2Col =       6;
static const int LahtoArvCol = 7;
static const int LahtoTotCol = 8;
static const int MaaliCol =    9;
static const int TulosCol =   10;
static const int TarkCol =    11;
static const int AmpSakotCol =12;
static const int SakkoCol =   13;
static const int SijaCol =    14;
static const int RataCol =    15;
static const int EraCol =     16;
static const int EnnatysCol = 17;
static const int TavoiteCol = 18;
static const int YhtTlsCol =  19;
static const int YSijaCol =   20;
static const int PistCol =    21;

ColTp PvGridCols0[] = {
	{L"Vaihe", 50, 0, 1, 0, true},
	{L"Bib", 40, 0, 1, 0, true},
	{L"Sarja", 50, 0, 1, 0, true},
	{L"Badge", 60, 0, 1, 0, true},
	{L"L", 15, 0, 1, 0, true},
	{L"Badge-2", 60, 0, 1, 0, true},
	{L"L2", 15, 0, 1, 0, true},
	{L"Lähtö-arv", 70, 0, 1, 0, true},
	{L"Lähtö-tot", 70, 0, 1, 0, true},
	{L"Maali", 70, 0, 1, 0, true},
	{L"Tulos", 70, 0, 1, 0, true},
	{L"Tark", 45, 0, 1, 0, true},
	{L"Amp.sakot", 60, 0, 1, 0, false},
	{L"Sakko", 45, 0, 1, 0, true},
	{L"Sija", 35, 0, 1, 0, true},
	{L"Rata", 50, 0, 1, 0, true},
	{L"Erä", 50, 0, 1, 0, false},
	{L"Ennätys", 60, 0, 1, 0, false},
	{L"Tavoite", 60, 0, 1, 0, false},
	{L"Yht.tls.", 60, 0, 1, 0, false},
	{L"Y-sija", 40, 0, 1, 0, false},
	{L"Pist-1", 40, 0, 1, 0, true}
};

static const char salausVal[2] = "YJ";
//---------------------------------------------------------------------------
__fastcall TFormKilpailijatiedot::TFormKilpailijatiedot(TComponent* Owner)
   : TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	Sarja = -1;
	PvPist = 0;
	ValPist = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtKilpnoChanged(TObject *Sender)
{
	int KilpNo, d;

	KilpNo = _wtoi(EdtKilpno->Text.c_str());
	if (_wtoi(EdtKilpno->Text.c_str()) == Kilp.id(ToimintaTila == 1 ? -1 : k_pv))
		return;
	if (EdtSeura->ReadOnly == true) {
	   if ((d = getpos(KilpNo, true)) > 0) {
		  dKilp = d;
		  Kilp.GETREC(d);
		  Kilp1 = Kilp;
		  Sarja = Kilp.Sarja();
		  nimihaku = false;
		  naytaTiedot();
		  }
	   return;
	   }
	if (Lisays && kilpparam.bibvaihtelee) {
		for (int i = -1; i < kilpparam.n_pv_akt; i++)
			Kilp.setId(haeSeurVapaaKno(i, KilpNo), i);
		}
	else if (Lisays) {
		Kilp.setIdAll(haeSeurVapaaKno(-1, KilpNo));
		}
	else {
		if (KilpNo < 0 || KilpNo > maxkilpno || (ToimintaTila == 2 && KilpNo < minkilpno)) {
			Application->MessageBoxW(L"Numero ei sallitulla alueella", MB_OK);
			return;
			}
		if (ToimintaTila == 2 && (d = getpos(KilpNo)) > 0) {
			if (d != dKilp)
				Application->MessageBoxW(L"Numero jo käytössä", MB_OK);
			return;
			}
		if (Application->MessageBoxW(L"Haluatko todella muuttaa kilpailijan numeroa",
			L"Vahvista", MB_YESNO) == IDYES) {
			Kilp.setId(KilpNo, ToimintaTila == 1 ? -1 : k_pv);
			}
		else
			EdtKilpno->Text = Kilp.id(ToimintaTila == 1 ? -1 : k_pv);
		}
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::naytaKilpailija(int d)
{
	if (ToimintaTila == 1) {
		RGSelaus->ItemIndex = 2;
		RGSelaus->Enabled = false;
		}
   if (d > 0) {
	  dKilp = d;
	  Kilp.GETREC(d);
	  Kilp1 = Kilp;
	  Sarja = Kilp.Sarja();
	  nimihaku = false;
	  naytaTiedot();
	  }
}//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::naytaTiedot(void)
	{
	wchar_t ch, cs, ch2, ct, line[80], trk;
	int i, n, d, ep, rvali, sj;
	int tl, tlsum;

	if (SrjVal->Items->Count != sarjaluku) {
		SrjVal->Clear();
		for (int srj = 0; srj < sarjaluku; srj++)
			SrjVal->Items->Add(Sarjat[srj].sarjanimi);
		SrjVal->ItemIndex = 0;
	}
	BtnSalli->Visible = sallimuokkausvalinta;
	BtnPeruuta->Visible = sallimuokkaus;
	BtnTallenna->Visible = sallimuokkaus;
	BtnPaivita->Visible = !sallimuokkaus;
	memset(line, 0, sizeof(line));
	EdtArvo->Visible = kilpparam.larvo != 0;
	LblArvo->Visible = kilpparam.larvo != 0;
	EdtYhd->Visible = kilpparam.lyhdistys != 0;
	LblYhd->Visible = kilpparam.lyhdistys != 0;
	EdtJoukkue->Visible = kilpparam.ljoukkue != 0;
	LblJoukkue->Visible = kilpparam.ljoukkue != 0;
	EdtSeuralyh->Visible = kilpparam.lseuralyh != 0;
	LblSeuralyh->Visible = kilpparam.lseuralyh != 0;
	Sarja = Kilp.Sarja();
	if (Lisays || Kilp.id() || (ToimintaTila == 1 && dKilp)) {
		if (SrjVal->ItemIndex != Sarja) {
			for (int i = 0; i < SrjVal->Items->Count; i++) {
				if (SrjVal->Items->Strings[i] == UnicodeString(Sarjat[Sarja].sarjanimi)) {
					SrjVal->ItemIndex = i;
					break;
					}
				}
			}
		Kilp.nimi(Kohde, sizeof(Kohde)/2-1, 0);
		FormMain->SetFormKohde(this, Kohde);
		sijat(Sarja);
		if (Lisays || kilpparam.sarjavaihtelee) {
			epv = 0;
			vpv = kilpparam.n_pv_akt - 1;
			}
		else {
			epv = Sarjat[Kilp.Sarja()].enspv;
			vpv = Sarjat[Kilp.Sarja()].viimpv;
			}
		EdtKilpno->Text = UnicodeString(Kilp.id());
		EdtPerusNo->Text = UnicodeString(Kilp.kilpno);
		EdtLisno->Text = UnicodeString(Kilp.lisno[0]);
		EdtKvId->Text = UnicodeString(Kilp.lisno[1]);
		EdtSukunimi->Text = UnicodeString(Kilp.sukunimi);
		EdtEtunimi->Text = UnicodeString(Kilp.etunimi);
		EdtArvo->Text = UnicodeString(Kilp.arvo);
		EdtMaa->Text = UnicodeString(Kilp.maa);
		EdtSeura->Text = UnicodeString(Kilp.seura);
		EdtSeuralyh->Text = UnicodeString(Kilp.seuralyh);
		EdtJoukkue->Text = UnicodeString(Kilp.joukkue);
		EdtPiiri->Text = UnicodeString(Kilp.piiri);
		EdtYhd->Text = UnicodeString(Kilp.yhdistys);
		EdPisteet->Text = UnicodeString(Kilp.pisteet[ValPist]);
		EdRankipisteet->Text = UnicodeString(Kilp.pisteet[2]);
		EdtSp->Text = UnicodeString(Kilp.sukup);
		switch (Kilp.ikasarja) {
			case 0:
				EdtIkasrj->Text = L"";
				break;
			case 1:
				EdtIkasrj->Text = L"YL";
				break;
			case 2:
				EdtIkasrj->Text = L"AV";
				break;
			default:
				EdtIkasrj->Text = UnicodeString(Kilp.ikasarja);
			}
		EdtSynt->Text = UnicodeString(Kilp.synt);
		CBsalanimi->ItemIndex = stschind(Kilp.pv[0].salaa, salausVal) + 1;
		PvGrid->DefaultRowHeight = 20 * Screen->PixelsPerInch / 96;
		PvGrid->RowCount = vpv - epv + 2;
		PvGrid->Height = (PvGrid->DefaultRowHeight+3) * (PvGrid->RowCount+1);
		PvGridCols[BibCol].Visible = kilpparam.bibvaihtelee;
		PvGridCols[SarjaCol].Visible = kilpparam.sarjavaihtelee;
		if (wcswcind(kilpparam.kilplaji, L"NPU") >= 0)
			PvGridCols[EraCol].Visible = true;
		PvGridCols[EnnatysCol].Visible = Nytomaenntys1->Checked;
		PvGridCols[TavoiteCol].Visible = Nyttavoitetulos1->Checked;
		if (vpv - epv > 0) {
			PvGridCols[YhtTlsCol].Visible = true;
			PvGridCols[YSijaCol].Visible = true;
			}
		PvGridCols[LahtoTotCol].Visible = false;
		for (int jpv = epv; jpv <= vpv; jpv++)
			if (pvparam[k_pv].hiihtolahto) {
				PvGridCols[LahtoTotCol].Visible = true;
				break;
			}
		PvGridCols[AmpSakotCol].Visible = Sarjat[Kilp.Sarja()].lsak[k_pv] > 0;
		PvGrid->ColCount = 5;
		n = 0;
		for (int i = 0; i < sizeof(PvGridCols0)/sizeof(ColTp); i++) {
			if (PvGridCols[i].Visible) {
				if (n >= PvGrid->ColCount)
					PvGrid->ColCount = n+1;
				PvGrid->Cells[n][0] = UnicodeString(PvGridCols[i].Caption);
				PvGrid->ColWidths[n] = PvGridCols[i].Width * Screen->PixelsPerInch / 96;
				PvGridCols[i].Order = n;
				n++;
				}
			}
		for (int pv = epv; pv <= vpv; pv++) {
			for (int i = 0; i < sizeof(PvGridCols0)/sizeof(ColTp); i++) {
				if (PvGridCols[i].Visible) {
					int col = PvGridCols[i].Order;
					ColIx[col] = i;
					if (i != VaiheCol && i != SarjaCol && Sarjat[Kilp.Sarja(pv)].luonne[pv] < 0) {
						PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] = L"";
						continue;
						}
					switch (i) {
						case VaiheCol:
							PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
								UnicodeString(pv+1)+L". vaihe";
							break;
						case BibCol:
							PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
								UnicodeString(Kilp.pv[pv].bib);
							break;
						case SarjaCol:
							PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
								Sarjat[Kilp.Sarja(pv)].sarjanimi;
							break;
						case BadgeCol:
							PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
								UnicodeString(Kilp.pv[pv].badge[0]);
							break;
						case LCol:
							PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
								UnicodeString(Kilp.pv[pv].laina[0]);
							break;
						case Badge2Col:
							PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
								UnicodeString(Kilp.pv[pv].badge[1]);
							break;
						case L2Col:
							PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
								UnicodeString(Kilp.pv[pv].laina[1]);
							break;
						case LahtoArvCol:
							tl = Kilp.pv[pv].tlahto;
							if (tl != TMAALI0) {
								AIKATOWSTRS(line, tl, t0);
								line[8] = 0;
								}
							else
								line[0] = 0;
								PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
								UnicodeString(line);
							break;
						case LahtoTotCol:
							tl = Kilp.pv[pv].va[0].vatulos;
							if (tl != TMAALI0) {
								AIKATOWSTRS(line, tl, t0);
								line[pvparam[pv].laika] = 0;
								}
							else
								line[0] = 0;
								PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
								UnicodeString(line);
							break;
						case MaaliCol:
							line[0] = 0;
							if (pv == k_pv) {
								tl = Kilp.maali(0);
								if (tl != TMAALI0) {
									AIKATOWSTRS(line, tl, t0);
									line[pvparam[k_pv].laika] = 0;
									}
								}
							PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
								UnicodeString(line);
							break;
						case TulosCol:
							tl = Kilp.tulos_pv(pv);
							if (tl != 0) {
								AIKATOWSTRS(line, tl, 0);
								line[pvparam[pv].laika] = 0;
								}
							else
								wcscpy(line , L"00.00.00");
							PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
								UnicodeString(line);
							break;
						case TarkCol:
							switch (Kilp.tark(pv)) {
								case L'K' :
									wcscpy(line, L"Kesk");
									break;
								case L'H' :
									wcscpy(line, L"Hyl");
									break;
								case L'M' :
									wcscpy(line, L"Virhe");
									break;
								case L'E' :
									wcscpy(line, L"Eiläht.");
									break;
								case L'B' :
									wcscpy(line, L"Havaittu");
									break;
								case L'T' :
									wcscpy(line, L"Tark.");
									break;
								case L'I' :
									wcscpy(line, L"esItys");
									break;
								case L'V' :
									wcscpy(line, L"Vakantti");
									break;
								case L'O' :
									wcscpy(line, L"Ohitettu");
									break;
								case L'X' :
									wcscpy(line, L"EiAikaa");
									break;
								case L'P' :
									wcscpy(line, L"Poissa");
									break;
								default :
									wcscpy(line, L"Avoin");
									break;
								}
							PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
								UnicodeString(line);
							break;
						case AmpSakotCol:
							PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] = Kilp.asakkoStr(pv);
							/*
							if (kilpparam.kilplaji == L'A') {
								PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
									UnicodeString(Kilp.pv[pv].asakot).SubString(1, Sarjat[Kilp.Sarja(pv)].lsak[pv]);
								}
							else {
								int ss = 0;
								for (int i = 0; i < Sarjat[Kilp.Sarja(pv)].lsak[Kilp.Sarja(pv)]; i++) {
									if (Kilp.pv[pv].asakot[i] > L'0')
										ss += Kilp.pv[pv].asakot[i] - L'0';
									}
								PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] = ss;
								}
							*/
							break;
						case SakkoCol:
							if (Sakkoinamsekkorjaus->Checked)
								PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] = Kilp.pv[pv].sakko;
							else if (Sarjat[Kilp.Sarja(pv)].rrtas > 1 && Sarjat[Kilp.Sarja(pv)].tsak[pv] > 0) {
								PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
									UnicodeString(Kilp.pv[pv].tasoitus/Sarjat[Kilp.Sarja(pv)].tsak[pv]);
								}
							else
								PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] = 0;
							break;
						case SijaCol:
							if (Kilp.tulos_pv(pv, true))
								PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
									UnicodeString(Kilp.sija(pv));
							else
								PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
									L"";
							break;
						case RataCol:
							PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
								UnicodeString(Kilp.pv[pv].rata);
							break;
						case EraCol:
							PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
								UnicodeString(Kilp.pv[pv].era);
							break;
						case EnnatysCol:
							PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
								Kilp.pv[pv].enn ?
									UnicodeString(aikatowstr_s(line, Kilp.pv[pv].enn/SEK, 0)) :
									UnicodeString(L"");
							break;
						case TavoiteCol:
							PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
								Kilp.pv[pv].tav ?
									UnicodeString(aikatowstr_s(line, Kilp.pv[pv].tav/SEK, 0)) :
									UnicodeString(L"");
							break;
						case YhtTlsCol:
							if (pv > epv) {
								tl = Kilp.ytulos(pv);
								if (tl != 0) {
									AIKATOWSTRS(line, tl, 0);
									line[pvparam[pv].laika] = 0;
									}
								else
									wcscpy(line , L"00.00.00");
								}
							else
								line[0] = 0;
							PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
								UnicodeString(line);
							break;
						case YSijaCol:
							if (pv > epv) {
								PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
									UnicodeString(Kilp.y_sija(pv));
								}
							break;
						case PistCol:
							PvGrid->Cells[PvGridCols[i].Order][pv-epv+1] =
								UnicodeString(Kilp.pv[pv].pvpisteet[PvPist]);
							break;
						}
					}
				}
			}
		}
	PvGrid->Row = (k_pv - epv + 1) == 0 ? 1 : (k_pv - epv + 1);
	}

//---------------------------------------------------------------------------
/*
void __fastcall TFormKilpailijatiedot::haeTiedot(kilptietue *Kilp1)
{
   wchar_t etu[60], suku[60];
   int srj;

	Kilp1->lisno[0] = _wtoi(EdtLisno->Text.c_str());
	Kilp1->lisno[1] = _wtoi(EdtKvId->Text.c_str());
	Kilp1->setSarja(SrjVal->ItemIndex);
	wcsncpy(Kilp1->sukunimi, EdtSukunimi->Text.c_str(), kilpparam.lsnimi);
	wcsncpy(Kilp1->etunimi, EdtEtunimi->Text.c_str(), kilpparam.lenimi);
	if (kilpparam.larvo)
		wcsncpy(Kilp1->arvo, EdtArvo->Text.c_str(), kilpparam.larvo);
	if (kilpparam.lseura)
		wcsncpy(Kilp1->seura, EdtSeura->Text.c_str(), kilpparam.lseura);
	if (kilpparam.lseuralyh)
		wcsncpy(Kilp1->seuralyh, EdtSeuralyh->Text.c_str(), kilpparam.lseuralyh);
	if (kilpparam.ljoukkue)
		wcsncpy(Kilp1->joukkue, EdtJoukkue->Text.c_str(), kilpparam.ljoukkue);
	if (kilpparam.lyhdistys)
		wcsncpy(Kilp1->yhdistys, EdtYhd->Text.c_str(), kilpparam.lyhdistys);
	wcsncpy(Kilp1->maa, EdtMaa->Text.c_str(), 3);
	Kilp1->piiri = _wtoi(EdtPiiri->Text.c_str());
	Kilp1->piste[0] = _wtoi(EdPisteet->Text.c_str());
	Kilp1->piste[2] = _wtoi(EdRankipisteet->Text.c_str());
	if (kilpparam.sarjavaihtelee) {
		epv = 0;
		vpv = kilpparam.n_pv_akt - 1;
		}
	else {
		epv = Sarjat[Kilp.Sarja()].enspv;
		vpv = Sarjat[Kilp.Sarja()].viimpv;
		}
	for (int pv = epv; pv <= vpv; pv++) {
		int ipv = pv-epv+1;
		for (int i = 1; i < sizeof(PvGridCols0)/sizeof(ColTp); i++) {
			if (PvGridCols[i].Visible && PvGrid->Cells[PvGridCols[i].Order][ipv].Length() > 0) {
				int k = PvGridCols[i].Order;
				switch (i) {
					case 1:
						Kilp1->pv[pv].bib = _wtoi(PvGrid->Cells[k][ipv].c_str());
						break;
					case 2:
						srj = haesarja_w(PvGrid->Cells[k][ipv].c_str(), false);
						if (srj >= 0)
							Kilp1->setSarja(srj, pv);
						break;
					case 3:
						Kilp1->pv[pv].badge[0] = _wtoi(PvGrid->Cells[k][ipv].c_str());
						break;
					case 4:
						Kilp1->pv[pv].laina[0] = FirstChar(PvGrid->Cells[k][ipv]);
						break;
					case 5:
						Kilp1->pv[pv].badge[1] = _wtoi(PvGrid->Cells[k][ipv].c_str());
						break;
					case 6:
						Kilp1->pv[pv].laina[1] = FirstChar(PvGrid->Cells[k][ipv]);
						break;
					case 7:
						Kilp1->pv[pv].tlahto = wstrtoaika_vap(PvGrid->Cells[k][ipv].c_str(), t0);
						if (!pvparam[k_pv].hiihtolahto)
							Kilp1->pv[pv].va[0].vatulos = Kilp1->pv[pv].tlahto;
						break;
					case 8:
						Kilp1->pv[pv].va[0].vatulos = wstrtoaika_vap(PvGrid->Cells[k][ipv].c_str(), t0);
						break;
					case 10:
						Kilp1->tall_tulos_pv(wstrtoaika_vap(PvGrid->Cells[k][ipv].c_str(), 0), pv);
						break;
					case 11:
						switch (towupper(FirstChar(PvGrid->Cells[k][ipv]))) {
							case L'E' :
								if (PvGrid->Cells[k][ipv].Length() > 1 && towupper(PvGrid->Cells[k][ipv][2]) != L'S')
									Kilp1->set_tark(L'E', pv);
								else
									Kilp1->set_tark(L'I', pv);
								break;
							case L'A' :
							case L'L' :
								Kilp1->set_tark(L'-', pv);
								break;
							default :
								Kilp1->set_tark(towupper(FirstChar(PvGrid->Cells[k][ipv])), pv);
								break;
							}
						break;
					case 12:
						wcsncpy(Kilp1->pv[pv].asakot, PvGrid->Cells[k][ipv].c_str(), Sarjat[Kilp1->Sarja()].lsak[pv]);
						break;
					case 13:
						Kilp1->pv[pv].tasoitus = _wtoi(PvGrid->Cells[k][ipv].c_str());
						break;
					case 15:
						wcsncpy(Kilp1->pv[pv].rata, PvGrid->Cells[k][ipv].c_str(), sizeof(Kilp1->pv[pv].rata)/2-1);
						break;
					case 18:
						Kilp1->pv[pv].pvpisteet[0] = _wtoi(PvGrid->Cells[k][ipv].c_str());
						break;
					}
				}
			}
		}
}
*/
//---------------------------------------------------------------------------

int __fastcall TFormKilpailijatiedot::tallennaTiedot(void)
   {
   kilptietue Kilp2;

   if (Lisays) {
//	   Kilp.setId(EdtKilpno->Text.ToInt());
//	   haeTiedot(&Kilp);
	   if (ToimintaTila > 1 && !Kilp.id() || getpos(Kilp.id()) > 0) {
			Application->MessageBox(L"Kilpailijanumero ei ole käytettävissä, ei tallenneta", L"Virhe",
				MB_OK);
			return(1);
			}
	   EnterCriticalSection(&tall_CriticalSection);
	   Kilp.addtall(&dKilp, 0);
	   LeaveCriticalSection(&tall_CriticalSection);
	   Kilp.GETREC(dKilp);
	   Kilp1 = Kilp;
	   return(0);
	   }
   if (Kilp1 == Kilp) {
	  Application->MessageBox(L"Kilpailijan tiedot eivät ole muuttuneet", L"Ei tallenneta",
		  MB_OK);
	  return(1);
	  }

   Kilp2.GETREC(dKilp);
   if (!(Kilp2 == Kilp1)) {
	  Application->MessageBox(L"Kilpailijan tietoja muutettu toisaalla, tietoja ei tallenneta", L"Este",
		  MB_OK);
	  return(1);
	  }

	if (Kilp1.id() != Kilp.id() && getpos(Kilp.id()) > 0) {
	  Application->MessageBox(L"Kilpailijanumero ei ole käytettävissä, ei tallenneta", L"Virhe",
		  MB_OK);
	  return(1);
	  }

   EnterCriticalSection(&tall_CriticalSection);
   Kilp.tallenna(dKilp, 0, 0, 0, &Kilp1);
   LeaveCriticalSection(&tall_CriticalSection);
   Kilp.GETREC(dKilp);
   Kilp1 = Kilp;
   return(0);
   }
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtPerusNoExit(TObject *Sender)
{
	if (sallimuokkaus)
		Kilp.kilpno = _wtoi(EdtPerusNo->Text.c_str());
	EdtPerusNo->Text = Kilp.kilpno;
}

//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtSukunimiExit(TObject *Sender)
{
	if (sallimuokkaus) {
		wmemset(Kilp.sukunimi, 0, LSNIMI);
		wcsncpy(Kilp.sukunimi, EdtSukunimi->Text.c_str(), kilpparam.lsnimi);
		}
	EdtSukunimi->Text = Kilp.sukunimi;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtEtunimiExit(TObject *Sender)
{
	if (sallimuokkaus) {
		wmemset(Kilp.etunimi, 0, LENIMI);
		wcsncpy(Kilp.etunimi, EdtEtunimi->Text.c_str(), kilpparam.lenimi);
		}
	EdtEtunimi->Text = Kilp.etunimi;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtArvoExit(TObject *Sender)
{
	if (sallimuokkaus) {
		wmemset(Kilp.arvo, 0, LARVO);
		wcsncpy(Kilp.arvo, EdtArvo->Text.c_str(), kilpparam.larvo);
		}
	EdtArvo->Text = Kilp.arvo;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtSeuraExit(TObject *Sender)
{
	if (sallimuokkaus) {
		wmemset(Kilp.seura, 0, LSEURA);
		wcsncpy(Kilp.seura, EdtSeura->Text.c_str(), kilpparam.lseura);
		}
	EdtSeura->Text = Kilp.seura;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtSeuralyhExit(TObject *Sender)
{
	if (sallimuokkaus) {
		wmemset(Kilp.seuralyh, 0, LLYH);
		wcsncpy(Kilp.seuralyh, EdtSeuralyh->Text.c_str(), kilpparam.lseuralyh);
		}
	EdtSeuralyh->Text = Kilp.seuralyh;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtYhdExit(TObject *Sender)
{
	if (sallimuokkaus) {
		wmemset(Kilp.yhdistys, 0, LYHD);
		wcsncpy(Kilp.yhdistys, EdtYhd->Text.c_str(), kilpparam.lyhdistys);
		}
	EdtYhd->Text = Kilp.yhdistys;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtJoukkueExit(TObject *Sender)
{
	if (sallimuokkaus) {
		wmemset(Kilp.joukkue, 0, LJK);
		wcsncpy(Kilp.joukkue, EdtJoukkue->Text.c_str(), kilpparam.ljoukkue);
		}
	EdtJoukkue->Text = Kilp.joukkue;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtMaaExit(TObject *Sender)
{
	if (sallimuokkaus) {
		wmemset(Kilp.maa, 0, 3);
		wcsncpy(Kilp.maa, EdtMaa->Text.c_str(), 3);
		}
	EdtMaa->Text = Kilp.maa;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::SrjValExit(TObject *Sender)
{
	if (sallimuokkaus) {
		Kilp.setSarja(SrjVal->ItemIndex);
		if (kilpparam.sarjavaihtelee) {
			if (Application->MessageBoxW(L"Asetetaanko kaikkien vaiheiden sarjat samaksi", L"Valitse", MB_YESNO) == IDYES) {
				Kilp.setSarjaAll(SrjVal->ItemIndex);
				}
			}
		naytaTiedot();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtLisnoExit(TObject *Sender)
{
	if (sallimuokkaus)
		Kilp.lisno[0] = _wtoi(EdtLisno->Text.c_str());
	EdtLisno->Text = Kilp.lisno[0];
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtKvIdExit(TObject *Sender)
{
	if (sallimuokkaus)
		Kilp.lisno[1] = _wtoi(EdtKvId->Text.c_str());
	EdtKvId->Text = Kilp.lisno[1];
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdPisteetExit(TObject *Sender)
{
	if (sallimuokkaus)
		Kilp.pisteet[ValPist] = _wtoi(EdPisteet->Text.c_str());
	EdPisteet->Text = Kilp.pisteet[ValPist];
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdRankipisteetExit(TObject *Sender)
{
	if (sallimuokkaus)
		Kilp.pisteet[2] = _wtoi(EdRankipisteet->Text.c_str());
	EdRankipisteet->Text = Kilp.pisteet[2];
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtPiiriExit(TObject *Sender)
{
	if (sallimuokkaus)
		Kilp.piiri = _wtoi(EdtPiiri->Text.c_str());
	EdtPiiri->Text = Kilp.piiri;
}
//---------------------------------------------------------------------------

int __fastcall TFormKilpailijatiedot::paivitaMuutos(int col, int row)
{
	int i, k, srj, ipv, upv;
	int tl, tle;
	wchar_t line[20], kh;

	k = row;
	ipv = row + epv - 1;
	if (ColIx[col] != SarjaCol && Sarjat[Kilp.Sarja(ipv)].luonne[ipv] < 0) {
		PvGrid->Cells[col][row] = L"";
		return(0);
		}
	switch (ColIx[col]) {
		case BibCol:
			Kilp.pv[ipv].bib = _wtoi(PvGrid->Cells[col][k].c_str());
			PvGrid->Cells[col][k] = UnicodeString(Kilp.pv[ipv].bib);
			break;
		case SarjaCol:
			if (kilpparam.sarjavaihtelee) {
				srj = haesarja_w(PvGrid->Cells[col][k].c_str(), false);
				if (srj >= 0)
					Kilp.setSarja(srj, ipv);
				}
			PvGrid->Cells[col][k] = Sarjat[Kilp.Sarja(ipv)].sarjanimi;
			break;
		case BadgeCol:
			tl = _wtoi(PvGrid->Cells[col][k].c_str());
			if (tl != Kilp.pv[ipv].badge[0]) {
				upv = ipv;
				if (ipv < vpv &&
					Application->MessageBoxW(L"Kopioidaanko muutos myöhempiin vaiheisiin", L"Valitse", MB_YESNO) == IDYES)
					upv = vpv;
				for (int jpv = ipv; jpv <= upv; jpv++) {
					Kilp.pv[jpv].badge[0] = tl;
					PvGrid->Cells[col][k+jpv-ipv] = Kilp.pv[jpv].badge[0];
					}
				}
			PvGrid->Cells[col][k] = Kilp.pv[ipv].badge[0];
			break;
		case LCol:
			Kilp.pv[ipv].laina[0] = FirstChar(PvGrid->Cells[col][k]);
			PvGrid->Cells[col][k] = Kilp.pv[ipv].laina[0];
			break;
		case Badge2Col:
			tl = _wtoi(PvGrid->Cells[col][k].c_str());
			if (tl != Kilp.pv[ipv].badge[1]) {
				upv = ipv;
				if (ipv < vpv &&
					Application->MessageBoxW(L"Kopioidaanko muutos myöhempiin vaiheisiin", L"Valitse", MB_YESNO) == IDYES)
					upv = vpv;
				for (int jpv = ipv; jpv <= upv; jpv++) {
					Kilp.pv[jpv].badge[1] = tl;
					PvGrid->Cells[col][k+jpv-ipv] = Kilp.pv[jpv].badge[1];
					}
				}
			PvGrid->Cells[col][k] = Kilp.pv[ipv].badge[1];
			break;
		case L2Col:
			Kilp.pv[ipv].laina[1] = FirstChar(PvGrid->Cells[col][k]);
			PvGrid->Cells[col][k] = Kilp.pv[ipv].laina[1];
			break;
		case LahtoArvCol:
			wcsncpy(line,PvGrid->Cells[col][k].c_str(), 10);
			line[10] = 0;
			elimwbl(line);
			if (line[0] == 0 || line[0] == L':') {
				tl = TMAALI0;
				}
			else {
				int np = 0;
				for (wchar_t *p = line; *p; p++) {
					if (*p == L'.' || *p == L':' || *p == L'/') {
						*p = L':';
						np++;
						}
					if (*p == L',') {
						np++;
						}
					}
				if (np == 1)
					wcscat(line, L":00");
				tl = wstrtoaika_vap(line, t0);
				}
			PvGrid->Cells[col][k] = UnicodeString(AIKATOWSTRS(line, tl, t0)).SubString(1, 8);
			if ((tle = tl - Kilp.pv[ipv].tlahto) != 0) {
				if (!pvparam[ipv].hiihtolahto || Kilp.pv[ipv].va[0].vatulos == Kilp.pv[ipv].tlahto) {
					Kilp.pv[ipv].tlahto = tl;
					Kilp.pv[ipv].va[0].vatulos = tl;
					if (ipv == k_pv && Kilp.p_aika(0) != 0) {
						Kilp.tall_tulos(0, Kilp.p_aika(0) - tle);
						if (PvGridCols[TulosCol].Visible)
							PvGrid->Cells[PvGridCols[TulosCol].Order][k] = UnicodeString(AIKATOWSTRS(line, Kilp.p_aika(0) , 0)).SubString(1, pvparam[k_pv].laika);
						}
					if (PvGridCols[LahtoTotCol].Visible)
						PvGrid->Cells[PvGridCols[LahtoTotCol].Order][k] = UnicodeString(AIKATOWSTRS(line, tl, t0)).SubString(1, pvparam[k_pv].laika);
					}
				else
					Kilp.pv[ipv].tlahto = tl;
				}
			break;
		case LahtoTotCol:
			if (pvparam[ipv].hiihtolahto) {
				wcsncpy(line,PvGrid->Cells[col][k].c_str(), 10);
				line[10] = 0;
				elimwbl(line);
				if (line[0] == 0 || line[0] == L':') {
					tl = TMAALI0;
					}
				else {
					int np = 0;
					for (wchar_t *p = line; *p; p++) {
						if (*p == L'.' || *p == L':' || *p == L'/') {
							*p = L':';
							np++;
							}
						if (*p == L',') {
							np++;
							}
						}
					if (np == 1)
						wcscat(line, L":00");
					tl = wstrtoaika_vap(line, t0);
					}
				PvGrid->Cells[col][k] = UnicodeString(AIKATOWSTRS(line, tl, t0)).SubString(1, pvparam[k_pv].laika);
				if ((tle = tl - Kilp.TLahto(ipv)) != 0) {
					Kilp.pv[ipv].va[0].vatulos = tl;
					if (ipv == k_pv && Kilp.p_aika(0) != 0) {
						Kilp.tall_tulos(0, Kilp.p_aika(0) - tle);
						if (PvGridCols[TulosCol].Visible)
							PvGrid->Cells[PvGridCols[TulosCol].Order][k] = UnicodeString(AIKATOWSTRS(line, Kilp.p_aika(0) , 0)).SubString(1, pvparam[k_pv].laika);
						}
					}
				}
			else
				PvGrid->Cells[col][k] = UnicodeString(AIKATOWSTRS(line, Kilp.TLahto(ipv), t0)).SubString(1, pvparam[k_pv].laika);
			break;
		case MaaliCol:
			tl = wstrtoaika_vap(PvGrid->Cells[col][k].c_str(), t0);
			if (ipv == k_pv && PvGridCols[TulosCol].Visible && tl != Kilp.maali(0)) {
				PvGrid->Cells[col][k] = UnicodeString(AIKATOWSTRS(line, tl, t0)).SubString(1, pvparam[k_pv].laika);
				Kilp.set_tulos(0, tl);
				PvGrid->Cells[PvGridCols[TulosCol].Order][k] = UnicodeString(AIKATOWSTRS(line, Kilp.p_aika(0) , 0)).SubString(1, pvparam[k_pv].laika);
				}
			break;
		case TulosCol:
			tl = wstrtoaika_vap(PvGrid->Cells[col][k].c_str(), 0);
			PvGrid->Cells[col][k] = UnicodeString(AIKATOWSTRS(line, tl, 0)).SubString(1, pvparam[ipv].laika);
//			if (ipv == k_pv && PvGridCols[9].Visible && tl != Kilp.p_aika(0)) {
			if (tl && ipv == k_pv && Kilp.p_aika(0))
				tl -= Kilp.p_aika(0, 0) - Kilp.p_aika(0, 0, 2);
			if (tl != Kilp.tulos_pv(ipv, false, 0)) {
				Kilp.tall_tulos_pv(tl, ipv, 0);
				if (PvGridCols[MaaliCol].Visible && ipv == k_pv)
					PvGrid->Cells[PvGridCols[MaaliCol].Order][k] = UnicodeString(AIKATOWSTRS(line, Kilp.maali(0), t0)).SubString(1, pvparam[k_pv].laika);
				}
			break;
		case TarkCol:
			switch (towupper(FirstChar(PvGrid->Cells[col][k]))) {
				case L'E' :
					if (PvGrid->Cells[col][k].Length() > 1 && towupper(PvGrid->Cells[col][k][2]) == L'S')
						kh = L'I';
					else if (PvGrid->Cells[col][k].Length() > 2 && towupper(PvGrid->Cells[col][k][3]) == L'A')
						kh = L'X';
					else
						kh = L'E';
					break;
				case L'V' :
					if (PvGrid->Cells[col][k].Length() > 1 && towupper(PvGrid->Cells[col][k][2]) == L'I')
						kh = L'M';
					else
						kh = L'V';
					break;
				case L'H' :
					if (PvGrid->Cells[col][k].Length() > 1 && towupper(PvGrid->Cells[col][k][2]) == L'A')
						kh = L'B';
					else
						kh = L'H';
					break;
				case L'A' :
				case L'L' :
					kh = L'-';
					break;
				default :
					kh = towupper(FirstChar(PvGrid->Cells[col][k]));
					break;
				}
			switch (kh) {
				case L'K' :
					wcscpy(line, L"Kesk");
					break;
				case L'X' :
					wcscpy(line, L"EiAikaa");
					break;
				case L'H' :
					wcscpy(line, L"Hyl");
					break;
				case L'M' :
					wcscpy(line, L"Virhe");
					break;
				case L'E' :
					wcscpy(line, L"Eiläht.");
					break;
				case L'B' :
					wcscpy(line, L"Havaittu");
					break;
				case L'T' :
					wcscpy(line, L"Tark.");
					break;
				case L'I' :
					wcscpy(line, L"esItys");
					break;
				case L'V' :
					wcscpy(line, L"Vakantti");
					break;
				case L'O' :
					wcscpy(line, L"Ohitettu");
					break;
				case L'P' :
					wcscpy(line, L"Poissa");
					break;
				default :
					wcscpy(line, L"Avoin");
					break;
				}
			PvGrid->Cells[col][k] = UnicodeString(line);
			Kilp.set_tark(kh, ipv);
			break;
		case AmpSakotCol:
			if (Sarjat[Kilp.Sarja(ipv)].lsak[ipv] > 0) {
				Kilp.tall_asakot(PvGrid->Cells[col][k].c_str(), ipv);
/*
				wcsncpy(line, PvGrid->Cells[col][k].c_str(), Sarjat[Kilp.Sarja(ipv)].lsak[ipv]+1);
				line[Sarjat[Kilp.Sarja(ipv)].lsak[ipv]] = 0;
				for (int is = 0; is < Sarjat[Kilp.Sarja(ipv)].lsak[ipv]; is++) {
					if (line[is] < L'0' || (kilpparam.kilplaji == L'A' && line[is] > L'0'+kilpparam.maxasakko))
						line[is] = L'-';
					}
				Kilp.tall_asakot(line, ipv);
				PvGrid->Cells[col][k] = line;
*/
				PvGrid->Cells[col][k] = Kilp.asakkoStr(ipv);
				if (PvGridCols[TulosCol].Visible)
					PvGrid->Cells[PvGridCols[TulosCol].Order][k] = UnicodeString(AIKATOWSTRS(line, Kilp.tulos_pv(ipv), 0)).SubString(1, pvparam[ipv].laika);
				}
			break;
		case SakkoCol:
			if (Sakkoinamsekkorjaus->Checked) {
				Kilp.pv[ipv].sakko = _wtoi(PvGrid->Cells[col][k].c_str());
				}
			else if (Sarjat[Kilp.Sarja(ipv)].rrtas > 1 && Sarjat[Kilp.Sarja(ipv)].tsak[ipv] > 0) {
				tl = _wtoi(PvGrid->Cells[col][k].c_str()) * Sarjat[Kilp.Sarja(ipv)].tsak[ipv];
				PvGrid->Cells[col][k] = UnicodeString(tl/Sarjat[Kilp.Sarja(ipv)].tsak[ipv]);
				Kilp.tall_sakko(tl, ipv);
				if (PvGridCols[TulosCol].Visible)
					PvGrid->Cells[PvGridCols[TulosCol].Order][k] = UnicodeString(AIKATOWSTRS(line, Kilp.tulos_pv(ipv), 0)).SubString(1, pvparam[ipv].laika);
				}
			else
				PvGrid->Cells[col][k] = L"0";
			break;
		case RataCol:
			memset(Kilp.pv[ipv].rata, 0, sizeof(rata));
			wcsncpy(Kilp.pv[ipv].rata, PvGrid->Cells[col][k].UpperCase().c_str(), sizeof(Kilp.pv[0].rata)/2-1);
			PvGrid->Cells[col][k] = Kilp.pv[ipv].rata;
			break;
		case EraCol:
			Kilp.pv[ipv].era = _wtoi(PvGrid->Cells[col][k].c_str());
			PvGrid->Cells[col][k] = Kilp.pv[ipv].era;
			break;
		case EnnatysCol:
			Kilp.pv[ipv].enn = wstrtoaika_vap(PvGrid->Cells[col][k].c_str(), 0);
			PvGrid->Cells[col][k] =
				Kilp.pv[ipv].enn ?
					UnicodeString(aikatowstr_s(line, Kilp.pv[ipv].enn/SEK, 0)) :
					UnicodeString(L"");
			break;
		case TavoiteCol:
			Kilp.pv[ipv].tav = wstrtoaika_vap(PvGrid->Cells[col][k].c_str(), 0);
			PvGrid->Cells[col][k] =
				Kilp.pv[ipv].tav ?
					UnicodeString(aikatowstr_s(line, Kilp.pv[ipv].tav/SEK, 0)) :
					UnicodeString(L"");
			break;
		case PistCol:
			Kilp.pv[ipv].pvpisteet[PvPist] = _wtoi(PvGrid->Cells[col][k].c_str());
			break;
		}
	return(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::SrjKarkiBtn(TObject *Sender)
{
	int p = -1, lj = 0;

	if (nilm[Sarja] > 0) {
		if (ToimintaTila == 2) {
			EnterCriticalSection(&tall_CriticalSection);

			for (lj = 0; lj < 5; lj++)
				if ((p = jalku[Sarja][lj]) != 0)
					break;
			if (p > 0) {
				dKilp = p;
				Kilp.GETREC(p);
				Kilp1 = Kilp;
				Sarja = Kilp.Sarja();
				nimihaku = false;
				naytaTiedot();
				}
			LeaveCriticalSection(&tall_CriticalSection);
			}
		else if (Sarja != Kilp.Sarja()) {
			kilptietue kilp;
			for (p = 1; p < nrec; p++) {
				kilp.GETREC(p);
				if (kilp.kilpstatus == 0 && kilp.Sarja() == Sarja) {
					dKilp = p;
					Kilp.GETREC(p);
					Kilp1 = Kilp;
					Sarja = Kilp.Sarja();
					naytaTiedot();
					}
				}
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::SeurBtnClick(TObject *Sender)
{
   int sj;
	int p = -1;

   if (dKilp <= 0)
	  return;
	EnterCriticalSection(&tall_CriticalSection);
	switch (RGSelaus->ItemIndex) {
		case 0:
		   p = JarjSeur(0, 0, dKilp);
		   nimihaku = false;
		   break;
		case 1:
			getpos(Kilp.id());
			p = nextpos();
/*
			for (int k = Kilp.id()+1; k <= maxkilpno; k++) {
				if ((p = getpos(k)) > 0) {
					nimihaku = false;
					break;
					}
				}
*/
			break;
		case 2:
			wchar_t st[61];
			char keyn[MAXINDL+1];

			if (!nimihaku) {
				keyaak(&Kilp, keyn, nameindex.keyl, 0);
				searchix(&nameindex, keyn, &p);
				}
			nimihaku = true;
			if (nextix(&nameindex, keyn, &p)) {
				nextix(&nameindex, keyn, &p);
				previx(&nameindex, keyn, &p);
				}
			break;
		}
	if (p >= 0) {
		Kilp.GETREC(p);
		Kilp1 = Kilp;
		dKilp = p;
		Sarja = Kilp.Sarja();
		naytaTiedot();
		}
	LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdBtnClick(TObject *Sender)
{
	int sj;
	int p = -1, p1;

	if (dKilp <= 0)
		return;
	EnterCriticalSection(&tall_CriticalSection);
	switch (RGSelaus->ItemIndex) {
		case 0:
			sj = Kilp.p_sija(0);
			p = -1;
			if (sj > 1 && sj <= ntulos[Sarja][0]) {
				p = jalku[Sarja][0];
				while ((p1 = JarjSeur(0, 0, p)) >= 0 && p1 != dKilp)
					p = p1;
				if (p1 != dKilp)
					p = -1;
				}
			nimihaku = false;
			break;
		case 1:
			getpos(Kilp.id());
			p = prevpos();
/*
			for (int k = Kilp.id()-1; k >= minkilpno; k--) {
				if ((p = getpos(k)) > 0) {
					nimihaku = false;
					break;
					}
				}
*/
			break;
		case 2:
			wchar_t st[61];
			char keyn[MAXINDL+1];

			if (!nimihaku) {
				keyaak(&Kilp, keyn, nameindex.keyl, 0);
				searchix(&nameindex, keyn, &p);
				}
			nimihaku = true;
			if (previx(&nameindex, keyn, &p)) {
				previx(&nameindex, keyn, &p);
				nextix(&nameindex, keyn, &p);
				}
			break;
		}
	if (p >= 0) {
		Kilp.GETREC(p);
		Kilp1 = Kilp;
		dKilp = p;
		Sarja = Kilp.Sarja();
		naytaTiedot();
		}
	LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtKilpnoKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
	if (Key == VK_RETURN)
		EdtKilpnoChanged(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::SrjValChange(TObject *Sender)
{
	if (!sallimuokkaus) {
		if (nilm[SrjVal->ItemIndex] > 0) {
			Sarja = SrjVal->ItemIndex;
			SrjKarkiBtn(Sender);
			}
		else {
			SrjVal->ItemIndex = Sarja;
			return;
			}
		}
	if (kilpparam.sarjavaihtelee || SrjVal->ItemIndex < 0)
		PvGrid->RowCount = kilpparam.n_pv_akt+1;
	else
		PvGrid->RowCount =
			Sarjat[SrjVal->ItemIndex].viimpv - Sarjat[SrjVal->ItemIndex].enspv + 2;
	FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::FormDestroy(TObject *Sender)
{
   delete[] PvGridCols;
}
//---------------------------------------------------------------------------


void __fastcall TFormKilpailijatiedot::BtnSalliClick(TObject *Sender)
{
	sallimuokkaus = !sallimuokkaus;
	BtnPeruuta->Visible = sallimuokkaus;
	BtnTallenna->Visible = sallimuokkaus;
	BtnPaivita->Visible = !sallimuokkaus;
	EdtSp->ReadOnly = !sallimuokkaus;
	EdtIkasrj->ReadOnly = !sallimuokkaus;
	EdtSukunimi->ReadOnly = !sallimuokkaus;
	EdtEtunimi->ReadOnly = !sallimuokkaus;
	EdtArvo->ReadOnly = !sallimuokkaus;
	EdtSeura->ReadOnly = !sallimuokkaus;
	EdtSeuralyh->ReadOnly = !sallimuokkaus;
	EdtYhd->ReadOnly = !sallimuokkaus;
	EdtJoukkue->ReadOnly = !sallimuokkaus;
	EdtMaa->ReadOnly = !sallimuokkaus;
	EdtPiiri->ReadOnly = !sallimuokkaus;
	EdtLisno->ReadOnly = !sallimuokkaus;
	EdtKvId->ReadOnly = !sallimuokkaus;
	EdPisteet->ReadOnly = !sallimuokkaus;
	EdRankipisteet->ReadOnly = !sallimuokkaus;
	EdtNimihaku->Enabled = !sallimuokkaus;
	SrjKarki->Enabled = !sallimuokkaus;
	SeurBtn->Enabled = !sallimuokkaus;
	EdBtn->Enabled = !sallimuokkaus;
	if (sallimuokkaus) {
		PvGrid->Options << goEditing;
		if (Lisays)
			Tila->Caption = UnicodeString(L"Uuden tietueen muokkaus");
		else
			Tila->Caption = UnicodeString(L"Muokkaustila");
		BtnSalli->Caption = L"Hakuun ja katseluun";
		GBHaku->Visible = false;
		}
	else {
		PvGrid->Options >> goEditing;
		Tila->Caption = UnicodeString(L"Haku- ja katselutila");
		BtnSalli->Caption = L"Salli muokkaus";
		GBHaku->Visible = true;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::BtnPeruutaClick(TObject *Sender)
{
	if (Lisays) {
		Lisays = false;
		BtnSalliClick(Sender);
		EdBtnClick(Sender);
		}
	Kilp = Kilp1;
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::BtnTallennaClick(TObject *Sender)
{
	if (tallennaTiedot() == 0) {
		BtnPeruutaClick(Sender);
		}
	BtnSalliClick(Sender);
	FocusControl(EdtKilpno);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::PvGridExit(TObject *Sender)
{
	if (sallimuokkaus && aktcol > 0 && aktrow > 0) {
		paivitaMuutos(aktcol, aktrow);
	}

	aktrow = 0;
	aktcol = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtNimihakuChange(TObject *Sender)
{
	if (EdtSeura->ReadOnly == true) {
		wchar_t txt[62];
		char key[MAXINDL+1];
		int d;

		wcsncpy(txt, EdtNimihaku->Text.c_str(),60);
		txt[60] = 0;
		keyfromwname(key, txt, nameindex.keyl-2, 0);
		searchix(&nameindex, key, &d);
		nimihaku = true;
		if (d > 0) {
			dKilp = d;
			Kilp.GETREC(d);
			Kilp1 = Kilp;
			Sarja = Kilp.Sarja();
			RGSelaus->ItemIndex == 2;
			naytaTiedot();
			}
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormKilpailijatiedot::FormClose(TObject *Sender, TCloseAction &Action)
{
//	kilptietue Kilp1;

	if (!EdtSukunimi->ReadOnly) {
//		Kilp1 = Kilp;
//		haeTiedot(&Kilp1);
		if (!(Kilp1 == Kilp) && Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallennus",
			MB_YESNO) == IDYES) {
			tallennaTiedot();
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::BtnSuljeClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::BtnOnlineClick(TObject *Sender)
{
	if (!FormKilpailijaOnline)
		FormKilpailijaOnline = new TFormKilpailijaOnline(this);
	FormKilpailijaOnline->sallimuokkaus = sallimuokkausvalinta;
	FormKilpailijaOnline->naytaKilpailija(dKilp);
	FormKilpailijaOnline->Show();
	if (FormKilpailijaOnline->WindowState == wsMinimized)
		FormKilpailijaOnline->WindowState = wsNormal;
	FormKilpailijaOnline->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::BtnEmitClick(TObject *Sender)
{
	if (!FormKilpailijaEmit)
		FormKilpailijaEmit = new TFormKilpailijaEmit(this);
	FormKilpailijaEmit->naytaKilpailija(dKilp);
	FormKilpailijaEmit->Show();
	if (FormKilpailijaEmit->WindowState == wsMinimized)
		FormKilpailijaEmit->WindowState = wsNormal;
	FormKilpailijaEmit->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::PvGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_F9 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (sallimuokkaus) {
			BtnTallennaClick(Sender);
			}
		else {
			BtnSalliClick(Sender);
			}
		}
	if (Key == VK_RIGHT && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (PvGrid->Col < PvGrid->ColCount-1)
			PvGrid->Col++;
		}
	if (Key == VK_LEFT && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (PvGrid->Col > 0)
			PvGrid->Col--;
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormKilpailijatiedot::FormResize(TObject *Sender)
{
	PvGrid->Width = ClientWidth - 12 * Screen->PixelsPerInch / 96;
	PvGrid->Height = (PvGrid->DefaultRowHeight+3) * (PvGrid->RowCount+1);
	ClientHeight = PvGrid->Top + PvGrid->Height + BtnSalli->Height + 30 * Screen->PixelsPerInch / 96;
	BtnSalli->Top = PvGrid->Top + PvGrid->Height + 8 * Screen->PixelsPerInch / 96;
	BtnTallenna->Top = BtnSalli->Top;
	BtnPeruuta->Top = BtnSalli->Top;
	BtnPaivita->Top = BtnSalli->Top;
	BtnOnline->Top = BtnSalli->Top;
	BtnEmit->Top = BtnSalli->Top;
	BtnSulje->Top = BtnSalli->Top;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::PvGridSelectCell(TObject *Sender, int ACol,
		  int ARow, bool &CanSelect)
{
	if (aktrow == ARow && aktcol == ACol)
		return;
	if (sallimuokkaus && aktcol > 0 && aktrow > 0) {
		paivitaMuutos(aktcol, aktrow);
	}
	aktrow = ARow;
	aktcol = ACol;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::FormShow(TObject *Sender)
{
	SrjVal->Clear();
	for (int srj = 0; srj < sarjaluku; srj++)
		SrjVal->Items->Add(Sarjat[srj].sarjanimi);
	PvGrid->DefaultRowHeight = 20 * Screen->PixelsPerInch / 96;
	PvGrid->RowCount = kilpparam.n_pv_akt + 1;
	FormResize(Sender);
	LblPerusNo->Visible = kilpparam.bibvaihtelee;
	EdtPerusNo->Visible = kilpparam.bibvaihtelee;
	EdtPerusNo->Enabled = kilpparam.bibvaihtelee;
	Vaihemuutokset1->Checked = false;
	if (dKilp)
		naytaKilpailija(dKilp);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::FormCreate(TObject *Sender)
{
	Sarja = 0;
	dKilp = -1;
	nimihaku = false;
	sallimuokkaus = false;
	sallimuokkausvalinta = false;
	Kilp.nollaa();
	SrjVal->Clear();
	for (int srj = 0; srj < sarjaluku; srj++)
		SrjVal->Items->Add(Sarjat[srj].sarjanimi);
	SrjVal->ItemIndex = 0;
	PvGridCols = new ColTp[sizeof(PvGridCols0)/sizeof(ColTp)];
	memcpy(PvGridCols, PvGridCols0, sizeof(PvGridCols0));
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Yksi osanottaja");
}
//---------------------------------------------------------------------------


void __fastcall TFormKilpailijatiedot::EdtKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift)
{
	if (Key == VK_F9 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (sallimuokkaus) {
			BtnTallennaClick(Sender);
			}
		else {
		   if (Vaihemuutokset1->Checked) {
			  FocusControl(PvGrid);
			  PvGrid->Col = 1;
			  PvGrid->Row = (k_pv - epv + 1) == 0 ? 1 : (k_pv - epv + 1);
			  }
			BtnSalliClick(Sender);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtKilpnoKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		EdtKilpnoChanged(Sender);
		Key = 0;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::Nytseuraluettelo1Click(TObject *Sender)
{
	Nytseuraluettelo1->Checked = !Nytseuraluettelo1->Checked;
	if (Nytseuraluettelo1->Checked) {
		FormSeurat->Show();
		FormSeurat->Left = Left+Width;
		FormSeurat->Top = 300*Screen->PixelsPerInch/96;
		if (FormSeurat->WindowState == wsMinimized)
			FormSeurat->WindowState = wsNormal;
		FormSeurat->BringToFront();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::Pivitseura1Click(TObject *Sender)
{
	Pivitseura1->Checked = !Pivitseura1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::SiirraSeuraTiedot(int haku)
{
	int KSeura;
	wchar_t nimi[LSEURA+1], lyhenne[LLYH+1], maa[4];
	int piiri;

	if (FormSeurat->SeuraHakuOn()) {
		KSeura = FormSeurat->Seuratiedot(haku, &piiri, maa, lyhenne, nimi);
		if (KSeura >= 0) {
			EdtPiiri->Text = UnicodeString(piiri);
			EdtSeura->Text = UnicodeString(nimi);
			EdtSeuralyh->Text = UnicodeString(lyhenne);
			if (maa[0])
				EdtMaa->Text = UnicodeString(maa);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtSeuraKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	EdKey = Key;
	if (Nytseuraluettelo1->Checked && ((!kilpparam.seuralyh_on && EdtSeura->Focused()) ||
		(kilpparam.seuralyh_on && EdtSeuralyh->Focused()))) {
		if ((Key == VK_RETURN || Key == VK_F6)
//		if ((Key == VK_F6)
			&& !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
			Key = (Key == VK_RETURN) ? VK_TAB : 0;
			SiirraSeuraTiedot(0);
			}
		else if ((Key == VK_UP || Key == VK_F5)
			&& !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
			Key = 0;
			SiirraSeuraTiedot(-1);
			}
		else if ((Key == VK_DOWN ||	Key == VK_F7)
			&& !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
			Key = 0;
			SiirraSeuraTiedot(1);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtSeuraChange(TObject *Sender)
{
	if (sallimuokkaus) {
		if ((EdtSeura->Focused() || EdtSeuralyh->Focused()) && (EdKey == VK_DOWN || EdKey == VK_UP || EdKey == VK_F5 || EdKey == VK_F7))
			return;
		if (Nytseuraluettelo1->Checked && FormSeurat->SeuraHakuOn()) {
			if (!kilpparam.seuralyh_on) {
				FormSeurat->NaytaSeurat(EdtSeura->Text.SubString(1, LSe).c_str(),
					(EdtSeura->Text.SubString(1, LSe)+UnicodeString(L"ööö")).c_str());
				}
			else {
				FormSeurat->NaytaSeurat(EdtSeuralyh->Text.SubString(1, LSe).c_str(),
					(EdtSeuralyh->Text.SubString(1, LSe)+UnicodeString(L"ööö")).c_str());
				}
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtSeuraEnter(TObject *Sender)
{
	if (sallimuokkaus && !kilpparam.seuralyh_on) {
		LSe = EdtSeura->Text.Length();
		EdtSeuraChange(Sender);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtSeuralyhEnter(TObject *Sender)
{
	if (sallimuokkaus && kilpparam.seuralyh_on) {
		LSe = EdtSeuralyh->Text.Length();
		EdtSeuraChange(Sender);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::Emitkoodihaku1Click(TObject *Sender)
{
	int kno;
	wchar_t wch;

	FormEmithaku = new TFormEmithaku(this);
	FormEmithaku->ShowModal();
	if (FormEmithaku->ModalResult == mrOk) {
		kno = FormEmithaku->Kilpno;
		if (kno > 0) {
			naytaKilpailija(getpos(kno));
			}
		}
	delete FormEmithaku;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::Lisenssinumerohaku1Click(TObject *Sender)
{
	int lisno = 0;
	wchar_t wch;

	inputint_prompt(&lisno, L"Anna haettava koodi", &wch, this);
	if (wch == L'\r') {
		int d;
		kilptietue kilp;

		for (d = 1; d < nrec; d++) {
			kilp.GETREC(d);
			if (kilp.kilpstatus == 0 && kilp.lisno[0] == lisno)
				break;
			}
		if (d < nrec) {
			naytaKilpailija(d);
			}
		else {
			Application->MessageBoxW(L"Koodi ei ole käytössä", L"Ei löydy", MB_OK);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::Perusnumerohaku1Click(TObject *Sender)
{
	int kno = 0;
	wchar_t wch;

	inputint_prompt(&kno, L"Anna haettava numero", &wch, this);
	if (wch == L'\r') {
		int d;
		kilptietue kilp;

		for (d = 1; d < nrec; d++) {
			kilp.GETREC(d);
			if (kilp.kilpstatus == 0 && kilp.kilpno == kno)
				break;
			}
		if (d < nrec) {
			naytaKilpailija(d);
			}
		else {
			Application->MessageBoxW(L"Numero ei ole käytössä", L"Ei löydy", MB_OK);
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::Liskilpailija1Click(TObject *Sender)
{
	dKilp = 0;
	Kilp.nollaa();
	Lisays = true;
	sallimuokkaus = false;
	BtnSalliClick(Sender);
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::Poistakilpailija1Click(TObject *Sender)
{
	if (Application->MessageBoxW(L"Poista näytetty kilpailija", L"Vahvista poisto", MB_YESNO) == IDYES)	{
		deltall(dKilp, 0);
		EdBtnClick(Sender);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::PvGridClick(TObject *Sender)
{
	bool sel;

	PvGridSelectCell(Sender, PvGrid->Col, PvGrid->Row, sel);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::PvGridKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (sallimuokkaus && Key == L'\r' && PvGrid->Col > 0 && PvGrid->Row > 0) {
		Key = 0;
		paivitaMuutos(PvGrid->Col, PvGrid->Row);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtSpChange(TObject *Sender)
{
	if (sallimuokkaus) {
		if (EdtSp->Text.Length() > 0) {
			Kilp.sukup = towupper(EdtSp->Text.c_str()[0]);
			}
		else
			Kilp.sukup = 0;
		}
	if (Kilp.sukup < L'1')
		Kilp.sukup = 0;
	if (Kilp.sukup)
		EdtSp->Text = UnicodeString(Kilp.sukup);
	else
		EdtSp->Text = L"";
}
//---------------------------------------------------------------------------


void __fastcall TFormKilpailijatiedot::EdtIkasrjExit(TObject *Sender)
{
	if (sallimuokkaus) {
		Kilp.ikasarja = _wtoi(EdtIkasrj->Text.c_str());
		if (Kilp.ikasarja == 0) {
			switch (towupper(EdtIkasrj->Text.c_str()[0])) {
				case L'Y':
					Kilp.ikasarja = 1;
					break;
				case L'A':
					Kilp.ikasarja = 2;
					break;
				}
			}
		}
	switch (Kilp.ikasarja) {
		case 0:
			EdtIkasrj->Text = L"";
			break;
		case 1:
			EdtIkasrj->Text = L"YL";
			break;
		case 2:
			EdtIkasrj->Text = L"AV";
			break;
		default:
			EdtIkasrj->Text = UnicodeString(Kilp.ikasarja);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::EdtSyntExit(TObject *Sender)
{
	if (sallimuokkaus) {
		Kilp.synt = _wtoi(EdtSynt->Text.c_str());
		if (Kilp.synt > 0 && Kilp.synt < 100) {
			if (Kilp.synt < 14)
				Kilp.synt += 2000;
			else
				Kilp.synt += 1900;
			}
		}
	EdtSynt->Text = UnicodeString(Kilp.synt);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::Vaihemuutokset1Click(TObject *Sender)
{
	Vaihemuutokset1->Checked = !Vaihemuutokset1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::Suljekaavake1Click(TObject *Sender)
{
	BtnSuljeClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::BtnPaivitaClick(TObject *Sender)
{
	if (dKilp) {
		Kilp.GETREC(dKilp);
		Kilp1 = Kilp;
		Sarja = Kilp.Sarja();
		}
	naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::Nytomaenntys1Click(TObject *Sender)
{
	Nytomaenntys1->Checked = !Nytomaenntys1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::Nyttavoitetulos1Click(TObject *Sender)
{
	Nyttavoitetulos1->Checked = !Nyttavoitetulos1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::Pisteet2ei1Click(TObject *Sender)
{
	Pisteet2ei1->Checked = !Pisteet2ei1->Checked;
	ValPist = Pisteet2ei1->Checked ? 1 : 0;
	Label4->Caption = Pisteet2ei1->Checked ? L"Pisteet-2" : L"Pisteet-1";
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::Vaihepisteet2ei1Click(TObject *Sender)
{
	Vaihepisteet2ei1->Checked = !Vaihepisteet2ei1->Checked;
	PvPist = Vaihepisteet2ei1->Checked ? 1 : 0;
	wcscpy(PvGridCols[PistCol].Caption, Vaihepisteet2ei1->Checked ? L"Pist-2" : L"Pist-1");
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::SakkoinamsekkorjausClick(TObject *Sender)
{
	Sakkoinamsekkorjaus->Checked = !Sakkoinamsekkorjaus->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijatiedot::CBsalanimiClick(TObject *Sender)
{
	Kilp.pv[0].salaa = salausVal[(CBsalanimi->ItemIndex+2)%3];
}
//---------------------------------------------------------------------------

