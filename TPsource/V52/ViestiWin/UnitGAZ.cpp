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
#include <io.h>
#include <fcntl.h>
#pragma hdrstop
#include <sys/stat.h>
#include <winsock2.h>

#include "UnitGAZ.h"
#include "TpLaitteet.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

//#define GAZTEST

TFormGAZ *FormGAZ;

extern TauluParamTp TauluParam[NREGNLY];

extern TFormGAZ *FrmGAZ[2];

extern HANDLE hComm[MAX_PORTTI];
wchar_t tauluflnm[41] = L"numtaulu.txt";
int jatktaulu;
#ifdef GAZTEST
FILE *taulufile;
#endif

//---------------------------------------------------------------------------
__fastcall TFormGAZ::TFormGAZ(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
	SGTaulut->RowCount = NREGNLY+1;
	SGTaulut->ColCount = 11;
	SGTaulut->Cells[0][0] = L"LaiteNo";
	SGTaulut->Cells[1][0] = L"Laite";
	SGTaulut->Cells[2][0] = L"Yhteys";
	SGTaulut->Cells[3][0] = L"Portti";
	SGTaulut->Cells[4][0] = L"VastaIPosoite";
	SGTaulut->Cells[5][0] = L"VastaPortti";
	SGTaulut->Cells[6][0] = L"LaiteID";
	SGTaulut->Cells[7][0] = L"KorttiID";
	SGTaulut->Cells[8][0] = L"Lev";
	SGTaulut->Cells[9][0] = L"Kork";
	SGTaulut->Cells[10][0] = L"Tila";
	SGTaulut->ColWidths[0] = 40 * Screen->PixelsPerInch / 96;
	SGTaulut->ColWidths[1] = 50 * Screen->PixelsPerInch / 96;
	SGTaulut->ColWidths[2] = 45 * Screen->PixelsPerInch / 96;
	SGTaulut->ColWidths[3] = 50 * Screen->PixelsPerInch / 96;
	SGTaulut->ColWidths[4] = 110 * Screen->PixelsPerInch / 96;
	SGTaulut->ColWidths[5] = 60 * Screen->PixelsPerInch / 96;
	SGTaulut->ColWidths[6] = 110 * Screen->PixelsPerInch / 96;
	SGTaulut->ColWidths[7] = 60 * Screen->PixelsPerInch / 96;
	SGTaulut->ColWidths[8] = 40 * Screen->PixelsPerInch / 96;
	SGTaulut->ColWidths[9] = 40 * Screen->PixelsPerInch / 96;
	SGTaulut->ColWidths[10] = 40 * Screen->PixelsPerInch / 96;
	PageControl1->ActivePageIndex = 0;
	t_com = taulu_com;
	t_baud = taulu_baud;
	Edt[0] = Edit1;
	Edt[1] = Edit2;
	Edt[2] = Edit3;
	Edt[3] = Edit4;
	Edt[4] = Edit5;
	Osuus[0] = Osuus1;
	Osuus[1] = Osuus2;
	Osuus[2] = Osuus3;
	Osuus[3] = Osuus4;
	Osuus[4] = Osuus5;
	Karki[0] = Karki1;
	Karki[1] = Karki2;
	Karki[2] = Karki3;
	Karki[3] = Karki4;
	Karki[4] = Karki5;
	CBak[0] = CheckBox1;
	CBak[1] = CheckBox2;
	CBak[2] = CheckBox3;
	CBak[3] = CheckBox4;
	CBak[4] = CheckBox5;
	CBauto[0] = CheckBox6;
	CBauto[1] = CheckBox7;
	CBauto[2] = CheckBox8;
	CBauto[3] = CheckBox9;
	CBauto[4] = CheckBox10;
	GAZ1[0][0] = Edit6;
	GAZ2[0][0] = Edit16;
	for (int c = 0; c < 6; c++) {
		for (int r = 0; r < 5; r++) {
			if ((c == 0 && r == 0))
				continue;
			GAZ1[r][c] = new TEdit(TabSheet1);
			GAZ1[r][c]->Parent = TabSheet1;
			GAZ1[r][c]->Left = GAZ1[0][0]->Left + c*(GAZ1[0][0]->Width-1);
			GAZ1[r][c]->Top = Osuus[r]->Top;
			GAZ1[r][c]->Width = GAZ1[0][0]->Width;
			GAZ1[r][c]->Text = L"";
			GAZ1[r][c]->ReadOnly = true;
			GAZ1[r][c]->TabStop = false;
			GAZ2[r][c] = new TEdit(TabSheet1);
			GAZ2[r][c]->Parent = TabSheet1;
			GAZ2[r][c]->Left = GAZ2[0][0]->Left + c*(GAZ2[0][0]->Width-1);
			GAZ2[r][c]->Top = Osuus[r]->Top;
			GAZ2[r][c]->Width = GAZ2[0][0]->Width;
			GAZ2[r][c]->Text = L"";
			GAZ2[r][c]->ReadOnly = true;
			GAZ2[r][c]->TabStop = false;
			}
		}
	EdtPort->Text = t_com;
	EdCPOs[0][0] = EdCPOs1;
	CBCPTieto[0][0] = CBCPTieto1;
	EdCPNaytto[0][0] = EdCPNaytto1;
	EdTeksti[0][0] = EdTeksti1;
	EdLaite[0] = EdLaite1;
	BtnAsKello[0] = BtnAsKello1;
	EdRiveja[0] = EdRiveja1;
	EdSar[0] = EdSar1;
	EdKirk[0] = EdKirk1;
	UpDown[0] = UpDown1;
	RBOn[0] = RBOn1;
	RBTyhja[0] = RBTyhja1;
	RBOletus[0] = RBOletus1;
	PlCP[0] = PlCP1;
	LblT[0] = LblT1;
	NLed = 0;
	ledNRivi[0] = 1;
	CBCPTieto[0][0]->ItemIndex = 3;
	for (int i = 0; i < NREGNLY; i++) {
		cpow_w[i] = 128;
		cpow_h[i] = 32;
		fntsz[i] = 4;
		}
#ifdef GAZTEST
	if (taulufile == NULL)
		taulufile = fopen("taulusan.txt", "wb");
#endif
}
//---------------------------------------------------------------------------

