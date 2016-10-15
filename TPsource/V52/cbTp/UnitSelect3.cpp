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
#include <process.h>
#pragma hdrstop

#include "UnitSelect3.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

static TFormSelect3 *FormThis;
static int KestoAika;
static bool SelTimerThreadOn;

//---------------------------------------------------------------------------
__fastcall TFormSelect3::TFormSelect3(TComponent* Owner)
   : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormSelect3::Button1Click(TObject *Sender)
{
	SelTimerThreadOn = false;
	selResult = 1;
	 ModalResult = 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormSelect3::Button2Click(TObject *Sender)
{
	SelTimerThreadOn = false;
	selResult = 2;
	 ModalResult = 2;

}
//---------------------------------------------------------------------------

void __fastcall TFormSelect3::Button3Click(TObject *Sender)
{
	SelTimerThreadOn = false;
	selResult = 3;
	 ModalResult = 3;
}
//---------------------------------------------------------------------------

void __fastcall TFormSelect3::Kysy(int nButton, wchar_t *Teksti, wchar_t *Otsikko,
		wchar_t *Vast1, wchar_t *Vast2, wchar_t *Vast3, int Kesto /* = 0 */)
{
	selResult = 0;
	KestoAika = Kesto;
	Caption = Otsikko;
	Sanoma->Text = Teksti;
	if (wcslen(Vast1) > 20)
		Button1->Font->Size = 8;
	Button1->Caption = Vast1;
	if (nButton == 1) {
		Button2->Visible = false;
		Button2->TabStop = false;
		Button3->Visible = false;
		Button3->TabStop = false;
		Button1->Left = (Button1->Left + Button3->Left) / 2;
		}
	else {
		if (wcslen(Vast2) > 20)
			Button2->Font->Size = 8;
		Button2->Caption = Vast2;
		 if (nButton == 2) {
			 Button3->Visible = false;
			 Button3->TabStop = false;
			 Button1->Left = (Button1->Left + Button2->Left) / 2;
			 Button2->Left = (Button2->Left + Button3->Left) / 2;
			 }
		 else  {
			if (wcslen(Vast3) > 20)
				Button3->Font->Size = 8;
			Button3->Caption = Vast3;
			}
		}
}
//---------------------------------------------------------------------------

MESSAGE void __fastcall TFormSelect3::SelTimerHandler(TMyMessage2 &msg)
{
	Button1Click(this);
}
//---------------------------------------------------------------------------

void SelTimerThread(LPVOID lpCn)
	{
	int wait = *(int *) lpCn;

	SelTimerThreadOn = true;
	for (int i = 0; SelTimerThreadOn && 200*i < wait; i++) {
		Sleep(200);
		}
	if (SelTimerThreadOn) {
		SendMessage(FormThis->Handle,WM_MYSELTIMER,0,NULL);
		}
	}
//---------------------------------------------------------------------------

void __fastcall TFormSelect3::FormShow(TObject *Sender)
{
	FormThis = this;
	Button1->SetFocus();
	if (KestoAika) {
		_beginthread(SelTimerThread, 10240, &KestoAika);
	}
}

//---------------------------------------------------------------------------

