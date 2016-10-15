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
#include "WinHk.h"
#include "TapUnit2.h"
#include "TulosUnit.h"
#include "UnitKilpailijaOnline.h"
#include "UnitKirjoitinVal.h"
#include "UnitAjanotto.h"
#include "UnitAikavert.h"
#include "UnitYhteysStatus.h"
#include "HkDeclare.h"
#include "HkMuotoilu.h"
#include "UnitJakelu.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormStatus *FormStatus;
extern TTapForm *FrmTap[NTAPFORM];
extern TTilanneForm *TilFrm[NTILANNEFORM];
int viimTap = MAXTAPAHTUMA-1;
tapTp Tapahtuma[MAXTAPAHTUMA];
bool TapEtunimiensin = true;
extern TFormKilpailijaOnline *FormKilpailijaOnlineA[NFORMKILPAILIJA];
void puuttuvat(wchar_t *paika, INT *psija, INT srj, INT df);

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
	LkmGrid->ColWidths[0] = 55 * Screen->PixelsPerInch / 96;
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
	if (VaiheenMahd->Checked)
		IkkParam.ParasSija = 1;
	else if (YhteisMahd->Checked)
		IkkParam.ParasSija = 2;
	else
		IkkParam.ParasSija = 0;
	IkkParam.PalkJako = Palkintojenjako1->Checked;
}
//---------------------------------------------------------------------------

int TFormStatus::applyParams(void)
{
	VaiheenMahd->Checked = IkkParam.ParasSija == 1;
	YhteisMahd->Checked = IkkParam.ParasSija == 2;
	Palkintojenjako1->Checked = IkkParam.PalkJako;
	if (IkkParam.yhteysikkuna) {
		if (FormYhteysStatus == NULL)
			FormYhteysStatus = new TFormYhteysStatus(FormMain);
		FormYhteysStatus->Show();
		}
	FormResize(this);
	colMuutos = true;
	return(0);
}
//---------------------------------------------------------------------------
void tyhjtapahtumat(void)
	{
	viimTap = MAXTAPAHTUMA-1;
	memset(Tapahtuma, 0, sizeof(Tapahtuma));
	}
