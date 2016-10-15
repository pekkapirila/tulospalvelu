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
	StringGrid1->ColWidths[3] = 200 * Screen->PixelsPerInch / 96;
	StringGrid1->Cells[0][0] = L"Rivi";
	StringGrid1->Cells[1][0] = L"KilpNo";
	StringGrid1->Cells[2][0] = L"Osuus";
	StringGrid1->Cells[3][0] = L"Joukkue";
	StringGrid1->Cells[4][0] = L"Sakot";
	EditS[0] = EditS1;
	EditS[1] = EditS2;
	EditS[2] = EditS3;
	EditS[3] = EditS4;
	InitializeCriticalSection(&sakkoGrid_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::EditKnoExit(TObject *Sender)
{
	dKilp = getpos(_wtoi(EditKno->Text.c_str()));
	if (dKilp > 0) {
		Kilp.getrec(dKilp);
		if (Kilp.kilpstatus != 0) {
			dKilp = 0;
			}
		else {
			Osuus = Sarjat[Kilp.sarja].osuusluku;
			for (int os = 0; os < Sarjat[Kilp.sarja].osuusluku; os++) {
				int i;
				for (i = 0; i < Sarjat[Kilp.sarja].paikat[os]; i++)
					if (Kilp.Ampsakot(os)[i] < L'0')
						break;
				if (i < Sarjat[Kilp.sarja].paikat[os]) {
					Osuus = os;
					os = Sarjat[Kilp.sarja].osuusluku;
					break;
					}
				}
			}
		}
	NaytaKilp();
}
//---------------------------------------------------------------------------
void __fastcall TFormASakot::NaytaKilp(void)
{
	int i;
	wchar_t st[60], as[20];

	for (i = 0; i < 4; i++) {
		EditS[i]->Text = L"";
		EditS[i]->Color = clWindow;
		}
	if (dKilp > 0) {
		if (Osuus >= Sarjat[Kilp.sarja].osuusluku) {
			Osuus = Sarjat[Kilp.sarja].osuusluku - 1;
			}
		EditOsuus->Text = Osuus+1;
		EditNimi->Text = Kilp.Joukkue(st, 30);
		if (Kilp.Maali(Osuus, 0) != TMAALI0) {
			EditPiste->Text = L"M";
			EditAika->Text = aikatowstr_cols_n(as, Kilp.Maali(Osuus, 0), t0, 0, 8);
			}
		else {
			for (i = Sarjat[Kilp.sarja].valuku[Sarjat[Kilp.sarja].yosuus[Osuus]]; i > 0; i--) {
				if (Kilp.Maali(Osuus, i)) {
					EditPiste->Text = i;
					EditAika->Text = aikatowstr_cols_n(as, Kilp.Maali(Osuus, i), t0, 0, 8);
					break;
					}
				}
			if (i == 0) {
				EditPiste->Text = L"";
				EditAika->Text = L"";
				}
			}
		for (i = 0; i < Sarjat[Kilp.sarja].paikat[Osuus]; i++) {
			EditS[i]->Visible = true;
			EditS[i]->Text = Kilp.Ampsakot(Osuus, st)[i];
			}
		for (i = Sarjat[Kilp.sarja].paikat[Osuus]; i < 4; i++)
			EditS[i]->Visible = false;
		}
	else {
		EditNimi->Text = L"";
		EditPiste->Text = L"";
		EditAika->Text = L"";
		EditOsuus->Text = 0;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::EditKnoKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		Key = 0;
		FocusControl(EditOsuus);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::EditOsuusExit(TObject *Sender)
{
	Osuus = _wtoi(EditOsuus->Text.c_str()) - 1;
	if (Osuus < 0)
		Osuus = 0;
	NaytaKilp();
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::EditOsuusKeyPress(TObject *Sender, System::WideChar &Key)
{
	wchar_t st[20];

	if (Key == L'\r') {
		Key = 0;
		if (dKilp && Osuus < Sarjat[Kilp.sarja].osuusluku) {
			int i;

			for (i = Sarjat[Kilp.sarja].paikat[Osuus] - 1; i >= 0; i--) {
				if (Kilp.Ampsakot( Osuus, st)[i] >= L'0')
					break;
				}
			if (i == Sarjat[Kilp.sarja].paikat[Osuus] - 1)
				i = -1;
			if (Kilp.Ampsakot( Osuus, st)[i+1] >= L'0')
				EditS[i+1]->Color = clRed;
			FocusControl(EditS[i+1]);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::Button2Click(TObject *Sender)
{
	EditKno->Text =  _wtoi(EditKno->Text.c_str()) + 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::Button3Click(TObject *Sender)
{
	EditKno->Text =  _wtoi(EditKno->Text.c_str()) - 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::EditS1KeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == L'\r') {
		Key = 0;
		if (Sarjat[Kilp.sarja].paikat[Osuus] > 1)
			FocusControl(EditS[1]);
		else
			FocusControl(Button1);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::EditS2KeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		Key = 0;
		if (Sarjat[Kilp.sarja].paikat[Osuus] > 2)
			FocusControl(EditS[2]);
		else
			FocusControl(Button1);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::EditS3KeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		Key = 0;
		if (Sarjat[Kilp.sarja].paikat[Osuus] > 3)
			FocusControl(EditS[3]);
		else
			FocusControl(Button1);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::EditS4KeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == L'\r') {
		Key = 0;
		FocusControl(Button1);
		}

}
//---------------------------------------------------------------------------

void __fastcall TFormASakot::Button1Click(TObject *Sender)
{
	wchar_t st[60] = L"";

	for (int i = 0; i < Sarjat[Kilp.sarja].paikat[Osuus]; i++)
	   st[i] = EditS[i]->Text[1];
	EnterCriticalSection(&tall_CriticalSection);
	Kilp.getrec(dKilp);
	Kilp.setAmpsakot(Osuus, st);
	Kilp.Tallenna(dKilp, 0, 0, 0, 0);
	LeaveCriticalSection(&tall_CriticalSection);
	EnterCriticalSection(&sakkoGrid_CriticalSection);
	if (StringGrid1->RowCount > 2 || _wtoi(StringGrid1->Cells[0][1].c_str()) > 0)
		StringGrid1->RowCount++;
	StringGrid1->Cells[0][StringGrid1->RowCount-1] = StringGrid1->RowCount-1;
	StringGrid1->Cells[1][StringGrid1->RowCount-1] = Kilp.kilpno;
	StringGrid1->Cells[2][StringGrid1->RowCount-1] = Osuus + 1;
	StringGrid1->Cells[3][StringGrid1->RowCount-1] = Kilp.Joukkue(st, 30, false);
	memset(st, 0, sizeof(st));
	Kilp.Ampsakot(Osuus, st);
	StringGrid1->Cells[4][StringGrid1->RowCount-1] = st;
	StringGrid1->Row = StringGrid1->RowCount-1;
	LeaveCriticalSection(&sakkoGrid_CriticalSection);
	FocusControl(EditKno);
}
//---------------------------------------------------------------------------


void __fastcall TFormASakot::FormResize(TObject *Sender)
{
	EditKno->Top = ClientHeight - 75 * Screen->PixelsPerInch / 96;
	EditOsuus->Top = EditKno->Top;
	EditNimi->Top = EditKno->Top;
	EditPiste->Top  = EditKno->Top;
	EditAika->Top = EditKno->Top;
	EditS1->Top = EditKno->Top;
	EditS2->Top = EditKno->Top;
	EditS3->Top = EditKno->Top;
	EditS4->Top = EditKno->Top;
	Label1->Top = EditKno->Top + EditKno->Height + 2 * Screen->PixelsPerInch / 96;
	Label2->Top = Label1->Top;
	Label3->Top = Label1->Top;
	Button1->Top = ClientHeight - 35 * Screen->PixelsPerInch / 96;
	Button2->Top = Button1->Top;
	Button3->Top = Button1->Top;
	Button4->Top = Button1->Top;
	StringGrid1->Height = ClientHeight - 97 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------

MESSAGE void __fastcall TFormASakot::AddSakkoHandler(TMyMessage &msg)
{
	kilptietue kilp;
	wchar_t st[60];

	kilp.getrec(msg.wparam);
	EnterCriticalSection(&sakkoGrid_CriticalSection);
	if (StringGrid1->RowCount > 2 || _wtoi(StringGrid1->Cells[0][1].c_str()) > 0)
		StringGrid1->RowCount++;
	StringGrid1->Cells[0][StringGrid1->RowCount-1] = StringGrid1->RowCount-1;
	StringGrid1->Cells[1][StringGrid1->RowCount-1] = kilp.kilpno;
	StringGrid1->Cells[2][StringGrid1->RowCount-1] = Osuus;
	StringGrid1->Cells[3][StringGrid1->RowCount-1] = kilp.Joukkue(st, 40, false);
	StringGrid1->Cells[4][StringGrid1->RowCount-1] = kilp.Ampsakot(Osuus, st);
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
	if (EditS[paikka]->Text.Length() > 1) {
		EditS[paikka]->Text = EditS[paikka]->Text.SubString(1, 1);
		EditS[paikka]->Color = clYellow;
		}
	if ((EditS[paikka]->Text[1] < L'0' || EditS[paikka]->Text[1] > L'9')
		&& EditS[paikka]->Text[1] != L'-') {
		EditS[paikka]->Text = L"-";
		EditS[paikka]->Color = clYellow;
		}
	if (EditS[paikka]->Text[1] > L'5') {
		EditS[paikka]->Color = clYellow;
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


void __fastcall TFormASakot::Button4Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

