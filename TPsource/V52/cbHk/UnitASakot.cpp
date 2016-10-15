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

#include "UnitASakot.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormASakot *FormASakot;

static CRITICAL_SECTION sakkoGrid_CriticalSection;

//---------------------------------------------------------------------------
__fastcall TFormASakot::TFormASakot(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormASakot::EditKnoChange(TObject *Sender)
{
	int i;
	wchar_t st[60], as[20];

	dKilp = getpos(_wtoi(EditKno->Text.c_str()));
	for (i = 0; i < 4; i++)
		EditS[i]->Text = L"";
	if (dKilp > 0) {
		Kilp.GETREC(dKilp);
		EditNimi->Text = Kilp.nimi(st, 59, false);
		if (kilpparam.kilplaji != L'P' || wcswcind(pvparam[k_pv].alalaji, L"HT") < 0) {
			if (Kilp.maali(0) != TMAALI0) {
				EditPiste->Text = L"M";
				EditAika->Text = aikatowstr_cols_n(as, Kilp.maali(0), t0, 0, 8);
				}
			else {
				if (kilpparam.kilplaji != L'P' || wcswcind(pvparam[k_pv].alalaji, L"HT") < 0) {
					for (i = Sarjat[Kilp.Sarja()].valuku[k_pv]; i > 0; i--) {
						if (Kilp.maali(i) != TMAALI0) {
							EditPiste->Text = i;
							EditAika->Text = aikatowstr_cols_n(as, Kilp.maali(i), t0, 0, 8);
							break;
							}
						}
					}
				if (i == 0) {
					EditPiste->Text = L"";
					EditAika->Text = L"";
					}
				}
            }
		for (i = 0; i < Sarjat[Kilp.Sarja()].lsak[k_pv]; i++) {
			if (Kilp.pv[k_pv].asakot[i] >= L'0')
				EditS[i]->Text = (int) (Kilp.pv[k_pv].asakot[i] - L'0');
			else
				EditS[i]->Text = L'-';
//				EditS[i]->Text = Kilp.pv[k_pv].asakot[i];
			EditS[i]->Visible = true;
			}
		for ( ; i < 4; i++)
			EditS[i]->Visible = false;
		if (kilpparam.lsakmax < nEdit) {
			EditS[nEdit-1]->Text = 20 * Kilp.pv[k_pv].sakko / SEK;
			EditS[nEdit-1]->Visible = true;
			}
		}
	else {
		EditNimi->Text = L"";
		EditPiste->Text = L"";
		EditAika->Text = L"";
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::EditKnoKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		Key = 0;
		if (dKilp) {
			int i;

			for (i = Sarjat[Kilp.Sarja()].lsak[k_pv] - 1; i >= 0; i--) {
				if (Kilp.pv[k_pv].asakot[i] >= L'0')
					break;
				}
			if (i == Sarjat[Kilp.Sarja()].lsak[k_pv] - 1) {
				if (kilpparam.lsakmax < nEdit)
					i = nEdit-2;
				else
					i = -1;
				}
			FocusControl(EditS[i+1]);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::BtnSeurClick(TObject *Sender)
{
	EditKno->Text =  _wtoi(EditKno->Text.c_str()) + 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::BtnEdellClick(TObject *Sender)
{
	EditKno->Text =  _wtoi(EditKno->Text.c_str()) - 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::EditS1KeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == L'\r') {
		Key = 0;
		for (int i = 1; i < 4; i++)
			if (EditS[i]->Visible) {
				FocusControl(EditS[i]);
				return;
				}
		FocusControl(BtnHyvaksy);
		}
}
//---------------------------------------------------------------------------



void __fastcall TFormASakot::EditS2KeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		Key = 0;
		for (int i = 2; i < 4; i++)
			if (EditS[i]->Visible) {
				FocusControl(EditS[i]);
				return;
				}
		FocusControl(BtnHyvaksy);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::EditS3KeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		Key = 0;
		if (EditS[3]->Visible) {
			FocusControl(EditS[3]);
			}
		else
			FocusControl(BtnHyvaksy);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::EditS4KeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		Key = 0;
		FocusControl(BtnHyvaksy);
		}

}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::BtnHyvaksyClick(TObject *Sender)
{
	wchar_t st[60] = L"";

	if (pvparam[k_pv].asakkolaji == 1 && kilpparam.maxasakko < 10) {
		for (int i = 0; i < Sarjat[Kilp.Sarja()].lsak[k_pv]; i++) {
			if (EditS[i]->Text.Length() > 0) {
				st[i] = EditS[i]->Text[1];
				if (st[i] < L'0' || st[i] > L'0' + kilpparam.maxasakko)
					st[i] = L'-';
				}
			else
				st[i] = L'-';
			}
		}
	else {
		int i;
		for (i = 0; i < Sarjat[Kilp.Sarja()].lsak[k_pv]; i++) {
			if (i > 0)
				wcscat(st, L"/");
			if (EditS[i]->Text.c_str()[0] >= L'0' && _wtoi(EditS[i]->Text.c_str()) <= kilpparam.maxasakko)
				wcscat(st, EditS[i]->Text.c_str());
			else
				wcscat(st, L"-");
			}
		if (kilpparam.lsakmax < nEdit) {
			wcscat(st, L"/");
			wcscat(st, EditS[nEdit-1]->Text.c_str());
			}
		}
	EnterCriticalSection(&tall_CriticalSection);
	Kilp.GETREC(dKilp);
	Kilp.tall_asakot(st);
	Kilp.tallenna(dKilp, 0, 0, 0, 0);
	LeaveCriticalSection(&tall_CriticalSection);
	FocusControl(EditKno);
}
//---------------------------------------------------------------------------


void __fastcall TFormASakot::FormResize(TObject *Sender)
{
	BtnHyvaksy->Top = ClientHeight - BtnHyvaksy->Height - 5 * Screen->PixelsPerInch / 96;
	BtnSeur->Top = BtnHyvaksy->Top;
	BtnEdell->Top = BtnHyvaksy->Top;
	BtnSulje->Top = BtnHyvaksy->Top;
	EditKno->Top = BtnHyvaksy->Top - EditKno->Height - 4 * Screen->PixelsPerInch / 96;
	EditNimi->Top = EditKno->Top;
	EditPiste->Top  = EditKno->Top;
	EditAika->Top = EditKno->Top;
	EditS1->Top = EditKno->Top;
	EditS2->Top = EditKno->Top;
	EditS3->Top = EditKno->Top;
	EditS4->Top = EditKno->Top;
	Label1->Top = EditKno->Top - Label1->Height - 3 * Screen->PixelsPerInch / 96;
	Label2->Top = Label1->Top;
	Label3->Top = Label1->Top;
	Label4->Top = Label1->Top;
	Label5->Top = Label1->Top;
	Label6->Top = Label1->Top;
	StringGrid1->Height = Label1->Top - StringGrid1->Top - 3 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------

MESSAGE void __fastcall TFormASakot::AddSakkoHandler(TMyMessage &msg)
{
	kilptietue kilp;
	wchar_t st[60];
	int i;

	kilp.GETREC(msg.wparam);
	EnterCriticalSection(&sakkoGrid_CriticalSection);
	if (StringGrid1->RowCount > 2 || _wtoi(StringGrid1->Cells[0][1].c_str()) > 0)
		StringGrid1->RowCount++;
	StringGrid1->Cells[0][StringGrid1->RowCount-1] = StringGrid1->RowCount-1;
	StringGrid1->Cells[1][StringGrid1->RowCount-1] = kilp.id();
	StringGrid1->Cells[2][StringGrid1->RowCount-1] = kilp.nimi(st, 59, false);
	StringGrid1->Cells[3][StringGrid1->RowCount-1] = kilp.asakkoStr();
	StringGrid1->Row = StringGrid1->RowCount-1;
	LeaveCriticalSection(&sakkoGrid_CriticalSection);
}
//---------------------------------------------------------------------------

void add_sakko(int d)
{
	if (FormASakot)
		SendMessage(FormASakot->Handle, WM_MYADDSAKKO, d, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::EditSExit(int paikka)
{
	EditS[paikka]->Color = clWindow;
	if (EditS[paikka]->Text.Length() == 0) {
		EditS[paikka]->Text = L"-";
		EditS[paikka]->Color = clYellow;
		return;
		}
	if (kilpparam.maxasakko < 10) {
		if (EditS[paikka]->Text.Length() > 1) {
			EditS[paikka]->Text = EditS[paikka]->Text.SubString(1, 1);
			EditS[paikka]->Color = clYellow;
			}
		if ((EditS[paikka]->Text[1] < L'0' || EditS[paikka]->Text[1] > L'9')
			&& EditS[paikka]->Text[1] != L'-') {
			EditS[paikka]->Text = L"-";
			EditS[paikka]->Color = clYellow;
			}
		if (EditS[paikka]->Text[1] > L'0' + kilpparam.maxasakko) {
			EditS[paikka]->Color = clYellow;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::EditS1Exit(TObject *Sender)
{
	EditSExit(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::EditS2Exit(TObject *Sender)
{
	EditSExit(1);
}
//---------------------------------------------------------------------------


void __fastcall TFormASakot::EditS3Exit(TObject *Sender)
{
	EditSExit(2);
}
//---------------------------------------------------------------------------
void __fastcall TFormASakot::EditS4Exit(TObject *Sender)
{
	EditSExit(3);
}
//---------------------------------------------------------------------------


void __fastcall TFormASakot::BtnSuljeClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::FormCreate(TObject *Sender)
{
	StringGrid1->ColWidths[2] = 200 * Screen->PixelsPerInch / 96;
	StringGrid1->ColWidths[3] = 80 * Screen->PixelsPerInch / 96;
	StringGrid1->Cells[0][0] = L"Rivi";
	StringGrid1->Cells[1][0] = L"KilpNo";
	StringGrid1->Cells[2][0] = L"Nimi";
	StringGrid1->Cells[3][0] = L"Sakot";
	EditS[0] = EditS1;
	EditS[1] = EditS2;
	EditS[2] = EditS3;
	EditS[3] = EditS4;
	nEdit = kilpparam.lsakmax;
	if (nEdit < 4 && pvparam[k_pv].asakkolaji > 2)
		nEdit++;
	Label6->Caption = L"  ";
	if (kilpparam.kilplaji == L'P' && pvparam[k_pv].alalaji == L'T') {
		Label3->Caption = L"Tark x10";
		Label6->Caption = L"Pit (10 cm)";
		}
	else if (kilpparam.kilplaji == L'P' && pvparam[k_pv].alalaji == 'H') {
		Label3->Caption = L"Tulos-1";
		Label6->Caption = L"Tulos-2";
		}
	else if (wcswcind(kilpparam.kilplaji, L"HT") >= 0)
		Label3->Caption = L"Sakot (sek)";
	for (int i = nEdit; i < 4; i++)
		EditS[i]->Visible = false;

	if (pvparam[k_pv].asakkolaji > 1 && kilpparam.lsakmax < 4) {
		EditS[0]->Width = 40 * Screen->PixelsPerInch / 96;
		if (nEdit == 2) {
			EditS[1]->Width = 40 * Screen->PixelsPerInch / 96;
			EditS[1]->Left = EditS[0]->Left + 60 * Screen->PixelsPerInch / 96;
			}
		if (nEdit == 3) {
			EditS[0]->Width = 35 * Screen->PixelsPerInch / 96;
			EditS[1]->Width = 35 * Screen->PixelsPerInch / 96;
			EditS[2]->Width = 35 * Screen->PixelsPerInch / 96;
			EditS[1]->Left = EditS[0]->Left + 41 * Screen->PixelsPerInch / 96;
			EditS[2]->Left = EditS[0]->Left + 82 * Screen->PixelsPerInch / 96;
			if (kilpparam.kilplaji == L'P' && pvparam[k_pv].alalaji == 'H') {
				Label3->Caption = L"Tls-1";
				Label6->Caption = L"Tls-2";
				Label7->Caption = L"Napa";
				Label7->Visible = true;
				Label7->Left = EditS[2]->Left;
				}
			Label6->Left = EditS[1]->Left;
			}
		}
	FormResize(Sender);
	InitializeCriticalSection(&sakkoGrid_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::FormShow(TObject *Sender)
{
	if (wcswcind(kilpparam.kilplaji, L"NP") >= 0)
		FormASakot->Caption = L"Sakkojen ja pisteiden syöttö";
}
//---------------------------------------------------------------------------

