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

#include "UnitASuunn.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormASuunn *FormASuunn;

static CRITICAL_SECTION sakkoGrid_CriticalSection;

//---------------------------------------------------------------------------
__fastcall TFormASuunn::TFormASuunn(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
		}
	EditS[0] = EditA1;
	EditS[1] = EditA2;
	EditS[2] = EditAX;
	EditS[3] = EditKl;
	EditS[4] = EditKlX;
	InitializeCriticalSection(&sakkoGrid_CriticalSection);
}
//---------------------------------------------------------------------------

void __fastcall TFormASuunn::FormCreate(TObject *Sender)
{
	StringGrid1->ColWidths[2] = 200 * Screen->PixelsPerInch / 96;
	StringGrid1->ColWidths[3] = 45 * Screen->PixelsPerInch / 96;
	StringGrid1->ColWidths[4] = 45 * Screen->PixelsPerInch / 96;
	StringGrid1->ColWidths[5] = 45 * Screen->PixelsPerInch / 96;
	StringGrid1->ColWidths[6] = 45 * Screen->PixelsPerInch / 96;
	StringGrid1->ColWidths[7] = 45 * Screen->PixelsPerInch / 96;
	StringGrid1->Cells[0][0] = L"Rivi";
	StringGrid1->Cells[1][0] = L"KilpNo";
	StringGrid1->Cells[2][0] = L"Nimi";
	StringGrid1->Cells[3][0] = L"Amm-1";
	StringGrid1->Cells[4][0] = L"Amm-2";
	StringGrid1->Cells[5][0] = L"Amm-X";
	StringGrid1->Cells[6][0] = L"Kl-mm";
	StringGrid1->Cells[7][0] = L"Kl-X";
}
//---------------------------------------------------------------------------

static int AmpPLkm(kilptietue &kilp)
{
	int n = 0;
	for (int i = 0; i < Sarjat[kilp.id()].lsak[k_pv]; i++)
		if (Sarjat[kilp.id()].asuunnsakot[k_pv][i] == L'S')
			n++;
	return(n);
}
//---------------------------------------------------------------------------

