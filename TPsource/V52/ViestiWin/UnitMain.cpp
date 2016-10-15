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
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#pragma hdrstop
#include <bstrings.h>
#include <HTMLHelpViewer.hpp>

#include <process.h>
#include "UnitMain.h"
#include "UnitStatus.h"
//#include "UnitSelect.h"
#include "UnitYv.h"
#include "OsuusTiedot.h"
#include "UnitVaGr.h"
#include "UnitSeurGr.h"
#include "TulosUnit.h"
#include "VertAikaUnit.h"
#include "TulostusUnit2.h"
#include "TapUnit2.h"
#include "UnitSelect3.h"
#include "UnitOhjtiedot.h"
#include "UnitNollaus.h"
#include "UnitSarjatiedot.h"
#include "UnitKilpMaaritys.h"
#include "UnitMessages.h"
#include "UnitExport.h"
#include "UnitImport.h"
#include "UnitJoukkueet.h"
#include "UnitJoukkuetiedot.h"
#include "UnitLisatiedot.h"
#include "UnitRadat.h"
#include "UnitAvausMaar.h"
#include "UnitEmit.h"
#include "UnitYLahdot.h"
#include "UnitMerkKoodeja.h"
#include "UnitUusinta.h"
#include "UnitComfile.h"
#include "UnitArvonta.h"
#include "UnitLuenta.h"
#include "UnitAjanotto.h"
#include "UnitVaTuloste.h"
#include "UnitGAZ.h"
#include "UnitASakot.h"
#include "UnitSakkoKierr.h"
#include "UnitOsuuskopiointi.h"
#include "UnitMySQL.h"
#include "UnitLahestyjat.h"
#include "UnitJakelu.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
#pragma link "Vcl.HtmlHelpViewer"

TFormMain *FormMain;
TFormYhtveto *Yv1;
TFormVaGr *FrmGr[NFORMVAGR];
TTapForm *TapFrm[NTAPFORM];
TFormOsuus *FOs[NFORMOSUUS];
TSeurantaForm *SeurFrmGr[NSEURANTAFORM];
TTilanneForm *TilFrm[NTILANNEFORM];
TFormJoukkuetiedot *JoukkFrm[NJOUKKFORM];
TFormGAZ *FrmGAZ[2];
UCHAR selected;
int SeurLstPaivita = 0;
int SeurGrPaivita = 1;
int TlsLstPaivita = 1;
int ppi0 = 96;
extern int cpower;

tapTp Tapahtuma[MAXTAPAHTUMA];
int viimTap = MAXTAPAHTUMA-1;
bool TapEtunimiensin = true;
wchar_t tapfilename[] = L"taplista.txt";
void lopetus1(void);
void luetapahtumat(void);
void talltapahtumat(void);
void LukumaaratTh(LPVOID lpCn);
void ProsEmitThread(LPVOID);
void asetaOletusarvot(void);

typedef struct {
	TForm **Form;
	wchar_t	Title[20];
	wchar_t	Kohde[32];
} FormItem;

extern wchar_t ikkunaots[];

FormItem  FormList[] = {
	{(TForm **)&FormStatus, L"Yleistilanne"},
	{(TForm **)&FormSarjatiedot, L"Sarjatiedot"},
	{(TForm **)&FormJoukkueet, L"Joukkuetaulukko"},
	{(TForm **)&TulosteForm, L"Tulosteet"},
	{(TForm **)&FormEmit, L"Emittoiminnot"},
	{(TForm **)&FormAjanotot[0], L"Ajanotto 1"},
	{(TForm **)&FormAjanotot[1], L"Ajanotto 2"},
	{(TForm **)&FormAjanotot[2], L"Ajanotto 3"},
	{(TForm **)&FormAjanotot[3], L"Ajanotto 4"},
	{(TForm **)&FormLahestyjat[0], L"Lähestyjät 1"},
	{(TForm **)&FormLahestyjat[1], L"Lähestyjät 2"},
	{(TForm **)&FormLahestyjat[2], L"Lähestyjät 3"},
	{(TForm **)&FormLahestyjat[3], L"Lähestyjät 4"},
	{(TForm **)&TilFrm[0], L"Tilanne 1:"},
	{(TForm **)&TilFrm[1], L"Tilanne 2:"},
	{(TForm **)&TilFrm[2], L"Tilanne 3:"},
	{(TForm **)&TilFrm[3], L"Tilanne 4:"},
	{(TForm **)&TilFrm[4], L"Tilanne 5:"},
	{(TForm **)&TilFrm[5], L"Tilanne 6:"},
	{(TForm **)&TapFrm[0], L"Viim. tap. 1:"},
	{(TForm **)&TapFrm[1], L"Viim. tap. 2:"},
	{(TForm **)&TapFrm[2], L"Viim. tap. 3:"},
	{(TForm **)&TapFrm[3], L"Viim. tap. 4:"},
	{(TForm **)&JoukkFrm[0], L"Joukk. 1:"},
	{(TForm **)&JoukkFrm[1], L"Joukk. 2:"},
	{(TForm **)&JoukkFrm[2], L"Joukk. 3:"},
	{(TForm **)&JoukkFrm[3], L"Joukk. 4:"},
	{(TForm **)&JoukkFrm[4], L"Joukk. 5:"},
	{(TForm **)&JoukkFrm[5], L"Joukk. 6:"},
	{(TForm **)&FOs[0], L"Os-tieto 1:"},
	{(TForm **)&FOs[1], L"Os-tieto 2:"},
	{(TForm **)&FOs[2], L"Os-tieto 3:"},
	{(TForm **)&FOs[3], L"Os-tieto 4:"},
	{(TForm **)&FOs[4], L"Os-tieto 5:"},
	{(TForm **)&FOs[5], L"Os-tieto 6:"},
	{(TForm **)&SeurFrmGr[0], L"Seur-graf 1:"},
	{(TForm **)&SeurFrmGr[1], L"Seur-graf 2:"},
	{(TForm **)&SeurFrmGr[2], L"Seur-graf 3:"},
	{(TForm **)&SeurFrmGr[3], L"Seur-graf 4:"},
	{(TForm **)&SeurFrmGr[4], L"Seur-graf 5:"},
	{(TForm **)&SeurFrmGr[5], L"Seur-graf 6:"},
	{(TForm **)&FrmGr[0], L"Emitgraf 1:"},
	{(TForm **)&FrmGr[1], L"Emitgraf 2:"},
	{(TForm **)&FrmGr[2], L"Emitgraf 3:"},
	{(TForm **)&FrmGr[3], L"Emitgraf 4:"},
	{(TForm **)&FrmGr[4], L"Emitgraf 5:"},
	{(TForm **)&FrmGr[5], L"Emitgraf 6:"}
	};

