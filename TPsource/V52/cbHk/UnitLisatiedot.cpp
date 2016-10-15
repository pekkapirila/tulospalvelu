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

#include "UnitLisatiedot.h"
#include "WinHk.h"
#include "UnitTietoTulk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormLisatiedot *FormLisatiedot;

//---------------------------------------------------------------------------
typedef struct {
	char in[(2*LSEURA+1)/3];
	wchar_t	out[LSEURA+1];
} typestos;

typedef struct {
	char in[(2*LSEURA+1)/3];
	int	out;
} typeston;

typedef struct {
	int	in;
	wchar_t	out[LSEURA+1];
} typentos;

typedef struct {
	int	in;
	int	out;
} typenton;

static char monivaiheTieto[] = "01001101000000000011111111000001111111";
static wchar_t deskohde[] = L"6 24 25 26 27 33";
static int sarjaTunniste = 10;
static int sarjaTieto = 20;
static double deskerroin = 1000.0;

//---------------------------------------------------------------------------
__fastcall TFormLisatiedot::TFormLisatiedot(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormLisatiedot::SiirraTiedot(int sellaisinaan)
{
	int Tunniste, Tieto, TunnPv, TietoPv, TunnSar, TietoSar, LTunn, LTieto, epv, vpv;
	int nrv = 0, nsiirr = 0, PohjTunniste;
	wchar_t st[20] = L"", erotin, erottimet[] = L" \t;,";
	typestos *stos = 0;
	typeston *ston = 0;
	typentos *ntos = 0;
	typenton *nton = 0;

	wcsncpy(st, EdtKerroin->Text.c_str(), 19);
	deskerroin = StrToDouble(st);
	Tunniste = RGTunniste->ItemIndex;
	PohjTunniste = RGTunniste->Items->Count - 1;
	Tieto = RGTieto->ItemIndex;
	TunnPv = CBTunnPv->ItemIndex - 1;
	TietoPv = CBTietoPv->ItemIndex - 2;
	TunnSar = EditTunniste->Text.ToInt();
	TietoSar = EditTieto->Text.ToInt();
	erotin = erottimet[CBErotin->ItemIndex];
	if (TietoPv == -1) {
		epv = 0;
		vpv = kilpparam.n_pv_akt-1;
		}
	else {
		epv = TietoPv;
		vpv = epv;
		}
	if (TunnSar <= 0 || TietoSar <= 0 || TunnSar == TietoSar) {
		Application->MessageBoxW(L"Määrittele sarakkeet", L"Virhe", MB_OK);
		return;
		}
	if (Tunniste < 0 || Tieto < 0) {
		Application->MessageBoxW(L"Valitse molemmat kentät", L"Virhe", MB_OK);
		return;
		}
	if ((Tunniste == 1 || Tunniste == 7 || Tunniste == 9) && kilpparam.n_pv_akt > 1
		&& TunnPv < 0) {
		Application->MessageBoxW(L"Valitse rintanumeron tai koodin päivä", L"Virhe", MB_OK);
		return;
		}
	if (TietoPv < -1 && kilpparam.n_pv_akt > 1 && monivaiheTieto[Tieto] > '0') {
		Application->MessageBoxW(L"Valitse kohdetiedon vaihe", L"Este", MB_OK);
		return;
		}
	if (Tunniste > 3 && Tunniste < 7 && Tieto < 7) {
		Application->MessageBoxW(L"Mahdoton yhdistelmä", L"Virhe", MB_OK);
		return;
		}
	if (!kilpparam.bibvaihtelee && Tieto < 2 && TietoPv >= 0) {
		Application->MessageBoxW(L"Siirto mahdollista vain kaikille vaiheille, kun rintanumero ei vaihtele.", L"Rajoitus", MB_OK);
		return;
		}
	if (!kilpparam.bibvaihtelee && Tunniste == 1) {
		if (Application->MessageBoxW(L"Käytä valintaan perusnumeroa, kun rintanumero ei vaihtele. Käytetäänkö poikkeavasti vaiheen tietoa?", L"Huomautus", MB_YESNO) == IDNO)
			return;
		}
	if (ToimintaTila == 2 && ((Tieto == 0 && !kilpparam.bibvaihtelee) ||
		(Tieto == 1 && epv <= k_pv && vpv >= k_pv))) {
		if (Application->MessageBoxW(L"Tässä vaiheessa käytössä olevia numeroita koskeva siirto toimii ennustettavasti "
			L"vain, jos numeroiden kohdealue on kokonaisuudessaan vapaa. Toteutetaanko siirto silti?",
			L"Varoitus", MB_YESNO) == IDNO)
			return;
        }
	switch (Tunniste) {
		case 4:
			LTunn = kilpparam.lseura;
			break;
		case 5:
			LTunn = kilpparam.lseuralyh;
			break;
		case 6:
			LTunn = kilpparam.ljoukkue;
			break;
		default:
			LTunn = 0;
		}
	switch (Tieto) {
		case 6:
		case 24:
		case 25:
		case 26:
		case 27:
		case 33:
			LTieto = -1;
			break;
		case 7:
			LTieto = 10;
			break;
		case 8:
			LTieto = kilpparam.lseura;
			break;
		case 9:
			LTieto = kilpparam.lseuralyh;
			break;
		case 10:
			LTieto = 3;
			break;
		case 12:
			LTieto = kilpparam.ljoukkue;
			break;
		case 13:
			LTieto = kilpparam.lyhdistys;
			break;
		case 14:
			LTieto = kilpparam.lenimi;
			break;
		case 15:
			LTieto = kilpparam.lsnimi;
			break;
		case 16:
		case 17:
			LTieto = kilpparam.lenimi+kilpparam.lsnimi+1;
			break;
		case 18:
		case 29:
		case 34:
		case 35:
			LTieto = 1;
			break;
		case 31:
			LTieto = kilpparam.lsakmax;
		default:
			LTieto = 0;
		}
	if (sellaisinaan <= 0) {
		if (sellaisinaan == 0)
			OpenDialog1->InitialDir = FormMain->CurrentDir;
		if (sellaisinaan < 0 || OpenDialog1->Execute()) {
			TextFl *InFile;
			if (sellaisinaan == 0)
				InFile = new TextFl(OpenDialog1->FileName.c_str(), L"rt");

			if (sellaisinaan < 0 || InFile->IsOpen()) {

				if (sellaisinaan == 0) {
					while (!InFile->Feof()) {
						wchar_t line[1000];

						InFile->ReadLine(line, 999);
						if (wcslen(line) > 3)
							nrv++;
						}
					}
				else {
					nrv = FormTietoTulk->tietolkm;
					}
				if (nrv == 0) {
					Application->MessageBoxW(L"Ei siirretävää tietoa", L"Virhe", MB_OK);
					delete InFile;
					return;
					}
				if (sellaisinaan == 0)
					InFile->Rewind();
				if (LTunn == 0 && LTieto <= 0) {
					nton = new typenton[nrv];
					}
				if (LTunn == 0 && LTieto > 0) {
					ntos = new typentos[nrv];
					}
				if (LTunn > 0 && LTieto <= 0) {
					ston = new typeston[nrv];
					}
				if (LTunn > 0 && LTieto > 0) {
					stos = new typestos[nrv];
					}
				nrv = 0;
				while (sellaisinaan < 0 || !InFile->Feof()) {
					wchar_t line[1000], *p, tunnst[LSEURA+1], tietost[LSEURA+1];

					if (sellaisinaan == 0) {
						InFile->ReadLine(line, 999);
						if (wcslen(line) <= 3)
						   continue;
						tunnst[0] = 0;
						tietost[0] = 0;
						tunnst[LSEURA] = 0;
						tietost[LSEURA] = 0;
						if (erotin == L' ')
							p = wcstok(line, L" \t\n");
						else
							p = wcstokch(line, erotin);
						for (int i = 0; p != NULL; i++) {
							if (i == TunnSar-1)
								wcsncpy(tunnst, p, LSEURA);
							if (i == TietoSar-1)
								wcsncpy(tietost, p, LSEURA);
							elimwbl2(tunnst);
							elimwbl2(tietost);
							if (tunnst[0] && tietost[0])
								break;
							if (erotin == L' ')
								p = wcstok(NULL, L" \t\n");
							else
								p = wcstokch(NULL, erotin);
							}
						}
					else {
						if (nrv >= FormTietoTulk->tietolkm)
							break;
						FormTietoTulk->tunniste(tunnst, LSEURA, nrv);
						FormTietoTulk->tieto(tietost, LSEURA, nrv);
						}
					if (tunnst[0] && tietost[0]) {
						if (LTunn == 0) {
							if (nton) {
								if (Tunniste == 10) {
									nton[nrv].in = haesarja_w(tunnst, false);
									}
								else {
									if (Tunniste == 2 && tunnst[0] == L'T')
										nton[nrv].in = _wtoi(tunnst+1);
									else
										nton[nrv].in = _wtoi(tunnst);
									}
								}
							if (ntos) {
								if (Tunniste == 2 && tunnst[0] == L'T')
									ntos[nrv].in = _wtoi(tunnst+1);
								else
									ntos[nrv].in = _wtoi(tunnst);
								}
							}
						if (LTunn > 0) {
							if (ston)
								keyfromwname(ston[nrv].in, tunnst, (2*LSEURA+1)/3, 0);
							if (stos)
								keyfromwname(stos[nrv].in, tunnst, (2*LSEURA+1)/3, 0);
							}
						if (LTieto < 0) {
							int val = 0;
							if (!wmemcmp(tietost, L"---", 3)) {
							   val = 9999999;
								}
							else if ((p = wcsstr(tietost, L".")) != NULL || (p = wcsstr(tietost, L",")) != NULL || CBDesLuku->Checked) {
								if (p && *p == L',')
									*p = L'.';
								val = (int) floor(deskerroin * _wtof(tietost)+0.5);
								}
							else {
								val = _wtoi(tietost);
								}
							if (nton)
								nton[nrv].out = val;
							if (ston)
								ston[nrv].out = val;
							}
						if (LTieto == 0) {
							if (Tieto == 20) {
								int srj = haesarja_w(tietost, false);
								if (nton)
									nton[nrv].out = srj;
								if (ston)
									ston[nrv].out = srj;
								}
							else if (Tieto == 22) {
								if (nton)
									nton[nrv].out = wstrtoaika_vap(tietost, t0);
								if (ston)
									ston[nrv].out = wstrtoaika_vap(tietost, t0);
								}
							else if (Tieto == 23 || Tieto == 36 || Tieto == 37) {
								if (nton)
									nton[nrv].out = wstrtoaika_vap(tietost, 0);
								if (ston)
									ston[nrv].out = wstrtoaika_vap(tietost, 0);
								}
							else {
								int sft = 0;
								if (Tieto == 2 && tietost[0] == L'T')
									sft = 1;
								if (nton)
									nton[nrv].out = _wtoi(tietost+sft);
								if (ston)
									ston[nrv].out = _wtoi(tietost+sft);
								}
							}
						if (LTieto > 0) {
							if (ntos)
								wcscpy(ntos[nrv].out, tietost);
							if (stos)
								wcscpy(stos[nrv].out, tietost);
							}
						nrv++;
						}
					}
				if (nrv) {
					if (nton) {
						qsortint(nton, nrv, sizeof(nton[0]), 0, 1);
						}
					if (ntos) {
						qsortint(ntos, nrv, sizeof(ntos[0]), 0, 1);
						}
					if (ston) {
						quicksort((char *)ston, nrv, sizeof(ston[0].in), sizeof(ston[0]));
						}
					if (stos) {
						quicksort((char *)stos, nrv, sizeof(stos[0].in), sizeof(stos[0]));
						}
					}
				}
			else {
				Application->MessageBoxW(L"Tiedoston avaaminen ei onnistunut", L"Este", MB_OK);
				}
			if (sellaisinaan == 0)
				delete InFile;
			}
		else
			return;
		}
	kilptietue kilp;

	TulkEhdot();
	EnterCriticalSection(&tall_CriticalSection);
	for (int d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (!kilp.kilpstatus) {
			int ikey = -999999999, ival = -999999999, pos;
			char key[(2*LSEURA+1)/3] = "";
			wchar_t sval[LSNIMI+LENIMI+2] = L"\x01";
			int keyl = (2 * LSEURA + 1) / 3;

			if (!Ehto(&kilp))
				continue;
			switch (Tunniste) {
				case 0:
					ikey = kilp.kilpno;
					break;
				case 1:
					ikey = kilp.pv[TunnPv].bib;
					break;
				case 2:
					ikey = kilp.lisno[0];
					break;
				case 3:
					ikey = kilp.lisno[1];
					break;
				case 4:
					if (sellaisinaan > 0)
						wcscpy(sval, kilp.seura);
					else
						keyfromwname(key, kilp.seura, (2*LSEURA+1)/3, 0);
					break;
				case 5:
					if (sellaisinaan > 0)
						wcscpy(sval, kilp.seuralyh);
					else
						keyfromwname(key, kilp.seuralyh, (2*LSEURA+1)/3, 0);
					break;
				case 6:
					if (sellaisinaan > 0)
						wcscpy(sval, kilp.joukkue);
					else
						keyfromwname(key, kilp.joukkue, (2*LSEURA+1)/3, 0);
					break;
				case 7:
					ikey = kilp.pv[TunnPv].badge[0];
					break;
				case 8:
					if (sellaisinaan > 0)
						_itow(kilp.ilmlista+Lisays, sval, 10);
					else
						ikey = kilp.ilmlista;
					break;
				case 9:
					if (sellaisinaan > 0)
						wcscpy(sval, kilp.pv[TunnPv].rata);
					else
						keyfromwname(key, kilp.pv[TunnPv].rata, (2*LSEURA+1)/3, 0);
					break;
				case 10:
					ikey = kilp.Sarja(TunnPv);
					break;
				case 11:
					if (sellaisinaan > 0)
						_itow(kilp.piiri+Lisays, sval, 10);
					else
						ikey = kilp.piiri;
					break;
				}
			if (sellaisinaan > 0 && Tunniste == PohjTunniste) {
				ival = _wtoi(EdtPohjustus->Text.c_str());
				wcsncpy(sval, EdtPohjustus->Text.c_str(), LSEURA);
			}
			if (sellaisinaan > 0 && ikey != -999999999) {
				ival = ikey + Lisays;
				}
			if (nton && ikey != -999999999) {
				pos = bsrchint(ikey, nton, nrv, sizeof(nton[0]), 0, 1);
				if (pos < 0 || nton[pos].in != ikey)
					continue;
				ival = nton[pos].out;
				}
			if (ntos && ikey != -999999999) {
				pos = bsrchint(ikey, ntos, nrv, sizeof(ntos[0]), 0, 1);
				if (pos < 0 || ntos[pos].in != ikey)
					continue;
				wcsncpy(sval, ntos[pos].out, LSEURA);
				}
			if (ston && key[0]) {
				typeston *ston1;

				pos = nrv;
				ston1 = (typeston *)bsrch(key, (char *)ston, &pos, sizeof(ston[0].in), sizeof(ston[0]));
				if (!ston1 || memcmp(ston1->in, key, keyl))
					continue;
				ival = ston1->out;
				}
			if (stos && key[0]) {
				typestos *stos1;

				pos = nrv;
				stos1 = (typestos *)bsrch(key, (char *)stos, &pos, sizeof(stos[0].in), sizeof(stos[0]));
				if (!stos1 || memcmp(stos1->in, key, keyl))
					continue;
				wcsncpy(sval, stos1->out, LSEURA);
				}
			if (ival != -999999999 || sval[0] != L'\x01') {
				wchar_t *p;
				int len;

				switch (Tieto) {
					case 0:
					   kilp.kilpno = ival;
					   if (!kilpparam.bibvaihtelee)
						  kilp.setIdAll(ival);
					   break;
					case 1:
						for (int ip = epv; ip <= vpv; ip++)
							kilp.pv[ip].bib = ival;
					   break;
					case 2:
					   kilp.lisno[0] = ival;
					   break;
					case 3:
					   kilp.lisno[1] = ival;
					   break;
					case 4:
						for (int ip = epv; ip <= vpv; ip++)
							kilp.pv[ip].badge[0] = ival;
					   break;
					case 5:
						for (int ip = epv; ip <= vpv; ip++)
							kilp.pv[ip].badge[1] = ival;
					   break;
					case 6:
					   kilp.pisteet[2] = ival;
					   break;
					case 7:
						for (int ip = epv; ip <= vpv; ip++)
							wcsncpy(kilp.pv[ip].rata, sval, sizeof(kilp.pv[ip].rata)/2-1);
					   break;
					case 8:
						wcsncpy(kilp.seura, sval, kilpparam.lseura);
					   break;
					case 9:
						wcsncpy(kilp.seuralyh, sval, kilpparam.lseuralyh);
					   break;
					case 10:
						wcsncpy(kilp.maa, sval, 3);
					   break;
					case 11:
					   kilp.piiri = ival;
					   break;
					case 12:
						wcsncpy(kilp.joukkue, sval, kilpparam.ljoukkue);
					   break;
					case 13:
						wcsncpy(kilp.yhdistys, sval, kilpparam.lyhdistys);
					   break;
					case 14:
						wcsncpy(kilp.etunimi, sval, kilpparam.lenimi);
					   break;
					case 15:
						wcsncpy(kilp.sukunimi, sval, kilpparam.lsnimi);
					   break;
					case 16:
						len = wcslen(sval);
						p = wcstok(sval, L" ");
						wcsncpy(kilp.sukunimi, sval, kilpparam.lsnimi);
						for (p = sval+wcslen(sval); p < sval+len && !*p; p++) ;
						wcsncpy(kilp.etunimi, p, kilpparam.lenimi);
						break;
					case 17:
						for (p = sval+wcslen(sval)-1; p > sval && *p != L' '; p--) ;
						if (p > sval) {
							*p = 0;
							p++;
							wcsncpy(kilp.etunimi, sval, kilpparam.lenimi);
							}
						wcsncpy(kilp.sukunimi, p, kilpparam.lsnimi);
						break;
					case 18:
						for (int ip = epv; ip <= vpv; ip++)
							kilp.set_tark(sval[0], ip);
						break;
					case 19:
						for (int ip = epv; ip <= vpv; ip++)
							kilp.pv[ip].laina[0] = L'L';
						break;
					case 20:
						if (ival >= 0) {
							if (kilpparam.n_pv_akt == 1 || !kilpparam.sarjavaihtelee ||
								vpv > epv)
								kilp.setSarjaAll(ival);
							else
								kilp.setSarja(ival, epv);
							}
						break;
					case 21:
						for (int ip = epv; ip <= vpv; ip++)
							kilp.pv[ip].era = ival;
						break;
					case 22:
						for (int ip = epv; ip <= vpv; ip++)
							kilp.tall_lajat_pv(ival, ip);
						break;
					case 23:
						for (int ip = epv; ip <= vpv; ip++)
							kilp.tall_tulos_pv(ival, ip, 0);
						break;
					case 24:
						for (int ip = epv; ip <= vpv; ip++)
							kilp.pv[ip].pvpisteet[0] = ival;
						break;
					case 25:
						for (int ip = epv; ip <= vpv; ip++)
							kilp.pv[ip].pvpisteet[1] = ival;
						break;
					case 26:
						kilp.pisteet[0] = ival;
						break;
					case 27:
						kilp.pisteet[1] = ival;
						break;
					case 28:
						kilp.synt = ival;
						break;
					case 29:
						kilp.sukup = sval[0];
						break;
					case 30:
						kilp.ikasarja = ival;
						break;
					case 31:
						for (int ip = epv; ip <= vpv; ip++) {
							kilp.set_Asakot(sval, ip);
							}
					   break;
					case 32:
						for (int ip = epv; ip <= vpv; ip++) {
							kilp.pv[ip].tasoitus = ival;
							}
					   break;
					case 33:
						for (int ip = epv; ip <= vpv; ip++) {
							kilp.pv[ip].sakko = ival;
							}
					   break;
					case 34:
						for (int ip = epv; ip <= vpv; ip++) {
							kilp.pv[ip].qual = (char)sval[0];
							}
						break;
					case 35:
						for (int ip = epv; ip <= vpv; ip++) {
							kilp.pv[ip].gps = (char)sval[0];
							}
						break;
					case 36:
						for (int ip = epv; ip <= vpv; ip++) {
							kilp.pv[ip].enn = ival;
							}
						break;
					case 37:
						for (int ip = epv; ip <= vpv; ip++) {
							kilp.pv[ip].tav = ival;
							}
						break;
					}
				kilp.tallenna(d, 0, 0, 0, 0);
				nsiirr++;
				}
			}
		}
	LeaveCriticalSection(&tall_CriticalSection);
	Application->MessageBoxW((UnicodeString(L"Käsitelty ")+UnicodeString(nrv)+
		L" siirto-ohjetta, siirretty "+UnicodeString(nsiirr)+L" tietoa").c_str(),
		L"Valmis", MB_OK);
	if (nton)
		delete[] nton;
	if (ntos)
		delete[] ntos;
	if (ston)
		delete[] ston;
	if (stos)
		delete[] stos;
}
//---------------------------------------------------------------------------
void __fastcall TFormLisatiedot::FormClose(TObject *Sender, TCloseAction &Action)

{
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------

void __fastcall TFormLisatiedot::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Lisätietojen siirto");
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TFormLisatiedot::Button1Click(TObject *Sender)
{
	SiirraTiedot(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormLisatiedot::Button2Click(TObject *Sender)
{
	SiirraTiedot(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormLisatiedot::FormShow(TObject *Sender)
{
	CBTunnPv->Clear();
	CBTietoPv->Clear();
	CBEhtoVaihe->Clear();
	CBTunnPv->Items->Add(L"Ei valittu");
	CBTietoPv->Items->Add(L"Ei valittu");
	CBTietoPv->Items->Add(L"Kaikki");
	CBEhtoVaihe->Items->Add(L"Perustieto");
	for (int i = 0;i < kilpparam.n_pv_akt; i++) {
	   CBTunnPv->Items->Add(UnicodeString(i+1)+L". vaihe");
	   CBTietoPv->Items->Add(UnicodeString(i+1)+L". vaihe");
	   CBEhtoVaihe->Items->Add(UnicodeString(i+1)+L". vaihe");
	   }
	if (kilpparam.n_pv_akt == 1) {
		CBTunnPv->ItemIndex = 1;
		CBTietoPv->ItemIndex = 1;
		GBKohdevaihe->Visible = false;
		GBTunnvaihe->Visible = false;
		}
	else {
		CBTunnPv->ItemIndex = 0;
		CBTietoPv->ItemIndex = 0;
		GBKohdevaihe->Visible = true;
		GBTunnvaihe->Visible = true;
		}
	CBEhto->ItemIndex = 0;
	CBEhtoVaihe->ItemIndex = 0;
	EdtEhto->Text = L"";
	EdtLisays->Text = 0;
	Lisays = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormLisatiedot::Button3Click(TObject *Sender)
{
	int Tunniste, Tieto, TunnPv, TietoPv, TunnSar, TietoSar, LTunn, LTieto, epv, vpv;
	int nrv = 0, nsiirr = 0;
	wchar_t erotin, erottimet[] = L" \t;,";
	kilptietue kilp;

	Tunniste = RGTunniste->ItemIndex;
	if (Tunniste == RGTunniste->Items->Count-1) {
		Application->MessageBoxW(L"Pohjustus ei mahdollinen tiedostoon kirjoitettaessa" , L"Virhe", MB_OK);
		}
	Tieto = RGTieto->ItemIndex;
	TunnPv = CBTunnPv->ItemIndex - 1;
	TietoPv = CBTietoPv->ItemIndex - 2;
	erotin = erottimet[CBErotin->ItemIndex];
	if (TietoPv < 0) {
		epv = 0;
		vpv = kilpparam.n_pv_akt-1;
		}
	else {
		epv = TietoPv;
		vpv = epv;
		}
	if (Tunniste < 0 || Tieto < 0) {
		Application->MessageBoxW(L"Valitse molemmat kentät", L"Virhe", MB_OK);
		return;
		}
	if ((Tunniste == 1 || Tunniste == 7 || Tunniste == 9) && kilpparam.n_pv_akt > 1
		&& TunnPv < 0) {
		Application->MessageBoxW(L"Valitse rintanumeron tai koodin päivä", L"Virhe", MB_OK);
		return;
		}
	if (TietoPv < -1 && kilpparam.n_pv_akt > 1 && monivaiheTieto[Tieto] > '0') {
		Application->MessageBoxW(L"Valitse kohdetiedon vaihe", L"Este", MB_OK);
		return;
		}
	if (Tunniste > 3 && Tunniste < 7 && Tieto < 7) {
		Application->MessageBoxW(L"Mahdoton yhdistelmä", L"Virhe", MB_OK);
		return;
		}
	if (!kilpparam.bibvaihtelee && Tunniste == 1) {
		if (Application->MessageBoxW(L"Käytä valintaan perusnumeroa, kun rintanumero ei vaihtele. Käytetäänkö poikkeavasti vaiheen tietoa?", L"Huomautus", MB_YESNO) == IDNO)
			return;
		}
	TulkEhdot();
	SaveDialog1->InitialDir = FormMain->CurrentDir;
	if (SaveDialog1->Execute()) {
		TextFl *OutFile = new TextFl(SaveDialog1->FileName.c_str(), L"wt");

		if (OutFile && OutFile->IsOpen()) {

			for (int d = 1; d < nrec; d++) {
				kilp.GETREC(d);
				if (!kilp.kilpstatus) {
					int ikey = -999999999;
					int ival[N_PV];
					bool monivaihe = false;
					wchar_t key[200] = L"";
					wchar_t line[400] = L"", as[40];

					for (int ip = 0; ip < N_PV; ip++)
						ival[ip] = 0x40000000;
					if (!Ehto(&kilp))
						continue;
					switch (Tunniste) {
						case 0:
							ikey = kilp.kilpno;
							break;
						case 1:
							ikey = kilp.pv[TunnPv].bib;
							break;
						case 2:
							ikey = kilp.lisno[0];
							break;
						case 3:
							ikey = kilp.lisno[1];
							break;
						case 4:
							wcsncpy(key, kilp.seura, LSEURA);
							break;
						case 5:
							wcsncpy(key, kilp.seuralyh, LLYH);
							break;
						case 6:
							wcsncpy(key, kilp.joukkue, LJK);
							break;
						case 7:
							ikey = kilp.pv[TunnPv].badge[0];
							break;
						case 8:
							ikey = kilp.ilmlista;
							break;
						case 9:
							wcsncpy(key, kilp.pv[TunnPv].rata, sizeof(kilp.pv[TunnPv].rata)/2-1);
							break;
						case 10:
							wcscpy(key, Sarjat[kilp.Sarja(TunnPv)].sarjanimi);
							break;
						case 11:
							ikey = kilp.piiri;
							break;
						}
					if (ikey != -999999999)
						swprintf(line, L"%d", ikey);
					else
						swprintf(line, L"\"%s\"", key);
					switch (Tieto) {
						case 0:
							ival[0] = kilp.kilpno;
						   break;
						case 1:
							monivaihe = true;
							for (int ip = epv; ip <= vpv; ip++)
								ival[ip] = kilp.pv[ip].bib;
						   break;
						case 2:
						   ival[0] = kilp.lisno[0];
						   break;
						case 3:
						   ival[0] = kilp.lisno[1];
						   break;
						case 4:
							monivaihe = true;
							for (int ip = epv; ip <= vpv; ip++)
								ival[ip] = kilp.pv[ip].badge[0];
						   break;
						case 5:
							monivaihe = true;
							for (int ip = epv; ip <= vpv; ip++)
								ival[ip] = kilp.pv[ip].badge[1];
						   break;
						case 6:
						   ival[0] = kilp.pisteet[2];
						   break;
						case 7:
							for (int ip = epv; ip <= vpv; ip++)
							   swprintf(line+wcslen(line), L"%s%s", erotin, kilp.pv[ip].rata);
						   break;
						case 8:
						   swprintf(line+wcslen(line), L"%c\"%s\"", erotin, kilp.seura);
						   break;
						case 9:
						   swprintf(line+wcslen(line), L"%c\"%s\"", erotin, kilp.seuralyh);
						   break;
						case 10:
						   swprintf(line+wcslen(line), L"%c\"%s\"", erotin, kilp.maa);
						   break;
						case 11:
						   ival[0] = kilp.piiri;
						   break;
						case 12:
						   swprintf(line+wcslen(line), L"%c\"%s\"", erotin, kilp.joukkue);
						   break;
						case 13:
						   swprintf(line+wcslen(line), L"%c\"%s\"", erotin, kilp.yhdistys);
						   break;
						case 14:
						   swprintf(line+wcslen(line), L"%c\"%s\"", erotin, kilp.etunimi);
						   break;
						case 15:
						   swprintf(line+wcslen(line), L"%c\"%s\"", erotin, kilp.sukunimi);
						   break;
						case 16:
						   swprintf(line+wcslen(line), L"%c\"%s %s\"", erotin, kilp.sukunimi, kilp.etunimi);
							break;
						case 17:
						   swprintf(line+wcslen(line), L"%c\"%s %s\"", erotin, kilp.etunimi, kilp.sukunimi);
							break;
						case 18:
							for (int ip = epv; ip <= vpv; ip++)
								swprintf(line+wcslen(line), L"%c%c", erotin, kilp.tark(ip));
							break;
						case 19:
							for (int ip = epv; ip <= vpv; ip++)
								swprintf(line+wcslen(line), L"%c%c", erotin,
									kilp.pv[ip].laina[0] ? kilp.pv[ip].laina[0] : L' ');
							break;
						case 20:
							if (kilpparam.sarjavaihtelee) {
								for (int ip = epv; ip <= vpv; ip++) {
									if (kilp.Sarja(ip) >= 0)
										swprintf(line+wcslen(line), L"%c\"%s\"", erotin, Sarjat[kilp.Sarja(ip)].sarjanimi);
									else
										swprintf(line+wcslen(line), L"%c\"%s\"", erotin, L" ");
									}
								}
							else {
								if (kilp.sarja >= 0)
									swprintf(line+wcslen(line), L"%c\"%s\"", erotin, Sarjat[kilp.sarja].sarjanimi);
								else
									swprintf(line+wcslen(line), L"%c\"%s\"", erotin, L" ");
								}
							break;
						case 21:
							monivaihe = true;
							for (int ip = epv; ip <= vpv; ip++)
								ival[ip] = kilp.pv[ip].era;
							break;
						case 22:
							for (int ip = epv; ip <= vpv; ip++) {
								swprintf(line+wcslen(line), L"%c\"%s\"", erotin,
									aikatowstr_cols_n(as, kilp.TLahto(ip, false), t0, L',', pvparam[ip].laika2));
								if (pvparam[ip].hiihtolahto)
									swprintf(line+wcslen(line), L"%c\"%s\"", erotin,
										aikatowstr_cols_n(as, kilp.TLahto(ip, true), t0, L',', pvparam[ip].laika));
								}
							break;
						case 23:
							for (int ip = epv; ip <= vpv; ip++)
								swprintf(line+wcslen(line), L"%c\"%s\"", erotin,
									aikatowstr_cols_n(as, kilp.tulos_pv(ip, false), 0, L',', pvparam[ip].laika2));
							break;
						case 24:
							monivaihe = true;
							for (int ip = epv; ip <= vpv; ip++)
								ival[ip] = kilp.pv[ip].pvpisteet[0];
							break;
						case 25:
							monivaihe = true;
							for (int ip = epv; ip <= vpv; ip++)
								ival[ip] = kilp.pv[ip].pvpisteet[1];
							break;
						case 26:
							ival[0] = kilp.pisteet[0];
							break;
						case 27:
							ival[0] = kilp.pisteet[1];
							break;
						case 28:
							swprintf(line+wcslen(line), L"%c%d", erotin, kilp.synt);
							break;
						case 29:
							swprintf(line+wcslen(line), L"%c%c", erotin, kilp.sukup);
							break;
						case 30:
							swprintf(line+wcslen(line), L"%c%d", erotin, kilp.ikasarja);
							break;
						case 31:
							for (int ip = epv; ip <= vpv; ip++)
								swprintf(line+wcslen(line), L"%s%s", erotin, kilp.asakkoStr(ip));
							break;
						case 32:
							monivaihe = true;
							for (int ip = epv; ip <= vpv; ip++)
								ival[ip] = kilp.pv[ip].tasoitus;
							break;
						case 33:
							monivaihe = true;
							for (int ip = epv; ip <= vpv; ip++)
								ival[ip] = kilp.pv[ip].sakko;
							break;
						case 34:
							for (int ip = epv; ip <= vpv; ip++)
								swprintf(line+wcslen(line), L"%c%c", erotin, (wchar_t)kilp.pv[ip].qual);
							break;
						case 35:
							for (int ip = epv; ip <= vpv; ip++)
								swprintf(line+wcslen(line), L"%c%c", erotin, (wchar_t)kilp.pv[ip].gps);
							break;
						case 36:
							for (int ip = epv; ip <= vpv; ip++)
								swprintf(line+wcslen(line), L"%c\"%s\"", erotin,
									aikatowstr_cols_n(as, kilp.pv[ip].enn, 0, L',', pvparam[ip].laika2));
							break;
						case 37:
							for (int ip = epv; ip <= vpv; ip++)
								swprintf(line+wcslen(line), L"%c\"%s\"", erotin,
									aikatowstr_cols_n(as, kilp.pv[ip].tav, 0, L',', pvparam[ip].laika2));
							break;
						}
					if (monivaihe || ival[0] != 0x40000000) {
						for (int ip = (monivaihe ? epv : 0); ip <= (monivaihe ? vpv : 0); ip++) {
						if (IntInList(Tieto, deskohde) < 0 || CBKirjDesLuku->ItemIndex <= 0)
								swprintf(line+wcslen(line), L"%c%d", erotin, ival[ip]);
							else {
								wchar_t numst[30] = L"", *pp;
								wchar_t fmt[10] = L"%f";
								if (deskerroin == 10)
									wcscpy(fmt, L"%.1f");
								if (deskerroin == 100)
									wcscpy(fmt, L"%.2f");
								if (deskerroin == 1000)
									wcscpy(fmt, L"%.3f");
								if (deskerroin == 10000)
									wcscpy(fmt, L"%.4f");
								swprintf_s(numst, 29, fmt, (double)ival[ip]/deskerroin);
								if ((pp = wcsstr(numst, L".")) != NULL) {
									if (wcslen(numst) > pp - numst + 4)
										pp[4] = 0;
									}
								if (CBKirjDesLuku->ItemIndex == 1) {
									if ((pp = wcsstr(numst, L".")) != NULL)
										*pp = L',';
									}
								swprintf(line+wcslen(line), L"%c%s", erotin, numst);
								}
							}
						}
					wcscat(line, L"\n");
					OutFile->WriteLine(line);
					nsiirr++;
					}
				}
			}
		else {
			Application->MessageBoxW(L"Tiedoston avaaminen ei onnistunut", L"Este", MB_OK);
			}
		delete OutFile;
		Application->MessageBoxW(UnicodeString(L"Siirretty "+UnicodeString(nsiirr)+L" tietoa").c_str(),
			L"Valmis", MB_OK);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormLisatiedot::CBEhtoClick(TObject *Sender)
{
	bool Vaiheellinen[] = {false, true, false, false, false, true, false, false, true, true, false, true};
	Ehtolaji = CBEhto->ItemIndex;
	Label5->Enabled = Vaiheellinen[Ehtolaji];
	CBEhtoVaihe->Enabled = Label5->Enabled;
	CBEhtoSrj->Visible = Ehtolaji == 1;
	EdtEhto->Visible = !CBEhtoSrj->Visible;
	if (CBEhtoSrj->Visible)
		LataaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormLisatiedot::LataaSarjat(void)
{
	int EhtoPv;

	EhtoPv = CBEhtoVaihe->ItemIndex - 1;
	CBEhtoSrj->Clear();
	for (int i = 0; i < sarjaluku; i++)
		if (EhtoPv < 0 || (Sarjat[i].enspv <= EhtoPv && Sarjat[i].viimpv >= EhtoPv))
			CBEhtoSrj->Items->Add(Sarjat[i].sarjanimi);
	CBEhtoSrj->ItemIndex = -1;
}
//---------------------------------------------------------------------------

void __fastcall TFormLisatiedot::CBEhtoVaiheClick(TObject *Sender)
{
	if (Ehtolaji == 1)
		LataaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormLisatiedot::TulkEhdot(void)
{
	EhtoPv = CBEhtoVaihe->ItemIndex - 1;
	Ehtolaji = CBEhto->ItemIndex;
	switch (Ehtolaji) {
		case 1:
			EhtoArvo = haesarja_w(CBEhtoSrj->Text.c_str(), false);
			break;
		case 2:
		case 3:
		case 4:
		case 5:
		case 8:
		case 9:
		case 10:
			EhtoArvo = _wtoi(EdtEhto->Text.c_str());
			break;
		case 6:
		case 7:
		case 11:
			wcsncpy(EhtoSt, EdtEhto->Text.c_str(), sizeof(EhtoSt)/2-1);
			break;
		}
}
//---------------------------------------------------------------------------

bool __fastcall TFormLisatiedot::Ehto(kilptietue *kilp)
{
	switch (Ehtolaji) {
		case 1:
			return(kilp->Sarja(EhtoPv) == EhtoArvo);
		case 2:
			return(kilp->lisno[0] == EhtoArvo);
		case 3:
			return(kilp->lisno[1] == EhtoArvo);
		case 4:
			return(kilp->kilpno == EhtoArvo);
		case 5:
			return(kilp->id(EhtoPv) == EhtoArvo);
		case 6:
			return(wcscmp(kilp->seura, EhtoSt) == 0);
		case 7:
			return(wcscmp(kilp->seuralyh, EhtoSt) == 0);
		case 8:
			if (EhtoPv < 0)
				return(false);
			return(kilp->pv[EhtoPv].badge[0] == EhtoArvo);
		case 9:
			if (EhtoPv < 0)
				return(false);
			return(kilp->pv[EhtoPv].badge[1] == EhtoArvo);
		case 10:
			return(kilp->pisteet[2] == EhtoArvo);
		case 11:
			if (EhtoPv < 0)
				return(false);
			return(kilp->tark(EhtoPv) == EhtoSt[0]);
		default:
			return(true);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormLisatiedot::EdtLisaysExit(TObject *Sender)
{
	Lisays = _wtoi(EdtLisays->Text.c_str());
	EdtLisays->Text = Lisays;
}
//---------------------------------------------------------------------------

void __fastcall TFormLisatiedot::EdtKerroinExit(TObject *Sender)
{
	wchar_t st[20] = L"";

	wcsncpy(st, EdtKerroin->Text.c_str(), 19);
	deskerroin = StrToDouble(st);
	EdtKerroin->Text = deskerroin;
}
//---------------------------------------------------------------------------

void __fastcall TFormLisatiedot::Button4Click(TObject *Sender)
{
	if (!FormTietoTulk)
		FormTietoTulk = new TFormTietoTulk(this);
	FormTietoTulk->NaytaSar = true;
	FormTietoTulk->Show();
	if (FormTietoTulk->WindowState == wsMinimized)
		FormTietoTulk->WindowState = wsNormal;
	FormTietoTulk->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormLisatiedot::Button5Click(TObject *Sender)
{
	if (FormTietoTulk && FormTietoTulk->Visible)
		SiirraTiedot(-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormLisatiedot::RGTietoClick(TObject *Sender)
{
	if (IntInList(RGTieto->ItemIndex, deskohde) >= 0) {
		CBKirjDesLuku->Visible = true;
		Label8->Visible = true;
		Label9->Visible = true;
		EdtKerroin->Visible = true;
		LblKerroin->Visible = true;
		}
	else {
		CBKirjDesLuku->Visible = false;
		Label8->Visible = false;
		Label9->Visible = false;
		EdtKerroin->Visible = false;
		LblKerroin->Visible = false;
		}
	if (RGTunniste->ItemIndex == sarjaTunniste && RGTieto->ItemIndex == sarjaTieto)
		Application->MessageBoxW(L"Sarjatietoja voidaan siirtää myös omalla kaavakkeellaan", L"Vihje", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TFormLisatiedot::RGTunnisteClick(TObject *Sender)
{
	if (RGTunniste->ItemIndex == sarjaTunniste && RGTieto->ItemIndex == sarjaTieto)
		Application->MessageBoxW(L"Sarjatietoja voidaan siirtää myös omalla kaavakkeellaan", L"Vihje", MB_OK);
}
//---------------------------------------------------------------------------

