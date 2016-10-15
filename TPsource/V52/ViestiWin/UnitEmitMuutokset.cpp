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

#include "UnitEmitMuutokset.h"
#include "UnitEmit.h"
#include "VDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEmitMuutokset *FormEmitMuutokset;
//---------------------------------------------------------------------------
__fastcall TFormEmitMuutokset::TFormEmitMuutokset(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
	CBOsuus->Items->Clear();
	CBOsuus->Items->Add(L"Ei valittu");
	for (int os = 1; os <= kilpparam.osuusluku; os++)
		CBOsuus->Items->Add(UnicodeString(os)+L". osuus");
	CBOsuus->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitMuutokset::BtnPeruutaClick(TObject *Sender)
{
	FormEmit->PalautaMuutokset();
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitMuutokset::BtnTallennaClick(TObject *Sender)
{
	if (joTiedot) {
		if (Application->MessageBoxW(L"Tallentaminen valitulle kilpailijalle johtaa aeimmin kirjattujen tietojen menettämiseen."
			L" Haluatko aivan varmasti tallentaa?", L"Varoitus", MB_YESNO) != IDYES) {
			return;
			}
		}
	if (_wtoi(EdtKilpno->Text.c_str()) != FormEmit->Kilp.kilpno ||
		CBOsuus->ItemIndex > 0 && CBOsuus->ItemIndex != FormEmit->Osuus+1) {
		FormEmit->SiirraKilpailijalle(_wtoi(EdtKilpno->Text.c_str()), CBOsuus->ItemIndex-1);
		}
	if (!JoukkueHaku && !OsuusHaku)
		FormEmit->TallennaMuutokset();
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitMuutokset::CBSalliMuutoksetClick(TObject *Sender)
{
	FormEmit->SalliMuutokset(CBSalliMuutokset->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitMuutokset::naytaMemoOhje(bool nayta)
{
	if (nayta) {
		MemoOhje->Lines->Clear();
		MemoOhje->Lines->Text = L"Tällä kaavakkeella annetaan kilpailijan numero, kun kortti "
			L"on viallinen tai väärä. Näytetty numero on vaihdettava.";
		MemoOhje->Color = clYellow;
		}
	else {
		MemoOhje->Lines->Clear();
		MemoOhje->Color = clWindow;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitMuutokset::EdtKilpnoExit(TObject *Sender)
{
	int kno, d;

	kno = _wtoi(EdtKilpno->Text.c_str());
	EdtKilpno->Text = kno;
	DKilp = getpos(kno);
	if (kno > 0 && DKilp <= 0) {
		Application->MessageBoxW(L"Kilpailunumero ei käytössä", L"", MB_OK);
		return;
		}
	Nayta(DKilp, CBOsuus->ItemIndex-1);
//	FocusControl(CBOsuus);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::CBOsuusChange(TObject *Sender)
{
	if (DKilp > 0)
		Nayta(DKilp, CBOsuus->ItemIndex-1);
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitMuutokset::Nayta(int d, int osuus)
{
	kilptietue kilp;
	wchar_t st[62];

	joTiedot = false;
	DKilp = d;
	if (d > 0) {
		kilp.getrec(d);
		if (osuus < 0)
			osuus = 0;
		if (osuus >= Sarjat[kilp.sarja].osuusluku)
			osuus = Sarjat[kilp.sarja].osuusluku - 1;
		EdtKilpno->Text = kilp.kilpno;
		EdtSeura->Text = kilp.Joukkue(st, LSEURA);
		EdtSarja->Text = Sarjat[kilp.sarja].sarjanimi;
		EdtNimi->Text = kilp.Nimi(st, kilpparam.lnimi, osuus);
		EdtBadge->Text = kilp.ostiet[osuus].badge[0];
		if (kilp.ostiet[osuus].badge[0] > BADGEASKEL) {
			joTiedot = true;
			if (((kilp.ostiet[osuus].badge[0] - FormEmit->Em.badge0) % BADGEASKEL) == 0) {
				MemoOhje->Lines->Clear();
				MemoOhje->Lines->Text = L"Kortti jo luettu samalle osuudelle. Valitse toinen osuus tai merkitse"
					L" ohitetuksi.";
				MemoOhje->Color = clFuchsia;
				}
			else {
				MemoOhje->Lines->Clear();
				MemoOhje->Lines->Text = L"Kilpailijalle jo luettu Emit-tiedot. Yhteys aiempiin "
					L"tietoihin menetetään tallennettaessa.";
				MemoOhje->Color = clFuchsia;
				}
			}
		else {
			MemoOhje->Lines->Clear();
			MemoOhje->Lines->Text = L"Ei esteitä tietojen valitulle kilpailijalle tallentamiselle.";
			MemoOhje->Color = clWindow;
			}
		}
	else {
		EdtKilpno->Text = 0;
		EdtSeura->Text = L"";
		EdtSarja->Text = L"";
		EdtNimi->Text = L"";
		EdtBadge->Text = 0;
		}
	CBOsuus->ItemIndex = osuus+1;
	if (kilp.kilpno-minkilpno > 0 && osuus >= 0)
		CBLuettu->Checked = EmitJarr(kilp.kilpno, osuus) >= 0;
	else
		CBLuettu->Checked = false;
}
//---------------------------------------------------------------------------


void __fastcall TFormEmitMuutokset::EdtKilpnoKeyPress(TObject *Sender, System::WideChar &Key)
{
	NimiHaku = false;
	if (Key == L'\r') {
		EdtKilpnoExit(Sender);
		Key = 0;
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormEmitMuutokset::BtnSeurClick(TObject *Sender)
{
	int d;
	char key[MAXINDL];

	if (!NimiHaku) {
		keyfromU16name(key, EdtSeura->Text.c_str(), nameindex.keyl-3, 0);
		searchix(&nameindex, key, &d);
		NimiHaku = true;
		}
	if (nextix(&nameindex, key, &d)) {
		nextix(&nameindex, key, &d);
		previx(&nameindex, key, &d);
		}
	Nayta(d, CBOsuus->ItemIndex-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::BtnEdellClick(TObject *Sender)
{
	int d;
	char key[MAXINDL];

	if (!NimiHaku) {
		keyfromU16name(key, EdtSeura->Text.c_str(), nameindex.keyl-3, 0);
		searchix(&nameindex, key, &d);
		NimiHaku = true;
		}
	if (previx(&nameindex, key, &d)) {
		previx(&nameindex, key, &d);
		nextix(&nameindex, key, &d);
		}
	Nayta(d, CBOsuus->ItemIndex-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::EdtSeuraKeyPress(TObject *Sender, System::WideChar &Key)

{
	int d;
	char key[MAXINDL];

	if (Key == L'\x08')
		return;
	if (Key ==  L'\r') {
		keyfromU16name(key, EdtSeura->Text.c_str(), nameindex.keyl-3, 0);
		if (!searchix(&nameindex, key, &d)) {
			Nayta(d, CBOsuus->ItemIndex-1);
			NimiHaku = true;
			}
		}
	if (Key < L' ') {
		Key = 0;
		}
}
//---------------------------------------------------------------------------



void __fastcall TFormEmitMuutokset::BtnKohdennaClick(TObject *Sender)
{
	if (FormEmit->Kilp.Maali(FormEmit->Osuus, 0) != TMAALI0 && FormEmit->TViimRasti != TMAALI0) {
		FormEmit->Em.time += (FormEmit->Kilp.Maali(FormEmit->Osuus, 0) - FormEmit->TViimRasti) / KSEK;
		FormEmit->Nayta();
		EdtLuenta->Text = FormEmit->EdtLuettu->Text;
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormEmitMuutokset::EdtLuentaExit(TObject *Sender)
{
	int t;
	wchar_t as[20];

	if (CBLuenta->Checked) {
		t = wstrtoaika_vap(EdtLuenta->Text.c_str(), t0);
		EdtLuenta->Text = aikatowstr_cols_n(as, t, t0, L':', 8);
		FormEmit->Em.time = t / KSEK;
		FormEmit->Nayta();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::EdtLuentaKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		Key = 0;
		EdtLuentaExit(Sender);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::BtnPuolitaClick(TObject *Sender)
{
	FormEmit->PuolitaKortinAjat();
}
//---------------------------------------------------------------------------


void __fastcall TFormEmitMuutokset::BtnVaihdaClick(TObject *Sender)
{
int d;

	if (Application->MessageBoxW(L"Tietojen vaihto toteutetaan ilman lisävahvistusta. Jatka", L"Vahvista", MB_YESNO) == IDYES) {
		d = getpos(_wtoi(EdtKilpno->Text.c_str()));
		FormEmit->TallennaVaihto(d, CBOsuus->ItemIndex-1);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::BTnSiirraClick(TObject *Sender)
{
	FormEmit->SiirraKilpailijalle(_wtoi(EdtKilpno->Text.c_str()), CBOsuus->ItemIndex-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::EdtKilpnoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		Key = 0;
		if (CBOsuus->ItemIndex > 0)
			BtnTallennaClick(Sender);
		else
			FocusControl(CBOsuus);
		return;
		}
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		FocusControl(CBOsuus);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::Button2Click(TObject *Sender)
{
	EdtNimi->Enabled = true;
	BtnEdell->Enabled = true;
	BtnSeur->Enabled = true;
	BtnEdell->Font->Color = clWindowText;
	BtnSeur->Font->Color = clWindowText;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::FormShow(TObject *Sender)
{
	if (JoukkueHaku || OsuusHaku) {
		BtnVaihda->Visible = false;
		MemoOhje->Visible = true;
		MemoOhje->Lines->Clear();
		MemoOhje->Lines->Text = L"Kortti viallinen tai tunnistamaton. Anna joukkueen numero ja"
			L" kilpailijan osuus.";
//		MemoOhje->Lines->Add(L"Kortti viallinen tai tunnistamaton");
//		MemoOhje->Lines->Add(L"Anna joukkueen nimero ja");
//		MemoOhje->Lines->Add(L"kilpailijan osuus.");
		}
	EdtNimi->Enabled = false;
	BtnEdell->Enabled = false;
	BtnSeur->Enabled = false;
	CBLuenta->Checked = false;
	BtnEdell->Font->Color = clGrayText;
	BtnSeur->Font->Color = clGrayText;

	if (OsuusHaku)
		FocusControl(CBOsuus);
	else
		FocusControl(EdtKilpno);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::Button1Click(TObject *Sender)
{
	FormEmit->MerkitseOhitetuksi();
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Emit-korjaukset");
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::FormClose(TObject *Sender, TCloseAction &Action)
{
	JoukkueHaku = false;
	OsuusHaku = false;
	MemoOhje->Visible = false;
	FormEmit->EmitMuutosFlag = false;
	CBSalliMuutokset->Checked = false;
	FormEmit->SalliMuutokset(false);
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::BtnTallennaEmClick(TObject *Sender)
{
	FormEmit->TallennaEmMuutokset();
	Close();
}
//---------------------------------------------------------------------------


void __fastcall TFormEmitMuutokset::BtnPeruutaEmClick(TObject *Sender)
{
	FormEmit->PalautaEmMuutokset();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::BtnSulje1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::BtnKasvataClick(TObject *Sender)
{
	FormEmit->KasvataKortinAjat(_wtoi(EdtLisa->Text.c_str()));
}
//---------------------------------------------------------------------------

