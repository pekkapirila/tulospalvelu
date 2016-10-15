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

#include "UnitStatus.h"
#include "UnitMain.h"
#include "TapUnit2.h"
#include "TulosUnit.h"
#include "UnitSeurGr.h"
#include "VDeclare.h"
#include "UnitAjanotto.h"
#include "UnitAikavert.h"
#include "UnitGAZ.h"
#include "UnitYhteysStatus.h"
#include "UnitLahestyjat.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormStatus *FormStatus;
extern TTapForm *TapFrm[NTAPFORM];
extern TTilanneForm *TilFrm[NTILANNEFORM];
extern TSeurantaForm *SeurFrmGr[NSEURANTAFORM];
extern TFormGAZ *FrmGAZ[2];
extern int viimTap;

static int YhtLkm;
typedef struct {
	int yhteys;
	wchar_t tila[3];
	int jonossa;
	int lahetetty;
	} yhtstatustp;

static yhtstatustp yhtstatus[MAX_PORTTI];

//---------------------------------------------------------------------------
__fastcall TFormStatus::TFormStatus(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	LkmGrid->ColWidths[0] = 55 * Screen->PixelsPerInch / ppi0;
	memset(edTap, '\xff', sizeof(edTap));
}
//---------------------------------------------------------------------------
statusIkkParamClass::statusIkkParamClass(void)
{
	memset(this, 0, sizeof(statusIkkParamClass));
}
//---------------------------------------------------------------------------
void statusIkkParamClass::operator=(statusIkkParamClass& ikkPar)
{
	memcpy(this, &ikkPar, sizeof(statusIkkParamClass));
}
//---------------------------------------------------------------------------
void TFormStatus::haeParams(void)
{
	//
}
//---------------------------------------------------------------------------

