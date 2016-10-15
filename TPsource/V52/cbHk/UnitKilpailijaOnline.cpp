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

#include "HkDeclare.h"
#include "UnitKilpailijaOnline.h"
#include "WinHk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormKilpailijaOnline *FormKilpailijaOnline;

tietolinkkitp *HenkLinkki;

ColTp PvOlGridCols0[] = {
	{L"Päivä", 40, 0, 1, 0, true},
	{L"Bib", 40, 0, 1, 0, false},
	{L"Badge", 40, 0, 1, 0, false},
	{L"Lähtö-arv", 70, 0, 1, 0, true},
	{L"Lähtö-tot", 70, 0, 1, 0, false},
	{L"Maali", 70, 0, 1, 0, true},
	{L"Tulos", 70, 0, 1, 0, true},
	{L"Tark", 40, 0, 1, 0, true},
	{L"Amp.sakot", 60, 0, 1, 0, false},
	{L"Sakko", 40, 0, 1, 0, false},
	{L"Sija", 40, 0, 1, 0, true},
	{L"Ennätys", 60, 0, 1, 0, false},
	{L"Tavoite", 60, 0, 1, 0, false},
	{L"Yht.tls.", 70, 0, 1, 0, false},
	{L"Y-sija", 40, 0, 1, 0, false}
};

ColTp VaGridCols0[] = {
	{L"Piste", 40, 0, 1, 0, true},
	{L"Matka", 60, 0, 1, 0, false},
	{L"Aika", 80, 0, 1, 0, true},
	{L"Sija", 40, 0, 1, 0, true},
	{L"Edellisestä", 80, 0, 1, 0, true},
	{L"Yht.aika", 80, 0, 1, 0, false}
//	{L"Y-sija", 40, 0, 1, 0, false}
};

