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
#include "UnitSprintSarja.h"
#include "HkDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSarja *FormSarja;

wchar_t *luonteet[3] = {L"Itsen‰inen", L"Jatko", L"Takaa-ajo"};
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
	naytaSarja();
}
//---------------------------------------------------------------------------
void __fastcall TFormSarja::naytaPaivat(void)
{
	for (int ipv = 0; ipv < 5; ipv++) {
		EL[ipv]->Visible = ipv >= Sarja1.enspv && ipv <= Sarja1.viimpv;
		EPort[ipv]->Visible = EL[ipv]->Visible;
		EM[ipv]->Visible = EL[ipv]->Visible;
		ELa[ipv]->Visible = EL[ipv]->Visible;
		ELV[ipv]->Visible = EL[ipv]->Visible;
		EJLa[ipv]->Visible = EL[ipv]->Visible;
		ELS[ipv]->Visible = EL[ipv]->Visible;
		ETS[ipv]->Visible = EL[ipv]->Visible;
		EBib[ipv]->Visible = EL[ipv]->Visible;
		EMtk[ipv]->Visible = EL[ipv]->Visible;
		ETapa[ipv]->Visible = EL[ipv]->Visible;
		EPari[ipv]->Visible = EL[ipv]->Visible;
		EdtFlags[ipv]->Visible = EL[ipv]->Visible;
		CBLuonne[ipv]->Visible = EL[ipv]->Visible;
		if (CBLuonne[ipv]->Visible) {
			int ii = CBLuonne[ipv]->ItemIndex;
			CBLuonne[ipv]->Clear();
			CBLuonne[ipv]->Items->Add(L"Itsen.");
			CBLuonne[ipv]->Items->Add(L"Jatko");
			CBLuonne[ipv]->Items->Add(L"Tak-ajo");
			if (ipv > Sarja1.enspv && ipv < Sarja1.viimpv)
				CBLuonne[ipv]->Items->Add(L"Ohita");
			if (ipv == Sarja1.enspv)
				CBLuonne[ipv]->ItemIndex = 0;
			else {
				if (ii >= CBLuonne[ipv]->Items->Count)
					CBLuonne[ipv]->ItemIndex = 1;
				else
					CBLuonne[ipv]->ItemIndex = ii;
				}
			CBLuonne[ipv]->Enabled = ipv > Sarja1.enspv;
			}
		LbV[ipv]->Visible = EL[ipv]->Visible;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSarja::naytaSarja(void)
{
	wchar_t st[20];

	EdtSarja->Text = UnicodeString(SarjaNo+1);
	SNimi->Text = UnicodeString(Sarja1.sarjanimi);
	PNimi->Text = UnicodeString(Sarja1.psarjanimi);
	EPv->Text = UnicodeString(Sarja1.enspv+1);
	VPv->Text = UnicodeString(Sarja1.viimpv+1);
	Alku->Text = UnicodeString(Sarja1.sarjaalku);
	swprintf(st, L"%d,%02d", Sarja1.maksu/100, Sarja1.maksu%100);
	Mks->Text = UnicodeString(st);
	swprintf(st, L"%d,%02d", Sarja1.maksu1/100, Sarja1.maksu1%100);
	Mks1->Text = UnicodeString(st);
	swprintf(st, L"%d,%02d", Sarja1.maksual/100, Sarja1.maksual%100);
	MksAl->Text = UnicodeString(st);
	CBSak->ItemIndex = Sarja1.rrtas;
	CBVakanttisarja->Checked = Sarja1.vakanttisarja;
	CBSprintSrj->ItemIndex = Sarja1.sprintSrj.vaihelkm > 1 ? 1 : 0;
	naytaPaivat();
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		EL[ipv]->Text = UnicodeString(Sarja1.lno[ipv]);
		EPort[ipv]->Text = UnicodeString(Sarja1.portti[ipv]);
		EM[ipv]->Text = UnicodeString(Sarja1.mno[ipv]);
		EBib[ipv]->Text = UnicodeString(Sarja1.bibalku[ipv]);
		ELa[ipv]->Text = UnicodeString(AIKATOWSTRS(st, Sarja1.enslahto[ipv], t0)).SubString(1,8);
		EJLa[ipv]->Text = UnicodeString(AIKATOWSTRS(st, Sarja1.ensjlahto[ipv], t0)).SubString(1,8);
		EPari[ipv]->Text = UnicodeString(Sarja1.pari[ipv]);
		ELV[ipv]->Text = UnicodeString(Sarja1.lahtovali[ipv]);
		ELS[ipv]->Text = UnicodeString(Sarja1.lsak[ipv]);
		ETS[ipv]->Text = UnicodeString(Sarja1.tsak[ipv]);
		EMtk[ipv]->Text = UnicodeString(Sarja1.matka[ipv]);
		ETapa[ipv]->Text = UnicodeString(Sarja1.tapa[ipv]);
		EdtFlags[ipv]->Text = UnicodeString(Sarja1.flags[ipv]);
		CBLuonne[ipv]->ItemIndex = Sarja1.luonne[ipv] >= 0 ? Sarja1.luonne[ipv] : CBLuonne[ipv]->Items->Count - 1;
		}
	if (kilpparam.kilplaji == L'B') {
		int k = 0;
		for (int i = 0; i < 5; i++) {
			CBAsLaji[i]->ItemIndex = wcswcind(Sarja1.asuunnlaji[as_pv][i], L"SLOB") + 1;
			if (Sarja1.asuunnlaji[as_pv][i] == L'S') {
				EdtAsSakko[i]->Text = Sarja1.asuunnsakot[as_pv][k];
				if (k == 0)
					k = 1;
				}
			else if (Sarja1.asuunnlaji[as_pv][i] == L'L')
				EdtAsSakko[i]->Text = Sarja1.asuunnsakot[as_pv][3];
			else
				EdtAsSakko[i]->Text = 0;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::tallSarja(void)
{
	if (pakotaIsot)
		wcscpy(Sarja1.sarjanimi, upcasewstr(SNimi->Text.SubString(1, LSARJA).c_str()));
	else
		wcscpy(Sarja1.sarjanimi, SNimi->Text.SubString(1, LSARJA).c_str());
	wcscpy(Sarja1.psarjanimi, PNimi->Text.SubString(1, LPSARJA).c_str());
	Sarja1.enspv = _wtoi(EPv->Text.c_str())-1;
	if (Sarja1.enspv < 0)
		Sarja1.enspv = 0;
	if (Sarja1.enspv >= kilpparam.n_pv_akt)
		Sarja1.enspv = kilpparam.n_pv_akt-1;
	Sarja1.viimpv = _wtoi(VPv->Text.c_str())-1;
	if (Sarja1.viimpv >= kilpparam.n_pv_akt)
		Sarja1.viimpv = kilpparam.n_pv_akt-1;
	if (Sarja1.viimpv < Sarja1.enspv)
		Sarja1.viimpv = Sarja1.enspv;
	Sarja1.sarjaalku = _wtoi(Alku->Text.c_str());
	Sarja1.maksu = wcstomaksu(Mks->Text.c_str());
	Sarja1.maksu1 = wcstomaksu(Mks1->Text.c_str());
	Sarja1.maksual = wcstomaksu(MksAl->Text.c_str());
	Sarja1.rrtas = CBSak->ItemIndex;
	Sarja1.piilotatulos = CBSak->ItemIndex == 5;
	Sarja1.vakanttisarja = CBVakanttisarja->Checked;
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		Sarja1.lno[ipv] = _wtoi(EL[ipv]->Text.c_str());
		Sarja1.portti[ipv] = _wtoi(EPort[ipv]->Text.c_str());
		Sarja1.mno[ipv] = _wtoi(EM[ipv]->Text.c_str());
		Sarja1.bibalku[ipv] = _wtoi(EBib[ipv]->Text.c_str());
		Sarja1.enslahto[ipv] = wstrtoaika_vap(ELa[ipv]->Text.c_str(), t0);
		Sarja1.ensjlahto[ipv] = wstrtoaika_vap(EJLa[ipv]->Text.c_str(), t0);
		Sarja1.pari[ipv] = _wtoi(EPari[ipv]->Text.c_str());
		if (Sarja1.pari[ipv] < 1)
			Sarja1.pari[ipv] = 1;
		Sarja1.lahtovali[ipv] = _wtoi(ELV[ipv]->Text.c_str());
		Sarja1.lsak[ipv] = (kilpparam.kilplaji == L'B' ? 5 : _wtoi(ELS[ipv]->Text.c_str()));
		Sarja1.tsak[ipv] = _wtoi(ETS[ipv]->Text.c_str());
		wcscpy(Sarja1.matka[ipv], EMtk[ipv]->Text.SubString(1, sizeof(Sarja1.matka[ipv])/2-1).c_str());
		wcscpy(Sarja1.tapa[ipv], ETapa[ipv]->Text.SubString(1, 2).c_str());
		wcscpy(Sarja1.flags[ipv], EdtFlags[ipv]->Text.SubString(1, sizeof(Sarja1.flags[ipv])/2-1).c_str());
		Sarja1.luonne[ipv] = CBLuonne[ipv]->ItemIndex;
		if (ipv < Sarja1.enspv || ipv > Sarja1.viimpv)
			Sarja1.luonne[ipv] = -1;
		else if (ipv == Sarja1.enspv)
			Sarja1.luonne[ipv] = 0;
		else if (Sarja1.luonne[ipv] < 0)
			Sarja1.luonne[ipv] = 1;
		else if (Sarja1.luonne[ipv] >= CBLuonne[0]->Items->Count)
			Sarja1.luonne[ipv] = -1;
		}
	if (kilpparam.kilplaji == L'B') {
		int k = 0;
		for (int i = 0; i < 5; i++) {
			if (CBAsLaji[i]->ItemIndex > 0)
				Sarja1.asuunnlaji[as_pv][i] = L"SLOB"[CBAsLaji[i]->ItemIndex-1];
			else
				Sarja1.asuunnlaji[as_pv][i] = L' ';
			if (Sarja1.asuunnlaji[as_pv][i] == L'S') {
				if (k++ == 0)
					Sarja1.asuunnsakot[as_pv][0] = _wtoi(EdtAsSakko[i]->Text.c_str());
				Sarja1.asuunnsakot[as_pv][1] = _wtoi(EdtAsSakko[i]->Text.c_str());
				}
			if (Sarja1.asuunnlaji[as_pv][i] == L'L')
				Sarja1.asuunnsakot[as_pv][3] = _wtoi(EdtAsSakko[i]->Text.c_str());
			}
		elimwbl(Sarja1.asuunnlaji[as_pv]);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::FormCreate(TObject *Sender)
{
	EL[0] = L1;
	EL[1] = L2;
	EL[2] = L3;
	EL[3] = L4;
	EL[4] = L5;
	EM[0] = M1;
	EM[1] = M2;
	EM[2] = M3;
	EM[3] = M4;
	EM[4] = M5;
	ELa[0] = La1;
	ELa[1] = La2;
	ELa[2] = La3;
	ELa[3] = La4;
	ELa[4] = La5;
	EPort[0] = Port1;
	EPort[1] = Port2;
	EPort[2] = Port3;
	EPort[3] = Port4;
	EPort[4] = Port5;
	EJLa[0] = JLa1;
	EJLa[1] = JLa2;
	EJLa[2] = JLa3;
	EJLa[3] = JLa4;
	EJLa[4] = JLa5;
	ELV[0] = LV1;
	ELV[1] = LV2;
	ELV[2] = LV3;
	ELV[3] = LV4;
	ELV[4] = LV5;
	ELS[0] = LS1;
	ELS[1] = LS2;
	ELS[2] = LS3;
	ELS[3] = LS4;
	ELS[4] = LS5;
	ETS[0] = TS1;
	ETS[1] = TS2;
	ETS[2] = TS3;
	ETS[3] = TS4;
	ETS[4] = TS5;
	EBib[0] = Bib1;
	EBib[1] = Bib2;
	EBib[2] = Bib3;
	EBib[3] = Bib4;
	EBib[4] = Bib5;
	EMtk[0] = Mtk1;
	EMtk[1] = Mtk2;
	EMtk[2] = Mtk3;
	EMtk[3] = Mtk4;
	EMtk[4] = Mtk5;
	ETapa[0] = Tapa1;
	ETapa[1] = Tapa2;
	ETapa[2] = Tapa3;
	ETapa[3] = Tapa4;
	ETapa[4] = Tapa5;
	EPari[0] = Pari1;
	EPari[1] = Pari2;
	EPari[2] = Pari3;
	EPari[3] = Pari4;
	EPari[4] = Pari5;
	EdtFlags[0] = EdtFlags1;
	EdtFlags[1] = EdtFlags2;
	EdtFlags[2] = EdtFlags3;
	EdtFlags[3] = EdtFlags4;
	EdtFlags[4] = EdtFlags5;
	CBLuonne[0] = CBLuonne1;
	CBLuonne[1] = CBLuonne2;
	CBLuonne[2] = CBLuonne3;
	CBLuonne[3] = CBLuonne4;
	CBLuonne[4] = CBLuonne5;
	CBAsLaji[0] = CBAsLaji1;
	CBAsLaji[1] = CBAsLaji2;
	CBAsLaji[2] = CBAsLaji3;
	CBAsLaji[3] = CBAsLaji4;
	CBAsLaji[4] = CBAsLaji5;
	EdtAsSakko[0] = EdtAsSakko1;
	EdtAsSakko[1] = EdtAsSakko2;
	EdtAsSakko[2] = EdtAsSakko3;
	EdtAsSakko[3] = EdtAsSakko4;
	EdtAsSakko[4] = EdtAsSakko5;
	LbV[0] = LbV1;
	LbV[1] = LbV2;
	LbV[2] = LbV3;
	LbV[3] = LbV4;
	LbV[4] = LbV5;
	if (ToimintaTila == 2) {
		EdtSarja->ReadOnly = true;
		EdtSarja->Enabled = false;
		EdtSarja->TabStop = false;
		Button2->Enabled = false;
		Button2->TabStop = false;
		Button4->Enabled = false;
		Button4->TabStop = false;
		}
	GBAmpSuunn->Visible = kilpparam.kilplaji == L'B';
	CBSprintSrj->Visible = kilpparam.kilplaji == L'H' && kilpparam.alalaji == L'S';
	if (kilpparam.kilplaji == L'B') {
		ClientWidth = GBAmpSuunn->Left + GBAmpSuunn->Width + 11 * Screen->PixelsPerInch /96;
		CBVaiheAS->ItemIndex = 0;
		CBVaiheAS->Enabled = kilpparam.n_pv_akt > 1;
		as_pv = 0;
		}
	else
		ClientWidth = Button2->Left + Button2->Width + 11 * Screen->PixelsPerInch /96;
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::FormShow(TObject *Sender)
{
	naytaSarja();
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
			if (SarjaNo >= sarjaluku)
				SarjaNo = sarjaluku-1;
			lataaSarja(SarjaNo);
			naytaSarja();
			FormSarjatiedot->NaytaSarjat();
			}
		}
	else {
		Application->MessageBoxW(L"Sarjan numero ei k‰ytˆss‰", L"Virhe", MB_OK);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::Button2Click(TObject *Sender)
{
	lataaSarja(-1);
	SNimi->Text = L"";
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::BtnTallennaClick(TObject *Sender)
{
	int srj;

	srj = EdtSarja->Text.ToInt() - 1;
	if (srj < 0) {
		if (Sarjat[sarjaluku-1].vakanttisarja)
			srj = sarjaluku - 1;
		else
			srj = sarjaluku;
	   }
	if (srj > sarjaluku)
		srj = sarjaluku;
	tallSarja();
	naytaSarja();
	EdtSarja->Text = UnicodeString(srj+1);
	if (SarjaNo == -1 || srj == sarjaluku ||
		(srj == sarjaluku - 1 && Sarjat[sarjaluku-1].vakanttisarja && !Sarja1.vakanttisarja)) {
		if (srj < sarjaluku) {
			if (srj == sarjaluku - 1 && Sarjat[sarjaluku-1].vakanttisarja) {
				if (Application->MessageBoxW(L"Sarja lis‰t‰‰n loppuun vakanttisarjan edelle. Vahvista",
					L"Sarjan lis‰ys", MB_YESNO) != IDYES)
					return;
				}
			else
				if (Application->MessageBoxW(L"Sarja lis‰t‰‰n annetun sarjanumeron paikalle. Vahvista",
					L"Sarjan lis‰ys", MB_YESNO) != IDYES)
					return;
			memmove(&Sarjat[srj+1], &Sarjat[srj], (sarjaluku-srj)*sizeof(Sarjat[0]));
			Sarjat[srj] = Sarja1;
			sarjaluku++;
			vaihdakilpsrj(srj, sarjaluku-2, 1, true);
			}
		else {
			if (Application->MessageBoxW(L"Sarja lis‰t‰‰n viimeiseksi. Vahvista",
				L"Sarjan lis‰ys", MB_YESNO) != IDYES)
				return;
			sarjaluku++;
			Sarjat[srj] = Sarja1;
			}
		}
	else if (srj != SarjaNo) {
		if (Application->MessageBoxW(L"Sarja siirret‰‰n annetun sarjanumeron paikalle. Vahvista",
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
		Sarjat[srj] = Sarja1;
		}
	if (Sarjat[srj].lsak[k_pv] > kilpparam.lsakmax)
		kilpparam.lsakmax = Sarjat[srj].lsak[k_pv];
	kirjEventXml(sarjaxmlfname, true, false, false);
//	kirjsarjat();
	lataaSarja(srj);
//	naytaSarja();
	FormSarjatiedot->NaytaSarjat();
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

void __fastcall TFormSarja::Mks1Exit(TObject *Sender)
{
	int m;
	wchar_t st[20];

	m = wcstomaksu(Mks1->Text.c_str());
	swprintf(st, L"%d,%02d", m/100, m%100);
	Mks1->Text = UnicodeString(st);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::LaExit(int ipv)
{
	wchar_t st[20];

	ELa[ipv]->Text = UnicodeString(
		AIKATOWSTRS(st, wstrtoaika_vap(ELa[ipv]->Text.c_str(), t0), t0)).SubString(1,8);
}
//---------------------------------------------------------------------------
void __fastcall TFormSarja::JLaExit(int ipv)
{
	wchar_t st[20];

	EJLa[ipv]->Text = UnicodeString(
		AIKATOWSTRS(st, wstrtoaika_vap(EJLa[ipv]->Text.c_str(), t0), t0)).SubString(1,8);
}
//---------------------------------------------------------------------------
void __fastcall TFormSarja::La1Exit(TObject *Sender)
{
	LaExit(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::La2Exit(TObject *Sender)
{
	LaExit(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::La3Exit(TObject *Sender)
{
	LaExit(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::La4Exit(TObject *Sender)
{
	LaExit(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::La5Exit(TObject *Sender)
{
	LaExit(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::JLa1Exit(TObject *Sender)
{
	JLaExit(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::JLa2Exit(TObject *Sender)
{
	JLaExit(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::JLa3Exit(TObject *Sender)
{
	JLaExit(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::JLa4Exit(TObject *Sender)
{
	JLaExit(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::JLa5Exit(TObject *Sender)
{
	JLaExit(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::MtkExit(int ipv)
{
	EMtk[ipv]->Text = EMtk[ipv]->Text.SubString(1, sizeof(Sarjat[0].matka[0])/2-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::Mtk1Exit(TObject *Sender)
{
	MtkExit(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::Mtk2Exit(TObject *Sender)
{
	MtkExit(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::Mtk3Exit(TObject *Sender)
{
	MtkExit(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::Mtk4Exit(TObject *Sender)
{
	MtkExit(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::Mtk5Exit(TObject *Sender)
{
	MtkExit(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::CBSakClick(TObject *Sender)
{
	if (kilpparam.kilplaji != L'A') {
		for	(int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++)
			ETS[ipv]->Text = (kilpparam.kilplaji == L'S' && CBSak->ItemIndex == 2) ?
				kilpparam.sakkoyks : 1;
		}

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

void __fastcall TFormSarja::VPvExit(TObject *Sender)
{
	int vpv = _wtoi(VPv->Text.c_str()) - 1;
	for (int i_pv = Sarja1.viimpv + 1; i_pv <= vpv; i_pv++) {
		if (Sarja1.luonne[i_pv] < 0 || Sarja1.luonne[i_pv] >= CBLuonne[0]->Items->Count)
			Sarja1.luonne[i_pv] = i_pv == Sarja1.enspv ? 0 : 1;
		}
	Sarja1.viimpv = vpv;
	if (Sarja1.viimpv < 0)
		Sarja1.viimpv = 0;
	if (Sarja1.viimpv >= kilpparam.n_pv_akt)
		Sarja1.viimpv = kilpparam.n_pv_akt-1;
	naytaPaivat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::EPvExit(TObject *Sender)
{
	int epv = _wtoi(EPv->Text.c_str()) - 1;
	for (int i_pv = epv; i_pv < Sarja1.enspv; i_pv++) {
		if (Sarja1.luonne[i_pv] < 0 || Sarja1.luonne[i_pv] >= CBLuonne[0]->Items->Count)
			Sarja1.luonne[i_pv] = i_pv == Sarja1.enspv ? 0 : 1;
		}
	Sarja1.enspv = _wtoi(VPv->Text.c_str()) - 1;
	if (Sarja1.enspv < 0)
		Sarja1.enspv = 0;
	if (Sarja1.enspv >= kilpparam.n_pv_akt)
		Sarja1.enspv = kilpparam.n_pv_akt-1;
	naytaPaivat();
}
//---------------------------------------------------------------------------

void __fastcall TFormSarja::CBSprintSrjClick(TObject *Sender)
{
	if (FormSprintSarja == NULL)
		FormSprintSarja = new TFormSprintSarja(this);
	FormSprintSarja->sprSrj = Sarja1.sprintSrj;
	FormSprintSarja->pSarja = SarjaNo;
	FormSprintSarja->ShowModal();
	if (FormSprintSarja->ModalResult == mrOk) {
		Sarja1.sprintSrj = FormSprintSarja->sprSrj;
		CBSprintSrj->ItemIndex = Sarja1.sprintSrj.vaihelkm > 0 ? 1 : 0;
		Sarja1.viimpv = Sarja1.enspv + Sarja1.sprintSrj.vaihelkm - 1;
		BtnTallennaClick(Sender);
		}
	delete FormSprintSarja;
	FormSprintSarja = NULL;
}
//---------------------------------------------------------------------------


void __fastcall TFormSarja::CBVaiheASChange(TObject *Sender)
{
	if (kilpparam.kilplaji == L'B') {
		for (int i = 0; i < 5; i++) {
			if (CBAsLaji[i]->ItemIndex > 0)
				Sarja1.asuunnlaji[as_pv][i], L"SLOB"[CBAsLaji[i]->ItemIndex-1];
			else
				Sarja1.asuunnlaji[as_pv][i] = 0;
			Sarja1.asuunnsakot[as_pv][i] = _wtoi(EdtAsSakko[i]->Text.c_str());
			}
		}
	as_pv = CBVaiheAS->ItemIndex;
	naytaSarja();
}
//---------------------------------------------------------------------------


void __fastcall TFormSarja::TapaExit(TObject *Sender)
{
	for	(int i = 0; i < 5; i++) {
		if (Sender == ETapa[i]) {
			ETapa[i]->Text = ETapa[i]->Text.UpperCase();
			for (int k = 0; k < ETapa[i]->Text.Length(); k++) {
				if (wcswcind(ETapa[i]->Text[k+1], L"VP") < 0) {
					UnicodeString St = ETapa[i]->Text;
					St.Delete(k+1, 1);
					ETapa[i]->Text = St;
					}
				if (k >= ETapa[i]->Text.Length())
					break;
				}
			if (ETapa[i]->Text.Length() > 2)
				ETapa[i]->Text = ETapa[i]->Text.SubString(1, 2);
			}
		}
}
//---------------------------------------------------------------------------