int TFormStatus::applyParams(void)
{
	if (IkkParam.yhteysikkuna) {
		if (FormYhteysStatus == NULL)
			FormYhteysStatus = new TFormYhteysStatus(FormMain);
		FormYhteysStatus->Show();
		}
	FormResize(this);
	return(0);
}
//---------------------------------------------------------------------------
void naytayhteydet(void)
	{
	INT cn, t, yhtavattu = 0, yhtlkm = 0, jonossa = 0, jonoja = 0;
	UINT32 ipno;
	wchar_t line[105], line1[10], dest[20];
	static int edlahkello[MAX_LAHPORTTI];
	TStringGrid *YhteysGrid;

	if (FormStatus == NULL || FormStatus->YhteysGrid == NULL) {
		Sleep(200);
		return;
		}
	if (FormStatus->IkkParam.yhteysikkuna)
		YhteysGrid = FormYhteysStatus->YhteysGrid;
	else
		YhteysGrid = FormStatus->YhteysGrid;
	YhteysGrid->Cells[0][0] = UnicodeString(L"Yhteys");
	YhteysGrid->Cells[1][0] = UnicodeString(L"Kohde");
	YhteysGrid->Cells[2][0] = UnicodeString(L"I/O");
	YhteysGrid->Cells[3][0] = UnicodeString(L"Tila");
	YhteysGrid->Cells[4][0] = UnicodeString(L"Jonossa");
	YhteysGrid->Cells[5][0] = UnicodeString(L"Lähetetty");
	YhteysGrid->ColCount = 6;
	YhteysGrid->RowCount = YhtLkm > 1 ? YhtLkm+1 : 2;
	jonossa = 0;
	for (cn = 0; cn < maxyhteys; cn++) {
		memset(line, 0, sizeof(line));
		if (!comopen[cn]) {
//			swprintf(line, L"%3d Ei avattu", cn+1);
//			wmemset(line+wcslen(line), L' ', 64-wcslen(line));
			}
		else if (comtype[cn] >= 0 || comtype[cn] <= 2) {
			yhtlkm++;
			if (YhteysGrid->RowCount < yhtlkm+1) {
				YhteysGrid->RowCount = yhtlkm+1;
				}

			switch (comtype[cn]) {
				case comtpRS :
					swprintf(line, L"COM%d", portti[cn]);
					break;
				case comtpUDP :
					if (wcscmp(ipparam[cn].destaddr, L"AUTO") || (ipno = UDPaddr(hComm[cn])) == 0) {
						wcsncpy(dest, ipparam[cn].destaddr, 15);
						dest[15] = 0;
						}
					else {
						swprintf(dest, L"%u.%u.%u.%u", ipno/16777216L, (ipno/65536L) & 255,
							(ipno/256) & 255, ipno & 255);
						}
					swprintf(line, L"UDP:%5d/%15.15s:%d", (int)ipparam[cn].srvport,
						dest, (int)UDPcliport(hComm[cn]));
					break;
				case comtpTCP :
				case comtpTCPLOKI :
				case comtpTCPXML :
					swprintf(line, L"%3d TCP   %15.15s:%d", cn+1,
						ansitowcs(dest, TCPdestaddr(hComm[cn]), 16), TCPdestport(hComm[cn]));
					break;
				case comtpTCPSRV :
					swprintf(line, L"%3d TCP   %15.15s", cn+1,
						ansitowcs(dest, TCPdestaddr(hComm[cn]), 16));
					break;
				}
			if (!lkmThreadOn)
				return;
			if (wcscmp(yhtstatus[cn].tila, L"Ei") && yhtstatus[cn].tila[0])
				yhtavattu++;
			if (vastcom[cn])
				wcscpy(line1, L"In ");
			else
				wcscpy(line1, L"   ");
			if (lahcom[cn])
				wcscat(line1, L"Out");
			else
				wcscat(line1, L"   ");
			if (!lkmThreadOn)
				return;
			YhteysGrid->Cells[0][yhtlkm] = UnicodeString(cn+1);
			YhteysGrid->Cells[1][yhtlkm] = UnicodeString(line);
			YhteysGrid->Cells[2][yhtlkm] = UnicodeString(line1);
			DSPQ(0, cn);
			YhteysGrid->Cells[3][yhtlkm] = yhtstatus[cn].tila;
			YhteysGrid->Cells[4][yhtlkm] = yhtstatus[cn].jonossa;
			jonossa += yhtstatus[cn].jonossa;
			if (yhtstatus[cn].jonossa >= jonokynnys)
				jonoja++;
			YhteysGrid->Cells[5][yhtlkm] = yhtstatus[cn].lahetetty;
			if (lahkello[cn] && (t = mstimer()) > edlahkello[cn] + lahkello[cn] &&
				lahettamatta(cn) == 0) {
				lahetakello(cn, 0);
				edlahkello[cn] = t;
				}
			}
		}
	YhtLkm = yhtlkm;
	if (!lkmThreadOn)
		return;
	YhteysGrid->RowCount = (yhtlkm > 0) ? yhtlkm+1 : 2;
	FormMain->EdtYhtLkm->Text = UnicodeString(yhtlkm);
	FormMain->EdtYhtAvattu->Text = UnicodeString(yhtavattu);
	FormMain->EdtLahet->Text = jonossa;
	if (jonossa < 2*yhtlkm+5) {
		FormMain->EdtLahet->Color = clWindow;
		FormMain->LblLahet->Font->Color = clWindowText;
		}
	else if (jonossa < 2*yhtlkm+50) {
		FormMain->EdtLahet->Color = clYellow;
		FormMain->LblLahet->Font->Color = clWindowText;
		}
	else {
		FormMain->EdtLahet->Color = clYellow;
		FormMain->LblLahet->Font->Color = clRed;
		}
	lahetaMonitorille(yhtlkm, yhtavattu, jonossa, jonoja);
	}

