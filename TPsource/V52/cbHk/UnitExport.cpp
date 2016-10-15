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

#include "WinHk.h"
#include "UnitExport.h"
#include "UnitMySQL.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormExport *FormExport;

extern bool kirjXMLkaikki;

//---------------------------------------------------------------------------
__fastcall TFormExport::TFormExport(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormExport::RGLajiClick(TObject *Sender)
{
	Label2->Visible = false;
	RGdatRajaus->Visible = false;
	switch (RGLaji->ItemIndex) {
		case 0:
			GBXML->Visible = true;
			GBXML->Enabled = true;
			GBcsv->Visible = false;
			GBSQL->Visible = false;
			LBSarjat->Enabled = true;
			LBSarjaVal->Enabled = true;
			break;
		case 1:
			GBXML->Visible = false;
			GBcsv->Visible = true;
			GBcsv->Enabled = true;
			GBSQL->Visible = false;
			LBSarjat->Enabled = true;
			LBSarjaVal->Enabled = true;
			break;
		case 2:
			GBXML->Visible = false;
			GBcsv->Visible = false;
			GBSQL->Visible = false;
			GBSQL->Enabled = false;
			LBSarjat->Enabled = false;
			LBSarjaVal->Enabled = false;
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
		case 4:
			RGdatRajaus->Visible = true;
			GBXML->Visible = false;
			GBcsv->Visible = false;
			GBSQL->Visible = false;
			GBSQL->Enabled = false;
			LBSarjat->Enabled = true;
			LBSarjaVal->Enabled = true;
			break;
		}
}
//---------------------------------------------------------------------------
wchar_t *TFormExport::otsikkorivi(wchar_t *line, int len, wchar_t erotin)
{
	wmemset(line, 0, len);
	for (int i = 0; i < Kexp_kentat; i++) {
		if (exp_kentat_local[i].len) {
			wcscat(line, exp_kentat_local[i].name);
			line[wcslen(line)] = erotin;
			}
		}
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		for (int i = Kexp_kentat; i < Kexp_kentat+Pexp_kentat; i++) {
			if (exp_kentat_local[i].len) {
				swprintf(line+wcslen(line), L"%s-%d%c", exp_kentat_local[i].name, ipv+1, erotin);
				}
			}
		}
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		for (int iva = 0; iva < kilpparam.valuku; iva++) {
			for (int i = Kexp_kentat+Pexp_kentat; i < Kexp_kentat+Pexp_kentat+Vexp_kentat; i++) {
				if (exp_kentat_local[i].len) {
					swprintf(line+wcslen(line), L"%s-%d-%d%c", exp_kentat_local[i].name, ipv+1, iva+1, erotin);
					}
				}
			}
		}
	line[wcslen(line)-1] = L'\n';
	return(line);
}
//---------------------------------------------------------------------------

