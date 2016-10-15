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

#include "HkDeclare.h"
#include "UnitKilpailijaEmit.h"
#include "WinHk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormKilpailijaEmit *FormKilpailijaEmit;

extern rastivatp *rastiva;

//---------------------------------------------------------------------------
__fastcall TFormKilpailijaEmit::TFormKilpailijaEmit(TComponent* Owner)
   : TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	Sarja = -1;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaEmit::EdtKilpnoChanged(TObject *Sender)
{
   wchar_t txt[20];
   int KilpNo, d;

   wcsncpy(txt, EdtKilpno->Text.c_str(),19);
   txt[19] = 0;
   KilpNo = _wtoi(txt);
   if ((d = getpos(KilpNo, true)) > 0) {
	  dKilp = d;
	  Kilp->GETREC(d);
	  Sarja = Kilp->Sarja();
	  naytaTiedot();
	  }
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaEmit::naytaTiedot(void)
{
   wchar_t ch, cs, ch2, ct, line[80], trk, selitys[17];
	int i, d, rvali, sj, emvaluku;
   int tl, tlsum;
	emittp em;
   emitvaanaltp *eva = 0;
   rastivaanaltp *rstva = 0;

	memset(line, 0, sizeof(line));
   if (Kilp->id()) {
      if (SrjVal->ItemIndex != Sarja) {
         for (int i = 0; i < SrjVal->Items->Count; i++) {
			if (SrjVal->Items->Strings[i] == UnicodeString(Sarjat[Sarja].sarjanimi)) {
               SrjVal->ItemIndex = i;
               break;
               }
            }
			}
		sijat(Sarja);
      EdtKilpno->Text = UnicodeString(Kilp->id());
		Kilp->nimi(line, 38, false);
	  wcsncpy(Kohde, line, sizeof(Kohde)/2-1);
	  FormMain->SetFormKohde(this, Kohde);
	  Nimi->Text = UnicodeString(line);
	  Maa->Text = UnicodeString(Kilp->maa);
	  wcscpy(line, Kilp->seuratieto(0));
	  EdtSeura->Text = UnicodeString(line);
      tl = Kilp->maali(0);
      if (tl != TMAALI0) {
		 AIKATOWSTRS(line, tl, t0);
		 line[pvparam[k_pv].laika] = 0;
         TMaali->Text = UnicodeString(line);
         }
      else
			TMaali->Clear();
      tl = Kilp->pv[k_pv].tlahto;
      if (tl != TMAALI0) {
		 AIKATOWSTRS(line, tl, t0);
		 line[8] = 0;
         TLahto->Text = UnicodeString(line);
         }
      else
         TLahto->Clear();
		switch (Kilp->tark(k_pv)) {
         case L'K' :
				Tulos->Text = UnicodeString(L"Kesk");
				break;
			case L'H' :
				Tulos->Text = UnicodeString(L"Hyl");
				break;
			case L'M' :
				Tulos->Text = UnicodeString(L"Virhe");
				break;
			case L'X' :
				Tulos->Text = UnicodeString(L"EiAikaa");
				break;
			case L'E' :
				Tulos->Text = UnicodeString(L"Eiläht.");
				break;
			case L'B' :
				Tulos->Text = UnicodeString(L"Havaittu");
				break;
			case L'V' :
				Tulos->Text = UnicodeString(L"Vakant");
				break;
			case L'-' :
			case L'T' :
			case L'I' :
				tl = Kilp->p_aika(0);
				AIKATOWSTRS(line, tl, 0);
				line[pvparam[k_pv].laika] = 0;
				Tulos->Text = UnicodeString(line);
				break;
			default :
				Tulos->Clear();
				break;
			}
		Sija->Text = UnicodeString(Kilp->p_sija(0));
		VaGrid->RowCount = 1 + Sarjat[Kilp->Sarja()].valuku[k_pv];
		VaGrid->DefaultRowHeight = 18 * Screen->PixelsPerInch / 96;
		if (Sarjat[Kilp->Sarja()].valuku[k_pv]) {
			VaGrid->Cells[1][0] = UnicodeString(L"Sija");
		 VaGrid->Cells[2][0] = UnicodeString(L"V-aika");
		 for (i = 1; i <= Sarjat[Kilp->Sarja()].valuku[k_pv]; i++) {
            VaGrid->Cells[0][i] = UnicodeString(i)+L". va";
				if ((tl = Kilp->p_aika(i)) > 0) {
			   VaGrid->Cells[1][i] = UnicodeString(Kilp->p_sija(i));
			   AIKATOWSTRS(line, tl, 0);
               line[pvparam[k_pv].laika] = 0;
					VaGrid->Cells[2][i] = UnicodeString(line);
               }
            else {
			   VaGrid->Cells[1][i] = UnicodeString(L" ");
					VaGrid->Cells[2][i] = UnicodeString(L" ");
               }
            }
         }
		if (emitanal_fl && getem(&em, Kilp->id(), 0) >= 0) {
			EmitGrid->RowCount = 1;
			EnterCriticalSection(&rastiva_CriticalSection);
			laskeemitvaliajat(&em, Kilp, &eva, &rstva, &emvaluku);
			if (eva) {
				int nradalla = 0, nsarjasta = 0;
				int rr = haerata(Kilp) - rata;
				if (rstva)
					nradalla = rastiva[rr].nva;
				nsarjasta = ntulos[Kilp->Sarja()][0];
				EmitGrid->RowCount = emvaluku+1;
				EmitGrid->ColCount = 13;
				EmitGrid->DefaultRowHeight = 18 * Screen->PixelsPerInch / 96;
				EmitGrid->ColWidths[0] = 40 * Screen->PixelsPerInch / 96;
				EmitGrid->ColWidths[1] = 40 * Screen->PixelsPerInch / 96;
				EmitGrid->ColWidths[2] = 70 * Screen->PixelsPerInch / 96;
				EmitGrid->ColWidths[3] = 50 * Screen->PixelsPerInch / 96;
				EmitGrid->ColWidths[4] = 50 * Screen->PixelsPerInch / 96;
				EmitGrid->ColWidths[5] = 50 * Screen->PixelsPerInch / 96;
				EmitGrid->ColWidths[6] = 50 * Screen->PixelsPerInch / 96;
				EmitGrid->ColWidths[7] = 50 * Screen->PixelsPerInch / 96;
				EmitGrid->ColWidths[8] = 50 * Screen->PixelsPerInch / 96;
				EmitGrid->ColWidths[9] = 50 * Screen->PixelsPerInch / 96;
				EmitGrid->ColWidths[10] = 40 * Screen->PixelsPerInch / 96;
				EmitGrid->ColWidths[11] = 65 * Screen->PixelsPerInch / 96;
				EmitGrid->ColWidths[12] = 55 * Screen->PixelsPerInch / 96;
				EmitGrid->Width = (80+70+350+95+65+55) * Screen->PixelsPerInch / 96;
				EmitGrid->Cells[0][0] = UnicodeString(L"Nro");
				EmitGrid->Cells[1][0] = UnicodeString(L"Koodi");
				EmitGrid->Cells[2][0] = UnicodeString(L"Aika");
				EmitGrid->Cells[3][0] = UnicodeString(L"Ero/nop.");
				EmitGrid->Cells[4][0] = UnicodeString(L"Sj/")+nradalla;
				EmitGrid->Cells[5][0] = UnicodeString(L"R-väli");
				EmitGrid->Cells[6][0] = UnicodeString(L"Suht rv.");
				EmitGrid->Cells[7][0] = UnicodeString(L"Vert.aik");
				EmitGrid->Cells[8][0] = UnicodeString(L"Ero/nop.");
				EmitGrid->Cells[9][0] = UnicodeString(L"Nopein");
				EmitGrid->Cells[10][0] = UnicodeString(L"Sj/")+nsarjasta;
				EmitGrid->Cells[11][0] = UnicodeString(L"Suht yht.");
				EmitGrid->Cells[12][0] = UnicodeString(L"min/km");
				tlsum = 0;
				for (i = 0; i < emvaluku; i++) {
				   int mtk;
				   if (eva[i].rvno < 0)
					  break;
				   EmitGrid->Cells[0][i+1] = UnicodeString(i+1);
				   EmitGrid->Cells[1][i+1] = UnicodeString(eva[i].rkoodi%1000);
				   aikatowstr_s(line, eva[i].aika, 0);
				   elimwzb1(line);
				   EmitGrid->Cells[2][i+1] = UnicodeString(line);
				   if (rstva) {
					   tl = rstva[i].aika - rstva[i].kaika;
					   line[0] = L'+';
					   aikatowstr_s(line+1, tl, 0);
					   elimwzb1(line+1);
					   EmitGrid->Cells[3][i+1] = line;
					   EmitGrid->Cells[4][i+1] = rstva[i].sj;
						}
				   else {
					   EmitGrid->Cells[3][i+1] = L"";
					   EmitGrid->Cells[4][i+1] = L"";
						}
				   aikatowstr_s(line, eva[i].rvaika, 0);
				   elimwzb1(line);
				   EmitGrid->Cells[5][i+1] = UnicodeString(line);
				   tl = eva[i].rvaika - rvalit[eva[i].rvno].vertaika[Sarja];
				   tlsum += tl;
				   aikatowstr_s(line+1, abs(tl), 0);
				   elimwzb1(line+1);
				   line[0] = (tl >= 0) ? L'+' : L'-';
				   EmitGrid->Cells[6][i+1] = UnicodeString(line);
				   tl = rvalit[eva[i].rvno].vertaika[Sarja];
				   aikatowstr_s(line, abs(tl), 0);
				   elimwzb1(line);
				   EmitGrid->Cells[7][i+1] = UnicodeString(line);
				   tl = rvalit[eva[i].rvno].ajat[Sarja][0];
				   aikatowstr_s(line, tl, 0);
				   elimwzb1(line);
				   EmitGrid->Cells[9][i+1] = UnicodeString(line);
					tl = eva[i].rvaika - tl;
				   aikatowstr_s(line+1, abs(tl), 0);
				   elimwzb1(line+1);
				   line[0] = (tl >= 0) ? L'+' : L'-';
				   EmitGrid->Cells[8][i+1] = UnicodeString(line);
				   sj = bSrchInt(eva[i].rvaika, rvalit[eva[i].rvno].ajat[Sarja],
						rvalit[eva[i].rvno].lkm[Sarja]) + 1;
				   EmitGrid->Cells[10][i+1] = UnicodeString(sj);
				   aikatowstr_s(line+1, abs(tlsum), 0);
				   elimwzb1(line+1);
				   line[0] = (tlsum >= 0) ? L'+' : L'-';
				   EmitGrid->Cells[11][i+1] = UnicodeString(line);
				   line[0] = 0;
				   if ((mtk = rvalit[eva[i].rvno].matka) > 10) {
						if (eva[i].rvaika) {
							int kmaika;
							kmaika = (100000 * (eva[i].rvaika) + 30 * mtk) / (60 * mtk);
							swprintf(line, L"%3d,%02d", kmaika/100, kmaika%100);
							}
						}
				   EmitGrid->Cells[12][i+1] = UnicodeString(line);
				   }
				free(eva);
				free(rstva);
				}
			LeaveCriticalSection(&rastiva_CriticalSection);
			}
		else
			EmitGrid->RowCount = 1;
		}
}

//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaEmit::naytaKilpailija(int d)
{
	 if (d > 0 && d < nrec)
		Kilp->GETREC(d);
	 if (Kilp->kilpstatus == 0) {
		dKilp = d;
		Sarja = Kilp->Sarja();
		naytaTiedot();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaEmit::SrjKarkiBtn(TObject *Sender)
{
   int p;

   if (ntulos[Sarja][0] > 0) {
	  p = jalku[Sarja][0];
	  if (p > 0) {
		 naytaKilpailija(p);
		 }
	  }
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaEmit::SeurBtnClick(TObject *Sender)
{
   int sj;
	int p;

   if (!Kilp->id())
      return;
   EnterCriticalSection(&tall_CriticalSection);
   p = JarjSeur(0, 0, dKilp);
   if (p >= 0) {
      Kilp->GETREC(p);
      dKilp = p;
      Sarja = Kilp->Sarja();
      naytaTiedot();
      }
   LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaEmit::EdBtnClick(TObject *Sender)
{
	int sj;
	int p, p1;

	if (!Kilp->id())
		return;
	EnterCriticalSection(&tall_CriticalSection);
	sj = Kilp->p_sija(0);
	if (sj > 1 && sj <= ntulos[Sarja][0]) {
		p = jalku[Sarja][0];
		while ((p1 = JarjSeur(0, 0, p)) >= 0 && p1 != dKilp)
			p = p1;
		if (p1 == dKilp) {
			Kilp->GETREC(p);
			dKilp = p;
			Sarja = Kilp->Sarja();
			naytaTiedot();
			}
		}
	LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaEmit::EdtKilpnoKeyDown(TObject *Sender, WORD &Key,
		TShiftState Shift)
{
	if (Key == VK_RETURN)
		EdtKilpnoChanged(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaEmit::FormResize(TObject *Sender)
{
	if (wcswcind(kilpparam.kilplaji, L"SBYT") >= 0) {
		EmitGrid->ClientHeight = ClientHeight - EmitGrid->Top - 8 * Screen->PixelsPerInch / 96;
		EmitGrid->Refresh();
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormKilpailijaEmit::FormCreate(TObject *Sender)
{
	Sarja = 0;
	dKilp = -1;
	Kilp = new kilptietue;
	Kilp->nollaa();
	for (int srj = 0; srj < sarjaluku; srj++)
		SrjVal->Items->Add(Sarjat[srj].sarjanimi);
	if (!emitanal_fl) {
		EmitGrid->Enabled = false;
		EmitGrid->Visible = false;
		if (kilpparam.maxvaluku >= 4)
			VaGrid->Height = 20*(kilpparam.maxvaluku+1) * Screen->PixelsPerInch / 96;
		ClientHeight = VaGrid->Top + VaGrid->Height + 10 * Screen->PixelsPerInch / 96;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaEmit::SrjValChange(TObject *Sender)
{
   if (ntulos[SrjVal->ItemIndex][0] > 0) {
      Sarja = SrjVal->ItemIndex;
      SrjKarkiBtn(Sender);
      }
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaEmit::FormDestroy(TObject *Sender)
{
   delete Kilp;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaEmit::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Kilpailun seuranta");
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaEmit::EmitGridMouseWheelDown(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
	if (EmitGrid->RowCount - EmitGrid->FixedRows >= EmitGrid->VisibleRowCount + EmitGrid->TopRow)
		EmitGrid->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaEmit::EmitGridMouseWheelUp(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
	if (EmitGrid->TopRow > EmitGrid->FixedRows)
		EmitGrid->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormKilpailijaEmit::Sulje1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

