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

#include "OsuusTiedot.h"
#include "VDeclare.h"
#include "UnitMain.h"
#include "UnitHenkHaku.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormOsuus *FormOsuus;

void uusiemitanalyysi(bool uusi);
extern rastivatp *rastiva;

//---------------------------------------------------------------------------
__fastcall TFormOsuus::TFormOsuus(TComponent* Owner)
   : TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
   Sarja = 0;
   Osuus = 1;
	KilpNo = 0;
	laika = kuuluttaja ? kilpparam.laika : kilpparam.laika2;
	if (emitfl) {
		EmitGrid->Enabled = true;
		EmitGrid->Visible = true;
		}
	else
		ClientHeight = EmitGrid->Top;
}
//---------------------------------------------------------------------------

void __fastcall TFormOsuus::EdtJoukkueChanged(TObject *Sender)
{
   char txt[20];

   WcsToMbs(txt, EdtJoukkue->Text.c_str(),19);
   txt[19] = 0;
	KilpNo = atoi(txt);
   naytaTiedot();
}
//---------------------------------------------------------------------------


void __fastcall TFormOsuus::naytaTiedot(void)
   {
   wchar_t ch, cs, ch2, ct, line[80], trk, selitys[17];
	int i, os, yos, d, rvali, sj, emvaluku;
   int tl, tlsum;
	kilptietue kilp;
	emittp em;
   emitvaanaltp *eva = 0;
   rastivaanaltp *rstva = 0;

   if (inNaytaTiedot)
	  return;
   inNaytaTiedot = 1;
   os = Osuus - 1;
   memset(line, 0, sizeof(line));
   if (KilpNo && (dKilp = getpos(KilpNo)) > 0) {
	  GETREC(&kilp, dKilp);
	  EdtJoukkue->Text = AnsiString(KilpNo);
	  Sarja = kilp.sarja;
	  ValSarja->ItemIndex = Sarja;
	  if (os < 0)
		 os = 0;
	  if (os >= Sarjat[kilp.sarja].osuusluku)
		 os = Sarjat[kilp.sarja].osuusluku - 1;
	  ValOsuus->ItemIndex = os;
	  Osuus = os + 1;
	  yos = Sarjat[Sarja].yosuus[os];
	  kilp.Joukkue(line, 20);
	  swprintf(Kohde, L"%s %d. os", line, Osuus);
	  FormMain->SetFormKohde(this, Kohde);
	  kilp.Joukkue(line);
	  JoukkNimi->Text = line;
	  Maa->Text = kilp.Maa();
	  kilp.Nimi(line, OSNIMIL, os);
	  OsNimi->Text = line;
	  tl = kilp.Maali(os, 0);
	  if (tl != TMAALI0) {
		 aikatowstr_cols_n(line, tl, t0, 0, laika);
		 TMaali->Text = line;
		 }
	  else
		 TMaali->Clear();
		tl = osuuslahto(&kilp, os);
	  if (tl != TMAALI0) {
		 aikatowstr_cols_n(line, tl, t0, 0, laika);
		 TLahto->Text = line;
		 }
	  else
		 TLahto->Clear();
	  switch (kilp.ostiet[os].keskhyl) {
		 case 'K' :
				Tulos->Text = L"Kesk";
			break;
		 case 'H' :
			Tulos->Text = L"Hyl";
			break;
		 case 'S' :
			Tulos->Text = L"Sulj";
			break;
		 case 'E' :
			Tulos->Text = L"Eiläht.";
			break;
		 case '-' :
		 case 'T' :
		 case 'I' :
			tl = tulos(&kilp, os, 0);
			aikatowstr_cols_n(line, tl, 0, 0, laika);
			Tulos->Text = line;
			break;
			default :
			Tulos->Clear();
			break;
		 }
		tl = osuustulos(&kilp, os, 0);
		aikatowstr_cols_n(line, tl, 0, 0, laika);
		OsTls->Text = line;
		Sija->Text = psija(kilp.kilpno, kilp.sarja, os, 0);
		OsSija->Text = psija(kilp.kilpno, kilp.sarja, os, kilpparam.maxvaluku+1);
		Ekoodi->Text = kilp.ostiet[os].badge[0];
		if (naytahajonta)
			EdHajonta->Text = kilp.ostiet[os].Hajonta(line);
	  if (os > 0 && (sj = psija(kilp.kilpno, kilp.sarja, os-1, 0)) > 0)
		 LahtoSija->Text = sj;
	  else
			LahtoSija->Clear();
	  if (emitfl && getem(&em, kilp.kilpno, os) >= 0) {
		EmitGrid->RowCount = 1;
		EnterCriticalSection(&rastiva_CriticalSection);
		 laskeemitvaliajat(&em, &kilp, &eva, &rstva, &emvaluku);
		 if (eva) {
			int nradalla = 0, nsarjasta = 0;
			int rr = haerata(&kilp, os) - rata;
			if (rstva)
				nradalla = rastiva[rr].nva;
			nsarjasta = ntulosix[kilp.sarja][yos][kilpparam.valuku+1];
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
			EmitGrid->ColWidths[10] = 55 * Screen->PixelsPerInch / 96;
			EmitGrid->ColWidths[11] = 65 * Screen->PixelsPerInch / 96;
			EmitGrid->ColWidths[12] = 55 * Screen->PixelsPerInch / 96;
			EmitGrid->Width = (80+70+350+105+65+55) * Screen->PixelsPerInch / 96;
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
			   tl = eva[i].rvaika - rvalit[eva[i].rvno].vertaika[Sarja][yos];
			   tlsum += tl;
			   aikatowstr_s(line+1, abs(tl), 0);
			   elimwzb1(line+1);
			   line[0] = (tl >= 0) ? L'+' : L'-';
			   EmitGrid->Cells[6][i+1] = UnicodeString(line);
			   tl = rvalit[eva[i].rvno].vertaika[Sarja][yos];
			   aikatowstr_s(line, abs(tl), 0);
			   elimwzb1(line);
			   EmitGrid->Cells[7][i+1] = UnicodeString(line);
			   tl = rvalit[eva[i].rvno].ajat[Sarja][yos][0];
			   aikatowstr_s(line, tl, 0);
			   elimwzb1(line);
			   EmitGrid->Cells[9][i+1] = UnicodeString(line);
				tl = eva[i].rvaika - tl;
			   aikatowstr_s(line+1, abs(tl), 0);
			   elimwzb1(line+1);
			   line[0] = (tl >= 0) ? L'+' : L'-';
			   EmitGrid->Cells[8][i+1] = UnicodeString(line);
			   sj = bSrchInt(eva[i].rvaika, rvalit[eva[i].rvno].ajat[Sarja][yos],
					rvalit[eva[i].rvno].lkm[Sarja][yos]) + 1;
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
	inNaytaTiedot = 0;
   }
//---------------------------------------------------------------------------
void __fastcall TFormOsuus::naytaOsuus(int KNo, int osuus)
{
	KilpNo = KNo;
	Osuus = osuus;
	ValOsuus->ItemIndex = Osuus-1;
}
//---------------------------------------------------------------------------
void __fastcall TFormOsuus::OsKarkiBtnClick(TObject *Sender)
{
   int eos, yos;
   int p;
   kilptietue kilp;

   yos = Sarjat[Sarja].yosuus[Osuus-1];
   EnterCriticalSection(&tall_CriticalSection);
   p = JosAlku(Sarja , yos, &eos);
   if (p > 0) {
	  Osuus = eos+1;
	  dKilp = p;
	  GETREC(&kilp, dKilp);
	  KilpNo = kilp.kilpno;
	  naytaTiedot();
	  }
   LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormOsuus::OsSeurClick(TObject *Sender)
{
   int sj, eos, yos;
   int p;
   kilptietue kilp;

   if (!KilpNo)
	  return;
   yos = Sarjat[Sarja].yosuus[Osuus-1];
   EnterCriticalSection(&tall_CriticalSection);
   eos = Osuus - 1;
   p = OsJarjSeur(Yhd, Sarja, &eos, dKilp);
   if (p > 0) {
	   Osuus = eos + 1;
	   dKilp = p;
	   GETREC(&kilp, dKilp);
	   KilpNo = kilp.kilpno;
	   naytaTiedot();
	   }
   LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormOsuus::OsEdClick(TObject *Sender)
{
   int sj, eos, yos;
   int p;
   kilptietue kilp;

   if (!KilpNo)
	  return;
   yos = Sarjat[Sarja].yosuus[Osuus-1];
   EnterCriticalSection(&tall_CriticalSection);
   p = JosAlku(Sarja , yos, &eos);
   entossija(dKilp, &p, Sarja, yos, &eos, Yhd, 0);
   if (p > 0) {
	  Osuus = eos + 1;
	  dKilp = p;
	  GETREC(&kilp, p);
	  KilpNo = kilp.kilpno;
	  naytaTiedot();
	  }
   LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormOsuus::VaihdKarkiBtn(TObject *Sender)
{
   int p;
   kilptietue kilp;

   EnterCriticalSection(&tall_CriticalSection);
   p = jalku[Sarja][Osuus-1][0];
   if (p > 0) {
	  dKilp = p;
	  GETREC(&kilp, dKilp);
	  KilpNo = kilp.kilpno;
	  naytaTiedot();
	  }
   LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormOsuus::VaihdSeurBtn(TObject *Sender)
{
   int sj;
   int p;
   kilptietue kilp;

   if (!KilpNo)
	  return;
   EnterCriticalSection(&tall_CriticalSection);
   p = JarjSeur(Yhd, Osuus-1, 0, dKilp);
   if (p > 0) {
	   dKilp = p;
	   GETREC(&kilp, dKilp);
	   KilpNo = kilp.kilpno;
	   naytaTiedot();
	   }
   LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormOsuus::VaihdEdBtn(TObject *Sender)
{
   int sj;
   int p;
   kilptietue kilp;

   if (!KilpNo)
	  return;
   EnterCriticalSection(&tall_CriticalSection);
   p = jalku[Sarja][Osuus-1][0];
   entsija(dKilp, &p, Osuus-1, 0, Yhd, 0);
   if (p > 0) {
	  dKilp = p;
	  GETREC(&kilp, p);
	  KilpNo = kilp.kilpno;
	  naytaTiedot();
	  }
   LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormOsuus::EdtJoukkueKeyDown(TObject *Sender, WORD &Key,
	  TShiftState Shift)
{
   if (Key == VK_RETURN)
	  EdtJoukkueChanged(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormOsuus::FormResize(TObject *Sender)
{
	if (emitfl) {
		EmitGrid->Height = ClientHeight - EmitGrid->Top;
		EmitGrid->Width = ClientWidth - EmitGrid->Left;
		EmitGrid->Refresh();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormOsuus::ValSarjaChange(TObject *Sender)
{
   Sarja = ValSarja->ItemIndex;
   if (Osuus > Sarjat[Sarja].osuusluku)
	  Osuus = Sarjat[Sarja].osuusluku;
   ValOsuus->Clear();
   for (int os = 0; os < Sarjat[Sarja].osuusluku; os++) {
	   ValOsuus->Items->Add(UnicodeString(L"Osuus ") + wosuuskoodi(Sarja, os, 0, 0));
	   }
   ValOsuus->ItemIndex = Osuus - 1;
   VaihdKarkiBtn(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormOsuus::FormCreate(TObject *Sender)
{
   ValSarja->Clear();
   for (int srj = 0; srj < sarjaluku; srj++)
      ValSarja->Items->Add(Sarjat[srj].sarjanimi);
   ValSarja->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormOsuus::FormShow(TObject *Sender)
{
	Rastivliaikaanalyysi1->Visible = !emitanal_fl;
   ValOsuus->Clear();
   for (int os = 0; os < Sarjat[Sarja].osuusluku; os++) {
	   ValOsuus->Items->Add(UnicodeString(L"Osuus ") + wosuuskoodi(Sarja, os, 0, 0));
	   }
   ValOsuus->ItemIndex = Osuus - 1;
   naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsuus::ValOsuusChange(TObject *Sender)
{
   Osuus = ValOsuus->ItemIndex + 1;
   naytaTiedot();
}
//---------------------------------------------------------------------------

void __fastcall TFormOsuus::Haenimell1Click(TObject *Sender)
{
	FormHenkHaku = new TFormHenkHaku(this);
	FormHenkHaku->ShowModal();
	if (FormHenkHaku->ModalResult == mrOk && FormHenkHaku->KilpNo) {
		KilpNo = FormHenkHaku->KilpNo;
		Osuus = FormHenkHaku->Osuus+1;
		ValOsuus->ItemIndex = Osuus-1;
		naytaTiedot();
		}
	delete FormHenkHaku;
}
//---------------------------------------------------------------------------

void __fastcall TFormOsuus::eeanalyysi1Click(TObject *Sender)
{
   uusiemitanalyysi(true);
   naytaTiedot();
}
//---------------------------------------------------------------------------

