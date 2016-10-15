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
#include <process.h>
#include <bstrings.h>

#include "UnitEmit.h"
#include "UnitEmitMuutokset.h"
#include "UnitKirjoitinVal.h"
#include "UnitMTR.h"
#include "UnitEmiTag.h"
#include "UnitPartio.h"
#include "UnitKartta.h"
#include "UnitPiikit.h"
#include "UnitEmitFile.h"
#include "UnitEmit_VA.h"
#include "UnitNaytaTulos.h"
#include "UnitHylkRap.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEmit *FormEmit;

int onrata(wchar_t *tunnus);
int onrata(char *tunnus);
INT haelukija(emittp *em);
INT haebdg(INT32 badge, int *toinen);
static int seurVapaaBadge;
int em_HaeSeuraava(int eptr);
int em_HaeEdellinen(int eptr);
int em_HaeViimeinen(void);
int em_HaeEnsimmainen(void);
void haeEmiterot(void);
INT32 e_maaliaika(emittp *em, kilptietue *kilp, INT os);
void tulostaRastiVa(tulostusparamtp *tulprm, int kilpno, int osuus);
void addemitanal(emittp *em, int uusi);

//---------------------------------------------------------------------------
__fastcall TFormEmit::TFormEmit(TComponent* Owner)
   : TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
	Kilp.nollaa();
	Tietue = 0;
	Em.nollaa();
	InitTila();
#ifndef _DEBUG
	if (DeveloperMode)
#endif
		HylkRap->Visible = true;
}
//---------------------------------------------------------------------------
__fastcall TFormEmit::~TFormEmit(void)
{
	ProsEmitOn = false;
	Sleep(600);
}
//---------------------------------------------------------------------------

emitIkkParamClass::emitIkkParamClass(void)
{
	memset(this, 0, sizeof(emitIkkParamClass));
}
//---------------------------------------------------------------------------
void emitIkkParamClass::operator=(emitIkkParamClass& ikkPar)
{
	memcpy(this, &ikkPar, sizeof(emitIkkParamClass));
}
//---------------------------------------------------------------------------
void TFormEmit::haeParams(void)
{
	IkkParam.NaytaKortinKoodi = Kortinkoodi1->Checked;
	if (Maalintietokone1->Checked)
		IkkParam.KoneTeht = 1;
	else if (Lhdntietokone1->Checked)
		IkkParam.KoneTeht = 2;
	else
		IkkParam.KoneTeht = 0;
	IkkParam.MerkLainat = Merkitsekortitlainakorteiksi1->Checked;
	IkkParam.VaadiToisto = Vaadikaksoisluenta1->Checked;
	IkkParam.LueVialliset = Luemysviallisetkortit1->Checked;
}
//---------------------------------------------------------------------------

