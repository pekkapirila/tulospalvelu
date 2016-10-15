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
#include "UnitMain.h"
#include "VMuotoilu.h"
TFormMuotoilu *FormMuotoilu;

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

static int Varit[9][2];
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
	SGTls[2] = SGtlsTied;
	SGTls[3] = SGtlsMTied;
	PageControl1->ActivePageIndex = 0;
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
		for (int j = 0; j < 2; j++)
			ClBx[j]->Items->AddObject(OlVarit[cl].nimi, reinterpret_cast<TObject*>(OlVarit[cl].vari));
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

void __fastcall TFormMuotoilu::FormCreate(TObject *Sender)
{
	ClBx[0] = ClBBgHtml;
	ClBx[1] = ClBFgHtml;
	CBVariKohdeHtml->ItemIndex = 0;
//	Lisamaarityksia1->Visible = DeveloperMode;
	PageControl1->TabIndex = 0;

	for (int j = 0; j < 2; j++) {
		for (int cl = 0; cl < sizeof(OlVarit)/sizeof(OlVarit[0]) && OlVarit[cl].nimi[0] != 0; cl++)
			ClBx[j]->Items->AddObject(OlVarit[cl].nimi, reinterpret_cast<TObject*>(OlVarit[cl].vari));
		ClBx[j]->Items->Strings[0] = L"Uusi v‰ri";
		ClBx[j]->ItemIndex = haeVariNo(Varit[0][j])+1;
		}
	NaytaArvot(Sender);
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
   EdtOikMarg->Text = tulosmuot.omarg;
   EdtYlaMarg->Text = tulosmuot.ymarg;
   EdtAlaMarg->Text = tulosmuot.amarg;
   CBsnimifl->Checked = tulosmuot.snimifl;
   CBtpvafl->Checked = tulosmuot.tpvafl;
   CBtklofl->Checked = tulosmuot.tklofl;
   CBotsikot->Checked = tulosmuot.otsikot;
   CBvaihdanimet->Checked = tulosmuot.etusuku < 0;
   CBnollakarki->Checked = tulosmuot.nollakarki;
//   Edit7->Text = numlev;
//   CBuusisivu->Checked = uusisivu;
//   CBpitkaseura->Checked = pitkaseura;
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
   CBPienfont->ItemIndex = SmallFnt.fontfamily;
   EdtPienfsize->Text = UnicodeString(SmallFnt.size);
   EdtPienrv->Text = UnicodeString(SmallFnt.line);
   CBPienlaatu->ItemIndex = SmallFnt.bold + 2*SmallFnt.italic;
   CBLihfont->ItemIndex = BoldFnt.fontfamily;
   EdtLihfsize->Text = UnicodeString(BoldFnt.size);
   EdtLihrv->Text = UnicodeString(BoldFnt.line);
   CBLihlaatu->ItemIndex = BoldFnt.bold + 2*BoldFnt.italic;
   CBVakfont->ItemIndex = CourFnt.fontfamily-4;
   EdtVakfsize->Text = UnicodeString(120/CourFnt.pitch);
   EdtVakrv->Text = UnicodeString(CourFnt.line);
   CBVaklaatu->ItemIndex = CourFnt.bold + 2*CourFnt.italic;
	RGTKirjRivit->ItemIndex = tulosmuot.tiiv_rivit;
	RGTKirjFontit->ItemIndex = tulosmuot.tiiv_fontit;
	RGTKirjOsno->ItemIndex = tulosmuot.tiiv_osno;
	CBTKirjArvo->Checked = tulosmuot.tiiv_arvo != 0;

	EdtTTiedrivpit->Text = filetulosmuot.tiiv_len;
	RGTTiedRivit->ItemIndex = filetulosmuot.tiiv_rivit;
	RGTTiedFontit->ItemIndex = filetulosmuot.tiiv_fontit;
	RGTTiedOsno->ItemIndex = filetulosmuot.tiiv_osno;
	CBTTiedArvo->Checked = filetulosmuot.tiiv_arvo != 0;
	CBffTied->ItemIndex = filetulosmuot.fontfamily;
	CBfszTied->ItemIndex = filetulosmuot.fontsize;

	CBtaulHtmlTied->Checked = (filetulosmuot.tauluhtml & 3);
	CBSarLev->Enabled = (filetulosmuot.tauluhtml & 3);
	if (CBSarLev->Enabled)
		CBSarLev->ItemIndex = (filetulosmuot.tauluhtml & 3) - 1;
	CBRvaLeveydet->Checked = (filetulosmuot.tauluhtml & 4);
	EdLevKerr->Text = filetulosmuot.levkerr;
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
	Varit[0][0] = switchColors(filetulosmuot.bgcolor);
	Varit[0][1] = switchColors(filetulosmuot.fontcolor);
	Varit[1][0] = switchColors(filetulosmuot.td1color);
	Varit[1][1] = switchColors(filetulosmuot.td1fontcolor);
	Varit[2][0] = switchColors(filetulosmuot.td2color);
	Varit[2][1] = switchColors(filetulosmuot.td2fontcolor);
	Varit[3][0] = switchColors(filetulosmuot.tdjkcolor);
	Varit[3][1] = switchColors(filetulosmuot.tdjkfontcolor);
	Varit[4][0] = switchColors(filetulosmuot.thcolor);
	Varit[4][1] = switchColors(filetulosmuot.thfontcolor);
	Varit[5][1] = switchColors(filetulosmuot.bordercolor);
	Varit[6][0] = switchColors(filetulosmuot.framecolor);
	Varit[6][1] = switchColors(filetulosmuot.framefontcolor);
	Varit[7][0] = switchColors(filetulosmuot.tdecolor);
	Varit[7][1] = switchColors(filetulosmuot.tdefontcolor);
	Varit[8][0] = switchColors(filetulosmuot.thecolor);
	Varit[8][1] = switchColors(filetulosmuot.thefontcolor);
	Varit[5][0] = Varit[1][0];

	for (int ij = 0; ij < 6; ij++) {
		for (int fg = 0; fg < 2; fg++) {
			int ic = haeVariNo(Varit[ij][fg]);
			if (ic < 0)
				LisaaOmaVari(Varit[ij][fg]);
			}
		}
	ClBx[0]->ItemIndex = haeVariNo(Varit[CBVariKohdeHtml->ItemIndex][0])+1;
	ClBx[1]->ItemIndex = haeVariNo(Varit[CBVariKohdeHtml->ItemIndex][1])+1;
//   RadioGroup1->ItemIndex = wcswcind(merkit, L"IRWAP");

}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::Poistu1Click(TObject *Sender)
{
	Close();
}