//---------------------------------------------------------------------------
void addtapahtuma(kilptietue *kilp, int piste)
	{
	wchar_t ln[100];
	int tap = (viimTap + 1) % MAXTAPAHTUMA, pst = 0;

	if (piste < 0)
		return;
	Tapahtuma[tap].Kilpno = kilp->id();
	kilp->nimi(ln, 99, TapEtunimiensin);
	Tapahtuma[tap].Nimi = ln;
	wcsncpy(ln, kilp->seuratieto(0, kilpparam.seuralyh_on), 99);
	Tapahtuma[tap].Seura = ln;
	Tapahtuma[tap].Maa = kilp->maa;
	Tapahtuma[tap].Sarja = kilp->Sarja();
	Tapahtuma[tap].Piste = piste;
	if (piste <= Sarjat[kilp->Sarja()].valuku[k_pv])
		pst = piste;
	else if (piste == kilpparam.valuku+2) {
		for (int i = 1; i <= Sarjat[kilp->Sarja()].valuku[k_pv]+1; i++) {
			int j = i % (Sarjat[kilp->Sarja()].valuku[k_pv]+1);
			if (kilp->p_aika(j) > 0)
				pst = j;
			}
		}
	else
		pst = 0;
	Tapahtuma[tap].Sija = kilp->p_sija(pst);
	Tapahtuma[tap].Tulos = kilp->p_aika(pst);
	if (kilpparam.kilplaji == L'A' && kilp->hyv()) {
		memset(ln, 0, sizeof(ln));
		wcsncpy(ln, kilp->pv[k_pv].asakot, 4);
		}
	else {
		ln[0] = kilp->tark(k_pv);
		ln[1] = 0;
		}
	Tapahtuma[tap].Tark = ln;
	Tapahtuma[tap].pisteet = kilp->pv[k_pv].pvpisteet[0];
	if (k_pv) {
		Tapahtuma[tap].yTulos = kilp->ytulos_p(piste);
		Tapahtuma[tap].ySija = piste ? 0 : kilp->y_sija(k_pv);
		}
	viimTap = tap;
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
					swprintf(line, L"TCP:%15.15s:%d",
						ansitowcs(dest, TCPdestaddr(hComm[cn]), 16), TCPdestport(hComm[cn]));
					break;
				case comtpTCPSRV :
				case comtpTCPSRVXML :
					swprintf(line, L"TCP:%15.15s",
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

	if (!status_on)
		return;
	nj = lahettamatta(cn);
	yhtstatus[cn].jonossa = nj + lah_alku[cn];
	yhtstatus[cn].lahetetty = cjens[cn];
	}

//---------------------------------------------------------------------------
void __fastcall muutos(int srj, int piste)
	{

	if (TlsLstPaivita) {
		for (int i = 0; i < NTILANNEFORM; i++) {
			if (!lkmThreadOn)
				return;
			tlsmuutos[srj][piste] = 0;
			if (TilFrm[i] && TilFrm[i]->Visible && (!TilFrm[i]->IkkParam.vainjarj ||
				TilFrm[i]->IkkParam.Va == piste || (!piste && TilFrm[i]->IkkParam.Va == kilpparam.valuku+1))) {
				if (TilFrm[i]->IkkParam.Sarja == srj) {
// Muuta sanomaksi
					TilFrm[i]->PaivBtnClick(FormStatus);
					}
				if (TilFrm[i]->IkkParam.Sarja >= sarjaluku) {
					for (int yhd = 0; yhd < nsarjayhd; yhd++) {
						if (yhdsarjat[yhd][srj]) {
// Muuta sanomaksi
							TilFrm[i]->PaivBtnClick(FormStatus);
							break;
							}
						}
					}
				}
			}
		}
	}

//---------------------------------------------------------------------------
void __fastcall TFormStatus::Lukumaarat(void)
	{
	int nAvoin;
	int nEmit;
	int cc;
	static int kv, kokvert, srjvert[MAXSARJALUKU][VALUKU+1], edTap;
	extern TTapForm *TapForm;
	static long edAika[NTILANNEFORM], edKAika[NFORMKILPAILIJA+1], edsrjlkm, edvaluku, ed_pv;
	static int edAikaTap[NTAPFORM];
	int aikaNyt, pohjaaika;
	wchar_t aikastr[16];

	while (lkmThreadOn) {
		if (suljesuoraan || !lkmThreadOn)
			break;
		YhteisMahd->Visible = k_pv > 0;
		GBAutotulostus->Visible = autofl;
		FormMain->EdtTietueita->Text = UnicodeString(nilmt);
		FormMain->NaytaIkkunalista();
		if (colMuutos || sarjaluku != edsrjlkm || kilpparam.maxvaluku != edvaluku || k_pv != ed_pv) {
			colMuutos = false;
			edsrjlkm = sarjaluku;
			edvaluku = kilpparam.maxvaluku;
			ed_pv = k_pv;
			LkmGrid->RowCount = sarjaluku+2;
			LkmGrid->Cells[1][0] = UnicodeString(L"Ilm");
			LkmGrid->Cells[2][0] = UnicodeString(L"Tulos");
			LkmGrid->Cells[3][0] = UnicodeString(L"Kesk");
			LkmGrid->Cells[4][0] = UnicodeString(L"Hyl");
			LkmGrid->Cells[5][0] = UnicodeString(L"Eil (Aika)");
			LkmGrid->Cells[6][0] = UnicodeString(L"Avoin");
			LkmGrid->ColWidths[5] = 55*Screen->PixelsPerInch/96;
			cc = 7;
			if (kilpparam.badgelaji == L'E') {
				LkmGrid->Cells[7][0] = UnicodeString(L"Esit");
				LkmGrid->Cells[8][0] = UnicodeString(L"Emit");
				cc = 9;
				}
			LkmGrid->ColCount = cc+kilpparam.maxvaluku;
			for (int k = 1; k <= kilpparam.maxvaluku; k++)
				LkmGrid->Cells[cc++][0] = UnicodeString(k)+UnicodeString(L".va");
			if (VaiheenMahd->Checked || YhteisMahd->Checked) {
				LkmGrid->ColCount++;
				mahdCol = cc;
				LkmGrid->Cells[cc++][0] = L"Mahd";
				}
			else
				mahdCol = 0;
			if (Palkintojenjako1->Checked) {
				LkmGrid->ColCount++;
				jakoCol = cc;
				LkmGrid->Cells[jakoCol][0] = L"Jako";
				}
			else
				jakoCol = 0;
			}
		naytayhteydet();
//      FormMain->TarkPyynnot();

		EdtLaajuus->Text = AutoLaaj;
		EdtKynnys->Text = AutoKynnys;
		EdtKorkein->Text = AutoKorkein;
		EdtAikaaskel->Text = AutoAikaaskel;

		aikaNyt = ((Nyt() + 24*TUNTI) / SEK) * SEK - 24*TUNTI;

		pohjaaika = eralahto[era_akt] != AIKAJAK*TMAALI0 ? (eralahto[era_akt]/AIKAJAK - t0_regnly) : 0;

		for (int i = 0; i < NTAPFORM; i++) {
			TFormKilpailijaOnline *Frm;
			if (FrmTap[i]) {
				if (FrmTap[i]->Visible) {
					if (edAikaTap[i] != aikaNyt - pohjaaika) {
						AIKATOWSTRS(aikastr, aikaNyt-pohjaaika, t0);
						aikastr[8] = 0;
						FrmTap[i]->EdtKello->Text = aikastr;
						FrmTap[i]->EdtKello->Refresh();
						FrmTap[i]->EdtEra->Text = era_akt;
						FrmTap[i]->EdtEra->Refresh();
						edAikaTap[i] = aikaNyt-pohjaaika;
						}
					if (edTap != viimTap) {
						FrmTap[i]->Paivita(FormMain);
						}
					}
				Frm = FrmTap[i]->FormKilpailijaOnlineTap;
				if (Frm != NULL && Frm->Visible && Frm->Seuranta) {
					Frm->naytaSeuranta();
					}
				}
			}
		edTap = viimTap;

		for (int i = 0; i < NTILANNEFORM; i++) {
			TFormKilpailijaOnline *Frm;
			if (TilFrm[i]) {
				if (TilFrm[i]->Visible && TilFrm[i]->aikaNyt != aikaNyt) {
					if (TilFrm[i]->aikaNyt == 0)
						Sleep(200);
					AIKATOWSTRS(aikastr, aikaNyt, t0);
					aikastr[8] = 0;
					TilFrm[i]->EditKello->Text = aikastr;
					TilFrm[i]->EditKello->Refresh();
					TilFrm[i]->aikaNyt = aikaNyt;
					if ((TilFrm[i]->LA && TilFrm[i]->Seuraalhtj1->Checked)
						|| (TilFrm[i]->IkkParam.smartJrj))
						TilFrm[i]->PaivBtnClick(FormStatus);
					}
				Frm = TilFrm[i]->FormKilpailijaOnlineTilanne;
				if (Frm != NULL && Frm->Visible && Frm->Seuranta) {
					Frm->naytaSeuranta();
					}
				}
			}

		for (int i = -1; i < NFORMKILPAILIJA; i++) {
			TFormKilpailijaOnline *Frm;

			if (!lkmThreadOn)
				return;
			if (i == -1)
				Frm = FormKilpailijaOnline;
			else
				Frm = FormKilpailijaOnlineA[i];
			if (Frm && Frm->Visible && Frm->Seuranta) {
				Frm->naytaSeuranta();
				}
			}

		if (paivitaAikanaytot) {
			for (int i = 0; i < NAJANOTOT; i++) {
				if (FormAjanotot[i])
					FormAjanotot[i]->Paivita_Aikanaytto();
				if (FormAikavertailut[i])
					FormAikavertailut[i]->Paivita_Aikanaytto();
				}

			paivitaAikanaytot = false;
			}

		nAvoin = nilmt-ntulost-nkeskt-nhylt-neilt;
		nEmit = emithead;

		if (FormMain->Nyttulokset1->Checked)
			FormMain->EdtTuloksia->Text = UnicodeString(ntulost);
		else
			FormMain->EdtTuloksia->Text = UnicodeString(nAvoin);

		if (aikaNyt == edaikaNyt) {
			kv = ntulost + 7*nkeskt + 11*nhylt + 19*nesitt + 23*nEmit+ 31*nAvoin;
			for (int srj = 0; srj < sarjaluku; srj++) {
				kv += 43*(srj+1)*nilm[srj] + 49 * palkJaettu[srj];
				for (int Va = 0; Va < Sarjat[srj].valuku[k_pv]; Va++) {
					kv += 41* ntulos[srj][Va+1];
					}
				}
			if (kv == kokvert)
				break;
			kokvert = kv;
			}
		edaikaNyt = aikaNyt;

		LkmGrid->Cells[0][1] = UnicodeString(L"Yht.");
		LkmGrid->Cells[1][1] = UnicodeString(nilmt);
		LkmGrid->Cells[2][1] = UnicodeString(ntulost);
		LkmGrid->Cells[3][1] = UnicodeString(nkeskt);
		LkmGrid->Cells[4][1] = UnicodeString(nhylt);
		LkmGrid->Cells[5][1] = UnicodeString(neilt);
		LkmGrid->Cells[6][1] = UnicodeString(nAvoin);
		if (kilpparam.badgelaji == L'E') {
			LkmGrid->Cells[7][1] = UnicodeString(nesitt);
			LkmGrid->Cells[8][1] = UnicodeString(nEmit);
		 cc = 9;
			}
		else
		 cc = 7;
		for (int Va = 0; Va < kilpparam.maxvaluku; Va++) {
			FormStatus->LkmGrid->Cells[cc+Va][1] = UnicodeString(nva[Va]);
		 }
		for (int srj = 0; srj < sarjaluku; srj++) {
		 int navoin;
		 int ov = ntulos[srj][0] + 7*nkesk[srj] + 11*nhyl[srj];
			if (tlsmuutos[srj][0] || ov != srjvert[srj][0]) {
				muutos(srj, 0);
				srjvert[srj][0] = ov;
				}
			for (int Va = 0; Va < kilpparam.maxvaluku; Va++) {
				if (tlsmuutos[srj][Va+1] || srjvert[srj][Va+1] != ntulos[srj][Va+1]) {
					muutos(srj, Va+1);
					srjvert[srj][Va+1] = ntulos[srj][Va+1];
					}
				}
			LkmGrid->Cells[0][srj+2] = UnicodeString(Sarjat[srj].sarjanimi);
			LkmGrid->Cells[1][srj+2] = UnicodeString(nilm[srj]);
			LkmGrid->Cells[2][srj+2] = UnicodeString(ntulos[srj][0]);
			LkmGrid->Cells[3][srj+2] = UnicodeString(nkesk[srj]);
			LkmGrid->Cells[4][srj+2] = UnicodeString(nhyl[srj]);
			if (neilaika[srj])
				LkmGrid->Cells[5][srj+2] = UnicodeString(neil[srj])+L" ("+neilaika[srj]+L")";
			else
				LkmGrid->Cells[5][srj+2] = UnicodeString(neil[srj]);
			navoin = nilm[srj]-ntulos[srj][0]-nkesk[srj]-nhyl[srj]-neil[srj];
			LkmGrid->Cells[6][srj+2] = UnicodeString(navoin);
			for (int Va = 1; Va <= Sarjat[srj].valuku[k_pv]; Va++)
				LkmGrid->Cells[cc-1+Va][srj+2] = UnicodeString(ntulos[srj][Va]);
			if (mahdCol) {
				wchar_t mst[20];
				int psija;
				puuttuvat(mst,&psija, srj, VaiheenMahd->Checked ? 1 : 0);
				LkmGrid->Cells[mahdCol][srj+2] = psija;
				}
			if (jakoCol) {
				LkmGrid->Cells[jakoCol][srj+2] = palkJaettu[srj] ? L"X" : L" ";
				}
			}
		break;
		}
	if (YhteysGrid->Height < GBAutotulostus->Height)
		FormResize(this);
	}

//---------------------------------------------------------------------------
void paivita_aikanaytto(void)
{
	FormStatus->paivitaAikanaytot = true;
}
//---------------------------------------------------------------------------
void __fastcall TFormStatus::FormResize(TObject *Sender)
{
	if (ClientHeight < 240 * Screen->PixelsPerInch / 96 || ClientWidth < 40 * Screen->PixelsPerInch / 96)
		return;
	LkmGrid->Width = ClientWidth - 10 * Screen->PixelsPerInch / 96;
	if (autofl || (YhtLkm > 4 && !IkkParam.yhteysikkuna)) {
		YhteysGrid->Height = GBAutotulostus->Height;
		}
	else {
		if (IkkParam.yhteysikkuna) {
			YhteysGrid->Height = 0;
			}
		else
			YhteysGrid->Height = (YhtLkm > 1 ? YhtLkm+1 : 2) * (YhteysGrid->RowHeights[1]+3);
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
void __fastcall TFormStatus::FormCreate(TObject *Sender)
{
	static int init;

	if (!init) {
//		LkmGrid->ColWidths[0] = 55 * Screen->PixelsPerInch / 96;
		}
	init = 1;
}
//---------------------------------------------------------------------------

void autostatus(int kynnys, int korkein, wchar_t rajaus, int viimsija)
{
	UnicodeString Laaj;

	if (rajaus == L'P')
		Laaj = UnicodeString(rajaus)+UnicodeString(viimsija);
	else
		Laaj = UnicodeString(rajaus);
	wcsncpy(FormStatus->AutoLaaj, Laaj.c_str(), 19);
	FormStatus->AutoKynnys = kynnys;
	FormStatus->AutoKorkein = korkein;
	FormStatus->AutoAikaaskel = intv[0]/18;
}

void __fastcall TFormStatus::VaiheenMahdClick(TObject *Sender)
{
	VaiheenMahd->Checked = !VaiheenMahd->Checked;
	if (VaiheenMahd->Checked) {
		IkkParam.ParasSija = 1;
		YhteisMahd->Checked = false;
		}
	else
		IkkParam.ParasSija = 0;
	colMuutos = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormStatus::YhteisMahdClick(TObject *Sender)
{
	YhteisMahd->Checked = !YhteisMahd->Checked;
	if (YhteisMahd->Checked) {
		IkkParam.ParasSija = 2;
		VaiheenMahd->Checked = false;
		}
	else
		IkkParam.ParasSija = 0;
	colMuutos = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormStatus::Palkintojenjako1Click(TObject *Sender)
{
	Palkintojenjako1->Checked = !Palkintojenjako1->Checked;
	IkkParam.PalkJako = Palkintojenjako1->Checked;
	if (Palkintojenjako1->Checked) {
		TextFl *flJako;
		wchar_t pjaot[] = L"\\PalkJaot1.txt";

		pjaot[9] = L'1'+k_pv;
		flJako = new TextFl((FormMain->CurrentDir + pjaot).c_str(), L"rt");
		if (flJako && flJako->IsOpen()) {
			for (int srj = 0; srj < sarjaluku; srj++) {
				wchar_t line[40];
				flJako->ReadLine(line, 39);
				palkJaettu[srj] = (line[0] == L'+');
				}
			}
		if (flJako)
			delete flJako;
		}
	Tuloskirjoitin1->Visible = Palkintojenjako1->Checked;
	Valinnat1->Visible = Palkintojenjako1->Checked;
	colMuutos = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormStatus::LkmGridDblClick(TObject *Sender)
{
	TextFl *flJako;
	wchar_t pjaot[] = L"\\PalkJaot1.txt";
	static int lkm = 5;
	int srj;

	if (jakoCol && LkmGrid->Col == jakoCol && LkmGrid->Row > 1 && LkmGrid->Row < sarjaluku+2) {
		palkJaettu[LkmGrid->Row-2] = !palkJaettu[LkmGrid->Row-2];
		pjaot[9] = L'1'+k_pv;
		flJako = new TextFl((FormMain->CurrentDir + pjaot).c_str(), L"wt");
		if (flJako && flJako->IsOpen()) {
			for (srj = 0; srj < sarjaluku; srj++) {
				wchar_t line[40];
				swprintf(line, L"%c  %s\n", palkJaettu[srj] ? L'+' : L'-',
					Sarjat[srj].sarjanimi);
				flJako->WriteLine(line);
				}
			}
		if (flJako)
			delete flJako;
		srj = LkmGrid->Row-2;
		if (palkJaettu[srj] && lsttulparam.lstf && lsttulparam.printer == GDIPRINTER) {
			wchar_t ch;
			inputint_prompt(&lkm, L"Tulostettavien lukumäärä", &ch, this);
			if (ch != ESC && lkm > 0)
				tulostaParhaat(srj, lkm);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormStatus::Suljekaavake1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormStatus::Tuloskirjoitin1Click(TObject *Sender)
{
	wchar_t LstKirjoitin[200];

	FormKirjVal = new TFormKirjVal(this);
	FormKirjVal->ShowModal();
	wcsncpy(LstKirjoitin,
		FormKirjVal->KirjoitinVal->Items->Strings[FormKirjVal->KirjoitinVal->ItemIndex].c_str(), 198);
	delete FormKirjVal;
	if (lsttulparam.lstf)
		closeprfile(lsttulparam.lstf);
	lsttulparam.printer = GDIPRINTER;
	if ((lsttulparam.lstf = openprfile(LstKirjoitin, 1, TRUE, FALSE, 0, FALSE))
		== NULL) {
		Application->MessageBoxW(L"Kirjoittimen käyttö ei onnistu", L"Virhe", MB_OK);
		return;
		}
	if (lsttulparam.lstf && lsttulparam.lstf->prno == 2)
		initgdiprinter(&lsttulparam);
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