//---------------------------------------------------------------------------
void __fastcall TFormStatus::Lukumaarat(void)
   {
	int nTulos, nVaTulos;
	int nKesk;
	int nHyl;
	int nEil;
	int nAvoin;
	static int nEsit = 0;
	static int nEmit = 0;
	int nEmitKoodit;
	int nYhtLht;
	int srj;
	static int kv, kokvert = -1, osvert[MAXSARJALUKU][MAXOSUUSLUKU][VAIKALUKU+1];
	wchar_t wst[80];
	static int edmsT = 0;
	int msT;

	while (lkmThreadOn) {
		for (int i = 0; i < 2; i++) {
			if (sulkemassa) {
				break;
				}
			if (FrmGAZ[i]) {
				FrmGAZ[i]->paiv_taulu();
				}
			}

		for (int i = 0; i < NLAHEST; i++) {
			if (FormLahestyjat[i] && FormLahestyjat[i]->Visible) {
				msT = mstimer();
				if (msT >= edmsT + 800) {
					edmsT = msT;
					FormLahestyjat[i]->Paivita(this);
					}
				}
			}

		for (int i = 0; i < NTILANNEFORM; i++) {
			static int edAika[NTILANNEFORM];
			long krki, aikaNyt;
			char aikastr[10];

			if (sulkemassa) {
				break;
				}
//			if (TilFrm[i] && !TilFrm[i]->Aktiivinen) {
//				delete TilFrm[i];
//				TilFrm[i] = NULL;
//				}
			if (TilFrm[i] && TilFrm[i]->Aktiivinen && TilFrm[i]->Visible) {
				int piste = TilFrm[i]->IkkParam.Va;
				if (piste < 0 || piste > kilpparam.maxvaluku)
					piste = 0;
				krki = pkarki[TilFrm[i]->IkkParam.Sarja][TilFrm[i]->IkkParam.Osuus][piste];
				if (krki <= 0)
					continue;
				krki += Sarjat[TilFrm[i]->IkkParam.Sarja].lahto;
				aikaNyt = ((T_TIME(biostime(0, 0)) - krki/KSEK) / 10) * 10;
				aikaNyt = (aikaNyt + 864000) % 864000;
				if (edAika[i] != aikaNyt) {
					strncpy(aikastr, aikatostr_l(aikaNyt, 0), 8);
					aikastr[8] = 0;
					TilFrm[i]->EditAlusta->Text = aikastr;
					TilFrm[i]->EditAlusta->Refresh();
					edAika[i] = aikaNyt;
					}
				}
			}

		if (paivitaAikanaytot) {
//			Sleep(20);
			if (sulkemassa) {
				break;
				}
			for (int i = 0; i < NAJANOTOT; i++) {
				if (FormAjanotot[i] != NULL)
					FormAjanotot[i]->Paivita_Aikanaytto();
				if (FormAikavertailut[i])
					FormAikavertailut[i]->Paivita_Aikanaytto();
				}
			paivitaAikanaytot = false;
			}

		naytayhteydet();
		if (sulkemassa) {
			break;
			}
//		FormStatus->TarkPyynnot();
		FormMain->EdtTietueita->Text = UnicodeString(nilmt);
		FormMain->NaytaIkkunalista();
		nTulos = 0;
		nKesk = 0;
		nHyl = 0;
		nEil = 0;
		nAvoin = 0;
		if (kilpparam.partio) {
			for (int srj = 0; srj < sarjaluku; srj++) {
				int os = Sarjat[srj].osuusluku-1;
				nTulos += ntulos[srj][os][0];
				nKesk += nkesk[srj][os];
				nHyl += nhyl[srj][os];
				nEil += neil[srj][os];
				nAvoin += navoin[srj][os];
				}
			}
		else
			{
			nVaTulos = 0;
			nYhtLht = 0;
			nEmitKoodit = 0;
			for (int os = 0; os < kilpparam.ntosuus; os++) {
				nTulos += ntulost[os];
				nKesk += nkeskt[os];
				nHyl += nhylt[os];
				nEil += neilt[os];
				nAvoin += navoint[os];
				for (int srj = 0; srj < sarjaluku; srj++) {
					nYhtLht += nylahto[srj][os];
					nEmitKoodit += nekoodi[srj][os];
					}
				}
			}
		kv = colMuutos ? 1 : 0;
		colMuutos = false;
		if (emitfl) {
			if (nEsit != nesitt || nEmit != emithead)
				kv += 1;
			nEsit = nesitt;
			nEmit = emithead;
			}
		else {
			nEsit = 0;
			nEmit = 0;
			}
		if (FormMain->Nyttulokset1->Checked)
			FormMain->EdtTuloksia->Text = UnicodeString(nTulos);
		else
			FormMain->EdtTuloksia->Text = UnicodeString(nAvoin);
		for (int va = 0; va <= kilpparam.maxvaluku; va++) {
			for (srj = 0; srj < sarjaluku; srj++) {
				for (int os = 0; os < Sarjat[srj].ntosuus; os++)
					{
					kv += tlsmuutos[srj][os][va];
					if (kv)
						break;
					}
				if (kv)
					break;
				}
			if (kv)
				break;
			}

		if (!kv)
			break;

		if (sulkemassa) {
			break;
			}

		FormStatus->LkmGrid->Cells[monisarja+1][1] = nilmtt;
		FormStatus->LkmGrid->Cells[monisarja+2][1] = nTulos;
		FormStatus->LkmGrid->Cells[monisarja+3][1] = nKesk;
		FormStatus->LkmGrid->Cells[monisarja+4][1] = nHyl;
		FormStatus->LkmGrid->Cells[monisarja+5][1] = nEil;
		FormStatus->LkmGrid->Cells[monisarja+6][1] = nAvoin;
		if (IkkParam.ylahdot)
			FormStatus->LkmGrid->Cells[monisarja+7][1] = nYhtLht;
		if (IkkParam.esitykset)
			FormStatus->LkmGrid->Cells[monisarja+IkkParam.ylahdot+7][1] = nEsit;
		if (IkkParam.emittiedot)
			FormStatus->LkmGrid->Cells[monisarja+IkkParam.ylahdot+IkkParam.esitykset+7][1] = nEmit;
		if (IkkParam.emitkoodeja)
			FormStatus->LkmGrid->Cells[monisarja+IkkParam.ylahdot+IkkParam.esitykset+IkkParam.emittiedot+7][1] = nEmitKoodit;
		for (int va = 0; va < kilpparam.maxvaluku; va++)
			FormStatus->LkmGrid->Cells[monisarja+iLisa+1+va][1] = L"";

		for (srj = 0; srj < sarjaluku; srj++) {
			if (sulkemassa) {
				break;
				}
			for (int os = 0; os < Sarjat[srj].ntosuus; os++)
				{
				int rr;
				if (kilpparam.partio) {
					if (os < Sarjat[srj].ntosuus-1)
						continue;
					rr = srj+2;
					}
				else
					rr = rcnt[srj]+os+2;
				for (int va = 0; va <= kilpparam.maxvaluku; va++) {
					if (tlsmuutos[srj][os][va]) {
						FormStatus->muutos(srj, os, va);
						tlsmuutos[srj][os][va] = 0;
						}
					}
				if (monisarja)
					FormStatus->LkmGrid->Cells[0][rr] = ansitowcs(wst, Sarjat[srj].sarjanimi, 40);
				FormStatus->LkmGrid->Cells[monisarja][rr] = os+1;
				FormStatus->LkmGrid->Cells[monisarja+1][rr] = nilm[srj];
				FormStatus->LkmGrid->Cells[monisarja+2][rr] = ntulos[srj][os][0];
				FormStatus->LkmGrid->Cells[monisarja+3][rr] = nkesk[srj][os];
				FormStatus->LkmGrid->Cells[monisarja+4][rr] = nhyl[srj][os];
				FormStatus->LkmGrid->Cells[monisarja+5][rr] = neil[srj][os];
				FormStatus->LkmGrid->Cells[monisarja+6][rr] = navoin[srj][os];
				if (IkkParam.ylahdot)
					FormStatus->LkmGrid->Cells[monisarja+7][rr] = nylahto[srj][os];
				if (IkkParam.esitykset)
					FormStatus->LkmGrid->Cells[monisarja+IkkParam.ylahdot+7][rr] = L"";
				if (IkkParam.emittiedot)
					FormStatus->LkmGrid->Cells[monisarja+IkkParam.ylahdot+IkkParam.esitykset+7][rr] = L"";
				if (IkkParam.emitkoodeja)
					FormStatus->LkmGrid->Cells[monisarja+IkkParam.ylahdot+IkkParam.esitykset+IkkParam.emittiedot+7][rr] = nekoodi[srj][os];
				for (int va = 0; va < kilpparam.maxvaluku; va++)
					FormStatus->LkmGrid->Cells[monisarja+iLisa+1+va][rr] = ntulos[srj][os][va+1];
				}
			}
		for (int i = 0; i < NTAPFORM; i++) {
			if (sulkemassa) {
				break;
				}
			if (TapFrm[i] && TapFrm[i]->Visible && edTap[i] != viimTap) {
				TapFrm[i]->Paivita(FormStatus);
				edTap[i] = viimTap;
				}
			}
		break;
		}
	if (FormStatus->YhteysGrid->Height < FormStatus->GBAutotulostus->Height)
		FormStatus->FormResize(this);
	}