int TFormEmit::applyParams(void)
{
	Kortinkoodi1->Checked = IkkParam.NaytaKortinKoodi;
	Maalintietokone1->Checked = IkkParam.KoneTeht == 1;
	kuntoMaali = IkkParam.KoneTeht == 1;
	Lhdntietokone1->Checked = IkkParam.KoneTeht == 2;
	kuntoLahto = IkkParam.KoneTeht == 2;
	Merkitsekortitlainakorteiksi1->Checked = IkkParam.MerkLainat;
	Vaadikaksoisluenta1->Checked = IkkParam.VaadiToisto;
	Luemysviallisetkortit1->Checked = IkkParam.LueVialliset;
	return(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::InitTila(void)
{
	static bool init = true;
	ratatp *rt;

	if (init) {
		init = false;
		Em.nollaa();
		Em_0.nollaa();
		Osuus = -1;
		Osuus_0 = -1;
		DKilp = 0;
		DKilp_0 = 0;
		Kilp.nollaa();
		Kilp_0.nollaa();
		EmitGrid->DefaultRowHeight = 18 * Screen->PixelsPerInch / 96;
		EmitGrid->ColWidths[0] = 28 * Screen->PixelsPerInch / 96;
		EmitGrid->ColWidths[1] = 42 * Screen->PixelsPerInch / 96;
		EmitGrid->ColWidths[2] = 52 * Screen->PixelsPerInch / 96;
		EmitGrid->ColWidths[3] = 70 * Screen->PixelsPerInch / 96;
		EmitGrid->ColWidths[4] = 70 * Screen->PixelsPerInch / 96;
		EmitGrid->ColWidths[5] = 70 * Screen->PixelsPerInch / 96;
		EmitGrid->ColWidths[6] = 70 * Screen->PixelsPerInch / 96;
		Cells[0][0].text = L"No";
		Cells[1][0].text = L"Koodi";
		Cells[2][0].text = L"Rasti";
		Cells[3][0].text = L"Kortin aika";
		Cells[4][0].text = L"Korj. (s)";
		Cells[5][0].text = L"Korj.";
		Cells[6][0].text = L"Kello";
		EdtBadge->Color = clVaaleanHarmaa;
		EdtKilpno->Color = clVaaleanHarmaa;
		EdtKilpno->Font->Color = clBlack;
		for (int i = 0; i < NREGNLY; i++) {
			if (regnly[i] == 13)
				MTRlaitteenohjaus1->Visible = true;
			}
#ifdef DBGFILE
		if (dbgtofile) {
			Avaadebugtiedosto1->Visible = true;
			Suljedebugtiedosto1->Visible = true;
			}
#endif

		Vaadikaksoisluenta1->Checked = lukutoisto;
		Label10->Visible = true;
		CRata->Visible = true;
		EdtOrigBadge->Visible = Kortinkoodi1->Checked;
		LblOrigBadge->Visible = EdtOrigBadge->Visible;
		Maalintietokone1->Visible = esitark == 0;
		Lhdntietokone1->Visible = esitark == 1;
		CSarja->Items->Clear();
		for (int i = 0; i < sarjaluku; i++) {
			CSarja->Items->Add(UnicodeString(Sarjat[i].sarjanimi));
			}
		CSarja->ItemIndex = 0;
		CBOsuus->Items->Clear();
		CBHakuOsuus->Items->Clear();
		CBOsuus->Items->Add(L"Ei valittu");
		if (kilpparam.maxnosuus == 1 || sarjaluku > 1) {
			for (int i = 1; i <= kilpparam.osuusluku; i++) {
				CBOsuus->Items->Add(UnicodeString(i) + L". osuus");
				CBHakuOsuus->Items->Add(UnicodeString(i) + L". osuus");
				}
			}
		else {
			for (int i = 0; i < kilpparam.osuusluku; i++) {
				CBOsuus->Items->Add(UnicodeString(L"Os. ")+wosuuskoodi(0, i, 0, 0));
				CBHakuOsuus->Items->Add(UnicodeString(L"Os. ")+wosuuskoodi(0, i, 0, 0));
				}
			}
		CBOsuus->ItemIndex = 0;
		CBHakuOsuus->ItemIndex = 0;

		CRata->Items->Clear();
		CRata->Items->Add(L"Ei rataa");
		for (rt = rata ; rt < rata+nrata; rt++)
			CRata->Items->Add(rt->rata);
		CRata->ItemIndex = 0;
		CBLue->Checked = emitluenta;
		if (!emitluenta) {
			CBLue->Visible = false;
			}
		Panel1->Top = ClientHeight - Panel1->Height - 4;
		}
	if (esitark == 1) {
		Width = 265 * Screen->PixelsPerInch / 96;
		EdtKilpno->Enabled = true;
		EdtKilpno->ReadOnly = false;
		EdtKilpno->Color = clWindow;
		EdtKilpno->TabStop = true;
		CBOsuus->Enabled = true;
		CBOsuus->Color = clWindow;
		CBOsuus->TabStop = true;
		LblOrigBadge->Visible = false;
		EdtOrigBadge->Visible = false;
		LblToimi->Visible = true;
		BtnMuutokset->Visible = false;
		BtnLasketulos->Visible = false;
		Label10->Visible = false;
		CRata->Visible = false;
		EdtLahto->Top = TarkVal->Top;
		Label5->Top = Label6->Top;
		Label6->Top = Label10->Top;
		TarkVal->Top = CRata->Top;
		Label3->Visible = false;
		EdtTulos->Visible = false;
		Label8->Visible = false;
		EdtSelitys->Visible = false;
		Label4->Visible = false;
		EdtMaali->Visible = false;
		Label7->Visible = false;
		Label12->Visible = false;
		EdtLuettu->Visible = false;
		EdtEro->Visible = false;
		Label14->Visible = false;
		EdtSakkoja->Visible = false;
		CBHaeSeurLaskettava->Visible = false;
		Panel1->Visible = false;
		if (esitark == -2) {
			Caption = L"Väliaikatulosteet";
			EdtBdgMsg->Text = L"Lue kortti tai syötä KilpNo";
			Height = 500 * Screen->PixelsPerInch / 96;
			}
		else if (esitark) {
			Merkitsekortitlainakorteiksi1->Visible = true;
			Height = 620 * Screen->PixelsPerInch / 96;
			}
		else {
			Height = 447 * Screen->PixelsPerInch / 96;
			BtnHaeOo->Top = ClientHeight - BtnHaeOo->Height -
				6 * Screen->PixelsPerInch /96;
			}
		}
	else {
		Width = 710 * Screen->PixelsPerInch / 96;
		EdtKilpno->Enabled = true;
		EdtKilpno->ReadOnly = true;
		EdtKilpno->Color = clVaaleanHarmaa;
		EdtKilpno->TabStop = false;
		CBOsuus->Enabled = false;
		CBOsuus->Color = clVaaleanHarmaa;
		CBOsuus->TabStop = false;
		LblToimi->Visible = false;
		BtnMuutokset->Visible = true;
		BtnLasketulos->Visible = true;
		EdtLahto->Top = EdtTulos->Top + EdtTulos->Height - 1;
		Label5->Top = EdtLahto->Top + 4 * Screen->PixelsPerInch / 96;
		TarkVal->Top = CRata->Top + CRata->Height - 1;
		Label6->Top = TarkVal->Top + 4 * Screen->PixelsPerInch / 96;
		Label6->Visible = true;
		TarkVal->Visible = true;
		Label3->Visible = true;
		EdtTulos->Visible = true;
		Label8->Visible = true;
		EdtSelitys->Visible = true;
		Label4->Visible = true;
		EdtMaali->Visible = true;
		Label7->Visible = true;
		Label12->Visible = true;
		EdtLuettu->Visible = true;
		EdtEro->Visible = true;
		Label14->Visible = true;
		EdtSakkoja->Visible = true;
		CBHaeSeurLaskettava->Visible = true;
		EdtOrigBadge->Visible = Kortinkoodi1->Checked;
		LblOrigBadge->Visible = EdtOrigBadge->Visible;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::FormActivate(TObject *Sender)
{
/*
	if (Tietue <= 0) {
		HaeTietue(emithead2-1, 0);
		if (DKilp) {
			Kilp.GETREC(DKilp);
			Nayta();
			TallVals();
		}
	}
*/
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::TallVals(void)
{
	ValOsuus = CBOsuus->ItemIndex;
	ValSNimi = EdtSNimi->Text;
	ValSeura = EdtSeura->Text;
	ValTulos = EdtTulos->Text;
	ValMaali = EdtMaali->Text;
	ValLahto = EdtLahto->Text;
	ValSakkoja = EdtSakkoja->Text;
	ValSelitys = EdtSelitys->Text;
	ValTarkVal = TarkVal->ItemIndex;
	ValRata = CRata->ItemIndex;
	ValSarja = CSarja->ItemIndex;
	BtnTallenna->Visible = false;
	BtnPeruuta->Visible = false;
	Tietue_0 = Tietue;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::PeruutaMuutokset(void)
{
	if (DKilp > 0)
		Kilp.getrec(DKilp);
	EdtSNimi->Text = ValSNimi;
	CBOsuus->ItemIndex = ValOsuus;
	EdtSeura->Text = ValSeura;
	EdtTulos->Text = ValTulos;
	EdtMaali->Text = ValMaali;
	EdtLahto->Text = ValLahto;
	EdtSakkoja->Text = ValSakkoja;
	EdtSelitys->Text = ValSelitys;
	TarkVal->ItemIndex = ValTarkVal;
	CRata->ItemIndex = ValRata;
	CSarja->ItemIndex = ValSarja;
	Tietue = Tietue_0;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::NaytaKilp(void)
{
	wchar_t st[60] = L"";
	int rr;

	EdtKilpno->Text = UnicodeString(Kilp.kilpno);
	EdtBadge->Clear();
	EdtOrigBadge->Clear();
	CSarja->ItemIndex = Kilp.sarja;
	CBOsuus->ItemIndex = Osuus+1;
	if (Kilp.kilpno < minkilpno) {
		EdtSeura->Text = L"";
		}
	else
		EdtSeura->Text = UnicodeString(Kilp.Joukkue(st, LSEURA));
	if (Kilp.kilpno < minkilpno || Osuus < 0) {
		EdtLahto->Text = L"";
		EdtMaali->Text = L"";
		EdtTulos->Text = L"";
		EdtSNimi->Text = L"";
		EdtSelitys->Text = L"";
		TarkVal->ItemIndex = 0;
		LblSulj->Visible = false;
		}
	else {
		if (Em.badge) {
			Kilp.ostiet[Osuus].badge[0] = Em.badge;
			if (!kilpparam.kaksibadge && Kilp.ostiet[Osuus].badge[1] == 0)
				Kilp.ostiet[Osuus].badge[1] = Kilp.ostiet[Osuus].badge[0];
			}
		EdtBadge->Text = Kilp.ostiet[Osuus].badge[0];
//		LblLaina->Visible = lainakortti(&Kilp);
		EdtSNimi->Text = UnicodeString(Kilp.Nimi(st, OSNIMIL, Osuus));
		rr = onrata(Kilp.ostiet[Osuus].hajonta);
		CRata->ItemIndex = rr < 0 ? 0 : rr;
		EdtSelitys->Text = UnicodeString(Kilp.Selitys(Osuus));
		if (kilpparam.partio)
			TarkVal->ItemIndex = wcswcind(Kilp.wTark(Sarjat[Kilp.sarja].osuusluku-1, false), L"-TIKHEVP");
		else
			TarkVal->ItemIndex = wcswcind(Kilp.wTark(Osuus, false), L"-TIKHEVP");
		LblSulj->Visible = Kilp.wTark(Osuus, true) == L'S';
		EdtSakkoja->Enabled = (Sarjat[Kilp.sarja].tsak[Osuus] != 0);
		if (Sarjat[Kilp.sarja].tsak[Osuus])
			EdtSakkoja->Text = UnicodeString(Kilp.ostiet[Osuus].sakko/(SEK*Sarjat[Kilp.sarja].tsak[Osuus]));
		AIKATOWSTRS(st, Kilp.Tulos(Osuus, 0), 0);
		st[kilpparam.laika2] = 0;
		EdtTulos->Text = st;
		if (Kilp.Lahto(Osuus) != TMAALI0) {
			AIKATOWSTRS(st, Kilp.Lahto(Osuus), t0);
			st[8] = 0;
			EdtLahto->Text = st;
			}
		else
			EdtLahto->Text = L"";
		if (Kilp.Maali(Osuus, 0) != TMAALI0) {
			AIKATOWSTRS(st, Kilp.Maali(Osuus, 0), t0);
			st[kilpparam.laika2] = 0;
			EdtMaali->Text = st;
			}
		else
			EdtMaali->Text = L"";
		if (Kilp.Seuranta(Osuus) == L'G') {
			EdtMessage->Text = L"Kilpailija GPS seurannassa";
			EdtMessage->Color = clYellow;
			}
		else if (!wcscmp(EdtMessage->Text.c_str(), L"Kilpailija GPS seurannassa")) {
			NollaaMessage();
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::NaytaEmit(void)
{
	wchar_t st[60];
	int tm = 0, nc = 0, lk, n_lk = 0, enn, virhe = 0, tulkinta[51], style = 0;
	UnicodeString AS = L"";
	bool on99 = false;
	ratatp *rt;
	static int LeimSammCnt;

	if (esitark)
		return;
	if (Em.badge > 0)
		EdtBadge->Text = Em.badge;
	EdtOrigBadge->Text = Em.badge0;
	TViimRasti = TMAALI0;
	EmitGrid->Color = clWindow;
	EmitGrid->RowCount = 51;
	if (Em.time != TMAALI0) {
		aikatowstr_ls(st, Em.time, t0);
		st[kilpparam.laika2] = 0;
		EdtLuettu->Text = st;
		}
	else
		EdtLuettu->Clear();
	lk = haelukija(&Em);
	memset(tulkinta, 0, sizeof(tulkinta));
	if (Kilp.kilpno > 0 && Osuus >= 0 && Kilp.kilpno == Em.kilpno && Kilp.ostiet[Osuus].badge[0] == Em.badge) {
		memset(tulkinta, 0, sizeof(tulkinta));
		virhe = tarkista(&Em, &Kilp, tulkinta, lk, 0);
		enn = haeennakko(&Em, &Kilp, Osuus);
		}
	if (Em.badge || eiLiitetty) {
		if (lk >= 0 && lk < 49) {
			tm = Em.ctrltime[lk];
			n_lk = tulkinta[lk];
			}
		for (nc = 49; nc > 1; nc--)
			if (Em.ctrlcode[nc] != 0 || Em.ctrltime[nc] != 0)
				break;
		nc++;
	//		EmitGrid->RowCount = nc+1;
	//		if (virhe)
	//			EmitGrid->Color = clYellow;
		if (Rastikoodit1->Checked && Kilp.kilpno) {
			rt = haerata(&Kilp, Osuus);
			}
		for (int i = 0; i < nc; i++) {
			style = virhe ? 2 : 0;
			Cells[0][i+1].text = i;
			Cells[1][i+1].text = UnicodeString((short)Em.ctrlcode[i]);
			if (Em.ctrlcode[i] == 99) {
				on99 = true;
				style = 2 - virhe;
				}
			else if (tulkinta[i] < 0) {
				AS = UnicodeString(L"(")+UnicodeString(-tulkinta[i])+UnicodeString(L")");
				style = virhe ? 4 : 1;
				}
			else {
				AS = UnicodeString(tulkinta[i]);
				if (tulkinta[i] == 0)
					style = virhe ? 5 : 1;
				else
					style = virhe ? 3 : 0;
				}
			if (Rastikoodit1->Checked && rt && tulkinta[i] != 0)
				AS = AS+L"("+ rt->rastikoodi[abs(tulkinta[i])-1] +L")";
			Cells[2][i+1].text = AS;
			Cells[3][i+1].text = Em.ctrltime[i];
			Cells[4][i+1].text = Em.ctrltime[i]-enn;
			aikatowstr_ls(st, 10*(Em.ctrltime[i]-enn), 0);
			st[kilpparam.laika2] = 0;
			Cells[5][i+1].text = st;
			aikatowstr_ls(st, Em.time+10*(Em.ctrltime[i]-tm), t0);
			st[kilpparam.laika2] = 0;
			Cells[6][i+1].text = st;
			if (tulkinta[i] == (n_lk - 1))
				TViimRasti = KSEK*(Em.time+10*(Em.ctrltime[i]-tm));
			for (int col = 0; col < 7; col++) {
				Cells[col][i+1].style = style;
				}
			}
		if (Kilp.kilpno > 0 && Osuus >= 0 && Kilp.kilpno == Em.kilpno && Kilp.ostiet[Osuus].badge[0] == Em.badge) {
			if (TViimRasti != TMAALI0 && Kilp.Maali(Osuus, 0) != TMAALI0) {
				EdtEro->Text = UnicodeString((Kilp.Maali(Osuus, 0)-TViimRasti)/SEK);
				}
			}
		}
	else {
		EdtLuettu->Clear();
		EdtEro->Clear();
		}
	for (int i = nc+1; i < 51; i++) {
		Cells[0][i].text = 0;
		Cells[1][i].text = 0;
		Cells[2][i].text = L"";
		Cells[3][i].text = L"";
		Cells[4][i].text = L"";
		Cells[5][i].text = L"";
		Cells[6][i].text = L"";
		for (int col = 0; col < 7; col++) {
			Cells[col][i].style = virhe ? 2 : 0;
			}
		}
	if (on99 && EdtMessage->Text.Length() == 0) {
		LeimSammCnt = 3;
		EdtMessage->Text = L"Leimasin sammumassa (koodi 99)";
		EdtMessage->Color = clYellow;
		}
	if (!on99 && LeimSammCnt) {
		if (!Sail99varoitus->Checked && wcscmp(EdtMessage->Text.c_str(), L"Leimasin sammumassa (koodi 99)") == 0) {
			LeimSammCnt--;
			if (LeimSammCnt <= 0) {
				EdtMessage->Text = L"";
				EdtMessage->Color = clBtnFace;
				}
			}
		else
			LeimSammCnt = 0;
		}
	EmitGrid->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::NaytaPartio(void)
{
	if (!FormPartio)
		FormPartio = new TFormPartio(this);
//	FormPartio->Kilp = Kilp;
	FormPartio->Show();
	if (FormPartio->WindowState == wsMinimized)
		FormPartio->WindowState = wsNormal;
	FormPartio->BringToFront();
	FormPartio->Partio(&Kilp, DKilp);
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::Nayta(void)
{
	NaytaKilp();
	NaytaEmit();
	if (kilpparam.partio)
		NaytaPartio();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::Paivita(emittp *pEm)
{
	int d, n = 0, tl = 0;
	wchar_t as[20];
	static int edBadge;
	bool VakTietue = false;

	Em = *pEm;
	if (EmitJarr(Em.kilpno, Em.osuus) == Tietue)
		eiLiitetty = false;
	if (!eiLiitetty && Em.kilpno && (d = getpos(Em.kilpno)) > 0) {
		Kilp.getrec(d);
		DKilp = d;
		Osuus = Em.osuus;
		}
	else {
		Kilp.nollaa();
		DKilp = 0;
		Osuus = -1;
		}
//	if (kuntosuunnmoodi != 3 && kuntosuunnmoodi != 4 && Kilp.ostiet[Osuus].uusi != L'U') {
	Nayta();
	if (UusiTietue && FormNaytaTulos && FormNaytaTulos->Visible) {
		wchar_t st[60];
		int tls = Kilp.Tulos(Em.osuus, 0);
		Kilp.Nimi(st, 40, Em.osuus, false);
		FormNaytaTulos->NaytaTulos(st, Kilp.wTark(Em.osuus, false),
			tls, Kilp.Sija(Em.osuus, 0), tls - pkarki[Kilp.sarja][Em.osuus][0]);
		}
	UusiTietue = false;
	TallVals();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::SBTietueScroll(TObject *Sender, TScrollCode ScrollCode,
		  int &ScrollPos)
{
	int suunta, pos;
	static int edsuunta;

	if (emithead2 > 1)
		SBTietue->Max = emithead2-1;
	else
		SBTietue->Max = 1;
	if (!CBLue->Checked)
		CBViim->Checked = false;
	switch (ScrollCode) {
		case scLineUp:
			SBTietue->Position = Tietue;
			edsuunta = -1;
			return;
		case scLineDown:
			SBTietue->Position = Tietue;
			edsuunta = 1;
			return;
		case scEndScroll:
			suunta = 0;
			break;
		default:
			return;
		}
	if (edsuunta)
		pos = _wtoi(EdtTietue->Text.c_str()) - 1 + edsuunta;
	else
		pos = SBTietue->Position;
		NollaaMessage();
	HaeTietue(pos, edsuunta);
	edsuunta = 0;
	EdtTietue->Text = UnicodeString(Tietue+1);
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::SBTietueEnter(TObject *Sender)
{
	if (emithead2 > 1)
		SBTietue->Max = emithead2-1;
	else
		SBTietue->Max = 1;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::SBTietueChange(TObject *Sender)
{
//   HaeTietue(SBTietue->Position, 0);
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::FormResize(TObject *Sender)
{
   EmitGrid->Height = ClientHeight - EmitGrid->Top - 29 * Screen->PixelsPerInch / 96;
   EmitGrid->Width = ClientWidth - EmitGrid->Left - 4 * Screen->PixelsPerInch / 96;
   Panel1->Top = ClientHeight - Panel1->Height - 4 * Screen->PixelsPerInch / 96;
   BtnLeimRap->Top = ClientHeight - 24 * Screen->PixelsPerInch / 96;
   BtnPiikit->Top = BtnLeimRap->Top;
   BtnHylkRap->Top = BtnLeimRap->Top;
   BtnVaTuloste->Top = BtnLeimRap->Top;
   BtnNaytaKartta->Top = BtnLeimRap->Top;
   Label3->Visible = !Panel1->Visible || Panel1->Top > Label3->Top + Label3->Height;
   Label5->Visible = !Panel1->Visible || Panel1->Top > Label5->Top + Label5->Height;
   Label7->Visible = !Panel1->Visible || Panel1->Top > Label7->Top + Label7->Height;
   Label12->Visible = !Panel1->Visible || Panel1->Top > Label12->Top + Label12->Height;
}
//---------------------------------------------------------------------------

MESSAGE void __fastcall TFormEmit::DisplTimerHandler(TMyMessage &msg)
{
	if (BdgMsgKirkkaus > 255) {
		BdgMsgKirkkaus = 255;
		}
	else if (BdgMsgKirkkaus > 40) {
		BdgMsgKirkkaus -= 10;
		}
	EdtBdgMsg->Color = TColor((1 + 256 + 65536) * BdgMsgKirkkaus);
}
//---------------------------------------------------------------------------
bool DisplTimerThreadOn;

void DisplTimerThread(LPVOID lpCn)
	{
	DisplTimerThreadOn = true;

	while (lkmThreadOn && DisplTimerThreadOn) {
		Sleep(200);
		SendMessage(FormEmit->Handle,WM_MYDISPLTIMER,0,0);
	   }
	DisplTimerThreadOn = false;
	}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::HaeTulostukseen(int badge)
{
	int i, kno;
	kilptietue kilp;

	if ((kno = bdg2kno(badge, 0)) <= 0) {
		for (int d = 1; d < datf2.numrec; d++) {
			kilp.getrec(d);
			if ((kilp.kilpstatus == 0) && kilp.ostiet[Osuus].badge[1] == badge) {
				kno = kilp.kilpno;
				break;
				}
			}
		}
	if (kno > 0) {
		if (EmitKirjoitin[0] == 0) {
			Valitsekirjoitin1Click(this);
			}
		if (EmitKirjoitin[0] != 0) {
			DKilp = getpos(kno);
			if (DKilp > 0) {
				Kilp.getrec(DKilp);
				i = getem(&Em, Kilp.kilpno, Osuus);
				if (i >= 0) {
					HaeTietue(i, 0);
					Nayta();
					Tulosta(4);
					}
				}
			}
		}
}
//---------------------------------------------------------------------------
int __fastcall TFormEmit::HaeKilpailija(void)
{
	kilptietue kilp;
	int os, kno, d;

	Em.osuus = -1;
	if (Em.badge != 200 && Em.badge && (kno = bdg2kno(Em.badge, 0)) != 0) {
		d = getpos(kno);
		kilp.getrec(d);
		for (os = Sarjat[kilp.sarja].osuusluku-1; os >= 0; os--) {
			if (kilp.ostiet[os].badge[0] == Em.badge)
				break;
			}
		if (os >= 0 && (EmitJarr(kno, os) == -1
			&& (!huomaatark || stschind(kilp.Tark(os, 0), "TIH") < 0))) {
			Em.kilpno = kno;
			if (val_korkein)
				Em.osuus = os;
			else {
				for (os = 0 ; os < Sarjat[kilp.sarja].osuusluku; os++) {
					if (kilp.ostiet[os].badge[0] == Em.badge &&
						EmitJarr(kno, os) == -1 &&
						(!huomaatark || stschind(kilp.Tark(os, 0), "TIH") < 0)) {
						Em.osuus = os;
						break;
						}
					}
				}
			}
		}
	Osuus = Em.osuus;
	return(0);
}
//---------------------------------------------------------------------------

int __fastcall TFormEmit::KysyKilpailija(void)
{
	CBOsuus->Enabled = true;
	if (Em.kilpno == 0) {
		EdtKilpno->ReadOnly = false;
		if (Em.badge == 200) {
			EdtMessage->Text = L"Viallinen kortti, koodi 200. Syötä kilpno ja osuus";
			EdtMessage->Color = clYellow;
			EdtMessage->Visible = true;
			}
		else if (!jalkisyotto) {
			EdtMessage->Text = L"Tuntematon koodi. Syötä kilpailijan numero ja osuus";
			EdtMessage->Color = clYellow;
			EdtMessage->Visible = true;
			}
		else {
			EdtMessage->Text = L"Syötä kilpailijan numero ja osuus";
			EdtMessage->Color = clYellow;
			EdtMessage->Visible = true;
			}
		Kilp.nollaa();

		DKilp = -1;
		Osuus = -1;
		Nayta();
		Refresh();
		FormEmitMuutokset->JoukkueHaku = true;
		BtnMuutoksetClick(this);
		}
	else {
		EdtMessage->Text = L"Kortin osuutta ei voitu määrittää. Anna osuus.";
		EdtMessage->Color = clYellow;
		EdtMessage->Visible = true;
		DKilp = getpos(Em.kilpno);
		Kilp.getrec(DKilp);
		Osuus = -1;
		Refresh();
		Nayta();
		FormEmitMuutokset->OsuusHaku = true;
		BtnMuutoksetClick(this);
		}
	EdtMessage->Text = L"";
	EdtMessage->Color = clBtnFace;
	EdtMessage->Visible = true;
	return(0);
}
//---------------------------------------------------------------------------

void naytatulos(int tulos);

int __fastcall TFormEmit::ProsEmitMaali(void)
{
	ratatp *rt;
	int tallkilp = 0, ino = 0, piste = 0;
	aikatp itm, etm;
	int ptm;

	if (esitark < 0 || DKilp <= 0 || Osuus < 0)
		return(0);

	rt = getrata(Kilp.ostiet[Osuus].hajonta);
	if (rt && rt->maalilaji == 4 && Em.maali == TMAALI0/KSEK) {
			Em.maali = e_maaliaika(&Em, &Kilp, Osuus)/KSEK;
			if (Em.maali == TMAALI0/KSEK) {
				naytatulos(3);
				erbeep();
				writeerror("Maaliaikaa ei pystytty määräämään", 0);
				}
			else
			   tallkilp = 1;
			}
	 itm.t = Em.maali*SEK/10*AIKAJAK;
	 if (aikajono[ino]) {
		 if (Em.maali != TMAALI0*10/SEK) {
			itm.t = (Em.maali*SEK/10 + aikajono[ino]->aikakorjaus)*AIKAJAK;
			itm.date = tm_date(itm.t);
			if (!esitark && ((Em.package > 10000000L && Em.package < 11000000L &&
			   (!rt || rt->maalilaji != 4)))) {
			   piste = 0;
			   itm.kno = Em.kilpno;
			   itm.osuus = Em.osuus;
			   itm.piste = piste;
			   itm.badge = Em.badge;
			   itm.lahde = kone;
			   itm.jono = ino;
			   tall_rivi(ino, &itm, NULL, NULL, 0, 0, 0);
			   tallkilp = 1;
			   }
			}
		 else if (autokilp && seurkrivi) {
			for (ptm = aikajono[ino]->rwtime-1; ptm >= 0; ptm--)
			   if (Em.kilpno == aikajono[ino]->getAktTime(&etm, ptm)->kno &&
				  Em.osuus == etm.osuus)
				  break;
			if (Em.kilpno == aikajono[ino]->getAktTime(&etm, ptm)->kno &&
			   Em.osuus == etm.osuus) {
			   if (ptm - seurkrivi > -10) seurkrivi = ptm;
			   }
			else {
			   while (seurkrivi - aikajono[ino]->rwtime < 0 && aikajono[ino]->getAktTime(&etm, seurkrivi)->kno)
				  seurkrivi++;
			   if (seurkrivi - aikajono[ino]->rwtime < 0) {
				  aikajono[ino]->getAktTime(&etm, seurkrivi);
				  itm.t = etm.t;
				  itm.date = etm.date;
				  itm.kno = Em.kilpno;
				  itm.osuus = Em.osuus;
				  itm.piste = piste;
				  itm.badge = Em.badge;
				  itm.lahde = kone;
				  itm.jono = ino;
				  tall_rivi(ino, &itm, &etm, NULL, 0, 0, 0);
				  seurkrivi++;
				  tallkilp = 1;
				  }
			   }
			aikaruutu(0);
			}
		}
	 if (tallkilp && !esitark && itm.t != TMAALI0*AIKAJAK && Kilp.Maali(Osuus, piste) == TMAALI0) {
		Kilp.setMaali(Osuus, piste, pyoristatls(purajak(itm.t), 1));
		tallkilp = 1;
		}
	return(tallkilp);
	}

void __fastcall TFormEmit::ProsEmit(void)
{
	int kh, tallkilp;
	kilptietue kilp1;

	lueem(&Em, emithead2);
	Em.kirjaus = T_TIME(biostime(0,0));
	attnbeep();
	emithead2++;
	if (Em.badge == 0) {
		putem(&Em, emithead2-1, 0);
		return;
		}
	if (!jalkisyotto) {
		if (HaeKilpailija())
			return;
		}
	if (Em.kilpno == 0 || Em.osuus < 0) {
		if (KysyKilpailija())
			return;
		}
	if (Em.kilpno > 0 && Em.osuus >= 0) {
		EnterCriticalSection(&tall_CriticalSection);
		DKilp = getpos(Em.kilpno);
		Osuus = Em.osuus;
		Kilp.getrec(DKilp);
		Em.badge = haeVapaaKoodi(Em.badge);
		if (!kilpparam.kaksibadge && Kilp.ostiet[Em.osuus].badge[1] == 0)
			Kilp.ostiet[Em.osuus].badge[1] = Kilp.ostiet[Em.osuus].badge[0];
		Kilp.ostiet[Em.osuus].badge[0] = Em.badge;
		setEmitJarr(Em.kilpno, Em.osuus, emithead2-1);
		ProsEmitMaali();
		kh = tark_badge(&Em, &Kilp, false);
		tallenna(&Kilp, DKilp, 0, 0, 0, 0);
		LeaveCriticalSection(&tall_CriticalSection);
		EdtKilpno->ReadOnly = true;
		CBOsuus->Enabled = false;
		if (!putem(&Em, emithead2-1, 0)) {
			laheta_em(&Em, 1, 0);
#ifdef PARTIO
			if (kilpparam.partio && !kilpparam.rogaining) {
				if (tark_partio(emittulparam, &Kilp, (autorap & 2) || (autorap && kh == 2), 0) >= 0) {
					tallenna(&Kilp,DKilp,TRUE, 0, 0, 0);
					}
				}
#endif
			if (emittulparam && (autorap == 2 || (autorap == 1 && kh == 2) || autorapsrj[Kilp.sarja]))
				emit_rap(emittulparam, Em.kilpno, Em.osuus);
			}
		}
	HaeTietue(emithead2-1, 0);
}
//---------------------------------------------------------------------------

MESSAGE void __fastcall TFormEmit::NewCardHandler(TMyMessage &msg)
{
	int Vast, bdg;
	wchar_t msgtxt[200];

	if (msg.wparam > 0) {
		HaeTulostukseen(msg.wparam);
		return;
		}
	if (!esitark) {
		if (Tietue < emithead-1 && !CBLue->Checked && CBViim->Checked) {
			Tietue = em_HaeSeuraava(emithead2-1);
			HaeTietue(emithead2-1, 0);
			return;
			}
		if (emithead2 < emithead && Tietue >= 0) {
			if (Tietue < em_HaeViimeinen() || EmitMuutosFlag) {
				BtnLoppuun->Visible = true;
					return;
				}
			else {
				BtnLoppuun->Visible = false;
				if (OnkoMuutoksia()) {
					EdtMessage->Text = L"Uusi kortti, kun muutoksia tallentamatta";
					EdtMessage->Color = clYellow;
					return;
/*
					Vast = select3(3, L"Käsitellyissä tiedoissa tallentamattomia muutoksia. Tallennetaanko ennen uuden kortin käsittelyä?",
						L"Tallennetaanko muutokset", L"Tallenna", L"Hylkää muutokset", L"Jatka käsittelyssä olevan käsittelyä", this);
					switch (Vast) {
						case 1:
							tallennaKilpailija(false);
							return;
						case 2:
							break;
						case 3:
							return;
						default:
							CBViim->Checked = false;
							BtnLoppuun->Visible = false;
							return;
						}
*/
					}
				}
			}
		}
	BtnLoppuun->Visible = false;
	if (emithead2 < emithead) {
		emittp em;
		lueem(&em, emithead2);
		if (em.kirjaus != TMAALI0/KSEK) {
			emithead2++;
			return;
			}
		bdg = em.badge;
		if (bdg <= 0) {
			Application->MessageBoxW(msgtxt, L"Virhe luennassa", MB_OK);
			return;
			}
		EdtBdgMsg->Text = UnicodeString(L"Käsittelyyn kortti ")+UnicodeString(bdg);
		BdgMsgKirkkaus = 256;
		if (!DisplTimerThreadOn)
			_beginthread(DisplTimerThread, 10240, &Vast);
		UusiTietue = true;
		NollaaMessage();
		ProsEmit();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::HaeUusiTietue(void)
{
	SendMessage(Handle,WM_MYMSGNEWCARD,0,0);
}
//---------------------------------------------------------------------------

void emitLuettu(int badge)
{
	if (badge > 0)
		SendMessage(FormEmit->Handle,WM_MYMSGNEWCARD,badge,0);
}
//---------------------------------------------------------------------------

int __fastcall TFormEmit::HaeTietue(int pos, int suunta)
{
	int i_em, kilpno;
	emittp em;

	switch (suunta) {
		case 0:
			if ((i_em = em_HaeSeuraava(pos-1)) != pos) {
				i_em = em_HaeEdellinen(pos);
				}
			break;
		case 1:
			i_em = em_HaeSeuraava(pos-1);
			break;
		case -1:
			i_em = em_HaeEdellinen(pos+1);
			break;
		}
	emitptr = i_em;
	Tietue = emitptr;
	if (Tietue >= 0)
		lueem(&em, Tietue);
	else
		em.nollaa();
	SBTietue->SetParams(Tietue, 0, (emithead2 > 1) ? (emithead2 - 1) : 1);
	EdtTietue->Text = UnicodeString(Tietue+1);
	Paivita(&em);
	return(0);
}
//---------------------------------------------------------------------------

int __fastcall TFormEmit::OtaTietue(int pos)
{
	int i_em, kilpno;
	emittp em;

	if (emithead <= 0)
		return(0);
	if (pos < 0)
		pos = 0;
	if (pos >= emithead)
		pos = emithead - 1;
	emitptr = pos;
	Tietue = emitptr;
	lueem(&em, Tietue);
	if (em.kilpno < minkilpno || EmitJarr(em.kilpno, em.osuus) != pos) {
		eiLiitetty = true;
		EdtMessage->Text = L"Leimoja ei liitetty kilpailijaan";
		EdtMessage->Color = clYellow;
		}
	else {
		eiLiitetty = false;
		EdtMessage->Text = L"";
		EdtMessage->Color = clBtnFace;
		}
	SBTietue->SetParams(Tietue, 0, (emithead2 > 1) ? (emithead2 - 1) : 1);
	EdtTietue->Text = UnicodeString(Tietue+1);
	Paivita(&em);
	return(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtTietueExit(TObject *Sender)
{
   int pos;

   if (EdtTietue->Text.IsEmpty()) {
	  pos = emithead2;
	  }
   else {
	  pos = _wtoi(EdtTietue->Text.c_str());
	  }
   NollaaMessage();
   HaeTietue(pos-1, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::NollaaMessage(void)
{
	EdtMessage->Text = L"";
	EdtMessage->Color = clBtnFace;
}
//---------------------------------------------------------------------------


void __fastcall TFormEmit::EmitGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_NEXT && Shift.Contains(ssCtrl)) {
		NollaaMessage();
		HaeTietue(Tietue+1, 1);
	}
	if (Key == VK_PRIOR && Shift.Contains(ssCtrl)) {
		NollaaMessage();
		HaeTietue(Tietue-1, -1);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Emit");
}
//---------------------------------------------------------------------------


void __fastcall TFormEmit::BtnTallennaClick(TObject *Sender)
{
	tallennaKilpailija(false);
	if (CBHaeSeurLaskettava->Checked) {
		NollaaMessage();
		HaeTietue(Tietue+1, 1);
		BtnLasketulosClick(Sender);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::CSarjaChange(TObject *Sender)
{
	int srj;

	srj = ValSarja;
	Kilp.sarja = CSarja->ItemIndex;
	NaytaEmit();
	ValSarja = srj;
	OnkoMuutoksia();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::CRataChange(TObject *Sender)
{
	int i;

	i = ValRata;
	Kilp.setHajonta(Osuus, CRata->Text.c_str());
	NaytaEmit();
	ValRata = i;
	OnkoMuutoksia();

}
//---------------------------------------------------------------------------



void __fastcall TFormEmit::Kortinkoodi1Click(TObject *Sender)
{
	Kortinkoodi1->Checked = !Kortinkoodi1->Checked;
	EdtOrigBadge->Visible = Kortinkoodi1->Checked;
	LblOrigBadge->Visible = EdtOrigBadge->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key < L' ' && Key != 8) {
	   Key = 0;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtTietueKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		EdtTietueExit(Sender);
		}
	if (Key < L' ' && Key != 8) {
	   Key = 0;
	}
}
//---------------------------------------------------------------------------


void __fastcall TFormEmit::EdtBadgeExit(TObject *Sender)
{
//	EdtBadge->Color = clBtnFace;
	EdtBadge->Color = clVaaleanHarmaa;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::tallennaKilpailija(bool kysy)
{
	int d, bdg, orgbdg;
	wchar_t koodit[] = L"--TIKHEP";
	wchar_t st[20];

	if (DKilp != getpos(Kilp.kilpno)) {
		Application->MessageBox(L"Ristiriita joukkueen numerossa, ei tallenneta", L"Virhe",	MB_OK);
		return;
		}
	if (CSarja->ItemIndex < 0) {
		Application->MessageBox(L"Valitse sarja ennen tallentamista", L"Virhe",	MB_OK);
		return;
		}
	if (kysy) {
		if (Application->MessageBox(L"Vahvista tallennus", L"Vahvista",
			MB_YESNO) != IDYES)
		return;
		}
	EdtKilpno->ReadOnly = true;
	CBOsuus->Enabled = false;
	EnterCriticalSection(&tall_CriticalSection);
	Kilp.getrec(DKilp);
	Kilp.sarja = CSarja->ItemIndex;
	if (Sarjat[Kilp.sarja].tsak[Osuus])
		Kilp.ostiet[Osuus].sakko = SEK*Sarjat[Kilp.sarja].tsak[Osuus]*_wtoi(EdtSakkoja->Text.c_str());
	if (kilpparam.partio) {
		for (int os = 0; os < Sarjat[Kilp.sarja].osuusluku; os++)
			Kilp.SetTark(os, koodit[TarkVal->ItemIndex+1]);
		}
	else
		Kilp.SetTark(Osuus, koodit[TarkVal->ItemIndex+1]);
	Kilp.setMaali(Osuus, 0, wstrtoaika_vap(EdtMaali->Text.c_str(), t0));
	if (CRata->ItemIndex > 0)
		Kilp.setHajonta(Osuus, CRata->Text.c_str());
	Kilp.ostiet[Osuus].uusi = 0;
	Kilp.setSelitys(Osuus, EdtSelitys->Text.c_str());
	tallenna(&Kilp, DKilp, 0, 0, 0, 0);
	LeaveCriticalSection(&tall_CriticalSection);
	Nayta();
	TallVals();
	if (!EdtKilpno->ReadOnly)
		FocusControl(EdtKilpno);
	if (LblToimi->Visible)
		LblToimi->Caption = L"Lue kortti";
//	if (CBSeuralisa->Checked && CBSeurahaku->Checked && FormSeurat->SeuraHakuOn())
//		FormSeurat->TallennaSeura(Kilp.piiri, Kilp.maa, Kilp.seuralyh, Kilp.seura, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtBadgeKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	EdKey = Key + (Shift.Contains(ssShift) ? 0x10000 : 0) + (Shift.Contains(ssCtrl) ? 0x20000 : 0)
		+ (Shift.Contains(ssAlt) ? 0x40000 : 0);
	if (Key == VK_RETURN)
		FocusControl(EdtSNimi);

}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::lueKilp3(int d)
{
	kilptietue kilp1;

	DKilp = d;
	kilp1.getrec(DKilp);
	kilp1.SetTark(Osuus, Kilp.wTark(Osuus, false));
	kilp1.setMaali(Osuus, 0, Kilp.Maali(Osuus, 0));
	kilp1.ostiet[Osuus].badge[0] = Kilp.ostiet[Osuus].badge[0];
	kilp1.ostiet[Osuus].uusi = Kilp.ostiet[Osuus].uusi;
	Kilp = kilp1;
	Nayta();
	OnkoMuutoksia();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtKilpnoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	int d, kno;
	wchar_t st[20];

	if (Key == VK_RETURN && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		memset(st, 0, sizeof(st));
		wcsncpy(st, EdtKilpno->Text.c_str(), sizeof(st)/2-1);
		if (st[0] == L'%')
			wmemmove(st, st+1, wcslen(st));
		kno = _wtoi(st);
		d = getpos(kno);
		if (d != DKilp) {
			if (d <= 0) {
				Application->MessageBoxW(L"Pyydetty kilpailijanumero ei ole käytössä", L"Ei löydy",
					MB_OK);
				return;
				}
			else {
				lueKilp3(d);
				tallennaKilpailija(false);
				}
			}
		tallennaKilpailija(esitark == 0);
		return;
		}
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		memset(st, 0, sizeof(st));
		wcsncpy(st, EdtKilpno->Text.c_str(), sizeof(st)/2-1);
		if (st[0] == L'%')
			wmemmove(st, st+1, wcslen(st));
		kno = _wtoi(st);
		d = getpos(kno);
		if (d <= 0) {
			Application->MessageBoxW(L"Pyydetty kilpailijanumero ei ole käytössä", L"Ei löydy",
				MB_OK);
			}
		else {
			if (esitark == -2) {
				kilptietue kilp1;
				kilp1.getrec(d);
				HaeTulostukseen(kilp1.ostiet[Osuus].badge[0]);
				}
			else {
				lueKilp3(d);
				tallennaKilpailija(false);
				}
			}
		}
}
//---------------------------------------------------------------------------
bool __fastcall TFormEmit::OnkoMuutoksia(void)
{
	bool retval = false;
	if (Tietue < 0 || DKilp <= 0)
		return(false);
	if (
		ValOsuus != CBOsuus->ItemIndex ||
		ValSNimi != EdtSNimi->Text ||
		ValSeura != EdtSeura->Text ||
		ValTulos != EdtTulos->Text ||
		ValMaali != EdtMaali->Text ||
		ValLahto != EdtLahto->Text ||
		ValSelitys != EdtSelitys->Text ||
		ValTarkVal != TarkVal->ItemIndex ||
		ValRata != CRata->ItemIndex ||
		ValSarja != CSarja->ItemIndex) {
		retval = true;
		}
	BtnTallenna->Visible = retval;
	BtnPeruuta->Visible = retval;
	return(retval);
}

void __fastcall TFormEmit::EdtRataChange(TObject *Sender)
{
	OnkoMuutoksia();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::TarkValChange(TObject *Sender)
{
	OnkoMuutoksia();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtSelitysChange(TObject *Sender)
{
	OnkoMuutoksia();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtMaaliChange(TObject *Sender)
{
	OnkoMuutoksia();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::TarkValKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		Key = 0;
		tallennaKilpailija(true);
		FocusControl(EdtBadge);
		return;
		}
	if (Key == VK_RETURN)
		FocusControl(EdtSelitys);

}
//---------------------------------------------------------------------------


void __fastcall TFormEmit::CRataKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		Key = 0;
		tallennaKilpailija(true);
		FocusControl(EdtBadge);
		return;
		}
	if (Key == VK_RETURN)
		FocusControl(TarkVal);

}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::CSarjaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		Key = 0;
		tallennaKilpailija(true);
		FocusControl(EdtBadge);
		return;
		}
	if (Key == VK_RETURN) {
		Key = 0;
		if (CRata->Visible)
			FocusControl(CRata);
		else if (TarkVal->Visible)
			FocusControl(TarkVal);
		else if (BtnTallenna->Visible)
			FocusControl(BtnTallenna);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::EdtSelitysKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		Key = 0;
		tallennaKilpailija(true);
		FocusControl(EdtBadge);
		return;
		}
	if (Key == VK_RETURN)
		FocusControl(EdtMaali);
}
//---------------------------------------------------------------------------


void __fastcall TFormEmit::EdtRataKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		Key = 0;
		tallennaKilpailija(true);
		FocusControl(EdtBadge);
		return;
		}
	if (Key == VK_RETURN)
		if (TarkVal->Visible)
			FocusControl(TarkVal);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::BadgeHaku(void)
{
/*
	int n = 0, badge;

	badge = _wtoi(EdtBadge->Text.c_str());

	if (badge && Henkiltietokanta1->Checked && FormTKanta->HenkiloHakuOn()) {
		n = FormTKanta->NaytaHenkilot(NULL, NULL, NULL, NULL, badge);
		}
	if (n == 1) {
		NimiKeyHit = False;
		SiirraHTTiedot(0);
		if (Application->MessageBoxW(L"Oikea kilpailija löytynyt?", L"Vahvista", MB_YESNO) == IDYES) {
			tallennaKilpailija(false);
			return;
			}
		FocusControl(EdtSNimi);
		}
	if (n > 1) {
		Application->MessageBoxW(L"Valitse osanottaja luettelosta käyttäen alas/ylös -näppäimiä Emit-kentässä", L"Valitse", MB_OK);
		EdtBadge->Color = clLime;
		FocusControl(EdtBadge);
		}
*/
}
//---------------------------------------------------------------------------

static bool MuutosHaku = true, eroSelaus = false;
static int JatkaHakua;
extern emiterotp *EmitErot;
extern int nEmitErot;
static int kEmitErot = 0;

void __fastcall TFormEmit::CBVaadiChange(TObject *Sender)
{
	if (CBVaadi->ItemIndex >= CBVaadi->Items->Count - 2) {
		naytaEiAkt = true;
		Application->MessageBoxW(L"Ei-aktiivisiin tietoihin tehdyt muutokset eivät siirry verkossa."
			L"Haku vain lukemisjärjestyksen mukaan.",
			L"Varoitus", MB_OK);
		RGHakuPeruste->ItemIndex = 3;
		RGHakuPeruste->Enabled = false;
		}
	else {
		eiLiitetty = false;
		EdtMessage->Text = L"";
		EdtMessage->Color = clBtnFace;
		naytaEiAkt = false;
		RGHakuPeruste->Enabled = true;
		}
	CSarjaVaat->Visible = CBVaadi->ItemIndex == 7;
	LblSarjaVaat->Visible = CSarjaVaat->Visible;
	if (CSarjaVaat->Visible) {
		CSarjaVaat->Clear();
		for (int srj = 0; srj < sarjaluku; srj++) {
			CSarjaVaat->Items->Add(Sarjat[srj].sarjanimi);
		CSarjaVaat->ItemIndex = 0;
		}
	}
}
//---------------------------------------------------------------------------

static bool On99(emittp *Em)
{
	for (int i = 0; i < 50; i++)
		if (Em->ctrlcode[i] == 99)
			return(true);
	return(false);
}
//---------------------------------------------------------------------------

bool __fastcall TFormEmit::HakuEhto(bool ylos)
{
	bool vast = false;

	eroSelaus = false;
	if (RGHakuPeruste->ItemIndex == 1) {
		if (Em.osuus != CBHakuOsuus->ItemIndex)
			return(false);
		}
	switch (CBVaadi->ItemIndex) {
		case 0:
			vast = true;
			break;
		case 1:
			if (Kilp.ostiet[Osuus].badge[0] == Em.badge) {
				if (Kilp.wTark(Osuus, false) != L'T' || Kilp.Maali(Osuus, 0) == TMAALI0 ||
					tarkista(&Em, &Kilp, 0, -1, 0))
					vast = true;
				}
			break;
		case 2:
			if (Kilp.wTark(Osuus, false) == L'I')
				vast = true;
			break;
		case 3:
			if (Kilp.wTark(Osuus, false) == L'H')
				vast = true;
			break;
		case 4:
			if (Kilp.wTark(Osuus, false) == L'K')
				vast = true;
			break;
		case 5:
			if (Kilp.wTark(Osuus, false) == L'-')
				vast = true;
			break;
		case 6:
			if (Kilp.Maali(Osuus, 0) == TMAALI0)
				vast = true;
			break;
		case 7:
			if (Kilp.sarja == CSarjaVaat->ItemIndex)
				vast = true;
			break;
		case 8:
			vast = On99(&Em);
			break;
		case 9:
			vast = true;
			break;
		case 10:
			if (Em.badge == 0 || Em.kilpno == 0 || Kilp.ostiet[Em.osuus].badge[0] != Em.badge ||
				Em.kilpno != Kilp.kilpno) {
				vast = true;
				}
			break;
	}
	return(vast);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::RGHakuPerusteClick(TObject *Sender)
{
	switch (RGHakuPeruste->ItemIndex) {
		case 0:
			EdtHakuArvo->Text = EdtSeura->Text;
			Label11->Visible = true;
			CBHakuOsuus->Visible = true;
			break;
		case 1:
			EdtHakuArvo->Text = EdtKilpno->Text;
			Label11->Visible = true;
			CBHakuOsuus->Visible = true;
			break;
		case 2:
			EdtHakuArvo->Text = L"";
			Label11->Visible = false;
			CBHakuOsuus->Visible = false;
			break;
		case 3:
			EdtHakuArvo->Text = UnicodeString(Tietue+1);
			Label11->Visible = false;
			CBHakuOsuus->Visible = false;
			break;
		case 4:
			haeEmiterot();
			Label11->Visible = false;
			CBHakuOsuus->Visible = false;
			if (kEmitErot >= nEmitErot)
				kEmitErot = nEmitErot - 1;
			EdtHakuArvo->Text = UnicodeString(EmitErot[kEmitErot].ero);
			if (nEmitErot) {
				NollaaMessage();
				HaeTietue(EmitErot[kEmitErot].pos, 0);
				}
			break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::BtnHaeSeurClick(TObject *Sender)
{
	wchar_t txt[62];
	char key[MAXINDL+1];
	int d, i, k, count = 0;

	NollaaMessage();
	CBViim->Checked = false;
	do {
		JatkaHakua = 1;
		switch (RGHakuPeruste->ItemIndex) {
			case 0:
				if (nextix(&nameindex, key, &d)) {
					JatkaHakua = 0;
					break;
					}
				DKilp = d;
				Kilp.getrec(d);
				MuutosHaku = false;
				EdtHakuArvo->Text = Kilp.Joukkue(txt, 60);
				i = getem(&Em, Kilp.kilpno, CBHakuOsuus->ItemIndex);
				if (i >= 0) {
					HaeTietue(i, 0);
					}
				else {
					Tietue = emithead2-1;
					Nayta();
					}
				MuutosHaku = true;
				break;
			case 1:
				i = _wtoi(EdtHakuArvo->Text.c_str());
				EdtHakuArvo->Text = UnicodeString(i+1);
				if (i >= maxkilpno)
					JatkaHakua = 0;
				break;
			case 2:
				i = _wtoi(EdtHakuArvo->Text.c_str());
				k = haebdg(i, 0);
				if (k >= 0 && k < nbadge[0]-1) {
					i = bdg_kno[0][k+1].badge;
					}
				else if (k < 0) {
					i = bdg_kno[0][-k - 1].badge;
					}
				else
					i++;
				EdtHakuArvo->Text = UnicodeString(i);
				if (i > 19999999)
					JatkaHakua = 0;
				break;
			case 3:
				i = _wtoi(EdtHakuArvo->Text.c_str());
				EdtHakuArvo->Text = UnicodeString(i+1);
				if (i >= emithead2)
					JatkaHakua = 0;
				break;
			case 4:
				if (kEmitErot < nEmitErot - 1) {
					kEmitErot++;
					eroSelaus = true;
					HaeTietue(EmitErot[kEmitErot].pos, 0);
					EdtHakuArvo->Text = UnicodeString(EmitErot[kEmitErot].ero);
					}
				JatkaHakua = 0;
				break;
			}
		} while(JatkaHakua > 1 || (JatkaHakua && HakuEhto(true) == false));
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::BtnHaeEdClick(TObject *Sender)
{
	wchar_t txt[62];
	char key[MAXINDL+1];
	int d, i, k;

	NollaaMessage();
	CBViim->Checked = false;
	do {
		JatkaHakua = 1;
		switch (RGHakuPeruste->ItemIndex) {
			case 0:
				if (previx(&nameindex, key, &d)) {
					JatkaHakua = 0;
					break;
					}
				DKilp = d;
				Kilp.getrec(d);
				MuutosHaku = false;
				EdtHakuArvo->Text = Kilp.Joukkue(txt, 60);
				i = getem(&Em, Kilp.kilpno, CBHakuOsuus->ItemIndex);
				if (i >= 0) {
					HaeTietue(i, 0);
					}
				else {
					Tietue = emithead2;
					Nayta();
					}
				MuutosHaku = true;
				break;
			case 1:
			case 3:
				i = _wtoi(EdtHakuArvo->Text.c_str());
				if (i > 1)
					EdtHakuArvo->Text = UnicodeString(i-1);
				else
					JatkaHakua = 0;
				break;
			case 2:
				i = _wtoi(EdtHakuArvo->Text.c_str());
				if (i > 1) {
					k = haebdg(i, 0);
					if (k > 0) {
						i = bdg_kno[0][k-1].badge;
						}
					else if (k < -1) {
						i = bdg_kno[0][-k - 2].badge;
						}
					else
						i--;
					EdtHakuArvo->Text = UnicodeString(i);
					}
				else
					JatkaHakua = 0;
				break;
			case 4:
				if (kEmitErot > 0) {
					kEmitErot--;
					HaeTietue(EmitErot[kEmitErot].pos, 0);
					eroSelaus = true;
					EdtHakuArvo->Text = UnicodeString(EmitErot[kEmitErot].ero);
					}
				JatkaHakua = 0;
				break;
			}
		} while(JatkaHakua > 1 || (JatkaHakua && HakuEhto(false) == false));
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::BtnHakuValmisClick(TObject *Sender)
{
	Panel1->Visible = false;
	BtnHaeOo->Visible = true;
	if (FormEmit->Width < 280 * Screen->PixelsPerInch /96)
		FormEmit->ClientHeight = BtnHaeOo->Top + BtnHaeOo->Height +
			6 * Screen->PixelsPerInch /96;
   Label3->Visible = true;
   Label5->Visible = true;
   Label7->Visible = true;
   Label12->Visible = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtHakuArvoChange(TObject *Sender)
{
	wchar_t txt[62];
	char key[MAXINDL+1];
	int d, i, k;

	if (!MuutosHaku || !CBHaeArvo->Checked)
		return;
	NollaaMessage();
	CBViim->Checked = false;
	switch (RGHakuPeruste->ItemIndex) {
		case 0:
			wcsncpy(txt, EdtHakuArvo->Text.c_str(),60);
			txt[60] = 0;
			keyfromU16name(key, txt, nameindex.keyl-3, 0);
			searchix(&nameindex, key, &d);
			DKilp = d;
			Kilp.getrec(d);
			Osuus = CBHakuOsuus->ItemIndex;
			if (!esitark) {
				i = getem(&Em, Kilp.kilpno, CBHakuOsuus->ItemIndex);
				if (i >= 0) {
					HaeTietue(i, 0);
					}
				else {
					Tietue = emithead2-1;
					Nayta();
					}
				}
			else
				Nayta();
			break;
		case 1:
			i = _wtoi(EdtHakuArvo->Text.c_str());
			if (i < minkilpno || i > maxkilpno) {
				JatkaHakua = false;
				break;
				}
			if ((d = getpos(i)) > 0) {
				Osuus = CBHakuOsuus->ItemIndex;
				DKilp = d;
				Kilp.getrec(d);
				if (!esitark) {
					i = getem(&Em, Kilp.kilpno, CBHakuOsuus->ItemIndex);
					if (i >= 0) {
						HaeTietue(i, 0);
						}
					else {
						Tietue = emithead2-1;
						Nayta();
						}
					}
				else
					Nayta();
				}
			else {
				Kilp.nollaa();
				DKilp = 0;
				Em.nollaa();
				Nayta();
				JatkaHakua = 2;
				}
			break;
		case 2:
			i = _wtoi(EdtHakuArvo->Text.c_str());
			if (i < bdg_kno[0][0].badge) {
				i = bdg_kno[0][0].badge;
				JatkaHakua = 0;
				break;
				}
			if (i > bdg_kno[0][nbadge[0]-1].badge) {
				i = bdg_kno[0][nbadge[0]-1].badge;
				JatkaHakua = 0;
				break;
				}
			k = bdg2kno(i, 0);
			if (k > 0 && (d = getpos(k)) > 0) {
				int os;
				DKilp = d;
				Kilp.getrec(d);
				for (os = 0; os < Sarjat[Kilp.sarja].osuusluku; os++)
					if (i == Kilp.ostiet[os].badge[0])
						break;
				if (os >= Sarjat[Kilp.sarja].osuusluku)
					os = 0;
				Osuus = os;
				i = getem(&Em, Kilp.kilpno, Osuus);
				if (i >= 0) {
					HaeTietue(i, 0);
					}
				else {
					Em.nollaa();
					Nayta();
					}
				}
			else {
				Kilp.nollaa();
				DKilp = 0;
				Em.nollaa();
				Nayta();
				JatkaHakua = 2;
				}
			break;
		case 3:
			i = _wtoi(EdtHakuArvo->Text.c_str());
			if (i < 1 || i > emithead2) {
				JatkaHakua = 0;
				if (i > emithead2)
					i = emithead2;
				if (i < 1)
					i = 1;
				EdtHakuArvo->Text = UnicodeString(i);
				break;
				}
			if (naytaEiAkt)
				OtaTietue(i-1);
			else
				HaeTietue(i-1, 0);
			break;
		case 4:
			if (eroSelaus) {
				break;
				}
			i = _wtoi(EdtHakuArvo->Text.c_str());
			for (kEmitErot = 0; kEmitErot < nEmitErot-1; kEmitErot++) {
				if (EmitErot[kEmitErot].ero >= i)
					break;
				}
			if (nEmitErot) {
				HaeTietue(EmitErot[kEmitErot].pos, 0);
				}
			break;
	}
	eroSelaus = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::CBHakuOsuusClick(TObject *Sender)
{
	EdtHakuArvoChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::BtnHaeOoClick(TObject *Sender)
{
	if (Height < 650 * Screen->PixelsPerInch / 96)
		Height = 650 * Screen->PixelsPerInch / 96;
	Panel1->Visible = true;
	if (Panel1->Visible) {
		Panel1->BringToFront();
		}
   Label3->Visible = Panel1->Top > Label3->Top + Label3->Height;
   Label5->Visible = Panel1->Top > Label5->Top + Label5->Height;
   Label7->Visible = Panel1->Top > Label7->Top + Label7->Height;
   Label12->Visible = Panel1->Top > Label12->Top + Label12->Height;
	CBHaeArvo->Checked = true;
	BtnHaeOo->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtBadgeChange(TObject *Sender)
{
	if (Em.badge)
		EdtBadge->Text = UnicodeString(Em.badge);
	else
		EdtBadge->Text = UnicodeString(Kilp.ostiet[Osuus].badge[0]);
}
//---------------------------------------------------------------------------


void __fastcall TFormEmit::EdtBadgeKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key != L'\t')
		Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::BtnLoppuunClick(TObject *Sender)
{
	if (EmitMuutosFlag) {
		Application->MessageBoxW(L"Luenta voi jatkua vasta, kun kaavake \"Emit-muutokset\" on suljettu", L"Ohje", MB_OK);
		return;
		}
	if (OnkoMuutoksia()) {
		int Vast;
		Vast = select3(3, L"Muutoksia tallentamatta. Tallennetaanko? ",
		L"Tallennetaanko muutokset", L"Tallenna", L"Hylkää muutokset", L"Jatka käsittelyä",
		this);
		switch (Vast) {
			case 1:
				tallennaKilpailija(false);
				break;
			case 3:
				CBViim->Checked = false;
				return;
			default:
				Nayta();
				break;
			}
		}
	CBViim->Checked = true;
	NollaaMessage();
	HaeTietue(emithead2-1, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::BtnPeruutaClick(TObject *Sender)
{
//	PeruutaMuutokset();
	if (DKilp > 0)
		Kilp.getrec(DKilp);
	Nayta();
	Refresh();
}
//---------------------------------------------------------------------------

int __fastcall TFormEmit::LaskeTulos(void)
{
	int tm, tlahto = TMAALI0;
	kilptietue kilp;
	wchar_t st[20];

	kilp = Kilp;
	tm = e_maaliaika(&Em, &kilp, Osuus);

	if (tm != TMAALI0) {
		AIKATOWSTRS(st, tm, t0);
		st[kilpparam.laika2] = 0;
		EdtMaali->Text = st;
		kilp.setMaali(Osuus, 0, tm);
		AIKATOWSTRS(st, kilp.Tulos(Osuus, 0), 0);
		st[kilpparam.laika2] = 0;
		EdtTulos->Text = st;
		if (kilp.Lahto(Osuus) != TMAALI0) {
			AIKATOWSTRS(st, kilp.Lahto(Osuus), t0);
			st[8] = 0;
			EdtLahto->Text = st;
			}
		else
			EdtLahto->Text = L"";
		}
	EdtEro->Text = L"";
	OnkoMuutoksia();
	return(kilp.Maali(Osuus, 0));
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::BtnLasketulosClick(TObject *Sender)
{
	LaskeTulos();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::BtnMuutoksetClick(TObject *Sender)
{
	static bool initialisoitu;

	if (Kilp.kilpno > 0 && Osuus >= 0 && EmitJarr(Kilp.kilpno, Osuus) < 0) {
		Application->MessageBoxW(L"Valittulla kilpailijalla ei leimaustietoja", L"Este", MB_OK);
		return;
		}
	Kilp_0 = Kilp;
	DKilp_0 = DKilp;
	Em_0 = Em;
	Osuus_0 = Osuus;
	if (!FormEmitMuutokset)
		FormEmitMuutokset = new TFormEmitMuutokset(this);
	FormEmitMuutokset->PageControl1->ActivePageIndex = 0;
	FormEmitMuutokset->naytaMemoOhje(naytaMuutosOhje);
	FormEmitMuutokset->Nayta(DKilp, Osuus);
	FormEmitMuutokset->NimiHaku = false;
	EmitMuutosFlag = true;
	if (FormEmitMuutokset->JoukkueHaku || FormEmitMuutokset->OsuusHaku)
		FormEmitMuutokset->ShowModal();
	else {
		FormEmitMuutokset->Show();
		if (FormEmitMuutokset->WindowState == wsMinimized)
			FormEmitMuutokset->WindowState = wsNormal;
		FormEmitMuutokset->BringToFront();
		}
	if (!initialisoitu) {
		int MLeft = Left+Width;
		if (MLeft > Screen->Width - FormEmitMuutokset->Width)
			MLeft = Screen->Width - FormEmitMuutokset->Width;
		FormEmitMuutokset->Left = MLeft;
		FormEmitMuutokset->Top = Top + 50 * Screen->PixelsPerInch / 96;
		initialisoitu = true;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::PalautaMuutokset(void)
{
	Kilp = Kilp_0;
	DKilp = DKilp_0;
	Em.kilpno = Kilp_0.kilpno;
	Nayta();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::PalautaEmMuutokset(void)
{
//	EmitGrid->Options >> goEditing >> goTabs;
	Em = Em_0;
	Nayta();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::SiirraKilpailijalle(int kno, int osuus)
{
	kilptietue kilp;

	int d = getpos(kno);
	if (d > 0) {
		kilp.getrec(d);
			{
			DKilp = d;
			Kilp = kilp;
			if (eiLiitetty) {
				if (Em.badge == 0 && Kilp.ostiet[osuus].badge[0] != 0)
					Em.badge = Kilp.ostiet[osuus].badge[0];
				else if (Em.badge == 0 && Em.badge0 != 0)
					Em.badge = haeVapaaKoodi(Em.badge0);
				}
			Kilp.ostiet[osuus].badge[0] = Em.badge;
			if (DKilp_0 > 0) {
				Kilp.ostiet[osuus].laina[0] = Kilp_0.ostiet[Osuus_0].laina[0];
				}
			else {
				Kilp_0 = Kilp;
				DKilp_0 = DKilp;
				Osuus_0 = Osuus;
				}
			Em.kilpno = kno;
			Em.osuus = osuus;
			Osuus = osuus;
			Nayta();
			}
		}
	if (d != DKilp)
		Application->MessageBoxW(L"Siirrolle on este", L"Este", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::TallennaMuutokset(void)
{
	int Vast = 2, kno = 0;
	int kilpVapaa = 0;
	int d, tm;
	kilptietue vkilp, kilp, *kilp0;
	bool vakantilta = false;

	if (eiLiitetty) {
		if (Em.badge != 0 && Em.badge == Kilp.ostiet[Osuus].badge[0] && Em.kilpno == Kilp.kilpno) {
			EnterCriticalSection(&tall_CriticalSection);
			tallenna(&Kilp, DKilp, 0, -1, 0, 0);
			Em.put(Tietue, false);
			LeaveCriticalSection(&tall_CriticalSection);
			addbadge(Em.badge, Kilp.kilpno, 0, 0, 0);
			setEmitJarr(Em.kilpno, Em.osuus, Tietue);
			eiLiitetty = false;
			EdtMessage->Text = L"";
			EdtMessage->Color = clBtnFace;
			}
		return;
		}
	if (DKilp_0 == DKilp && Osuus == Osuus_0) {
		Application->MessageBoxW(L"Ei tallennettavaa muutosta", L"", MB_OK);
		return;
		}
	if (EmitJarr(Kilp.kilpno, Osuus) >= 0) {
		Vast = select3(2, L"Kohdekilpailijalla jo leimaustiedot.", L"Valitse toimintatapa",
			L"Luovu siirrosta", L"Siirrä menettäen yhteys \r\n aiempiin leimaustietoihin",
			L"", this);
		if (Vast <= 1) {
			lueem(&Em, Tietue);
			Paivita(&Em);
			return;
			}
		}
	EdtKilpno->ReadOnly = true;
	CBOsuus->Enabled = false;
	EnterCriticalSection(&tall_CriticalSection);
	kilp.getrec(DKilp);
	if (kilp.Maali(Osuus, 0) == TMAALI0)
		kilpVapaa = 1;
	if (kilp.wTark(Osuus, false) == L'-' || kilp.wTark(Osuus, false) == L'E' || kilp.wTark(Osuus, false) == L'V')
		kilpVapaa |= 2;

	// Tallenna alkuperäinen badge[0] kakkospaikkaan
	if (!kilpparam.kaksibadge && Kilp.ostiet[Osuus].badge[1] == 0)
		Kilp.ostiet[Osuus].badge[1] = Kilp.ostiet[Osuus].badge[0];

	Kilp.ostiet[Osuus].badge[0] = Em.badge;
	if (DKilp_0 == DKilp)
		kilp0 = &Kilp;
	else
		kilp0 = &Kilp_0;

	// Palauta alkuperäinen badge[0]
	if (!kilpparam.kaksibadge)
		kilp0->ostiet[Osuus_0].badge[0] = kilp0->ostiet[Osuus_0].badge[1];
	else if (Em.badge == kilp0->ostiet[Osuus_0].badge[0])
		kilp0->ostiet[Osuus_0].badge[0] = Em.badge0;
	if (kilp0->Maali(Osuus_0, 0) != TMAALI0) {
		ratatp *rt;
		rt = getrata(kilp0->ostiet[Osuus_0].hajonta);
		if (rt && rt->maalilaji == 4) {
			tm = kilp0->Maali(Osuus_0, 0);
			kilp0->setMaali(Osuus_0, 0, TMAALI0);
			}
		}
	kilp0->SetTark(Osuus_0, L'-');
	// Poista koodit ja mahdollisesti tulos aiemmalta kilpailijalta
	tallkvaihto(kilp0->kilpno, Osuus_0, Kilp.kilpno, Osuus, 0);
	if (DKilp_0 != DKilp)
		tallenna(&Kilp_0, DKilp_0, -1, 0, 0, 0);
	// Lisää koodit uudelle kilpailijalle
	tallenna(&Kilp, DKilp, 0, -1, 0, 0);
	// Tee muutokset emit-tietueeseen ja taulukkoon emitjarr.
	// Tämä edellyttää, että vanha linkki on jäljellä taulukossa emitjarr
	// sekä että uudelle kilpailijalle ei ole linkkiä toiseen tietueeseen

	// tarkasta leimat ja määrää tulos leimoista, jos aikaa ja/tarkastustietoa ei ole
	if (kilpVapaa) {
		int tulkinta[51];
		if (kilpVapaa & 1)
			Kilp.setMaali(Osuus, 0, LaskeTulos());
		if (!kilpparam.partio && (kilpVapaa & 2)) {
			memset(tulkinta, 0, sizeof(tulkinta));
			if (tarkista(&Em, &Kilp, tulkinta, -1, 0) == 0)
				Kilp.SetTark(Osuus, L'T');
			else
				Kilp.SetTark(Osuus, L'I');
			}
		tallenna(&Kilp, DKilp, 0, -1, 0, 0);
		if (emittulparam && autorap > 9 && Em.kilpno == Kilp.kilpno &&
			(autorap > 1 || Kilp.wTark(Osuus, false) == L'I' || autorapsrj[Kilp.sarja]))
			emit_rap(emittulparam, Em.kilpno, Osuus);
//		if (emittulparam && autorap < 0 && Em.kilpno == Kilp.kilpno)
//			tulostaRastiVa(emittulparam, Em.kilpno, Osuus);
		if (emitanal_fl)
		   addemitanal(&Em, 1);
		LeaveCriticalSection(&tall_CriticalSection);
		}
	else {
		LeaveCriticalSection(&tall_CriticalSection);
		Application->MessageBoxW(L"Kilpailijan tulosta ja leimantarkastustietoa ei "
			L"muutettu vastaamaan siirrettyjä leimoja. Aiemmat tiedot säilytetty kilpailijalla",
			L"Varoitus", MB_OK);
		}
	Nayta();
	TallVals();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::MerkitseOhitetuksi(void)
{
	if (DKilp_0 > 0) {
		if (!kilpparam.kaksibadge)
			Kilp_0.ostiet[Osuus].badge[0] = Kilp_0.ostiet[Osuus].badge[1];
		else {
			if (Em.badge == Kilp_0.ostiet[Osuus].badge[0])
				Kilp_0.ostiet[Osuus].badge[0] = Em.badge0;
			else if (Em.badge == Kilp_0.ostiet[Osuus].badge[1])
				Kilp_0.ostiet[Osuus].badge[1] = Em.badge0;
			}
		if (Application->MessageBoxW(L"Poistetaanko osuuden tulos ja tarkastustieto?",
			L"Valitse", MB_YESNO) == IDYES) {
			Kilp_0.setMaali(Osuus, 0, TMAALI0);
			Kilp_0.SetTark(Osuus, L'-');
			}
		Em.kilpno = 0;
		Em.badge = 0;
		putem(&Em, Tietue, false);
		tallkvaihto(Kilp.kilpno, Osuus_0, 0, 0, 0);
		tallenna(&Kilp_0, DKilp_0, -1, 0, 0, 0);
		}
	else {
		Em.kilpno = 0;
		Em.badge = 0;
		putem(&Em, Tietue, false);
		setEmitJarr(Kilp.kilpno, Osuus, -1);
		}
	EdtKilpno->ReadOnly = true;
	CBOsuus->Enabled = false;
	Nayta();
	TallVals();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::TallennaVaihto(int d, int os)
{
	int kno, i_ptr;
	int vd;
	kilptietue vkilp, kilp;

	if (d == 0 || (DKilp_0 == d && os == Osuus)) {
		Application->MessageBoxW(L"Ei tallennettavaa vaihtoa", L"", MB_OK);
		return;
		}
	EdtKilpno->ReadOnly = true;
	CBOsuus->Enabled = false;
	if (DKilp_0 == d) {
		kilp.ostiet[os].badge[0] = Kilp_0.ostiet[Osuus].badge[0];
		kilp.ostiet[os].badge[1] = Kilp_0.ostiet[Osuus].badge[1];
		kilp.ostiet[os].laina[0] = Kilp_0.ostiet[Osuus].laina[0];
		kilp.ostiet[os].laina[1] = Kilp_0.ostiet[Osuus].laina[1];
		Kilp_0.ostiet[Osuus].badge[0] = Kilp_0.ostiet[os].badge[0];
		Kilp_0.ostiet[Osuus].badge[1] = Kilp_0.ostiet[os].badge[1];
		Kilp_0.ostiet[Osuus].laina[0] = Kilp_0.ostiet[os].laina[0];
		Kilp_0.ostiet[Osuus].laina[1] = Kilp_0.ostiet[os].laina[1];
		Kilp_0.ostiet[os].badge[0] = kilp.ostiet[os].badge[0];
		Kilp_0.ostiet[os].badge[1] = kilp.ostiet[os].badge[1];
		Kilp_0.ostiet[os].laina[0] = kilp.ostiet[os].laina[0];
		Kilp_0.ostiet[os].laina[1] = kilp.ostiet[os].laina[1];
		tallenna(&Kilp_0, DKilp_0, 0, -1, 0, 0);
		}
	else {
//		kno = haeSeurVakantti(0);
		vd = getpos(kno);
		kilp.getrec(d);
		vkilp.getrec(vd);
		vkilp.ostiet[os].badge[0] = kilp.ostiet[os].badge[0];
		vkilp.ostiet[os].badge[1] = kilp.ostiet[os].badge[1];
		vkilp.ostiet[os].laina[0] = kilp.ostiet[os].laina[0];
		vkilp.ostiet[os].laina[1] = kilp.ostiet[os].laina[1];
		tallenna(&vkilp, vd, 0, -1, 0, 0);
		kilp.ostiet[os].badge[0] = Kilp_0.ostiet[Osuus].badge[0];
		kilp.ostiet[os].badge[1] = Kilp_0.ostiet[Osuus].badge[1];
		kilp.ostiet[os].laina[0] = Kilp_0.ostiet[Osuus].laina[0];
		kilp.ostiet[os].laina[1] = Kilp_0.ostiet[Osuus].laina[1];
		tallenna(&kilp, d, 0, -1, 0, 0);
		Kilp_0.ostiet[Osuus].badge[0] = vkilp.ostiet[os].badge[0];
		Kilp_0.ostiet[Osuus].badge[1] = vkilp.ostiet[os].badge[1];
		Kilp_0.ostiet[Osuus].laina[0] = vkilp.ostiet[os].laina[0];
		Kilp_0.ostiet[Osuus].laina[1] = vkilp.ostiet[os].laina[1];
		tallenna(&Kilp_0, DKilp_0, 0, -1, 0, 0);
		vkilp.SetTark(os, L'V');
		vkilp.ostiet[os].badge[0] = 0;
		vkilp.ostiet[os].badge[1] = 0;
		vkilp.ostiet[os].laina[0] = 0;
		vkilp.ostiet[os].laina[1] = 0;
		tallenna(&vkilp, vd, 0, -1, 0, 0);
		}
	lueem(&Em, Tietue);
	DKilp = getpos(Em.kilpno);
	Kilp.getrec(DKilp);
	DKilp_0 = DKilp;
	Kilp_0 = Kilp;
	Osuus = os;
	Nayta();
	TallVals();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::TallennaEmMuutokset(void)
{
//	EmitGrid->Options >> goEditing >> goTabs;
	if (FormEmitMuutokset->CBLuenta->Checked)
		Em.time = wstrtoaika_vap(FormEmitMuutokset->EdtLuenta->Text.c_str(), t0) / KSEK;
	for (int r = 1; r < EmitGrid->RowCount; r++) {

		Em.ctrlcode[r-1] = _wtoi(Cells[1][r].text.c_str());
		Em.ctrltime[r-1] = _wtoi(Cells[3][r].text.c_str());
		}
	for (int r = EmitGrid->RowCount; r < 50; r++) {
		Em.ctrlcode[r-1] = 0;
		Em.ctrltime[r-1] = 0;
		}
	putem(&Em, Tietue, true);
	Nayta();
	TallVals();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::SalliMuutokset(bool salli)
{
	kirjaaEmitMuutokset = salli;
	Lisrivi1->Enabled = salli;
	Poistarivi1->Enabled = salli;
/*	if (salli)
		EmitGrid->Options << goEditing << goTabs;
	else
		EmitGrid->Options >> goEditing >> goTabs;
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::PuolitaKortinAjat(void)
{
	for (int i = 0; i < 50; i++)
	   Em.ctrltime[i] /= 2;
	Nayta();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::KasvataKortinAjat(int lisa)
{
	for (int i = 0; i < 50; i++)
	   if (Em.ctrlcode[i] != 0 && Em.ctrlcode[i] < 254)
		   Em.ctrltime[i] += lisa;
	Nayta();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EmitGridSelectCell(TObject *Sender, int ACol, int ARow,
		  bool &CanSelect)
{
//	if (EmitGrid->Options.Contains(goEditing) && aktrow > 0 && aktcol > 0 &&
	if (kirjaaEmitMuutokset && aktrow > 0 && aktcol > 0 &&
		(ACol != aktcol || ARow != aktrow)) {
		paivitaMuutos();
	}

	aktrow = ARow;
	aktcol = ACol;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EmitGridExit(TObject *Sender)
{
	if (kirjaaEmitMuutokset && aktrow > 0 && aktcol > 0) {
		paivitaMuutos();
	}

	aktrow = 0;
	aktcol = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::paivitaMuutos(void)
{
	if (kirjaaEmitMuutokset && aktrow > 0 && (aktcol == 1 || aktcol == 3)) {
		int k;

		k = _wtoi(Cells[aktcol][aktrow].text.c_str());
		Cells[aktcol][aktrow].text = k;
		if (aktcol == 1)
			Em.ctrlcode[aktrow-1] = k;
		else
			Em.ctrltime[aktrow-1] = k;
		Nayta();
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormEmit::EmitGridDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
		  TGridDrawState State)
{
	int FontSize = 9, w;

   Graphics::TBitmap *pBitmap = new Graphics::TBitmap();
	TRect Rect1;
   pBitmap->Width = Rect.Width();
	pBitmap->Height = Rect.Height();
   Rect1.Left = 0;
   Rect1.Top = 0;
   Rect1.Right = pBitmap->Width;
   Rect1.Bottom = pBitmap->Height;
   if (ARow == 0)
	  pBitmap->Canvas->Brush->Color = clBtnFace;
   else
	{
	  switch (Cells[ACol][ARow].style) {
		case 0:
			pBitmap->Canvas->Brush->Color = clWindow;
			pBitmap->Canvas->Font->Style = TFontStyles();
			break;
		case 1:
			pBitmap->Canvas->Brush->Color = TColor(0xd0d0d0);  //vaaleanharmaa
			break;
		case 2:
			pBitmap->Canvas->Brush->Color = clYellow;  //clYellow;
			break;
		case 3:
			pBitmap->Canvas->Brush->Color = TColor(0xc0ffc0);  //vihreä
			break;
		case 4:
			pBitmap->Canvas->Brush->Color = clLime;
			break;
		case 5:
			pBitmap->Canvas->Brush->Color = clFuchsia;
			break;
		case 6:
			pBitmap->Canvas->Brush->Color = clAqua;
			break;
		 }
	  }
   pBitmap->Canvas->FillRect(Rect1);
   pBitmap->Canvas->Font->Name = L"Arial";
   pBitmap->Canvas->Font->Size = FontSize;
   w = pBitmap->Canvas->TextWidth(Cells[ACol][ARow].text);
   if (w < pBitmap->Width - 3)
		w = pBitmap->Width - w - 2;
   else
		w = 1;
   pBitmap->Canvas->TextOut(w,1,Cells[ACol][ARow].text);
   EmitGrid->Canvas->Draw(Rect.Left, Rect.Top, pBitmap);
   delete pBitmap;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EmitGridSetEditText(TObject *Sender, int ACol, int ARow,
		  const UnicodeString Value)
{
	if (kirjaaEmitMuutokset) {
		if (ARow > 0) {
			if (ACol == 1) {
				Cells[ACol][ARow].text = Value;
				Em.ctrlcode[ARow-1] = (unsigned char) _wtoi(Value.c_str());
				}
			if (ACol == 3) {
				Cells[ACol][ARow].text = Value;
				Em.ctrltime[ARow-1] = (unsigned char) _wtoi(Value.c_str());
				}
			}
		}
	EmitGrid->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::CBNaytaLeimatClick(TObject *Sender)
{
	if (CBNaytaLeimat->Checked)
		Width = 702 * Screen->PixelsPerInch / 96;
	else
		Width = 25 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::BtnPerKirjausClick(TObject *Sender)
{
	EdtKilpno->ReadOnly = true;
	CBOsuus->Enabled = false;
	Kilp.getrec(DKilp);
	Kilp.ostiet[Osuus].badge[0] = 0;
	Kilp.SetTark(Osuus, L'E');
	tallenna(&Kilp, DKilp, 0, 0,0, 0);
	Kilp.nollaa();
	Nayta();
	TallVals();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::Tulosta(int tuloste)
{
	if (wcslen(EmitKirjoitin) == 0) {
		FormKirjVal = new TFormKirjVal(this);
		FormKirjVal->FormShow(this);
		wcsncpy(EmitKirjoitin,
			FormKirjVal->KirjoitinVal->Items->Strings[FormKirjVal->KirjoitinVal->ItemIndex].c_str(), 198);
		delete FormKirjVal;
		}
	EmitTulPrm.printer = GDIPRINTER;
	EmitTulPrm.language = lsttulparam.language;
	if (!EmitTulPrm.lstf) {
		if ((EmitTulPrm.lstf = openprfile(EmitKirjoitin, 1, TRUE, FALSE, 0, FALSE))
			== NULL) {
			Application->MessageBoxW(L"Kirjoittimen käyttö ei onnistu", L"Virhe", MB_OK);
			return;
			}
		if (EmitTulPrm.lstf && EmitTulPrm.lstf->prno == 2)
			initgdiprinter(&EmitTulPrm);
		}
	switch (tuloste) {
		case 1:
			emit_rap(&EmitTulPrm, Em.kilpno, Osuus);
			break;
		case 2:
			tark_rap(&EmitTulPrm, Em.kilpno, Osuus);
			break;
		case 3:
			emitlista(&EmitTulPrm);
			break;
		case 4:
			tulostaRastiVa(&EmitTulPrm, Em.kilpno, Osuus);
			break;
		}
	closeprfile(EmitTulPrm.lstf);
	EmitTulPrm.lstf = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Valitsekirjoitin1Click(TObject *Sender)
{
	FormKirjVal = new TFormKirjVal(this);
	FormKirjVal->ShowModal();
	wcsncpy(EmitKirjoitin,
		FormKirjVal->KirjoitinVal->Items->Strings[FormKirjVal->KirjoitinVal->ItemIndex].c_str(), 198);
	delete FormKirjVal;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Kortintiedot1Click(TObject *Sender)
{
	Tulosta(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtSakkojaChange(TObject *Sender)
{
	int sk, ero;
	wchar_t st[20];

	if (Sarjat[Kilp.sarja].tsak[Osuus] == 0)
		return;
//	LaskeTulos();
	sk = _wtoi(EdtSakkoja->Text.c_str());
	ero = SEK*sk*Sarjat[Kilp.sarja].tsak[Osuus] - Kilp.ostiet[Osuus].sakko;
	if (Kilp.Tulos(Osuus, 0)) {
		AIKATOWSTRS(st, Kilp.Tulos(Osuus, 0)+ero, 0);
		st[kilpparam.laika2] = 0;
		EdtTulos->Text = st;
		}
	OnkoMuutoksia();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Automaattinentulostus1Click(TObject *Sender)
{
	int vast;

	vast = select3(3, L"Hylkäysesitystietojen automaattinen tulostus", L"Tulostus",
		L"Paikallisesti luetut kortit", L"Tiedonsiirron tuomat", L"Ei tulosteta", this);
	if (vast < 3) {
		if (!emittulparam) {
			emittulparam = &lsttulparam;
			}
		if (!emittulparam->lstf) {
			if (wcslen(EmitKirjoitin) == 0) {
				FormKirjVal = new TFormKirjVal(this);
				FormKirjVal->FormShow(this);
				wcsncpy(EmitKirjoitin,
					FormKirjVal->KirjoitinVal->Items->Strings[FormKirjVal->KirjoitinVal->ItemIndex].c_str(), 198);
				delete FormKirjVal;
			}
			lsttulparam.printer = GDIPRINTER;
			if (!lsttulparam.lstf) {
				if ((lsttulparam.lstf = openprfile(EmitKirjoitin, 1, TRUE, FALSE, 0, FALSE))
					== NULL) {
					Application->MessageBoxW(L"Kirjoittimen käyttö ei onnistu", L"Virhe", MB_OK);
					vast = 3;
					}
				if (lsttulparam.lstf && lsttulparam.lstf->prno == 2)
					initgdiprinter(&lsttulparam);
				}
			}
		}
	comautorap = (vast == 2) ? 1 : 0;
	autorap = (vast < 3) ? 1 : 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::CBHaeArvoClick(TObject *Sender)
{
	if (CBHaeArvo->Checked)
		EdtHakuArvoChange(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormEmit::Vliaikatuloste1Click(TObject *Sender)
{
	Tulosta(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::BtnVaTulosteClick(TObject *Sender)
{
	Tulosta(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::BtnLeimRapClick(TObject *Sender)
{
	Kortintiedot1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Automaattinenvliaikatuloste1Click(TObject *Sender)
{
	int vast;

	vast = select3(3, L"Väliaikatulosteiden automaattinen tulostus", L"Tulostus",
		L"Paikallisesti luetuille", L"Tiedonsiirron tuomille", L"Ei tulosteta", this);
	if (vast < 3) {
		if (!emittulparam)
			emittulparam = &lsttulparam;
		if (!emittulparam->lstf) {
			if (wcslen(EmitKirjoitin) == 0) {
				FormKirjVal = new TFormKirjVal(this);
				FormKirjVal->FormShow(this);
				wcsncpy(EmitKirjoitin,
					FormKirjVal->KirjoitinVal->Items->Strings[FormKirjVal->KirjoitinVal->ItemIndex].c_str(), 198);
				delete FormKirjVal;
			}
			lsttulparam.printer = GDIPRINTER;
			if (!lsttulparam.lstf) {
				if ((lsttulparam.lstf = openprfile(EmitKirjoitin, 1, TRUE, FALSE, 0, FALSE))
					== NULL) {
					Application->MessageBoxW(L"Kirjoittimen käyttö ei onnistu", L"Virhe", MB_OK);
					vast = 3;
					}
				if (lsttulparam.lstf && lsttulparam.lstf->prno == 2)
					initgdiprinter(&lsttulparam);
				}
			}
		}
	comautorap = (vast == 2) ? -1 : 0;
	autorap = (vast < 3) ? -1 : 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Lueratatiedotuudelleen1Click(TObject *Sender)
{
	lueRatatiedotUudelleen();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Vaadikaksoisluenta1Click(TObject *Sender)
{
	Vaadikaksoisluenta1->Checked = !Vaadikaksoisluenta1->Checked;
	lukutoisto = Vaadikaksoisluenta1->Checked ? 2 : 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EmitGridMouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (EmitGrid->RowCount - EmitGrid->FixedRows >= EmitGrid->VisibleRowCount + EmitGrid->TopRow)
		EmitGrid->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EmitGridMouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (EmitGrid->TopRow > EmitGrid->FixedRows)
		EmitGrid->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Etsileimojavastaavarata1Click(TObject *Sender)
{
	int rr, vast;
	wchar_t st[32];

	rr = etsiesarja(&Em);
	if (rr < 0) {
		Application->MessageBoxW(L"Leimat eivät vastaa mitää rataa", L"Ei löydy", MB_OK);
		}
	else {
		wchar_t msg[200];
		swprintf(msg, L"Leimat vastaavat rataa %s", ansitowcs(st, rata[rr].rata, 30));
		vast = select3(2, msg, L"Löytyi", L"Vaihda rata kilpailijalle", L"Jätä rata ennalleen", L"", this);
		if (vast == 1) {
			CRata->ItemIndex = rr+1;
			CRataChange(Sender);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::MTRlaitteenohjaus1Click(TObject *Sender)
{
	if (!FormMTR)
		FormMTR = new TFormMTR(this);
	FormMTR->Show();
	if (FormMTR->WindowState == wsMinimized)
		FormMTR->WindowState = wsNormal;
	FormMTR->BringToFront();
//	delete FormMTR;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::emiTagluennanohjaus1Click(TObject *Sender)
{
	if (!FormEmiTag)
		FormEmiTag = new TFormEmiTag(this);
	FormEmiTag->Show();
	if (FormEmiTag->WindowState == wsMinimized)
		FormEmiTag->WindowState = wsNormal;
	FormEmiTag->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Maalintietokone1Click(TObject *Sender)
{
	Maalintietokone1->Checked = !Maalintietokone1->Checked;
	if (Maalintietokone1->Checked && Lhdntietokone1->Checked) {
		Lhdntietokone1->Checked = false;
		InitTila();
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::Lhdntietokone1Click(TObject *Sender)
{
	Lhdntietokone1->Checked = !Lhdntietokone1->Checked;
	if (Lhdntietokone1->Checked) {
		Maalintietokone1->Checked = false;
		}
	InitTila();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Merkitsekortitlainakorteiksi1Click(TObject *Sender)
{
	Merkitsekortitlainakorteiksi1->Checked = !Merkitsekortitlainakorteiksi1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Avaadebugtiedosto1Click(TObject *Sender)
{
#ifdef DBGFILE
	opendbgfile();
#endif
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Suljedebugtiedosto1Click(TObject *Sender)
{
#ifdef DBGFILE
	closedbgfile();
#endif
}
//---------------------------------------------------------------------------


void __fastcall TFormEmit::FormShow(TObject *Sender)
{
	if (Tietue <= 0) {
		HaeTietue(emithead2-1, 0);
		if (DKilp) {
			Kilp.getrec(DKilp);
			Nayta();
			TallVals();
			}
		}
	if (!FormEmitMuutokset)
		FormEmitMuutokset = new TFormEmitMuutokset(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::CBOsuusChange(TObject *Sender)
{
	if (Osuus != CBOsuus->ItemIndex - 1) {
		if (Osuus >= 0)
			Kilp.ostiet[Osuus] = Kilp_0.ostiet[Osuus];
		Osuus = CBOsuus->ItemIndex - 1;
		}
	Nayta();
	TallVals();
}
//---------------------------------------------------------------------------


void __fastcall TFormEmit::EdtMaaliKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		Key = 0;
		tallennaKilpailija(true);
		FocusControl(EdtBadge);
		return;
		}
	if (Key == VK_RETURN) {
//		LaskeTulos();
		OnkoMuutoksia();
		if (BtnTallenna->Visible)
			FocusControl(BtnTallenna);
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormEmit::BtnNaytaKarttaClick(TObject *Sender)
{
	ratatp *rt;
	int tulkinta[50];
	wchar_t st[60], st1[60];

	if (!DKilp || (rt = haerata(&Kilp, Osuus)) == NULL)
		return;
	if (!FormKartta) {
		if (!kilpparam.kartta[0]) {
			OpenDialog1->DefaultExt = "bmp";
			OpenDialog1->Filter = "Kuvatiedostot (*.bmp) |*.bmp|Kaikki tiedostot (*.*)|*.*";
			if (!OpenDialog1->Execute() || OpenDialog1->FileName.Length() < 5)
				return;
			wcsncpy(kilpparam.kartta, OpenDialog1->FileName.c_str(), sizeof(kilpparam.kartta)/2-1);
			}
		FormKartta = new TFormKartta(this);
		}
	if (!FormKartta->KarttaLadattu() && kilpparam.kartta[0])
		FormKartta->Lataakartta(kilpparam.kartta);
	if (!FormKartta->KarttaLadattu()) {
		Application->MessageBoxW(L"Kartan lataaminen ei onnistu", L"Este", MB_OK);
		return;
		}
	FormKartta->Show();
	if (FormKartta->WindowState == wsMinimized)
		FormKartta->WindowState = wsNormal;
	FormKartta->BringToFront();
	memset(tulkinta, 0, sizeof(tulkinta));
	if (Em.kilpno == Kilp.kilpno && Em.osuus == Osuus) {
		tarkista(&Em, &Kilp, tulkinta, -1, 0);
		FormKartta->naytaLeimat(rt, tulkinta, Em.ctrlcode);
		}
	else
		FormKartta->naytaLeimat(rt, 0, 0);
	FormKartta->NaytaOtsikko(UnicodeString(Kilp.kilpno)+L"-"+
		UnicodeString(Osuus+1)+L" - "+Kilp.Joukkue(st, 40)+L" - "+Kilp.Nimi(st1, 40, Osuus, 1)+L" - Rata: "+rt->tunnus);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Suljekaavake1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Lhetnytetytleimattiedonsiirtoon1Click(TObject *Sender)

{
	laheta_em(&Em, 0, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Lisrivi1Click(TObject *Sender)
{
	if (EmitGrid->Row < 1)
		return;
	for (int r = 49; r >= EmitGrid->Row; r--) {
		Em.ctrlcode[r] = Em.ctrlcode[r-1];
		Em.ctrltime[r] = Em.ctrltime[r-1];
		}
	Em.ctrlcode[EmitGrid->Row-1] = 0;
	Em.ctrltime[EmitGrid->Row-1] = 0;
	Nayta();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Poistarivi1Click(TObject *Sender)
{
	if (EmitGrid->Row < 1)
		return;
	for (int r = EmitGrid->Row-1; r < 49; r++) {
		Em.ctrlcode[r] = Em.ctrlcode[r+1];
		Em.ctrltime[r] = Em.ctrltime[r+1];
		}
	Em.ctrlcode[49] = 0;
	Em.ctrltime[49] = 0;
	Nayta();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Mrluentaajatmallinajoista1Click(TObject *Sender)
{
	for (int ep = 0; ep < emithead2; ep++) {
		emittp Em;
		kilptietue kilp;
		int tm = 0, tvr, nc = 0, lk, n_lk = 0, tulkinta[51];

		lueem(&Em, ep);
		if (Em.badge > 0 && Em.kilpno > 0) {
			kilp.getrec(getpos(Em.kilpno));
			if (kilp.kilpstatus == 0 && Em.badge == kilp.ostiet[Em.osuus].badge[0]) {
				tvr = TMAALI0;
				lk = haelukija(&Em);
				memset(tulkinta, 0, sizeof(tulkinta));
				if (tarkista(&Em, &kilp, tulkinta, lk, 0) < 0)
					continue;

				if (lk >= 0 && lk < 49) {
					tm = Em.ctrltime[lk];
					n_lk = tulkinta[lk];
					}
				for (nc = 49; nc > 1; nc--)
					if (Em.ctrlcode[nc] != 0 || Em.ctrltime[nc] != 0)
						break;
				nc++;
				for (int i = 0; i < nc; i++) {
					if (tulkinta[i] == (n_lk - 1)) {
						tvr = SEK*(tm - Em.ctrltime[i]);
						break;
						}
					}
				if (tvr != TMAALI0 && kilp.Maali(Em.osuus, 0) != TMAALI0) {
					Em.time = (kilp.Maali(Em.osuus, 0)+tvr)/KSEK;
					putem(&Em, ep, false);
					}
				}
			}
		}
	Nayta();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::oimimatonkortti1Click(TObject *Sender)
{
	//
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::BtnPiikitClick(TObject *Sender)
{
	if (!FormPiikit) {
		FormPiikit = new TFormPiikit(this);
		}
	FormPiikit->KilpNo = Kilp.kilpno;
	FormPiikit->Osuus = Osuus;
	FormPiikit->Show();
	if (FormPiikit->WindowState == wsMinimized)
		FormPiikit->WindowState = wsNormal;
	FormPiikit->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Luemysviallisetkortit1Click(TObject *Sender)
{
	Application->MessageBoxW(L"Viallisten korttien hyväksyminen voi johtaa virheisiin "
		L"myös muiden korttien lukemisessa. Käytä tätä valintaa vain tilapäisesti yrityksenä "
		L"lukea tiedot, jotka eivät muuten tallennu", L"Varoitus", MB_OK);
	Luemysviallisetkortit1->Checked = !Luemysviallisetkortit1->Checked;
	sallisummavirhe = Luemysviallisetkortit1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Luetietojatiedostosta1Click(TObject *Sender)
{
	bool viim;

	viim = CBViim->Checked;
	CBViim->Checked = false;
	FormEmitFile = new TFormEmitFile(this);
	FormEmitFile->ShowModal();
	delete FormEmitFile;
	CBViim->Checked = viim;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Onlineaikojenlaskentarastivliajoista1Click(TObject *Sender)

{
	if (!FormEmit_VA)
		FormEmit_VA = new TFormEmit_VA(this);

	FormEmit_VA->Show();
	if (FormEmit_VA->WindowState == wsMinimized)
		FormEmit_VA->WindowState = wsNormal;
	FormEmit_VA->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::arkastuksentuloskaavake1Click(TObject *Sender)
{
	arkastuksentuloskaavake1->Checked = !arkastuksentuloskaavake1->Checked;
	if (arkastuksentuloskaavake1->Checked) {
		if (!FormNaytaTulos)
			FormNaytaTulos = new TFormNaytaTulos(this);
		FormNaytaTulos->Show();
		if (FormNaytaTulos->WindowState == wsMinimized)
			FormNaytaTulos->WindowState = wsNormal;
		FormNaytaTulos->BringToFront();
		}
	else
		FormNaytaTulos->Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Rastikoodit1Click(TObject *Sender)
{
	Rastikoodit1->Checked = !Rastikoodit1->Checked;
	NaytaEmit();
}
//---------------------------------------------------------------------------


void __fastcall TFormEmit::HylkRapClick(TObject *Sender)
{
	if (!FormHylkRap)
		FormHylkRap = new TFormHylkRap(this);

	FormHylkRap->NaytaKilpailija(Kilp.kilpno, Osuus, Em.badge0);
	FormHylkRap->Show();
	if (FormHylkRap->WindowState == wsMinimized)
		FormHylkRap->WindowState = wsNormal;
	FormHylkRap->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Sail99varoitusClick(TObject *Sender)

{
	Sail99varoitus->Checked = !Sail99varoitus->Checked;
}
//---------------------------------------------------------------------------