int nForms = sizeof(FormList)/sizeof(FormItem);


//---------------------------------------------------------------------------
__fastcall TFormMain::TFormMain(TComponent* Owner)
   : TForm(Owner)
{
	Application->HelpFile = ExtractFilePath(Application->ExeName)+UnicodeString(L"ViestiWin.chm");
	InitDir = GetCurrentDir();
#ifdef DEMO
	Caption = L"ViestiWin - DEMO versio - max 20 joukkuetta";
#endif
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
	status_on = false;
	Left = 10;

	asetaOletusarvot();
}
//---------------------------------------------------------------------------
__fastcall TFormMain::~TFormMain(void)
{

}

#define N_YHTILA 8

void __fastcall TFormMain::TarkPyynnot(void)
   {
	}
void __fastcall TFormMain::Initialisoi(wchar_t *kilphak, wchar_t *cfgflnm)
{
	int k;
	wchar_t path[200], *p, wst[100];
	char st[200];

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
		writeerror_w(L"Siirtyminen pyydettyyn toimintatilaan ei onnistunut. Vain käynnistysasetusten muuttaminen sallittua", 0);
		return;
	}
	if (ikkunaots[0])
		Caption = Caption + L" - " + ikkunaots;
	EditKilpailu->Text = UnicodeString(kilpailu);
	BtnIlmTila->Enabled = false;
	BtnTulPalv->Enabled = false;
	Joukkueet1->Enabled = true;
	Seuranta1->Enabled = true;
	tulosteet1->Enabled = true;
	Numerointi1->Enabled = true;
	Osanottajat1->Enabled = true;
	if (ToimintaTila == 1) {
		LbVaihe->Caption = L"Esivalmistelu";
		Nollaatietoja1->Enabled = true;
		}
	else {
		Ajanotto1->Visible = true;
		Ajanotto21->Visible = true;
		Tilannepisteess1->Enabled = true;
		Viimeisettapahtumat1->Enabled = true;
		Tulospalvelu1->Enabled = true;
		analyysit1->Enabled = true;
		SisLuenta->Visible = sisaanluenta != 0;
		Emitluenta1->Visible = sisaanluenta < 2;
		LbVaihe->Caption = sisaanluenta > 1 ? L"Vaihe: Sisäänluenta" : L"Tulospalvelutila";
		if (kilpparam.lsakmax > 0) {
			Ammunnansakot1->Enabled = true;
			Ammunnansakot1->Visible = true;
			Sakkoseuranta1->Enabled = true;
			Sakkoseuranta1->Visible = true;
			}
		if (taulu_com || cpower)
			GAZtaulujenohjaus1->Visible = true;
		}
	if (RastiVaTuloste) {
		Emitluenta1->Visible = false;
		Vliaikatulosteet1->Visible = true;
		}
	if (emitfl < 1 || sisaanluenta > 1) {
		analyysit1->Enabled = false;
		analyysit1->Visible = false;
		Emitluenta1->Visible = false;
//		Asetukset1->Enabled = false;
//		Asetukset1->Visible = false;
		}
	if (alkulayout[0]) {
		HaeIkkunat(GetCurrentDir()+UnicodeString(L"\\")+alkulayout);
		}
	lkmThreadOn = 1;
	hLkmThread = _beginthread(LukumaaratTh, 20480, 0);
	if (sqlparam.autostart && !FormMySQL)
		FormMySQL = new TFormMySQL(this);

	if (emitfl || sisaanluenta) {
	   ProsEmitOn = true;
	   hProsEmitThread = _beginthread(ProsEmitThread, 20480, 0);
	   }
	if (ToimintaTila == 2) {
		int nt = 0;
		for (int os = 0; os < kilpparam.ntosuus; os++) {
			nt += ntulost[os];
			for (int srj = 0; srj < sarjaluku; srj++)
				nt += ntulos[srj][os][1]+ntulos[srj][os][2];
			}
		if (nt)
			luetapahtumat();
		}
	setBrowserEmulation();
}
//---------------------------------------------------------------------------

