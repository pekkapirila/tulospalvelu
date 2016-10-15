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

#include "UnitMain.h"
#include "UnitCfg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ScSSHClient"
#pragma resource "*.dfm"
TFormCfg *FormCfg;

INT yhteys_no(wchar_t *st, INT *pos);

//---------------------------------------------------------------------------
__fastcall TFormCfg::TFormCfg(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	PageControl1->ActivePageIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormCfg::FormCreate(TObject *Sender)
{
	SG1->RowCount = MAX_LAHPORTTI+1;
	SG1->Cells[0][0] = L"No";
	SG1->Cells[1][0] = L"Laji";
	SG1->Cells[2][0] = L"I/O";
	SG1->Cells[3][0] = L"Portti";
	SG1->Cells[4][0] = L"VastaIPosoite";
	SG1->Cells[5][0] = L"VastaPortti";
	SG1->Cells[6][0] = L"Sanomat";
	SG1->Cells[7][0] = L"Ei väl.";
	SG1->ColWidths[0] = 40 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[1] = 50 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[2] = 40 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[3] = 50 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[4] = 110 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[5] = 60 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[6] = 70 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[7] = 45 * Screen->PixelsPerInch / 96;
	SGLeimat->RowCount = NREGNLY+1;
	SGLeimat->Cells[0][0] = L"LaiteNo";
	SGLeimat->Cells[1][0] = L"Laite";
	SGLeimat->Cells[2][0] = L"Yhteys";
	SGLeimat->Cells[3][0] = L"Portti";
	SGLeimat->Cells[4][0] = L"VastaIPosoite";
	SGLeimat->Cells[5][0] = L"VastaPortti";
	SGLeimat->Cells[6][0] = L"Ajanotto";
	SGLeimat->ColWidths[0] = 40 * Screen->PixelsPerInch / 96;
	SGLeimat->ColWidths[1] = 50 * Screen->PixelsPerInch / 96;
	SGLeimat->ColWidths[2] = 45 * Screen->PixelsPerInch / 96;
	SGLeimat->ColWidths[3] = 50 * Screen->PixelsPerInch / 96;
	SGLeimat->ColWidths[4] = 110 * Screen->PixelsPerInch / 96;
	SGLeimat->ColWidths[5] = 60 * Screen->PixelsPerInch / 96;
	SGLeimat->ColWidths[6] = 70 * Screen->PixelsPerInch / 96;
	SGAjat->RowCount = 13;
	SGAjat->Cells[0][0] = L"Lähde";
	SGAjat->Cells[1][0] = L"Kohde";
	SGAjat->Cells[2][0] = L"Jono";
	SGAjat->Cells[0][1] = L"Sarjaportti";
	SGAjat->Cells[0][2] = L"Varalla";
	SGAjat->Cells[0][3] = L"Näppäimistö";
	SGAjat->Cells[0][4] = L"Kellon kanava 0";
	SGAjat->Cells[0][5] = L"Kellon kanava 1";
	SGAjat->Cells[0][6] = L"Kellon kanava 2";
	SGAjat->Cells[0][7] = L"Kellon kanava 3";
	SGAjat->Cells[0][8] = L"Kellon kanava 4";
	SGAjat->Cells[0][9] = L"Kellon kanava 5";
	SGAjat->Cells[0][10] = L"Kellon kanava 6";
	SGAjat->Cells[0][11] = L"Kellon kanava 7";
	SGAjat->Cells[0][12] = L"Kellon kanava 8";
	SGAjat->ColWidths[0] = 100 * Screen->PixelsPerInch / 96;
	SGAjat->ColWidths[1] = 50 * Screen->PixelsPerInch / 96;
	SGAjat->ColWidths[2] = 50 * Screen->PixelsPerInch / 96;
	TulkitseTiedot(true);
	Memo1->Lines->Add(L"//CfgEditorAlku - Konfiguraatioeditori ei tulkitse tai muuta tätä riviä edeltätäviä rivejä");
	Memo1->Lines->Add(L"//CfgEditorLoppu - Konfiguraatioeditori ei tulkitse tai muuta tätä riviä seuraavia rivejä");
	RGNaytaVal->ItemIndex = 2;
	ValSetup();
}
//---------------------------------------------------------------------------
void __fastcall TFormCfg::FormResize(TObject *Sender)
{
	Memo1->Width = ClientWidth - Memo1->Left;
	Memo1->Height = ClientHeight - Memo1->Top;
	PageControl1->Height = ClientHeight - PageControl1->Top;
	SG1->Height = TabSheet2->ClientHeight - SG1->Top;
	SGLeimat->Height = TabSheet4->ClientHeight - SGLeimat->Top;
	SGAjat->Height = TabSheet3->ClientHeight - SGAjat->Top - 6 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------
void __fastcall TFormCfg::Luetiedosto1Click(TObject *Sender)
{
	TextFl *cfgfile;

	OpenDialog1->InitialDir = FormMain->CurrentDir;
	OpenDialog1->FileName = UnicodeString(L"laskenta.cfg");
	OpenDialog1->Filter = L"Konfiguraatiot (*.cfg)|*.CFG|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		Memo1->Clear();
		cfgfile = new TextFl(OpenDialog1->FileName.c_str(), L"r");
		if (cfgfile->IsOpen()) {
			while (!cfgfile->Feof()) {
				wchar_t line[200];

				if (!cfgfile->ReadLine(line, 198))
					break;
				wtrimline(line);
				if (wcslen(line) > 0)
					Memo1->Lines->Add(line);
				}
			}
		delete cfgfile;
		}
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::Kirjoitatiedosto1Click(TObject *Sender)
{
	TextFl *cfgfile;

	SaveDialog1->InitialDir = FormMain->CurrentDir;
	SaveDialog1->Filter = L"Konfiguraatiot (*.cfg)|*.CFG|Kaikki tiedostot (*.*)|*.*";
	if (SaveDialog1->Execute()) {
		cfgfile = new TextFl(SaveDialog1->FileName.c_str(), L"w");
		if (cfgfile->IsOpen()) {
			for (int r = 0; r < Memo1->Lines->Count; r++) {
				cfgfile->WriteLine((Memo1->Lines->Strings[r]+L"\n").c_str());
				}
			}
		delete cfgfile;
		}
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::Suljekaavake1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void TFormCfg::PoistaTulkitut(void)
{
	for (int r = Memo1->Lines->Count-1; r >= 0; r--)
		if (Tulkitut[r]) {
			Memo1->Lines->Delete(r);
			if (Loppu > r)
				Loppu--;
			}
	memset(Tulkitut, 0, sizeof(Tulkitut));
}
//---------------------------------------------------------------------------
void __fastcall TFormCfg::Button2Click(TObject *Sender)
{
	TulkitseTiedot(true);
	TulkitseTiedot(false);
}
//---------------------------------------------------------------------------
void __fastcall TFormCfg::Pivitvalinnatikkunaan1Click(TObject *Sender)
{
	PoistaTulkitut();
	PaivitaIkkuna();
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::Poistatulkitutrivit1Click(TObject *Sender)
{
	PoistaTulkitut();
}
//---------------------------------------------------------------------------
void TFormCfg::PaivitaIkkuna(void)
{
	int r, val;
	UnicodeString US;

	memset(Tulkitut, 0, sizeof(Tulkitut));
	Alku = 0;
	Loppu = 0;
	for (int i = 0; i < Memo1->Lines->Count; i++) {
		wchar_t fld[200];
		wcsncpy(fld, Memo1->Lines->Strings[i].c_str(), 199);
		if (wmemcmpU(fld, L"//CfgEditorAlku", 15) == 0) {
			Alku = i+1;
			break;
			}
		}
	for (int i = 0; i < Memo1->Lines->Count; i++) {
		wchar_t fld[200];
		wcsncpy(fld, Memo1->Lines->Strings[i].c_str(), 199);
		if (wmemcmpU(fld, L"//CfgEditorLoppu", 15) == 0) {
			Loppu = i;
			break;
			}
		}
	if (Loppu == 0) {
		Memo1->Lines->Insert(Alku, L"//CfgEditorLoppu - Konfiguraatioeditori ei tulkitse tai muuta tätä riviä seuraavia rivejä");
		}
	if (Alku == 0) {
		Memo1->Lines->Insert(0,L"//CfgEditorAlku - Konfiguraatioeditori ei tulkitse tai muuta tätä riviä edeltätäviä rivejä");
		Alku = 1;
		Loppu++;
		}
	r = Alku;
	US = EdKone->Text.Trim();
	if (US.Length() > 0) {
		Memo1->Lines->Insert(r++, UnicodeString(L"KONE=")+US.SubString(1,2));
		Loppu++;
		}
	US = EdIkkOts->Text.Trim();
	if (US.Length() > 0) {
		Memo1->Lines->Insert(r++, L"IKKUNAOTS="+US);
		Loppu++;
		}
	if (CBIkkunat->Checked) {
		US = EdIkkunat->Text.Trim();
		if (US.Length() > 0 && wmemcmpU(L"ikkunat.xml", US.c_str(), 12))
			Memo1->Lines->Insert(r++, UnicodeString(L"IKKUNAT=")+US);
		else
			Memo1->Lines->Insert(r++, UnicodeString(L"IKKUNAT"));
		Loppu++;
		}
	if (CBAani->ItemIndex != 1) {
		Memo1->Lines->Insert(r++, UnicodeString(L"ÄÄNI=")+UnicodeString(CBAani->ItemIndex));
		Loppu++;
		}
	US = EdSammSalasana->Text.Trim();
	if (US.Length() > 0) {
		Memo1->Lines->Insert(r++, L"SULKUSALASANA="+US);
		Loppu++;
		}
	if (CBJoustoViesti->Checked) {
		Memo1->Lines->Insert(r++, UnicodeString(L"JOUSTOVIESTI"));
		Loppu++;
		}
	if (CBSailHyl->Checked) {
		Memo1->Lines->Insert(r++, UnicodeString(L"SÄILHYL"));
		Loppu++;
		}
	if (CBVuorokaudet->Checked) {
		Memo1->Lines->Insert(r++, UnicodeString(L"VUOROKAUDET"));
		Loppu++;
		}
	if (CBNaytaHaj->Checked) {
		Memo1->Lines->Insert(r++, UnicodeString(L"NÄYTÄHAJONTA=EI"));
		Loppu++;
		}
	if (CBComfile->ItemIndex == 1) {
		Memo1->Lines->Insert(r++, UnicodeString(L"COMFILE=S"));
		Loppu++;
		}
	val = _wtoi(EdMaxYhteys->Text.c_str());
	if (val != 0) {
		Memo1->Lines->Insert(r++, L"MAXYHTEYS="+UnicodeString(val));
		Loppu++;
		}
	val = _wtoi(EdPortBase->Text.c_str());
	if (val != 0 && val != 15900) {
		Memo1->Lines->Insert(r++, L"PORTBASE="+UnicodeString(val));
		Loppu++;
		}
	if (CBValita->ItemIndex == 1) {
		Memo1->Lines->Insert(r++, UnicodeString(L"VÄLITÄ=EI"));
		Loppu++;
		}
	for (int y = 0; y < SG1->RowCount-1; y++) {
		int ny;
		UnicodeString InOut = L"";
		if (SG1->Cells[2][y+1].Trim().Length() > 0)
			InOut = SG1->Cells[2][y+1].Trim().SubString(1,1);
		if (SG1->Cells[0][y+1].Length() == 0 || (ny = SG1->Cells[0][y+1].ToInt()) < 1)
			continue;
		UnicodeString YhtStr = L"YHTEYS"+UnicodeString(SG1->Cells[0][y+1].ToInt())+L"="+
			SG1->Cells[1][y+1];
		if (wmemcmpU(SG1->Cells[1][y+1].c_str(), L"COM", 3) == 0)
			YhtStr = YhtStr+UnicodeString(SG1->Cells[3][y+1].ToInt())+InOut;
		else {
			if (SG1->Cells[6][y+1].Pos(L"Xml") != 0)
				YhtStr = YhtStr+L"X";
			else if (SG1->Cells[6][y+1].Pos(L"Zzz") != 0)
				YhtStr = YhtStr+L"Z";
			else if (SG1->Cells[6][y+1].Pos(L"Lki") != 0)
				YhtStr = YhtStr+L"L";
			else
				YhtStr = YhtStr+InOut;
			US = SG1->Cells[4][y+1].Trim();
			val = _wtoi(SG1->Cells[3][y+1].c_str());
			if (US.Length() == 0) {
				if (val > 0) {
					YhtStr = YhtStr + L":"+UnicodeString(val);
					}
				}
			else {
				if (wmemcmpU(SG1->Cells[1][y+1].c_str(), L"TCP", 3) != 0)
					YhtStr = YhtStr + L":"+UnicodeString(val);
				YhtStr = YhtStr + L"/" + US + L":"+SG1->Cells[5][y+1].Trim();
				}
			}
		Memo1->Lines->Insert(r++, YhtStr);
		Loppu++;
		if (SG1->Cells[6][y+1].Pos(L"Aik") != 0) {
			Memo1->Lines->Insert(r++, L"LÄHAIKA"+UnicodeString(ny));
			Loppu++;
			}
		if (SG1->Cells[6][y+1].Pos(L"AiV") != 0) {
			Memo1->Lines->Insert(r++, L"LÄHAIKA"+UnicodeString(ny)+L"=V");
			Loppu++;
			}
		if (SG1->Cells[6][y+1].Pos(L"AiP") != 0) {
			Memo1->Lines->Insert(r++, L"LÄHAIKA"+UnicodeString(ny)+L"=P");
			Loppu++;
			}
		if (SG1->Cells[6][y+1].Pos(L"Emt") != 0) {
			Memo1->Lines->Insert(r++, L"LÄHEMIT"+UnicodeString(ny));
			Loppu++;
			}
		if (SG1->Cells[6][y+1].Pos(L"EmI") != 0) {
			Memo1->Lines->Insert(r++, L"LÄHEMIT"+UnicodeString(ny)+L"=I");
			Loppu++;
			}
		if (SG1->Cells[6][y+1].Pos(L"EmO") != 0) {
			Memo1->Lines->Insert(r++, L"LÄHEMIT"+UnicodeString(ny)+L"=O");
			Loppu++;
			}
		if (SG1->Cells[6][y+1].Pos(L"EmV") != 0) {
			Memo1->Lines->Insert(r++, L"LÄHEMIT"+UnicodeString(ny)+L"=V");
			Loppu++;
			}
		if (SG1->Cells[6][y+1].Pos(L"EVa") != 0) {
			Memo1->Lines->Insert(r++, L"LÄHEMVA"+UnicodeString(ny));
			Loppu++;
			}
		if (SG1->Cells[6][y+1].Pos(L"ECd") != 0) {
			Memo1->Lines->Insert(r++, L"LÄHECODE"+UnicodeString(ny));
			Loppu++;
			}
		if (SG1->Cells[6][y+1].Pos(L"Tie") != 0) {
			Memo1->Lines->Insert(r++, L"LÄHTIEDOSTOT"+UnicodeString(ny)+
				(CBLahKaikkiPolku->Checked ? L"/K" : L""));
			Loppu++;
			}
		if (SG1->Cells[7][y+1].Trim().Length() > 0) {
			if (SG1->Cells[7][y+1].Pos(L"Ei") != 0)
				Memo1->Lines->Insert(r++, L"VÄLITÄ"+UnicodeString(ny)+L"=Ei");
			else
				Memo1->Lines->Insert(r++, L"VÄLITÄ"+UnicodeString(ny)+L"="+SG1->Cells[7][y+1].Trim());
			Loppu++;
			}
		}
	if (CBLaite1->ItemIndex > 0) {
		UnicodeString Str;
		Str = CBLaite1->Text + L"=";
		switch (CBLaiteYht1->ItemIndex) {
			case 0:
				Str = Str + EdLaitePort1->Text.Trim();
				break;
			case 1:
				Str = Str + L"TCPS:" + EdLaitePort1->Text.Trim();
				break;
			case 2:
				Str = Str + L"TCP:" + EdLaiteIP1->Text.Trim() + L":" + EdLaitePort1->Text.Trim();
				break;
			case 3:
				Str = Str + L"UDP:" + EdLaiteIP1->Text.Trim() + L":" + EdLaitePort1->Text.Trim();
				break;
			}
		Memo1->Lines->Insert(r++, Str);
		Loppu++;
		if (CBLaiteBaud1->ItemIndex > 0) {
			Memo1->Lines->Insert(r++, L"KELLOBAUD=" + CBLaiteBaud1->Text);
			Loppu++;
			}
		if (EdAikaEro1->Text.Trim().Length() > 0) {
			val = wstrtoaika_vap(EdAikaEro1->Text.c_str(), 0)/SEK;
			if (val > 43200)
				val -= 86400;
			Memo1->Lines->Insert(r++, L"AIKAERO1=" + UnicodeString(val));
			Loppu++;
			}
		}
	if (CBLahTiedostot->Checked) {
		Memo1->Lines->Insert(r++, UnicodeString(L"LÄHTIEDOSTOT") + (CBLahKaikkiPolku->Checked ? L"/K" : L""));
		Loppu++;
		}
	if (CBYhteysAjat->Checked) {
		Memo1->Lines->Insert(r++, UnicodeString(L"YHTEYSAJAT"));
		Loppu++;
		if (EdYhtAikaEro->Text.Trim().Length() > 0) {
			val = wstrtoaika_vap(EdYhtAikaEro->Text.c_str(), 0)/SEK;
			if (val > 43200)
				val -= 86400;
			Memo1->Lines->Insert(r++, L"AIKAERO2=" + UnicodeString(val));
			Loppu++;
			}
		}
	if (_wtoi(EdOlOsuus->Text.c_str()) > 0) {
		Memo1->Lines->Insert(r++, UnicodeString(L"OSUUSOLETUS=")+EdOlOsuus->Text.Trim());
		Loppu++;
		}
	if (_wtoi(EdOsuusraja->Text.c_str()) > 0) {
		Memo1->Lines->Insert(r++, UnicodeString(L"OSUUSRAJA=")+EdOsuusraja->Text.Trim());
		Loppu++;
		}
	if (_wtoi(EdSamapiste->Text.c_str()) > 0) {
		Memo1->Lines->Insert(r++, UnicodeString(L"SAMAPISTE=")+EdSamapiste->Text.Trim());
		Loppu++;
		}
	if (CBComAjat->Checked && _wtoi(EdComAjat->Text.c_str()) > 0) {
		Memo1->Lines->Insert(r++, UnicodeString(L"AIKACOM=")+EdComAjat->Text.Trim());
		Loppu++;
		}
	if (RGJonot->ItemIndex > 0) {
		if (CBSailAjat->Checked)
			Memo1->Lines->Insert(r++, UnicodeString(L"AJAT=/S"));
		else
			Memo1->Lines->Insert(r++, UnicodeString(L"AJAT"));
		Loppu++;
		if (RGJonot->ItemIndex == 2 && _wtoi(EdMaxjono->Text.c_str()) > 0) {
			Memo1->Lines->Insert(r++, UnicodeString(L"MAXJONO=")+EdMaxjono->Text.Trim());
			Loppu++;
			}
		}
	if (RGPiste->ItemIndex > 0) {
		UnicodeString Pisteet = L"", Jonot = L"", Par;
		int n = 0;
		switch (RGPiste->ItemIndex) {
			case 1:
				if (EdYhtPiste->Text.Trim().Length() > 0) {
					Memo1->Lines->Insert(r++, UnicodeString(L"PISTE=")+EdYhtPiste->Text.Trim());
					Loppu++;
					}
				break;
			case 2:
				for (int r = 1; r < SGAjat->RowCount; r++) {
					if (SGAjat->Cells[1][r].Trim().Length() == 0 && (
						RGJonot->ItemIndex != 2 || SGAjat->Cells[2][r].Trim().Length() == 0))
						break;
					if (SGAjat->Cells[1][r].Trim().Length() >= 0)
						Pisteet = Pisteet+SGAjat->Cells[1][r].Trim().SubString(1,1);
					if (RGJonot->ItemIndex == 2 && SGAjat->Cells[2][r].Trim().Length() >= 0)
						Jonot = Jonot+SGAjat->Cells[2][r].Trim().SubString(1,1);
					}
				if (Pisteet.Length() > 0) {
					Memo1->Lines->Insert(r++, UnicodeString(L"PISTEET=")+Pisteet);
					Loppu++;
					}
				if (Jonot.Length() > 0) {
					Memo1->Lines->Insert(r++, UnicodeString(L"JONOT=")+Jonot);
					Loppu++;
					}
				break;
			case 3:
				Memo1->Lines->Insert(r++, UnicodeString(L"PISTE=A"));
				Loppu++;
				break;
			case 4:
				Memo1->Lines->Insert(r++, UnicodeString(L"LÄHDEPISTEET"));
				Loppu++;
				break;
			}

		}
	if (CBVainVA->Checked) {
		Memo1->Lines->Insert(r++, UnicodeString(L"VAINVÄLIAJAT"));
		Loppu++;
		}
	if (CBKelloNo->Checked) {
		Memo1->Lines->Insert(r++, UnicodeString(L"KELLONO"));
		Loppu++;
		}
	if (RGEmittoiminto->ItemIndex == 1) {
		Memo1->Lines->Insert(r++, UnicodeString(L"EMIT"));
		Loppu++;
		}
	if (RGEmittoiminto->ItemIndex > 1) {
		Memo1->Lines->Insert(r++, UnicodeString(L"SISÄÄNLUENTA")+
			(RGEmittoiminto->ItemIndex == 3 ? L"=MYÖS" : L""));
		Loppu++;
		}
	for (int y = 0; y < SGLeimat->RowCount-1; y++) {
		int ny;
		if (SGLeimat->Cells[0][y+1].Length() == 0 || (ny = SGLeimat->Cells[0][y+1].ToInt()) < 1)
			continue;
		UnicodeString YhtStr = SGLeimat->Cells[1][y+1].Trim()+UnicodeString(SGLeimat->Cells[0][y+1].ToInt())+L"=";
		if (wmemcmpU(SGLeimat->Cells[2][y+1].c_str(), L"COM", 3) == 0)
			YhtStr = YhtStr+UnicodeString(_wtoi(SGLeimat->Cells[3][y+1].c_str()));
		else {
			YhtStr = YhtStr+SGLeimat->Cells[2][y+1].Trim();
			US = SGLeimat->Cells[4][y+1].Trim();
			val = _wtoi(SGLeimat->Cells[3][y+1].c_str());
			if (US.Length() == 0) {
				if (val > 0) {
					YhtStr = YhtStr + L":"+UnicodeString(val);
					}
				}
			else {
				if (wmemcmpU(SGLeimat->Cells[2][y+1].c_str(), L"TCP", 3) != 0)
					YhtStr = YhtStr + L":"+UnicodeString(val);
				YhtStr = YhtStr + L"/" + US + L":"+SGLeimat->Cells[5][y+1].Trim();
				}
			}
		Memo1->Lines->Insert(r++, YhtStr);
		Loppu++;
		if (SGLeimat->Cells[6][y+1].Trim().Length() > 3) {
			Memo1->Lines->Insert(r++, L"AIKALUKIJA"+UnicodeString(ny)+L"="+SGLeimat->Cells[6][y+1].Trim());
			Loppu++;
			}
		}
	if (CBEmitAnal->Checked) {
		Memo1->Lines->Insert(r++, UnicodeString(L"EMITANAL"));
		Loppu++;
		}
	if (RGAutorap->Visible && RGAutorap->ItemIndex > 0) {
		switch (RGAutorap->ItemIndex) {
			case 1:
				Memo1->Lines->Insert(r++, UnicodeString(L"AUTORAP"));
				break;
			case 2:
				Memo1->Lines->Insert(r++, UnicodeString(L"AUTORAP=H"));
				break;
			case 3:
				Memo1->Lines->Insert(r++, UnicodeString(L"RASTIVATULOSTE"));
				break;
			}
		Loppu++;
		}
	if (RGComAutorap->Visible && RGComAutorap->ItemIndex > 0) {
		switch (RGComAutorap->ItemIndex) {
			case 1:
				Memo1->Lines->Insert(r++, UnicodeString(L"COMAUTORAP"));
				break;
			case 2:
				Memo1->Lines->Insert(r++, UnicodeString(L"COMAUTORAP=H"));
				break;
			}
		Loppu++;
		}
	if (RGLainat->Visible && RGLainat->ItemIndex > 0) {
		switch (RGLainat->ItemIndex) {
			case 1:
				Memo1->Lines->Insert(r++, UnicodeString(L"LAINAT"));
				break;
			case 2:
				if (EdLainakortit->Text.Trim().Length() > 0)
					Memo1->Lines->Insert(r++, UnicodeString(L"LAINALUETTELO=")+EdLainakortit->Text.Trim());
				else
					Memo1->Lines->Insert(r++, UnicodeString(L"LAINALUETTELO"));
				break;
			case 3:
				if (EdLainakortit->Text.Trim().Length() > 0)
					Memo1->Lines->Insert(r++, UnicodeString(L"LAINALUETTELO=")+EdLainakortit->Text.Trim()+L"/VAIN");
				else
					Memo1->Lines->Insert(r++, UnicodeString(L"LAINALUETTELO/VAIN"));
				break;
			}
		Loppu++;
		}
	if (CBJalkiSyotto->Checked) {
		Memo1->Lines->Insert(r++, UnicodeString(L"JÄLKISYÖTTÖ"));
		Loppu++;
		}
	if (CBVaadiAika->Checked) {
		Memo1->Lines->Insert(r++, UnicodeString(L"VAADIAIKA"));
		Loppu++;
		}
	if (RGEmittoiminto->ItemIndex > 1 && CBLuentaLoki->Checked) {
		Memo1->Lines->Insert(r++, UnicodeString(L"LUENTALOKI"));
		Loppu++;
		}
	if (CBKaksiTunn->Checked) {
		Memo1->Lines->Insert(r++, UnicodeString(L"KAKSITUNN"));
		Loppu++;
		}
	if (EdKartta->Text.Trim().Length() > 0) {
		Memo1->Lines->Insert(r++, UnicodeString(L"KARTTA=") + EdKartta->Text.Trim());
		Loppu++;
		}
	if (_wtoi(EdEmitAlku->Text.c_str()) > 0) {
		Memo1->Lines->Insert(r++, UnicodeString(L"EMITALKU=") + EdEmitAlku->Text.Trim());
		Loppu++;
		}
	if (CBAikaLukija->ItemIndex > 0) {
		switch (CBAikaLukija->ItemIndex) {
			case 1:
				Memo1->Lines->Insert(r++, UnicodeString(L"AIKALUKIJA=VAIN"));
				Loppu++;
				break;
			case 2:
				if (EdAikaLukija->Text.Trim().Length() > 0) {
					Memo1->Lines->Insert(r++, UnicodeString(L"AIKALUKIJA=VAIN")+EdAikaLukija->Text.SubString(1,1));
					Loppu++;
					}
				break;
			case 3:
				Memo1->Lines->Insert(r++, UnicodeString(L"AIKALUKIJA=VAINA"));
				Loppu++;
				break;
			case 4:
				if (EdAikaLukija->Text.Trim().Length() > 0) {
					Memo1->Lines->Insert(r++, UnicodeString(L"AIKALUKIJA=LÄHDE")+EdAikaLukija->Text.SubString(1,1));
					Loppu++;
					}
				break;
			}
		}
	if (_wtoi(EdEmitJono->Text.c_str()) > 1) {
		Memo1->Lines->Insert(r++, UnicodeString(L"EMITJONO=") + EdEmitJono->Text.Trim());
		Loppu++;
		}
	if (CBEmitAjat->Checked) {
		if (_wtoi(EdEmitAjat->Text.c_str()) > 1)
			Memo1->Lines->Insert(r++, UnicodeString(L"EMITAJAT=") + EdEmitAjat->Text.Trim());
		else
			Memo1->Lines->Insert(r++, UnicodeString(L"EMITAJAT"));
		Loppu++;
		}
	if (RGKirjoitin->ItemIndex > 0) {
		if (RGKirjoitin->ItemIndex == 1 || EdKirjoitin->Text.Trim().Length() == 0)
			Memo1->Lines->Insert(r++, UnicodeString(L"LISTA"));
		else
			Memo1->Lines->Insert(r++, UnicodeString(L"LISTA=") + EdKirjoitin->Text.Trim());
		Loppu++;
		}
	if (EdMuotoilu->Text.Trim().Length() > 0) {
		Memo1->Lines->Insert(r++, UnicodeString(L"MUOTOILU=") + EdMuotoilu->Text.Trim());
		Loppu++;
		}
	if (RGLoki->ItemIndex > 0) {
		if (RGLoki->ItemIndex == 1 || EdLoki->Text.Trim().Length() == 0)
			Memo1->Lines->Insert(r++, UnicodeString(L"LOKI"));
		else
			Memo1->Lines->Insert(r++, UnicodeString(L"LOKI=") + EdLoki->Text.Trim());
		Loppu++;
		}
/*
	if (CBAutoLaajuus->ItemIndex > 0) {
		UnicodeString autoStr;
		switch (CBAutoLaajuus->ItemIndex) {
			case 1:
				autoStr = L"AUTO=L";
				break;
			case 2:
				autoStr = L"AUTO=I";
				break;
			case 3:
				autoStr = L"AUTO=P"+EdAutoParhaat->Text.Trim();
				break;
			}
		autoStr = autoStr + L"/" + EdAutoKynnys->Text.Trim() + L"/" + EdAutoAskel->Text.Trim() +
			L"/" + EdAutoMaalit->Text.Trim();
		if (CBAutoMonivaihe->ItemIndex > 0)
			autoStr = autoStr + L"/" + L"2VY1"[CBAutoMonivaihe->ItemIndex-1];
		Memo1->Lines->Insert(r++, autoStr);
		Loppu++;
		}
*/
	if (RBEiAutotied->Checked == false) {
		if (RBXml->Checked) {
			Memo1->Lines->Insert(r++, UnicodeString(L"XML=") + EdAutoTiedosto->Text.Trim() + L"/" +
				EdAutoTiedVali->Text.Trim());
			Loppu++;
			}
		else {
			Memo1->Lines->Insert(r++, UnicodeString(L"HTML=") + EdAutoTiedosto->Text.Trim() + L"/" +
				EdAutoTiedVali->Text.Trim() + (RBSarjatHtml->Checked ? L"/S" : L"") +
				(CBAfMolemmat->Checked ? L"/M" : L""));
			Loppu++;
			if (_wtoi(EdHtmlTitle->Text.c_str()) > 0) {
				Memo1->Lines->Insert(r++, UnicodeString(L"HTMLTITLE=") + EdHtmlTitle->Text.Trim());
				Loppu++;
				}
			if (_wtoi(EdHtmlOts->Text.c_str()) > 0) {
				Memo1->Lines->Insert(r++, UnicodeString(L"HTMLOTS=") + EdHtmlOts->Text.Trim());
				Loppu++;
				}
			}
		}
	if (CBVaadiTark->Checked) {
		Memo1->Lines->Insert(r++, UnicodeString(L"VAADITARK"));
		Loppu++;
		}
	if (CBAutoJakelu->ItemIndex == 1) {
		Memo1->Lines->Insert(r++, UnicodeString(L"AUTOJAKELU=") + EdAutoJakelu->Text.Trim());
		Loppu++;
		}
	if (CBAutoJakelu->ItemIndex == 2) {
		Memo1->Lines->Insert(r++, UnicodeString(L"KOMENTO=") + EdAutoJakelu->Text.Trim());
		Loppu++;
		}
	if (CBAutoEtuSuku->Checked == false) {
		Memo1->Lines->Insert(r++, UnicodeString(L"HTMLSÄILNIMI"));
		Loppu++;		
		}
	if (CB25rivi->Checked) {
		Memo1->Lines->Insert(r++, UnicodeString(L"RIVIT=25"));
		Loppu++;		
		}
	if (CBKirkas->Checked) {
		Memo1->Lines->Insert(r++, UnicodeString(L"KIRKAS"));
		Loppu++;		
		}
	if (EdVarit->Text.Trim().Length() > 3) {
		Memo1->Lines->Insert(r++, UnicodeString(L"VÄRIT=") + EdVarit->Text.Trim());
		Loppu++;
		}
	if (CBBoot->ItemIndex > 10) {
		Memo1->Lines->Insert(r++, UnicodeString(L"BOOT") + 
			(CBBoot->ItemIndex > 1 ? UnicodeString(L"=M")+UnicodeString(CBBoot->ItemIndex-1): UnicodeString(L"")));
		Loppu++;			
		}
	if (CBUudenTulos->Checked) {
		Memo1->Lines->Insert(r++, UnicodeString(L"SUORASYÖTTÖ"));
		Loppu++;		
		}
	if (EdNappain->Text.Trim().Length() > 2 && EdNappain->Text.Trim() != L"0,0") {
		Memo1->Lines->Insert(r++, UnicodeString(L"NÄPPÄIN=") + EdNappain->Text.Trim());
		Loppu++;
		}
	if (CBEnnakoi->Checked) {
		Memo1->Lines->Insert(r++, UnicodeString(L"ENNAKOI"));
		Loppu++;
		}
	for (int r = Alku; r < Loppu; r++)
		Tulkitut[r] = 1;
}
//---------------------------------------------------------------------------

static void tulktcpipparam(wchar_t *str, wchar_t *iposoite, int *port)
{
	wchar_t *p;
	int j = 0;

	iposoite[0] = 0;
	*port = 0;
	p = wcstok(str, L":,/=");
	if (p) {
		wcsncpy(iposoite, p, 98);
		p = wcstok(NULL, L":,/=");
		}
	if (p) {
		if ((j = _wtoi(p)) > 0)
			*port = j;
		}
	else {
		*port = _wtoi(p);
		}
}
//---------------------------------------------------------------------------
void TFormCfg::TulkitseTiedot(bool tyhjenna)
{
   wchar_t fld[200] = L"", fldn[90] = L"";
   wchar_t *p;
   wchar_t msg[401];
	INT i, j, ny, nf,nc,ncl,pos;
   INT32 y, y1;

	i = 0;
	Alku = 0;
	Loppu = Memo1->Lines->Count;
	if (!tyhjenna) {
		memset(Tulkitut, 0, sizeof(Tulkitut));
		for (i = 0; (tyhjenna && i == 0) || i < Memo1->Lines->Count; i++) {
			wcsncpy(fld, Memo1->Lines->Strings[i].c_str(), 199);
			if (wmemcmpU(fld, L"//CfgEditorAlku", 15) == 0) {
				Alku = i+1;
				break;
				}
			}
		}
	for (i = Alku; (tyhjenna && i == 0) || i < Memo1->Lines->Count; i++) {
		if (!tyhjenna) {
			wcsncpy(fld, Memo1->Lines->Strings[i].c_str(), 199);
			if (wmemcmpU(fld, L"//CfgEditorLoppu", 16) == 0) {
				Loppu = i;
				break;
				}
			 memset(fldn, 0, sizeof(fldn));
			 for (p = fld; *p; p++) {
				switch (*p) {
				   case 196 :
					  *p = L'Ä';
					  break;
				   case 228 :
					case 245 :
					  *p = L'ä';
					  break;
				   case 214 :
					case 205 :
					  *p = L'Ö';
					  break;
					case 246 :
				   case 247 :
					  *p = L'ö';
					  break;
				   case 197 :
					  *p = L'';
					  break;
				   case 229 :
					case 213 :
					  *p = L'å';
					  break;
				   }
				}
			 wcsncpy(fldn, fld, 7);
//			 upcasewstr(fldn);
			 if (!wmemcmpU(fldn, L"HTML", 4) || !wmemcmpU(fldn, L"LISTA=", 6)
				|| !wmemcmpU(fldn, L"SQL", 3)
				|| !wmemcmpU(fldn, L"LOKI=", 5) || !wmemcmpU(fldn, L"TARRA=", 6)
				|| !wmemcmpU(fldn, L"KOMENTO", 7) || !wmemcmpU(fldn, L"TAULUOTS", 8)
				|| !wmemcmpU(fldn, L"KILPKDI", 7) || !wmemcmpU(fldn, L"SIRITAR", 7)
				|| !wmemcmpU(fldn, L"SIRITDE", 7) || !wmemcmpU(fldn, L"IKKUNAO", 7)) {
				if (wcslen(fld) > 7)
				   wcsncpy(fldn+7, fld+7, 76);
				}
			 else
				{
				wcsncpy(fldn, fld, 89);
				for (p = fldn; *p && *p != L' '; p++) ;
				if (*p == L' ') *p = 0;
//				upcasewstr(fldn);
				}
			}
		if (tyhjenna) {
			EdKone->Text = L"";
			}
		else if( !wmemcmpU(fldn, L"KONE=",5)) {
			Tulkitut[i] = 1;
			 fldn[7] = 0;
			 EdKone->Text = fldn+5;
			 continue;
			 }
		if (tyhjenna) {
			EdIkkOts->Text = L"";
			}
		else if(!wmemcmpU(fldn, L"IKKUNAO", 7)) {
			Tulkitut[i] = 1;
			p = wcsstr(fldn+6,L"=");
			if (p) {
				EdIkkOts->Text = p+1;
				}
			continue;
			}
		if (tyhjenna) {
			CBIkkunat->Checked = false;
			}
		else if(!wmemcmpU(fldn, L"IKKUNAT", 7)) {
			Tulkitut[i] = 1;
			CBIkkunat->Checked = true;
			if (fldn[7] == L'=')
				EdIkkunat->Text = fldn+8;
			else
				EdIkkunat->Text = L"ikkunat.xml";
			continue;
			}
		if (tyhjenna) {
			CBKirkas->Checked = false;
			}
		else if(!wmemcmpU(fldn, L"KIRKAS", 5)) {
			Tulkitut[i] = 1;
			CBKirkas->Checked = true;
			continue;
			}
		if (tyhjenna) {
			EdVarit->Text = L"";
			}
		else if(!wmemcmpU(fldn, L"VÄRIT=", 6)) {
			Tulkitut[i] = 1;
			EdVarit->Text = fldn+6;
			continue;
			}
		if (tyhjenna) {
			CB25rivi->Checked = false;
			}
		else if(!wmemcmpU(fldn, L"RIVIT=25", 8)) {
			Tulkitut[i] = 1;
			CB25rivi->Checked = true;
			continue;
			}
		else if(!wmemcmpU(fldn, L"RASTIVATULOSTE", 9)) {
			Tulkitut[i] = 1;
			RGAutorap->ItemIndex = 3;
			continue;
			}
		else if(!wmemcmpU(fldn, L"EMITALKU=", 9)) {
			Tulkitut[i] = 1;
			if ((p = wcsstr(fldn, L"=")) != NULL) {
				EdEmitAlku->Text = _wtoi(p+1);
				}
			continue;
			}
		if (tyhjenna) {
			CBVaadiTark->Checked = false;
			}
		else if(!wmemcmpU(fldn, L"VAADITARK", 8)) {
			Tulkitut[i] = 1;
			CBVaadiTark->Checked = true;
			continue;
			}
		if (tyhjenna) {
			CBJoustoViesti->Checked = false;
			}
		else if(!wmemcmpU(fldn, L"JOUSTOVIESTI", 8)) {
			Tulkitut[i] = 1;
			CBJoustoViesti->Checked = true;
			continue;
			}
		if (tyhjenna) {
			CBVuorokaudet->Checked = false;
			}
		else if(!wmemcmpU(fldn, L"VUOROKAUDET", 8)) {
			Tulkitut[i] = 1;
			CBVuorokaudet->Checked = true;
			continue;
			}
		if (tyhjenna) {
			CBNaytaHaj->Checked = false;
			}
		else if(!wmemcmpU(fldn, L"NÄYTÄHAJONTA=EI", 14)) {
			Tulkitut[i] = 1;
			CBVuorokaudet->Checked = true;
			continue;
			}
		if (tyhjenna) {
			CBSailHyl->Checked = false;
			}
		else if(!wmemcmpU(fldn, L"SÄILHYL", 8)) {
			Tulkitut[i] = 1;
			CBSailHyl->Checked = true;
			continue;
			}
		if (tyhjenna) {
			EdMuotoilu->Text = L"";
			}
		else if(!wmemcmpU(fldn, L"MUOTOILU", 5)) {
			Tulkitut[i] = 1;
			EdMuotoilu->Text = fldn+9;
			continue;
			}
		if (tyhjenna) {
			RGLoki->ItemIndex = 0;
			EdLoki->Text = L"";
			}
		else if(!wmemcmpU(fldn, L"LOKI", 4)) {
			Tulkitut[i] = 1;
			if (wcslen(fldn) > 5) {
				EdLoki->Text = fldn+5;
				RGLoki->ItemIndex = 2;
				}
			else {
				RGLoki->ItemIndex = 1;
				EdLoki->Text = L"";
				}
			continue;
			}
		if (tyhjenna) {
			RGKirjoitin->ItemIndex = 0;
			EdKirjoitin->Text = L"";
			}
		else if (!wmemcmpU(fldn, L"LISTA", 5)) {
			Tulkitut[i] = 1;
			if (fldn[5] == L'=') {
				EdKirjoitin->Text = fldn+6;
				RGKirjoitin->ItemIndex = 2;
				}
			else {
				EdKirjoitin->Text = L"";
				RGKirjoitin->ItemIndex = 1;
				}
			continue;
			}
/*
		if (tyhjenna) {
			CBAutoLaajuus->ItemIndex = 0;
			EdAutoParhaat->Text = L"10";
			EdAutoKynnys->Text = L"10";
			EdAutoAskel->Text = L"60";
			EdAutoMaalit->Text = L"123456789";
			CBAutoMonivaihe->ItemIndex = 0;
			}
		else if( !wmemcmpU(fldn, L"AUTO", 4) && (!fldn[4] || fldn[4] == L'=')) {
			Tulkitut[i] = 1;
			if (!fldn[4]) {
				CBAutoLaajuus->ItemIndex = 1;
				}
			else {
				p = wcstok(fldn+5, L"/,;");
				if (p) {
					switch (towupper(*p)) {
						case L'L':
							CBAutoLaajuus->ItemIndex = 1;
							break;
						case L'I':
							CBAutoLaajuus->ItemIndex = 2;
							break;
						case L'P':
							CBAutoLaajuus->ItemIndex = 3;
							EdAutoParhaat->Text = _wtoi(p+1);
							break;
						}
					p = wcstok(NULL, L"/,;");
					}
				if (p) {
					EdAutoKynnys->Text  = _wtoi(p);
					p = wcstok(NULL, L"/,;");
					}
				if (p) {
					EdAutoAskel->Text  = _wtoi(p);
					p = wcstok(NULL, L"/,;");
					}
				if (p) {
					EdAutoKynnys->Text  = p;
					p = wcstok(NULL, L"/,;");
					}
				if (p) {
					CBAutoMonivaihe->ItemIndex = wcswcind(*p, L"2VY1") + 1;
					}
				}
			continue;
			}
		if (tyhjenna) {
			CBDesErotin->ItemIndex = 0;
			}
		else if (!wmemcmpU(fldn, L"DESEROTIN=", 10)) {
			Tulkitut[i] = 1;
			CBDesErotin->ItemIndex = fldn[10] == L'.' ? 1 : 0;
			continue;
			}
*/
		if (tyhjenna) {
			RBEiAutotied->Checked = true;
			EdAutoTiedosto->Text = L"";
			EdAutoJakelu->Text = L"";
			CBAutoJakelu->ItemIndex = 0;
			CBAutoEtuSuku->Checked = true;
			CBAfMolemmat->Checked = false;
			EdAutoTiedVali->Text = 60;
			}
		else if (!wmemcmpU(fldn, L"HTML=", 5) || !wmemcmpU(fldn, L"XML=", 4)) {
			Tulkitut[i] = 1;
			p = wcstok(fldn, L"=");
			if (towupper(*p) == L'X')
				RBXml->Checked = true;
			else {
				RBYksiHtml->Checked = true;
				}
			if ((p = wcstok(NULL, L",/")) != NULL) {
				EdAutoTiedosto->Text = p;
				p = wcstok(NULL, L",/");
				}
			if (p) {
				if ((ny = _wtoi(p)) != 0)
					EdAutoTiedVali->Text = ny;
				p = wcstok(NULL, L",/");
				}
			if (p && towupper(*p) != L'M' &&
				(towupper(*p) == L'S' || ((p = wcstok(NULL, L",/")) != NULL) && towupper(*p) == L'S')) {
				RBSarjatHtml->Checked = true;
			   }
			if (p && (towupper(*p) == L'M' ||
				((p = wcstok(NULL, L",/")) != NULL) && towupper(*p) == L'M')) {
				CBAfMolemmat->Checked = false;
				}
			continue;
			}
		if (tyhjenna) {
			EdHtmlTitle->Text = L"";
			}
		else if( !wmemcmpU(fldn, L"HTMLTIT", 7)) {
			Tulkitut[i] = 1;
			p = wcsstr(fldn, L"=");
			if (p && p[1])
				EdHtmlTitle->Text = p+1;
			continue;
			}
		if (tyhjenna) {
			EdHtmlOts->Text = L"";
			}
		else if( !wmemcmpU(fldn, L"HTMLOTS", 7)) {
			Tulkitut[i] = 1;
			p = wcsstr(fldn, L"=");
			if (p && p[1])
				EdHtmlOts->Text = p+1;
			continue;
			}
		if (tyhjenna) {
			CBAutoJakelu->ItemIndex = 0;
			EdAutoJakelu->Text = L"";
			}
		else if( !wmemcmpU(fldn, L"KOMENTO", 7)) {
			Tulkitut[i] = 1;
			CBAutoJakelu->ItemIndex = 2;
			p = wcsstr(fldn, L"=");
			if (p && p[1]) {
				EdAutoJakelu->Text = p+1;
				}
			continue;
			}
		if (tyhjenna) {
			}
		else if( !wmemcmpU(fldn, L"AUTOJAKELU", 7)) {
			Tulkitut[i] = 1;
			CBAutoJakelu->ItemIndex = 1;
			p = wcsstr(fldn, L"=");
			if (p && p[1]) {
				EdAutoJakelu->Text = p+1;
				}
			continue;
			}
		if (tyhjenna) {
			CBBoot->ItemIndex = 0;
			}
		else if( !wmemcmpU(fldn, L"BOOT", 4)) {
			Tulkitut[i] = 1;
			if (!wmemcmpU(fldn + 4, L"=M", 2)) {
				if (fldn[6] > L'0' && fldn[6] <= L'0' + MAXJONO)
					CBBoot->ItemIndex = (fldn[6] - L'0') + 1;
				else
					CBBoot->ItemIndex = 2;
				}
			else
				CBBoot->ItemIndex = 1;
			continue;
			}
		if (tyhjenna) {
			for (int r = 1; r < SG1->RowCount; r++) {
				for (int c = 0; c < SG1->ColCount; c++)
					SG1->Cells[c][r] = L"";
				}
			}
		else if (((!wmemcmpU(fldn, L"COM",3) && (pos = 3) != 0) ||
			(!wmemcmpU(fldn, L"YHTEYS",6) && (pos = 6) != 0)) &&
			fldn[pos] <= L'=' && (ny = yhteys_no(fldn, &pos)) != 0) {
			int r;
			Tulkitut[i] = 1;
			for (r = 1; r < SG1->RowCount-1; r++)
				if (SG1->Cells[0][r].Length() == 0 || SG1->Cells[0][r].ToInt() == 0)
					break;
			SG1->Cells[0][r] = ny;
			if (!wmemcmpU(fldn+pos+1, L"UDP", 3) || !wmemcmpU(fldn+pos+1, L"BRO", 3)) {
				SG1->Cells[1][r] = UnicodeString(fldn+pos+1).SubString(1,3);
				pos += 4;
				if (towupper(fldn[pos]) == L'I') {
					SG1->Cells[2][r] = L"In";
					pos++;
					}
				else if (towupper(fldn[pos]) == L'O') {
					SG1->Cells[2][r] = L"Out";
					pos++;
					}
				p = fldn+pos;
				if (wcswcind(*p, L":,/=") >= 0) {
					p = wcstok(p+1, L":,/=");
					if (p && (j = _wtoi(p)) >= 0) {
						if (j)
							SG1->Cells[3][r] = j;
						p = wcstok(NULL, L":,/=");
						if (p) {
							SG1->Cells[4][r] = p;
							p = wcstok(NULL, L":,/=");
							}
						if (p) {
							if ((j = _wtoi(p)) >= 0) {
								if (!j) {
									if (towupper(*p) == L'Y') {
										while (*p > L'9')
											p++;
											if ((j = _wtoi(p)) > 0)
												SG1->Cells[5][r] = L"Y"+UnicodeString(j);
										}
									}
								else
									SG1->Cells[5][r] = UnicodeString(j);
								}
							}
						}
					}
				continue;
				}
			if (!wmemcmpU(fldn+pos+1, L"TCP", 3)) {
				SG1->Cells[1][r] = L"TCP";
				if (towupper(fldn[pos+4]) == L'X' || wmemcmpU(fldn+pos+4, L"SX", 2) == 0) {
					pos++;
					if (wmemcmpU(fldn + pos + 3, L"XS", 2) == 0 || wmemcmpU(fldn + pos + 3, L"SX", 2) == 0) {
						SG1->Cells[1][r] = L"TCPS";
						pos++;
						}
					SG1->Cells[2][r] = L"Out";
					SG1->Cells[6][r] = SG1->Cells[6][r] + L"Xml";
					}
				if (towupper(fldn[pos+4]) == L'Z') {
					SG1->Cells[2][r] = L"Out";
					SG1->Cells[6][r] = SG1->Cells[6][r] + L"Zzz";
					pos++;
					}
				if (towupper(fldn[pos+4]) == L'L') {
					SG1->Cells[2][r] = L"Out";
					SG1->Cells[6][r] = SG1->Cells[6][r] + L"Lki";
					pos++;
					}
				if (towupper(fldn[pos+4]) == L'S') {
					SG1->Cells[1][r] = L"TCPS";
					pos++;
					}
				if (towupper(fldn[pos+4]) == L'I') {
					SG1->Cells[2][r] = L"In";
					pos++;
					}
				else if (towupper(fldn[pos+4]) == L'O') {
					SG1->Cells[2][r] = L"Out";
					pos++;
					}
				p = fldn+pos+4;
				if (wcswcind(*p, L":,/=") >= 0) {
					wchar_t iposoite[100];
					tulktcpipparam(p, iposoite, &j);
					if (iposoite[0]) {
						SG1->Cells[4][r] = iposoite;
						if (j > 0)
							SG1->Cells[5][r] = j;
						}
					else {
						SG1->Cells[3][r] = j;
						}
					}
				continue;
				}
			if (!wmemcmpU(fldn+pos+1, L"COM", 3)) pos += 3;
			pos++;
			SG1->Cells[3][r] = _wtoi(fldn+pos);
			p = fldn + wcslen(fldn) - 1;
			if (towupper(*p) == L'I')
				SG1->Cells[2][r] = L"In";
			if (towupper(*p) == L'O')
				SG1->Cells[2][r] = L"Out";
			continue;
			}
		if (tyhjenna) {
			EdPortBase->Text = 15900;
			}
		else if (!wmemcmpU(fldn, L"PORTBASE=",9)) {
			Tulkitut[i] = 1;
			EdPortBase->Text = _wtoi(fldn+9);
			continue;
			}
		if (tyhjenna) {
			EdMaxYhteys->Text = 0;
			}
		else if (!wmemcmpU(fldn, L"MAXYHTEYS=",10)) {
			Tulkitut[i] = 1;
			EdMaxYhteys->Text = _wtoi(fldn+10);
			continue;
			}
		if (tyhjenna) {
			}
		else if (!wmemcmpU(fldn, L"VÄLITÄ",6) && (pos = 6) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
			Tulkitut[i] = 1;
			for (y = 1; y < SG1->RowCount; y++) {
				if (pos == 6 || (SG1->Cells[0][y].Length() > 0 && SG1->Cells[0][y].ToInt() == ny)) {
					if (fldn[pos+1] > L'0' && fldn[pos+1] <= L'9')
						SG1->Cells[7][y] = fldn+pos+1;
					else
						SG1->Cells[7][y] = L"Ei";
					}
				}
			continue;
			}
		if (tyhjenna) {
			CBLahTiedostot->Checked = false;
			CBLahKaikkiPolku->Checked = false;
			}
		else if (!wmemcmpU(fldn, L"LÄHTIEDOSTOT",12)) {
			Tulkitut[i] = 1;
			pos = 12;
			if (fldn[12] == 0 || fldn[12] == L'/') {
				CBLahTiedostot->Checked = true;
				}
			else {
				if ((ny = yhteys_no(fldn, &pos)) != 0) {
					for (y = 1; y < SG1->RowCount; y++) {
						if (SG1->Cells[0][y].Length() > 0 && SG1->Cells[0][y].ToInt() == ny) {
							SG1->Cells[6][y] = SG1->Cells[6][y]+L"Tie";
							}
						}
					}
				}
			if (wcsstr(fldn, L"/K") != NULL || wcsstr(fldn, L"/k") != NULL) {
				CBLahKaikkiPolku->Checked = false;
				}
			continue;
			}
		if (tyhjenna) {
			EdSammSalasana->Text = L"";
			}
		else if (!wmemcmpU(fldn, L"SULKUSALASANA=",14)) {
			Tulkitut[i] = 1;
			EdSammSalasana->Text = fldn+14;
			continue;
			}
		if (tyhjenna) {
			}
		else if( !wmemcmpU(fldn, L"BAUD",4) && (pos = 4) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
			Tulkitut[i] = 1;
			for (y = 1; y < SG1->RowCount; y++)
				if (SG1->Cells[1][y] == UnicodeString(L"COM"))
					SG1->Cells[4][y] = _wtoi(fldn+pos+1);
			continue;
			}
		if (tyhjenna) {
			CBComfile->ItemIndex = 0;
			}
		else if( !wmemcmpU(fldn, L"COMFILE",7)) {
			Tulkitut[i] = 1;
			if (fldn[7] && towupper(fldn[8]) == L'S')
				CBComfile->ItemIndex = 1;
			continue;
			}
		if (tyhjenna) {
			CBYhteysAjat->Checked = false;
			}
		else if(!wmemcmpU(fldn, L"VA-AJAT", 7) || !wmemcmpU(fldn, L"YHTEYSAJAT", 10)) {
			Tulkitut[i] = 1;
			CBYhteysAjat->Checked = true;
			continue;
			}
		if (tyhjenna) {
			CBAani->ItemIndex = 1;
			}
		else if( !wmemcmpU(fldn, L"ÄÄNI=",5)) {
			Tulkitut[i] = 1;
			CBAani->ItemIndex = fldn[5] - L'0';
			continue;
			}
		 if (tyhjenna) {
			CBSailAjat->Checked = true;
			RGJonot->ItemIndex = 0;
			}
		else if(!wmemcmpU(fldn, L"AJAT", 4)) {
			Tulkitut[i] = 1;
			if (RGJonot->ItemIndex == 0)
				RGJonot->ItemIndex = 1;
			CBSailAjat->Checked = false;
			if ((fldn[wcslen(fldn)-2] == L',' ||
				fldn[wcslen(fldn)-2] == L'/') &&
				towupper(fldn[wcslen(fldn)-1]) == L'S')
				CBSailAjat->Checked = true;
			continue;
			}
		 if (tyhjenna) {
			}
		else if(!wmemcmpU(fldn, L"MAXJONO=", 8)) {
			Tulkitut[i] = 1;
			RGJonot->ItemIndex = 2;
			EdMaxjono->Text = fldn+8;
			continue;
			}
		 if (tyhjenna) {
			EdNappain->Text = L"0,0";
			}
		else if(!wmemcmpU(fldn, L"NÄPPÄIN=", 8)) {
			Tulkitut[i] = 1;
			if ((p = wcstok(fldn+8, L",/")) == NULL) continue;
			y = _wtoi(p);
			if ((p = wcstok(NULL, L",/")) != NULL && (ny = _wtoi(p)) != 0)
				EdNappain->Text = UnicodeString(y)+L","+UnicodeString(ny);
			continue;
			}
		if (tyhjenna) {
			}
		else if( !wmemcmpU(fldn, L"LÄHAIKA",7) && (pos = 7) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
			Tulkitut[i] = 1;
			for (y = 1; y < SG1->RowCount; y++) {
				if (pos == 6 || (SG1->Cells[0][y].Length() > 0 && SG1->Cells[0][y].ToInt() == ny)) {
					if (towupper(fldn[pos+1]) == L'V')
						SG1->Cells[6][y] = SG1->Cells[6][y]+L"AiV";
					else if (towupper(fldn[pos+1]) == L'P')
						SG1->Cells[6][y] = SG1->Cells[6][y]+L"AiP";
					else
						SG1->Cells[6][y] = SG1->Cells[6][y]+L"Aik";
					}
				}
			continue;
			}
		 if (tyhjenna) {
			}
		else if( !wmemcmpU(fldn, L"LÄHECODE",8) && (pos = 8) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
			Tulkitut[i] = 1;
			for (y = 1; y < SG1->RowCount; y++) {
				if (pos == 6 || (SG1->Cells[0][y].Length() > 0 && SG1->Cells[0][y].ToInt() == ny)) {
					SG1->Cells[6][y] = SG1->Cells[6][y]+L"ECd";
					}
				}
			continue;
			}
		 if (tyhjenna) {
			CBEnnakoi->Checked = false;
			}
		else if( !wmemcmpU(fldn, L"ENNAKOI",7)) {
			Tulkitut[i] = 1;
			CBEnnakoi->Checked = true;
			continue;
			}
		 if (tyhjenna) {
			EdAikaEro1->Text = L"";
			EdYhtAikaEro->Text = L"";
			}
		else if (!wmemcmpU(fldn, L"AIKAERO", 7)) {
			wchar_t as[20];
			Tulkitut[i] = 1;
			if (fldn[7] == L'=') {
				j = (86400 + _wtoi(fldn+8)) % 86400;
				EdAikaEro1->Text = aikatowstr_cols_n(as, SEK*j, 0, 0, 8);
				EdYhtAikaEro->Text = EdAikaEro1->Text;
				}
			else {
				pos = 7;
				if ((ny = yhteys_no(fldn, &pos)) != 0 && ny <= NREGNLY) {
					j = (86400 + _wtoi(fldn+pos+1)) % 86400;
					if (ny == 0)
						EdAikaEro1->Text = aikatowstr_cols_n(as, SEK*j, 0, 0, 8);
					if (ny == 2)
						EdYhtAikaEro->Text = aikatowstr_cols_n(as, SEK*j, 0, 0, 8);
					}
				}
			continue;
			}
		 if (tyhjenna) {
			EdOlOsuus->Text = 0;
			}
		else if( !wmemcmpU(fldn, L"OSUUSOLETUS=",12)) {
			Tulkitut[i] = 1;
			EdOlOsuus->Text = _wtoi(fldn+12);
			continue;
			}
		 if (tyhjenna) {
			EdOsuusraja->Text = 0;
			}
		else if( !wmemcmpU(fldn, L"OSUUSRAJA=",10)) {
			Tulkitut[i] = 1;
			EdOsuusraja->Text = _wtoi(fldn+10);
			continue;
			}
		 if (tyhjenna) {
			EdSamapiste->Text = 0;
			}
		else if( !wmemcmpU(fldn, L"SAMAPISTE=",10)) {
			Tulkitut[i] = 1;
			EdSamapiste->Text = _wtoi(fldn+10);
			continue;
			}
		 if (tyhjenna) {
			CBComAjat->Checked = false;
			EdComAjat->Text = 0;
			}
		else if( !wmemcmpU(fldn, L"AIKA_COM=",9)) {
			Tulkitut[i] = 1;
			CBComAjat->Checked = true;
			EdComAjat->Text = _wtoi(fldn+9);
			continue;
			}
		if (tyhjenna) {
			CBKelloNo->Checked = false;
			}
		else if ((!wmemcmpU(fldn, L"REGNLYNO",8) && (pos = 8) != 0)
			  || (!wmemcmpU(fldn, L"KELLONO",7) && (pos = 7) != 0)
			  || (!wmemcmpU(fldn, L"COMETNO",7) && (pos = 7) != 0)
			  || (!wmemcmpU(fldn, L"TIMYNO",6) && (pos = 6) != 0)
			  || (!wmemcmpU(fldn, L"TDCNO",5) && (pos = 5) != 0)
			  || (!wmemcmpU(fldn, L"ASCNO",5) && (pos = 5) != 0)) {
			Tulkitut[i] = 1;
			CBKelloNo->Checked = true;
		   }
		if (tyhjenna) {
			CBLaite1->ItemIndex = 0;
			CBLaiteYht1->ItemIndex = 0;
			EdLaitePort1->Text = 0;
			EdLaiteIP1->Text = L"000.000.000.000";
			CBLaiteBaud1->ItemIndex = 0;
			}
		else if (!wmemcmpU(fldn, L"REGNLY",5)) {
			Tulkitut[i] = 1;
			if (fldn[6] == L'2')
				CBLaite1->ItemIndex = 7;
			else
				CBLaite1->ItemIndex = 6;
			continue;
			}
		if (tyhjenna) {
			}
		else if (!wmemcmpU(fldn, L"ALGE",4)
			  || !wmemcmpU(fldn, L"COMET",5)
			  || !wmemcmpU(fldn, L"RADIOEMIT",8)
			  || !wmemcmpU(fldn, L"TIMY",4)
			  || !wmemcmpU(fldn, L"FEIG",4)
			  || !wmemcmpU(fldn, L"SIRIT",5)
			  || !wmemcmpU(fldn, L"IMPINJ",6)
			  || !wmemcmpU(fldn, L"ARES",4)
			  || !wmemcmpU(fldn, L"SW2000",6)
			  || !wmemcmpU(fldn, L"ASC",3)) {
			int rno = 0;
			if( !wmemcmpU(fldn, L"COMET",5))
			   {
				Tulkitut[i] = 1;
				CBLaite1->ItemIndex = 3;
			   }
			else if (!wmemcmpU(fldn, L"TIMY",4)) {
				Tulkitut[i] = 1;
				CBLaite1->ItemIndex = 1;
			   }
			else if (!wmemcmpU(fldn, L"ARES",4)) {
			   }
			else if (!wmemcmpU(fldn, L"ASC",3)) {
				Tulkitut[i] = 1;
				CBLaite1->ItemIndex = 2;
			   }
			else if (!wmemcmpU(fldn, L"TDC",3)) {
			   }
			else if (!wmemcmpU(fldn, L"ALGE4",5) || !wmemcmpU(fldn, L"S4",2)) {
				Tulkitut[i] = 1;
				CBLaite1->ItemIndex = 4;
			   }
			else if (!wmemcmpU(fldn, L"ALGE=",5) || !wmemcmpU(fldn, L"S3",2)) {
				Tulkitut[i] = 1;
				CBLaite1->ItemIndex = 5;
			   }
			else if (!wmemcmpU(fldn, L"RADIOEMIT",8)) {
			   }
			else if (!wmemcmpU(fldn, L"FEIG",4)) {
			   }
			else if (!wmemcmpU(fldn, L"SIRIT",5)) {
			   }
			else if (!wmemcmpU(fldn, L"IMPINJ",6)) {
			   }
			else if (!wmemcmpU(fldn, L"ALGET",5)) {
			   }
			else if (!wmemcmpU(fldn, L"SW2000",6)) {
			   }
			if (Tulkitut[i]) {
				if ((p = wcstok(fldn, L"=")) != NULL) {
					p = fldn+wcslen(p);
					if (!wmemcmpU(p+1, L"TCP", 3)) {
						wchar_t iposoite[100];
						int destport;
						tulktcpipparam(p+4, iposoite, &destport);
						CBLaiteYht1->ItemIndex = (iposoite[0] == 0 ? 1 : 2);
						EdLaiteIP1->Text = iposoite;
						EdLaitePort1->Text = destport;
						}
					else {
						EdLaitePort1->Text = _wtoi(p+1);
						}
					}
				}
			continue;
			}
		if (tyhjenna) {
			CBLaiteBaud1->ItemIndex = 0;
			}
		else if( !wmemcmpU(fldn, L"KELLOBAUD=",10)) {
			Tulkitut[i] = 1;
			for (int j = 1; j < CBLaiteBaud1->Items->Count-1; j++)
				if (wcscmp(fldn+10, CBLaiteBaud1->Items->Strings[j].c_str()) == 0)
					CBLaiteBaud1->ItemIndex = j;
			continue;
			}
		if (tyhjenna) {
			RGPiste->ItemIndex = 0;
			}
		else if( !wmemcmpU(fldn, L"PISTE=",6)) {
			Tulkitut[i] = 1;
			switch (towupper(fldn[6])) {
				case L'A':
					RGPiste->ItemIndex = 3;
					break;
				case L'M':
				case L'L':
					RGPiste->ItemIndex = 1;
					EdYhtPiste->Text = towupper(fldn[6]);
					break;
				default :
					RGPiste->ItemIndex = 1;
					EdYhtPiste->Text = _wtoi(fldn+6);
				}
			continue;
			}
		 if (tyhjenna) {
			}
		else if( !wmemcmpU(fldn, L"JONOT=",6)) {
			Tulkitut[i] = 1;
			for (p = fldn + 6, y = 1; *p && y < SGAjat->RowCount; p++, y++) {
			   if (*p > L'0' && *p <= L'0'+MAXJONO) {
				  SGAjat->Cells[2][y] = *p - L'1';
				  }
			   }
			continue;
			}
		 if (tyhjenna) {
			}
		else if( !wmemcmpU(fldn, L"LÄHDEPISTEET",8)) {
			Tulkitut[i] = 1;
			RGPiste->ItemIndex = 4;
			continue;
			}
		 if (tyhjenna) {
			EdEmitJono->Text = 1;
			}
		else if (!wmemcmpU(fldn, L"EMITJONO=", 9)) {
			Tulkitut[i] = 1;
			 EdEmitJono->Text = (int)(fldn[9]-L'0');
			 continue;
			}
		 if (tyhjenna) {
			EdMaxjono->Text = L"";
			}
		else if (!wmemcmpU(fldn, L"MAXJONO=", 8)) {
			Tulkitut[i] = 1;
			 if ((p = wcsstr(fldn, L"=")) != NULL)
				EdMaxjono->Text = _wtoi(p+1);
			 continue;
			}
		if (tyhjenna) {
			for (int r = 1; r < SGAjat->RowCount; r++) {
				for (int c = 1; c < SGAjat->ColCount; c++)
					SGAjat->Cells[c][r] = L"";
				}
			}
		else if( !wmemcmpU(fldn, L"PISTEET=",8)) {
			Tulkitut[i] = 1;
			wchar_t *p1, *p2 = NULL;
			p = fldn + 8;
			nc = 0;
			for (p1 = p; *p1 && wcswcind(*p1, L"/,;") < 0; p1++) ;
			if (p[2] == L'/' || (*p1 && *p1 != L'/')) {
				p1 = 0;
				p2 = wcsstr(p, L"/");
				if (p2)
					*p2 = 0;
				p1 = wcstok(p, L",;");
				while (p1 && p1[0]) {
					if (p1[1] == 0) {
					   if ((ny = wcswcind(*p1, ch_piste)) >= 0) {
						  SGAjat->Cells[1][y] = *p1;
						  }
					   }
					else
						SGAjat->Cells[1][y] = _wtoi(p1);
					y++;
					if (y >= SGAjat->RowCount)
						break;
					p1 = wcstok(NULL, L",;");
					}
				if (p2) {
					*p2 = L'/';
					p = p2;
					}
				}
			else {
				for (y = 1; y < SGAjat->RowCount; y++, p++) {
				   if ((ny = wcswcind(*p, ch_piste)) >= 0) {
					  SGAjat->Cells[1][y] = *p;
					  }
				   else
					  break;
				   }
				}
			if (*p == L'/' && !wmemcmpU(p+1, L"VAIN", 4) && y > 0) {
				RGPiste->ItemIndex = 1;
			   }
			continue;
			}
		 if (tyhjenna) {
			CBVainVA->Checked = false;
			}
		else if( !wmemcmpU(fldn, L"VAINVÄLIAJAT",12)) {
			Tulkitut[i] = 1;
			CBVainVA->Checked = true;
			continue;
			}
		 if (tyhjenna) {
			RGLainat->ItemIndex = 0;
			EdLainakortit->Text = L"";
			}
		else if( !wmemcmpU(fldn, L"LAINALUETTELO",9)) {
			Tulkitut[i] = 1;
			RGLainat->ItemIndex = 2;
			p = wcsstr(fldn, L"=");
			if (p != NULL) {
				wchar_t *p2;

				p2 = wcsstr(fldn, L"/");
				if (p2 == NULL)
					p2 = p;
				if (!wcscmp(p2+1, L"V") || !wcscmp(p2+1, L"VAIN")) {
					RGLainat->ItemIndex = 3;
					*p2 = 0;
					}
				if (p[0] != 0 && p[1] != 0) {
					EdLainakortit->Text = p+1;
					}
				}
			continue;
			}
		 if (tyhjenna) {
			RGEmittoiminto->ItemIndex = 0;
			}
		else if( !wmemcmpU(fldn, L"EMIT",5)) {
			Tulkitut[i] = 1;
			RGEmittoiminto->ItemIndex = 1;
			continue;
			}
		 if (tyhjenna) {
			}
		else if( !wmemcmpU(fldn, L"SISÄÄNLUENTA",8)) {
			Tulkitut[i] = 1;
			RGEmittoiminto->ItemIndex =
				(wcsstr(fldn, L"=M") != NULL) ? 3 : 2;
			continue;
			}
		 if (tyhjenna) {
			}
		else if( !wmemcmpU(fldn, L"LAINAT",5)) {
			Tulkitut[i] = 1;
			if (RGLainat->ItemIndex == 0)
				RGLainat->ItemIndex = 1;
			continue;
			}
		 if (tyhjenna) {
			CBEmitAjat->Checked = false;
			EdEmitAjat->Text = 1;
			}
		else if( !wmemcmpU(fldn, L"EMITAJAT",8)) {
			Tulkitut[i] = 1;
			CBEmitAjat->Checked = true;
			if (fldn[8] == L'=')
				EdEmitAjat->Text = _wtoi(fldn+9);
			continue;
			}
		 if (tyhjenna) {
			EdKartta->Text = L"";
			}
		else if( !wmemcmpU(fldn, L"KARTTA=",7)) {
			Tulkitut[i] = 1;
			EdKartta->Text = fldn+7;
			continue;
			}

		if (tyhjenna) {
			for (int r = 1; r < SGLeimat->RowCount; r++) {
				for (int c = 0; c < SGLeimat->ColCount; c++)
					SGLeimat->Cells[c][r] = L"";
				}
			}
		else if ((!wmemcmpU(fldn, L"LUKIJA",6) && (pos = 6) != 0)
			|| (!wmemcmpU(fldn, L"EMITAG",6) && (pos = 6) != 0)
			|| (!wmemcmpU(fldn, L"MTR",3) && (pos = 3) != 0)
			) {
			int r, orgpos = pos;
			Tulkitut[i] = 1;
			for (r = 1; r < SGLeimat->RowCount-1; r++)
				if (SGLeimat->Cells[0][r].Length() == 0 || SGLeimat->Cells[0][r].ToInt() == 0)
					break;
			SGLeimat->Cells[1][r] = UnicodeString(fldn).SubString(1, pos);
			ny = yhteys_no(fldn, &pos) - 1;
			if (pos == orgpos || ny > NREGNLY-1 || ny < 0)
				ny = NREGNLY-1;
			SGLeimat->Cells[0][r] = ny+1;
			if (!wmemcmpU(fldn+pos+1, L"UDP", 3)) {
				if (towupper(fldn[pos+4]) == L'S') {
					SGLeimat->Cells[2][r] = L"UDPS";
					pos++;
					}
				else
					SGLeimat->Cells[2][r] = L"UDP";
				if (wcswcind(fldn[pos+4], L":,/=") >= 0) {
					p = wcstok(fldn+pos+5, L":,/=");
					if (p && (j = _wtoi(p)) > 0) {
						SGLeimat->Cells[3][r] = j;
						}
					}
				}
			else if (!wmemcmpU(fldn+pos+1, L"TCP", 3)) {
				wchar_t iposoite[100];
				if (towupper(fldn[pos+4]) == L'S') {
					pos++;
					SGLeimat->Cells[2][r] = L"TCPS";
					}
				else
					SGLeimat->Cells[2][r] = L"TCP";
				tulktcpipparam(fldn+pos+4, iposoite, &j);
				if (iposoite[0]) {
					SGLeimat->Cells[4][r] = iposoite;
					SGLeimat->Cells[5][r] = j;
					}
				else
					SGLeimat->Cells[3][r] = j;
				}
			else if ((p = wcstok(fldn, L"=")) != NULL) {
			   if ((p = wcstok(NULL,L":,-/")) != NULL) {
					SGLeimat->Cells[2][r] = L"COM";
					SGLeimat->Cells[3][r] = _wtoi(p);
					if ((p = wcstok(NULL,L":,-/")) != NULL) {
						if (towupper(*p) == L'U')
							SGLeimat->Cells[2][r] = L"COM/U";
						}
					}
			   }
			continue;
			}
		if (tyhjenna) {
			CBEmitAnal->Checked = false;
			}
		else if (!wmemcmpU(fldn, L"EMITANAL",7)) {
			Tulkitut[i] = 1;
			CBEmitAnal->Checked = true;
			continue;
			}
		 if (tyhjenna) {
			CBJalkiSyotto->Checked = false;
			}
		else if( !wmemcmpU(fldn, L"JÄLKISYÖTTÖ",7)) {
			Tulkitut[i] = 1;
			CBJalkiSyotto->Checked = true;
			continue;
			}
		 if (tyhjenna) {
			RGComAutorap->ItemIndex = 0;
			}
		else if( !wmemcmpU(fldn, L"COMAUTORAP",10)) {
			Tulkitut[i] = 1;
			RGComAutorap->ItemIndex = 1;
			if (fldn[10] == L'=') {
			   switch (towupper(fldn[11])) {
				  case L'1':
				  case L'H':
				  case L'S':
					RGComAutorap->ItemIndex = 2;
					 break;
				  }
				}
			continue;
			}
		 if (tyhjenna) {
			RGAutorap->ItemIndex = 0;
			}
		else if( !wmemcmpU(fldn, L"AUTORAP",7)) {
			Tulkitut[i] = 1;
			RGAutorap->ItemIndex = 1;
			if (fldn[7] == L'=') {
				switch (towupper(fldn[8])) {
					case L'1':
					case L'H':
					case L'S':
						RGAutorap->ItemIndex = 2;
						break;
					}
			   }
			continue;
			}
		if (tyhjenna) {
			CBAikaLukija->ItemIndex = 0;
			EdAikaLukija->Text = L"";
			}
		else if( !wmemcmpU(fldn, L"AIKALUKIJA",6)) {
			int r = 0;
			Tulkitut[i] = 1;
			ny = 0;
			if ((ny = _wtoi(fldn+10)) > NREGNLY || ny < 0)
				ny = 0;
			if (ny > 0) {
				for (r = 1; r < SGLeimat->RowCount-1; r++)
					if (SGLeimat->Cells[0][r].Length() > 0 && SGLeimat->Cells[0][r].ToInt() == ny)
						break;
				}
			p = wcsstr(fldn, L"=");
			if (p)
				pos = p - fldn;
			else
				pos = 10;
			if (!wmemcmpU(fldn+pos,L"=V",2) || !wmemcmpU(fldn+pos,L"=K",2) || !wmemcmpU(fldn+pos,L"=L",2)) {
				int ii;
			   if (towupper(fldn[pos+1]) == L'L') {
					 pos = wcslen(fldn) - 1;
					 if (fldn[pos] >= L'0' && fldn[pos] <= L'9') {
						while(fldn[pos-1] >= L'0' && fldn[pos-1] <= L'9')
							pos--;
						j = _wtoi(fldn+pos);
						}
					 else
						j = ny;
					if (ny == 0) {
						CBAikaLukija->ItemIndex = 4;
						EdAikaLukija->Text = j;
						}
					else {
						SGLeimat->Cells[6][r] = L"LÄHDE"+j;
						}
					 continue;
				  }
			   pos = wcslen(fldn) - 1;
			   if (fldn[pos] >= L'0' && fldn[pos] <= L'9') {
				  while(fldn[pos-1] >= L'0' && fldn[pos-1] <= L'9')
					  pos--;
				  j = _wtoi(fldn+pos);
				  ii = 2;
				  }
			   else {
				  if (towupper(fldn[pos]) == L'A') {
					 j = -2;
					 ii = 3;
					 }
				  else if (towupper(fldn[pos]) == L'L') {
					 j = -1;
					 ii = 2;
					 }
				  else {
					 j = -3;
					 ii = 1;
					 }
				  }
				if (ny == 0) {
					CBAikaLukija->ItemIndex = ii;
					EdAikaLukija->Text = (j > 0 ? UnicodeString(j) : (j > -2 ? UnicodeString(L"LM"[j+1]) : UnicodeString(L"")));
					}
				else {
					SGLeimat->Cells[6][r] = L"VAIN"+UnicodeString(fldn[pos]);
					}
				 continue;
			   }
			else {
				if (ny == 0) {
					CBAikaLukija->ItemIndex = 1;
					}
				else {
					SGLeimat->Cells[6][r] = L"VAIN";
					}
			}
		continue;
		}
		 if (tyhjenna) {
			RGPiste->ItemIndex = 0;
			}
		else if( !wmemcmpU(fldn, L"SJBOX",5) || !wmemcmpU(fldn, L"LÄHDEPISTEET",8)) {
			Tulkitut[i] = 1;
			RGPiste->ItemIndex = 4;
			continue;
			}
		else if( !wmemcmpU(fldn, L"VAADIAIKA",7)) {
			Tulkitut[i] = 1;
			CBVaadiAika->Checked = true;
			continue;
			}
		 if (tyhjenna) {
			CBVaadiAika->Checked = false;
			}
		else if( !wmemcmpU(fldn, L"KAKSITUNN",7)) {
			Tulkitut[i] = 1;
			CBKaksiTunn->Checked = true;
			continue;
			}
		 if (tyhjenna) {
			CBLuentaLoki->Checked = false;
			}
		else if( !wmemcmpU(fldn, L"LUENTALOKI",7)) {
			Tulkitut[i] = 1;
			CBLuentaLoki->Checked = true;
			continue;
			}
		if( !wmemcmpU(fldn, L"LÄHEMIT",7) && (pos = 7) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
			int r = 0;
			Tulkitut[i] = 1;
			for (r = 1; r < SG1->RowCount-1; r++)
				if (SG1->Cells[0][r].Length() > 0 && SG1->Cells[0][r].ToInt() == ny)
					break;
			if (fldn[pos] == L'=') {
			   if (towupper(fldn[pos+1]) == L'I')
					SG1->Cells[6][r] = SG1->Cells[6][r]+L"EmI";
			   else if (towupper(fldn[pos+1]) == L'O')
					SG1->Cells[6][r] = SG1->Cells[6][r]+L"EmO";
			   else if (towupper(fldn[pos+1]) == L'V')
					SG1->Cells[6][r] = SG1->Cells[6][r]+L"EmV";
			   }
			else
				SG1->Cells[6][r] = SG1->Cells[6][r]+L"Emt";
			continue;
			}
		 if (tyhjenna) {
			}
		else if( !wmemcmpU(fldn, L"LÄHEMVA",7) && (pos = 7) != 0 &&
			(ny = yhteys_no(fldn, &pos)) != 0) {
			int r = 0;
			Tulkitut[i] = 1;
			for (r = 1; r < SG1->RowCount-1; r++)
				if (SG1->Cells[0][r].Length() > 0 && SG1->Cells[0][r].ToInt() == ny)
					break;
			SG1->Cells[6][r] = SG1->Cells[6][r]+L"EVa";
			continue;
			}
		if (tyhjenna)
			break;
	  }
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::CBIkkunatClick(TObject *Sender)
{
	EdIkkunat->Enabled = CBIkkunat->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::RGJonotClick(TObject *Sender)
{
	EdMaxjono->Visible = RGJonot->ItemIndex == 2;
	LbMaxjono->Visible = RGJonot->ItemIndex == 2;
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::CBLaite1Change(TObject *Sender)
{
	CBLaiteYht1->Enabled = CBLaite1->ItemIndex != 0;
	EdLaitePort1->Enabled = CBLaite1->ItemIndex != 0;
	EdLaiteIP1->Enabled = (CBLaite1->ItemIndex != 0 && CBLaiteYht1->ItemIndex != 0);
	CBLaiteBaud1->Enabled = (CBLaite1->ItemIndex != 0 && CBLaiteYht1->ItemIndex == 0);
	EdAikaEro1->Enabled = CBLaite1->ItemIndex != 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::CBLaiteYht1Click(TObject *Sender)
{
	EdLaiteIP1->Visible = (CBLaite1->ItemIndex != 0 && CBLaiteYht1->ItemIndex != 0);
	LbLaiteIP1->Visible = EdLaiteIP1->Visible;
	CBLaiteBaud1->Visible = (CBLaite1->ItemIndex != 0 && CBLaiteYht1->ItemIndex == 0);
	LbLaiteBaud1->Visible = CBLaiteBaud1->Visible;
}

//---------------------------------------------------------------------------

void __fastcall TFormCfg::CBYhteysAjatClick(TObject *Sender)
{
	EdYhtAikaEro->Enabled = CBYhteysAjat->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::CBComAjatClick(TObject *Sender)
{
	EdComAjat->Enabled = CBComAjat->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::CBAikaLukijaClick(TObject *Sender)
{
	EdAikaLukija->Enabled = CBAikaLukija->ItemIndex == 2 || CBAikaLukija->ItemIndex == 4;
	EdEmitJono->Enabled = CBAikaLukija->ItemIndex > 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::RGLainatClick(TObject *Sender)
{
	EdLainakortit->Visible = RGLainat->ItemIndex > 1;
	LbLainakortit->Visible = EdLainakortit->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::RGKirjoitinClick(TObject *Sender)
{
	EdKirjoitin->Enabled = RGKirjoitin->ItemIndex > 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::RGLokiClick(TObject *Sender)
{
	EdLoki->Enabled = RGLoki->ItemIndex > 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::CBAutoLaajuusClick(TObject *Sender)
{
	EdAutoParhaat->Enabled = CBAutoLaajuus->ItemIndex == 3;
	EdAutoKynnys->Enabled = CBAutoLaajuus->ItemIndex > 0;
	EdAutoAskel->Enabled = CBAutoLaajuus->ItemIndex > 0;
	EdAutoMaalit->Enabled = CBAutoLaajuus->ItemIndex > 0;
	CBAutoMonivaihe->Enabled = CBAutoLaajuus->ItemIndex > 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::CBAutoJakeluClick(TObject *Sender)
{
	EdAutoJakelu->Enabled = CBAutoJakelu->ItemIndex > 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::RGPisteClick(TObject *Sender)
{
	EdYhtPiste->Visible = RGPiste->ItemIndex == 1;
	LbYhtPiste->Visible = RGPiste->ItemIndex == 1;
	SGAjat->Visible = RGPiste->ItemIndex == 2;
	LbAjat->Visible = RGPiste->ItemIndex == 2;
	GBVuoro->Visible = RGPiste->ItemIndex == 3;
}
//---------------------------------------------------------------------------



void __fastcall TFormCfg::SG1MouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift,
		  int X, int Y)
{
	if (Button == mbRight && SG1->Row > 0) {
		switch (SG1->Col) {
			case 1:
				PopupMenuYht->Popup(X+SG1->ClientOrigin.X, Y+SG1->ClientOrigin.Y);
				break;
			case 2:
				PopupMenuIO->Popup(X+SG1->ClientOrigin.X, Y+SG1->ClientOrigin.Y);
				break;
			case 6:
				PopupMenuSan->Popup(X+SG1->ClientOrigin.X, Y+SG1->ClientOrigin.Y);
				break;
			case 7:
				PopupMenuVal->Popup(X+SG1->ClientOrigin.X, Y+SG1->ClientOrigin.Y);
				break;
//			default:
//				PopupMenuMalli->Popup(X+SG1->ClientOrigin.X, Y+SG1->ClientOrigin.Y);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::UDP1Click(TObject *Sender)
{
	if (Sender == UDP1)
		SG1->Cells[SG1->Col][SG1->Row] = L"UDP";
	else if (Sender == TCP1)
		SG1->Cells[SG1->Col][SG1->Row] = L"TCP";
	else if (Sender == TCPS1)
		SG1->Cells[SG1->Col][SG1->Row] = L"TCPS";
	else if (Sender == BRO1)
		SG1->Cells[SG1->Col][SG1->Row] = L"BRO";
	else if (Sender == COM1)
		SG1->Cells[SG1->Col][SG1->Row] = L"COM";
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::SanValClick(TObject *Sender)
{
	if (Sender == Perus1)
		SG1->Cells[SG1->Col][SG1->Row] = L"";
	else if (Sender == Ajat1)
		SG1->Cells[SG1->Col][SG1->Row] = SG1->Cells[SG1->Col][SG1->Row]+L"Aik";
	else if (Sender == Leimat1)
		SG1->Cells[SG1->Col][SG1->Row] = SG1->Cells[SG1->Col][SG1->Row]+L"Emt";
	else if (Sender == Vainajat1)
		SG1->Cells[SG1->Col][SG1->Row] = SG1->Cells[SG1->Col][SG1->Row]+L"AiV";
	else if (Sender == AikaPakota)
		SG1->Cells[SG1->Col][SG1->Row] = SG1->Cells[SG1->Col][SG1->Row]+L"AiP";
	else if (Sender == Vainleimat1)
		SG1->Cells[SG1->Col][SG1->Row] = SG1->Cells[SG1->Col][SG1->Row]+L"EmV";
	else if (Sender == LeimatIn1)
		SG1->Cells[SG1->Col][SG1->Row] = SG1->Cells[SG1->Col][SG1->Row]+L"EmI";
	else if (Sender == LeimatOut1)
		SG1->Cells[SG1->Col][SG1->Row] = SG1->Cells[SG1->Col][SG1->Row]+L"EmO";
	else if (Sender == Rastiva1)
		SG1->Cells[SG1->Col][SG1->Row] = SG1->Cells[SG1->Col][SG1->Row]+L"EVa";
	else if (Sender == ECode1)
		SG1->Cells[SG1->Col][SG1->Row] = SG1->Cells[SG1->Col][SG1->Row]+L"ECd";
	else if (Sender == Tied1)
		SG1->Cells[SG1->Col][SG1->Row] = SG1->Cells[SG1->Col][SG1->Row]+L"Tie";
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::Valita1Click(TObject *Sender)
{
	if (Sender == Valita1)
		SG1->Cells[SG1->Col][SG1->Row] = L"";
	else if (Sender == ValitaEi1)
		SG1->Cells[SG1->Col][SG1->Row] = L"Ei";
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::InOut1Click(TObject *Sender)
{
	if (Sender == InOut1)
		SG1->Cells[SG1->Col][SG1->Row] = L"";
	else if (Sender == In1)
		SG1->Cells[SG1->Col][SG1->Row] = L"In";
	else if (Sender == Out1)
		SG1->Cells[SG1->Col][SG1->Row] = L"Out";
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::MalliClick(TObject *Sender)
{
	if (Sender == Valita1)
		SG1->Cells[SG1->Col][SG1->Row] = L"";
	else if (Sender == ValitaEi1)
		SG1->Cells[SG1->Col][SG1->Row] = L"Ei";
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::SGLeimatMouseDown(TObject *Sender, TMouseButton Button,
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

void __fastcall TFormCfg::LaiteClick(TObject *Sender)
{
	if (Sender == Lukija1)
		SGLeimat->Cells[SGLeimat->Col][SGLeimat->Row] = L"LUKIJA";
	else if (Sender == emiTag1)
		SGLeimat->Cells[SGLeimat->Col][SGLeimat->Row] = L"EMITAG";
	else if (Sender == LaiteMTR1)
		SGLeimat->Cells[SGLeimat->Col][SGLeimat->Row] = L"MTR";
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::LaiteYhtClick(TObject *Sender)
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

void __fastcall TFormCfg::LaiteAjatClick(TObject *Sender)
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
	else if (Sender == Lahdekoodi1)
		SGLeimat->Cells[SGLeimat->Col][SGLeimat->Row] = L"LÄHDExx";
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::Help1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Konfigurointi");
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::RGNaytaValClick(TObject *Sender)
{
	KaVal = RGNaytaVal->ItemIndex == 2;
	SuVal = RGNaytaVal->ItemIndex == 0;
	HiVal = RGNaytaVal->ItemIndex == 1;
	ValSetup();
}
//---------------------------------------------------------------------------
void TFormCfg::ValSetup(void)
{
	Panel2->Visible = KaVal;
	Panel21->Visible = KaVal;
	Panel3->Visible = KaVal;
	Panel4->Visible = KaVal;
	GBYhtAjat->Visible = KaVal;
	GBComAjat->Visible = KaVal;
	RGEmittoiminto->Visible = KaVal || SuVal;
	Panel5->Visible = KaVal &&
		(RGEmittoiminto->ItemIndex == 1 || RGEmittoiminto->ItemIndex == 3);
	Panel6->Visible = (KaVal || HiVal) &&
		(RGEmittoiminto->ItemIndex == 1 || RGEmittoiminto->ItemIndex == 3);
	Panel7->Visible = KaVal;
	Panel10->Visible = (KaVal || SuVal) &&
		(RGEmittoiminto->ItemIndex == 1 || RGEmittoiminto->ItemIndex == 3);
	RGAutorap->Visible = (KaVal || SuVal) &&
		(RGEmittoiminto->ItemIndex == 1 || RGEmittoiminto->ItemIndex == 3);
	RGComAutorap->Visible = RGAutorap->Visible;
	RGLainat->Visible = RGAutorap->Visible;
	LbLainakortit->Visible = RGLainat->Visible && RGLainat->ItemIndex > 1;
	EdLainakortit->Visible = LbLainakortit->Visible;
	CBEmitAnal->Visible = KaVal &&
		(RGEmittoiminto->ItemIndex == 1 || RGEmittoiminto->ItemIndex == 3);
	Panel9->Visible = KaVal;
//	GBAutoTul->Visible = KaVal;
	GBAutoTied->Visible = KaVal;
	RGPisteClick(this);
	RGJonotClick(this);
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::RGEmittoimintoClick(TObject *Sender)
{
	CBLuentaLoki->Visible = RGEmittoiminto->ItemIndex > 1;
	ValSetup();
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Ajanoton konfigurointi");
}
//---------------------------------------------------------------------------

void __fastcall TFormCfg::BitBtn3Click(TObject *Sender)
{
	Application->HelpKeyword(L"emiTag");
}
//---------------------------------------------------------------------------

