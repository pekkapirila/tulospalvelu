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
//#include <FileCtrl.hpp>
#pragma hdrstop

#include "UnitHylkRap.h"
#include "WinHk.h"
#include "HkMuotoilu.h"
#include "UnitKirjoitinVal.h"
#include "UnitEmithaku.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormHylkRap *FormHylkRap;

#define HRP_HylRaportti             1
#define HRP_KilpNo                  2
#define HRP_Nimi                    3
#define HRP_Seura                   4
#define HRP_AvausAika               5
#define HRP_TallAika                6
#define HRP_Luonne                  7
#define HRP_Paatos_1                8
#define HRP_Paatos_2                9
#define HRP_Myonto                 10
#define HRP_Selitys                11
#define HRP_Muistio                12
#define HRP_Piikit                 13
#define HRP_Sarja                  14
#define HRP_Rivi                   15
#define HRP_Emit                   16
#define HRP_Rata                   17
#define HRP_Toimitsija			   18

static tagListTp HrpTags[] = {
	{HRP_HylRaportti, L"HylRaportti"},
	{HRP_KilpNo, L"KilpNo"},
	{HRP_Nimi, L"Nimi"},
	{HRP_Seura, L"Seura"},
	{HRP_AvausAika, L"AvausAika"},
	{HRP_TallAika, L"TallAika"},
	{HRP_Luonne, L"Luonne"},
	{HRP_Paatos_1, L"Paatos_1"},
	{HRP_Paatos_2, L"Paatos_2"},
	{HRP_Myonto, L"Myonto"},
	{HRP_Selitys, L"Selitys"},
	{HRP_Muistio, L"Muistio"},
	{HRP_Piikit, L"Piikit"},
	{HRP_Sarja, L"Sarja"},
	{HRP_Rivi, L"Rivi"},
	{HRP_Emit, L"Emit"},
	{HRP_Rata, L"Rata"},
	{HRP_Toimitsija, L"Toimitsija"}
	};
static int nHrpTags = sizeof(HrpTags)/sizeof(HrpTags[0]);
static wchar_t *LuonneStr[] = {L"Aiheeton / Without basis", L"Keskeyttänyt / DNF",
	L"Emit-korttia ei ole luettu / Emit card not read",	L"Leima puuttuu / Missing code",
	L"Väärä leima / Wrong code", L"Leimausjärjestys / Wrong order", L"Muu / Other"};
static wchar_t *RatkStr[] = {L"Ei käsitelty tai käsittely kesken / No decision",
	L"Hyväksytty / OK", L"Hylätty / Disq.", L"Keskeyttänyt / DNF"};
static wchar_t *MyontoStr[] = {L"Ei tiedossa / Unknown", L"Hyväksyy esityksen / Agrees",
	L"Kiistää esityksen / Disagrees"};
static wchar_t *PiikkiStr[] = {L"Ei käsitelty / Not verified", L"Ei korttia / No card",
	L"OK", L"Jälki puuttuu / Punch missing", L"Epäselvä / Ambiguous"};