void __fastcall TFormStatus::muutos(int srj, int os, int piste)
	{
	for (int i = 0; i < NSEURANTAFORM; i++) {
		if (SeurFrmGr[i] && SeurFrmGr[i]->Visible) {
			if (SeurLstPaivita)
				SeurFrmGr[i]->PaivitaButtonClick(FormMain);
			if (SeurGrPaivita)
				SeurFrmGr[i]->PiirraBtnClick(FormMain);
			}
		}
	for (int i = 0; i < NTILANNEFORM; i++) {
		if (TilFrm[i] && TilFrm[i]->Aktiivinen && TilFrm[i]->Visible && TilFrm[i]->IkkParam.Seuraa
		 && TilFrm[i]->IkkParam.Sarja == srj && TilFrm[i]->IkkParam.Osuus == os
		 && TilFrm[i]->IkkParam.Va == piste) {
		 if (TlsLstPaivita)
				TilFrm[i]->PaivBtnClick(FormMain);
		 }
	  }
   }

void __fastcall TFormStatus::FormResize(TObject *Sender)
{
	if (ClientHeight < 240 * Screen->PixelsPerInch / 96 || ClientWidth < 40 * Screen->PixelsPerInch / 96)
		return;
	LkmGrid->Width = ClientWidth;
	if (autofl || (YhtLkm > 4 && !IkkParam.yhteysikkuna)) {
		YhteysGrid->Height = GBAutotulostus->Height;
		}
	else {
		if (IkkParam.yhteysikkuna) {
			YhteysGrid->Height = 0;
			}
		else {
			YhteysGrid->Height = (YhtLkm > 1 ? YhtLkm+1 : 2) * (YhteysGrid->RowHeights[1]+3);
			}
		}
	YhteysGrid->Visible = !IkkParam.yhteysikkuna;
	YhteysGrid->Top = ClientHeight - (YhteysGrid->Height ? YhteysGrid->Height - 4 : 0);
	LkmGrid->Height = YhteysGrid->Top - LkmGrid->Top - 4;
	GBAutotulostus->Top = ClientHeight - GBAutotulostus->Height - 4;
	if (autofl) {
		GBAutotulostus->Left = LkmGrid->Left + LkmGrid->Width - GBAutotulostus->Width;
		YhteysGrid->Width = GBAutotulostus->Left - YhteysGrid->Left - 4;
		}
	else
		YhteysGrid->Width = LkmGrid->Width;

}
//---------------------------------------------------------------------------
void WRITEYHT(wchar_t *st, INT cn)
   {
   if (comtype[cn] == 2)
	  return;
	wcsncpy(yhtstatus[cn].tila, st, 2);
	}

