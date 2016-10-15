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

#include "UnitJoukkueTilanne.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormJoukkueTilanne *FormJoukkueTilanne;
void joukkuepisteet(bool naytolle, tulostusparamtp *tulprm);
void joukkuetul(bool naytolle, int srj, tulostusparamtp *tulprm);

//---------------------------------------------------------------------------
__fastcall TFormJoukkueTilanne::TFormJoukkueTilanne(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	Memo1->Font->Name = L"Lucida Console";
	Memo1->Font->Size = 10;
	viimSj = 15;
	tlsLaji = 3;
	tunnTieto = L'J';
	joukkKoko = 3;
	minKoko = 3;
}
//---------------------------------------------------------------------------
void __fastcall TFormJoukkueTilanne::FormResize(TObject *Sender)
{
	Memo1->Width = ClientWidth - Memo1->Left - 4;
	Memo1->Height = ClientHeight - Memo1->Top - 4;
}
//---------------------------------------------------------------------------
void __fastcall TFormJoukkueTilanne::Button1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------
void __fastcall TFormJoukkueTilanne::Button2Click(TObject *Sender)
{
	if (Uudetpisteetsijoituksista1->Checked) {
		tulparam.optiot = 0x40 * wcswcind(tunnTieto, L"SLYJP") +
			(Voittajallelispiste1->Checked ? 0x200 : 0) +
			(Sarjankokorajoittaavoittajanpisteit1->Checked ? 0x400 : 0);
		tulparam.optiot2 = 16*viimSj;
		joukkuepisteet(true, &tulparam);
		}
	else {
		tulparam.optiot = 4 * tlsLaji + (Joukkuetietovaaditaan1->Checked ? 0x10 : 0) +
			0x40 * wcswcind(tunnTieto, L"SLYJP");
		tulparam.optiot2 = 16*joukkKoko + 4096*minKoko;
		joukkuetul(true, CBSarja->ItemIndex, &tulparam);
		}
	Memo1->Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueTilanne::FormShow(TObject *Sender)
{
	Button2Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueTilanne::Yhteisaika1Click(TObject *Sender)
{
	Yhteisaika1->Checked = true;
	Kilpailijoidenpisteist1->Checked = false;
	Sijalukusumma1->Checked = false;
	Uudetpisteetsijoituksista1->Checked = false;
	Voittajallelispiste1->Visible = false;
	Sarjankokorajoittaavoittajanpisteit1->Visible = Voittajallelispiste1->Visible;
	Joukkuetietovaaditaan1->Visible = !Voittajallelispiste1->Visible;
	if (CBSarja->ItemIndex < 0)
		CBSarja->ItemIndex = 0;
	CBSarja->Enabled = true;
	tlsLaji = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueTilanne::Kilpailijoidenpisteist1Click(TObject *Sender)
{
	Kilpailijoidenpisteist1->Checked = true;
	Yhteisaika1->Checked = false;
	Sijalukusumma1->Checked = false;
	Uudetpisteetsijoituksista1->Checked = false;
	Voittajallelispiste1->Visible = false;
	Sarjankokorajoittaavoittajanpisteit1->Visible = Voittajallelispiste1->Visible;
	Joukkuetietovaaditaan1->Visible = !Voittajallelispiste1->Visible;
	if (CBSarja->ItemIndex < 0)
		CBSarja->ItemIndex = 0;
	CBSarja->Enabled = true;
	tlsLaji = 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueTilanne::Sijalukusumma1Click(TObject *Sender)
{
	Sijalukusumma1->Checked = true;
	Yhteisaika1->Checked = false;
	Kilpailijoidenpisteist1->Checked = false;
	Uudetpisteetsijoituksista1->Checked = false;
	Voittajallelispiste1->Visible = false;
	Sarjankokorajoittaavoittajanpisteit1->Visible = Voittajallelispiste1->Visible;
	Joukkuetietovaaditaan1->Visible = !Voittajallelispiste1->Visible;
	if (CBSarja->ItemIndex < 0)
		CBSarja->ItemIndex = 0;
	CBSarja->Enabled = true;
	tlsLaji = 2;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueTilanne::Uudetpisteetsijoituksista1Click(TObject *Sender)
{
	Uudetpisteetsijoituksista1->Checked = true;
	Yhteisaika1->Checked = false;
	Kilpailijoidenpisteist1->Checked = false;
	Sijalukusumma1->Checked = false;
	Voittajallelispiste1->Visible = true;
	Sarjankokorajoittaavoittajanpisteit1->Visible = Voittajallelispiste1->Visible;
	Joukkuetietovaaditaan1->Visible = !Voittajallelispiste1->Visible;
	CBSarja->Enabled = false;
	CBSarja->ItemIndex = -1;
	CBSarja->Text = L"Koko kilpailu";
	tlsLaji = 3;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueTilanne::Sulje1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueTilanne::Pivit1Click(TObject *Sender)
{
	Button2Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueTilanne::Help1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Joukkuetulokset");
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueTilanne::Seura1Click(TObject *Sender)
{
	Seura1->Checked = true;
	Seuralyhenne1->Checked = false;
	Yhdistys1->Checked = false;
	Joukkuekentt1->Checked = false;
	Piiri1->Checked = false;
	tunnTieto = L'S';
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueTilanne::Seuralyhenne1Click(TObject *Sender)
{
	Seuralyhenne1->Checked = true;
	Seura1->Checked = false;
	Seuralyhenne1->Checked = false;
	Yhdistys1->Checked = false;
	Joukkuekentt1->Checked = false;
	Piiri1->Checked = false;
	tunnTieto = L'L';
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueTilanne::Yhdistys1Click(TObject *Sender)
{
	Yhdistys1->Checked = true;
	Seura1->Checked = false;
	Seuralyhenne1->Checked = false;
	Joukkuekentt1->Checked = false;
	Piiri1->Checked = false;
	tunnTieto = L'Y';
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueTilanne::Joukkuekentt1Click(TObject *Sender)
{
	Joukkuekentt1->Checked = true;
	Seura1->Checked = false;
	Seuralyhenne1->Checked = false;
	Yhdistys1->Checked = false;
	Piiri1->Checked = false;
	tunnTieto = L'J';
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueTilanne::Piiri1Click(TObject *Sender)
{
	Piiri1->Checked = true;
	Seura1->Checked = false;
	Seuralyhenne1->Checked = false;
	Yhdistys1->Checked = false;
	Joukkuekentt1->Checked = false;
	tunnTieto = L'P';
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueTilanne::Joukkueenkoko1Click(TObject *Sender)
{
	int k;
	wchar_t ch;

	if (tlsLaji == 3) {
		inputint_prompt(&viimSj, L"Anna pisteisiin oikeuttava huonoin sijoitus", &ch, this);
		return;
		}
	k = joukkKoko;
	inputint_prompt(&k, L"Anna joukkueen (maximi) koko", &ch, this);
	if (ch == ESC)
		return;
	joukkKoko = k;
	if (tlsLaji != 3) {
		k = minKoko;
		inputint_prompt(&k, L"Anna joukkueen minimikoko", &ch, this);
		if (ch != ESC)
			minKoko = k;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueTilanne::Voittajallelispiste1Click(TObject *Sender)
{
	Voittajallelispiste1->Checked = !Voittajallelispiste1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueTilanne::Sarjankokorajoittaavoittajanpisteit1Click(TObject *Sender)

{
	Sarjankokorajoittaavoittajanpisteit1->Checked = !Sarjankokorajoittaavoittajanpisteit1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueTilanne::Joukkuetietovaaditaan1Click(TObject *Sender)

{
	Joukkuetietovaaditaan1->Checked = !Joukkuetietovaaditaan1->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormJoukkueTilanne::FormCreate(TObject *Sender)
{
	CBSarja->Clear();
	for (int srj = 0; srj < sarjaluku+nsarjayhd; srj++) {
		CBSarja->Items->Add(UnicodeString(Sarjat[srj].sarjanimi));
		}
	CBSarja->ItemIndex = -1;
}
//---------------------------------------------------------------------------

