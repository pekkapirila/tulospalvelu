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
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormComfile *FormComfile;

extern sendtestparamtp sendtestparam;
void lahetaKopiot(int cn, combufrec obuf);
void init_class_len(void);
extern int class_len[];
extern int  combufsize;

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
	ActComfile = FormMain->CurrentDir+UnicodeString(L"\\comfile.dat");
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
	BtnLaheta->Visible = false;
	OpenDialog1->FileName = ActComfile;
	OpenDialog1->DefaultExt = L"dat";
	OpenDialog1->Filter = L"DAT-tiedostot (*.dat)|*.DAT|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
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
	osrec *ostiet;

	ostiet = &kilp.ostiet[0];
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
		memcpy(&kilp, ComBuf.d.k.ckilp, ComBuf.len);
		SGSanoma->RowCount = 11;
		SGSanoma->Cells[0][1] = L"Sanomalaji";
		SGSanoma->Cells[0][2] = L"Status";
		SGSanoma->Cells[0][3] = L"Pos";
		SGSanoma->Cells[0][4] = L"EntNo";
		SGSanoma->Cells[0][5] = L"Lähetetty";
		SGSanoma->Cells[0][6] = L"KilpNo";
		SGSanoma->Cells[0][7] = L"Seura";
		SGSanoma->Cells[0][8] = L"Joukkue";
		SGSanoma->Cells[0][9] = L"Sarja";
		SGSanoma->Cells[0][10] = L"Maa";
		SGSanoma->Cells[1][1] = L"Perustiedot";
		SGSanoma->Cells[1][2] = UnicodeString(kilp.kilpstatus);
		SGSanoma->Cells[1][3] = UnicodeString(ComBuf.d.k.dk);
		SGSanoma->Cells[1][4] = UnicodeString(ComBuf.d.k.entno);
		SGSanoma->Cells[1][5] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
		SGSanoma->Cells[1][6] = UnicodeString(kilp.kilpno);
		SGSanoma->Cells[1][7] = UnicodeString(kilp.Seura(st, LSEURA));
		SGSanoma->Cells[1][8] = UnicodeString(kilp.joukkue);
		SGSanoma->Cells[1][9] = UnicodeString(kilp.sarja);
		SGSanoma->Cells[1][10] = UnicodeString(kilp.Maa(st));
		}
	if (ComBuf.pkgclass == 2) {
		memcpy(ostiet, ComBuf.d.os.cos, ComBuf.len);
		SGSanoma->RowCount = 13;
		SGSanoma->Cells[0][1] = L"Sanomalaji";
		SGSanoma->Cells[0][2] = L"Pos";
		SGSanoma->Cells[0][3] = L"Lähetetty";
		SGSanoma->Cells[0][4] = L"Kilpno";
		SGSanoma->Cells[0][5] = L"Osuus";
		SGSanoma->Cells[0][6] = L"Nimi";
		SGSanoma->Cells[0][7] = L"Badge";
		SGSanoma->Cells[0][8] = L"Rata";
		SGSanoma->Cells[0][9] = L"Lähtö";
		SGSanoma->Cells[0][10] = L"Tark";
		SGSanoma->Cells[0][11] = L"Sakko";
		SGSanoma->Cells[0][12] = L"Muutos";
		SGSanoma->Cells[1][1] = L"Osuustiedot";
		SGSanoma->Cells[1][2] = UnicodeString(ComBuf.d.os.dk);
		SGSanoma->Cells[1][3] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
		SGSanoma->Cells[1][4] = UnicodeString(ComBuf.d.os.kno);
		SGSanoma->Cells[1][5] = UnicodeString(ComBuf.d.os.osuus);
		SGSanoma->Cells[1][6] = UnicodeString(ostiet->Nimi(st, kilpparam.lnimi));
		SGSanoma->Cells[1][7] = UnicodeString(ostiet->badge[0])+" / "+UnicodeString(ostiet->badge[1]);
		SGSanoma->Cells[1][8] = UnicodeString(ostiet->hajonta);
		SGSanoma->Cells[1][9] = UnicodeString(aikatowstr_cols_n(st, ostiet->ylahto, t0, 0, 11))+
									" / "+UnicodeString((int)ostiet->lahtolaji);
		SGSanoma->Cells[1][10] = UnicodeString(ostiet->keskhyl);
		SGSanoma->Cells[1][11] = UnicodeString(ostiet->sakko);
		_itow(ComBuf.d.os.muutos, st, 16);
		SGSanoma->Cells[1][12] = st;
		}
	if (ComBuf.pkgclass == 3) {
		SGSanoma->RowCount = 8;
		SGSanoma->Cells[0][1] = L"Sanomalaji";
		SGSanoma->Cells[0][2] = L"Pos";
		SGSanoma->Cells[0][3] = L"Lähetetty";
		SGSanoma->Cells[0][4] = L"Kilpno";
		SGSanoma->Cells[0][5] = L"Osuus";
		SGSanoma->Cells[0][6] = L"Piste";
		SGSanoma->Cells[0][7] = L"Aika";
		SGSanoma->Cells[1][1] = L"Tulostieto";
		SGSanoma->Cells[1][2] = UnicodeString(ComBuf.d.p.dk);
		SGSanoma->Cells[1][3] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
		SGSanoma->Cells[1][4] = UnicodeString(ComBuf.d.p.kno);
		SGSanoma->Cells[1][5] = UnicodeString(ComBuf.d.p.osuus);
		SGSanoma->Cells[1][6] = UnicodeString(ComBuf.d.p.piste);
		SGSanoma->Cells[1][7] = UnicodeString(aikatowstr_cols_n(st, *(int *)ComBuf.d.p.cva, t0, 0, 11));
		}
	if (ComBuf.pkgclass == 4) {
		SGSanoma->RowCount = 17;
		SGSanoma->Cells[0][1] = L"Sanomalaji";
		SGSanoma->Cells[0][2] = L"Lähetetty";
		SGSanoma->Cells[0][3] = L"Aika";
		SGSanoma->Cells[0][4] = L"Kilpno";
		SGSanoma->Cells[0][5] = L"Osuus";
		SGSanoma->Cells[0][6] = L"Piste";
		SGSanoma->Cells[0][7] = L"Badge";
		SGSanoma->Cells[0][8] = L"Kanava";
		SGSanoma->Cells[0][9] = L"Aikajono";
		SGSanoma->Cells[0][10] = L"Ed. aika";
		SGSanoma->Cells[0][11] = L"Ed. kilpno";
		SGSanoma->Cells[0][12] = L"Ed. osuus";
		SGSanoma->Cells[0][13] = L"Ed. piste";
		SGSanoma->Cells[0][14] = L"Ed. badge";
		SGSanoma->Cells[0][15] = L"Ed. kanava";
		SGSanoma->Cells[0][16] = L"Ed. aikajono";
		SGSanoma->Cells[1][1] = L"Ajanottotieto";
		SGSanoma->Cells[1][2] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
		SGSanoma->Cells[1][3] = UnicodeString(aikatowstr_cols_n(st, ComBuf.d.a.iaika.t/AIKAJAK, t0, 0, 11));
		SGSanoma->Cells[1][4] = UnicodeString(ComBuf.d.a.iaika.kno);
		SGSanoma->Cells[1][5] = UnicodeString(ComBuf.d.a.iaika.osuus+1);
		SGSanoma->Cells[1][6] = UnicodeString(ComBuf.d.a.iaika.piste);
		SGSanoma->Cells[1][7] = UnicodeString(ComBuf.d.a.iaika.badge);
		SGSanoma->Cells[1][8] = UnicodeString(ComBuf.d.a.iaika.kanava);
		SGSanoma->Cells[1][9] = UnicodeString(ComBuf.d.a.iaika.jono+1);
		SGSanoma->Cells[1][10] = UnicodeString(aikatowstr_cols_n(st, ComBuf.d.a.daika.t/AIKAJAK, t0, 0, 11));
		SGSanoma->Cells[1][11] = UnicodeString(ComBuf.d.a.daika.kno);
		SGSanoma->Cells[1][12] = UnicodeString(ComBuf.d.a.daika.osuus+1);
		SGSanoma->Cells[1][13] = UnicodeString(ComBuf.d.a.daika.piste);
		SGSanoma->Cells[1][14] = UnicodeString(ComBuf.d.a.daika.badge);
		SGSanoma->Cells[1][15] = UnicodeString(ComBuf.d.a.daika.kanava);
		SGSanoma->Cells[1][16] = UnicodeString(ComBuf.d.a.daika.jono+1);
		}
	if (ComBuf.pkgclass == 5) {
		SGSanoma->RowCount = 9;
		SGSanoma->Cells[0][1] = L"Sanomalaji";
		SGSanoma->Cells[0][2] = L"Pos";
		SGSanoma->Cells[0][3] = L"Lähetetty";
		SGSanoma->Cells[0][4] = L"Kilpno";
		SGSanoma->Cells[0][5] = L"Osuus";
		SGSanoma->Cells[0][6] = L"Ampsakot";
		SGSanoma->Cells[0][7] = L"Sakko";
		SGSanoma->Cells[0][8] = L"Tark";
		SGSanoma->Cells[1][1] = L"Tarkastustieto";
		SGSanoma->Cells[1][2] = UnicodeString(ComBuf.d.trk.dk);
		SGSanoma->Cells[1][3] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
		SGSanoma->Cells[1][4] = UnicodeString(ComBuf.d.trk.kno);
		SGSanoma->Cells[1][5] = UnicodeString(ComBuf.d.trk.osuus);
		SGSanoma->Cells[1][5] = UnicodeString(ComBuf.d.trk.ampsakot);
		SGSanoma->Cells[1][6] = UnicodeString(ComBuf.d.trk.sakko);
		SGSanoma->Cells[1][8] = UnicodeString(ComBuf.d.trk.keskhyl);
		}
	if (ComBuf.pkgclass == 6) {
		SGSanoma->RowCount = 59;
		SGSanoma->Cells[0][1] = L"Sanomalaji";
		SGSanoma->Cells[0][2] = L"Lähetetty";
		SGSanoma->Cells[0][3] = L"Badge";
		SGSanoma->Cells[0][4] = L"Kilpno";
		SGSanoma->Cells[0][5] = L"Osuus";
		SGSanoma->Cells[0][6] = L"Maali";
		SGSanoma->Cells[0][7] = L"Luettu";
		SGSanoma->Cells[0][8] = L"Kirjattu";
		SGSanoma->Cells[1][1] = L"Emit-tieto";
		SGSanoma->Cells[1][2] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
		SGSanoma->Cells[1][3] = UnicodeString(ComBuf.d.em.badge)+L" (orig:"+UnicodeString(ComBuf.d.em.badge0)+L")";
		SGSanoma->Cells[1][4] = UnicodeString(ComBuf.d.em.kilpno);
		SGSanoma->Cells[1][5] = UnicodeString(ComBuf.d.em.osuus+1);
		SGSanoma->Cells[1][6] = UnicodeString(aikatowstr_cols_n(st, KSEK*ComBuf.d.em.maali, t0, 0, 11));
		SGSanoma->Cells[1][7] = UnicodeString(aikatowstr_cols_n(st, KSEK*ComBuf.d.em.time, t0, 0, 11));
		SGSanoma->Cells[1][8] = UnicodeString(aikatowstr_cols_n(st, KSEK*ComBuf.d.em.kirjaus, t0, 0, 11));
		for (int rr = 0; rr < 50; rr++) {
			SGSanoma->Cells[0][9+rr] = UnicodeString(rr+1)+L". leima";
			SGSanoma->Cells[1][9+rr] = UnicodeString((int)(ComBuf.d.em.ctrlcode[rr])) +
				L": "+ UnicodeString(ComBuf.d.em.ctrltime[rr]);
			}
		}
	if (ComBuf.pkgclass == 8) {
		SGSanoma->RowCount = 7;
		SGSanoma->Cells[0][1] = L"Sanomalaji";
		SGSanoma->Cells[0][2] = L"Lähetetty";
		SGSanoma->Cells[0][3] = L"Kilpno";
		SGSanoma->Cells[0][4] = L"Osuus";
		SGSanoma->Cells[0][5] = L"EntKno";
		SGSanoma->Cells[0][6] = L"EntOs";
		SGSanoma->Cells[1][1] = L"Emitvaihto";
		SGSanoma->Cells[1][2] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
		SGSanoma->Cells[1][3] = UnicodeString(ComBuf.d.ev.kno);
		SGSanoma->Cells[1][4] = UnicodeString(ComBuf.d.ev.os+1);
		SGSanoma->Cells[1][5] = UnicodeString(ComBuf.d.ev.ekno);
		SGSanoma->Cells[1][6] = UnicodeString(ComBuf.d.ev.eos+1);
		}
	if (ComBuf.pkgclass == 9) {
		SGSanoma->RowCount = 5+2*kilpparam.n_os_akt;
		SGSanoma->Cells[0][1] = L"Sanomalaji";
		SGSanoma->Cells[0][2] = L"Lähetetty";
		SGSanoma->Cells[0][3] = L"Sarja";
		SGSanoma->Cells[0][4] = L"Flag";
		SGSanoma->Cells[1][1] = L"Yhteislähtö";
		SGSanoma->Cells[1][2] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
		SGSanoma->Cells[1][3] = UnicodeString(ComBuf.d.y.sarja);
		SGSanoma->Cells[1][4] = UnicodeString(ComBuf.d.y.fl);
		for (int rr = 0; rr < kilpparam.n_os_akt; rr++) {
			SGSanoma->Cells[0][5+2*rr] = UnicodeString(rr+1)+L". os. ylähtö";
			SGSanoma->Cells[1][5+2*rr] = UnicodeString(aikatowstr_cols_n(st, ComBuf.d.y.ylahto[rr][0], t0, 0, 11));
			SGSanoma->Cells[0][6+2*rr] = UnicodeString(rr+1)+L". os. kynnys";
			SGSanoma->Cells[1][6+2*rr] = UnicodeString(aikatowstr_cols_n(st, ComBuf.d.y.ylahto[rr][1], t0, 0, 11));
			}
		}
	if (ComBuf.pkgclass == 13) {
		SGSanoma->RowCount = 11 + ComBuf.d.emva.rastiluku;
		SGSanoma->Cells[0][1] = L"Sanomalaji";
		SGSanoma->Cells[0][2] = L"Lähetetty";
		SGSanoma->Cells[0][3] = L"Badge";
		SGSanoma->Cells[0][4] = L"Kilpno";
		SGSanoma->Cells[0][5] = L"Osuus";
		SGSanoma->Cells[0][6] = L"Tulos";
		SGSanoma->Cells[0][7] = L"OK";
		SGSanoma->Cells[0][8] = L"Rastiluku";
		SGSanoma->Cells[0][9] = L"Rata";
		SGSanoma->Cells[1][1] = L"Emitväliajat";
		SGSanoma->Cells[1][2] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
		SGSanoma->Cells[1][3] = UnicodeString(ComBuf.d.emva.badge);
		SGSanoma->Cells[1][4] = UnicodeString(ComBuf.d.emva.kilpno);
		SGSanoma->Cells[1][5] = UnicodeString(ComBuf.d.emva.osuus+1);
		SGSanoma->Cells[1][6] = UnicodeString(aikatowstr_cols_n(st, ComBuf.d.emva.tulos, 0, 0, 11));
		SGSanoma->Cells[1][7] = UnicodeString(ComBuf.d.emva.ok);
		SGSanoma->Cells[1][8] = UnicodeString(ComBuf.d.emva.rastiluku);
		SGSanoma->Cells[1][9] = UnicodeString(ComBuf.d.emva.rata);
		for (int rr = 0; rr <= ComBuf.d.emva.rastiluku && rr < 50; rr++) {
			SGSanoma->Cells[0][10+rr] = UnicodeString(rr+1)+L". rasti";
			SGSanoma->Cells[1][10+rr] = UnicodeString(ComBuf.d.emva.rastit[rr][0]) +
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
	char cbuf[2000];
	combufrec *combuf = (combufrec *) cbuf;
	int tl, d;
	wchar_t st[100];
	kilptietue kilp, lkilp;
	osrec ostiet;

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
	SGJono->Cells[8][0] = L"Osuus";
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
			memcpy(&kilp, combuf->d.k.ckilp, combuf->len);
			d = combuf->d.k.dk;
			SGJono->Cells[5][r] = UnicodeString(combuf->d.k.entno);
			SGJono->Cells[6][r] = UnicodeString(kilp.kilpno);
			SGJono->Cells[9][r] = UnicodeString(kilp.kilpstatus);
			}
		if (combuf->pkgclass == 2) {
			d = combuf->d.os.dk;
			memcpy(&ostiet, combuf->d.os.cos, combuf->len);
			SGJono->Cells[5][r] = UnicodeString(combuf->d.os.kno);
			SGJono->Cells[7][r] = UnicodeString(ostiet.badge[0]);
			SGJono->Cells[8][r] = UnicodeString(combuf->d.os.osuus);
			SGJono->Cells[10][r] = UnicodeString(aikatowstr_cols_n(st, ostiet.ylahto, t0, 0, 11));
			_itow(combuf->d.os.muutos, st, 16);
			SGJono->Cells[9][r] = st;
			}
		if (combuf->pkgclass == 3) {
			d = combuf->d.p.dk;
			SGJono->Cells[5][r] = UnicodeString(combuf->d.p.kno);
			SGJono->Cells[8][r] = UnicodeString(combuf->d.p.osuus);
			SGJono->Cells[9][r] = UnicodeString(combuf->d.p.piste);
			tl = *(int *)combuf->d.p.cva;
			SGJono->Cells[10][r] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
			}
		if (combuf->pkgclass == 4) {
			SGJono->Cells[5][r] = UnicodeString(combuf->d.a.daika.kno);
			SGJono->Cells[6][r] = UnicodeString(combuf->d.a.iaika.kno);
			SGJono->Cells[7][r] = UnicodeString(combuf->d.a.iaika.badge);
			SGJono->Cells[8][r] = UnicodeString(combuf->d.a.iaika.osuus+1);
			SGJono->Cells[9][r] = UnicodeString(combuf->d.a.iaika.piste);
			tl = combuf->d.a.iaika.t / AIKAJAK;
			SGJono->Cells[10][r] = UnicodeString(aikatowstr_cols_n(st, tl, t0, 0, 11));
			}
		if (combuf->pkgclass == 5) {
			d = combuf->d.trk.dk;
			SGJono->Cells[8][r] = UnicodeString(combuf->d.trk.osuus);
			SGJono->Cells[9][r] = UnicodeString(combuf->d.trk.keskhyl);
			SGJono->Cells[10][r] = UnicodeString(combuf->d.trk.ampsakot);
			}
		if (combuf->pkgclass == 6) {
			SGJono->Cells[5][r] = UnicodeString(combuf->d.em.kilpno);
			SGJono->Cells[7][r] = UnicodeString(combuf->d.em.badge);
			SGJono->Cells[8][r] = UnicodeString(combuf->d.em.osuus+1);
			SGJono->Cells[9][r] = UnicodeString(aikatowstr_cols_n(st, KSEK*combuf->d.em.kirjaus, t0, 0, 11));
			SGJono->Cells[10][r] = UnicodeString(aikatowstr_cols_n(st, KSEK*combuf->d.em.time, t0, 0, 11));
			}
		if (combuf->pkgclass == 8) {
			SGJono->Cells[7][r] = UnicodeString(combuf->d.ev.kno);
			SGJono->Cells[8][r] = UnicodeString(combuf->d.ev.os+1);
			SGJono->Cells[9][r] = UnicodeString(combuf->d.ev.ekno);
			SGJono->Cells[10][r] = UnicodeString(combuf->d.ev.eos+1);
			}
		if (combuf->pkgclass == 13) {
			SGJono->Cells[5][r] = UnicodeString(combuf->d.emva.kilpno);
			SGJono->Cells[7][r] = UnicodeString(combuf->d.emva.badge);
			SGJono->Cells[8][r] = UnicodeString(combuf->d.emva.osuus);
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
	char cbuf[2000];
	combufrec *combuf = (combufrec *) cbuf;
	kilptietue *kilp = (kilptietue *) combuf->d.k.ckilp;
	int r, kno;

	if (SGJono->RowCount < 2)
		return;
	r = SGJono->Row % (SGJono->RowCount-1) + 1;
	kno = _wtoi(EdKilpno->Text.c_str());
	for	( ; r != SGJono->Row;) {
		fseek(ComFile, (r * NJono + Jono) * RecLen, SEEK_SET);
		if (fread(cbuf, RecLen, 1, ComFile) != 1)
			break;
		if (combuf->pkgclass == 1 && (combuf->d.k.entno == kno ||
			kilp->kilpno == kno))
			break;
		if (combuf->pkgclass == 2 && combuf->d.os.kno == kno)
			break;
		if (combuf->pkgclass == 3 && combuf->d.p.kno == kno)
			break;
		if (combuf->pkgclass == 4 && (combuf->d.a.daika.kno == kno ||
			combuf->d.a.daika.kno == kno))
			break;
		if (combuf->pkgclass == 5 && combuf->d.trk.kno == kno)
			break;
		if (combuf->pkgclass == 6 && combuf->d.em.kilpno == kno)
			break;
		if (combuf->pkgclass == 8 && (combuf->d.ev.ekno == kno ||
			combuf->d.ev.kno == kno))
			break;
		if (combuf->pkgclass == 13 && combuf->d.emva.kilpno == kno)
			break;
		r = r % (SGJono->RowCount-1) + 1;
		}
	SGJono->Row = r;
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::Button3Click(TObject *Sender)
{
	char cbuf[2000];
	combufrec *combuf = (combufrec *) cbuf;
	osrec *ostiet = (osrec *) combuf->d.os.cos;
	int r, pos;

	if (SGJono->RowCount < 2)
		return;
	r = SGJono->Row % (SGJono->RowCount-1) + 1;
	pos = _wtoi(EdPos->Text.c_str());
	for	( ; r != SGJono->Row;) {
		fseek(ComFile, (r * NJono + Jono) * RecLen, SEEK_SET);
		if (fread(cbuf, RecLen, 1, ComFile) != 1)
			break;
		if (combuf->pkgclass > 1 && combuf->pkgclass < 6 && combuf->pkgclass != 4 &&
			combuf->d.k.dk == pos)
			break;
		r = r % (SGJono->RowCount-1) + 1;
		}
	SGJono->Row = r;
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

void __fastcall TFormComfile::BtnBadgehakuClick(TObject *Sender)
{
	char cbuf[2000];
	combufrec *combuf = (combufrec *) cbuf;
	osrec *ostiet = (osrec *) combuf->d.os.cos;
	int r, bdg;

	if (SGJono->RowCount < 2)
		return;
	r = SGJono->Row % (SGJono->RowCount-1) + 1;
	bdg = _wtoi(EdtBadge->Text.c_str());
	for	( ; r != SGJono->Row;) {
		fseek(ComFile, (r * NJono + Jono) * RecLen, SEEK_SET);
		if (fread(cbuf, RecLen, 1, ComFile) != 1)
			break;
		if (combuf->pkgclass == 6 && (((combuf->d.em.badge - bdg) % BADGEASKEL) == 0 ||
			combuf->d.em.badge0 == bdg))
			break;
		if (combuf->pkgclass == 2 && (((ostiet->badge[0]- bdg) % BADGEASKEL) == 0 ||
			((ostiet->badge[1]- bdg) % BADGEASKEL) == 0))
			break;
		if (combuf->pkgclass == 8 && combuf->d.ev.badge == bdg)
			break;
		if (combuf->pkgclass == 13 && combuf->d.emva.badge == bdg)
			break;
		r = r % (SGJono->RowCount-1) + 1;
		}
	SGJono->Row = r;
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::Button7Click(TObject *Sender)
{
	int vast;

	vast = select3(3, L"Valitse tiedostotyyppi", L"Valitse", L"Binaaritiedosto",
		L"Tekstitiedosto", L"Peruuta", this);
	if (vast == 3)
		return;
	if (vast == 1) {
		FILE *outfl = NULL;
		char cbuf[2000];
		SaveDialog1->DefaultExt = L"dat";
		SaveDialog1->Filter = L"DAT-tiedostot (*.dat)|*.DAT|Kaikki tiedostot (*.*)|*.*";
		if (SaveDialog1->Execute())
			outfl = _wfopen(SaveDialog1->FileName.c_str(), L"wb");
		if (outfl) {
			memset(cbuf, 0, sizeof(cbuf));
			cbuf[6] = 1;
			*(int *)(cbuf+10) = RecLen;
			fwrite(cbuf, RecLen, 1, outfl);
			for	(int r = 1; r < SGJono->RowCount; r++) {
				fseek(ComFile, (r * NJono + Jono) * RecLen, SEEK_SET);
				if (fread(cbuf, RecLen, 1, ComFile) != 1)
					break;
				fwrite(cbuf, RecLen, 1, outfl);
				}
			fclose(outfl);
			}
		}
	else {
		TextFl *outfl = NULL;
		wchar_t line[200];

		SaveDialog1->DefaultExt = L"txt";
		SaveDialog1->Filter = L"txt-tiedostot (*.txt)|*.txt|Kaikki tiedostot (*.*)|*.*";
		if (SaveDialog1->Execute())
			outfl = new TextFl(SaveDialog1->FileName.c_str(), L"wt");
		if (outfl && outfl->IsOpen()) {
			for	(int r = 1; r < SGJono->RowCount; r++) {
				line[0] = 0;
				for (int c = 0; c < SGJono->ColCount; c++) {
					if (c > 0) {
						wcscat(line, L"\t");
						}
					if (SGJono->Cells[c][r].Length() > 0)
						wcscat(line, SGJono->Cells[c][r].c_str());
					}
				wcscat(line, L"\n");
				outfl->WriteLine(line);
				}
			}
		if (outfl)
			delete outfl;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::FormShow(TObject *Sender)
{
	FormResize(Sender);
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
	FormComfile->ComFile = sendtestparam.ComFile;
	FormComfile->Jono = sendtestparam.Jono;
	FormComfile->NJono = sendtestparam.NJono;
	FormComfile->RecLen = sendtestparam.RecLen;
	GBLahetys->Visible = true;
	FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormComfile::RGValitClick(TObject *Sender)
{
	IkkParam.ValiLaji = RGValit->ItemIndex;
	if (RGValit->ItemIndex == 1) {
		EdtValit->EditLabel->Caption = L"Kerroin";
		EdtValit->Text = L"1,0";
		EdtAloitus->EditLabel->Caption = L"Vertailuaika";
		if (EdtAloitus->Text.Length() == 0 || wstrtoaika_vap(EdtAloitus->Text.c_str(), t0) == TMAALI0)
			EdtAloitus->Text = SGJono->Cells[2][1].SubString(1,8);
		IkkParam.VertAika = wstrtoaika_vap(EdtAloitus->Text.c_str(), t0);
		}
	else {
		EdtValit->EditLabel->Caption = L"Sanomaväli";
		EdtAloitus->EditLabel->Caption = L"Aloitushetki";
		if (EdtAloitus->Text.Length() == 0 || wstrtoaika_vap(EdtAloitus->Text.c_str(), t0) == TMAALI0)
			EdtAloitus->Text = wkello();
		}
	if (EdtLahAlku->Text.Length() == 0 || _wtoi(EdtLahAlku->Text.c_str()) == 0)
		EdtLahAlku->Text = 1;
	if (EdtLahLoppu->Text.Length() == 0 || _wtoi(EdtLahLoppu->Text.c_str()) == 0)
		EdtLahLoppu->Text = NJRec[Jono];
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
	fseek(ComFile, (r * NJono + Jono) * RecLen, SEEK_SET);
	if (fread(&combuf, RecLen, 1, ComFile) == 1) {
		lahetaKopiot(cn, combuf);
		r++;
		EdtLahAlku->Text = r;
		}
}
//---------------------------------------------------------------------------

MESSAGE void __fastcall TFormComfile::PaivHandler(TMyMessage &msg)
{
	EdtLahAlku->Text = sendtestparam.LahRivi;
	EdtLahAlku->Refresh();
}

//---------------------------------------------------------------------------
void SendCopyThread(LPVOID lpCn);

void __fastcall TFormComfile::BtnLahValClick(TObject *Sender)
{
	wchar_t *p, st[20] = L"";

	if (sendtestparam.inSendCopy) {
		sendtestparam.inSendCopy = false;
		return;
		}
	sendtestparam.inSendCopy = true;
	if (RGAloitus->ItemIndex == 0)
		EdtAloitus->Text = wkello();
	sendtestparam.valilaji = RGValit->ItemIndex;
	sendtestparam.cn = CBLahYhteys->ItemIndex - 1;
	sendtestparam.ens = _wtoi(EdtLahAlku->Text.c_str());
	if (EdtValit->Text.Length() > 0)
		wcsncpy(st, EdtValit->Text.c_str(), 19);
	p = wcsstr(st, L",");
	if (p)
		*p = L'.';
	sendtestparam.ValiKerroin = StrToDouble(st);
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
	sendtestparam.ComFile = FormComfile->ComFile;
	sendtestparam.Jono = FormComfile->Jono;
	sendtestparam.NJono = FormComfile->NJono;
	sendtestparam.RecLen = FormComfile->RecLen;
	IkkParam.ValiKerroin = sendtestparam.ValiKerroin;

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
}
//---------------------------------------------------------------------------

void naytaTestRivi(void)
{
	SendMessage(FormComfile->Handle,WM_MYPAIVITA,0,0);
}

void sendTestLopetus(void)
{
	FormComfile->EdtLahAlku->Text = sendtestparam.LahRivi;
	FormComfile->BtnLahVal->Caption = L"Lähetä valintojen mukaan";
}

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
	if (ComBuf.pkgclass == 5) {
		ComBuf.lahetetty = UStrToAika(SGSanoma->Cells[1][3], t0) / KSEK;
		}
	if (ComBuf.pkgclass == 6) {
		ComBuf.lahetetty = UStrToAika(SGSanoma->Cells[1][2], t0) / KSEK;
		}
	if (ComBuf.pkgclass == 8) {
		ComBuf.lahetetty = UStrToAika(SGSanoma->Cells[1][2], t0) / KSEK;
		}
	if (ComBuf.pkgclass == 9) {
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


