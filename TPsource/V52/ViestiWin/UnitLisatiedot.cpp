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
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormLisatiedot *FormLisatiedot;
//---------------------------------------------------------------------------

#define MAXTIETOPIT 400
#define MAXKEYPIT 60

wchar_t *wcstokch(wchar_t *st, wchar_t erotin);
static int PohjTunnus = 10;
static int KopioTunnus = 11;

__fastcall TFormLisatiedot::TFormLisatiedot(TComponent* Owner)
	: TForm(Owner)
{
}
typedef struct {
	char in[MAXKEYPIT+1];
	wchar_t	out[MAXTIETOPIT+1];
} typestos;

typedef struct {
	char in[MAXKEYPIT+1];
	int	out;
} typeston;

typedef struct {
	int	in;
	wchar_t	out[MAXTIETOPIT+1];
} typentos;

typedef struct {
	int	in;
	int	out;
} typenton;

//---------------------------------------------------------------------------
void __fastcall TFormLisatiedot::SiirraTiedot(bool sellaisinaan)
{
	int Tunniste, Tieto, TunnPv, TunnPvA = 0, TunnPvY = 0, TietoPv, TunnSar, TietoSar, LTunn, LTieto, epv, vpv, Ehtolaji;
	int nrv = 0, nsiirr = 0, EhtoArvo = -1, err = 0;
	double deskerroin = 1000.0;
	wchar_t erotin, erottimet[] = L" \t;,";
	typestos *stos = 0;
	typeston *ston = 0;
	typentos *ntos = 0;
	typenton *nton = 0;

	EdtOhjelkm->Text = 0;
	EdtTietolkm->Text = 0;
	EdtOhjelkm->Refresh();
	EdtTietolkm->Refresh();

	Tunniste = RGTunniste->ItemIndex;
	Tieto = RGTieto->ItemIndex;
	if (GroupBox1->Visible)
		TunnPv = CBTunnPv->ItemIndex - 1;
	else
		TunnPv = -1;
	TietoPv = CBTietoPv->ItemIndex - 1;
	TunnSar = EditTunniste->Text.ToInt();
	TietoSar = EditTieto->Text.ToInt();
	Ehtolaji = RGEhto->ItemIndex;
	if (!sellaisinaan && TietoPv == -1 && Tieto == PohjTunnus) {
		if (Application->MessageBoxW(L"T‰ss‰ valinnassa siirret‰‰n ensimm‰inen hajontakoodi jokaiselle osuudelle. Jatketaanko?",
			L"Varoitus", MB_YESNO) != IDYES)
			return;
		}
	switch (Ehtolaji) {
		case 1:
			EhtoArvo = haesarja_w(EdtEhto->Text.c_str(), true);
			break;
		case 2:
			EhtoArvo = _wtoi(EdtEhto->Text.c_str());
			break;
		}
	if (Ehtolaji && EhtoArvo < 0)
		return;
	erotin = erottimet[CBErotin->ItemIndex];
	if (TietoPv < 0) {
		epv = 0;
		vpv = kilpparam.osuusluku-1;
		}
	else {
		epv = TietoPv;
		vpv = epv;
		}
	if (TunnSar <= 0 || TietoSar <= 0 || TunnSar == TietoSar) {
		Application->MessageBoxW(L"M‰‰rittele sarakkeet", L"Virhe", MB_OK);
		return;
		}
	if (Tunniste < 0 || Tieto < 0) {
		Application->MessageBoxW(L"Valitse molemmat kent‰t", L"Virhe", MB_OK);
		return;
		}
	if (Tunniste == 5 || Tunniste == 6 || Tunniste == 8 || Tunniste == 9) {
		if (TunnPv < 0 && TietoPv < 0) {
			if (Application->MessageBoxW(L"Toimenpide kohdistetaan jokaiseen osuuteen vuorollaan. Toteutetaanko?",
				L"Varoitus", MB_YESNO) != IDYES)
				return;
			TunnPvY = kilpparam.osuusluku - 1;
			}
		else if (TunnPv < 0) {
			Application->MessageBoxW(L"Mahdoton yhdistelm‰", L"Virhe", MB_OK);
			return;
			}
		else {
			TunnPvA = TunnPv;
			TunnPvY = TunnPv;
			}
		}
	if (Tunniste > 1 && Tunniste < 4 && Tieto == 0) {
		Application->MessageBoxW(L"Mahdoton yhdistelm‰", L"Virhe", MB_OK);
		return;
		}
	switch (Tunniste) {
		case 1:
			LTunn = kilpparam.lseura;
			break;
		case 2:
			LTunn = 3;
			break;
		case 3:
			LTunn = -1;
			break;
		case 8:
			LTunn = HAJONTA;
			break;
		default:
			LTunn = 0;
		}
	switch (Tieto) {
		case 1:
			LTieto = kilpparam.lseura;
			break;
		case 2:
			LTieto = 3;
			break;
		case 3:
			LTieto = -1;
			break;
		case 4:
			LTieto = LJK;
			break;
		case 9:
			LTieto = HAJONTA;
			break;
		case 12:
		case 13:
		case 14:
		case 15:
			LTieto = kilpparam.lnimi;
			break;
		case 16:
		case 17:
		case 19:
		case 26:
			LTieto = 1;
			break;
		case 18:
		case 20:
			LTieto = -2;
			break;
		case 24:
			LTieto = kilpparam.lsakmax;
			break;
		case 27:
			LTieto = MAXTIETOPIT;
			break;
		default:
			LTieto = 0;
			break;
		}
	if (!sellaisinaan) {
		if (OpenDialog1->Execute()) {
			TextFl *InFile = new TextFl(OpenDialog1->FileName.c_str(), L"rt");

			if (InFile->IsOpen()) {

				while (!InFile->Feof()) {
					wchar_t line[1000];

					InFile->ReadLine(line, 999);
					if (wcslen(line) > 3)
						nrv++;
					}
				if (nrv == 0) {
					Application->MessageBoxW(L"Ei siirret‰v‰‰ tietoa", L"Virhe", MB_OK);
					delete InFile;
					return;
					}
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
				while (!InFile->Feof()) {
					wchar_t line[1000], *p, tunnst[MAXKEYPIT+10], tietost[MAXTIETOPIT+1];

					InFile->ReadLine(line, 999);
					if (wcslen(line) <= 3)
					   continue;
					tunnst[0] = 0;
					tietost[0] = 0;
					tunnst[MAXKEYPIT] = 0;
					tietost[MAXTIETOPIT] = 0;
					if (erotin == L' ')
						p = wcstok(line, L" \t\n");
					else
						p = wcstokch(line, erotin);
					for (int i = 0; p != NULL; i++) {
						if (i == TunnSar-1)
							wcsncpy(tunnst, p, MAXKEYPIT);
						if (i == TietoSar-1)
							wcsncpy(tietost, p, MAXTIETOPIT);
						elimwbl2(tunnst);
						elimwbl2(tietost);
						if (tunnst[0] && tietost[0])
							break;
						if (i >= TunnSar-1 && i == TietoSar-2 && LTieto == MAXTIETOPIT) {
							p = p+wcslen(p)+1;
							wcsncpy(tietost, p, MAXTIETOPIT);
							break;
							}
						if (erotin == L' ')
							p = wcstok(NULL, L" \t\n");
						else
							p = wcstokch(NULL, erotin);
						}
					if (tunnst[0] && tietost[0]) {
						if (LTunn == 0) {
							if (nton)
								nton[nrv].in = _wtoi(tunnst);
							if (ntos)
								ntos[nrv].in = _wtoi(tunnst);
							}
						if (LTunn == -1) {
							int srj = haesarja_w(tunnst, false);
							if (srj >= 0) {
								if (nton)
									nton[nrv].in = srj;
								if (ntos)
									ntos[nrv].in = srj;
								}
							}
						if (LTunn > 0) {
							if (ston)
								keyfromU16name(ston[nrv].in, tunnst, MAXKEYPIT, 0);
							if (stos)
								keyfromU16name(stos[nrv].in, tunnst, MAXKEYPIT, 0);
							}
						if (LTieto == -1) {
							int srj = haesarja_w(tietost, false);
							if (nton)
								nton[nrv].out = srj;
							if (ston)
								ston[nrv].out = srj;
							}
						if (LTieto == -2) {
							if (nton)
								nton[nrv].out = wstrtoaika_vap(tietost, t0);
							if (ston)
								ston[nrv].out = wstrtoaika_vap(tietost, t0);
							}
						if (LTieto == 0) {
							int val;
							if (CBDes1000->Checked) {
								if ((p = wcsstr(tietost, L",")) != NULL)
									*p = L'.';
								val = (int) floor(deskerroin * _wtof(tietost)+0.5);
								}
							else
								val = _wtoi(tietost);
							if (nton)
								nton[nrv].out = val;
							if (ston)
								ston[nrv].out = val;
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
			delete InFile;
			}
		else
			return;
		}
	kilptietue kilp;

	EdtOhjelkm->Text = nrv;
	EdtOhjelkm->Refresh();
	Sleep(100);
	EnterCriticalSection(&tall_CriticalSection);
	UINT32 kirjheti0 = kirjheti;
	kirjheti = 0;
	for (int TunnPv = TunnPvA; TunnPv <= TunnPvY; TunnPv++) {
		if (TunnPvA < TunnPvY) {
			epv = TunnPv;
			vpv = TunnPv;
			}
		for (int d = 1; d < datf2.numrec; d++) {
			kilp.getrec(d);
			if (!kilp.kilpstatus) {
				int ikey = 0, ival = -999999999, pos;
				char key[MAXKEYPIT+20] = "";
				wchar_t sval[MAXTIETOPIT+20] = L"\x01", st[200];
				int keyl = MAXKEYPIT + 1;

				if (Ehtolaji == 1 && kilp.sarja != EhtoArvo)
					continue;
				switch (Tunniste) {
					case 0:
						ikey = kilp.kilpno;
						break;
					case 1:
						if (sellaisinaan)
							kilp.Seura(sval, MAXTIETOPIT);
						else
							keyfromU16name(key, kilp.Seura(st, LSEURA), LSEURA, 0);
						break;
					case 2:
						if (sellaisinaan)
							kilp.Maa(sval);
						else
							keyfromU16name(key, kilp.Maa(st), LSEURA, 0);
						break;
					case 3:
						ikey = kilp.sarja;
						break;
					case 4:
						ikey = kilp.joukkid;
						break;
					case 5:
						ikey = kilp.ostiet[TunnPv].lisno;
						break;
					case 6:
						ikey = kilp.ostiet[TunnPv].badge[0];
						break;
					case 7:
						ikey = kilp.piiri;
						break;
					case 8:
						if (sellaisinaan)
							kilp.Hajonta(TunnPv, sval);
						else
							keyfromU16name(key, kilp.Hajonta(TunnPv, st), MAXKEYPIT, 0);
						break;
					case 9:
						ikey = kilp.ostiet[TunnPv].pisteet[0];
						break;
					}
				if (sellaisinaan && Tunniste == PohjTunnus) {
					wcsncpy(sval, EdtPohjustus->Text.c_str(), MAXTIETOPIT);
					if (LTieto == -2)
						ival = wstrtoaika_vap(sval, t0);
					else
						ival = _wtoi(sval);
					}
				if (sellaisinaan && ikey) {
					ival = ikey;
					}
				if (nton && ikey) {
					pos = bsrchint(ikey, nton, nrv, sizeof(nton[0]), 0, 1);
					if (pos < 0 || nton[pos].in != ikey)
						continue;
					ival = nton[pos].out;
					}
				if (ntos && ikey) {
					pos = bsrchint(ikey, ntos, nrv, sizeof(ntos[0]), 0, 1);
					if (pos < 0 || ntos[pos].in != ikey)
						continue;
					wcsncpy(sval, ntos[pos].out, MAXTIETOPIT);
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
					wcsncpy(sval, stos1->out, MAXTIETOPIT);
					}
				if (ival != -999999999 || sval[0] != L'\x01') {
					wchar_t *p;
					int len;

					switch (Tieto) {
						case 0:
						   kilp.kilpno = ival;
						   break;
						case 1:
							kilp.setSeura(sval);
						   break;
						case 2:
							kilp.setMaa(sval);
						   break;
						case 3:
						   kilp.sarja = ival;
						   break;
						case 4:
							kilp.setJoukknimi(sval);
						   break;
						case 5:
						   kilp.joukkid = ival;
						   break;
						case 6:
							for (int ip = epv; ip <= vpv; ip++)
								kilp.ostiet[ip].lisno = ival;
						   break;
						case 7:
							for (int ip = epv; ip <= vpv; ip++)
								kilp.ostiet[ip].badge[0] = ival;
						   break;
						case 8:
							for (int ip = epv; ip <= vpv; ip++)
								kilp.ostiet[ip].badge[1] = ival;
						   break;
						case 9:
							for (int ip = epv; !err && ip <= vpv; ip++)
								err = kilp.setHajonta(ip, sval);
						   break;
						case 10:
						   kilp.piiri = ival;
						   break;
						case 11:
						   kilp.era = ival;
						   break;
						case 12:
							for (int ip = epv; ip <= vpv; ip++)
								kilp.setEtuNimi(sval, ip);
						   break;
						case 13:
							for (int ip = epv; ip <= vpv; ip++)
								kilp.setSukuNimi(sval, ip);
						   break;
						case 14:
							for (int ip = epv; ip <= vpv; ip++)
								kilp.setNimi(sval, ip, -1);
						   break;
						case 15:
							for (int ip = epv; ip <= vpv; ip++)
								kilp.setNimi(sval, ip, 1);
						   break;
						case 16:
							for (int ip = epv; ip <= vpv; ip++)
								kilp.SetTark(ip, sval[0]);
							break;
						case 17:
							for (int ip = epv; ip <= vpv; ip++)
								kilp.setLaina(ip, sval[0]);
							break;
						case 18:
							for (int ip = epv; ip <= vpv; ip++) {
								kilp.ostiet[ip].ylahto = ival;
								}
							break;
						case 19:
							for (int ip = epv; ip <= vpv; ip++) {
								int ll;
								ll = wcswcind(towupper(sval[0]), L"A0Y1K2") / 2;
								if (ll >= 0)
									kilp.ostiet[ip].lahtolaji = ll;
								}
							break;
						case 20:
							for (int ip = epv; ip <= vpv; ip++)
								kilp.setMaali(ip, 0, ival);
							break;
						case 21:
							kilp.pisteet[0] = ival;
							break;
						case 22:
							kilp.pisteet[1] = ival;
							break;
						case 23:
							kilp.pisteet[2] = ival;
							break;
						case 24:
							for (int ip = epv; ip <= vpv; ip++) {
								kilp.setAmpsakot(ip, sval);
								}
						   break;
						case 25:
							for (int ip = epv; ip <= vpv; ip++) {
								kilp.ostiet[ip].sakko = ival;
								}
						   break;
						case 26:
							for (int ip = epv; ip <= vpv; ip++) {
								kilp.setSeuranta(ip, sval[0]);
								}
						   break;
						case 27:
							for (int ip = 0; ip < kilpparam.osuusluku; ip++) {
								wchar_t *p;
								if (ip == 0)
									p = wcstok(sval, L" ;\t\n");
								else
									p = wcstok(NULL, L" ;\t\n");
								if (p == NULL)
									break;
								kilp.setHajonta(ip, p);
								}
							break;
						case 28:
						case 29:
						case 30:
							for (int ip = epv; ip <= vpv; ip++) {
								kilp.ostiet[ip].pisteet[Tieto-28] = ival;
								}
						   break;
						}
					if (err)
						break;
					tallenna(&kilp, d, 0, 0, 0, 0);
					nsiirr++;
					EdtTietolkm->Text = nsiirr;
					if (nsiirr%10 == 0) {
						EdtTietolkm->Refresh();
						Sleep(10);
						}
					}
				}
			if (err)
				break;
			}
		if (err)
			break;
		}
	kirjheti = kirjheti0;
	LeaveCriticalSection(&tall_CriticalSection);
	if (err)
		Application->MessageBoxW(L"Tietojen siirto keskeytyi virheelliseen tietoon (esim. liian pitk‰ merkkijono)",
			L"Virhe", MB_OK);
	Application->MessageBoxW((UnicodeString(L"K‰sitelty ")+UnicodeString(nrv)+
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
	Application->HelpKeyword(L"Ilmoittautumiset");
}
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
void __fastcall TFormLisatiedot::Button1Click(TObject *Sender)
{
	SiirraTiedot(false);
}
//---------------------------------------------------------------------------
void __fastcall TFormLisatiedot::Button2Click(TObject *Sender)
{
	if (RGTunniste->ItemIndex != KopioTunnus)
		SiirraTiedot(true);
	else
		KopioiTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormLisatiedot::FormShow(TObject *Sender)
{
	CBTunnPv->Clear();
	CBTietoPv->Clear();
	CBTunnPv->Items->Add(L"Kaikki vuorollan");
	CBTietoPv->Items->Add(L"Kaikki");
	for (int i = 0;i < kilpparam.osuusluku; i++) {
	   CBTunnPv->Items->Add(UnicodeString(i+1)+L". osuus");
	   CBTietoPv->Items->Add(UnicodeString(i+1)+L". osuus");
	   }
	CBTunnPv->ItemIndex = 0;
	CBTietoPv->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormLisatiedot::Button3Click(TObject *Sender)
{
	int Tunniste, Tieto, TunnPv, TunnPvA = 0, TunnPvY = 0, TietoPv, LTunn, LTieto, epv, vpv, Ehtolaji;
	int nrv = 0, nsiirr = 0, EhtoArvo = -1;
	wchar_t erotin, erottimet[] = L" \t;,";
	kilptietue kilp;

	EdtTietolkm->Text = nsiirr;
	EdtTietolkm->Refresh();
	Sleep(10);
	Tunniste = RGTunniste->ItemIndex;
	if (Tunniste == RGTunniste->Items->Count-1) {
		Application->MessageBoxW(L"Pohjustus ei mahdollinen tiedostoon kirjoitettaessa" , L"Virhe", MB_OK);
		}
	Tieto = RGTieto->ItemIndex;
	TunnPv = CBTunnPv->ItemIndex - 1;
	TietoPv = CBTietoPv->ItemIndex - 1;
	Ehtolaji = RGEhto->ItemIndex;
	switch (Ehtolaji) {
		case 1:
			EhtoArvo = haesarja_w(EdtEhto->Text.c_str(), true);
			break;
		}
	if (Ehtolaji && EhtoArvo < 0)
		return;
	erotin = erottimet[CBErotin->ItemIndex];
	if (TietoPv < 0) {
		epv = 0;
		vpv = kilpparam.osuusluku-1;
		}
	else {
		epv = TietoPv;
		vpv = epv;
		}
	if (Tunniste < 0 || Tieto < 0) {
		Application->MessageBoxW(L"Valitse molemmat kent‰t", L"Virhe", MB_OK);
		return;
		}
	if (Tunniste == 5 || Tunniste == 6 || Tunniste == 8 || Tunniste == 9) {
		if (TunnPv < 0 && TietoPv >= 0) {
			Application->MessageBoxW(L"Valitse tunnistavan tiedon osuus", L"Virhe", MB_OK);
			return;
			}
		else if (TunnPv < 0) {
			Application->MessageBoxW(L"Jokaiselle riville kirjoitetaan toiseen sarakkeeseen osuuden numero",
				L"Varoitus", MB_OK);
			TunnPvY = kilpparam.osuusluku - 1;
			}
		else {
			TunnPvA = TunnPv;
			TunnPvY = TunnPv;
			}
		}
	if (Tunniste > 1 && Tunniste < 4 && Tieto == 0) {
		Application->MessageBoxW(L"Mahdoton yhdistelm‰", L"Virhe", MB_OK);
		return;
		}

	if (SaveDialog1->Execute()) {
		TextFl *OutFile = new TextFl(SaveDialog1->FileName.c_str(), L"wt");

		if (OutFile && OutFile->IsOpen()) {

			for (int d = 1; d < datf2.numrec; d++) {
				int y_os;
				kilp.getrec(d);
				if (!kilp.kilpstatus) {
					int ikey[MAXOSUUSLUKU];
					for (int os = 0; os < MAXOSUUSLUKU; os++)
						ikey[os] = -999999999;
					wchar_t key[MAXOSUUSLUKU][200];
					wchar_t line[400] = L"", as[40];
					memset(key, 0, sizeof(key));

					if (TunnPvA > Sarjat[kilp.sarja].osuusluku - 1)
						continue;
					if (TunnPvY > Sarjat[kilp.sarja].osuusluku - 1)
						y_os = Sarjat[kilp.sarja].osuusluku - 1;
					else
						y_os = TunnPvY;

					if (Ehtolaji == 1 && kilp.sarja != EhtoArvo)
						continue;
					switch (Tunniste) {
						case 0:
							ikey[0] = kilp.kilpno;
							break;
						case 1:
							kilp.Seura(key[0], LSEURA);
							break;
						case 2:
							kilp.Maa(key[0]);
							break;
						case 3:
							Sarjat[kilp.sarja].Sarjanimi(key[0]);
							break;
						case 4:
							ikey[0] = kilp.joukkid;
							break;
						case 5:
							for (int os = TunnPvA; os <= y_os; os++)
								ikey[os] = kilp.ostiet[os].lisno;
							break;
						case 6:
							for (int os = TunnPvA; os <= y_os; os++)
								ikey[os] = kilp.ostiet[os].badge[0];
							break;
						case 7:
							ikey[0] = kilp.piiri;
							break;
						case 8:
							for (int os = TunnPvA; os <= y_os; os++)
								kilp.Hajonta(os, key[os]);
							break;
						case 9:
							for (int os = TunnPvA; os <= y_os; os++)
								ikey[os] = kilp.ostiet[os].pisteet[0];
							break;
						}
					for (int os = TunnPvA; os <= y_os; os++) {
						if (ikey[os] != -999999999)
							swprintf(line, L"%d", ikey[os]);
						else if (key[os][0])
							swprintf(line, L"\"%c\"", key[os]);
						else
							continue;
						if (TunnPvA < TunnPvY) {
							swprintf(line+wcslen(line), L"%c%d", erotin, os+1);
							epv = os;
							vpv = os;
							}
						switch (Tieto) {
							wchar_t st[200];
							case 0:
								swprintf(line+wcslen(line), L"%c%d", erotin, kilp.kilpno);
							   break;
							case 1:
							   swprintf(line+wcslen(line), L"%c%d", erotin, kilp.Seura(st, LSEURA));
							   break;
							case 2:
							   swprintf(line+wcslen(line), L"%c%d", erotin, kilp.Maa(st));
							   break;
							case 3:
							   swprintf(line+wcslen(line), L"%c%d", erotin, Sarjat[kilp.sarja].Sarjanimi(st));
							   break;
							case 4:
							   swprintf(line+wcslen(line), L"%c%d", erotin, kilp.Joukknimi(st, LJK));
							   break;
							case 5:
							   swprintf(line+wcslen(line), L"%c%d", erotin, kilp.joukkid);
							   break;
							case 6:
								for (int ip = epv; ip <= vpv; ip++)
									swprintf(line+wcslen(line), L"%c%d", erotin, kilp.ostiet[ip].lisno);
							   break;
							case 7:
								for (int ip = epv; ip <= vpv; ip++)
									swprintf(line+wcslen(line), L"%c%d", erotin, kilp.ostiet[ip].badge[0]);
							   break;
							case 8:
								for (int ip = epv; ip <= vpv; ip++)
									swprintf(line+wcslen(line), L"%c%d", erotin, kilp.ostiet[ip].badge[1]);
							   break;
							case 9:
								for (int ip = epv; ip <= vpv; ip++)
								   swprintf(line+wcslen(line), L"%c%s", erotin, kilp.Hajonta(ip, st));
							   break;
							case 10:
							   swprintf(line+wcslen(line), L"%c%d", erotin, kilp.piiri);
							   break;
							case 11:
							   swprintf(line+wcslen(line), L"%c%d", erotin, kilp.era);
							   break;
							case 12:
								for (int ip = epv; ip <= vpv; ip++)
								   swprintf(line+wcslen(line), L"%c%s", erotin, kilp.EtuNimi(st, OSNIMIL, ip));
							   break;
							case 13:
								for (int ip = epv; ip <= vpv; ip++)
								   swprintf(line+wcslen(line), L"%c%s", erotin, kilp.SukuNimi(st, OSNIMIL, ip));
							   break;
							case 14:
								for (int ip = epv; ip <= vpv; ip++)
								   swprintf(line+wcslen(line), L"%c%s", erotin, kilp.Nimi(st, OSNIMIL, ip, -1));
							   break;
							case 15:
								for (int ip = epv; ip <= vpv; ip++)
								   swprintf(line+wcslen(line), L"%c%s", erotin, kilp.Nimi(st, OSNIMIL, ip, 1));
							   break;
							case 16:
								for (int ip = epv; ip <= vpv; ip++)
									swprintf(line+wcslen(line), L"%c%c", erotin, kilp.wTark(ip, false));
								break;
							case 17:
								for (int ip = epv; ip <= vpv; ip++)
									swprintf(line+wcslen(line), L"%c%c", erotin, kilp.Laina(ip));
								break;
							case 18:
								for (int ip = epv; ip <= vpv; ip++) {
									swprintf(line+wcslen(line), L"%c\"%s\"", erotin,
										aikatowstr_cols_n(as, kilp.ostiet[ip].ylahto, t0, L',', kilpparam.laika));
									}
								break;
							case 19:
								for (int ip = epv; ip <= vpv; ip++)
									swprintf(line+wcslen(line), L"%c%d", erotin, kilp.ostiet[ip].lahtolaji);
							   break;
							case 20:
								for (int ip = epv; ip <= vpv; ip++)
									swprintf(line+wcslen(line), L"%c\"%s\"", erotin,
										aikatowstr_cols_n(as, kilp.Maali(ip, 0), t0, L',', kilpparam.laika));
								break;
							case 21:
							   swprintf(line+wcslen(line), L"%c%d", erotin, kilp.pisteet[0]);
								break;
							case 22:
							   swprintf(line+wcslen(line), L"%c%d", erotin, kilp.pisteet[1]);
								break;
							case 23:
							   swprintf(line+wcslen(line), L"%c%d", erotin, kilp.pisteet[2]);
								break;
							case 24:
								for (int ip = epv; ip <= vpv; ip++)
									swprintf(line+wcslen(line), L"%c%d", erotin, kilp.Ampsakot(ip, st));
								break;
							case 25:
								for (int ip = epv; ip <= vpv; ip++)
									swprintf(line+wcslen(line), L"%c%d", erotin, kilp.ostiet[ip].sakko);
								break;
							case 26:
								for (int ip = epv; ip <= vpv; ip++)
									swprintf(line+wcslen(line), L"%c%c", erotin, kilp.Seuranta(ip));
								break;
							case 27:
								for (int ip = 0; ip < Sarjat[kilp.sarja].osuusluku ; ip++)
								   swprintf(line+wcslen(line), L"%c%s", erotin, kilp.Hajonta(ip, st));
							   break;
							case 28:
							case 29:
							case 30:
								for (int ip = epv; ip <= vpv; ip++)
									swprintf(line+wcslen(line), L"%c%d", erotin, kilp.ostiet[ip].pisteet[Tieto-28]);
								break;
							}
						wcscat(line, L"\n");
						OutFile->WriteLine(line);
						nsiirr++;
						EdtTietolkm->Text = nsiirr;
						if (nsiirr%10 == 0) {
							EdtTietolkm->Refresh();
							Sleep(10);
							}
						}
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
void __fastcall TFormLisatiedot::KopioiTiedot(void)
{
	int Tieto, TunnPv, TietoPv, epv, vpv, Ehtolaji;
	int nrv = 0, nsiirr = 0, EhtoArvo = -1;
	kilptietue kilp;

	EdtTietolkm->Text = nsiirr;
	EdtTietolkm->Refresh();
	Sleep(10);
	Tieto = RGTieto->ItemIndex;
	TunnPv = CBTunnPv->ItemIndex - 1;
	TietoPv = CBTietoPv->ItemIndex - 1;
	Ehtolaji = RGEhto->ItemIndex;
	if (TunnPv < 0) {
		Application->MessageBoxW(L"Valitse kopioitavan tiedon osuus", L"Virhe", MB_OK);
		return;
		}

	switch (Ehtolaji) {
		case 1:
			EhtoArvo = haesarja_w(EdtEhto->Text.c_str(), true);
			break;
		}
	if (Ehtolaji && EhtoArvo < 0)
		return;
	if (TietoPv < 0) {
		epv = 0;
		vpv = kilpparam.osuusluku-1;
		}
	else {
		epv = TietoPv;
		vpv = epv;
		}
	if (Tieto < 0) {
		Application->MessageBoxW(L"Valitse kopioitava tieto", L"Virhe", MB_OK);
		return;
		}

	EnterCriticalSection(&tall_CriticalSection);
	UINT32 kirjheti0 = kirjheti;
	kirjheti = 0;
	for (int d = 1; d < datf2.numrec; d++) {
		kilp.getrec(d);
		if (!kilp.kilpstatus) {
			if (Ehtolaji == 1 && kilp.sarja != EhtoArvo)
				continue;
			switch (Tieto) {
				wchar_t st[200];
				case 6:
					for (int ip = epv; ip <= vpv; ip++)
						kilp.ostiet[ip].lisno = kilp.ostiet[TunnPv].lisno;
				   break;
				case 7:
					for (int ip = epv; ip <= vpv; ip++)
						kilp.ostiet[ip].badge[0] = kilp.ostiet[TunnPv].badge[0];
				   break;
				case 8:
					for (int ip = epv; ip <= vpv; ip++)
						kilp.ostiet[ip].badge[1] = kilp.ostiet[TunnPv].badge[1];
				   break;
				case 9:
					for (int ip = epv; ip <= vpv; ip++)
					   strcpy(kilp.ostiet[ip].hajonta, kilp.ostiet[TunnPv].hajonta);
				   break;
				case 14:
					for (int ip = epv; ip <= vpv; ip++)
					   strcpy(kilp.ostiet[ip].nimi, kilp.ostiet[TunnPv].nimi);
				   break;
				case 16:
					for (int ip = epv; ip <= vpv; ip++)
						kilp.ostiet[ip].keskhyl = kilp.ostiet[TunnPv].keskhyl;
					break;
				case 17:
					for (int ip = epv; ip <= vpv; ip++)
						kilp.ostiet[ip].laina[0] = kilp.ostiet[TunnPv].laina[0];
					break;
				case 18:
					for (int ip = epv; ip <= vpv; ip++) {
						kilp.ostiet[ip].ylahto = kilp.ostiet[TunnPv].ylahto;
						}
					break;
				case 19:
					for (int ip = epv; ip <= vpv; ip++)
						kilp.ostiet[ip].lahtolaji = kilp.ostiet[TunnPv].lahtolaji;
				   break;
				case 20:
					for (int ip = epv; ip <= vpv; ip++)
						kilp.ostiet[ip].va[0].aika = kilp.ostiet[TunnPv].va[0].aika;
					break;
				case 24:
					for (int ip = epv; ip <= vpv; ip++)
					   strcpy(kilp.ostiet[ip].ampsakot, kilp.ostiet[TunnPv].ampsakot);
					break;
				case 25:
					for (int ip = epv; ip <= vpv; ip++)
						kilp.ostiet[ip].sakko = kilp.ostiet[TunnPv].sakko;
					break;
				case 26:
					for (int ip = epv; ip <= vpv; ip++)
						kilp.ostiet[ip].seuranta = kilp.ostiet[TunnPv].seuranta;
					break;
				case 28:
				case 29:
				case 30:
					for (int ip = epv; ip <= vpv; ip++)
						kilp.ostiet[ip].pisteet[Tieto-28] = kilp.ostiet[TunnPv].pisteet[Tieto-28];
					break;
				case 12:
				case 13:
				case 15:
					Application->MessageBoxW(L"Siirr‰ nimi muodossa 'Sukunimi Etunimi'", L"Este", MB_OK);
					d = 9999999;
				   break;
				default :
					Application->MessageBoxW(L"Tieto ei ole osuuskohtainen", L"Este", MB_OK);
					d = 9999999;
				}
			if (d >= datf2.numrec)
				break;
			kilp.Tallenna(d, 0, 0, 0, 0);
			nsiirr++;
			EdtTietolkm->Text = nsiirr;
			if (nsiirr%10 == 0) {
				EdtTietolkm->Refresh();
				Sleep(10);
				}
			}
		}
	kirjheti = kirjheti0;
	LeaveCriticalSection(&tall_CriticalSection);
	Application->MessageBoxW(UnicodeString(L"Siirretty "+UnicodeString(nsiirr)+L" tietoa").c_str(),
		L"Valmis", MB_OK);
}

//---------------------------------------------------------------------------

void __fastcall TFormLisatiedot::RGTunnisteClick(TObject *Sender)
{
	GroupBox1->Visible = L"01111122122P2"[RGTunniste->ItemIndex+1] == L'2';
	GroupBox4->Visible = L"01111122122P2"[RGTunniste->ItemIndex+1] == L'P';
	if (RGTunniste->ItemIndex == KopioTunnus)
		GroupBox1->Caption = L" Kopioitavan tiedon osuus ";
	else
		GroupBox1->Caption = L" Tunnistavan tiedon osuus ";
	GroupBox3->Visible = !GroupBox4->Visible;
	Button3->Visible = !GroupBox4->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TFormLisatiedot::RGTietoClick(TObject *Sender)
{
	GroupBox2->Visible = L"01111112222112222222221112221222  "[RGTieto->ItemIndex+1] == L'2';
}
//---------------------------------------------------------------------------

