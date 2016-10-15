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
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <bstrings.h>
#include <HTMLHelpViewer.hpp>

#include "WinHk.h"
#include "UnitJakelu.h"
#include "UnitAvausMaar.h"
#include "UnitStatus.h"
#include "UnitSelect.h"
#include "UnitKilpailijaEmit.h"
#include "UnitKilpailijaOnline.h"
#include "UnitKilpailijatiedot.h"
#include "UnitVaGr.h"
#include "UnitYv.h"
#include "TulosUnit.h"
#include "VertAikaUnit.h"
#include "TapUnit2.h"
//#include "EmitUnit.h"
#include "UnitEmit.h"
#include "TulostusUnit2.h"
#include "LoppuKilpUnit.h"
#include "TakaaAjoUnit.h"
#include "AsetusUnit.h"
#include "UnitExport.h"
#include "UnitImport.h"
#include "UnitKilpMaaritys.h"
#include "UnitSarjatiedot.h"
#include "UnitOsanottajat.h"
#include "UnitNollaus.h"
#include "UnitVakLisays.h"
#include "UnitArvonta.h"
//#include "UnitIlmoitt.h"
#include "UnitIlm2.h"
#include "UnitSeuratoimet.h"
#include "UnitOhjtiedot.h"
#include "UnitSeurat.h"
#include "UnitLisatiedot.h"
#include "UnitTarkMerk.h"
#include "UnitComfile.h"
#include "UnitPistelasku.h"
#include "UnitSprinthiihto.h"
//#include "UnitLahtoajat.h"
#include "UnitMySQL.h"
#include "UnitRadat.h"
#include "UnitMessages.h"
#include "UnitKilpSeurat.h"
#include "UnitAikaSiirto.h"
#include "UnitUusinta.h"
#include "UnitTapahtSiirrot.h"
#include "UnitKisaLuett.h"
#include "UnitValitseKilp.h"
#include "UnitEmitKoodit.h"
#include "UnitFIS.h"
#include "UnitAjanotto.h"
#include "UnitJoukkueTilanne.h"
#include "UnitASakot.h"
#include "UnitSakkoKierr.h"
#include "UnitAmpPaikat.h"
#include "UnitASuunn.h"
#include "UnitYhdistetty.h"
#include "UnitTaulu.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#pragma link "Vcl.HtmlHelpViewer"

TFormMain *FormMain;
TFormAvausMaar *FormAvausMaar;
TFormYhtveto *Yv1;
TFormVaGr *FrmGr[NFORMVAGR];
TTilanneForm *TilFrm[NTILANNEFORM];
TFormKilpailijaEmit *FormKilpailijaEmitA[NFORMKILPAILIJA];
TFormKilpailijaOnline *FormKilpailijaOnlineA[NFORMKILPAILIJA];
TTapForm *FrmTap[NTAPFORM];
//TEmitForm *EmitForm;
TFormEmit *FormEmit;
TTulosteForm *TulosteForm;
TLoppuKilpForm *LoppuKilpForm;
TTakaaAjoForm *TakaaAjoForm;
TFormImport *FormImport;
TFormKilpMaaritys *FormKilpMaaritys;
TFormNollaus *FormNollaus;
TFormVakLisays *FormVakLisays;
extern TFormBrowser *FormBrowser;

extern void naytayhteydet(void);
void lopetus1(void);
void joukkuepisteet(bool naytolle, tulostusparamtp *tulprm);

typedef struct {
	TForm **Form;
	wchar_t	Title[20];
	wchar_t	Kohde[30];
} FormItem;

extern int viimTap;
extern wchar_t ikkunaots[];

void LukumaaratTh(LPVOID lpCn);
wchar_t selected;
int TlsLstPaivita = 1;
static bool kisaluettelo = false;
void ProsEmitThread(LPVOID);

USEFORM("UnitVaGr.cpp", FormVaGr);

FormItem  FormList[] = {
	{(TForm **)&FormStatus, L"Yleistilanne",L""},
	{(TForm **)&FormSarjatiedot, L"Sarjatiedot",L""},
	{(TForm **)&FormIlm2, L"Ilmoittautumiset",L""},
	{(TForm **)&FormArvonta, L"Arvonta",L""},
	{(TForm **)&FormOsanottajat, L"Osanottajat",L""},
	{(TForm **)&FormSeurat, L"Seuraluettelo",L""},
	{(TForm **)&TulosteForm, L"Tulosteet",L""},
	{(TForm **)&FormEmit, L"Emitluenta",L""},
	{(TForm **)&FormEmitKoodit, L"Emitkoodien syöttö",L""},
	{(TForm **)&FormASakot, L"AHsakkojen syöttö",L""},
	{(TForm **)&FormSakkoKierr, L"Sakkokierr. syöttö",L""},
	{(TForm **)&FormASuunn, L"Ampumasuunn.",L""},
	{(TForm **)&FormAmpPaikat, L"Ampumapaikat",L""},
	{(TForm **)&FormAjanotot[0], L"Ajanotto 1",L""},
	{(TForm **)&FormAjanotot[1], L"Ajanotto 2",L""},
	{(TForm **)&FormAjanotot[2], L"Ajanotto 3",L""},
	{(TForm **)&FormAjanotot[3], L"Ajanotto 4",L""},
	{(TForm **)&TilFrm[0], L"Sarjan tilanne 1",L""},
	{(TForm **)&TilFrm[1], L"Sarjan tilanne 2",L""},
	{(TForm **)&TilFrm[2], L"Sarjan tilanne 3",L""},
	{(TForm **)&TilFrm[3], L"Sarjan tilanne 4",L""},
	{(TForm **)&TilFrm[4], L"Sarjan tilanne 5",L""},
	{(TForm **)&TilFrm[5], L"Sarjan tilanne 6",L""},
	{(TForm **)&FrmTap[0], L"Viim. tapahtumat 1",L""},
	{(TForm **)&FrmTap[1], L"Viim. tapahtumat 2",L""},
	{(TForm **)&FrmTap[2], L"Viim. tapahtumat 3",L""},
	{(TForm **)&FrmTap[3], L"Viim. tapahtumat 4",L""},
	{(TForm **)&FrmTap[4], L"Viim. tapahtumat 5",L""},
	{(TForm **)&FrmTap[5], L"Viim. tapahtumat 6",L""},
	{(TForm **)&FormKilpailijaOnlineA[0], L"Kilpailija 1",L""},
	{(TForm **)&FormKilpailijaOnlineA[1], L"Kilpailija 2",L""},
	{(TForm **)&FormKilpailijaOnlineA[2], L"Kilpailija 3",L""},
	{(TForm **)&FormKilpailijaOnlineA[3], L"Kilpailija 4",L""},
	{(TForm **)&FormKilpailijaOnlineA[4], L"Kilpailija 5",L""},
	{(TForm **)&FormKilpailijaOnlineA[5], L"Kilpailija 6",L""},
	{(TForm **)&FormKilpailijaEmitA[0], L"Kilpailija/emit 1",L""},
	{(TForm **)&FormKilpailijaEmitA[1], L"Kilpailija/emit 2",L""},
	{(TForm **)&FormKilpailijaEmitA[2], L"Kilpailija/emit 3",L""},
	{(TForm **)&FormKilpailijaEmitA[3], L"Kilpailija/emit 4",L""},
	{(TForm **)&FormKilpailijaEmitA[4], L"Kilpailija/emit 5",L""},
	{(TForm **)&FormKilpailijaEmitA[5], L"Kilpailija/emit 6",L""},
	{(TForm **)&FrmGr[0], L"Emitgrafiikka 1",L""},
	{(TForm **)&FrmGr[1], L"Emitgrafiikka 2",L""},
	{(TForm **)&FrmGr[2], L"Emitgrafiikka 3",L""},
	{(TForm **)&FrmGr[3], L"Emitgrafiikka 4",L""},
	{(TForm **)&FrmGr[4], L"Emitgrafiikka 5",L""},
	{(TForm **)&FrmGr[5], L"Emitgrafiikka 6",L""}
	};

int nForms = sizeof(FormList)/sizeof(FormItem);