bool kilptietue::kirjoitaTiedostoon(void)
{
	return(FormExport->SarjaValinnat[Sarja()]);
}
//---------------------------------------------------------------------------
wchar_t *TFormExport::datarivi(kilptietue *kilp, wchar_t *line, int len, wchar_t erotin)
{
	wchar_t st[40];

	wmemset(line, 0, len);
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		for (int i = 0; i < Kexp_kentat+Pexp_kentat; i++) {
			if ((int)wcslen(line) > len-40)
				break;
			if (!exp_kentat_local[i].len)
				continue;
			if (ipv > 0 && i < Kexp_kentat)
				continue;
			switch (exp_kentat_local[i].oik) {
				case 1:
					swprintf(line+wcslen(line), L"%d%c", kilp->kilpno, erotin);
					break;
				case 2:
					swprintf(line+wcslen(line), L"%d%c", kilp->lisno[0], erotin);
					break;
				case 3:
					swprintf(line+wcslen(line), L"%d%c", kilp->lisno[1], erotin);
					break;
				case 4:
					swprintf(line+wcslen(line), L"%d%c", kilp->piiri, erotin);
					break;
				case 5:
					swprintf(line+wcslen(line), L"%s%c", kilp->sukunimi[0] ? kilp->sukunimi : L" ", erotin);
					break;
				case 6:
					swprintf(line+wcslen(line), L"%s%c", kilp->etunimi[0] ? kilp->etunimi : L" ", erotin);
					break;
				case 7:
					swprintf(line+wcslen(line), L"%s%c", kilp->arvo[0] ? kilp->arvo : L" ", erotin);
					break;
				case 8:
					swprintf(line+wcslen(line), L"%s%c", kilp->seura[0] ? kilp->seura : L" ", erotin);
					break;
				case 9:
					swprintf(line+wcslen(line), L"%s%c", kilp->seuralyh[0] ? kilp->seuralyh : L" ", erotin);
					break;
				case 10:
					swprintf(line+wcslen(line), L"%s%c", kilp->yhdistys[0] ? kilp->yhdistys : L" ", erotin);
					break;
				case 11:
					swprintf(line+wcslen(line), L"%s%c", kilp->joukkue[0] ? kilp->joukkue : L" ", erotin);
					break;
				case 12:
					swprintf(line+wcslen(line), L"%s%c", kilp->maa[0] ? kilp->maa : L" ", erotin);
					break;
				case 13:
					swprintf(line+wcslen(line), L"%s%c", Sarjat[kilp->sarja].sarjanimi, erotin);
					break;
				case 14:
					line[wcslen(line)] = kilp->sukup;
					line[wcslen(line)] = erotin;
					break;
				case 15:
					swprintf(line+wcslen(line), L"%d%c", kilp->synt, erotin);
					break;
				case 16:
					swprintf(line+wcslen(line), L"%d%c", kilp->ikasarja, erotin);
					break;
				case 17:
					swprintf(line+wcslen(line), L"%d%c", kilp->arvryhma, erotin);
					break;
				case 18:
					swprintf(line+wcslen(line), L"%d%c", kilp->ilmlista, erotin);
					break;
				case 19:
					swprintf(line+wcslen(line), L"%d%c", kilp->pisteet[0], erotin);
					break;
				case 20:
					swprintf(line+wcslen(line), L"%d%c", kilp->pisteet[1], erotin);
					break;
				case 21:
					swprintf(line+wcslen(line), L"%d%c", kilp->pisteet[2], erotin);
					break;
				case 22:
					swprintf(line+wcslen(line), L"%d%c", kilp->pv[ipv].bib, erotin);
					break;
				case 23:
					if (kilpparam.sarjavaihtelee && kilp->pv[ipv].sarja >= 0)
						swprintf(line+wcslen(line), L"%s%c", Sarjat[kilp->pv[ipv].sarja].sarjanimi, erotin);
					else
						swprintf(line+wcslen(line), L" %c", erotin);
					break;
				case 24:
					swprintf(line+wcslen(line), L"%d%c", kilp->pv[ipv].badge[0], erotin);
					break;
				case 25:
					swprintf(line+wcslen(line), L"%c%c", kilp->pv[ipv].laina[0] ? kilp->pv[ipv].laina[0] : L' ', erotin);
					break;
				case 26:
					swprintf(line+wcslen(line), L"%d%c", kilp->pv[ipv].badge[1], erotin);
					break;
				case 27:
					swprintf(line+wcslen(line), L"%c%c", kilp->pv[ipv].laina[1] ? kilp->pv[ipv].laina[1] : L' ', erotin);
					break;
				case 28:
					swprintf(line+wcslen(line), L"%s%c", aikatowstr_cols_n(st, kilp->pv[ipv].tlahto, t0, L',', 8), erotin);
					break;
				case 29:
					swprintf(line+wcslen(line), L"%s%c", aikatowstr_cols_n(st, kilp->pv[ipv].va[0].vatulos, t0, L',', pvparam[ipv].laika), erotin);
					break;
				case 30:
					swprintf(line+wcslen(line), L"%s%c", aikatowstr_cols_n(st, kilp->tulos_pv(ipv), 0, L',', pvparam[ipv].laika), erotin);
					break;
				case 31:
					swprintf(line+wcslen(line), L"%c%c", kilp->tark(ipv), erotin);
					break;
				case 32:
					swprintf(line+wcslen(line), L"%d%c", kilp->sija(ipv), erotin);
					break;
				case 33:
					if (kilpparam.lsakmax > 0)
						swprintf(line+wcslen(line), L"%s%c", kilp->pv[ipv].asakot[0] ? kilp->asakkoStr(ipv) : L" ", erotin);
					else
						swprintf(line+wcslen(line), L" %c", erotin);
					break;
				case 34:
					swprintf(line+wcslen(line), L"%d%c", kilp->pv[ipv].tasoitus, erotin);
					break;
				case 35:
					elimwbl2(kilp->pv[ipv].rata);
					swprintf(line+wcslen(line), L"%s%c", kilp->pv[ipv].rata[0] ? kilp->pv[ipv].rata : L" ", erotin);
					break;
				case 36:
					swprintf(line+wcslen(line), L"%d%c", kilp->pv[ipv].era, erotin);
					break;
				case 37:
					elimwbl2(kilp->pv[ipv].selitys);
					swprintf(line+wcslen(line), L"%s%c", kilp->pv[ipv].selitys[0] ? kilp->pv[ipv].selitys : L" ", erotin);
					break;
				case 38:
					swprintf(line+wcslen(line), L"%d%c", kilp->pv[ipv].pvpisteet[0], erotin);
					break;
				case 39:
					swprintf(line+wcslen(line), L"%d%c", kilp->pv[ipv].pvpisteet[1], erotin);
					break;
				case 40:
					swprintf(line+wcslen(line), L"%s%c", aikatowstr_cols_n(st, kilp->pv[ipv].enn, 0, L',', pvparam[ipv].laika2), erotin);
					break;
				case 41:
					swprintf(line+wcslen(line), L"%s%c", aikatowstr_cols_n(st, kilp->pv[ipv].tav, 0, L',', pvparam[ipv].laika2), erotin);
					break;
				case 42:
					swprintf(line+wcslen(line), L"%8x/%8x%c", kilp->pv[ipv].flags[0], kilp->pv[ipv].flags[1], erotin);
					break;
				}
			}
		}
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		for (int iva = 0; iva < kilpparam.valuku; iva++) {
			for (int i = Kexp_kentat+Pexp_kentat; i < Kexp_kentat+Pexp_kentat+Vexp_kentat; i++) {
				if ((int)wcslen(line) > len-20)
					break;
				if (!exp_kentat_local[i].len)
					continue;
				switch (exp_kentat_local[i].oik) {
					case FLDID_VATULOS:
						swprintf(line+wcslen(line), L"%s%c", aikatowstr_cols_n(st, kilp->pv[ipv].va[iva+2].vatulos, 0, L',', pvparam[ipv].laika), erotin);
						break;
					case FLDID_VASIJA:
						swprintf(line+wcslen(line), L"%d%c", kilp->pv[ipv].va[iva+2].vasija, erotin);
						break;
					}
				}
			}
		}
	line[wcslen(line)-1] = L'\n';
	return(line);
}
//---------------------------------------------------------------------------
static kilpindex ind;

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
				if (ok)
					ok = !searchix(&ind, key, &d);
				break;
			case 4:
				ind = *srjaakindex;
				ind.ix = 0;
				ok = !teeindeksi(&ind, true);
				if (ok)
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
			for (d = ed+1; d < nrec; d++) {
				kilp.GETREC(d);
				if (kilp.kilpstatus == 0)
					break;
				}
			if (d == nrec)
				d = -1;
			break;
		case 1:
			if (ed < 0)
				kno = minkilpno-1;
			d = getpos(kno+1, true);
			if (d > 0) {
				kilp.GETREC(d);
				kno = kilp.id();
				}