void __fastcall TFormASuunn::EditKnoChange(TObject *Sender)
{
	int i;
	wchar_t st[60], as[20];

	dKilp = getpos(_wtoi(EditKno->Text.c_str()));
	for (i = 0; i < 5; i++)
		EditS[i]->Text = L"";
	if (dKilp > 0) {
		Kilp.GETREC(dKilp);
		EditA2->Enabled = AmpPLkm(Kilp) > 1;
		EditNimi->Text = Kilp.nimi(st, 59, false);
		if (Kilp.maali(0) != TMAALI0) {
			EditPiste->Text = L"M";
			EditAika->Text = aikatowstr_cols_n(as, Kilp.maali(0), t0, 0, 8);
			}
		else {
			for (i = Sarjat[Kilp.Sarja()].valuku[k_pv]; i > 0; i--) {
				if (Kilp.maali(i) != TMAALI0) {
					EditPiste->Text = i;
					EditAika->Text = aikatowstr_cols_n(as, Kilp.maali(i), t0, 0, 8);
					break;
					}
				}
			if (i == 0) {
				EditPiste->Text = L"";
				EditAika->Text = L"";
				}
			}
		for (i = 0; i < 5; i++) {
			if (Kilp.pv[k_pv].asakot[i] >= L'0')
				EditS[i]->Text = (int) (Kilp.pv[k_pv].asakot[i] - L'0');
			else
				EditS[i]->Text = L'-';
			}
		}
	else {
		EditNimi->Text = L"";
		EditPiste->Text = L"";
		EditAika->Text = L"";
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormASuunn::EditKnoKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		Key = 0;
		if (dKilp) {
			int i;

			if (EditS[0]->Text == L'-')
				FocusControl(EditS[0]);
			else
				FocusControl(EditS[3]);
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormASuunn::Button2Click(TObject *Sender)
{
	EditKno->Text =  _wtoi(EditKno->Text.c_str()) + 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormASuunn::Button3Click(TObject *Sender)
{
	EditKno->Text =  _wtoi(EditKno->Text.c_str()) - 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormASuunn::EditS1KeyPress(TObject *Sender, System::WideChar &Key)
{

	if (Key == L'\r') {
		Key = 0;
		if (EditS[1]->Enabled)
			FocusControl(EditS[1]);
		else
			FocusControl(Button1);
		}
}
//---------------------------------------------------------------------------



void __fastcall TFormASuunn::EditS2KeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		Key = 0;
		FocusControl(Button1);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormASuunn::EditS3KeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		Key = 0;
		FocusControl(Button1);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormASuunn::EditKlKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L'\r') {
		Key = 0;
		FocusControl(Button1);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormASuunn::EditKlXKeyPress(TObject *Sender, System::WideChar &Key)
{
	if (Key == L'\r') {
		Key = 0;
		FocusControl(Button1);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormASuunn::Button1Click(TObject *Sender)
{
	wchar_t st[60] = L"";

	for (int i = 0; i < 5; i++) {
		if (EditS[i]->Text.c_str()[0] >= L'0') {
			wcscat(st, EditS[i]->Text.c_str());
//			st[i] = L'0' + _wtoi(EditS[i]->Text.c_str());
			}
		else {
			wcscat(st, L"-");
//			st[i] = L'-';
			}
		wcscat(st, L"/");
		}
	EnterCriticalSection(&tall_CriticalSection);
	Kilp.GETREC(dKilp);
	Kilp.tall_asakot(st);
	Kilp.tallenna(dKilp, 0, 0, 0, 0);
	LeaveCriticalSection(&tall_CriticalSection);
	EnterCriticalSection(&sakkoGrid_CriticalSection);
	if (StringGrid1->RowCount > 2 || _wtoi(StringGrid1->Cells[0][1].c_str()) > 0)
		StringGrid1->RowCount++;
	StringGrid1->Cells[0][StringGrid1->RowCount-1] = StringGrid1->RowCount-1;
	StringGrid1->Cells[1][StringGrid1->RowCount-1] = Kilp.id();
	StringGrid1->Cells[2][StringGrid1->RowCount-1] = Kilp.nimi(st, 59, false);
	for (int c = 3; c < 8; c++) {
		StringGrid1->Cells[c][StringGrid1->RowCount-1] =
			Kilp.pv[k_pv].asakot[c-3] >= L'0' ? UnicodeString((int)(Kilp.pv[k_pv].asakot[c-3] - L'0')) : UnicodeString(L"-");
		}
	StringGrid1->Row = StringGrid1->RowCount-1;
	LeaveCriticalSection(&sakkoGrid_CriticalSection);
	FocusControl(EditKno);
}
//---------------------------------------------------------------------------


void __fastcall TFormASuunn::FormResize(TObject *Sender)
{
	EditKno->Top = ClientHeight - 75 * Screen->PixelsPerInch / 96;
	EditNimi->Top = EditKno->Top;
	EditPiste->Top  = EditKno->Top;
	EditAika->Top = EditKno->Top;
	EditA1->Top = EditKno->Top;
	EditA2->Top = EditKno->Top;
	EditAX->Top = EditKno->Top;
	EditKl->Top = EditKno->Top;
	EditKlX->Top = EditKno->Top;
	Label1->Top = EditKno->Top - 17 * Screen->PixelsPerInch / 96;
	Label2->Top = Label1->Top - 14 * Screen->PixelsPerInch / 96;
	Label3->Top = Label2->Top;
	Label4->Top = Label1->Top;
	Label5->Top = Label1->Top;
	Label6->Top = Label1->Top;
	Label7->Top = Label1->Top;
	Label8->Top = Label1->Top;
	Button1->Top = ClientHeight - 35 * Screen->PixelsPerInch / 96;
	Button2->Top = Button1->Top;
	Button3->Top = Button1->Top;
	Button4->Top = Button1->Top;
	StringGrid1->Height = ClientHeight - 110 * Screen->PixelsPerInch / 96;
	StringGrid1->Width = ClientWidth;
}
//---------------------------------------------------------------------------

MESSAGE void __fastcall TFormASuunn::AddSakkoHandler(TMyMessage &msg)
{
	kilptietue kilp;
	wchar_t st[60];

	kilp.GETREC(msg.wparam);
	EnterCriticalSection(&sakkoGrid_CriticalSection);
	if (StringGrid1->RowCount > 2 || _wtoi(StringGrid1->Cells[0][1].c_str()) > 0)
		StringGrid1->RowCount++;
	StringGrid1->Cells[0][StringGrid1->RowCount-1] = StringGrid1->RowCount-1;
	StringGrid1->Cells[1][StringGrid1->RowCount-1] = kilp.id();
	StringGrid1->Cells[2][StringGrid1->RowCount-1] = kilp.nimi(st, 59, false);
	StringGrid1->Cells[3][StringGrid1->RowCount-1] = (int) (kilp.pv[k_pv].asakot[0]-L'-');
	StringGrid1->Cells[4][StringGrid1->RowCount-1] = (int) (kilp.pv[k_pv].asakot[1]-L'-');
	StringGrid1->Cells[5][StringGrid1->RowCount-1] = (int) (kilp.pv[k_pv].asakot[2]-L'-');
	StringGrid1->Cells[6][StringGrid1->RowCount-1] = (int) (kilp.pv[k_pv].asakot[3]-L'-');
	StringGrid1->Cells[7][StringGrid1->RowCount-1] = (int) (kilp.pv[k_pv].asakot[4]-L'-');
	StringGrid1->Row = StringGrid1->RowCount-1;
	LeaveCriticalSection(&sakkoGrid_CriticalSection);
}
//---------------------------------------------------------------------------

void add_sakko(int d)
{
	if (FormASuunn)
		SendMessage(FormASuunn->Handle, WM_MYADDSAKKO, d, 0);
}
//---------------------------------------------------------------------------

void __fastcall TFormASuunn::EditSExit(int paikka)
{
	EditS[paikka]->Color = clWindow;
	if (EditS[paikka]->Text.Length() == 0) {
		EditS[paikka]->Text = L"-";
		EditS[paikka]->Color = clYellow;
		return;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormASuunn::EditA1Exit(TObject *Sender)
{
	EditSExit(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormASuunn::EditA2Exit(TObject *Sender)
{
	EditSExit(1);
}
//---------------------------------------------------------------------------


void __fastcall TFormASuunn::EditAXExit(TObject *Sender)
{
	EditSExit(2);
}
//---------------------------------------------------------------------------
void __fastcall TFormASuunn::EditKlExit(TObject *Sender)
{
	EditSExit(3);
}
//---------------------------------------------------------------------------
void __fastcall TFormASuunn::EditKlXExit(TObject *Sender)
{
	EditSExit(4);
}
//---------------------------------------------------------------------------

void __fastcall TFormASuunn::Button4Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------



