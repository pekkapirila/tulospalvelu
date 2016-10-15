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

#include "UnitEmithaku.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEmithaku *FormEmithaku;
//---------------------------------------------------------------------------
__fastcall TFormEmithaku::TFormEmithaku(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmithaku::FormShow(TObject *Sender)
{
	SG1->ColCount = 5;
	SG1->ColWidths[3] = 60*Screen->PixelsPerInch / 96;
	SG1->ColWidths[4] = 120*Screen->PixelsPerInch / 96;
	SG1->Cells[0][0] = L"Emit-1";
	SG1->Cells[1][0] = L"Emit-2";
	SG1->Cells[2][0] = L"Kilpno";
	SG1->Cells[3][0] = L"Sarja";
	SG1->Cells[4][0] = L"Nimi";
	SG1->RowCount = 2;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmithaku::Button1Click(TObject *Sender)
{
	kilptietue kilp;

	if (Kdi == 0)
		return;
	nLoytyi = 0;
	for (int d = 1; d < nrec; d++) {
		bool loytyi = false;
		kilp.GETREC(d);
		if (kilp.kilpstatus)
			continue;
		for (int i = 0; i < (RG1->ItemIndex == 0 ? 1 : 9); i++) {
			if (kilp.Badge(k_pv, 0) == Kdi+i*BADGEASKEL ||
				RG1->ItemIndex == 1 && kilp.Badge(k_pv, 1) == Kdi+i*BADGEASKEL) {
				loytyi = true;
				break;
				}
			}
		if (loytyi) {
			wchar_t st[50];
			nLoytyi++;
			if (nLoytyi > 0) {
				SG1->RowCount = nLoytyi + 2;
				}
			SG1->Cells[0][nLoytyi] = kilp.pv[k_pv].badge[0];
			SG1->Cells[1][nLoytyi] = kilp.pv[k_pv].badge[1];
			SG1->Cells[2][nLoytyi] = kilp.id();
			SG1->Cells[3][nLoytyi] = Sarjat[kilp.Sarja()].sarjanimi;
			SG1->Cells[4][nLoytyi] = kilp.nimi(st, 30, false);
			}
		}
	if (nLoytyi == 0) {
		SG1->RowCount = 2;
		for (int c = 0; c < SG1->ColCount; c++)
			SG1->Cells[c][1] = L"";
		}
	SG1->Row = 1;
	if (nLoytyi == 1)
		BtnHyvaksy->SetFocus();
	else if (nLoytyi == 0)
		 Edit1->SetFocus();
	else
		SG1->SetFocus();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmithaku::Edit1Exit(TObject *Sender)
{
	Kdi = _wtoi(Edit1->Text.c_str());
	Edit1->Text = Kdi;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmithaku::BtnHyvaksyClick(TObject *Sender)
{
	Kilpno = _wtoi(SG1->Cells[2][SG1->Row].c_str());
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmithaku::Button2Click(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmithaku::SG1DblClick(TObject *Sender)
{
	BtnHyvaksyClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormEmithaku::Edit1KeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == VK_RETURN) {
		Key = 0;
		Kdi = _wtoi(Edit1->Text.c_str());
		Button1Click(Sender);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmithaku::SG1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	if (Key == VK_RETURN)
		BtnHyvaksyClick(Sender);
}
//---------------------------------------------------------------------------
