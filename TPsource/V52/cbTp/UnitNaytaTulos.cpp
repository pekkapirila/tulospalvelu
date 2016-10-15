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

#include <process.h>
#include "UnitNaytaTulos.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormNaytaTulos *FormNaytaTulos;

#ifdef MAXOSUUSLUKU
static bool vaiheenvaihto = false;
#endif

//---------------------------------------------------------------------------
__fastcall TFormNaytaTulos::TFormNaytaTulos(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	Kesto[0] = 6;
	Kesto[1] = 6;
#ifndef MAXOSUUSLUKU
	Esiluennanlhtraja1->Visible = true;
	IkkParam.LahtoRaja = 60;
#endif
}
//---------------------------------------------------------------------------
NaytaTulosIkkParamClass::NaytaTulosIkkParamClass(void)
{
	memset(this, 0, sizeof(emitIkkParamClass));
}
//---------------------------------------------------------------------------
void NaytaTulosIkkParamClass::operator=(NaytaTulosIkkParamClass& ikkPar)
{
	memcpy(this, &ikkPar, sizeof(NaytaTulosIkkParamClass));
}
//---------------------------------------------------------------------------
void TFormNaytaTulos::haeParams(void)
{
	IkkParam.NaytaTulos = (Nyttulosjasija1->Checked ? 1 : 0);
	IkkParam.OkKesto = Kesto[0];
	IkkParam.OngKesto = Kesto[1];
	for (int r = 0; r < sizeof(IkkParam.Teksti) / sizeof(IkkParam.Teksti[0]); r++) {
		if (r < Memo1->Lines->Count) {
			wcsncpy(IkkParam.Teksti[r], Memo1->Lines->Strings[r].c_str(), sizeof(IkkParam.Teksti[0])/2-1);
			wtrimline(IkkParam.Teksti[r]);
			}
		else
			IkkParam.Teksti[r][0] = 0;
		}
}
//---------------------------------------------------------------------------

int TFormNaytaTulos::applyParams(void)
{
	Nyttulosjasija1->Checked= IkkParam.NaytaTulos;
	Kesto[0] = IkkParam.OkKesto;
	Kesto[0] = IkkParam.OngKesto;
	Memo1->Clear();
	for (int r = 0; r < sizeof(IkkParam.Teksti) / sizeof(IkkParam.Teksti[0]); r++) {
		if (IkkParam.Teksti[r]) {
			for (int i = Memo1->Lines->Count; i < r+1; i++) {
				if (i < r)
					Memo1->Lines->Add(L"");
				else
					Memo1->Lines->Add(IkkParam.Teksti[r]);
				}
			}
		}
	return(0);
}
//---------------------------------------------------------------------------
static bool NaytaTulosTimerThreadOn;

void NaytaTulosTimerThread(LPVOID lpCn)
	{
	NaytaTulosTimerThreadOn = true;

	while (taustaon && !vaiheenvaihto && lkmThreadOn && NaytaTulosTimerThreadOn) {
		Sleep(200);
		SendMessage(FormNaytaTulos->Handle,WM_MYDISPLTIMER,0,0);
	   }
	NaytaTulosTimerThreadOn = false;
	}
//---------------------------------------------------------------------------
void __fastcall TFormNaytaTulos::NaytaTulos(wchar_t *nimi, wchar_t trk, int tls, int sj, int ntls)
{
	int Vast;
	wchar_t st[60];

	Label1->Caption = nimi;
	LblTulos->Caption = aikatowstr_cols_n(st, tls, 0, 0, 8);
	LblSija->Visible = (sj != 0 || ntls != 0);
#ifndef MAXOSUUSLUKU
	swprintf(st, L"%d./%d", sj, ntls);
#else
	aikatowstr_cols_n(st+1, ntls, 0, 0, 8);
	LblEro->Visible = LblSija->Visible;
	elimwzb1(st+1);
	st[0] = ntls ? L'+' : L' ';
	LblEro->Caption = st;
	swprintf(st, L"%d.", sj);
#endif
	LblSija->Caption = st;
	Panel1->Visible = (trk == L'T');
	Memo1->Visible = !Panel1->Visible;
	Aika = 5*Kesto[(trk == L'T') ? 0 : 1];
	if (!NaytaTulosTimerThreadOn)
		_beginthread(NaytaTulosTimerThread, 10240, &Vast);
}
//---------------------------------------------------------------------------
MESSAGE void __fastcall TFormNaytaTulos::DisplTimerHandler(TMyMessage &msg)
{
	if (Aika <= 0) {
		Label1->Caption = L"Kilpailija";
		LblTulos->Caption = L"";
		LblSija->Caption = L"";
		LblEro->Caption = L"";
		Panel1->Visible = false;
		Memo1->Visible = false;
		NaytaTulosTimerThreadOn = false;
		}
	else
		Aika--;
}
//---------------------------------------------------------------------------

