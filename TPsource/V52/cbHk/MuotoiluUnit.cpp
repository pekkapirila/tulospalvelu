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
#include <stdio.h>
#include <bstrings.h>

#include "MuotoiluUnit.h"
#include "WinHk.h"
#include "HkMuotoilu.h"
TFormMuotoilu *FormMuotoilu;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

static int Varit[2][8][2];
static struct {
	wchar_t nimi[20];
	int vari;
	} OlVarit[40] = {
	{L"Valkea", 0xffffff},
	{L"Vaalea-1", 0xffffd0},
	{L"Vaalea-2", 0xffd0ff},
	{L"Vaalea-3", 0xc0ffff},
	{L"Vaalea-4", 0xd8e0ff},
	{L"Vaalea-5", 0xffd8d8},
	{L"Vaalea-6", 0x48ffd8},
	{L"Vaalea-7", 0xffffe8},
	{L"Musta", clBlack},
	{L"Sininen", clBlue},
	{L"Vihre‰", clGreen},
	{L"Punainen", clRed},
	{L"Keltainen", clYellow},
	{L"Syaani", clLime},
	{L"Turkoosi", clAqua},
	{L"Magenta", 0xff00ff},
	{L"Harmaa", 0xa0a0a0},
	{L"VaalHarmaa", 0xd8d8d8}
	};

static int omaVariNo;

//---------------------------------------------------------------------------
__fastcall TFormMuotoilu::TFormMuotoilu(TComponent* Owner)
   : TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	SGTls[0] = SGtlsKirj;
	SGTls[1] = SGtlsMKirj;
	SGTls[2] = SGtls1Kirj;
	SGTls[3] = SGtlsTied;
	SGTls[4] = SGtlsMobil;
	SGLl[0] = SGllKirj;
	SGLl[1] = SGllTied;
}

static int switchColors(int col)
{
	return(65536*(col & 0x000000ff) + (col & 0x0000ff00) + (col & 0x00ff0000) / 65536);
}

int TFormMuotoilu::LisaaOmaVari(int vari)
{
	int cl;

	for (cl = 0; cl < sizeof(OlVarit)/sizeof(OlVarit[0]) && OlVarit[cl].nimi[0] != 0; cl++) ;

	if (cl < sizeof(OlVarit)/sizeof(OlVarit[0])) {
		swprintf(OlVarit[cl].nimi, L"Oma-%d", ++omaVariNo);
		OlVarit[cl].vari = vari;
		for (int i = 0; i < 2; i++)
			for (int j = 0; j < 2; j++)
				ClBx[i][j]->Items->AddObject(OlVarit[cl].nimi, reinterpret_cast<TObject*>(OlVarit[cl].vari));
		}
	else cl = 0;
	return(cl);
}

static int haeVariNo(int vari)
{
	for (int cl = 0; cl < sizeof(OlVarit)/sizeof(OlVarit[0]) && OlVarit[cl].nimi[0] != 0; cl++) {
		if (OlVarit[cl].vari == vari)
			return(cl);
		}
	return(-1);
}

