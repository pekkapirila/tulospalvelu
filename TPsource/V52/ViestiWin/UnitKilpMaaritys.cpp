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
#include <FileCtrl.hpp>
#pragma hdrstop

#include <time.h>
#include "UnitKilpMaaritys.h"
#include "VDeclare.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormKilpMaaritys *FormKilpMaaritys;
extern TFormMain *FormMain;

static wchar_t TunnType[] = L" ETSM", orig_laji;
static wchar_t lajit[] = L"SHJAM", muutlajit[] = L"PNM", alalajit[] = L" SJPRD";
static int orig_n_os;
int powi(int i, int j);

//---------------------------------------------------------------------------
__fastcall TFormKilpMaaritys::TFormKilpMaaritys(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	CBVaihe[0] = CBVaihe1;
	CBVaihe[1] = CBVaihe2;
	CBVaihe[2] = CBVaihe3;
	CBVaihe[3] = CBVaihe4;
	CBVaihe[4] = CBVaihe5;
	Label3->Caption = L"Osuuksien tilavaraus (version max. "+UnicodeString(MAXOSUUSLUKU)+L")";
	Label4->Caption = L"Väliaikojen tilavaraus (version max. "+UnicodeString(VAIKALUKU)+L")";
	if (KSEK == 100 && CBTarkkuus->Items->Count == 3) {
		CBTarkkuus->Items->Add(L"0,001 s");
		CBJrjTarkkuus->Items->Add(L"0,001 s");
		CBTultarkkuus->Items->Add(L"0,001 s");
		CBVaTarkkuus->Items->Add(L"0,001 s");
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormKilpMaaritys::ButtonOKClick(TObject *Sender)
{
	int NSrj = 0;
	FILE *sfl;
	int laika = 11, laika2 = 8, laikasj = 11;
	int ed_n_os_akt;

	kilpparam.n_os = _wtoi(EditTiedOsluku->Text.c_str());
	kilpparam.valuku = _wtoi(EditTiedVa->Text.c_str());
	if (EditOsluku->Text.ToInt() > kilpparam.n_os) {
		Application->MessageBox(L"Joukkuekoko ei voi ylittää tiedoston tilavarausta. Korjaa tiedot.", L"Virhe",
			MB_OK);
		return;
		}
	if (EditOsluku->Text.ToInt() < kilpparam.osuusluku) {
		Application->MessageBox(L"Kilpailun joukkuekoko ei voi alittaa minkään sarjan joukkuekokoa. Korjaa tiedot.", L"Virhe",
			MB_OK);
		return;
		}
	ed_n_os_akt = kilpparam.n_os_akt;
	kilpparam.n_os_akt = _wtoi(EditOsluku->Text.c_str());
	int osLkm = _wtoi(EdtOsLkm->Text.c_str());
	if (CBSarjat->Checked && (osLkm < 1 || osLkm > kilpparam.n_os_akt)) {
		Application->MessageBoxW(L"Muodostettavien sarjojen osuusluku mahdoton", L"Este", MB_OK);
		return;
		}
	kilpparam.maxnosuus = _wtoi(EdtMaxRinn->Text.c_str());
	wcsncpy(kilpailu, EditKilpailu->Text.c_str(), sizeof(kilpailu)/2-1);
//	wcscpy(llparam.luetots0, kilpailu);
	wcsncpy(kilpparam.kilpkoodi, EditKoodi->Text.c_str(), sizeof(kilpparam.kilpkoodi)/2-1);
	wcsncpy(kilpparam.Name, EdtXmlNimi->Text.c_str(), sizeof(kilpparam.Name)/2-1);
	wcsncpy(kilpparam.Date, EdtPvm->Text.c_str(), sizeof(kilpparam.Date)/2-1);
	kilpparam.vuosi = EditVuosi->Text.ToInt();
	if (lajit[RGlaji->ItemIndex] == L'M') {
		kilpparam.kilplaji = muutlajit[RGMuuLaji->ItemIndex];
		}
	else
		kilpparam.kilplaji = lajit[RGlaji->ItemIndex];
	kilpparam.alalaji = alalajit[RGtasmennys->ItemIndex];
	if (kilpparam.alalaji == L'P') {
		if (!kilpparam.partio)
		kilpparam.partio = 2;
		kilpparam.rogaining = false;
		}
	if (kilpparam.alalaji == L'R') {
		kilpparam.partio = 2;
		kilpparam.rogaining = true;
		}
	else {
		kilpparam.partio = 0;
		kilpparam.rogaining = false;
		}
	kilpparam.sakkoyks = _wtoi(EdtSakko->Text.c_str());
	if (kilpparam.sakkoyks == 0)
		kilpparam.sakkoyks = 1;
	kilpparam.badgelaji = TunnType[CBTunnistin->ItemIndex];
	kilpparam.kaksibadge = CB2badge->Checked ? 1 : 0;

	kilpparam.pyor[1] =  SEK/powi(10, CBTarkkuus->ItemIndex);
	kilpparam.pyor[2] =  SEK/powi(10, CBJrjTarkkuus->ItemIndex);
	kilpparam.pyor[3] =  SEK/powi(10, CBTultarkkuus->ItemIndex);
	kilpparam.pyor[4] =  SEK/powi(10, CBVaTarkkuus->ItemIndex);
	kilpparam.laika = CBTarkkuus->ItemIndex ? CBTarkkuus->ItemIndex + 9 : 8;
	kilpparam.laikasj = CBTarkkuus->ItemIndex ? CBJrjTarkkuus->ItemIndex + 9 : 8;
	kilpparam.laika2 = CBTultarkkuus->ItemIndex ? CBTultarkkuus->ItemIndex + 9 : 8;
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
			for (int i = 0; i < NSrj; i++) {
				Sarjat[i].nollaa();
				sprintf(Sarjat[i].sarjanimi, "S%d", i+1);
				Sarjat[i].ntosuus = osLkm;
				}
			sarjaluku = NSrj;
			orig_laji = kilpparam.kilplaji;
			kirjEventXml(sarjaxmlfname, true, false, false);
			}
		else {
			Application->MessageBox(L"Sarjalukumäärä ei sallitulla alueella", L"Virhe",
				MB_OK);
			return;
			}
		}
	else {
		if (sarjaluku > 0) {
/*
			if (sarjaluku < MAXSARJALUKU && CBTunnistin->ItemIndex > 0 && !Sarjat[sarjaluku-1].vakanttisarja && haesarja_w(L"VAKANTIT", false) < 0) {
				if (ToimintaTila == 0)
					vakantit = select3(2, L"Sarjojen viimeiseksi suositellaan vakanttisarjaa, kun käytössä on tunnistin (esim. Emit). Lisätäänkö", L"Vakanttisarja",
						L"Lisää sarja", L"Älä lisää sarjaa", L"", this);
				else
					vakantit = select3(3, L"Sarjojen viimeiseksi suositellaan vakanttisarjaa ja 20 kilpailijaa, kun käytössä on tunnistin (esim. Emit). Lisätäänkö", L"Vakanttisarja",
						L"Lisää sarja", L"Älä lisää sarjaa", L"Lisää sarja ja 20 vakanttikilpailijaa", this);
				if (vakantit != 2) {
					sarjaluku++;
					Sarjat[sarjaluku-1].nollaa();
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
*/
			if (Application->MessageBox(L"Tallennetaanko mahdolliset muutokset sarjatiedostoon?", L"Tallennus",
				MB_YESNO) == IDYES) {
				int err = 0;
				orig_laji = kilpparam.kilplaji;
				if ((err = maaraaOsuusluku(false)) != 0) {
					if (err == 2)
						Application->MessageBoxW(L"Osuusmäärän tilavaraus ristiriidassa aiempien sarjatietojen kanssa",
							L"Virhe", MB_OK);
					if (err == 1)
						Application->MessageBoxW(L"Määritelty osuusluku ristiriidassa aiempien sarjatietojen kanssa",
							L"Virhe", MB_OK);
					return;
					}
				kirjEventXml(sarjaxmlfname, true, false, false);
				kilpparam.maxnosuus = _wtoi(EdtMaxRinn->Text.c_str());
				}
			}
		else {
			Application->MessageBox(L"Valinnat tallennetaan vasta, kun sarjatiedosto on muodostettu, aiemman sarjatiedoston lukeminen kumoaa valinnat",
				L"Rajoitus", MB_OK);
			}
		}
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormKilpMaaritys::NaytaVaiheet(void)
{
}
//---------------------------------------------------------------------------

static int PyorIx(int pyor)
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

void __fastcall TFormKilpMaaritys::NaytaArvot(void)
{
	EditKilpailu->Text = UnicodeString(kilpailu);
	EditTiedOsluku->Text = UnicodeString(kilpparam.n_os);
	EditTiedVa->Text = UnicodeString(kilpparam.valuku);
	EditOsluku->Text = UnicodeString(kilpparam.n_os_akt);
	EdtMaxRinn->Text = kilpparam.maxnosuus;
	EdtSakko->Text = kilpparam.sakkoyks;
	CBTunnistin->ItemIndex = wcswcind(kilpparam.badgelaji, TunnType);
	if (CBTunnistin->ItemIndex < 0)
		CBTunnistin->ItemIndex = 0;
	CB2badge->Checked = kilpparam.kaksibadge == 1;
	if (!kilpparam.kilpkoodi[0])
		wcscpy(kilpparam.kilpkoodi, L"0000");
	EditKoodi->Text = UnicodeString(kilpparam.kilpkoodi);
	EdtXmlNimi->Text = UnicodeString(kilpparam.Name);
	EdtPvm->Text = UnicodeString(kilpparam.Date);
	if (kilpparam.vuosi < 1900) {
		time_t clock;
		struct tm *now;
		time(&clock);
		now = localtime(&clock);
		kilpparam.vuosi = now->tm_year + 1900;
		}
	EditVuosi->Text = UnicodeString(kilpparam.vuosi);
	RGlaji->ItemIndex = wcswcind(kilpparam.kilplaji, lajit);
	if (RGlaji->ItemIndex < 0)
		RGlaji->ItemIndex = 4;
	RGtasmennys->ItemIndex = wcswcind(kilpparam.alalaji, alalajit);
	if (RGtasmennys->ItemIndex < 0)
		RGtasmennys->ItemIndex = 0;
	CBTarkkuus->ItemIndex = PyorIx(kilpparam.pyor[1]);
	CBJrjTarkkuus->ItemIndex = PyorIx(kilpparam.pyor[2]);
	CBTultarkkuus->ItemIndex = PyorIx(kilpparam.pyor[3]);
	CBVaTarkkuus->ItemIndex = PyorIx(kilpparam.pyor[4]);
//	CBLahtoportti->Checked = pvparam[k_pv].hiihtolahto;
}
//---------------------------------------------------------------------------
void __fastcall TFormKilpMaaritys::FormActivate(TObject *Sender)
{
	static int init = 0;

	if (!init && ToimintaTila == 0) {
		if (kilpparam.n_os > MAXOSUUSLUKU)
			kilpparam.n_os = MAXOSUUSLUKU;
		if (kilpparam.valuku > VAIKALUKU)
			kilpparam.valuku = VAIKALUKU;
		init = 1;
		}
	if (ToimintaTila != 0) {
		BtnLueSrj->Enabled = false;
		CBSarjat->Checked = false;
		CBSarjat->Enabled = false;
		orig_n_os = kilpparam.n_os_akt;
		}
	orig_laji = kilpparam.kilplaji;
	EditHakem->Text = FormMain->CurrentDir;
	if (wcswcind(kilpparam.alalaji, L"JPR") >= 0)
		EdtMaxRinn->Enabled = false;
	NaytaArvot();
}
//---------------------------------------------------------------------------
void __fastcall TFormKilpMaaritys::Button1Click(TObject *Sender)
{
  ModalResult = mrCancel;
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
		maaraaOsuusluku(true);
		orig_n_os = kilpparam.n_os_akt;
		}
	NaytaArvot();
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Kilpailun luominen");
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::EditTiedOslukuExit(TObject *Sender)
{
	int n;

	n = _wtoi(EditTiedOsluku->Text.c_str());
	if (ToimintaTila > 0 && n != kilpparam.n_os) {
		Application->MessageBox(L"Osuuksien tilavarausta ei voi muuttaa, kun kilpailijatiedot on jo luotu",
			L"Rajoitus", MB_OK);
		EditTiedOsluku->Text = kilpparam.n_os;
		}
	else if (n < 1 || n > MAXOSUUSLUKU) {
		Application->MessageBox(L"Osuuksien tilavaraus ei sallitulla alueella", L"Virhe",
			MB_OK);
		EditTiedOsluku->Text = kilpparam.n_os;
		}
	else {
		if (_wtoi(EditOsluku->Text.c_str()) > n) {
			Application->MessageBoxW((L"Kilpailun joukkuekooksi asetettu "+UnicodeString(n)).c_str(),
				L"Muutos", MB_OK);
			}
		EditOsluku->Text = n;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::EditTiedVaExit(TObject *Sender)
{
	int n;

	n = _wtoi(EditTiedVa->Text.c_str());
	if (ToimintaTila > 0 && n != kilpparam.valuku) {
		Application->MessageBox(L"Väliaikojen tilavarausta ei voi muuttaa, kun kilpailijatiedot on jo luotu",
			L"Rajoitus", MB_OK);
		EditTiedVa->Text = kilpparam.valuku;
		}
	if (n < 1 || n > VAIKALUKU) {
		Application->MessageBox(L"Väliaikojen tilavaraus ei sallitulla alueella", L"Virhe",
			MB_OK);
		EditTiedVa->Text = kilpparam.valuku;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::RGlajiClick(TObject *Sender)
{
	if (RGlaji->ItemIndex == 0) {
		CBTarkkuus->ItemIndex = 2;
		CBJrjTarkkuus->ItemIndex = 2;
		CBVaTarkkuus->ItemIndex = 2;
		CBTultarkkuus->ItemIndex = 0;
//		CBLahtoportti->Checked = false;
		EdtSakko->Text = 600;
		CBTunnistin->ItemIndex = 1;
		PanelMuuLaji->Visible = false;
//		FormKilpMaaritys->Width = RGSarja->Left + RGSarja->Width + 11 * Screen->PixelsPerInch / 96;
		}
	else {
		CBTarkkuus->ItemIndex = 2;
		CBJrjTarkkuus->ItemIndex = 2;
		CBVaTarkkuus->ItemIndex = 2;
		CBTultarkkuus->ItemIndex = RGlaji->ItemIndex == 1 ? 1 : 0;
//		CBLahtoportti->Checked = RGlaji->ItemIndex == 1 || RGlaji->ItemIndex == 3;
		EdtSakko->Text = (RGlaji->ItemIndex == 3) ? 0 : 1;
		CBTunnistin->ItemIndex = 0;
		PanelMuuLaji->Visible = false;
//		FormKilpMaaritys->Width = RGSarja->Left + RGSarja->Width + 11 * Screen->PixelsPerInch / 96;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::RGtasmennysClick(TObject *Sender)
{
	RGlajiClick(Sender);
	if (RGtasmennys->ItemIndex >= 2 && RGtasmennys->ItemIndex <= 4) {
		EdtMaxRinn->Text = 1;
		EdtMaxRinn->Enabled = false;
		}
	else
		EdtMaxRinn->Enabled = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::RGMuuLajiClick(TObject *Sender)
{
	for (int i = 0; i < 5; i++) {
		CBVaihe[i]->Items->Clear();
		CBVaihe[i]->Items->Add(L"Ei käytössä");
		CBVaihe[i]->ItemIndex = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::EditOslukuExit(TObject *Sender)
{
	int n = _wtoi(EditOsluku->Text.c_str());
	if (ToimintaTila == 2 && n != kilpparam.n_os_akt) {
		n = kilpparam.n_os_akt;
		Application->MessageBoxW(L"Joukkuekoon maksimin muuttaminen on mahdollista vain esivalmistelutilassa",
			L"Rajoitus", MB_OK);
		}
	else if (n < 1) {
		n = 1;
		Application->MessageBoxW((L"Kilpailun joukkuekooksi asetettu "+UnicodeString(n)).c_str(),
			L"Rajoitus", MB_OK);
		}
	else if (n > _wtoi(EditTiedOsluku->Text.c_str())) {
		n = _wtoi(EditTiedOsluku->Text.c_str());
		Application->MessageBoxW((L"Kilpailun joukkuekooksi asetettu "+UnicodeString(n)+
			L". Muuta tarvittaessa tiedostomuoto ensin.").c_str(), L"Rajoitus", MB_OK);
		}
	EditOsluku->Text = n;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpMaaritys::EditSarjojaExit(TObject *Sender)
{
	CBSarjat->Checked = _wtoi(EditSarjoja->Text.c_str()) > 0;
}
//---------------------------------------------------------------------------

