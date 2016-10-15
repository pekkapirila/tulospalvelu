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

#include "UnitComfile.h"
#include "WinHk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormComfile *FormComfile;

void lahetaKopiot(int cn, combufrec obuf);
void SendCopyThread(LPVOID lpCn);
void init_class_len(void);
extern int class_len[];
extern int  combufsize;
extern sendtestparamtp sendtestparam;

//---------------------------------------------------------------------------
__fastcall TFormComfile::TFormComfile(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::FormCreate(TObject *Sender)
{
	if (sendtestparam.ComFile) {
		ComFile = sendtestparam.ComFile;
		LahComFile = sendtestparam.ComFile;
		Button1->Enabled = false;
		BtnLaheta->Visible = true;
		Jono = sendtestparam.Jono;
		NJRec[Jono] = sendtestparam.viim;
		BtnLahetaClick(Sender);
		}
	else
		ActComfile = FormMain->CurrentDir+UnicodeString(L"\\comfile") + UnicodeString(k_pv+1) + L".dat";
}
//---------------------------------------------------------------------------

TestausIkkParamClass::TestausIkkParamClass(void)
{
	memset(this, 0, sizeof(TestausIkkParamClass));
}
//---------------------------------------------------------------------------

void TestausIkkParamClass::operator=(TestausIkkParamClass& ikkPar)
{
	memcpy(this, &ikkPar, sizeof(TestausIkkParamClass));
}
//---------------------------------------------------------------------------

void TFormComfile::haeParams(void)
{
	IkkParam.ValiLaji = RGValit->ItemIndex;
	if (IkkParam.ValiLaji == 1)
		IkkParam.VertAika = wstrtoaika_vap(EdtAloitus->Text.c_str(), t0);
	else
		IkkParam.VertAika = TMAALI0;
	IkkParam.LahJono = RGAloitus->ItemIndex + 1;
	IkkParam.LahYhteys = CBLahYhteys->ItemIndex;
	wcsncpy(IkkParam.Tiedosto, 	ActComfile.c_str(), sizeof(IkkParam.Tiedosto)/2-1);
	IkkParam.TiedJono = Jono + 1;
}
//---------------------------------------------------------------------------

int TFormComfile::applyParams(void)
{
	wchar_t st[20];

	RGValit->ItemIndex = IkkParam.ValiLaji;
	if (IkkParam.ValiLaji == 1)
		EdtAloitus->Text = aikatowstr_cols_n(st, IkkParam.VertAika, t0, 0, 8);
	RGAloitus->ItemIndex = IkkParam.LahJono - 1;
	ActComfile = IkkParam.Tiedosto;
	if (ActComfile.Length() > 3)
		ComFile = avaacomfile(IkkParam.Tiedosto);
	if (ComFile) {
		Jono = IkkParam.TiedJono - 1;
		lataaJono(Jono);
		BtnLahetaClick(this);
		CBLahYhteys->ItemIndex = IkkParam.LahYhteys;
		}
	return(0);
}
//---------------------------------------------------------------------------

FILE *TFormComfile::avaacomfile(wchar_t *obfilename)
	{
	int i, j;
	fpos_t len;
	wchar_t ch, msg[90];
	char st[200];
	combufrec obfbuf[2];
	int maxyht;
	FILE *obfile;

	init_class_len();
	NJono = 0;
	NRec = 0;
	if (CBSalliMuokkaus->Checked) {
		obfile = _wfopen(obfilename, L"r+b");
		BtnTallMuutos->Visible = true;
		SGSanoma->Options << goEditing;
		}
	else {
		CBSalliMuokkaus->Visible = false;
		obfile = _wfopen(obfilename, L"rb");
		BtnTallMuutos->Visible = false;
		SGSanoma->Options >> goEditing;
		}
	if (obfile == NULL)
		Application->MessageBoxW(L"Tiedoston avaaminen ei onnistu", L"", MB_OK);
	else {
		fseek(obfile, 0, SEEK_END);
		fgetpos(obfile, &len);
		if (len < combufsize) {
			Application->MessageBoxW(L"Tiedosto tyhjä tai liian lyhyt", L"", MB_OK);
			fclose(obfile);
			return(0);
			}
		rewind(obfile);
		fread(obfbuf, combufsize, 1, obfile);
		combufsize = obfbuf[0].len;
		NJono = obfbuf[0].so;
		NRec = len / (NJono*combufsize);
		if (len != NRec * NJono * combufsize) {
			swprintf(msg, L"Tiedoston pituus %d ei odotettu moninkerta. Tietue %d, jonoja %d, tietueita %d",
				len, combufsize, NJono, NRec);
			Application->MessageBoxW(msg, L"", MB_OK);
			fclose(obfile);
			return(0);
			}
		}
	RecLen = combufsize;
	return(obfile);
	}

//---------------------------------------------------------------------------

void __fastcall TFormComfile::FormShow(TObject *Sender)
{
	FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::JonoPituudet(void)
{
	combufrec combuf;
	memset(NJRec, 0, sizeof(NJRec));
	memset(NJLah, 0, sizeof(NJLah));
	fseek(ComFile, NJono*RecLen, SEEK_SET);
	for (int i = 0; i < NJono*NRec; i++) {
		if (fread(&combuf, RecLen, 1, ComFile) != 1)
			break;
		if (combuf.len > 0 && combuf.len == class_len[combuf.pkgclass]) {
			NJRec[i % NJono]++;
			if (combuf.lahetetty != 0)
				NJLah[i % NJono]++;
			}
		}
	rewind(ComFile);
	SGJonot->RowCount = NJono+1;
	SGJonot->Cells[0][0] = L"Jono";
	SGJonot->Cells[1][0] = L"Lkm";
	SGJonot->Cells[2][0] = L"Lah";
	for (int r = 1; r <= NJono; r++) {
		SGJonot->Cells[0][r] = UnicodeString(r);
		SGJonot->Cells[1][r] = UnicodeString(NJRec[r-1]);
		SGJonot->Cells[2][r] = UnicodeString(NJLah[r-1]);
		}
}

//---------------------------------------------------------------------------

void __fastcall TFormComfile::Button1Click(TObject *Sender)
{
	OpenDialog1->InitialDir = FormMain->CurrentDir;
	OpenDialog1->FileName = UnicodeString(L"comfile.dat");
	OpenDialog1->DefaultExt = L"dat";
	OpenDialog1->Filter = L"DAT-tiedostot (*.dat)|*.DAT|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		if (LahComFile == ComFile) {
			fclose(ComFile);
			ComFile = NULL;
			LahComFile = NULL;
			sendtestparam.ComFile = NULL;
			}
		else {
			fclose(ComFile);
			ComFile = NULL;
			}
		ComFile = avaacomfile(OpenDialog1->FileName.c_str());
		if (ComFile && OpenDialog1->FileName != ActComfile)
			BtnLaheta->Visible = true;
		}
	EJonoja->Text = UnicodeString(NJono);
	ETietuekoko->Text = UnicodeString(RecLen);
	ESanomia->Text = UnicodeString(NRec-1);
	if (ComFile)
		JonoPituudet();
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::FormClose(TObject *Sender, TCloseAction &Action)
{
	if (ComFile) {
		fclose(ComFile);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::FormResize(TObject *Sender)
{
	SGJono->Height = ClientHeight - SGJono->Top - 4;
	if (GBLahetys->Visible) {
		GBLahetys->Top = ClientHeight - GBLahetys->Height - 4;
		SGJonot->Height = GBLahetys->Top - SGJonot->Top - 4;
		}
	else
		SGJonot->Height = ClientHeight - SGJonot->Top - 4;
	SGJono->Width = ClientWidth - SGJono->Left - 234 * Screen->PixelsPerInch / 96;
	SGSanoma->Height = ClientHeight - SGSanoma->Top - 4;
	SGSanoma->Left = SGJono->Left + SGJono->Width + 6;
}
//---------------------------------------------------------------------------
void __fastcall TFormComfile::naytaSanoma(int r)
{
	int tl, d;
	wchar_t st[100];
	kilptietue kilp, lkilp;
	kilppvtp pv;

	SGSanoma->ColWidths[0] = 70 * Screen->PixelsPerInch / 96;
	SGSanoma->ColWidths[1] = 150 * Screen->PixelsPerInch / 96;
	SGSanoma->Cells[0][0] = L"Tieto";
	SGSanoma->Cells[1][0] = L"Arvo";
	if (r <= 0 || r > NJRec[Jono]) {
		SGSanoma->RowCount = 2;
		return;
		}
	fseek(ComFile, (r * NJono + Jono) * RecLen, SEEK_SET);
	if (fread(&ComBuf, RecLen, 1, ComFile) != 1) {
		SGSanoma->RowCount = 2;
		return;
		}
	tl = ComBuf.lahetetty * KSEK;
	if (ComBuf.pkgclass == 1) {
		kilp.unpack0(ComBuf.d.k.ckilp);
		SGSanoma->RowCount = 12;
		SGSanoma->Cells[0][1] = L"Sanomalaji";
		SGSanoma->Cells[0][2] = L"Status";
		SGSanoma->Cells[0][3] = L"Pos";
		SGSanoma->Cells[0][4] = L"EntNo";
		SGSanoma->Cells[0][5] = L"Lähetetty";
		SGSanoma->Cells[0][6] = L"KilpNo";
		SGSanoma->Cells[0][7] = L"Sukunimi";
		SGSanoma->Cells[0][8] = L"Etunimi";
		SGSanoma->Cells[0][9] = L"Sarja";
		SGSanoma->Cells[0][10] = L"Seura";
		SGSanoma->Cells[0][11] = L"Seuralyh";
		SGSanoma->Cells[1][1] = L"Perustiedot";
		SGSanoma->Cells[1][2] = UnicodeString(kilp.kilpstatus);
		SGSanoma->Cells[1][3] = UnicodeString(ComBuf.d.k.dk);
		SGSanoma->Cells[1][4] = UnicodeString(ComBuf.d.k.entno);
		SGSanoma->Cells[1][5] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
		SGSanoma->Cells[1][6] = UnicodeString(kilp.kilpno);
		SGSanoma->Cells[1][7] = UnicodeString(kilp.sukunimi);
		SGSanoma->Cells[1][8] = UnicodeString(kilp.etunimi);
		SGSanoma->Cells[1][9] = UnicodeString(kilp.Sarja());
		SGSanoma->Cells[1][10] = UnicodeString(kilp.seura);
		SGSanoma->Cells[1][11] = UnicodeString(kilp.seuralyh);
		}
	if (ComBuf.pkgclass == 2) {
		if (ComBuf.d.pv.valuku == kilpparam.valuku) {
			pv.unpack(ComBuf.d.pv.cpv);
			}
		else {
			pv.unpack0(ComBuf.d.pv.cpv);
			}
		SGSanoma->RowCount = 12;
		SGSanoma->Cells[0][1] = L"Sanomalaji";
		SGSanoma->Cells[0][2] = L"Pos";
		SGSanoma->Cells[0][3] = L"Lähetetty";
		SGSanoma->Cells[0][4] = L"Vaihe";
		SGSanoma->Cells[0][5] = L"Sarja";
		SGSanoma->Cells[0][6] = L"Bib";
		SGSanoma->Cells[0][7] = L"Badge";
		SGSanoma->Cells[0][8] = L"Lähtö";
		SGSanoma->Cells[0][9] = L"Tark";
		SGSanoma->Cells[0][10] = L"Sakko";
		SGSanoma->Cells[0][11] = L"Ammunta";
		SGSanoma->Cells[1][1] = L"Vaihetiedot";
		SGSanoma->Cells[1][2] = UnicodeString(ComBuf.d.k.dk);
		SGSanoma->Cells[1][3] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
		SGSanoma->Cells[1][4] = UnicodeString(ComBuf.d.pv.pv + 1);
		SGSanoma->Cells[1][5] = UnicodeString(pv.sarja);
		SGSanoma->Cells[1][6] = UnicodeString(pv.bib);
		SGSanoma->Cells[1][7] = UnicodeString(pv.badge[0])+" / "+UnicodeString(pv.badge[1]);
		SGSanoma->Cells[1][8] = UnicodeString(aikatowstr_cols_n(st, pv.tlahto, t0, 0, 11));
		SGSanoma->Cells[1][9] = UnicodeString(pv.keskhyl);
		SGSanoma->Cells[1][10] = UnicodeString(pv.tasoitus);
		SGSanoma->Cells[1][11] = UnicodeString(pv.asakot);
		}
	if (ComBuf.pkgclass == 3) {
		SGSanoma->RowCount = 8;
		SGSanoma->Cells[0][1] = L"Sanomalaji";
		SGSanoma->Cells[0][2] = L"Pos";
		SGSanoma->Cells[0][3] = L"Lähetetty";
		SGSanoma->Cells[0][4] = L"Bib";
		SGSanoma->Cells[0][5] = L"Vaihe";
		SGSanoma->Cells[0][6] = L"Piste";
		SGSanoma->Cells[0][7] = L"Aika/Tark";
		SGSanoma->Cells[1][1] = L"Tulostieto";
		SGSanoma->Cells[1][2] = UnicodeString(ComBuf.d.k.dk);
		SGSanoma->Cells[1][3] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
		SGSanoma->Cells[1][4] = UnicodeString(ComBuf.d.v.bib);
		SGSanoma->Cells[1][5] = UnicodeString(ComBuf.d.v.k_pv + 1);
		SGSanoma->Cells[1][6] = UnicodeString(ComBuf.d.v.vali);
		SGSanoma->Cells[1][7] = SGJono->Cells[10][r];
		}
	if (ComBuf.pkgclass == 4) {
		SGSanoma->RowCount = 15;
		SGSanoma->Cells[0][1] = L"Sanomalaji";
		SGSanoma->Cells[0][2] = L"Lähetetty";
		SGSanoma->Cells[0][3] = L"Aika";
		SGSanoma->Cells[0][4] = L"Bib";
		SGSanoma->Cells[0][5] = L"Piste";
		SGSanoma->Cells[0][6] = L"Badge";
		SGSanoma->Cells[0][7] = L"Kanava";
		SGSanoma->Cells[0][8] = L"Jono";
		SGSanoma->Cells[0][9] = L"Ed. aika";
		SGSanoma->Cells[0][10] = L"Ed. bib";
		SGSanoma->Cells[0][11] = L"Ed. piste";
		SGSanoma->Cells[0][12] = L"Ed. badge";
		SGSanoma->Cells[0][13] = L"Ed. kanava";
		SGSanoma->Cells[0][14] = L"Ed. jono";
		SGSanoma->Cells[1][1] = L"Ajanottotieto";
		SGSanoma->Cells[1][2] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
		SGSanoma->Cells[1][3] = UnicodeString(aikatowstr_cols_n(st, ComBuf.d.a.iaika.t/AIKAJAK, t0, 0, 11));
		SGSanoma->Cells[1][4] = UnicodeString(ComBuf.d.a.iaika.kno);
		SGSanoma->Cells[1][5] = UnicodeString(ComBuf.d.a.iaika.piste);
		SGSanoma->Cells[1][6] = UnicodeString(ComBuf.d.a.iaika.badge);
		SGSanoma->Cells[1][7] = UnicodeString(ComBuf.d.a.iaika.kanava);
		SGSanoma->Cells[1][8] = UnicodeString(ComBuf.d.a.iaika.jono);
		SGSanoma->Cells[1][9] = UnicodeString(aikatowstr_cols_n(st, ComBuf.d.a.daika.t/AIKAJAK, t0, 0, 11));
		SGSanoma->Cells[1][10] = UnicodeString(ComBuf.d.a.daika.kno);
		SGSanoma->Cells[1][11] = UnicodeString(ComBuf.d.a.daika.piste);
		SGSanoma->Cells[1][12] = UnicodeString(ComBuf.d.a.daika.badge);
		SGSanoma->Cells[1][13] = UnicodeString(ComBuf.d.a.daika.kanava);
		SGSanoma->Cells[1][14] = UnicodeString(ComBuf.d.a.daika.jono);
		}
	if (ComBuf.pkgclass == 5) {
		}
	if (ComBuf.pkgclass == 6) {
		SGSanoma->RowCount = 58;
		SGSanoma->Cells[0][1] = L"Sanomalaji";
		SGSanoma->Cells[0][2] = L"Lähetetty";
		SGSanoma->Cells[0][3] = L"Badge";
		SGSanoma->Cells[0][4] = L"Bib";
		SGSanoma->Cells[0][5] = L"Maali";
		SGSanoma->Cells[0][6] = L"Luettu";
		SGSanoma->Cells[0][7] = L"Kirjattu";
		SGSanoma->Cells[1][1] = L"Emit-tieto";
		SGSanoma->Cells[1][2] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
		SGSanoma->Cells[1][3] = UnicodeString(ComBuf.d.em.badge)+L" (orig:"+UnicodeString(ComBuf.d.em.badge0)+L")";
		SGSanoma->Cells[1][4] = UnicodeString(ComBuf.d.em.kilpno);
		SGSanoma->Cells[1][5] = UnicodeString(aikatowstr_cols_n(st, KSEK*ComBuf.d.em.maali, t0, 0, 11));
		SGSanoma->Cells[1][6] = UnicodeString(aikatowstr_cols_n(st, KSEK*ComBuf.d.em.time, t0, 0, 11));
		SGSanoma->Cells[1][7] = UnicodeString(aikatowstr_cols_n(st, KSEK*ComBuf.d.em.kirjaus, t0, 0, 11));
		for (int rr = 0; rr < MAXNLEIMA; rr++) {
			SGSanoma->Cells[0][8+rr] = UnicodeString(rr+1)+L". leima";
			SGSanoma->Cells[1][8+rr] = UnicodeString((int)(ComBuf.d.em.ctrlcode[rr])) +
				L": "+ UnicodeString(ComBuf.d.em.ctrltime[rr]);
			}
		}
	if (ComBuf.pkgclass == 7) {
		SGSanoma->RowCount = 6;
		SGSanoma->Cells[0][1] = L"Sanomalaji";
		SGSanoma->Cells[0][2] = L"Seura";
		SGSanoma->Cells[0][3] = L"Seuralyh";
		SGSanoma->Cells[0][4] = L"Maa";
		SGSanoma->Cells[0][5] = L"Piiri";
		SGSanoma->Cells[1][1] = L"Seuratieto";
		SGSanoma->Cells[1][2] = UnicodeString(ComBuf.d.sra.snimi);
		SGSanoma->Cells[1][3] = UnicodeString(ComBuf.d.sra.lyhenne);
		SGSanoma->Cells[1][4] = UnicodeString(ComBuf.d.sra.maa);
		SGSanoma->Cells[1][5] = UnicodeString(ComBuf.d.sra.piiri);
		}
	if (ComBuf.pkgclass == 13) {
		SGSanoma->RowCount = 10 + ComBuf.d.emva.rastiluku;
		SGSanoma->Cells[0][1] = L"Sanomalaji";
		SGSanoma->Cells[0][2] = L"Lähetetty";
		SGSanoma->Cells[0][3] = L"Badge";
		SGSanoma->Cells[0][4] = L"Bib";
		SGSanoma->Cells[0][5] = L"Tulos";
		SGSanoma->Cells[0][6] = L"OK";
		SGSanoma->Cells[0][7] = L"Rastiluku";
		SGSanoma->Cells[0][8] = L"Rata";
		SGSanoma->Cells[1][1] = L"Emitväliajat";
		SGSanoma->Cells[1][2] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
		SGSanoma->Cells[1][3] = UnicodeString(ComBuf.d.emva.badge);
		SGSanoma->Cells[1][4] = UnicodeString(ComBuf.d.emva.kilpno);
		SGSanoma->Cells[1][5] = UnicodeString(aikatowstr_cols_n(st, ComBuf.d.emva.tulos, 0, 0, 11));
		SGSanoma->Cells[1][6] = UnicodeString(ComBuf.d.emva.ok);
		SGSanoma->Cells[1][7] = UnicodeString(ComBuf.d.emva.rastiluku);
		SGSanoma->Cells[1][8] = UnicodeString(ComBuf.d.emva.rata);
		for (int rr = 0; rr <= ComBuf.d.emva.rastiluku && rr < MAXNLEIMA; rr++) {
			SGSanoma->Cells[0][9+rr] = UnicodeString(rr+1)+L". rasti";
			SGSanoma->Cells[1][9+rr] = UnicodeString(ComBuf.d.emva.rastit[rr][0]) +
				L": "+ UnicodeString(ComBuf.d.emva.rastit[rr][1]);
			}
		}
	if (ComBuf.pkgclass == 14) {
		SGSanoma->RowCount = 8;
		SGSanoma->Cells[0][1] = L"Sanomalaji";
		SGSanoma->Cells[0][2] = L"Pos";
		SGSanoma->Cells[0][3] = L"Lähetetty";
		SGSanoma->Cells[0][4] = L"Arvo 1";
		SGSanoma->Cells[0][5] = L"Arvo 2";
		SGSanoma->Cells[0][6] = L"Arvo 3";
		SGSanoma->Cells[0][7] = L"Arvo 4";
		SGSanoma->Cells[1][1] = L"Extra";
		SGSanoma->Cells[1][2] = UnicodeString(ComBuf.d.k.dk);
		SGSanoma->Cells[1][3] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
		SGSanoma->Cells[1][4] = UnicodeString(ComBuf.d.x.d1);
		SGSanoma->Cells[1][5] = UnicodeString(ComBuf.d.x.d2);
		SGSanoma->Cells[1][6] = UnicodeString(ComBuf.d.x.d3);
		SGSanoma->Cells[1][7] = UnicodeString(ComBuf.d.x.d4);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::lataaJono(int jono)
{
	char cbuf[4000];
	combufrec *combuf = (combufrec *) cbuf;
	int tl, d;
	wchar_t st[100];
	kilptietue kilp, lkilp;
	kilppvtp pv;

	if (!ComFile)
		return;
	SGJono->RowCount = NJRec[jono] + 1;
	SGJono->ColCount = 11;
	SGJono->ColWidths[2] = 65 * Screen->PixelsPerInch / 96;
	SGJono->ColWidths[10] = 65 * Screen->PixelsPerInch / 96;
	SGJono->Cells[0][0] = L"Rivi";
	SGJono->Cells[1][0] = L"Class";
	SGJono->Cells[2][0] = L"Lähetetty";
	SGJono->Cells[3][0] = L"Pit";
	SGJono->Cells[4][0] = L"Pos";
	SGJono->Cells[5][0] = L"Entno";
	SGJono->Cells[6][0] = L"Bib";
	SGJono->Cells[7][0] = L"Badge";
	SGJono->Cells[8][0] = L"Vaihe";
	SGJono->Cells[9][0] = L"Piste";
	SGJono->Cells[10][0] = L"Aika";
	for (int r = 1; r <= NJRec[jono]; r++) {
		d = 0;
		fseek(ComFile, (r * NJono + jono) * RecLen, SEEK_SET);
		if (fread(cbuf, RecLen, 1, ComFile) != 1)
			break;
		SGJono->Cells[0][r] = UnicodeString(r);
		SGJono->Cells[1][r] = UnicodeString((int)combuf->pkgclass);
		tl = combuf->lahetetty * KSEK;
		SGJono->Cells[2][r] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
		SGJono->Cells[3][r] = UnicodeString(combuf->len);
		SGJono->Cells[5][r] = L"";
		SGJono->Cells[6][r] = L"";
		SGJono->Cells[7][r] = L"";
		SGJono->Cells[8][r] = L"";
		SGJono->Cells[9][r] = L"";
		SGJono->Cells[10][r] = L"";
		if (combuf->pkgclass == 1) {
			kilp.unpack0(combuf->d.k.ckilp);
			d = combuf->d.k.dk;
			SGJono->Cells[5][r] = UnicodeString(combuf->d.k.entno);
			SGJono->Cells[6][r] = UnicodeString(kilp.kilpno);
			SGJono->Cells[9][r] = UnicodeString(kilp.kilpstatus);
			}
		if (combuf->pkgclass == 2) {
			d = combuf->d.pv.dk;
			if (combuf->d.pv.valuku == kilpparam.valuku) {
				pv.unpack(combuf->d.pv.cpv);
				}
			else {
				pv.unpack0(combuf->d.pv.cpv);
				}
			SGJono->Cells[6][r] = UnicodeString(pv.bib);
			SGJono->Cells[7][r] = UnicodeString(pv.badge[0]);
			SGJono->Cells[8][r] = UnicodeString(combuf->d.pv.pv + 1);
			SGJono->Cells[10][r] = UnicodeString(aikatowstr_cols_n(st, pv.tlahto, t0, 0, 11));
			}
		if (combuf->pkgclass == 3) {
			d = combuf->d.v.dk;
			SGJono->Cells[6][r] = UnicodeString(combuf->d.v.bib);
			SGJono->Cells[9][r] = UnicodeString(combuf->d.v.vali);
			if (combuf->d.v.vali <= kilpparam.valuku) {
				tl = combuf->d.v.aika;
				SGJono->Cells[10][r] = UnicodeString(aikatowstr_cols_n(st, tl, 0, 0, 11));
				}
			else {
				wcsncpy(st, (wchar_t *) &combuf->d.v.aika, 4);
				if (wcswcind(st[0], L"-EVHIKPTXM") >= 0) {
					st[1] = 0;
				   SGJono->Cells[10][r] = UnicodeString(st);
				   }
				else
				   SGJono->Cells[10][r] = UnicodeString(*((int *)st));
				}
			}
		if (combuf->pkgclass == 4) {
			SGJono->Cells[5][r] = UnicodeString(combuf->d.a.daika.kno);
			SGJono->Cells[6][r] = UnicodeString(combuf->d.a.iaika.kno);
			SGJono->Cells[7][r] = UnicodeString(combuf->d.a.iaika.badge);
			SGJono->Cells[8][r] = UnicodeString(combuf->d.a.iaika.kanava);
			SGJono->Cells[9][r] = UnicodeString(combuf->d.a.iaika.piste);
			tl = combuf->d.a.iaika.t / AIKAJAK;
			SGJono->Cells[10][r] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
			}
		if (combuf->pkgclass == 6) {
			SGJono->Cells[6][r] = UnicodeString(combuf->d.em.kilpno);
			SGJono->Cells[7][r] = UnicodeString(combuf->d.em.badge);
			SGJono->Cells[8][r] = UnicodeString(aikatowstr_cols_n(st, KSEK*combuf->d.em.maali, t0, 0, 11));
			SGJono->Cells[9][r] = UnicodeString(aikatowstr_cols_n(st, KSEK*combuf->d.em.kirjaus, t0, 0, 11));
			SGJono->Cells[10][r] = UnicodeString(aikatowstr_cols_n(st, KSEK*combuf->d.em.time, t0, 0, 11));
			}
		if (combuf->pkgclass == 7) {
			SGJono->Cells[7][r] = UnicodeString(combuf->d.sra.lyhenne);
			SGJono->Cells[8][r] = UnicodeString(combuf->d.sra.maa);
			SGJono->Cells[9][r] = UnicodeString(combuf->d.sra.piiri);
			SGJono->Cells[10][r] = UnicodeString(combuf->d.sra.snimi);
			}
		if (combuf->pkgclass == 13) {
			SGJono->Cells[6][r] = UnicodeString(combuf->d.emva.kilpno);
			SGJono->Cells[7][r] = UnicodeString(combuf->d.emva.badge);
			SGJono->Cells[8][r] = UnicodeString(combuf->d.emva.rastiluku);
			SGJono->Cells[9][r] = UnicodeString(combuf->d.emva.rata);
			SGJono->Cells[10][r] = UnicodeString(aikatowstr_cols_n(st, combuf->d.emva.tulos, 0, 0, 11));
			}
		if (combuf->pkgclass == 14) {
			SGJono->Cells[7][r] = UnicodeString(combuf->d.x.d1);
			SGJono->Cells[8][r] = UnicodeString(combuf->d.x.d2);
			SGJono->Cells[9][r] = UnicodeString(combuf->d.x.d3);
			SGJono->Cells[10][r] = UnicodeString(combuf->d.x.d4);
			}
		if (d > 0)
			SGJono->Cells[4][r] = UnicodeString(d);
		else
			SGJono->Cells[4][r] = L"";
		if (r == SGJono->Row) {
			naytaSanoma(r);
			}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::EdtJonoValExit(TObject *Sender)
{
	int j;

	j = _wtoi(EdtJonoVal->Text.c_str());
	if (j > 0 && j <= NJono) {
		Jono = j-1;
		lataaJono(j-1);
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormComfile::EdSanomaExit(TObject *Sender)
{
	int r;
	r = _wtoi(EdSanoma->Text.c_str());
	if (r > 0 && r <= NJRec[Jono])
		SGJono->Row = r;
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::SGJonoSelectCell(TObject *Sender, int ACol, int ARow,
		  bool &CanSelect)
{
	naytaSanoma(ARow);
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::EdtJonoValKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	int j;

	if (Key == VK_RETURN) {
		j = _wtoi(EdtJonoVal->Text.c_str());
		if (j > 0 && j <= NJono) {
			Jono = j-1;
			lataaJono(j-1);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::EdSanomaKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN) {
		int r;
		r = _wtoi(EdSanoma->Text.c_str());
		if (r > 0 && r <= NJRec[Jono])
			SGJono->Row = r;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::Button5Click(TObject *Sender)
{
	if (SGJono->Row < NJRec[Jono])
		SGJono->Row++;
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::Button4Click(TObject *Sender)
{
	if (SGJono->Row > 1)
		SGJono->Row--;
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::Button2Click(TObject *Sender)
{
	if (CBkytke->Checked) {
		EdPos->Text = getpos(_wtoi(EdKilpno->Text.c_str()));
		HaeRivi(3);
		}
	else
		HaeRivi(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::Button3Click(TObject *Sender)
{
	HaeRivi(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::EdKilpnoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN)
		Button2Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::EdPosKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN)
		Button3Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::SGJonotDblClick(TObject *Sender)
{
	if (_wtoi(SGJonot->Cells[0][SGJonot->Row].c_str()) > 0) {
		EdtJonoVal->Text = SGJonot->Cells[0][SGJonot->Row];
		EdtJonoValExit(Sender);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::Button6Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::cbufout(TextFl *outtxt, char *cbuf)
{
	combufrec *combuf = (combufrec *) cbuf;
	int tl, d;
	wchar_t st[100], st1[40], st2[40], line[2000];
	kilptietue kilp, lkilp;
	kilppvtp pv;
	static int r;

	if (cbuf == 0) {
		wcscpy(line, L"Rivi\tClass\tLähetetty\tPit\tPos\tEntno\tBib\tBadge\tVaihe\tPiste\tAika\n");
		r = 0;
		}
	else {
		r++;
		tl = combuf->lahetetty * KSEK;
		swprintf(line, L"%d\t%d\t%s\t%d\t",
			r, (int)combuf->pkgclass, aikatowstr_cols_n(st, tl, t0, 0, 11), combuf->len);
		if (combuf->pkgclass == 1) {
			kilp.unpack0(combuf->d.k.ckilp);
			d = combuf->d.k.dk;
			swprintf(line+wcslen(line), L"%d\t%d\t%d\t\t\t%d\n",
				d, combuf->d.k.entno, kilp.kilpno, kilp.kilpstatus);
			}
		if (combuf->pkgclass == 2) {
			d = combuf->d.pv.dk;
			if (combuf->d.pv.valuku == kilpparam.valuku) {
				pv.unpack(combuf->d.pv.cpv);
				}
			else {
				pv.unpack0(combuf->d.pv.cpv);
				}
			swprintf(line+wcslen(line), L"%d\t\t%d\t%d\t%d\t\t%s\n",
				d, pv.bib, pv.badge[0], combuf->d.pv.pv + 1, aikatowstr_cols_n(st, pv.tlahto, t0, 0, 11));
			}
		if (combuf->pkgclass == 3) {
			d = combuf->d.v.dk;
			if (combuf->d.v.vali <= kilpparam.valuku) {
				tl = combuf->d.v.aika;
				aikatowstr_cols_n(st, tl, 0, 0, 11);
				}
			else {
				if (wcswcind(st[0], L"-EVHIKPT") >= 0) {
					wcsncpy(st, (wchar_t *) &combuf->d.v.aika, 4);
					st[1] = 0;
				   }
				else
					_itow(combuf->d.v.aika, st, 10);
				}
			swprintf(line+wcslen(line), L"%d\t\t%d\t\t\t%d\t%s\n",
				d, combuf->d.v.bib, combuf->d.v.vali, st);
			}
		if (combuf->pkgclass == 4) {
			tl = combuf->d.a.iaika.t / AIKAJAK;
			swprintf(line+wcslen(line), L"\t%d\t%d\t%d\t%d\t%d\t%s\n",
				combuf->d.a.daika.kno, combuf->d.a.iaika.kno, combuf->d.a.iaika.badge,
				combuf->d.a.iaika.kanava, combuf->d.a.iaika.piste, aikatowstr_cols_n(st, tl, t0, 0, 11));
			}
		if (combuf->pkgclass == 6) {
			aikatowstr_cols_n(st, KSEK*combuf->d.em.maali, t0, 0, 11);
			aikatowstr_cols_n(st1, KSEK*combuf->d.em.kirjaus, t0, 0, 11);
			aikatowstr_cols_n(st2, KSEK*combuf->d.em.time, t0, 0, 11);
			swprintf(line+wcslen(line), L"\t\t%d\t%d\t%s\t%s\t%s\n",
				combuf->d.em.kilpno, combuf->d.em.badge, st, st1, st2);
			}
		if (combuf->pkgclass == 7) {
			swprintf(line+wcslen(line), L"\t\t\t\t%s\t%s\t%d\t%s\n",
				combuf->d.sra.lyhenne, combuf->d.sra.maa, combuf->d.sra.piiri,
				combuf->d.sra.snimi);
			}
		if (combuf->pkgclass == 13) {
			swprintf(line+wcslen(line), L"\t\t\t%d\t%d\t%d\t%s\t%s\n",
				combuf->d.emva.kilpno, combuf->d.emva.badge, combuf->d.emva.rastiluku,
				combuf->d.emva.rata, aikatowstr_cols_n(st, combuf->d.emva.tulos, 0, 0, 11));
			}
		if (combuf->pkgclass == 14) {
			swprintf(line+wcslen(line), L"\t\t\t\t%d\t%d\t%d\t%d\n",
				combuf->d.x.d1, combuf->d.x.d2, combuf->d.x.d3, combuf->d.x.d4);
			}
		}
	outtxt->WriteLine(line);
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::Button8Click(TObject *Sender)
{
	FILE *outfl = NULL;
	TextFl *outtxt = NULL;
	int vast;
	char cbuf[2000];

	vast = select3(3, L"Valitse tiedostotyyppi", L"Valitse", L"Tekstitiedosto", L"COMFILE.DAT", L"Peruuta",
		this);
	if (vast == 3)
		return;
	if (vast == 2) {
		if (SaveDialog1->Execute())
			outfl = _wfopen(SaveDialog1->FileName.c_str(), L"wb");
		}
	else {
		if (SaveDialog1->Execute())
			outtxt = new TextFl(SaveDialog1->FileName.c_str(), L"wt");
		}
	if (outfl || outtxt) {
		if (outfl) {
			combufrec *ob;
			memset(cbuf, 0, sizeof(cbuf));
			ob = (combufrec *)cbuf;
			ob->so = 1;
			ob->len = RecLen;
			fwrite(cbuf, RecLen, 1, outfl);
			}
		else
			cbufout(outtxt, 0);
		for	(int r = 1; r < SGJono->RowCount; r++) {
			fseek(ComFile, (r * NJono + Jono) * RecLen, SEEK_SET);
			if (fread(cbuf, RecLen, 1, ComFile) != 1)
				break;
			if (outfl)
				fwrite(cbuf, RecLen, 1, outfl);
			else
				cbufout(outtxt, cbuf);
			}
		}
	if (outfl)
		fclose(outfl);
	if (outtxt)
		delete outtxt;
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::BtnHaebadgeClick(TObject *Sender)
{
	HaeRivi(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::HaeRivi(int flg)
{
	char cbuf[2000];
	combufrec *combuf = (combufrec *) cbuf;
	kilptietue kilp;
	kilppvtp pv;
	int r, bdg, kno, pos;

	if (SGJono->RowCount < 2)
		return;
	r = SGJono->Row % (SGJono->RowCount-1) + 1;
	bdg = _wtoi(EdBadge->Text.c_str());
	kno = _wtoi(EdKilpno->Text.c_str());
	pos = _wtoi(EdPos->Text.c_str());
	for	( ; r != SGJono->Row;) {
		fseek(ComFile, (r * NJono + Jono) * RecLen, SEEK_SET);
		if (fread(cbuf, RecLen, 1, ComFile) != 1)
			break;
		if (combuf->pkgclass == 1)
			kilp.unpack0(combuf->d.k.ckilp);
		if (combuf->pkgclass == 2)
			pv.unpack0(combuf->d.pv.cpv);
		if (flg & 1) {
			if (combuf->pkgclass == 1 && combuf->d.k.entno == kno ||
				kilp.kilpno == kno)
				break;
			if (combuf->pkgclass == 2 && pv.bib == kno)
				break;
			if (combuf->pkgclass == 3 && combuf->d.v.bib == kno)
				break;
			if (combuf->pkgclass == 4 && (combuf->d.a.iaika.kno == kno ||
				combuf->d.a.daika.kno == kno))
				break;
			if (combuf->pkgclass == 6 && combuf->d.em.kilpno == kno)
				break;
			if (combuf->pkgclass == 13 && combuf->d.emva.kilpno == kno)
				break;
			}
		if (flg & 2) {
			if (combuf->pkgclass > 1 && combuf->pkgclass < 4 &&
				combuf->d.k.dk == pos)
				break;
			}
		if (flg & 4) {
			if (combuf->pkgclass == 2 && (pv.badge[0] == bdg ||
				pv.badge[1] == bdg))
				break;
			if (combuf->pkgclass == 4 && (combuf->d.a.iaika.badge == bdg ||
				combuf->d.a.daika.badge == bdg))
				break;
			if (combuf->pkgclass == 6 && (combuf->d.em.badge == bdg ||
				combuf->d.em.badge0 == bdg))
				break;
			if (combuf->pkgclass == 13 && combuf->d.emva.badge == bdg)
				break;
			}
		r = r % (SGJono->RowCount-1) + 1;
		}
	SGJono->Row = r;
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::CBkytkeClick(TObject *Sender)
{
	EdPos->Enabled = !CBkytke->Checked;
	Button3->Visible = EdPos->Enabled;
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::BtnLahetaClick(TObject *Sender)
{
	wchar_t as[20];

	CBLahYhteys->Clear();
	CBLahYhteys->Items->Add(L"Kaikki");
	for (int cn = 0; cn < maxyhteys; cn++) {
		CBLahYhteys->Items->Add(UnicodeString(cn+1));
		}
	if (sendtestparam.NJono) {
		RGValit->ItemIndex = sendtestparam.valilaji;
		RGAloitus->ItemIndex = sendtestparam.aloituslaji;
		CBLahYhteys->ItemIndex = sendtestparam.cn + 1;
		EdtLahAlku->Text = sendtestparam.ens;
		EdtValit->Text = sendtestparam.ValiKerroin;
		EdtLahLoppu->Text = sendtestparam.viim;
		if (sendtestparam.valilaji == 1) {
			EdtAloitus->EditLabel->Caption = L"Vertailuaika";
			EdtAloitus->Text = aikatowstr_cols_n(as, sendtestparam.t_vert, t0, 0, 8);
			}
		RGAloitus->ItemIndex = sendtestparam.aloituslaji;
		if (sendtestparam.valilaji == 0 && sendtestparam.aloituslaji == 1)
			EdtAloitus->Text = aikatowstr_cols_n(as, sendtestparam.t_aloitus, t0, 0, 8);
		if (sendtestparam.ComFile) {
			LahComFile = sendtestparam.ComFile;
			if (LahComFile != ComFile)
				Application->MessageBoxW(L"Lähetettävien sanomien tiedosto on jo avattu ja poikkeaa kaavakkeen tiedostosta", L"Varoitus", MB_OK);
			}
		else
			LahComFile = ComFile;
		Jono = sendtestparam.Jono;
		NJono = sendtestparam.NJono;
		RecLen = sendtestparam.RecLen;
		}
	else
		LahComFile = ComFile;
	GBLahetys->Visible = true;
	FormResize(Sender);
	if (sendtestparam.inSendCopy) {
		BtnLahVal->Caption = L"Keskeytä lähettäminen";
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::RGValitClick(TObject *Sender)
{
	if (RGValit->ItemIndex == 1) {
		EdtValit->EditLabel->Caption = L"Kerroin";
		EdtValit->Text = L"1,0";
		EdtAloitus->EditLabel->Caption = L"Vertailuaika";
		}
	else {
		EdtValit->EditLabel->Caption = L"Sanomaväli";
		EdtAloitus->EditLabel->Caption = L"Aloitushetki";
		}
	if (EdtLahAlku->Text.Length() == 0 || _wtoi(EdtLahAlku->Text.c_str()) == 0)
		EdtLahAlku->Text = 1;
	if (EdtLahLoppu->Text.Length() == 0 || _wtoi(EdtLahLoppu->Text.c_str()) == 0)
		EdtLahLoppu->Text = NJRec[Jono];
	if (EdtAloitus->Text.Length() == 0 || wstrtoaika_vap(EdtAloitus->Text.c_str(), t0) == TMAALI0)
		EdtAloitus->Text = wkello();
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::BtnLahYksiClick(TObject *Sender)
{
	combufrec combuf;
	int r, cn = -1;

	cn = CBLahYhteys->ItemIndex - 1;
	r = _wtoi(EdtLahAlku->Text.c_str());
	if (r <= 0 || r > NJRec[Jono]) {
		return;
		}
	fseek(LahComFile, (r * NJono + Jono) * RecLen, SEEK_SET);
	if (fread(&combuf, RecLen, 1, LahComFile) == 1) {
		lahetaKopiot(cn, combuf);
		r++;
		EdtLahAlku->Text = r;
		}
}
//---------------------------------------------------------------------------

MESSAGE void __fastcall TFormComfile::PaivHandler(TMyMessage &msg)
{
	EdtLahAlku->Text = LahRivi;
	EdtLahAlku->Refresh();
}

//---------------------------------------------------------------------------
void __fastcall TFormComfile::BtnLahValClick(TObject *Sender)
{
	wchar_t *p, st[20] = L"";

	if (sendtestparam.inSendCopy) {
		sendtestparam.inSendCopy = false;
		Button1->Enabled = true;
		BtnLahVal->Caption = L"Lähetätä valintojen mukaan";
		return;
		}
	sendtestparam.inSendCopy = true;
	if (sendtestparam.valilaji == 0 && RGAloitus->ItemIndex == 0)
		EdtAloitus->Text = wkello();
	sendtestparam.valilaji = RGValit->ItemIndex;
	sendtestparam.cn = CBLahYhteys->ItemIndex - 1;
	sendtestparam.ens = _wtoi(EdtLahAlku->Text.c_str());
	if (EdtValit->Text.Length() > 0)
		wcsncpy(st, EdtValit->Text.c_str(), 19);
	p = wcsstr(st, L",");
	if (p)
		*p = L'.';
	sendtestparam.ValiKerroin = _wtof(st);
	sendtestparam.t_vert = TMAALI0;
	sendtestparam.viim = _wtoi(EdtLahLoppu->Text.c_str());
	if (sendtestparam.viim > NJRec[FormComfile->Jono])
		sendtestparam.viim = NJRec[FormComfile->Jono];
	if (sendtestparam.valilaji == 1) {
		sendtestparam.t_vert = wstrtoaika_vap(EdtAloitus->Text.c_str(), t0);
		}
	sendtestparam.aloituslaji = RGAloitus->ItemIndex;
	if (sendtestparam.valilaji == 0 && sendtestparam.aloituslaji == 1)
		sendtestparam.t_aloitus = wstrtoaika_vap(EdtAloitus->Text.c_str(), t0);
	sendtestparam.ComFile = FormComfile->LahComFile;
	sendtestparam.Jono = FormComfile->Jono;
	sendtestparam.NJono = FormComfile->NJono;
	sendtestparam.RecLen = FormComfile->RecLen;
	IkkParam.ValiKerroin = sendtestparam.ValiKerroin;
	if (sendtestparam.ComFile == NULL) {
		if (LahComFile == NULL)
			LahComFile = ComFile;
		sendtestparam.ComFile = LahComFile;
		}

	_beginthread(SendCopyThread, 10240, NULL);
	BtnLahVal->Caption = L"Keskeytä lähettäminen";
}
//---------------------------------------------------------------------------

void lahetatestikaynnistys(INT kielto);

void kaynnistatesti(int kielto)
{
	lahetatestikaynnistys(kielto);
	if (FormComfile && FormComfile->GBLahetys->Visible) {
		FormComfile->BtnLahValClick(FormComfile);
		}
	else
		_beginthread(SendCopyThread, 10240, NULL);
}
//---------------------------------------------------------------------------

void naytaTestRivi(void)
{
	if (FormComfile && FormComfile->Visible)
		SendMessage(FormComfile->Handle,WM_MYPAIVITA,0,0);
}

void sendTestLopetus(void)
{
	if (FormComfile && FormComfile->Visible) {
		FormComfile->EdtLahAlku->Text = sendtestparam.LahRivi;
		FormComfile->BtnLahVal->Caption = L"Lähetä valintojen mukaan";
		}
}

//---------------------------------------------------------------------------

void __fastcall TFormComfile::BtnTallMuutosClick(TObject *Sender)
{
	if (ComBuf.pkgclass == 1) {
		ComBuf.lahetetty = UStrToAika(SGSanoma->Cells[1][5], t0) / KSEK;
		}
	if (ComBuf.pkgclass == 2) {
		ComBuf.lahetetty = UStrToAika(SGSanoma->Cells[1][3], t0) / KSEK;
		}
	if (ComBuf.pkgclass == 3) {
		ComBuf.lahetetty = UStrToAika(SGSanoma->Cells[1][3], t0) / KSEK;
		}
	if (ComBuf.pkgclass == 4) {
		ComBuf.lahetetty = UStrToAika(SGSanoma->Cells[1][2], t0) / KSEK;
		}
	if (ComBuf.pkgclass == 6) {
		ComBuf.lahetetty = UStrToAika(SGSanoma->Cells[1][2], t0) / KSEK;
		}
	if (ComBuf.pkgclass == 13) {
		ComBuf.lahetetty = UStrToAika(SGSanoma->Cells[1][2], t0) / KSEK;
		}
	if (ComBuf.pkgclass == 14) {
		ComBuf.lahetetty = UStrToAika(SGSanoma->Cells[1][3], t0) / KSEK;
		}
	fseek(ComFile, (SGJono->Row * NJono + Jono) * RecLen, SEEK_SET);
	fwrite(&ComBuf, RecLen, 1, ComFile);
}
//---------------------------------------------------------------------------

