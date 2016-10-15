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

#include "UnitMySQL.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMySQL *FormMySQL;
//---------------------------------------------------------------------------
__fastcall TFormMySQL::TFormMySQL(TComponent* Owner)
	: TForm(Owner)
{
	EdtServer->Text = sqlparam.hostname;
	EdtDB->Text = sqlparam.database;
	EdtUser->Text = sqlparam.User_Name;
	EdtPW->Text = sqlparam.Password;
	if (sqlparam.autostart)
		Button1Click(FormMySQL);
}
//---------------------------------------------------------------------------
void __fastcall TFormMySQL::Button1Click(TObject *Sender)
{
	Button3Click(Sender);
	SQLConnection1->Open();
	Sleep(1000);
//	if (SQLConnection1->Connected) {
//		SQLConnection1->ExecuteDirect(L"SET NAMES 'utf8';");
//		}
	Button2Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMySQL::Button2Click(TObject *Sender)
{
	Edit1->Text = L"";
	Edit2->Text = L"";
	Edit3->Text = L"";
	CBAvattu->Checked = SQLConnection1->Connected;
	CBjatkuva->Checked = SQLConnection1->Connected && sqlparam.sqltallennus;
	if (SQLConnection1->Connected) {
		CBjatkuva->Enabled = true;
		datatallfl[1] = sqlparam.sqltallennus;
		if (sqlparam.sqltallennus)
			kirjKilpSQL(0);
		for (int i = 0; inSQLquery > 0 && i < 50; i++)
			Sleep(200);
		if (inSQLquery > 0)
			return;
		inSQLquery++;
		SQLDataSet1->CommandText = UnicodeString(L"SELECT count(*) from vosanotot;");
		SQLDataSet1->Open();
		SQLDataSet1->First();
		Edit1->Text = SQLDataSet1->FieldValues[L"count(*)"];
		SQLDataSet1->Close();
		SQLDataSet1->CommandText = UnicodeString(L"SELECT count(*) from osuudet;");
		SQLDataSet1->Open();
		SQLDataSet1->First();
		Edit2->Text = SQLDataSet1->FieldValues[L"count(*)"];
		SQLDataSet1->Close();
		SQLDataSet1->CommandText = UnicodeString(L"SELECT count(*) from vtulos;");
		SQLDataSet1->Open();
		SQLDataSet1->First();
		Edit3->Text = SQLDataSet1->FieldValues[L"count(*)"];
		SQLDataSet1->Close();
		inSQLquery--;
		}
	else {
		datatallfl[1] = 0;
		CBjatkuva->Enabled = false;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMySQL::Button3Click(TObject *Sender)
{
/*
	OpenDialog1->FileName = FormWK->CurrentDir+UnicodeString(L"dbxkilp.ini");
	OpenDialog1->DefaultExt = L"ini";
	OpenDialog1->Filter = L"Ini-tiedostot (*.ini)|*.INI|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		TextFl *InFile = new TextFl(OpenDialog1->FileName.c_str(), L"rt");

		if (InFile) {
			SQLConnection1->Params->Clear();
			while (!InFile->Feof()){
				wchar_t line[200];
				InFile->ReadLine(line, 199);
				if (wcslen(line) > 5)
			SQLConnection1->Params->Clear();
				}
			delete InFile;
			}
*/
//		SQLConnection1->LoadParamsFromIniFile(FName);

		{

		SQLConnection1->Params->Clear();
		SQLConnection1->Params->Add(UnicodeString(L"drivername=")+sqlparam.drivername);
		SQLConnection1->Params->Add(UnicodeString(L"librarynameosx=")+sqlparam.librarynameosx);
		SQLConnection1->Params->Add(UnicodeString(L"vendorlibwin64=")+sqlparam.vendorlibwin64);
		SQLConnection1->Params->Add(UnicodeString(L"vendorlibosx=")+sqlparam.vendorlibosx);
		SQLConnection1->Params->Add(UnicodeString(L"blobsize=")+UnicodeString(sqlparam.blobsize));
		SQLConnection1->Params->Add(UnicodeString(L"database=")+EdtDB->Text);
		SQLConnection1->Params->Add(UnicodeString(L"hostname=")+EdtServer->Text);
		SQLConnection1->Params->Add(UnicodeString(L"localecode=")+sqlparam.localecode);
		SQLConnection1->Params->Add(UnicodeString(L"Password=")+EdtPW->Text);
		SQLConnection1->Params->Add(UnicodeString(L"User_Name=")+EdtUser->Text);
		SQLConnection1->Params->Add(UnicodeString(L"compressed=")+(sqlparam.compressed ? L"True" : L"False"));
		SQLConnection1->Params->Add(UnicodeString(L"encrypted=")+(sqlparam.encrypted ? L"True" : L"False"));

//		Memo1->Text = SQLConnection1->Params->Text;
		}

}
//---------------------------------------------------------------------------

void __fastcall TFormMySQL::Button4Click(TObject *Sender)
{
	SQLConnection1->Close();
	Button2Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMySQL::Button5Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormMySQL::FormShow(TObject *Sender)
{
	Button2Click(Sender);
}
//---------------------------------------------------------------------------

int __fastcall TFormMySQL::lukumSQL(void)
{
	int n = 0;

	if (SQLConnection1->Connected) {
		SQLDataSet1->CommandText = UnicodeString(L"SELECT count(*) from vosanotot WHERE kilpailu='")+
			kilpparam.kilpkoodi+L"'";
		SQLDataSet1->Open();
		SQLDataSet1->First();
		n = SQLDataSet1->FieldValues[L"count(*)"];
		SQLDataSet1->Close();
		}
	return(n);
}
//---------------------------------------------------------------------------
bool TFormMySQL::readSQLtulos(vatp *va, int dKilp, int os, int piste)
{
	wchar_t as[20];

	SQLDataSet1->CommandText =
		UnicodeString(L"SELECT aika, sija from vtulos WHERE\r\n"
		L"kilpailu='"+UnicodeString(kilpparam.kilpkoodi)+L"' AND\r\n"
		L"tietue="+UnicodeString(dKilp)+L" AND\r\n"
		L"osuus="+UnicodeString(os+1)+L" AND\r\n"
		L"piste="+UnicodeString(piste));
	SQLDataSet1->Open();
	if (!SQLDataSet1->IsEmpty()) {
		SQLDataSet1->First();
		va->aika = SQLDataSet1->FieldValues[L"aika"];
		va->aika -= t0*TUNTI;
		va->sija = SQLDataSet1->FieldValues[L"sija"];
		SQLDataSet1->Close();
		return(true);
		}
	SQLDataSet1->Close();
	return(false);
}

//---------------------------------------------------------------------------
bool TFormMySQL::readSQLosuus(kilptietue *kilp, int dKilp, int os)
{
	osrec *ostiet = kilp->ostiet + os;
	UnicodeString ust, ust2;

	SQLDataSet1->CommandText =
		UnicodeString(L"SELECT lisno, sukunimi, etunimi, arvo, seura, badge, badge2, "
			L"laina, laina2, rata, selitys, "
			L"tlahto, lahtolaji, keskhyl, ampsakot, sakko, gps from osuudet WHERE\r\n"
		L"kilpailu='"+UnicodeString(kilpparam.kilpkoodi)+L"' AND\r\n"
		L"tietue="+UnicodeString(dKilp)+L" AND\r\n"
		L"osuus="+UnicodeString(os+1));
	SQLDataSet1->Open();
	if (!SQLDataSet1->IsEmpty()) {
		SQLDataSet1->First();
		ostiet->lisno = SQLDataSet1->FieldValues[L"lisno"];
		ust = SQLDataSet1->FieldValues[L"sukunimi"];
		if (ust == NULL)
			ust = L"";
		ust2 = SQLDataSet1->FieldValues[L"etunimi"];
		if (ust2 == NULL)
			ust2 = L"";
		ostiet->setNimi(ust2.c_str(), ust.c_str());

		ust = SQLDataSet1->FieldValues[L"arvo"];
		if (ust != NULL && ust.Length() > 0)
			wcstoansi(ostiet->arvo, ust.c_str(), LARVO);

		ust = SQLDataSet1->FieldValues[L"seura"];
		if (ust != NULL && ust.Length() > 0)
			wcstoansi(ostiet->osseura, ust.c_str(), OSSEURAL);

		ostiet->badge[0] = SQLDataSet1->FieldValues[L"badge"];
		ostiet->badge[1] = SQLDataSet1->FieldValues[L"badge2"];
		ust = SQLDataSet1->FieldValues[L"laina"];
		if (ust != NULL && ust.Length() > 0)
			ostiet->laina[0] = ust.c_str()[0];
		ust = SQLDataSet1->FieldValues[L"laina2"];
		if (ust != NULL && ust.Length() > 0)
			ostiet->laina[1] = ust.c_str()[0];
		ust = SQLDataSet1->FieldValues[L"rata"];
		if (ust != NULL && ust.Length() > 0)
			ostiet->setHajonta(ust.c_str());
		ust = SQLDataSet1->FieldValues[L"selitys"];
		if (ust != NULL && ust.Length() > 0)
			ostiet->setSelitys(ust.c_str());
		ostiet->ylahto = SQLDataSet1->FieldValues[L"tlahto"];
		ostiet->ylahto -= t0*TUNTI;
		ostiet->lahtolaji = SQLDataSet1->FieldValues[L"lahtolaji"];
		ust = SQLDataSet1->FieldValues[L"keskhyl"];
		if (ust != NULL && ust.Length() > 0)
			kilp->SetTark(os, ust.c_str()[0]);
		ust = SQLDataSet1->FieldValues[L"ampsakot"];
		if (ust != NULL && ust.Length() > 0)
			ostiet->setAmpsakot(ust.c_str());
		ostiet->sakko = SQLDataSet1->FieldValues[L"sakko"];
		ust = SQLDataSet1->FieldValues[L"gps"];
		if (ust != NULL && ust.Length() > 0)
			ostiet->setSeuranta(ust.c_str()[0]);
		SQLDataSet1->Close();
		for (int piste = 0; piste <= Sarjat[kilp->sarja].valuku[os]; piste++) {
			readSQLtulos(&ostiet->va[piste], dKilp, os, piste);
			}
		return(true);
		}
	SQLDataSet1->Close();
	return(false);
}

//---------------------------------------------------------------------------
bool TFormMySQL::readSQLrivi(kilptietue *kilp, int dKilp)
{
	UnicodeString ust;

	kilp->nollaa();
	SQLDataSet1->CommandText =
		UnicodeString(L"SELECT id, sarja, seura, joukkue, joukkid, joukkuenimi, maa, piiri, "
			L"ilmlista, piste1, piste2, piste3, era\r\n"
			L"from vosanotot WHERE\r\n"
		L"kilpailu='"+UnicodeString(kilpparam.kilpkoodi)+L"' AND\r\n"
		L"tietue="+UnicodeString(dKilp));
	SQLDataSet1->Open();
	if (!SQLDataSet1->IsEmpty()) {
		SQLDataSet1->First();
		kilp->kilpno = SQLDataSet1->FieldValues[L"id"];
		kilp->sarja = haesarja_w(SQLDataSet1->FieldValues[L"sarja"], false);
		ust = SQLDataSet1->FieldValues[L"seura"];
		if (ust != NULL && ust.Length() > 0)
			kilp->setSeura(ust.c_str());
		kilp->joukkue = SQLDataSet1->FieldValues[L"joukkue"];
		kilp->joukkid = SQLDataSet1->FieldValues[L"joukkid"];
		ust = SQLDataSet1->FieldValues[L"joukkuenimi"];
		if (ust != NULL && ust.Length() > 0)
			wcstoansi(kilp->joukknimi, ust.c_str(), LJK);

		ust = SQLDataSet1->FieldValues[L"maa"];
		if (ust != NULL && ust.Length() > 0)
			wcstoansi(kilp->maa, ust.c_str(), 3);
		kilp->piiri = SQLDataSet1->FieldValues[L"piiri"];
		kilp->ilmlista = SQLDataSet1->FieldValues[L"ilmlista"];
		kilp->pisteet[0] = SQLDataSet1->FieldValues[L"piste1"];
		kilp->pisteet[1] = SQLDataSet1->FieldValues[L"piste2"];
		kilp->pisteet[2] = SQLDataSet1->FieldValues[L"piste3"];
		kilp->era = SQLDataSet1->FieldValues[L"era"];
		SQLDataSet1->Close();
		for (int os = 0 ; os < Sarjat[kilp->sarja].osuusluku; os++) {
			readSQLosuus(kilp, dKilp, os);
			}
		return(true);
		}
	SQLDataSet1->Close();
	return(false);
}

//---------------------------------------------------------------------------
bool TFormMySQL::addSQLtulos(vatp *va, int dKilp, int os, int piste)
{
	wchar_t as[20];

	if (va->aika == TMAALI0)
		return(false);
	SQLDataSet1->CommandText =
		UnicodeString(L"INSERT into vtulos (kilpailu, tietue, osuus, piste, aika, "
			L"aikatime, sija) values ('")+
		UnicodeString(kilpparam.kilpkoodi)+L"','"+
		UnicodeString(dKilp)+L"','"+
		UnicodeString(os+1)+L"','"+
		UnicodeString(piste)+L"','"+
		UnicodeString(va->aika + t0*TUNTI)+L"','"+
			((va->aika != TMAALI0) ? (UnicodeString(aikatowstr_cols_n(as,va->aika, t0, 0, 8))) : UnicodeString(L"00:00:00")) + L"','"+
		UnicodeString(va->sija)+L"')";
		SQLDataSet1->ExecSQL(true);
	return(true);
}
//---------------------------------------------------------------------------
bool TFormMySQL::addSQLosuus(kilptietue *kilp, int dKilp, int os)
{
	wchar_t as[20], as2[20], st[60], sta[30], sts[40], sth[40], stsel[40], et[60];
	osrec *ostiet = kilp->ostiet + os;

	MbsToWcs(sts, ostiet->osseura, 39);
	SQLDataSet1->CommandText =
		UnicodeString(L"INSERT into osuudet (kilpailu, tietue, osuus, lisno, sukunimi, etunimi, "
			L"arvo, seura, badge, badge2, laina, laina2, rata, selitys, tlahto, tlahtotime, "
			L"lahtolaji, keskhyl, ampsakot, sakko, gps, ossija) values ('")+
			UnicodeString(kilpparam.kilpkoodi)+L"','"+
			UnicodeString(dKilp)+L"','"+
			UnicodeString(os+1)+L"','"+
			UnicodeString(ostiet->lisno)+L"','"+
			UnicodeString(ostiet->SukuNimi(st, 59))+L"','"+
			UnicodeString(ostiet->EtuNimi(et, 59))+L"','"+
			UnicodeString(ostiet->Arvo(sta))+L"','"+
			UnicodeString(sts)+L"','"+
			UnicodeString(ostiet->badge[0])+L"','"+
			UnicodeString(ostiet->badge[1])+L"','"+
			UnicodeString(ostiet->laina[0] == L'L' ? L"L" : L"")+L"','"+
			UnicodeString(ostiet->laina[1] == L'L' ? L"L" : L"")+L"','"+
			UnicodeString(ostiet->Hajonta(sth))+L"','"+
			UnicodeString(ostiet->Selitys(stsel))+L"','"+
			UnicodeString(ostiet->ylahto+t0*TUNTI)+L"','"+
			((ostiet->ylahto != TMAALI0) ? (UnicodeString(aikatowstr_cols_n(as,ostiet->ylahto, t0, 0, 8))) : UnicodeString(L"00:00:00")) + L"','"+
			UnicodeString(ostiet->lahtolaji)+L"','"+
			UnicodeString(ostiet->keskhyl)+L"','"+
			UnicodeString(ostiet->ampsakot)+L"','"+
			UnicodeString(ostiet->sakko)+L"','"+
			(ostiet->seuranta >= L'A' ? UnicodeString(ostiet->seuranta) : UnicodeString(L""))+L"','"+
			UnicodeString(ostiet->ossija)+L"')";
	SQLDataSet1->ExecSQL(true);
	for (int piste = 0; piste <= Sarjat[kilp->sarja].valuku[os]; piste++) {
		addSQLtulos(&ostiet->va[piste], dKilp, os, piste);
		}
	return(true);
}
//---------------------------------------------------------------------------

bool TFormMySQL::addSQLrivi(kilptietue *kilp, int dKilp)
{
	wchar_t st[60], st1[32], st2[10];

	MbsToWcs(st1, kilp->joukknimi, 30);
	MbsToWcs(st2, kilp->maa, 4);
	SQLDataSet1->CommandText =
		UnicodeString(L"INSERT into vosanotot (kilpailu, tietue, id, sarja, "
			L"seura, joukkue, joukkid, joukkuenimi, maa, piiri, "
			L"ilmlista, piste1, piste2, piste3, era) values ('")+
		UnicodeString(kilpparam.kilpkoodi)+L"','"+
		UnicodeString(dKilp)+L"','"+
		UnicodeString(kilp->kilpno)+L"','"+
		UnicodeString(Sarjat[kilp->sarja].sarjanimi)+L"','"+
		UnicodeString(kilp->Seura(st, 59))+L"','"+
		UnicodeString(kilp->joukkue)+L"','"+
		UnicodeString(kilp->joukkid)+L"','"+
		UnicodeString(st1)+L"','"+
		UnicodeString(st2)+L"','"+
		UnicodeString(kilp->piiri)+L"','"+
		UnicodeString(kilp->ilmlista)+L"','"+
		UnicodeString(kilp->pisteet[0])+L"','"+
		UnicodeString(kilp->pisteet[1])+L"','"+
		UnicodeString(kilp->pisteet[2])+L"','"+
		UnicodeString(kilp->era)+L"')";
	SQLDataSet1->ExecSQL(true);
	for (int os = 0; os < Sarjat[kilp->sarja].osuusluku; os++) {
		addSQLosuus(kilp, dKilp, os);
		}
	return(false);
}
//---------------------------------------------------------------------------

bool TFormMySQL::kirjSQLrivi(int dKilp)
{
	kilptietue kilp;
	bool paivita = false;
	TDBXTransaction *TrA;

	for (int i = 0; inSQLquery > 0 && i < 50; i++)
		Sleep(200);
	if (inSQLquery > 0)
		return(false);

	kilp.getrec(dKilp);
	if (kilp.kilpstatus)
		return(false);

	SQLDataSet1->CommandText =
		UnicodeString(L"SELECT tietue from vosanotot where kilpailu='")+
		UnicodeString(kilpparam.kilpkoodi)+
		UnicodeString(L"' and tietue=")+UnicodeString(dKilp)+UnicodeString(L";");
	inSQLquery++;
	TrA = SQLConnection1->BeginTransaction();
	SQLDataSet1->Open();
	if (!SQLDataSet1->IsEmpty())
		paivita = true;
	SQLDataSet1->Close();
	if (paivita) {
		updateSQLrivi(&kilp, dKilp);
		}
	else {
		addSQLrivi(&kilp, dKilp);
		}
	SQLConnection1->CommitFreeAndNil(TrA);
	inSQLquery--;
	return(true);
}

//---------------------------------------------------------------------------
bool TFormMySQL::updateSQLtulos(vatp *va, int dKilp, int os, int piste)
{
	wchar_t as[20];
	vatp va1;

	if (!readSQLtulos(&va1, dKilp, os, piste))
		addSQLtulos(va, dKilp, os, piste);
	else if (va->aika != va1.aika) {
		SQLDataSet1->CommandText =
			UnicodeString(L"UPDATE vtulos SET \r\n")+
			L"aika="+UnicodeString(va->aika + t0*TUNTI)+L",\r\n"+
			L"aikatime='"+
			((va->aika != TMAALI0) ? (UnicodeString(aikatowstr_cols_n(as,va->aika, t0, 0, 8))) : UnicodeString(L"00:00:00")) + L"',\r\n" +
			L"sija="+UnicodeString(va->sija)+L"\r\n"
			L"WHERE kilpailu='"+kilpparam.kilpkoodi+L"' AND tietue="+UnicodeString(dKilp)+
			L" AND osuus="+UnicodeString(os+1)+L" AND piste="+UnicodeString(piste);
		SQLDataSet1->ExecSQL(true);
		}
	return(true);
}
//---------------------------------------------------------------------------
bool TFormMySQL::updateSQLosuus(kilptietue *kilp, osrec *ostiet1, int dKilp, int os)
{
	wchar_t as[20], as2[20], st[60], st1[60], sta[30], sts[40], sth[40], stsel[40], et[60], et1[60];
	osrec *ostiet = kilp->ostiet + os;

	if (*ostiet == *ostiet1)
		return(true);
	SQLDataSet1->CommandText =
		UnicodeString(L"SELECT tietue from osuudet where kilpailu='")+
		UnicodeString(kilpparam.kilpkoodi)+
		UnicodeString(L"' AND tietue=")+UnicodeString(dKilp)+
		UnicodeString(L" AND osuus=")+UnicodeString(os+1)+UnicodeString(L";");
	SQLDataSet1->Open();
	if (SQLDataSet1->IsEmpty()) {
		SQLDataSet1->Close();
		addSQLosuus(kilp, dKilp, os);
		}
	else {
		SQLDataSet1->Close();
		if (ostiet1->vert0(*ostiet)) {
			MbsToWcs(sts, ostiet->osseura, 39);
			SQLDataSet1->CommandText =
				UnicodeString(L"UPDATE osuudet SET \r\n")+
				(ostiet->lisno != ostiet1->lisno ? (L"badge="+UnicodeString(ostiet->badge[0])+L",\r\n") : UnicodeString(L""))+
				(wcscmp(ostiet->SukuNimi(st, 59), ostiet1->SukuNimi(st1, 59)) ? (L"sukunimi='"+UnicodeString(st)+L"',\r\n") : UnicodeString(L""))+
				(wcscmp(ostiet->EtuNimi(et, 59), ostiet1->EtuNimi(et1, 59)) ? (L"etunimi='"+UnicodeString(et)+L"',\r\n") : UnicodeString(L""))+
				(strcmp(ostiet->arvo, ostiet1->arvo) ? (L"arvo='"+UnicodeString(ostiet->Arvo(sta))+L"',\r\n") : UnicodeString(L""))+
				(strcmp(ostiet->osseura, ostiet1->osseura) ? (L"seura='"+UnicodeString(sts)+L"',\r\n") : UnicodeString(L""))+
				(ostiet->badge[0] != ostiet1->badge[0] ? (L"badge="+UnicodeString(ostiet->badge[0])+L",\r\n") : UnicodeString(L""))+
				(ostiet->badge[1] != ostiet1->badge[1] ? (L"badge2="+UnicodeString(ostiet->badge[1])+L",\r\n") : UnicodeString(L""))+
				(ostiet->laina[0] != ostiet1->laina[0] ? (L"laina='"+UnicodeString(ostiet->laina[0])+L"',\r\n") : UnicodeString(L""))+
				(ostiet->laina[1] != ostiet1->laina[1] ? (L"laina2='"+UnicodeString(ostiet->laina[1])+L"',\r\n") : UnicodeString(L""))+
				(strcmp(ostiet->hajonta, ostiet1->hajonta) ? (L"rata='"+UnicodeString(ostiet->Hajonta(sth))+L"',\r\n") : UnicodeString(L""))+
				(strcmp(ostiet->selitys, ostiet1->selitys) ? (L"selitys='"+UnicodeString(ostiet->Selitys(stsel))+L"',\r\n") : UnicodeString(L""))+
				(ostiet->ylahto != ostiet1->ylahto ? (L"tlahto="+UnicodeString(ostiet->ylahto+t0*TUNTI)+L",\r\n") : UnicodeString(L""))+
				(ostiet->ylahto != ostiet1->ylahto ? (L"tlahtotime='"+
					((ostiet->ylahto != TMAALI0) ? (UnicodeString(aikatowstr_cols_n(as,ostiet->ylahto, t0, 0, 8))) : UnicodeString(L"00:00:00")) +L"',\r\n") : UnicodeString(L""))+
				(ostiet->keskhyl != ostiet1->keskhyl ? (L"keskhyl='"+UnicodeString(ostiet->keskhyl)+L"',\r\n") : UnicodeString(L""))+
				(strcmp(ostiet->ampsakot, ostiet1->ampsakot) ? (L"ampsakot='"+UnicodeString(ostiet->ampsakot)+L"',\r\n") : UnicodeString(L""))+
				(ostiet->sakko != ostiet1->sakko ? (L"sakko="+UnicodeString(ostiet->sakko)+L",\r\n") : UnicodeString(L""))+
				(ostiet->seuranta != ostiet1->seuranta ? (L"gps="+UnicodeString(ostiet->seuranta)+L",\r\n") : UnicodeString(L""))+
				L"ossija="+UnicodeString(ostiet->ossija)+L"\r\n"
				L"WHERE kilpailu='"+kilpparam.kilpkoodi+L"' AND tietue="+UnicodeString(dKilp)+
				L" AND osuus="+UnicodeString(os+1);
			SQLDataSet1->ExecSQL(true);
			}
		for (int piste = 0; piste <= Sarjat[kilp->sarja].valuku[os]; piste++) {
			updateSQLtulos(&ostiet->va[piste], dKilp, os, piste);
			}
		}
	return(true);
}
//---------------------------------------------------------------------------

bool TFormMySQL::updateSQLrivi(kilptietue *kilp, int dKilp)
{
	kilptietue kilp1;
	wchar_t st[60], st1[32], st2[10];

	kilp1.nollaa();
	if (!readSQLrivi(&kilp1, dKilp))
		return(false);

	if (kilp1 == *kilp)
		return(false);

	if (kilp->vert0(kilp1)) {
		MbsToWcs(st1, kilp->joukknimi, 30);
		MbsToWcs(st2, kilp->maa, 4);
		SQLDataSet1->CommandText =
			UnicodeString(L"UPDATE vosanotot SET \r\n")+
			(kilp->kilpno != kilp1.kilpno ? (L"id="+UnicodeString(kilp->kilpno)+L",\r\n") : UnicodeString(L""))+
			(kilp->sarja != kilp1.sarja ? (L"sarja='"+UnicodeString(Sarjat[kilp->sarja].sarjanimi)+L"',\r\n") : UnicodeString(L""))+
			(strcmp(kilp->seura, kilp1.seura) ? (L"seura='"+UnicodeString(kilp->Seura(st, 59))+L"',\r\n") : UnicodeString(L""))+
			(kilp->joukkue != kilp1.joukkue ? (L"joukkue="+UnicodeString(kilp->joukkue)+L",\r\n") : UnicodeString(L""))+
			(kilp->joukkid != kilp1.joukkid ? (L"joukkid="+UnicodeString(kilp->joukkid)+L",\r\n") : UnicodeString(L""))+
			(strcmp(kilp->joukknimi, kilp1.joukknimi) ? (L"joukkue='"+UnicodeString(st1)+L"',\r\n") : UnicodeString(L""))+
			(strcmp(kilp->maa, kilp1.maa) ? (L"maa='"+UnicodeString(st2)+L"',\r\n") : UnicodeString(L""))+
			(kilp->piiri != kilp1.piiri ? (L"piiri="+UnicodeString(kilp->piiri)+L",\r\n") : UnicodeString(L""))+
			(kilp->ilmlista != kilp1.ilmlista ? (L"ilmlista="+UnicodeString(kilp->ilmlista)+L",\r\n") : UnicodeString(L""))+
			(kilp->pisteet[0] != kilp1.pisteet[0] ? (L"piste1="+UnicodeString(kilp->pisteet[0])+L",\r\n") : UnicodeString(L""))+
			(kilp->pisteet[1] != kilp1.pisteet[1] ? (L"piste2="+UnicodeString(kilp->pisteet[1])+L",\r\n") : UnicodeString(L""))+
			(kilp->pisteet[2] != kilp1.pisteet[2] ? (L"piste3="+UnicodeString(kilp->pisteet[2])+L",\r\n") : UnicodeString(L""))+
			(kilp->era != kilp1.era ? (L"era="+UnicodeString(kilp->era)+L",\r\n") : UnicodeString(L""))+
			L"WHERE kilpailu='"+kilpparam.kilpkoodi+L"' AND tietue="+UnicodeString(dKilp);
		SQLDataSet1->ExecSQL(true);
		}
	for (int os = 0; os < Sarjat[kilp->sarja].osuusluku; os++) {
		updateSQLosuus(kilp, kilp1.ostiet+os, dKilp, os);
		}
	return(true);
}
//---------------------------------------------------------------------------
bool TFormMySQL::deleteSQLrivi(int dKilp)
{
	SQLDataSet1->CommandText =
		UnicodeString(L"DELETE from vosanotot where kilpailu='")+
		UnicodeString(kilpparam.kilpkoodi)+
		UnicodeString(L"' and tietue=")+UnicodeString(dKilp)+UnicodeString(L";");
	SQLDataSet1->ExecSQL(true);
	SQLDataSet1->CommandText =
		UnicodeString(L"DELETE from osuudet where kilpailu='")+
		UnicodeString(kilpparam.kilpkoodi)+
		UnicodeString(L"' and tietue=")+UnicodeString(dKilp)+UnicodeString(L";");
	SQLDataSet1->ExecSQL(true);
	SQLDataSet1->CommandText =
		UnicodeString(L"DELETE from vtulos where kilpailu='")+
		UnicodeString(kilpparam.kilpkoodi)+
		UnicodeString(L"' and tietue=")+UnicodeString(dKilp)+UnicodeString(L";");
	SQLDataSet1->ExecSQL(true);
	return(true);
}

//---------------------------------------------------------------------------

void __fastcall TFormMySQL::Button6Click(TObject *Sender)
{
	kilptietue kilp;
	wchar_t st[60];

	readSQLrivi(&kilp, 1);
	Memo1->Lines->Add(kilp.Joukkue(st, 59));
}
//---------------------------------------------------------------------------
#define MAXSQLJONO 10000
static int headSQLjono, tailSQLjono, dKilpSQLjono[MAXSQLJONO];
static HTHREAD kirjSQLThread;
static bool varmistaSQLsiirto = false;
FILE *sqlVarmFl = NULL;
static CRITICAL_SECTION SQL_CriticalSection;


static void kirjSQLTh(LPVOID lpCn)
{
	int d, n;
	long pos;
	wchar_t sqlVarmFlName[100] = L"SQLjonoVarm.dat";

	inkirjSQLThread = true;

	while (taustaon && inkirjSQLThread) {
		varmistaSQLsiirto = FormMySQL->CBVarmista->Checked;
		if (varmistaSQLsiirto) {
			if (!sqlVarmFl) {
				int vals[2] = {0,0}, nr = 0, ir = 0, er = 0;

				ir = tailSQLjono;
				InitializeCriticalSection(&SQL_CriticalSection);
				EnterCriticalSection(&SQL_CriticalSection);
				sqlVarmFl = _wfopen(sqlVarmFlName, L"a+b");
				if (sqlVarmFl) {
					er = fseek(sqlVarmFl, 0, SEEK_END);
					pos = ftell(sqlVarmFl);
					if (pos >= 8) {
						er = fseek(sqlVarmFl, -8, SEEK_END);
						er = fread(vals, 8, 1, sqlVarmFl);
						if ((nr = vals[1]) > 0 && nr <= pos / 8) {
							if (nr > MAXSQLJONO - ((headSQLjono-tailSQLjono+MAXSQLJONO)%MAXSQLJONO))
								nr = MAXSQLJONO - ((headSQLjono-tailSQLjono+MAXSQLJONO)%MAXSQLJONO);
							er = fseek(sqlVarmFl, -8*nr, SEEK_END);
							pos = (tailSQLjono + MAXSQLJONO - nr) % MAXSQLJONO;
							tailSQLjono = pos;
							for (int i = 0; i < nr; +i++) {
								er = fread(vals, 8, 1, sqlVarmFl);
								dKilpSQLjono[pos] = vals[0];
								pos = (pos+1) % MAXSQLJONO;
								}
							}
						}
					fseek(sqlVarmFl, 0, SEEK_END);
					vals[1] = (headSQLjono-tailSQLjono+MAXSQLJONO)%MAXSQLJONO;
					while (ir != headSQLjono) {
						vals[0] = dKilpSQLjono[ir];
						fwrite(vals, 8, 1, sqlVarmFl);
						ir = (ir + 1) % MAXSQLJONO;
						}
					}
				else
					varmistaSQLsiirto = false;
				LeaveCriticalSection(&SQL_CriticalSection);
				}
			}
		if (FormMySQL->SQLConnection1->Connected && sqlparam.sqltallennus &&
			tailSQLjono != headSQLjono && !FormMySQL->inSQLquery) {
			d = dKilpSQLjono[tailSQLjono];
			FormMySQL->kirjSQLrivi(d);
			if (d == dKilpSQLjono[tailSQLjono])
				tailSQLjono = (tailSQLjono + 1) % MAXSQLJONO;
			}
		else {
			Sleep(200);
			if (varmistaSQLsiirto && sqlVarmFl) {
				pos = ftell(sqlVarmFl);
				if (pos > 4) {
					EnterCriticalSection(&SQL_CriticalSection);
					n = (headSQLjono-tailSQLjono+MAXSQLJONO)%MAXSQLJONO + 1;
					fseek(sqlVarmFl, -4, SEEK_END);
					fwrite(&n, 4, 1, sqlVarmFl);
					fflush(sqlVarmFl);
					LeaveCriticalSection(&SQL_CriticalSection);
					}
				}
			}
		FormMySQL->EdtJono->Text = UnicodeString((headSQLjono-tailSQLjono+MAXSQLJONO)%MAXSQLJONO);
		FormMySQL->EdtJono->Refresh();
		Sleep(100);
		}
}

void kirjKilpSQL(int d)
{
	static int Cn;
	int vals[2];
	FILE *FlDupl;

	varmistaSQLsiirto = FormMySQL->CBVarmista->Checked;

	if (d) {
		dKilpSQLjono[headSQLjono] = d;
		if (varmistaSQLsiirto && sqlVarmFl) {
			EnterCriticalSection(&SQL_CriticalSection);
			vals[0] = d;
			vals[1] = (headSQLjono-tailSQLjono+MAXSQLJONO)%MAXSQLJONO + 1;
			fwrite(vals, 8, 1, sqlVarmFl);
			LeaveCriticalSection(&SQL_CriticalSection);
			}
		headSQLjono = (headSQLjono + 1) % MAXSQLJONO;
		if (tailSQLjono == headSQLjono)
			tailSQLjono = (tailSQLjono + 1) % MAXSQLJONO;
		}
	if (!inkirjSQLThread)
		kirjSQLThread = _beginthread(kirjSQLTh, 163840, &Cn);
}
//---------------------------------------------------------------------------


void __fastcall TFormMySQL::CBjatkuvaClick(TObject *Sender)
{
	sqlparam.sqltallennus = CBjatkuva->Checked;
	Button2Click(Sender);
}
//---------------------------------------------------------------------------


