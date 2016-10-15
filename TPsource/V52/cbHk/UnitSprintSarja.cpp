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

#include "UnitSprintSarja.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSprintSarja *FormSprintSarja;
/*
class sprintsarja {
public:
	int vaihelkm;
	int eralkm[N_PV-1];
	int erakoko[N_PV-1];
	int perussrj;
	int sarjat[N_PV-1][6];

	__fastcall sprintsarja(void);
	__fastcall ~sprintsarja(void);
	void nollaa(void);
	void operator=(sprintsarja&);
	bool operator==(sprintsarja&);
   };
*/
//---------------------------------------------------------------------------
__fastcall TFormSprintSarja::TFormSprintSarja(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
	CBS[1] = CBS11;
	CBS[2] = CBS12;
	CBS[3] = CBS13;
	CBS[4] = CBS14;
	CBS[5] = CBS15;
	CBS[6] = CBS16;
	CBS[7] = CBS21;
	CBS[8] = CBS22;
	CBS[9] = CBS31;
	CBS[10] = CBS32;
}
//---------------------------------------------------------------------------
void __fastcall TFormSprintSarja::FormShow(TObject *Sender)
{
	for	(int i = 1; i < 11; i++) {
		CBS[i]->Clear();
		for (int srj = 0; srj < sarjaluku; srj++)
			CBS[i]->Items->Add(Sarjat[srj].sarjanimi);
		if (i <= sprSrj.eralkm[0])
			CBS[i]->ItemIndex = sprSrj.sarjat[0][i-1];
		else if (i >= 6 && i <= 6+sprSrj.eralkm[1])
			CBS[i]->ItemIndex = sprSrj.sarjat[1][i-7];
		else if (i >= 8 && i <= 8+sprSrj.eralkm[2])
			CBS[i]->ItemIndex = sprSrj.sarjat[2][i-9];
		else {
			CBS[i]->ItemIndex = -1;
			CBS[i]->Text = L"";
			}
		}
	EdtPerusSarja->Text = Sarjat[pSarja].sarjanimi;
	EdtVaiheLkm->Text = sprSrj.vaihelkm;
	EdtEraLkm1->Text = sprSrj.eralkm[0];
	EdtEraLkm2->Text = sprSrj.eralkm[1];
	EdtEraLkm3->Text = sprSrj.eralkm[2];
	EdtEraKoko1->Text = sprSrj.erakoko[0];
	EdtEraKoko2->Text = sprSrj.erakoko[1];
	EdtEraKoko3->Text = sprSrj.erakoko[2];
	EdtEraLkm2->Enabled = sprSrj.vaihelkm != 2;
	EdtEraKoko2->Enabled = EdtEraLkm2->Enabled;
	EdtEraLkm3->Enabled = sprSrj.vaihelkm != 2 && sprSrj.vaihelkm != 3;
	EdtEraKoko3->Enabled = EdtEraLkm3->Enabled;
	if (!EdtEraLkm2->Enabled)
		EdtEraLkm2->Text = 0;
	if (!EdtEraLkm3->Enabled)
		EdtEraLkm3->Text = 0;
}
//---------------------------------------------------------------------------
void __fastcall TFormSprintSarja::BtnPeruutaClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TFormSprintSarja::BtnTallennaClick(TObject *Sender)
{
	for	(int i = 1; i < 11; i++) {
		if (i <= 6)
			sprSrj.sarjat[0][i-1] = CBS[i]->ItemIndex;
		else if (i <= 8)
			sprSrj.sarjat[1][i-7] = CBS[i]->ItemIndex;
		else
			sprSrj.sarjat[2][i-9] = CBS[i]->ItemIndex;
		}
	sprSrj.vaihelkm = _wtoi(EdtVaiheLkm->Text.c_str());
	sprSrj.eralkm[0] = _wtoi(EdtEraLkm1->Text.c_str());
	sprSrj.eralkm[1] = _wtoi(EdtEraLkm2->Text.c_str());
	sprSrj.eralkm[2] = _wtoi(EdtEraLkm3->Text.c_str());
	sprSrj.erakoko[0] = _wtoi(EdtEraKoko1->Text.c_str());
	sprSrj.erakoko[1] = _wtoi(EdtEraKoko2->Text.c_str());
	sprSrj.erakoko[2] = _wtoi(EdtEraKoko3->Text.c_str());
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TFormSprintSarja::EdtVaiheLkmExit(TObject *Sender)
{
	if (sprSrj.vaihelkm == 0) {
		sprSrj.vaihelkm = _wtoi(EdtVaiheLkm->Text.c_str());
		sprSrj.eralkm[0] = 5;
		sprSrj.eralkm[1] = 2;
		sprSrj.eralkm[2] = 1;
		sprSrj.erakoko[0] = 6;
		sprSrj.erakoko[1] = 6;
		sprSrj.erakoko[2] = 6;
		}
	sprSrj.vaihelkm = _wtoi(EdtVaiheLkm->Text.c_str());
	if (sprSrj.vaihelkm >= 3 && sprSrj.eralkm[1] < sprSrj.vaihelkm - 2) {
		sprSrj.eralkm[1] = sprSrj.vaihelkm > 3 ? 2 : 1;
		sprSrj.erakoko[1] = 6;
		}
	if (sprSrj.vaihelkm == 4 && sprSrj.eralkm[2] == 0) {
		sprSrj.eralkm[2] = 1;
		sprSrj.erakoko[2] = 6;
		}
	FormShow(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormSprintSarja::CBS11Change(TObject *Sender)
{
	for (int i = 1; i < sprSrj.eralkm[0]; i++) {
		CBS[i+1]->ItemIndex = CBS[1]->ItemIndex + i;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSprintSarja::CBS21Change(TObject *Sender)
{
	for (int i = 1; i < sprSrj.eralkm[1]; i++) {
		CBS[i+7]->ItemIndex = CBS[7]->ItemIndex + i;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSprintSarja::CBS31Change(TObject *Sender)
{
	for (int i = 1; i < sprSrj.eralkm[2]; i++) {
		CBS[i+9]->ItemIndex = CBS[9]->ItemIndex + i;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSprintSarja::EdtEraLkm1Change(TObject *Sender)
{
	sprSrj.eralkm[0] = _wtoi(EdtEraLkm1->Text.c_str());
	for (int i = 0; i < 6; i++) {
		CBS[i+1]->Visible = i < sprSrj.eralkm[0];
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSprintSarja::EdtEraLkm2Change(TObject *Sender)
{
	sprSrj.eralkm[1] = _wtoi(EdtEraLkm2->Text.c_str());
	for (int i = 6; i < 8; i++) {
		CBS[i+1]->Visible = i < sprSrj.eralkm[1]+6;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSprintSarja::EdtEraLkm3Change(TObject *Sender)
{
	sprSrj.eralkm[2] = _wtoi(EdtEraLkm3->Text.c_str());
	for (int i = 8; i < 10; i++) {
		CBS[i+1]->Visible = i < sprSrj.eralkm[2]+8;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSprintSarja::Button1Click(TObject *Sender)
{
	int nVaihe = 1;

	if (sprSrj.eralkm[2] > 0)
		nVaihe = 4;
	else if (sprSrj.eralkm[1] > 0)
		nVaihe = 3;
	else if (sprSrj.eralkm[0] > 0)
		nVaihe = 2;
	for (int vaihe = 1; vaihe < nVaihe; vaihe++) {
		for	(int i = 0; i < sprSrj.eralkm[vaihe-1]; i++) {
			wchar_t snimi[60];
			int srj;
			swprintf(snimi, L"%s%c%c", Sarjat[pSarja].sarjanimi,
				L"QSF"[vaihe+3-nVaihe], (vaihe == nVaihe-1 ? L'A'+i : L'1'+i));
			if ((srj = haesarja_w(snimi, false)) < 0) {
				if (sarjaluku >= MAXSARJALUKU)
					break;
				Sarjat[sarjaluku].nollaa();
				wcscpy(Sarjat[sarjaluku].sarjanimi, snimi);
				Sarjat[sarjaluku].enspv = vaihe;
				Sarjat[sarjaluku].viimpv = vaihe;
				Sarjat[sarjaluku].luonne[vaihe] = 0;
				srj = sarjaluku;
				sarjaluku++;
				for (int j = 1; j < 11; j++)
					CBS[j]->Items->Add(Sarjat[srj].sarjanimi);
				}
			CBS[i+(vaihe > 1 ? 3 + 2*vaihe : 1)]->ItemIndex = srj;
			if (kilpparam.bibvaihtelee) {
				if (vaihe == 1)
					Sarjat[srj].bibalku[vaihe] = Sarjat[pSarja].bibalku[0];
				else
					Sarjat[srj].bibalku[vaihe] = 0;
				}
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSprintSarja::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Sprinthiihdon sarjat");
}
//---------------------------------------------------------------------------

