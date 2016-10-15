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
#include "HkDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEmitMuutokset *FormEmitMuutokset;
//---------------------------------------------------------------------------
__fastcall TFormEmitMuutokset::TFormEmitMuutokset(TComponent* Owner)
	: TForm(Owner)
{
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
	if (_wtoi(EdtKilpno->Text.c_str()) != FormEmit->Kilp.id()) {
		FormEmit->SiirraKilpailijalle(_wtoi(EdtKilpno->Text.c_str()), RGBadge->ItemIndex);

//		if (Application->MessageBoxW(L"Viimeisintä muutosta ei vilä siirretty"
//		L"Tallennetaanko kaavakkeen Emit-tiedot tilanne?", L"Vahvistus", MB_YESNO) != IDYES) {
//			return;
//			}

		}
	FormEmit->TallennaMuutokset();
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitMuutokset::CBSalliMuutoksetClick(TObject *Sender)
{
	FormEmit->SalliMuutokset(CBSalliMuutokset->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitMuutokset::EdtKilpnoExit(TObject *Sender)
{
	int kno, d;

	kno = _wtoi(EdtKilpno->Text.c_str());
	EdtKilpno->Text = kno;
	d = getpos(kno);
	if (d <= 0) {
		Application->MessageBoxW(L"Kilpailunumero ei käytössä", L"", MB_OK);
		return;
		}
	Nayta(d);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::Nayta(int d)
{
	kilptietue kilp;
	wchar_t st[62];
	INT16 ibdg;

	kilp.GETREC(d);
	if (kilp.tark(k_pv) == L'P') {
		Application->MessageBoxW(L"Kilpailija merkitty poissaolevaksi", L"", MB_OK);
		return;
		}
	dKilp = d;
	EdtKilpno->Text = kilp.id();
	EdtNimi->Text = kilp.nimi(st, 60, false);
	EdtSeura->Text = kilp.seuratieto(0);
	EdtSarja->Text = Sarjat[kilp.Sarja()].sarjanimi;
	EdtBadge->Text = kilp.pv[k_pv].badge[RGBadge->ItemIndex];
	ibdg = RGBadge->ItemIndex;
	CBLuettu->Checked = getEmitJarr(kilp.id(), &ibdg) >= 0;
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
		keyfromwname(key, EdtNimi->Text.c_str(), nameindex.keyl-2, 0);
		searchix(&nameindex, key, &d);
		NimiHaku = true;
		}
	if (nextix(&nameindex, key, &d)) {
		nextix(&nameindex, key, &d);
		previx(&nameindex, key, &d);
		}
	Nayta(d);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::BtnEdellClick(TObject *Sender)
{
	int d;
	char key[MAXINDL];

	if (!NimiHaku) {
		keyfromwname(key, EdtNimi->Text.c_str(), nameindex.keyl-2, 0);
		searchix(&nameindex, key, &d);
		NimiHaku = true;
		}
	if (previx(&nameindex, key, &d)) {
		previx(&nameindex, key, &d);
		nextix(&nameindex, key, &d);
		}
	Nayta(d);
}
//---------------------------------------------------------------------------



void __fastcall TFormEmitMuutokset::EdtNimiKeyPress(TObject *Sender, System::WideChar &Key)

{
	int d;
	char key[MAXINDL];

	if (Key == L'\x08')
		return;
	if (Key ==  L'\r') {
		keyfromwname(key, EdtNimi->Text.c_str(), nameindex.keyl-2, 0);
		if (!searchix(&nameindex, key, &d)) {
			Nayta(d);
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
	if (FormEmit->Kilp.maali(0) != TMAALI0 && FormEmit->TViimRasti != TMAALI0) {
		FormEmit->Em.time += (FormEmit->Kilp.maali(0) - FormEmit->TViimRasti) / KSEK;
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
		if (d > 0)
			FormEmit->TallennaVaihto(d);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::BTnSiirraClick(TObject *Sender)
{
	FormEmit->SiirraKilpailijalle(_wtoi(EdtKilpno->Text.c_str()), RGBadge->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::EdtKilpnoKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		Key = 0;
		BtnTallennaClick(Sender);
		return;
		}
	if (Key == VK_RETURN && !Shift.Contains(ssShift) && !Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		FocusControl(BtnTallenna);
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
	EdtNimi->Enabled = false;
	BtnEdell->Enabled = false;
	BtnSeur->Enabled = false;
	CBLuenta->Checked = false;
	dKilp = 0;
	RGBadge->ItemIndex = 0;
	RGBadge->Visible = kilpparam.kaksibadge == 2;
	BtnEdell->Font->Color = clGrayText;
	BtnSeur->Font->Color = clGrayText;
	EdtKilpno->SetFocus();
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
	MemoOhje->Visible = false;
	FormEmit->EmitMuutosFlag = false;
	CBSalliMuutokset->Checked = false;
	FormEmit->SalliMuutokset(false);
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

void __fastcall TFormEmitMuutokset::RGBadgeClick(TObject *Sender)
{
	if (dKilp > 0)
		Nayta(dKilp);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitMuutokset::BtnKasvataClick(TObject *Sender)
{
	FormEmit->KasvataKortinAjat(_wtoi(EdtLisa->Text.c_str()));
}
//---------------------------------------------------------------------------

