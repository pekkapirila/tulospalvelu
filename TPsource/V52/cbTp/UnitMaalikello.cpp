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

#include "UnitMaalikello.h"
#include "TpLaitteet.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMaalikello *FormMaalikello;

void rem_regnly(INT r_no);
INT start_regnly(INT r_no);
extern comparamtp comparam[];
static int laiteLkm = 2;

//---------------------------------------------------------------------------
__fastcall TFormMaalikello::TFormMaalikello(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
#ifdef MAXOSUUSLUKU
	GBLaite2->Visible = false;
	laiteLkm = 1;
#endif
	CBLaite[0] = CBLaite1;
	CBLaiteYht[0] = CBLaiteYht1;
	EdLaitePort[0] = EdLaitePort1;
	EdLaiteIP[0] = EdLaiteIP1;
	EdAikaEro[0] = EdAikaEro1;
	CBLaite[1] = CBLaite2;
	CBLaiteYht[1] = CBLaiteYht2;
	EdLaitePort[1] = EdLaitePort2;
	EdLaiteIP[1] = EdLaiteIP2;
	EdAikaEro[1] = EdAikaEro2;
	LblYhteys[0] = LblYhteys1;
	LblYhteys[1] = LblYhteys2;
	BtnLaiteAvaa[0] = BtnLaite1Avaa;
	BtnLaiteAvaa[1] = BtnLaite2Avaa;
	EC[0][0] = ECC1;
	EC[0][1] = ECX1;
	EC[0][2] = ECK1;
	EC[0][3] = EC01;
	EC[0][4] = EC11;
	EC[0][5] = EC21;
	EC[0][6] = EC31;
	EC[0][7] = EC41;
	EC[0][8] = EC51;
	EC[0][9] = EC61;
	EC[0][10] = EC71;
	EC[0][11] = EC81;
	EC[1][0] = ECC2;
	EC[1][1] = ECX2;
	EC[1][2] = ECK2;
	EC[1][3] = EC02;
	EC[1][4] = EC12;
	EC[1][5] = EC22;
	EC[1][6] = EC32;
	EC[1][7] = EC42;
	EC[1][8] = EC52;
	EC[1][9] = EC62;
	EC[1][10] = EC72;
	EC[1][11] = EC82;
	EJ[0][0] = EJC1;
	EJ[0][1] = EJX1;
	EJ[0][2] = EJK1;
	EJ[0][3] = EJ01;
	EJ[0][4] = EJ11;
	EJ[0][5] = EJ21;
	EJ[0][6] = EJ31;
	EJ[0][7] = EJ41;
	EJ[0][8] = EJ51;
	EJ[0][9] = EJ61;
	EJ[0][10] = EJ71;
	EJ[0][11] = EJ81;
	EJ[1][0] = EJC2;
	EJ[1][1] = EJX2;
	EJ[1][2] = EJK2;
	EJ[1][3] = EJ02;
	EJ[1][4] = EJ12;
	EJ[1][5] = EJ22;
	EJ[1][6] = EJ32;
	EJ[1][7] = EJ42;
	EJ[1][8] = EJ52;
	EJ[1][9] = EJ62;
	EJ[1][10] = EJ72;
	EJ[1][11] = EJ82;
	Panel[0] = Panel1;
	Panel[1] = Panel2;
	for (int i = 0; i < 2; i++) {
		CBLaite[i]->ItemIndex = 0;
		CBLaiteYht[i]->ItemIndex = 0;
		for (int j = 0; j < 12; j++) {
			ECExit(EC[i][j]);
			EJExit(EJ[i][j]);
			}
		}
	PageControl1->ActivePageIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormMaalikello::FormResize(TObject *Sender)
{
	PageControl1->Top = 0;
	PageControl1->Left = 0;
	PageControl1->Height = ClientHeight;
	PageControl1->Width = ClientWidth;
}
//---------------------------------------------------------------------------
void __fastcall TFormMaalikello::CBLaite1Change(TObject *Sender)
{
	CBLaiteYht1->Enabled = CBLaite1->ItemIndex != 0;
	EdLaitePort1->Enabled = CBLaite1->ItemIndex != 0;
	EdLaiteIP1->Visible = (CBLaite1->ItemIndex != 0 && CBLaiteYht1->ItemIndex != 0);
	LbLaiteIP1->Visible = EdLaiteIP1->Visible;
	CBLaiteBaud1->Visible = (CBLaite1->ItemIndex != 0 && CBLaiteYht1->ItemIndex == 0);
	LbLaite1Baud->Visible = CBLaiteBaud1->Visible;
	EdAikaEro1->Enabled = CBLaite1->ItemIndex != 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormMaalikello::CBLaiteYht1Change(TObject *Sender)
{
	EdLaiteIP1->Visible = (CBLaite1->ItemIndex != 0 && CBLaiteYht1->ItemIndex != 0);
	LbLaiteIP1->Visible = EdLaiteIP1->Visible;
	CBLaiteBaud1->Visible = (CBLaite1->ItemIndex != 0 && CBLaiteYht1->ItemIndex == 0);
	LbLaite1Baud->Visible = CBLaiteBaud1->Visible;
}
//---------------------------------------------------------------------------
void __fastcall TFormMaalikello::CBLaite2Change(TObject *Sender)
{
	CBLaiteYht2->Enabled = (CBLaite2->Enabled && CBLaite2->ItemIndex != 0);
	EdLaitePort2->Enabled = (CBLaite2->Enabled && CBLaite2->ItemIndex != 0);
	EdLaiteIP2->Visible = (CBLaite2->Enabled && CBLaite2->ItemIndex != 0 &&
		CBLaiteYht2->ItemIndex != 0);
	LbLaiteIP2->Visible = EdLaiteIP2->Visible;
	EdAikaEro2->Enabled = (CBLaite2->Enabled && CBLaite2->ItemIndex != 0);
}
//---------------------------------------------------------------------------
void __fastcall TFormMaalikello::CBLaiteYht2Change(TObject *Sender)
{
	EdLaiteIP2->Visible = (CBLaite2->Enabled && CBLaite2->ItemIndex != 0 &&
		CBLaiteYht2->ItemIndex != 0);
	LbLaiteIP2->Visible = EdLaiteIP2->Visible;
}
//---------------------------------------------------------------------------
void __fastcall TFormMaalikello::FormShow(TObject *Sender)
{
	int lj, rno = 0;
	wchar_t as[20];

	GBS4->Enabled = false;
	GBRTR2->Enabled = false;
	for (int laite = 0; laite < laiteLkm; laite++) {
		LblYhteys[laite]->Caption = L"Yhteys suljettu";
		if (laite == 1) {
			for (rno = 1; rno < NREGNLY; rno++)
				if (cn_regnly[rno] >= NREGNLY && (lj = IntInList(regnly[rno], L"6")) >= 0)
					break;
			if (rno == NREGNLY)
				break;
			EdLaite2No->Text = rno + 1;
			}
		if (cn_regnly[rno] >= NREGNLY && (lj = IntInList(regnly[rno], L"6 4 5 3 1 2")) >= 0) {
			CBLaite[laite]->ItemIndex = lj + 1;
			if (comtype[cn_regnly[rno]] == comtpRS) {
				CBLaiteYht[laite]->ItemIndex = 0;
				EdLaitePort[laite]->Text = portti[cn_regnly[rno]];
				if (laite == 0)
					CBLaiteBaud1->ItemIndex = comparam[cn_regnly[rno]].baud - 3;
				}
			else if (comtype[cn_regnly[rno]] == comtpTCPSRV) {
				CBLaiteYht[laite]->ItemIndex = 2;
				EdLaitePort[laite]->Text = ipparam[cn_regnly[rno]].srvport;
				}
			else if (comtype[cn_regnly[rno]] & comtpTCP) {
				CBLaiteYht[laite]->ItemIndex = 1;
				EdLaitePort[laite]->Text = ipparam[cn_regnly[rno]].destport;
				EdLaiteIP[laite]->Text = ipparam[cn_regnly[rno]].destaddr;
				}
			else if (comtype[cn_regnly[rno]] & comtpUDP) {
				CBLaiteYht[laite]->ItemIndex = 3;
				EdLaitePort[laite]->Text = ipparam[cn_regnly[rno]].destport;
				EdLaiteIP[laite]->Text = ipparam[cn_regnly[rno]].destaddr;
				}
			EdAikaEro[laite]->Text = aikatowstr_cols_n(as, aikaero[rno], 0, 0, 8);
			for (int i = 0; i < 12; i++) {
				if (aika_tunnus[rno][i] >= -2 && aika_tunnus[rno][i] <= 8)
					EC[laite][i]->Text = ch_piste[aika_tunnus[rno][i]+2];
				else
					EC[laite][i]->Text = aika_tunnus[rno][i];
				EJ[laite][i]->Text = aika_jono[rno][i] + 1;
				ECExit(EC[laite][i]);
				EJExit(EJ[laite][i]);
				}
			if (laite == 0)
				CBLaite1Change(Sender);
			else {
				CBLaite2->Enabled = _wtoi(EdLaite2No->Text.c_str()) > 1;
				CBLaite2Change(Sender);
				}
			if (regnly_open[rno]) {
				if (rno == 0 && regnly[0] == LID_ALGE4)
					GBS4->Enabled = true;
				if (rno == 0 && regnly[0] == LID_RTR2)
					GBRTR2->Enabled = true;
				Panel[laite]->Enabled = false;
				BtnLaiteAvaa[laite]->Enabled = false;
				LblYhteys[laite]->Caption = L"Yhteys on";
				}
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormMaalikello::BtnLaite1SuljeClick(TObject *Sender)
{
	if (regnly_open[0]) {
		katkaisuPyydetty = 1;
		comclose[cn_regnly[0]] = 1;
		rem_regnly(0);
		Sleep(500);
		katkaisuPyydetty = 0;
		}
	GBS4->Enabled = false;
	GBRTR2->Enabled = false;
	Panel1->Enabled = true;
	BtnLaiteAvaa[0]->Enabled = true;
	LblYhteys1->Caption = L"Yhteys suljettu";
}
//---------------------------------------------------------------------------
void __fastcall TFormMaalikello::BtnLaite2SuljeClick(TObject *Sender)
{
	int rno = _wtoi(EdLaite2No->Text.c_str()) - 1;
	if (rno > 0 && rno <= NREGNLY && regnly_open[rno]) {
		katkaisuPyydetty = 1;
		comclose[cn_regnly[rno]] = 1;
		rem_regnly(rno);
		Sleep(500);
		katkaisuPyydetty = 0;
		}
	Panel2->Enabled = true;
	BtnLaiteAvaa[1]->Enabled = true;
	LblYhteys2->Caption = L"Yhteys suljettu";
}
//---------------------------------------------------------------------------
void __fastcall TFormMaalikello::BtnLaite1AvaaClick(TObject *Sender)
{
	LaiteAvaa(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormMaalikello::BtnLaite2AvaaClick(TObject *Sender)
{
	int laite = _wtoi(EdLaite2No->Text.c_str());

	if (laite < 1)
		return;
	if (laite == 1) {
		Application->MessageBoxW(L"Määrittele laite 1 vasemmalla", L"Este", MB_OK);
		return;
		}
	if (regnly_open[laite-1] != 0) {
		Application->MessageBoxW(L"Laitenumero varattu toiselle laitteelle", L"Este", MB_OK);
		return;
		}
	LaiteAvaa(1);
}
//---------------------------------------------------------------------------
void __fastcall TFormMaalikello::LaiteAvaa(int laite)
{
	int cn, rno = 0;
	if (laite == 1)
		rno = _wtoi(EdLaite2No->Text.c_str()) - 1;
	if (regnly_open[rno]) {
		Application->MessageBoxW(L"Laiteyhteys jo avattu", L"Este", MB_OK);
		return;
		}
	cn = MAX_LAHPORTTI + rno;
	switch (CBLaite[laite]->ItemIndex) {
		case 0:
			return;
		case 1:
			regnly[rno] = LID_TIMY;
			break;
		case 2:
			regnly[rno] = LID_COMET;
			break;
		case 3:
			regnly[rno] = LID_ALGE4;
			break;
		case 4:
			regnly[rno] = LID_ALGE;
			break;
		case 5:
			regnly[rno] = LID_RTR;
			break;
		case 6:
			regnly[rno] = LID_RTR2;
			break;
		}
	if (laite == 0 && CBLaiteYht1->ItemIndex == 0) {
		if (CBLaiteBaud1->ItemIndex == 0)
			kello_baud = 0;
		else
			kello_baud = CBLaiteBaud1->ItemIndex + 3;
		}
	switch (CBLaiteYht[laite]->ItemIndex) {
		case 0:
			comtype[cn] = comtpRS;
			port_regnly[rno] = _wtoi(EdLaitePort[laite]->Text.c_str());
			break;
		case 1:
			comtype[cn] = comtpTCP;
			ipparam[cn].destport = _wtoi(EdLaitePort[laite]->Text.c_str());
			wcscpy(ipparam[cn].destaddr, EdLaiteIP[laite]->Text.c_str());
			break;
		case 2:
			comtype[cn] = comtpTCPSRV;
			ipparam[cn].srvport = _wtoi(EdLaitePort[laite]->Text.c_str());
			break;
		case 3:
			comtype[cn] = comtpUDP;
			ipparam[cn].destport = _wtoi(EdLaitePort[laite]->Text.c_str());
			wcscpy(ipparam[cn].destaddr, EdLaiteIP[laite]->Text.c_str());
			break;
		}
	aikaero[rno] = (wstrtoaika_vap(EdAikaEro[laite]->Text.c_str(), 0) + 12 * TUNTI) % (24 * TUNTI) - 12 * TUNTI;
	comclose[cn_regnly[rno]] = 0;
	start_regnly(rno);
	Sleep(1000);
	for (int i = 0; i < 12; i++) {
		int cc;
		if (EC[laite][i]->Text.Length() == 1 &&
			(cc = wcswcind(EC[laite][i]->Text.c_str()[0], ch_piste)) >= 0)
			aika_tunnus[rno][i] = cc - 2;
		aika_jono[rno][i] = _wtoi(EJ[laite][i]->Text.c_str()) - 1;
		}
	if (regnly_open[rno]) {
		LblYhteys[laite]->Caption = L"Yhteys on";
		if (rno == 0 && regnly[0] == LID_ALGE4)
			GBS4->Enabled = true;
		if (rno == 0 && regnly[0] == LID_RTR2)
			GBRTR2->Enabled = true;
		Panel[laite]->Enabled = false;
		BtnLaiteAvaa[laite]->Enabled = false;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMaalikello::RGUusintaClick(TObject *Sender)
{
	EdAlku->Enabled = RGUusinta->ItemIndex == 1;
	EdLoppu->Enabled = RGUusinta->ItemIndex == 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormMaalikello::BtnS4UudelleenClick(TObject *Sender)
{
	int n;
	char san[20] = "";

	if (RGUusinta->ItemIndex == 0)
		strcpy(san, "\x85\r");
	else {
		int ens = _wtoi(EdAlku->Text.c_str());
		int viim = _wtoi(EdLoppu->Text.c_str());
		if (viim >= ens)
			sprintf(san, "\x84%04d%04d\r", ens, viim);
		}
	if (san[0])
		wrt_st_x(cn_regnly[0], strlen(san), san, &n);
}
//---------------------------------------------------------------------------

void __fastcall TFormMaalikello::BtnViiveClick(TObject *Sender)
{
	int n, viive;
	char san[20] = "";

	viive = _wtoi(EdViive->Text.c_str());
	sprintf(san, "\x82%02d\r\x83%02d\r", viive, viive);
	wrt_st_x(cn_regnly[0], strlen(san), san, &n);
}
//---------------------------------------------------------------------------

void __fastcall TFormMaalikello::BtnTyhjennaClick(TObject *Sender)
{
	int n;
	if (Application->MessageBoxW(L"Vahvista kaikkien aikojen poisto kellon muistista",
		L"Vahvista", MB_YESNO) == IDYES) {
		wrt_st_x(cn_regnly[0], 2, "\x95\r", &n);
        }
}
//---------------------------------------------------------------------------

void __fastcall TFormMaalikello::Button1Click(TObject *Sender)
{
	int i, n, lkm = 9999, ens, viim;
	char san[20];

	if (RGRTR2Uusinta->ItemIndex == 1)
		lkm = _wtoi(EdLukum->Text.c_str());
	wrt_st_x(cn_regnly[0], 2, "+\r", &n);
	Sleep(500);
	sprintf(san, "!-%04d\r", lkm);
	i_flush_x(cn_regnly[0]);
	wrt_st_x(cn_regnly[0], strlen(san), san, &n);
	ens = 2;
	for (i = 0; i < 40; i++) {
		Sleep(200);
		read_st_x(cn_regnly[0], ens, san, &viim, &n);
		bytecount = (bytecount + viim) % bytecountmax;
		if (viim >= ens) break;
		ens -= viim;
		}
	if (i >= 40)
		Application->MessageBoxW(L"RTR-kello ei vahvistanut pyynnön hyväksymistä", L"", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TFormMaalikello::RGRTR2UusintaClick(TObject *Sender)
{
	EdLukum->Enabled = RGRTR2Uusinta->ItemIndex == 1;
}
//---------------------------------------------------------------------------


void __fastcall TFormMaalikello::EJExit(TObject *Sender)
{
	for (int i = 0; i < laiteLkm; i++) {
		for (int j = 0; j < 12; j++) {
			if (Sender == EJ[i][j]) {
				int k = _wtoi(EJ[i][j]->Text.c_str());
				if (k < 1)
					k = 1;
				if (k > maxjono + 1)
					k = maxjono + 1;
				EJ[i][j]->Text = k;
				}
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMaalikello::ECExit(TObject *Sender)
{
	for (int i = 0; i < laiteLkm; i++) {
		for (int j = 0; j < 12; j++) {
			if (Sender == EC[i][j]) {
				int k = _wtoi(EC[i][j]->Text.c_str());
				if (k > 0) {
					if (k > kilpparam.valuku)
						k = kilpparam.valuku;
					EC[i][j]->Text = k;
					}
				else {
					if (wcswcind(towupper(EC[i][j]->Text.c_str()[0]), ch_piste) >= 0)
						EC[i][j]->Text = towupper(EC[i][j]->Text.c_str()[0]);
					else
						EC[i][j]->Text = L'A';
					}
				}
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMaalikello::EdLaite2NoExit(TObject *Sender)
{
	int laite = _wtoi(EdLaite2No->Text.c_str());

	if (laite < 1)
		return;
	if (laite == 1) {
		Application->MessageBoxW(L"Määrittele laite 1 vasemmalla", L"Este", MB_OK);
		return;
		}
	if (regnly_open[laite-1] != 0) {
		Application->MessageBoxW(L"Laitenumero varattu toiselle laitteelle", L"Este", MB_OK);
		return;
		}
	CBLaite2->Enabled = _wtoi(EdLaite2No->Text.c_str()) > 1;
	CBLaiteYht2->Enabled = (CBLaite2->Enabled && CBLaite2->ItemIndex != 0);
	EdLaitePort2->Enabled = (CBLaite2->Enabled && CBLaite2->ItemIndex != 0);
	EdLaiteIP2->Visible = (CBLaite2->Enabled && CBLaite2->ItemIndex != 0 &&
		CBLaiteYht2->ItemIndex != 0);
	LbLaiteIP2->Visible = EdLaiteIP2->Visible;
	EdAikaEro2->Enabled = (CBLaite2->Enabled && CBLaite2->ItemIndex != 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormMaalikello::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Maalikellon käyttö");
}
//---------------------------------------------------------------------------

