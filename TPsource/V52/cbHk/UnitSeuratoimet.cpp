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

#include "UnitSeuratoimet.h"
#include "UnitSeurat.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSeuratoimet *FormSeuratoimet;
//---------------------------------------------------------------------------
__fastcall TFormSeuratoimet::TFormSeuratoimet(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormSeuratoimet::Button1Click(TObject *Sender)
{
	FormSeurat->Show();
	if (FormSeurat->WindowState == wsMinimized)
		FormSeurat->WindowState = wsNormal;
	FormSeurat->BringToFront();
}
//---------------------------------------------------------------------------
void __fastcall TFormSeuratoimet::Button2Click(TObject *Sender)
{
	kilptietue kilp, entkilp;
	int d, srj, n = 0;
	bool vapaakoko;

	vapaakoko = CBVapaaKoko->Checked;
	if (RGToiminto->ItemIndex == 2 || RGToiminto->ItemIndex == 3) {
		if (!FormSeurat->SeuraHakuOn()) {
			Application->MessageBoxW(L"Avaa ensin seuraluettelo", L"Este", MB_OK);
			return;
			}
		if (CBMaa->Checked == !FormSeurat->Maakooditkytss1->Checked) {
			Application->MessageBoxW(L"Maakoodit eivät käytössä seuraluettelossa", L"Este", MB_OK);
			return;
			}
		}
	else {
		if (CBPiiri->Checked) {
			Application->MessageBoxW(L"Piiri siirrettävissä vain seuraluettelosta", L"Este", MB_OK);
			return;
			}
		if (CBMaa->Checked) {
			Application->MessageBoxW(L"Maa siirrettävissä vain seuraluettelosta", L"Este", MB_OK);
			return;
			}
		}
	EnterCriticalSection(&tall_CriticalSection);
	for (d = 1; d < nrec; d++) {
		int piiri;
		wchar_t nimi[LSEURA+1]=L"", lyhenne[LLYH+1]=L"", maa[4]=L"";

	   kilp.GETREC(d);
	   entkilp = kilp;
	   if (kilp.kilpstatus)
		   continue;
	   switch (RGToiminto->ItemIndex) {
		   case 0:              // lyh -> sra kopio
			  wcscpy(kilp.seura, kilp.seuralyh);
			  break;
		   case 1:              // sra -> lyh kopio
			  wcsncpy(kilp.seuralyh, kilp.seura, kilpparam.lseuralyh);
			  kilp.seuralyh[kilpparam.lseuralyh] = 0;
			  break;
		   case 2:              // lyh -> sra muunnos
			  wcsncpy(lyhenne, kilp.seuralyh, kilpparam.lseuralyh);
			  if (!FormSeurat->HaeTiedot(true, &piiri, maa, lyhenne, nimi, vapaakoko)) {
				  wcsncpy(kilp.seura, nimi, kilpparam.lseura);
				  kilp.seura[kilpparam.lseura] = 0;
				  if (CBPiiri->Checked)
					  kilp.piiri = piiri;
				  if (CBMaa->Checked) {
					  wcsncpy(kilp.maa, maa, 3);
					  kilp.maa[3] = 0;
					  }
				  }
			  break;
		   case 3:              // sra -> lyh muunnos
			  wcsncpy(nimi, kilp.seura, kilpparam.lseura);
			  if (!FormSeurat->HaeTiedot(false, &piiri, maa, lyhenne, nimi, vapaakoko)) {
				  wcsncpy(kilp.seuralyh, lyhenne, kilpparam.lseuralyh);
				  kilp.seuralyh[kilpparam.lseuralyh] = 0;
				  if (CBPiiri->Checked)
					  kilp.piiri = piiri;
				  if (CBMaa->Checked) {
					  wcsncpy(kilp.maa, maa, 3);
					  kilp.maa[3] = 0;
					  }
				  }
			  break;
		   case 4:              // lyh -> joukkue
			  wcsncpy(kilp.joukkue, kilp.seuralyh, kilpparam.ljoukkue);
			  kilp.joukkue[kilpparam.ljoukkue] = 0;
			  break;
		   case 5:              // sra -> joukkue
			  wcsncpy(kilp.joukkue, kilp.seura, kilpparam.ljoukkue);
			  kilp.joukkue[kilpparam.ljoukkue] = 0;
			  break;
		   case 6:              // lyh -> yhd
			  wcsncpy(kilp.yhdistys, kilp.seuralyh, kilpparam.lyhdistys);
			  kilp.yhdistys[kilpparam.lyhdistys] = 0;
			  break;
		   case 7:              // sra -> yhd
			  wcsncpy(kilp.yhdistys, kilp.seura, kilpparam.lyhdistys);
			  kilp.yhdistys[kilpparam.lyhdistys] = 0;
			  break;
		   case 8:              // FIN -> maa
			  if (kilp.maa[0] == 0 || !wcscmp(kilp.maa, L" ") ||
				  !wcscmp(kilp.maa, L"  ") || !wcscmp(kilp.maa, L"  ")) {
				  wcscpy(kilp.maa, L"FIN");
				  }
			  break;
		   case 9:              // sarjatiedot
			  srj = kilp.Sarja(CBsrj1->ItemIndex-1);
			  if (CBsrj2->ItemIndex == 0)
				  kilp.setSarjaAll(srj);
			  else
				  kilp.setSarja(srj, CBsrj2->ItemIndex-2);
			  break;
		   case 10:              // sarjasiirto
			  if (kilp.Sarja(CBsrj1->ItemIndex-1) == CBLahto->ItemIndex) {
				  if (CBsrj2->ItemIndex == 0)
					  kilp.setSarjaAll(CBUusisarja->ItemIndex);
				  else
					  kilp.setSarja(CBUusisarja->ItemIndex, CBsrj2->ItemIndex-2);
				 }
			  break;
		   }
	   if (!(kilp == entkilp)) {
			kilp.tallenna(d, 0, 0, 0, &entkilp);
			n++;
			}
	   }
	LeaveCriticalSection(&tall_CriticalSection);
	Application->MessageBoxW((UnicodeString(L"Tallennettu ")+UnicodeString(n)+
		UnicodeString(L" tietoa.")).c_str(), L"Valmis", MB_OK);
}
//---------------------------------------------------------------------------
void __fastcall TFormSeuratoimet::RGToimintoClick(TObject *Sender)
{
	GroupBox1->Visible = RGToiminto->ItemIndex >= 9;
	GroupBox2->Visible = RGToiminto->ItemIndex == 10;
	CBLahto->Clear();
	CBUusisarja->Clear();
	for (int srj = 0; srj < sarjaluku; srj++) {
		CBLahto->Items->Add(Sarjat[srj].sarjanimi);
		CBUusisarja->Items->Add(Sarjat[srj].sarjanimi);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormSeuratoimet::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Ilmoittautumiset");
}
//---------------------------------------------------------------------------

void __fastcall TFormSeuratoimet::Button3Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormSeuratoimet::FormShow(TObject *Sender)
{
	CBsrj1->Clear();
	CBsrj1->Items->Add(L"Perussarja");
	CBsrj2->Clear();
	CBsrj2->Items->Add(L"Kaikki vaiheet");
	CBsrj2->Items->Add(L"Perussarja");
	for (int i = 0;i < kilpparam.n_pv_akt; i++) {
	   CBsrj1->Items->Add(UnicodeString(i+1)+L". vaiheen sarja");
	   CBsrj2->Items->Add(UnicodeString(i+1)+L". vaiheen sarja");
		}
	CBsrj1->ItemIndex = 0;
	CBsrj2->ItemIndex = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormSeuratoimet::CBsrj2Change(TObject *Sender)
{
	if (!kilpparam.sarjavaihtelee && CBsrj2->ItemIndex != 0) {
		Application->MessageBoxW(L"Kohteeksi voi valita vain kaikki vaiheet, kun sarja ei vaihtele",
			L"Rajoitus", MB_OK);
		CBsrj2->ItemIndex = 0;
		}
}
//---------------------------------------------------------------------------