//---------------------------------------------------------------------------
void __fastcall TFormMuotoilu::NaytaArvot(TObject *Sender)
{
   Edtpaaots->Text = UnicodeString(paaots);
   Edtalaots->Text = UnicodeString(sarjaots);
   Edtalaots2->Text = UnicodeString(alaots);

   EdtSivPit->Text = tulosmuot.sivpit;
   Edtesirivit->Text = tulosmuot.esirivit;
   EdtVasMarg->Text = tulosmuot.marg;
   CBsnimiflKirj->Checked = tulosmuot.snimifl;
   CBtpvaflKirj->Checked = tulosmuot.tpvafl;
   CBtkloflKirj->Checked = tulosmuot.tklofl;
   CBlkmKirj->Checked = tulosmuot.lkmfl;
   EdMtkKirj->Text = tulosmuot.matkafl;
   CBotsikotKirj->Checked = tulosmuot.otsikot & 1;
   CBvaihdanimetKirj->Checked = tulosmuot.vaihdanimet;
   CBnollakarkiKirj->Checked = tulosmuot.nollakarki;
   EdtAlaMarg->Text = tulosmuot.amarg;
   EdtYlaMarg->Text = tulosmuot.ymarg;
   EdtPisteDesim->Text = tulosmuot.pistedesim;
   CBDesSep->ItemIndex = tulosmuot.des_sep == L'.' ? 1 : 0;

   CBsnimiflTied->Checked = filetulosmuot.snimifl;
   CBtpvaflTied->Checked = filetulosmuot.tpvafl;
   CBtkloflTied->Checked = filetulosmuot.tklofl;
   CBlkmTied->Checked = filetulosmuot.lkmfl;
   EdMtkTied->Text = filetulosmuot.matkafl;
   CBotsikotTied->Checked = filetulosmuot.otsikot & 1;
   CBautoOtsTied->Checked = filetulosmuot.otsikot & 2;
   CBvaihdanimetTied->Checked = filetulosmuot.vaihdanimet;
   CBnollakarkiTied->Checked = filetulosmuot.nollakarki;
   CBffTied->ItemIndex = filetulosmuot.fontfamily;
   CBfszTied->ItemIndex = filetulosmuot.fontsize;
	CBtaulHtmlTied->Checked = filetulosmuot.tauluhtml;
	CBSarLevTied->Enabled = filetulosmuot.tauluhtml;
	CBSarLevTied->ItemIndex = (filetulosmuot.tauluhtml & 0x6) / 2;
//	CBsuhtTied->Checked = filetulosmuot.tauluhtml == 2;
	RGlinkitTied->ItemIndex = filetulosmuot.sarjalinkit;
	RGcssTied->ItemIndex = filetulosmuot.cssfl;
	EdtVariRivitFile->Text = filetulosmuot.riviryhma;
	EdtPaaOtsKokoHtml->Text = filetulosmuot.paaotskoko;
	EdtAlaOtsKokoHtml->Text = filetulosmuot.alaotskoko;
	EdtViivatHtml->Text = filetulosmuot.border;
	EdtRVaKokoHtml->Text = filetulosmuot.efontsize;
	EdtRvaViivatHtml->Text = filetulosmuot.eborder;
	EdtHPadHtml->Text = filetulosmuot.hpad;
	EdtVPadHtml->Text = filetulosmuot.vpad;
	CBSarOtsLihHtml->Checked = filetulosmuot.sarotsbold;
	Varit[0][0][0] = switchColors(filetulosmuot.bgcolor);
	Varit[0][0][1] = switchColors(filetulosmuot.fontcolor);
	Varit[0][1][0] = switchColors(filetulosmuot.td1color);
	Varit[0][1][1] = switchColors(filetulosmuot.td1fontcolor);
	Varit[0][2][0] = switchColors(filetulosmuot.td2color);
	Varit[0][2][1] = switchColors(filetulosmuot.td2fontcolor);
	Varit[0][3][0] = switchColors(filetulosmuot.thcolor);
	Varit[0][3][1] = switchColors(filetulosmuot.thfontcolor);
	Varit[0][4][1] = switchColors(filetulosmuot.bordercolor);
	Varit[0][5][0] = switchColors(filetulosmuot.framecolor);
	Varit[0][5][1] = switchColors(filetulosmuot.framefontcolor);
	Varit[0][6][0] = switchColors(filetulosmuot.tdecolor);
	Varit[0][6][1] = switchColors(filetulosmuot.tdefontcolor);
	Varit[0][7][0] = switchColors(filetulosmuot.thecolor);
	Varit[0][7][1] = switchColors(filetulosmuot.thefontcolor);
	Varit[0][4][0] = Varit[0][1][0];

   CBsnimiflMobil->Checked = mobiltulosmuot.snimifl;
   CBtpvaflMobil->Checked = mobiltulosmuot.tpvafl;
   CBtkloflMobil->Checked = mobiltulosmuot.tklofl;
   CBlkmMobil->Checked = mobiltulosmuot.lkmfl;
   EdMtkMobil->Text = mobiltulosmuot.matkafl;
   CBotsikotMobil->Checked = mobiltulosmuot.otsikot & 1;
   CBautoOtsMobil->Checked = mobiltulosmuot.otsikot & 2;
   CBvaihdanimetMobil->Checked = mobiltulosmuot.vaihdanimet;
   CBnollakarkiMobil->Checked = mobiltulosmuot.nollakarki;
   CBffMobil->ItemIndex = mobiltulosmuot.fontfamily;
   CBfszMobil->ItemIndex = mobiltulosmuot.fontsize;
	CBtaulHtmlMobil->Checked = mobiltulosmuot.tauluhtml;
	CBSarLevMobil->Enabled = mobiltulosmuot.tauluhtml;
	CBSarLevMobil->ItemIndex = (mobiltulosmuot.tauluhtml & 0x6) / 2;
//	CBsuhtMobil->Checked = mobiltulosmuot.tauluhtml == 2;
	RGlinkitMobil->ItemIndex = mobiltulosmuot.sarjalinkit;
	RGcssMobil->ItemIndex = mobiltulosmuot.cssfl;
	EdtVariRivitMobil->Text = mobiltulosmuot.riviryhma;
	EdtPaaOtsKokoMobil->Text = mobiltulosmuot.paaotskoko;
	EdtAlaOtsKokoMobil->Text = mobiltulosmuot.alaotskoko;
	EdtViivatMobil->Text = mobiltulosmuot.border;
	EdtRVaKokoMobil->Text = mobiltulosmuot.efontsize;
	EdtRvaViivatMobil->Text = mobiltulosmuot.eborder;
	EdtHPadMobil->Text = mobiltulosmuot.hpad;
	EdtVPadMobil->Text = mobiltulosmuot.vpad;
	CBSarOtsLihMobil->Checked = mobiltulosmuot.sarotsbold;

	Varit[1][0][0] = switchColors(mobiltulosmuot.bgcolor);
	Varit[1][0][1] = switchColors(mobiltulosmuot.fontcolor);
	Varit[1][1][0] = switchColors(mobiltulosmuot.td1color);
	Varit[1][1][1] = switchColors(mobiltulosmuot.td1fontcolor);
	Varit[1][2][0] = switchColors(mobiltulosmuot.td2color);
	Varit[1][2][1] = switchColors(mobiltulosmuot.td2fontcolor);
	Varit[1][3][0] = switchColors(mobiltulosmuot.thcolor);
	Varit[1][3][1] = switchColors(mobiltulosmuot.thfontcolor);
	Varit[1][4][1] = switchColors(mobiltulosmuot.bordercolor);
	Varit[1][5][0] = switchColors(mobiltulosmuot.framecolor);
	Varit[1][5][1] = switchColors(mobiltulosmuot.framefontcolor);
	Varit[1][4][0] = Varit[1][1][0];
	Varit[1][6][0] = switchColors(mobiltulosmuot.tdecolor);
	Varit[1][6][1] = switchColors(mobiltulosmuot.tdefontcolor);
	Varit[1][7][0] = switchColors(mobiltulosmuot.thecolor);
	Varit[1][7][1] = switchColors(mobiltulosmuot.thefontcolor);

   CBPOfont->ItemIndex = TitleFnt.fontfamily;
   EdtPOfsize->Text = UnicodeString(TitleFnt.size);
   EdtPOrv->Text = UnicodeString(TitleFnt.line);
   CBPOlaatu->ItemIndex = TitleFnt.bold + 2*TitleFnt.italic;
   CBAOfont->ItemIndex = HeaderFnt.fontfamily;
   EdtAOfsize->Text = UnicodeString(HeaderFnt.size);
   EdtAOrv->Text = UnicodeString(HeaderFnt.line);
   CBAOlaatu->ItemIndex = HeaderFnt.bold + 2*HeaderFnt.italic;
   CBDeffont->ItemIndex = DefFnt.fontfamily;
   EdtDeffsize->Text = UnicodeString(DefFnt.size);
   EdtDefrv->Text = UnicodeString(DefFnt.line);
   CBDeflaatu->ItemIndex = DefFnt.bold + 2*DefFnt.italic;
   CBVakfont->ItemIndex = CourFnt.fontfamily-4;
   EdtVakfsize->Text = UnicodeString(120/CourFnt.pitch);
   EdtVakrv->Text = UnicodeString(CourFnt.line);
   CBVaklaatu->ItemIndex = CourFnt.bold + 2*CourFnt.italic;
//   RadioGroup1->ItemIndex = wcswcind(merkit, L"IRWAP");

	for (int ii = 0; ii < 2; ii++) {
		for (int ij = 0; ij < 6; ij++) {
			for (int fg = 0; fg < 2; fg++) {
				int ic = haeVariNo(Varit[ii][ij][fg]);
				if (ic < 0)
					LisaaOmaVari(Varit[ii][ij][fg]);
				}
			}
		}
	ClBx[0][0]->ItemIndex = haeVariNo(Varit[0][CBVariKohdeHtml->ItemIndex][0])+1;
	ClBx[0][1]->ItemIndex = haeVariNo(Varit[0][CBVariKohdeHtml->ItemIndex][1])+1;
	ClBx[1][0]->ItemIndex = haeVariNo(Varit[1][CBVariKohdeHtml->ItemIndex][0])+1;
	ClBx[1][1]->ItemIndex = haeVariNo(Varit[1][CBVariKohdeHtml->ItemIndex][1])+1;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::Poistu1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::naytaLl(int lj)
{
	int rowCount = 19;

	SGLl[lj]->ColCount = 22;
	SGLl[lj]->RowCount = rowCount;
	SGLl[lj]->ColWidths[0] = 60 * Screen->PixelsPerInch / 96;
	SGLl[lj]->Cells[0][0] = L"Kentt‰";
	SGLl[lj]->Cells[1][0] = L"Sarj.";
	SGLl[lj]->Cells[2][0] = L"Aakk.";
	SGLl[lj]->Cells[3][0] = L"Seur.";
	SGLl[lj]->Cells[4][0] = L"L‰hd.";
	SGLl[lj]->Cells[5][0] = L"Srj-2";
	SGLl[lj]->Cells[6][0] = L"Aak-2";
	SGLl[lj]->Cells[7][0] = L"Seu-2";
	if (lj == 0) {
		EdtLots0->Text = UnicodeString(llparam.luetots0);
		EdtLots1->Text = UnicodeString(llparam.luetots1);
		EdtLots21->Text = UnicodeString(llparam.luetots21);
		EdtLots22->Text = UnicodeString(llparam.luetots22);
		EdtLots3->Text = UnicodeString(llparam.luetots3);
		}
	for (int i = 1; i <= 7; i++) {
		SGLl[lj]->Cells[i+7][0] = SGLl[lj]->Cells[i][0];
		SGLl[lj]->Cells[i+14][0] = SGLl[lj]->Cells[i][0];
		SGLl[lj]->ColWidths[i] = 32 * Screen->PixelsPerInch / 96;
		SGLl[lj]->ColWidths[i+7] = 32 * Screen->PixelsPerInch / 96;
		SGLl[lj]->ColWidths[i+14] = 32 * Screen->PixelsPerInch / 96;
		}
	SGLl[lj]->Cells[0][rowCount-3] = L"Aikap/Marg";
	SGLl[lj]->Cells[0][rowCount-2] = L"Pv/Klo/Mtk";
	SGLl[lj]->Cells[0][rowCount-1] = L"Etunm ens";
//	SGLl[lj]->Cells[0][rowCount-1] = L"Seura/Etunm";
	for (int r = 1; r < rowCount; r++) {
		if (r < rowCount-3)
			SGLl[lj]->Cells[0][r] = UnicodeString(tulkentta[lj][r-1].nimi);
		for (int i = 1; i <= 7; i++) {
			if (r < rowCount-3) {
				SGLl[lj]->Cells[i][r] = UnicodeString(tulkentta[lj][r-1].pos[i-1]);
				SGLl[lj]->Cells[i+7][r] = UnicodeString(tulkentta[lj][r-1].len[i-1]);
				SGLl[lj]->Cells[i+14][r] = tulkentta[lj][r-1].tas[i-1] ? L"Oik" : L"Vas";
				}
			if (r == rowCount-3) {
				if (i > 4)
					SGLl[lj]->Cells[i][r] = UnicodeString(llparam.aikaporras[lj][i-5]);
				else
					SGLl[lj]->Cells[i][r] = L"";
				SGLl[lj]->Cells[i+7][r] = UnicodeString(llparam.marg[lj][i-1]);
				}
			if (r == rowCount-2) {
				SGLl[lj]->Cells[i][r] = UnicodeString(llparam.pvfl[lj][i-1]);
				SGLl[lj]->Cells[i+7][r] = UnicodeString(llparam.klfl[lj][i-1]);
				SGLl[lj]->Cells[i+14][r] = UnicodeString(llparam.matkafl[lj][i-1]);
				}
			if (r == rowCount-1) {
				SGLl[lj]->Cells[i][r] = UnicodeString(llparam.vaihdanimet[lj][i-1]);
				SGLl[lj]->Cells[i+7][r] = L"";
				SGLl[lj]->Cells[i+14][r] = L"";
				}
			}
		}
	SGLl[lj]->Options << goEditing;
}
//---------------------------------------------------------------------------
void __fastcall TFormMuotoilu::naytaLlKirj(void)
{
	naytaLl(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormMuotoilu::naytaLlTied(void)
{
	naytaLl(1);
}
//---------------------------------------------------------------------------
void __fastcall TFormMuotoilu::naytaTls(int lj)
{
	SGTls[lj]->ColCount = lj == 0 ? 5 : 6;
	SGTls[lj]->RowCount = n_frmflds[lj]+1;
	SGTls[lj]->ColWidths[0] = 60 * Screen->PixelsPerInch / 96;
	SGTls[lj]->ColWidths[1] = 60 * Screen->PixelsPerInch / 96;
	SGTls[lj]->Cells[0][0] = L"Kentt‰";
	SGTls[lj]->Cells[1][0] = L"Otsikko";
	SGTls[lj]->Cells[2][0] = L"Sij.";
	SGTls[lj]->Cells[3][0] = L"Merkk.";
	SGTls[lj]->Cells[4][0] = L"Tas.";
	if (lj != 0)
		SGTls[lj]->Cells[5][0] = L"Askel";
	for (int i = 2; i < SGTls[lj]->ColCount; i++) {
		SGTls[lj]->ColWidths[i] = (lj == 0 ? 50 : 36) * Screen->PixelsPerInch / 96;
		}
	for (int r = 1; r <= n_frmflds[lj]; r++) {
		SGTls[lj]->Cells[0][r] = UnicodeString(frmflds[lj][r-1].name);
		SGTls[lj]->Cells[1][r] = UnicodeString(frmflds[lj][r-1].ots);
		SGTls[lj]->Cells[2][r] = UnicodeString(frmflds[lj][r-1].pos);
		SGTls[lj]->Cells[3][r] = UnicodeString(frmflds[lj][r-1].len);
		SGTls[lj]->Cells[4][r] = frmflds[lj][r-1].oik ? L"Oik" : L"Vas";
		if (lj != 0)
			SGTls[lj]->Cells[5][r] = UnicodeString(frmflds[lj][r-1].askel);
		}
	SGTls[lj]->Options << goEditing;
}
//---------------------------------------------------------------------------
void __fastcall TFormMuotoilu::naytaTlsKirj(void)
{
	naytaTls(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormMuotoilu::naytaTlsMKirj(void)
{
	naytaTls(1);
}
//---------------------------------------------------------------------------
void __fastcall TFormMuotoilu::naytaTls1Kirj(void)
{
	naytaTls(2);
}
//---------------------------------------------------------------------------
void __fastcall TFormMuotoilu::naytaTlsTied(void)
{
	naytaTls(3);
}
//---------------------------------------------------------------------------
void __fastcall TFormMuotoilu::naytaTlsMobil(void)
{
	naytaTls(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BtnOta1Click(TObject *Sender)
{
   GDIfontTp Fnt;

   tulosmuot.sivpit = EdtSivPit->Text.ToInt();
   tulosmuot.esirivit = Edtesirivit->Text.ToInt();
   tulosmuot.snimifl = CBsnimiflKirj->Checked ? 1 : 0;
   tulosmuot.tpvafl = CBtpvaflKirj->Checked ? 1 : 0;
   tulosmuot.tklofl = CBtkloflKirj->Checked ? 1 : 0;
   tulosmuot.lkmfl = CBlkmKirj->Checked ? 1 : 0;
   tulosmuot.matkafl = _wtoi(EdMtkKirj->Text.c_str());
   tulosmuot.otsikot = (tulosmuot.otsikot & 2) | (CBotsikotKirj->Checked ? 1 : 0);
   tulosmuot.vaihdanimet = CBvaihdanimetKirj->Checked ? 1 : 0;
   tulosmuot.nollakarki = CBnollakarkiKirj->Checked ? 1 : 0;
   tulosmuot.marg = EdtVasMarg->Text.ToInt();
   tulosmuot.amarg = _wtoi(EdtAlaMarg->Text.c_str());
   tulosmuot.ymarg = _wtoi(EdtYlaMarg->Text.c_str());
   tulosmuot.pistedesim = _wtoi(EdtPisteDesim->Text.c_str());
   if (tulosmuot.pistedesim < 0)
	   tulosmuot.pistedesim = 0;
   tulosmuot.des_sep = L",,."[CBDesSep->ItemIndex+1];

   lsttulparam.tulmuot = tulosmuot;
   autotulostusparam.tulmuot = tulosmuot;

   filetulosmuot.pistedesim = tulosmuot.pistedesim;
   mobiltulosmuot.pistedesim = tulosmuot.pistedesim;
   filetulosmuot.des_sep = tulosmuot.des_sep;
   mobiltulosmuot.des_sep = tulosmuot.des_sep;

   filetulosmuot.snimifl = CBsnimiflTied->Checked ? 1 : 0;
   filetulosmuot.tpvafl = CBtpvaflTied->Checked ? 1 : 0;
   filetulosmuot.tklofl = CBtkloflTied->Checked ? 1 : 0;
   filetulosmuot.lkmfl = CBlkmTied->Checked ? 1 : 0;
   filetulosmuot.matkafl = _wtoi(EdMtkTied->Text.c_str());
   filetulosmuot.otsikot = (CBautoOtsTied->Checked ? 2 : 0) | (CBotsikotTied->Checked ? 1 : 0);
   filetulosmuot.vaihdanimet = CBvaihdanimetTied->Checked ? 1 : 0;
   filetulosmuot.nollakarki = CBnollakarkiTied->Checked ? 1 : 0;

   mobiltulosmuot.snimifl = CBsnimiflMobil->Checked ? 1 : 0;
   mobiltulosmuot.tpvafl = CBtpvaflMobil->Checked ? 1 : 0;
   mobiltulosmuot.tklofl = CBtkloflMobil->Checked ? 1 : 0;
   mobiltulosmuot.lkmfl = CBlkmMobil->Checked ? 1 : 0;
   mobiltulosmuot.matkafl = _wtoi(EdMtkMobil->Text.c_str());
   mobiltulosmuot.otsikot = (CBautoOtsMobil->Checked ? 2 : 0) | (CBotsikotMobil->Checked ? 1 : 0);
   mobiltulosmuot.vaihdanimet = CBvaihdanimetMobil->Checked ? 1 : 0;
   mobiltulosmuot.nollakarki = CBnollakarkiMobil->Checked ? 1 : 0;

   wcsncpy(paaots, Edtpaaots->Text.c_str(), 80);
   wcsncpy(sarjaots, Edtalaots->Text.c_str(), 80);
   wcsncpy(alaots, Edtalaots2->Text.c_str(), 119);

   Fnt = TitleFnt;
   TitleFnt.fontfamily = CBPOfont->ItemIndex;
   TitleFnt.size = EdtPOfsize->Text.ToInt();
   TitleFnt.line = EdtPOrv->Text.ToInt();
   TitleFnt.bold = CBPOlaatu->ItemIndex % 2;
   TitleFnt.italic = CBPOlaatu->ItemIndex / 2;
   if (memcmp(&Fnt, &TitleFnt, sizeof(GDIfontTp)))
	   TitleFnt.hfont = NULL;
   Fnt = HeaderFnt;
   HeaderFnt.fontfamily = CBAOfont->ItemIndex;
   HeaderFnt.size = EdtAOfsize->Text.ToInt();
   HeaderFnt.line = EdtAOrv->Text.ToInt();
   HeaderFnt.bold = CBAOlaatu->ItemIndex % 2;
   HeaderFnt.italic = CBAOlaatu->ItemIndex / 2;
   if (memcmp(&Fnt, &HeaderFnt, sizeof(GDIfontTp)))
	   HeaderFnt.hfont = NULL;
   Fnt = DefFnt;
   DefFnt.fontfamily = CBDeffont->ItemIndex;
   DefFnt.size = EdtDeffsize->Text.ToInt();
   DefFnt.line = EdtDefrv->Text.ToInt();
   DefFnt.bold = CBDeflaatu->ItemIndex % 2;
   DefFnt.italic = CBDeflaatu->ItemIndex / 2;
   if (memcmp(&Fnt, &DefFnt, sizeof(GDIfontTp)))
	   DefFnt.hfont = NULL;
   Fnt = CourFnt;
   CourFnt.fontfamily = CBVakfont->ItemIndex+4;
   CourFnt.pitch = 120/EdtVakfsize->Text.ToInt();
   CourFnt.line = EdtVakrv->Text.ToInt();
   CourFnt.bold = CBVaklaatu->ItemIndex % 2;
   CourFnt.italic = CBVaklaatu->ItemIndex / 2;
   if (memcmp(&Fnt, &CourFnt, sizeof(GDIfontTp)))
	   CourFnt.hfont = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BtnOtaLl1Click(TObject *Sender)
{
	int rowCount = SGllKirj->RowCount;
	wcsncpy(llparam.luetots0, EdtLots0->Text.c_str(), sizeof(llparam.luetots0)/2-1);
	wcsncpy(llparam.luetots1, EdtLots1->Text.c_str(), sizeof(llparam.luetots1)/2-1);
	wcsncpy(llparam.luetots21, EdtLots21->Text.c_str(), sizeof(llparam.luetots21)/2-1);
	wcsncpy(llparam.luetots22, EdtLots22->Text.c_str(), sizeof(llparam.luetots22)/2-1);
	wcsncpy(llparam.luetots3, EdtLots3->Text.c_str(), sizeof(llparam.luetots3)/2-1);
	for (int r = 1; r < rowCount; r++) {
		if (r < rowCount-3)
			wcsncpy(tulkentta1[r-1].nimi, SGllKirj->Cells[0][r].c_str(), sizeof(tulkentta1[r-1].nimi)/2-1);
		for (int i = 1; i <= 7; i++) {
			if (r < rowCount-3) {
				tulkentta1[r-1].pos[i-1] = SGllKirj->Cells[i][r].ToInt();
				tulkentta1[r-1].len[i-1] = SGllKirj->Cells[i+7][r].ToInt();
				tulkentta1[r-1].tas[i-1] = towupper(*(SGllKirj->Cells[i+14][r].c_str())) == L'O';
				}
			if (r == rowCount-3) {
				if (i > 4)
					llparam.aikaporras[0][i-5] = SGllKirj->Cells[i][r].ToInt();
				llparam.marg[0][i-1] = SGllKirj->Cells[i+7][r].ToInt();
				}
			if (r == rowCount-2) {
				llparam.pvfl[0][i-1] = SGllKirj->Cells[i][r].ToInt();
				llparam.klfl[0][i-1] = SGllKirj->Cells[i+7][r].ToInt();
				llparam.matkafl[0][i-1] = SGllKirj->Cells[i+14][r].ToInt();
				}
			if (r == rowCount-1) {
				llparam.vaihdanimet[0][i-1] = SGllKirj->Cells[i][r].ToInt();
				}
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BtnOtaLl2Click(TObject *Sender)
{
	int rowCount = SGllTied->RowCount;
	for (int r = 1; r < rowCount; r++) {
		if (r < rowCount-3)
			wcsncpy(tulkentta2[r-1].nimi, SGllTied->Cells[0][r].c_str(), sizeof(tulkentta2[r-1].nimi)/2-1);
		for (int i = 1; i <= 7; i++) {
			if (r < rowCount-3) {
				tulkentta2[r-1].pos[i-1] = SGllTied->Cells[i][r].ToInt();
				tulkentta2[r-1].len[i-1] = SGllTied->Cells[i+7][r].ToInt();
				tulkentta2[r-1].tas[i-1] = towupper(*(SGllTied->Cells[i+14][r].c_str())) == L'O';
				}
			if (r == rowCount-3) {
				if (i > 4)
					llparam.aikaporras[1][i-5] = SGllTied->Cells[i][r].ToInt();
				llparam.marg[1][i-1] = SGllTied->Cells[i+7][r].ToInt();
				}
			if (r == rowCount-2) {
				llparam.pvfl[1][i-1] = SGllTied->Cells[i][r].ToInt();
				llparam.klfl[1][i-1] = SGllTied->Cells[i+7][r].ToInt();
				llparam.matkafl[1][i-1] = SGllTied->Cells[i+14][r].ToInt();
				}
			if (r == rowCount-1) {
				llparam.vaihdanimet[1][i-1] = SGllTied->Cells[i][r].ToInt();
				}
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BtnLue1Click(TObject *Sender)
{
	OpenDialog1->InitialDir = FormMain->CurrentDir;
	OpenDialog1->DefaultExt = L"xml";
	OpenDialog1->Filter = L"XML-tiedostot (*.xml)|*.xml|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		LueXMLMuotoilu(OpenDialog1->FileName.c_str());
		}
	SetCurrentDir(FormMain->CurrentDir);
	naytaTlsKirj();
	naytaTlsTied();
	naytaTlsMobil();
	naytaTlsMKirj();
	naytaTls1Kirj();
	naytaLlTied();
	naytaLlKirj();
	NaytaArvot(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BtnKirj1Click(TObject *Sender)
{
	BtnOta1Click(Sender);
	BtnOtaTul1Click(Sender);
	BtnOtaTul2Click(Sender);
	BtnOtaLl1Click(Sender);
	BtnOtaLl2Click(Sender);
	SaveDialog1->InitialDir = FormMain->CurrentDir;
	SaveDialog1->DefaultExt = L"xml";
	SaveDialog1->Filter = L"XML-tiedostot (*.xml)|*.xml|Kaikki tiedostot (*.*)|*.*";
	if (SaveDialog1->Execute()) {
		TallXMLMuotoilu(SaveDialog1->FileName.c_str());
		}
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::Luevanhatulosmuotoilu1Click(TObject *Sender)
{
	OpenDialog1->InitialDir = FormMain->CurrentDir;
	OpenDialog1->Filter = L"Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		lue_muotoilu(OpenDialog1->FileName.c_str(), 0);
		}
	SetCurrentDir(FormMain->CurrentDir);
	naytaTlsKirj();
	naytaTlsTied();
	naytaTlsMobil();
	naytaTlsMKirj();
	naytaTls1Kirj();
	NaytaArvot(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormMuotoilu::BtnLueLl1Click(TObject *Sender)
{
	OpenDialog1->InitialDir = FormMain->CurrentDir;
	OpenDialog1->Filter = L"Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		lue_ll_muotoilu(OpenDialog1->FileName.c_str(), 0);
		}
	SetCurrentDir(FormMain->CurrentDir);
	naytaLlTied();
	naytaLlKirj();
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BtnKirjLl1Click(TObject *Sender)
{
	BtnOtaLl1Click(Sender);
	SaveDialog1->InitialDir = FormMain->CurrentDir;
	SaveDialog1->Filter = L"Kaikki tiedostot (*.*)|*.*";
	if (SaveDialog1->Execute()) {
		kirj_ll_muotoilu(SaveDialog1->FileName.c_str());
		}
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BtnLueLl2Click(TObject *Sender)
{
	OpenDialog1->InitialDir = FormMain->CurrentDir;
	OpenDialog1->Filter = L"Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		lue_ll_muotoilu(OpenDialog1->FileName.c_str(), 0);
		}
	SetCurrentDir(FormMain->CurrentDir);
	naytaLlTied();
	naytaLlKirj();
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BtnKirjLl2Click(TObject *Sender)
{
	SaveDialog1->InitialDir = FormMain->CurrentDir;
	BtnOtaLl2Click(Sender);
	SaveDialog1->Filter = L"Kaikki tiedostot (*.*)|*.*";
	if (SaveDialog1->Execute()) {
		kirj_ll_muotoilu(SaveDialog1->FileName.c_str());
		}
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BtnOtaTul1Click(TObject *Sender)
{
	for (int r = 1; r < SGtlsKirj->RowCount; r++) {
		wcsncpy(prtflds[r-1].ots, SGtlsKirj->Cells[1][r].c_str(), sizeof(prtflds[r-1].ots)/2-1);
		prtflds[r-1].pos = _wtoi(SGtlsKirj->Cells[2][r].c_str());
		prtflds[r-1].len = _wtoi(SGtlsKirj->Cells[3][r].c_str());
		prtflds[r-1].akt = prtflds[r-1].len != 0;
		prtflds[r-1].oik = towupper(*(SGtlsKirj->Cells[4][r].c_str())) == L'O';
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BtnOtaTul2Click(TObject *Sender)
{
	for (int r = 1; r < SGtlsMKirj->RowCount; r++) {
		wcsncpy(prtfldsm[r-1].ots, SGtlsMKirj->Cells[1][r].c_str(), sizeof(prtfldsm[r-1].ots)/2-1);
		prtfldsm[r-1].pos = _wtoi(SGtlsMKirj->Cells[2][r].c_str());
		prtfldsm[r-1].len = _wtoi(SGtlsMKirj->Cells[3][r].c_str());
		prtfldsm[r-1].akt = prtfldsm[r-1].len != 0;
		prtfldsm[r-1].oik = towupper(*(SGtlsMKirj->Cells[4][r].c_str())) == L'O';
		prtfldsm[r-1].askel = _wtoi(SGtlsMKirj->Cells[5][r].c_str());
		}
	for (int r = 1; r < SGtls1Kirj->RowCount; r++) {
		wcsncpy(prtflds1[r-1].ots, SGtls1Kirj->Cells[1][r].c_str(), sizeof(prtflds1[r-1].ots)/2-1);
		prtflds1[r-1].pos = _wtoi(SGtls1Kirj->Cells[2][r].c_str());
		prtflds1[r-1].len = _wtoi(SGtls1Kirj->Cells[3][r].c_str());
		prtflds1[r-1].akt = prtflds1[r-1].len != 0;
		prtflds1[r-1].oik = towupper(*(SGtls1Kirj->Cells[4][r].c_str())) == L'O';
		prtflds1[r-1].askel = _wtoi(SGtls1Kirj->Cells[5][r].c_str());
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BtnOtaTul3Click(TObject *Sender)
{
	for (int r = 1; r < SGtlsTied->RowCount; r++) {
		wcsncpy(fileflds[r-1].ots, SGtlsTied->Cells[1][r].c_str(), sizeof(fileflds[r-1].ots)/2-1);
		fileflds[r-1].pos = _wtoi(SGtlsTied->Cells[2][r].c_str());
		fileflds[r-1].len = _wtoi(SGtlsTied->Cells[3][r].c_str());
		fileflds[r-1].akt = fileflds[r-1].len != 0;
		fileflds[r-1].oik = towupper(*(SGtlsTied->Cells[4][r].c_str())) == L'O';
		fileflds[r-1].askel = _wtoi(SGtlsTied->Cells[5][r].c_str());
		}
	for (int r = 1; r < SGtlsMobil->RowCount; r++) {
		wcsncpy(mobilflds[r-1].ots, SGtlsMobil->Cells[1][r].c_str(), sizeof(mobilflds[r-1].ots)/2-1);
		mobilflds[r-1].pos = _wtoi(SGtlsMobil->Cells[2][r].c_str());
		mobilflds[r-1].len = _wtoi(SGtlsMobil->Cells[3][r].c_str());
		mobilflds[r-1].akt = mobilflds[r-1].len != 0;
		mobilflds[r-1].oik = towupper(*(SGtlsMobil->Cells[4][r].c_str())) == L'O';
		mobilflds[r-1].askel = _wtoi(SGtlsMobil->Cells[5][r].c_str());
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::Help1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Muotoilu");
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::SGllKirjMouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGllKirj->RowCount - SGllKirj->FixedRows >= SGllKirj->VisibleRowCount + SGllKirj->TopRow)
		SGllKirj->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::SGllKirjMouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGllKirj->TopRow > SGllKirj->FixedRows)
		SGllKirj->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::SGllTiedMouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGllTied->RowCount - SGllTied->FixedRows >= SGllTied->VisibleRowCount + SGllTied->TopRow)
		SGllTied->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::SGllTiedMouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGllTied->TopRow > SGllTied->FixedRows)
		SGllTied->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::SGtlsKirjMouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGtlsKirj->RowCount - SGtlsKirj->FixedRows >= SGtlsKirj->VisibleRowCount + SGtlsKirj->TopRow)
		SGtlsKirj->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::SGtlsKirjMouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGtlsKirj->TopRow > SGtlsKirj->FixedRows)
		SGtlsKirj->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::SGtlsTiedMouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGtlsTied->RowCount - SGtlsTied->FixedRows >= SGtlsTied->VisibleRowCount + SGtlsTied->TopRow)
		SGtlsTied->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::SGtlsTiedMouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGtlsTied->TopRow > SGtlsTied->FixedRows)
		SGtlsTied->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::SGtlsMobilMouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGtlsMobil->RowCount - SGtlsMobil->FixedRows >= SGtlsMobil->VisibleRowCount + SGtlsMobil->TopRow)
		SGtlsMobil->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::SGtlsMobilMouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGtlsMobil->TopRow > SGtlsMobil->FixedRows)
		SGtlsMobil->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::SGtlsMKirjMouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGtlsMKirj->RowCount - SGtlsMKirj->FixedRows >= SGtlsMKirj->VisibleRowCount + SGtlsMKirj->TopRow)
		SGtlsMKirj->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::SGtlsMKirjMouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGtlsMKirj->TopRow > SGtlsMKirj->FixedRows)
		SGtlsMKirj->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::SGtls1KirjMouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGtls1Kirj->RowCount - SGtls1Kirj->FixedRows >= SGtls1Kirj->VisibleRowCount + SGtls1Kirj->TopRow)
		SGtls1Kirj->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::SGtls1KirjMouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGtls1Kirj->TopRow > SGtls1Kirj->FixedRows)
		SGtls1Kirj->TopRow--;
}
//---------------------------------------------------------------------------


void __fastcall TFormMuotoilu::FormResize(TObject *Sender)
{
	PageControl1->Height = ClientHeight - PageControl1->Top;
	BtnOtaLl1->Top = TabSheet1->Height - BtnOtaLl1->Height - 2 * Screen->PixelsPerInch / 96;
	BtnKirjLl1->Top = BtnOtaLl1->Top;
	BtnLueLl1->Top = BtnOtaLl1->Top;
	BtnOtaLl2->Top = BtnOtaLl1->Top;
	BtnKirjLl2->Top = BtnOtaLl1->Top;
	BtnLueLl2->Top = BtnOtaLl1->Top;
	BtnKirjLl1->Top = BtnOtaLl1->Top;
	BtnLueLl1->Top = BtnOtaLl1->Top;
	SGllKirj->Height = BtnOtaLl1->Top - SGllKirj->Top - 4 * Screen->PixelsPerInch / 96;
	SGllTied->Height = BtnOtaLl2->Top - SGllTied->Top - 4 * Screen->PixelsPerInch / 96;
	SGtlsKirj->Height = TabSheet1->Height - SGtlsKirj->Top - 4 * Screen->PixelsPerInch / 96;
	SGtlsTied->Height = TabSheet1->Height - SGtlsTied->Top - 4 * Screen->PixelsPerInch / 96;
	SGtlsMobil->Height = TabSheet1->Height - SGtlsMobil->Top - 4 * Screen->PixelsPerInch / 96;
	SGtlsMKirj->Height = TabSheet1->Height - SGtlsMKirj->Top - 4 * Screen->PixelsPerInch / 96;
	SGtls1Kirj->Height = TabSheet1->Height - SGtls1Kirj->Top - 4 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------


void __fastcall TFormMuotoilu::Luemuotoiluttiedostosta1Click(TObject *Sender)
{
	BtnLue1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::allennamuotoiluttiedostoon1Click(TObject *Sender)
{
	BtnKirj1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::CBtaulHtmlTiedClick(TObject *Sender)
{
	CBSarLevTied->Enabled = CBtaulHtmlTied->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::CBtaulHtmlMobilClick(TObject *Sender)
{
	CBSarLevMobil->Enabled = CBtaulHtmlMobil->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::Csseditori1Click(TObject *Sender)
{
	//
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BtnOtaHtmlClick(TObject *Sender)
{
	filetulosmuot.fontfamily = CBffTied->ItemIndex;
	filetulosmuot.fontsize = CBfszTied->ItemIndex;
	filetulosmuot.tauluhtml = CBtaulHtmlTied->Checked ? (1 + 2 * CBSarLevTied->ItemIndex) : 0;
//	filetulosmuot.tauluhtml = CBtaulHtmlTied->Checked ? (CBsuhtTied->Checked ? 2 : 1) : 0;
	filetulosmuot.sarjalinkit = RGlinkitTied->ItemIndex;
	filetulosmuot.cssfl = RGcssTied->ItemIndex;
	filetulosmuot.paaotskoko = _wtoi(EdtPaaOtsKokoHtml->Text.c_str());
	filetulosmuot.alaotskoko = _wtoi(EdtAlaOtsKokoHtml->Text.c_str());
	filetulosmuot.border = _wtoi(EdtViivatHtml->Text.c_str());
	filetulosmuot.efontsize = _wtoi(EdtRVaKokoHtml->Text.c_str());
	filetulosmuot.eborder = _wtoi(EdtRvaViivatHtml->Text.c_str());
	filetulosmuot.hpad = _wtoi(EdtHPadHtml->Text.c_str());
	filetulosmuot.vpad = _wtoi(EdtVPadHtml->Text.c_str());
	filetulosmuot.sarotsbold = CBSarOtsLihHtml->Checked;
	filetulosmuot.bgcolor = switchColors(Varit[0][0][0]);
	filetulosmuot.fontcolor = switchColors(Varit[0][0][1]);
	filetulosmuot.td1color = switchColors(Varit[0][1][0]);
	filetulosmuot.td1fontcolor = switchColors(Varit[0][1][1]);
	filetulosmuot.td2color = switchColors(Varit[0][2][0]);
	filetulosmuot.td2fontcolor = switchColors(Varit[0][2][1]);
	filetulosmuot.thcolor = switchColors(Varit[0][3][0]);
	filetulosmuot.thfontcolor = switchColors(Varit[0][3][1]);
	filetulosmuot.bordercolor = switchColors(Varit[0][4][1]);
	filetulosmuot.framecolor = switchColors(Varit[0][5][0]);
	filetulosmuot.framefontcolor = switchColors(Varit[0][5][1]);
	filetulosmuot.tdecolor = switchColors(Varit[0][6][0]);
	filetulosmuot.tdefontcolor = switchColors(Varit[0][6][1]);
	filetulosmuot.thecolor = switchColors(Varit[0][7][0]);
	filetulosmuot.thefontcolor = switchColors(Varit[0][7][1]);
	filetulosmuot.riviryhma = _wtoi(EdtVariRivitFile->Text.c_str());
	mobiltulosmuot.fontfamily = CBffMobil->ItemIndex;
	mobiltulosmuot.fontsize = CBfszMobil->ItemIndex;
	mobiltulosmuot.tauluhtml = CBtaulHtmlMobil->Checked ? (1 + 2 * CBSarLevMobil->ItemIndex) : 0;
	mobiltulosmuot.sarjalinkit = RGlinkitMobil->ItemIndex;
	mobiltulosmuot.cssfl = RGcssMobil->ItemIndex;
	mobiltulosmuot.paaotskoko = _wtoi(EdtPaaOtsKokoMobil->Text.c_str());
	mobiltulosmuot.alaotskoko = _wtoi(EdtAlaOtsKokoMobil->Text.c_str());
	mobiltulosmuot.border = _wtoi(EdtViivatMobil->Text.c_str());
	mobiltulosmuot.efontsize = _wtoi(EdtRVaKokoMobil->Text.c_str());
	mobiltulosmuot.eborder = _wtoi(EdtRvaViivatMobil->Text.c_str());
	mobiltulosmuot.hpad = _wtoi(EdtHPadMobil->Text.c_str());
	mobiltulosmuot.vpad = _wtoi(EdtVPadMobil->Text.c_str());
	mobiltulosmuot.sarotsbold = CBSarOtsLihMobil->Checked;
	mobiltulosmuot.bgcolor = switchColors(Varit[1][0][0]);
	mobiltulosmuot.fontcolor = switchColors(Varit[1][0][1]);
	mobiltulosmuot.td1color = switchColors(Varit[1][1][0]);
	mobiltulosmuot.td1fontcolor = switchColors(Varit[1][1][1]);
	mobiltulosmuot.td2color = switchColors(Varit[1][2][0]);
	mobiltulosmuot.td2fontcolor = switchColors(Varit[1][2][1]);
	mobiltulosmuot.thcolor = switchColors(Varit[1][3][0]);
	mobiltulosmuot.thfontcolor = switchColors(Varit[1][3][1]);
	mobiltulosmuot.bordercolor = switchColors(Varit[1][4][1]);
	mobiltulosmuot.framecolor = switchColors(Varit[1][5][0]);
	mobiltulosmuot.framefontcolor = switchColors(Varit[1][5][1]);
	mobiltulosmuot.tdecolor = switchColors(Varit[1][6][0]);
	mobiltulosmuot.tdefontcolor = switchColors(Varit[1][6][1]);
	mobiltulosmuot.thecolor = switchColors(Varit[1][7][0]);
	mobiltulosmuot.thefontcolor = switchColors(Varit[1][7][1]);
	mobiltulosmuot.riviryhma = _wtoi(EdtVariRivitMobil->Text.c_str());

	for (int ii = 0; ii < 2; ii++) {
		for (int ij = 0; ij < 6; ij++) {
			for (int fg = 0; fg < 2; fg++) {
				int ic = haeVariNo(Varit[ii][ij][fg]);
				if (ic < 0)
					LisaaOmaVari(Varit[ii][ij][fg]);
				}
			}
		}
}
//---------------------------------------------------------------------------

void TFormMuotoilu::naytaVariEsim(int i)
{
	wchar_t st[20];
	int v1 = switchColors(Varit[i][CBVariKohde[i]->ItemIndex][0]);
	int v2 = switchColors(Varit[i][CBVariKohde[i]->ItemIndex][1]);
	EdtCl[i]->Color = (TColor) Varit[i][CBVariKohde[i]->ItemIndex][0];
	EdtCl[i]->Font->Color = (TColor) Varit[i][CBVariKohde[i]->ItemIndex][1];
	swprintf(st, L"%03d%03d%03d/%03d%03d%03d", v1/65536, (v1/256)%256, v1%256,
		v2/65536, (v2/256)%256, v2%256);
	EdtCl[i]->Text = st;
}

void __fastcall TFormMuotoilu::FormCreate(TObject *Sender)
{
	ClBx[0][0] = ClBBgHtml;
	ClBx[0][1] = ClBFgHtml;
	ClBx[1][0] = ClBBgMobil;
	ClBx[1][1] = ClBFgMobil;
	EdtCl[0] = EdtClHtml;
	EdtCl[1] = EdtClMobil;
	CBVariKohde[0] = CBVariKohdeHtml;
	CBVariKohde[1] = CBVariKohdeMobil;
	CBVariKohde[0]->ItemIndex = 0;
	CBVariKohde[1]->ItemIndex = 0;
	Lisamaarityksia1->Visible = DeveloperMode;
	PageControl1->TabIndex = 0;
	naytaLlKirj();
	naytaLlTied();
	naytaTlsKirj();
	naytaTlsTied();
	naytaTlsMobil();
	naytaTlsMKirj();
	naytaTls1Kirj();

	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 2; j++) {
			for (int cl = 0; cl < sizeof(OlVarit)/sizeof(OlVarit[0]) && OlVarit[cl].nimi[0] != 0; cl++)
				ClBx[i][j]->Items->AddObject(OlVarit[cl].nimi, reinterpret_cast<TObject*>(OlVarit[cl].vari));
			ClBx[i][j]->Items->Strings[0] = L"Uusi v‰ri";
			ClBx[i][j]->ItemIndex = haeVariNo(Varit[i][0][j])+1;
			}
		}
	NaytaArvot(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::ClBBgHtmlClick(TObject *Sender)
{
	wchar_t st[20];
	int i = 0, fg = 0;

	for (i = 0; i < 2; i++) {
		for (fg = 0; fg < 2; fg++) {
			if (Sender == ClBx[i][fg])
				break;
			}
		if (fg < 2)
			break;
		}
	if (i > 1 || fg > 1)
		return;
	Varit[i][CBVariKohde[i]->ItemIndex][fg] =  ClBx[i][fg]->Colors[ClBx[i][fg]->ItemIndex];
	naytaVariEsim(i);
}
//---------------------------------------------------------------------------


void __fastcall TFormMuotoilu::CBVariKohdeHtmlChange(TObject *Sender)
{
	ClBx[0][0]->ItemIndex = haeVariNo(Varit[0][CBVariKohdeHtml->ItemIndex][0])+1;
	ClBx[0][1]->ItemIndex = haeVariNo(Varit[0][CBVariKohdeHtml->ItemIndex][1])+1;
	naytaVariEsim(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::CBVariKohdeMobilChange(TObject *Sender)
{
	ClBx[1][0]->ItemIndex = haeVariNo(Varit[1][CBVariKohdeMobil->ItemIndex][0])+1;
	ClBx[1][1]->ItemIndex = haeVariNo(Varit[1][CBVariKohdeMobil->ItemIndex][1])+1;
	naytaVariEsim(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BitBtn7Click(TObject *Sender)
{
	Application->HelpKeyword(L"Html-lis‰m‰‰ritykset");
}
//---------------------------------------------------------------------------