//---------------------------------------------------------------------------
__fastcall TFormHylkRap::TFormHylkRap(TComponent* Owner)
	: TForm(Owner)
{
	wchar_t *p;

	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
	if (RaporttiHak[0] != L'.' && (((p = wcsstr(RaporttiHak, L":")) != NULL && p - RaporttiHak < 3) ||
		((p = wcsstr(RaporttiHak, L"\\")) != NULL && p - RaporttiHak < 3) ||
		((p = wcsstr(RaporttiHak, L"/")) != NULL && p - RaporttiHak < 3))) {
		wcsncpy(ReportDir, RaporttiHak, sizeof(ReportDir)/2-1);
		}
	else {
		wcsncpy(ReportDir, WorkingDir, sizeof(ReportDir)/2-1);
		wcscat(ReportDir, L"\\");
		int len = wcslen(ReportDir);
		wcsncpy(ReportDir+len, RaporttiHak, sizeof(ReportDir)/2-len-1);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormHylkRap::FormShow(TObject *Sender)
{
	NaytaTiedot();
}
//---------------------------------------------------------------------------
void  TFormHylkRap::NaytaKilpailija(int kno, int bdg)
{
	KilpNo = kno;
	Badge = bdg;
	wcscpy(AvausAika, ISOdatetime(0,0));
}
//---------------------------------------------------------------------------
void  TFormHylkRap::NaytaTiedot(void)
{
	int d;
	kilptietue kilp;
	wchar_t st[100];

	RGXmlLuku->ItemIndex = 0;
	EdtAvausaika->Text = L"";
	RGLuonne->ItemIndex = 0;
	RGPiikit->ItemIndex = 0;
	RGRatk_1->ItemIndex = 0;
	RGKilpKanta->ItemIndex = 0;
	RGTuom->ItemIndex = 0;
	CBNimihaku->ItemIndex = 0;
	EdtSeura->Text = L"";
	EdtNimi->Text = L"";
	EdtSarja->Text = L"";
	EdtRata->Text = L"";
	EdtSelitys->Text = L"";
	EdtToimitsija->Text = L"";
	EdtBadge->Text = L"";
	Memo1->Clear();
	if ((d = getpos(KilpNo)) > 0) {
		kilp.GETREC(d);
		EdtAvausaika->Text = AvausAika;
		EdtKilpno->Text = KilpNo;
		EdtSeura->Text = kilp.seuratieto(0);
		EdtNimi->Text = kilp.nimi(st, 40, false);
		EdtSarja->Text = Sarjat[kilp.Sarja()].sarjanimi;
		EdtRata->Text = kilp.pv[k_pv].rata;
		if (Badge == 0)
			Badge = kilp.pv[k_pv].badge[0];
		EdtBadge->Text = Badge;
		EdtSelitys->Text = kilp.pv[k_pv].selitys;
		if (kilp.pv[k_pv].luonne)
			RGLuonne->ItemIndex = stschind(kilp.pv[k_pv].luonne, "KEPVJM")+1;
		else {
			switch (kilp.tark(k_pv)) {
				case 'K':
					RGLuonne->ItemIndex = 1;
					break;
				default:
					RGLuonne->ItemIndex = -1;

				}
			}
		RGPiikit->ItemIndex = stschind(kilp.pv[k_pv].piikit, "0OPA")+1;
		RGRatk_1->ItemIndex = stschind(kilp.pv[k_pv].ratk_1, "THK")+1;
		RGKilpKanta->ItemIndex = stschind(kilp.pv[k_pv].myonto, "HK")+1;
		RGTuom->ItemIndex = stschind(kilp.pv[k_pv].ratk_2, "THK")+1;
		lueXmlRaportti();
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormHylkRap::Suljekaavake1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormHylkRap::ValitseHakemisto1Click(TObject *Sender)
{
	wchar_t Hakemisto[MAX_PATH];

	wcsncpy(Hakemisto, ReportDir, MAX_PATH);
	if (!ValitseHakemisto(Hakemisto, MAX_PATH)) {
		wcsncpy(ReportDir, Hakemisto, sizeof(ReportDir)/2-1);
		if (ReportDir[wcslen(ReportDir)-1] == L'\\')
			ReportDir[wcslen(ReportDir)-1] = 0;
		}
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------
void  TFormHylkRap::hylk_rap(tulostusparamtp *tulprm)
{
	int m = 50, x1 = 400, x2 = 470;
	tulprm->Initpage(NULL);
	tulprm->Paaots_on();
	tulprm->Putfld(L"Hylkäysesitys", m, 20, 0, 0);
	tulprm->Endline(0);
	tulprm->Paaots_pois();
	tulprm->Aliots_on();
	tulprm->Putfld(L"Disqualification proposal", m, 30, 0, 0);
	tulprm->Aliots_pois();
	tulprm->Endline(3);
	tulprm->Putfld(L"Tapaus avattu / Case opened", m, 30, 0, 0);
	tulprm->Putfld(EdtAvausaika->Text.c_str(), m+x1, 30, 0, 0);
	tulprm->Endline(1);
	tulprm->Putfld(L"Viimeisin tallennus / Latest processing", m, 45, 0, 0);
	tulprm->Putfld(EdtTallennusaika->Text.c_str(), m+x1, 30, 0, 0);
	tulprm->Endline(1);
	tulprm->Putfld(L"Sarja / Class", m, 30, 0, 0);
	tulprm->Putfld(EdtSarja->Text.c_str(), m+x1, 30, 0, 0);
	tulprm->Endline(1);
	if (EdtRata->Text.Length() > 0) {
		tulprm->Putfld(L"Rata / Course", m, 30, 0, 0);
		tulprm->Putfld(EdtRata->Text.c_str(), m+x1, 30, 0, 0);
		tulprm->Endline(1);
		}
	tulprm->Putfld(L"Kilpailija / Competitor", m, 30, 0, 0);
	tulprm->Putfld((UnicodeString(KilpNo)+L" :").c_str(), m+x1, 6, 1, 0);
	tulprm->Putfld(EdtNimi->Text.c_str(), m+x2, 40, 0, 0);
	tulprm->Endline(1);
	tulprm->Putfld(L"Seura / Club", m, 30, 0, 0);
	tulprm->Putfld(EdtSeura->Text.c_str(), m+x2, 40, 0, 0);
	tulprm->Endline(1);
	tulprm->Putfld(L"Emit-koodi / Emit code", m, 30, 0, 0);
	tulprm->Putfld(EdtBadge->Text.c_str(), m+x1, 30, 0, 0);
	tulprm->Endline(1);
	tulprm->Putfld(L"Ongelma / Problem", m, 30, 0, 0);
	if (RGLuonne->ItemIndex > 0)
		tulprm->Putfld(LuonneStr[RGLuonne->ItemIndex], m+x1, 40, 0, 0);
	tulprm->Endline(1);
	tulprm->Putfld(L"Piikinjäljet / Control card", m, 40, 0, 0);
	if (RGPiikit->ItemIndex > 0)
	tulprm->Putfld(PiikkiStr[RGPiikit->ItemIndex], m+x1, 40, 0, 0);
	tulprm->Endline(1);
	tulprm->Putfld(L"Lisätietoa / Additional notes", m, 40, 0, 0);
	tulprm->Putfld(EdtSelitys->Text.c_str(), m+x1, 30, 0, 0);
	tulprm->Endline(0);
	for (int r = 0; r < 5; r++) {
		if (r < Memo1->Lines->Count)
			tulprm->Putfld(Memo1->Lines->Strings[r].c_str(), m+x1, 50, 0, 0);
		tulprm->Endline(0);
		}
	tulprm->Endline(0);
	tulprm->Putfld(L"Toimitsijan päätös / Official's decision", m, 45, 0, 0);
	if (RGRatk_1->ItemIndex > 0)
		tulprm->Putfld(RatkStr[RGRatk_1->ItemIndex], m+x1, 40, 0, 0);
	tulprm->Endline(0);
	tulprm->Putfld(L"Toimitsija / Official", m, 40, 0, 0);
	tulprm->Putfld(EdtToimitsija->Text.c_str(), m+x1, 30, 0, 0);
	tulprm->Endline(1);
	tulprm->Putfld(L"Kilpailijan kanta / Competitor's view", m, 45, 0, 0);
	if (RGKilpKanta->ItemIndex > 0)
		tulprm->Putfld(MyontoStr[RGKilpKanta->ItemIndex], m+x1, 40, 0, 0);
	tulprm->Endline(1);
	tulprm->Putfld(L"Lopullinen päätös / Final decision", m, 40, 0, 0);
	if (RGTuom->ItemIndex > 0)
		tulprm->Putfld(RatkStr[RGTuom->ItemIndex], m+x1, 40, 0, 0);
	tulprm->Endline(3);
	tulprm->Putfld(L"Allekirjoitukset / Signatures", m, 30, 0, 0);
	tulprm->Endline(4);
	tulprm->Putfld(L"Toimitsija / Official", m, 30, 0, 0);
	tulprm->Putfld(L"Kilpailija / Competitor", m+x1, 30, 0, 0);
	tulprm->Endline(0);
	tulprm->Endpage();
	tulprm->Enddoc();
}
//---------------------------------------------------------------------------
void  TFormHylkRap::tulostaRaportti(void)
{
	if (RapKirjoitin[0] == 0) {
		Valitsekirjoitin1Click(this);
		}
	if (RapKirjoitin[0] != 0) {
	RapTulPrm.printer = GDIPRINTER;
	RapTulPrm.language = lsttulparam.language;
	if (!RapTulPrm.lstf) {
		EnterCriticalSection(&print_CriticalSection);
		if ((RapTulPrm.lstf = openprfile(RapKirjoitin, 1, TRUE, FALSE, 0, FALSE))
			== NULL) {
			Application->MessageBoxW(L"Kirjoittimen käyttö ei onnistu", L"Virhe", MB_OK);
			return;
			}
		if (RapTulPrm.lstf && RapTulPrm.lstf->prno == 2)
			initgdiprinter(&RapTulPrm);
		}
		hylk_rap(&RapTulPrm);
		closeprfile(RapTulPrm.lstf);
		RapTulPrm.lstf = NULL;
		LeaveCriticalSection(&print_CriticalSection);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormHylkRap::Avaahylkysraportti1Click(TObject *Sender)
{
	OpenDialog1->DefaultExt = "x,l";
	OpenDialog1->Filter = "Xml-tiedostot (*.xml) |*.xml|Kaikki tiedostot (*.*)|*.*";
	OpenDialog1->InitialDir = ReportDir;
	if (OpenDialog1->Execute()) {
		int kno = 0;
		UnicodeString Fname = OpenDialog1->FileName.UpperCase();
		int pos = Fname.Pos(L".XML");
		if (pos > 7 && Fname[pos-2] == L'_' && Fname[pos-7] == L'_') {
			kno = Fname.SubString(pos-6, 4).ToInt();
			if (kno > 0) {
				KilpNo = kno;
				NaytaTiedot();
				}
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormHylkRap::allennanytetyttiedothylkysraporttiin1Click(TObject *Sender)
{
	kirjoitaXmlRaportti();
}
//---------------------------------------------------------------------------
void __fastcall TFormHylkRap::Valitsekirjoitin1Click(TObject *Sender)
{
	FormKirjVal = new TFormKirjVal(this);
	FormKirjVal->ShowModal();
	wcsncpy(RapKirjoitin,
		FormKirjVal->KirjoitinVal->Items->Strings[FormKirjVal->KirjoitinVal->ItemIndex].c_str(), 198);
	delete FormKirjVal;
}
//---------------------------------------------------------------------------

void __fastcall TFormHylkRap::BtnTallennaClick(TObject *Sender)
{
	int d;

	EdtTallennusaika->Text = ISOdatetime(0,0);
	d = getpos(KilpNo);
	EnterCriticalSection(&tall_CriticalSection);
	if (d > 0)
		Kilp.GETREC(d);
	if (d < 1 || Kilp.kilpstatus != 0) {
		LeaveCriticalSection(&tall_CriticalSection);
		Application->MessageBoxW(L"Virheellinen kilpailunumero", L"Este", MB_OK);
		return;
		}
	wcsncpy(Kilp.pv[k_pv].selitys, EdtSelitys->Text.c_str(), sizeof(Kilp.pv[k_pv].selitys)/2-1);
	Kilp.pv[k_pv].luonne = RGLuonne->ItemIndex < 1 ? 0 : " KEPVJM"[RGLuonne->ItemIndex];
	Kilp.pv[k_pv].piikit = RGPiikit->ItemIndex < 1 ? 0 : " 0OPA"[RGPiikit->ItemIndex];
	Kilp.pv[k_pv].ratk_1 = RGRatk_1->ItemIndex < 1 ? 0 : " THK"[RGRatk_1->ItemIndex];
	Kilp.pv[k_pv].myonto = RGKilpKanta->ItemIndex < 1 ? 0 : " HK"[RGKilpKanta->ItemIndex];
	Kilp.pv[k_pv].ratk_2 = RGTuom->ItemIndex < 1 ? 0 : " THK"[RGTuom->ItemIndex];
	Kilp.tallenna(d, 0, 0, 0, 0);
	LeaveCriticalSection(&tall_CriticalSection);
	kirjoitaXmlRaportti();
}
//---------------------------------------------------------------------------

void __fastcall TFormHylkRap::BtnTulostaClick(TObject *Sender)
{
	tulostaRaportti();
}
//---------------------------------------------------------------------------

void __fastcall TFormHylkRap::BtnTallennaTulostaClick(TObject *Sender)
{
	BtnTallennaClick(Sender);
	tulostaRaportti();
}

wchar_t * TFormHylkRap::RapFile(void)
{
	static wchar_t fname[MAX_PATH];

	swprintf(fname, L"%s\\HylRap_%04d_%1d.xml", ReportDir, KilpNo, k_pv+1);
	return(fname);
}

//---------------------------------------------------------------------------
void  TFormHylkRap::kirjoitaXmlRaportti(void)
{
	TextFl *tallfl;
	int level = 0, nfld;
	wchar_t ln[400];

	tallfl = new TextFl(RapFile(), L"wt");

	if (tallfl->IsOpen()) {
		tallfl->WriteLine(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
		tallfl->WriteLine(L"<?xml-stylesheet type=\"text/xsl\" href=\"hylkrap.xsl\"?>\n");
		swprintf(ln, L"%s createTime=\"%s\" creator=\"ViestiWin %s\"",
			XMLhae_tagName(HRP_HylRaportti, HrpTags, nHrpTags), ISOdatetime(0,1), Build());
		tallfl->put_wtag(ln, level++);

		tallfl->put_wxml_s(XMLhae_tagName(HRP_Sarja, HrpTags, nHrpTags), EdtSarja->Text.c_str(), level);
		tallfl->put_wxml_d(XMLhae_tagName(HRP_KilpNo, HrpTags, nHrpTags), KilpNo, level);
		if (EdtNimi->Text.Length() > 0)
			tallfl->put_wxml_s(XMLhae_tagName(HRP_Nimi, HrpTags, nHrpTags), EdtNimi->Text.c_str(), level);
		if (EdtSeura->Text.Length() > 0)
			tallfl->put_wxml_s(XMLhae_tagName(HRP_Seura, HrpTags, nHrpTags), EdtSeura->Text.c_str(), level);
		if (EdtRata->Text.Length() > 0)
			tallfl->put_wxml_s(XMLhae_tagName(HRP_Rata, HrpTags, nHrpTags), EdtRata->Text.c_str(), level);
		if (_wtoi(EdtBadge->Text.c_str()) > 0)
			tallfl->put_wxml_d(XMLhae_tagName(HRP_Emit, HrpTags, nHrpTags), _wtoi(EdtBadge->Text.c_str()), level);
		if (EdtAvausaika->Text.Length() > 0)
			tallfl->put_wxml_s(XMLhae_tagName(HRP_AvausAika, HrpTags, nHrpTags), EdtAvausaika->Text.c_str(), level);
		if (EdtTallennusaika->Text.Length() > 0)
			tallfl->put_wxml_s(XMLhae_tagName(HRP_TallAika, HrpTags, nHrpTags), EdtTallennusaika->Text.c_str(), level);
		if (RGLuonne->ItemIndex >= 0)
			tallfl->put_wxml_s(XMLhae_tagName(HRP_Luonne, HrpTags, nHrpTags), LuonneStr[RGLuonne->ItemIndex], level);
		if (RGPiikit->ItemIndex >= 0)
			tallfl->put_wxml_s(XMLhae_tagName(HRP_Piikit, HrpTags, nHrpTags), PiikkiStr[RGPiikit->ItemIndex], level);
		if (RGRatk_1->ItemIndex >= 0)
			tallfl->put_wxml_s(XMLhae_tagName(HRP_Paatos_1, HrpTags, nHrpTags), RatkStr[RGRatk_1->ItemIndex], level);
		if (RGKilpKanta->ItemIndex >= 0)
			tallfl->put_wxml_s(XMLhae_tagName(HRP_Myonto, HrpTags, nHrpTags), MyontoStr[RGKilpKanta->ItemIndex], level);
		if (RGTuom->ItemIndex >= 0)
			tallfl->put_wxml_s(XMLhae_tagName(HRP_Paatos_2, HrpTags, nHrpTags), RatkStr[RGTuom->ItemIndex], level);
		if (EdtSelitys->Text.Length() > 0)
			tallfl->put_wxml_s(XMLhae_tagName(HRP_Selitys, HrpTags, nHrpTags), EdtSelitys->Text.c_str(), level);
		if (EdtToimitsija->Text.Length() > 0)
			tallfl->put_wxml_s(XMLhae_tagName(HRP_Toimitsija, HrpTags, nHrpTags), EdtToimitsija->Text.c_str(), level);
		if (Memo1->Lines->Count > 0) {
			for (int i = 0; i < Memo1->Lines->Count; i++) {
				tallfl->put_wtag(XMLhae_tagName(HRP_Muistio, HrpTags, nHrpTags), level++);
				tallfl->put_wxml_s(XMLhae_tagName(HRP_Rivi, HrpTags, nHrpTags), Memo1->Lines->Strings[i].c_str(), level);
				tallfl->put_wantitag(XMLhae_tagName(HRP_Muistio, HrpTags, nHrpTags), --level);
				}
			}
		tallfl->put_wantitag(XMLhae_tagName(HRP_HylRaportti, HrpTags, nHrpTags), --level);
		}
	delete tallfl;

}
//---------------------------------------------------------------------------
int TFormHylkRap::tulkXMLRaportti(xml_node *node, int nnode)
{
	int er = 0, inode, DepthIn, haara = 0, iassign = -1, val = 0;
	wchar_t ln[200];

	XMLhaenodeid(node, nnode, HrpTags, nHrpTags);
	DepthIn = node[0].depth + 1;
	for (inode = 1; inode <= nnode; inode++) {
		if (node[inode].depth == DepthIn) {
			haara = 0;
			switch (node[inode].tagid) {
				case HRP_KilpNo :
					node[inode].gettext_int(&KilpNo);
					break;
				case HRP_Nimi :
					node[inode].gettext(ln, 198);
					EdtNimi->Text = ln;
					break;
				case HRP_Seura :
					node[inode].gettext(ln, 198);
					EdtSeura->Text = ln;
					break;
				case HRP_Rata :
					node[inode].gettext(ln, 198);
					EdtRata->Text = ln;
					break;
				case HRP_Emit :
					node[inode].gettext_int(&Badge);
					EdtBadge->Text = Badge;
					break;
				case HRP_AvausAika :
					node[inode].gettext(ln, 198);
					EdtAvausaika->Text = ln;
					break;
				case HRP_TallAika :
					node[inode].gettext(ln, 198);
					EdtTallennusaika->Text = ln;
					break;
				case HRP_Luonne :
					node[inode].gettext(ln, 198);
					RGLuonne->ItemIndex = haeStrIx(ln, LuonneStr, sizeof(LuonneStr)/sizeof(LuonneStr[0]));
					break;
				case HRP_Piikit :
					node[inode].gettext(ln, 198);
					RGPiikit->ItemIndex = haeStrIx(ln, PiikkiStr, sizeof(PiikkiStr)/sizeof(PiikkiStr[0]));
					break;
				case HRP_Paatos_1 :
					node[inode].gettext(ln, 198);
					RGRatk_1->ItemIndex = haeStrIx(ln, RatkStr, sizeof(RatkStr)/sizeof(RatkStr[0]));
					break;
				case HRP_Myonto :
					node[inode].gettext(ln, 198);
					RGKilpKanta->ItemIndex = haeStrIx(ln, MyontoStr, sizeof(MyontoStr)/sizeof(MyontoStr[0]));
					break;
				case HRP_Paatos_2 :
					node[inode].gettext(ln, 198);
					RGTuom->ItemIndex = haeStrIx(ln, RatkStr, sizeof(RatkStr)/sizeof(RatkStr[0]));
					break;
				case HRP_Selitys :
					node[inode].gettext(ln, 198);
					EdtSelitys->Text = ln;
					break;
				case HRP_Toimitsija :
					node[inode].gettext(ln, 198);
					EdtToimitsija->Text = ln;
					break;
				case HRP_Muistio :
					haara = 1;
					break;
				}
			continue;
			}
		if (node[inode].depth == DepthIn + 1) {
			if (haara == 1) {
				switch (node[inode].tagid) {
					case HRP_Rivi :
						node[inode].gettext(ln, 198);
						if (wcslen(ln) > 0)
							Memo1->Lines->Add(ln);
						break;
					}
				continue;
				}
			continue;
			}
		}
	return(er);
}

//---------------------------------------------------------------------------

int TFormHylkRap::lueXmlRaportti(void)
{
	TextFl *infile;
	int ntag = 10000, haara = 0, inode, inode_in, depth = 0, rv, er = 0, lntype;
	short k[MAXXMLDEPTH];
	wchar_t line[1000], *txtbuf[MAXXMLDEPTH];
	wchar_t msg[200] = L"", tag[MAXXMLTAGLEN+1];
	xml_tree tree;
	xml_node nd;

	memset(k, 0, sizeof(k));
	infile = new TextFl(RapFile(), L"rt");
	if (infile->IsOpen()) {
		for (int i = 0; i < MAXXMLDEPTH; i++) {
			txtbuf[i] = new wchar_t[XMLTXTBUFLEN+1];
			memset(txtbuf[i], 0, 2*(XMLTXTBUFLEN+1));
			}
		tree.node = new xml_node[ntag];
		tree.maxnode = ntag;
		inode = -1;
		rv = 0;
		while (!er && !infile->Feof()) {
			if (infile->ReadLine(line, 999) == NULL)
				break;
			rv++;
			elimwbl2(line);
			lntype = nd.interpretXmlLine(line, tag);
			if (lntype == -5)
				continue;
			nd.depth = depth;
			for (int i = 0; i <= depth; i++)
				nd.nodeno[i] = k[i];           // merkitään eri tasojen järjestysnumerot solmuun
			if (lntype > 0)
				nd.tagid = XMLhae_tagid(nd.tag, HrpTags, nHrpTags);
			if (lntype == 1) {
				tree.node[++inode] = nd;
				k[depth++]++;
				if (depth == 1) {
					inode_in = inode;
					switch (nd.tagid) {
						case HRP_HylRaportti :
							haara = 0;
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er)
								tulkXMLRaportti(tree.node+inode_in, inode-inode_in);
							inode = inode_in-1;
							break;
						}
					continue;
					}
				if (depth > 1) {
					er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
					--depth;
					}
				}
			else if (lntype == 8) {                  // sulkeva tag uudella rivillä
				int jnode;
				depth--;
				k[depth]++;                 // kasvatetaan tason laskuria sulkevän tagin tasolla
				// Etsitään vastaava avaava tag aiemmasta ketjusta
				for (jnode = inode; jnode > 0 && tree.node[jnode].depth > depth; jnode--) ;
				if (wcscmp(tag, tree.node[jnode].tag)) {      // Varmistetaan, että tagit ovat pari
					er = 1;
					swprintf(msg, L"XML-muotoilun tulkinta ei onnnistu. Rivi %d, Tag: '%.30s', odotettu: '%.30s'",
						rv, tag, tree.node[jnode].tag);
					break;
					}
				if (wcslen(txtbuf[depth])) {    // Tallennetaan tason mahdollinen teksti solmuun
					tree.node[jnode].text = new wchar_t[wcslen(txtbuf[depth])+1];
					wcscpy(tree.node[jnode].text, txtbuf[depth]);
					}
				if (depth == 0) {
					break;
					}
				continue;
				}
			}      // while(!infile->Feof())
		delete[] tree.node;
		tree.node = 0;
		for (int i=0; i < MAXXMLDEPTH; i++)
			delete[] txtbuf[i];
		}  		  // infile->Open()
	else {
		if (infile)
			delete infile;
		RGXmlLuku->ItemIndex = 2;
		return(-1);
		}
	delete infile;
	if (depth) {
		er = 1;
		swprintf(msg, L"XML-tiedosto %s päättyi rivillä %d tasolla %d", RapFile(), rv, depth);
		}
	if (er) {
		if (!msg[0]) {
			swprintf(msg, L"Tiedostossa %s virhe rivillä %d tai aikaisemmin", RapFile(), rv);
			}
		writeerror_w(msg, 0, true);
		RGXmlLuku->ItemIndex = 2;
		}
	else
		RGXmlLuku->ItemIndex = 1;
	tree.nodecount = inode+1;
	return(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormHylkRap::EdtKilpnoExit(TObject *Sender)
{
	if (KilpNo != _wtoi(EdtKilpno->Text.c_str())) {
		Badge = 0;
		KilpNo = _wtoi(EdtKilpno->Text.c_str());
		NaytaTiedot();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormHylkRap::EdtNimiChange(TObject *Sender)
{
	if (!EdtNimi->ReadOnly && EdtNimi->Focused()) {
		wchar_t txt[62];
		char key[MAXINDL+1];
		int d;

		wcsncpy(txt, EdtNimi->Text.c_str(),60);
		txt[60] = 0;
		keyfromwname(key, txt, nameindex.keyl-2, 0);
		searchix(&nameindex, key, &d);
		if (d > 0) {
			Kilp.GETREC(d);
			KilpNo = Kilp.id();
			Badge = Kilp.pv[k_pv].badge[0];
			EdtKilpno->Text = KilpNo;
			EdtSeura->Text = Kilp.seuratieto(0);
			EdtSarja->Text = Sarjat[Kilp.Sarja()].sarjanimi;
			EdtRata->Text = Kilp.pv[k_pv].rata;
			EdtBadge->Text = Badge;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormHylkRap::EdtNimiExit(TObject *Sender)
{
	NaytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormHylkRap::EdtKilpnoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN) {
		Key = 0;
		EdtKilpnoExit(Sender);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormHylkRap::CBNimihakuChange(TObject *Sender)
{
	if (CBNimihaku->ItemIndex == 1) {
		EdtNimi->ReadOnly = false;
		FocusControl(EdtNimi);
		}
	else
		EdtNimi->ReadOnly = true;

	if (CBNimihaku->ItemIndex == 2) {
		int kno;
		wchar_t wch;

		FormEmithaku = new TFormEmithaku(this);
		FormEmithaku->ShowModal();
		if (FormEmithaku->ModalResult == mrOk) {
			kno = FormEmithaku->Kilpno;
			if (kno > 0) {
				KilpNo = kno;
				Badge = 0;
				NaytaTiedot();
				}
			}
		delete FormEmithaku;
		CBNimihaku->ItemIndex = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormHylkRap::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Hylkäysraportit");
}
//---------------------------------------------------------------------------

