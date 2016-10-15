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
#include "UnitKilpMaaritys.h"
#include "UnitVaiheMaar.h"
#include "HkDeclare.h"
#include "HkMuotoilu.h"
#include "WinHk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern TFormKilpMaaritys *FormKilpMaaritys;
extern TFormMain *FormMain;

static wchar_t TunnType[] = L" ETSO", orig_laji, orig_alalajit[N_PV+1];;
static int orig_n_pv;
static wchar_t YhtTlsLaji[] = L"SLPV";

//---------------------------------------------------------------------------
__fastcall TFormKilpMaaritys::TFormKilpMaaritys(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	Width = RGSarja->Left + RGSarja->Width + 21 * Screen->PixelsPerInch / 96;
	CBVaihe[0] = CBVaihe1;
	CBVaihe[1] = CBVaihe2;
	CBVaihe[2] = CBVaihe3;
	CBVaihe[3] = CBVaihe4;
	CBVaihe[4] = CBVaihe5;
	Label3->Caption = L"Vaiheiden tilavaraus (version max. "+UnicodeString(N_PV)+L")";
	Label4->Caption = L"Väliaikojen tilavaraus (version max. "+UnicodeString(VALUKU)+L")";
}
//---------------------------------------------------------------------------
void __fastcall TFormKilpMaaritys::ButtonOKClick(TObject *Sender)
{
	hyvaksyMuutokset = 1;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormKilpMaaritys::NaytaVaiheet(void)
{
	int k;
	PanelMuuLaji->Visible = true;
	Width = PanelMuuLaji->Left + PanelMuuLaji->Width + 11 * Screen->PixelsPerInch / 96;
	if (RGMuuLaji->ItemIndex == 0) {
		for (int i = 0; i < kilpparam.n_pv_akt; i++) {
			if ((k = wcswcind(pvparam[i].alalaji, L"HOWTC")) >= 0)
				CBVaihe[i]->ItemIndex = k;
			}
		}
	if (RGMuuLaji->ItemIndex == 1) {
		for (int i = 0; i < kilpparam.n_pv_akt; i++) {
			if ((k = wcswcind(pvparam[i].alalaji, L"OLUFC")) >= 0)
				CBVaihe[i]->ItemIndex = k;
			}
		}
	if (RGMuuLaji->ItemIndex == 2) {
		for (int i = 0; i < 5; i++) {
			CBVaihe[i]->ItemIndex = wcswcind(kilpparam.asuunnlaji[i], L"SLO") + 1;
			}
		}
}
//---------------------------------------------------------------------------

int PyorIx(int pyor)
{
	int i;
	if (pyor == 0)
		pyor = 1;
	pyor = SEK/pyor;
	for (i = 0; i < 3; i++) {
		if (pyor < 10)
			break;
		pyor /= 10;
		}
	return(i);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::NaytaTarkkuudet(void)
{
	int k;
	CBTarkkuus->ItemIndex = PyorIx(pvparam[k_pv].pyor[1]);
	k = PyorIx(pvparam[0].pyor[2]);
	for (int i = 1; i < kilpparam.n_pv_akt; i++)
		if (PyorIx(pvparam[i].pyor[2]) != PyorIx(pvparam[0].pyor[2]))
			k = PyorIx(pvparam[i].pyor[2]);
	if (k != PyorIx(pvparam[0].pyor[2]))
		CBJrjTarkkuus->ItemIndex = 4;
	else
		CBJrjTarkkuus->ItemIndex = PyorIx(pvparam[k_pv].pyor[2]);
	k = PyorIx(pvparam[0].pyor[3]);
	for (int i = 1; i < kilpparam.n_pv_akt; i++)
		if (PyorIx(pvparam[i].pyor[3]) != PyorIx(pvparam[0].pyor[3]))
			k = PyorIx(pvparam[i].pyor[3]);
	if (k != PyorIx(pvparam[0].pyor[3]))
		CBTulTarkkuus->ItemIndex = 4;
	else
		CBTulTarkkuus->ItemIndex = PyorIx(pvparam[k_pv].pyor[3]);
	CBTakAjoPyor->ItemIndex = PyorIx(pvparam[k_pv].pyor[4]);
	CBVaTarkkuus->ItemIndex = PyorIx(pvparam[k_pv].pyor[5]);
	k = 0;
	for (int i = 1; i < kilpparam.n_pv_akt; i++)
		if (pvparam[i].hiihtolahto != pvparam[0].hiihtolahto)
			k = 1;
	if (k == 0) {
		CBLahtoportti->Enabled = true;
		CBLahtoportti->Checked = pvparam[0].hiihtolahto;
		}
	else {
		CBLahtoportti->Enabled = false;
		CBLahtoportti->Checked = pvparam[k_pv].hiihtolahto;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::NaytaArvot(void)
{
	RGSarja->ItemIndex = kilpparam.sarjavaihtelee ? 1 : 0;
	RGBib->ItemIndex = kilpparam.bibvaihtelee ? 1 : 0;
	RGSeura->ItemIndex = kilpparam.seuralyh_on ?  1 : 0;
	EditKilpailu->Text = UnicodeString(kilpailu);
	EditTiedPv->Text = UnicodeString(kilpparam.n_pv);
	EditTiedVa->Text = UnicodeString(kilpparam.valuku);
	EditPv->Text = UnicodeString(kilpparam.n_pv_akt);
	EdtSakko->Text = kilpparam.sakkoyks;
	EdtMaxSakko->Text = kilpparam.maxasakko;
	CBTunnistin->ItemIndex = wcswcind(kilpparam.badgelaji, TunnType);
	if (CBTunnistin->ItemIndex < 0)
		CBTunnistin->ItemIndex = 0;
	CB2badge->Checked = kilpparam.kaksibadge == 1;
	CBYhtTulos->ItemIndex = wcswcind(kilpparam.yhttlslaji, YhtTlsLaji);
	if (CBYhtTulos->ItemIndex < 0)
		CBYhtTulos->ItemIndex = 0;
	if (!kilpparam.kilpkoodi[0])
		wcscpy(kilpparam.kilpkoodi, L"0000");
	EditKoodi->Text = UnicodeString(kilpparam.kilpkoodi);
	if (kilpparam.vuosi < 1900) {
		time_t clock;
		struct tm *now;
		time(&clock);
		now = localtime(&clock);
		kilpparam.vuosi = now->tm_year + 1900;
		}
	EditVuosi->Text = UnicodeString(kilpparam.vuosi);
	switch (kilpparam.kilplaji) {
		case L'S':
			RGlaji->ItemIndex = 0;
			break;
		case L'H':
			RGlaji->ItemIndex = 1;
			break;
		case L'J':
			RGlaji->ItemIndex = 2;
			break;
		case L'A':
			RGlaji->ItemIndex = 3;
			break;
		case L'U':
			RGlaji->ItemIndex = 4;
			break;
		case L'N':
			RGlaji->ItemIndex = 5;
			RGMuuLajiClick(this);
			RGMuuLaji->ItemIndex = 1;
			NaytaVaiheet();
			break;
		case L'P':
			RGlaji->ItemIndex = 5;
			RGMuuLajiClick(this);
			RGMuuLaji->ItemIndex = 0;
			NaytaVaiheet();
			break;
		case L'B':
			RGlaji->ItemIndex = 5;
			RGMuuLajiClick(this);
			RGMuuLaji->ItemIndex = 2;
			NaytaVaiheet();
			break;
		default:
			RGlaji->ItemIndex = 5;
			break;
		}
	RGtasmennys->ItemIndex = wcswcind(kilpparam.alalaji, L"SEDA") + 1;
	NaytaTarkkuudet();
}
//---------------------------------------------------------------------------
void __fastcall TFormKilpMaaritys::Button1Click(TObject *Sender)
{
	hyvaksyMuutokset = -1;
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormKilpMaaritys::Button2Click(TObject *Sender)
{
	wchar_t pathStr[MAX_PATH];

	wcsncpy(pathStr, FormMain->CurrentDir.c_str(), MAX_PATH);
	if (!ValitseHakemisto(pathStr, MAX_PATH)) {
		EditHakem->Text = pathStr;
		SetCurrentDir(pathStr);
		FormMain->CurrentDir = GetCurrentDir()+UnicodeString(L"\\");
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::BtnLueSrjClick(TObject *Sender)
{
	OpenDialog1->FileName = UnicodeString(L"KilpSrj.xml");
	OpenDialog1->DefaultExt = L"xml";
	OpenDialog1->Filter = L"XML-tiedostot (*.xml)|*.XML|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		lueEventXml(OpenDialog1->FileName.c_str(), true, false, false);
		orig_n_pv = kilpparam.n_pv_akt;
		if (sarjaluku > 0) {
			CBSarjat->Checked = false;
			GBSrj->Enabled = false;
			BtnLueSrj->Enabled = false;
			}
		}
	NaytaArvot();
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Kilpailun luominen");
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::EditTiedPvExit(TObject *Sender)
{
	int n;

	n = _wtoi(EditTiedPv->Text.c_str());
	if (ToimintaTila > 0 && n != kilpparam.n_pv) {
		Application->MessageBox(L"Vaiheiden tilavarausta ei voi muuttaa, kun kilpailijatiedot on jo luotu",
			L"Rajoitus", MB_OK);
		EditTiedPv->Text = kilpparam.n_pv;
		}
	else if (n < 1 || n > N_PV) {
		Application->MessageBox(L"Vaiheiden tilavaraus ei sallitulla alueella", L"Virhe",
			MB_OK);
		EditTiedPv->Text = kilpparam.n_pv;
		}
	else {
		if (_wtoi(EditPv->Text.c_str()) > n) {
			Application->MessageBoxW((L"Kilpailun aktiivisten vaiheiden lukumääräksi asetettu "+UnicodeString(n)).c_str(),
				L"Muutos", MB_OK);
			}
		EditPv->Text = n;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::EditTiedVaExit(TObject *Sender)
{
	int n;

	n = _wtoi(EditTiedVa->Text.c_str());
	if (ToimintaTila > 0 && n != kilpparam.valuku) {
		Application->MessageBox(L"Vaiheiden tilavarausta ei voi muuttaa, kun kilpailijatiedot on jo luotu",
			L"Rajoitus", MB_OK);
		EditTiedVa->Text = kilpparam.valuku;
		}
	if (n < 1 || n > VALUKU) {
		Application->MessageBox(L"Väliaikojen tilavaraus ei sallitulla alueella", L"Virhe",
			MB_OK);
		EditTiedVa->Text = kilpparam.valuku;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::RGlajiClick(TObject *Sender)
{
	if (inInit)
		return;
	CBTakAjoPyor->ItemIndex = 0;
	CB2badge->Checked = false;
	if (RGlaji->ItemIndex == 0) {
		CBTarkkuus->ItemIndex = RGtasmennys->ItemIndex == 1 ? 1 : 0;
		CBJrjTarkkuus->ItemIndex = 0;
		CBVaTarkkuus->ItemIndex = 0;
		CBTulTarkkuus->ItemIndex = 0;
		CBLahtoportti->Checked = false;
		RGSeura->ItemIndex = RGtasmennys->ItemIndex == 2 ? 0 : 1;
		EdtSakko->Text = RRSAKKO;
		CBTunnistin->ItemIndex = 1;
		PanelMuuLaji->Visible = false;
		Width = RGSarja->Left + RGSarja->Width + 21 * Screen->PixelsPerInch / 96;
		}
	else if (RGlaji->ItemIndex == 4) {
		CBTarkkuus->ItemIndex = 2;
		CBJrjTarkkuus->ItemIndex = CBTarkkuus->ItemIndex;
		CBVaTarkkuus->ItemIndex = CBTarkkuus->ItemIndex;
		CBTulTarkkuus->ItemIndex = CBTarkkuus->ItemIndex;
		CBLahtoportti->Checked = true;
		RGSeura->ItemIndex = RGtasmennys->ItemIndex == 0;
		CBTunnistin->ItemIndex = 0;
		Width = RGSarja->Left + RGSarja->Width + 21 * Screen->PixelsPerInch / 96;
		}
	else if (RGlaji->ItemIndex == 5) {
		if (!sotilas && wcswcind(kilpparam.kilplaji, L"NPB") < 0) {
			wchar_t st[20] = L"", ch = 0;
			inputstr_prompt(st, 19, L"Anna lisätoimintojen tunnus", &ch, this);
			if (wmemcmpU(st, L"SOTILASLA", 9) == 0)
				sotilas = 1;
			}
		CBTarkkuus->ItemIndex = RGMuuLaji->ItemIndex == 1;
		CBJrjTarkkuus->ItemIndex = CBTarkkuus->ItemIndex;
		CBVaTarkkuus->ItemIndex = CBTarkkuus->ItemIndex;
		CBTulTarkkuus->ItemIndex = CBTarkkuus->ItemIndex;
		CBLahtoportti->Checked = true;
		RGSeura->ItemIndex = RGtasmennys->ItemIndex == 0;
		EdtSakko->Text = 1;
		CBTunnistin->ItemIndex = 0;
		if (sotilas) {
			if (RGMuuLaji->ItemIndex < 0)
				RGMuuLaji->ItemIndex = 2;
			PanelMuuLaji->Visible = true;
			Width = PanelMuuLaji->Left + PanelMuuLaji->Width + 11 * Screen->PixelsPerInch / 96;
			}
		}
	else {
		if (ToimintaTila == 0 && RGlaji->ItemIndex == 1 && RGtasmennys->ItemIndex == 1 &&
			kilpparam.n_pv <= 2 && nrec == 0) {
			kilpparam.n_pv = 4;
			kilpparam.n_pv_akt = 4;
			EditTiedPv->Text = kilpparam.n_pv;
			EditPv->Text = kilpparam.n_pv_akt;
			RGBib->ItemIndex = 1;
			RGSarja->ItemIndex = 1;
			}
		CBTarkkuus->ItemIndex = 2;
		CBJrjTarkkuus->ItemIndex = RGtasmennys->ItemIndex == 1 ? 2 : 1;
		CBVaTarkkuus->ItemIndex = RGtasmennys->ItemIndex == 1 ? 2 : 1;
		CBTulTarkkuus->ItemIndex = RGlaji->ItemIndex == 2 ? 0 : CBJrjTarkkuus->ItemIndex;
		CBLahtoportti->Checked = RGlaji->ItemIndex == 1 || RGlaji->ItemIndex == 3;
		RGSeura->ItemIndex = 0;
		EdtSakko->Text = (RGlaji->ItemIndex == 3) ? 60 : 1;
		EdtMaxSakko->Text = (RGlaji->ItemIndex == 3) ? 5 : 9999;
		CBTunnistin->ItemIndex = 0;
		PanelMuuLaji->Visible = false;
		Width = RGSarja->Left + RGSarja->Width + 21 * Screen->PixelsPerInch / 96;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::RGtasmennysClick(TObject *Sender)
{
	RGlajiClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::RGMuuLajiClick(TObject *Sender)
{
	int tark[3] = {1, 1, 0};

	if (ToimintaTila > 0 && kilpparam.n_pv < 5 && RGMuuLaji->ItemIndex < 2) {
		Application->MessageBoxW(L"Kilpailu jo muodostettu alle 5-vaiheisena. "
			L"Lajin vaihto vaatii tiedoston KILP.DAT poistamisen", L"Este", MB_OK);
		RGMuuLaji->ItemIndex = 2;
		return;
		}
	CBTarkkuus->ItemIndex = 2*tark[RGMuuLaji->ItemIndex];
	CBJrjTarkkuus->ItemIndex = tark[RGMuuLaji->ItemIndex];
	CBVaTarkkuus->ItemIndex = tark[RGMuuLaji->ItemIndex];
	CBTulTarkkuus->ItemIndex = tark[RGMuuLaji->ItemIndex];
	kilpparam.pyorlahimpaan = RGMuuLaji->ItemIndex == 1;
	CBIntPenth->Visible = false;

	if (RGMuuLaji->ItemIndex == 0) {
		EditTiedPv->Text = 5;
		EditPv->Text = 5;
//		CBIntPenth->Visible = true;
		for (int i = 0; i < 5; i++) {
			CBVaihe[i]->Items->Clear();
			CBVaihe[i]->Items->Add(L"Ammunta");
			CBVaihe[i]->Items->Add(L"Esterata");
			CBVaihe[i]->Items->Add(L"Esteuinti");
			CBVaihe[i]->Items->Add(L"Heitto");
			CBVaihe[i]->Items->Add(L"Maastojuoksu");
			CBVaihe[i]->ItemIndex = i;
			}
		}
	else if (RGMuuLaji->ItemIndex == 1) {
		EditTiedPv->Text = 5;
		EditPv->Text = 5;
		for (int i = 0; i < 5; i++) {
			CBVaihe[i]->Items->Clear();
			CBVaihe[i]->Items->Add(L"Esterata");
			CBVaihe[i]->Items->Add(L"Pelastusuinti");
			CBVaihe[i]->Items->Add(L"Tehtäväuinti");
			CBVaihe[i]->Items->Add(L"Merimiestaito");
			CBVaihe[i]->Items->Add(L"Maastorata");
			CBVaihe[i]->ItemIndex = i;
			}
		}
	if (RGMuuLaji->ItemIndex == 2) {
		if (ToimintaTila == 0 && EditPv->Text == L"5") {
			EditTiedPv->Text = 2;
			EditPv->Text = 1;
			}
		for (int i = 0; i < 5; i++) {
			CBVaihe[i]->Items->Clear();
			CBVaihe[i]->Items->Add(L"Ei vaihetta");
			CBVaihe[i]->Items->Add(L"Ammunta");
			CBVaihe[i]->Items->Add(L"Kartanluku");
			CBVaihe[i]->Items->Add(L"Suunnistus");
			CBVaihe[i]->Items->Add(L"Tauko");
			CBVaihe[i]->ItemIndex = i < 3 ? i+1 : 0;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::EditPvExit(TObject *Sender)
{
	int n = _wtoi(EditPv->Text.c_str());
	if (ToimintaTila == 2 && n != kilpparam.n_pv_akt) {
		n = kilpparam.n_pv_akt;
		Application->MessageBoxW(L"Kilpailun vaiheiden lukumäärän muuttaminen on mahdollista vain esivalmistelutilassa",
			L"Rajoitus", MB_OK);
		}
	else if (n < 1) {
		n = 1;
		Application->MessageBoxW((L"Kilpailun vaiheiden lukumääräksi asetettu "+UnicodeString(n)).c_str(),
			L"Rajoitus", MB_OK);
		}
	else if (n > _wtoi(EditTiedPv->Text.c_str())) {
		n = _wtoi(EditTiedPv->Text.c_str());
		Application->MessageBoxW((L"Kilpailun vaiheiden lukumääräksi asetettu "+UnicodeString(n)+
			L". Muuta tarvittaessa tiedostomuoto ensin.").c_str(), L"Rajoitus", MB_OK);
		}
	EditPv->Text = n;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::FormShow(TObject *Sender)
{
	static int init = 0;

	inInit = true;
	kilpparam0 = kilpparam;
	orig_laji = kilpparam.kilplaji;
	for (int i = 0; i < N_PV; i++) {
		pvparam0[i] = pvparam[i];
		orig_alalajit[i] = pvparam[i].alalaji;
		}
	if (!init && ToimintaTila == 0) {
		if (kilpparam.n_pv > 2)
			kilpparam.n_pv = 2;
		if (kilpparam.valuku > 10)
			kilpparam.valuku = 10;
		init = 1;
		}
	if (ToimintaTila != 0 || sarjaluku > 0) {
		BtnLueSrj->Enabled = false;
		CBSarjat->Checked = false;
		GBSrj->Enabled = false;
		orig_n_pv = kilpparam.n_pv_akt;
		}
	hyvaksyMuutokset = 0;
	EditHakem->Text = FormMain->CurrentDir;
	NaytaArvot();
	inInit = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if (hyvaksyMuutokset == 0) {
		int vast = Application->MessageBoxW(L"Kaavake suljetaan. Hyväksytääntö tehdyt muutokset? (Cancel: Palaa kaavakkeelle)",
			L"Muutosten vahvistus", MB_YESNOCANCEL);
		switch (vast) {
			case idYes:
				hyvaksyMuutokset = 1;
				break;
			case idNo:
				hyvaksyMuutokset = -1;
				break;
			default:
				return;
			}
		}
	if (hyvaksyMuutokset < 0) {
		hyvaksyMuutokset = 0;
		kilpparam = kilpparam0;
		for (int i = 0; i < N_PV; i++)
			pvparam[i] = pvparam0[i];
		CanClose = true;
		ModalResult = mrCancel;
		}
	else {
		wchar_t lajit[] = L"SHJAUM", muutlajit[] = L"PNB", alalajit[] = L" SEDA", NSrj = 0;
		FILE *sfl;
		int laika = 8, laika2 = 8, laikasj = 8;
		int ed_n_pv_akt;
		int pohjustus = 0, vakantit = 0;

		CanClose = false;
		hyvaksyMuutokset = 0;
		kilpparam.n_pv = _wtoi(EditTiedPv->Text.c_str());
		kilpparam.valuku = _wtoi(EditTiedVa->Text.c_str());
		if (EditPv->Text.ToInt() > kilpparam.n_pv) {
			Application->MessageBox(L"Kilpailun vaiheiden lukumäärä ei voi ylittää tiedoston tilavarausta. Korjaa tiedot.", L"Virhe",
				MB_OK);
			return;
			}
		ed_n_pv_akt = kilpparam.n_pv_akt;
		if (ToimintaTila == 1 && nyilmt > 0 && EditPv->Text.ToInt() > ed_n_pv_akt) {
			pohjustus = select3(3, L"Pohjustetaanko lisätyt vaiheet? Pohjustus hävittää mahdolliset vanhat tiedot, mutta on tarpeen, ellei vanhoja tietoja ole.", L"Tallennus",
				L"Pohjusta", L"Ohita pohjustus", L"Peruuta muutosten tallennus", this);
			if (pohjustus == 3)
				return;
			if (pohjustus == 2)
				pohjustus = 0;
			}
		kilpparam.n_pv_akt = EditPv->Text.ToInt();
		wcsncpy(kilpailu, EditKilpailu->Text.c_str(), sizeof(kilpailu)/2-1);
		wcscpy(llparam.luetots0, kilpailu);
		wcsncpy(kilpparam.kilpkoodi, EditKoodi->Text.c_str(), sizeof(kilpparam.kilpkoodi)/2-1);
		kilpparam.vuosi = EditVuosi->Text.ToInt();
		if (RGlaji->ItemIndex == 5) {
			kilpparam.kilplaji = muutlajit[RGMuuLaji->ItemIndex];
			if (kilpparam.kilplaji == L'B') {
				for (int i = 0; i < 5; i++) {
					if (CBVaihe[i]->ItemIndex > 0)
						kilpparam.asuunnlaji[i] = L" SLOB"[CBVaihe[i]->ItemIndex];
					else
						kilpparam.asuunnlaji[i] = 0;
					}
				}
			for (int i = 0; i < kilpparam.n_pv_akt; i++) {
				pvparam[i].asakkolaji = 2;
				}
			}
		else
			kilpparam.kilplaji = lajit[RGlaji->ItemIndex];
		kilpparam.alalaji = alalajit[RGtasmennys->ItemIndex];
		kilpparam.sarjavaihtelee = RGSarja->ItemIndex == 1;
		kilpparam.bibvaihtelee = RGBib->ItemIndex == 1;
		kilpparam.seuralyh_on = RGSeura->ItemIndex == 1;
		kilpparam.sakkoyks = _wtoi(EdtSakko->Text.c_str());
		if (kilpparam.sakkoyks == 0)
			kilpparam.sakkoyks = 1;
		kilpparam.badgelaji = TunnType[CBTunnistin->ItemIndex];
		kilpparam.kaksibadge = CB2badge->Checked ? 1 : 0;
		kilpparam.yhttlslaji = YhtTlsLaji[CBYhtTulos->ItemIndex];
		if (kilpparam.kilplaji == L'A') {
			for (int i = 0; i < kilpparam.n_pv_akt; i++) {
				pvparam[i].asakkolaji = 1;
				}
			}
		kilpparam.maxasakko = _wtoi(EdtMaxSakko->Text.c_str());

		pvparam[k_pv].pyor[1] =  SEK/powi(10, CBTarkkuus->ItemIndex);
		if (CBJrjTarkkuus->ItemIndex < 4)
			pvparam[k_pv].pyor[2] =  SEK/powi(10, CBJrjTarkkuus->ItemIndex);
		if (CBTulTarkkuus->ItemIndex < 4)
			pvparam[k_pv].pyor[3] =  SEK/powi(10, CBTulTarkkuus->ItemIndex);
		pvparam[k_pv].pyor[4] =  SEK/powi(10, CBTakAjoPyor->ItemIndex);
		pvparam[k_pv].pyor[5] =  SEK/powi(10, CBVaTarkkuus->ItemIndex);
		for (int i = 0; i < kilpparam.n_pv; i++) {
			pvparam[i].pyor[1] = pvparam[k_pv].pyor[1];
			if (CBJrjTarkkuus->ItemIndex < 4)
				pvparam[i].pyor[2] = pvparam[k_pv].pyor[2];
			if (CBTulTarkkuus->ItemIndex < 4)
				pvparam[i].pyor[3] = pvparam[k_pv].pyor[3];
			pvparam[i].pyor[4] = pvparam[k_pv].pyor[4];
			pvparam[i].pyor[5] = pvparam[k_pv].pyor[5];
			pvparam[i].laika = CBTarkkuus->ItemIndex ? CBTarkkuus->ItemIndex + 9 : 8;
			pvparam[i].laikasj = PyorIx(pvparam[i].pyor[2]) ? PyorIx(pvparam[i].pyor[2]) + 9 : 8;
			pvparam[i].laika2 = PyorIx(pvparam[i].pyor[3]) ? PyorIx(pvparam[i].pyor[3]) + 9 : 8;
//			pvparam[i].hiihtolahto = CBLahtoportti->Checked ? 1 : 0;
			}
		SetCurrentDir(FormMain->CurrentDir);
		if ((sfl = _wfopen(L"ilmoitt.cfg", L"rt")) == NULL) {
			sfl = _wfopen(L"ilmoitt.cfg", L"wt");
			}
		fclose(sfl);
		if ((sfl = _wfopen(L"laskenta.cfg", L"rt")) == NULL) {
			sfl = _wfopen(L"laskenta.cfg", L"wt");
			}
		fclose(sfl);
		if (CBSarjat->Checked) {
			NSrj = EditSarjoja->Text.ToInt();
			if (NSrj > 0 && NSrj <= MAXSARJALUKU) {
				FILE *sfl;
				if ((sfl = _wfopen(sarjaxmlfname, L"rt")) != NULL) {
					fclose(sfl);
					if (Application->MessageBox(L"Sarjatiedosto jo olemassa, korvataanko uudella?", L"Vahvista",
						MB_YESNO) != IDYES) {
						return;
						}
					}
				sarjaluku = NSrj;
				if (kilpparam.kilplaji == L'P') {
					kilpparam.lsakmax = 2;
					for (int i = 0; i < kilpparam.n_pv_akt; i++) {
						pvparam[i].alalaji = L"HOWTC"[CBVaihe[i]->ItemIndex];
						}
					}
				if (kilpparam.kilplaji == L'N') {
					kilpparam.lsakmax = 1;
					for (int i = 0; i < kilpparam.n_pv_akt; i++) {
						pvparam[i].alalaji = L"OLUFC"[CBVaihe[i]->ItemIndex];
						}
					}
				if (kilpparam.kilplaji == L'B') {
					for (int i = 0; i < 5; i++) {
						if (CBVaihe[i]->ItemIndex > 0)
							kilpparam.asuunnlaji[i] = L" SLOB"[CBVaihe[i]->ItemIndex];
						else
							kilpparam.asuunnlaji[i] = 0;
						}
					}
				for (int i = 0; i < sarjaluku; i++) {
					Sarjat[i].nollaa();
					Sarjat[i].viimpv = kilpparam.n_pv_akt - 1;
					swprintf(Sarjat[i].sarjanimi, L"S%d", i+1);
					}
				orig_laji = kilpparam.kilplaji;
				for (int i = 0; i < kilpparam.n_pv_akt; i++)
					orig_alalajit[i] = pvparam[i].alalaji;
				kirjEventXml(sarjaxmlfname, true, false, false);
				}
			else {
				Application->MessageBox(L"Sarjalukumäärä ei sallitulla alueella", L"Virhe",
					MB_OK);
				return;
				}
			}
		if (sarjaluku > 0) {
			if (sarjaluku < MAXSARJALUKU && kilpparam.kilplaji == L'S' && !Sarjat[sarjaluku-1].vakanttisarja && haesarja_w(L"VAKANTIT", false) < 0) {
				if (ToimintaTila == 0)
					vakantit = select3(2, L"Sarjojen viimeiseksi suositellaan suunnistuksessa vakanttisarjaa. Lisätäänkö", L"Vakanttisarja",
						L"Lisää sarja", L"Älä lisää sarjaa", L"", this);
				else
					vakantit = select3(3, L"Sarjojen viimeiseksi suositellaan suunnistuksessa vakanttisarjaa ja siihen vähintään 20 kilpailijaa. Lisätäänkö", L"Vakanttisarja",
						L"Lisää sarja", L"Älä lisää sarjaa",
							(UnicodeString(L"Lisää sarja ja ") + UnicodeString(minvakanttiluku) + L" vakanttikilpailijaa").c_str(), this);
				if (vakantit != 2) {
					sarjaluku++;
					Sarjat[sarjaluku-1].nollaa();
					Sarjat[sarjaluku-1].viimpv = kilpparam.n_pv_akt - 1;
					wcscpy(Sarjat[sarjaluku-1].sarjanimi, L"VAKANTIT");
					Sarjat[sarjaluku-1].vakanttisarja = true;
					kilpparam.vakanttisarja = sarjaluku-1;
					Sarjat[sarjaluku-1].sarjaalku = 9900;
					for (int i = 0; i < kilpparam.n_pv_akt; i++)
						Sarjat[sarjaluku-1].bibalku[i] = 9900;
					}
				if (vakantit != 3)
					vakantit = 0;
				}
			if (!CBSarjat->Checked) {
				if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset sarjatiedostoon?", L"Tallennus",
					MB_YESNO) != IDYES) {
					Application->MessageBox(L"Valitse \"Poistu tallentamatta muutoksia\", jos et halua tallentaa",
						L"Rajoitus", MB_OK);
					return;
					}
				if (kilpparam.kilplaji == L'P') {
					for (int i = 0; i < kilpparam.n_pv_akt; i++)
						pvparam[i].alalaji = L"HOWTC"[CBVaihe[i]->ItemIndex];
					}
				if (kilpparam.kilplaji == L'N') {
					for (int i = 0; i < kilpparam.n_pv_akt; i++)
						pvparam[i].alalaji = L"OLUFC"[CBVaihe[i]->ItemIndex];
					}
				for (int srj = 0; srj < sarjaluku; srj++) {
					if (Sarjat[srj].viimpv >= kilpparam.n_pv_akt ||
						(Sarjat[srj].viimpv < kilpparam.n_pv_akt - 1 && Sarjat[srj].viimpv == orig_n_pv - 1))
						Sarjat[srj].viimpv = kilpparam.n_pv_akt - 1;
					if (wcswcind(kilpparam.kilplaji, L"NP") >= 0)
						Sarjat[srj].AlustaKaavat(orig_laji, orig_alalajit);
					}
				orig_laji = kilpparam.kilplaji;
				for (int i = 0; i < kilpparam.n_pv_akt; i++)
					orig_alalajit[i] = pvparam[i].alalaji;
				}
			kirjEventXml(sarjaxmlfname, true, false, false);
			}
		else {
			 Application->MessageBox(L"Kilpailua koskevat valinnat tallennetaan vasta, kun sarjatiedosto on muodostettu.",
				L"Rajoitus", MB_OK);
			return;
			}
		if (pohjustus)
			pohjustalisavaiheet(ed_n_pv_akt);
		if (vakantit) {
			lisaavakantit(20);
		}
	CanClose = true;
	ModalResult = mrOk;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::BtnVaiheMaarClick(TObject *Sender)
{
	kilpparam.n_pv = _wtoi(EditTiedPv->Text.c_str());
	kilpparam.n_pv_akt = EditPv->Text.ToInt();
	if (CBJrjTarkkuus->ItemIndex < 4)
		pvparam[0].pyor[2] =  SEK/powi(10, CBJrjTarkkuus->ItemIndex);
	if (CBTulTarkkuus->ItemIndex < 4)
		pvparam[0].pyor[3] =  SEK/powi(10, CBTulTarkkuus->ItemIndex);
	for (int i = 1; i < kilpparam.n_pv; i++) {
		if (CBJrjTarkkuus->ItemIndex < 4)
			pvparam[i].pyor[2] = pvparam[0].pyor[2];
		if (CBTulTarkkuus->ItemIndex < 4)
			pvparam[i].pyor[3] = pvparam[0].pyor[3];
		}
	FormVaiheMaar = new TFormVaiheMaar(this);
	FormVaiheMaar->ShowModal();
	delete FormVaiheMaar;
	NaytaTarkkuudet();
}
//---------------------------------------------------------------------------


void __fastcall TFormKilpMaaritys::EditSarjojaChange(TObject *Sender)
{
	CBSarjat->Checked = _wtoi(EditSarjoja->Text.c_str()) > 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::CBLahtoporttiClick(TObject *Sender)
{
	if (inInit)
		return;
	for	(int i = 0; i < kilpparam.n_pv; i++)
		pvparam[i].hiihtolahto = CBLahtoportti->Checked ? 1 : 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::Kvlisenssinumero1Click(TObject *Sender)
{
	int kdi;
	wchar_t ch;

	kdi = kilpparam.kvlisno;
	inputint_prompt(&kdi, L"Anna nollasta eroava numero, jos maakoodi kirjoitetaan lisenssinumeron "
		L"alkuun. Annettu numeroarvo kertoo lisenssikoodin numero-osan minimipituuden", &ch, this);
	if (ch != ESC)
		kilpparam.kvlisno = kdi;
}
//---------------------------------------------------------------------------