void luetapahtumat(void)
	{
	FILE *tapfile;
	wchar_t line[200], *p, *pp;
	int tap = 0, i;

	tapfile = _wfopen(tapfilename, L"rt");
	if (tapfile) {
		while (!feof(tapfile)) {
			fgetws(line, 198, tapfile);
			while (wcslen(line) > 0 && wcswcind(line[wcslen(line)-1], L"\n\r") >= 0)
				line[wcslen(line)-1] = 0;
			if (wcslen(line) < 10)
				continue;

			p = wcstokch(line, L'\t');
			for (i = 0; p != NULL; i++) {
				switch (i) {
					case 0:
						Tapahtuma[tap].Kilpno = _wtoi(p);
						break;
					case 1:
						Tapahtuma[tap].Joukkue = p;
						break;
					case 2:
						Tapahtuma[tap].Nimi = p;
						break;
					case 3:
						Tapahtuma[tap].Maa = p;
						break;
					case 4:
						Tapahtuma[tap].Sarja = _wtoi(p);
						break;
					case 5:
						Tapahtuma[tap].Osuus = _wtoi(p);
						break;
					case 6:
						Tapahtuma[tap].Piste = _wtoi(p);
						break;
					case 7:
						Tapahtuma[tap].Sija = _wtoi(p);
						break;
					case 8:
						Tapahtuma[tap].Tulos = _wtoi(p);
						break;
					case 9:
						Tapahtuma[tap].Tark = p;
					}
				p = wcstokch(NULL, L'\t');
				}
			if (getpos(Tapahtuma[tap].Kilpno) <= 0 ||
				Tapahtuma[tap].Sarja < 0 || Tapahtuma[tap].Osuus < 0 || Tapahtuma[tap].Piste < 0 ||
				Tapahtuma[tap].Sarja >= sarjaluku ||
				Tapahtuma[tap].Osuus >= Sarjat[Tapahtuma[tap].Sarja].osuusluku ||
				Tapahtuma[tap].Piste > Sarjat[Tapahtuma[tap].Sarja].Vaikaluku())
				break;
			tap++;
			}
		fclose(tapfile);
		if (tap > 0)
			viimTap = tap-1;
//		if (tap > 0)
//			viimTap = tap;
		}
	}

void talltapahtumat(void)
	{
	FILE *tapfile;
	int tap = (viimTap + 1) % MAXTAPAHTUMA;

	tapfile = _wfopen(tapfilename, L"wt");
	if (tapfile) {
		do {
			if (getpos(Tapahtuma[tap].Kilpno) > 0 && Tapahtuma[tap].Sarja >= 0
				&& Tapahtuma[tap].Sarja < sarjaluku) {
				fwprintf(tapfile, L"%d\t%s\t%s\t%s\t%d\t%d\t%d\t%d\t%d\t%s\n",
				Tapahtuma[tap].Kilpno,
				Tapahtuma[tap].Joukkue.c_str(),
				Tapahtuma[tap].Nimi.c_str(),
				Tapahtuma[tap].Maa.c_str(),
				Tapahtuma[tap].Sarja,
				Tapahtuma[tap].Osuus,
				Tapahtuma[tap].Piste,
				Tapahtuma[tap].Sija,
				Tapahtuma[tap].Tulos,
				Tapahtuma[tap].Tark.c_str());
				}
			tap = (tap + 1) % MAXTAPAHTUMA;
			} while (tap != ((viimTap + 1) % MAXTAPAHTUMA));

		fclose(tapfile);
		}
	}

void addtapahtuma(kilptietue *kilp, int osuus, int piste)
	{
	char ln[100];
	wchar_t wln[100], wln2[80];
	int tap = (viimTap + 1) % MAXTAPAHTUMA;
	tapTp uusiTap;

	if (inLopetus) {
		return;
		}
	if (kilpparam.alalaji == L'D' && osuus == Sarjat[kilp->sarja].osuusluku - 1)
		return;
	uusiTap = Tapahtuma[viimTap];
	uusiTap.Kilpno = kilp->kilpno;
	kilp->Nimi(wln, 99, osuus, TapEtunimiensin ? -1 : 1);
	uusiTap.Nimi = wln;
//	swprintf(wln, L"%s-%d", kilp->Seura(wln2), (int) kilp->joukkue);
	kilp->Joukkue(wln, 99);
	uusiTap.Joukkue = wln;
	uusiTap.Maa = kilp->Maa(wln);
	uusiTap.Sarja = kilp->sarja;
	uusiTap.Osuus = osuus;
	uusiTap.Piste = piste;
	uusiTap.Sija = kilp->Sija(osuus, piste);
	uusiTap.Tulos = kilp->Tulos(osuus, piste, true);
	if (Sarjat[uusiTap.Sarja].maxpaikat) {
		uusiTap.Tark = kilp->Ampsakot(osuus, wln);
		}
	else
		{
		wln[0] = kilp->wTark(osuus, 1);
		wln[1] = 0;
		uusiTap.Tark = wln;
		}
	if (!(uusiTap == Tapahtuma[viimTap])) {
		Tapahtuma[tap] = uusiTap;
		viimTap = tap;
		}
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

MESSAGE void __fastcall TFormMain::LukumaaraReqHandler(TMyMessage &msg)
{
/*
	if (msg.wparam == 99) {
		suljesuoraan = true;
		sulkemassa = true;
		lkmThreadOn = 0;
		writewarning_w(L"Ohjelma suljetaan 3 s kuluttua verkkokomennon perusteella", 3000);
		}
*/
	if (!suljesuoraan && lkmThreadOn)
		FormStatus->Lukumaarat();
	if (suljesuoraan) {
		Sleep(500);
		Close();
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
				if (autofile())
					t_af = t_ta + 50*KSEK;
				else
					t_af = t_ta + autofileparam.afvali;
				}
			else {
				autofile();
				t_af = t_ta + autofileparam.afvali;
				}
			}
		t_af = NORMKELLO(t_af);
		}
}
//---------------------------------------------------------------------------

