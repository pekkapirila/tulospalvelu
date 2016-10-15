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

#include "UnitYLahdot.h"
#include "VDeclare.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormYLahdot *FormYLahdot;

static int ylvert[MAXSARJALUKU][MAXOSUUSLUKU];

//---------------------------------------------------------------------------
__fastcall TFormYLahdot::TFormYLahdot(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormYLahdot::LBSarjatDblClick(TObject *Sender)
{
	int srj;

	srj = haesarja_w(LBSarjat->Items->Strings[LBSarjat->ItemIndex].c_str(), false);
	if (srj >= 0)
		SarjaValinnat[srj] = true;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------
void __fastcall TFormYLahdot::LBSarjaValDblClick(TObject *Sender)
{
	int srj;

	srj = haesarja_w(LBSarjaVal->Items->Strings[LBSarjaVal->ItemIndex].c_str(), false);
	if (srj >= 0)
		SarjaValinnat[srj] = false;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------
void __fastcall TFormYLahdot::BtnLisKaikkiClick(TObject *Sender)
{
	for (int srj = 0; srj < sarjaluku; srj++)
		SarjaValinnat[srj] = true;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------
void __fastcall TFormYLahdot::BtnLisValitutClick(TObject *Sender)
{
	for (int rv = 0; rv < LBSarjat->Items->Count; rv++) {
		if (LBSarjat->Selected[rv]) {
			int srj;
			if ((srj = haesarja_w(LBSarjat->Items->Strings[rv].c_str(), false)) >= 0)
				SarjaValinnat[srj] = true;
			}
		}
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------
void __fastcall TFormYLahdot::BtnPoisValitutClick(TObject *Sender)
{
	for (int rv = 0; rv < LBSarjaVal->Items->Count; rv++) {
		if (LBSarjaVal->Selected[rv]) {
			int srj;
			if ((srj = haesarja_w(LBSarjaVal->Items->Strings[rv].c_str(), false)) >= 0)
				SarjaValinnat[srj] = false;
			}
		}
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------
void __fastcall TFormYLahdot::BtnPoisKaikkiClick(TObject *Sender)
{
	for (int srj = 0; srj < sarjaluku; srj++)
		SarjaValinnat[srj] = false;
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------

void TFormYLahdot::NaytaSarjavalinnat(void)
{
	LBSarjat->Clear();
	LBSarjaVal->Clear();
	for (int srj = 0; srj < sarjaluku; srj++) {
		if (SarjaValinnat[srj])
			LBSarjaVal->Items->Add(UnicodeString(Sarjat[srj].sarjanimi));
		else
			LBSarjat->Items->Add(UnicodeString(Sarjat[srj].sarjanimi));
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormYLahdot::FormResize(TObject *Sender)
{
	LBSarjat->Height = ClientHeight - LBSarjat->Top - 8;
	LBSarjaVal->Height = ClientHeight - LBSarjaVal->Top - 8;
	SG1->Height = ClientHeight - SG1->Top - 8;
}
//---------------------------------------------------------------------------
void __fastcall TFormYLahdot::FormShow(TObject *Sender)
{
	int vos = 1;
	int srj;

	for (srj = 0; srj < sarjaluku; srj++) {
		for (int os = 0; os < Sarjat[srj].ntosuus; os++)
			ylvert[srj][os] = Sarjat[srj].ylahto[os] + Sarjat[srj].ylkynnys[os];
		}
	for (srj = 0; srj < sarjaluku; srj++) {
		if (SarjaValinnat[srj]) {
			for (int os = 1; os < Sarjat[srj].ntosuus; os++) {
				wchar_t as[20];
				SG1->Cells[1][os] = aikatowstr_cols_n(as, Sarjat[srj].ylahto[os], t0, 0, 8);
				SG1->Cells[2][os] = aikatowstr_cols_n(as, Sarjat[srj].ylkynnys[os], t0, 0, 8);
				}
			vos = Sarjat[srj].ntosuus;
			break;
			}
		}
	for (int os = vos; os < kilpparam.ntosuus; os++) {
		SG1->Cells[1][os] = L"";
		SG1->Cells[2][os] = L"";
		}
	for (int srj1 = 0; srj1 < sarjaluku; srj1++) {
		if (srj1 == srj)
			continue;
		if (Sarjat[srj1].ntosuus != Sarjat[srj].ntosuus)
			SarjaValinnat[srj1] = false;
		SarjaValinnat[srj1] = true;
		for (int os = 1; os < Sarjat[srj].ntosuus; os++) {
			if (Sarjat[srj1].ylahto[os] != Sarjat[srj].ylahto[os] ||
				Sarjat[srj1].ylkynnys[os] != Sarjat[srj].ylkynnys[os]) {
				SarjaValinnat[srj1] = false;
				break;
				}
			}
		}
	NaytaSarjavalinnat();
}
//---------------------------------------------------------------------------
void __fastcall TFormYLahdot::FormCreate(TObject *Sender)
{
	for (int srj = 0; srj < sarjaluku; srj++) {
		SarjaValinnat[srj] = true;
		}
	SG1->RowCount = kilpparam.ntosuus;
	SG1->ColWidths[0] = 55 * Screen->PixelsPerInch / 96;
	SG1->Cells[0][0] = L"Osuus";
	SG1->Cells[1][0] = L"Yht.lähtö";
	SG1->Cells[2][0] = L"Vaihto sulj.";
	for (int os = 2; os <= kilpparam.ntosuus; os++) {
		wchar_t as[20];
		SG1->Cells[0][os-1] = UnicodeString(os)+L". osuus";
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormYLahdot::Button1Click(TObject *Sender)
{
	int n_os = 0;

	for (int srj = 0; srj < sarjaluku; srj++) {
		if (SarjaValinnat[srj]) {
			if (n_os < Sarjat[srj].ntosuus)
				n_os = Sarjat[srj].ntosuus;
			}
		}
	for (int os = 2; os < n_os; os++) {
		SG1->Cells[1][os] = SG1->Cells[1][1];
		SG1->Cells[2][os] = SG1->Cells[2][1];
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormYLahdot::Button2Click(TObject *Sender)
{
	for (int os = 1; os < kilpparam.ntosuus; os++) {
		SG1->Cells[2][os] = L"";
		}
}
//---------------------------------------------------------------------------
static bool ylmuutt(int srj) {
	for (int os = 0; os < Sarjat[srj].ntosuus; os++) {
		if (ylvert[srj][os] != Sarjat[srj].ylahto[os] + Sarjat[srj].ylkynnys[os])
			return(true);
		}
	return(false);
}
//---------------------------------------------------------------------------
void __fastcall TFormYLahdot::Button3Click(TObject *Sender)
{
	for (int srj = 0; srj < sarjaluku; srj++) {
		if (SarjaValinnat[srj]) {
			Sarjat[srj].ylkynnys[0] = TMAALI0;
			for (int os = 1; os < Sarjat[srj].ntosuus; os++) {
				wchar_t as[20];
				Sarjat[srj].ylahto[os] = wstrtoaika_vap(SG1->Cells[1][os].c_str(), t0);
				Sarjat[srj].ylkynnys[os] = wstrtoaika_vap(SG1->Cells[2][os].c_str(), t0);
				if (Sarjat[srj].ylkynnys[0] == TMAALI0)
					Sarjat[srj].ylkynnys[0] = Sarjat[srj].ylkynnys[os];
				SG1->Cells[1][os] = aikatowstr_cols_n(as, Sarjat[srj].ylahto[os], t0, 0, 8);
				SG1->Cells[2][os] = aikatowstr_cols_n(as, Sarjat[srj].ylkynnys[os], t0, 0, 8);
				}
			Sarjat[srj].ylahto[0] = Sarjat[srj].ylahto[1];
			}
		}
	kirjEventXml(sarjaxmlfname, true, false, false);
	for (int srj = 0; srj < sarjaluku; srj++) {
		if (ylmuutt(srj)) {
			laheta_yl(srj,0);
			Sleep(1000);
			for (int os = 1; os < Sarjat[srj].ntosuus; os++) {
				if (Sarjat[srj].ylkynnys[os] != TMAALI0) {
					Label1->Visible = true;
					Label2->Caption = Sarjat[srj].sarjanimi;
					Label2->Visible = true;
					Label1->Refresh();
					Label2->Refresh();
					Sleep(100);
					merk_yht_lahdot(srj, false);
					Label1->Visible = false;
					Label2->Visible = false;
					break;
					}
				}
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormYLahdot::Button6Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormYLahdot::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Yhteislähdöt");
}
//---------------------------------------------------------------------------

