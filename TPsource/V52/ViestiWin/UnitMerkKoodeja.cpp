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

#include "UnitMerkKoodeja.h"
#include "VDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMerkKoodeja *FormMerkKoodeja;
//---------------------------------------------------------------------------
__fastcall TFormMerkKoodeja::TFormMerkKoodeja(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormMerkKoodeja::FormCreate(TObject *Sender)
{
	SG1->ColCount = 5;
	SG1->Cells[0][0] = L"No";
	SG1->Cells[1][0] = L"Osuus";
	if (RGTark->ItemIndex >= 7)
		SG1->Cells[2][0] = L"GPS";
	else
		SG1->Cells[2][0] = L"Tila";
	SG1->Cells[3][0] = L"Aika";
	SG1->Cells[4][0] = L"Joukkue";
	SG1->ColWidths[0] = 50 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[1] = 50 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[2] = 50 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[3] = 60 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[4] = 160 * Screen->PixelsPerInch / 96;
	SG1->RowCount = 2;
	SG1->Cells[0][1] = L"";
	SG1->Cells[1][1] = L"";
	SG1->Cells[2][1] = L"";
	SG1->Cells[3][1] = L"";
	SG1->Cells[4][1] = L"";
	if (kilpparam.kilplaji != L'S') {
		CBJoukkEil->Checked = true;
		CBLoppuEil->Checked = true;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormMerkKoodeja::SG1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	kilptietue kilp;
	int kno, os, d;
	wchar_t st[60];

	if (SG1->Col > 1 || SG1->Row == 0) {
		if (Key >= L' ')
			Key = 0;
		return;
		}
	if (SG1->Col == 0 && (Key == L'\r' || Key == L'\t')) {
		if ((kno = _wtoi(SG1->Cells[0][SG1->Row].c_str())) >= minkilpno &&
			(d = getpos(kno)) > 0) {
			kilp.getrec(d);
			SG1->Cells[4][SG1->Row] = kilp.Joukkue(st, 35);
			SG1->Cells[1][SG1->Row] = L"";
			SG1->Cells[2][SG1->Row] = L"";
			SG1->Cells[3][SG1->Row] = L"";
			}
		SG1->Col = 1;
		return;
		}
	if (SG1->Col == 1 && (Key == L'\r' || Key == L'\t')) {
		if ((kno = _wtoi(SG1->Cells[0][SG1->Row].c_str())) >= minkilpno &&
			(os =_wtoi(SG1->Cells[1][SG1->Row].c_str()) - 1) >= 0 &&
			(d = getpos(kno)) > 0) {
			kilp.getrec(d);
			SG1->Cells[4][SG1->Row] = kilp.Joukkue(st, 35);
			if (os < Sarjat[kilp.sarja].osuusluku) {
				if (RGTark->ItemIndex >= 7) {
					SG1->Cells[2][SG1->Row] = kilp.Seuranta(os);
					}
				else
					SG1->Cells[2][SG1->Row] = kilp.TarkStr(os);
				SG1->Cells[3][SG1->Row] = aikatowstr_cols_n(st, kilp.Maali(os, 0), t0, 0, 8);
				}
			if (SG1->RowCount <= SG1->Row + 1) {
				SG1->RowCount = SG1->Row + 2;
				SG1->Cells[0][SG1->Row+1] = L"";
				SG1->Cells[1][SG1->Row+1] = L"";
				SG1->Cells[2][SG1->Row+1] = L"";
				SG1->Cells[3][SG1->Row+1] = L"";
				SG1->Cells[4][SG1->Row+1] = L"";
				}
			SG1->Col = 0;
			SG1->Row++;
			}
		}

}
//---------------------------------------------------------------------------
void __fastcall TFormMerkKoodeja::Button3Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormMerkKoodeja::Button2Click(TObject *Sender)
{
	SG1->RowCount = 2;
	SG1->Cells[0][1] = L"";
	SG1->Cells[1][1] = L"";
	SG1->Cells[2][1] = L"";
	SG1->Cells[3][1] = L"";
	SG1->Cells[4][1] = L"";
}
//---------------------------------------------------------------------------

void __fastcall TFormMerkKoodeja::Button1Click(TObject *Sender)
{
	kilptietue kilp;
	int kno, os, d;
	wchar_t kh;

	if (RGTark->ItemIndex < 0)
		return;
	kh = L"E-KHTPI"[RGTark->ItemIndex];
	EnterCriticalSection(&tall_CriticalSection);
	for	(int r = 1; r < SG1->RowCount; r++) {
		if ((kno = _wtoi(SG1->Cells[0][r].c_str())) >= minkilpno &&
			(os =_wtoi(SG1->Cells[1][r].c_str()) - 1) >= 0 &&
			(d = getpos(kno)) > 0) {
			kilp.getrec(d);
			if (os < Sarjat[kilp.sarja].osuusluku) {
				if (RGTark->ItemIndex < 7) {
					kilp.SetTark(os, kh);
					if (CBJoukkEil->Checked && kh == L'E' && os == 0)
						for (int os1 = 1; os1 < Sarjat[kilp.sarja].osuusluku; os1++)
							kilp.SetTark(os1, kh);
					if (CBLoppuEil->Checked && wcswcind(kh, L"EKH"))
						for (int os1 = os+1; os1 < Sarjat[kilp.sarja].osuusluku; os1++)
							kilp.SetTark(os1, L'E');
					}
				else {
					kilp.setSeuranta(os, RGTark->ItemIndex == 7 ? L'G' : 0);
					}
				tallenna(&kilp, d, 0, 0, 0, 0);
				if (RGTark->ItemIndex < 7)
					SG1->Cells[2][r] = kilp.TarkStr(os);
				else
					SG1->Cells[2][r] = kilp.Seuranta(os);
				}
			}
		}
	LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------

