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

#include "UnitEmit_VA.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEmit_VA *FormEmit_VA;
//---------------------------------------------------------------------------
__fastcall TFormEmit_VA::TFormEmit_VA(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit_VA::FormCreate(TObject *Sender)
{
	SG1->RowCount = kilpparam.valuku+2;
	SG1->Cells[0][0] = L"Alku";
	SG1->Cells[1][0] = L"Loppu";
	SG1->Cells[2][0] = L"Piste";
	SG1->Cells[3][0] = L"Pohjapiste";
	for (int r = 1; r < SG1->RowCount; r++) {
		for (int c = 0; c < 4; c++)
			SG1->Cells[c][r] = L"";
		}
	CBSarja->Clear();
	CBSarja->Items->Add(L"Valitse sarja");
	for (int i = 0; i < sarjaluku; i++)
		CBSarja->Items->Add(Sarjat[i].sarjanimi);
	CBSarja->ItemIndex = 0;
	CBVaihe->Clear();
	for (int i = 0; i < kilpparam.n_pv_akt; i++)
		CBVaihe->Items->Add(i+1);
	CBVaihe->ItemIndex = k_pv;
	if (ToimintaTila != 2 || emitfl < 1) {
		BtnLaskeSrj->Enabled = false;
		BtnLaskeKaikki->Enabled = false;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit_VA::CBSarjaChange(TObject *Sender)
{
	if ((Sarja = CBSarja->ItemIndex - 1)  < 0)
		return;
	Vaihe = CBVaihe->ItemIndex;
	CBAuto->Checked = Sarjat[Sarja].laskemtb[Vaihe];
	SG1->RowCount = Sarjat[Sarja].valuku[Vaihe]+2;
	for	(int r = 0; r <= Sarjat[Sarja].valuku[Vaihe]; r++) {
		SG1->Cells[0][r+1] = Sarjat[Sarja].mtb[Vaihe][r].alkukdi;
		SG1->Cells[1][r+1] = Sarjat[Sarja].mtb[Vaihe][r].loppukdi;
		SG1->Cells[2][r+1] = Sarjat[Sarja].mtb[Vaihe][r].piste;
		SG1->Cells[3][r+1] = Sarjat[Sarja].mtb[Vaihe][r].pohja;
		}
}
//---------------------------------------------------------------------------
void TFormEmit_VA::TallSarja(int Srj)
{
	int rr = 0;
	if (Srj < 0)
		return;
	Vaihe = CBVaihe->ItemIndex;
	Sarjat[Srj].laskemtb[Vaihe] = CBAuto->Checked;
	for	(int r = 0; r <= Sarjat[Srj].valuku[Vaihe]; r++) {
		rr++;
		for (; rr < SG1->RowCount && _wtoi(SG1->Cells[0][rr].c_str())+_wtoi(SG1->Cells[1][rr].c_str()) == 0; rr++) ;
		if (rr >= SG1->RowCount) {
			Sarjat[Srj].mtb[Vaihe][r].alkukdi = 0;
			Sarjat[Srj].mtb[Vaihe][r].loppukdi = 0;
			Sarjat[Srj].mtb[Vaihe][r].piste = 0;
			Sarjat[Srj].mtb[Vaihe][r].pohja = 0;
			continue;
			}
		Sarjat[Srj].mtb[Vaihe][r].alkukdi = _wtoi(SG1->Cells[0][rr].c_str());
		Sarjat[Srj].mtb[Vaihe][r].loppukdi = _wtoi(SG1->Cells[1][rr].c_str());
		Sarjat[Srj].mtb[Vaihe][r].piste = _wtoi(SG1->Cells[2][rr].c_str());
		Sarjat[Srj].mtb[Vaihe][r].pohja = _wtoi(SG1->Cells[3][rr].c_str());
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit_VA::BtnTallSarjaClick(TObject *Sender)
{
	TallSarja(Sarja);
	CBSarjaChange(Sender);
	kirjEventXml(sarjaxmlfname, true, false, false);
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit_VA::BtnTallKaikkiClick(TObject *Sender)
{
	for (int Srj = 0; Srj < sarjaluku; Srj++)
		TallSarja(Srj);
	CBSarjaChange(Sender);
	kirjEventXml(sarjaxmlfname, true, false, false);
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit_VA::FormResize(TObject *Sender)
{
	SG1->Height = ClientHeight - SG1->Top - 4 * Screen->PixelsPerInch / 96;
	BtnSulje->Top = SG1->Top + SG1->Height - BtnSulje->Height;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit_VA::BtnSuljeClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit_VA::BtnLaskeSrjClick(TObject *Sender)
{
	kilptietue kilp;

	for (int d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (!kilp.kilpstatus && kilp.Sarja() == Sarja)
			laskeEm_VA(d, CBSail->Checked);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit_VA::BtnLaskeKaikkiClick(TObject *Sender)
{
	for (int d = 1; d < nrec; d++)
		laskeEm_VA(d, CBSail->Checked);
}
//---------------------------------------------------------------------------
void __fastcall TFormEmit_VA::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Ajat leimoista");
}
//---------------------------------------------------------------------------

