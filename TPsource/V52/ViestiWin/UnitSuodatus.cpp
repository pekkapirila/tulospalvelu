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

#include "UnitSuodatus.h"
#include "UnitJoukkueet.h"
#include "VDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSuodatus *FormSuodatus;
//---------------------------------------------------------------------------
__fastcall TFormSuodatus::TFormSuodatus(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormSuodatus::BtnKaytaClick(TObject *Sender)
{
//	nMukana = 0;
	FormJoukkueet->naytaTiedot();
	EdtValittu->Text = FormJoukkueet->NKilp;
}
//---------------------------------------------------------------------------
void __fastcall TFormSuodatus::BtnSuljeClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormSuodatus::Button1Click(TObject *Sender)
{
	RGPeruste->ItemIndex = 0;
	CBSarja->ItemIndex = 0;
	CBSeura->ItemIndex = 0;
	CBTark->ItemIndex = 0;
	BtnKaytaClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormSuodatus::EdtTietoChange(TObject *Sender)
{
	if (RGPeruste->ItemIndex > 0 && RGPeruste->ItemIndex <= 4)
		BtnKaytaClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormSuodatus::CBSarjaChange(TObject *Sender)
{
	BtnKaytaClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormSuodatus::CBSeuraChange(TObject *Sender)
{
	BtnKaytaClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormSuodatus::CBTarkChange(TObject *Sender)
{
	BtnKaytaClick(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TFormSuodatus::EdtTietoKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == KB_CR) {
		Key = 0;
		BtnKaytaClick(Sender);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSuodatus::FormCreate(TObject *Sender)
{
	int nsra = 0;
	wchar_t (*seuralista)[LSEURA+5];

	CBSarja->Clear();
	CBSarja->Items->Add(L"Ei sarjasuodatusta");
	for (int srj = 0; srj < sarjaluku; srj++)
		CBSarja->Items->Add(UnicodeString(Sarjat[srj].sarjanimi));
	CBSarja->ItemIndex = 0;

	CBOsuus->Clear();
	for (int os = 0; os < kilpparam.osuusluku; os++)
		CBOsuus->Items->Add(UnicodeString(os+1)+L". osuus");
	CBOsuus->ItemIndex = 0;

	seuralista = new wchar_t[nilmt+1][LSEURA+5];
//	HaeSeuraLista(0, k_pv+1, true, seuralista, &nsra, nilmt+10);
	CBSeura->Clear();
	CBSeura->Items->Add(L"Ei seurasuodatusta");
	for (int r = 0; r < nsra; r++) {
		CBSeura->Items->Add(seuralista[r]);
		}
	delete[] seuralista;
	CBSeura->ItemIndex = 0;
}
//---------------------------------------------------------------------------

