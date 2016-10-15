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
#include <io.h>

#include "UnitEmit.h"
#include "UnitTKanta.h"
#include "UnitSeurat.h"
#include "UnitEmitMuutokset.h"
#include "UnitKirjoitinVal.h"
#include "UnitMTR.h"
#include "UnitEmiTag.h"
#include "UnitKartta.h"
#include "UnitPiikit.h"
#include "UnitLisaTietue.h"
#include "UnitEmitFile.h"
#include "UnitEmit_VA.h"
#include "UnitNaytaTulos.h"
#include "HkMuotoilu.h"
#include "TpLaitteet.h"
#include "UnitHylkRap.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern TFormEmit *FormEmit;

static wchar_t TarkKoodit[] = L"--TIKHEVPMXB";

//---------------------------------------------------------------------------
__fastcall TFormEmit::TFormEmit(TComponent* Owner)
   : TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
}
//---------------------------------------------------------------------------
__fastcall TFormEmit::~TFormEmit(void)
{
	ProsEmitOn = false;
	Sleep(600);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::FormCreate(TObject *Sender)
{
	Kilp.nollaa();
	Tietue = 0;
	Em.nollaa();
	kuntoMaali = KuntoMaali > 0;
	kuntoLahto = KuntoMaali < 0;
	CBSelSN->ItemIndex = 0;
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
	LblPari->Visible = kilpparam.kaksibadge == 2;
	EdtTark1->Visible = kilpparam.kaksibadge == 2;
	EdtTark2->Visible = kilpparam.kaksibadge == 2;
	if (kilpparam.kaksibadge == 2) {
		LblOrigBadge->Caption = L"Toinen";
		EdtOrigBadge->Font->Style << fsBold;
		EdtOrigBadge->Font->Size = 11;
		}
	EdtBadge->Color = clVaaleanHarmaa;
	EdtKilpno->Color = clVaaleanHarmaa;
	EdtKilpno->Font->Color = clBlack;
	for (int i = 0; i < NREGNLY; i++) {
		if (regnly[i] == LID_MTR)
			MTRlaitteenohjaus1->Visible = true;
		if (regnly[i] == LID_EMITAG)
			emiTagluennanohjaus1->Visible = true;
		}
#ifdef DBGFILE
	if (dbgtofile)
		{
		Avaadebugtiedosto1->Visible = true;
		Suljedebugtiedosto1->Visible = true;
		}
#endif

	Vaadikaksoisluenta1->Checked = lukutoisto;
	Maalintietokone1->Checked = kuntoMaali;
	Label10->Visible = true;
	CRata->Visible = true;
	EdtSeura->Visible = Seura1->Checked;
	LblSeura->Visible = EdtSeura->Visible;
	EdtOrigBadge->Visible = Kortinkoodi1->Checked;
	LblOrigBadge->Visible = EdtOrigBadge->Visible;
	BtnEiBadge->Visible = (kuntosuunnmoodi == 1);
	Vahvistaaina1->Checked = vahvistaaina;
	Maalintietokone1->Visible = kuntosuunnmoodi == 1;
	Lhdntietokone1->Visible = kuntosuunnmoodi == 1;
	Emitvaihdottuleville->Visible = k_pv < kilpparam.n_pv_akt - 1;
	NaytaMemo(0);
	CSarja->Items->Clear();
	CSarja->Items->Add(L"Valitse sarja");
	for (int i = 0; i < sarjaluku; i++) {
		CSarja->Items->Add(UnicodeString(Sarjat[i].sarjanimi));
		}
	CSarja->ItemIndex = 0;
	CRata->Items->Clear();
	CRata->Items->Add(L"Sarjan mukaan");
	for (ratatp *rt = rata; rt < rata+nrata; rt++)
		CRata->Items->Add(rt->tunnus);
	CRata->ItemIndex = 0;
	CBLue->Checked = emitluenta;
	if (!emitluenta) {
		CBLue->Visible = false;
		}
	Panel1->Top = ClientHeight - Panel1->Height - 4;
	EdHeight = Height;
	CBSelSN->Visible = Salanimienkasittely1->Checked;
	CBSelSN->ItemIndex = 0;
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
	IkkParam.NaytaSeura = Seura1->Checked;
	IkkParam.NaytaKortinKoodi = Kortinkoodi1->Checked;
	IkkParam.HenkTietokanta = Henkiltietokanta1->Checked;
	IkkParam.Seuraluettelo = Seuraluettelo1->Checked;
	IkkParam.VahvIlmAina = Vahvistaaina1->Checked;
	if (Maalintietokone1->Checked)
		IkkParam.KuntoKone = 1;
	else if (Lhdntietokone1->Checked)
		IkkParam.KuntoKone = 2;
	else
		IkkParam.KuntoKone = 0;
	IkkParam.MerkLainat = Merkitsekortitlainakorteiksi1->Checked;
	IkkParam.TietokantaSarjat = Kythenkiltietokannansarjaa1->Checked;
	IkkParam.VaadiToisto = Vaadikaksoisluenta1->Checked;
	IkkParam.LueVialliset = Luemysviallisetkortit1->Checked;
	IkkParam.EmitvaihtoVaiheille = Emitvaihdottuleville->Checked;
}
//---------------------------------------------------------------------------

int TFormEmit::applyParams(void)
{
	Seura1->Checked = IkkParam.NaytaSeura;
	Kortinkoodi1->Checked = IkkParam.NaytaKortinKoodi;
	Henkiltietokanta1->Checked = IkkParam.HenkTietokanta;
	Seuraluettelo1->Checked = IkkParam.Seuraluettelo;
	Vahvistaaina1->Checked = IkkParam.VahvIlmAina;
	Maalintietokone1->Checked = IkkParam.KuntoKone == 1;
	kuntoMaali = IkkParam.KuntoKone == 1;
	Lhdntietokone1->Checked = IkkParam.KuntoKone == 2;
	kuntoLahto = IkkParam.KuntoKone == 2;
	Merkitsekortitlainakorteiksi1->Checked = IkkParam.MerkLainat;
	Kythenkiltietokannansarjaa1->Checked = IkkParam.TietokantaSarjat;
	Vaadikaksoisluenta1->Checked = IkkParam.VaadiToisto;
	Luemysviallisetkortit1->Checked = IkkParam.LueVialliset;
	Emitvaihdottuleville->Checked = IkkParam.EmitvaihtoVaiheille;
	return(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::InitTila(void)
{
	ratatp *rt;

	if (esiluenta || kuntoLahto) {
		Width = 265 * Screen->PixelsPerInch / 96;
		EdtKilpno->Enabled = true;
		EdtKilpno->ReadOnly = false;
		EdtKilpno->Color = clWindow;
		EdtKilpno->TabStop = true;
		LblOrigBadge->Visible = false;
		EdtOrigBadge->Visible = false;
		BtnMuutokset->Visible = false;
		BtnLasketulos->Visible = false;
		Label10->Visible = false;
		CRata->Visible = false;
		CBLue->Enabled = false;
		CBViim->Enabled = false;
		CBLue->Checked = true;
		CBViim->Checked = true;
		BtnLeimRap->TabStop = false;
		BtnPiikit->TabStop = false;
		BtnNaytaKartta->TabStop = false;
		BtnVaTuloste->TabStop = false;
		BtnTyhjSanoma->TabStop = false;
		EdtLahto->Top = TarkVal->Top;
		Label5->Top = Label6->Top;
		Label6->Top = Label10->Top;
		TarkVal->Top = CRata->Top;
		if (kuntoLahto) {
			LblToimi->Visible = true;
			Label6->Visible = false;
			TarkVal->Visible = false;
			}
		Label3->Visible = false;
		EdtTulos->Visible = false;
		EdtSelitys->Visible = Salanimienkasittely1->Checked;
		CBSelSN->Top = CRata->Top;
		EdtSelitys->Top = CRata->Top;
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
		BtnOhita->Visible = true;
		if (esiluenta == 2) {
			Caption = L"Väliaikatulosteet";
			EdtBdgMsg->Text = L"Lue kortti tai syötä KilpNo";
			Height = 350 * Screen->PixelsPerInch / 96;
			BtnOhita->Visible = false;
			}
		else if (esiluenta) {
			Merkitsekortitlainakorteiksi1->Visible = true;
			Height = 620 * Screen->PixelsPerInch / 96;
			}
		else {
			Height = 447 * Screen->PixelsPerInch / 96;
			BtnHaeOo->Top = ClientHeight - BtnHaeOo->Height -
				6 * Screen->PixelsPerInch /96;
			ProgressBar1->Top = BtnHaeOo->Top;
			ProgressBar1->Left = 8 * Screen->PixelsPerInch / 96;
			Label17->Visible = true;
			Label17->Top = ProgressBar1->Top - 14 * Screen->PixelsPerInch /96;
			}
		}
	else {
		Width = 710 * Screen->PixelsPerInch / 96;
		EdtKilpno->Enabled = true;
		EdtKilpno->ReadOnly = true;
		EdtKilpno->Color = clVaaleanHarmaa;
		EdtKilpno->TabStop = false;
		LblToimi->Visible = false;
		BtnMuutokset->Visible = true;
		BtnLasketulos->Visible = true;
		BtnOhita->Visible = false;
		Label10->Visible = monirata;
		CBLue->Enabled = true;
		CBViim->Enabled = true;
		CRata->Visible = monirata;
		BtnLeimRap->TabStop = true;
		BtnPiikit->TabStop = true;
		BtnNaytaKartta->TabStop = true;
		BtnVaTuloste->TabStop = true;
		BtnTyhjSanoma->TabStop = true;
		EdtLahto->Top = 383  * Screen->PixelsPerInch / 96;
		Label5->Top = 387 * Screen->PixelsPerInch / 96;
		Label6->Top = 251 * Screen->PixelsPerInch / 96;
		TarkVal->Top = 247 * Screen->PixelsPerInch / 96;
		ProgressBar1->Top = 29 * Screen->PixelsPerInch / 96;
		ProgressBar1->Left = 380 * Screen->PixelsPerInch / 96;
		Label17->Visible = false;
		Label6->Visible = true;
		TarkVal->Visible = true;
		Label3->Visible = true;
		EdtTulos->Visible = true;
		CBSelSN->Visible = Salanimienkasittely1->Checked && kilpparam.kaksibadge != 2;
		EdtSelitys->Visible = kilpparam.kaksibadge != 2;
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
		BtnHaeOo->Top = BtnMuutokset->Top + BtnMuutokset->Height +
			6 * Screen->PixelsPerInch /96;
		}
	Label8->Visible = (EdtSelitys->Visible && !Salanimienkasittely1->Checked);
	if (!CBSelSN->Visible)
		CBSelSN->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::NaytaMemo(int flag)
{
	MemoKno->Visible = (flag & 2);
	MemoEmit->Visible = (flag & 1);
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::TallVals(void)
{
	ValSNimi = EdtSNimi->Text;
	ValENimi = EdtENimi->Text;
	ValSeura = EdtSeura->Text;
	ValTulos = EdtTulos->Text;
	ValMaali = EdtMaali->Text;
	ValLahto = EdtLahto->Text;
	ValSelitys = EdtSelitys->Text;
	ValTarkVal = TarkVal->ItemIndex;
	if (monirata)
		ValRata = CRata->ItemIndex;
	ValSarja = CSarja->ItemIndex;
	ValSalaa = CBSelSN->ItemIndex;
	ValLisno = EdtLisno->Text;
	ValJoukkue = EdtJoukkue->Text;
	BtnTallenna->Visible = false;
	BtnPeruuta->Visible = false;
	ValHSeura = HSeura;
	ValHSeuralyh = HSeuralyh;
	if (kilpparam.kaksibadge == 2) {
		ValTark[0] = EdtTark1->Text.c_str()[0];
		ValTark[1] = EdtTark2->Text.c_str()[0];
		}
	Tietue_0 = Tietue;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::PeruutaMuutokset(void)
{
	if (DKilp > 0)
		Kilp.GETREC(DKilp);
	EdtSNimi->Text = ValSNimi;
	EdtENimi->Text = ValENimi;
	EdtSeura->Text = ValSeura;
	EdtTulos->Text = ValTulos;
	EdtMaali->Text = ValMaali;
	EdtLahto->Text = ValLahto;
	EdtSelitys->Text = ValSelitys;
	TarkVal->ItemIndex = ValTarkVal;
	if (monirata)
		CRata->ItemIndex = ValRata;
	CSarja->ItemIndex = ValSarja;
	CBSelSN->ItemIndex = ValSalaa;
	EdtLisno->Text = ValLisno;
	EdtJoukkue->Text = ValJoukkue;
	HSeura = ValHSeura;
	HSeuralyh = ValHSeuralyh;
	if (kilpparam.kaksibadge == 2) {
		EdtTark1->Text = ValTark[0];
		EdtTark2->Text = ValTark[1];
		}
	Tietue = Tietue_0;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::NaytaKilp(void)
{
	wchar_t st[60];

	EdtKilpno->Text = UnicodeString(Kilp.id());
//	if (UusiTietue || !esiluenta)
		EdtBadge->Text = UnicodeString(Kilp.pv[k_pv].badge[0]);
	EdtOrigBadge->Text = UnicodeString(Kilp.pv[k_pv].badge[1]);
	LblLaina->Visible = lainakortti(&Kilp);
	EdtLisno->Text = UnicodeString(Kilp.lisno[0]);
	EdtSNimi->Text = UnicodeString(Kilp.sukunimi);
	EdtENimi->Text = UnicodeString(Kilp.etunimi);
	EdtSeura->Text = UnicodeString(Kilp.seuratieto(0));
	EdtJoukkue->Text = UnicodeString(Kilp.joukkue);
	if (kilpparam.kaksibadge == 2) {
		EdtTark1->Text = Kilp.pv[k_pv].asakot[0];
		EdtTark2->Text = Kilp.pv[k_pv].asakot[1];
		}
	CSarja->ItemIndex = Kilp.Sarja()+1;
	if (monirata) {
		int rr;
		rr = onrata(Kilp.pv[k_pv].rata);
		CRata->ItemIndex = rr < 0 ? 0 : rr;
		}
	if (!Salanimienkasittely1->Checked)
		EdtSelitys->Text = UnicodeString(Kilp.pv[k_pv].selitys);
	else {
		CBSelSN->ItemIndex = (Kilp.pv[0].salaa == 'Y') ? 1 : 0;
		EdtSelitys->Text = UnicodeString(Kilp.yhdistys);
		}
	TarkVal->ItemIndex = wcswcind(Kilp.tark(k_pv), TarkKoodit+1);
	EdtSakkoja->Enabled = (Sarjat[Kilp.Sarja()].rrtas == 2);
	if (Sarjat[Kilp.Sarja()].tsak[k_pv])
		EdtSakkoja->Text = UnicodeString(Kilp.pv[k_pv].tasoitus/Sarjat[Kilp.Sarja()].tsak[k_pv]);
	AIKATOWSTRS(st, Kilp.p_aika(0), 0);
	st[pvparam[k_pv].laika] = 0;
	EdtTulos->Text = st;
	if (Kilp.pv[k_pv].tlahto != TMAALI0) {
		AIKATOWSTRS(st, Kilp.pv[k_pv].tlahto, t0);
		st[8] = 0;
		EdtLahto->Text = st;
		}
	else
		EdtLahto->Text = L"";
	if (Kilp.maali(0) != TMAALI0) {
		AIKATOWSTRS(st, Kilp.maali(0), t0);
		st[pvparam[k_pv].laika] = 0;
		EdtMaali->Text = st;
		}
	else
		EdtMaali->Text = L"";
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::NaytaEmit(void)
{
	wchar_t st[60];
	int tm = 0, nc = 0, lk = -1, n_lk = 0, enn, virhe = 0, tulkinta[MAXNLEIMA+1], style = 0;
	UnicodeString AS;
	ratatp *rt;

	if (esiluenta)
		return;
	TViimRasti = TMAALI0;
	EmitGrid->Color = clWindow;
	EmitGrid->RowCount = MAXNLEIMA+1;
	EdtLuettu->Clear();
	EdtEro->Clear();
	if (kilpparam.kaksibadge == 2) {
		LblPari->Caption = UnicodeString(Em.osuus + 1);
		EdtBadge->Color = Em.osuus ? clBtnFace : clYellow;
		EdtOrigBadge->Color = Em.osuus ? clYellow : clBtnFace;
		}
	if (_wtoi(EdtBadge->Text.c_str()) == 0)
		EdtBadge->Text = Em.badge;
	if (_wtoi(EdtOrigBadge->Text.c_str()) == 0)
		EdtOrigBadge->Text = Em.badge0;
	memset(tulkinta, 0, sizeof(tulkinta));
	if (Kilp.id() > 0 && Kilp.id() == Em.kilpno && Kilp.Badge(k_pv, Em.osuus) == Em.badge) {
		if (Em.time != TMAALI0) {
			aikatowstr_ls(st, Em.time, t0);
			st[pvparam[k_pv].laika] = 0;
			EdtLuettu->Text = st;
			}
		virhe = tarkista(&Em, tulkinta, 0, &Kilp);
		enn = haeennakko(&Em, &Kilp);
		lk = haelukija(&Em);
		if (lk >= 0 && lk < MAXNLEIMA-1) {
			tm = Em.ctrltime[lk];
			n_lk = tulkinta[lk];
			}
		}
	for (nc = MAXNLEIMA-1; nc > 0; nc--)
		if (Em.ctrlcode[nc] != 0 || Em.ctrltime[nc] != 0)
			break;
	if (lk < 0 && nc > 0) {
		if (Em.ctrlcode[nc] == 254)
			lk = nc-1;
		else
			lk = nc;
		}
	if (nc > 0)
		nc++;
//		EmitGrid->RowCount = nc+1;
//		if (virhe)
//			EmitGrid->Color = clYellow;
	if (Rastikoodit1->Checked) {
		rt = haerata(&Kilp);
		}
	for (int i = 0; i < nc; i++) {
		style = virhe ? 2 : 0;
		Cells[0][i+1].text = i;
		Cells[1][i+1].text = UnicodeString((short)Em.ctrlcode[i]);
		if (tulkinta[i] < 0) {
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
		st[pvparam[k_pv].laika] = 0;
		Cells[5][i+1].text = st;
		aikatowstr_ls(st, Em.time+10*(Em.ctrltime[i]-tm), t0);
		st[pvparam[k_pv].laika] = 0;
		Cells[6][i+1].text = st;
		if (tulkinta[i] == (n_lk - 1))
			TViimRasti = KSEK*(Em.time+10*(Em.ctrltime[i]-tm));
		for (int col = 0; col < 7; col++) {
			Cells[col][i+1].style = style;
			}
		}
	if (Kilp.id() > 0 && Kilp.id() == Em.kilpno && Kilp.Badge(k_pv, Em.osuus) == Em.badge &&
		TViimRasti != TMAALI0 && Kilp.maali(0) != TMAALI0) {
		EdtEro->Text = UnicodeString((Kilp.maali(0)-TViimRasti)/SEK);
		}
	for (int i = nc+1; i < MAXNLEIMA+1; i++) {
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
	EmitGrid->Refresh();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::Nayta(void)
{
	if (NimiHTHaku == -2) {
		NimiHTHaku = 0;
		Henkiltietokanta1->Checked = false;
		}
	if (seurahaku == -2) {
		seurahaku = 0;
		Seuraluettelo1->Checked = false;
		}
	NaytaKilp();
	NaytaEmit();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::NollaaHenktiedot(void)
{
	EdtLisno->Text = UnicodeString(0);
	EdtSNimi->Text = L"";
	EdtENimi->Text = L"";
	EdtSeura->Text = L"";
	EdtJoukkue->Text = L"";
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::Paivita(emittp *pEm)
{
	int d, n = 0, tl = 0;
	wchar_t as[20];
	static int edBadge;
	bool VakTietue = false;

	Em = *pEm;
	if (Em.kilpno >= minkilpno && getEmitJarr(Em.kilpno, &Em.osuus) == Tietue)
		eiLiitetty = false;
	if (!eiLiitetty && Em.kilpno && (d = getpos(Em.kilpno)) > 0) {
		Kilp.GETREC(d);
		DKilp = d;
		}
	else
		Kilp.nollaa();
//	if (kuntosuunnmoodi != 3 && kuntosuunnmoodi != 4 && Kilp.pv[k_pv].uusi != 'U') {
	Nayta();
	TallVals();
	if (UusiTietue) {
		wchar_t st[60];
		if (FormNaytaTulos && FormNaytaTulos->Visible) {
			if (esiluenta) {
				if (Kilp.p_aika(0) == 0 && wcswcind(Kilp.tark(), L"TIKHPV") < 0 &&
					NORMTULOS(Kilp.TLahto()-Nyt()) < FormNaytaTulos->IkkParam.LahtoRaja*MINUUTTI)
					FormNaytaTulos->NaytaTulos(Kilp.nimi(st, 40, false), L'T', Kilp.TLahto()+t0*TUNTI, 0, 0);
				else
					FormNaytaTulos->NaytaTulos(Kilp.nimi(st, 40, false), L'H', Kilp.TLahto()+t0*TUNTI, 0, 0);
				}
			else
				FormNaytaTulos->NaytaTulos(Kilp.nimi(st, 40, false), Kilp.tark(), Kilp.p_aika(0),
					Kilp.p_sija(0), ntulos[Kilp.Sarja()][0]);
			}
		if (Kilp.pv[k_pv].vac == 'V')
			VakTietue = true;
		BtnPerKirjaus->Visible = false;
		BtnOhita->Visible = (esiluenta || kuntoLahto);
		}
	UusiTietue = false;
	if (Kilp.kilpno && Em.badge != edBadge && (Tietue == emithead2 - 1 || kuntosuunnmoodi == 99)) {
		if (kuntosuunnmoodi == 99 && Kilp.pv[k_pv].uusi != 'U') {
			int Vast;
			setEmitJarr(-1, Kilp.kilpno, 0);
			KasittelyKesken = true;
			Vast = select3(2, (L"Kortti jo rekisteröity kilpailijalle "+UnicodeString(Kilp.kilpno)).c_str(),
				L"Valitse toimintatapa", L"Jätä kortti aiemmalle käyttäjälle",
				L"Ota kortti uuteen käyttöön", L"", this);
			switch (Vast) {
				case 1:
					NaytaMemo(1);
					Kilp.nollaa();
					break;
				case 2:
//					for (; haebdg(seurVapaaBadge) >= 0; seurVapaaBadge++) ;
					Kilp.pv[k_pv].badge[1] = Kilp.pv[k_pv].badge[0];
					Kilp.pv[k_pv].badge[0] = haeSeurVapaaBadge(Kilp.pv[k_pv].badge[0]);
					Kilp.tallenna(DKilp, 0, 0, 0, 0);
					Kilp.nollaa();
					Kilp.pv[k_pv].badge[0] = Em.badge;
					Kilp.tall_lajat_pv(tl, k_pv);         // Mitä tässä pitäisi antaa lähtöajaksi?
					Kilp.set_tark(L'-', k_pv);
					Kilp.pv[k_pv].uusi = 'U';
					NaytaMemo(2);
					break;
				}
			Em.kilpno = 0;
			Em.badge = 0;
			Em.put(Tietue, false);
			KasittelyKesken = false;
			Nayta();
			TallVals();
			EdtKilpno->Text = L"";
			}
		else if (Kilp.pv[k_pv].uusi == 'U') {
			KasittelyKesken = true;
			edBadge = Em.badge;
			EdtBadge->Text = UnicodeString(Em.badge);
			BtnTallenna->Visible = true;
			BtnPeruuta->Visible = true;
			if (kuntoLahto)
				Kilp.setSarjaAll(-1);
			if (Kilp.p_aika(0) == 0) {
				tl = SEK*(T_TIME(biostime(0, 0))/10);
				EdtLahto->Text = UnicodeString(aikatowstr_cols_n(as, tl, t0, L':', 8));
				}
			if (Henkiltietokanta1->Checked && FormTKanta->HenkiloHakuOn()) {
				FormTKanta->Caller = FormEmit;
				n = FormTKanta->NaytaHenkilot(NULL, NULL, NULL, NULL, Em.badge);
//				KasittelyKesken = true;
				if (n == 1) {
					bool Vast = true;
					erbeep();
					NimiKeyHit = False;
					SiirraHTTiedot(0);
					if (vahvistaaina) {
						if (Application->MessageBoxW(L"Oikea kilpailija löytynyt?", L"Vahvista", MB_YESNO) != IDYES)
							Vast = false;
						}
	//			if (select3(2, L"Onko haettu osanottaja oikea?", L"Vahvista",
	//				L"Oikea valittuna", L"Hae tai syötä toinen", L"", this) == 1)
					if (Vast) {
						if (!tallennaKilpailija(false)) {
							FocusControl(EdtBadge);
							}
						}
					else {
						NollaaHenktiedot();
						FocusControl(EdtSNimi);
						}
					}
				if (n > 1) {
					erbeep();
					erbeep();
					Application->MessageBoxW(L"Valitse osanottaja luettelosta käyttäen alas/ylös -näppäimiä Emit-kentässä", L"Valitse", MB_OK);
					EdtBadge->Color = clLime;
					FocusControl(EdtBadge);
					}
				if (n == 0) {
					erbeep();
					erbeep();
					Application->MessageBoxW(L"Syötä osanottajan tiedot tai valitse käyttäen nimihakua", L"Anna tiedot", MB_OK);
					FocusControl(EdtSNimi);
					}
				}
			else {
				if (!EdtKilpno->ReadOnly) {
//					int srj = -1, tls = 0, tlahto;
//					wchar_t kh = L'-';

//					tlahto = KSEK*T_TIME(biostime(0, 0));
					if (kuntosuunnmoodi == 99) {
						for (int ipv = k_pv; ipv < kilpparam.n_pv_akt; ipv++) {
							Kilp.pv[ipv].badge[0] = 0;
							Kilp.set_tark(L'V', ipv);
							Kilp.tall_lajat_pv(TMAALI0, ipv);
							}
						Kilp.tall_tulos(0, 0);
						Kilp.setSarja(kilpparam.vakanttisarja);
						EnterCriticalSection(&tall_CriticalSection);
						Kilp.tallenna(d, 0, 0, 0, 0);
						LeaveCriticalSection(&tall_CriticalSection);
						setEmitJarr(-1, Em.kilpno, 0);
						Em.kilpno = 0;
						Em.badge = 0;
						Em.put(Tietue, 0);
						Kilp.nollaa();
						Kilp.pv[k_pv].badge[0] = edBadge;
						Kilp.tall_lajat_pv(tl, k_pv);
						Kilp.set_tark(L'-', k_pv);
						Kilp.pv[k_pv].uusi = 'U';
						Nayta();
						TallVals();
						EdtKilpno->Text = L"";
						NaytaMemo(2);
						}
					else {
						Nayta();
						TallVals();
						}
					FocusControl(EdtKilpno);
					}
				}
			}
		else if (kuntosuunnmoodi == 3 && Kilp.pv[k_pv].badge[1] == 0) {
			tallennaKilpailija(false);
			}
		}
	Refresh();
	if (VakTietue && kuntosuunnmoodi != 1 && kuntosuunnmoodi != 2
		&& kuntosuunnmoodi != 98 && kuntosuunnmoodi != 99) {
		naytaMuutosOhje = true;
		BtnMuutoksetClick(this);
		naytaMuutosOhje = false;
		}
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
//	if (!CBLue->Checked)
//		CBViim->Checked = false;
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
	HaeTietue(pos, edsuunta);
	edsuunta = 0;
	EdtTietue->Text = UnicodeString(Tietue+1);
	CBViim->Checked = (pos >= emithead2 - 1 && CBLue->Checked);

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
	Label4->Visible = !esiluenta && !kuntoLahto && (!Panel1->Visible || Panel1->Top > Label4->Top+Label4->Height);
	Label5->Visible = kuntoLahto || !Panel1->Visible || Panel1->Top > Label5->Top+Label5->Height;
	Label7->Visible =  !esiluenta && !kuntoLahto &&
		(!Panel1->Visible || Panel1->Top > Label7->Top+Label7->Height);
	Label12->Visible = !esiluenta && !kuntoLahto &&
		(!Panel1->Visible || Panel1->Top > Label12->Top+Label12->Height);
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
	if (BdgMsgKirkkaus <= 40 && esiluenta == 2) {
		FormEmit->EdtBdgMsg->Text = L"Lue kortti tai syötä KilpNo";
		EdtBdgMsg->Color = TColor((1 + 256 + 65536) * 255);
		DKilp = 0;
		if (Em.kilpno) {
			Em.nollaa();
			Kilp.nollaa();
			DKilp = 0;
			Nayta();
			}
		}
}
//---------------------------------------------------------------------------
bool DisplTimerThreadOn;

void DisplTimerThread(LPVOID lpCn)
	{
	DisplTimerThreadOn = true;

	while (taustaon && !vaiheenvaihto && lkmThreadOn && DisplTimerThreadOn) {
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

	if ((kno = bdg2kno(badge)) <= 0) {
		for (int d = 1; d < nrec; d++) {
			kilp.GETREC(d);
			if ((kilp.kilpstatus == 0) && kilp.Badge(k_pv, 1) == badge) {
				kno = kilp.id();
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
				Kilp.GETREC(DKilp);
				i = getem(&Em, Kilp.id(), 0);
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

MESSAGE void __fastcall TFormEmit::NewCardHandler(TMyMessage &msg)
{
	int Vast, bdg;
	wchar_t msgtxt[200];

	if (KasittelyKesken)
		return;
	if (msg.wparam > 0) {
		EdtBdgMsg->Text = UnicodeString(L"Luettu kortti ")+UnicodeString(msg.wparam);
		BdgMsgKirkkaus = 256;
		if (!DisplTimerThreadOn)
			_beginthread(DisplTimerThread, 10240, &Vast);
		HaeTulostukseen(msg.wparam);
		return;
		}
	if (!esiluenta) {
		if (Tietue < emithead-1 && !CBLue->Checked && CBViim->Checked) {
			Tietue = em_HaeSeuraava(emithead2-1);
			HaeTietue(emithead2-1, 0);
			return;
			}
		if (emithead2 < emithead && Tietue >= 0) {
			if (!CBViim->Checked || Tietue < em_HaeViimeinen() || EmitMuutosFlag) {
				if (!BtnLoppuun->Visible && aani) {
					Beep(500,400);
					}
				BtnLoppuun->Visible = true;
					return;
				}
			else if (OnkoMuutoksia()) {
				EdtMessage->Text = L"Uusi kortti, kun muutoksia tallentamatta";
				EdtMessage->Color = clYellow;
				return;
/*
				Vast = select3(3, L"Käsitellyissä tiedoissa tallentamattomia muutoksia. Tallennetaanko ennen uuden kortin käsittelyä?",
					L"Tallennetaanko muutokset", L"Tallenna", L"Hylkää muutokset", L"Palaa aiempiin tietoihin", this);
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
	BtnLoppuun->Visible = false;
	if (emithead2 < emithead) {
		msgtxt[0] = 0;
		bdg = pros_emit(&EKilp, msgtxt);
		if (bdg <= 0) {
			Application->MessageBoxW(msgtxt, L"Virhe luennassa", MB_OK);
			return;
			}
		if (esiluenta || kuntoLahto) {
			MemoMessage->Text = msgtxt;
			MemoMessage->Color = msgtxt[0] ? clYellow : clBtnFace;
			MemoMessage->Visible = msgtxt[0] != 0;
			}
		else {
			EdtMessage->Text = msgtxt;
			EdtMessage->Color = msgtxt[0] ? clYellow : clBtnFace;
			}
		EdtBdgMsg->Text = UnicodeString(L"Käsittelyyn kortti ")+UnicodeString(bdg);
		BdgMsgKirkkaus = 256;
		if (!DisplTimerThreadOn)
			_beginthread(DisplTimerThread, 10240, &Vast);
		UusiTietue = true;
		HaeTietue(emithead2-1, 0);
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
	em.get(Tietue);
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
	em.get(Tietue);
	if (em.kilpno < minkilpno || getEmitJarr(em.kilpno , &Em.osuus) != pos) {
		eiLiitetty = true;
		EdtMessage->Text = L"Leimoja ei liitetty kilpailijaan";
		EdtMessage->Color = clYellow;
		}
	else {
		if (eiLiitetty) {
			EdtMessage->Text = L"";
			EdtMessage->Color = clBtnFace;
			}
		eiLiitetty = false;
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
   HaeTietue(pos-1, 0);
}
//---------------------------------------------------------------------------


void __fastcall TFormEmit::EmitGridKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_NEXT && Shift.Contains(ssCtrl)) {
		HaeTietue(Tietue+1, 1);
	}
	if (Key == VK_PRIOR && Shift.Contains(ssCtrl)) {
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
		HaeTietue(Tietue+1, 1);
		BtnLasketulosClick(Sender);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::CSarjaChange(TObject *Sender)
{
	int srj;

	srj = ValSarja-1;
	Kilp.setSarja(CSarja->ItemIndex-1);
	NaytaEmit();
	ValSarja = srj+1;
	if (Kilp.p_aika(0) == 0)
		LaskeTulos();
	OnkoMuutoksia();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::CRataChange(TObject *Sender)
{
	int i;

	i = ValRata;
	wcsncpy(Kilp.pv[k_pv].rata, CRata->Text.c_str(), sizeof(Kilp.pv[k_pv].rata)/2-1);
	NaytaEmit();
	ValRata = i;
	if (Kilp.p_aika(0) == 0)
		LaskeTulos();
	OnkoMuutoksia();
}
//---------------------------------------------------------------------------


void __fastcall TFormEmit::Seura1Click(TObject *Sender)
{
	Seura1->Checked = !Seura1->Checked;
	EdtSeura->Visible = Seura1->Checked;
	LblSeura->Visible = EdtSeura->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Kortinkoodi1Click(TObject *Sender)
{
	Kortinkoodi1->Checked = !Kortinkoodi1->Checked;
	EdtOrigBadge->Visible = Kortinkoodi1->Checked;
	LblOrigBadge->Visible = EdtOrigBadge->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Henkiltietokanta1Click(TObject *Sender)
{
	Henkiltietokanta1->Checked = !Henkiltietokanta1->Checked;
	if (Henkiltietokanta1->Checked) {
		FormTKanta->Show();
		FormTKanta->Caller = FormEmit;
		FormTKanta->Left = Left + Width;
//		FormTKanta->Left = Screen->Width - FormTKanta->Width;
		FormTKanta->Top = 20;
		if (FormTKanta->WindowState == wsMinimized)
			FormTKanta->WindowState = wsNormal;
		FormTKanta->BringToFront();
		}
	else if (FormTKanta) {
		FormTKanta->Caller = NULL;
		FormTKanta->Close();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Seuraluettelo1Click(TObject *Sender)
{
	Seuraluettelo1->Checked = !Seuraluettelo1->Checked;
	if (Seuraluettelo1->Checked) {
		FormSeurat->Show();
		FormSeurat->Caller = FormEmit;
		FormSeurat->Left = Left + Width;
//		FormSeurat->Left = Screen->Width - FormSeurat->Width;
		FormSeurat->Top = 300*Screen->PixelsPerInch/96;
		if (FormSeurat->WindowState == wsMinimized)
			FormSeurat->WindowState = wsNormal;
		FormSeurat->BringToFront();
		}
	else if (FormSeurat) {
		FormSeurat->Caller = NULL;
		FormSeurat->Close();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtSeuraKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	SeuraKeyHit = 1;
	NimiKeyHit = false;

	EdKey = Key + (Shift.Contains(ssShift) ? 0x10000 : 0) + (Shift.Contains(ssCtrl) ? 0x20000 : 0)
		+ (Shift.Contains(ssAlt) ? 0x40000 : 0);
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		Key = 0;
		tallennaKilpailija(true);
		FocusControl(EdtBadge);
		return;
		}
	if (Seuraluettelo1->Checked) {
		if (Key == 83 && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {   // Ctrl-S
			FormSeurat->Caller = FormEmit;
			FormSeurat->TallennaSeura(0, L"", L"", EdtSeura->Text.c_str(), 0);
			Key = 0;
			return;
			}
		if ((Key == VK_RETURN || Key == VK_F6)
//		if ((Key == VK_F6)
			&& !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
//			if (Key == VK_RETURN)
//				FocusControl(TarkVal);
			Key = (Key == VK_RETURN) ? VK_RETURN : 0;
			SeuraKeyHit = 0;
			if (EdtSeura->Text.Length() > 1)
				SiirraSeuraTiedot(0);
			}
		else if ((Key == VK_UP || Key == VK_F5)
			&& !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
			Key = 0;
			SeuraKeyHit = 0;
			SiirraSeuraTiedot(-1);
			}
		else if ((Key == VK_DOWN ||	Key == VK_F7)
			&& !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
			Key = 0;
			SeuraKeyHit = 0;
			SiirraSeuraTiedot(1);
			}
		}
	if (Key == VK_RETURN) {
		FocusControl(CSarja);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtSNimiKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	NimiKeyHit = true;
	SeuraKeyHit = 0;

	EdKey = Key + (Shift.Contains(ssShift) ? 0x10000 : 0) + (Shift.Contains(ssCtrl) ? 0x20000 : 0)
		+ (Shift.Contains(ssAlt) ? 0x40000 : 0);
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		Key = 0;
		tallennaKilpailija(true);
		FocusControl(EdtBadge);
		return;
		}
	if (Henkiltietokanta1->Checked) {
		if (Key == VK_RETURN ||
			(Key == VK_F6 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt))) {
			if (Key == VK_RETURN) {
				if (EdtENimi->Focused())
					FocusControl(EdtSeura);
				else
					FocusControl(EdtENimi);
				}
			Key = (Key == VK_RETURN) ? VK_TAB : 0;
			NimiKeyHit = false;
			SiirraHTTiedot(0);
			}
		if (Key == VK_UP ||
			Key == VK_F5 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
			Key = 0;
			NimiKeyHit = false;
			SiirraHTTiedot(-1);
			}
		if (Key == VK_DOWN ||
			Key == VK_F7 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
			Key = 0;
			NimiKeyHit = false;
			SiirraHTTiedot(1);
			}
		}
	else if (Key == VK_RETURN) {
		if (EdtENimi->Focused()) {
			if (EdtSeura->Visible)
				FocusControl(EdtSeura);
			else
				FocusControl(CSarja);
			}
		else
			FocusControl(EdtENimi);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtSNimiChange(TObject *Sender)
{
	OnkoMuutoksia();
	if (EdtSNimi->Text == NULL)
		return;
	if (EdKey == VK_RETURN || EdKey == VK_TAB) {
		EdtSNimi->SelectAll();
		return;
		}
	if (EdKey == VK_DOWN || EdKey == VK_UP || EdKey == VK_F5 || EdKey == VK_F7)
		return;
	if (NimiKeyHit || EdtSNimi->Text.Length() < 2) {
	   LSn = EdtSNimi->Text.Length();
		if (Henkiltietokanta1->Checked && FormTKanta->HenkiloHakuOn()) {
			FormTKanta->Caller = FormEmit;
			FormTKanta->NaytaHenkilot(EdtSNimi->Text.SubString(1, LSn).c_str(),
				(EdtSNimi->Text.SubString(1, LSn)+UnicodeString(L"ööö")).c_str(), NULL, NULL, 0);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtENimiChange(TObject *Sender)
{
	OnkoMuutoksia();
	if (EdtENimi->Text == NULL)
		return;
	if (EdKey == VK_RETURN || EdKey == VK_TAB) {
		EdtENimi->SelectAll();
		return;
		}
	if (EdKey == VK_DOWN || EdKey == VK_UP || EdKey == VK_F5 || EdKey == VK_F7)
		return;
	if (LSn < EdtSNimi->Text.Length())
		return;
	if (NimiKeyHit) {
	   LEn = EdtENimi->Text.Length();
		if (Henkiltietokanta1->Checked && FormTKanta->HenkiloHakuOn()) {
			FormTKanta->Caller = FormEmit;
			FormTKanta->NaytaHenkilot(EdtSNimi->Text.c_str(), EdtSNimi->Text.c_str(),
				EdtENimi->Text.SubString(1, LEn).c_str(), (EdtENimi->Text.SubString(1, LEn)+UnicodeString(L"ööö")).c_str(), 0);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtSeuraChange(TObject *Sender)
{
	OnkoMuutoksia();
	if (EdtSeura->Focused() && (EdKey == VK_DOWN || EdKey == VK_UP || EdKey == VK_F5 || EdKey == VK_F7))
		return;
	LSe = EdtSeura->Text.Length();
	if (Seuraluettelo1->Checked && FormSeurat->SeuraHakuOn()) {
		FormSeurat->Caller = FormEmit;
		FormSeurat->NaytaSeurat(EdtSeura->Text.SubString(1, LSe).c_str(),
			(EdtSeura->Text.SubString(1, LSe)+UnicodeString(L"ööö")).c_str());
		}
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

void __fastcall TFormEmit::BtnEiBadgeClick(TObject *Sender)
{
	wchar_t as[20];
	int kno;

	kno = haeSeurVakantti(0);
	DKilp = getpos(kno);
	Kilp.GETREC(DKilp);
	Kilp.set_tark(L'-', k_pv);
	Em.nollaa();
	Nayta();
	TallVals();
	Kilp.pv[k_pv].uusi = 'U';
	Kilp.set_tark(L'-', k_pv);
	Kilp.tall_lajat_pv(KSEK*T_TIME(biostime(0, 0)), k_pv);
	Nayta();
	BtnTallenna->Visible = true;
	BtnPeruuta->Visible = true;
	FocusControl(EdtSNimi);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtBadgeExit(TObject *Sender)
{
//	EdtBadge->Color = clBtnFace;
	EdtBadge->Color = clVaaleanHarmaa;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::PalaaHenkHausta(bool tallenna)
{
	if (tallenna) {
		SiirraHTTiedot(0);
		FocusControl(CSarja);
//		CSarja->SetFocus();
		}
	BringToFront();
	return;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::SiirraHTTiedot(int haku)
{
	int KHenk;
	wchar_t snimi[LSNIMI+1], enimi[LENIMI+1], seura[LSEURA+1], maa[4], sarja[20];
	wchar_t joukkue[LJK+1], sukup;
	int id, intid, badge, bib, ika, synt, d;

	if (!FormTKanta->HenkiloHakuOn())
		return;
	if (haku) {
		FormTKanta->Caller = FormEmit;
		FormTKanta->BringToFront();
		FormTKanta->Selaa(haku);
		return;
		}
	KHenk =	FormTKanta->THenktiedot(haku, snimi, enimi, seura, maa, sarja, joukkue, &id, &intid,
		&badge, &bib, &sukup, &ika, &synt);
	if (KHenk >= 0) {
		if (kuntosuunnmoodi != 2 || CSarja->ItemIndex <= 0
			|| (kilpparam.vakanttisarja > 0 && CSarja->ItemIndex == kilpparam.vakanttisarja+1)) {
			int srj = -1;
			if (Kythenkiltietokannansarjaa1->Checked && wcslen(sarja) > 0)
				srj = haesarja_w(sarja, false);
			if (srj >= 0 || kuntoLahto)
				CSarja->ItemIndex = srj+1;
			}
		EdtLisno->Text = UnicodeString(id);
		EdtSNimi->Text = UnicodeString(snimi);
		EdtENimi->Text = UnicodeString(enimi);
		EdtSeura->Text = UnicodeString(seura);
		EdtJoukkue->Text = UnicodeString(joukkue);
		HTBadge = badge;
		}
	else {
		EdtLisno->Text = UnicodeString(0);
		EdtSNimi->Text = L"";
		EdtENimi->Text = UnicodeString(Kilp.id());
		EdtSeura->Text = L"";
		EdtJoukkue->Text = L"";
		}
	if (kilpparam.seuralyh_on) {
		HSeuralyh = EdtSeura->Text;
		HSeura = L"";
		}
	else {
		HSeura = EdtSeura->Text;
		HSeuralyh = L"";
		}
	Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::PalaaSeuraHausta(bool tallenna)
{
	if (tallenna) {
		SiirraSeuraTiedot(0);
		FocusControl(CSarja);
		}
	BringToFront();
	return;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::SiirraSeuraTiedot(int haku)
{
	int KSeura;
	wchar_t nimi[LSEURA+1], lyhenne[LLYH+1], maa[4];
	int piiri;

	if (FormSeurat->SeuraHakuOn()) {
		if (haku) {
			FormSeurat->Caller = FormEmit;
			FormSeurat->BringToFront();
			FormSeurat->Selaa(haku);
			OnkoMuutoksia();
			return;
			}
		KSeura = FormSeurat->Seuratiedot(haku, &piiri, maa, lyhenne, nimi);
		if (KSeura >= 0) {
			HSeura = UnicodeString(nimi);
			HSeuralyh = UnicodeString(lyhenne);
			EdtSeura->Text = kilpparam.seuralyh_on ? HSeuralyh : HSeura;
			}
		else if (!(EdtSeura->Focused() && (EdKey & 0xffff) == VK_RETURN)){
			HSeura = L"";
			HSeuralyh = L"";
			EdtSeura->Text = L"";
			}
		}
	OnkoMuutoksia();
}
//---------------------------------------------------------------------------

int __fastcall TFormEmit::tallennaKilpailija(bool kysy)
{
	int d, kilpno, bdg, orgbdg;
	wchar_t st[20];

	kilpno = Kilp.id();
	if (CSarja->ItemIndex <= 0) {
//		Application->MessageBox(L"Valitse sarja ennen tallentamista", L"Virhe",	MB_OK);
		CSarja->ItemIndex = 0;
		CSarja->Color = clYellow;
		FocusControl(CSarja);
		return(1);
		}
	if (kuntosuunnmoodi && Henkiltietokanta1->Checked) {
		bdg = _wtoi(EdtBadge->Text.c_str());
		orgbdg = _wtoi(EdtOrigBadge->Text.c_str());
		if (Kilp.pv[k_pv].laina[1] != L'L' && !(on_lainakortti(bdg) || on_lainakortti(orgbdg)) && HTBadge != 0 &&
			((bdg != HTBadge && bdg > 30000 && bdg < 1000000) || (orgbdg != HTBadge && orgbdg > 30000 && bdg < 1000000)) &&
			_wtoi(EdtLisno->Text.c_str()) != 0) {
			if (Application->MessageBox(L"Vaihdetaanko tietokannan Emit-koodi?", L"Pysyvä muutos?",
				MB_YESNO) != IDYES) {
				Kilp.pv[k_pv].laina[1] = L'L';
				}
			}
		}
	if (kysy) {
		if (Application->MessageBox(L"Vahvista tallennus", L"Vahvista",
			MB_YESNO) != IDYES)
		return(0);
		}
	if ((kuntosuunnmoodi == 1) && Kilp.pv[k_pv].uusi == 'U' && (Kilp.p_aika(0) || (Em.ctrlcode[1] && Em.ctrlcode[1] != 250))) {
		if (kuntoMaali) {
			if (select3(2, L"Suunnistajan kortti saattaa sisältää suorituksen. "
				L"Merkitäänkö kuitenkin lähtijäksi? Jos ei merkitä, tulkitaan suunnistaja maaliintulijaksi.",
				L"Onko lähtijä?", L"Lähtijä", L"Maaliin tulija", L"", this) != 1) {
/*
			if (Application->MessageBoxW(L"Suunnistajan kortti saattaa sisältää suorituksen. "
				L"Merkitäänkö kuitenkin lähtijäksi? Jos ei merkitä, tulkitaan suunnistaja maaliintulijaksi.",
				L"Onko lähtijä?", MB_YESNO) != IDYES) {
			if (select3(2, L"Suunnistajalla saattaa olla jo suoritus. "
				L"Merkitäänkö kuitenkin lähtijäksi?",
				L"Onko lähtijä?", L"Lähtijä", L"Maaliin tulija", L"", this) != 1)
*/
				Kilp.pv[k_pv].uusi = 0;
				}
			else
				EdtLahto->Text = EdtLuettu->Text;
			}
		else
			EdtLahto->Text = EdtLuettu->Text;
		}
	Kilp.setSarja(CSarja->ItemIndex-1);
	Kilp.lisno[0] = EdtLisno->Text.ToInt();
	wcsncpy(Kilp.sukunimi, EdtSNimi->Text.c_str(),kilpparam.lsnimi);
	wcsncpy(Kilp.etunimi, EdtENimi->Text.c_str(),kilpparam.lenimi);
	if (!Salanimienkasittely1->Checked)
		wcsncpy(Kilp.pv[k_pv].selitys, EdtSelitys->Text.c_str(),sizeof(Kilp.pv[k_pv].selitys)/2-1);
	else {
		wcsncpy(Kilp.yhdistys, EdtSelitys->Text.c_str(),sizeof(Kilp.yhdistys)/2-1);
		Kilp.pv[0].salaa = CBSelSN->ItemIndex == 1 ? 'Y' : 0;
		}
	if (kilpparam.seuralyh_on) {
		if (HSeura.Length() > 0)
			wcsncpy(Kilp.seura, HSeura.c_str(), kilpparam.lseura);
		}
	else {
		if (HSeuralyh.Length() > 0)
			wcsncpy(Kilp.seuralyh, HSeuralyh.c_str(), kilpparam.lseuralyh);
		}
	Kilp.set_seuratieto(EdtSeura->Text.c_str());
	wcsncpy(Kilp.joukkue, EdtJoukkue->Text.c_str(), kilpparam.ljoukkue);
	if (Sarjat[Kilp.Sarja()].tsak[k_pv])
		Kilp.pv[k_pv].tasoitus = Sarjat[Kilp.Sarja()].tsak[k_pv]*_wtoi(EdtSakkoja->Text.c_str());
	Kilp.set_tark(TarkKoodit[TarkVal->ItemIndex+1], k_pv);
	if (esiluenta) {
		Kilp.pv[k_pv].badge[0] = _wtoi(EdtBadge->Text.c_str());
		if (Merkitsekortitlainakorteiksi1->Checked) {
			Kilp.pv[k_pv].laina[0] = L'L';
			}
		}
	if (kilpparam.kaksibadge == 2) {
		Kilp.pv[k_pv].asakot[0] = towupper(EdtTark1->Text.c_str()[0]);
		Kilp.pv[k_pv].asakot[1] = towupper(EdtTark2->Text.c_str()[0]);
		if (Kilp.pv[k_pv].asakot[0] < L'A')
			Kilp.pv[k_pv].asakot[0] = L'-';
		if (Kilp.pv[k_pv].asakot[1] < L'A')
			Kilp.pv[k_pv].asakot[1] = L'-';
		}
	Kilp.tall_lajat_pv(wstrtoaika_vap(EdtLahto->Text.c_str(), t0), k_pv);
	Kilp.tall_tulos(0, wstrtoaika_vap(EdtTulos->Text.c_str(), 0));
	if (CRata->ItemIndex > 0)
		wcsncpy(Kilp.pv[k_pv].rata, CRata->Text.c_str(), sizeof(Kilp.pv[k_pv].rata)/2-1);
	if (kuntosuunnmoodi && on_lainakortti(Kilp.pv[k_pv].badge[0]))
		Kilp.pv[k_pv].laina[0] = L'L';
	if (kuntosuunnmoodi && on_lainakortti(Kilp.pv[k_pv].badge[1]))
		Kilp.pv[k_pv].laina[1] = L'L';
	if (kuntosuunnmoodi && kuntosuunnmoodi != 99 &&
		(kuntosuunnmoodi == 2 || kuntosuunnmoodi == 3 || (Kilp.pv[k_pv].uusi != 'U' &&
		getEmitJarr(Kilp.id(), 0) >= 0)) &&
		Kilp.pv[k_pv].badge[1] == 0 && Kilp.pv[k_pv].badge[0] < BADGEASKEL) {
		Kilp.pv[k_pv].badge[0] = haeSeurVapaaBadge(Kilp.pv[k_pv].badge[0]);
		Kilp.pv[k_pv].badge[1] = Em.badge;
		Em.badge = Kilp.pv[k_pv].badge[0];
		if (Em.badge0 != 0 && DKilp == getpos(Kilp.id()))
			putem(&Em, Tietue, 0);
		Kilp.pv[k_pv].laina[1] = Kilp.pv[k_pv].laina[0];
		}
	if (kuntosuunnmoodi == 1 && Kilp.pv[k_pv].uusi == 'U') {
		Kilp.set_tark(L'-', k_pv);
		Kilp.tall_tulos(k_pv, 0);
		setEmitJarr(-1, Em.kilpno, 0);
		Em.kilpno = 0;
		Em.badge = 0;
		if (Em.badge0 != 0 && DKilp == getpos(Kilp.id()))
			putem(&Em, Tietue, 0);
		}
	if (esiluenta) {
		Kilp.set_tark(L'-', k_pv);
		Kilp.tall_tulos(k_pv, 0);
		Kilp.tall_lajat(SEK*(T_TIME(biostime(0, 0))/10));
		BtnPerKirjaus->Visible = true;
		BtnOhita->Visible = false;
		}
/*	if (kuntosuunnmoodi && (kuntosuunnmoodi == 2 || Kilp.pv[k_pv].uusi != 'U') &&
		Kilp.pv[k_pv].badge[1] == 0) {
		for (; haebdg(seurVapaaBadge) >= 0; seurVapaaBadge++) ;
		Kilp.pv[k_pv].badge[1] = Em.badge;
		Em.badge = seurVapaaBadge;
		Kilp.pv[k_pv].badge[0] = Em.badge;
		putem(&Em, Tietue, 0);
		Kilp.pv[k_pv].laina[1] = Kilp.pv[k_pv].laina[0];
		}
*/
	Kilp.pv[k_pv].uusi = 0;
	EnterCriticalSection(&tall_CriticalSection);
	if (DKilp == getpos(Kilp.id()))
		Kilp.tallenna(DKilp, 0, 0, 0, 0);
	LeaveCriticalSection(&tall_CriticalSection);
	KasittelyKesken = false;
	Nayta();
	TallVals();
	if (!EdtKilpno->ReadOnly)
		FocusControl(EdtKilpno);
	if (esiluenta == 1)
		NaytaMemo(1);
	if (LblToimi->Visible)
		LblToimi->Caption = L"Lue kortti";
	return(0);
//	if (CBSeuralisa->Checked && CBSeurahaku->Checked && FormSeurat->SeuraHakuOn())
//		FormSeurat->TallennaSeura(Kilp.piiri, Kilp.maa, Kilp.seuralyh, Kilp.seura, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtBadgeKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{

	EdKey = Key + (Shift.Contains(ssShift) ? 0x10000 : 0) + (Shift.Contains(ssCtrl) ? 0x20000 : 0)
		+ (Shift.Contains(ssAlt) ? 0x40000 : 0);
	if (FormTKanta->HenkiloHakuOn()) {
		FormTKanta->Caller = FormEmit;
		if (Key == VK_RETURN ||
			(Key == VK_F6 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt))) {
			if (Key == VK_RETURN)
				FocusControl(EdtSNimi);
			Key = (Key == VK_RETURN) ? VK_TAB : 0;
			SiirraHTTiedot(0);
			}
		if (Key == VK_UP ||
			Key == VK_F5 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
			Key = 0;
			SiirraHTTiedot(-1);
			}
		if (Key == VK_DOWN ||
			Key == VK_F7 && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
			Key = 0;
			SiirraHTTiedot(1);
			}
		}
	else if (Key == VK_RETURN)
		FocusControl(EdtSNimi);

}
//---------------------------------------------------------------------------
int __fastcall TFormEmit::lueKilp3(int d)
{
	kilptietue kilp1;

	kilp1.GETREC(d);
	if (kilp1.tark(k_pv) == L'P')
		return(1);
	DKilp = d;
	kilp1.set_tark(Kilp.tark(k_pv), k_pv);
	kilp1.tall_tulos(0, Kilp.p_aika(0));
	kilp1.tall_lajat_pv(Kilp.p_aika(-1), k_pv);
	kilp1.pv[k_pv].badge[0] = Kilp.pv[k_pv].badge[0];
	kilp1.pv[k_pv].uusi = Kilp.pv[k_pv].uusi;
	kilp1.pv[k_pv].vac = 0;
	Kilp = kilp1;
	Nayta();
	OnkoMuutoksia();
	return(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtKilpnoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	int d, kno;
	wchar_t st[20];

	SeuraKeyHit = 0;
	NimiKeyHit = false;
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
				if (lueKilp3(d))
					Application->MessageBoxW(L"Pyydetty kilpailija merkitty poissaolevaksi", L"Este",
						MB_OK);
				else
					tallennaKilpailija(false);
				}
			}
		tallennaKilpailija(esiluenta == 0);
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
			if (esiluenta == 2) {
				kilptietue kilp1;
				kilp1.GETREC(d);
				if (kilp1.tark(k_pv) == L'P') {
					Application->MessageBoxW(L"Pyydetty kilpailija merkitty poissaolevaksi", L"Este",
						MB_OK);
					}
				else {
					BdgMsgKirkkaus = 255;
					HaeTulostukseen(kilp1.pv[k_pv].badge[0]);
					}
				}
			else {
				if (lueKilp3(d))
					Application->MessageBoxW(L"Pyydetty kilpailija merkitty poissaolevaksi", L"Este",
						MB_OK);
				else
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
		ValSNimi != EdtSNimi->Text ||
		ValENimi != EdtENimi->Text ||
		ValSeura != EdtSeura->Text ||
		ValTulos != EdtTulos->Text ||
		ValMaali != EdtMaali->Text ||
		ValLahto != EdtLahto->Text ||
		ValSelitys != EdtSelitys->Text ||
		ValTarkVal != TarkVal->ItemIndex ||
		(monirata && ValRata != CRata->ItemIndex) ||
		ValSarja != CSarja->ItemIndex ||
		ValSalaa != CBSelSN->ItemIndex ||
		ValLisno != EdtLisno->Text ||
		ValJoukkue != EdtJoukkue->Text||
		ValHSeura != HSeura ||
		ValHSeuralyh != HSeuralyh) {
		retval = true;
		}
	if (!retval && kilpparam.kaksibadge == 2 &&
		(ValTark[0] != EdtTark1->Text.c_str()[0] ||
		 ValTark[1] != EdtTark2->Text.c_str()[0])) {
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

void __fastcall TFormEmit::EdtTulosChange(TObject *Sender)
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
	CSarja->Color = clWindow;
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
		FocusControl(EdtTulos);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtTulosKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

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
	for (int i = 0; i < MAXNLEIMA; i++)
		if (Em->ctrlcode[i] == 99)
			return(true);
	return(false);
}
//---------------------------------------------------------------------------

bool __fastcall TFormEmit::HakuEhto(bool ylos)
{
	bool vast = false;

	eroSelaus = false;
	switch (CBVaadi->ItemIndex) {
		case 0:
			vast = true;
			break;
		case 1:
			if (Kilp.Badge(k_pv, 0) == Em.badge) {
				if (Kilp.tark(k_pv) != L'T' || !Kilp.p_aika(0) ||
					tarkista(&Em, 0, 0, 0))
				vast = true;
				}
			break;
		case 2:
			if (Kilp.tark(k_pv) == L'I')
				vast = true;
			break;
		case 3:
			if (Kilp.tark(k_pv) == L'H' || Kilp.tark(k_pv) == L'M')
				vast = true;
			break;
		case 4:
			if (Kilp.tark(k_pv) == L'K' || Kilp.tark(k_pv) == L'X')
				vast = true;
			break;
		case 5:
			if (Kilp.tark(k_pv) == L'-')
				vast = true;
			break;
		case 6:
			if (Kilp.tulos_pv(k_pv, false, 0) == 0)
				vast = true;
			break;
		case 7:
			if (Kilp.Sarja() == CSarjaVaat->ItemIndex)
				vast = true;
			break;
		case 8:
			vast = On99(&Em);
			break;
		case 9:
			vast = true;
			break;
		case 10:
			if (Em.badge == 0 || Em.kilpno == 0 || Kilp.Badge(k_pv, 0) != Em.badge ||
				Em.kilpno != Kilp.id()) {
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
			EdtHakuArvo->Text = EdtSNimi->Text + L" " + EdtENimi->Text;
			break;
		case 1:
			EdtHakuArvo->Text = EdtKilpno->Text;
			break;
		case 2:
			EdtHakuArvo->Text = L"";
			break;
		case 3:
			EdtHakuArvo->Text = UnicodeString(Tietue+1);
			break;
		case 4:
			haeEmiterot();
			if (kEmitErot >= nEmitErot)
				kEmitErot = nEmitErot - 1;
			EdtHakuArvo->Text = UnicodeString(EmitErot[kEmitErot].ero);
			if (nEmitErot) {
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
	int d, i, count = 0;

	do {
		JatkaHakua = 1;
		switch (RGHakuPeruste->ItemIndex) {
			case 0:
				if (nextix(&nameindex, key, &d)) {
					JatkaHakua = 0;
					break;
					}
				DKilp = d;
				Kilp.GETREC(d);
				MuutosHaku = false;
				EdtHakuArvo->Text = Kilp.nimi(txt, 60, false);
				i = getem(&Em, Kilp.id(), 0);
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
				EdtHakuArvo->Text = UnicodeString(i+1);
				if (i > 999999)
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
	int d, i;

	do {
		JatkaHakua = 1;
		switch (RGHakuPeruste->ItemIndex) {
			case 0:
				if (previx(&nameindex, key, &d)) {
					JatkaHakua = 0;
					break;
					}
				DKilp = d;
				Kilp.GETREC(d);
				MuutosHaku = false;
				EdtHakuArvo->Text = Kilp.nimi(txt, 60, false);
				i = getem(&Em, Kilp.id(), 0);
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
			case 2:
			case 3:
				i = _wtoi(EdtHakuArvo->Text.c_str());
				if (i > 1)
					EdtHakuArvo->Text = UnicodeString(i-1);
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
	Label4->Visible = !esiluenta && !kuntoLahto && (!Panel1->Visible || Panel1->Top > Label4->Top+Label4->Height);
	Label5->Visible = kuntoLahto || !Panel1->Visible || Panel1->Top > Label5->Top+Label5->Height;
	Label7->Visible =  !esiluenta && !kuntoLahto &&
		(!Panel1->Visible || Panel1->Top > Label7->Top+Label7->Height);
	Label12->Visible = !esiluenta && !kuntoLahto &&
		(!Panel1->Visible || Panel1->Top > Label12->Top+Label12->Height);
	if (esiluenta || kuntoLahto)
		BtnLoppuunClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtHakuArvoChange(TObject *Sender)
{
	wchar_t txt[62];
	char key[MAXINDL+1];
	int d, i, k;

	if (!MuutosHaku || !CBHaeArvo->Checked)
		return;
	CBViim->Checked = false;
	switch (RGHakuPeruste->ItemIndex) {
		case 0:
			wcsncpy(txt, EdtHakuArvo->Text.c_str(),60);
			txt[60] = 0;
			keyfromwname(key, txt, nameindex.keyl-2, 0);
			if (searchix(&nameindex, key, &d))
				break;
			DKilp = d;
			Kilp.GETREC(d);
			if (!esiluenta) {
				i = getem(&Em, Kilp.id(), 0);
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
				DKilp = d;
				Kilp.GETREC(d);
				if (!esiluenta) {
					i = getem(&Em, Kilp.id(), 0);
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
			else
				JatkaHakua = 2;
			break;
		case 2:
			i = _wtoi(EdtHakuArvo->Text.c_str());
			if (i <= 0 || i > bdg_kno[nbadge-1].badge) {
				JatkaHakua = 0;
				break;
				}
			if ((k = bdg2kno(i)) <= 0 && kuntosuunnmoodi > 0) {
				for (d = 1; d < nrec; d++) {
					Kilp.GETREC(d);
					if (Kilp.kilpstatus == 0 &&
						Kilp.Badge(k_pv, 1) == i) {
						k = Kilp.id();
						break;
						}
					}
				}
			if (k > 0 && (d = getpos(k)) > 0) {
				DKilp = d;
				Kilp.GETREC(d);
				i = getem(&Em, Kilp.id(), 0);
				if (i >= 0) {
					HaeTietue(i, 0);
					}
				else {
					Tietue = emithead2-1;
					Nayta();
					}
				}
			else
				JatkaHakua = 2;
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

void __fastcall TFormEmit::BtnHaeOoClick(TObject *Sender)
{
	EdHeight = Height;
	if (Height < 650 * Screen->PixelsPerInch / 96)
		Height = 650 * Screen->PixelsPerInch / 96;
	Panel1->Visible = true;
	if (Panel1->Visible) {
		Panel1->BringToFront();
		}
	Label4->Visible = !esiluenta && !kuntoLahto && (!Panel1->Visible || Panel1->Top > Label4->Top+Label4->Height);
	Label5->Visible = kuntoLahto || !Panel1->Visible || Panel1->Top > Label5->Top+Label5->Height;
	Label7->Visible =  !esiluenta && !kuntoLahto &&
		(!Panel1->Visible || Panel1->Top > Label7->Top+Label7->Height);
	Label12->Visible = !esiluenta && !kuntoLahto &&
		(!Panel1->Visible || Panel1->Top > Label12->Top+Label12->Height);
	CBHaeArvo->Checked = true;
	BtnHaeOo->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtBadgeChange(TObject *Sender)
{
	if (Em.badge && (kilpparam.kaksibadge != 2 || Kilp.pv[k_pv].badge[0] == 0))
		EdtBadge->Text = UnicodeString(Em.badge);
	else
		EdtBadge->Text = UnicodeString(Kilp.pv[k_pv].badge[0]);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtBadgeEnter(TObject *Sender)
{
	if (suljesuoraan || !lkmThreadOn || !FormTKanta)
		return;
	if (Henkiltietokanta1->Checked && FormTKanta->HenkiloHakuOn() &&
		!FormTKanta->ValBadge) {
		FormTKanta->Caller = FormEmit;
		FormTKanta->NaytaHenkilot(NULL, NULL, NULL, NULL, Em.badge);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtBadgeKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key != L'\t')
		Key = 0;
}
//---------------------------------------------------------------------------


void __fastcall TFormEmit::BtnPeruutaClick(TObject *Sender)
{
//	PeruutaMuutokset();
	if (DKilp > 0)
		Kilp.GETREC(DKilp);
	KasittelyKesken = false;
	Nayta();
	Refresh();
}
//---------------------------------------------------------------------------

int __fastcall TFormEmit::LaskeTulos(void)
{
	int tm, tlahto = TMAALI0;
	kilptietue kilp;
	wchar_t st[20];

	if (Sarjat[Kilp.Sarja()].laskemtb[k_pv]) {
		laskeEm_VA(DKilp, false);
		Kilp.GETREC(DKilp);
		kilp = Kilp;
		AIKATOWSTRS(st, kilp.p_aika(0), 0);
		st[pvparam[k_pv].laika] = 0;
		EdtTulos->Text = st;
		if (kilp.pv[k_pv].tlahto != TMAALI0) {
			AIKATOWSTRS(st, kilp.pv[k_pv].tlahto, t0);
			st[8] = 0;
			EdtLahto->Text = st;
			}
		else
			EdtLahto->Text = L"";
		if (kilp.maali(0) != TMAALI0) {
			AIKATOWSTRS(st, kilp.maali(0), t0);
			st[pvparam[k_pv].laika] = 0;
			EdtMaali->Text = st;
			}
		else
			EdtMaali->Text = L"";
		}
	else {
		kilp = Kilp;
		tm = e_maaliaika(&Em, &kilp, &tlahto);

		if (kilpparam.kaksibadge == 2 && kilp.Badge(k_pv, 1)) {
			int ip2, tm2 = TMAALI0, tlahto2;
			emittp em2;

			if ((ip2 = em2.getbykno(kilp.id(), 1-Em.osuus)) >= 0) {
				tm2 = e_maaliaika(&em2, &kilp, &tlahto2);
				if (tm2 == TMAALI0)
					tm = TMAALI0;
				else if (NORMKELLO(tm2-tm) > 0)
					tm = NORMKELLO(tm2);
				}
			}

		if (tm != TMAALI0) {
			if (tlahto != TMAALI0) {
	//			if (kilp.pv[k_pv].tlahto == TMAALI0 || kilp.pv[k_pv].tlahto == 0)
				kilp.tall_lajat_pv(tlahto, k_pv);
	//		else {
	//			tm += kilp.pv[k_pv].tlahto - tlahto;
				tm = (tm + 36*TUNTI) % (24*TUNTI) - 12*TUNTI;
				}
			kilp.pv[k_pv].tasoitus =
				Sarjat[kilp.Sarja()].tsak[k_pv]*_wtoi(EdtSakkoja->Text.c_str());
			kilp.set_tulos(0, tm);
			AIKATOWSTRS(st, kilp.p_aika(0), 0);
			st[pvparam[k_pv].laika] = 0;
			EdtTulos->Text = st;
			if (kilp.pv[k_pv].tlahto != TMAALI0) {
				AIKATOWSTRS(st, kilp.pv[k_pv].tlahto, t0);
				st[8] = 0;
				EdtLahto->Text = st;
				}
			else
				EdtLahto->Text = L"";
			if (kilp.maali(0) != TMAALI0) {
				AIKATOWSTRS(st, kilp.maali(0), t0);
				st[pvparam[k_pv].laika] = 0;
				EdtMaali->Text = st;
				}
			else
				EdtMaali->Text = L"";
			}
		EdtEro->Text = L"";
		}
	OnkoMuutoksia();
	return(kilp.p_aika(0));
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

	if (getEmitJarr(Kilp.id(), &Em.osuus) < 0) {
		Application->MessageBoxW(L"Valittulla kilpailijalla ei leimaustietoja", L"Este", MB_OK);
		return;
		}
	Kilp_0 = Kilp;
	if (Kilp_0.pv[k_pv].uusi != 'U')
		Kilp_0.pv[k_pv].vac = 0;
	DKilp_0 = DKilp;
	Em_0 = Em;
	if (!FormEmitMuutokset)
		FormEmitMuutokset = new TFormEmitMuutokset(this);
	FormEmitMuutokset->PageControl1->ActivePageIndex = 0;
	FormEmitMuutokset->MemoOhje->Visible = naytaMuutosOhje;
	FormEmitMuutokset->Nayta(DKilp);
	FormEmitMuutokset->NimiHaku = false;
	EmitMuutosFlag = true;
	FormEmitMuutokset->Show();
	if (FormEmitMuutokset->WindowState == wsMinimized)
		FormEmitMuutokset->WindowState = wsNormal;
	FormEmitMuutokset->BringToFront();
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
	Em.kilpno = Kilp_0.id();
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

void __fastcall TFormEmit::SiirraKilpailijalle(int kno, INT16 kbdg)
{
	kilptietue kilp;

	int d = getpos(kno);
	if (d > 0)
		kilp.GETREC(d);
	if (d > 0 && kilp.tark(k_pv) != L'P') {
		if (kbdg == 1 && Kilp_0.pv[k_pv].badge[1] == 0) {
			Kilp_0.pv[k_pv].badge[1] = Kilp_0.pv[k_pv].badge[0];
			Kilp_0.pv[k_pv].badge[0] = 0;
			if (Emitvaihdottuleville->Checked) {
				for (int i_pv = k_pv + 1; i_pv < kilpparam.n_pv_akt; i_pv++)
					Kilp_0.pv[i_pv].badge[0] = 0;
				}
			}
		DKilp = d;
		Kilp = kilp;
		if (Kilp_0.pv[k_pv].badge[1-kbdg]) {
			Kilp.pv[k_pv].badge[1-kbdg] = Kilp_0.pv[k_pv].badge[1-kbdg];
			Kilp.pv[k_pv].laina[1-kbdg] = Kilp_0.pv[k_pv].laina[1-kbdg];
			}
		if (wcswcind(Kilp_0.tark(k_pv), L"-VEB") < 0)
			Kilp.set_tark(Kilp_0.tark(k_pv), k_pv);
		if (eiLiitetty) {
			if (Em.badge == 0 && Kilp.pv[k_pv].badge[kbdg] != 0)
				Em.badge = Kilp.pv[k_pv].badge[kbdg];
			else if (Em.badge == 0 && Em.badge0 != 0)
				Em.badge = haeSeurVapaaBadge(Em.badge0);
			}
		Kilp.pv[k_pv].badge[kbdg] = Em.badge;
		if (Emitvaihdottuleville->Checked) {
			for (int i_pv = k_pv + 1; i_pv < kilpparam.n_pv_akt; i_pv++)
				Kilp.pv[i_pv].badge[kbdg] = Em.badge;
			}
		Em.kilpno = kno;
		Em.osuus = kbdg;
		Nayta();
		}
	else
		Application->MessageBoxW(L"Siirrolle on este", L"Este", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::TallennaMuutokset(void)
{
	int Vast = 2, kno = 0;
	INT16 ibdg;
	int kilpVapaa = 0;
	int d;
	kilptietue vkilp, kilp;
	bool vakantilta = false;

	if (eiLiitetty) {
		if (Em.badge != 0 && Em.badge == Kilp.pv[k_pv].badge[Em.osuus] && Em.kilpno == Kilp.id()) {
			EnterCriticalSection(&tall_CriticalSection);
			Kilp.tallenna(DKilp, 0, -1, 0, 0);
			Em.put(Tietue, false);
			LeaveCriticalSection(&tall_CriticalSection);
			addbadge(Em.badge, Kilp.id(), 0);
			setEmitJarr(Tietue, Em.kilpno, Em.osuus);
			eiLiitetty = false;
			EdtMessage->Text = L"";
			EdtMessage->Color = clBtnFace;
			}
		return;
		}
	if (DKilp_0 == DKilp) {
		Application->MessageBoxW(L"Ei tallennettavaa muutosta", L"", MB_OK);
		return;
		}
	ibdg = Em.osuus;
	if (getEmitJarr(Kilp.id(), &Em.osuus) >= 0) {
		Vast = select3(3, L"Kohdekilpailijalla jo leimaustiedot.", L"Valitse toimintatapa",
			L"Luovu siirrosta", L"Siirrä menettäen yhteys \r\n aiempiin leimaustietoihin",
			L"Siirrä aiemmat \r\nleimat vakantille", this);
		if (Vast <= 1) {
			Em.get(Tietue);
			Paivita(&Em);
			return;
			}
		}
	kilp.GETREC(DKilp);
	if (kilp.p_aika(0) == 0)
		kilpVapaa |= 1;
	if (kilp.tark(k_pv) == L'-' || kilp.tark(k_pv) == L'E' || kilp.tark(k_pv) == L'B' || kilp.tark(k_pv) == L'V')
		kilpVapaa |= 2;
	if (Vast == 3) {

		kno = haeSeurVakantti(0);
		if (kno == 0) {
			Application->MessageBoxW(L"Ei vakantteja, muutos perutaan", L"", MB_OK);
			return;
			}
		EnterCriticalSection(&tall_CriticalSection);
		d = getpos(kno);
		vkilp.GETREC(d);
		vkilp.setSarjaAll(kilp.Sarja());
		vkilp.set_tark(kilp.tark(k_pv), k_pv);
		vkilp.tall_lajat_pv(kilp.p_aika(-1), k_pv);
		vkilp.tall_tulos(0, kilp.p_aika(0));
		vkilp.pv[k_pv].badge[0] = kilp.pv[k_pv].badge[0];
		vkilp.pv[k_pv].badge[1] = kilp.pv[k_pv].badge[1];
		vkilp.pv[k_pv].laina[0] = kilp.pv[k_pv].laina[0];
		vkilp.pv[k_pv].laina[1] = kilp.pv[k_pv].laina[1];
		vkilp.tallenna(d, 0, -1, 0, 0);
		}
	else
		EnterCriticalSection(&tall_CriticalSection);
	Em.get(Tietue);
	if (Em.osuus != ibdg) {
		Em.osuus = ibdg;
		Em.put(Tietue, false);
		}
	Kilp.pv[k_pv].badge[ibdg] = Em.badge;
	Kilp.tallenna(DKilp, 0, -1, 0, 0);
	if (Kilp_0.pv[k_pv].vac == 'V') {
		int kno_0 = Kilp_0.id();
		for (int ipv = k_pv; ipv < kilpparam.n_pv_akt; ipv++) {
			Kilp_0.pv[ipv].nollaa();
			Kilp_0.set_tark(L'V', ipv);
			Kilp_0.setId(kno_0, ipv);
			}
		Kilp_0.setSarjaAll(kilpparam.vakanttisarja);
		_itow(Kilp_0.id(), Kilp_0.etunimi, 10);
		vakantilta = true;
		}
	else {
		Kilp_0.pv[k_pv].badge[0] = 0;
		Kilp_0.pv[k_pv].badge[1] = 0;
		Kilp_0.pv[k_pv].laina[0] = 0;
		Kilp_0.pv[k_pv].laina[1] = 0;
		for (int i_pv = k_pv + 1; i_pv < kilpparam.n_pv_akt; i_pv++)
			Kilp_0.pv[i_pv].badge[0] = 0;
		}
	Kilp_0.tallenna(DKilp_0, -1, 0, 0, 0);
	LeaveCriticalSection(&tall_CriticalSection);
	Em.get(Tietue);
	if (kilpVapaa) {
		int tulkinta[MAXNLEIMA+1];
		if (kilpVapaa & 1)
			Kilp.tall_tulos(0, LaskeTulos());
		if (kilpVapaa & 6) {
			int srj1;

			if (autosarja && (srj1 = etsiesarja(&Em)) >= 0)
				Kilp.setSarja(srj1);
			memset(tulkinta, 0, sizeof(tulkinta));
			if (kilpparam.kaksibadge < 2 || Kilp.pv[k_pv].badge[1] == 0) {
				if (tarkista(&Em, tulkinta, 0, &Kilp) == 0)
					Kilp.set_tark(L'T', k_pv);
				else
					Kilp.set_tark(L'I', k_pv);
				}
			else {
				if (tarkista(&Em, tulkinta, 0, &Kilp) == 0)
					Kilp.pv[k_pv].asakot[Em.osuus] = L'T';
				else
					Kilp.pv[k_pv].asakot[Em.osuus] = L'I';
				switch (Kilp.pv[k_pv].asakot[1-Em.osuus]) {
					case L'I' :
						Kilp.set_tark(L'I', k_pv);
						break;
					case L'T' :
						Kilp.set_tark(Kilp.pv[k_pv].asakot[Em.osuus], k_pv);
						break;
					case L'-' :
						Kilp.set_tark(L'-', k_pv);
						break;
					}
				}
			}
		if (kuntosuunnmoodi && kuntosuunnmoodi != 99 &&
			(kuntosuunnmoodi == 2 || kuntosuunnmoodi == 3 ||
			(Kilp.pv[k_pv].uusi != 'U' && getEmitJarr(Kilp.id(), 0) >= 0)) &&
			Kilp.pv[k_pv].badge[1] == 0 && Em.badge < BADGEASKEL) {
			Kilp.pv[k_pv].badge[1] = Em.badge;
			Em.badge = haeSeurVapaaBadge(Em.badge);
			Em.kilpno = Kilp.id();
			putem(&Em, Tietue, 0);
			Kilp.pv[k_pv].badge[0] = Em.badge;
			Kilp.pv[k_pv].laina[1] = Kilp.pv[k_pv].laina[0];
			}
		EnterCriticalSection(&tall_CriticalSection);
		Kilp.tallenna(DKilp, 0, -1, 0, 0);
		LeaveCriticalSection(&tall_CriticalSection);
		if (emittulparam && autorap > 9 && Em.kilpno == Kilp.id() &&
			(autorap > 1 || Kilp.tark(k_pv) == L'I' || autorapsrj[Kilp.Sarja()]))
			emit_rap(emittulparam, Em.kilpno);
		if (emittulparam && autorap < 0 && Em.kilpno == Kilp.id())
			tulostaRastiVa(emittulparam, Em.kilpno);
		if (emitanal_fl && vakantilta && Em.osuus == 0)
		   addemitanal(&Em, 1);
		}
	else {
		Application->MessageBoxW(L"Kilpailijan tulosta ja leimantarkastustietoa ei "
			L"muutettu vastaamaan siirrettyjä leimoja. Aiemmat tiedot säilytetty kilpailijalla",
			L"Varoitus", MB_OK);
		}
	if (kno) {
		Application->MessageBoxW((L"Vanhat tiedot siirretty vakantille "+UnicodeString(kno)).c_str(),
			 L"Kaksi siirtoa tehty", MB_OK);
		}
	Nayta();
	TallVals();
	KasittelyKesken = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::MerkitseOhitetuksi(void)
{
	EnterCriticalSection(&tall_CriticalSection);
	if (Kilp_0.pv[k_pv].vac == 'V') {
		int kno_0 = Kilp_0.id();
		for (int ipv = k_pv; ipv < kilpparam.n_pv_akt; ipv++) {
			Kilp_0.pv[ipv].nollaa();
			Kilp_0.set_tark(L'V', ipv);
			Kilp_0.setId(kno_0, ipv);
			Kilp_0.setSarja(kilpparam.vakanttisarja, ipv);
			}
		_itow(Kilp_0.id(), Kilp_0.etunimi, 10);
		}
	else {
		Kilp_0.pv[k_pv].badge[0] = 0;
		Kilp_0.pv[k_pv].badge[1] = 0;
		Kilp_0.pv[k_pv].laina[0] = 0;
		Kilp_0.pv[k_pv].laina[1] = 0;
		}
	Kilp_0.tallenna(DKilp_0, -1, 0, 0, 0);
	LeaveCriticalSection(&tall_CriticalSection);
	Em.kilpno = 0;
	Em.badge = 0;
	Em.put(Tietue, false);
	Nayta();
	TallVals();
	KasittelyKesken = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::TallennaVaihto(int d)
{
	int kno, i_ptr;
	int vd;
	kilptietue vkilp, kilp;

	if (d == 0 || DKilp_0 == d) {
		Application->MessageBoxW(L"Ei tallennettavaa vaihtoa", L"", MB_OK);
		return;
		}
	kno = haeSeurVakantti(0);
	if (kno == 0) {
		Application->MessageBoxW(L"Ei vaihtoon tarvittavaa vakanttia, muutos perutaan", L"", MB_OK);
		return;
		}
	EnterCriticalSection(&tall_CriticalSection);
	vd = getpos(kno);
	kilp.GETREC(d);
	vkilp.GETREC(vd);
	vkilp.pv[k_pv].badge[0] = kilp.pv[k_pv].badge[0];
	vkilp.pv[k_pv].badge[1] = kilp.pv[k_pv].badge[1];
	vkilp.pv[k_pv].laina[0] = kilp.pv[k_pv].laina[0];
	vkilp.pv[k_pv].laina[1] = kilp.pv[k_pv].laina[1];
	vkilp.tallenna(vd, 0, -1, 0, 0);
	kilp.pv[k_pv].badge[0] = Kilp_0.pv[k_pv].badge[0];
	kilp.pv[k_pv].badge[1] = Kilp_0.pv[k_pv].badge[1];
	kilp.pv[k_pv].laina[0] = Kilp_0.pv[k_pv].laina[0];
	kilp.pv[k_pv].laina[1] = Kilp_0.pv[k_pv].laina[1];
	if (Emitvaihdottuleville->Checked) {
		for (int i_pv = k_pv + 1; i_pv < kilpparam.n_pv_akt; i_pv++)
			kilp.pv[i_pv].badge[0] = Kilp_0.pv[k_pv].badge[0];
		}
	kilp.tallenna(d, 0, -1, 0, 0);
	Kilp_0.pv[k_pv].badge[0] = vkilp.pv[k_pv].badge[0];
	Kilp_0.pv[k_pv].badge[1] = vkilp.pv[k_pv].badge[1];
	Kilp_0.pv[k_pv].laina[0] = vkilp.pv[k_pv].laina[0];
	Kilp_0.pv[k_pv].laina[1] = vkilp.pv[k_pv].laina[1];
	if (Emitvaihdottuleville->Checked) {
		for (int i_pv = k_pv + 1; i_pv < kilpparam.n_pv_akt; i_pv++)
			Kilp_0.pv[i_pv].badge[0] = vkilp.pv[k_pv].badge[0];
		}
	Kilp_0.tallenna(DKilp_0, 0, -1, 0, 0);
	vkilp.set_tark(L'V', k_pv);
	vkilp.pv[k_pv].badge[0] = 0;
	vkilp.pv[k_pv].badge[1] = 0;
	vkilp.pv[k_pv].laina[0] = 0;
	vkilp.pv[k_pv].laina[1] = 0;
	vkilp.tallenna(vd, 0, -1, 0, 0);
	LeaveCriticalSection(&tall_CriticalSection);
	Em.get(Tietue);
	DKilp = getpos(Em.kilpno);
	Kilp.GETREC(DKilp);
	DKilp_0 = DKilp;
	Kilp_0 = Kilp;
	Nayta();
	TallVals();
	KasittelyKesken = false;
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
	for (int r = EmitGrid->RowCount; r < MAXNLEIMA; r++) {
		Em.ctrlcode[r-1] = 0;
		Em.ctrltime[r-1] = 0;
		}
	Em.put(Tietue, true);
	Nayta();
	TallVals();
	KasittelyKesken = false;
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
	for (int i = 0; i < MAXNLEIMA; i++)
	   Em.ctrltime[i] /= 2;
	Nayta();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::KasvataKortinAjat(int lisa)
{
	for (int i = 0; i < MAXNLEIMA; i++)
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

void __fastcall TFormEmit::Vahvistaaina1Click(TObject *Sender)
{
	Vahvistaaina1->Checked = !Vahvistaaina1->Checked;
	vahvistaaina = Vahvistaaina1->Checked;
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
	int tl = Kilp.TLahto();
	Kilp.pv[k_pv].badge[0] = 0;
	Kilp.set_tark(L'E', k_pv);
	Kilp.tall_lajat_pv(TMAALI0, k_pv);
	Kilp.tallenna(DKilp, 0, 0,0, 0);
	Kilp.nollaa();
	KasittelyKesken = False;
	DKilp = 0;
	if (kuntosuunnmoodi == 99) {
		Em.kilpno = 0;
		Em.badge = Em.badge0;
		UusiTietue = true;
		MemoEmit->Visible = false;
		MemoKno->Visible = true;
		Paivita(&Em);
		FocusControl(EdtKilpno);
		}
	else {
	Kilp.tall_lajat_pv(tl, k_pv);
		Nayta();
		TallVals();
		}
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
			emit_rap(&EmitTulPrm, Em.kilpno);
			break;
		case 2:
			tark_rap(&EmitTulPrm, Em.kilpno);
			break;
		case 3:
			emitlista(&EmitTulPrm);
			break;
		case 4:
			tulostaRastiVa(&EmitTulPrm, Em.kilpno);
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

	if (Sarjat[Kilp.Sarja()].rrtas < 2 || Sarjat[Kilp.Sarja()].tsak[k_pv] == 0)
		return;
//	LaskeTulos();
	sk = _wtoi(EdtSakkoja->Text.c_str());
	ero = SEK*sk*Sarjat[Kilp.Sarja()].tsak[k_pv] - Kilp.sakko_pv(k_pv);
	if (Kilp.p_aika(0)) {
		AIKATOWSTRS(st, Kilp.p_aika(0)+ero, 0);
		st[pvparam[k_pv].laika] = 0;
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
	Tulosta(1);
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
	int rr, vast, srj = -1;

	rr = etsierata(&Em);
	if (rr < 0) {
		Application->MessageBoxW(L"Leimat eivät vastaa mitää rataa", L"Ei löydy", MB_OK);
		}
	else {
		wchar_t msg[200];
		if ((srj = haeradansarja(rata[rr].tunnus, NULL)) >= 0)
			swprintf(msg, L"Leimat vastaavat sarjaa %s", Sarjat[srj].sarjanimi);
		else
			swprintf(msg, L"Leimat vastaavat rataa %s", rata[rr].tunnus);
		vast = select3(2, msg, L"Löytyi", L"Vaihda rata kilpailijalle", L"Jätä rata ennalleen", L"", this);
		if (vast == 1) {
			if (srj < 0) {
				CRata->ItemIndex = rr+1;
				CRataChange(Sender);
				}
			else {
				CSarja->ItemIndex = srj+1;
				CRata->ItemIndex = 0;
				CSarjaChange(Sender);
				}
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
	kuntoMaali = Maalintietokone1->Checked;
	if (Maalintietokone1->Checked && Lhdntietokone1->Checked) {
		Lhdntietokone1->Checked = false;
		kuntoLahto = false;
		InitTila();
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit::Lhdntietokone1Click(TObject *Sender)
{
	Lhdntietokone1->Checked = !Lhdntietokone1->Checked;
	kuntoLahto = Lhdntietokone1->Checked;
	if (Lhdntietokone1->Checked) {
		Maalintietokone1->Checked = false;
		kuntoMaali = false;
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
	Maalintietokone1->Checked = kuntoMaali;
	if (kuntoMaali)
		kuntoLahto = false;
	Lhdntietokone1->Checked = kuntoLahto;
	if (esiluenta == 2) {
		DKilp = 0;
		Em.nollaa();
		Nayta();
		}
	else if (Tietue <= 0) {
		Tietue = em_HaeViimeinen();
		HaeTietue(Tietue, 0);
		if (DKilp) {
			Kilp.GETREC(DKilp);
			Nayta();
			TallVals();
			}
		}
	InitTila();
	if (!Initialisoitu) {
		if (kuntosuunnmoodi == 1 || kuntosuunnmoodi == 2) {
			if (NimiHTHaku == -1)
				NimiHTHaku = 1;
			if (seurahaku == 2) {
				Seuraluettelo1->Checked = true;
				}
			}
		if (NimiHTHaku == 1) {
			Henkiltietokanta1->Checked = true;
			}
		if (NimiHTHaku == 1) {
			FormTKanta->Caller = FormEmit;
			FormTKanta->Show();
			FormTKanta->Left = Left + Width;
			FormTKanta->Top = 20;
			}
		if (Seuraluettelo1->Checked) {
			FormSeurat->Caller = FormEmit;
			FormSeurat->Show();
			FormSeurat->Left = Left + Width;
			FormSeurat->Top = 300*Screen->PixelsPerInch/96;
			}
		}
	Initialisoitu = true;
	FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Kythenkiltietokannansarjaa1Click(TObject *Sender)
{
	Kythenkiltietokannansarjaa1->Checked = !Kythenkiltietokannansarjaa1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Laskeuudelleen1Click(TObject *Sender)
{
	bool muutahyv = false, muutahyl = false;
	int vast;
	INT16 ibdg;

	if (Application->MessageBoxW(L"Vahvista kaikkien aikojen uudelleenlaskenta. Virheettömät avoimet ja hylkäysesityksen saaneet hyväksytään samalla.", L"Vahvista", MB_YESNO) != IDYES)
		return;
	vast = Application->MessageBoxW(L"Merkitäänkö samalla hyväksyntä myös virheettömille hylätyille ja hylkäysesitys leimausvirheille.", L"Tarkastusmerkintä", MB_YESNOCANCEL);
	switch (vast) {
		case IDYES:
			muutahyv = true;
			muutahyl = true;
			break;
		case IDCANCEL:
			return;
		}
	for (int i_ptr = 0; i_ptr < emithead2; i_ptr++) {
		emittp em, em2;
		em.get(i_ptr);
		ibdg = 0;
		if (em.osuus)
			continue;
		if (em.badge && em.kilpno && getEmitJarr(em.kilpno, &ibdg) == i_ptr) {
			kilptietue kilp;
			int d, tm, tlahto = TMAALI0;

			d = getpos(em.kilpno);
			if (d <= 0)
				continue;
			kilp.GETREC(d);
			tm = e_maaliaika(&em, &kilp, &tlahto);
			if (tarkista(&em, 0, 0, &kilp) == 0) {
				if (kilpparam.kaksibadge == 2)
					kilp.pv[k_pv].asakot[0] = L'T';
				if (wcswcind(kilp.tark(), L"-I") >= 0 || muutahyl)
					kilp.set_tark(L'T');
				}
			else if (kilp.tark() == L'-' || (kilp.tark() == L'T' && muutahyv)) {
				if (kilpparam.kaksibadge == 2)
					kilp.pv[k_pv].asakot[0] = L'I';
				kilp.set_tark(L'I');
				}
			if (kilpparam.kaksibadge == 2 && kilp.pv[k_pv].badge[1]) {
				if (em2.getbykno(em.kilpno, 1) >= 0) {
					int tlahto2, tm2;

					tm2 = e_maaliaika(&em2, &kilp, &tlahto2);
					if (tm2 == TMAALI0)
						tm = TMAALI0;
					else if (NORMKELLO(tm2 - tm) > 0)
						tm = tm2;
					}
				else
					tm = TMAALI0;

				if (muutahyv) {
					if (tarkista(&em2, 0, 0, &kilp) == 0) {
						kilp.pv[k_pv].asakot[1] = L'T';
						if (kilp.pv[k_pv].asakot[0] == L'T' &&
							(wcswcind(kilp.tark(), L"-I") >= 0 || muutahyl))
							kilp.set_tark(L'T');
						}
					else {
						kilp.pv[k_pv].asakot[1] = L'I';
						if (kilp.tark() == L'-' || (kilp.tark() == L'T' && muutahyv))
							kilp.set_tark(L'I');
						}
					}
				}
			if (tm != TMAALI0) {
				if (tlahto != TMAALI0) {
					kilp.tall_lajat_pv(tlahto, k_pv);
					tm = (tm + 36*TUNTI) % (24*TUNTI) - 12*TUNTI;
					}
				kilp.set_tulos(0, tm);
				if (kilp.p_aika(0, 0))
					kilp.tallenna(d, 0, 0, 0, 0);
				}
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtSNimiKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key < L' ' && Key != 8) {
	   Key = 0;
	   }
	else if (EdtSNimi->Text.Length() == 0 && Key >= L'a')
		Key = towupper2(Key);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtENimiKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key < L' ' && Key != 8) {
	   Key = 0;
	   }
	else if (EdtENimi->Text.Length() == 0 && Key >= L'a')
		Key = towupper2(Key);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::BtnOhitaClick(TObject *Sender)
{
	Em.kilpno = 0;
	Em.badge = 0;
	Em.put(Tietue, false);
	EKilp.tallenna(DKilp, 0, 0, 0, 0);
	Kilp.nollaa();
	KasittelyKesken = false;
	Nayta();
	TallVals();
}
//---------------------------------------------------------------------------


void __fastcall TFormEmit::BtnNaytaKarttaClick(TObject *Sender)
{
	ratatp *rt;
	int tulkinta[MAXNLEIMA];
	wchar_t st[60];

	if (!DKilp || (rt = haerata(&Kilp)) == NULL)
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
	tarkista(&Em, tulkinta, 0, &Kilp);
	FormKartta->naytaLeimat(rt, tulkinta, Em.ctrlcode);
	FormKartta->NaytaOtsikko(UnicodeString(Kilp.id())+L" - "+Kilp.nimi(st, 40, false)+L" - Rata: "+rt->tunnus);
	if (FormKartta->WindowState == wsMinimized)
		FormKartta->WindowState = wsNormal;
	FormKartta->BringToFront();
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

void __fastcall TFormEmit::BtnTyhjSanomaClick(TObject *Sender)
{
	EdtMessage->Text = L"";
	EdtMessage->Color = clBtnFace;
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


void __fastcall TFormEmit::EdtTark1Exit(TObject *Sender)
{
	if (EdtTark1->Text.Length() == 0 || wcswcind(EdtTark1->Text.c_str()[0], L"THKIthki") < 0)
		EdtTark1->Text = L"-";
	EdtTark1->Text = EdtTark1->Text.UpperCase();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EdtTark2Exit(TObject *Sender)
{
	if (EdtTark2->Text.Length() == 0 || wcswcind(EdtTark2->Text.c_str()[0], L"THKIthki") < 0)
		EdtTark2->Text = L"-";
	EdtTark2->Text = EdtTark2->Text.UpperCase();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Lisrivi1Click(TObject *Sender)
{
	if (EmitGrid->Row < 1)
		return;
	for (int r = MAXNLEIMA-1; r >= EmitGrid->Row; r--) {
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
	for (int r = EmitGrid->Row-1; r < MAXNLEIMA-1; r++) {
		Em.ctrlcode[r] = Em.ctrlcode[r+1];
		Em.ctrltime[r] = Em.ctrltime[r+1];
		}
	Em.ctrlcode[MAXNLEIMA-1] = 0;
	Em.ctrltime[MAXNLEIMA-1] = 0;
	Nayta();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::BtnPiikitClick(TObject *Sender)
{
	if (!FormPiikit) {
		FormPiikit = new TFormPiikit(this);
		}
	FormPiikit->KilpNo = Kilp.id();
	FormPiikit->Show();
	if (FormPiikit->WindowState == wsMinimized)
		FormPiikit->WindowState = wsNormal;
	FormPiikit->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Kapealeimausraportti1Click(TObject *Sender)
{
	Kapealeimausraportti1->Checked = !Kapealeimausraportti1->Checked;
	emitsivu = Kapealeimausraportti1->Checked ? -1 : 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Kirjoitakopioemittiedoista1Click(TObject *Sender)
{
	static wchar_t EFName[200] = L"EmitCopy.dat";

	SaveDialog1->FileName = EFName;
	SaveDialog1->DefaultExt = L"dat";
	SaveDialog1->Filter = L"dat-tiedostot (*.dat)|*.DAT|Kaikki tiedostot (*.*)|*.*";
	if (SaveDialog1->Execute()) {
		FILE *efl;

		wcsncpy(EFName, SaveDialog1->FileName.c_str(), 198);
		if ((efl = _wfopen(EFName, L"wb")) != NULL) {
			EnterCriticalSection(&emit_CriticalSection);
			_lseek(emitfile, 0, SEEK_SET);
			for (int i = 0; i < emithead; i++) {
				emittp em;
				if ((_read(emitfile, &em, sizeof(emittp))) < sizeof(emittp))
					break;
				if (fwrite(&em, sizeof(emittp), 1, efl) != 1)
					break;
				}
			LeaveCriticalSection(&emit_CriticalSection);
			fclose(efl);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::EmitvaihdottulevilleClick(TObject *Sender)
{
	Emitvaihdottuleville->Checked = !Emitvaihdottuleville->Checked;
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
	KasittelyKesken = false;
	CBViim->Checked = true;
	HaeTietue(emithead2-1, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::VaihdaluentaajatClick(TObject *Sender)
{
	bool muutahyv = false, muutahyl = false;
	int vast;
	INT16 ibdg;

	if (Application->MessageBoxW(L"Vahvista kaikkien luenta-aikojen vaihtaminen.", L"Vahvista", MB_YESNO) != IDYES)
		return;
	for (int i_ptr = 0; i_ptr < emithead2; i_ptr++) {
		emittp em, em2;
		em.get(i_ptr);
		ibdg = 0;
		if (em.osuus)
			continue;
		if (em.badge && em.kilpno && getEmitJarr(em.kilpno, &ibdg) == i_ptr) {
			kilptietue kilp;
			int d, tm, tlahto = TMAALI0;

			d = getpos(em.kilpno);
			if (d <= 0)
				continue;
			kilp.GETREC(d);
			tm = kilp.maali(0);
//			tm = e_maaliaika(&em, &kilp, &tlahto);
			if (tm != TMAALI0 && em.maali != TMAALI0/KSEK && em.time != TMAALI0/KSEK) {
				em.time = NORMKELLO(KSEK*(em.time-em.maali)+tm)/KSEK;
				em.maali = tm/KSEK;
				em.put(i_ptr, false);
				}
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::Listietueksiteltvksi1Click(TObject *Sender)
{
	FormLisaTietue = new TFormLisaTietue(this);
	FormLisaTietue->ShowModal();
	delete FormLisaTietue;
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


void __fastcall TFormEmit::Salanimienkasittely1Click(TObject *Sender)
{
	Salanimienkasittely1->Checked = !Salanimienkasittely1->Checked;
	CBSelSN->Visible = Salanimienkasittely1->Checked;
	EdtSelitys->Visible = Salanimienkasittely1->Checked || !esiluenta;
	Label8->Visible = EdtSelitys->Visible && !CBSelSN->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmit::BtnHylkRapClick(TObject *Sender)
{
	if (!FormHylkRap)
		FormHylkRap = new TFormHylkRap(this);

	FormHylkRap->NaytaKilpailija(Kilp.id(), Em.badge0);
	FormHylkRap->Show();
	if (FormHylkRap->WindowState == wsMinimized)
		FormHylkRap->WindowState = wsNormal;
	FormHylkRap->BringToFront();
}
//---------------------------------------------------------------------------

