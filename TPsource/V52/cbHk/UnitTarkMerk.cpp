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

#include "UnitTarkMerk.h"
#include "WinHk.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormTarkMerk *FormTarkMerk;
//---------------------------------------------------------------------------
__fastcall TFormTarkMerk::TFormTarkMerk(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	EdNo[0] = EdNo0;
	EdSrj[0] = EdSrj0;
	EdNimi[0] = EdNimi0;
	EdLA[0] = EdLA0;
	EdTrk[0] = EdTrk0;
	EdTls[0] = EdTls0;
	EdNo[1] = Edit1;
	EdSrj[1] = Edit2;
	EdNimi[1] = Edit3;
	EdLA[1] = Edit4;
	EdTrk[1] = Edit5;
	EdTls[1] = Edit6;
	EdNo[2] = Edit7;
	EdSrj[2] = Edit8;
	EdNimi[2] = Edit9;
	EdLA[2] = Edit10;
	EdTrk[2] = Edit11;
	EdTls[2] = Edit12;
	EdNo[3] = Edit13;
	EdSrj[3] = Edit14;
	EdNimi[3] = Edit15;
	EdLA[3] = Edit16;
	EdTrk[3] = Edit17;
	EdTls[3] = Edit18;
	EdNo[4] = Edit19;
	EdSrj[4] = Edit20;
	EdNimi[4] = Edit21;
	EdLA[4] = Edit22;
	EdTrk[4] = Edit23;
	EdTls[4] = Edit24;
	EdNo[5] = Edit25;
	EdSrj[5] = Edit26;
	EdNimi[5] = Edit27;
	EdLA[5] = Edit28;
	EdTrk[5] = Edit29;
	EdTls[5] = Edit30;
	EdNo[6] = Edit31;
	EdSrj[6] = Edit32;
	EdNimi[6] = Edit33;
	EdLA[6] = Edit34;
	EdTrk[6] = Edit35;
	EdTls[6] = Edit36;
	EdNo[7] = Edit37;
	EdSrj[7] = Edit38;
	EdNimi[7] = Edit39;
	EdLA[7] = Edit40;
	EdTrk[7] = Edit41;
	EdTls[7] = Edit42;
	EdNo[8] = Edit43;
	EdSrj[8] = Edit44;
	EdNimi[8] = Edit45;
	EdLA[8] = Edit46;
	EdTrk[8] = Edit47;
	EdTls[8] = Edit48;
	EdNo[9] = Edit49;
	EdSrj[9] = Edit50;
	EdNimi[9] = Edit51;
	EdLA[9] = Edit52;
	EdTrk[9] = Edit53;
	EdTls[9] = Edit54;
	EdNo[10] = Edit55;
	EdSrj[10] = Edit56;
	EdNimi[10] = Edit57;
	EdLA[10] = Edit58;
	EdTrk[10] = Edit59;
	EdTls[10] = Edit60;
	EdNo[11] = Edit61;
	EdSrj[11] = Edit62;
	EdNimi[11] = Edit63;
	EdLA[11] = Edit64;
	EdTrk[11] = Edit65;
	EdTls[11] = Edit66;
	EdNo[12] = Edit67;
	EdSrj[12] = Edit68;
	EdNimi[12] = Edit69;
	EdLA[12] = Edit70;
	EdTrk[12] = Edit71;
	EdTls[12] = Edit72;
	EdNo[13] = Edit73;
	EdSrj[13] = Edit74;
	EdNimi[13] = Edit75;
	EdLA[13] = Edit76;
	EdTrk[13] = Edit77;
	EdTls[13] = Edit78;
	EdNo[14] = Edit79;
	EdSrj[14] = Edit80;
	EdNimi[14] = Edit81;
	EdLA[14] = Edit82;
	EdTrk[14] = Edit83;
	EdTls[14] = Edit84;
}
//---------------------------------------------------------------------------
void __fastcall TFormTarkMerk::EdNoChange(int i)
{
	int d;
	wchar_t buf[32];

	if ((d = getpos(_wtoi(EdNo[i]->Text.c_str()))) > 0) {
		kilptietue kilp;
		kilp.GETREC(d);
		EdSrj[i]->Text = UnicodeString(Sarjat[kilp.Sarja()].sarjanimi);
		EdNimi[i]->Text = UnicodeString(kilp.nimi(buf, 30, 0));
		EdLA[i]->Text = UnicodeString(aikatowstr_cols_n(buf, kilp.pv[k_pv].tlahto, t0, 0, 8));
		EdTrk[i]->Text = UnicodeString(kilp.tark(k_pv));
		EdTls[i]->Text = UnicodeString(aikatowstr_cols_n(buf, kilp.tulos_pv(k_pv), 0, 0, 8));
		}
	else {
		EdSrj[i]->Text = L"";
		EdNimi[i]->Text = L"";
		EdLA[i]->Text = L"";
		EdTrk[i]->Text = L"";
		EdTls[i]->Text = L"";
    }
}
//---------------------------------------------------------------------------
void __fastcall TFormTarkMerk::EdNo0Change(TObject *Sender)
{
	EdNoChange(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit1Change(TObject *Sender)
{
	EdNoChange(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit7Change(TObject *Sender)
{
	EdNoChange(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit13Change(TObject *Sender)
{
	EdNoChange(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit19Change(TObject *Sender)
{
	EdNoChange(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit25Change(TObject *Sender)
{
	EdNoChange(5);
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit31Change(TObject *Sender)
{
	EdNoChange(6);
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit37Change(TObject *Sender)
{
	EdNoChange(7);
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit43Change(TObject *Sender)
{
	EdNoChange(8);
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit49Change(TObject *Sender)
{
	EdNoChange(9);
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit55Change(TObject *Sender)
{
	EdNoChange(10);
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit61Change(TObject *Sender)
{
	EdNoChange(11);
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit67Change(TObject *Sender)
{
	EdNoChange(12);
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit73Change(TObject *Sender)
{
	EdNoChange(13);
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit79Change(TObject *Sender)
{
	EdNoChange(14);
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Button1Click(TObject *Sender)
{
	wchar_t kh, khs[7] = L"EP-TKH";

	kh = khs[CBTrk->ItemIndex];
//	if (ToimintaTila == 2 && kh == L'P') {
//		Application->MessageBoxW(L"Merkintä 'Poissa' voidaan tehdä vain esivalmistelutilassa, merkitse ei-lähteneeksi", L"Rajoitus", MB_OK);
//		return;
//		}
	EnterCriticalSection(&tall_CriticalSection);
	for (int i = 0; i < MERKEDITLKM; i++) {
		int d;
		if ((d = getpos(EdNo[i]->Text.ToInt())) > 0) {
			kilptietue kilp;
			kilp.GETREC(d);
			kilp.set_tark(kh, k_pv);
			kilp.tallenna(d, 0, 0, 0, 0);
			EdTrk[i]->Text = UnicodeString(kilp.tark(k_pv));
			}
		}
	LeaveCriticalSection(&tall_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Button2Click(TObject *Sender)
{
	for (int i = 0; i < MERKEDITLKM; i++) {
		EdNo[i]->Text = L"0";
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::EdNo0KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN &&	!Shift.Contains(ssShift) &&
		!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		EdNo[1]->SetFocus();
		Key = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit1KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN &&	!Shift.Contains(ssShift) &&
		!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		EdNo[2]->SetFocus();
		Key = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit7KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN &&	!Shift.Contains(ssShift) &&
		!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		EdNo[3]->SetFocus();
		Key = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit13KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN &&	!Shift.Contains(ssShift) &&
		!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		EdNo[4]->SetFocus();
		Key = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit19KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN &&	!Shift.Contains(ssShift) &&
		!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		EdNo[5]->SetFocus();
		Key = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit25KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN &&	!Shift.Contains(ssShift) &&
		!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		EdNo[6]->SetFocus();
		Key = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit31KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN &&	!Shift.Contains(ssShift) &&
		!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		EdNo[7]->SetFocus();
		Key = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit37KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN &&	!Shift.Contains(ssShift) &&
		!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		EdNo[8]->SetFocus();
		Key = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit43KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN &&	!Shift.Contains(ssShift) &&
		!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		EdNo[9]->SetFocus();
		Key = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit49KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN &&	!Shift.Contains(ssShift) &&
		!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		EdNo[10]->SetFocus();
		Key = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit55KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN &&	!Shift.Contains(ssShift) &&
		!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		EdNo[11]->SetFocus();
		Key = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit61KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN &&	!Shift.Contains(ssShift) &&
		!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		EdNo[12]->SetFocus();
		Key = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit67KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN &&	!Shift.Contains(ssShift) &&
		!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		EdNo[13]->SetFocus();
		Key = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit73KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN &&	!Shift.Contains(ssShift) &&
		!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		EdNo[14]->SetFocus();
		Key = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Edit79KeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
	if (Key == VK_RETURN &&	!Shift.Contains(ssShift) &&
		!Shift.Contains(ssCtrl) && !Shift.Contains(ssAlt)) {
		EdNo[0]->SetFocus();
		Key = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::EdNo0KeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r')
		Key = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::FormShow(TObject *Sender)
{
	CBVaihe->ItemIndex = k_pv+1;
	CBVaihe->Visible = ToimintaTila == 1;
	Label8->Visible = ToimintaTila == 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormTarkMerk::Button4Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