/*
			for (kno = kno+1; kno <= maxkilpno; kno++) {
				if ((d = getpos(kno)) > 0)
					break;
				}
			if (kno > maxkilpno)
				d = -1;
*/
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
	wchar_t line[10000];
	int Jarj, d, nrv = 0;
	TextFl *outfl;
	kilptietue kilp;

	int nfld = Kexp_kentat + Pexp_kentat + Vexp_kentat + Xexp_kentat;
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
	if (outfl->IsOpen()) {
		d = -1;
		otsikkorivi(line, sizeof(line)/2, erotin);
		outfl->WriteLine(line);
		EnterCriticalSection(&tall_CriticalSection);
		while (!outfl->Feof()) {
			d = Seuraava(Jarj, d);
			if (d < 0)
				break;
			kilp.GETREC(d);
			if (!SarjaValinnat[kilp.Sarja()])
				continue;
			datarivi(&kilp, line, sizeof(line)/2, erotin);
			outfl->WriteLine(line);
			nrv++;
			}
		LeaveCriticalSection(&tall_CriticalSection);
		}
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
	while (d < nrec && FormMySQL->SQLConnection1->Connected) {
		kilp.GETREC(d);
		if (kilp.kilpstatus ||
			(CBOhitaEil->Checked &&	!kilp.lasna(k_pv)) ||
			!SarjaValinnat[kilp.Sarja()]) {
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
int TFormExport::kirjKilpDat(wchar_t *fname, int rajaus)
{
	datafile datf_tmp;
	char cst[202];
	kilptietue kilp;
	UINT d;
	char buf[KILPBUFSIZE];

	makefile(&datf_tmp,wcstooem(cst, fname, 200),kilpparam.kilprecsize);
	for (int i = 1; i < nrec; i++) {
		kilp.GETREC(i);
		if (rajaus > 0 && kilp.kilpstatus)
			continue;
		if (rajaus > 1 && !SarjaValinnat[kilp.Sarja()])
			continue;
		if (rajaus > 2 && kilp.tark() == L'P')
			continue;
		if (rajaus > 3 && kilp.tark() == L'V')
			continue;
		kilp.pack(buf);
		if (i < nrec-1)
			addrec0(&datf_tmp, &d,buf);
		else
			addrec(&datf_tmp, &d,buf);
		}
	closefile(&datf_tmp);
	return(0);
}

//---------------------------------------------------------------------------
void __fastcall TFormExport::Button1Click(TObject *Sender)
{
	if (RGLaji->ItemIndex < 0) {
		Application->MessageBoxW(L"Valitse tiedoston tyyppi", L"Valitse", MB_OK);
		return;
		}
	kirjXMLkaikki = CBLaajennettu->Checked;
	arv_pv = 0;
	SaveDialog1->InitialDir = FormMain->CurrentDir;
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
			kirjKilpDat(SaveDialog1->FileName.c_str(), RGdatRajaus->ItemIndex);
			Application->MessageBoxW(L"Tiedosto kirjoitettu", L"Valmis", MB_OK);
			}
		}
	SetCurrentDir(FormMain->CurrentDir);
	kirjXMLkaikki = false;
	arv_pv = k_pv;
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

