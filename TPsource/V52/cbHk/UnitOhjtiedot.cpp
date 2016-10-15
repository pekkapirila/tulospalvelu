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

#include "UnitOhjtiedot.h"
#include "HkDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormOhjtiedot *FormOhjtiedot;
//---------------------------------------------------------------------------
__fastcall TFormOhjtiedot::TFormOhjtiedot(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	Label3->Caption = L"Versio "+UnicodeString(VersioNo());
	Label4->Caption = L"Build "+UnicodeString(Build());
	Memo1->Clear();
	Memo1->Lines->Add(L"Vaiheiden maksimiluku: "+UnicodeString(N_PV));
	Memo1->Lines->Add(L"Väliaikojen maksimiluku: "+UnicodeString(VALUKU));
	Memo1->Lines->Add(L"Suurin sallittu kilpailijanumero: "+UnicodeString(MAXKILPNO));
	Memo1->Lines->Add(L"Sarjojen maksimiluku: "+UnicodeString(MAXSARJALUKU));
	Memo1->Lines->Add(L"Sarjayhdistelmien maksimiluku: "+UnicodeString(MAXYHD));
	Memo1->Lines->Add(L"Tiedonsiirtoyhteyksien maksimiluku: "+UnicodeString(MAX_LAHPORTTI));
	Memo1->Lines->Add(L"Laiteyhteyksien maksimiluku: "+UnicodeString(NREGNLY));
	Memo1->Lines->Add(L"Ajanottojonojen maksimiluku: "+UnicodeString(MAXJONO));
	Memo1->Lines->Add(L"Yhden radan rastien maksimiluku: "+UnicodeString(MAXNRASTI));
	Memo1->Lines->Add(L"Ratojen maksimiluku: "+UnicodeString(MAXNRATA));
	Memo1->Lines->Add(L"Yhden radan variaatioiden maksimiluku: "+UnicodeString(MAXVARIATION));
	Memo1->Lines->Add(L"Rastipisteide maksimiluku: "+UnicodeString(MAXNPISTE));
	Memo1->Lines->Add(L"Ratatunnuksen pituus: "+UnicodeString(HAJONTA));
	Memo1->Lines->Add(L"Sukunimikentän pituus: "+UnicodeString(LSNIMI));
	Memo1->Lines->Add(L"Etunimikentän pituus: "+UnicodeString(LENIMI));
	Memo1->Lines->Add(L"Seuranimikentän pituus: "+UnicodeString(LSEURA));
	Memo1->Lines->Add(L"Seuralyhennekentän pituus: "+UnicodeString(LLYH));
	Memo1->Lines->Add(L"Joukkunimikentän pituus: "+UnicodeString(LJK));
	Memo1->Lines->Add(L"Yhdistysnimikentän pituus: "+UnicodeString(LYHD));
	Memo1->Lines->Add(L"Sarjakoodin pituus: "+UnicodeString(LSARJA));
	Memo1->Lines->Add(L"Pitkän sarjanimen pituus: "+UnicodeString(LPSARJA));
	Memo1->Lines->Add(L"Sakkopisteiden maksimiluku: "+UnicodeString(LSAKKO));
}
//---------------------------------------------------------------------------
void __fastcall TFormOhjtiedot::FormClose(TObject *Sender, TCloseAction &Action)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TFormOhjtiedot::FormResize(TObject *Sender)
{
	Memo1->Height = ClientHeight - Memo1->Top - 4;
}
//---------------------------------------------------------------------------

