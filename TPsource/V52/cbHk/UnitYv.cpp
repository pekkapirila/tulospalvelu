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
#include <Clipbrd.hpp>
#pragma hdrstop

#include <iostream>
#include <fstream.h>
#include "UnitYv.h"
#include "WinHk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormYhtveto *FormYhtveto;
//---------------------------------------------------------------------------
__fastcall TFormYhtveto::TFormYhtveto(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormYhtveto::YvGridClick(TObject *Sender)
{
   LaadiYhtveto();
}
//---------------------------------------------------------------------------
void __fastcall TFormYhtveto::LaadiYhtveto(void)
   {
   int r = 1;

   YvGrid->RowCount = 1;
   YvGrid->ColCount = 8;
   YvGrid->Cells[0][0] = UnicodeString(L"Rivi");
   YvGrid->Cells[1][0] = UnicodeString(L"R-väli");
   YvGrid->Cells[2][0] = UnicodeString(L"Sarja");
   YvGrid->Cells[3][0] = UnicodeString(L"Lukum.");
   YvGrid->Cells[4][0] = UnicodeString(L"Vert.aika");
   YvGrid->Cells[5][0] = UnicodeString(L"Keskim.");
   YvGrid->Cells[6][0] = UnicodeString(L"Paras");
   YvGrid->Cells[7][0] = UnicodeString(L"Kilpailija");
   for (int rv = 0; rv < rvluku; rv++) {
      for (int srj = 0; srj < sarjaluku; srj++) {
         if (rvalit[rv].lkm[srj] > 0) {
            YvGrid->RowCount++;
            YvGrid->Cells[0][r] = UnicodeString(r);
            YvGrid->Cells[1][r] = UnicodeString(rvalit[rv].kdi);
            YvGrid->Cells[2][r] = UnicodeString(Sarjat[srj].sarjanimi);
            YvGrid->Cells[3][r] = UnicodeString(rvalit[rv].lkm[srj]);
			YvGrid->Cells[4][r] = UnicodeString(rvalit[rv].vertaika[srj]);
            YvGrid->Cells[5][r] =
               UnicodeString(rvalit[rv].sum[srj]/rvalit[rv].lkm[srj]);
            YvGrid->Cells[6][r] = UnicodeString(rvalit[rv].ajat[srj][0]);
            YvGrid->Cells[7][r] = UnicodeString(rvalit[rv].kilpno[srj][0]);
            r++;
            }
         }
      }

   }

void __fastcall TFormYhtveto::BtnSuljeClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormYhtveto::BtnPaivitaClick(TObject *Sender)
{
   LaadiYhtveto();
}
//---------------------------------------------------------------------------

void __fastcall TFormYhtveto::FormActivate(TObject *Sender)
{
   LaadiYhtveto();
}
//---------------------------------------------------------------------------

void __fastcall TFormYhtveto::Kirjoitatiedostoon1Click(TObject *Sender)
{
   if (SaveDialog1->Execute()) {
	  ofstream  outfile(SaveDialog1->FileName.c_str());
	  outfile << (L"R-väli")
		 << L"\t" << (L"Sarja")
		 << L"\t" << (L"Lukum.")
		 << L"\t" << (L"Vert.aika")
		 << L"\t" << (L"Keskim.")
		 << L"\t" << (L"Paras")
		 << L"\t" << (L"Kilpailija") << L"\n";
	  for (int rv = 0; rv < rvluku; rv++) {
		 for (int srj = 0; srj < sarjaluku; srj++) {
			if (rvalit[rv].lkm[srj] > 0) {
			   int n = rvalit[rv].lkm[srj];
			   outfile << (rvalit[rv].kdi)
				  << L"\t" << (Sarjat[srj].sarjanimi)
				  << L"\t" << (n)
				  << L"\t" << (rvalit[rv].vertaika[srj])
				  << L"\t" << (rvalit[rv].sum[srj]/rvalit[rv].lkm[srj])
				  << L"\t" << (rvalit[rv].ajat[srj][0])
				  << L"\t" << (rvalit[rv].kilpno[srj][0])
				  << L"\t" << (rvalit[rv].ajat[srj][(n+10)/20])
				  << L"\t" << (rvalit[rv].ajat[srj][(n+5)/10])
				  << L"\t" << (rvalit[rv].ajat[srj][(n+2)/4])
				  << L"\t" << (rvalit[rv].ajat[srj][(n+1)/2])
				  << L"\t" << (rvalit[rv].ajat[srj][(n*3+2)/4])
				  << L"\t" << (rvalit[rv].ajat[srj][(n*9+5)/10])
				  << L"\t" << (rvalit[rv].ajat[srj][(n*19+10)/20])
				  << L"\n";
			   }
			}
		 }
	  outfile.close();
	  }
	SetCurrentDir(FormMain->CurrentDir);
}
//---------------------------------------------------------------------------

void __fastcall TFormYhtveto::Suljekaavake1Click(TObject *Sender)
{
	BtnSuljeClick(Sender);
}
//---------------------------------------------------------------------------

