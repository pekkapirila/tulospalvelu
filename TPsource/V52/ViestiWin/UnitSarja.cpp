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

#include "UnitSarja.h"
#include "UnitSarjatiedot.h"
#include "VDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSarja *FormSarja;

//---------------------------------------------------------------------------
__fastcall TFormSarja::TFormSarja(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormSarja::lataaSarja(int Srj)
{
	if (Srj >= 0)
		Sarja1 = Sarjat[Srj];
	else
		Sarja1.nollaa();
	SarjaNo = Srj;
	naytaSarja(true);
}
//---------------------------------------------------------------------------
void vaihdakilpsrj(int ala, int yla, int muutos, bool indeksoi)
   {
   int d;
   kilptietue kilp;

   if (datf2.numrec <= 1) return;
   for (d = 1; d < datf2.numrec; d++) {
	  kilp.getrec(d);
	  if (kilp.kilpstatus == 0) {
		  if (kilp.sarja >= ala && kilp.sarja <= yla) {
			  kilp.sarja = kilp.sarja + muutos;
			  kilp.putrec(d, true);
			  }
		  }
	  }
//   if (indeksoi)
//	  uusijarj();
   }
//---------------------------------------------------------------------------
void __fastcall TFormSarja::naytaSarja(bool uusi)
{
	wchar_t st[20];

	EdtSarja->Text = UnicodeString(SarjaNo+1);
	if (uusi) {
		SNimi->Text = UnicodeString(Sarja1.sarjanimi);
		PNimi->Text = UnicodeString(Sarja1.psarjanimi);
		swprintf(st, L"%d,%02d", Sarja1.maksu/100, Sarja1.maksu%100);
		Mks->Text = UnicodeString(st);
		swprintf(st, L"%d,%02d", Sarja1.maksual/100, Sarja1.maksual%100);
		MksAl->Text = UnicodeString(st);
		}
	EOsLkm->Text = UnicodeString(Sarja1.ntosuus);
	ERinnMax->Enabled = kilpparam.maxnosuus > 1;
	ERinnMax->Text = Sarja1.maxnosuus;
	Alku->Text = UnicodeString(Sarja1.sarjaalku);
	EdtKnoSiirto->Text = UnicodeString(Sarja1.knosiirto);
	CBSak->ItemIndex = Sarja1.taslaji/10;
	CBAmpSak->ItemIndex = Sarja1.taslaji%10;
	L1->Text = UnicodeString(Sarja1.lno);
	M1->Text = UnicodeString(Sarja1.mno);
	La1->Text = UnicodeString(AIKATOWSTRS(st, Sarja1.lahto, t0)).SubString(1,8);
	naytaOsuudet();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::tallSarja(void)
{
	wcstoansi(Sarja1.sarjanimi, upcasewstr(SNimi->Text.SubString(1, LSARJA).c_str()), LSARJA);
	wcstoansi(Sarja1.psarjanimi, PNimi->Text.SubString(1, LPSARJA).c_str(), LPSARJA);
	Sarja1.ntosuus = _wtoi(EOsLkm->Text.c_str());
	Sarja1.maxnosuus = _wtoi(ERinnMax->Text.c_str());
	if (Sarja1.maxnosuus == 1 || Sarja1.osuusluku < Sarja1.ntosuus)
		Sarja1.osuusluku = Sarja1.ntosuus;
	Sarja1.sarjaalku = _wtoi(Alku->Text.c_str());
	Sarja1.knosiirto = _wtoi(EdtKnoSiirto->Text.c_str());
	Sarja1.maksu = wcstomaksu(Mks->Text.c_str());
	Sarja1.maksual = wcstomaksu(MksAl->Text.c_str());
	Sarja1.taslaji = CBAmpSak->ItemIndex + 10 * CBSak->ItemIndex;
	Sarja1.lno = _wtoi(L1->Text.c_str());
	Sarja1.mno = _wtoi(M1->Text.c_str());
	Sarja1.lahto = wstrtoaika_vap(La1->Text.c_str(), t0);
	tallOsuudet();
}
//---------------------------------------------------------------------------
static wchar_t *JulkSt(int taso)
{
static wchar_t *julkstr[5] =
	{L"/Piil", L"/Taso1", L"/Taso2", L"/Taso3", L"/Julk"};

	if (taso > 4)
		taso = 4;
	return(julkstr[taso]);
}
//---------------------------------------------------------------------------
static int valuku = 0, lisarivit0 = 0, lisarivit1 = 0, lisarivit2 = 0, lisarivit3 = 0;

void __fastcall TFormSarja::naytaOsuudet(void)
{
//	int valuku = 0, lisarivit0 = 0, lisarivit1 = 0, lisarivit2 = 0, lisarivit3 = 0;
	wchar_t str[100];

	if (Sarja1.maxnosuus > 1)
		lisarivit0 = 1;
	if (Sarja1.taslaji/10 > 0)
		lisarivit1 = 1;
	if (Sarja1.taslaji % 10 > 0)
		lisarivit2 = Sarja1.taslaji % 10;
	if (wcswcind(kilpparam.alalaji, L"JPR") >= 0)
		lisarivit3 = 1;
	for (int os = 0; os < Sarja1.ntosuus; os++)
		if (valuku < Sarja1.valuku[os])
			valuku = Sarja1.valuku[os];

	SG1->ColCount = Sarja1.ntosuus + 1;
	SG1->RowCount = valuku+5+lisarivit0+lisarivit1+lisarivit2+lisarivit3;
	if (lisarivit0)
		SG1->Cells[0][1] = L"Rinn.lkm.";
	SG1->Cells[0][1+lisarivit0] = L"Matka (km)";
	SG1->Cells[0][2+lisarivit0] = L"Väliaikoja";
	if (lisarivit1)
		SG1->Cells[0][3+lisarivit0] = L"Sak/Tas-yks.";
	if (lisarivit2)
		SG1->Cells[0][3+lisarivit0+lisarivit1] = L"Sakkopaikat";
	if (lisarivit2 == 2)
		SG1->Cells[0][4+lisarivit0+lisarivit1] = L"Ammunnan sakko";
	SG1->Cells[0][3+lisarivit0+lisarivit1+lisarivit2] = L"Yht.lähtö";
	SG1->Cells[0][4+lisarivit0+lisarivit1+lisarivit2] = L"Yht.l.kynnys";
	if (lisarivit3)
		SG1->Cells[0][5+lisarivit0+lisarivit1+lisarivit2] = L"Lähtöväli";
	for (int va = 0; va < valuku; va++) {
		SG1->Cells[0][va+5+lisarivit0+lisarivit1+lisarivit2+lisarivit3] = UnicodeString(va+1)+L". va-matka";
		}
	for (int os = 0; os < Sarja1.ntosuus; os++) {
		SG1->ColWidths[os+1] = 65 * Screen->PixelsPerInch / 96;
		SG1->Cells[os+1][0] = UnicodeString(os+1) + L". osuus";
		if (lisarivit0)
			SG1->Cells[os+1][1] = Sarja1.nosuus[os];
		SG1->Cells[os+1][1+lisarivit0] = Sarja1.matka[os];
		SG1->Cells[os+1][2+lisarivit0] = Sarja1.valuku[os];
		if (lisarivit1)
			SG1->Cells[os+1][3+lisarivit0] = Sarja1.tsak[os];
		if (lisarivit2)
			SG1->Cells[os+1][3+lisarivit0+lisarivit1] = Sarja1.paikat[os];
		if (lisarivit2 == 2)
			SG1->Cells[os+1][4+lisarivit0+lisarivit1] = Sarja1.amptsak[os];
		SG1->Cells[os+1][3+lisarivit0+lisarivit1+lisarivit2] = aikatowstr_cols_n(str, Sarja1.ylahto[os], t0, L':', 8);
		SG1->Cells[os+1][4+lisarivit0+lisarivit1+lisarivit2] = aikatowstr_cols_n(str, Sarja1.ylkynnys[os], t0, L':', 8);
		if (lisarivit3)
			SG1->Cells[os+1][5+lisarivit0+lisarivit1+lisarivit2] = Sarja1.lahtovali[os];
		for (int va = 0; va < valuku; va++) {
			if (va < Sarja1.valuku[os])
				SG1->Cells[os+1][va+5+lisarivit0+lisarivit1+lisarivit2+lisarivit3] =
					Sarja1.va_matka[os][va]+UnicodeString(JulkSt(Sarja1.va_piilota[os][va]));
			else
				SG1->Cells[os+1][va+5+lisarivit0+lisarivit1+lisarivit2+lisarivit3] = L"";
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::tallOsuudet(void)
{
	int osluku = 0;
	wchar_t st[30] = L"", *p;

	Sarja1.aosuus[0] = -1;
	Sarja1.ylahto[0] = TMAALI0;
	for (int os = 0; os < Sarja1.ntosuus; os++) {
		if (lisarivit0)
			Sarja1.nosuus[os] = _wtoi(SG1->Cells[os+1][1].c_str());
		Sarja1.aosuus[os+1] = Sarja1.aosuus[os] + Sarja1.nosuus[os];
		for (int yos = Sarja1.aosuus[os]+1; yos <= Sarja1.aosuus[os+1]; yos++)
			Sarja1.yosuus[yos] = os;
		osluku += Sarja1.nosuus[os];
		wcstoansi(Sarja1.matka[os], SG1->Cells[os+1][1+lisarivit0].c_str(), sizeof(Sarja1.matka[os])-1);
		Sarja1.valuku[os] = _wtoi(SG1->Cells[os+1][2+lisarivit0].c_str());
		if (lisarivit1)
			Sarja1.tsak[os] = _wtoi(SG1->Cells[os+1][3+lisarivit0].c_str());
		if (lisarivit2)
			Sarja1.paikat[os] = _wtoi(SG1->Cells[os+1][3+lisarivit0+lisarivit1].c_str());
		if (lisarivit2 == 2)
			Sarja1.amptsak[os] = _wtoi(SG1->Cells[os+1][4+lisarivit0+lisarivit1].c_str());
		Sarja1.ylahto[os] = wstrtoaika_vap(SG1->Cells[os+1][3+lisarivit0+lisarivit1+lisarivit2].c_str(), t0);
		if (os > 0) {
			Sarja1.ylkynnys[os] = wstrtoaika_vap(SG1->Cells[os+1][4+lisarivit0+lisarivit1+lisarivit2].c_str(), t0);
			if (Sarja1.ylahto[0] == TMAALI0)
				Sarja1.ylahto[0] = Sarja1.ylahto[os];
			}
		if (lisarivit3)
			Sarja1.lahtovali[os] = _wtoi(SG1->Cells[os+1][5+lisarivit0+lisarivit1+lisarivit2].c_str());
		for (int va = 0; va < Sarja1.valuku[os]; va++) {
			wcsncpy(st, SG1->Cells[os+1][va+5+lisarivit0+lisarivit1+lisarivit2+lisarivit3].c_str(), 29);
			Sarja1.va_piilota[os][va] = 9;
			p = wcstok(st, L"/");
			if (p) {
				if (p - st == 0) {
					wcstoansi(Sarja1.va_matka[os][va], p, sizeof(Sarja1.va_matka[os][va])-1);
					p = wcstok(NULL, L"/");
					}
				else
					memset(Sarja1.va_matka[os][va], 0, sizeof(Sarja1.va_matka[os][va]));
				if (p) {
					elimwbl(p);
					if (towupper(*p) == L'N')
						Sarja1.va_piilota[os][va] = 0;
					else if(p[wcslen(p)-1] >= L'0' && p[wcslen(p)-1] < L'4')
						Sarja1.va_piilota[os][va] = p[wcslen(p)-1] - L'0';
					}
				}
			}
		}
	if (osluku > kilpparam.n_os_akt) {
		Application->MessageBoxW((UnicodeString(L"¨Korjaa sarjan osuusluku, joka ylittää kilpailun osuusluvun ")+
			UnicodeString(kilpparam.n_os_akt)).c_str(), L"Virhe", MB_OK);
		osluku = kilpparam.n_os_akt;
		}
	Sarja1.osuusluku = osluku;
}

//---------------------------------------------------------------------------

void __fastcall TFormSarja::FormShow(TObject *Sender)
{
	naytaSarja(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::Button3Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::Button4Click(TObject *Sender)
{
	if (sarjaluku > 1 && SarjaNo >= 0 && SarjaNo < sarjaluku) {
		if (Application->MessageBoxW((UnicodeString(L"Vahvista sarjan '")+UnicodeString(Sarjat[SarjaNo].sarjanimi)+
			UnicodeString(L"' poisto")).c_str(), L"Vahvistus", MB_YESNO) == IDYES) {
			memmove(&Sarjat[SarjaNo], &Sarjat[SarjaNo+1], (sarjaluku-SarjaNo-1)*sizeof(Sarjat[0]));
			vaihdakilpsrj(SarjaNo, sarjaluku-1, -1, true);
			sarjaluku--;
			kirjEventXml(sarjaxmlfname, true, false, false);
			maaraaOsuusluku(true);
			if (SarjaNo >= sarjaluku)
				SarjaNo = sarjaluku-1;
			lataaSarja(SarjaNo);
			naytaSarja(true);
			FormSarjatiedot->NaytaSarjat();
			}
		}
	else {
		Application->MessageBoxW(L"Sarjan numero ei käytössä", L"Virhe", MB_OK);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::Button2Click(TObject *Sender)
{
	lataaSarja(-1);
	SNimi->Text = L"";
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::Button1Click(TObject *Sender)
{
	int srj;
	sarjatietue Sarja0;

	srj = EdtSarja->Text.ToInt() - 1;
	if (srj < 0) {
		srj = sarjaluku;
	   }
	if (srj > sarjaluku)
		srj = sarjaluku;
	tallSarja();
	naytaSarja(true);
	Sarja0 = Sarja1;
	EdtSarja->Text = UnicodeString(srj+1);
	if (SarjaNo == -1 || srj == sarjaluku) {
		if (srj < sarjaluku) {
			if (Application->MessageBoxW(L"Sarja lisätään annetun sarjanumeron paikalle. Vahvista",
				L"Sarjan lisäys", MB_YESNO) != IDYES)
				return;
			memmove(&Sarjat[srj+1], &Sarjat[srj], (sarjaluku-srj)*sizeof(Sarjat[0]));
			Sarjat[srj] = Sarja1;
			sarjaluku++;
			vaihdakilpsrj(srj, sarjaluku-2, 1, true);
			}
		else {
			if (Application->MessageBoxW(L"Sarja lisätään viimeiseksi. Vahvista",
				L"Sarjan lisäys", MB_YESNO) != IDYES)
				return;
			sarjaluku++;
			Sarjat[srj] = Sarja1;
			}
		}
	else if (srj != SarjaNo) {
		if (Application->MessageBoxW(L"Sarja siirretään annetun sarjanumeron paikalle. Vahvista",
			L"Sarjan siirto", MB_YESNO) != IDYES)
			return;
		if (srj < SarjaNo) {
			memmove(&Sarjat[srj+1], &Sarjat[srj], (SarjaNo-srj)*sizeof(Sarjat[0]));
		   Sarjat[srj] = Sarja1;
		   vaihdakilpsrj(SarjaNo, SarjaNo, sarjaluku-SarjaNo, false);
		   vaihdakilpsrj(srj, SarjaNo-1, 1, false);
		   vaihdakilpsrj(sarjaluku, sarjaluku, srj - sarjaluku, true);
		   }
		else {
			memmove(&Sarjat[SarjaNo], &Sarjat[SarjaNo+1], (srj-SarjaNo)*sizeof(Sarjat[0]));
		   Sarjat[srj] = Sarja1;
		   vaihdakilpsrj(srj, srj, sarjaluku-srj, false);
		   vaihdakilpsrj(srj+1, SarjaNo, -1, false);
		   vaihdakilpsrj(sarjaluku, sarjaluku, SarjaNo - sarjaluku, true);
		   }
		}
	else {
		if (Application->MessageBoxW(L"Mahdolliset muutokset tallennetaan. Vahvista",
			L"Vahvistus", MB_YESNO) != IDYES)
			return;
		Sarja0 = Sarjat[srj];
		Sarjat[srj] = Sarja1;
		}
	maaraaOsuusluku(true);
	kirjEventXml(sarjaxmlfname, true, false, false);
//	kirjsarjat();
	lataaSarja(srj);
//	naytaSarja();
	FormSarjatiedot->SetColumns();
//	FormSarjatiedot->NaytaSarjat();
	if (ToimintaTila == 2) {
		if (Sarja1.lahto != Sarja0.lahto)
			laheta_yl(-srj-1, 0);
		if (memcmp(Sarja1.ylahto, Sarja0.ylahto, sizeof(Sarja0.ylahto)) ||
			memcmp(Sarja1.ylkynnys, Sarja0.ylkynnys, sizeof(Sarja0.ylkynnys))) {
			laheta_yl(srj, 0);
			merk_yht_lahdot(srj, false);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::Button5Click(TObject *Sender)
{
	lataaSarja(SarjaNo);
//	naytaSarja();
	FormSarjatiedot->NaytaSarjat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Sarjat");
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::BtnEdSrjClick(TObject *Sender)
{
	if (SarjaNo > 0)
		lataaSarja(SarjaNo-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::BtnSeurSrjClick(TObject *Sender)
{
	if (SarjaNo < sarjaluku-1)
		lataaSarja(SarjaNo+1);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::MksExit(TObject *Sender)
{
	int m;
	wchar_t st[20];

	m = wcstomaksu(Mks->Text.c_str());
	swprintf(st, L"%d,%02d", m/100, m%100);
	Mks->Text = UnicodeString(st);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::MksAlExit(TObject *Sender)
{
	int m;
	wchar_t st[20];

	m = wcstomaksu(MksAl->Text.c_str());
	swprintf(st, L"%d,%02d", m/100, m%100);
	MksAl->Text = UnicodeString(st);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::La1Exit(TObject *Sender)
{
	wchar_t st[20];

	La1->Text = UnicodeString(
		AIKATOWSTRS(st, wstrtoaika_vap(La1->Text.c_str(), t0), t0)).SubString(1,8);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::SNimiExit(TObject *Sender)
{
	SNimi->Text = UnicodeString(upcasewstr(SNimi->Text.SubString(1, LSARJA).c_str()));
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::PNimiExit(TObject *Sender)
{
	PNimi->Text = PNimi->Text.SubString(1, LPSARJA);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::FormResize(TObject *Sender)
{
	SG1->Height = ClientHeight - SG1->Top - 6;
	SG1->Width = ClientWidth - SG1->Left -6;
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::SG1Exit(TObject *Sender)
{
	if (aktrow > 0 && aktrow > 0) {
		paivitaMuutos(aktcol, aktrow);
		}
	aktcol = 0;
	aktrow = 0;	
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::SG1SelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect)
          
{
	if (SG1->Options.Contains(goEditing) && aktrow > 0 && aktrow > 0) {
		paivitaMuutos(aktcol, aktrow);
	}
	aktcol = ACol;
	aktrow = ARow;	
}
//---------------------------------------------------------------------------
int __fastcall TFormSarja::paivitaMuutos(int col, int row)
{
	tallOsuudet();
	naytaSarja(false);
	return(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::CBSakClick(TObject *Sender)
{
	Sarja1.taslaji = CBAmpSak->ItemIndex + 10 * CBSak->ItemIndex;
	naytaSarja(false);
}
//---------------------------------------------------------------------------


void __fastcall TFormSarja::ERinnMaxExit(TObject *Sender)
{
	int nr;

	nr = _wtoi(ERinnMax->Text.c_str());
	if (nr < 1)
		nr = 1;
	if (nr > kilpparam.maxnosuus)
		nr = kilpparam.maxnosuus;
	ERinnMax->Text = nr;
	if (nr != Sarja1.maxnosuus) {
		Sarja1.maxnosuus = nr;
		naytaSarja(false);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::EOsLkmExit(TObject *Sender)
{
	Sarja1.ntosuus = _wtoi(EOsLkm->Text.c_str());
	naytaSarja(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::Kopioi1osuudenmatkakaikilleosuuksille1Click(TObject *Sender)

{
	for	(int os = 1; os < Sarja1.ntosuus; os++)
		memcpy(Sarja1.matka[os], Sarja1.matka[0], sizeof(Sarja1.matka[0]));
	naytaSarja(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::Kopioi1osuudenvliajatkaikilleosuuksille1Click(TObject *Sender)

{
	for	(int os = 1; os < Sarja1.ntosuus; os++) {
		Sarja1.valuku[os] = Sarja1.valuku[0];
		memcpy(Sarja1.va_matka[os], Sarja1.va_matka[0], sizeof(Sarja1.va_matka[0]));
		memcpy(Sarja1.va_piilota[os], Sarja1.va_piilota[0], sizeof(Sarja1.va_piilota[0]));
		}
	naytaSarja(false);
}
//---------------------------------------------------------------------------

