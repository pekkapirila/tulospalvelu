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

#include "UnitEmiTag.h"
#include "UnitEmit.h"
#include "TpLaitteet.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEmiTag *FormEmiTag;
FILE *emittallfile;
int n_emittallfile;
static HTHREAD hPaivThread, hTiedotThread;
static bool PaivOn;
void emitTarkOn(void);

//---------------------------------------------------------------------------
__fastcall TFormEmiTag::TFormEmiTag(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
	SGLeimat->RowCount = NREGNLY+1;
	SGLeimat->ColCount = 8;
	SGLeimat->Cells[0][0] = L"LaiteNo";
	SGLeimat->Cells[1][0] = L"Laite";
	SGLeimat->Cells[2][0] = L"Yhteys";
	SGLeimat->Cells[3][0] = L"Portti";
	SGLeimat->Cells[4][0] = L"VastaIPosoite";
	SGLeimat->Cells[5][0] = L"VastaPortti";
	SGLeimat->Cells[6][0] = L"Ajanotto";
	SGLeimat->Cells[7][0] = L"Tila";
	SGLeimat->ColWidths[0] = 40 * Screen->PixelsPerInch / 96;
	SGLeimat->ColWidths[1] = 50 * Screen->PixelsPerInch / 96;
	SGLeimat->ColWidths[2] = 45 * Screen->PixelsPerInch / 96;
	SGLeimat->ColWidths[3] = 50 * Screen->PixelsPerInch / 96;
	SGLeimat->ColWidths[4] = 110 * Screen->PixelsPerInch / 96;
	SGLeimat->ColWidths[5] = 60 * Screen->PixelsPerInch / 96;
	SGLeimat->ColWidths[6] = 70 * Screen->PixelsPerInch / 96;
	SGLeimat->ColWidths[7] = 40 * Screen->PixelsPerInch / 96;
	PageControl1->ActivePageIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmiTag::LueEmiTagTiedot(void)
{
	wchar_t was[60];

	r_no = _wtoi(CBAsema->Items->Strings[CBAsema->ItemIndex].c_str()) - 1;
	if (r_no < 0 || regnly[r_no] != LID_EMITAG)
		return;
	EdtVersio->Text = ansitowcs(was, ecdata[r_no].devtype, 39);
	EdtMoodi->Text = ecdata[r_no].mode;
	EdtCode->Text = ecdata[r_no].code;
	EdtAlku->Text = ecdata[r_no].first;
	EdtLoppu->Text = ecdata[r_no].next-1;
	EdtTime->Text = AIKATOWSTRS(was, ecdata[r_no].devtime, t0);
	EdtRcvTime->Text = AIKATOWSTRS(was, ecdata[r_no].pctime, t0);
	EdtBattery->Text = ansitowcs(was, ecdata[r_no].health, 39);
	EdtComm->Text = ansitowcs(was, ecdata[r_no].comm, 39);
}
//---------------------------------------------------------------------------
void __fastcall TFormEmiTag::FormShow(TObject *Sender)
{
	PaivitaYhteydet();
}
//---------------------------------------------------------------------------
void TFormEmiTag::alustaEmiTag(void)
{
	int j = 0;

	CBAsema->Clear();
	for (int i = 0; i < NREGNLY; i++)
		if (regnly[i] == LID_EMITAG) {
			j++;
			CBAsema->Items->Add(i+1);
			}
	if (j == 0) {
		TabSheet2->Enabled = false;
		return;
		}
	TabSheet2->Enabled = true;
	CBAsema->ItemIndex = 0;
	if (j > 1)
		CBAsema->Enabled = true;
	LueEmiTagTiedot();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmiTag::FormClose(TObject *Sender, TCloseAction &Action)
{
	PaivOn = false;
	if (emittallfile) {
		Sleep(200);
		fclose(emittallfile);
		emittallfile = NULL;
		}
	Label12->Visible = false;
	EdtTallennettu->Visible = false;
//	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmiTag::BtnTimeClick(TObject *Sender)
{
	char st[20], as[20], *p;
	sprintf(st, "/SC%8.8s\r\n/ST\r\n", aikatostr_ls(as, t_time_l(biostime(0,0), t0), t0));
	for (p = st; *p; p++) {
		wrt_ch_x(cn_regnly[r_no], *p);
		Sleep(*p == '\n' ? 100 : 5);
		}
	LueEmiTagTiedot();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmiTag::BtnMoodiClick(TObject *Sender)
{
	char st[20], *p;
	int m;

	m = _wtoi(EdtMoodi->Text.c_str());
	if (m != 0) {
		if (Application->MessageBoxW(L"Haluatko varmasti asettaa ohjelman toimivuuden estävän moodin?", L"Vahvista", MB_YESNO)
			!= IDYES)
			return;
		}
	sprintf(st, "/PP%d\r\n/ST\r\n", m);
	for (p = st; *p; p++) {
		wrt_ch_x(cn_regnly[r_no], *p);
		Sleep(*p == '\n' ? 100 : 5);
		}
	LueEmiTagTiedot();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmiTag::BtnCodeClick(TObject *Sender)
{
	char st[20], *p;
	wchar_t msg[200];
	int i;

	i = _wtoi(EdtCode->Text.c_str());
	if (i < 0 || (i > 240 && IntInList(i, L"243 244 248 250 253") < 0)) {
		Application->MessageBoxW(L"Koodi ei sallittu", L"Virhe", MB_OK);
		return;
		}
	if (i == 0)
		wcscpy(msg, L"Lähtönollauksen koodi, toteutetaanko?");
	else if (i < 66)
		swprintf(msg, L"Koodi %d - Alle 66 oleva koodi kieltää tiedonsiirron, toteutetaanko?", i);
	else if (i < 240)
		swprintf(msg, L"Koodi %d - Asema lähettää tietoja, toteutetaanko?", i);
	else if (i < 244)
		swprintf(msg, L"Koodi %d - Tietojen luenta, 5 viim. päivää, toteutetaanko?", i);
	else if (i == 244)
		swprintf(msg, L"Koodi %d - Tietojen luenta, kaikki kortin tiedot, toteutetaanko?", i);
	else if (i == 248)
		wcscpy(msg, L"Maaliajanoton koodi, toteutetaanko?");
	else if (i < 254)
		swprintf(msg, L"Koodi %d - Tietojen luenta, viimeisin kilpailu, toteutetaanko?", i);
	if (Application->MessageBoxW(msg, L"Vahvista", MB_YESNO) == IDYES) {
		sprintf(st, "/CD%03d\r\n/ST\r\n", i);
		for (p = st; *p; p++) {
			wrt_ch_x(cn_regnly[r_no], *p);
			Sleep(*p == '\n' ? 100 : 5);
			}
		}
	LueEmiTagTiedot();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmiTag::Button1Click(TObject *Sender)
{
	char st[40], *p;

	if (Application->MessageBoxW(L"Haluatko varmasti poistaa KAIKKI tiedot laitteesta?", L"Tyhjennys",
		MB_YESNO) == IDYES) {
		if (Application->MessageBoxW(L"V)ahvista, että haluat tyhjentää laitteen muistin", L"Vahvistus",
			MB_YESNO) == IDYES) {
			sprintf(st, "/CL\r\n/ST\r\n");
			for (p = st; *p; p++) {
				wrt_ch_x(cn_regnly[r_no], *p);
				Sleep(*p == '\n' ? 100 : 5);
				}
			}
		}
	LueEmiTagTiedot();
}
//---------------------------------------------------------------------------
MESSAGE void __fastcall TFormEmiTag::PaivHandler(TMyMessage &msg)
{
	if (EdtTallennettu->Visible) {
		EdtTallennettu->Text = n_emittallfile;
		EdtTallennettu->Refresh();
		}
}
//---------------------------------------------------------------------------
static void PaivThread(LPVOID lpCn)
{
	while (FormEmiTag && taustaon && PaivOn) {
		SendMessage(FormEmiTag->Handle, WM_MYPAIVITA, 0, 0);
		Sleep(500);
		}
	hPaivThread = NULL;
	_endthread();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmiTag::Button2Click(TObject *Sender)
{
	char *p, st[30];
	INT i;

	if (RGkohde->ItemIndex == 1 && emittallfile == NULL) {
		SaveDialog1->DefaultExt = "dat";
		SaveDialog1->Filter = "Datatiedostot (*.dat) |*.dat|Kaikki tiedostot (*.*)|*.*";
		if (!SaveDialog1->Execute())
			return;
		emittallfile = _wfopen(SaveDialog1->FileName.c_str(), L"wb");
		n_emittallfile = 0;
		Label12->Visible = true;
		EdtTallennettu->Visible = true;
		}
	if (RGSanomat->ItemIndex == 1 || RGSanomat->ItemIndex == 2) {
		i = _wtoi(EdtFirst->Text.c_str());
		sprintf(st, "/Q%c%d\r\n", RGSanomat->ItemIndex == 1 ? 'F' : 'C', i);
		}
	if (RGSanomat->ItemIndex == 0 || RGSanomat->ItemIndex == 3) {
		sprintf(st, "/Q%c\r\n", RGSanomat->ItemIndex == 0 ? 'M' : 'D');
		}
	for (p = st; *p; p++) {
		wrt_ch_x(cn_regnly[r_no], *p);
		Sleep(*p == '\n' ? 100 : 5);
		}
	LueEmiTagTiedot();
	if (!hPaivThread) {
		PaivOn = true;
		hPaivThread = _beginthread(PaivThread, 10240, 0);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmiTag::Button3Click(TObject *Sender)
{
	LueEmiTagTiedot();
	EdtTallennettu->Text = n_emittallfile;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmiTag::BtnSuljeClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

MESSAGE void __fastcall TFormEmiTag::TiedotHandler(TMyMessage &msg)
{
	LueEmiTagTiedot();
	Refresh();
}
//---------------------------------------------------------------------------

void paivitaEcdata(void)
{
	if (FormEmiTag && FormEmiTag->Visible && FormEmiTag->TiedotOn) {
		SendMessage(FormEmiTag->Handle, WM_MYTIEDOT, 0, 0);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmiTag::CBPaivTiedotClick(TObject *Sender)
{
	TiedotOn = CBPaivTiedot->Checked;
}

void __fastcall TFormEmiTag::FormResize(TObject *Sender)
{
	SGLeimat->Height = Panel2->ClientHeight - SGLeimat->Top;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmiTag::BtnAvaaClick(TObject *Sender)
{
	int y = SGLeimat->Row - 1;
	int ny;
	bool xmlfl = false;

	if (SGLeimat->Cells[0][y+1].Length() == 0 || (ny = SGLeimat->Cells[0][y+1].ToInt() - 1) < 0 || ny >= NREGNLY) {
		Application->MessageBoxW(L"Rivillä ei hyväksyttävää yhteyden numeroa", L"Virhe", MB_OK);
		return;
		}
	if (regnly_open[ny]) {
		Application->MessageBoxW((UnicodeString(L"Laiteyhteys ") + UnicodeString(ny+1) + L" avattuna käyttöön").c_str(),
			L"Este", MB_OK);
		return;
		}
	if (wmemcmpU(SGLeimat->Cells[1][y+1].c_str(), L"LUKIJA", 3) == 0)
		regnly[ny] = LID_LUKIJA;
	else if (wmemcmpU(SGLeimat->Cells[1][y+1].c_str(), L"EMITAG", 3) == 0)
		regnly[ny] = LID_EMITAG;
	else if (wmemcmpU(SGLeimat->Cells[1][y+1].c_str(), L"MTR", 3) == 0)
		regnly[ny] = LID_MTR;
	else {
		Application->MessageBoxW((UnicodeString(L"Laitetyyppi ") + SGLeimat->Cells[1][y+1] + L" ei käytettävissä").c_str(),
			L"Este", MB_OK);
		return;
		}
	if (wmemcmpU(SGLeimat->Cells[2][y+1].c_str(), L"COM", 3) == 0) {
		comtype[cn_regnly[ny]] = comtpRS;
		port_regnly[ny] = _wtoi(SGLeimat->Cells[3][y+1].c_str());
		switch (towupper(SGLeimat->Cells[2][y+1].c_str()[SGLeimat->Cells[2][y+1].Length()-1])) {
			case L'U':
				usb_regnly[ny] = 1;
				break;
			case L'C':
				usb_regnly[ny] = 2;
				break;
			}
		}
	else if (wmemcmpU(SGLeimat->Cells[2][y+1].c_str(), L"TCPS", 4) == 0) {
		comtype[cn_regnly[ny]] = !xmlfl ? comtpTCPSRV : comtpTCPSRVXML;
		ipparam[cn_regnly[ny]].iptype = ipTCPSERVER;
		ipparam[cn_regnly[ny]].srvport = (USHORT) _wtoi(SGLeimat->Cells[3][y+1].c_str());
		}
	else if (wmemcmpU(SGLeimat->Cells[2][y+1].c_str(), L"TCP", 3) == 0) {
		comtype[cn_regnly[ny]] = !xmlfl ? comtpTCP : comtpTCPXML;
		ipparam[cn_regnly[ny]].iptype = ipTCPCLIENT;
		wcsncpy(ipparam[cn_regnly[ny]].destaddr, SGLeimat->Cells[4][y+1].Trim().c_str(), 63);
		ipparam[cn_regnly[ny]].destport = (USHORT) _wtoi(SGLeimat->Cells[5][y+1].c_str());
		}
	else if (wmemcmpU(SGLeimat->Cells[2][y+1].c_str(), L"UDP", 3) == 0) {
		comtype[cn_regnly[ny]] = comtpUDPSTREAM;
		ipparam[cn_regnly[ny]].iptype = ipUDPSTREAM;
		ipparam[cn_regnly[ny]].srvport = (USHORT) _wtoi(SGLeimat->Cells[3][y+1].c_str());
		}
	else {
		Application->MessageBoxW((UnicodeString(L"Yhteyslaji ") + SGLeimat->Cells[2][y+1] + L" ei käytettävissä").c_str(),
			L"Este", MB_OK);
		return;
		}
	if (SGLeimat->Cells[6][y+1].Trim().Length() > 3 &&
		wmemcmpU(SGLeimat->Cells[6][y+1].Trim().c_str(), L"VAIN", 4) == 0) {
		int pst;
		ajanottofl = 1;
		kaikki_ajat[ny+1] = 2;
		if (SGLeimat->Cells[6][y+1].Trim().Length() > 4) {
			switch (towupper(SGLeimat->Cells[6][y+1].Trim().c_str()[4])) {
				case L'A' :
					vainpiste[ny+1] = -2;
					break;
				default :
					pst = 0;
					if (wcswcind(towupper(SGLeimat->Cells[6][y+1].Trim().c_str()[4]), L"0M") >= 0 ||
						(pst = _wtoi(SGLeimat->Cells[6][y+1].Trim().c_str()+4)) > 0) {
						vainpiste[ny+1] = pst;
						}
					break;
				}
			}
		else {
			vainpiste[ny+1] = -3;
			}
		}
	if (SGLeimat->Cells[6][y+1].Trim().Length() > 5 &&
		wmemcmpU(SGLeimat->Cells[6][y+1].Trim().c_str(), L"LÄHDE", 4) == 0) {
		lukijalahde[ny] = _wtoi(SGLeimat->Cells[6][y+1].Trim().c_str()+5);
		}
	if (ajanottofl == -1)
		ajanottofl = 0;
	comclose[cn_regnly[ny]] = 0;
	inittimer();
	Sleep(500);
	PaivitaYhteydet();
}
//---------------------------------------------------------------------------
void TFormEmiTag::PaivitaYhteydet(void)
{
	int y = 0;
	bool onLaite = false;

	for (int r = 1; r < SGLeimat->RowCount-1; r++) {
		for (int c = 0; c < SGLeimat->ColCount-1; c++) {
			SGLeimat->Cells[c][r] = L"";
			}
		}
	for (int ny = 0; ny < NREGNLY; ny++) {
		if (IntInList(regnly[ny], L"12 13 14") >= 0) {
			SGLeimat->Cells[0][y+1] = ny+1;
			onLaite = true;
			switch (regnly[ny]) {
				case 12:
					SGLeimat->Cells[1][y+1] = L"Lukija";
					break;
				case 13:
					SGLeimat->Cells[1][y+1] = L"MTR";
					break;
				case 14:
					SGLeimat->Cells[1][y+1] = L"emiTag";
					break;
				}
			switch (comtype[cn_regnly[ny]]) {
				case comtpRS :
					SGLeimat->Cells[2][y+1] = L"COM";
					switch (usb_regnly[ny]) {
						case 1:
							SGLeimat->Cells[2][y+1] = SGLeimat->Cells[2][y+1] + L"\U";
							break;
						case 2:
							SGLeimat->Cells[2][y+1] = SGLeimat->Cells[2][y+1] + L"\C";
							break;
						}
					SGLeimat->Cells[3][y+1] = port_regnly[ny];
					break;
				case comtpTCPXML :
				case comtpTCP :
					SGLeimat->Cells[2][y+1] = L"TCP";
					SGLeimat->Cells[4][y+1] = ipparam[cn_regnly[ny]].destaddr;
					SGLeimat->Cells[5][y+1] = ipparam[cn_regnly[ny]].destport;
					break;
				case comtpTCPSRVXML :
				case comtpTCPSRV :
					SGLeimat->Cells[2][y+1] = L"TCPS";
					SGLeimat->Cells[3][y+1] = ipparam[cn_regnly[ny]].srvport;
					break;
				case comtpUDPSTREAM :
					SGLeimat->Cells[2][y+1] = L"UDP";
					SGLeimat->Cells[3][y+1] = ipparam[cn_regnly[ny]].srvport;
					break;
				}

			if (lahdepistehaku && lukijalahde[ny]) {
				SGLeimat->Cells[6][y+1] = L"LÄHDE" + UnicodeString(lukijalahde[ny]);
				}
			else if (kaikki_ajat[ny+1] >= 2) {
				switch (vainpiste[ny+1]) {
					case -3:
						SGLeimat->Cells[6][y+1] = L"VAIN";
						break;
					case -2:
						SGLeimat->Cells[6][y+1] = L"VAINA";
						break;
					case -1:
						SGLeimat->Cells[6][y+1] = L"VAINL";
						break;
					default:
						SGLeimat->Cells[6][y+1] = L"VAIN" + UnicodeString(vainpiste[ny+1]);
						break;
					}
				}
			SGLeimat->Cells[7][y+1] = (regnly_open[ny] ? L"On" : L"Ei");
			y++;
			}
		}
	if (onLaite && ajanottofl <= 0) {
		emitTarkOn();
		if (FormEmit) {
			FormEmit->CBLue->Visible = true;
			FormEmit->CBLue->Checked = true;
			}
		}
	alustaEmiTag();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmiTag::SGLeimatMouseDown(TObject *Sender, TMouseButton Button,
		  TShiftState Shift, int X, int Y)
{
	if (Button == mbRight && SGLeimat->Row > 0) {
		switch (SGLeimat->Col) {
			case 1:
				PopupMenuLaite->Popup(X+SGLeimat->ClientOrigin.X, Y+SGLeimat->ClientOrigin.Y);
				break;
			case 2:
				PopupMenuLaiteYht->Popup(X+SGLeimat->ClientOrigin.X, Y+SGLeimat->ClientOrigin.Y);
				break;
			case 6:
				PopupMenuLaiteAjat->Popup(X+SGLeimat->ClientOrigin.X, Y+SGLeimat->ClientOrigin.Y);
				break;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmiTag::LaiteClick(TObject *Sender)
{
	if (Sender == Lukija1)
		SGLeimat->Cells[SGLeimat->Col][SGLeimat->Row] = L"LUKIJA";
	else if (Sender == emiTag1)
		SGLeimat->Cells[SGLeimat->Col][SGLeimat->Row] = L"EMITAG";
	else if (Sender == LaiteMTR1)
		SGLeimat->Cells[SGLeimat->Col][SGLeimat->Row] = L"MTR";
}
//---------------------------------------------------------------------------


void __fastcall TFormEmiTag::LaiteYhtClick(TObject *Sender)
{
	if (Sender == LaiteSarjaportti)
		SGLeimat->Cells[SGLeimat->Col][SGLeimat->Row] = L"COM";
	else if (Sender == LaiteNopeaUSB)
		SGLeimat->Cells[SGLeimat->Col][SGLeimat->Row] = L"COM/U";
	else if (Sender == LaiteTCPasiakas)
		SGLeimat->Cells[SGLeimat->Col][SGLeimat->Row] = L"TCP";
	else if (Sender == LaiteTCPpalvelin)
		SGLeimat->Cells[SGLeimat->Col][SGLeimat->Row] = L"TCPS";
	else if (Sender == LaiteUDP)
		SGLeimat->Cells[SGLeimat->Col][SGLeimat->Row] = L"UDP";
}
//---------------------------------------------------------------------------

void __fastcall TFormEmiTag::LaiteAjatClick(TObject *Sender)
{
	if (Sender == Eiaikoja1)
		SGLeimat->Cells[SGLeimat->Col][SGLeimat->Row] = L"";
	else if (Sender == Maali1)
		SGLeimat->Cells[SGLeimat->Col][SGLeimat->Row] = L"VAIN0";
	else if (Sender == Valiaika1)
		SGLeimat->Cells[SGLeimat->Col][SGLeimat->Row] = L"VAIN1";
	else if (Sender == Valiaika2)
		SGLeimat->Cells[SGLeimat->Col][SGLeimat->Row] = L"VAIN2";
	else if (Sender == SjBox1)
		SGLeimat->Cells[SGLeimat->Col][SGLeimat->Row] = L"VAIN";
	else if (Sender == LaiteVuorot)
		SGLeimat->Cells[SGLeimat->Col][SGLeimat->Row] = L"VAINA";
}
//---------------------------------------------------------------------------

void __fastcall TFormEmiTag::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"emiTag");
}
//---------------------------------------------------------------------------

void __fastcall TFormEmiTag::BtnSuljeYhteysClick(TObject *Sender)
{
	int ny;

	if (SGLeimat->Cells[0][SGLeimat->Row].Length() == 0 ||
		(ny = SGLeimat->Cells[0][SGLeimat->Row].ToInt() - 1) < 0 || ny >= NREGNLY) {
		Application->MessageBoxW(L"Rivillä ei hyväksyttävää yhteyden numeroa", L"Virhe", MB_OK);
		return;
		}
/*
	if (!regnly_open[ny]) {
		Application->MessageBoxW((UnicodeString(L"Laiteyhteys ") + UnicodeString(ny+1) + L" ei ole avoinna").c_str(),
			L"Este", MB_OK);
		return;
		}
*/
	if (IntInList(regnly[ny], L"12 13 14") < 0) {
		Application->MessageBoxW((UnicodeString(L"Yhteyden ") + UnicodeString(ny+1) + L" laite ei ole lukija, emiTag-laite tai MTR").c_str(),
			L"Este", MB_OK);
		return;
		}
	katkaisuPyydetty = true;
	comclose[cn_regnly[ny]] = 1;
	rem_regnly(ny);
	katkaisuPyydetty = false;
	PaivitaYhteydet();
}
//---------------------------------------------------------------------------

