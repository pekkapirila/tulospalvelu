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
#include "HkDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TVertAikaFrm *VertAikaFrm;
//---------------------------------------------------------------------------
__fastcall TVertAikaFrm::TVertAikaFrm(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TVertAikaFrm::OKBtnClick(TObject *Sender)
{
   analvertmax = EdMaxLkm->Text.ToInt();
   analvertosuus = EdMaxPros->Text.ToInt();
   TlsLstPaivita = TlsLuetVal->Checked;
   if (emitanal_fl)
   	   laskevertajat();
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TVertAikaFrm::PeruutaBtnClick(TObject *Sender)
{
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TVertAikaFrm::FormShow(TObject *Sender)
{
   EdMaxLkm->Text = UnicodeString(analvertmax);
   EdMaxPros->Text = UnicodeString(analvertosuus);
   TlsLuetVal->Checked = TlsLstPaivita;
   CBAnal->Enabled = emitfl;
   CBAnal->Checked = emitanal_fl;
   GroupBox1->Enabled = emitfl;
}
//---------------------------------------------------------------------------

void __fastcall TVertAikaFrm::CBAnalClick(TObject *Sender)
{
	emitanal_fl = CBAnal->Checked;
	if (emitanal_fl)
		uusiemitanalyysi();
}
//---------------------------------------------------------------------------