void TFormGAZ::PaivitaYhteydet(void)
{
	int y = 0;
	bool onLaite = false;

	for (int r = 1; r < SGTaulut->RowCount-1; r++) {
		for (int c = 0; c < SGTaulut->ColCount-1; c++) {
			SGTaulut->Cells[c][r] = L"";
			}
		}
	if (cn_gaz) {
		SGTaulut->Cells[0][y+1] = 0;
		SGTaulut->Cells[1][y+1] = L"GAZ";
		SGTaulut->Cells[2][y+1] = L"COM";
		SGTaulut->Cells[3][y+1] = taulu_com;
		SGTaulut->Cells[10][y+1] = (GAZavoin ? L"On" : L"Ei");
		y++;
		}
	for (int ny = 0; ny < NREGNLY; ny++) {
		if (IntInList(regnly[ny], L"23") >= 0) {
			wchar_t st[30];
			SGTaulut->Cells[0][y+1] = ny+1;
			onLaite = true;
			switch (regnly[ny]) {
				case LID_CPOWER:
					SGTaulut->Cells[1][y+1] = L"C-Power";
					swprintf(st, L"%03d.%03d.%03d.%03d",
						TauluParam[ny].DevID & 0xff,
						(TauluParam[ny].DevID & 0xff00) / 0x100,
						(TauluParam[ny].DevID & 0xff0000) / 0x10000,
						TauluParam[ny].DevID >> 24);
					SGTaulut->Cells[6][y+1] = st;
					SGTaulut->Cells[7][y+1] = TauluParam[ny].CardID;
					SGTaulut->Cells[8][y+1] = TauluParam[ny].Leveys;
					SGTaulut->Cells[9][y+1] = TauluParam[ny].Korkeus;
					break;
				}
			switch (comtype[cn_regnly[ny]]) {
				case comtpRS :
					SGTaulut->Cells[2][y+1] = L"COM";
					SGTaulut->Cells[3][y+1] = port_regnly[ny];
					break;
				case comtpTCP :
					SGTaulut->Cells[2][y+1] = L"TCP";
					SGTaulut->Cells[4][y+1] = ipparam[cn_regnly[ny]].destaddr;
					SGTaulut->Cells[5][y+1] = ipparam[cn_regnly[ny]].destport;
					break;
				case comtpUDPSTREAM :
					SGTaulut->Cells[2][y+1] = L"UDP";
					SGTaulut->Cells[4][y+1] = ipparam[cn_regnly[ny]].destaddr;
					SGTaulut->Cells[5][y+1] = ipparam[cn_regnly[ny]].destport;
					break;
				}
			SGTaulut->Cells[10][y+1] = (regnly_open[ny] ? L"On" : L"Ei");
			y++;
			}
		}
}
//---------------------------------------------------------------------------

void TFormGAZ::opentaulu(void)
   {
   int os, er = 1;

   if (GAZavoin)
	   return;
   for (os = 0; os < kilpparam.osuusluku; os++) {
	  tkarki[os] = TMAALI0;
	  }
   t_baud = 5;
   cn_gaz = GAZCOM;
   htaulu = -1;
   if (openport_x(cn_gaz, t_com, t_baud, 'n', 8, 1, 0)) {
	  writeerror("GAZ-tiedonsiirron avaaminen ei onnistunut", 0);
	  t_com = 0;
	  taulu_com = 0;
	  return;
	  }
   GAZavoin = 1;
   }

int TFormGAZ::taulu_tied(char *text1[], int nrv)
   {
   int i;
   char st[100];

   if (htaulu < 0) {
	  htaulu = _wopen(tauluflnm, O_WRONLY | O_CREAT | O_TRUNC | O_TEXT, S_IREAD | S_IWRITE);
	  }
   if (htaulu < 0) {
	  return(1);
	  }
   for (i = 0; i < nrv; i++) {
	  _write(htaulu, text1[i], strlen(text1[i]));
	  }
   if (!jatktaulu) {
	  _close(htaulu);
	  htaulu = -1;
	  }
   return(0);
   }

int TFormGAZ::taulu_lah(char *text1[], int nrv)
   {
   int i, n = 0;
   static char vertrivi[TAULU_NRIVI][TAULU_RIVPIT+3];
   static int count[TAULU_NRIVI];

   for (i = 0; i < nrv; i++) {
	  if (strcmp(text1[i], vertrivi[i])) {
		 count[i] = 1;
		 n++;
		 }
	  else
		 count[i] = (count[i]+1) % 50;
	  }
   if (n == 0)
	  return(0);
   if (t_com > 64) {
	  if (!taulu_tied(text1, nrv)) {
		 for (i = 0; i < nrv; i++)
			strcpy(vertrivi[i], text1[i]);
		 }
	  return(0);
	  }
   outquesize_x(cn_gaz, &n);
   if (n < tauluqsize - nrv*30) {
	  for (i = 0; i < nrv; i++) {
		 if (count[i] != 1)
			 continue;
		 if (hidasta) {
			 for (char *p = text1[i]; *p; p++) {
				wrt_st_x(cn_gaz, 1, p, &n);
				Sleep(10);
				}
			 }
		 else
			 wrt_st_x(cn_gaz, strlen(text1[i]), text1[i], &n);
		 strcpy(vertrivi[i], text1[i]);
		 count[i] = 1;
		 }
	  }
   return(0);
   }

