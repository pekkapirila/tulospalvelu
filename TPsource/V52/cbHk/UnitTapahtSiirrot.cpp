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

#include "hkDeclare.h"
#include "UnitTapahtSiirrot.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

extern int vakbiblisa;

TFormTapahtSiirrot *FormTapahtSiirrot;
//---------------------------------------------------------------------------
__fastcall TFormTapahtSiirrot::TFormTapahtSiirrot(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	EdtVakBibLisa->Text = vakbiblisa;
}
//---------------------------------------------------------------------------
void __fastcall TFormTapahtSiirrot::Button1Click(TObject *Sender)
{
	char varatut[MAXKILPNO+1];
	int vaihe, alku, loppu, seur;
	kilptietue kilp;

	memset(varatut, 0, sizeof(varatut));
	vaihe = CBVaihe->ItemIndex;
	seur = _wtoi(EdtAlku->Text.c_str());
	alku = _wtoi(EdtAlaraja->Text.c_str());
	loppu = _wtoi(EdtYlaraja->Text.c_str());
	for (int d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (kilp.kilpstatus != 0)
			continue;
		varatut[kilp.id(vaihe)] = 1;
		}
	for (int d = 1; d < nrec; d++) {
		int edId;
		kilp.GETREC(d);
		if (kilp.kilpstatus != 0)
			continue;
		if ((edId = kilp.id(vaihe)) >= alku && edId <= loppu && wcswcind(kilp.tark(vaihe), L"EVP") < 0) {
			while (seur <= maxkilpno && varatut[seur])
				seur++;
			if (seur > maxkilpno)
				break;
			if (CBNumKaikki->Checked)
				kilp.setIdAll(seur);
			else
				kilp.setId(seur, vaihe);
			varatut[seur] = 1;
			varatut[edId] = 0;
			kilp.setSarja(kilp.Sarja(vaihe), -1);
			if (CBTulSarjat->Checked)
				for (int i_pv = vaihe+1; i_pv < kilpparam.n_pv_akt; i_pv++)
					kilp.setSarja(kilp.Sarja(vaihe), i_pv);
			kilp.tallenna(d, 0, 0, 0, 0);
			}
		}
	if (CBPalautaVakantit) {
		for (int d = 1; d < nrec; d++) {
			kilp.GETREC(d);
			if (kilp.kilpstatus != 0)
				continue;
			if (kilp.id(vaihe) >= alku+vakbiblisa && kilp.id(vaihe) <= loppu+vakbiblisa &&
				wcswcind(kilp.tark(vaihe), L"EVP") >= 0 && !varatut[kilp.id(vaihe)-vakbiblisa]) {
				if (CBNumKaikki->Checked)
					kilp.setIdAll(kilp.id(vaihe)-vakbiblisa);
				else
					kilp.setId(kilp.id(vaihe)-vakbiblisa, vaihe);
				kilp.tallenna(d, 0, 0, 0, 0);
				}
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormTapahtSiirrot::EdtVakBibLisaChange(TObject *Sender)
{
	vakbiblisa = _wtoi(EdtVakBibLisa->Text.c_str());
	EdtVakBibLisa->Text = vakbiblisa;
}
//---------------------------------------------------------------------------

void __fastcall TFormTapahtSiirrot::Button4Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

