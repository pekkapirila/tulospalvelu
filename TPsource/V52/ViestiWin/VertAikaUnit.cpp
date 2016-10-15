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

#include "VertAikaUnit.h"
#include "VDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TVertAikaFrm *VertAikaFrm;
//---------------------------------------------------------------------------
__fastcall TVertAikaFrm::TVertAikaFrm(TComponent* Owner)
   : TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
}
//---------------------------------------------------------------------------


void __fastcall TVertAikaFrm::OKBtnClick(TObject *Sender)
{
   analvertmax = EdMaxLkm->Text.ToInt();
   analvertosuus = EdMaxPros->Text.ToInt();
   SeurLstPaivita = SeurLstVal->Checked;
   SeurGrPaivita = SeurGrVal->Checked;
   TlsLstPaivita = TlsLuetVal->Checked;
//   NaytaNimet = NaytaNimetVal->Checked;
   laskevertajat();
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TVertAikaFrm::PeruutaBtnClick(TObject *Sender)
{
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TVertAikaFrm::Luekorostustiedot1Click(TObject *Sender)
{
	lueKorRajat();

}
//---------------------------------------------------------------------------

void __fastcall TVertAikaFrm::Kuuluttajarajoitukset1Click(TObject *Sender)
{
	Kuuluttajarajoitukset1->Checked = !Kuuluttajarajoitukset1->Checked;
	kuuluttaja = Kuuluttajarajoitukset1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TVertAikaFrm::FormShow(TObject *Sender)
{
   EdMaxLkm->Text = AnsiString(analvertmax);
   EdMaxPros->Text = AnsiString(analvertosuus);
   SeurLstVal->Checked = SeurLstPaivita;
   SeurGrVal->Checked = SeurGrPaivita;
   TlsLuetVal->Checked = TlsLstPaivita;
	Kuuluttajarajoitukset1->Checked = kuuluttaja;
}
//---------------------------------------------------------------------------

void __fastcall TVertAikaFrm::Maksimikesto1Click(TObject *Sender)
{
	wchar_t ch;
	inputint_prompt(&analmaxosuuskesto, L"Analysoitavan osuusden maksimikesto (sek)", &ch, this);
}
//---------------------------------------------------------------------------

