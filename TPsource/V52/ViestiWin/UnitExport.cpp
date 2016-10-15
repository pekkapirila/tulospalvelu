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

#include "UnitMain.h"
#include "UnitExport.h"
#include "UnitMySQL.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormExport *FormExport;

extern bool kirjXMLkaikki;
int kirjKilpDat(wchar_t *fname);

//---------------------------------------------------------------------------
__fastcall TFormExport::TFormExport(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormExport::RGLajiClick(TObject *Sender)
{
	switch (RGLaji->ItemIndex) {
		case 0:
			GBXML->Visible = true;
			GBXML->Enabled = true;
			GBcsv->Visible = false;
			Label2->Visible = false;
			GBSQL->Visible = false;
			LBSarjat->Enabled = true;
			LBSarjaVal->Enabled = true;
			break;
		case 1:
			GBXML->Visible = false;
			GBcsv->Visible = true;
			GBcsv->Enabled = true;
			Label2->Visible = false;
			GBSQL->Visible = false;
			LBSarjat->Enabled = true;
			LBSarjaVal->Enabled = true;
			break;
		case 3:

			GBXML->Visible = false;
			GBcsv->Visible = false;
			Label2->Visible = true;
			GBSQL->Visible = true;
			GBSQL->Enabled = true;
			LBSarjat->Enabled = true;
			LBSarjaVal->Enabled = true;
			break;
		case 2:
		case 4:
			GBXML->Visible = false;
			GBcsv->Visible = false;
			Label2->Visible = false;
			GBSQL->Visible = false;
			GBSQL->Enabled = false;
			LBSarjat->Enabled = false;
			LBSarjaVal->Enabled = false;
			break;
		}
}
//---------------------------------------------------------------------------
wchar_t *TFormExport::otsikkorivi(wchar_t *line, int len, wchar_t erotin)
{
	wchar_t st[100];

	wmemset(line, 0, len);
	for (int i = 0; i < Kexp_kentat; i++) {
		if (!exp_kentat_local[i].id || !exp_kentat_local[i].akt)
			continue;
		swprintf(line+wcslen(line), L"\"%s\"%c", exp_kentat_local[i].name, erotin);
		}

	for (int ipv = 0; ipv < kilpparam.n_os_akt; ipv++) {
		for (int i = Kexp_kentat; i < Kexp_kentat+Pexp_kentat; i++) {
			if (!exp_kentat_local[i].id || !exp_kentat_local[i].akt)
				continue;
			swprintf(line+wcslen(line), L"\"%s-%d\"%c", exp_kentat_local[i].name, ipv+1, erotin);
			}
		if (CBValiajat->Checked) {
			for (int iva = 0; iva < kilpparam.maxvaluku; iva++) {
				for (int i = Kexp_kentat+Pexp_kentat; i < Kexp_kentat+Pexp_kentat+Vexp_kentat; i++) {
					if (!exp_kentat_local[i].id || !exp_kentat_local[i].akt)
						continue;
					swprintf(line+wcslen(line), L"\"%s-%d-%d\"%c", exp_kentat_local[i].name, ipv+1, iva+1, erotin);
					}
				}
			}
		}
	line[wcslen(line)-1] = L'\n';
	return(line);
}
//---------------------------------------------------------------------------

//bool kilptietue::kirjoitaTiedostoon(void)
//{
//	return(FormExport->SarjaValinnat[Sarja()]);
//}
//---------------------------------------------------------------------------
wchar_t *TFormExport::datarivi(kilptietue *kilp, wchar_t *line, int len, wchar_t erotin)
{
	wchar_t st[100];
	char cst[20];

	wmemset(line, 0, len);
	for (int ipv = 0; ipv < kilpparam.n_os_akt; ipv++) {
		for (int i = 0; i < Kexp_kentat+Pexp_kentat; i++) {
			if ((int)wcslen(line) > len-40)
				break;
			if (!exp_kentat_local[i].id || !exp_kentat_local[i].akt)
				continue;
			if (ipv > 0 && i < Kexp_kentat)
				continue;
			int fldno = exp_kentat_local[i].id;
			switch (fldno) {
				case FLDID_KILPNO:
					swprintf(line+wcslen(line), L"%d%c", kilp->kilpno, erotin);
					break;
				case FLDID_JOUKKID:
					swprintf(line+wcslen(line), L"%d%c", kilp->joukkid, erotin);
					break;
				case FLDID_PIIRI:
					swprintf(line+wcslen(line), L"%d%c", kilp->piiri, erotin);
					break;
				case FLDID_SEURA:
					swprintf(line+wcslen(line), L"\"%s\"%c", kilp->seura[0] ? kilp->Seura(st) : L" ", erotin);
					break;
				case FLDID_JOUKKNO:
					swprintf(line+wcslen(line), L"%d%c", kilp->joukkue, erotin);
					break;
				case FLDID_JOUKKNIMI:
					MbsToWcs(st, kilp->joukknimi, 60);
					swprintf(line+wcslen(line), L"\"%s\"%c", (st[0] ? st : L" "), erotin);
					break;
				case FLDID_MAA:
					swprintf(line+wcslen(line), L"\"%s\"%c", kilp->maa[0] ? kilp->Maa(st) : L" ", erotin);
					break;
				case FLDID_SARJA:
					MbsToWcs(st, Sarjat[kilp->sarja].sarjanimi, 40);
					swprintf(line+wcslen(line), L"\"%s\"%c", st, erotin);
					break;
				case FLDID_ERA:
					swprintf(line+wcslen(line), L"%d%c", kilp->era, erotin);
					break;
				case FLDID_ILMLISTA:
					swprintf(line+wcslen(line), L"%d%c", kilp->ilmlista, erotin);
					break;
				case FLDID_PISTE1:
				case FLDID_PISTE2:
				case FLDID_PISTE3:
					swprintf(line+wcslen(line), L"%d%c", kilp->pisteet[fldno-FLDID_PISTE1], erotin);
					break;
				case FLDID_KILPID:
					swprintf(line+wcslen(line), L"%d%c", kilp->ostiet[ipv].lisno, erotin);
					break;
				case FLDID_SUKUNIMI:
					swprintf(line+wcslen(line), L"\"%s\"%c", wcslen(kilp->SukuNimi(st, 99, ipv)) > 0 ? st : L" ", erotin);
					break;
				case FLDID_ETUNIMI:
					swprintf(line+wcslen(line), L"\"%s\"%c", wcslen(kilp->EtuNimi(st, 99, ipv)) > 0 ? st : L" ", erotin);
					break;
				case FLDID_ARVO:
					MbsToWcs(st, kilp->ostiet[ipv].arvo, 99);
					swprintf(line+wcslen(line), L"\"%s\"%c", wcslen(st) > 0 ? st : L" ", erotin);
					break;
				case FLDID_OSSEURA:
					MbsToWcs(st, kilp->ostiet[ipv].osseura, 99);
					swprintf(line+wcslen(line), L"\"%s\"%c", wcslen(st) > 0 ? st : L" ", erotin);
					break;
				case FLDID_BADGE:
				case FLDID_BADGE2:
					swprintf(line+wcslen(line), L"%d%c", kilp->ostiet[ipv].badge[fldno-FLDID_BADGE], erotin);
					break;
				case FLDID_LAINA:
				case FLDID_LAINA2:
					swprintf(line+wcslen(line), L"%c%c", kilp->Laina(ipv, fldno-FLDID_LAINA), erotin);
					break;
				case FLDID_RATA:
					elimbl2(kilp->ostiet[ipv].hajonta);
					if (kilp->ostiet[ipv].hajonta[0])
						swprintf(line+wcslen(line), L"\"%s\"%c", kilp->Hajonta(ipv), erotin);
					else
						swprintf(line+wcslen(line), L"\"%s\"%c", L" ", erotin);
					break;
				case FLDID_TARK:
					swprintf(line+wcslen(line), L"%c%c", kilp->wTark(ipv, 0), erotin);
					break;
				case FLDID_AMPSAKKO:
					if (kilpparam.lsakmax > 0 && kilp->ostiet[ipv].ampsakot[0])
						swprintf(line+wcslen(line), L"\"%s\"%c", kilp->Ampsakot(ipv), erotin);
					else
						swprintf(line+wcslen(line), L"\"%s\"%c", L" ", erotin);
					break;
				case FLDID_SAKKO:
					swprintf(line+wcslen(line), L"%d%c", kilp->ostiet[ipv].sakko, erotin);
					break;
				case FLDID_OSPISTE1:
				case FLDID_OSPISTE2:
				case FLDID_OSPISTE3:
					swprintf(line+wcslen(line), L"%d%c", kilp->ostiet[ipv].pisteet[fldno-FLDID_OSPISTE1], erotin);
					break;
				case FLDID_SELITYS:
					memset(cst, 0, 4);
					strncpy(cst, kilp->ostiet[ipv].selitys, 3);
					if (cst[0]) {
						MbsToWcs(st, cst, 4);
						swprintf(line+wcslen(line), L"\"%s\"%c", st, erotin);
						}
					else
						swprintf(line+wcslen(line), L"\"%s\"%c", L" ", erotin);
					break;
				case FLDID_LAHTO:
					swprintf(line+wcslen(line), L"%s%c", aikatowstr_cols_n(st, kilp->ostiet[ipv].ylahto, t0, L',', kilpparam.laika), erotin);
					break;
				case FLDID_LAHTOLAJI:
					swprintf(line+wcslen(line), L"%d%c", kilp->ostiet[ipv].lahtolaji, erotin);
					break;
				case FLDID_MAALI:
					swprintf(line+wcslen(line), L"%s%c", aikatowstr_cols_n(st, kilp->Maali(ipv, 0), t0, L',', kilpparam.laika), erotin);
					break;
				case FLDID_SIJA:
					swprintf(line+wcslen(line), L"%d%c", kilp->Sija(ipv, 0), erotin);
					break;
				case FLDID_NIMI:
					swprintf(line+wcslen(line), L"\"%s\"%c", wcslen(kilp->Nimi(st, 99, ipv)) > 0 ? st : L" ", erotin);
					break;
				}
			}
		if (CBValiajat->Checked) {
			for (int iva = 0; iva < kilpparam.maxvaluku; iva++) {
				for (int i = Kexp_kentat+Pexp_kentat; i < Kexp_kentat+Pexp_kentat+Vexp_kentat; i++) {
					if (!exp_kentat_local[i].id || !exp_kentat_local[i].akt)
						continue;
					switch (exp_kentat_local[i].id) {
						case FLDID_VAAIKA:
							swprintf(line+wcslen(line), L"%s%c", aikatowstr_cols_n(st, kilp->Maali(ipv, iva+1), t0, L',', kilpparam.laika), erotin);
							break;
						case FLDID_VASIJA:
							swprintf(line+wcslen(line), L"%d%c", kilp->Sija(ipv, iva+1), erotin);
							break;
						}
					}
				}
			}
		}
	line[wcslen(line)-1] = L'\n';
	return(line);
}
//---------------------------------------------------------------------------
static kilpindex ind;
extern kilpindex *srjaakindex;
extern kilpindex *srjnroindex;
char *keysa(void *vkilp, char *key, int keylen, int flags);
char *keysrjnro(void *vkilp, char *key, int keylen, int flags);
void poistaindeksi(kilpindex *ind);
int haesarja_w(wchar_t *snimi, bool yhdfl);

static int Seuraava(int jarj, int ed)
{
	int d, ok = 1;
	static int kno;
	kilptietue kilp;
	char key[MAXINDL] = "";

	if (ed < 0 && jarj > 1) {
		switch (jarj) {
			case 2:
				ok = !searchix(&nameindex, key, &d);
				break;
			case 3:
				ind = *srjnroindex;
				ind.ix = 0;
				ok = !teeindeksi(&ind, true);
				ok = !searchix(&ind, key, &d);
				break;
			case 4:
				ind = *srjaakindex;
				ind.ix = 0;
				ok = !teeindeksi(&ind, true);
				ok = !searchix(&ind, key, &d);
				break;
			}
		if (!ok)
			d = -1;
		return(d);
		}

	switch(jarj) {
		case 0:
			if (ed < 0)
				ed = 0;
			for (d = ed+1; d < datf2.numrec; d++) {
				kilp.getrec(d);
				if (kilp.kilpstatus == 0)
					break;
				}
			if (d == datf2.numrec)
				d = -1;
			break;
		case 1:
			if (ed < 0)
				kno = minkilpno-1;
			for (kno = kno+1; kno <= maxkilpno; kno++) {
				if ((d = getpos(kno)) > 0)
					break;
				}
			if (kno > maxkilpno)
				d = -1;
			break;
		case 2:
			ok = !nextix(&nameindex, key, &d);
			if (!ok)
				d = -1;
			break;
		default:
			ok = !nextix(&ind, key, &d);
			if (!ok)
				d = -1;
			break;
		}
	 return(d);
}
//---------------------------------------------------------------------------
void TFormExport::kirjCsv(wchar_t *flname)
{
	wchar_t erotin, Erottimet[] = L";\t,";
	wchar_t merkisto, Merkistot[] = L"8AOW";
	wchar_t *line;
	int Jarj, d, nrv = 0, llen;
	TextFl *outfl;
	kilptietue kilp;


	int nfld = Kexp_kentat + Pexp_kentat + Vexp_kentat;
	exp_kentat_local = new FLD2[nfld];
	for (int i = 0; i < nfld; i++) {
		exp_kentat_local[i] = exp_kentat[i];
		if ((CBKaikkiKentat->Checked && i < Kexp_kentat + Pexp_kentat)
			|| (CBValiajat->Checked && i >= Kexp_kentat + Pexp_kentat))
			exp_kentat_local[i].len = 1;
		}
	erotin = Erottimet[CBErotin->ItemIndex];
	merkisto = Merkistot[CBMerkisto->ItemIndex];
	Jarj = CBJarj->ItemIndex;
	outfl = new TextFl(flname, L"wt", merkisto);
	llen = 200 + kilpparam.osuusluku * (220+32*kilpparam.maxvaluku);
	line = new wchar_t[llen+1];
	if (outfl->IsOpen()) {
		d = -1;
		otsikkorivi(line, llen, erotin);
		outfl->WriteLine(line);
		EnterCriticalSection(&tall_CriticalSection);
		while (!outfl->Feof()) {
			d = Seuraava(Jarj, d);
			if (d < 0)
				break;
			kilp.getrec(d);
			if (!SarjaValinnat[kilp.sarja])
				continue;
			datarivi(&kilp, line, llen, erotin);
			outfl->WriteLine(line);
			nrv++;
			if (nrv%10 == 0) {
				EdtSQLlkm->Text = nrv;
				EdtSQLlkm->Refresh();
				Sleep(1);
				}
			}
		LeaveCriticalSection(&tall_CriticalSection);
		}
	EdtSQLlkm->Text = nrv;
	delete[] line;
	delete outfl;
	if (ind.ix)
		poistaindeksi(&ind);
	Application->MessageBoxW((UnicodeString(L"Kirjoitettu ")+UnicodeString(nrv)+
		L" tietuetta").c_str(), L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------
void TFormExport::kirjSQL(void)
{
	int d, nrv = 0;
	kilptietue kilp;

	d = 1;
	EnterCriticalSection(&tall_CriticalSection);
	while (d < datf2.numrec && FormMySQL->SQLConnection1->Connected) {
		kilp.getrec(d);
		if (kilp.kilpstatus ||
			(CBOhitaEil->Checked &&	wcswcind(kilp.Tark(0), L"EP") >= 0) ||
			!SarjaValinnat[kilp.sarja]) {
			d++;
			continue;
			}
		if (sqlparam.sqltallennus) {
			kirjKilpSQL(d);
			nrv++;
			}
		else {
			if (FormMySQL->kirjSQLrivi(d))
				nrv++;
			EdtSQLlkm->Text = UnicodeString(nrv);
			EdtSQLlkm->Refresh();
			if ((nrv % 10) == 0) {
				Sleep(100);
				}
			}
		d++;
		}
	LeaveCriticalSection(&tall_CriticalSection);
	EdtSQLlkm->Text = UnicodeString(nrv);
	EdtSQLlkm->Refresh();
	Application->MessageBoxW((UnicodeString(L"Kirjoitettu ")+UnicodeString(nrv)+
		L" tietuetta").c_str(), L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TFormExport::Button1Click(TObject *Sender)
{
	kirjXMLkaikki = CBLaajennettu->Checked;
	if (RGLaji->ItemIndex == 0) {
		SaveDialog1->DefaultExt = L"xml";
		SaveDialog1->Filter = L"XML-tiedostot (*.xml)|*.XML|Kaikki tiedostot (*.*)|*.*";
		if (SaveDialog1->Execute()) {
			kirjEventXml(SaveDialog1->FileName.c_str() , CBXMLSarjat->Checked,
				CBXMLRadat->Checked, CBXMLKilpailijat->Checked);
			Application->MessageBoxW(L"Tiedosto kirjoitettu", L"Valmis", MB_OK);
			}
		}
	if (RGLaji->ItemIndex == 1) {
		SaveDialog1->DefaultExt = L"csv";
		SaveDialog1->Filter = L"CSV-tiedostot (*.csv)|*.csv|Kaikki tiedostot (*.*)|*.*";
		if (SaveDialog1->Execute()) {
			kirjCsv(SaveDialog1->FileName.c_str());
			}
		}
	if (RGLaji->ItemIndex == 2) {
		kirjsarjat();
		Application->MessageBoxW(L"Tiedosto kirjoitettu", L"Valmis", MB_OK);
		}
	if (RGLaji->ItemIndex == 3) {
		if (!FormMySQL)
			FormMySQL = new TFormMySQL(FormMain);
		if (!FormMySQL->SQLConnection1->Connected) {
			Application->MessageBoxW(L"Tiedonsiirto käynnistyy, kun SQL-määritysikkuna suljetaan", L"", MB_OK);
			FormMySQL->ShowModal();
			}
		if (FormMySQL->SQLConnection1->Connected) {
			kirjSQL();
			}
		}
	if (RGLaji->ItemIndex == 4) {

		SaveDialog1->DefaultExt = L"dat";
		SaveDialog1->Filter = L"DAT-tiedostot (*.dat)|*.DAT|Kaikki tiedostot (*.*)|*.*";
		if (SaveDialog1->Execute()) {
			kirjKilpDat(SaveDialog1->FileName.c_str());
			Application->MessageBoxW(L"Tiedosto kirjoitettu", L"Valmis", MB_OK);
			}
		}
	SetCurrentDir(FormMain->CurrentDir);
	kirjXMLkaikki = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormExport::Button2Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormExport::LBSarjatDblClick(TObject *Sender)
{
	int srj;

	srj = haesarja_w(LBSarjat->Items->Strings[LBSarjat->ItemIndex].c_str(), false);
	if (srj >= 0)
		SarjaValinnat[srj] = true;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------


void __fastcall TFormExport::LBSarjaValDblClick(TObject *Sender)
{
	int srj;

	srj = haesarja_w(LBSarjaVal->Items->Strings[LBSarjaVal->ItemIndex].c_str(), false);
	if (srj >= 0)
		SarjaValinnat[srj] = false;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormExport::BtnLisKaikkiClick(TObject *Sender)
{
	for (int srj = 0; srj < sarjaluku; srj++)
		SarjaValinnat[srj] = true;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormExport::BtnPoisKaikkiClick(TObject *Sender)
{
	for (int srj = 0; srj < sarjaluku; srj++)
		SarjaValinnat[srj] = false;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormExport::BtnLisValitutClick(TObject *Sender)
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

void __fastcall TFormExport::BtnPoisValitutClick(TObject *Sender)
{
	for (int rv = 0; rv < LBSarjaVal->Items->Count; rv++) {
		if (LBSarjaVal->Selected[rv]) {
			int srj;
			if ((srj = haesarja_w(LBSarjaVal->Items->Strings[rv].c_str(), false)) >= 0)
				SarjaValinnat[srj] = false;
			}
		}
	NaytaSarjavalinnat();
}//---------------------------------------------------------------------------

void __fastcall TFormExport::FormResize(TObject *Sender)
{
	LBSarjat->Height = ClientHeight - LBSarjat->Top - 8;
	LBSarjaVal->Height = ClientHeight - LBSarjaVal->Top - 8;
}
//---------------------------------------------------------------------------
void TFormExport::NaytaSarjavalinnat(void)
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

void __fastcall TFormExport::FormShow(TObject *Sender)
{
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------