//---------------------------------------------------------------------------

void DSPQ(INT r,INT cn)
	{
	INT nj;

	nj = lahettamatta(cn);
	yhtstatus[cn].jonossa = nj + lah_alku[cn];
	yhtstatus[cn].lahetetty = cjens[cn];
	}
//---------------------------------------------------------------------------
void paivita_aikanaytto(void)
{
	FormStatus->paivitaAikanaytot = true;
}
//---------------------------------------------------------------------------
void autostatus(int kynnys, int korkein, wchar_t rajaus, int viimsija)
{
	UnicodeString Laaj;
	extern int intv[];

	if (rajaus == L'P')
		Laaj = UnicodeString(rajaus)+UnicodeString(viimsija);
	else
		Laaj = UnicodeString(rajaus);
	FormStatus->EdtLaajuus->Text = Laaj;
	FormStatus->EdtKynnys->Text = kynnys;
	FormStatus->EdtKorkein->Text = korkein;
	FormStatus->EdtAikaaskel->Text = intv[0]/20;
}
//---------------------------------------------------------------------------

void __fastcall TFormStatus::Yhteislht1Click(TObject *Sender)
{
	Yhteislht1->Checked = !Yhteislht1->Checked;
	IkkParam.ylahdot = Yhteislht1->Checked ? 1 : 0;
	FormSetup();
}
//---------------------------------------------------------------------------

void __fastcall TFormStatus::Hylkysesitykset1Click(TObject *Sender)
{
	Hylkysesitykset1->Checked = !Hylkysesitykset1->Checked;
	IkkParam.esitykset = Hylkysesitykset1->Checked ? 1 : 0;
	FormSetup();
}
//---------------------------------------------------------------------------

void __fastcall TFormStatus::Emittietueet1Click(TObject *Sender)
{
	Emittietueet1->Checked = !Emittietueet1->Checked;
	IkkParam.emittiedot = Emittietueet1->Checked ? 1 : 0;
	FormSetup();
}
//---------------------------------------------------------------------------

void __fastcall TFormStatus::Help1Click(TObject *Sender)
{
	Application->HelpKeyword(L"ViestiWin");
}
//---------------------------------------------------------------------------

