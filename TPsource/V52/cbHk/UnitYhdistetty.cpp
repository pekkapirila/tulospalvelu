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

#include "UnitYhdistetty.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormYhdistetty *FormYhdistetty;

//---------------------------------------------------------------------------
__fastcall TFormYhdistetty::TFormYhdistetty(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
	kerroin = 15;
}
//---------------------------------------------------------------------------
void __fastcall TFormYhdistetty::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Yhdistetty");
}
//---------------------------------------------------------------------------
void __fastcall TFormYhdistetty::FormShow(TObject *Sender)
{
	CBSarjat->Clear();
	CBSarjat->Items->Add(L"Kaikki sarjat");
	for (int srj = 0; srj < sarjaluku; srj++)
		CBSarjat->Items->Add(Sarjat[srj].sarjanimi);
	CBSarjat->ItemIndex = 0;
	EdKerroin->Text = kerroin;
}
//---------------------------------------------------------------------------

void __fastcall TFormYhdistetty::Button1Click(TObject *Sender)
{
	int enssrj, viimsrj, makivaihe;
	int *d_pist;
	int d, n, maxpist;
	kilptietue kilp;

	makivaihe = RGLajiJarj->ItemIndex == 0 ? 0 : 1;
	if (CBSarjat->ItemIndex > 0) {
		enssrj = CBSarjat->ItemIndex - 1;
		viimsrj = enssrj;
		}
	else {
		enssrj = 0;
		viimsrj = sarjaluku - 1;
		}
	for (int srj = enssrj; srj <= viimsrj; srj++) {
		if (RGLajiJarj->ItemIndex == 0) {
			__int64 kk;
			kk = 60000000/kerroin;
			d_pist = new int[nilm[srj]];
			n = 0;
			maxpist = -9999999;
			for (int d = 1; d < nrec && n < nilm[srj]; d++) {
				kilp.GETREC(d);
				if (kilp.kilpstatus || kilp.Sarja(makivaihe) != srj)
					continue;
				d_pist[n] = d;
				if (maxpist < kilp.pv[makivaihe].pvpisteet[0])
					maxpist = kilp.pv[makivaihe].pvpisteet[0];
				n++;
				}
			for (int i = 0; i < n; i++) {
				__int64 ero;
				int tero;
				kilp.GETREC(d_pist[i]);
				ero = maxpist - kilp.pv[makivaihe].pvpisteet[0];
				if (CBMerkPois->Checked && kilp.pv[makivaihe].pvpisteet[0] == 0)
					kilp.set_tark(L'P', makivaihe+1);
				tero = (int)(((kk*ero/10) + 500000) / 1000000) * SEK;
				kilp.tall_tulos(0, tero + pvparam[makivaihe].pyor[1]);
				kilp.tallenna(d_pist[i], 0, 0, 0, 0);
				}
			delete[] d_pist;
			}
		else {
			int karki;
			d_pist = new int[nilm[srj]];
			n = 0;
			karki = 2000000000;
			for (int d = 1; d < nrec && n < nilm[srj]; d++) {
				int tls;
				kilp.GETREC(d);
				if (kilp.kilpstatus || kilp.Sarja(makivaihe-1) != srj)
					continue;
				d_pist[n] = d;
				if ((tls = kilp.ytulos(makivaihe-1, 2)) > 0 && tls < karki)
					karki = tls;
				n++;
				}
			for (int i = 0; i < n; i++) {
				int ero, tero;
				kilp.GETREC(d_pist[i]);
				tero = (kilp.ytulos(makivaihe-1, 2) - karki + SEK/2) / SEK;
				ero = (tero*kerroin + 3) / 6;
				kilp.pv[makivaihe-1].pvpisteet[0] = 1200 - ero;
				kilp.tallenna(d_pist[i], 0, 0, 0, 0);
				}
			delete[] d_pist;
			}
		}
	Application->MessageBoxW(L"Toimenpide suoritettu", L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------

void __fastcall TFormYhdistetty::EdKerroinExit(TObject *Sender)
{
	kerroin = _wtoi(EdKerroin->Text.c_str());
	EdKerroin->Text = kerroin;
}
//---------------------------------------------------------------------------

void __fastcall TFormYhdistetty::Button2Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

