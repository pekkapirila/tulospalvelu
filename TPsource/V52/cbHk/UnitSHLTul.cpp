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

#include "UnitSHLTul.h"
#include "TulostusUnit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSHLtul *FormSHLtul;
//---------------------------------------------------------------------------
__fastcall TFormSHLtul::TFormSHLtul(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormSHLtul::FormResize(TObject *Sender)
{
	SG1->Width = ClientWidth - SG1->Left - 4 * Screen->PixelsPerInch/96;
	SG1->Height = ClientHeight - SG1->Top - 4 * Screen->PixelsPerInch/96;
}
//---------------------------------------------------------------------------
void __fastcall TFormSHLtul::FormShow(TObject *Sender)
{
	int nsrj = 0;
	SG1->ColCount = 4;
	SG1->ColWidths[0] = 80 * Screen->PixelsPerInch/96;
	SG1->ColWidths[1] = 80 * Screen->PixelsPerInch/96;
	SG1->ColWidths[2] = 50 * Screen->PixelsPerInch/96;
	SG1->ColWidths[3] = 40 * Screen->PixelsPerInch/96;
	SG1->Cells[0][0] = L"Sarja";
	SG1->Cells[1][0] = L"Sarja tiedostossa";
	SG1->Cells[2][0] = L"Matka";
	SG1->Cells[3][0] = L"Tapa";
	for (int srj = 0; srj < sarjaluku + nsarjayhd; srj++)
		if (TulosteForm->SarjaValittu(srj))
			nsrj++;
	SG1->RowCount = nsrj+1;
	nsrj = 0;
	for (int srj = 0; srj < sarjaluku + nsarjayhd; srj++) {
		if (TulosteForm->SarjaValittu(srj)) {
			wchar_t  st[50], *p;
			SG1->Cells[0][nsrj+1] = Sarjat[srj].sarjanimi;
			SG1->Cells[1][nsrj+1] = Sarjat[srj].sarjanimi;
			wcscpy(st, Sarjat[srj].matka[k_pv]);
			if (wcslen(st) > 1) {
				for (p = st+wcslen(st)-1; p > st && wcswcind(*p, L"pPvV") >= 0; p--);
				if (++p < st+wcslen(st)) {
					SG1->Cells[3][nsrj+1] = UnicodeString(p).UpperCase();
					}
				for (p = st+wcslen(st)-1; p > st && *p < L'0' || *p > L'9'; p--)
					*p = 0;
				if (wcsstr(st, L".") == NULL && wcsstr(st, L",") == NULL)
					wcscat(st, L".0");
				}
			if (Sarjat[srj].tapa[k_pv][0])
				SG1->Cells[3][nsrj+1] = Sarjat[srj].tapa[k_pv];
			SG1->Cells[2][nsrj+1] = st;
			nsrj++;
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSHLtul::Button2Click(TObject *Sender)
{
	int nsrj;

	wcsncpy(TulosteForm->SHLkilpailu, EdtKilp->Text.c_str(), sizeof(TulosteForm->SHLkilpailu)/2-1);
	nsrj = 0;
	for (int srj = 0; srj < sarjaluku + nsarjayhd; srj++) {
		if (TulosteForm->SarjaValittu(srj)) {
			wcsncpy(TulosteForm->SHLSarja[srj], SG1->Cells[1][nsrj+1].c_str(), sizeof(TulosteForm->SHLSarja[0])/2-1);
			wcsncpy(TulosteForm->SHLMatka[srj], SG1->Cells[2][nsrj+1].c_str(), sizeof(TulosteForm->SHLMatka[0])/2-1);
			wcsncpy(TulosteForm->SHLTyyli[srj], SG1->Cells[3][nsrj+1].c_str(), sizeof(TulosteForm->SHLTyyli[0])/2-1);
			nsrj++;
			}
		}
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormSHLtul::Button3Click(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFormSHLtul::RGtyylitClick(TObject *Sender)
{
	if (RGtyylit->ItemIndex > 0)
	for	(int r = 1; r < SG1->RowCount; r++)
		SG1->Cells[3][r] = L"PV"[RGtyylit->ItemIndex-1];
}
//---------------------------------------------------------------------------
void __fastcall TFormSHLtul::Button1Click(TObject *Sender)
{
	for (int r = 1; r < SG1->RowCount; r++) {
		wchar_t stsrj[4], st2[4], sp;
		int ika;
		wcscpy(stsrj, L"MYL");
		wcsncpy(st2, SG1->Cells[0][r].c_str()+SG1->Cells[0][r].Length()-2,2);
		st2[2] = 0;
		ika = _wtoi(st2);
		sp = SG1->Cells[0][r].c_str()[0];
		if (wcswcind(sp, L"NDWL") >= 0) {
			 stsrj[0] = L'N';
			 if (ika >= 35) {
				wmemcpy(stsrj+1, L"35", 2);
				}
			 if (ika >= 45) {
				wmemcpy(stsrj+1, L"45", 2);
				}
			 }
		  else {
			 if (ika >= 35) {
				wmemcpy(stsrj+1, L"35", 2);
				}
			 if (ika >= 45) {
				wmemcpy(stsrj+1, L"45", 2);
				}
			 if (ika >= 55) {
				wmemcpy(stsrj+1, L"55", 2);
				}
			 if (ika >= 65) {
				wmemcpy(stsrj+1, L"65", 2);
				}
			 }
		  if (ika == 20) {
			 wmemcpy(stsrj+1, L"20", 2);
			 }
		  if (ika == 17 || ika == 18) {
			 wmemcpy(stsrj+1, L"18", 2);
			 }
		SG1->Cells[1][r] = stsrj;
		}
}
//---------------------------------------------------------------------------
