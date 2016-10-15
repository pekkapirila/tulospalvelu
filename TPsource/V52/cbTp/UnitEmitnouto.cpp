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

#include "UnitEmitnouto.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEmitNouto *FormEmitNouto;
//---------------------------------------------------------------------------
__fastcall TFormEmitNouto::TFormEmitNouto(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	EdtLaite[0] = EdtLaite1;
	EdtLaite[1] = EdtLaite2;
	EdtLaite[2] = EdtLaite3;
	EdtLaite[3] = EdtLaite4;
}
//---------------------------------------------------------------------------
void TFormEmitNouto::naytaStatus(void)
{
	inNaytaStatus = true;
	HakuVali = 50 * eTParam.eThakuvali;
	EdtHakuVali->Text = HakuVali;
	EdtServer->Text = eTParam.eThost;
	EdtPortti->Text = eTParam.eTport;
	EdtLaiteNo1->Text = (eTParam.eTrno + 1);
	CBAvoinna1->Checked = (eTParam.eThaku != 0);
	for (int dd = 0; dd < DEVLKM; dd++) {
		devices[dd] = eTParam.eTserial[dd];
		EdtLaite[dd]->Text = devices[dd];
		}
	if (eTParam.eTdate[0] && eTParam.eTtime[0])
		EdtAika->Text = UnicodeString(eTParam.eTdate)+L" "+eTParam.eTtime;
	else {
		EdtAika->Text = UnicodeString(ISOdatetime(3,1)).SubString(1, 10)+
			L" "+UnicodeString(ISOdatetime(3,1)).SubString(12, 8);
		}
	wcscpy(aikaraja, EdtAika->Text.c_str());
	inNaytaStatus = false;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitNouto::FormShow(TObject *Sender)
{
	naytaStatus();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitNouto::BtnUusihakuClick(TObject *Sender)
{
	CBAvoinna1->Checked = true;
	for (int no = 0; no < DEVLKM; no++)
		eTParam.eTserial[no] = devices[no];
	wcsncpy(eTParam.eTdate, aikaraja, 10);
	wcsncpy(eTParam.eTtime, aikaraja+11, 8);
	eTParam.eTtime[2] = eTParam.eTtime[5] = L':';
	eTParam.eThakuvali = _wtoi(EdtHakuVali->Text.c_str()) / 50;
	eTParam.eThaku = -1;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitNouto::EdtLaiteChange(TObject *Sender)
{
	int k = 0;

	if (inNaytaStatus)
		return;
	for (int no = 0; no < DEVLKM; no++)
		devices[no] = _wtoi(EdtLaite[no]->Text.c_str());
	for (int i = 0; i < DEVLKM; ) {
		if (devices[i] != 0) {
			k++;
			i++;
			}
		else {
			int dd = 0;
			for (int j = i+1; j < DEVLKM; j++) {
				devices[j-1] = devices[j];
				dd += devices[j];
				}
			devices[DEVLKM-1] = 0;
			if (dd == 0)
				break;
			}
		}
	for (int i = 0; i < DEVLKM; i++) {
		EdtLaite[i]->Text = devices[i];
		}
	devCount = k;
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitNouto::BtnNytClick(TObject *Sender)
{
	EdtAika->Text = UnicodeString(ISOdatetime(3,1)).SubString(1, 10)+
		L" "+UnicodeString(ISOdatetime(3,1)).SubString(12, 8);
	wcscpy(aikaraja, EdtAika->Text.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitNouto::EdtAikaChange(TObject *Sender)
{
	wcscpy(aikaraja, EdtAika->Text.c_str());
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitNouto::BtnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitNouto::CBAvoinna1Click(TObject *Sender)
{
	if (inNaytaStatus)
		return;
	if (!CBAvoinna1->Checked)
		eTParam.eThaku = 0;
	else {
		for (int no = 0; no < DEVLKM; no++)
			eTParam.eTserial[no] = devices[no];
		wcsncpy(eTParam.eTdate, aikaraja, 10);
		wcsncpy(eTParam.eTtime, aikaraja+11, 8);
		eTParam.eTtime[2] = eTParam.eTtime[5] = L':';
		eTParam.eThakuvali = _wtoi(EdtHakuVali->Text.c_str()) / 50;
		eTParam.eThaku = 1;
		}
}
//---------------------------------------------------------------------------
