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

#include "UnitVaiheMaar.h"
#include "HkDeclare.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormVaiheMaar *FormVaiheMaar;
static wchar_t *Pr[4] = {L"1", L"0,1", L"0,01", L"0,001"};
extern int PyorIx(int pyor);

//---------------------------------------------------------------------------
__fastcall TFormVaiheMaar::TFormVaiheMaar(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormVaiheMaar::FormShow(TObject *Sender)
{
	SG1->ColCount = kilpparam.n_pv_akt + 1;
	SG1->RowCount = 7;
	SG1->ColWidths[0] = 100 * Screen->PixelsPerInch / 96;
	SG1->Cells[0][1] = L"Id-koodi";
	SG1->Cells[0][2] = L"Päivämäärä";
	SG1->Cells[0][3] = L"Otsikko";
	SG1->Cells[0][4] = L"Sij.-tarkkuus";
	SG1->Cells[0][5] = L"Tulostarkkuus";
	SG1->Cells[0][6] = L"Lähtöportti";
//	SG1->Cells[0][7] = L"Takaa-ajon aikalisä";
	Paivita();
}
//---------------------------------------------------------------------------

void __fastcall TFormVaiheMaar::Button2Click(TObject *Sender)
{
	Paivita();
}
//---------------------------------------------------------------------------

void TFormVaiheMaar::Paivita()
{
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		SG1->ColWidths[ipv+1] = 120 * Screen->PixelsPerInch / 96;
		SG1->Cells[ipv+1][0] = UnicodeString(ipv+1) + L". vaihe";
		SG1->Cells[ipv+1][1] = pvparam[ipv].RaceId;
		SG1->Cells[ipv+1][2] = stDateNo(NULL, pvparam[ipv].Date);
		SG1->Cells[ipv+1][3] = pvparam[ipv].Title;
		SG1->Cells[ipv+1][4] = Pr[PyorIx(pvparam[ipv].pyor[2])];
		SG1->Cells[ipv+1][5] = Pr[PyorIx(pvparam[ipv].pyor[3])];
		SG1->Cells[ipv+1][6] = pvparam[ipv].hiihtolahto ? UnicodeString(L"On") : UnicodeString(L"Ei");
//		SG1->Cells[ipv+1][7] = pvparam[ipv].takaaAjoLisa ? UnicodeString(L"On") : UnicodeString(L"Ei");
		}
}
//---------------------------------------------------------------------------

static int pyorVal(wchar_t *st)
{
	wchar_t *p;
	int val = 0;

	if (st[0] == L'1')
		return(SEK);
	for (p = st; *p && *p < L'A'; p++) {
		if (*p == L',' || *p == L'.')
			val = SEK/10;
		if (*p == L'0')
			val /= 10;
		if (*p == L'1')
			break;
		}
	if (val == 0)
		val = SEK;
	return(val);
}
//---------------------------------------------------------------------------

void __fastcall TFormVaiheMaar::Button1Click(TObject *Sender)
{
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		wcsncpy(pvparam[ipv].RaceId, SG1->Cells[ipv+1][1].c_str(), sizeof(pvparam[ipv].RaceId)/2-1);
		pvparam[ipv].Date = DateNoFromSt(SG1->Cells[ipv+1][2].c_str());
		wcsncpy(pvparam[ipv].Title, SG1->Cells[ipv+1][3].c_str(), sizeof(pvparam[ipv].Title)/2-1);
		pvparam[ipv].pyor[2] = pyorVal(SG1->Cells[ipv+1][4].c_str());;
		pvparam[ipv].pyor[3] = pyorVal(SG1->Cells[ipv+1][5].c_str());;
		pvparam[ipv].hiihtolahto = (towupper(SG1->Cells[ipv+1][6].c_str()[0]) == L'O' ? 1 : 0);
//		pvparam[ipv].takaaAjoLisa = (towupper(SG1->Cells[ipv+1][7].c_str()[0]) == L'O' ? 1 : 0);
		}
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormVaiheMaar::FormClose(TObject *Sender, TCloseAction &Action)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TFormVaiheMaar::FormResize(TObject *Sender)
{
	SG1->Width = ClientWidth - 2*SG1->Left;
}
//---------------------------------------------------------------------------
void TFormVaiheMaar::PaivitaCell(int row, int col)
{
	if (row > 0 && col > 0) {
		switch (row) {
			case 1:
			case 3:
				SG1->Cells[col][row] = SG1->Cells[col][row].Trim();
				break;
			case 2:
				SG1->Cells[col][row] = stDateNo(NULL, DateNoFromSt(SG1->Cells[col][row].c_str()));
				break;
			case 4:
			case 5:
				SG1->Cells[col][row] = Pr[PyorIx(pyorVal(SG1->Cells[col][row].c_str()))];
				break;
			case 6:
//			case 7:
				SG1->Cells[col][row] = towupper(SG1->Cells[col][row].c_str()[0]) == L'O' ? L"On" : L"Ei";
				break;
		}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormVaiheMaar::SG1SelectCell(TObject *Sender, int ACol, int ARow,
		  bool &CanSelect)
{
	PaivitaCell(aktRow, aktCol);
	aktRow = ARow;
	aktCol = ACol;
}
//---------------------------------------------------------------------------

void __fastcall TFormVaiheMaar::SG1Exit(TObject *Sender)
{
	PaivitaCell(aktRow, aktCol);
	aktRow = 0;
	aktCol = 0;
}
//---------------------------------------------------------------------------