//---------------------------------------------------------------------------
__fastcall TFormKilpailijaOnline::TFormKilpailijaOnline(TComponent* Owner)
   : TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	Sarja = -1;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::EdtKilpnoChanged(TObject *Sender)
{
   wchar_t txt[20];
   int KilpNo, d;

	if (EdtSeura->ReadOnly == true) {
	   wcsncpy(txt, EdtKilpno->Text.c_str(),19);
	   txt[19] = 0;
	   KilpNo = _wtoi(txt);
	   if ((d = getpos(KilpNo, true)) > 0) {
		  dKilp = d;
		  Kilp.GETREC(d);
		  Sarja = Kilp.Sarja();
		  nimihaku = false;
		  naytaTiedot();
		  }
	   }
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::naytaKilpailija(int d)
{
	if (ToimintaTila == 1) {
		RGSelaus->ItemIndex = 2;
		RGSelaus->Enabled = false;
		}
   if (d > 0) {
	  dKilp = d;
	  Kilp.GETREC(d);
	  Sarja = Kilp.Sarja();
	  nimihaku = false;
	  naytaTiedot();
	  }
}//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::naytaTiedot(void)
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
	BtnSalli->Visible = sallimuokkaus;
	BtnTallenna->Visible = sallimuokkaus;
	BtnPeruuta->Visible = sallimuokkaus;
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
	if (Kilp.id() || (ToimintaTila == 1 && dKilp)) {
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
		EdtKilpno->Text = UnicodeString(Kilp.id());
		EdtSukunimi->Text = UnicodeString(Kilp.sukunimi);
		EdtEtunimi->Text = UnicodeString(Kilp.etunimi);
		EdtArvo->Text = UnicodeString(Kilp.arvo);
		EdtMaa->Text = UnicodeString(Kilp.maa);
		EdtSeura->Text = UnicodeString(Kilp.seura);
		EdtSeuralyh->Text = UnicodeString(Kilp.seuralyh);
		EdtJoukkue->Text = UnicodeString(Kilp.joukkue);
		EdtYhd->Text = UnicodeString(Kilp.yhdistys);
		EdtEnn->Text = Kilp.pv[k_pv].enn ?
			UnicodeString(aikatowstr_s(line, Kilp.pv[k_pv].enn/SEK, 0)) :
			UnicodeString(L"");
		EdtTav->Text = Kilp.pv[k_pv].tav ?
			UnicodeString(aikatowstr_s(line, Kilp.pv[k_pv].tav/SEK, 0)) :
			UnicodeString(L"");
		if (Sarjat[Kilp.Sarja()].viimpv - Sarjat[Kilp.Sarja()].enspv > 0) {
			PvGridCols[13].Visible = true;
			PvGridCols[14].Visible = true;
		}
		if (pvparam[k_pv].hiihtolahto) {
			PvGridCols[4].Visible = true;
		}
		if (Sarjat[Kilp.Sarja()].lsak[k_pv] > 0) {
			PvGridCols[8].Visible = true;
		}
		PvGrid->ColCount = 5;
		n = 0;
		PvGridWidth = 0;
		for (unsigned int i = 0; i < sizeof(PvOlGridCols0)/sizeof(ColTp); i++) {
			if (PvGridCols[i].Visible) {
				if (n >= PvGrid->ColCount)
					PvGrid->ColCount = n+1;
				PvGrid->Cells[n][0] = UnicodeString(PvGridCols[i].Caption);
				PvGrid->ColWidths[n] = PvGridCols[i].Width * Screen->PixelsPerInch / 96;
				PvGridWidth += PvGrid->ColWidths[n] + 2;
				PvGridCols[i].Order = n;
				n++;
				}
			}
		PvGridWidth += 12 * Screen->PixelsPerInch / 96;
		for (int pv = Sarjat[Kilp.Sarja()].enspv; pv <= Sarjat[Kilp.Sarja()].viimpv; pv++) {
			for (unsigned int i = 0; i < sizeof(PvOlGridCols0)/sizeof(ColTp); i++) {
				if (PvGridCols[i].Visible) {
					if (i > 0 && Sarjat[Kilp.Sarja()].luonne[pv] < 0) {
						PvGrid->Cells[PvGridCols[i].Order][pv-Sarjat[Kilp.Sarja()].enspv+1] = L"";
						continue;
						}
					switch (i) {
						case 0:
							PvGrid->Cells[PvGridCols[i].Order][pv-Sarjat[Kilp.Sarja()].enspv+1] =
								UnicodeString(pv+1)+L". pv";
							break;
						case 1:
							PvGrid->Cells[PvGridCols[i].Order][pv-Sarjat[Kilp.Sarja()].enspv+1] =
								UnicodeString(Kilp.pv[pv].bib);
							break;
						case 2:
							PvGrid->Cells[PvGridCols[i].Order][pv-Sarjat[Kilp.Sarja()].enspv+1] =
								UnicodeString(Kilp.pv[pv].badge[0]);
							break;
						case 3:
							tl = Kilp.pv[pv].tlahto;
							if (tl != TMAALI0) {
								AIKATOWSTRS(line, tl, t0);
								line[8] = 0;
								}
							else
								line[0] = 0;
							PvGrid->Cells[PvGridCols[i].Order][pv-Sarjat[Kilp.Sarja()].enspv+1] =
								UnicodeString(line);
							break;
						case 4:
							tl = Kilp.pv[pv].va[0].vatulos;
							if (tl != TMAALI0) {
								AIKATOWSTRS(line, tl, t0);
								line[pvparam[pv].laikasj] = 0;
								}
							else
								line[0] = 0;
							PvGrid->Cells[PvGridCols[i].Order][pv-Sarjat[Kilp.Sarja()].enspv+1] =
								UnicodeString(line);
							break;
						case 5:
							line[0] = 0;
							if (pv == k_pv) {
								tl = Kilp.maali(0);
								if (tl != TMAALI0) {
									AIKATOWSTRS(line, tl, t0);
									line[pvparam[k_pv].laikasj] = 0;
									}
								}
							PvGrid->Cells[PvGridCols[i].Order][pv-Sarjat[Kilp.Sarja()].enspv+1] =
								UnicodeString(line);
							break;
						case 6:
							tl = Kilp.tulos_pv(pv);
							if (tl != 0) {
								AIKATOWSTRS(line, tl, 0);
								line[pvparam[pv].laikasj] = 0;
								}
							else
								wcscpy(line , L"00.00.00");
							PvGrid->Cells[PvGridCols[i].Order][pv-Sarjat[Kilp.Sarja()].enspv+1] =
								UnicodeString(line);
							break;
						case 7:
							switch (Kilp.tark(pv)) {
								case L'K' :
									wcscpy(line, L"Kesk");
									break;
								case L'M' :
									wcscpy(line, L"Virhe");
									break;
								case L'O' :
									wcscpy(line, L"Ohit");
									break;
								case L'H' :
									wcscpy(line, L"Hyl");
									break;
								case L'E' :
									wcscpy(line, L"Eiläht.");
									break;
								case L'B' :
									wcscpy(line, L"Havaittu");
									break;
								case L'X' :
									wcscpy(line, L"EiAikaa");
									break;
								case L'V' :
									wcscpy(line, L"Vakantti");
									break;
								case L'T' :
									wcscpy(line, L"Tark.");
									break;
								case L'I' :
									wcscpy(line, L"esItys");
									break;
								default :
									wcscpy(line, L"-");
									break;
								}
							PvGrid->Cells[PvGridCols[i].Order][pv-Sarjat[Kilp.Sarja()].enspv+1] =
								UnicodeString(line);
							break;
						case 8:
							if (Sarjat[Kilp.Sarja()].lsak[pv]) {
								PvGrid->Cells[PvGridCols[i].Order][pv-Sarjat[Kilp.Sarja(pv)].enspv+1] =
									Kilp.asakkoStr(pv);
//									UnicodeString(Kilp.pv[pv].asakot).SubString(1, Sarjat[Kilp.Sarja()].lsak[pv]);
								}
							break;
						case 9:
							PvGrid->Cells[PvGridCols[i].Order][pv-Sarjat[Kilp.Sarja()].enspv+1] =
								UnicodeString(Kilp.pv[pv].tasoitus);
							break;
						case 10:
							if (Kilp.tulos_pv(pv, true))
								PvGrid->Cells[PvGridCols[i].Order][pv-Sarjat[Kilp.Sarja()].enspv+1] =
									UnicodeString(Kilp.sija(pv));
							else
								PvGrid->Cells[PvGridCols[i].Order][pv-Sarjat[Kilp.Sarja()].enspv+1] =
									L"";
							break;
						case 11:
							PvGrid->Cells[PvGridCols[i].Order][pv-Sarjat[Kilp.Sarja()].enspv+1] =
								Kilp.pv[pv].enn ?
									UnicodeString(aikatowstr_s(line, Kilp.pv[pv].enn/SEK, 0)) :
									UnicodeString(L"");
							break;
						case 12:
							PvGrid->Cells[PvGridCols[i].Order][pv-Sarjat[Kilp.Sarja()].enspv+1] =
								Kilp.pv[pv].tav ?
									UnicodeString(aikatowstr_s(line, Kilp.pv[pv].tav/SEK, 0)) :
									UnicodeString(L"");
							break;
						case 13:
							if (pv > Sarjat[Kilp.Sarja()].enspv) {
								tl = Kilp.ytulos(pv);
								if (tl != 0) {
									if (Sarjat[Sarja].pisteytulos() == false)
										AIKATOWSTRS(line, tl, 0);
									else if (Sarjat[Sarja].nouspisteet())
										_itow(tl/SEK, line, 10);
									else
										_itow((PISTEKATTO-tl)/SEK, line, 10);
									line[pvparam[pv].laikasj] = 0;
									}
								else
									wcscpy(line , L"00.00.00");
								}
							else
								line[0] = 0;
							PvGrid->Cells[PvGridCols[i].Order][pv-Sarjat[Kilp.Sarja()].enspv+1] =
								UnicodeString(line);
							break;
						case 14:
							if (pv > Sarjat[Kilp.Sarja()].enspv) {
								PvGrid->Cells[PvGridCols[i].Order][pv-Sarjat[Kilp.Sarja()].enspv+1] =
									UnicodeString(Kilp.y_sija(pv));
								}
							break;
						}
					}
				}
			}
		VaGrid->DefaultRowHeight = 22 * Screen->PixelsPerInch / 96;
		VaGrid->RowCount = 1 + Sarjat[Kilp.Sarja()].valuku[k_pv];
		if (k_pv - Sarjat[Kilp.Sarja()].enspv > 0) {
			VaGridCols[5].Visible = true;
//			VaGridCols[6].Visible = true;
		}
		VaGrid->ColCount = 3;
		n = 0;
		for (unsigned int i = 0; i < sizeof(VaGridCols0)/sizeof(ColTp); i++) {
			if (VaGridCols[i].Visible) {
				if (n >= VaGrid->ColCount)
					VaGrid->ColCount = n+1;
				VaGrid->Cells[n][0] = UnicodeString(VaGridCols[i].Caption);
				VaGrid->ColWidths[n] = VaGridCols[i].Width * Screen->PixelsPerInch / 96;
				VaGridCols[i].Order = n;
				n++;
				}
			}
		for (int iva = 1; iva <= Sarjat[Kilp.Sarja()].valuku[k_pv]; iva++) {
			tl = Kilp.p_aika(iva);
			for (unsigned int i = 0; i < sizeof(VaGridCols0)/sizeof(ColTp); i++) {
				if (VaGridCols[i].Visible) {
					switch (i) {
						case 0:
							VaGrid->Cells[VaGridCols[i].Order][iva] =
								UnicodeString(iva)+L". va";
							break;
						case 1:
							VaGrid->Cells[VaGridCols[i].Order][iva] =
								UnicodeString(Sarjat[Kilp.Sarja()].va_matka[k_pv][iva-1]);
							break;
						case 2:
							if (tl > 0) {
							   AIKATOWSTRS(line, tl, 0);
							   line[pvparam[k_pv].laikasj] = 0;
							   }
							else
								line[0] = 0;
							VaGrid->Cells[VaGridCols[i].Order][iva] =
								UnicodeString(line);
							break;
						case 3:
							if (Kilp.hyv() && tl > 0)
								VaGrid->Cells[VaGridCols[i].Order][iva] =
									UnicodeString(Kilp.p_sija(iva));
							else
								VaGrid->Cells[VaGridCols[i].Order][iva] =
									UnicodeString(L"");
							break;
						case 4:
							line[0] = 0;
							long tl1;
							if (iva > 1 && tl > 0) {
								if ((tl1 = Kilp.p_aika(iva-1)) > 0) {
								   AIKATOWSTRS(line, tl-tl1, 0);
								   line[pvparam[k_pv].laikasj] = 0;
								   }
								}
							VaGrid->Cells[VaGridCols[i].Order][iva] =
								UnicodeString(line);
							break;
						case 5:
							if ((tl = Kilp.ytulos_p(iva)) > 0) {
								if (Sarjat[Sarja].pisteytulos() == false)
									AIKATOWSTRS(line, tl, 0);
								else if (Sarjat[Sarja].nouspisteet())
									_itow(tl/SEK, line, 10);
								else
									_itow((PISTEKATTO-tl)/SEK, line, 10);
							   line[pvparam[k_pv].laikasj] = 0;
							   }
							else
								line[0] = 0;
							VaGrid->Cells[VaGridCols[i].Order][iva] =
								UnicodeString(line);
							break;
						}
					}
				}
			}
		}
	FormSetup();
	}