//---------------------------------------------------------------------------
void __fastcall TFormMuotoilu::naytaTls(int lj)
{
	SGTls[lj]->ColCount = 5;
	SGTls[lj]->RowCount = n_frmflds[lj]+1;
	SGTls[lj]->ColWidths[0] = 60 * Screen->PixelsPerInch / 96;
	SGTls[lj]->ColWidths[1] = 60 * Screen->PixelsPerInch / 96;
	SGTls[lj]->Cells[0][0] = L"Kentt‰";
	SGTls[lj]->Cells[1][0] = L"Otsikko";
	SGTls[lj]->Cells[2][0] = L"Sijainti";
	SGTls[lj]->Cells[3][0] = L"Merkkej‰";
	SGTls[lj]->Cells[4][0] = L"Tasaus";
	for (int i = 1; i <= 3; i++) {
		SGTls[lj]->ColWidths[i] = 50 * Screen->PixelsPerInch / 96;
		}
	for (int r = 1; r <= n_frmflds[lj]; r++) {
		SGTls[lj]->Cells[0][r] = UnicodeString(frmflds[lj][r-1].name);
		SGTls[lj]->Cells[1][r] = UnicodeString(frmflds[lj][r-1].ots);
		SGTls[lj]->Cells[2][r] = UnicodeString(frmflds[lj][r-1].pos);
		SGTls[lj]->Cells[3][r] = UnicodeString(frmflds[lj][r-1].len);
		SGTls[lj]->Cells[4][r] = frmflds[lj][r-1].oik ? L"Oik" : L"Vas";
		}
	SGTls[lj]->Options << goEditing;
}
//---------------------------------------------------------------------------
void __fastcall TFormMuotoilu::naytaTlsKirj(void)
{
	naytaTls(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormMuotoilu::naytaTlsTied(void)
{
	naytaTls(2);
}
//---------------------------------------------------------------------------
void __fastcall TFormMuotoilu::naytaTlsMKirj(void)
{
	naytaTls(1);
}
//---------------------------------------------------------------------------
void __fastcall TFormMuotoilu::naytaTlsMTied(void)
{
	naytaTls(3);
}
//---------------------------------------------------------------------------
void __fastcall TFormMuotoilu::FormShow(TObject *Sender)
{
	for (int lj = 0; lj < 4; lj++)
		naytaTls(lj);
	NaytaArvot(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BtnOta1Click(TObject *Sender)
{
   GDIfontTp Fnt;

   tulosmuot.sivpit = EdtSivPit->Text.ToInt();
   tulosmuot.esirivit = Edtesirivit->Text.ToInt();
   tulosmuot.snimifl = CBsnimifl->Checked ? 1 : 0;   // Ei k‰ytˆss‰
   tulosmuot.tpvafl = CBtpvafl->Checked ? 1 : 0;
   tulosmuot.tklofl = CBtklofl->Checked ? 1 : 0;
   tulosmuot.otsikot = CBotsikot->Checked ? 1 : 0;
   tulosmuot.etusuku = CBvaihdanimet->Checked ? -1 : 1;
   tulosmuot.nollakarki = CBnollakarki->Checked ? 1 : 0;  // Ei k‰ytˆss‰
   tulosmuot.marg = _wtoi(EdtVasMarg->Text.c_str());
   tulosmuot.omarg = _wtoi(EdtOikMarg->Text.c_str());
   tulosmuot.ymarg = _wtoi(EdtYlaMarg->Text.c_str());
   tulosmuot.amarg = _wtoi(EdtAlaMarg->Text.c_str());
   filetulosmuot.tpvafl = tulosmuot.tpvafl;
   filetulosmuot.tklofl = tulosmuot.tklofl;
   filetulosmuot.otsikot = tulosmuot.otsikot;
   filetulosmuot.etusuku = tulosmuot.etusuku;
   filetulosmuot.nollakarki = tulosmuot.nollakarki;  // Ei k‰ytˆss‰

   lsttulparam.tulmuot = tulosmuot;

   wcsncpy(paaots, Edtpaaots->Text.c_str(), 80);
   wcsncpy(sarjaots, Edtalaots->Text.c_str(), 80);
   wcsncpy(alaots, Edtalaots2->Text.c_str(), 119);
   initHtmlHeader(true);

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
   Fnt = SmallFnt;
   SmallFnt.fontfamily = CBPienfont->ItemIndex;
   SmallFnt.size = EdtPienfsize->Text.ToInt();
   SmallFnt.line = EdtPienrv->Text.ToInt();
   SmallFnt.bold = CBPienlaatu->ItemIndex % 2;
   SmallFnt.italic = CBPienlaatu->ItemIndex / 2;
   if (memcmp(&Fnt, &SmallFnt, sizeof(GDIfontTp)))
	   SmallFnt.hfont = NULL;
   Fnt = BoldFnt;
   BoldFnt.fontfamily = CBLihfont->ItemIndex;
   BoldFnt.size = EdtLihfsize->Text.ToInt();
   BoldFnt.line = EdtLihrv->Text.ToInt();
   BoldFnt.bold = CBLihlaatu->ItemIndex % 2;
   BoldFnt.italic = CBLihlaatu->ItemIndex / 2;
   if (memcmp(&Fnt, &BoldFnt, sizeof(GDIfontTp)))
	   BoldFnt.hfont = NULL;
   Fnt = CourFnt;
   CourFnt.fontfamily = CBVakfont->ItemIndex+4;
   CourFnt.pitch = 120/EdtVakfsize->Text.ToInt();
   CourFnt.line = EdtVakrv->Text.ToInt();
   CourFnt.bold = CBVaklaatu->ItemIndex % 2;
   CourFnt.italic = CBVaklaatu->ItemIndex / 2;
   if (memcmp(&Fnt, &CourFnt, sizeof(GDIfontTp)))
	   CourFnt.hfont = NULL;

	tulosmuot.tiiv_rivit = RGTKirjRivit->ItemIndex;
	tulosmuot.tiiv_fontit = RGTKirjFontit->ItemIndex;
	tulosmuot.tiiv_osno = RGTKirjOsno->ItemIndex;
	tulosmuot.tiiv_arvo = CBTKirjArvo->Checked ? 1 : 0;
	filetulosmuot.tiiv_len = _wtoi(EdtTTiedrivpit->Text.c_str());
	filetulosmuot.tiiv_rivit = RGTTiedRivit->ItemIndex;
	filetulosmuot.tiiv_fontit = RGTTiedFontit->ItemIndex;
	filetulosmuot.tiiv_osno = RGTTiedOsno->ItemIndex;
	filetulosmuot.tiiv_arvo = CBTTiedArvo->Checked ? 1 : 0;
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
	for (int lj = 0; lj < 4; lj++)
		naytaTls(lj);
	NaytaArvot(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BtnKirj1Click(TObject *Sender)
{
	BtnOta1Click(Sender);
	BtnOtaTul1Click(Sender);
	BtnOtaTul2Click(Sender);
	SaveDialog1->InitialDir = FormMain->CurrentDir;
	SaveDialog1->DefaultExt = L"xml";
	SaveDialog1->Filter = L"XML-tiedostot (*.xml)|*.xml|Kaikki tiedostot (*.*)|*.*";
	if (SaveDialog1->Execute()) {
		TallXMLMuotoilu(SaveDialog1->FileName.c_str());
		}
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::OtaTls(int lj)
{
	for (int r = 1; r < SGTls[lj]->RowCount; r++) {
		wcsncpy(frmflds[lj][r-1].ots, SGTls[lj]->Cells[1][r].c_str(), sizeof(frmflds[lj][r-1].ots)/2-1);
		frmflds[lj][r-1].pos = _wtoi(SGTls[lj]->Cells[2][r].c_str());
		frmflds[lj][r-1].len = _wtoi(SGTls[lj]->Cells[3][r].c_str());
		frmflds[lj][r-1].oik = towupper(*(SGTls[lj]->Cells[4][r].c_str())) == L'O';
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BtnOtaTul1Click(TObject *Sender)
{
	OtaTls(0);
	OtaTls(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BtnOtaTul2Click(TObject *Sender)
{
	OtaTls(1);
	OtaTls(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BtnOtaTul3Click(TObject *Sender)
{
	tulosmuot.tiiv_rivit = RGTKirjRivit->ItemIndex;
	tulosmuot.tiiv_fontit = RGTKirjFontit->ItemIndex;
	tulosmuot.tiiv_osno = RGTKirjOsno->ItemIndex;
	tulosmuot.tiiv_arvo = CBTKirjArvo->Checked ? 1 : 0;
	filetulosmuot.sivpit = 99999;
	filetulosmuot.tiiv_len = _wtoi(EdtTTiedrivpit->Text.c_str());
	filetulosmuot.tiiv_rivit = RGTTiedRivit->ItemIndex;
	filetulosmuot.tiiv_fontit = RGTTiedFontit->ItemIndex;
	filetulosmuot.tiiv_osno = RGTTiedOsno->ItemIndex;
	filetulosmuot.tiiv_arvo = CBTTiedArvo->Checked ? 1 : 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::Help1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Muotoilu");
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

void __fastcall TFormMuotoilu::SGtlsMTiedMouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGtlsMTied->RowCount - SGtlsMTied->FixedRows >= SGtlsMTied->VisibleRowCount + SGtlsMTied->TopRow)
		SGtlsMTied->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::SGtlsMTiedMouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGtlsMTied->TopRow > SGtlsMTied->FixedRows)
		SGtlsMTied->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::FormResize(TObject *Sender)
{
	PageControl1->Height = ClientHeight - PageControl1->Top;
	SGtlsKirj->Height = TabSheet3->ClientHeight - SGtlsKirj->Top;
	SGtlsTied->Height = TabSheet3->ClientHeight - SGtlsTied->Top;
	SGtlsMKirj->Height = TabSheet3->ClientHeight - SGtlsMKirj->Top;
	SGtlsMTied->Height = TabSheet3->ClientHeight - SGtlsMTied->Top;
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

void __fastcall TFormMuotoilu::Luevanhamuotoilutiedosto1Click(TObject *Sender)
{
	OpenDialog1->InitialDir = FormMain->CurrentDir;
	OpenDialog1->Filter = L"Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		lue_muotoilu(OpenDialog1->FileName.c_str(), 0);
		}
	SetCurrentDir(FormMain->CurrentDir);
	for (int lj = 0; lj < 4; lj++)
		naytaTls(lj);
	NaytaArvot(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BtnOta4Click(TObject *Sender)
{
	filetulosmuot.fontfamily = CBffTied->ItemIndex;
	filetulosmuot.fontsize = CBfszTied->ItemIndex;
	if (!CBtaulHtmlTied->Checked)
		filetulosmuot.tauluhtml = 0;
	else
		filetulosmuot.tauluhtml = CBSarLev->ItemIndex + 1;
	if (CBRvaLeveydet->Checked)
		filetulosmuot.tauluhtml += 4;
	filetulosmuot.levkerr = _wtoi(EdLevKerr->Text.c_str());
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
	filetulosmuot.bgcolor = switchColors(Varit[0][0]);
	filetulosmuot.fontcolor = switchColors(Varit[0][1]);
	filetulosmuot.td1color = switchColors(Varit[1][0]);
	filetulosmuot.td1fontcolor = switchColors(Varit[1][1]);
	filetulosmuot.td2color = switchColors(Varit[2][0]);
	filetulosmuot.td2fontcolor = switchColors(Varit[2][1]);
	filetulosmuot.tdjkcolor = switchColors(Varit[3][0]);
	filetulosmuot.tdjkfontcolor = switchColors(Varit[3][1]);
	filetulosmuot.thcolor = switchColors(Varit[4][0]);
	filetulosmuot.thfontcolor = switchColors(Varit[4][1]);
	filetulosmuot.bordercolor = switchColors(Varit[5][1]);
	filetulosmuot.framecolor = switchColors(Varit[6][0]);
	filetulosmuot.framefontcolor = switchColors(Varit[6][1]);
	filetulosmuot.tdecolor = switchColors(Varit[7][0]);
	filetulosmuot.tdefontcolor = switchColors(Varit[7][1]);
	filetulosmuot.thecolor = switchColors(Varit[8][0]);
	filetulosmuot.thefontcolor = switchColors(Varit[8][1]);
	filetulosmuot.riviryhma = _wtoi(EdtVariRivitFile->Text.c_str());

	for (int ij = 0; ij < 6; ij++) {
		for (int fg = 0; fg < 2; fg++) {
			int ic = haeVariNo(Varit[ij][fg]);
			if (ic < 0)
				LisaaOmaVari(Varit[ij][fg]);
			}
		}
}
//---------------------------------------------------------------------------
void TFormMuotoilu::naytaVariEsim(void)
{
	wchar_t st[20];
	int v1 = switchColors(Varit[CBVariKohdeHtml->ItemIndex][0]);
	int v2 = switchColors(Varit[CBVariKohdeHtml->ItemIndex][1]);
	EdtClHtml->Color = (TColor) Varit[CBVariKohdeHtml->ItemIndex][0];
	EdtClHtml->Font->Color = (TColor) Varit[CBVariKohdeHtml->ItemIndex][1];
	swprintf(st, L"%03d%03d%03d/%03d%03d%03d", v1/65536, (v1/256)%256, v1%256,
		v2/65536, (v2/256)%256, v2%256);
	EdtClHtml->Text = st;
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::ClBBgHtmlClick(TObject *Sender)
{
	wchar_t st[20];
	int fg = 0;

	for (fg = 0; fg < 2; fg++) {
		if (Sender == ClBx[fg])
			break;
		}
	if (fg > 1)
		return;
	Varit[CBVariKohdeHtml->ItemIndex][fg] =  ClBx[fg]->Colors[ClBx[fg]->ItemIndex];
	naytaVariEsim();
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::CBVariKohdeHtmlChange(TObject *Sender)
{
	ClBx[0]->ItemIndex = haeVariNo(Varit[CBVariKohdeHtml->ItemIndex][0])+1;
	ClBx[1]->ItemIndex = haeVariNo(Varit[CBVariKohdeHtml->ItemIndex][1])+1;
	naytaVariEsim();
}
//---------------------------------------------------------------------------

void __fastcall TFormMuotoilu::BitBtn3Click(TObject *Sender)
{
	Application->HelpKeyword(L"Html-lis‰m‰‰ritykset");
}
//---------------------------------------------------------------------------