void LukumaaratTh(LPVOID lpCn)
   {
	while (taustaon && lkmThreadOn) {
		Sleep(300);
		SendMessage(FormMain->Handle, WM_MYLUKUMAARAT, 0, 0);
		if (autofl || autofileparam.afvali) {
			tarkAutoTulostus();
			}
		}
	}
//---------------------------------------------------------------------------

void __fastcall TFormMain::onShow(TObject *Sender)
{
/*
   static int init;

	if (!init) {
		aloitus(_argc, _argv);
		if (alkulayout[0])
			HaeIkkunat(GetCurrentDir()+UnicodeString("\\")+alkulayout);
		lkmThreadOn = 1;
		hLkmThread = _beginthread(LukumaaratTh, 20480, 0);
		}
	init = 1;
*/
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Lopeta1Click(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Yhteenveto1Click(TObject *Sender)
{
	if (!Yv1)
		Yv1 = new TFormYhtveto(FormMain);
	Yv1->Show();
	if (Yv1->WindowState == wsMinimized)
		Yv1->WindowState = wsNormal;
	Yv1->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::JoukkueentiedotClick(int i)
{
	if (i < 0 || !JoukkFrm[i]) {
		if (i < 0)
			i = 0;
		 for (; i < NJOUKKFORM; i++)
			 if (JoukkFrm[i] == NULL)
				 break;
		if (i < NJOUKKFORM)
		   JoukkFrm[i] = new TFormJoukkuetiedot(FormMain);
		else
		   i = 0;
		   }
	JoukkFrm[i]->Show();
	if (JoukkFrm[i]->WindowState == wsMinimized)
		JoukkFrm[i]->WindowState = wsNormal;
	JoukkFrm[i]->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Joukkueentiedot1Click(TObject *Sender)
{
	JoukkueentiedotClick(-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N1ikkuna1Click(TObject *Sender)
{
	JoukkueentiedotClick(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N2ikkuna1Click(TObject *Sender)
{
	JoukkueentiedotClick(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N3ikkna1Click(TObject *Sender)
{
	JoukkueentiedotClick(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N4ikkuna1Click(TObject *Sender)
{
	JoukkueentiedotClick(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N5ikkuna1Click(TObject *Sender)
{
	JoukkueentiedotClick(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N6ikkuna1Click(TObject *Sender)
{
	JoukkueentiedotClick(5);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::EmitanalyysitClick(int i)
{
	if (i < 0 || !FrmGr[i]) {
		if (i < 0)
			i = 0;
		 for (; i < NFORMVAGR; i++)
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

void __fastcall TFormMain::Emitanalyysit1Click(TObject *Sender)
{
	EmitanalyysitClick(-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N1ikkuna2Click(TObject *Sender)
{
	EmitanalyysitClick(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N2ikkuna2Click(TObject *Sender)
{
	EmitanalyysitClick(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N3ikkuna1Click(TObject *Sender)
{
	EmitanalyysitClick(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N4ikkuna2Click(TObject *Sender)
{
	EmitanalyysitClick(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N5ikkuna2Click(TObject *Sender)
{
	EmitanalyysitClick(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N6ikkuna2Click(TObject *Sender)
{
	EmitanalyysitClick(5);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::KilpailuneteneminenClick(int i)
{
	if (i < 0 || !SeurFrmGr[i]) {
		if (i < 0)
			i = 0;
		 for (; i < NSEURANTAFORM; i++)
			 if (SeurFrmGr[i] == NULL)
				 break;
		if (i < NSEURANTAFORM)
		   SeurFrmGr[i] = new TSeurantaForm(FormMain);
		else
		   i = 0;
		}
	SeurFrmGr[i]->Show();
	if (SeurFrmGr[i]->WindowState == wsMinimized)
		SeurFrmGr[i]->WindowState = wsNormal;
	SeurFrmGr[i]->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Kilpailuneteneminen1Click(TObject *Sender)
{
	KilpailuneteneminenClick(-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N1ikkuna3Click(TObject *Sender)
{
	KilpailuneteneminenClick(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N2ikkuna3Click(TObject *Sender)
{
	KilpailuneteneminenClick(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N3ikkuna2Click(TObject *Sender)
{
	KilpailuneteneminenClick(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N4ikkuna3Click(TObject *Sender)
{
	KilpailuneteneminenClick(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N5ikkuna3Click(TObject *Sender)
{
	KilpailuneteneminenClick(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N6ikkuna3Click(TObject *Sender)
{
	KilpailuneteneminenClick(5);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TilannepisteessClick(int i)
{
	if (i < 0 || !TilFrm[i]) {
		if (i < 0)
			i = 0;
		 for (; i < NTILANNEFORM; i++)
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

void __fastcall TFormMain::Tilannepisteess1Click(TObject *Sender)
{
	TilannepisteessClick(-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N1ikkuna4Click(TObject *Sender)
{
	TilannepisteessClick(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N2ikkuna4Click(TObject *Sender)
{
	TilannepisteessClick(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N3ikkuna3Click(TObject *Sender)
{
	TilannepisteessClick(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N4ikkuna4Click(TObject *Sender)
{
	TilannepisteessClick(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N5ikkuna4Click(TObject *Sender)
{
	TilannepisteessClick(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N6ikkuna4Click(TObject *Sender)
{
	TilannepisteessClick(5);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::OsuudentiedotClick(int i)
{
	if (i < 0 || !FOs[i]) {
		if (i < 0)
			i = 0;
		for (; i < NFORMOSUUS; i++)
			 if (FOs[i] == NULL)
				 break;
		if (i < NFORMOSUUS)
		   FOs[i] = new TFormOsuus(FormMain);
		else
			 i = 0;
		}
	FOs[i]->Show();
	if (FOs[i]->WindowState == wsMinimized)
		FOs[i]->WindowState = wsNormal;
	FOs[i]->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Osuudentiedot1Click(TObject *Sender)
{
	OsuudentiedotClick(-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N1ikkuna5Click(TObject *Sender)
{
	OsuudentiedotClick(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N2ikkuna5Click(TObject *Sender)
{
	OsuudentiedotClick(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N3ikkuna4Click(TObject *Sender)
{
	OsuudentiedotClick(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N4ikkuna5Click(TObject *Sender)
{
	OsuudentiedotClick(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N5ikkuna5Click(TObject *Sender)
{
	OsuudentiedotClick(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N6ikkuna5Click(TObject *Sender)
{
	OsuudentiedotClick(5);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Vertailuajanmrittely1Click(TObject *Sender)
{
#ifdef EMIT
	TVertAikaFrm *vaFrm = new TVertAikaFrm(FormMain);
	vaFrm->ShowModal();
	delete vaFrm;
#endif
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Pidkaavakepllimmisen1Click(TObject *Sender)
{
   if (Pidkaavakepllimmisen1->Checked == True) {
		Pidkaavakepllimmisen1->Checked = False;
	  FormMain->FormStyle = fsNormal;
      }
   else {
      Pidkaavakepllimmisen1->Checked = True;
		FormMain->FormStyle = fsStayOnTop;
      }
}
//---------------------------------------------------------------------------


/*
void __fastcall FormMain::Laadituloste1Click(TObject *Sender)
{
   TTulValForm *valFrm = new TTulValForm(FormMain);
   valFrm->ShowModal();
	delete valFrm;
}
*/
//---------------------------------------------------------------------------

void __fastcall TFormMain::ViimTapahtumatClick(int i)
{
	if (i < 0 || !TapFrm[i]) {
		if (i < 0)
			i = 0;
		 for (; i < NTAPFORM; i++)
			 if (TapFrm[i] == NULL)
				 break;
		 if (i < NTAPFORM)
		   TapFrm[i] = new TTapForm(FormMain);
		else
			 i = 0;
		}
	TapFrm[i]->Show();
	if (TapFrm[i]->WindowState == wsMinimized)
		TapFrm[i]->WindowState = wsNormal;
	TapFrm[i]->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ViimTapahtumat1Click(TObject *Sender)
{
	ViimTapahtumatClick(-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N11Click(TObject *Sender)
{
	ViimTapahtumatClick(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N2ikkuna6Click(TObject *Sender)
{
	ViimTapahtumatClick(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N3ikkuna5Click(TObject *Sender)
{
	ViimTapahtumatClick(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N4ikkuna6Click(TObject *Sender)
{
	ViimTapahtumatClick(3);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::FormCloseQuery(TObject *Sender, bool &CanClose)
{

   if (!suljesuoraan && Application->MessageBoxW(L"Haluatko todella poistua ohjelmasta?", L"Vahvista",
	  MB_YESNO) != IDYES) {
	  CanClose = false;
	  }
   else {
	  lopetus1();
	  Sleep(500);
	  }
}
//---------------------------------------------------------------------------
void TallennaIkkunaYhtV(TextFl *tallfl, TForm *Frm)
{
	tallfl->put_wxml_d(L"vaakapos", Frm->Left, 0);
	tallfl->put_wxml_d(L"pystypos", Frm->Top, 0);
	tallfl->put_wxml_d(L"leveys", Frm->Width, 0);
	tallfl->put_wxml_d(L"korkeus", Frm->Height, 0);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::TallennaIkkunatV(UnicodeString FlNm)
{
	TextFl *tallfl;

	if (ikkParams) {
		TallennaIkkunat(FlNm);
		return;
		}
	tallfl = new TextFl(FlNm.c_str(), L"wt");

	if (tallfl->IsOpen()) {
		tallfl->put_wtag(L"ikkunat", 0);
		tallfl->put_wtag(L"ikkuna", 0);
		tallfl->put_wxml_s(L"laji", L"pääkaavake", 0);
		TallennaIkkunaYhtV(tallfl, FormMain);
		tallfl->put_wantitag(L"ikkuna", 0);
		for (int i = 0; i < NTILANNEFORM; i++) {
			if (TilFrm[i] && TilFrm[i]->Visible) {
				tallfl->put_wtag(L"ikkuna", 0);
				tallfl->put_wxml_s(L"laji", L"tilanne", 0);
				tallfl->put_wxml_d(L"numero", i+1, 0);
				TallennaIkkunaYhtV(tallfl, TilFrm[i]);
				tallfl->put_wxml_d(L"sarja", TilFrm[i]->SarjaVal->ItemIndex, 0);
				tallfl->put_wxml_d(L"osuus", TilFrm[i]->OsVal->ItemIndex, 0);
				tallfl->put_wxml_d(L"piste", TilFrm[i]->PisteVal->ItemIndex, 0);
				tallfl->put_wantitag(L"ikkuna", 0);
				}
			}
		for (int i = 0; i < NTAPFORM; i++) {
			if (TapFrm[i] && TapFrm[i]->Visible) {
				tallfl->put_wtag(L"ikkuna", 0);
				tallfl->put_wxml_s(L"laji", L"tapahtumat", 0);
				tallfl->put_wxml_d(L"numero", i+1, 0);
				TallennaIkkunaYhtV(tallfl, TapFrm[i]);
				tallfl->put_wxml_d(L"piste", TapFrm[i]->PisteVal->ItemIndex, 0);
				tallfl->put_wantitag(L"ikkuna", 0);
				}
			}
		if (FormEmit && FormEmit->Visible) {
			tallfl->put_wtag(L"ikkuna", 0);
			tallfl->put_wxml_s(L"laji", L"emitluenta", 0);
			TallennaIkkunaYhtV(tallfl, FormEmit);
			tallfl->put_wantitag(L"ikkuna", 0);
			}
		if (FormStatus && FormStatus->Visible) {
			tallfl->put_wtag(L"ikkuna", 0);
			tallfl->put_wxml_s(L"laji", L"status", 0);
			TallennaIkkunaYhtV(tallfl, FormStatus);
			tallfl->put_wantitag(L"ikkuna", 0);
			}
		tallfl->put_wantitag(L"ikkunat", 0);
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
	L"sarja",
	L"osuus"
	};

static taglkm = 11;

void __fastcall TFormMain::HaeIkkunatV(UnicodeString FlNm)
{
	TextFl *haefl;
	wchar_t line[200], cl, *p, st[100] = L"";
	int state = 0, antifl, tagno, val, laji, no = 0, valmis = 0;
	int lft = 0, tp = 0, wdth = 0, hgth = 0, pst = 0, srj = 0, os = 0;
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
														case 10:
															os = val;
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
							Frm = TilFrm[no-1];
							break;
						case 2:
							if (!TapFrm[no-1])
								TapFrm[no-1] = new TTapForm(FormMain);
							Frm = TapFrm[no-1];
							break;
						case 3:
							if (!FormEmit)
								FormEmit = new TFormEmit(FormMain);
							Frm = FormEmit;
							break;
						case 5:
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
							TilFrm[no-1]->SarjaVal->ItemIndex = srj;
							TilFrm[no-1]->OsVal->ItemIndex = os;
							TilFrm[no-1]->PisteVal->ItemIndex = pst;
							TilFrm[no-1]->IkkParam.Sarja = srj;
							TilFrm[no-1]->IkkParam.Osuus = os;
							TilFrm[no-1]->IkkParam.Va = pst;
							break;
						case 2:
							TapFrm[no-1]->PisteVal->ItemIndex = pst;
							break;
						}
					valmis = 0;
					}
			}
		delete haefl;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Tallennaikkunat1Click(TObject *Sender)
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
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Haeikkunat1Click(TObject *Sender)
{
	static init = 0;

	if (!init) {
		OpenDialog1->FileName = GetCurrentDir()+AnsiString("\\ikkunat.xml");
		init = 1;
	}
	OpenDialog1->DefaultExt = "xml";
	OpenDialog1->Filter = "Ikkunat (*.xml,*.ikk) |*.XML|*.IKK|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		HaeIkkunatV(OpenDialog1->FileName);
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::Laadituloste1Click(TObject *Sender)
{
	if (!TulosteForm)
		TulosteForm = new TTulosteForm(FormMain);
	TulosteForm->Show();
	if (TulosteForm->WindowState == wsMinimized)
		TulosteForm->WindowState = wsNormal;
	TulosteForm->BringToFront();
}

//---------------------------------------------------------------------------
/*
void lahetaSulkuSovellukselle(void) {
	SendMessage(FormMain->Handle,WM_MYLUKUMAARAT,99,0);
}
*/
//---------------------------------------------------------------------------

void ProsEmitThread(LPVOID)
{
	int bytecount0 = 0;

	while(lkmThreadOn && ProsEmitOn) {
		if (!sisaanluenta && !RastiVaTuloste) {
			if (FormEmit == NULL || (!FormEmit->CBLue->Checked && !FormEmit->CBViim->Checked)) {
				Sleep(1000);
				continue;
				}
	//		if (emithead2 >= emithead) {
			if ((bytecount - bytecount0 + bytecountmax) % bytecountmax > (bytecountmax/100)) {
				bytecount0 = bytecount;
				FormEmit->ProgressBar1->Position = 100*bytecount / bytecountmax;
				FormEmit->ProgressBar1->Refresh();
				}
			if (FormEmit->Tietue >= emithead-1) {
				Sleep(200);
				continue;
				}
			if (FormEmit != NULL && FormEmit->CBViim->Checked) {
				FormEmit->HaeUusiTietue();
				Sleep(100);
				}
			}
		else if (RastiVaTuloste) {
			if (FormVaTuloste == NULL) {
				Sleep(1000);
				continue;
				}
			if (FormVaTuloste) {
				FormVaTuloste->HaeUusiTietue();
				Sleep(100);
				}
			}
		else {
			if (FormLuenta == NULL) {
				Sleep(1000);
				continue;
				}
			if (FormLuenta) {
				if ((bytecount - bytecount0 + bytecountmax) % bytecountmax > (bytecountmax/100)) {
					bytecount0 = bytecount;
					FormLuenta->ProgressBar1->Position = 100*bytecount / bytecountmax;
					FormLuenta->ProgressBar1->Refresh();
					}
				FormLuenta->HaeUusiTietue();
				Sleep(100);
				}
			}
		}
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

void __fastcall TFormMain::Yhteenveto2Click(TObject *Sender)
{
	if (!Yv1)
		Yv1 = new TFormYhtveto(FormMain);
	Yv1->Show();
	if (Yv1->WindowState == wsMinimized)
		Yv1->WindowState = wsNormal;
	Yv1->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BtnTulPalvClick(TObject *Sender)
{
	int vast = mrNo;
	UnicodeString cfgFName = L"";

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

void __fastcall TFormMain::BtnIlmTilaClick(TObject *Sender)
{
	int vast = mrNo;
	UnicodeString cfgFName = L"";

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
			(*FormList[i].Form)->Show();
			if ((*FormList[i].Form)->WindowState == wsMinimized)
				(*FormList[i].Form)->WindowState = wsNormal;
			(*FormList[i].Form)->BringToFront();
			break;
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::lueInit(int *TTila, bool *avaa, UnicodeString initflnm, UnicodeString *cfgflnm)
{
	wchar_t line[200];

	if (initflnm.Length() == 0)
		initflnm = InitDir+L"\\Init.cfg";
	TextFl *InitFile = new TextFl(initflnm.c_str(), L"r");
	if (InitFile->IsOpen()) {
		InitFileName = initflnm;
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

void __fastcall TFormMain::SetFormKohde(TForm *Frm, wchar_t *Kohde)
{
	for (int i = 0; i < nForms; i++) {
		if (Frm == *FormList[i].Form) {
			wcsncpy(FormList[i].Kohde, Kohde, sizeof(FormList[i].Kohde)/2-1);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::Aloita(void)
{
	int k,cc;
	static int init;
	UnicodeString cfgflnm = L"", path = L"", initflnm = L"";
	wchar_t st[102];
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
		ToimintaTila = 0;
		if (_argc > 1) {
			wcsncpy(st, _wargv[1], sizeof(st)/2 - 1);
			if (st[4] == L'=') {
				st[4] = 0;
				upcasewstr(st);
				if (!wcscmp(st, L"INIT"))
					initflnm = st+5;
				}
			}
		lueInit(&ToimintaTila, &Avaa, initflnm, &cfgflnm);
		if (Avaa) {
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

void __fastcall TFormMain::iedot1Click(TObject *Sender)
{
	FormOhjtiedot = new TFormOhjtiedot(this);
	FormOhjtiedot->ShowModal();
	delete FormOhjtiedot;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Help2Click(TObject *Sender)
{
	Application->HelpKeyword(L"ViestiWin");
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::BitBtn1Click(TObject *Sender)
{
	Help2Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Nollaatietoja1Click(TObject *Sender)
{
	if (!FormNollaus)
		FormNollaus = new TFormNollaus(FormMain);
	FormNollaus->Show();
	if (FormNollaus->WindowState == wsMinimized)
		FormNollaus->WindowState = wsNormal;
	FormNollaus->BringToFront();
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

void __fastcall TFormMain::Sarjatiedot1Click(TObject *Sender)
{
	FormSarjatiedot->Show();
	if (FormSarjatiedot->WindowState == wsMinimized)
		FormSarjatiedot->WindowState = wsNormal;
	FormSarjatiedot->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Button2Click(TObject *Sender)
{
	FormKilpMaaritys = new TFormKilpMaaritys(FormMain);
	FormKilpMaaritys->ShowModal();
	delete FormKilpMaaritys;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Memo1DblClick(TObject *Sender)
{
	static bool init = false;
	FormMessages->Show();
	if (FormMessages->WindowState == wsMinimized)
		FormMessages->WindowState = wsNormal;
	FormMessages->BringToFront();
	if (!init) {
		init = true;
		FormMessages->Left = Screen->Width - FormMessages->Width;
		FormMessages->Top = Screen->Height - FormMessages->Height - 30 * Screen->PixelsPerInch / 96;
		}
}
//---------------------------------------------------------------------------

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

void __fastcall TFormMain::Haetiedostosta1Click(TObject *Sender)
{
	if (!FormImport)
		FormImport = new TFormImport(FormMain);
	FormImport->Show();
	if (FormImport->WindowState == wsMinimized)
		FormImport->WindowState = wsNormal;
	FormImport->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Joukkueet1Click(TObject *Sender)
{
	if (!FormJoukkueet)
		FormJoukkueet = new TFormJoukkueet(this);
	FormJoukkueet->Show();
	if (FormJoukkueet->WindowState == wsMinimized)
		FormJoukkueet->WindowState = wsNormal;
	FormJoukkueet->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Yksijoukkue1Click(TObject *Sender)
{
	if (!FormJoukkuetiedot)
		FormJoukkuetiedot = new TFormJoukkuetiedot(this);

	if (!kuuluttaja)
		FormJoukkuetiedot->sallimuokkausvalinta = true;
	FormJoukkuetiedot->Show();
	if (FormJoukkuetiedot->WindowState == wsMinimized)
		FormJoukkuetiedot->WindowState = wsNormal;
	FormJoukkuetiedot->BringToFront();
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

void SuljeKaikki(void)
{
	int wparam = 0;
	int lparam = 0;
	lkmThreadOn = false;
	if (FormJakelu)
		FormJakelu->CloseAll();
	talltapahtumat();
	SendMessage(FormMain->Handle,WM_MYMSGCLOSEAPPL,wparam,lparam);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Siirrlistietoja1Click(TObject *Sender)
{
	if (!FormLisatiedot)
		FormLisatiedot = new TFormLisatiedot(this);

	FormLisatiedot->Show();
	if (FormLisatiedot->WindowState == wsMinimized)
		FormLisatiedot->WindowState = wsNormal;
	FormLisatiedot->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Ratatiedot1Click(TObject *Sender)
{
	if (!FormRadat)
		FormRadat = new TFormRadat(this);

	FormRadat->Show();
	if (FormRadat->WindowState == wsMinimized)
		FormRadat->WindowState = wsNormal;
	FormRadat->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Avausmritykset1Click(TObject *Sender)
{
	if (!FormAvausMaar)
		FormAvausMaar = new TFormAvausMaar(FormMain);
	FormAvausMaar->Show();;
	if (FormAvausMaar->WindowState == wsMinimized)
		FormAvausMaar->WindowState = wsNormal;
	FormAvausMaar->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Emitluenta1Click(TObject *Sender)
{
	if (!FormEmit)
		FormEmit = new TFormEmit(this);

	FormEmit->Show();
	if (FormEmit->WindowState == wsMinimized)
		FormEmit->WindowState = wsNormal;
	FormEmit->BringToFront();
	sisaanluenta = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Yhteislhdt1Click(TObject *Sender)
{
	if (!FormYLahdot)
		FormYLahdot = new TFormYLahdot(this);

	FormYLahdot->Show();
	if (FormYLahdot->WindowState == wsMinimized)
		FormYLahdot->WindowState = wsNormal;
	FormYLahdot->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Merkitsetarkastuskoodeja1Click(TObject *Sender)
{
	if (!FormMerkKoodeja)
		FormMerkKoodeja = new TFormMerkKoodeja(this);

	FormMerkKoodeja->Show();
	if (FormMerkKoodeja->WindowState == wsMinimized)
		FormMerkKoodeja->WindowState = wsNormal;
	FormMerkKoodeja->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::iedonsiirtotiedostontarkastelu1Click(TObject *Sender)
{
   if (!FormComfile)
		FormComfile = new TFormComfile(this);
	FormComfile->Show();
	if (FormComfile->WindowState == wsMinimized)
		FormComfile->WindowState = wsNormal;
	FormComfile->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Asetuksetjauusinta1Click(TObject *Sender)
{
	if (!FormUusinta)
		FormUusinta = new TFormUusinta(this);
	FormUusinta->Show();
	if (FormUusinta->WindowState == wsMinimized)
		FormUusinta->WindowState = wsNormal;
	FormUusinta->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Numerointi1Click(TObject *Sender)
{
	if (!FormArvonta)
		FormArvonta = new TFormArvonta(this);
	FormArvonta->Show();
	if (FormArvonta->WindowState == wsMinimized)
		FormArvonta->WindowState = wsNormal;
	FormArvonta->BringToFront();
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::SisLuentaClick(TObject *Sender)
{
	if (!FormLuenta)
		FormLuenta = new TFormLuenta(this);
	FormLuenta->Show();
	if (FormLuenta->WindowState == wsMinimized)
		FormLuenta->WindowState = wsNormal;
	FormLuenta->BringToFront();
	if (sisaanluenta == 0)
		sisaanluenta = 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::AjanottoClick(int i)
{
   if (!FormAjanotot[i])
		FormAjanotot[i] = new TFormAjanotto(this);
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
void __fastcall TFormMain::Vliaikatulosteet1Click(TObject *Sender)
{
	if (!FormVaTuloste)
		FormVaTuloste = new TFormVaTuloste(this);
	FormVaTuloste->Show();
	if (FormVaTuloste->WindowState == wsMinimized)
		FormVaTuloste->WindowState = wsNormal;
	FormVaTuloste->BringToFront();
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

void __fastcall TFormMain::N1GAZ1Click(TObject *Sender)
{
	if (!FrmGAZ[0])
		FrmGAZ[0] = new TFormGAZ(this);
	FrmGAZ[0]->Show();
	if (FrmGAZ[0]->WindowState == wsMinimized)
		FrmGAZ[0]->WindowState = wsNormal;
	FrmGAZ[0]->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::N2GAZ1Click(TObject *Sender)
{
	if (!FrmGAZ[1])
		FrmGAZ[1] = new TFormGAZ(this);
	FrmGAZ[1]->Show();
	if (FrmGAZ[1]->WindowState == wsMinimized)
		FrmGAZ[1]->WindowState = wsNormal;
	FrmGAZ[1]->BringToFront();
}
//---------------------------------------------------------------------------

void closetaulu(void)
{
	for (int i = 0; i < 2; i++) {
		if (FrmGAZ[i])
			FrmGAZ[i]->closetaulu();
		}
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

void __fastcall TFormMain::Sakkoseuranta1Click(TObject *Sender)
{
	if (!FormSakkoKierr)
		FormSakkoKierr = new TFormSakkoKierr(this);

	FormSakkoKierr->Show();
	if (FormSakkoKierr->WindowState == wsMinimized)
		FormSakkoKierr->WindowState = wsNormal;
	FormSakkoKierr->BringToFront();
}

//---------------------------------------------------------------------------
void __fastcall TFormMain::MySQL1Click(TObject *Sender)
{
	if (!FormMySQL)
		FormMySQL = new TFormMySQL(this);
	FormMySQL->Show();
	if (FormMySQL->WindowState == wsMinimized)
		FormMySQL->WindowState = wsNormal;
	FormMySQL->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Kopioiosuustietoja1Click(TObject *Sender)
{
	if (!FormOsuuskopiointi) {
		FormOsuuskopiointi = new TFormOsuuskopiointi(this);
		}
	FormOsuuskopiointi->Show();
	if (FormOsuuskopiointi->WindowState == wsMinimized)
		FormOsuuskopiointi->WindowState = wsNormal;
	FormOsuuskopiointi->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::LhestyjienseurantaClick(TObject *Sender)
{
	int i = 0;

	if (Sender == Lhestyjt11)
		i = 0;
	else if (Sender == Lhestyjt21)
		i = 1;
	else if (Sender == Lhestyjt31)
		i = 2;
	else if (Sender == Lhestyjt41)
		i = 3;
	if (!FormLahestyjat[i]) {
		FormLahestyjat[i] = new TFormLahestyjat(this);
		}
	FormLahestyjat[i]->Show();
	if (FormLahestyjat[i]->WindowState == wsMinimized)
		FormLahestyjat[i]->WindowState = wsNormal;
	FormLahestyjat[i]->BringToFront();
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