void TFormGAZ::paiv_taulu(void)
   {
   int k, os;
   int d, nt, rno;
   char st[20] = "" , *rivi[TAULU_NRIVI], r1[TAULU_RIVPIT+3],r2[TAULU_RIVPIT+3];
   char r3[TAULU_RIVPIT+3],r4[TAULU_RIVPIT+3],r5[TAULU_RIVPIT+3];
   char txt[TAULU_RIVPIT+1];
   long t, tls;
   int p = -1, lj = -1;
   kilptietue kilp;
   static char edst[LED_LKM][LED_NRIVI][20];

   for (int taulu = 0; taulu < NLed; taulu++) {
	   if (CPavoin[taulu]) {
			for (int rivi = 0; rivi < ledNRivi[taulu]; rivi++) {
				int os;
				if (CBCPTieto[taulu][rivi] == NULL)
					continue;
				os = _wtoi(EdCPOs[taulu][rivi]->Text.c_str());
				memset(st, ' ', 19);
				switch (CBCPTieto[taulu][rivi]->ItemIndex) {
					case 0:
						wcstoansi(st, EdCPOs[taulu][rivi]->Text.c_str(), 19);
						break;
					case 1:
						if (os > 0 && os <= Sarjat[taulusrj].ntosuus) {
							sprintf(st, "%d.", os);
							st[strlen(st)] = ' ';
							sprintf(st + 3, "%4d", ntulos[taulusrj][os-1][0]);
							}
						break;
					case 2:
						if (os > 0 && os <= Sarjat[taulusrj].ntosuus) {
							tls = KSEK*T_TIME(biostime(0,0));
							tkarki[os-1] = TMAALI0;
							if (ntulos[taulusrj][os-1][0]) {
								d = jalku[taulusrj][os-1][0];
								if (d > 0) {
									GETREC(&kilp, d);
									tkarki[os-1] = kilp.Maali(os-1, 0);
									}
								}
							if (tkarki[os-1] != TMAALI0) {
								t = NORMTULOS(tls - tkarki[os-1]);
								aikatostr_s(st, t/SEK, 0);
								st[2] = ':';
								st[5] = ':';
								}
							}
						break;
					case 3:
						tls = KSEK*T_TIME(biostime(0,0));
						t = wstrtoaika_vap(EdTeksti[taulu][rivi]->Text.c_str(), t0);
						t = NORMTULOS(tls - t);
						aikatostr_s(st, t/SEK, 0);
						st[2] = ':';
						st[5] = ':';
						break;
					case 4:
						wcstoansi(st, EdTeksti[taulu][rivi]->Text.c_str(), 19);
						break;
					case 5:
						t = T_TIME(biostime(0,0));
						aikatostr_s(st, t/10, t0);
						st[2] = ':';
						st[5] = ':';
						break;
					}
				if (strcmp(st, edst[taulu][rivi])) {
					lahriviCPower(CPrno[taulu], rivi, st);
					strcpy(edst[taulu][rivi], st);
					EdCPNaytto[taulu][rivi]->Text = st;
					}
				}
		   }
	   }
   if (!GAZavoin)
	  return;
   if (nosuus < 1 || tosuus[0] < 1 || tosuus[0] > Sarjat[taulusrj].osuusluku)
	  return;
   rivi[0] = r1;
   rivi[1] = r2;
   rivi[2] = r3;
   rivi[3] = r4;
   rivi[4] = r5;
   for (k = 0; k < TAULU_NRIVI; k++) {
	  memset(rivi[k], ' ', TAULU_RIVPIT);
	  memcpy(rivi[k]+TAULU_RIVPIT, "\r", 2);
	  rivi[k][0] = 'A'+k;
	  }
   for (k = 0; k < nosuus; k++) {
	  os = tosuus[k] - 1;
	  if (os < 0 || os >= Sarjat[taulusrj].osuusluku)
		 continue;
	  nt = ntulos[taulusrj][os][0];
	  if (!lkmaikana[k]) {
		 tls = KSEK*T_TIME(biostime(0,0));
		 if (autokarki[os]) {
			tkarki[os] = TMAALI0;
			if (nt) {
			   d = jalku[taulusrj][os][0];
			   if (d > 0) {
				  GETREC(&kilp, d);
				  tkarki[os] = kilp.Maali(os, 0) + kviive[os];
				  }
			   }
			}
		 if (tkarki[os] != TMAALI0) {
			t = NORMTULOS(tls - tkarki[os]);
//            if (t < 8L*36000L)
			   {
			   AIKATOSTRS(st, t,0);
			   st[8] = 0;
			   st[2] = st[5] = ':';
//#ifdef GAZ2
			   if (st[0] == '0') {
				  st[0] = ' ';
				  if (st[1] == '0') {
					 st[1] = ' ';
					 if (st[3] == '0') {
						st[3] = ' ';
						if (st[4] == '0') {
						   st[4] = ' ';
						   if (st[6] == '0') {
							  st[6] = ' ';
							  }
						   }
						}
					 }
				  }
//#endif
			   memcpy(rivi[k]+9, st, 8);
			   }
			}
		 rivi[k][22] = os + '1';
		 putintright(nt%1000, rivi[k] + 3);
#ifdef GAZ2
		 if (nt >= 1000) {
			rivi[k][8] = nt/1000 + '0';
			}
#endif
		 }
	  else {
		 rivi[k][16] = '0' + nt%10;
		 if (nt >= 10)
			rivi[k][15] = '0' + ((nt/10)%10);
		 if (nt >= 100)
			rivi[k][13] = '0' + ((nt/100)%10);
		 if (nt >= 1000)
			rivi[k][12] = '0' + ((nt/1000)%10);
		 rivi[k][9] = os + '1';
		 rivi[k][11] = rivi[k][14] = ':';
		 }
	  memcpy(txt, rivi[k], TAULU_RIVPIT);
		GAZ1[k][0]->Text = txt[9];
		GAZ1[k][1]->Text = txt[10];
		GAZ1[k][2]->Text = txt[12];
		GAZ1[k][3]->Text = txt[13];
		GAZ1[k][4]->Text = txt[15];
		GAZ1[k][5]->Text = txt[16];
		GAZ2[k][0]->Text = txt[1];
		GAZ2[k][1]->Text = txt[2];
		GAZ2[k][2]->Text = txt[3];
		GAZ2[k][3]->Text = txt[20];
		GAZ2[k][4]->Text = txt[21];
		GAZ2[k][5]->Text = txt[22];
	  txt[TAULU_RIVPIT] = 0;
	  Edt[k]->Text = txt;
	  }
   taulu_lah(rivi, nosuus);
   }
//---------------------------------------------------------------------------

void TFormGAZ::closetaulu(void)
   {
   if (htaulu >= 0)
	  _close(htaulu);
   else
	  closeport_x(cn_gaz);
   }
//---------------------------------------------------------------------------