void __fastcall TFormNaytaTulos::Nytnkesto1Click(TObject *Sender)
{
	wchar_t ch;
	inputint_prompt(&Kesto[0], L"OK - näytön kesto sekunteina", &ch, this);
	inputint_prompt(&Kesto[1], L"Virhenäytön kesto sekunteina", &ch, this);
}
//---------------------------------------------------------------------------
void __fastcall TFormNaytaTulos::Teksti1Click(TObject *Sender)
{
	Teksti1->Checked = !Teksti1->Checked;
	Memo1->ReadOnly = !Teksti1->Checked;
}
//---------------------------------------------------------------------------
void __fastcall TFormNaytaTulos::FormResize(TObject *Sender)
{
	Panel1->Left = (ClientWidth * 24) / 450;
	Panel1->Width =	(ClientWidth * 185) / 450;
	Memo1->Left = (ClientWidth * 240) / 450;
	Memo1->Width = (ClientWidth * 185) / 450;
	Panel1->Top = (ClientHeight * 64) / 300;
	Panel1->Height = (ClientHeight * 217) / 300;
	Memo1->Top = Panel1->Top;
	Memo1->Height = Panel1->Height;
	Panel1->Font->Size = (ClientWidth * 20) / 450;
	Memo1->Font->Size = (ClientWidth * 12) / 450;
	Label1->Font->Size = (ClientWidth * 14) / 450;
	Label1->Top = (ClientHeight * 7) / 300;
	Label1->Left = Panel1->Left;
	LblTulos->Top = (ClientHeight * 35) / 300;
	LblSija->Top = LblTulos->Top;
	LblTulos->Font->Size = (ClientWidth * 14) / 450;
	LblSija->Font->Size = LblTulos->Font->Size;
	LblTulos->Left = Label1->Left;
	LblSija->Left = LblTulos->Left + (100 * LblTulos->Width) / 74;
#ifndef MAXOSUUSLUKU
	LblSija->Left = LblTulos->Left + (100 * LblTulos->Width) / 74;
	LblEro->Visible = false;
#else
	LblEro->Top = LblTulos->Top;
	LblEro->Left = LblTulos->Left + (100 * LblTulos->Width) / 74;
	LblEro->Font->Size = LblTulos->Font->Size;
	LblSija->Left = LblEro->Left + (100 * LblEro->Width) / 74;
#endif
}
//---------------------------------------------------------------------------
void __fastcall TFormNaytaTulos::Nyttulosjasija1Click(TObject *Sender)
{
	Nyttulosjasija1->Checked = !Nyttulosjasija1->Checked;
	LblTulos->Visible = Nyttulosjasija1->Checked;
	LblSija->Visible = Nyttulosjasija1->Checked;
#ifdef MAXOSUUSLUKU
	LblEro->Visible = Nyttulosjasija1->Checked;
#endif
}
//---------------------------------------------------------------------------

void __fastcall TFormNaytaTulos::FormShow(TObject *Sender)
{
#ifdef MAXOSUUSLUKU
	LblEro->Visible = LblTulos->Visible;
#endif
	Resize();
}
//---------------------------------------------------------------------------

void __fastcall TFormNaytaTulos::FormCreate(TObject *Sender)
{
#ifndef MAXOSUUSLUKU
	if (esiluenta) {
		Memo1->Clear();
		Memo1->Lines->Add(L" ");
		Memo1->Lines->Add(L" ");
		Memo1->Lines->Add(L"Varmista tiedot");
		Memo1->Lines->Add(L" ");
		Memo1->Lines->Add(L"Check information");
		Memo1->Lines->Add(L" ");
		Memo1->Lines->Add(L"Granska information");
		}
#endif
}
//---------------------------------------------------------------------------

void __fastcall TFormNaytaTulos::Esiluennanlhtraja1Click(TObject *Sender)
{
#ifndef MAXOSUUSLUKU
	wchar_t ch;
	inputint_prompt(&IkkParam.LahtoRaja, L"Varoitusraja lähtöajalle (min)", &ch, this);
#endif
}
//---------------------------------------------------------------------------

