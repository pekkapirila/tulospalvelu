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

#include "UnitMTR.h"
#include <process.h>
#include <time.h>
#include "TpLaitteet.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMTR *FormMTR;

#define MTR_ST_LEN 59
extern int in_aseta_mtr;
extern INT r_msg_len[NREGNLY];
extern INT mtr_yksi, lue_rno;
extern UINT32 pkg_alku[NREGNLY], pkg_loppu[NREGNLY];
extern UINT32 mtr_pyynto[NREGNLY];
INT16 r_tarksum(char *buf, INT len, char sum, INT16 xfl);
static void PaivThread(LPVOID lpCn);
extern FILE *emittallfile;
extern int n_emittallfile;
static HTHREAD hPaivThread;
static bool PaivOn;
#ifdef DBGFILE
extern FILE *dbgfile[NREGNLY];
#endif
//---------------------------------------------------------------------------
__fastcall TFormMTR::TFormMTR(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormMTR::LueMTRTiedot(void)
{
	wchar_t ch = L' ', msg[120], ist[20], as[20];
	char san[40];
	INT i, j, nq, nch;
	INT32 ed_alku, luku, c_time;
	INT32 e_time;
	time_t et_time;
	san_type vast1;
	struct tm *now;

	r_no = _wtoi(CBAsema->Items->Strings[CBAsema->ItemIndex].c_str()) - 1;
	if (r_no < 0 || regnly[r_no] != LID_MTR)
		return;

	in_aseta_mtr = r_no + 1;
	i_flush_x(cn_regnly[r_no]);
	wrt_st_x(cn_regnly[r_no],3,"/ST",&nch);
	c_time = t_time_l(biostime(0,0L), t0);
	for (i = 0; i < 5; i++) {
		Sleep(50);
		quesize_x(cn_regnly[r_no], &nq);
		if (nq >= MTR_ST_LEN)
			break;
#ifdef DBGFILE
		if (dbgfile[r_no]) {
			if (fwprintf(dbgfile[r_no], L"Uusintapyynto 1/%d\n\r", i+1) < 10)
			dbgfile[r_no] = 0;
			}
#endif
		}
	read_st_x(cn_regnly[r_no],MTR_ST_LEN,vastaus.bytes,&nch,&nq);
#ifdef DBGFILE
	if (dbgfile[r_no]) {
		vastaus.bytes[nch] = 0;
		if (fputs(vastaus.bytes,dbgfile[r_no]) < 0)
		dbgfile[r_no] = 0;
		}
#endif
	i_flush_x(cn_regnly[r_no]);
	if (nch < MTR_ST_LEN || vastaus.r13st.alku != 0xffffffffL ||
		vastaus.r13st.msglen != MTR_ST_LEN-4 || vastaus.r13st.tunnus != 'S' ||
		r_tarksum(vastaus.bytes, MTR_ST_LEN-2, vastaus.r13st.chksum2, 0)) {
		writeerror_w(L"MTR:n tilatietojen lukeminen epäonnistui", 0, true);
		in_aseta_mtr = 0;
		return;
		}
	e_time = vastaus.r13st.t_ms/100 + 10L*vastaus.r13st.t_sek +
		600L*vastaus.r13st.t_min + 36000L*vastaus.r13st.t_hour + t0_regn[r_no]/KSEK;
	EdtId->Text = vastaus.r13st.mtrid;
	swprintf(msg, L"%02d.%02d.%02d  %s",
		vastaus.r13st.t_day, vastaus.r13st.t_month, vastaus.r13st.t_year,
		aikatowstr_ls(as, e_time, t0));
	EdtKello->Text = msg;
	e_time = (e_time - c_time)/10;
	EdtEro->Text = e_time;
	wcscpy(msg, vastaus.r13st.battery ? L"HEIKKO" : L"OK");
	EdtParisto->Text = msg;
	if (!vastaus.r13st.pkgno)
		Application->MessageBoxW(L"Laitteessa ei tietoja", L"Tyhjä laite", MB_OK);
	else {
		SG1->ColCount = 5;
		SG1->ColCount = 10;
		SG1->ColWidths[0] = 55 * Screen->PixelsPerInch / 96;
		SG1->ColWidths[1] = 120 * Screen->PixelsPerInch / 96;
		SG1->ColWidths[2] = 55 * Screen->PixelsPerInch / 96;
		SG1->ColWidths[3] = 55 * Screen->PixelsPerInch / 96;
		SG1->ColWidths[4] = 55 * Screen->PixelsPerInch / 96;
		SG1->Cells[0][0] = L"Istunto";
		SG1->Cells[1][0] = L"Aloituspvm ja -aika";
		SG1->Cells[2][0] = L"Vanhin nro";
		SG1->Cells[3][0] = L"Uusin nro";
		SG1->Cells[4][0] = L"Lukumäärä";
		SG1->RowCount = 21;
		ed_alku = vastaus.r13st.pkgno + 1;
		for (i = 0; i < 20; i++) {
			if (i == 0)
				wcscpy(ist, L"Viimeisin");
			else
				swprintf(ist, L"-%d", i);
//			if (vastaus.r13st.s_start[i] >= vastaus.r13st.oldestpkgno &&
			if (vastaus.r13st.s_start[i] <= vastaus.r13st.pkgno) {
				Sleep(30);
				wrt_st_x(cn_regnly[r_no],3,"/GB",&nch);
				wrt_st_x(cn_regnly[r_no],4,(char *)(vastaus.r13st.s_start+i),
					&nch);
				for (j = 0; j < 10; j++) {
					Sleep(30);
					quesize_x(cn_regnly[r_no], &nq);
					if (nq >= r_msg_len[r_no])
						break;
#ifdef DBGFILE
					if (dbgfile[r_no]) {
						if (fwprintf(dbgfile[r_no], L"Uusintapyynto %d/%d\n\r", i+1, j+1) < 10)
							dbgfile[r_no] = 0;
						}
#endif
					}
				read_st_x(cn_regnly[r_no],r_msg_len[r_no],vast1.bytes,&nch,&nq);
#ifdef DBGFILE
				if (dbgfile[r_no]) {
					vast1.bytes[nch] = 0;
					if (fputs(vast1.bytes,dbgfile[r_no]) < 0)
						dbgfile[r_no] = 0;
					}
#endif
				i_flush_x(cn_regnly[r_no]);
				SG1->Cells[0][i+1] = ist;
				if (nch < r_msg_len[r_no] || vast1.r13.alku != 0xffffffffL ||
					vast1.r13.msglen != r_msg_len[r_no]-4 || vast1.r13.tunnus != 'M' ||
					r_tarksum(vast1.bytes, r_msg_len[r_no]-2, vast1.r13.chksum2, 0)) {
					SG1->Cells[1][i+1] = L"Lukeminen epäonnistui";
					}
				else {
					luku = ed_alku - vastaus.r13st.s_start[i];
					swprintf(msg, L"%2.2d.%2.2d.%2.2d %2.2d.%2.2d.%2.2d",
						vast1.r13.t_day, vast1.r13.t_month, vast1.r13.t_year,
						vast1.r13.t_hour, vast1.r13.t_min, vast1.r13.t_sek);
					SG1->Cells[1][i+1] = msg;
					SG1->Cells[2][i+1] = vastaus.r13st.s_start[i];
					SG1->Cells[3][i+1] = ed_alku-1;
					SG1->Cells[4][i+1] = luku;
					}
				ed_alku = vastaus.r13st.s_start[i];
				}
			else {
				SG1->Cells[0][i+1] = ist;
				SG1->Cells[1][i+1] = L"Ei tietoja";
				}
			}
		SG1->Cells[0][i+1] = L"Kaikkiaan";
		SG1->Cells[2][i+1] = vastaus.r13st.oldestpkgno;
		SG1->Cells[3][i+1] = vastaus.r13st.pkgno;
		SG1->Cells[4][i+1] = vastaus.r13st.pkgno - vastaus.r13st.oldestpkgno + 1;
		}
}

void __fastcall TFormMTR::FormShow(TObject *Sender)
{
	int j = 0;

	CBAsema->Clear();
   for (int i = 0; i < NREGNLY; i++)
	  if (regnly[i] == LID_MTR) {
		 j++;
		 CBAsema->Items->Add(i+1);
	  }
   if (j == 0) {
		Application->MessageBoxW(L"Ei MTR-laitetta", L"Virhe", MB_OK);
		return;
		}
   CBAsema->ItemIndex = 0;
   if (j > 1)
	   CBAsema->Enabled = true;
	LueMTRTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormMTR::Button1Click(TObject *Sender)
{
	char san[40];
	INT nch;

	if (RGkohde->ItemIndex < 0) {
		Application->MessageBoxW(L"Valitse lukemisen kohde", L"Este", MB_OK);
		return;
		}
	if (RGkohde->ItemIndex == 1 && emittallfile == NULL) {
		SaveDialog1->DefaultExt = "dat";
		SaveDialog1->Filter = "Datatiedostot (*.dat) |*.dat|Kaikki tiedostot (*.*)|*.*";
		if (!SaveDialog1->Execute())
			return;
		emittallfile = _wfopen(SaveDialog1->FileName.c_str(), L"wb");
		n_emittallfile = 0;
		}
	mtr_yksi = RGToisto->ItemIndex < 1;
	lue_rno = r_no;
	switch (RGRajaus->ItemIndex) {
		case 0:
			if (CBIstunto->ItemIndex == 0)
				pkg_loppu[r_no] = vastaus.r13st.pkgno;
			else
				pkg_loppu[r_no] = vastaus.r13st.s_start[CBIstunto->ItemIndex-1] - 1;
			pkg_alku[r_no] = vastaus.r13st.s_start[CBIstunto->ItemIndex];
			break;
		case 1:
			pkg_loppu[r_no] = vastaus.r13st.pkgno;
			pkg_alku[r_no] = _wtoi(EdtLkm->Text.c_str());
			pkg_alku[r_no] = pkg_loppu[r_no] - pkg_alku[r_no] + 1;
			break;
		case 2:
			pkg_alku[r_no] = _wtoi(EdtEns->Text.c_str());
			pkg_loppu[r_no] = _wtoi(EdtViim->Text.c_str());
			break;
		}
	if (mtr_yksi) {
		in_aseta_mtr = 0;
		Sleep(2000);
		in_aseta_mtr = r_no + 1;
		}
	else if (RGkohde->ItemIndex == 1) {
		Label12->Visible = true;
		EdtTallennettu->Visible = true;
		Application->MessageBoxW(L"Odota luennan päättymistä ennen kuin suljet kaavakkeen", L"Ohje", MB_OK);
		if (!hPaivThread) {
			PaivOn = true;
			hPaivThread = _beginthread(PaivThread, 10240, 0);
			}
		}
	else
		Application->MessageBoxW(L"Luenta käynnistyy, kun kaavake suljetaan", L"Ohje", MB_OK);
	if (pkg_alku[r_no] <= pkg_loppu[r_no]) {
//		if (pkg_alku[r_no] < vastaus.r13st.oldestpkgno)
//			pkg_alku[r_no] = vastaus.r13st.oldestpkgno;
		if (pkg_loppu[r_no] > vastaus.r13st.pkgno)
			pkg_loppu[r_no] = vastaus.r13st.pkgno;
		mtr_pyynto[r_no] = pkg_loppu[r_no];
		memcpy(san, "/GB", 3);
		*(INT32 *)(san+3) = pkg_alku[r_no];
		wrt_st_x(cn_regnly[r_no],7,san,&nch);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMTR::Button2Click(TObject *Sender)
{
	char san[40];
	INT nch;
	time_t et_time;
	struct tm *now;

	memcpy(san, "/SC", 3);
	time(&et_time);
	now = localtime(&et_time);
	san[3] = now->tm_year % 100;
	san[4] = now->tm_mon+1;
	san[5] = now->tm_mday;
	san[6] = now->tm_hour;
	san[7] = now->tm_min;
	san[8] = now->tm_sec;
	wrt_st_x(cn_regnly[r_no],9,san,&nch);
	LueMTRTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormMTR::Button3Click(TObject *Sender)
{
	char san[40];
	INT nch;

	if (Application->MessageBoxW(L"Haluatko varmasti sulkea istunnon (vain MTR3)?", L"Istunto",
		MB_YESNO) == IDYES) {
		memcpy(san, "/NS", 3);
		wrt_st_x(cn_regnly[r_no],3,san,&nch);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMTR::Button4Click(TObject *Sender)
{
	char san[40];
	INT nch;

	if (Application->MessageBoxW(L"Haluatko varmasti poistaa KAIKKI tiedot MTR-laitteesta?", L"Tyhjennys",
		MB_YESNO) == IDYES) {
		if (Application->MessageBoxW(L"V)ahvista, että haluat tyhjentää MTR-laitteen muistin", L"Vahvistus",
			MB_YESNO) == IDYES) {
			memcpy(san, L"/CL", 3);
			wrt_st_x(cn_regnly[r_no],3,san,&nch);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMTR::FormClose(TObject *Sender, TCloseAction &Action)
{
	PaivOn = false;
	if (emittallfile) {
		Sleep(200);
		fclose(emittallfile);
		emittallfile = NULL;
		}
	Label12->Visible = false;
	EdtTallennettu->Visible = false;
	in_aseta_mtr = 0;
//	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TFormMTR::RGRajausClick(TObject *Sender)
{
	switch (RGRajaus->ItemIndex) {
		case 0:
			CBIstunto->Enabled = true;
			EdtLkm->Enabled = false;
			EdtEns->Enabled = false;
			EdtViim->Enabled = false;
			break;
		case 1:
			CBIstunto->Enabled = false;
			EdtLkm->Enabled = true;
			EdtEns->Enabled = false;
			EdtViim->Enabled = false;
			break;
		case 2:
			CBIstunto->Enabled = false;
			EdtLkm->Enabled = false;
			EdtEns->Enabled = true;
			EdtViim->Enabled = true;
			break;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMTR::RGToistoClick(TObject *Sender)
{
	BtnSeur->Enabled = RGToisto->ItemIndex < 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormMTR::BtnSeurClick(TObject *Sender)
{
//	seur_mtr();
	in_aseta_mtr = 0;
	Sleep(2000);
	in_aseta_mtr = r_no + 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormMTR::BtnSuljeClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
MESSAGE void __fastcall TFormMTR::PaivHandler(TMyMessage &msg)
{
	if (EdtTallennettu->Visible) {
		EdtTallennettu->Text = n_emittallfile;
		EdtTallennettu->Refresh();
		}
}
//---------------------------------------------------------------------------
static void PaivThread(LPVOID lpCn)
{
	in_aseta_mtr = 0;
	while (FormMTR && taustaon && PaivOn) {
		SendMessage(FormMTR->Handle, WM_MYPAIVITA, 0, 0);
		Sleep(500);
		}
	_endthread();
	in_aseta_mtr = FormMTR->r_no + 1;
}
//---------------------------------------------------------------------------