void TFormGAZ::nayta_valinnat(void)
{
	nosuus = 0;
	for	(int i = 0; i < TAULU_NRIVI; i++) {
	   Osuus[i]->Text = tosuus[i];
	   if (tosuus[i] < 1)
		   continue;
	   nosuus++;
	   CBak[i]->Checked = lkmaikana[i];
	   CBauto[i]->Checked = autokarki[tosuus[i]-1];
	   if (autokarki[tosuus[i]-1]) {
		   Karki[i]->Text = kviive[tosuus[i]-1]/SEK;
		   }
	   else {
		   wchar_t st[20];
		   Karki[i]->Text = aikatowstr_cols_n(st, tkarki[tosuus[i]-1], t0, 0, 8);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::Button1Click(TObject *Sender)
{
	for	(int i = 0; i < TAULU_NRIVI; i++) {
	   tosuus[i] = _wtoi(Osuus[i]->Text.c_str());
	   if (tosuus[i] < 1)
		   continue;
	   lkmaikana[i] = CBak[i]->Checked;
	   autokarki[tosuus[i]-1] = CBauto[i]->Checked;
	   if (CBauto[i]->Checked) {
		   kviive[tosuus[i]-1] = SEK*_wtoi(Karki[i]->Text.c_str());
		   }
	   else {
		   tkarki[tosuus[i]-1] = wstrtoaika_vap(Karki[i]->Text.c_str(), t0);
		   }
	   }
	opentaulu();
	nayta_valinnat();
	in_taulu = 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::FormShow(TObject *Sender)
{
	nayta_valinnat();
	PaivitaYhteydet();
	if (cn_gaz)
		PageControl1->ActivePageIndex = 0;
	else
		PageControl1->ActivePageIndex = 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::CheckBox11Click(TObject *Sender)
{
	hidasta = CheckBox11->Checked;
}
//---------------------------------------------------------------------------

void TFormGAZ::PaivLEDsivu(void)
{
	int it = 0, nt = 0, yt, tabord;

	for (int rno = 0; rno < NREGNLY; rno++) {
		if (IntInList(regnly[rno], L"23") < 0 || !regnly_open[rno])
//		if (rno > 4)
			continue;
		for (it = 0; it < NLed; it++)
			if (rno == CPrno[it])
				break;
		if (it >= NLed) {
			for (it = 0; it < NLed; it++)
				if (regnly[CPrno[it]] == 0 ||
					(IntInList(regnly[CPrno[it]], L"23") >= 0 && !regnly_open[CPrno[it]]))
					break;
			}
		if (it >= LED_LKM)
			continue;

		/* Testi
		it = NLed;
		// itseT */

		if (it >= NLed)
			NLed = it + 1;

		CPrno[it] = rno;

		/* Testi
		regnly[rno] = 13;
		// itseT */


		if (ledNRivi[it] == 0) {
			/* Testi
			if (it == 1)
				ledNRivi[it] = 2;
			else
			// itseT */
				ledNRivi[it] = 1;
			}

		if (EdLaite[it] == NULL) {
			EdLaite[it] = new TEdit(TabSheet2);
			EdLaite[it]->Parent = TabSheet2;
			EdLaite[it]->Left = EdLaite[0]->Left;
			EdLaite[it]->Height = EdLaite[0]->Height;
			EdLaite[it]->Width = EdLaite[0]->Width;
			EdLaite[it]->Alignment = EdLaite[0]->Alignment;
			EdLaite[it]->Top = EdLaite[it-1]->Top + EdLaite[it-1]->Height + 4;
			EdLaite[it]->Text = rno + 1;
			}
		if (EdRiveja[it] == NULL) {
			EdRiveja[it] = new TEdit(TabSheet2);
			EdRiveja[it]->Parent = TabSheet2;
			EdRiveja[it]->Left = EdRiveja[0]->Left;
			EdRiveja[it]->Height = EdRiveja[0]->Height;
			EdRiveja[it]->Width = EdRiveja[0]->Width;
			EdRiveja[it]->Alignment = EdRiveja[0]->Alignment;
			EdRiveja[it]->Top = EdLaite[it]->Top;
			EdRiveja[it]->Text = ledNRivi[it];
			EdRiveja[it]->OnExit = EdRiveja[0]->OnExit;
			}
		if (EdSar[it] == NULL) {
			EdSar[it] = new TEdit(TabSheet2);
			EdSar[it]->Parent = TabSheet2;
			EdSar[it]->Left = EdSar[0]->Left;
			EdSar[it]->Height = EdSar[0]->Height;
			EdSar[it]->Width = EdSar[0]->Width;
			EdSar[it]->Alignment = EdSar[0]->Alignment;
			EdSar[it]->Top = EdLaite[it]->Top;
			EdSar[it]->Text = ledNRivi[it];
			EdSar[it]->ReadOnly = EdSar[0]->ReadOnly;
			}
		if (UpDown[it] == NULL) {
			UpDown[it] = new TUpDown(TabSheet2);
			UpDown[it]->Parent = TabSheet2;
			UpDown[it]->Left = UpDown[0]->Left;
			UpDown[it]->Height = UpDown[0]->Height;
			UpDown[it]->Width = UpDown[0]->Width;
			UpDown[it]->Top = UpDown[0]->Top + EdLaite[it]->Top - EdLaite[0]->Top;
			UpDown[it]->OnClick = UpDown[0]->OnClick;
			UpDown[it]->Min = UpDown[0]->Min;
			UpDown[it]->Max = UpDown[0]->Max;
			UpDown[it]->Position = UpDown[0]->Position;
			}
		if (EdKirk[it] == NULL) {
			EdKirk[it] = new TEdit(TabSheet2);
			EdKirk[it]->Parent = TabSheet2;
			EdKirk[it]->Left = EdKirk[0]->Left;
			EdKirk[it]->Height = EdKirk[0]->Height;
			EdKirk[it]->Width = EdKirk[0]->Width;
			EdKirk[it]->Alignment = EdKirk[0]->Alignment;
			EdKirk[it]->Top = EdLaite[it]->Top;
			EdKirk[it]->OnExit = EdKirk[0]->OnExit;
			EdKirk[it]->Text = EdKirk[0]->Text;
			EdKirk[it]->ReadOnly = EdKirk[0]->ReadOnly;
			}
		if (BtnAsKello[it] == NULL) {
			BtnAsKello[it] = new TButton(TabSheet2);
			BtnAsKello[it]->Parent = TabSheet2;
			BtnAsKello[it]->Left = BtnAsKello[0]->Left;
			BtnAsKello[it]->Height = BtnAsKello[0]->Height;
			BtnAsKello[it]->Width = BtnAsKello[0]->Width;
			BtnAsKello[it]->Caption = BtnAsKello[0]->Caption;
			BtnAsKello[it]->OnClick = BtnAsKello[0]->OnClick;
			BtnAsKello[it]->Hint = BtnAsKello[0]->Hint;
			BtnAsKello[it]->ShowHint = true;
			BtnAsKello[it]->Top = BtnAsKello[0]->Top + EdLaite[it]->Top - EdLaite[0]->Top;
			}
		if (PlCP[it] == NULL) {
			PlCP[it] = new TPanel(TabSheet2);
			PlCP[it]->Parent = TabSheet2;
			PlCP[it]->Left = PlCP[0]->Left;
			PlCP[it]->Height = PlCP[0]->Height;
			PlCP[it]->Width = PlCP[0]->Width;
			PlCP[it]->ShowCaption = False;
			PlCP[it]->Top = PlCP[0]->Top + EdLaite[it]->Top - EdLaite[0]->Top;
			}
		if (RBOn[it] == NULL) {
			RBOn[it] = new TRadioButton(PlCP[it]);
			RBOn[it]->Parent = PlCP[it];
			RBOn[it]->Left = RBOn[0]->Left;
			RBOn[it]->Height = RBOn[0]->Height;
			RBOn[it]->Width = RBOn[0]->Width;
			RBOn[it]->Top = RBOn[0]->Top;
			RBOn[it]->Caption = RBOn[0]->Caption;
			RBOn[it]->OnClick = RBOn[0]->OnClick;
			RBOn[it]->Checked = false;
			}
		if (RBTyhja[it] == NULL) {
			RBTyhja[it] = new TRadioButton(PlCP[it]);
			RBTyhja[it]->Parent = PlCP[it];
			RBTyhja[it]->Left = RBTyhja[0]->Left;
			RBTyhja[it]->Height = RBTyhja[0]->Height;
			RBTyhja[it]->Width = RBTyhja[0]->Width;
			RBTyhja[it]->Top = RBTyhja[0]->Top;
			RBTyhja[it]->Caption = RBTyhja[0]->Caption;
			RBTyhja[it]->OnClick = RBTyhja[0]->OnClick;
			RBTyhja[it]->Checked = false;
			}
		if (RBOletus[it] == NULL) {
			RBOletus[it] = new TRadioButton(PlCP[it]);
			RBOletus[it]->Parent = PlCP[it];
			RBOletus[it]->Left = RBOletus[0]->Left;
			RBOletus[it]->Height = RBOletus[0]->Height;
			RBOletus[it]->Width = RBOletus[0]->Width;
			RBOletus[it]->Top = RBOletus[0]->Top;
			RBOletus[it]->Caption = RBOletus[0]->Caption;
			RBOletus[it]->OnClick = RBOletus[0]->OnClick;
			RBOletus[it]->Checked = true;
			}
		}
	yt = EdLaite[0]->Top;
	tabord = 0;
	for (int it = 0; it < LED_LKM; it++) {
		if (it >= NLed) {
			if (LblT[it])
				LblT[it]->Visible = false;
			if (EdLaite[it] != NULL)
				EdLaite[it]->Visible = false;
			if (EdRiveja[it] != NULL)
				EdRiveja[it]->Visible = false;
			if (EdSar[it] != NULL)
				EdSar[it]->Visible = false;
			if (UpDown[it] != NULL)
				UpDown[it]->Visible = false;
			if (EdKirk[it] != NULL)
				EdKirk[it]->Visible = false;
			if (BtnAsKello[it] != NULL)
				BtnAsKello[it]->Visible = false;
			if (PlCP[it] != NULL)
				PlCP[it]->Visible = false;
			if (RBOn[it] != NULL)
				RBOn[it]->Visible = false;
			if (RBOletus[it] != NULL)
				RBOletus[it]->Visible = false;
			if (RBTyhja[it] != NULL)
				RBTyhja[it]->Visible = false;
			for (int ir = 0; ir < LED_NRIVI; ir++) {
				if (CBCPTieto[it][ir] != NULL)
					CBCPTieto[it][ir]->Visible = false;
				if (EdCPOs[it][ir] != NULL)
					EdCPOs[it][ir]->Visible = false;
				if (EdTeksti[it][ir] != NULL)
					EdTeksti[it][ir]->Visible = false;
				if (EdCPNaytto[it][ir] != NULL)
					EdCPNaytto[it][ir]->Visible = false;
				}
			continue;
			}
		if (LblT[it] == NULL) {
			LblT[it] = new TLabel(TabSheet2);
			LblT[it]->Parent = TabSheet2;
			LblT[it]->Caption = UnicodeString(it+1);
			LblT[it]->Height = LblT[0]->Height;
			LblT[it]->Width = LblT[0]->Height;
			LblT[it]->Left = LblT[0]->Left;
			LblT[it]->Top = yt + LblT[0]->Top - EdLaite[0]->Top;
			}
		LblT[it]->Visible = true;
		EdLaite[it]->Visible = true;
		EdRiveja[it]->Visible = true;
		EdSar[it]->Visible = true;
		UpDown[it]->Visible = true;
		EdKirk[it]->Visible = true;
		BtnAsKello[it]->Visible = true;
		PlCP[it]->Visible = true;
		RBOn[it]->Visible = true;
		RBOletus[it]->Visible = true;
		RBTyhja[it]->Visible = true;
		EdLaite[it]->TabStop = false;
		EdLaite[it]->ReadOnly = true;
		EdRiveja[it]->TabOrder = tabord++;
		EdSar[it]->TabStop = false;
//		EdSar[it]->TabOrder = tabord++;
		EdLaite[it]->Top = yt;
		LblT[it]->Top = LblT[0]->Top + yt - EdLaite[0]->Top;
		EdRiveja[it]->Top = EdRiveja[0]->Top + yt - EdLaite[0]->Top;
		EdSar[it]->Top = EdSar[0]->Top + yt - EdLaite[0]->Top;
		LblT[it]->Top = LblT[0]->Top + yt - EdLaite[0]->Top;
		UpDown[it]->Top = UpDown[0]->Top + yt - EdLaite[0]->Top;
		EdKirk[it]->Top = EdLaite[it]->Top;
		BtnAsKello[it]->Top = BtnAsKello[0]->Top + yt - EdLaite[0]->Top;
		PlCP[it]->Top = PlCP[0]->Top + yt - EdLaite[0]->Top;
		RBOn[it]->Top = RBOn[0]->Top;
		RBTyhja[it]->Top = RBTyhja[0]->Top;
		RBOletus[it]->Top = RBOletus[0]->Top;
		if ((nt = _wtoi(EdRiveja[it]->Text.c_str()) * _wtoi(EdSar[it]->Text.c_str())) > 0 && nt <= LED_NRIVI)
			ledNRivi[it] = nt;
		else if (ledNRivi[it] < 1 || ledNRivi[it] >= LED_NRIVI)
			ledNRivi[it] = 1;
		for (int ir = 0; ir < LED_NRIVI; ir++) {
			if (ir >= ledNRivi[it]) {
				if (CBCPTieto[it][ir] != NULL)
					CBCPTieto[it][ir]->Visible = false;
				if (EdCPOs[it][ir] != NULL)
					EdCPOs[it][ir]->Visible = false;
				if (EdTeksti[it][ir] != NULL)
					EdTeksti[it][ir]->Visible = false;
				if (EdCPNaytto[it][ir] != NULL)
					EdCPNaytto[it][ir]->Visible = false;
				continue;
				}
			if (CBCPTieto[it][ir] == NULL) {
				CBCPTieto[it][ir] = new TComboBox(TabSheet2);
				CBCPTieto[it][ir]->Parent = TabSheet2;
				CBCPTieto[it][ir]->Left = CBCPTieto[0][0]->Left;
				CBCPTieto[it][ir]->Height = CBCPTieto[0][0]->Height;
				CBCPTieto[it][ir]->Width = CBCPTieto[0][0]->Width;
				CBCPTieto[it][ir]->Items->Add(L"Vain os.");
				CBCPTieto[it][ir]->Items->Add(L"Sija");
				CBCPTieto[it][ir]->Items->Add(L"Ero kärkeen");
				CBCPTieto[it][ir]->Items->Add(L"Ero txt-ajasta");
				CBCPTieto[it][ir]->Items->Add(L"Teksti");
				CBCPTieto[it][ir]->Items->Add(L"Kello");
				CBCPTieto[it][ir]->ItemIndex = 4;
				}
			if (EdCPOs[it][ir] == NULL) {
				EdCPOs[it][ir] = new TEdit(TabSheet2);
				EdCPOs[it][ir]->Parent = TabSheet2;
				EdCPOs[it][ir]->Left = EdCPOs[0][0]->Left;
				EdCPOs[it][ir]->Height = EdCPOs[0][0]->Height;
				EdCPOs[it][ir]->Width = EdCPOs[0][0]->Width;
				EdCPOs[it][ir]->Alignment = EdCPOs[0][0]->Alignment;
				EdCPOs[it][ir]->Text = 1;
				}
			if (EdTeksti[it][ir] == NULL) {
				EdTeksti[it][ir] = new TEdit(TabSheet2);
				EdTeksti[it][ir]->Parent = TabSheet2;
				EdTeksti[it][ir]->Left = EdTeksti[0][0]->Left;
				EdTeksti[it][ir]->Height = EdTeksti[0][0]->Height;
				EdTeksti[it][ir]->Width = EdTeksti[0][0]->Width;
				EdTeksti[it][ir]->Alignment = EdTeksti[0][0]->Alignment;
				EdTeksti[it][ir]->Text = L"";
				}
			if (EdCPNaytto[it][ir] == NULL) {
				EdCPNaytto[it][ir] = new TEdit(TabSheet2);
				EdCPNaytto[it][ir]->Parent = TabSheet2;
				EdCPNaytto[it][ir]->Left = EdCPNaytto[0][0]->Left;
				EdCPNaytto[it][ir]->Height = EdCPNaytto[0][0]->Height;
				EdCPNaytto[it][ir]->Width = EdCPNaytto[0][0]->Width;
				EdCPNaytto[it][ir]->Alignment =  EdCPNaytto[0][0]->Alignment;
				EdCPNaytto[it][ir]->Font =  EdCPNaytto[0][0]->Font;
				EdCPNaytto[it][ir]->Text = L"";
				}
			CBCPTieto[it][ir]->Visible = true;
			EdCPOs[it][ir]->Visible = true;
			EdTeksti[it][ir]->Visible = true;
			EdCPNaytto[it][ir]->Visible = true;
			CBCPTieto[it][ir]->Top = CBCPTieto[0][0]->Top + yt - EdLaite[0]->Top;
			EdCPOs[it][ir]->Top = EdCPOs[0][0]->Top + yt - EdLaite[0]->Top;
			EdTeksti[it][ir]->Top = EdTeksti[0][0]->Top + yt - EdLaite[0]->Top;
			EdCPNaytto[it][ir]->Top = EdCPNaytto[0][0]->Top + yt - EdLaite[0]->Top;
			CBCPTieto[it][ir]->TabOrder = tabord++;
			EdCPOs[it][ir]->TabOrder = tabord++;
			EdTeksti[it][ir]->TabOrder = tabord++;
			EdCPNaytto[it][ir]->TabStop = false;
			EdCPNaytto[it][ir]->ReadOnly = true;
			if (ir == 0) {
				UpDown[it]->TabOrder = tabord++;
				EdKirk[it]->TabOrder = tabord++;
				BtnAsKello[it]->TabOrder = tabord++;
				PlCP[it]->TabOrder = tabord++;
            }
			yt += EdLaite[0]->Height + 3 * Screen->PixelsPerInch / 96;
			}
		}
}

//---------------------------------------------------------------------------
static int lahCPpuskuri(int cn, char *buf, int len)
{
	int nsent;

	if (comtype[cn] > 0)
		wrt_st_x(cn, len, buf, &nsent);
	else {
		buf[7] = 0xa5;
		buf[31+len] = 0xae;
		wrt_st_x(cn, len-6, buf+7, &nsent);
		}
	return(nsent);
}

//---------------------------------------------------------------------------
int TFormGAZ::lahriviCPower(int rno, int rivi, char *teksti)
{
	UCHAR buf[400];
	UINT16 chksum = 0;
	UINT16 sij = 0, len;
	UCHAR tas = 0;
	int nsent;
	static int inlahrivi[NREGNLY];

	for (int i = 0; i < 1000 && inlahrivi[rno]; i++)
		Sleep(10);
	if (inlahrivi[rno])
		return(0);
	inlahrivi[rno] = 1;
	len = strlen(teksti);
	memset(buf, 0, sizeof(buf));
	*(UINT32 *)buf = TauluParam[rno].DevID;
	*(UINT16 *)(buf + 4) = 23+len;
	buf[8] = 0x68;
	buf[9] = 0x32;
	buf[10] = TauluParam[rno].CardID;
	buf[11] = 0x7b;
	buf[12] = 0x01;
	*(UINT16 *)(buf + 13) = 12 + len;
	buf[15] = 0x00;
	buf[16] = 0x00;
	buf[17] = 0x12;      // Send pure text
	buf[18] = (UCHAR) rivi;
	buf[19] = 0x00;
	buf[20] = tas;
	buf[21] = 0x00;
	buf[22] = 0xff;
	buf[23] = 0xff;
	buf[24] = fntsz[rno];     // Font size
	buf[25] = 0xff;
	buf[26] = 0x00;
	buf[27] = 0x00;
	strcpy(buf+28, teksti);
	for (int i = 8; i < 29+len; i++)
		chksum += buf[i];
	*(UINT16 *)(buf + len + 29) = chksum;
	lahCPpuskuri(cn_regnly[rno], buf, len+31);
#ifdef GAZTEST
	if (taulufile)
		fwrite(buf, 48, 1, taulufile);
#endif
	Sleep(10);
	inlahrivi[rno] = 0;
	return(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::TyhjTaulu(int taulu)
{
	lahriviCPower(CPrno[taulu], 0, "  ");
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::TauluAkt(int taulu, bool on)
{
	CPavoin[taulu] = on ? 1 : 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::TauluJako(int taulu, int nrv, int nsar)
{
	UCHAR buf[100];
	UINT16 WX[8], WY[8], WH[8], WW[8], chksum = 0;
	int nikk;

	nikk = nrv * nsar;
	ledNRivi[taulu] = nikk;
	memset(buf, 0, sizeof(buf));
	*(UINT32 *)buf =  TauluParam[CPrno[taulu]].DevID;
	*(UINT16 *)(buf + 4) = 13 + 8*nikk;
	buf[8] = 0x68;         //  Packet type
	buf[9] = 0x32;         //  Card type
	buf[10] = TauluParam[CPrno[taulu]].CardID;
	buf[11] = 0x7b;        //  Protocol code
	buf[12] = 0x01;
	*(UINT16 *)(buf + 13) = 2 + 8*nikk;    // Packed data len
	buf[15] = 0x00;        // Packet number
	buf[16] = 0x00;        // Last packet number
	buf[17] = 0x01;        // CC: Division of display
	buf[18] = nikk;
	cpow_w[CPrno[taulu]] = TauluParam[CPrno[taulu]].Leveys;
	cpow_h[CPrno[taulu]] = TauluParam[CPrno[taulu]].Korkeus / nikk;
	switch (cpow_h[CPrno[taulu]]) {
		case 32:
			fntsz[CPrno[taulu]] = 4;
			break;
		case 16:
			fntsz[CPrno[taulu]] = 2;
			break;
		case 8:
			fntsz[CPrno[taulu]] = 1;
			break;
		case 64:
			fntsz[CPrno[taulu]] = 6;
			break;
	   default:
			fntsz[CPrno[taulu]] = 2;
		}
	for (int i = 0; i < nikk ; i++) {
		WX[i] = (TauluParam[CPrno[taulu]].Leveys / nsar) * (i % nsar);
		WY[i] = (TauluParam[CPrno[taulu]].Korkeus / nrv) * (i / nsar);
		WW[i] = TauluParam[CPrno[taulu]].Leveys / nsar;
		WH[i] = TauluParam[CPrno[taulu]].Korkeus / nrv;
		buf[19+8*i] = WX[i] / 256;
		buf[20+8*i] = WX[i] % 256;
		buf[21+8*i] = WY[i] / 256;
		buf[22+8*i] = WY[i] % 256;
		buf[23+8*i] = WW[i] / 256;
		buf[24+8*i] = WW[i] % 256;
		buf[25+8*i] = WH[i] / 256;
		buf[26+8*i] = WH[i] % 256;
		}
	for (int i = 8; i < 19+8*nikk; i++)
		chksum += buf[i];
	*(UINT16 *)(buf + 19 + 8*nikk) = chksum;
	lahCPpuskuri(cn_regnly[CPrno[taulu]], buf, 21 + 8*nikk);
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::TauluKirkkaus(int taulu, int val)
{
	UCHAR buf[60];
	UINT16 chksum = 0;
	int nsent;

	memset(buf, 0, sizeof(buf));
	*(UINT32 *)buf =  TauluParam[CPrno[taulu]].DevID;
	*(UINT16 *)(buf + 4) = 13;
	buf[8] = 0x68;
	buf[9] = 0x32;
	buf[10] = TauluParam[CPrno[taulu]].CardID;
	buf[11] = 0x45;
	buf[12] = 0x01;
	buf[13] = 0x00;
	buf[14] = 0xff;
	buf[15] = 0xff;
	buf[16] = 0xff;
	buf[17] = 0xff;
	buf[18] = 0x00;
	buf[19] = 0x00;
	for (int i = 8; i < 20; i++)
		chksum += buf[i];
	*(UINT16 *)(buf + 20) = chksum;
	if (comtype[cn_regnly[CPrno[taulu]]] > 0)
		wrt_st_x(cn_regnly[CPrno[taulu]], 22, buf, &nsent);
	else {
		buf[7] = 0xa5;
		buf[22] = 0xae;
		wrt_st_x(cn_regnly[CPrno[taulu]], 16, buf+7, &nsent);
		}
	Sleep(200);
	memset(buf, 0, sizeof(buf));
	*(UINT32 *)buf =  TauluParam[CPrno[taulu]].DevID;
	*(UINT16 *)(buf + 4) = 33;
	buf[8] = 0x68;
	buf[9] = 0x32;
	buf[10] = 0x01;
	buf[11] = 0x46;
	buf[12] = 0x01;
	buf[13] = 0x00;
	for (int i = 14; i < 38; i++)
		buf[i] = val;
	for (int i = 8; i < 38; i++)
		chksum += buf[i];
	*(UINT16 *)(buf + 38) = chksum;
	lahCPpuskuri(cn_regnly[CPrno[taulu]], buf, 40);
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::Naytaoletus(int taulu)
{
	UCHAR buf[30];
	UINT16 chksum = 0;
	int nsent;

	memset(buf, 0, sizeof(buf));
	*(UINT32 *)buf =  TauluParam[CPrno[taulu]].DevID;
	*(UINT16 *)(buf + 4) = 12;
	buf[8] = 0x68;
	buf[9] = 0x32;
	buf[10] = TauluParam[CPrno[taulu]].CardID;
	buf[11] = 0x7b;
	buf[12] = 0x01;
	*(UINT16 *)(buf + 13) = 1;
	buf[15] = 0x00;
	buf[16] = 0x00;
	buf[17] = 0x06;
	for (int i = 8; i < 18; i++)
		chksum += buf[i];
	*(UINT16 *)(buf + 18) = chksum;
	lahCPpuskuri(cn_regnly[CPrno[taulu]], buf, 20);
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::BtnAsKelloClick(TObject *Sender)
{
	UCHAR buf[1000], s, m, t, vp, pv, kk, v, vv;
	UINT16 cpow_w = 96, cpow_h = 16, chksum = 0;
	int nsent, taulu;
	struct tm *ttm;
	time_t tt;

	for (taulu = 0; taulu < NLed; taulu++) {
		if (Sender == BtnAsKello[taulu])
			break;
		}
	if (taulu >= NLed)
		return;
	time(&tt);
	ttm = localtime(&tt);
	s = ttm->tm_sec;
	m = ttm->tm_min;
	t = ttm->tm_hour;
	vp = ttm->tm_wday;
	pv = ttm->tm_mday;
	kk = ttm->tm_mon + 1;
	v = ttm->tm_year % 100;
	vv = 0;
	if (EdTeksti[taulu][0]->Text.Length() > 4) {
		int t1;
		t1 = wstrtoaika_vap(EdTeksti[taulu][0]->Text.c_str(), 0) / 1000;
		if (t1 >= 0) {
			t = (UCHAR) (t1 / 3600);
			t1 -= 3600 * t;
			m = (UCHAR) (t1 / 60);
			s = (UCHAR) (t1 % 60);
			}
		}
	memset(buf, 0, sizeof(buf));
	*(UINT32 *)buf =  TauluParam[CPrno[taulu]].DevID;
	*(UINT16 *)(buf + 4) = 13;
	buf[8] = 0x68;
	buf[9] = 0x32;
	buf[10] = TauluParam[CPrno[taulu]].CardID;
	buf[11] = 0x47;
	buf[12] = TauluParam[CPrno[taulu]].CardID;
	buf[13] = 0x00;
	buf[14] = s;
	buf[15] = m;
	buf[16] = t;
	buf[17] = vp;
	buf[18] = pv;
	buf[19] = kk;
	buf[20] = v;
	buf[21] = vv;
	for (int i = 8; i < 22; i++)
		chksum += buf[i];
	*(UINT16 *)(buf + 22) = chksum;
	lahCPpuskuri(cn_regnly[CPrno[taulu]], buf, 24);
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::SGTaulutMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	if (Button == mbRight && SGTaulut->Row > 0) {
		switch (SGTaulut->Col) {
			case 1:
				PopupMenuLaite->Popup(X+SGTaulut->ClientOrigin.X, Y+SGTaulut->ClientOrigin.Y);
				break;
			case 2:
				PopupMenuLaiteYht->Popup(X+SGTaulut->ClientOrigin.X, Y+SGTaulut->ClientOrigin.Y);
				break;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::LaiteYhtClick(TObject *Sender)
{
	if (Sender == LaiteSarjaportti)
		SGTaulut->Cells[SGTaulut->Col][SGTaulut->Row] = L"COM";
	else if (Sender == LaiteTCPasiakas)
		SGTaulut->Cells[SGTaulut->Col][SGTaulut->Row] = L"TCP";
	else if (Sender == LaiteUDP)
		SGTaulut->Cells[SGTaulut->Col][SGTaulut->Row] = L"UDP";
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::LaiteClick(TObject *Sender)
{
	if (Sender == miGAZ)
		SGTaulut->Cells[SGTaulut->Col][SGTaulut->Row] = L"GAZ";
	else if (Sender == CPower) {
		SGTaulut->Cells[SGTaulut->Col][SGTaulut->Row] = L"C-Power";
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::BtnAvaaYhtClick(TObject *Sender)
{
	int y = SGTaulut->Row - 1;
	int ny;
	bool xmlfl = false;

	if (wmemcmpU(SGTaulut->Cells[1][y+1].c_str(), L"GAZ", 3) == 0) {
		taulu_com = _wtoi(SGTaulut->Cells[3][y+1].c_str());
		opentaulu();
		PaivitaYhteydet();
		return;
		}
	if (SGTaulut->Cells[0][y+1].Length() == 0 || (ny = SGTaulut->Cells[0][y+1].ToInt() - 1) < 0 || ny >= NREGNLY) {
		Application->MessageBoxW(L"Rivillä ei hyväksyttävää yhteyden numeroa", L"Virhe", MB_OK);
		return;
		}
	if (regnly_open[ny]) {
		Application->MessageBoxW((UnicodeString(L"Laiteyhteys ") + UnicodeString(ny+1) + L" avattuna käyttöön").c_str(),
			L"Este", MB_OK);
		return;
		}
	if (wmemcmpU(SGTaulut->Cells[1][y+1].c_str(), L"GAZ", 3) == 0) {
		}
	else if (wmemcmpU(SGTaulut->Cells[1][y+1].c_str(), L"C-POWER", 3) == 0) {
		UINT32 i1 = 0, i2 = 0, i3 = 0, i4 = 0;
		regnly[ny] = LID_CPOWER;
		swscanf(SGTaulut->Cells[6][y+1].c_str(), L"%d.%d.%d.%d", &i1, &i2, &i3, &i4);
		TauluParam[ny].DevID = i4 + 256 * (i3 + 256 * (i2 + 256 *i1));
		TauluParam[ny].CardID = SGTaulut->Cells[7][y+1].ToInt();
		TauluParam[ny].Leveys = SGTaulut->Cells[8][y+1].ToInt();
		TauluParam[ny].Korkeus = SGTaulut->Cells[9][y+1].ToInt();
		}
	else {
		Application->MessageBoxW((UnicodeString(L"Laitetyyppi ") + SGTaulut->Cells[1][y+1] + L" ei käytettävissä").c_str(),
			L"Este", MB_OK);
		return;
		}
	if (wmemcmpU(SGTaulut->Cells[2][y+1].c_str(), L"COM", 3) == 0) {
		comtype[cn_regnly[ny]] = comtpRS;
		port_regnly[ny] = _wtoi(SGTaulut->Cells[3][y+1].c_str());
		}
	else if (wmemcmpU(SGTaulut->Cells[2][y+1].c_str(), L"TCP", 3) == 0) {
		comtype[cn_regnly[ny]] = comtpTCP;
		ipparam[cn_regnly[ny]].iptype = ipTCPCLIENT;
		wcsncpy(ipparam[cn_regnly[ny]].destaddr, SGTaulut->Cells[4][y+1].Trim().c_str(), 63);
		ipparam[cn_regnly[ny]].destport = (USHORT) _wtoi(SGTaulut->Cells[5][y+1].c_str());
		}
	else if (wmemcmpU(SGTaulut->Cells[2][y+1].c_str(), L"UDP", 3) == 0) {
		comtype[cn_regnly[ny]] = comtpUDPSTREAM;
		ipparam[cn_regnly[ny]].iptype = ipUDPSTREAM;
		wcsncpy(ipparam[cn_regnly[ny]].destaddr, SGTaulut->Cells[4][y+1].Trim().c_str(), 63);
		ipparam[cn_regnly[ny]].destport = (USHORT) _wtoi(SGTaulut->Cells[5][y+1].c_str());
		}
	else {
		Application->MessageBoxW((UnicodeString(L"Yhteyslaji ") + SGTaulut->Cells[2][y+1] + L" ei käytettävissä").c_str(),
			L"Este", MB_OK);
		return;
		}
	inittimer();
	Sleep(500);
	PaivitaYhteydet();
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::BtnSuljeYhtClick(TObject *Sender)
{
	int ny;

	if (wmemcmpU(SGTaulut->Cells[1][SGTaulut->Row].c_str(), L"GAZ", 3) == 0) {
		closetaulu();
		PaivitaYhteydet();
		return;
		}
	if (SGTaulut->Cells[0][SGTaulut->Row].Length() == 0 ||
		(ny = SGTaulut->Cells[0][SGTaulut->Row].ToInt() - 1) < 0 || ny >= NREGNLY) {
		Application->MessageBoxW(L"Rivillä ei hyväksyttävää yhteyden numeroa", L"Virhe", MB_OK);
		return;
		}
	if (!regnly_open[ny]) {
		Application->MessageBoxW((UnicodeString(L"Laiteyhteys ") + UnicodeString(ny+1) + L" ei ole avoinna").c_str(),
			L"Este", MB_OK);
		return;
		}
	if (IntInList(regnly[ny], L"23") < 0) {
		Application->MessageBoxW((UnicodeString(L"Yhteyden ") + UnicodeString(ny+1) + L" laite ei ole GAZ eikä LED-taulu").c_str(),
			L"Este", MB_OK);
		return;
		}
	katkaisuPyydetty = true;
	rem_regnly(ny);
	katkaisuPyydetty = false;
	PaivitaYhteydet();
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::RBTilaClick(TObject *Sender)
{
	for (int taulu = 0; taulu < NLed; taulu++) {
		int laite = EdLaite[taulu]->Text.ToInt() - 1;
		if (laite < 0 || laite >= NREGNLY || IntInList(regnly[laite], L"23") < 0 || !regnly_open[laite])
			continue;
		if (Sender == RBOletus[taulu]) {
			TauluAkt(taulu, false);
			Naytaoletus(taulu);
			return;
			}
		if (Sender == RBTyhja[taulu]) {
			TauluAkt(taulu, false);
			if (ledNRivi[taulu] != 1) {
				TauluJako(taulu, 1, 1);
				}
			TyhjTaulu(taulu);
			return;
			}
		if (Sender == RBOn[taulu]) {
			int n, m;
			TauluAkt(taulu, false);
			n = _wtoi(EdRiveja[taulu]->Text.c_str());
			m = _wtoi(EdSar[taulu]->Text.c_str());
			if (n * m > 0 && n * m <= LED_NRIVI && RBOn[taulu]->Checked) {
				TauluJako(taulu, n, m);
				PaivLEDsivu();
				}
			TauluAkt(taulu, true);
			return;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::TabSheet2Show(TObject *Sender)
{
	PaivLEDsivu();
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::UpDownClick(TObject *Sender, TUDBtnType Button)
{
	for	(int it = 0; it < NLed; it++) {
		if (Sender == UpDown[it]) {
			int kirk = UpDown[it]->Position - 2;
			if (kirk < -1)
				kirk = -1;
			if (kirk > 31)
				kirk = 31;
			EdKirk[it]->Text = kirk;
			TauluKirkkaus(it, kirk);
			break;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::EdKirkExit(TObject *Sender)
{
/*
	for	(int it = 0; it < NLed; it++) {
		if (Sender == EdKirk[it]) {
			int val = _wtoi(EdKirk[it]->Text.c_str());
			if (val < -1)
				val = -1;
			else if (val > 31)
				val	= 31;
			EdKirk[it]->Text = val;
			UpDown[it]->Position = val;
			TauluKirkkaus(it, val);
			}
		}
*/
}
//---------------------------------------------------------------------------



void __fastcall TFormGAZ::EdRiveja1Exit(TObject *Sender)
{
	for	(int it = 0; it < NLed; it++) {
		if (Sender == EdRiveja[it]) {
			int n, m;
			n = _wtoi(EdRiveja[it]->Text.c_str());
			if (JaaLEDtaulumyssarakkeisiin1->Checked && n == 2)
				EdSar[it]->Text = L"2";
			else
				EdSar[it]->Text = L"1";
			m = _wtoi(EdSar[it]->Text.c_str());
			if (n * m > 0 && n * m <= LED_NRIVI && RBOn[it]->Checked) {
				bool av = CPavoin[it];
				CPavoin[it] = false;
				TauluJako(it, n, m);
				PaivLEDsivu();
				CPavoin[it] = av;
				}
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormGAZ::JaaLEDtaulumyssarakkeisiin1Click(TObject *Sender)
{
	JaaLEDtaulumyssarakkeisiin1->Checked = !JaaLEDtaulumyssarakkeisiin1->Checked;
}
//---------------------------------------------------------------------------

