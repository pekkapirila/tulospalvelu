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

#include "UnitEmitSQL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormEmitSQL *FormEmitSQL;

#ifdef MAXOSUUSLUKU
static bool vaiheenvaihto = false;
#endif

void tall_etulos(INT32 badge, INT32 t, INT32 tms, INT r_no, int Jono);
//---------------------------------------------------------------------------
__fastcall TFormEmitSQL::TFormEmitSQL(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	EdtLaite[0] = EdtLaite1;
	EdtLaite[1] = EdtLaite2;
	EdtLaite[2] = EdtLaite3;
	EdtLaite[3] = EdtLaite4;
	EdtAlku[0] = EdtAlku1;
	EdtAlku[1] = EdtAlku2;
	EdtAlku[2] = EdtAlku3;
	EdtAlku[3] = EdtAlku4;
	Ala[0] = Ala1;
	Ala[1] = Ala2;
	Ala[2] = Ala3;
	Ala[3] = Ala4;
	Yla[0] = Yla1;
	Yla[1] = Yla2;
	Yla[2] = Yla3;
	Yla[3] = Yla4;
}
//---------------------------------------------------------------------------
static void haeSQLTh(LPVOID lpCn)
{
	while (taustaon && !vaiheenvaihto  && FormEmitSQL && FormEmitSQL->Jatkuva && FormEmitSQL->SQLConnection1->Connected) {
		FormEmitSQL->readSQLrivi(FormEmitSQL->aikaraja);
		Sleep(FormEmitSQL->HakuVali);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitSQL::BtnAvaaClick(TObject *Sender)
{
	BtnParamsClick(Sender);
	SQLConnection1->Open();
	Sleep(1000);
	BtnTestaaClick(Sender);
	int Cn = 0;
	if (Jatkuva)
		haeSQLThread = _beginthread(haeSQLTh, 163840, &Cn);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::BtnTestaaClick(TObject *Sender)
{
	Edit1->Text = L"";
	for (int i = 0; i < DEVLKM; i++) {
		Ala[i]->Text = L"0";
		Yla[i]->Text = L"0";
		ala[i] = 0;
		yla[i] = 0;
		}
	CBAvattu->Checked = SQLConnection1->Connected;
	if (SQLConnection1->Connected) {
		for (int i = 0; inSQLquery > 0 && i < 50; i++)
			Sleep(200);
		if (inSQLquery > 0)
			return;
		inSQLquery++;
		SQLDataSet1->CommandText = UnicodeString(L"SELECT count(*) from tags;");
		SQLDataSet1->Open();
		SQLDataSet1->First();
		Edit1->Text = SQLDataSet1->FieldValues[L"count(*)"];
		SQLDataSet1->Close();
		if (Edit1->Text.ToInt() > 0) {
			for (int i = 0; i < devCount; i++) {
				SQLDataSet1->CommandText =
					UnicodeString(L"SELECT min(M) as minM, max(M) as maxM\r\n"
						L"from tags WHERE\r\n"
					L"Y="+UnicodeString(devices[i])+L" AND M>"+UnicodeString(lastNo[i])+
					L" AND time>'"+FormEmitSQL->aikaraja+L"';");
				SQLDataSet1->Open();
				if (!SQLDataSet1->IsEmpty()) {
					SQLDataSet1->First();
					try {
						Ala[i]->Text = SQLDataSet1->FieldValues[L"minM"];
						Yla[i]->Text = SQLDataSet1->FieldValues[L"maxM"];
						ala[i] = Ala[i]->Text.ToInt();
						yla[i] = Yla[i]->Text.ToInt();
						}
					catch (...) {
//						break;
						}
					}
				SQLDataSet1->Close();
				}
			}
		inSQLquery--;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::BtnParamsClick(TObject *Sender)
{
	SQLConnection1->Params->Clear();
	SQLConnection1->Params->Add(UnicodeString(L"drivername=")+sqlEparam.drivername);
	SQLConnection1->Params->Add(UnicodeString(L"librarynameosx=")+sqlEparam.librarynameosx);
	SQLConnection1->Params->Add(UnicodeString(L"vendorlibwin64=")+sqlEparam.vendorlibwin64);
	SQLConnection1->Params->Add(UnicodeString(L"vendorlibosx=")+sqlEparam.vendorlibosx);
	SQLConnection1->Params->Add(UnicodeString(L"blobsize=")+UnicodeString(sqlEparam.blobsize));
	SQLConnection1->Params->Add(UnicodeString(L"database=")+EdtDB->Text);
	SQLConnection1->Params->Add(UnicodeString(L"hostname=")+EdtServer->Text);
	SQLConnection1->Params->Add(UnicodeString(L"port=")+EdtPortti->Text);
	SQLConnection1->Params->Add(UnicodeString(L"localecode=")+sqlEparam.localecode);
	SQLConnection1->Params->Add(UnicodeString(L"Password=")+EdtPW->Text);
	SQLConnection1->Params->Add(UnicodeString(L"User_Name=")+EdtUser->Text);
	SQLConnection1->Params->Add(UnicodeString(L"compressed=")+(sqlEparam.compressed ? L"True" : L"False"));
	SQLConnection1->Params->Add(UnicodeString(L"encrypted=")+(sqlEparam.encrypted ? L"True" : L"False"));
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::BtnSuljeYhtClick(TObject *Sender)
{
	if (Jatkuva) {
		Jatkuva = false;
		Application->MessageBoxW(L"Suljetaan ensin jatkuva haku, odota n. 10 sek ennen jatkamista",
			L"Sulkeminen", MB_OK);
		CBJatkuva->Checked = false;
		Sleep(500);
		}
	SQLConnection1->Close();
	Sleep(1000);
	BtnTestaaClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::BtnCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::FormShow(TObject *Sender)
{
	EdtLaiteChange(0);
	if (sqlEparam.autostart)
		BtnAvaaClick(FormEmitSQL);
	BtnTestaaClick(Sender);
}
//---------------------------------------------------------------------------

bool TFormEmitSQL::readSQLrivi(wchar_t *minTimeSt)
{
	UnicodeString ust;
//	wchar_t mintimeSt[30], timeSt[30];
	aikatp ut;
	int Tietue, Laite, maxNo, er = 0;
//	const int hakulkm = 100;

	for (int i = 0; i < devCount; i++) {
		int ed_val = -999999;
		wchar_t ed_ust[200];
		if (lastNo[i] < ala[i]-1)
			lastNo[i] = ala[i]-1;

		while (SQLConnection1->Connected) {
			if (yla[i] == 0) {
				SQLDataSet1->CommandText =
					UnicodeString(L"SELECT min(M) as minM, max(M) as maxM\r\n"
						L"from tags WHERE\r\n"
					L"Y="+UnicodeString(devices[i])+L" AND M>"+UnicodeString(lastNo[i])+
					L" AND time>'"+FormEmitSQL->aikaraja+L"';");
				SQLDataSet1->Open();
				if (!SQLDataSet1->IsEmpty()) {
					SQLDataSet1->First();
					if (SQLDataSet1->FieldCount == 2) {
						try {
							ala[i] = SQLDataSet1->FieldValues[L"minM"];
							yla[i] = SQLDataSet1->FieldValues[L"maxM"];
							if (lastNo[i] < ala[i]-1)
								lastNo[i] = ala[i]-1;
							}
						catch (...) {
	//						break;
							}
						}
					}
				SQLDataSet1->Close();
				}
			if (yla[i] == 0)
				break;
			if (hakulkm <= 0)
				maxNo = 10000000;
			else
				maxNo = lastNo[i] + hakulkm;
			if (testi && loki) {
				wchar_t msg[200];
				swprintf(msg, L"%10d SQLhakuun, Dev %d, Sanoma %6d", mstimer(), i+1, lastNo[i]+1);
				wkirjloki(msg);
				}
			SQLDataSet1->CommandText =
				UnicodeString(L"SELECT Y, E, N, C, M, time\r\n"
					L"from tags WHERE\r\n"
				L"Y="+UnicodeString(devices[i])+L" AND B='M' AND M>"+UnicodeString(lastNo[i])+
				L" AND M<"+UnicodeString(maxNo+1)+
				L" AND time>'"+UnicodeString(minTimeSt)+L"';");
			SQLDataSet1->Open();
			if (testi && loki) {
				wchar_t msg[200] = L"";
				wcsncpy(msg, SQLDataSet1->CommandText.c_str(), 199);
				wkirjloki(msg);
				swprintf(msg, L"%10d SQLhaettu, Dev %d, Sanoma %6d", mstimer(), i+1, lastNo[i]+1);
				wkirjloki(msg);
				}
			if (!SQLDataSet1->IsEmpty()) {
				bool first = true;

				SQLDataSet1->First();
				while(!SQLDataSet1->Eof) {
					if (!first)
						SQLDataSet1->Next();
					first = false;
					if (SQLDataSet1->FieldCount == 6) {
						int t = TMAALI0, val;
						memset(&ut, 0, sizeof(ut));
						ut.jono = Jono;
						try {
							val = SQLDataSet1->FieldValues["C"];
							ut.badge = SQLDataSet1->FieldValues["N"];
							Tietue = SQLDataSet1->FieldValues["M"];
							Laite = SQLDataSet1->FieldValues["Y"];
							ust = SQLDataSet1->FieldValues["E"];
							if (ust == NULL || ust.Length() == 0)
								continue;
							if (val == ed_val && wcscmp(ust.c_str(), ed_ust) == 0)
								continue;
							ed_val = val;
							wcsncpy(ed_ust, ust.c_str(), sizeof(ed_ust)/2-1);
							t = wstrtoaika_vap(ed_ust, t0);
							ut.t = AIKAJAK*t;
							ut.date = tm_date(ut.t);
#ifndef MAXOSUUSLUKU
							ut.kanava = (val & 0x3fff);
#else
							ut.lahde = (val & 0x3fff);
#endif
							if (Laite == devices[i] && lastNo[i] < Tietue) {
								lastNo[i] = Tietue;
								}
							if (ut.badge && t != TMAALI0 * (1000/SEK))
#ifndef MAXOSUUSLUKU
								{
								int rr = eTParam.eTrno;
								if (lahdepistehaku && vainpiste[eTParam.eTrno+1] <= -3 && vainpiste[0] <= -3)
									rr = NREGNLY + ut.kanava - 1;
								tall_etulos(ut.badge, 0, t + 43200000L, rr, Jono);
								}
							else
								tall_rivi(Jono, &ut, 0, 0, 0, 0, 0, false);
#else
								{
								int rr = eTParam.eTrno;
								if (lahdepistehaku && vainpiste[eTParam.eTrno+1] <= -3 && vainpiste[0] <= -3)
									rr = NREGNLY + ut.lahde - 1;
								tall_etulos(ut.badge, 0, t + 43200000L, rr, ut.lahde);
								}
							else
								tall_rivi(Jono, &ut, 0, 0, 0, 0, 0);
#endif
							}
						catch (...) {
//							break;
							}
						}
					if (er) {
						SQLDataSet1->Close();
						break;
						}
					}
				SQLDataSet1->Close();
				SendMessage(FormEmitSQL->Handle, WM_MYPAIVITA, 0, 0);
				Sleep(100);
				if (lastNo[i] < maxNo)
					break;
				}
			else {
				SQLDataSet1->Close();
				break;
				}
			}
		if (er)
			break;
		}
	if (testi && loki) {
		wchar_t msg[100];
		swprintf(msg, L"%10d SQLhaku valmis", mstimer());
		wkirjloki(msg);
		}
	return(false);
}
//---------------------------------------------------------------------------

MESSAGE void __fastcall TFormEmitSQL::PaivitaHandler(TMyMessage &msg)
{
	BtnPaivitaClick(this);
	Sleep(100);
}
//---------------------------------------------------------------------------
void __fastcall TFormEmitSQL::BtnHaeClick(TObject *Sender)
{
	LblHaku->Visible = true;
	readSQLrivi(aikaraja);
	LblHaku->Visible = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::EdtLaiteChange(int no)
{
	int k = 0;
	devices[no] = _wtoi(EdtLaite[no]->Text.c_str());
	for (int i = 0; i < DEVLKM; ) {
		if (devices[i] != 0) {
			k++;
			i++;
			}
		else {
			int dd = 0;
			for (int j = i+1; j < DEVLKM; j++) {
				devices[j-1] = devices[j];
				dd += devices[j];
				lastNo[j-1] = lastNo[j];
				}
			devices[DEVLKM-1] = 0;
			lastNo[DEVLKM-1] = 0;
			if (dd == 0)
				break;
			}
		}
	for (int i = 0; i < DEVLKM; i++) {
		EdtLaite[i]->Text = devices[i];
		EdtAlku[i]->Text = lastNo[i];
		}
	devCount = k;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::EdtAlkuChange(int no)
{
	lastNo[no] = _wtoi(EdtAlku[no]->Text.c_str());
	EdtAlku[no]->Text = lastNo[no];
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::EdtLaite1Exit(TObject *Sender)
{
	EdtLaiteChange(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::EdtLaite2Exit(TObject *Sender)
{
	EdtLaiteChange(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::EdtLaite3Exit(TObject *Sender)
{
	EdtLaiteChange(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::EdtLaite4Exit(TObject *Sender)
{
	EdtLaiteChange(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::EdtAlku1Exit(TObject *Sender)
{
	EdtAlkuChange(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::EdtAlku2Exit(TObject *Sender)
{
	EdtAlkuChange(1);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::EdtAlku3Exit(TObject *Sender)
{
	EdtAlkuChange(2);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::EdtAlku4Exit(TObject *Sender)
{
	EdtAlkuChange(3);
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::CBJatkuvaClick(TObject *Sender)
{
	int Cn = 0;
	Jatkuva = CBJatkuva->Checked;
	EdtAika->Enabled = !Jatkuva;
	EdtErakoko->Enabled = !Jatkuva;
	BtnHae->Enabled = !Jatkuva;
	BtnAvaa->Enabled = !Jatkuva;
	BtnTestaa->Enabled = !Jatkuva;
	BtnNyt->Enabled = !Jatkuva;
	for (int i = 0; i < DEVLKM; i++) {
		EdtAlku[i]->Enabled = !Jatkuva;
		EdtLaite[i]->Enabled = !Jatkuva;
		}
	if (Jatkuva) {
		hakulkm = _wtoi(EdtErakoko->Text.c_str());
		wcscpy(aikaraja, EdtAika->Text.c_str());
		haeSQLThread = _beginthread(haeSQLTh, 163840, &Cn);
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::EdtHakuValiExit(TObject *Sender)
{
	HakuVali = _wtoi(EdtHakuVali->Text.c_str());
	EdtHakuVali->Text = HakuVali;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::BtnPaivitaClick(TObject *Sender)
{
	for (int i = 0; i < DEVLKM; i++) {
		EdtLaite[i]->Text = devices[i];
		EdtAlku[i]->Text = lastNo[i];
		if (lastNo[i] > yla[i])
			yla[i] = lastNo[i];
		Ala[i]->Text = ala[i];
		Yla[i]->Text = yla[i];
//		EdtLaite[i]->Refresh();
//		EdtAlku[i]->Refresh();
		}
	Refresh();
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::CBJonoChange(TObject *Sender)
{
	Jono = CBJono->ItemIndex;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::BtnNytClick(TObject *Sender)
{
	EdtAika->Text = UnicodeString(ISOdatetime(3,1)).SubString(1, 10)+
		L" "+UnicodeString(ISOdatetime(3,1)).SubString(12, 8);
	wcscpy(aikaraja, EdtAika->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"emiTag");
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::FormCreate(TObject *Sender)
{
	HakuVali = 50 * eTParam.eThakuvali;
	EdtHakuVali->Text = HakuVali;
	CBJatkuva->Checked = Jatkuva;
	EdtServer->Text = sqlEparam.hostname;
	EdtDB->Text = sqlEparam.database;
	EdtUser->Text = sqlEparam.User_Name;
	EdtPW->Text = sqlEparam.Password;
	EdtPortti->Text = sqlEparam.port;
	for (int dd = 0; dd < DEVLKM; dd++)
		devices[dd] = eTParam.eTserial[dd];
	if (devices[0] == 0)
		devices[0] = _wtoi(EdtLaite[0]->Text.c_str());
	for (int dd = 0; dd < DEVLKM; dd++)
		EdtLaite[dd]->Text = devices[dd];
	if (eTParam.eTdate[0] && eTParam.eTtime[0])
		EdtAika->Text = UnicodeString(eTParam.eTdate)+L" "+eTParam.eTtime;
	else {
		EdtAika->Text = UnicodeString(ISOdatetime(3,1)).SubString(1, 10)+
			L" "+UnicodeString(ISOdatetime(3,1)).SubString(12, 8);
		}
	wcscpy(aikaraja, EdtAika->Text.c_str());
	hakulkm = _wtoi(EdtErakoko->Text.c_str());
	CBJono->Clear();
	if (maxjono) {
		for (int r = 0; r <= maxjono; r++) {
			CBJono->Items->Add(L"Jono "+UnicodeString(r+1));
			}
		}
#ifndef MAXOSUUSLUKU
	else if (lajatfl) {
		CBJono->Items->Add(L"Aika");
		CBJono->Items->Add(L"Lähtö");
		}
#endif
	else {
		CBJono->Items->Add(L"Jono 1");
		}
#ifndef MAXOSUUSLUKU
	if (tunnjono >= 0)
		Jono = tunnjono;
#endif
	CBJono->ItemIndex = Jono;
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::EdtAikaChange(TObject *Sender)
{
		wcscpy(aikaraja, EdtAika->Text.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFormEmitSQL::EdtErakokoChange(TObject *Sender)
{
	hakulkm = _wtoi(EdtErakoko->Text.c_str());
}
//---------------------------------------------------------------------------

