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

#include "UnitEmitKoodit.h"
#include "HkDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEmitKoodit *FormEmitKoodit;
//---------------------------------------------------------------------------
__fastcall TFormEmitKoodit::TFormEmitKoodit(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitKoodit::FormCreate(TObject *Sender)
{
	SG1->ColCount = 5;
	SG1->Cells[0][0] = L"No";
	SG1->Cells[1][0] = L"Emit";
	SG1->Cells[2][0] = L"Laina";
	SG1->Cells[3][0] = L"Nimi";
	SG1->Cells[4][0] = L"Seura";
	SG1->ColWidths[0] = 50 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[1] = 60 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[2] = 35 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[3] = 160 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[4] = 100 * Screen->PixelsPerInch / 96;
	SG1->RowCount = 2;
	SG1->Cells[0][1] = L"";
	SG1->Cells[1][1] = L"";
	SG1->Cells[1][1] = L"";
	SG1->Cells[2][1] = L"";
	SG1->Cells[3][1] = L"";
	if (kilpparam.n_pv_akt == k_pv + 1) {
		CBVaiheet->Visible = false;
		Label1->Visible = false;
		}
	CBVaiheet->Clear();
	CBVaiheet->Items->Add(L"Tämä ja myöhemmät");
	CBVaiheet->Items->Add(L"Vain tämä");
	for (int i = 1; i <= kilpparam.n_pv_akt; i++)
		CBVaiheet->Items->Add(UnicodeString(L"Vain ")+i+L". vaihe");
	CBVaiheet->ItemIndex = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitKoodit::SG1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
	kilptietue kilp;
	int kno, d, i_pv;
	wchar_t st[60];

	if (SG1->Col > 1 || SG1->Row == 0) {
		if (Key >= L' ')
			Key = 0;
		return;
		}
	i_pv = k_pv;
	if (CBVaiheet->ItemIndex > 1)
		i_pv = CBVaiheet->ItemIndex - 2;
	if (SG1->Col == 0 && (Key == L'\r' || Key == L'\t')) {
		if ((kno = _wtoi(SG1->Cells[0][SG1->Row].c_str())) >= minkilpno &&
			(d = getpos(kno)) > 0) {
			kilp.GETREC(d);
			SG1->Cells[1][SG1->Row] = kilp.pv[i_pv].badge[RGBadge->ItemIndex];
			SG1->Cells[2][SG1->Row] = kilp.pv[i_pv].laina[RGBadge->ItemIndex];
			SG1->Cells[3][SG1->Row] = kilp.nimi(st, 40, false);
			SG1->Cells[4][SG1->Row] = kilp.seuratieto(NULL);
			if (SG1->RowCount <= SG1->Row + 1) {
				SG1->RowCount = SG1->Row + 2;
				SG1->Cells[0][SG1->Row+1] = L"";
				SG1->Cells[1][SG1->Row+1] = L"";
				SG1->Cells[2][SG1->Row+1] = L"";
				SG1->Cells[3][SG1->Row+1] = L"";
				SG1->Cells[4][SG1->Row+1] = L"";
				}
			SG1->Col = 1;
			}
		}
	else if (SG1->Col == 1 && (Key == L'\r' || Key == L'\t')) {
		SG1->Col = 0;
		SG1->Row++;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitKoodit::Button2Click(TObject *Sender)
{
	SG1->RowCount = 2;
	SG1->Cells[0][1] = L"";
	SG1->Cells[1][1] = L"";
	SG1->Cells[2][1] = L"";
	SG1->Cells[3][1] = L"";
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitKoodit::Button3Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitKoodit::Button1Click(TObject *Sender)
{
	kilptietue kilp;
	int kno, d, epv, vpv, bdg, k_bdg;

	k_bdg = RGBadge->ItemIndex;
	if (CBVaiheet->ItemIndex > 1) {
		epv = CBVaiheet->ItemIndex - 2;
		vpv = epv;
		}
	else {
		epv = k_pv;
		vpv =  CBVaiheet->ItemIndex == 0 ? kilpparam.n_pv_akt-1 : k_pv;
		}
	for	(int r = 1; r < SG1->RowCount; r++) {
		bdg = _wtoi(SG1->Cells[1][r].c_str());
		if ((kno = _wtoi(SG1->Cells[0][r].c_str())) >= minkilpno &&
			(d = getpos(kno)) > 0) {
			kilp.GETREC(d);
			for (int ipv = epv; ipv <= vpv; ipv++) {
				kilp.pv[ipv].badge[k_bdg] = bdg;
				if (RGLaina->ItemIndex > 0)
					kilp.pv[ipv].laina[k_bdg] = (RGLaina->ItemIndex == 1 ? L'L' : 0);
				}
			kilp.tallenna(d, 0, 0, 0, 0);
			SG1->Cells[1][r] = bdg;
			SG1->Cells[2][r] = kilp.pv[epv].laina[k_bdg];
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitKoodit::FormResize(TObject *Sender)
{
	SG1->Height = ClientHeight - SG1->Top - 4 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitKoodit::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Emitkoodien luenta");
}
//---------------------------------------------------------------------------