//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
	: TForm(Owner)
{
	Application->HelpFile = ExtractFilePath(Application->ExeName)+UnicodeString(L"HkKisa.chm");
	InitDir = GetCurrentDir();
#ifdef DEMO
	Caption = L"HkKisaWin - DEMO versio - max 30 kilpailijaa";
#endif
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	Left = 10;
	swprintf(kilpparam.tiedlaji, L"HK%d", VersioInt());
	kilpparam.kilplaji = L'S';
	kilpparam.alalaji = L' ';
	kilpparam.badgelaji = L'E';
	kilpparam.lsnimi = LSNIMI;
	kilpparam.lenimi = LENIMI;
	kilpparam.larvo = LARVO;
	kilpparam.lseura = LSEURA;
	kilpparam.lseuralyh = LLYH;
	kilpparam.ljoukkue = LJK;
	kilpparam.lyhdistys = LYHD;
	kilpparam.lwrkoodi = LWRKOODI;
	kilpparam.maxasakko = 5;
	kilpparam.WisV = true;
	kilpparam.seuralyh_on = true;
	kilpparam.n_pv = 2;
	kilpparam.n_pv_akt = 1;
	kilpparam.valuku = 10;
	kilpparam.sakkoyks = RRSAKKO;

	for (int ipv = 0; ipv < N_PV; ipv++) {
		for (unsigned int i = 0; i < sizeof(pvparam[0].pyor)/sizeof(pvparam[0].pyor[0]); i++)
			pvparam[ipv].pyor[i] = (i == 0 ? 1 : SEK);
		pvparam[ipv].laika = 8;
		pvparam[ipv].laikasj = 8;
		pvparam[ipv].laika2 = 8;
		pvparam[ipv].laikava = 8;
		}
}
//---------------------------------------------------------------------------
__fastcall TFormMain::~TFormMain(void)
{
	if (Yv1) {
		delete Yv1;
		}
	for (int i = 0; i < NFORMVAGR; i++) {
		if (FrmGr[i])
			delete FrmGr[i];
		}
	for (int i = 0; i < NTILANNEFORM; i++) {
		if (TilFrm[i])
			delete TilFrm[i];
		}
	for (int i = 0; i < NFORMKILPAILIJA; i++) {
		if (FormKilpailijaEmitA[i])
			delete FormKilpailijaEmitA[i];
		}
	for (int i = 0; i < NTAPFORM; i++) {
		if (FrmTap[i])
			delete FrmTap[i];
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Initialisoi(wchar_t *kilphak, wchar_t *cfgflnm)
{
	int k;
	wchar_t path[200], *p;

	if (kilphak == NULL || kilphak[0] == 0) {
		wcsncpy(path, cfgflnm, 198);
		for (p = path+wcslen(path)-1; p > path; p--) {
			if (*p == L'\\')
				break;
			}
		if (*p == L'\\')
			p[1] = 0;
		}
	else {
		wcsncpy(path, kilphak, 198);
		if (path[wcslen(path)-1] != L'\\')
			wcscat(path, L"\\");
		}
	SetCurrentDir(UnicodeString(path));
	CurrentDir = GetCurrentDir();
	wcsncpy(WorkingDir, CurrentDir.c_str(), sizeof(WorkingDir)/2-1);
	CfgFileName = cfgflnm;
	LbVaihe->Caption = L"Tiedostojen luentavaihe - odota";
	LbVaihe->Visible = true;
	Refresh();
	Sleep(50);
	if (aloitus(0, 0, cfgflnm)) {
		ToimintaTila = 0;
		writeerror_w(L"Siirtyminen pyydettyyn toimintatilaan ei onnistunut. Vain käynnistysasetusten muuttaminen sallittua", 0);
		return;
	}
	if (ikkunaots[0])
		Caption = Caption + L" - " + ikkunaots;
	EditKilpailu->Text = UnicodeString(kilpailu);
	BtnIlmTila->Enabled = false;
	BtnTulPalv->Enabled = false;
	Osanottjat1->Enabled = true;
	Nytt1->Enabled = true;
	Tulosteet1->Enabled = true;
	Seuranimienkopioinnit1->Enabled = true;
	Aikojenjanumeroidensiirrot1->Enabled = true;
	if (kilpparam.kilplaji == L'H')
		Yhdistettyhiihto1->Visible = true;
	if (ToimintaTila == 1) {
		Nollaa1->Enabled = true;
		Numeroi1->Enabled = true;
		LbVaihe->Caption = L"Esivalmistelutila";
		if (kilpparam.n_pv_akt > 1) {
			Tapahtumasarjansiirrot1->Enabled = true;
			Tapahtumasarjansiirrot1->Visible = true;
			}
		}
	else {
		if (kilpparam.bibvaihtelee && k_pv < kilpparam.n_pv_akt - 1)
			Numeroi1->Enabled = true;
		Tulospalvelu1->Enabled = true;
		if (kilpparam.n_pv_akt > 1) {
			Vaiheenvaihto1->Visible = true;
			LbVaihe->Caption = L"Tulospalvelutila - vaihe "+UnicodeString(k_pv+1);
			}
		else
			LbVaihe->Caption = L"Tulospalvelutila";
		if (kuntosuunnmoodi == 0) {
			Sprinthiihdonsiirrot1->Enabled = true;
			if (!k_pv) {
				Loppukilpailunmuodostaminen1->Enabled = true;
				Loppukilpailunmuodostaminen1->Visible = true;
				}
			if (k_pv < kilpparam.n_pv_akt) {
				akaaajolhtjenmuodostaminen1->Enabled = true;
				akaaajolhtjenmuodostaminen1->Visible = true;
				}
			if (kilpparam.kilplaji == L'B') {
				Ampumasuunnistuksensakot1->Enabled = true;
				Ampumasuunnistuksensakot1->Visible = true;
				}
			else if (kilpparam.lsakmax > 0) {
				Ammunnansakot1->Enabled = true;
				Ammunnansakot1->Visible = true;
				Sakkokierrostenseuranta1->Visible =
					kilpparam.kilplaji == L'A';
				Ampumapaikat1->Visible = true;
				}
			}
		else {
			Ajanotto1->Visible = false;
			Ajanotto21->Visible = false;
			Laskekilpailijoidenpisteet1->Visible = false;
			Muutasarjanlhtaikoja1->Visible = false;
			Joukkuepisteet1->Visible = false;
			Numeroi1->Visible = false;
			Sprinthiihdonsiirrot1->Visible = false;
			Tapahtumasarjansiirrot1->Visible = false;
			Aikojenjanumeroidensiirrot1->Visible = false;
			Vliaikojenhakupalvelimelta1->Visible = false;
			}
		if (wcswcind(kilpparam.kilplaji, L"NP") >= 0) {
			Ammunnansakot1->Caption = L"&Sakkojen ja pisteiden syöttö";
			}
		if (wcswcind(kilpparam.kilplaji, L"NPU") >= 0) {
			Lueeratjaradat1->Visible = true;
			Valitseer1->Visible = true;
			}
		}
	if (emitfl < 1) {
		Emitgrafiikka1->Enabled = false;
		Emitgrafiikka1->Visible = false;
		Emitluenta1->Visible = false;
		Kilpailija1->Visible = false;
		Yhteenveto1->Visible = false;
//		Asetukset1->Enabled = false;
//		Asetukset1->Visible = false;
		}
	Haeikkunat->Enabled = true;
	if (alkulayout[0]) {
		HaeIkkunat(alkulayout);
		}
	lkmThreadOn = 1;
	hLkmThread = _beginthread(LukumaaratTh, 20480, 0);
	if (sqlparam.autostart && !FormMySQL) {
		FormMySQL = new TFormMySQL(FormMain);
		FormMySQL->StartMySQL();
		}

	if (emitfl) {
	   ProsEmitOn = true;
	   hProsEmitThread = _beginthread(ProsEmitThread, 20480, 0);
	   }
	setBrowserEmulation();
	if (DeveloperMode) {
		Tulostaulut1->Visible = true;
	}
}
//---------------------------------------------------------------------------

void haeIkkunat(void)
{
	if (alkulayout[0]) {
		FormMain->HaeIkkunat(alkulayout);
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Lopeta1Click(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::NaytaIkkunalista(void)
{
	int n = 0, m = 0, err = 0;

	if (ListBox1 == NULL)
		return;
	try
		{
		for (int i = 0; i < nForms; i++) {
			if (!lkmThreadOn) {
				err = 1;
				break;
				}
			if (*FormList[i].Form != NULL && (*FormList[i].Form)->Visible) {
				wchar_t title[100];
				m++;
				if (m > ListBox1->Items->Count) {
					break;
					}
				swprintf(title, L"%s %s", FormList[i].Title, FormList[i].Kohde);
				if (!wcscmp(ListBox1->Items->Strings[n].c_str(), title))
					n++;
				}
			}
		if (!err && lkmThreadOn && ListBox1 != NULL) {
			if (m != n || ListBox1->Items->Count != n) {
				ListBox1->Clear();
				for (int i = 0; i < nForms; i++) {
					if (!lkmThreadOn)
						break;
					if (*FormList[i].Form != NULL && (*FormList[i].Form)->Visible) {
						wchar_t title[100];
						swprintf(title, L"%s %s", FormList[i].Title, FormList[i].Kohde);
						ListBox1->Items->Add(UnicodeString(title));
						}
					}
				}
			}
		}
	catch (Exception &exception)
		{
		Application->ShowException(&exception);
		}
	catch (...)
		{
		try
			{
			throw Exception("");
			}
		catch (Exception &exception)
			{
			Application->ShowException(&exception);
			}
		}
}

//---------------------------------------------------------------------------

void __fastcall TFormMain::KilpailijaEmitClick(int i)
{
	if (i < 0 || !FormKilpailijaEmitA[i]) {
		 for (i = 0; i < NFORMKILPAILIJA; i++)
			 if (FormKilpailijaEmitA[i] == NULL)
				 break;
		 if (i < NFORMKILPAILIJA)
			 FormKilpailijaEmitA[i] = new TFormKilpailijaEmit(FormMain);
		 else
			 i = 0;
		 }
	 FormKilpailijaEmitA[i]->Show();
	if (FormKilpailijaEmitA[i]->WindowState == wsMinimized)
		FormKilpailijaEmitA[i]->WindowState = wsNormal;
	FormKilpailijaEmitA[i]->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::KilpailijaU1Click(TObject *Sender)
{
	KilpailijaEmitClick(-1);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::N1kilpailija1Click(TObject *Sender)
{
	KilpailijaEmitClick(0);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::N2kilpailija1Click(TObject *Sender)
{
	KilpailijaEmitClick(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N3kilpailija1Click(TObject *Sender)
{
	KilpailijaEmitClick(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N4kilpailija1Click(TObject *Sender)
{
	KilpailijaEmitClick(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N5kilpailija1Click(TObject *Sender)
{
	KilpailijaEmitClick(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N6kilpailija1Click(TObject *Sender)
{
	KilpailijaEmitClick(5);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::KilpailijaOnlineClick(int i)
{
	if (i < 0 || !FormKilpailijaOnlineA[i]) {
		 for (i = 0; i < NFORMKILPAILIJA; i++)
			 if (FormKilpailijaOnlineA[i] == NULL)
				 break;
		 if (i < NFORMKILPAILIJA)
			 FormKilpailijaOnlineA[i] = new TFormKilpailijaOnline(FormMain);
		 else
			 i = 0;
		 }
	 FormKilpailijaOnlineA[i]->sallimuokkaus = false;
	 FormKilpailijaOnlineA[i]->Show();
	if (FormKilpailijaOnlineA[i]->WindowState == wsMinimized)
		FormKilpailijaOnlineA[i]->WindowState = wsNormal;
	FormKilpailijaOnlineA[i]->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::KilpailijaU2Click(TObject *Sender)
{
	KilpailijaOnlineClick(-1);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::N1kilpailija2Click(TObject *Sender)
{
	KilpailijaOnlineClick(0);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::N2kilpailija2Click(TObject *Sender)
{
	KilpailijaOnlineClick(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N3kilpailija2Click(TObject *Sender)
{
	KilpailijaOnlineClick(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N4kilpailija2Click(TObject *Sender)
{
	KilpailijaOnlineClick(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N5kilpailija2Click(TObject *Sender)
{
	KilpailijaOnlineClick(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N6kilpailija2Click(TObject *Sender)
{
	KilpailijaOnlineClick(5);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TilanneClick(int i)
{
	if (i < 0 || !TilFrm[i]) {
		 for (i = 0; i < NTILANNEFORM; i++)
			 if (TilFrm[i] == NULL)
				 break;
		if (i < NTILANNEFORM) {
			 TilFrm[i] = new TTilanneForm(FormMain);
			 TilFrm[i]->TilanneFormNo = i+1;
		   }
		else
		   i = 0;
		}
	TilFrm[i]->Show();
	if (TilFrm[i]->WindowState == wsMinimized)
		TilFrm[i]->WindowState = wsNormal;
	TilFrm[i]->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TilanneU1Click(TObject *Sender)
{
	TilanneClick(-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N1tilanne1Click(TObject *Sender)
{
	TilanneClick(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N2tilanne1Click(TObject *Sender)
{
	TilanneClick(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N3tilanne1Click(TObject *Sender)
{
	TilanneClick(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N4tilanne1Click(TObject *Sender)
{
	TilanneClick(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N5tilanne1Click(TObject *Sender)
{
	TilanneClick(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N6tilanne1Click(TObject *Sender)
{
	TilanneClick(5);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::EmitGrafiikkaClick(int i)
{
	if (i < 0 || !FrmGr[i]) {
		if (!emitanal_fl) {
			Application->MessageBoxW(L"Emitaikojen analyysia ei ole käynnistetty", L"Este", MB_OK);
			return;
			}
		 for (i = 0; i < NFORMVAGR; i++)
			 if (FrmGr[i] == NULL)
				 break;
		if (i < NFORMVAGR)
		   FrmGr[i] = new TFormVaGr(FormMain);
		else
		   i = 0;
		}
	FrmGr[i]->Show();
	if (FrmGr[i]->WindowState == wsMinimized)
		FrmGr[i]->WindowState = wsNormal;
	FrmGr[i]->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::EmitgrafiikkaU1Click(TObject *Sender)
{
	EmitGrafiikkaClick(-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N1graafi1Click(TObject *Sender)
{
	EmitGrafiikkaClick(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N2graafi1Click(TObject *Sender)
{
	EmitGrafiikkaClick(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N3graafi1Click(TObject *Sender)
{
	EmitGrafiikkaClick(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N4graafi1Click(TObject *Sender)
{
	EmitGrafiikkaClick(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N5graafi1Click(TObject *Sender)
{
	EmitGrafiikkaClick(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N6graafi1Click(TObject *Sender)
{
	EmitGrafiikkaClick(5);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Asetukset1Click(TObject *Sender)
{
   TVertAikaFrm *vaFrm = new TVertAikaFrm(FormMain);
   vaFrm->ShowModal();
   delete vaFrm;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Yhteenveto1Click(TObject *Sender)
{
	if (!emitanal_fl) {
		Application->MessageBoxW(L"Emitaikojen analyysia ei ole käynnistetty", L"Este", MB_OK);
		return;
		}
   if (!Yv1)
	  Yv1 = new TFormYhtveto(FormMain);
   Yv1->Show();
	if (Yv1->WindowState == wsMinimized)
		Yv1->WindowState = wsNormal;
	Yv1->BringToFront();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ViimTapahtumatClick(int i)
{
	 if (i < 0 || !FrmTap[i]) {
		 for (i = 0; i < NTAPFORM; i++)
			 if (FrmTap[i] == NULL)
				 break;
		 if (i < NTAPFORM)
			 FrmTap[i] = new TTapForm(FormMain);
		 else
			 i = 0;
		 }
	 FrmTap[i]->Show();
	if (FrmTap[i]->WindowState == wsMinimized)
		FrmTap[i]->WindowState = wsNormal;
	FrmTap[i]->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ViimTapahtumatU1Click(TObject *Sender)
{
	ViimTapahtumatClick(-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N1ikkuna1Click(TObject *Sender)
{
	ViimTapahtumatClick(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N2ikkuna1Click(TObject *Sender)
{
	ViimTapahtumatClick(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N3ikkuna1Click(TObject *Sender)
{
	ViimTapahtumatClick(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N4ikkuna1Click(TObject *Sender)
{
	ViimTapahtumatClick(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N5ikkuna1Click(TObject *Sender)
{
	ViimTapahtumatClick(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N6ikkuna1Click(TObject *Sender)
{
	ViimTapahtumatClick(5);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if (!suljesuoraan && Application->MessageBox(L"Haluatko todella poistua ohjelmasta?", L"Vahvista",
	  MB_YESNO) != IDYES) {
		CanClose = false;
	  }
	else {
		if (ToimintaTila == 2)
			talljarj();
		lopetus1();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Asetukset2Click(TObject *Sender)
{
	if (!AsetusForm)
		AsetusForm = new TAsetusForm(this);
	AsetusForm->Show();
	if (AsetusForm->WindowState == wsMinimized)
		AsetusForm->WindowState = wsNormal;
	AsetusForm->BringToFront();
}
//---------------------------------------------------------------------------
void TallennaIkkunaYhtV(TextFl *tallfl, TForm *Frm, int level)
{
	tallfl->put_wxml_d(L"vaakapos", Frm->Left, level);
	tallfl->put_wxml_d(L"pystypos", Frm->Top, level);
	tallfl->put_wxml_d(L"leveys", Frm->Width, level);
	tallfl->put_wxml_d(L"korkeus", Frm->Height, level);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::TallennaIkkunatV(UnicodeString FlNm)
{
	TextFl *tallfl;
	int level = 0;

	if (ikkParams) {
		TallennaIkkunat(FlNm);
		return;
		}
	tallfl = new TextFl(FlNm.c_str(), L"wt");

	if (tallfl->IsOpen()) {
		tallfl->put_wtag(L"ikkunat", level++);
		tallfl->put_wtag(L"ikkuna", level++);
		tallfl->put_wxml_s(L"laji", L"pääkaavake", level);
		TallennaIkkunaYhtV(tallfl, FormMain, level);
		tallfl->put_wantitag(L"ikkuna", level);
		for (int i = 0; i < NTILANNEFORM; i++) {
			if (TilFrm[i] && TilFrm[i]->Visible) {
				tallfl->put_wtag(L"ikkuna", level++);
				tallfl->put_wxml_s(L"laji", L"tilanne", level);
				tallfl->put_wxml_d(L"numero", i+1, level);
				TallennaIkkunaYhtV(tallfl, TilFrm[i], level);
				tallfl->put_wxml_d(L"sarja", TilFrm[i]->SrjVal->ItemIndex, level);
				if (TilFrm[i]->IkkParam.smartJrj)
					tallfl->put_wxml_d(L"piste", -2, level);
				else
					tallfl->put_wxml_d(L"piste", TilFrm[i]->PisteVal->ItemIndex, level);
				tallfl->put_wantitag(L"ikkuna", --level);
				}
			}
		for (int i = 0; i < NTAPFORM; i++) {
			if (FrmTap[i] && FrmTap[i]->Visible) {
				tallfl->put_wtag(L"ikkuna", level++);
				tallfl->put_wxml_s(L"laji", L"tapahtumat", level);
				tallfl->put_wxml_d(L"numero", i+1, level);
				TallennaIkkunaYhtV(tallfl, FrmTap[i], level);
				tallfl->put_wxml_d(L"piste", FrmTap[i]->PisteVal->ItemIndex, level);
				tallfl->put_wantitag(L"ikkuna", --level);
				}
			}
		if (FormEmit && FormEmit->Visible) {
			tallfl->put_wtag(L"ikkuna", level++);
			tallfl->put_wxml_s(L"laji", L"emitluenta", level);
			TallennaIkkunaYhtV(tallfl, FormEmit, level);
			tallfl->put_wantitag(L"ikkuna", --level);
			}
		if (FormIlm2 && FormIlm2->Visible) {
			tallfl->put_wtag(L"ikkuna", level++);
			tallfl->put_wxml_s(L"laji", L"ilmoittautumiset", level);
			TallennaIkkunaYhtV(tallfl, FormIlm2, level);
			tallfl->put_wantitag(L"ikkuna", --level);
			}
		if (FormStatus && FormStatus->Visible) {
			tallfl->put_wtag(L"ikkuna", level++);
			tallfl->put_wxml_s(L"laji", L"status", level);
			TallennaIkkunaYhtV(tallfl, FormStatus, level);
			tallfl->put_wantitag(L"ikkuna", --level);
			}
		tallfl->put_wantitag(L"ikkunat", --level);
		}
	delete tallfl;
}
//---------------------------------------------------------------------------

static wchar_t *tags[] = {
	L"ikkunat",
	L"ikkuna",
	L"laji",
	L"numero",
	L"vaakapos",
	L"pystypos",
	L"leveys",
	L"korkeus",
	L"piste",
	L"sarja"
	};

static taglkm = 10;

void __fastcall TFormMain::HaeIkkunatV(UnicodeString FlNm)
{
	TextFl *haefl;
	wchar_t line[200], cl, *p, st[100] = L"";
	int state = 0, antifl, tagno, val, laji, no = 0, valmis = 0;
	int lft = 0, tp = 0, wdth = 0, hgth = 0, pst = 0, srj = 0;
	TForm *Frm;

	haefl = new TextFl(FlNm.c_str(), L"rt");
	if (haefl->IsOpen()) {
		while (!haefl->Feof()) {
			if (haefl->ReadLine(line, sizeof(line)/2-1) == NULL) {
				break;
				}
			if (wmemcmp(line, L"<Ikkunat", 8) == 0)	{
				delete haefl;
				HaeIkkunat(FlNm);
				return;
				}
			while (wcslen(line) > 0 && (cl = line[wcslen(line)-1]) == L'\r' || cl == L'\n' || cl == L' ') {
				line[wcslen(line)-1] = 0;
				}
			if (wcslen(line) < 1)
				continue;
			elimwbl(line);
			antifl = 0;
			if (line[0] == L'<') {
				p = wcstok(line, L"<>");
				if (p) {
					wcsncpy(st, p, 99);
					elimwbl(st);
					if (st[0] == L'/') {
						antifl = 1;
						wmemmove(st, st+1, wcslen(st));
						}
					tagno = -1;
					for (int i = 0; i < taglkm; i++) {
						if (!wcscmp(st, tags[i])) {
							 tagno = i;
							 break;
							 }
						}
					switch (tagno) {
						case 0:
							if (state == 0 && antifl == 0) {
								state = 1;
								}
							else
								state = -1;
							break;
						case 1:
							if (state == 1 && antifl == 0) {
								state = 2;
								laji = 0;
								no = 0;
								valmis = 0;
								}
							else if (antifl) {
								state = 1;
								valmis = 1;
								}
							else
								state = -1;
							break;
						default:
							if (tagno > 1 && tagno < taglkm) {
								if (antifl) {
									state = 2;
									}
								else {
									state++;
									p = wcstok(NULL, L"<>");
									if (p) {
										wcscpy(st, p);
										p = wcstok(NULL, L"<>");
										if (p) {
											elimwbl(p);
											if (*p == L'/' && !wcscmp(p+1, tags[tagno])) {
												if (tagno == 2) {
													if (!wcscmp(st, L"pääkaavake")) {
														laji = 0;
														}
													else if (!wcscmp(st, L"tilanne")) {
														laji = 1;
														}
													else if (!wcscmp(st, L"tapahtumat")) {
														laji = 2;
														}
													else if (!wcscmp(st, L"emitluenta")) {
														laji = 3;
														}
													else if (!wcscmp(st, L"ilmoittautumiset")) {
														laji = 4;
														}
													else if (!wcscmp(st, L"status")) {
														laji = 5;
														}
													}
												else {
													val = _wtoi(st);
													switch (tagno) {
														case 3:
															no = val;
															break;
														case 4:
															lft = val;
															break;
														case 5:
															tp = val;
															break;
														case 6:
															wdth = val;
															break;
														case 7:
															hgth = val;
															break;
														case 8:
															pst = val;
															break;
														case 9:
															srj = val;
															break;
														}
													}
												state--;
												}
											}
										}

									}
								}
							break;

						}
					}
				}
				if (valmis) {
					switch (laji) {
						case 0:
							Frm = FormMain;
							break;
						case 1:
							if (!TilFrm[no-1])
								TilFrm[no-1] = new TTilanneForm(FormMain);
							TilFrm[no-1]->TilanneFormNo = no;
							Frm = TilFrm[no-1];
							break;
						case 2:
							if (!FrmTap[no-1])
								FrmTap[no-1] = new TTapForm(FormMain);
							Frm = FrmTap[no-1];
							break;
						case 3:
							if (!FormEmit)
								FormEmit = new TFormEmit(FormMain);
							Frm = FormEmit;
							break;
						case 4:
							if (!FormIlm2)
								FormIlm2 = new TFormIlm2(FormMain);
							Frm = FormIlm2;
							break;
						case 5:
//							if (!FormStatus)
//								FormStatus = new TFormStatus(FormMain);
							Frm = FormStatus;
							break;
						}
					Frm->Left = lft;
					Frm->Top = tp;
					Frm->Width = wdth;
					Frm->Height = hgth;
					Frm->Show();
					switch (laji) {
						case 1:
							TilFrm[no-1]->SrjVal->ItemIndex = srj;
							if (pst == -2) {
								TilFrm[no-1]->Kaikkiajat1Click(FormMain);
								TilFrm[no-1]->IkkParam.smartJrj = true;
								}
							else
								TilFrm[no-1]->PisteVal->ItemIndex = pst;
							TilFrm[no-1]->IkkParam.Sarja = srj;
							TilFrm[no-1]->IkkParam.Va = pst;
							TilFrm[no-1]->PaivBtnClick(this);
							break;
						case 2:
							FrmTap[no-1]->PisteVal->ItemIndex = pst;
							break;
						}
					valmis = 0;
					}
			}
		delete haefl;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TallennaikkunatClick(TObject *Sender)
{
	static init = 0;

	if (!init) {
		SaveDialog1->FileName = GetCurrentDir()+UnicodeString(L"\\ikkunat.xml");
		init = 1;
	}
	SaveDialog1->DefaultExt = L"xml";
	SaveDialog1->Filter = L"xml-tiedostot (*.xml)|*.XML|Kaikki tiedostot (*.*)|*.*";
//	if (Application->MessageBoxW(L"Tallennetaanko laajat tiedot (uusi formaatti)", L"Tallennuslaajuus", MB_YESNO) == IDYES)
		ikkParams = true;
//	else
//		ikkParams = false;
	if (SaveDialog1->Execute()) {
		TallennaIkkunat(SaveDialog1->FileName);
		}
	SetCurrentDir(CurrentDir);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::HaeikkunatClick(TObject *Sender)
{
	static init = 0;

	if (!init) {
		OpenDialog1->FileName = GetCurrentDir()+UnicodeString(L"\\ikkunat.xml");
		init = 1;
	}
	OpenDialog1->DefaultExt = L"xml";
	OpenDialog1->Filter = L"Ikkunatiedostot (*.ikk,*.xml)|*.IKK;*.XML|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		HaeIkkunat(OpenDialog1->FileName);
		}
	SetCurrentDir(CurrentDir);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Loppukilpailunmuodostaminen1Click(TObject *Sender)
{
	if (!LoppuKilpForm)
		LoppuKilpForm = new TLoppuKilpForm(FormMain);
	LoppuKilpForm->Show();
	if (LoppuKilpForm->WindowState == wsMinimized)
		LoppuKilpForm->WindowState = wsNormal;
	LoppuKilpForm->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::akaaajolhtjenmuodostaminen1Click(TObject *Sender)
{
	if (!TakaaAjoForm)
		TakaaAjoForm = new TTakaaAjoForm(FormMain);
	TakaaAjoForm->Show();
	if (TakaaAjoForm->WindowState == wsMinimized)
		TakaaAjoForm->WindowState = wsNormal;
	TakaaAjoForm->BringToFront();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Vliaikojenhakupalvelimelta1Click(TObject *Sender)
{
#ifdef EI_OLE
	if (!GetHttpForm)
		GetHttpForm = new TGetHttpForm(FormMain);
	GetHttpForm->Show();
#endif
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::BtnTulPalvClick(TObject *Sender)
{
	int vast = mrNo;
	UnicodeString cfgFName = L"";

	if (kisaluettelo) {
		FormValitseKilp = new TFormValitseKilp(this);
		FormValitseKilp->ilmTila = false;
		vast = FormValitseKilp->ShowModal();
		if (vast == mrYes)
			cfgFName = FormValitseKilp->cfgFName;
		}
	if (vast == mrNo) {
		OpenDialog2->FileName = CurrentDir+UnicodeString(L"laskenta.cfg");
		OpenDialog2->DefaultExt = L"cfg";
		OpenDialog2->Filter = L"Konfiguraatiot (*.cfg)|*.CFG|Kaikki tiedostot (*.*)|*.*";
		if (OpenDialog2->Execute())
			cfgFName = OpenDialog2->FileName;
		else
			vast = mrCancel;
		}
	if ((vast == mrYes || vast == mrNo) && cfgFName.Length() > 2) {
		ToimintaTila = 2;
		Initialisoi(NULL, cfgFName.c_str());
		}
	else
		Application->MessageBoxW(L"Avaaminen peruutettu", L"", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Haeerottimellisestatekstitiedostosta1Click(TObject *Sender)
{
	if (!FormImport)
		FormImport = new TFormImport(FormMain);
	FormImport->Show();
	if (FormImport->WindowState == wsMinimized)
		FormImport->WindowState = wsNormal;
	FormImport->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Status1Click(TObject *Sender)
{
	FormStatus->Show();
	if (FormStatus->WindowState == wsMinimized)
		FormStatus->WindowState = wsNormal;
	FormStatus->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Button2Click(TObject *Sender)
{
	FormKilpMaaritys = new TFormKilpMaaritys(FormMain);
	FormKilpMaaritys->ShowModal();
	delete FormKilpMaaritys;
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::BtnIlmTilaClick(TObject *Sender)
{
	int vast = mrNo;
	UnicodeString cfgFName = L"";

	if (kisaluettelo) {
		FormValitseKilp = new TFormValitseKilp(this);
		FormValitseKilp->ilmTila = true;
		vast = FormValitseKilp->ShowModal();
		if (vast == mrYes)
			cfgFName = FormValitseKilp->cfgFName;
		}
	if (vast == mrNo) {
		OpenDialog2->FileName = CurrentDir+UnicodeString(L"ilmoitt.cfg");
		OpenDialog2->DefaultExt = L"cfg";
		OpenDialog2->Filter = L"Konfiguraatiot (*.cfg)|*.CFG|Kaikki tiedostot (*.*)|*.*";
		if (OpenDialog2->Execute())
			cfgFName = OpenDialog2->FileName;
		else
			vast = mrCancel;
		}
	if ((vast == mrYes || vast == mrNo) && cfgFName.Length() > 2) {
		ToimintaTila = 1;
		Initialisoi(NULL, cfgFName.c_str());
		}
	else
		Application->MessageBoxW(L"Avaaminen peruutettu", L"", MB_OK);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Nollaa1Click(TObject *Sender)
{
	if (!FormNollaus)
		FormNollaus = new TFormNollaus(FormMain);
	FormNollaus->Show();
	if (FormNollaus->WindowState == wsMinimized)
		FormNollaus->WindowState = wsNormal;
	FormNollaus->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Lisvakantteja1Click(TObject *Sender)
{
	if (!FormVakLisays)
		FormVakLisays = new TFormVakLisays(FormMain);
	FormVakLisays->Show();
	if (FormVakLisays->WindowState == wsMinimized)
		FormVakLisays->WindowState = wsNormal;
	FormVakLisays->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Numeroi1Click(TObject *Sender)
{
   if (!FormArvonta)
		FormArvonta = new TFormArvonta(FormMain);
	FormArvonta->Show();
	if (FormArvonta->WindowState == wsMinimized)
		FormArvonta->WindowState = wsNormal;
	FormArvonta->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Ilmoittautumiset1Click(TObject *Sender)
{
   if (!FormIlm2)
		FormIlm2 = new TFormIlm2(FormMain);
	FormIlm2->Left = 30*Screen->PixelsPerInch/96;
	FormIlm2->Show();
	if (FormIlm2->WindowState == wsMinimized)
		FormIlm2->WindowState = wsNormal;
	FormIlm2->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Seuranimienkopioinnit1Click(TObject *Sender)
{
   if (!FormSeuratoimet)
		FormSeuratoimet = new TFormSeuratoimet(FormMain);
	FormSeuratoimet->Show();
	if (FormSeuratoimet->WindowState == wsMinimized)
		FormSeuratoimet->WindowState = wsNormal;
	FormSeuratoimet->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Avausmritykset1Click(TObject *Sender)
{
	if (!FormAvausMaar)
		FormAvausMaar = new TFormAvausMaar(FormMain);
	FormAvausMaar->Show();
	if (FormAvausMaar->WindowState == wsMinimized)
		FormAvausMaar->WindowState = wsNormal;
	FormAvausMaar->BringToFront();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Osanottajat1Click(TObject *Sender)
{
	if (!FormOsanottajat)
		FormOsanottajat = new TFormOsanottajat(this);
	FormOsanottajat->Show();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Sarjatiedot1Click(TObject *Sender)
{
	FormSarjatiedot->Show();
	if (FormSarjatiedot->WindowState == wsMinimized)
		FormSarjatiedot->WindowState = wsNormal;
	FormSarjatiedot->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::iedot1Click(TObject *Sender)
{
	FormOhjtiedot = new TFormOhjtiedot(this);
	FormOhjtiedot->ShowModal();
	delete FormOhjtiedot;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ListBox1Click(TObject *Sender)
{
	wchar_t title[30] = L"";
	int r;

	if (ListBox1->Items->Count == 0)
		return;
	r = ListBox1->ItemIndex;
	if (r < 0 || r >= ListBox1->Items->Count || ListBox1->Items->Strings[r] == NULL)
		return;
	wcsncpy(title, ListBox1->Items->Strings[r].c_str(), 29);

	for (int i = 0; i < nForms; i++) {
		if (!wmemcmp(title, FormList[i].Title, wcslen(FormList[i].Title))) {
			if ((*FormList[i].Form)->WindowState == wsMinimized) {
				(*FormList[i].Form)->WindowState = wsNormal;
				}
			(*FormList[i].Form)->BringToFront();
			break;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SetFormKohde(TForm *Frm, wchar_t *Kohde)
{
	for (int i = 0; i < nForms; i++) {
		if (Frm == *FormList[i].Form) {
			wcsncpy(FormList[i].Kohde, Kohde, sizeof(FormList[i].Kohde)/2-1);
		}
	}
}

void __fastcall TFormMain::Kirjoitasiirtotiedostoon1Click(TObject *Sender)
{
   if (!FormExport)
		FormExport = new TFormExport(FormMain);
	FormExport->Show();
	if (FormExport->WindowState == wsMinimized)
		FormExport->WindowState = wsNormal;
	FormExport->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Help2Click(TObject *Sender)
{
	Application->HelpKeyword(L"HkKisaWin");
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"HkKisaWin");
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Siirrratakoodit1Click(TObject *Sender)
{
   //
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Siirrlistietoja1Click(TObject *Sender)
{
   if (!FormLisatiedot)
		FormLisatiedot = new TFormLisatiedot(FormMain);
	FormLisatiedot->Show();
	if (FormLisatiedot->WindowState == wsMinimized)
		FormLisatiedot->WindowState = wsNormal;
	FormLisatiedot->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Merkitsetarkastuskoodeja1Click(TObject *Sender)
{
   if (!FormTarkMerk)
		FormTarkMerk = new TFormTarkMerk(FormMain);
	FormTarkMerk->Show();
	if (FormTarkMerk->WindowState == wsMinimized)
		FormTarkMerk->WindowState = wsNormal;
	FormTarkMerk->BringToFront();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::COMFILEDAT1Click(TObject *Sender)
{
   if (!FormComfile)
		FormComfile = new TFormComfile(FormMain);
	FormComfile->Show();
	if (FormComfile->WindowState == wsMinimized)
		FormComfile->WindowState = wsNormal;
	FormComfile->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Laskekilpailijoidenpisteet1Click(TObject *Sender)
{
   if (!FormPistelasku)
		FormPistelasku = new TFormPistelasku(FormMain);
	FormPistelasku->Show();
	if (FormPistelasku->WindowState == wsMinimized)
		FormPistelasku->WindowState = wsNormal;
	FormPistelasku->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Emitluenta1Click(TObject *Sender)
{
   if (!FormEmit)
		FormEmit = new TFormEmit(FormMain);
//   FormEmit->Left = Screen->Width - FormEmit->Width;
	if (esiluenta) {
		FormEmit->Left = Left+Width+10*Screen->PixelsPerInch/96;
		FormEmit->Top = Top;
		}
	else {
		FormEmit->Left = 20*Screen->PixelsPerInch/96;
		FormEmit->Top = Top + 20*Screen->PixelsPerInch/96;;
		}
   FormEmit->Show();
	if (FormEmit->WindowState == wsMinimized)
		FormEmit->WindowState = wsNormal;
	FormEmit->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::AjanottoClick(int i)
{
   if (!FormAjanotot[i])
		FormAjanotot[i] = new TFormAjanotto(FormMain);
	FormAjanotot[i]->Show();
	if (FormAjanotot[i]->WindowState == wsMinimized)
		FormAjanotot[i]->WindowState = wsNormal;
	FormAjanotot[i]->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Ajanotto11Click(TObject *Sender)
{
	AjanottoClick(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Ajanotto21Click(TObject *Sender)
{
	AjanottoClick(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Ajanotto31Click(TObject *Sender)
{
	AjanottoClick(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Ajanotto41Click(TObject *Sender)
{
	AjanottoClick(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Kirjoitinjatiedostotulosteet1Click(TObject *Sender)
{
	if (!TulosteForm)
		TulosteForm = new TTulosteForm(FormMain);
	TulosteForm->Show();
	if (TulosteForm->WindowState == wsMinimized)
		TulosteForm->WindowState = wsNormal;
	TulosteForm->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Sprinthiihdonsiirrot1Click(TObject *Sender)
{
	static int ed_k_pv = -1;

	if (FormSprinthiihto && k_pv != ed_k_pv && ed_k_pv >= 0) {
		delete FormSprinthiihto;
		FormSprinthiihto = NULL;
		}
	if (!FormSprinthiihto)
		FormSprinthiihto = new TFormSprinthiihto(FormMain);
	ed_k_pv = k_pv;
	FormSprinthiihto->Show();
	if (FormSprinthiihto->WindowState == wsMinimized)
		FormSprinthiihto->WindowState = wsNormal;
	FormSprinthiihto->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Muutasarjanlhtaikoja1Click(TObject *Sender)
{
//	if (!FormLahtoajat)
//		FormLahtoajat = new TFormLahtoajat(FormMain);
//	FormLahtoajat->Show();
	if (!FormAikaSiirto)
		FormAikaSiirto = new TFormAikaSiirto(this);
	FormAikaSiirto->Show();
	if (FormAikaSiirto->WindowState == wsMinimized)
		FormAikaSiirto->WindowState = wsNormal;
	FormAikaSiirto->BringToFront();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::MySQL1Click(TObject *Sender)
{
	if (!FormMySQL)
		FormMySQL = new TFormMySQL(FormMain);
	FormMySQL->Show();
	if (FormMySQL->WindowState == wsMinimized)
		FormMySQL->WindowState = wsNormal;
	FormMySQL->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Ratatiedot1Click(TObject *Sender)
{
	if (!FormRadat) {
		FormRadat = new TFormRadat(FormMain);
	}
	FormRadat->Show();
	if (FormRadat->WindowState == wsMinimized)
		FormRadat->WindowState = wsNormal;
	FormRadat->BringToFront();
}
//---------------------------------------------------------------------------
void ProsEmitThread(LPVOID)
{
	int bytecount0 = 0;

	while(taustaon && ProsEmitOn) {
		if (vaiheenvaihto || FormEmit == NULL || !FormEmit->Initialisoitu ||
			(!FormEmit->CBLue->Checked && !FormEmit->CBViim->Checked)) {
			Sleep(1000);
			continue;
			}
//		if (emithead2 >= emithead) {
		if ((bytecount - bytecount0 + bytecountmax) % bytecountmax > (bytecountmax/100)) {
			bytecount0 = bytecount;
			FormEmit->ProgressBar1->Position = 100*bytecount / bytecountmax;
			FormEmit->ProgressBar1->Refresh();
			}
		if (vaiheenvaihto || FormEmit->Tietue >= emithead-1) {
			Sleep(200);
			continue;
			}
		if (!vaiheenvaihto && FormEmit != NULL) {
			FormEmit->HaeUusiTietue();
			if (!FormEmit->CBViim->Checked)
				Sleep(1000);
			else
				Sleep(100);
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::lueInit(int *TTila, int *KsMoodi, bool *kLuett, bool *avaa, UnicodeString *cfgflnm)
{
	wchar_t line[200];

	TextFl *InitFile = new TextFl((InitDir+L"\\Init.cfg").c_str(), L"r");
	if (InitFile->IsOpen()) {
		while(!InitFile->Feof()) {

			InitFile->ReadLine(line, 198);

			upcasewstr(line);
			if (!wmemcmp(line, L"ILM", 3)) {
				*TTila = 1;
				continue;
				}
			if (!wmemcmp(line, L"TULOSP", 6)) {
				*TTila = 2;
				continue;
				}
			if (!wmemcmp(line, L"KUNT", 4)) {
				*KsMoodi = -1;
				continue;
				}
			if (!wmemcmp(line, L"LUETT", 5)) {
				*kLuett = true;
				continue;
				}
			if (!wmemcmp(line, L"AVAA", 4)) {
				*avaa = true;
				continue;
				}
			if (!wmemcmp(line, L"CFG", 3)) {
				wchar_t *p;

				if ((p = wcsstr(line, L"=")) != NULL) {
					if (p[wcslen(p)-1] < L' ')
						p[wcslen(p)-1] = 0;
					*cfgflnm = UnicodeString(p+1);
					}
				continue;
				}
			}
		}
	delete InitFile;
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::Aloita(void)
{
	int k,cc;
	static int init;
	UnicodeString cfgflnm = L"", path = L"";
	bool Avaa = false;

	if (!init) {
		wchar_t *p, buf[200];
		GetModuleFileName(NULL, buf, sizeof(buf)/2-1);
		for (p = buf + wcslen(buf)-1; p > buf; p--)
			if (*p == L'\\' || *p == L':') {
				p++;
				break;
				}
		wcsncpy(exeName, p, sizeof(exeName)/2-1);
		init = 1;
		lueInit(&ToimintaTila, &kuntosuunnmoodi, &kisaluettelo, &Avaa, &cfgflnm);
		if (kuntosuunnmoodi == -1) {
			Button2->Enabled = false;
			BtnIlmTila->Enabled = false;
			}
		if (Avaa) {
			if (kisaluettelo) {
				int vast = mrNo;

				cfgflnm = L"";

				FormValitseKilp = new TFormValitseKilp(this);
				FormValitseKilp->ilmTila = ToimintaTila == 1;
				FormValitseKilp->Avaa = true;
				FormValitseKilp->ValitseKilp();
				vast = FormValitseKilp->ModalResult;
				FormValitseKilp->Avaa = false;
				if (vast == mrYes) {
					path = FormValitseKilp->Path;
					cfgflnm = FormValitseKilp->cfgFName;
//                    wcsncpy(paaots, FormValitseKilp->Title.c_str(), sizeof(paaots)/2-1);
					}
				else
					ToimintaTila = 0;
				delete FormValitseKilp;
				}
			if (cfgflnm.Length() > 0) {
				if (ToimintaTila == 0)
				   ToimintaTila = 2;
				bootfl = 1;
				Initialisoi(path.c_str(), cfgflnm.c_str());
				}
			}
		}
}
//---------------------------------------------------------------------------
MESSAGE void __fastcall TFormMain::CloseReqHandler(TMyMessage &msg)
{
	if (msg.lparam == 1) {
		erbeep();
		Sleep(200);
		erbeep();
		writewarning_w(L"Ohjelma suljetaan 3 s kuluttua verkkokomennon perusteella", 3000);
		}
	suljesuoraan = 1;
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Memo1DblClick(TObject *Sender)
{
	static bool init = false;
	FormMessages->Show();
	if (!init) {
		init = true;
		FormMessages->Left = Screen->Width - FormMessages->Width;
		FormMessages->Top = Screen->Height - FormMessages->Height - 30 * Screen->PixelsPerInch / 96;
		}
	if (FormMessages->WindowState == wsMinimized)
		FormMessages->WindowState = wsNormal;
	FormMessages->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Sanomaikkuna1Click(TObject *Sender)
{
	if (FormMessages->Visible)
		FormMessages->Close();
	else  {
		FormMessages->Show();
		if (FormMessages->WindowState == wsMinimized)
			FormMessages->WindowState = wsNormal;
		FormMessages->BringToFront();
		}
}
//---------------------------------------------------------------------------
void SuljeKaikki(void)
{
	int wparam = 0;
	int lparam = 0;
	lkmThreadOn = false;
	if (FormJakelu)
		FormJakelu->CloseAll();
	SendMessage(FormMain->Handle,WM_MYMSGCLOSEAPPL,wparam,lparam);
}
//---------------------------------------------------------------------------

MESSAGE void __fastcall TFormMain::LukumaaraReqHandler(TMyMessage &msg)
{
	if (!vaiheenvaihto && !suljesuoraan && lkmThreadOn)
		FormStatus->Lukumaarat();
	if (suljesuoraan) {
		Sleep(500);
		FormMain->Close();
		}
}
//---------------------------------------------------------------------------

void startAutojakelu(void)
{
	SendMessage(FormMain->Handle, WM_AUTOTULOSTUS, 0, 0);
}

MESSAGE void __fastcall TFormMain::AutotulostusReqHandler(TMyMessage &msg)
{
	if (!FormJakelu) {
		FormJakelu = new TFormJakelu(FormMain);
		}
	if (!FormJakelu->Initialized) {
		FormJakelu->Show();
		FormJakelu->Hide();
		}
	FormJakelu->LueJakeluMaaritykset(autofileparam.jakelumaar);
	Sleep(1000);
	FormJakelu->openFTP();
	if (!FormJakelu->KeepOpen)
		FormJakelu->CloseAll();
}
static void tarkAutoTulostus(void)
{
	if (autofl && lkmThreadOn) {
		autotulostus();
		}
	if (autofileparam.afvali && lkmThreadOn) {
		static int t_af = TMAALI0;
		int t_ta;

		t_ta = KSEK*T_TIME(biostime(0,0));
		if (t_af == TMAALI0 || NORMKELLO(t_ta - t_af) >= 0) {
			if (t_af != TMAALI0) {
				if (autofile(0))
					t_af = t_ta + 50*KSEK;
				else
					t_af = t_ta + autofileparam.afvali;
				}
			else {
				autofile(0);
				t_af = t_ta + autofileparam.afvali;
				}
			}
		t_af = NORMKELLO(t_af);
		}
}
//---------------------------------------------------------------------------
void etHaku(void);

void LukumaaratTh(LPVOID lpCn)
	{
	int ticks = 0;
	bool inwrerr = false;

	while (taustaon && lkmThreadOn) {
		Sleep(100);
		if (vaiheenvaihto)
			continue;
		if (virhesanoma.on && !inwrerr) {
			inwrerr = true;
			virhesanoma.on = false;
			writeerror_w(virhesanoma.msg, virhesanoma.kesto);
			inwrerr = false;
			}
		if (vaiheenvaihto)
			continue;
		SendMessage(FormMain->Handle, WM_MYLUKUMAARAT, 0, 0);
		if (suljesuoraan || !lkmThreadOn)
			break;
		if (vaiheenvaihto)
			continue;
		if (autofl || autofileparam.afvali) {
			tarkAutoTulostus();
			}
		if (autofl && ticks > intv[0]) {
			for (int j = 0; j < sarjaluku + nsarjayhd; j++)
			   if( (prior[j] < 1000000000L) )
				  prior[j] = 2*prior[j] - prior[j]/2;
			ticks = 0;
			}
		else
			ticks += 2;
		etHaku();
	   }
	}
//---------------------------------------------------------------------------
/*
MESSAGE void __fastcall TFormMain::UusiAikaHandler(TMyMessage &msg)
{
	for (int i = 0; i < NAJANOTOT; i++) {
		if (FormAjanotot[i] != NULL)
			FormAjanotot[i]->Paivita_Aikanaytto();
		}
}
//---------------------------------------------------------------------------

void paivita_aikanaytto(void)
{
	SendMessage(FormMain->Handle, WM_MYUUSIAIKA, 0, 0);
}
*/
//---------------------------------------------------------------------------

void __fastcall TFormMain::Seuratjaosanottajamrt1Click(TObject *Sender)
{
	if (!FormKilpSeurat)
		FormKilpSeurat = new TFormKilpSeurat(this);
	FormKilpSeurat->Show();
	if (FormKilpSeurat->WindowState == wsMinimized)
		FormKilpSeurat->WindowState = wsNormal;
	FormKilpSeurat->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Yksiosanottaja1Click(TObject *Sender)
{
	if (!FormKilpailijatiedot)
		FormKilpailijatiedot = new TFormKilpailijatiedot(this);

	FormKilpailijatiedot->sallimuokkausvalinta = true;
	FormKilpailijatiedot->Show();
	if (FormKilpailijatiedot->WindowState == wsMinimized)
		FormKilpailijatiedot->WindowState = wsNormal;
	FormKilpailijatiedot->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Aikojenjanumeroidensiirrot1Click(TObject *Sender)
{
	if (!FormAikaSiirto)
		FormAikaSiirto = new TFormAikaSiirto(this);
	FormAikaSiirto->Show();
	if (FormAikaSiirto->WindowState == wsMinimized)
		FormAikaSiirto->WindowState = wsNormal;
	FormAikaSiirto->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::iedonsiirronuusintalhetys1Click(TObject *Sender)
{
	if (!FormUusinta)
		FormUusinta = new TFormUusinta(this);
	FormUusinta->Show();
	if (FormUusinta->WindowState == wsMinimized)
		FormUusinta->WindowState = wsNormal;
	FormUusinta->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Ammunnansakot1Click(TObject *Sender)
{
	if (!FormASakot)
		FormASakot = new TFormASakot(this);

	FormASakot->Show();
	if (FormASakot->WindowState == wsMinimized)
		FormASakot->WindowState = wsNormal;
	FormASakot->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Joukkuepisteet1Click(TObject *Sender)
{
	if (!FormJoukkueTilanne) {
		FormJoukkueTilanne = new TFormJoukkueTilanne(this);
		}
	FormJoukkueTilanne->Show();
	if (FormJoukkueTilanne->WindowState == wsMinimized)
		FormJoukkueTilanne->WindowState = wsNormal;
	FormJoukkueTilanne->BringToFront();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Nyttulokset1Click(TObject *Sender)
{
	Nyttulokset1->Checked = true;
	Nytavoimet1->Checked = false;
	Label5->Caption = L"Tuloksia";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Nytavoimet1Click(TObject *Sender)
{
	Nyttulokset1->Checked = false;
	Nytavoimet1->Checked = true;
	Label5->Caption = L"Avoimia";
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Tapahtumasarjansiirrot1Click(TObject *Sender)
{
//
	if (FormTapahtSiirrot == NULL)
		FormTapahtSiirrot = new TFormTapahtSiirrot(this);
	FormTapahtSiirrot->ShowModal();
	delete FormTapahtSiirrot;
	FormTapahtSiirrot = NULL;
//
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Kilpailuluettelo1Click(TObject *Sender)
{
	if (FormKisaLuett == NULL)
		FormKisaLuett = new TFormKisaLuett(this);
	FormKisaLuett->Show();
	if (FormKisaLuett->WindowState == wsMinimized)
		FormKisaLuett->WindowState = wsNormal;
	FormKisaLuett->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::SytEmitkoodeja1Click(TObject *Sender)
{
   if (!FormEmitKoodit)
		FormEmitKoodit = new TFormEmitKoodit(FormMain);
	FormEmitKoodit->Show();
	if (FormEmitKoodit->WindowState == wsMinimized)
		FormEmitKoodit->WindowState = wsNormal;
	FormEmitKoodit->BringToFront();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Lhettiedosto1Click(TObject *Sender)
{
	OpenDialog2->Filter = L"Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog2->Execute()) {
		if (lah_tiedosto(OpenDialog2->FileName.c_str(), 0, 1))
			Application->MessageBoxW(L"Tiedoston lähettäminen ei onnistunut. Tiedoston on sijaittava työkansiossa tai sen alihakemistossa",
				L"Ei lähetetty", MB_OK);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Lueeratjaradat1Click(TObject *Sender)
{
	OpenDialog1->FileName = L"eradat.lst";
	OpenDialog1->Filter = L"Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute())
		lueeratiedot(OpenDialog1->FileName.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Valitseer1Click(TObject *Sender)
{
	wchar_t ch;
	int era;

	era = era_akt;
	inputint_prompt(&era, L"Anna erän numero", &ch);
	if (era >= 0 && era < MAXERA)
		era_akt = era;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::LueFIStiedostoja1Click(TObject *Sender)
{
   if (!FormFIS)
		FormFIS = new TFormFIS(FormMain);
	FormFIS->Show();
	if (FormFIS->WindowState == wsMinimized)
		FormFIS->WindowState = wsNormal;
	FormFIS->BringToFront();
}
//---------------------------------------------------------------------------
void tyhjtapahtumat(void);

void __fastcall TFormMain::Vaiheenvaihto1Click(TObject *Sender)
{
	int vaihe = k_pv+2;
	wchar_t ch = 0;

	for (int cn = 0; cn < maxyhteys; cn++) {
		if (comopen[cn] && !keyclose[cn] && lahettamatta(cn)) {
			writeerror_w(L"Avoimen yhteyden lähettämättä olevat sanomat estävät vaihdon", 0, true);
			return;
			}
		}
	if (Sender == Luetiedotuudelleen1) {
		vaihe = k_pv+1;
		}
	else {
		if (vaihe > kilpparam.n_pv_akt)
			vaihe = kilpparam.n_pv_akt;
		inputint_prompt(&vaihe, L"Anna uusi vaihe.", &ch);
		if (vaihe == k_pv+1) {
			Application->MessageBoxW(L"Valittu nykyinen vaihe. Toimenpide peruutettu", L"Este", MB_OK);
			ch = ESC;
			}
		if (ch == ESC)
			return;
		}
	if (vaiheenvaihto) {
		writeerror_w(L"Vaihto jo käynnissä", 0, true);
		return;
		}

	if (vaihe <= kilpparam.n_pv_akt && vaihe > 0) {
		vaiheenvaihto = true;
		lkmThreadOn = 0;
		ProsEmitOn = false;
		Sleep(500);
		if (FormOsanottajat) {
			FormOsanottajat->Close();
			delete FormOsanottajat;
			FormOsanottajat = NULL;
			}
		if (TulosteForm) {
			TulosteForm->Close();
			delete TulosteForm;
			TulosteForm = NULL;
			}
		if (FormSprinthiihto) {
			FormSprinthiihto->Close();
			delete FormSprinthiihto;
			FormSprinthiihto = NULL;
			}
		if (FormEmit) {
			FormEmit->Close();
			delete FormEmit;
			FormEmit = NULL;
			}
		for (int i = 0; i < 4; i++) {
			if (FormAjanotot[i]) {
				FormAjanotot[i]->Close();
				delete FormAjanotot[i];
				FormAjanotot[i] = NULL;
				}
			}
		for (int i = 0; i < sizeof(TilFrm)/sizeof(TilFrm[0]); i++) {
			if (TilFrm[i]) {
				TilFrm[i]->Yhteistulos1->Visible = vaihe > 1;
				if (k_pv == 0 && vaihe > 1)
					TilFrm[i]->Yhteistulos1->Checked = true;
				if (vaihe <= 1)
					TilFrm[i]->Yhteistulos1->Checked = false;
				TilFrm[i]->FormSetup(true, true);
				}
			}
		vaihdavaihe(vaihe-1, 0);
		Numeroi1->Enabled = kilpparam.bibvaihtelee && k_pv < kilpparam.n_pv_akt - 1;
		LbVaihe->Caption = L"Tulospalvelutila - vaihe "+UnicodeString(k_pv+1);
		Loppukilpailunmuodostaminen1->Enabled = (k_pv == 0);
		Loppukilpailunmuodostaminen1->Visible = (k_pv == 0);
		akaaajolhtjenmuodostaminen1->Enabled = (k_pv < kilpparam.n_pv_akt);
		akaaajolhtjenmuodostaminen1->Visible = (k_pv < kilpparam.n_pv_akt);
		if (ch != 0)
			tyhjtapahtumat();
		lkmThreadOn = 1;
		hLkmThread = _beginthread(LukumaaratTh, 20480, 0);
		if (emitfl) {
		   ProsEmitOn = true;
		   if (FormEmit && FormEmit->Initialisoitu && FormEmit->Tietue < emithead2) {
				em_HaeViimeinen();
				FormEmit->HaeTietue(em_HaeViimeinen(), 0);
				}
		   hProsEmitThread = _beginthread(ProsEmitThread, 20480, 0);
		   }
		vaiheenvaihto = false;
		}
}
//---------------------------------------------------------------------------
MESSAGE void __fastcall TFormMain::VaihdaVaiheHandler(TMyMessage &msg)
{
	int vaihe = msg.wparam;
	int kielto = msg.lparam;

	if (vaiheenvaihto)
		 return;
	for (int cn = 0; cn < maxyhteys; cn++) {
		if (comopen[cn] && !keyclose[cn] && lahettamatta(cn)) {
			writeerror_w(L"Vaiheen vaihtoa tai uusintakäynnistystä pyydetty. Avoimen yhteyden lähettämättä olevat sanomat estävät vaihdon", 0, true);
			return;
			}
		}
	if (vaihe < kilpparam.n_pv_akt && vaihe >= 0) {
		vaiheenvaihto = true;
		lkmThreadOn = 0;
		ProsEmitOn = false;
		writewarning_w(L"Vaiheen vaihto tai uusintakäynnistys. Odota muutama sekunti.", 3000);
		if (FormOsanottajat) {
			FormOsanottajat->Close();
			delete FormOsanottajat;
			FormOsanottajat = NULL;
			}
		if (TulosteForm) {
			TulosteForm->Close();
			delete TulosteForm;
			TulosteForm = NULL;
			}
		if (FormSprinthiihto) {
			FormSprinthiihto->Close();
			delete FormSprinthiihto;
			FormSprinthiihto = NULL;
			}
		if (FormEmit) {
			FormEmit->Close();
			delete FormEmit;
			FormEmit = NULL;
			}
		for (int i = 0; i < 4; i++) {
			if (FormAjanotot[i]) {
				FormAjanotot[i]->Close();
				delete FormAjanotot[i];
				FormAjanotot[i] = NULL;
				}
			}
		for (int i = 0; i < sizeof(TilFrm)/sizeof(TilFrm[0]); i++) {
			if (TilFrm[i]) {
				TilFrm[i]->Yhteistulos1->Visible = vaihe > 0;
				if (k_pv == 0 && vaihe > 0)
					TilFrm[i]->Yhteistulos1->Checked = true;
				if (vaihe == 0)
					TilFrm[i]->Yhteistulos1->Checked = false;
				TilFrm[i]->FormSetup(true, true);
				}
			}
		vaihdavaihe(vaihe, kielto);
		Numeroi1->Enabled = kilpparam.bibvaihtelee && k_pv < kilpparam.n_pv_akt - 1;
		LbVaihe->Caption = L"Tulospalvelutila - vaihe "+UnicodeString(k_pv+1);
		Loppukilpailunmuodostaminen1->Enabled = (k_pv == 0);
		Loppukilpailunmuodostaminen1->Visible = (k_pv == 0);
		akaaajolhtjenmuodostaminen1->Enabled = (k_pv < kilpparam.n_pv_akt);
		akaaajolhtjenmuodostaminen1->Visible = (k_pv < kilpparam.n_pv_akt);
		tyhjtapahtumat();
		lkmThreadOn = 1;
		hLkmThread = _beginthread(LukumaaratTh, 20480, 0);
		if (emitfl) {
		   ProsEmitOn = true;
		   if (FormEmit && FormEmit->Initialisoitu && FormEmit->Tietue < emithead2) {
				em_HaeViimeinen();
				FormEmit->HaeTietue(em_HaeViimeinen(), 0);
				}
		   hProsEmitThread = _beginthread(ProsEmitThread, 20480, 0);
		   }
		vaiheenvaihto = false;
		}
}
//---------------------------------------------------------------------------
int pyyda_vaihdavaihe(int vaihe, int kielto)
{
	SendMessage(FormMain->Handle, WM_MYVAIHDAVAIHE, (UINT32) vaihe, kielto);
	return(0);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Sakkokierrostenseuranta1Click(TObject *Sender)
{
	if (!FormSakkoKierr)
		FormSakkoKierr = new TFormSakkoKierr(this);
	FormSakkoKierr->Show();
	if (FormSakkoKierr->WindowState == wsMinimized)
		FormSakkoKierr->WindowState = wsNormal;
	FormSakkoKierr->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormShow(TObject *Sender)
{
	Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Ampumapaikat1Click(TObject *Sender)
{
	if (!FormAmpPaikat)
		FormAmpPaikat = new TFormAmpPaikat(this);
	FormAmpPaikat->Show();
	if (FormAmpPaikat->WindowState == wsMinimized)
		FormAmpPaikat->WindowState = wsNormal;
	FormAmpPaikat->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Ampumasuunnistuksensakot1Click(TObject *Sender)
{
	if (!FormASuunn)
		FormASuunn = new TFormASuunn(this);
	FormASuunn->Show();
	if (FormASuunn->WindowState == wsMinimized)
		FormASuunn->WindowState = wsNormal;
	FormASuunn->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Yhdistettyhiihto1Click(TObject *Sender)
{
	if (!FormYhdistetty)
		FormYhdistetty = new TFormYhdistetty(this);
	FormYhdistetty->Show();
	if (FormYhdistetty->WindowState == wsMinimized)
		FormYhdistetty->WindowState = wsNormal;
	FormYhdistetty->BringToFront();
}
//---------------------------------------------------------------------------
void emitTarkOn(void)
{
	emitluenta = true;
	if (!ProsEmitOn) {
		ProsEmitOn = true;
		hProsEmitThread = _beginthread(ProsEmitThread, 20480, 0);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Tulostaulut1Click(TObject *Sender)
{
	if (!FormTaulu)
		FormTaulu = new TFormTaulu(this);
	FormTaulu->Show();
	if (FormTaulu->WindowState == wsMinimized)
		FormTaulu->WindowState = wsNormal;
	FormTaulu->BringToFront();
}
//---------------------------------------------------------------------------



