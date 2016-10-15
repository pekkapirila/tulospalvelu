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

#include "UnitTuloskaava.h"
#include "UnitVaPisteet.h"
#include "HkDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormTuloskaava *FormTuloskaava;
//---------------------------------------------------------------------------
__fastcall TFormTuloskaava::TFormTuloskaava(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormTuloskaava::FormShow(TObject *Sender)
{
	CBSarja->Items->Clear();
	for (int srj = 0; srj < sarjaluku; srj++)
		CBSarja->Items->Add(Sarjat[srj].sarjanimi);
	CBSarja->ItemIndex = Srj;
	CBVaihe->Items->Clear();
	for (int i = 0; i < kilpparam.n_pv_akt; i++)
		CBVaihe->Items->Add(UnicodeString(i+1)+L". vaihe");
	CBVaihe->ItemIndex = Pv;
	CBLaskettava->Items->Clear();
	CBLaskettava->Items->Add(L"Vaiheen lopputulos");
	for (int i = 0; i < Sarjat[Srj].valuku[Pv]; i++)
		CBLaskettava->Items->Add(UnicodeString(i+1)+L". va-pisteen aika");
	CBLaskettava->Items->Add(L"Vaihekohtaiset pisteet");
	CBLaskettava->ItemIndex = (Va <= Sarjat[Srj].valuku[Pv]) ? Va : Sarjat[Srj].valuku[Pv]+1;
	EdtKaavaOn->Text = Sarjat[Srj].kaava[Pv][Va].on ? L"Kaava käytössä" : L"Kaava pois käytöstä";
	EdtYla->Text = Sarjat[Srj].kaava[Pv][Va].yla;
	EdtAla->Text = Sarjat[Srj].kaava[Pv][Va].ala;
	EdtKerroin->Text = Sarjat[Srj].kaava[Pv][Va].kerroin;
	EdtJakaja->Text = Sarjat[Srj].kaava[Pv][Va].jakaja;
	EdtAlaraja->Text = Sarjat[Srj].kaava[Pv][Va].minimi;
	EdtYlaraja->Text = Sarjat[Srj].kaava[Pv][Va].maksimi;
	EdtNollataso->Text = Sarjat[Srj].kaava[Pv][Va].nollataso;
	CBVaadiAla->Checked = Sarjat[Srj].kaava[Pv][Va].vaadiala;
	CBSakotPois->Checked = Sarjat[Srj].kaava[Pv][Va].sakotpois;
	EdtNollataso->Enabled = CBLaskettava->ItemIndex > Sarjat[Srj].valuku[Pv];
}
//---------------------------------------------------------------------------
void __fastcall TFormTuloskaava::AsetaPiste(int srj, int pv, int va)
{
	Srj = srj;
	Pv = pv;
	Va = va;
}
void __fastcall TFormTuloskaava::BtnKaikilleClick(TObject *Sender)
{
	for (int srj = 0; srj < sarjaluku; srj++) {
		Sarjat[srj].kaava[Pv][Va].yla = _wtoi(EdtYla->Text.c_str());
		Sarjat[srj].kaava[Pv][Va].ala = _wtoi(EdtAla->Text.c_str());
		Sarjat[srj].kaava[Pv][Va].kerroin = _wtoi(EdtKerroin->Text.c_str());
		Sarjat[srj].kaava[Pv][Va].jakaja = _wtoi(EdtJakaja->Text.c_str());
		Sarjat[srj].kaava[Pv][Va].minimi = _wtoi(EdtAlaraja->Text.c_str());
		Sarjat[srj].kaava[Pv][Va].maksimi = _wtoi(EdtYlaraja->Text.c_str());
		Sarjat[srj].kaava[Pv][Va].nollataso = _wtoi(EdtNollataso->Text.c_str());
		Sarjat[srj].kaava[Pv][Va].vaadiala = CBVaadiAla->Checked;
		Sarjat[srj].kaava[Pv][Va].sakotpois = CBSakotPois->Checked;
		Sarjat[srj].kaava[Pv][Va].on = true;
		}
	kirjEventXml(sarjaxmlfname, true, false, false);
	if (FormVaPisteet) {
		FormVaPisteet->naytaTiedot();
		FormVaPisteet->BringToFront();
		FormVaPisteet->Refresh();
		}
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormTuloskaava::BtnTallennaClick(TObject *Sender)
{
	if (_wtoi(EdtYla->Text.c_str()) == _wtoi(EdtAla->Text.c_str()) && _wtoi(EdtYla->Text.c_str()) != -1) {
		Application->MessageBoxW(L"Ylä- ja alatuloksen pisteet samat. Muuta kaavaa tai poista käytöstä",
			L"Virhe", MB_OK);
		return;
		}
	if (_wtoi(EdtKerroin->Text.c_str()) == 0 || _wtoi(EdtJakaja->Text.c_str()) == 0) {
		Application->MessageBoxW(L"Sekä kertoimen että jakajan pitää poiketa nollasta",
			L"Virhe", MB_OK);
		return;
		}
	Sarjat[Srj].kaava[Pv][Va].yla = _wtoi(EdtYla->Text.c_str());
	Sarjat[Srj].kaava[Pv][Va].ala = _wtoi(EdtAla->Text.c_str());
	Sarjat[Srj].kaava[Pv][Va].kerroin = _wtoi(EdtKerroin->Text.c_str());
	Sarjat[Srj].kaava[Pv][Va].jakaja = _wtoi(EdtJakaja->Text.c_str());
	Sarjat[Srj].kaava[Pv][Va].minimi = _wtoi(EdtAlaraja->Text.c_str());
	Sarjat[Srj].kaava[Pv][Va].maksimi = _wtoi(EdtYlaraja->Text.c_str());
	Sarjat[Srj].kaava[Pv][Va].nollataso = _wtoi(EdtNollataso->Text.c_str());
	Sarjat[Srj].kaava[Pv][Va].vaadiala = CBVaadiAla->Checked;
	Sarjat[Srj].kaava[Pv][Va].sakotpois = CBSakotPois->Checked;
	Sarjat[Srj].kaava[Pv][Va].on = true;
	kirjEventXml(sarjaxmlfname, true, false, false);
	if (FormVaPisteet)
		FormVaPisteet->naytaTiedot();
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormTuloskaava::BtnPoistaClick(TObject *Sender)
{
	Sarjat[Srj].kaava[Pv][Va].on = false;
	kirjEventXml(sarjaxmlfname, true, false, false);
	if (FormVaPisteet)
		FormVaPisteet->naytaTiedot();
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormTuloskaava::BtnPoistaKaikiltaClick(TObject *Sender)
{
	for (int srj = 0; srj < sarjaluku; srj++) {
		Sarjat[srj].kaava[Pv][Va].on = false;
		}
	kirjEventXml(sarjaxmlfname, true, false, false);
	if (FormVaPisteet)
		FormVaPisteet->naytaTiedot();
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormTuloskaava::Button2Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormTuloskaava::CBSarjaChange(TObject *Sender)
{
	Srj = CBSarja->ItemIndex;
	FormShow(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormTuloskaava::CBVaiheChange(TObject *Sender)
{
	Pv = CBVaihe->ItemIndex;
	FormShow(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormTuloskaava::CBLaskettavaChange(TObject *Sender)
{
	Va = CBLaskettava->ItemIndex;
	if (Va > Sarjat[Srj].valuku[Pv])
		Va = VALUKU+1;
	FormShow(Sender);
}
//---------------------------------------------------------------------------