void __fastcall TFormStatus::FormSetup(void)
{
	if (kilpparam.partio)
		FormStatus->LkmGrid->RowCount = sarjaluku+2;
	else {
		rcnt[0] = 0;
		for (int srj = 0; srj < sarjaluku; srj++)
			rcnt[srj+1] = rcnt[srj] + Sarjat[srj].ntosuus;
		FormStatus->LkmGrid->RowCount = rcnt[sarjaluku]+2;
		}
	FormStatus->LkmGrid->FixedRows = 2;
	iLisa = 6 + IkkParam.emittiedot + IkkParam.emitkoodeja + IkkParam.esitykset + IkkParam.ylahdot;
	FormStatus->LkmGrid->ColCount = iLisa + 1 + monisarja + kilpparam.maxvaluku;
	FormStatus->LkmGrid->FixedCols = 1 + monisarja;
	if (monisarja) {
		FormStatus->LkmGrid->Cells[0][0] = L"Sarja";
		FormStatus->LkmGrid->ColWidths[0] = 55 * Screen->PixelsPerInch / ppi0;
		}
	FormStatus->LkmGrid->Cells[monisarja][0] = L"Osuus";
	FormStatus->LkmGrid->Cells[monisarja+1][0] = L"Ilm";
	FormStatus->LkmGrid->Cells[monisarja+2][0] = L"Tulos";
	FormStatus->LkmGrid->Cells[monisarja+3][0] = L"Kesk";
	FormStatus->LkmGrid->Cells[monisarja+4][0] = L"Hyl";
	FormStatus->LkmGrid->Cells[monisarja+5][0] = L"Eil";
	FormStatus->LkmGrid->Cells[monisarja+6][0] = L"Avoin";
	if (IkkParam.ylahdot)
		FormStatus->LkmGrid->Cells[monisarja+7][0] = L"YhtLht";
	if (IkkParam.esitykset)
		FormStatus->LkmGrid->Cells[monisarja+IkkParam.ylahdot+7][0] = L"Esit";
	if (IkkParam.emittiedot)
		FormStatus->LkmGrid->Cells[monisarja+IkkParam.ylahdot+IkkParam.esitykset+7][0] = L"Emit";
	if (IkkParam.emitkoodeja)
		FormStatus->LkmGrid->Cells[monisarja+IkkParam.ylahdot+IkkParam.esitykset+IkkParam.emittiedot+7][0] = L"EmKoodit";
	for (int va = 1; va <= kilpparam.maxvaluku; va++)
		FormStatus->LkmGrid->Cells[monisarja+iLisa+va][0] = UnicodeString(va)+L".va";
	FormStatus->LkmGrid->Cells[0][1] = L"Yht.";
	colMuutos = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormStatus::FormActivate(TObject *Sender)
{
	monisarja = sarjaluku > 1 ? 1 : 0;
	IkkParam.esitykset = Hylkysesitykset1->Checked ? 1 : 0;
	IkkParam.ylahdot = Yhteislht1->Checked ? 1 : 0;
	IkkParam.emittiedot = Emittietueet1->Checked ? 1 : 0;
	IkkParam.emitkoodeja = Emitkoodeja1->Checked ? 1 : 0;
	FormSetup();
}
//---------------------------------------------------------------------------

void __fastcall TFormStatus::FormShow(TObject *Sender)
{
	static bool initialisoitu;

	if (!initialisoitu) {
		if (kilpparam.badgelaji == L'E')
			Hylkysesitykset1->Checked = true;
		if (emitfl) {
			Hylkysesitykset1->Checked = true;
			Emittietueet1->Checked = true;
			Emittietueet1->Visible = true;
			}
		}
	initialisoitu = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormStatus::Emitkoodeja1Click(TObject *Sender)
{
	Emitkoodeja1->Checked = !Emitkoodeja1->Checked;
	IkkParam.emitkoodeja = Emitkoodeja1->Checked ? 1 : 0;
	FormSetup();
}
//---------------------------------------------------------------------------

void __fastcall TFormStatus::Suljekaavake1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormStatus::Yhteydeterikaavakkeella1Click(TObject *Sender)
{
	Yhteydeterikaavakkeella1->Checked = !Yhteydeterikaavakkeella1->Checked;
	IkkParam.yhteysikkuna = Yhteydeterikaavakkeella1->Checked ? 1 : 0;
	if (IkkParam.yhteysikkuna) {
		if (FormYhteysStatus == NULL)
			FormYhteysStatus = new TFormYhteysStatus(FormMain);
		FormYhteysStatus->Show();
		if (FormYhteysStatus->WindowState == wsMinimized)
			FormYhteysStatus->WindowState = wsNormal;
		FormYhteysStatus->BringToFront();
		}
	else
		FormYhteysStatus->Close();
	FormResize(Sender);
}
//---------------------------------------------------------------------------

