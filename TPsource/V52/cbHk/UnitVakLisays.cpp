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

#include "UnitVakLisays.h"
#include "HkDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
extern TFormVakLisays *FormVakLisays;
//---------------------------------------------------------------------------
__fastcall TFormVakLisays::TFormVakLisays(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormVakLisays::FormShow(TObject *Sender)
{
	CBSarja->Clear();
	for (int srj = 0; srj < sarjaluku; srj++) {
		CBSarja->Items->Add(UnicodeString(Sarjat[srj].sarjanimi));
		}
	EditLuotu->Text = UnicodeString(0);
}
//---------------------------------------------------------------------------
void __fastcall TFormVakLisays::Button1Click(TObject *Sender)
{
	int srj, kno, d, l, maxKno = MAXKILPNO, n = 0;
	kilptietue kilp;
	wchar_t seura[LSEURA+1] = L"", suku[LSNIMI+1], *p;

	srj = CBSarja->ItemIndex;
	kno = _wtoi(EditAlku->Text.c_str());
	p = kilp.seuratieto(&l);
	wcsncpy(seura, EditSeura->Text.c_str(), l);
	wcsncpy(suku, EditNimi->Text.c_str(), kilpparam.lsnimi);
	if (CBAukot->Checked)
		maxKno = kno + _wtoi(EditLkm->Text.c_str());

	EnterCriticalSection(&tall_CriticalSection);
	for (int i = 0; i < _wtoi(EditLkm->Text.c_str()); i++) {
		kno = haeseurvapaa(kno);
		if (kno > maxKno)
			break;
		kilp.nollaa();
		kilp.setIdAll(kno);
		kilp.setSarjaAll(srj);
		wcscpy(p, seura);
		wcscpy(kilp.sukunimi, suku);
		_itow(kno, kilp.etunimi, 10);
		for (int Pv = 0; Pv < kilpparam.n_pv; Pv++) {
			kilp.set_tark(L"VVAE"[RGEil->ItemIndex+1], Pv);
			kilp.pv[Pv].tlahto = wstrtoaika_vap(EditLahto->Text.c_str(), t0);
			kilp.pv[Pv].va[0].vatulos = kilp.pv[Pv].tlahto;
			}
		kilp.addtall(&d, 0);
		n++;
	}
	LeaveCriticalSection(&tall_CriticalSection);
	EditLuotu->Text = n;
}
//---------------------------------------------------------------------------
void __fastcall TFormVakLisays::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Ilmoittautumiset");
}
//---------------------------------------------------------------------------

void __fastcall TFormVakLisays::Button4Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