//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::haeTiedot(kilptietue *Kilp1)
   {
   wchar_t etu[60], suku[60], st[20], t;
   int tt;

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
	if ((t = wstrtoaika_vap(EdtEnn->Text.c_str(), 0)) > SEK)
		Kilp1->pv[k_pv].enn = t;
	if ((t = wstrtoaika_vap(EdtTav->Text.c_str(), 0)) > SEK)
		Kilp1->pv[k_pv].tav = t;

	for (int pv = Sarjat[Kilp1->Sarja()].enspv; pv <= Sarjat[Kilp1->Sarja()].viimpv; pv++) {
		int ipv = pv-Sarjat[Kilp1->Sarja()].enspv+1;
		if (Sarjat[Kilp1->Sarja()].luonne[pv] < 0)
			continue;
		for (unsigned int i = 1; i < sizeof(PvOlGridCols0)/sizeof(ColTp); i++) {
			if (PvGridCols[i].Visible && PvGrid->Cells[PvGridCols[i].Order][ipv].Length() > 0) {
				int k = PvGridCols[i].Order;
				switch (i) {
					case 1:
						Kilp1->pv[pv].bib = _wtoi(PvGrid->Cells[k][ipv].c_str());
						break;
					case 2:
						Kilp1->pv[pv].badge[0] = _wtoi(PvGrid->Cells[k][ipv].c_str());
						break;
					case 3:
						Kilp1->pv[pv].tlahto = wstrtoaika_vap(PvGrid->Cells[k][ipv].c_str(), t0);
						if (!pvparam[pv].hiihtolahto)
							Kilp1->pv[pv].va[0].vatulos = Kilp1->pv[pv].tlahto;
						break;
					case 4:
						Kilp1->pv[pv].va[0].vatulos = wstrtoaika_vap(PvGrid->Cells[k][ipv].c_str(), t0);
						break;
					case 6:
						tt = wstrtoaika_vap(PvGrid->Cells[k][ipv].c_str(), 0);
						if (tt && pv == k_pv && Kilp1->p_aika(0))
							tt -= Kilp1->p_aika(0, 0) - Kilp1->p_aika(0, 0, 2);
						Kilp1->tall_tulos_pv(tt, pv, 0);
						break;
					case 7:
						switch (towupper(FirstChar(PvGrid->Cells[k][ipv]))) {
							case L'E' :
								if (PvGrid->Cells[k][ipv].Length() > 1 && towupper(PvGrid->Cells[k][ipv][2]) == L'S')
									Kilp1->set_tark(L'I', pv);
								else if (PvGrid->Cells[k][ipv].Length() > 2 && towupper(PvGrid->Cells[k][ipv][3]) == L'A')
									Kilp1->set_tark(L'X', pv);
								else
									Kilp1->set_tark(L'E', pv);
								break;
							case L'V' :
								if (PvGrid->Cells[k][ipv].Length() > 1 && towupper(PvGrid->Cells[k][ipv][2]) == L'I')
									Kilp1->set_tark(L'M', pv);
								else
									Kilp1->set_tark(L'V', pv);
								break;
							case L'H' :
								if (PvGrid->Cells[k][ipv].Length() > 1 && towupper(PvGrid->Cells[k][ipv][2]) == L'A')
									Kilp1->set_tark(L'B', pv);
								else
									Kilp1->set_tark(L'H', pv);
								break;
							case L'L' :
							case L'A' :
								Kilp1->set_tark(L'-', pv);
								break;
							default :
								Kilp1->set_tark(towupper(FirstChar(PvGrid->Cells[k][ipv])), pv);
								break;
							}
						break;
					case 8:
						Kilp1->tall_asakot(PvGrid->Cells[k][ipv].c_str(), pv);
						/*
						if (kilpparam.kilplaji == L'A') {
							wcsncpy(Kilp1->pv[pv].asakot, PvGrid->Cells[k][ipv].c_str(), Sarjat[Kilp1->Sarja()].lsak[pv]);
							Kilp1->pv[pv].asakot[Sarjat[Kilp1->Sarja()].lsak[pv]] = 0;
							}
						*/
						break;
					case 9:
						Kilp1->pv[pv].tasoitus = _wtoi(PvGrid->Cells[k][ipv].c_str());
						break;
					case 11:
						Kilp1->pv[ipv].enn = wstrtoaika_vap(PvGrid->Cells[k][ipv].c_str(), 0);
						PvGrid->Cells[k][ipv] =
							Kilp1->pv[ipv].enn ?
								UnicodeString(aikatowstr_s(st, Kilp1->pv[ipv].enn/SEK, 0)) :
								UnicodeString(L"");
						break;
					case 12:
						Kilp1->pv[ipv].tav = wstrtoaika_vap(PvGrid->Cells[k][ipv].c_str(), 0);
						PvGrid->Cells[k][ipv] =
							Kilp1->pv[ipv].tav ?
								UnicodeString(aikatowstr_s(st, Kilp1->pv[ipv].tav/SEK, 0)) :
								UnicodeString(L"");
						break;
					}
				}
			}
		}

	for (int iva = 1; iva <= Sarjat[Kilp1->Sarja()].valuku[k_pv]; iva++) {
		if (VaGridCols[2].Visible) {
			int tt = wstrtoaika_vap(VaGrid->Cells[VaGridCols[2].Order][iva].c_str(), 0);
			if (tt && Kilp1->p_aika(iva))
				tt -= Kilp1->p_aika(iva, 0) - Kilp1->p_aika(iva, 0, 2);
			Kilp1->tall_tulos(iva, tt);
			}
		}
   }

