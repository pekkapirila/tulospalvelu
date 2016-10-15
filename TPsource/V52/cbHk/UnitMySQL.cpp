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
}
//---------------------------------------------------------------------------

void TFormMySQL::StartMySQL(void)
{
	Button1Click(this);
}
//---------------------------------------------------------------------------
void __fastcall TFormMySQL::Button1Click(TObject *Sender)
{
	Button3Click(Sender);
	SQLConnection1->Open();
	Sleep(1000);
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
		SQLDataSet1->CommandText = UnicodeString(L"SELECT count(*) from osanotot;");
		SQLDataSet1->Open();
		SQLDataSet1->First();
		Edit1->Text = SQLDataSet1->FieldValues[L"count(*)"];
		SQLDataSet1->Close();
		SQLDataSet1->CommandText = UnicodeString(L"SELECT count(*) from osottopv;");
		SQLDataSet1->Open();
		SQLDataSet1->First();
		Edit2->Text = SQLDataSet1->FieldValues[L"count(*)"];
		SQLDataSet1->Close();
		SQLDataSet1->CommandText = UnicodeString(L"SELECT count(*) from tulos;");
		SQLDataSet1->Open();
		SQLDataSet1->First();
		Edit3->Text = SQLDataSet1->FieldValues[L"count(*)"];
		SQLDataSet1->Close();
		inSQLquery--;
		}
	else {
		datatallfl[1] = false;
		CBjatkuva->Enabled = false;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormMySQL::Button3Click(TObject *Sender)
{
/*
	OpenDialog1->FileName = FormMain->CurrentDir+UnicodeString(L"dbxkilp.ini");
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


int __fastcall TFormMySQL::lukumSQL(void)
{
	int n = 0;

	if (SQLConnection1->Connected) {
		SQLDataSet1->CommandText = UnicodeString(L"SELECT count(*) from osanotot WHERE kilpailu='")+
			kilpparam.kilpkoodi+L"'";
		SQLDataSet1->Open();
		SQLDataSet1->First();
		n = SQLDataSet1->FieldValues[L"count(*)"];
		SQLDataSet1->Close();
		}
	return(n);
}
//---------------------------------------------------------------------------
bool TFormMySQL::readSQLtulos(vatp *va, int dKilp, int ipv, int piste)
{
	wchar_t as[20];
	int tt0;

	tt0 = piste == 0 ? t0 : 0;
	SQLDataSet1->CommandText =
		UnicodeString(L"SELECT aika, sija from tulos WHERE\r\n"
		L"kilpailu='"+UnicodeString(kilpparam.kilpkoodi)+L"' AND\r\n"
		L"tietue="+UnicodeString(dKilp)+L" AND\r\n"
		L"vaihe="+UnicodeString(ipv+1)+L" AND\r\n"
		L"piste="+UnicodeString(piste-1));
	SQLDataSet1->Open();
	if (!SQLDataSet1->IsEmpty()) {
		SQLDataSet1->First();
		va->vatulos = SQLDataSet1->FieldValues[L"aika"];
		va->vatulos -= tt0*TUNTI;
		va->vasija = SQLDataSet1->FieldValues[L"sija"];
		SQLDataSet1->Close();
		return(true);
		}
	SQLDataSet1->Close();
	return(false);
}

//---------------------------------------------------------------------------
bool TFormMySQL::readSQLpv(kilptietue *kilp, int dKilp, int ipv)
{
	kilppvtp *pv = kilp->pv + ipv;
	UnicodeString ust;

	SQLDataSet1->CommandText =
		UnicodeString(L"SELECT sarja, era, bib, badge, badge2, laina, "
			L"laina2, rata, selitys, pvpiste1, pvpiste2, tlahto, keskhyl, "
			L"ampsakot, tasoitus, sakko, gps, qual from osottopv WHERE\r\n"
		L"kilpailu='"+UnicodeString(kilpparam.kilpkoodi)+L"' AND\r\n"
		L"tietue="+UnicodeString(dKilp)+L" AND\r\n"
		L"vaihe="+UnicodeString(ipv+1));
	SQLDataSet1->Open();
	if (!SQLDataSet1->IsEmpty()) {
		SQLDataSet1->First();
		pv->sarja = haesarja_w(SQLDataSet1->FieldValues[L"sarja"], false);
		pv->era = SQLDataSet1->FieldValues[L"era"];
		pv->bib = SQLDataSet1->FieldValues[L"bib"];
		pv->badge[0] = SQLDataSet1->FieldValues[L"badge"];
		pv->badge[1] = SQLDataSet1->FieldValues[L"badge2"];
		ust = SQLDataSet1->FieldValues[L"laina"];
		if (ust != NULL && ust.Length() > 0)
			pv->laina[0] = ust.c_str()[0];
		ust = SQLDataSet1->FieldValues[L"laina2"];
		if (ust != NULL && ust.Length() > 0)
			pv->laina[1] = ust.c_str()[0];
		ust = SQLDataSet1->FieldValues[L"rata"];
		if (ust != NULL && ust.Length() > 0)
			wcsncpy(pv->rata, ust.c_str(), sizeof(pv->rata)/2-1);
		ust = SQLDataSet1->FieldValues[L"selitys"];
		if (ust != NULL && ust.Length() > 0)
			wcsncpy(pv->selitys, ust.c_str(), sizeof(pv->selitys)/2-1);
		pv->pvpisteet[0] = SQLDataSet1->FieldValues[L"pvpiste1"];
		pv->pvpisteet[1] = SQLDataSet1->FieldValues[L"pvpiste2"];
		pv->tlahto = SQLDataSet1->FieldValues[L"tlahto"];
		pv->tlahto -= t0*TUNTI;
		pv->va[0].vatulos = pv->tlahto;
		ust = SQLDataSet1->FieldValues[L"keskhyl"];
		if (ust != NULL && ust.Length() > 0)
			pv->keskhyl = ust.c_str()[0];
		ust = SQLDataSet1->FieldValues[L"ampsakot"];
		if (ust != NULL && ust.Length() > 0)
			kilp->set_Asakot(ust.c_str(), ipv);
//			wcsncpy(pv->asakot, ust.c_str(), sizeof(pv->asakot)/2-1);
		pv->tasoitus = SQLDataSet1->FieldValues[L"tasoitus"];
		pv->sakko = SQLDataSet1->FieldValues[L"sakko"];
		ust = SQLDataSet1->FieldValues[L"gps"];
		if (ust != NULL && ust.Length() > 0)
			pv->gps = (char) ust.c_str()[0];
		ust = SQLDataSet1->FieldValues[L"qual"];
		if (ust != NULL && ust.Length() > 0)
			pv->qual = (char) ust.c_str()[0];
		SQLDataSet1->Close();
		for (int piste = pvparam[ipv].hiihtolahto ? 0 : 1; piste <= Sarjat[kilp->Sarja(ipv)].valuku[ipv]+1; piste++) {
			readSQLtulos(&pv->va[piste], dKilp, ipv, piste);
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
		UnicodeString(L"SELECT id, lisno, intid, wrkoodi, "
			L"sukunimi, etunimi, arvo, seura, seuralyh, yhdistys, joukkue, maa, piiri, "
			L"ilmlista, piste1, piste2, piste3, sarja, sukup, ikasarja, synt, alisarja, arvryhma\r\n"
			L"from osanotot WHERE\r\n"
		L"kilpailu='"+UnicodeString(kilpparam.kilpkoodi)+L"' AND\r\n"
		L"tietue="+UnicodeString(dKilp));
	SQLDataSet1->Open();
	if (!SQLDataSet1->IsEmpty()) {
		SQLDataSet1->First();
		kilp->kilpno = SQLDataSet1->FieldValues[L"id"];
		kilp->lisno[0] = SQLDataSet1->FieldValues[L"lisno"];
		kilp->lisno[1] = SQLDataSet1->FieldValues[L"intid"];
		ust = SQLDataSet1->FieldValues[L"wrkoodi"];
		if (ust != NULL && ust.Length() > 0)
			wcsncpy(kilp->wrkoodi, ust.c_str(), sizeof(kilp->wrkoodi)/2-1);
		ust = SQLDataSet1->FieldValues[L"sukunimi"];
		if (ust != NULL && ust.Length() > 0)
			wcsncpy(kilp->sukunimi, ust.c_str(), kilpparam.lsnimi);
		else
			kilp->sukunimi[0] = 0;
		ust = SQLDataSet1->FieldValues[L"etunimi"];
		if (ust != NULL && ust.Length() > 0)
			wcsncpy(kilp->etunimi, ust.c_str(), kilpparam.lenimi);
		if (kilpparam.larvo) {
			ust = SQLDataSet1->FieldValues[L"arvo"];

			if (ust != NULL && ust.Length() > 0)
				wcsncpy(kilp->arvo, ust.c_str(), kilpparam.larvo);
			}
		if (kilpparam.lseura) {
			ust = SQLDataSet1->FieldValues[L"seura"];

			if (ust != NULL && ust.Length() > 0)
				wcsncpy(kilp->seura, ust.c_str(), kilpparam.lseura);
			}
		if (kilpparam.lseuralyh) {
			ust = SQLDataSet1->FieldValues[L"seuralyh"];

			if (ust != NULL && ust.Length() > 0)
				wcsncpy(kilp->seuralyh, ust.c_str(), kilpparam.lseuralyh);
			}
		if (kilpparam.lyhdistys) {
			ust = SQLDataSet1->FieldValues[L"yhdistys"];

			if (ust != NULL && ust.Length() > 0)
				wcsncpy(kilp->yhdistys, ust.c_str(), kilpparam.lyhdistys);
			}
		if (kilpparam.ljoukkue) {
			ust = SQLDataSet1->FieldValues[L"joukkue"];

			if (ust != NULL && ust.Length() > 0)
				wcsncpy(kilp->joukkue, ust.c_str(), kilpparam.ljoukkue);
			}
		ust = SQLDataSet1->FieldValues[L"maa"];
		if (ust != NULL && ust.Length() > 0)
			wcsncpy(kilp->maa, ust.c_str(), 3);
		kilp->piiri = SQLDataSet1->FieldValues[L"piiri"];
		kilp->ilmlista = SQLDataSet1->FieldValues[L"ilmlista"];
		kilp->pisteet[0] = SQLDataSet1->FieldValues[L"piste1"];
		kilp->pisteet[1] = SQLDataSet1->FieldValues[L"piste2"];
		kilp->pisteet[2] = SQLDataSet1->FieldValues[L"piste3"];
		kilp->sarja = haesarja_w(SQLDataSet1->FieldValues[L"sarja"], false);
		ust = SQLDataSet1->FieldValues[L"sukup"];
		if (ust != NULL && ust.Length() > 0)
			kilp->sukup = ust.c_str()[0];
		kilp->ikasarja = SQLDataSet1->FieldValues[L"ikasarja"];
		kilp->synt = SQLDataSet1->FieldValues[L"synt"];
		kilp->alisarja = SQLDataSet1->FieldValues[L"alisarja"];
		kilp->arvryhma = SQLDataSet1->FieldValues[L"arvryhma"];
		SQLDataSet1->Close();
		for (int ipv = 0 ; ipv < kilpparam.n_pv_akt; ipv++) {
			readSQLpv(kilp, dKilp, ipv);
			}
		return(true);
		}
	SQLDataSet1->Close();
	return(false);
}

//---------------------------------------------------------------------------
bool TFormMySQL::addSQLtulos(vatp *va, int dKilp, int ipv, int piste)
{
	wchar_t as[20];
	int tt0;

	if (piste > 0 && va->vatulos == 0)
		return(false);
	tt0 = piste == 0 ? t0 : 0;
	SQLDataSet1->CommandText =
		UnicodeString(L"INSERT into tulos (kilpailu, tietue, vaihe, piste, aika, "
			L"aikatime, sija) values ('")+
		UnicodeString(kilpparam.kilpkoodi)+L"','"+
		UnicodeString(dKilp)+L"','"+
		UnicodeString(ipv+1)+L"','"+
		UnicodeString(piste-1)+L"','"+
		UnicodeString(va->vatulos + tt0*TUNTI)+L"','"+
		((va->vatulos != TMAALI0) ? (UnicodeString(aikatowstr_cols_n(as,va->vatulos, tt0, 0, 8))) : UnicodeString(L"00:00:00")) + L"','"+
		UnicodeString(va->vasija)+L"')";
		SQLDataSet1->ExecSQL(true);
	return(true);
}
//---------------------------------------------------------------------------
bool TFormMySQL::addSQLpv(kilptietue *kilp, int dKilp, int ipv)
{
	wchar_t as[20], as2[20];
	kilppvtp *pv = kilp->pv + ipv;

	SQLDataSet1->CommandText =
		UnicodeString(L"INSERT into osottopv (kilpailu, tietue, vaihe, sarja, era, "
			L"bib, badge, badge2, laina, laina2, rata, selitys, pvpiste1, pvpiste2, tlahto, tlahtotime, "
			L"keskhyl, ampsakot, tasoitus, sakko, gps, qual, yhtaika, yhtaikatime, ysija) values ('")+
			UnicodeString(kilpparam.kilpkoodi)+L"','"+
			UnicodeString(dKilp)+L"','"+
			UnicodeString(ipv+1)+L"','"+
			UnicodeString(Sarjat[kilp->Sarja(ipv)].sarjanimi)+L"','"+
			UnicodeString(pv->era)+L"','"+
			UnicodeString(pv->bib)+L"','"+
			UnicodeString(pv->badge[0])+L"','"+
			UnicodeString(pv->badge[1])+L"','"+
			UnicodeString(pv->laina[0] == L'L' ? L"L" : L"")+L"','"+
			UnicodeString(pv->laina[1] == L'L' ? L"L" : L"")+L"','"+
			UnicodeString(pv->rata)+L"','"+
			UnicodeString(pv->selitys)+L"','"+
			UnicodeString(pv->pvpisteet[0])+L"','"+
			UnicodeString(pv->pvpisteet[1])+L"','"+
			UnicodeString(pv->tlahto+t0*TUNTI)+L"','"+
			((pv->tlahto != TMAALI0) ? (UnicodeString(aikatowstr_cols_n(as,pv->tlahto, t0, 0, 8))) : UnicodeString(L"00:00:00")) + L"','"+
			UnicodeString(pv->keskhyl)+L"','"+
			UnicodeString(kilp->asakkoStr(ipv))+L"','"+
			UnicodeString(pv->tasoitus)+L"','"+
			UnicodeString(pv->sakko)+L"','"+
			(pv->gps >= 'A' ? UnicodeString(pv->gps) : UnicodeString(L""))+L"','"+
			(pv->qual >= 'A' ? UnicodeString(pv->qual) : UnicodeString(L""))+L"','"+
			UnicodeString(kilp->ytulos(ipv))+L"','"+
			UnicodeString(aikatowstr_cols_n(as2,kilp->ytulos(ipv), 0, 0, 8))+L"','"+
			UnicodeString(kilp->y_sija(ipv))+L"')";
	SQLDataSet1->ExecSQL(true);
	for (int piste = pvparam[ipv].hiihtolahto ? 0 : 1; piste <= Sarjat[kilp->Sarja(ipv)].valuku[ipv]+1; piste++) {
		addSQLtulos(&pv->va[piste], dKilp, ipv, piste);
		}
	return(true);
}
//---------------------------------------------------------------------------

bool TFormMySQL::addSQLrivi(kilptietue *kilp, int dKilp)
{
	SQLDataSet1->CommandText =
		UnicodeString(L"INSERT into osanotot (kilpailu, tietue, id, lisno, intid, wrkoodi, "
			L"sukunimi, etunimi, arvo, seura, seuralyh, yhdistys, joukkue, maa, piiri, "
			L"ilmlista, piste1, piste2, piste3, sarja, sukup, ikasarja, synt, alisarja, arvryhma) values ('")+
		UnicodeString(kilpparam.kilpkoodi)+L"','"+
		UnicodeString(dKilp)+L"','"+
		UnicodeString(kilp->kilpno)+L"','"+
		UnicodeString(kilp->lisno[0])+L"','"+
		UnicodeString(kilp->lisno[1])+L"','"+
		UnicodeString(kilp->wrkoodi)+L"','"+
		UnicodeString(kilp->sukunimi)+L"','"+
		UnicodeString(kilp->etunimi)+L"','"+
		UnicodeString(kilp->arvo)+L"','"+
		UnicodeString(kilp->seura)+L"','"+
		UnicodeString(kilp->seuralyh)+L"','"+
		UnicodeString(kilp->yhdistys)+L"','"+
		UnicodeString(kilp->joukkue)+L"','"+
		UnicodeString(kilp->maa)+L"','"+
		UnicodeString(kilp->piiri)+L"','"+
		UnicodeString(kilp->ilmlista)+L"','"+
		UnicodeString(kilp->pisteet[0])+L"','"+
		UnicodeString(kilp->pisteet[1])+L"','"+
		UnicodeString(kilp->pisteet[2])+L"','"+
		UnicodeString(Sarjat[kilp->sarja].sarjanimi)+L"','"+
		(kilp->sukup >= L'A' ? UnicodeString(kilp->sukup) : UnicodeString(L""))+L"','"+
		UnicodeString(kilp->ikasarja)+L"','"+
		UnicodeString(kilp->synt)+L"','"+
		UnicodeString(kilp->alisarja)+L"','"+
		UnicodeString(kilp->arvryhma)+L"')";
	SQLDataSet1->ExecSQL(true);
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		addSQLpv(kilp, dKilp, ipv);
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

	kilp.GETREC(dKilp);
	if (kilp.kilpstatus)
		return(false);

	SQLDataSet1->CommandText =
		UnicodeString(L"SELECT tietue from osanotot where kilpailu='")+
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
bool TFormMySQL::updateSQLtulos(vatp *va, int dKilp, int ipv, int piste)
{
	wchar_t as[20];
	int tt0;
	vatp va1;

	tt0 = piste == 0 ? t0 : 0;
	if (!readSQLtulos(&va1, dKilp, ipv, piste))
		addSQLtulos(va, dKilp, ipv, piste);
	else if (va->vatulos != va1.vatulos) {
		SQLDataSet1->CommandText =
			UnicodeString(L"UPDATE tulos SET \r\n")+
			L"aika="+UnicodeString(va->vatulos + tt0*TUNTI)+L",\r\n"+
			L"aikatime='"+
			((va->vatulos != TMAALI0) ? (UnicodeString(aikatowstr_cols_n(as,va->vatulos, tt0, 0, 8))) : UnicodeString(L"00:00:00")) + L"',\r\n" +
			L"sija="+UnicodeString(va->vasija)+L"\r\n"
			L"WHERE kilpailu='"+kilpparam.kilpkoodi+L"' AND tietue="+UnicodeString(dKilp)+
			L" AND vaihe="+UnicodeString(ipv+1)+L" AND piste="+UnicodeString(piste-1);
		SQLDataSet1->ExecSQL(true);
		}
	return(true);
}
//---------------------------------------------------------------------------
bool TFormMySQL::updateSQLpv(kilptietue *kilp, kilppvtp *pv1, int dKilp, int ipv)
{
	wchar_t as[20], as2[20];
	kilppvtp *pv = kilp->pv + ipv;

	if (*pv == *pv1)
		return(true);
	SQLDataSet1->CommandText =
		UnicodeString(L"SELECT tietue from osottopv where kilpailu='")+
		UnicodeString(kilpparam.kilpkoodi)+
		UnicodeString(L"' AND tietue=")+UnicodeString(dKilp)+
		UnicodeString(L" AND vaihe=")+UnicodeString(ipv+1)+UnicodeString(L";");
	SQLDataSet1->Open();
	if (SQLDataSet1->IsEmpty()) {
		SQLDataSet1->Close();
		addSQLpv(kilp, dKilp, ipv);
		}
	else {
		SQLDataSet1->Close();
		if (pv1->vert0(*pv)) {
			SQLDataSet1->CommandText =
				UnicodeString(L"UPDATE osottopv SET \r\n")+
				((pv->sarja != pv1->sarja) ? (L"sarja='"+UnicodeString(Sarjat[pv->sarja].sarjanimi)+L"',\r\n") : UnicodeString(L""))+
				(pv->era != pv1->era ? (L"era="+UnicodeString(pv->era)+L",\r\n") : UnicodeString(L""))+
				(pv->bib != pv1->bib ? (L"bib="+UnicodeString(pv->bib)+L",\r\n") : UnicodeString(L""))+
				(pv->badge[0] != pv1->badge[0] ? (L"badge="+UnicodeString(pv->badge[0])+L",\r\n") : UnicodeString(L""))+
				(pv->badge[1] != pv1->badge[1] ? (L"badge2="+UnicodeString(pv->badge[1])+L",\r\n") : UnicodeString(L""))+
				(pv->laina[0] != pv1->laina[0] ? (L"laina='"+UnicodeString(pv->laina[0])+L"',\r\n") : UnicodeString(L""))+
				(pv->laina[1] != pv1->laina[1] ? (L"laina2='"+UnicodeString(pv->laina[1])+L"',\r\n") : UnicodeString(L""))+
				(wcscmp(pv->rata, pv1->rata) ? (L"rata='"+UnicodeString(pv->rata)+L"',\r\n") : UnicodeString(L""))+
				(wcscmp(pv->selitys, pv1->selitys) ? (L"selitys='"+UnicodeString(pv->selitys)+L"',\r\n") : UnicodeString(L""))+
				(pv->pvpisteet[0] != pv1->pvpisteet[0] ? (L"pvpiste1="+UnicodeString(pv->pvpisteet[0])+L",\r\n") : UnicodeString(L""))+
				(pv->pvpisteet[1] != pv1->pvpisteet[1] ? (L"pvpiste2="+UnicodeString(pv->pvpisteet[1])+L",\r\n") : UnicodeString(L""))+
				(pv->tlahto != pv1->tlahto ? (L"tlahto="+UnicodeString(pv->tlahto+t0*TUNTI)+L",\r\n") : UnicodeString(L""))+
				(pv->tlahto != pv1->tlahto ? (L"tlahtotime='"+
					((pv->tlahto != TMAALI0) ? (UnicodeString(aikatowstr_cols_n(as,pv->tlahto, t0, 0, 8))) : UnicodeString(L"00:00:00")) +L"',\r\n") : UnicodeString(L""))+
				(pv->keskhyl != pv1->keskhyl ? (L"keskhyl='"+UnicodeString(pv->keskhyl)+L"',\r\n") : UnicodeString(L""))+
				(wmemcmp(pv->asakot, pv1->asakot, Sarjat[pv->sarja].lsak[ipv]) ? (L"ampsakot='"+UnicodeString(kilp->asakkoStr(ipv))+L"',\r\n") : UnicodeString(L""))+
				(pv->tasoitus != pv1->tasoitus ? (L"tasoitus="+UnicodeString(pv->tasoitus)+L",\r\n") : UnicodeString(L""))+
				(pv->sakko != pv1->sakko ? (L"sakko="+UnicodeString(pv->sakko)+L",\r\n") : UnicodeString(L""))+
				(pv->gps != pv1->gps ? (L"gps="+UnicodeString(pv->gps)+L",\r\n") : UnicodeString(L""))+
				(pv->qual != pv1->qual ? (L"qual="+UnicodeString(pv->qual)+L",\r\n") : UnicodeString(L""))+
				(pv->va[1].vatulos != pv1->va[1].vatulos ? (L"yhtaika="+UnicodeString(kilp->ytulos(ipv))+L",\r\n") : UnicodeString(L""))+
				(pv->va[1].vatulos != pv1->va[1].vatulos ? (L"yhtaikatime='"+UnicodeString(aikatowstr_cols_n(as2,kilp->ytulos(ipv), 0, 0, 8))+L"',\r\n") : UnicodeString(L""))+
				L"ysija="+UnicodeString(kilp->y_sija(ipv))+L"\r\n"
				L"WHERE kilpailu='"+kilpparam.kilpkoodi+L"' AND tietue="+UnicodeString(dKilp)+
				L" AND vaihe="+UnicodeString(ipv+1);
			SQLDataSet1->ExecSQL(true);
			}
		for (int piste = pvparam[ipv].hiihtolahto ? 0 : 1; piste <= Sarjat[kilp->Sarja(ipv)].valuku[ipv]+1; piste++) {
			updateSQLtulos(&pv->va[piste], dKilp, ipv, piste);
			}
		}
	return(true);
}
//---------------------------------------------------------------------------

bool TFormMySQL::updateSQLrivi(kilptietue *kilp, int dKilp)
{
	kilptietue kilp1;

	kilp1.nollaa();
	if (!readSQLrivi(&kilp1, dKilp))
		return(false);

	if (kilp1 == *kilp)
		return(false);

	if (kilp->vert0(kilp1)) {
		SQLDataSet1->CommandText =
			UnicodeString(L"UPDATE osanotot SET \r\n")+
			(kilp->kilpno != kilp1.kilpno ? (L"id="+UnicodeString(kilp->kilpno)+L",\r\n") : UnicodeString(L""))+
			(kilp->lisno[0] != kilp1.lisno[0] ? (L"lisno="+UnicodeString(kilp->lisno[0])+L",\r\n") : UnicodeString(L""))+
			(kilp->lisno[1] != kilp1.lisno[1] ? (L"intid="+UnicodeString(kilp->lisno[1])+L",\r\n") : UnicodeString(L""))+
			(wcscmp(kilp->wrkoodi, kilp1.wrkoodi) ? (L"wrkoodi='"+UnicodeString(kilp->wrkoodi)+L"',\r\n") : UnicodeString(L""))+
			(wcscmp(kilp->sukunimi, kilp1.sukunimi) ? (L"sukunimi='"+UnicodeString(kilp->sukunimi)+L"',\r\n") : UnicodeString(L""))+
			(wcscmp(kilp->etunimi, kilp1.etunimi) ? (L"etunimi='"+UnicodeString(kilp->etunimi)+L"',\r\n") : UnicodeString(L""))+
			(wcscmp(kilp->arvo, kilp1.arvo) ? (L"arvo='"+UnicodeString(kilp->arvo)+L"',\r\n") : UnicodeString(L""))+
			(wcscmp(kilp->seura, kilp1.seura) ? (L"seura='"+UnicodeString(kilp->seura)+L"',\r\n") : UnicodeString(L""))+
			(wcscmp(kilp->seuralyh, kilp1.seuralyh) ? (L"seuralyh='"+UnicodeString(kilp->seuralyh)+L"',\r\n") : UnicodeString(L""))+
			(wcscmp(kilp->yhdistys, kilp1.yhdistys) ? (L"yhdistys='"+UnicodeString(kilp->yhdistys)+L"',\r\n") : UnicodeString(L""))+
			(wcscmp(kilp->joukkue, kilp1.joukkue) ? (L"joukkue='"+UnicodeString(kilp->joukkue)+L"',\r\n") : UnicodeString(L""))+
			(wcscmp(kilp->maa, kilp1.maa) ? (L"maa='"+UnicodeString(kilp->maa)+L"',\r\n") : UnicodeString(L""))+
			(kilp->piiri != kilp1.piiri ? (L"piiri="+UnicodeString(kilp->piiri)+L",\r\n") : UnicodeString(L""))+
			(kilp->ilmlista != kilp1.ilmlista ? (L"ilmlista="+UnicodeString(kilp->ilmlista)+L",\r\n") : UnicodeString(L""))+
			(kilp->pisteet[0] != kilp1.pisteet[0] ? (L"piste1="+UnicodeString(kilp->pisteet[0])+L",\r\n") : UnicodeString(L""))+
			(kilp->pisteet[1] != kilp1.pisteet[1] ? (L"piste2="+UnicodeString(kilp->pisteet[1])+L",\r\n") : UnicodeString(L""))+
			(kilp->pisteet[2] != kilp1.pisteet[2] ? (L"piste3="+UnicodeString(kilp->pisteet[2])+L",\r\n") : UnicodeString(L""))+
			(kilp->sarja != kilp1.sarja ? (L"sarja='"+UnicodeString(Sarjat[kilp->sarja].sarjanimi)+L"',\r\n") : UnicodeString(L""))+
			(kilp->sukup != kilp1.sukup ? (L"sukup="+UnicodeString(kilp->sukup)+L",\r\n") : UnicodeString(L""))+
			(kilp->ikasarja != kilp1.ikasarja ? (L"ikasarja="+UnicodeString(kilp->ikasarja)+L",\r\n") : UnicodeString(L""))+
			(kilp->synt != kilp1.synt ? (L"synt="+UnicodeString(kilp->synt)+L",\r\n") : UnicodeString(L""))+
			(kilp->alisarja != kilp1.alisarja ? (L"salisarja="+UnicodeString(kilp->alisarja)+L",\r\n") : UnicodeString(L""))+
			L"arvryhma="+UnicodeString(kilp->arvryhma)+L"\r\n"
			L"WHERE kilpailu='"+kilpparam.kilpkoodi+L"' AND tietue="+UnicodeString(dKilp);
		SQLDataSet1->ExecSQL(true);
		}
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++) {
		updateSQLpv(kilp, kilp1.pv+ipv, dKilp, ipv);
		}
	return(true);
}
//---------------------------------------------------------------------------
bool TFormMySQL::deleteSQLrivi(int dKilp)
{
	SQLDataSet1->CommandText =
		UnicodeString(L"DELETE from osanotot where kilpailu='")+
		UnicodeString(kilpparam.kilpkoodi)+
		UnicodeString(L"' and tietue=")+UnicodeString(dKilp)+UnicodeString(L";");
	SQLDataSet1->ExecSQL(true);
	SQLDataSet1->CommandText =
		UnicodeString(L"DELETE from osottopv where kilpailu='")+
		UnicodeString(kilpparam.kilpkoodi)+
		UnicodeString(L"' and tietue=")+UnicodeString(dKilp)+UnicodeString(L";");
	SQLDataSet1->ExecSQL(true);
	SQLDataSet1->CommandText =
		UnicodeString(L"DELETE from tulos where kilpailu='")+
		UnicodeString(kilpparam.kilpkoodi)+
		UnicodeString(L"' and tietue=")+UnicodeString(dKilp)+UnicodeString(L";");
	SQLDataSet1->ExecSQL(true);
	return(true);
}

//---------------------------------------------------------------------------

void __fastcall TFormMySQL::Button6Click(TObject *Sender)
{
	kilptietue kilp;

	readSQLrivi(&kilp, 1);
	Memo1->Lines->Add(kilp.sukunimi);
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
	FormMySQL->kesken = true;

	while (taustaon && !vaiheenvaihto && inkirjSQLThread) {
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
			if (!inkirjSQLThread)
				break;
			Sleep(200);
			if (!inkirjSQLThread)
				break;
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
		if (!inkirjSQLThread)
			break;
		Sleep(100);
		}
	FormMySQL->kesken = false;
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

void __fastcall TFormMySQL::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	for (int i = 0; i < 10 && kesken; i++)
		Sleep(200);
}
//---------------------------------------------------------------------------

