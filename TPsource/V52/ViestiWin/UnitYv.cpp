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
#include "VDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormYhtveto *FormYhtveto;
void uusiemitanalyysi(bool uusi);

//---------------------------------------------------------------------------
__fastcall TFormYhtveto::TFormYhtveto(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
}

#ifdef EMIT

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
   YvGrid->ColCount = 9;
   YvGrid->Cells[0][0] = AnsiString("Rivi");
   YvGrid->Cells[1][0] = AnsiString("R-väli");
   YvGrid->Cells[2][0] = AnsiString("Sarja");
   YvGrid->Cells[3][0] = AnsiString("Osuus");
   YvGrid->Cells[4][0] = AnsiString("Lukum.");
   YvGrid->Cells[5][0] = AnsiString("Vert.aika");
   YvGrid->Cells[6][0] = AnsiString("Keskim.");
   YvGrid->Cells[7][0] = AnsiString("Paras");
   YvGrid->Cells[8][0] = AnsiString("Joukkue");
   for (int rv = 0; rv < rvluku; rv++) {
      for (int srj = 0; srj < sarjaluku; srj++) {
		 for (int os = 0; os < Sarjat[srj].ntosuus; os++) {
			if (rvalit[rv].lkm[srj][os] > 0) {
			   YvGrid->RowCount++;
			   YvGrid->Cells[0][r] = AnsiString(r);
			   YvGrid->Cells[1][r] = AnsiString(rvalit[rv].kdi);
					YvGrid->Cells[2][r] = AnsiString(Sarjat[srj].sarjanimi);
			   YvGrid->Cells[3][r] = AnsiString(os+1);
			   YvGrid->Cells[4][r] = AnsiString(rvalit[rv].lkm[srj][os]);
			   YvGrid->Cells[5][r] = AnsiString(rvalit[rv].vertaika[srj][os]);
			   YvGrid->Cells[6][r] =
				  AnsiString(rvalit[rv].sum[srj][os]/rvalit[rv].lkm[srj][os]);
			   YvGrid->Cells[7][r] = AnsiString(rvalit[rv].ajat[srj][os][0]);
			   YvGrid->Cells[8][r] = AnsiString(rvalit[rv].kilpno[srj][os][0]);
			   r++;
			   }
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
	if (!emitanal_fl)
		Teerastivlianalyysi1->Visible = true;
   LaadiYhtveto();
}
//---------------------------------------------------------------------------

void __fastcall TFormYhtveto::Kirjoitatiedostoon1Click(TObject *Sender)
{
   if (SaveDialog1->Execute()) {
	  ofstream  outfile(SaveDialog1->FileName.c_str());
	  outfile << ("R-väli")
		 << "\t" << ("Sarja")
		 << "\t" << ("Osuus")
		 << "\t" << ("Lukum.")
		 << "\t" << ("Vert.aika")
		 << "\t" << ("Keskim.")
		 << "\t" << ("Paras")
		 << "\t" << ("Joukkue") << "\n";
	  for (int rv = 0; rv < rvluku; rv++) {
		 for (int srj = 0; srj < sarjaluku; srj++) {
			for (int os = 0; os < Sarjat[srj].ntosuus; os++) {
			   if (rvalit[rv].lkm[srj][os] > 0) {
				  int n = rvalit[rv].lkm[srj][os];
				  outfile << (rvalit[rv].kdi)
					 << "\t" << (Sarjat[srj].sarjanimi)
					 << "\t" << (os+1)
					 << "\t" << (n)
					 << "\t" << (rvalit[rv].vertaika[srj][os])
					 << "\t" <<
						(rvalit[rv].sum[srj][os]/rvalit[rv].lkm[srj][os])
					 << "\t" << (rvalit[rv].ajat[srj][os][0])
					 << "\t" << (rvalit[rv].kilpno[srj][os][0])
					 << "\t" << (rvalit[rv].ajat[srj][os][(n+10)/20])
					 << "\t" << (rvalit[rv].ajat[srj][os][(n+5)/10])
					 << "\t" << (rvalit[rv].ajat[srj][os][(n+2)/4])
					 << "\t" << (rvalit[rv].ajat[srj][os][(n+1)/2])
					 << "\t" << (rvalit[rv].ajat[srj][os][(n*3+2)/4])
					 << "\t" << (rvalit[rv].ajat[srj][os][(n*9+5)/10])
							<< "\t" << (rvalit[rv].ajat[srj][os][(n*19+10)/20])
					 << "\n";
				  }
               }
            }
         }
      outfile.close();
      }
}
//---------------------------------------------------------------------------

#endif // EMIT
void __fastcall TFormYhtveto::FormResize(TObject *Sender)
{
	YvGrid->Height = ClientHeight - YvGrid->Top - 40 * Screen->PixelsPerInch / 96;
	YvGrid->Width = ClientWidth - YvGrid->Left;
	BtnPaivita->Top = ClientHeight - YvGrid->Top - 32 * Screen->PixelsPerInch / 96;
	BtnSulje->Top = BtnPaivita->Top;
}
//---------------------------------------------------------------------------

void __fastcall TFormYhtveto::Teerastivlianalyysi1Click(TObject *Sender)
{
	uusiemitanalyysi(true);
	LaadiYhtveto();
}
//---------------------------------------------------------------------------