//---------------------------------------------------------------------------

int __fastcall TFormKilpailijaOnline::tallennaTiedot(void)
   {
   wchar_t etu[60], suku[60];
   kilptietue Kilp1;

   if (dKilp <= 0 || dKilp >= nrec)
	  return(1);
   Kilp1 = Kilp;
	Kilp1.setId(_wtoi(EdtKilpno->Text.c_str()));
	if (Kilp1.id() != Kilp.id() && getpos(Kilp1.id()) > 0) {
	  Application->MessageBox(L"Kilpailijanumero ei ole käytettävissä", L"Virhe",
	  MB_OK);
	  return(1);
	}
   haeTiedot(&Kilp1);
   EnterCriticalSection(&tall_CriticalSection);
   Kilp1.tallenna(dKilp, 0, 0, 0, &Kilp);
   LeaveCriticalSection(&tall_CriticalSection);
   Kilp.GETREC(dKilp);
   return(0);
   }
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::SrjKarkiBtn(TObject *Sender)
{
   int p;

   if (ntulos[Sarja][0] > 0) {
	  p = jalku[Sarja][0];
	  if (p > 0) {
		 dKilp = p;
		 Kilp.GETREC(p);
		 Sarja = Kilp.Sarja();
		 nimihaku = false;
		 naytaTiedot();
		 }
	  }
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::SeurBtnClick(TObject *Sender)
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
	if (p > 0) {
		Kilp.GETREC(p);
		dKilp = p;
		Sarja = Kilp.Sarja();
		naytaTiedot();
		}
	LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::EdBtnClick(TObject *Sender)
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
	if (p > 0) {
		Kilp.GETREC(p);
		dKilp = p;
		Sarja = Kilp.Sarja();
		naytaTiedot();
		}
	LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------

void TFormKilpailijaOnline::FormSetup(void)
{
	if (ToimintaTila == 2 && Sarja >= 0 && Sarja < sarjaluku && Sarjat[Sarja].enspv <= k_pv)
		PvGrid->RowCount = k_pv - Sarjat[Sarja].enspv + 2;
	if (Seuranta) {
		SeurGrid->Height = ClientHeight - SeurGrid->Top - 6 * Screen->PixelsPerInch / 96;
		if (PvGrid->Left + PvGridWidth + SeurGridWidth < ClientWidth + 8 * Screen->PixelsPerInch / 96) {
			PvGrid->Width = PvGridWidth;
			SeurGrid->Left = PvGrid->Left + PvGridWidth + 4 * Screen->PixelsPerInch / 96;
			SeurGrid->Width = SeurGridWidth;
			}
		else if (SeurGridWidth < ClientWidth - PvGrid->Left - 490 * Screen->PixelsPerInch / 96) {
			SeurGrid->Left = ClientWidth - SeurGridWidth - 4 * Screen->PixelsPerInch / 96;
			PvGrid->Width = SeurGrid->Left - PvGrid->Left - 4 * Screen->PixelsPerInch / 96;
			SeurGrid->Width = SeurGridWidth;
			}
		else {
			PvGrid->Width = 486 * Screen->PixelsPerInch / 96;
			SeurGrid->Left = PvGrid->Left + PvGrid->Width + 4 * Screen->PixelsPerInch / 96;
			SeurGrid->Width = ClientWidth - SeurGrid->Left - 4 * Screen->PixelsPerInch / 96;
			}
		}
	else {
		if (PvGridWidth < ClientWidth - PvGrid->Left)
			PvGrid->Width = PvGridWidth;
		else
			PvGrid->Width < ClientWidth - PvGrid->Left;
		}
	PvGridHeight = PvGrid->RowCount * (PvGrid->DefaultRowHeight + 2) + 16 * Screen->PixelsPerInch / 96;
	PvGrid->Height = PvGridHeight;
	VaGrid->Top = PvGrid->Top + PvGrid->Height + 4 * Screen->PixelsPerInch / 96;
	VaGrid->ClientHeight = ClientHeight - VaGrid->Top - 50 * Screen->PixelsPerInch / 96;
	VaGrid->Width = PvGrid->Width;
	BtnSalli->Top = ClientHeight - 36 * Screen->PixelsPerInch / 96;
	BtnTallenna->Top = BtnSalli->Top;
	BtnPeruuta->Top = BtnSalli->Top;
	BtnPaivita->Top = BtnSalli->Top;
	Button4->Top = BtnSalli->Top;
	CBSeuranta->Top = BtnSalli->Top-2;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::FormResize(TObject *Sender)
{
	FormSetup();
	VaGrid->Refresh();
}
//---------------------------------------------------------------------------


void __fastcall TFormKilpailijaOnline::FormCreate(TObject *Sender)
{
	Sarja = 0;
	dKilp = -1;
	nimihaku = false;
	sallimuokkaus = false;
	Seuranta = false;
	Kilp.nollaa();
	PvGridHeight = 60 * Screen->PixelsPerInch / 96;
	for (int srj = 0; srj < sarjaluku; srj++)
		SrjVal->Items->Add(Sarjat[srj].sarjanimi);
	PvGridCols = new ColTp[sizeof(PvOlGridCols0)/sizeof(ColTp)];
	memcpy(PvGridCols, PvOlGridCols0, sizeof(PvOlGridCols0));
	VaGridCols = new ColTp[sizeof(VaGridCols0)/sizeof(ColTp)];
	memcpy(VaGridCols, VaGridCols0, sizeof(VaGridCols0));
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::SrjValChange(TObject *Sender)
{
	if (EdtEtunimi->ReadOnly == true) {
		if (ntulos[SrjVal->ItemIndex][0] > 0) {
			Sarja = SrjVal->ItemIndex;
			SrjKarkiBtn(Sender);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::FormDestroy(TObject *Sender)
{
   delete[] PvGridCols;
   delete[] VaGridCols;
}
//---------------------------------------------------------------------------


void __fastcall TFormKilpailijaOnline::BtnSalliClick(TObject *Sender)
{
	EdtSukunimi->ReadOnly = false;
	EdtEtunimi->ReadOnly = false;
	EdtArvo->ReadOnly = false;
	EdtSeura->ReadOnly = false;
	EdtSeuralyh->ReadOnly = false;
	EdtYhd->ReadOnly = false;
	EdtJoukkue->ReadOnly = false;
	EdtMaa->ReadOnly = false;
	EdtNimihaku->Enabled = false;
	SeurBtn->Enabled = false;
	EdBtn->Enabled = false;
	PvGrid->Options << goEditing;
	VaGrid->Options << goEditing;
	Tila->Caption = UnicodeString(L"Muokkaustila");
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::BtnPeruutaClick(TObject *Sender)
{
	EdtSukunimi->ReadOnly = true;
	EdtEtunimi->ReadOnly = true;
	EdtArvo->ReadOnly = true;
	EdtSeura->ReadOnly = true;
	EdtSeuralyh->ReadOnly = true;
	EdtYhd->ReadOnly = true;
	EdtJoukkue->ReadOnly = true;
	EdtMaa->ReadOnly = true;
	EdtNimihaku->Enabled = true;
	SeurBtn->Enabled = true;
	EdBtn->Enabled = true;
	PvGrid->Options >> goEditing;
	VaGrid->Options >> goEditing;
	naytaTiedot();
	Tila->Caption = UnicodeString(L"Haku- ja katselutila");
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::BtnTallennaClick(TObject *Sender)
{
	if (tallennaTiedot() == 0) {
		BtnPeruutaClick(Sender);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::VaGridExit(TObject *Sender)
{
	if (VaGridCols[2].Visible) {
		int k = VaGridCols[2].Order;

		for (int iva = 1; iva <= Sarjat[Kilp.Sarja()].valuku[k_pv]; iva++) {
			long vatulos;
			wchar_t line[20];

			if (VaGrid->Cells[k][iva].Length() > 0) {
				vatulos = wstrtoaika_vap(VaGrid->Cells[k][iva].c_str(), 0);
				AIKATOWSTRS(line, vatulos, 0);
				line[pvparam[k_pv].laikasj] = 0;
				VaGrid->Cells[k][iva] = UnicodeString(line);
				}
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::PvGridExit(TObject *Sender)
{
	wchar_t line[20];

	for (int ipv = Sarjat[Kilp.Sarja()].enspv; ipv <= Sarjat[Kilp.Sarja()].viimpv; ipv++) {
		long tls = 0, tm = TMAALI0, tl = TMAALI0, tl1 = TMAALI0;
		if (Sarjat[Kilp.Sarja()].luonne[ipv] < 0)
			continue;
		for (unsigned int i = 1; i < sizeof(PvOlGridCols0)/sizeof(ColTp); i++) {
			if (PvGridCols[i].Visible) {
				int k = PvGridCols[i].Order;

				switch (i) {
					case 3:
						if (PvGrid->Cells[i][ipv+1].Length() > 3)
							tl = wstrtoaika_vap(PvGrid->Cells[k][ipv+1].c_str(), t0);
						AIKATOWSTRS(line, tl, t0);
						line[pvparam[ipv].laikasj] = 0;
						PvGrid->Cells[k][ipv+1] = UnicodeString(line);
						break;
					case 4:
						if (PvGrid->Cells[k][ipv+1].Length() > 3)
							tl1 = wstrtoaika_vap(PvGrid->Cells[k][ipv+1].c_str(), t0);
						AIKATOWSTRS(line, tl1, t0);
						line[pvparam[ipv].laikasj] = 0;
						PvGrid->Cells[k][ipv+1] = UnicodeString(line);
						break;
					case 5:
						if (ipv == k_pv) {
							if (PvGrid->Cells[k][ipv+1].Length() > 3)
								tm = wstrtoaika_vap(PvGrid->Cells[k][ipv+1].c_str(), t0);
							AIKATOWSTRS(line, tm, t0);
							line[pvparam[k_pv].laikasj] = 0;
							PvGrid->Cells[k][ipv+1] = UnicodeString(line);
							}
						break;
					case 6:
						if (PvGrid->Cells[k][ipv+1].Length() > 3)
							tls = wstrtoaika_vap(PvGrid->Cells[k][ipv+1].c_str(), 0);
						AIKATOWSTRS(line, tls, 0);
						line[pvparam[ipv].laikasj] = 0;
						PvGrid->Cells[k][ipv+1] = UnicodeString(line);
						break;
					case 7:
						switch (towupper(FirstChar(PvGrid->Cells[k][ipv+1]))) {
							case L'K' :
								PvGrid->Cells[k][ipv+1] = UnicodeString(L"Kesk");
								break;
							case L'O' :
								PvGrid->Cells[k][ipv+1] = UnicodeString(L"Ohit");
								break;
							case L'X' :
								PvGrid->Cells[k][ipv+1] = UnicodeString(L"EiAikaa");
								break;
							case L'M' :
								PvGrid->Cells[k][ipv+1] = UnicodeString(L"Virhe");
								break;
							case L'H' :
								PvGrid->Cells[k][ipv+1] = UnicodeString(L"Hyl");
								break;
							case L'E' :
								if (PvGrid->Cells[k][ipv+1] > 1 && towupper(PvGrid->Cells[k][ipv+1][2]) == L'S')
									PvGrid->Cells[k][ipv+1] = UnicodeString(L"esItys");
								else if (PvGrid->Cells[k][ipv+1] > 2 && towupper(PvGrid->Cells[k][ipv+1][3]) == L'A')
									PvGrid->Cells[k][ipv+1] = UnicodeString(L"EiAikaa");
								else
									PvGrid->Cells[k][ipv+1] = UnicodeString(L"Eiläht.");
								break;
							case L'T' :
								PvGrid->Cells[k][ipv+1] = UnicodeString(L"Tark.");
								break;
							case L'I' :
								PvGrid->Cells[k][ipv+1] = UnicodeString(L"esItys");
								break;
							case L'P' :
								PvGrid->Cells[k][ipv+1] = UnicodeString(L"Poissa");
								break;
							case L'V' :
								if (PvGrid->Cells[k][ipv+1] > 1 && towupper(PvGrid->Cells[k][ipv+1][2]) == L'I')
									PvGrid->Cells[k][ipv+1] = UnicodeString(L"Virhe");
								else
									PvGrid->Cells[k][ipv+1] = UnicodeString(L"Vakantti");
								break;
							default :
								PvGrid->Cells[k][ipv+1] = UnicodeString(L"-");
								break;
							}
						break;
					}
				}
			}
		if (ipv == k_pv) {
			if (pvparam[k_pv].hiihtolahto) {
				if (tl1 == Kilp.p_aika(-1) && tm == Kilp.maali(0) && tls != Kilp.p_aika(0)) {
					tm += tls - Kilp.p_aika(0);
					AIKATOWSTRS(line, tm, t0);
					line[pvparam[k_pv].laikasj] = 0;
					PvGrid->Cells[PvGridCols[5].Order][ipv+1] = UnicodeString(line);
					}
				if (tl1 != Kilp.p_aika(-1) && tm == Kilp.maali(0) && tls == Kilp.p_aika(0)) {
					tm -= tl1 - Kilp.p_aika(-1);
					AIKATOWSTRS(line, tm, t0);
					line[pvparam[k_pv].laikasj] = 0;
					PvGrid->Cells[PvGridCols[5].Order][ipv+1] = UnicodeString(line);
					}
				if (tl1 == Kilp.p_aika(-1) && tm != Kilp.maali(0) && tls == Kilp.p_aika(0)) {
					tls += tm - Kilp.maali(0);
					AIKATOWSTRS(line, tls, 0);
					line[pvparam[k_pv].laikasj] = 0;
					PvGrid->Cells[PvGridCols[6].Order][ipv+1] = UnicodeString(line);
					}
				}
			else {
				if (tl == Kilp.pv[k_pv].tlahto && tm == Kilp.maali(0) && tls != Kilp.p_aika(0)) {
					tm += tls - Kilp.p_aika(0);
					AIKATOWSTRS(line, tm, t0);
					line[pvparam[k_pv].laikasj] = 0;
					PvGrid->Cells[PvGridCols[5].Order][ipv+1] = UnicodeString(line);
					}
				if (tl != Kilp.pv[k_pv].tlahto && tm == Kilp.maali(0) && tls == Kilp.p_aika(0)) {
					tm -= tl1 - Kilp.pv[k_pv].tlahto;
					AIKATOWSTRS(line, tm, t0);
					line[pvparam[k_pv].laikasj] = 0;
					PvGrid->Cells[PvGridCols[5].Order][ipv+1] = UnicodeString(line);
					}
				if (tl == Kilp.pv[k_pv].tlahto && tm != Kilp.maali(0) && tls == Kilp.p_aika(0)) {
					tls += tm - Kilp.maali(0);
					AIKATOWSTRS(line, tls, 0);
					line[pvparam[k_pv].laikasj] = 0;
					PvGrid->Cells[PvGridCols[6].Order][ipv+1] = UnicodeString(line);
					}
				}
			}
		}

}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::EdtNimihakuChange(TObject *Sender)
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
		RGSelaus->ItemIndex = 2;
		if (d > 0 && d < nrec) {
			dKilp = d;
			Kilp.GETREC(d);
			Sarja = Kilp.Sarja();
			naytaTiedot();
			}
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormKilpailijaOnline::FormClose(TObject *Sender, TCloseAction &Action)
{
	kilptietue Kilp1;

	if (!EdtSukunimi->ReadOnly) {
		haeTiedot(&Kilp1);
		if (!(Kilp1 == Kilp) && Application->MessageBox(L"Tallennetaanko mahdolliset muutokset?", L"Tallennus",
			MB_YESNO) == IDYES) {
			tallennaTiedot();
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::Button4Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormKilpailijaOnline::naytaSeuranta(void)
{
	int nrivi, nTls;
//   int d, k, edT = 0, t, sj, karki, ykarki, KH = 0;
	int d, k, tvert = 0, t, sj, sj1, LA = 0, Yhd = 0, edelt = 2;
	wchar_t ln[80], ln2[20];
	int p, p1, alkuVa, Va, Va1, tls[2] = {0,0};
	kilptietue kilp;
	bool ontulos = false, ytlsfl = false;

	if (dKilp <= 0 || dKilp >= nrec)
		return;
	Va = CBSeurPiste->ItemIndex-1;
	kilp = Kilp;
	Kilp.GETREC(dKilp);
	if (!(kilp == Kilp))
		naytaTiedot();
	if (Va > 0)
		alkuVa = Va+4;
	else if (Va < 0) {
//		alkuVa = kilpparam.valuku + 5 + 5 * (k_pv > 0);
//		Va = kilpparam.valuku + 1 + (k_pv > 0);
		alkuVa = kilpparam.valuku + 10;
		Va = kilpparam.valuku + 2;
		LA = 1;

		return;

//		Cells[2][0].text = UnicodeString(L"Lähtö");
		}
	else {
		alkuVa = Va;
		Cells[2][0].text = UnicodeString(L"Tulos");
		}
	Va1 = Va;
	if (Va == kilpparam.valuku+1) {
		Va1 = 0;
		ytlsfl = true;
		}
	if (LA) {
		nTls = nilm[Sarja]-neil[Sarja];
		nrivi = nTls+1;
		}
	else {
		nTls = ntulos[Sarja][Va];
		nrivi = nTls+1;
		if (!Kilp.p_aika(Va))
			nrivi++;
		}
	if (nrivi > SeurGrid->ClientHeight/SeurGrid->DefaultRowHeight-1)
		nrivi = SeurGrid->ClientHeight/SeurGrid->DefaultRowHeight-1;
	SeurGrid->RowCount = nrivi;
	if (nrivi == 1) {
		return;
		}
	SeurGrid->FixedRows = 1;
	EnterCriticalSection(&tall_CriticalSection);
//   karki = 0;
	if (!LA) {
		sijat(Sarja);
		}
	p = jalku[Sarja][alkuVa];
	ontulos = true;
	if (!LA) {
//		tvert = pkarki[Sarja][Va1];
		sj1 = Kilp.p_sija(Va);
		if ((tvert = Kilp.p_aika(Va)) == 0)
			sj1 = 0;
		if (sj1 <= 0 || (ytlsfl && Sarjat[Sarja].pisteytulos()) && !Kilp.p_aika(0)) {
			ontulos = false;
			if (!ytlsfl || Sarjat[Sarja].pisteytulos() == false)
				tls[0] = Kilp.tulos1(Va, KSEK*T_TIME(biostime(0, 0)));
			else {
				kilptietue kilp2;
				kilp2 = Kilp;
				kilp2.set_tulos(0, KSEK*T_TIME(biostime(0, 0)));
				kilp2.pv[k_pv].pvpisteet[0] = Pisteet(&Sarjat[Sarja].pistekaava[k_pv], 0, nTls, kilp2.p_aika(0), 0);
				LaskeKilpYhtPisteet(&kilp2, &Sarjat[Sarja].pistekaava[k_pv], Sarja);
				tls[0] = kilp2.ytulos(k_pv, 2);
				}
			tvert = tls[0];
			p1 = p;
			sj1 = intsija((UINT32 *)tls, &p1, Va, 0, 0);
			}
		}
	k = 1;
	NKarki = nrivi/4;
	if (NKarki > 3)
		NKarki = 3;
	edelt = nrivi/4;
	if (sj1 > 0 && edelt > sj1)
		edelt = sj1;
	if (NKarki > sj1 - edelt)
		NKarki = sj1 - edelt;
/*
	if (sj1 > 0 && edelt > sj1-1-NKarki)
		edelt = sj1-1-NKarki;
//	if (edelt < 0)
//		edelt = 0;
	if (edelt >= 0 && NKarki > edelt + 1)
		NKarki = edelt + 1;
*/
	if (edelt + NKarki + nTls - sj1 + 2 < nrivi)
		edelt = nrivi + sj1 - nTls - NKarki - 2;
	if (ontulos)
		sj1--;
	SeurRow = -1;
	for (int j = 0; j < ontulos ? nTls : nTls+1; j++) {
		p1 = p;
		if (p > 0)
			p = JarjSeur(Yhd, Va, p);
//      if (j && Seuraa && j <= nTls - nrivi)
//         continue;
		if (k >= nrivi)
			break;
		if (!ontulos && (k == edelt+NKarki || (p1 <= 0 && sj1 == nTls+1))) {
			Cells[0][k].text = UnicodeString(L"");
			Kilp.nimi(ln, 20, false);
			Cells[1][k].text = UnicodeString(ln);
			t = tls[0];
			if (!ytlsfl || Sarjat[Sarja].pisteytulos() == false) {
				AIKATOWSTRS(ln, t, LA*t0);
				ln[pvparam[k_pv].laikasj] = 0;
				elimwzb1(ln);
				}
			else if (Sarjat[Sarja].nouspisteet())
				_itow(t/SEK, ln, 10);
			else
				_itow((PISTEKATTO-t)/SEK, ln, 10);
			Cells[2][k].text = UnicodeString(ln);
			Cells[3][k].text = L"";;
			SeurRow = k++;
			sj1 = 0;
			}
		if (p1 <= 0)
			break;
		kilp.GETREC(p1);
		if (kilp.id() == Kilp.id())
			SeurRow = k;
		if (LA)
			t = kilp.pv[k_pv].tlahto;
		else
			t = kilp.p_aika(Va);
		if (j < NKarki || j >= sj1-edelt) {
			Cells[0][k].text = UnicodeString(j+1);
			Cells[0][k].justify = 1;
			kilp.nimi(ln, 20, false);
			Cells[1][k].text = UnicodeString(ln);
			Cells[1][k].justify = 0;
			if (!ytlsfl || Sarjat[Sarja].pisteytulos() == false) {
				AIKATOWSTRS(ln, t, LA*t0);
				ln[pvparam[k_pv].laikasj] = 0;
				elimwzb1(ln);
				}
			else if (Sarjat[Sarja].nouspisteet())
				_itow(t/SEK, ln, 10);
			else
				_itow((PISTEKATTO-t)/SEK, ln, 10);
			Cells[2][k].text = UnicodeString(ln);
			Cells[2][k].justify = 1;
			if (!LA) {
				t -= tvert;
				if (t == 0)
					ln2[0] = 0;
				else {
					if (t >= 0)
						ln2[0] = L'+';
					else {
						ln2[0] = L'-';
						t = -t;
						}
					if (!ytlsfl || Sarjat[Sarja].pisteytulos() == false) {
						AIKATOWSTRS(ln2+1, t, 0);
						ln2[pvparam[k_pv].laikasj+1] = 0;
						elimwzb1(ln2+1);
						}
					else if (Sarjat[Sarja].nouspisteet())
						_itow(t/SEK, ln2+1, 10);
					else
						_itow(t/SEK, ln2, 10);
					}
				Cells[3][k].text = UnicodeString(ln2);
				Cells[3][k].justify = 1;
				}
			k++;
			}
		}
	for (; k > 0 && k < nrivi; k++) {
		Cells[0][k].text = L"";
		Cells[1][k].text = L"";
		Cells[2][k].text = L"";
		Cells[3][k].text = L"";
		}
	LeaveCriticalSection(&tall_CriticalSection);
	SeurGrid->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::CBSeurantaClick(TObject *Sender)
{
	Seuranta = CBSeuranta->Checked;
	CBSeuranta->Checked = Seuranta;
	SeurGrid->Visible = Seuranta;
	LblSeurPiste->Visible = Seuranta;
	CBSeurPiste->Visible = Seuranta;
	if (Seuranta)
		Width = SeurGrid->Left + SeurGrid->Width + 14 * Screen->PixelsPerInch / 96;
	else
		Width = EdtNimihaku->Left + EdtNimihaku->Width + 14 * Screen->PixelsPerInch / 96;
	FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::FormShow(TObject *Sender)
{
	CBSeuranta->Checked = Seuranta;
	SeurGrid->Visible = Seuranta;
	LblSeurPiste->Visible = Seuranta;
	CBSeurPiste->Visible = Seuranta;
	CBSeurPiste->Items->Clear();
	CBSeurPiste->Items->Add(L"Lähtö");
	CBSeurPiste->Items->Add(L"Maali");
	PvGrid->DefaultRowHeight = 20 * Screen->PixelsPerInch / 96;
	if (ToimintaTila < 2)
		PvGrid->RowCount = kilpparam.n_pv_akt + 1;
	if (ToimintaTila == 2)
		PvGrid->RowCount = k_pv + 2;
	PvGrid->Row = 1;
	PvGrid->Height = (PvGrid->DefaultRowHeight+3) * PvGrid->RowCount;
	VaGrid->Top = PvGrid->Top + PvGrid->Height + 6 * Screen->PixelsPerInch / 96;
	SeurGrid->ColWidths[0] = 36 * Screen->PixelsPerInch/96;
	SeurGrid->ColWidths[1] = 120 * Screen->PixelsPerInch/96;
	SeurGrid->ColWidths[2] = 65 * Screen->PixelsPerInch/96;
	SeurGrid->ColWidths[3] = 60 * Screen->PixelsPerInch/96;
	SeurGridWidth = 304 * Screen->PixelsPerInch/96;
	SeurGrid->Width = SeurGridWidth;
	Cells[0][0].text = UnicodeString(L"Sija");
	Cells[1][0].text = UnicodeString(L"Nimi");
	Cells[2][0].text = UnicodeString(L"Aika");
	Cells[3][0].text = UnicodeString(L"Ero");
	for (int p = 0; p < kilpparam.valuku; p++)
		CBSeurPiste->Items->Add(UnicodeString(p+1)+UnicodeString(L". väliaika"));
	if (k_pv)
		CBSeurPiste->Items->Add(L"Yhteistulos");
	CBSeurPiste->ItemIndex = 1;
	FormResize(Sender);
	Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::SeurGridDrawCell(TObject *Sender, int ACol,
		  int ARow, TRect &Rect, TGridDrawState State)
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
   if (ARow == 0 || ACol == 0)
	  pBitmap->Canvas->Brush->Color = clBtnFace;
   else if (ARow == SeurRow) {
		pBitmap->Canvas->Brush->Color = clYellow;  //clYellow;
		}
   else if (ARow <= NKarki) {
		pBitmap->Canvas->Brush->Color = clLime;  //clYellow;
		}
   else {
		pBitmap->Canvas->Brush->Color = clWindow;
		}
	pBitmap->Canvas->Font->Style = TFontStyles();
   pBitmap->Canvas->FillRect(Rect1);
   pBitmap->Canvas->Font->Size = 8;
   X = 1;
   if (Cells[ACol][ARow].justify == 1) {
	   X =  Rect.Width() - pBitmap->Canvas->TextWidth(Cells[ACol][ARow].text) - 2;
	   if (X < 1)
		  X = 1;
	   }
   pBitmap->Canvas->TextOut(X,1,Cells[ACol][ARow].text);
   SeurGrid->Canvas->Draw(Rect.Left, Rect.Top, pBitmap);
   delete pBitmap;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::BtnTietoaClick(TObject *Sender)
{
	if (!HenkLinkki) {
		if (Application->MessageBoxW(L"Linkkejä tietoihin ei ole avattu - avataanko nyt?",
		L"Linkit", MB_YESNO) != IDYES)
			return;
		OpenDialog1->InitialDir = FormMain->CurrentDir;
		OpenDialog1->FileName = L"Tietolista.txt";
		OpenDialog1->DefaultExt = L"txt";
		OpenDialog1->Filter = L"Tekstitiedostot (*.txt)|*.TXT|Kaikki tiedostot (*.*)|*.*";
		if (OpenDialog1->Execute()) {
			TextFl *InFile = new TextFl(OpenDialog1->FileName.c_str(), L"rt");

			if (InFile->IsOpen()) {
				int n = 0;
				wchar_t ln[242], *p, *p1;

				while (!InFile->Feof()) {
					InFile->ReadLine(ln, 240);
					if (wcslen(ln) > 3)
						n++;
					}
				if (n > 0)
					HenkLinkki = new tietolinkkitp[n+2];
				if (HenkLinkki) {
					memset(HenkLinkki, 0, (n+2)*sizeof(tietolinkkitp));
					InFile->Rewind();
					n = 0;
					while (!InFile->Feof()) {
						InFile->ReadLine(ln, 240);
						if (wcslen(ln) > 3) {
							p1 = ln + wcslen(ln);
							p = wcstok(ln, L" \t;");
							if (p) {
								for (;p1[-1] && p1 > p; p1--) ;
								elimwbl2(p);
								upcasewstr(p);
								elimwbl2(p1);
								p1[160] = 0;
								if (wcslen(p) > 0 && wcslen(p1) > 1) {
									int k;

									if (!wcscmp(p, L"TIEDOSTO")) {
										wchar_t st[200], *p2;

										wcscpy(st, p1);
										upcasewstr(st);
										if (wmemcmp(st, L"FILE:", 5) && wmemcmp(st, L"HTTP:", 5)) {
											if (st[1] == L':')
												swprintf(st, L"file:///%s", p1);
											else
												swprintf(st, L"file:///%s/%s",
													FormMain->CurrentDir.c_str(), p1);
											p1[0] = 0;
											wcscpy(p1, st);
											}
										k = 0;
										}
									else {
										k = ++n;
										if (iswdigit(*p))
											swprintf(HenkLinkki[k].ref, L"%08d", _wtoi(p));
										else
											wcsncpy(HenkLinkki[k].ref, p, sizeof(HenkLinkki[0].ref)/2-1);
										}
									wcsncpy(HenkLinkki[k].linkki, p1, sizeof(HenkLinkki[0].linkki)/2-1);
									}
								}
							}
						}
					}
				}
			delete InFile;
			if (!HenkLinkki) {
				Application->MessageBoxW(L"Linkkejä ei löytynyt tiedostosta", L"Virhe", MB_OK);
				return;
				}
			}
		else
			return;
		}

	wchar_t ref[22] = L"", linkki[162] = L"";

	if (Kilp.lisno[0])
		swprintf(ref, L"%08d", Kilp.lisno[0]);
	else if (Kilp.lisno[1]) {
		swprintf(ref, L"%3.3s%08d", Kilp.maa, Kilp.lisno[1]);
		upcasewstr(ref);
		}
	if (ref[0]) {
		int r;
		for (r = 1; HenkLinkki[r].ref[0]; r++)
			if (!wcscmp(HenkLinkki[r].ref, ref))
				break;
		if (!wcscmp(HenkLinkki[r].ref, ref)) {
			if (HenkLinkki[r].linkki[0] == L'#') {
				if (HenkLinkki[0].linkki[0]) {
					wcscpy(linkki,HenkLinkki[0].linkki);
					wcsncpy(linkki+wcslen(linkki), HenkLinkki[r].linkki,
						sizeof(linkki)/2-wcslen(linkki)-1);
					}
				}
			else
				wcscpy(linkki,HenkLinkki[r].linkki);
			}
		if (linkki[0]) {
			FormBrowser->WebBrowser1->Navigate(linkki);
			FormBrowser->Show();
			if (FormBrowser->WindowState == wsMinimized)
				FormBrowser->WindowState = wsNormal;
			FormBrowser->BringToFront();
			}
		else {
			Application->MessageBoxW(L"Kilpailijalle ei ole tietolinkkiä", L"Ei löydy", MB_OK);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Kilpailun seuranta");
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::EdtKilpnoKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		EdtKilpnoChanged(Sender);
		Key = 0;
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormKilpailijaOnline::EdtKeyDown(TObject *Sender, WORD &Key,
		  TShiftState Shift)
{
	kilptietue Kilp1;

	if (sallimuokkaus && Key == VK_F9 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		if (!EdtSukunimi->ReadOnly) {
			haeTiedot(&Kilp1);
			if (!(Kilp1 == Kilp) && Application->MessageBoxW(L"Tallenna mahdolliset muutokset", L"Vahvistus", MB_YESNO) == IDYES)
				BtnTallennaClick(Sender);
			else
				BtnPeruutaClick(Sender);
			}
		else {
			BtnSalliClick(Sender);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::VaGridMouseWheelDown(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
	if (VaGrid->RowCount - VaGrid->FixedRows >= VaGrid->VisibleRowCount + VaGrid->TopRow)
		VaGrid->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaOnline::VaGridMouseWheelUp(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
	if (VaGrid->TopRow > VaGrid->FixedRows)
		VaGrid->TopRow--;
}
//---------------------------------------------------------------------------


void __fastcall TFormKilpailijaOnline::BtnPaivitaClick(TObject *Sender)
{
	if (dKilp)
		Kilp.GETREC(dKilp);
	naytaTiedot();
}
//---------------------------------------------------------------------------

