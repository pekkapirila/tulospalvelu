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

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#pragma hdrstop
#include "HkDeclare.h"
#include "HkMuotoilu.h"

extern wchar_t FISMaarFileName[200];
int flatxml = 1;

   wchar_t
      tag_event[] = L"Event",
	  tag_evnm[] = L"EventName",
	  tag_evcode[] = L"EventCode",
      tag_cl[] = L"EventClass",
	  tag_clnm[] = L"ClassName",
	  tag_clnml[] = L"ClassNameLong",
	  tag_clsty[] = L"ClassStyle",
	  tag_cldst[] = L"ClassDist",
	  tag_cmptor[] = L"Competitor",
		tag_stnr[] = L"StartNumber",
	  tag_order[] = L"Order",
	  tag_id[] = L"Identity",
	  tag_nm[] = L"Name",
	  tag_fnm[] = L"Family",
	  tag_gnm[] = L"Given",
	  tag_clubid[] = L"ClubID",
	  tag_clubnm[] = L"ClubName",
	  tag_nat[] = L"Nationality",
	  tag_st[] = L"StartTime",
	  tag_stsecs[] = L"STSecs",
	  tag_res[] = L"Time",
	  tag_ressecs[] = L"TSecs",
	  tag_behind[] = L"TimeBehind",
	  tag_rank[] = L"Rank",
	  tag_status[] = L"Status",
#ifdef MAKI
	  tag_splitts[] = L"SplitTimes",
	  tag_splitt[] = L"SplitTime",
	  tag_splittpos[] = L"SplitTimePos",
#else
	  tag_splitt[] = L"SplitTimes",
		tag_lastdate[] = L"LastDate",
		tag_partcount[] = L"PartCount",
#endif
	  tag_ce[] = L"Course",
//      tag_emno[] = L"Emit",
	  tag_ctl[] = L"Control",
	  tag_ctdst[] = L"ControlDist",
	  tag_cn[] = L"ControlOrder",
	  tag_cr[] = L"ControlRank",
	  tag_cc[] = L"CCode",
	  tag_ct[] = L"ControlTime",
//      tag_cd[] = L"CTDiff",
	  tag_ctsecs[] = L"CTSecs",
#if defined(TCPSIIRTO)
	  tag_update[] = L"Update",
	  tag_key[] = L"Key",
	  tag_msgno[] = L"Msgno",
	  tag_esplits[] = L"ESplits",
#endif
#ifdef MAKI
	  tag_cmptors[] = L"Competitors",
	  tag_pnts[] = L"Points",
	  tag_rounds[] = L"Rounds",
	  tag_round[] = L"Round",
	  tag_rndno[] = L"RoundNo",
	  tag_rndrank[] = L"RoundRank",
	  tag_rndpnts[] = L"RoundPoints",
	  tag_speed[] = L"Speed",
	  tag_dist[] = L"Dist",
	  tag_distpts[] = L"DistPoints",
	  tag_jdga[] = L"JudgeA",
	  tag_jdgb[] = L"JudgeB",
	  tag_jdgc[] = L"JudgeC",
	  tag_jdgd[] = L"JudgeD",
	  tag_jdge[] = L"JudgeE",
	  tag_jdg[] = L"JudgePoints",
	  tag_judges[] = L"Judges",
	  tag_judge[] = L"Judge",
	  tag_jdgpt[] = L"JdgPt",
	  tag_pttype[] = L"PtType",
	  tag_pt[] = L"Pts",
#endif
	  tag_penal[] = L"Penalties";

static wchar_t xmlpiste[MAXNLEIMA+1];
static int xmlorder;

int xmlIOF30ots(tulostusparamtp *tulprm);
int xmlIOF30loppu(tulostusparamtp *tulprm);
int xmlIOF30srjots(int sarja, tulostusparamtp *tulprm);
int xmlIOF30srjloppu(tulostusparamtp *tulprm);
void xmlIOF30tulos(kilptietue& kilp, INT sj, tulostusparamtp *tulprm);
int xmlTulosots(tulostusparamtp *tulprm);
int xmlTulosloppu(tulostusparamtp *tulprm);
int xmlTulossrjots(int sarja, tulostusparamtp *tulprm);
int xmlTulossrjloppu(tulostusparamtp *tulprm);
void xmlTulostulos(kilptietue& kilp, int sarja, int sj, tulostusparamtp *tulprm);

int xmlots(tulostusparamtp *tulprm)
   {
   wchar_t ln[202], *style;
   TextFl *tul_tied;

   if (tulprm->xmlstd == L'I' || tulprm->xmlstd == L'J')
	   return(xmlIOF30ots(tulprm));
   if (tulprm->xmlstd == L'L')
	   return(xmlTulosots(tulprm));
#ifdef MAKI
	style = L"skijump.xsl";
	if (tulprm->merkit == L'A')
		swprintf(ln, L"<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>\n"
			L"<!-- ?xml-stylesheet type=\"text/xsl\" href=\"%s\"? -->", style);
	else
		swprintf(ln, L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
			L"<!-- ?xml-stylesheet type=\"text/xsl\" href=\"%s\"? -->", style);
#else
   if (tulprm->xmlva == L'E')
	  style = L"tlsemittyyli.xsl";
	else
		style = L"tlstyyli.xsl";
	if (tulprm->merkit == L'A')
	   swprintf(ln, L"<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>\n"
			L"<?xml-stylesheet type=\"text/xsl\" href=\"%s\"?>", style);
	else
		swprintf(ln, L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
			L"<?xml-stylesheet type=\"text/xsl\" href=\"%s\"?>", style);
#endif

   tul_tied = tulprm->lstf->u.Fl;
   tulprm->writeline(ln);
   wcsncpy(ln, kilpailu, 80);
   ln[80] = 0;
   elimwbl(ln);
#if !defined(MAKI) && N_PV > 1
   swprintf(ln+wcslen(ln), L" - päivä %d", k_pv+1);
   if (k_pv && tulprm->yhttuljarj) wcscat(ln, L"yht.");
#endif
   tul_tied->put_wtag(tag_event, 0);
   if (pvparam[k_pv].RaceId[0])
		tul_tied->put_wxml_s(tag_evcode, pvparam[k_pv].RaceId, 0);
   else if (kilpparam.kilpkoodi[0])
		tul_tied->put_wxml_s(tag_evcode, kilpparam.kilpkoodi, 0);
   tul_tied->put_wxml_s(tag_evnm, ln, 0);
   return(0);
   }

int xmlloppu(tulostusparamtp *tulprm)
   {
   TextFl *tul_tied;

   if (tulprm->xmlstd == L'I' || tulprm->xmlstd == L'J')
	   return(xmlIOF30loppu(tulprm));
   if (tulprm->xmlstd == L'L')
	   return(xmlTulosloppu(tulprm));
   tul_tied = tulprm->lstf->u.Fl;
   tul_tied->put_wantitag(tag_event, 0);
   return(0);
   }

int xmlsrjots(int sarja, tulostusparamtp *tulprm)
   {
   TextFl *xmllstfl;
   int i, k, insrj = 0;
   wchar_t buf[40], *p;
   TextFl *tul_tied;

   if (tulprm->xmlstd == L'I' || tulprm->xmlstd == L'J')
	   return(xmlIOF30srjots(sarja, tulprm));
   if (tulprm->xmlstd == L'L')
	   return(xmlTulossrjots(sarja, tulprm));
	sijat(sarja);
	xmlorder = 1;
   tul_tied = tulprm->lstf->u.Fl;
   for (i = 0; i < MAXNLEIMA; i++) {
	  xmlpiste[i] = i+1;
	  }
   xmllstfl = new TextFl(L"XMLPISTE.LST", L"rt");
   if (xmllstfl->IsOpen()) {
	  memset(xmlpiste, 0, sizeof(xmlpiste));
	  while (!xmllstfl->Feof()) {
		 if (xmllstfl->ReadLine(buf, 38) == NULL)
			break;
		 if (!insrj) {
			if (!wmemcmp(buf, Sarjat[sarja].sarjanimi, wcslen(Sarjat[sarja].sarjanimi))) {
			   insrj = 1;
			   }
			}
		 else {
			p = wcstok(buf, L" ,;\t\n");
			if (p && (k = _wtoi(p)) > 0) {
			   if ((p = wcstok(NULL, L" ,;\t\n")) != 0) {
				  i = _wtoi(p);
				  if (i > 0 && i <= MAXNLEIMA) {
					 xmlpiste[i-1] = k;
					 }
				  }
			   }
			else
			   break;
			}
		 }
	  }
	delete xmllstfl;
   tul_tied->put_wtag(tag_cl, 0);
   if (flatxml) {
		tul_tied->put_wxml_s(tag_evcode, kilpparam.kilpkoodi, 0);
		}
   tul_tied->put_wxml_s(tag_clnm, Sarjat[sarja].sarjanimi, 0);
   if (Sarjat[sarja].psarjanimi[0])
	   tul_tied->put_wxml_s(tag_clnml, Sarjat[sarja].psarjanimi, 0);
   else
	   tul_tied->put_wxml_s(tag_clnml, Sarjat[sarja].sarjanimi, 0);
#ifdef MAKI
	if (k_pv != makipv)
#endif
		{
		wchar_t tapa[3] = L"";
		wcscpy(buf, Sarjat[sarja].matka[k_pv]);
		if (wcslen(buf)) {
			if (buf[wcslen(buf)-1] >= L'P') {
				if (buf[wcslen(buf)-2] >= L'P') {
					wcscpy(tapa, buf + wcslen(buf)-2);
					buf[wcslen(buf)-2] = 0;
					}
				else
					tapa[0] = buf[wcslen(buf)-1];
				buf[wcslen(buf)-1] = 0;
				}
			tul_tied->put_wxml_s(tag_cldst, buf, 0);
			}
		if (Sarjat[sarja].tapa[k_pv][0] >= L'P')
			wcsncpy(tapa, Sarjat[sarja].tapa[k_pv],2);
		buf[0] = 0;
		for (i = 0; i < 2; i++) {
			if (wcswcind(tapa[i], L"VP") >= 0) {
				if (buf[0])
					wcscat(buf,L",");
				wcscat(buf, tapa[i] == L'P' ? L"Classic" : L"Free");
				}
			}
		if (buf[0])
			tul_tied->put_wxml_s(tag_clsty, buf, 0);
		for (i = 0; i < Sarjat[sarja].valuku[k_pv]; i++) {
			if (Sarjat[sarja].va_matka[k_pv][i][0]) {
				swprintf(buf, L"SplitDist_%d", i+1);
				tul_tied->put_wxml_s(buf, Sarjat[sarja].va_matka[k_pv][i], 0);
				}
			}
	  }
   return(0);
   }

int xmlsrjloppu(tulostusparamtp *tulprm)
   {
   TextFl *tul_tied;

   if (tulprm->xmlstd == L'I' || tulprm->xmlstd == L'J')
	   return(xmlIOF30srjloppu(tulprm));
   if (tulprm->xmlstd == L'L')
	   return(xmlTulossrjloppu(tulprm));
   tul_tied = tulprm->lstf->u.Fl;
   tul_tied->put_wantitag(tag_cl, 0);
   return(0);
   }

void xmltulos(kilptietue& kilp, int sarja, int sj, tulostusparamtp *tulprm)
   {
	wchar_t ln[100];
	INT i,piste = 0;
	INT32 tl, ero;
	emittp em;
	INT k, tulkinta[MAXNLEIMA+1];
	INT32 enn;
	ratatp *rt;
	time_t tm_t;
	struct tm *dttm;
	TextFl *tul_tied;
	static INT32 karki;
	bool yht = tulprm->yhttuljarj;

   if (tulprm->xmlstd == L'I' || tulprm->xmlstd == L'J')
	   return(xmlIOF30tulos(kilp, sj, tulprm));
   if (tulprm->xmlstd == L'L')
	   return(xmlTulostulos(kilp, sarja, sj, tulprm));
	tul_tied = tulprm->lstf->u.Fl;
	if (tulprm->yhttuljarj)
		if (!kilp.p_aika(kilpparam.valuku+1))
			return;
		else
			if (autofileparam.af_flag && !autofileparam.af_kaikki && wcswcind(kilp.tark(k_pv), L"KOHMX") == -1) {
				for (i = 0; i <= Sarjat[kilp.Sarja()].valuku[k_pv]; i++) {
					if (kilp.p_aika(i) != 0)
						break;
					}
				if (i > Sarjat[kilp.Sarja()].valuku[k_pv])
					return;
				}
			tul_tied->put_wtag(tag_cmptor, 0);
			tul_tied->put_wxml_d(tag_stnr, kilp.id(), 0);
//			if (fiskoodi == 1) {
				if (kilp.lisno[1]) {
					kilp.strval(tulprm, ln, 15, F_LISNO2, k_pv, 0, 0, 0);
					tul_tied->put_wxml_s(tag_id, ln, 0);
					}
				else if (kilp.lisno[0])
					 tul_tied->put_wxml_d(tag_id, kilp.lisno[0], 0);
//				}
			if (flatxml) {
				tul_tied->put_wxml_s(tag_evcode, kilpparam.kilpkoodi, 0);
				tul_tied->put_wxml_s(tag_clnm, Sarjat[kilp.Sarja()].sarjanimi, 0);
				tul_tied->put_wxml_d(tag_order, xmlorder++, 0);
				tul_tied->put_wxml_s(tag_fnm, kilp.sukunimi, 0);
				tul_tied->put_wxml_s(tag_gnm, kilp.etunimi, 0);
				tul_tied->put_wxml_s(tag_nm, kilp.nimi(ln, sizeof(ln)/2, tulprm->tulmuot.vaihdanimet != 0), 0);
				}
			else {
				tul_tied->put_wtag(tag_nm, 0);
				tul_tied->put_wxml_s(tag_fnm, kilp.sukunimi, 0);
				tul_tied->put_wxml_s(tag_gnm, kilp.etunimi, 0);
				tul_tied->put_wantitag(tag_nm, 0);
				}
			if (kilp.seuralyh[0])
				tul_tied->put_wxml_s(tag_clubid, kilp.seuralyh, 0);
			tul_tied->put_wxml_s(tag_clubnm, kilp.seuratieto(0, false), 0);
			if (kilp.maa[0]) {
				tul_tied->put_wxml_s(tag_nat, kilp.maa, 0);
				}
			if (kilp.hyv()) {
#if defined(MAKI)
				if (k_pv == makipv) {
					tul_tied->put_wxml_d(tag_rank, sj, 0);
					tul_tied->put_wxml_s(tag_pnts, format1(yhtpisteet(kilp, aktkierros), ln, -1, wdec_sep), 0);
					tl = kilp.tulos2(0) - pkarki[kilp.Sarja()][0];
					AIKATOWSTRS(ln, tl, 0);
					ln[2] = L':';
					ln[5] = L':';
					ln[8] = 0;

					wpoistanollat(ln);
					tul_tied->put_wxml_s(tag_res, ln, 0);
					swprintf(ln, L"%ld%c%2.2ld", tl/SEK, wdec_sep, (tl%SEK)*100/SEK);
					if (pvparam[k_pv].laika2 < 11 && wcslen(ln)-11+pvparam[k_pv].laika2 > 0)
					ln[wcslen(ln)-11+pvparam[k_pv].laika2] = 0;
					tul_tied->put_wxml_s(tag_ressecs, ln, 0);
					tul_tied->put_wxml_s(tag_status, L"OK", 0);
					}
				else
#endif
					{
					if (pvparam[k_pv].hiihtolahto)
						tl = kilp.p_aika(-1);
					else
						tl = kilp.pv[k_pv].tlahto;
					AIKATOWSTRS(ln, tl, t0);
					ln[2] = L':';
					ln[5] = L':';
					ln[8] = 0;
					tul_tied->put_wxml_s(tag_st, ln, 0);
					tul_tied->put_wxml_d(tag_stsecs, kilp.pv[k_pv].tlahto/SEK+t0*3600L, 0);
					}
				if (sj) {
					tul_tied->put_wxml_d(tag_rank, sj, 0);
					tl = kilp.tulos2(0, 2);
					if (k_pv > 0 && tulprm->yhttuljarj) {
						piste = kilpparam.valuku+1;
						tl = kilp.tulos2(piste, 2);
						}
					if (sj == 1)
						karki = tl;
					ero = tl - karki;
					AIKATOWSTRS(ln, tl, 0);
					ln[2] = L':';
					ln[5] = L':';
					ln[pvparam[k_pv].laika2] = 0;
					if (pvparam[k_pv].laika2 >= 10)
						ln[8] = L'.';
			//		 if (!flatxml)
					wpoistanollat(ln);
					if (k_pv > 0 && Sarjat[kilp.Sarja()].luonne[k_pv] == 2 && sj > 1) {
						wmemmove(ln+1, ln, 11);
						ln[0] = L'+';
						}
					if (piste == kilpparam.valuku+1)
						tul_tied->put_wxml_s(tag_res, ln, 0, L"type=\"Cumulative\"");
					else
						tul_tied->put_wxml_s(tag_res, ln, 0);
					swprintf(ln, L"%ld.%03.3ld", kilp.tulos2(piste, 2)/SEK, kilp.tulos2(piste, 2)%SEK);
					if (pvparam[k_pv].laika2 < 12 && wcslen(ln)-12+pvparam[k_pv].laika2 > 0)
						ln[wcslen(ln)-12+pvparam[k_pv].laika2] = 0;
					tul_tied->put_wxml_s(tag_ressecs, ln, 0);
					if (ero > 0) {
						AIKATOWSTRS(ln, ero, 0);
						ln[2] = L':';
						ln[5] = L':';
						ln[pvparam[k_pv].laika2] = 0;
						if (pvparam[k_pv].laika2 >= 10)
							ln[8] = L'.';
			//		 if (!flatxml)
					wpoistanollat(ln);
					wmemmove(ln+1, ln, 11);
					ln[0] = L'+';
					tul_tied->put_wxml_s(tag_behind, ln, 0);
					}
				else
					tul_tied->put_wxml_s(tag_behind, L" ", 0);
				if (Sarjat[kilp.Sarja()].lsak[k_pv])
					tul_tied->put_wxml_s(tag_penal, kilp.asakkoStr(k_pv), 0);
				}
			tul_tied->put_wxml_s(tag_status, L"", 0);
			}
		else {
			ln[0] = 0;
			switch (kilp.tark(k_pv)) {
				case L'K':
					wcscpy(ln, L"DNF");
					break;
				case L'O':
					wcscpy(ln, L"LAP");
					break;
				case L'H':
					wcscpy(ln, L"DQ");
					break;
				case L'M':
					wcscpy(ln, L"MIS");
					break;
				case L'X':
					wcscpy(ln, L"EXCL");
					break;
				case L'E':
				case L'B':
				case L'V':
					wcscpy(ln, L"DNS");
					break;
				}
			if (ln[0]) {
				tul_tied->put_wxml_s(tag_status, ln, 0);
				tul_tied->put_wxml_s(tag_res, ln, 0);
				}
			}
		if (tulprm->xmlva == L'E') {
			getem(&em, kilp.id(), 0);
			if (em.kilpno == kilp.id()) {
				int tark;
				tark = tarkista(&em, tulkinta, 0, &kilp);
				enn = haeennakko(&em, &kilp);
				rt = haerata(&kilp);
			if (rt) {
				tul_tied->put_wxml_s(tag_ce, rt->tunnus, 0);
				tul_tied->put_wtag(tag_splitt, 0);
				for (k = MAXNLEIMA-1; k >= 0; k--)
					if (tulkinta[k] > 0)
						i = k;
				if (tulkinta[i] > 1) {
					for (k = i-1; k >= 0; k--)
						if (em.ctrltime[k] > em.ctrltime[k+1])
							break;
					i = k;
					if (em.ctrltime[k] > em.ctrltime[k+1])
						i++;
					}
				for (k = i; k < MAXNLEIMA; k++) {
					int tlk;
					if ((tlk = tulkinta[k]) == 0)
						continue;
					if (tlk < 0) {
						int kk;
						for (kk = 0; kk < MAXNLEIMA; kk++)
							if (tlk == -tulkinta[kk])
								break;
						if (kk < MAXNLEIMA)
							continue;
						tlk = - tlk;
						}
		//				   continue;
		//               tulkinta[k] = -tulkinta[k];
					if (tlk > rt->rastiluku || tlk > MAXNLEIMA || !xmlpiste[tlk-1])
						continue;
					AIKATOWSTRS(ln, SEK*(em.ctrltime[k]-enn) , 0);
					ln[2] = L':';
					ln[5] = L':';
					ln[8] = 0;
					wpoistanollat(ln);
					tul_tied->put_wtag(tag_ctl, 0);
					tul_tied->put_wxml_d(tag_cn, xmlpiste[tlk-1], 0);
					tul_tied->put_wxml_d(tag_cc, rt->rastikoodi[tlk-1], 0);
					tul_tied->put_wxml_s(tag_ct, ln, 0);
					tul_tied->put_wxml_d(tag_ctsecs, em.ctrltime[k]-enn, 0);
					tul_tied->put_wantitag(tag_ctl, 0);
					}
				tul_tied->put_wantitag(tag_splitt, 0);
				}
			}
		}
#ifndef MAKI
	if (k_pv == 0 && flatxml && Sarjat[kilp.Sarja()].valuku[k_pv] < kilpparam.valuku &&
		kilp.pv[0].va[kilpparam.valuku+1].vatulos > 24*TUNTI) {
		tm_t = kilp.pv[0].va[kilpparam.valuku+1].vatulos;
		dttm = localtime(&tm_t);
		swprintf(ln, L"%02d.%02d.%4d", dttm->tm_mday, dttm->tm_mon+1,
			dttm->tm_year+1900);
		tul_tied->put_wxml_s(tag_lastdate, ln, 0);
		tul_tied->put_wxml_d(tag_partcount, kilp.pv[0].va[kilpparam.valuku+1].vasija, 0);
		}
//   if (tulprm->yhttuljarj && tulprm.xmlva == L'M') {
	if (tulprm->xmlva == L'M') {
		for (i = 0; i < kilpparam.valuku; i++) {
			if (kilp.tulos2(i+1))
				break;
			}
		if (i < kilpparam.valuku) {
			tul_tied->put_wtag(tag_splitt, 0);
			for (i = 1; i <= kilpparam.valuku; i++) {
				if (!xmlpiste[i-1])
					continue;
				if ((tl = kilp.tulos2(i, 2)) > 0) {
					AIKATOWSTRS(ln, tl, 0);
					ln[2] = L':';
					ln[5] = L':';
					ln[pvparam[k_pv].laika2] = 0;
					if (pvparam[k_pv].laika2 >= 10)
					ln[8] = L'.';
//			   if (!flatxml)
					wpoistanollat(ln);
					tul_tied->put_wtag(tag_ctl, 0);
					tul_tied->put_wxml_s(tag_ctdst, Sarjat[kilp.Sarja()].va_matka[k_pv][i-1], 0);
					tul_tied->put_wxml_d(tag_cn, xmlpiste[i-1], 0);
					tul_tied->put_wxml_d(tag_cr, kilp.pv[k_pv].va[i+1].vasija, 0);
					tul_tied->put_wxml_s(tag_ct, ln, 0);
					swprintf(ln, L"%ld.%03.3ld", tl/SEK, tl%SEK);
					if (pvparam[k_pv].laika2 < 12 && wcslen(ln)-12+pvparam[k_pv].laika2 > 0)
						ln[wcslen(ln)-12+pvparam[k_pv].laika2] = 0;
					tul_tied->put_wxml_s(tag_ctsecs, ln, 0);
					tul_tied->put_wantitag(tag_ctl, 0);
					}
				}
			tul_tied->put_wantitag(tag_splitt, 0);
			}
		}
	tul_tied->put_wantitag(tag_cmptor, 0);
	}
#endif // !MAKI

#ifdef MAKI
static int xmlvaots(int sarja)
   {
   FILE *tul_tied;

   tul_tied = autofileparam.af_flag ? autofileparam.af_tied : tul_tied_val;
   put_tag(tul_tied, tag_splitts);
   return(0);
   }

static int xmlvaloppu(void)
   {
   FILE *tul_tied;

   tul_tied = autofileparam.af_flag ? autofileparam.af_tied : tul_tied_val;
   put_antitag(tul_tied, tag_splitts);
   return(0);
   }

void xmlvajat(kilptietue *kilp)
	{
   char ln[100];
   INT piste = 0;
   INT32 tl;
   FILE *tul_tied;

	if (!hyv(kilp))
		return;
   tul_tied = autofileparam.af_flag ? autofileparam.af_tied : tul_tied_val;
	for (piste = 1; piste <= Sarjat[kilp->Sarja()].valuku; piste++) {
		if (!xmlpiste[piste-1])
			continue;
		if ((tl = tulos2(kilp, piste)) == 0)
			continue;
	   put_tag(tul_tied, tag_splitt);
		put_xml_s(tul_tied, tag_evcode, kilpparam.kilpkoodi);
		put_xml_s(tul_tied, tag_clnm, (char *)Sarjat[kilp->Sarja()].sarjanimi);
	   put_xml_d(tul_tied, tag_stnr, kilp->id());
		AIKATOWSTRS(ln, tl, 0);
		ln[2] = ':';
		ln[5] = ':';
		ln[pvparam[k_pv].laika2] = 0;
		if (pvparam[k_pv].laika2 >= 10)
			ln[8] = dec_sep;
		wpoistanollat(ln);
		put_xml_s(tul_tied, tag_ctdst, (char *) Sarjat[kilp->Sarja()].va_matka[k_pv][piste-1]);
		put_xml_d(tul_tied, tag_cn, xmlpiste[piste-1]);
		put_xml_d(tul_tied, tag_cr, kilp->vasija[k_pv][piste+1]);
		put_xml_s(tul_tied, tag_ct, ln);
		swprintf(ln, "%ld%c%2.2ld", tl/SEK, dec_sep, (tl%SEK)*100/SEK);
		if (pvparam[k_pv].laika2 < 11 && wcslen(ln)-11+pvparam[k_pv].laika2 > 0)
			ln[wcslen(ln)-11+pvparam[k_pv].laika2] = 0;
		put_xml_s(tul_tied, tag_ctsecs, ln);
	   put_antitag(tul_tied, tag_splitt);
		}
	}

static int xmlkierrosots(int sarja)
   {
   FILE *tul_tied;

   tul_tied = autofileparam.af_flag ? autofileparam.af_tied : tul_tied_val;
   put_tag(tul_tied, tag_rounds);
   return(0);
   }

static int xmlkierrosloppu(void)
   {
   FILE *tul_tied;

   tul_tied = autofileparam.af_flag ? autofileparam.af_tied : tul_tied_val;
   put_antitag(tul_tied, tag_rounds);
   return(0);
   }

static int xmltuomariots(int sarja)
   {
   FILE *tul_tied;

   tul_tied = autofileparam.af_flag ? autofileparam.af_tied : tul_tied_val;
   put_tag(tul_tied, tag_judges);
   return(0);
   }

static int xmltuomariloppu(void)
   {
   FILE *tul_tied;

   tul_tied = autofileparam.af_flag ? autofileparam.af_tied : tul_tied_val;
   put_antitag(tul_tied, tag_judges);
   return(0);
   }

void xmltuomaripisteet(kilptietue *kilp)
	{
   char ln[100], st1[10];
   INT i,j,r,ra = 1,ry;
   INT32 tl;
   FILE *tul_tied;

	if (!hyv(kilp))
		return;
   tul_tied = autofileparam.af_flag ? autofileparam.af_tied : tul_tied_val;
	ry = aktkierros;
	if (ry == 0)
		ra = 0;
	for (r = ra; r <= ry; r++) {
		if ((tl = hyppypisteet(kilp, r)) == 0)
			continue;
		for (i = 0; i < tuomariluku; i++) {
			for (j = -1; j < 3; j++) {
				if ((j >= 0 && kilp->tuomari[r][i][j] >= 0) || j == -1) {
					put_tag(tul_tied, tag_jdgpt);
					put_xml_s(tul_tied, tag_evcode, kilpparam.kilpkoodi);
					put_xml_d(tul_tied, tag_stnr, kilp->id());
					put_xml_d(tul_tied, tag_rndno, r);
					st1[0] = 'A'+i;
					st1[1] = 0;
					put_xml_s(tul_tied, tag_judge, st1);
					put_xml_d(tul_tied, tag_rndrank, kilp->vasija[k_pv][2*r]);
					switch (j) {
						case -1:
							wcscpy(st1, "Tot");
							break;
						case 0:
							wcscpy(st1, "Flight");
							break;
						case 1:
							wcscpy(st1, "Landing");
							break;
						case 2:
							wcscpy(st1, "Outrun");
							break;
						}
					put_xml_s(tul_tied, tag_pttype, st1);
					if (j == -1)
						put_xml_s(tul_tied, tag_pt, format1(tuomaripisteet(kilp,r,i), ln, -1, dec_sep));
					else
						put_xml_s(tul_tied, tag_pt, format1(kilp->tuomari[r][i][j], ln, -1, dec_sep));
					put_antitag(tul_tied, tag_jdgpt);
					}
				}
			}
		}
	}

void xmlkierrospisteet(kilptietue *kilp)
	{
   char ln[100], *tag, tag1[10];
   INT i,j,r,ra = 1,ry,piste = 0;
   INT32 tl;
   FILE *tul_tied;

	if (!hyv(kilp))
		return;
   tul_tied = autofileparam.af_flag ? autofileparam.af_tied : tul_tied_val;
	ry = aktkierros;
	if (ry == 0)
		ra = 0;
	for (r = ra; r <= ry; r++) {
		if ((tl = hyppypisteet(kilp, r)) == 0)
			continue;
	   put_tag(tul_tied, tag_round);
		put_xml_s(tul_tied, tag_evcode, kilpparam.kilpkoodi);
		put_xml_s(tul_tied, tag_clnm, (char *)Sarjat[kilp->Sarja()].sarjanimi);
	   put_xml_d(tul_tied, tag_stnr, kilp->id());
	   put_xml_d(tul_tied, tag_rndno, r);
		put_xml_d(tul_tied, tag_rndrank, kilp->vasija[k_pv][2*r]);
		put_xml_s(tul_tied, tag_pnts, format1(tl, ln, -1, dec_sep));
		put_xml_s(tul_tied, tag_speed, format1(kilp->nopeus[r], ln, -1, dec_sep));
		put_xml_s(tul_tied, tag_dist, format1(kilp->pituus[r], ln, -1, dec_sep));
		put_xml_s(tul_tied, tag_distpts, format1(pituuspisteet(kilp,r), ln, -1, dec_sep));
		if (tuomariluku) {
			put_xml_s(tul_tied, tag_jdg, format1(tyylipisteet(kilp,r), ln, -1, dec_sep));
			for (i = 0; i < tuomariluku; i++) {
				switch (i) {
					case 0:
						tag = tag_jdga;
						break;
					case 1:
						tag = tag_jdgb;
						break;
					case 2:
						tag = tag_jdgc;
						break;
					case 3:
						tag = tag_jdgd;
						break;
					case 4:
						tag = tag_jdge;
						break;
					}
				put_xml_s(tul_tied, tag, format1(tuomaripisteet(kilp,r,i), ln, -1, dec_sep));
#ifdef EI_OLE
				for (j = 0; j < 3; j++) {
					if (kilp->tuomari[r][i][j] >= 0) {
						wcscpy(tag1, tag);
						switch (j) {
							case 0:
								strcat(tag1, "Fl");
								break;
							case 1:
								strcat(tag1, "La");
								break;
							case 2:
								strcat(tag1, "Ou");
								break;
							}
						put_xml_s(tul_tied, tag1, format1(kilp->tuomari[r][i][j], ln, -1, dec_sep));
						}
					}
#endif
				}
			}
	   put_antitag(tul_tied, tag_round);
		}
	}
#endif

#ifdef TCPSIIRTO
int xmlsanoma(char *buf, int maxlen, kilptietue& kilp, int dkilp, int piste, int msgno)
   {
   int len = 0, len1;
   wchar_t ln[1002], str[182];
   wchar_t wbuf[1000];
	wchar_t tapa[3] = L"";
   INT32 tl;

   buf[0] = 0;
   if (xmlsarja >= 0 && kilp.Sarja() != xmlsarja)
	  return(0);
   wcscpy(ln, L"<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>\n");
   len1 = wcslen(ln);
   if (len+len1 > maxlen)
	  return(0);
   wcscpy(wbuf+len, ln);
   len += len1;
   set_wtag(ln, tag_update);
   len1 = wcslen(ln);
   if (len+len1 > maxlen)
	  return(0);
   wcscpy(wbuf+len, ln);
   len += len1;
   wcsncpy(str, kilpailu, 80);
   str[80] = 0;
   elimwbl(str);
   if (kilpparam.n_pv_akt > 1) {
		swprintf(str+wcslen(str), L" - päivä %d", k_pv+1);
	   if (k_pv && piste == kilpparam.valuku+1) wcscat(str, L"yht.");
	   }
   set_wxml_s(ln, tag_evnm, str);
   set_wxml_d(ln+wcslen(ln), tag_msgno, msgno);
   len1 = wcslen(ln);
   if (len+len1 > maxlen)
	  return(0);
   wcscpy(wbuf+len, ln);
   len += len1;
   ln[0] = 0;
   if (k_pv && piste == 0)
	  piste = kilpparam.valuku+1;
   if (piste >= 0 && (piste <= Sarjat[kilp.Sarja()].valuku[k_pv] || piste == kilpparam.valuku+1)) {
	  tl = kilp.tulos2(piste, 2);
	  }
   if (piste == -1)
	  tl = kilp.p_aika(-1);
   set_wtag(ln, tag_cmptor);
   set_wxml_s(ln+wcslen(ln), tag_clnm, Sarjat[kilp.Sarja()].sarjanimi);
   wcscpy(str, Sarjat[kilp.Sarja()].matka[k_pv]);
	if (wcslen(str)) {
		if (str[wcslen(str)-1] >= L'P') {
			if (str[wcslen(str)-2] >= L'P') {
				wcscpy(tapa, str + wcslen(str)-2);
				str[wcslen(str)-2] = 0;
				}
			else
				tapa[0] = str[wcslen(str)-1];
			str[wcslen(str)-1] = 0;
			}
		set_wxml_s(ln+wcslen(ln), tag_cldst, str);
		}
	if (Sarjat[kilp.Sarja()].tapa[k_pv][0] >= L'P')
		wcsncpy(tapa, Sarjat[kilp.Sarja()].tapa[k_pv],2);
	str[0] = 0;
	for (int i = 0; i < 2; i++) {
		if (wcswcind(tapa[i], L"VP") >= 0) {
			if (str[0])
				wcscat(str,L",");
			wcscat(str, tapa[i] == L'P' ? L"Classic" : L"Free");
			}
		}
	if (str[0])
	   set_wxml_s(ln+wcslen(ln), tag_clsty, str);
   set_wxml_d(ln+wcslen(ln), tag_key, dkilp);
   set_wxml_d(ln+wcslen(ln), tag_stnr, kilp.id());
	set_wtag(ln+wcslen(ln), tag_nm);
   set_wxml_s(ln+wcslen(ln), tag_fnm, kilp.sukunimi);
   set_wxml_s(ln+wcslen(ln), tag_gnm, kilp.etunimi);
   set_wantitag(ln+wcslen(ln), tag_nm);
   if (kilp.seuralyh[0])
	   set_wxml_s(ln+wcslen(ln), tag_clubid, kilp.seuralyh);
   set_wxml_s(ln+wcslen(ln), tag_clubnm, kilp.seuratieto(0, false));
   if (kilp.maa[0]) {
	  set_wxml_s(ln+wcslen(ln), tag_nat, kilp.maa);
      }
   len1 = wcslen(ln);
   if (len+len1 > maxlen)
	  return(0);
   wcscpy(wbuf+len, ln);
   len += len1;
   ln[0] = 0;
	if (piste == 0 || piste == kilpparam.valuku+1) {
      if (kilp.hyv()) {
         AIKATOWSTRS(str, tl, 0);
         str[2] = L':';
         str[5] = L':';
		 str[pvparam[k_pv].laika2] = 0;
		 if (pvparam[k_pv].laika2 >= 10)
            str[8] = L'.';
         wpoistanollat(str);
         set_wxml_s(ln+wcslen(ln), tag_res, str);
         if (!tl)
			wcscpy(str, L"0");
         else {
            swprintf(str, L"%ld.%2.2ld", tl/SEK, (tl%SEK)*100/SEK);
			if (pvparam[k_pv].laika2 < 11 && wcslen(str)-11+pvparam[k_pv].laika2 > 0)
			   str[wcslen(str)-11+pvparam[k_pv].laika2] = 0;
            }
         set_wxml_s(ln+wcslen(ln), tag_ressecs, str);
		 if (Sarjat[kilp.Sarja()].lsak[k_pv])
			set_wxml_s(ln+wcslen(ln), tag_penal, kilp.asakkoStr(k_pv));
		 }
	  else {
         str[0] = 0;
		 switch (kilp.tark(k_pv)) {
			case L'K':
			   wcscpy(str, L"DNF");
			   break;
			case L'O':
			   wcscpy(str, L"LAP");
			   break;
			case L'H':
			   wcscpy(str, L"DQ");
			   break;
			case L'M':
			   wcscpy(str, L"MIS");
			   break;
			case L'X':
			   wcscpy(str, L"EXCL");
			   break;
			case L'E':
			case L'B':
			case L'V':
			   wcscpy(str, L"DNS");
               break;
			}
		 if (str[0]) {
			set_wxml_s(ln+wcslen(ln), tag_status, str);
            set_wxml_s(ln+wcslen(ln), tag_ressecs, L"0");
            }
         }
      }
   else if (piste >0 && piste <= kilpparam.valuku && kilp.hyv()) {
      set_wtag(ln+wcslen(ln), tag_splitt);
      AIKATOWSTRS(str, tl, 0);
	  str[2] = L':';
	  str[5] = L':';
	  str[pvparam[k_pv].laika2] = 0;
	  if (pvparam[k_pv].laika2 >= 10)
		 str[8] = L'.';
	  wpoistanollat(str);
	  set_wtag(ln+wcslen(ln), tag_ctl);
		set_wxml_s(ln+wcslen(ln), tag_ctdst, Sarjat[kilp.Sarja()].va_matka[k_pv][piste-1]);
	  set_wxml_d(ln+wcslen(ln), tag_cn, xmlpiste[piste-1]);
		set_wxml_d(ln+wcslen(ln), tag_cr, kilp.pv[k_pv].va[piste+1].vasija);
	  set_wxml_s(ln+wcslen(ln), tag_ct, str);
	  swprintf(str, L"%ld.%2.2ld", tl/SEK, (tl%SEK)*100/SEK);
	  if (pvparam[k_pv].laika2 < 11 && wcslen(str)-11+pvparam[k_pv].laika2 > 0) {
		 str[wcslen(str)-11+pvparam[k_pv].laika2] = 0;
		 set_wxml_s(ln+wcslen(ln), tag_ctsecs, str);
		 set_wantitag(ln+wcslen(ln), tag_ctl);
		 }
	  set_wantitag(ln+wcslen(ln), tag_splitt);
	  }
   else if (piste == -1) {
	  AIKATOWSTRS(str, tl, t0);
	  str[2] = ':';
	  str[5] = ':';
	  str[8] = 0;
	  set_wxml_s(ln+wcslen(ln), tag_st, str);
	  tl += t0*TUNTI;
	  set_wxml_d(ln+wcslen(ln), tag_stsecs, tl/SEK);
	  }
   set_wantitag(ln+wcslen(ln), tag_cmptor);
   set_wantitag(ln+wcslen(ln), tag_update);
   len1 = wcslen(ln);
   if (len+len1 > maxlen)
	  return(0);
   wcscpy(wbuf+len, ln);
   len += len1;
   wcstoansi((char *)buf, wbuf, len+1);
   return(len);
   }

int xmlemitvasanoma(char *buf, int maxlen, emitvatp emva, int msgno)
   {
   int len = 0, len1, d, k;
   wchar_t ln[402], wcs[82], wbuf[400];
	kilptietue kilp;

	buf[0] = 0;
	if ((d = getpos(emva.kilpno)) <= 0)
		return(0);
	kilp.GETREC(d);
   wcscpy(ln, L"<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>\n");
   len1 = wcslen(ln);
   if (len+len1 > maxlen)
	  return(0);
   wcscpy(wbuf+len, ln);
   len += len1;
   set_wtag(ln, tag_esplits);
   len1 = wcslen(ln);
   if (len+len1 > maxlen)
	  return(0);
   wcscpy(wbuf+len, ln);
   len += len1;
   wcsncpy(wcs, kilpailu, 80);
   wcs[80] = 0;
   elimwbl(wcs);
   set_wxml_s(ln, tag_evnm, wcs);
   set_wxml_d(ln+wcslen(ln), tag_msgno, msgno);
   len1 = wcslen(ln);
   if (len+len1 > maxlen)
	  return(0);
   wcscpy(wbuf+len, ln);
   len += len1;
   ln[0] = 0;
   set_wtag(ln, tag_cmptor);
   set_wxml_d(ln+wcslen(ln), tag_stnr, kilp.id());
   set_wtag(ln+wcslen(ln), tag_nm);
   set_wxml_s(ln+wcslen(ln), tag_fnm, kilp.sukunimi);
   set_wxml_s(ln+wcslen(ln), tag_gnm, kilp.etunimi);
   set_wantitag(ln+wcslen(ln), tag_nm);
   AIKATOWSTRS(wcs, emva.tulos, 0);
   wcs[2] = ':';
   wcs[5] = ':';
   wcs[pvparam[k_pv].laika2] = 0;
   if (pvparam[k_pv].laika2 >= 10)
	  wcs[8] = '.';
   wpoistanollat(wcs);
   set_wxml_s(ln+wcslen(ln), tag_res, wcs);
	if (!emva.tulos)
	  wcscpy(wcs, L"0");
   else {
	  swprintf(wcs, L"%ld.%2.2ld", emva.tulos/SEK, (emva.tulos%SEK)*100/SEK);
	  if (pvparam[k_pv].laika2 < 11 && wcslen(wcs)-11+pvparam[k_pv].laika2 > 0)
		 wcs[wcslen(wcs)-11+pvparam[k_pv].laika2] = 0;
	  }
   set_wxml_s(ln+wcslen(ln), tag_ressecs, wcs);
   set_wtag(ln+wcslen(ln), tag_splitt);
   len1 = wcslen(ln);
   if (len+len1 > maxlen)
	  return(0);
   wcscpy(wbuf+len, ln);
   len += len1;
   ln[0] = 0;
   for (k = 0; k < MAXNRASTI; k++) {
	   if (!emva.rastit[k][0])
		   continue;
		AIKATOWSTRS(wcs, SEK*(emva.rastit[k][1]), 0);
		wcs[2] = ':';
		wcs[5] = ':';
		wcs[8] = 0;
		wpoistanollat(wcs);
		set_wtag(ln, tag_ctl);
		set_wxml_d(ln+wcslen(ln), tag_cn, k+1);
		set_wxml_d(ln+wcslen(ln), tag_cc, emva.rastit[k][0]);
		set_wxml_s(ln+wcslen(ln), tag_ct, wcs);
		set_wxml_d(ln+wcslen(ln), tag_ctsecs, emva.rastit[k][1]);
		set_wantitag(ln+wcslen(ln), tag_ctl);
		len1 = wcslen(ln);
		if (len+len1 > maxlen)
			return(0);
		wcscpy(wbuf+len, ln);
		len += len1;
		ln[0] = 0;
		}
   set_wantitag(ln, tag_splitt);
   set_wantitag(ln+wcslen(ln), tag_cmptor);
   set_wantitag(ln+wcslen(ln), tag_esplits);
   len1 = wcslen(ln);
   if (len+len1 > maxlen)
	  return(0);
   wcscpy(wbuf+len, ln);
   len += len1;
   wcstoansi((char *) buf, wbuf, len+1);
   return(len);
   }

#endif
   wchar_t
	  tag_fisresults[] = L"Fisresults",
      tag_fisevent[] = L"CC_race",
      tag_fisseason[] = L"Season",
      tag_fisevcode[] = L"Codex",
      tag_fisdiscipline[] = L"Discipline",
	  tag_fiscategory[] = L"Category",
      tag_fisraceinfo[] = L"CC_raceinfo",
      tag_fisracedate[] = L"Racedate",
      tag_fisday[] = L"Day",
	  tag_fismonth[] = L"Month",
      tag_fisyear[] = L"Year",
      tag_fisplace[] = L"Place",
	  tag_fisdelegate[] = L"TD Function=\"Delegate\"",
	  tag_fistd[] = L"TD",
      tag_fistdnumber[] = L"Tdnumber",
      tag_fistdfnm[] = L"Tdlastname",
      tag_fistdgnm[] = L"Tdfirstname",
	  tag_fistdnat[] = L"Tdnation",
	  tag_fislist[] = L"Usedfislist",
      tag_fispenalty[] = L"Appliedpenalty",
	  tag_fiscalcpenalty[] = L"Calculatedpenalty",
      tag_fisfvalue[] = L"Fvalue",
      tag_fisclassified[] = L"CC_classified",
      tag_fisnotclassified[] = L"CC_notclassified",
      tag_fisranked[] = L"CC_ranked",
      tag_fisnotranked[] = L"CC_notranked",
      tag_fisevnm[] = L"Eventname",
      tag_fiscl[] = L"EventClass",
      tag_fisclnm[] = L"ClassName",
	  tag_fisclsty[] = L"ClassStyle",
      tag_fiscldst[] = L"ClassDist",
      tag_fiscmptor[] = L"Competitor",
      tag_fisstnr[] = L"Bib",
      tag_fisorder[] = L"Order",
      tag_fisid[] = L"Fiscode",
      tag_fisnm[] = L"Name",
      tag_fisfnm[] = L"Lastname",
      tag_fisgnm[] = L"Firstname",
      tag_fissex[] = L"Sex",
      tag_fisyearbirth[] = L"Yearofbirth",
	  tag_fisclubid[] = L"ClubID",
	  tag_fisclubnm[] = L"ClubName",
      tag_fisnat[] = L"Nation",
      tag_fisresult[] = L"CC_result",
	  tag_fistime[] = L"Totaltime",
      tag_fislevel[] = L"Level",
	  tag_fispoints[] = L"Racepoints",
      tag_fisrank[] = L"Rank",
#ifdef MAKI
      tag_fissplitts[] = L"SplitTimes",
      tag_fissplitt[] = L"SplitTime",
	  tag_fissplittpos[] = L"SplitTimePos",
#else
      tag_fissplitt[] = L"SplitTimes",
	  tag_fislastdate[] = L"LastDate",
	  tag_fispartcount[] = L"PartCount",
#endif
#if defined(TCPSIIRTO)
      tag_fisupdate[] = L"Update",
      tag_fiskey[] = L"Key",
	  tag_fismsgno[] = L"Msgno",
      tag_fisesplits[] = L"ESplits",
#endif
#ifdef MAKI
      tag_fiscmptors[] = L"Competitors",
	  tag_fispnts[] = L"Points",
	  tag_fisrounds[] = L"Rounds",
	  tag_fisround[] = L"Round",
	  tag_fisrndno[] = L"RoundNo",
	  tag_fisrndrank[] = L"RoundRank",
	  tag_fisrndpnts[] = L"RoundPoints",
	  tag_fisspeed[] = L"Speed",
	  tag_fisdist[] = L"Dist",
	  tag_fisdistpts[] = L"DistPoints",
	  tag_fisjdga[] = L"JudgeA",
	  tag_fisjdgb[] = L"JudgeB",
	  tag_fisjdgc[] = L"JudgeC",
	  tag_fisjdgd[] = L"JudgeD",
	  tag_fisjdge[] = L"JudgeE",
	  tag_fisjdg[] = L"JudgePoints",
	  tag_fisjudges[] = L"Judges",
	  tag_fisjudge[] = L"Judge",
	  tag_fisjdgpt[] = L"JdgPt",
	  tag_fispttype[] = L"PtType",
	  tag_fispt[] = L"Pts",
#endif
	  tag_fispenal[] = L"Penalties";

static int xmlfisranked, fvalue = 800, minpenalty = 1500, fispenalty = 1500, fislist;
static wchar_t fis_dec_sep = L'.', fisdiscipline[10] = L"DI", fissex[] = L"M";
static wchar_t fislevel[21] = L"Final", *fisvarlevels[4] = {L"A-final", L"B-final",L"Semifinal",L"First"};
static int fisvariablelevel;

#if defined(__BORLANDC__)
#pragma warn -par
#endif
int xmlfisots(tulostusparamtp *tulprm)
   {
   wchar_t ln[202];

   if (tulprm->merkit == L'A')
		swprintf(ln, L"<?xml version=\"1.0\" encoding=\"ISO-8859-1\"?>\n<%s>",
		   tag_fisresults);
   else
		swprintf(ln, L"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<%s>",
			tag_fisresults);
   tulprm->writeline(ln);
   return(0);
   }
#if defined(__BORLANDC__)
#pragma warn +par
#endif

int xmlfisloppu(tulostusparamtp *tulprm)
   {
   tulprm->lstf->u.Fl->put_wantitag(tag_fisresults, 0);
   return(0);
   }

void laskefispisteet(int srj, tulostusparamtp *tulprm)
   {
   INT d, p, sj, sj1, k;
   INT32 tls, tls1, parhaat[3] = {9999999,9999999,9999999};
   kilptietue kilp;
   double fpiste, karki;

   if (tulprm->yhttuljarj && k_pv == 0)
	  return;
   if (srj >= sarjaluku+nsarjayhd)
	   return;
	  sj = 1;
	  tls = 0;
	  p = jalku[srj][0];
	  while (p > 0) {
		 d = p;
		 kilp.GETREC(d);
		 if (!kilp.hyv())
			 break;
		 if (kilp.lisno[1] != 0) {
			 if (tulprm->yhttuljarj)
				tls1 = kilp.ytulos(k_pv, 2);
			 else
				tls1 = kilp.p_aika(0, 2);
			 if (sj == 1)
				 karki = tls1;
			 if (tls1 != tls) {
				sj1 = sj;
				tls = tls1;
				}
			 if (sj1 > 5)
				 break;
			 for (k = 0; k < 3; k++) {
				if (parhaat[k] > kilp.pisteet[2])
					 break;
				}
			 if (k < 2)
				memmove(parhaat+k+1, parhaat+k, (2-k) * sizeof(parhaat[0]));
			 if (k < 3)
				 parhaat[k] = kilp.pisteet[2];
			 sj++;
			 }
		 p = SeuraavaSrj(srj, 0, p);
		 }
	  parhaat[0] += parhaat[1] + parhaat[2];
	  fispenalty = (8*parhaat[0])/30;
	  fispenalty = (fispenalty+5)/10;
	  if (!wcscmp(fisdiscipline, L"SP") || (!wcscmp(fisdiscipline, L"PUR") && k_pv == 1))
		  parhaat[1] = 26400;
	  else if (!wcscmp(fisdiscipline, L"MS") || !wcscmp(fisdiscipline, L"PUR"))
		  parhaat[1] = 30800;
	  else
		  parhaat[1] = 17600;
	  if (fispenalty > parhaat[1])
		  fispenalty = parhaat[1];
	  sj = 1;
	  tls = 0;
	  p = jalku[srj][0];
	  while (fvalue && p > 0) {
		 d = p;
		 kilp.GETREC(d);
		 if (!kilp.hyv())
			 break;
		 if (kilp.lisno[1] != 0) {
			 if (tulprm->yhttuljarj)
				tls = kilp.ytulos(k_pv, 2);
			 else
				tls = kilp.p_aika(0, 2);
			 fpiste = (100.0*fvalue*(tls-karki))/karki;
			 kilp.pisteet[0] = (INT32) floor(fpiste+0.5);
			 kilp.PUTREC(d, 1);
			 }
		 p = SeuraavaSrj(srj, 0, p);
		 }
   }

int xmlfissrjots(int sarja, tulostusparamtp *tulprm)
   {
   int i, k, insrj = 0;
   wchar_t tag[40], buf[40], *p, cs = 0;
   TextFl *tul_tied, *kisa_tied = NULL;
   static wchar_t kisaflnm[200] = L"fiskisa.txt";

   xmlfisranked = 1;
   tul_tied = tulprm->lstf->u.Fl;

	if (wcswcind(towupper(Sarjat[sarja].sarjanimi[0]), L"NDLWT") >= 0)
		fissex[0] = L'L';
	else if (wcswcind(towupper(Sarjat[sarja].sarjanimi[0]), L"MGHP") >= 0)
		fissex[0] = L'M';
	else {
		writeerror_w(L"Sukupuolta ei voi päätellä sarjan nimestä. Vaihda sarjan nimi", 0);
		return(1);
		}
   swprintf(tag, L"Raceheader Sector=\"CC\" Sex=\"%c\"", fissex[0]);
   tul_tied->put_wtag(tag, 0);
   clrln(ySize-3);
#ifdef _CONSOLE
   vidspwmsg(ySize-3, 0, 7, 0, L"Kilpailun määritystiedosto:");
   while (1) {
	   inputwstr(kisaflnm, 45, 28, ySize-3, L"\r\x1b", &cs, FALSE);
#else
   while (1) {
		wcscpy(kisaflnm, FISMaarFileName);
#endif
	   if (cs == ESC)
		   return(1);
	   kisa_tied = new TextFl(kisaflnm, L"rt");
	   if (kisa_tied->IsOpen()) {
		   for (i = 0; i < 15; i++) {
			   kisa_tied->ReadLine(buf, 38);
			  if (wcslen(buf) < 3)
				  break;
			  while (wcslen(buf) > 0 && buf[wcslen(buf)-1] == L'\n')
				  buf[wcslen(buf)-1] = 0;
			  p = wcsstr(buf, L"=");
			  if (!p) break;
			  p++;
			  switch (i) {
				  case 0:
					  fvalue = _wtoi(p);
					  break;
				  case 1:
					  minpenalty = 100*_wtoi(p);
					  break;
				  case 2:
					  fislist = _wtoi(p);
					  break;
				  case 3:
					  tul_tied->put_wxml_s(tag_fisseason, p, 0);
					  break;
				  case 4:
					  tul_tied->put_wxml_s(tag_fisevcode, p, 0);
					  break;
				  case 5:
					  tul_tied->put_wxml_s(tag_fisnat, p, 0);
					  break;
				  case 6:
					  tul_tied->put_wxml_s(tag_fisdiscipline, p, 0);
					  wcsncpy(fisdiscipline, p, 9);
					  break;
				  case 7:
					  tul_tied->put_wxml_s(tag_fiscategory, p, 0);
					  break;
				  case 8:
					  tul_tied->put_wtag(tag_fisracedate, 0);
					  p[2] = 0;
					  tul_tied->put_wxml_s(tag_fisday, p, 0);
					  p[5] = 0;
					  tul_tied->put_wxml_s(tag_fismonth, p+3, 0);
					  tul_tied->put_wxml_s(tag_fisyear, p+6, 0);
					  tul_tied->put_wantitag(tag_fisracedate, 0);
					  break;
				  case 9:
					  tul_tied->put_wxml_s(tag_fisevnm, p, 0);
					  break;
				  case 10:
					  tul_tied->put_wxml_s(tag_fisplace, p, 0);
					  break;
				  case 11:
					  tul_tied->put_wtag(tag_fisdelegate, 0);
					  tul_tied->put_wxml_s(tag_fistdnumber, p, 0);
					  break;
				  case 12:
					  tul_tied->put_wxml_s(tag_fistdfnm, p, 0);
					  break;
				  case 13:
					  tul_tied->put_wxml_s(tag_fistdgnm, p, 0);
					  break;
				  case 14:
					  tul_tied->put_wxml_s(tag_fistdnat, p, 0);
					  break;
				  case 15:
					  wcsncpy(fislevel, p, 20);
					  fislevel[20] = 0;
					  if (towupper(fislevel[0]) == L'V')
						  fisvariablelevel = 1;
					  break;
				  }
			   }
		   if (i > 11)
			   tul_tied->put_wantitag(tag_fistd, 0);
		   break;
		   }
#ifdef _CONSOLE
	   writeerror_w(L"Tiedostoa ei löydy, peruuta painamalla Enter ja ESC", 0);
#else
	   writeerror_w(L"Kilpailun määritystiedostoa ei ole valittu", 0, true);
	   return(1);
#endif
	   }
   if (kisa_tied)
	   delete kisa_tied;
   tul_tied->put_wantitag(L"Raceheader", 0);
   laskefispisteet(sarja, tulprm);
   tul_tied->put_wtag(tag_fisevent, 0);
   tul_tied->put_wtag(tag_fisraceinfo, 0);
   tul_tied->put_wxml_d(tag_fislist, fislist, 0);
   k = fispenalty;
   if (k < minpenalty)
	   k = minpenalty;
   swprintf(buf, L"%d.%02d", k/100, k%100);
   tul_tied->put_wxml_s(tag_fispenalty, buf, 0);
   swprintf(buf, L"%d.%02d", fispenalty/100, fispenalty%100, 0);
   tul_tied->put_wxml_s(tag_fiscalcpenalty, buf, 0);
   tul_tied->put_wxml_d(tag_fisfvalue, fvalue, 0);
   tul_tied->put_wantitag(tag_fisraceinfo, 0);
   tul_tied->put_wtag(tag_fisclassified, 0);
   return(0);
   }

int xmlfissrjloppu(tulostusparamtp *tulprm)
   {
   TextFl *tul_tied;

   tul_tied = tulprm->lstf->u.Fl;
   if (xmlfisranked)
		tul_tied->put_wantitag(tag_fisclassified, 0);
   else
		tul_tied->put_wantitag(tag_fisnotclassified, 0);
   tul_tied->put_wantitag(tag_fisevent, 0);
   return(0);
   }

void xmlfistulos(kilptietue *kilp, INT sj, tulostusparamtp *tulprm)
   {
   wchar_t stat[4], ln[100];
   INT i,piste = 0;
   INT32 tl;
   TextFl *tul_tied;
//   time_t tm_t;
//   struct tm *dttm;
   static INT32 karki;
   static int edtls, fissj, totfissj;

	if (kilp->lisno[1] == 0)
		return;
	if (kilp->hyv()) {
		if (tulprm->yhttuljarj)
			tl = kilp->ytulos(k_pv, 2);
		else
			tl = kilp->p_aika(0, 2);
		if (sj == 1 && (fissj > 1 || tl != edtls)) {
			edtls = 0;
			fissj = 0;
			totfissj = 0;
			}
		totfissj++;
		if (fissj == 0 || tl > edtls)
			fissj = totfissj;
		edtls = tl;
		}
	else {
		edtls = 0;
		fissj = 0;
		totfissj = 0;
		}
   tul_tied = tulprm->lstf->u.Fl;
   if (xmlfisranked && !kilp->hyv()) {
		xmlfisranked = 0;
		tul_tied->put_wantitag(tag_fisclassified, 0);
		tul_tied->put_wtag(tag_fisnotclassified, 0);
		}
   if (xmlfisranked) {
		tul_tied->put_wtag(L"CC_ranked Status=\"QLF\"", 0);
		tul_tied->put_wxml_d(tag_fisrank, fissj, 0);
		}
   else {
	  stat[0] = 0;
	  switch (kilp->tark(k_pv)) {
		 case L'H':
			wcscpy(stat, L"DSQ");
			break;
		 case L'E':
		 case L'B':
			wcscpy(stat, L"DNS");
			break;
		 default:
			wcscpy(stat, L"DNF");
			break;
		 }
	  swprintf(ln, L"CC_notranked Status=\"%s\"", stat);
	  tul_tied->put_wtag(ln, 0);
	  }
   tul_tied->put_wxml_d(tag_fisstnr, kilp->id(), 0);
   tul_tied->put_wtag(tag_fiscmptor, 0);
   tul_tied->put_wxml_d(tag_fisid, kilp->lisno[1], 0);
	tul_tied->put_wxml_s(tag_fisfnm, kilp->sukunimi, 0);
	tul_tied->put_wxml_s(tag_fisgnm, kilp->etunimi, 0);
	tul_tied->put_wxml_s(tag_fissex, fissex, 0);
   if (kilp->maa[0]) {
	  tul_tied->put_wxml_s(tag_fisnat, kilp->maa, 0);
	  }
   else
	  tul_tied->put_wxml_s(tag_fisnat, kilp->seuratieto(0, false), 0);
	tul_tied->put_wxml_d(tag_fisyearbirth, kilp->synt, 0);
	tul_tied->put_wantitag(tag_fiscmptor, 0);
   if (kilp->hyv()) {
		tul_tied->put_wtag(tag_fisresult, 0);
		AIKATOWSTRS(ln, tl, 0);
		ln[2] = L':';
		ln[5] = L':';
		ln[pvparam[k_pv].laika2] = 0;
		if (pvparam[k_pv].laika2 >= 10)
			ln[8] = fis_dec_sep;
		wpoistanollat(ln);
		if (fvalue == 0) {
			ln[0] = 0;
			kilp->pisteet[0] = 0;
			}
		tul_tied->put_wxml_s(tag_fistime, ln, 0);
		swprintf(ln, L"%d.%02d", kilp->pisteet[0]/100, kilp->pisteet[0]%100);
		tul_tied->put_wxml_s(tag_fispoints, ln, 0);
		if (fisvariablelevel) {
			i = (fissj - 1)%6;
			if (i > 1 && i < 6)
				i = 2;
			if (i >= 6)
				i = 3;
			tul_tied->put_wxml_s(tag_fislevel, fisvarlevels[i], 0);
			}
		else
			tul_tied->put_wxml_s(tag_fislevel, fislevel, 0);
		tul_tied->put_wantitag(tag_fisresult, 0);
	  }
   if (xmlfisranked)
	   tul_tied->put_wantitag(tag_fisranked, 0);
   else
	   tul_tied->put_wantitag(tag_fisnotranked, 0);
   }


static wchar_t *ControlType[6] = {L"Control", L"Start", L"Finish", L"CrossingPoint", L"EndOfMarkedRoute", L""};

#define TAGEvent				0
#define TAGTitle				2
#define TAGHeader				3
#define TAGRace					4
#define TAGRaceNo			5
#define TAGCreated				6
#define TAGName 				13
#define TAGFamily 				14
#define TAGGiven 				15
#define TAGCountry 				20
#define TAGDate 				32
#define TAGClass 				43
#define TAGCount				44
#define TAGId					74
#define TAGOrganisation			80
#define TAGStartTime  		    81
#define TAGResult				82
#define TAGStatus				83
#define TAGPoints				85
#define TAGModifyDate  		   102
#define TAGClock       		   103
#define TAGMap         		   104
#define TAGScale       		   105
#define TAGAutoResult		   106
#define TAGStartPoint  		   107
#define TAGStartPointCode	   108
#define TAGControlPosition     109
#define TAGMapPosition 		   110
#define TAGControl     		   111
#define TAGControlCode 		   112
#define TAGFinishPoint 		   113
#define TAGFinishPointCode	   114
#define TAGCourse      		   115
#define TAGCourseName  		   116
#define TAGCourseId    		   117
#define TAGClassShortName	   118
#define TAGCourseLength		   121
#define TAGCourseClimb 		   122
#define TAGCourseControl	   123
#define TAGSequence    		   124
#define TAGLegLength   		   125
#define TAGDistanceToFinish	   126
#define TAGMultiRace      	   128
#define TAGUnitCode            130
#define TAGClearECard		   131
#define TAGPublic			   132
#define TAGUseClassId  		   133
#define TAGTotal		       138
#define TAGRaceCourseData 	   139
#define TAGPosition            140
#define TAGLength              141
#define TAGClimb               142
#define TAGTechnique           143
#define TAGClassName		   144
#define TAGResultList          145
#define TAGClassResult         146
#define TAGPersonResult        147
#define TAGPerson              148
#define TAGScore		       149
#define TAGBibNumber           150
#define TAGFinishTime          151
#define TAGTime                152
#define TAGTimeBehind          153
#define TAGTimeStr             154
#define TAGTimeBehindStr       155
#define TAGSplitTime           156
#define TAGSplitTimes          157
#define TAGPersonEntry         160
#define TAGControlCard 		   161
#define TAGRaceNumber          162
#define TAGShortName 		   163
#define TAGNationality 		   164
#define TAGEntryList		   165

static tagListTp TulosTags[] = {
	{TAGEvent, L"Event"},
	{TAGTitle, L"Title"},
	{TAGHeader, L"Header"},
	{TAGRace, L"Race"},
	{TAGRaceNo, L"RaceNo"},
	{TAGCreated, L"Created"},
	{TAGName, L"Name"},
	{TAGFamily, L"Family"},
	{TAGGiven, L"Given"},
	{TAGCountry, L"Country"},
	{TAGDate, L"Date"},
	{TAGClass, L"Class"},
	{TAGCount, L"Count"},
	{TAGId, L"Id"},
	{TAGOrganisation, L"Organisation"},
	{TAGStartTime, L"StartTime"},
	{TAGResult, L"Result"},
	{TAGStatus, L"Status"},
	{TAGPoints, L"Points"},
	{TAGModifyDate, L"ModifyDate"},
	{TAGClock, L"Clock"},
	{TAGMap, L"Map"},
	{TAGAutoResult, L"AutoResult"},
	{TAGStartPoint, L"StartPoint"},
	{TAGStartPointCode, L"StartPointCode"},
	{TAGMapPosition, L"MapPosition"},
	{TAGControl, L"Control"},
	{TAGControlCode, L"ControlCode"},
	{TAGFinishPoint, L"FinishPoint"},
	{TAGFinishPointCode, L"FinishPointCode"},
	{TAGCourse, L"Course"},
	{TAGCourseName, L"CourseName"},
	{TAGCourseId, L"CourseId"},
	{TAGClassShortName, L"ClassShortName"},
	{TAGCourseLength, L"CourseLength"},
	{TAGCourseClimb, L"CourseClimb"},
	{TAGCourseControl, L"CourseControl"},
	{TAGSequence, L"Sequence"},
	{TAGLegLength, L"LegLength"},
	{TAGDistanceToFinish, L"DistanceToFinish"},
	{TAGMultiRace, L"MultiRace"},
	{TAGUnitCode, L"UnitCode"},
	{TAGClearECard, L"ClearECard"},
	{TAGPublic, L"Public"},
	{TAGUseClassId, L"UseClassId"},
	{TAGTotal, L"Total"},
	{TAGRaceCourseData, L"RaceCourseData"},
	{TAGPosition, L"Position"},
	{TAGLength, L"Length"},
	{TAGClimb, L"Climb"},
	{TAGTechnique, L"Technique"},
	{TAGClassName, L"ClassName"},
	{TAGResultList, L"ResultList"},
	{TAGClassResult, L"ClassResult"},
	{TAGPersonResult, L"PersonResult"},
	{TAGPerson, L"Person"},
	{TAGScore, L"Score"},
	{TAGBibNumber, L"BibNumber"},
	{TAGFinishTime, L"FinishTime"},
	{TAGTime, L"Time"},
	{TAGTimeBehind, L"TimeBehind"},
	{TAGTimeStr, L"TimeStr"},
	{TAGTimeBehindStr, L"TimeBehindStr"},
	{TAGSplitTime, L"SplitTime"},
	{TAGPersonEntry, L"PersonEntry"},
	{TAGSplitTimes, L"SplitTimes"},
	{TAGControlCard, L"ControlCard"},
	{TAGRaceNumber, L"RaceNumber"},
	{TAGShortName, L"ShortName"},
	{TAGNationality, L"Nationality"},
	{TAGEntryList, L"EntryList"}
	};

static int nTulosTags = sizeof(TulosTags)/sizeof(TulosTags[0]);

int xmlTulosots(tulostusparamtp *tulprm)
   {
   wchar_t ln[602], level = 0;
   TextFl *outfl;

   outfl = tulprm->lstf->u.Fl;

	if (wcslen(kilpparam.kilpkoodi) == 0) {
		wcscpy(kilpparam.kilpkoodi, L"KOODI");
		}
	swprintf(ln, L"<?xml version=\"1.0\" encoding=%s standalone=\"yes\"?>\n",
		tulprm->merkit == L'A' ? L"\"ISO-8859-1\"" : L"\"UTF-8\"");
	outfl->WriteLine(ln);
	if (tulprm->muottied[0]) {
		swprintf(ln, L"<?xml-stylesheet type=\"text/xsl\" href=\"%s\"?>\n", tulprm->muottied);
		outfl->WriteLine(ln);
		}
	swprintf(ln, L"%s xmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
		L"\tcreateTime=\"%s\"\n"
		L"\tcreator=\"HkKisaWin %s\"\n"
		L"\tstatus=\"%s\"",
		XMLhae_tagName(TAGResultList, TulosTags, nTulosTags), ISOdatetime(0,1), Build(), L"Complete");
	outfl->put_wtag(ln, level++);

	outfl->put_wtag(XMLhae_tagName(TAGEvent, TulosTags, nTulosTags), level++);
	outfl->put_wxml_s(XMLhae_tagName(TAGTitle, TulosTags, nTulosTags), kilpailu, level);
	outfl->put_wxml_s(XMLhae_tagName(TAGHeader, TulosTags, nTulosTags), paaots, level, L"level='1'");
	outfl->put_wxml_s(XMLhae_tagName(TAGHeader, TulosTags, nTulosTags), alaots, level, L"level='2'");
	outfl->put_wxml_s(XMLhae_tagName(TAGRace, TulosTags, nTulosTags), pvparam[k_pv].Title, level);
	outfl->put_wtag(XMLhae_tagName(TAGStartTime, TulosTags, nTulosTags), level++);
	outfl->put_wxml_s(XMLhae_tagName(TAGDate, TulosTags, nTulosTags), stDateNo(NULL, pvparam[k_pv].Date), level);
	outfl->put_wantitag(XMLhae_tagName(TAGStartTime, TulosTags, nTulosTags), --level);
/*
	outfl->put_wtag(srjtag[TAGEndTime], level++);
	outfl->put_wxml_s(XMLhae_tagName(TAGDate, TulosTags, nTulosTags), stDateNo(NULL, pvparam[k_pv].Date), level);
	outfl->put_wantitag(srjtag[TAGEndTime], --level);
*/
	outfl->put_wantitag(XMLhae_tagName(TAGEvent, TulosTags, nTulosTags), --level);

   return(0);
   }

int xmlTulosloppu(tulostusparamtp *tulprm)
   {
   TextFl *tul_tied;

   tul_tied = tulprm->lstf->u.Fl;
   tul_tied->put_wantitag(XMLhae_tagName(TAGResultList, TulosTags, nTulosTags), 0);
   return(0);
   }

int xmlTulossrjots(int sarja, tulostusparamtp *tulprm)
   {
   int level = 1;
   TextFl *tul_tied;
   wchar_t ln[100];
   ratatp *rt;

   tul_tied = tulprm->lstf->u.Fl;
   tul_tied->put_wtag(XMLhae_tagName(TAGClassResult, TulosTags, nTulosTags), level++);

   tul_tied->put_wtag(XMLhae_tagName(TAGClass, TulosTags, nTulosTags), level++);
   tul_tied->put_wxml_s(XMLhae_tagName(TAGId, TulosTags, nTulosTags), Sarjat[sarja].sarjanimi, level);
   if (Sarjat[sarja].psarjanimi[0])
		tul_tied->put_wxml_s(XMLhae_tagName(TAGName, TulosTags, nTulosTags), Sarjat[sarja].psarjanimi, level);
   else
		tul_tied->put_wxml_s(XMLhae_tagName(TAGName, TulosTags, nTulosTags), Sarjat[sarja].sarjanimi, level);
   tul_tied->put_wantitag(XMLhae_tagName(TAGClass, TulosTags, nTulosTags), --level);
   for (int i_pv = 0; i_pv <= k_pv; i_pv++) {
		swprintf(ln, L"race='%d'", i_pv+1);
		tul_tied->put_wxml_s(XMLhae_tagName(TAGLength, TulosTags, nTulosTags), Sarjat[sarja].matka[i_pv], level, ln);
		if (Sarjat[sarja].tapa[i_pv][0])
			tul_tied->put_wxml_s(XMLhae_tagName(TAGTechnique, TulosTags, nTulosTags), Sarjat[sarja].tapa[i_pv], level, ln);
		}
   tul_tied->put_wxml_d(XMLhae_tagName(TAGCount, TulosTags, nTulosTags), nilm[sarja], level, L"type='Race'");
   tul_tied->put_wxml_d(XMLhae_tagName(TAGCount, TulosTags, nTulosTags), nyilm[sarja], level, L"type='Total'");
   tul_tied->put_wxml_d(XMLhae_tagName(TAGCount, TulosTags, nTulosTags), ntulos[sarja][0], level, L"type='Result'");
   tul_tied->put_wxml_d(XMLhae_tagName(TAGCount, TulosTags, nTulosTags), ntulos[sarja][kilpparam.valuku+1], level, L"type='CombResult'");
   tul_tied->put_wxml_d(XMLhae_tagName(TAGCount, TulosTags, nTulosTags), nkesk[sarja], level, L"type='Disq'");
   tul_tied->put_wxml_d(XMLhae_tagName(TAGCount, TulosTags, nTulosTags), nhyl[sarja], level, L"type='DNF'");
   tul_tied->put_wxml_d(XMLhae_tagName(TAGCount, TulosTags, nTulosTags), neil[sarja], level, L"type='DNS'");
   tul_tied->put_wxml_d(XMLhae_tagName(TAGCount, TulosTags, nTulosTags), n_avoin(sarja), level, L"type='Open'");
   if ((rt = haerata_sarja(sarja, k_pv)) != NULL) {
	   tul_tied->put_wtag(XMLhae_tagName(TAGCourse, TulosTags, nTulosTags), level++);
	   tul_tied->put_wxml_d(XMLhae_tagName(TAGLength, TulosTags, nTulosTags), rt->ratapit, level);
	   if (rt->nousu)
		   tul_tied->put_wxml_d(XMLhae_tagName(TAGLength, TulosTags, nTulosTags), rt->nousu, level);
	   tul_tied->put_wantitag(XMLhae_tagName(TAGCourse, TulosTags, nTulosTags), --level);
	   }
	if (tulprm->xmlva == L'M') {
	   tul_tied->put_wtag(XMLhae_tagName(TAGSplitTimes, TulosTags, nTulosTags), level++);
		for (int iva = 1; iva <= Sarjat[sarja].valuku[k_pv]; iva++) {
			swprintf(ln, L"order='%d'", iva);
		   tul_tied->put_wtagparams(XMLhae_tagName(TAGSplitTime, TulosTags, nTulosTags), ln, false, level++);
		   tul_tied->put_wxml_s(XMLhae_tagName(TAGName, TulosTags, nTulosTags), Sarjat[sarja].va_matka[k_pv][iva-1], level);
		   tul_tied->put_wantitag(XMLhae_tagName(TAGSplitTime, TulosTags, nTulosTags), --level);
			}
	   tul_tied->put_wantitag(XMLhae_tagName(TAGSplitTimes, TulosTags, nTulosTags), --level);
		}
   return(0);
   }

int xmlTulossrjloppu(tulostusparamtp *tulprm)
   {
   TextFl *tul_tied;

   tul_tied = tulprm->lstf->u.Fl;
   tul_tied->put_wantitag(XMLhae_tagName(TAGClassResult, TulosTags, nTulosTags), 1);
   return(0);
   }

void xmlTulostulos(kilptietue& kilp, int sarja, INT sj, tulostusparamtp *tulprm)
   {
   wchar_t ln[100];
   INT piste = 0, tls, sj1 = 0, level = 2;
   emittp em;
   ratatp *rt;
   TextFl *tul_tied;
   static INT32 karki = 0;

   tul_tied = tulprm->lstf->u.Fl;

   tul_tied->put_wtag(XMLhae_tagName(TAGPersonResult, TulosTags, nTulosTags), level++);

   tul_tied->put_wtag(XMLhae_tagName(TAGPerson, TulosTags, nTulosTags), level++);
   if (kilp.lisno[1]) {
	   tul_tied->put_wxml_d(XMLhae_tagName(TAGId, TulosTags, nTulosTags), kilp.lisno[1], level, L"type=\"IOF\"");
	   }
   tul_tied->put_wxml_d(XMLhae_tagName(TAGId, TulosTags, nTulosTags), kilp.lisno[0], level, L"type=\"local\"");
//   if (kilp.lisno[1] && kilp.maa[0]) {
//		swprintf(ln, L"%s%d", kilp.maa, kilp.lisno[1]);
//	   tul_tied->put_wxml_s(srjtag[TAGId], ln, level, L"type=\"WRE\"");
//		}
   tul_tied->put_wtag(XMLhae_tagName(TAGName, TulosTags, nTulosTags), level++);
   tul_tied->put_wxml_s(XMLhae_tagName(TAGFamily, TulosTags, nTulosTags), kilp.sukunimi, level);
   tul_tied->put_wxml_s(XMLhae_tagName(TAGGiven, TulosTags, nTulosTags), kilp.etunimi, level);
   tul_tied->put_wantitag(XMLhae_tagName(TAGName, TulosTags, nTulosTags), --level);
   tul_tied->put_wantitag(XMLhae_tagName(TAGPerson, TulosTags, nTulosTags), --level);

   tul_tied->put_wtag(XMLhae_tagName(TAGOrganisation, TulosTags, nTulosTags), level++);
   tul_tied->put_wxml_s(XMLhae_tagName(TAGId, TulosTags, nTulosTags), kilp.seuralyh, level);
   tul_tied->put_wxml_s(XMLhae_tagName(TAGName, TulosTags, nTulosTags), kilp.seura, level);
   if (kilp.maa[0]) {
	   swprintf(ln, L"code=\"%s\"", kilp.maa);
	   tul_tied->put_wxml_s(XMLhae_tagName(TAGCountry, TulosTags, nTulosTags), MaaNimi(kilp.maa), level, ln);
	   }
   tul_tied->put_wantitag(XMLhae_tagName(TAGOrganisation, TulosTags, nTulosTags), --level);

   for (int i_pv = Sarjat[sarja].enspv; i_pv <= k_pv + 1; i_pv++) {
		if (k_pv == Sarjat[sarja].enspv && i_pv > 0)
			break;
		if (i_pv <= k_pv && Sarjat[sarja].luonne[i_pv] < 0)
			continue;
		if (i_pv <= k_pv)
			swprintf(ln, L"race=\"%d\"", i_pv+1);
		else
			wcscpy(ln, L"race=\"Total\"");
	   tul_tied->put_wtagparams(XMLhae_tagName(TAGResult, TulosTags, nTulosTags), ln, false, level++);
	   if (kilpparam.sarjavaihtelee || sarja >= sarjaluku) {
		   tul_tied->put_wxml_s(XMLhae_tagName(TAGClass, TulosTags, nTulosTags), Sarjat[kilp.Sarja(i_pv)].sarjanimi, level, L"type='short'");
		   }
	   tul_tied->put_wxml_d(XMLhae_tagName(TAGBibNumber, TulosTags, nTulosTags), kilp.id(), level);
	   if (i_pv == k_pv) {
		   if (kilp.TLahto(0) != TMAALI0)
				tul_tied->put_wxml_time(XMLhae_tagName(TAGStartTime, TulosTags, nTulosTags), pvparam[k_pv].Date, kilp.TLahto(), t0, SEK, 8, L'.', level);
		   if (kilp.maali(0) != TMAALI0)
				tul_tied->put_wxml_time(XMLhae_tagName(TAGFinishTime, TulosTags, nTulosTags), pvparam[k_pv].Date + (kilp.maali(0) < kilp.TLahto() ? 1 : 0),
					kilp.maali(0), t0, SEK, pvparam[k_pv].laika, L'.', level);
		   }
	   if ((i_pv <= k_pv && kilp.hyv_pv(i_pv) && (tls = kilp.tulos_pv(i_pv)) > 0) ||
			(i_pv > k_pv && kilp.y_hyv(k_pv) && (tls = kilp.ytulos(k_pv)) > 0)) {
		   tul_tied->put_wxml_s(XMLhae_tagName(TAGTime, TulosTags, nTulosTags), sekTulos(NULL, tls, pvparam[k_pv].pyor[3]), level);
		   tul_tied->put_wxml_time(XMLhae_tagName(TAGTimeStr, TulosTags, nTulosTags), 0, tls, 0, SEK, -(int)pvparam[k_pv].laika2, L',', level);
		   if (i_pv == k_pv) {
			   karki = pkarki[kilp.Sarja()][0];
			   sj1 = kilp.sija(i_pv);
			   }
		   else if (i_pv > k_pv) {
			   karki = pkarki[kilp.Sarja()][kilpparam.valuku+1];
			   sj1 = kilp.y_sija(k_pv);
			   }
		   else {
			   sj1 = kilp.sija(i_pv);
			   karki = 0;
			   }
		   if (karki) {
			   tul_tied->put_wxml_s(XMLhae_tagName(TAGTimeBehind, TulosTags, nTulosTags),
					sekTulos(NULL, tls-karki, pvparam[k_pv].pyor[3]), level);
			   tul_tied->put_wxml_time(XMLhae_tagName(TAGTimeBehindStr, TulosTags, nTulosTags),
				   0, tls - karki, 0, SEK, -(int)pvparam[k_pv].laika2, L',', level);
			   }
		   if (sj1 >= 1)
			   tul_tied->put_wxml_d(XMLhae_tagName(TAGPosition, TulosTags, nTulosTags), sj1, level);
		   if (i_pv <= k_pv) {
				for (int i = 0; i < 2; i++) {
					if (kilp.pv[i_pv].pvpisteet[i]) {
						swprintf(ln, L"order='%d'", i+1);
						tul_tied->put_wxml_d(XMLhae_tagName(TAGPoints, TulosTags, nTulosTags), kilp.pv[i_pv].pvpisteet[i], level, ln);
						}
					}
				}
		   else {
				for (int i = 0; i < 3; i++) {
					if (kilp.pisteet[i]) {
						swprintf(ln, L"order='%d'", i+1);
						tul_tied->put_wxml_d(XMLhae_tagName(TAGPoints, TulosTags, nTulosTags), kilp.pisteet[i], level, ln);
						}
					}
				}
		   }
	   tul_tied->put_wxml_s(XMLhae_tagName(TAGStatus, TulosTags, nTulosTags),
			kilp.tarkstr(ln, i_pv < k_pv ? i_pv : k_pv, i_pv > k_pv, 8, tulprm->language), level);
	   if (kilp.pv[k_pv].rata[0] && (rt = haerata(&kilp)) != NULL &&
			!wcscmp(kilp.pv[k_pv].rata, rt->tunnus)) {
		   tul_tied->put_wtag(XMLhae_tagName(TAGCourse, TulosTags, nTulosTags), level++);
		   tul_tied->put_wxml_s(XMLhae_tagName(TAGName, TulosTags, nTulosTags), kilp.pv[k_pv].rata, level);

		   if (rt) {
			   tul_tied->put_wxml_d(XMLhae_tagName(TAGLength, TulosTags, nTulosTags), rt->ratapit, level);
			   if (rt->nousu)
				   tul_tied->put_wxml_d(XMLhae_tagName(TAGLength, TulosTags, nTulosTags), rt->nousu, level);
			   }
		   tul_tied->put_wantitag(XMLhae_tagName(TAGCourse, TulosTags, nTulosTags), --level);
		   }
	   if (i_pv == k_pv && tulprm->xmlva == L'E') {
		  getem(&em, kilp.id(), 0);
		  if (em.kilpno == kilp.id()) {
			  emitvatp emva;
			  bool leim_ok = true;

			 tee_emva(&emva, &em);
			 rt = haerata(&kilp);
			 for (int r = 0; r < emva.rastiluku; r++) {
				 if (kilp.tark() != L'T' && emva.rastit[r][1] == 0)
					 leim_ok = false;

				 if (emva.rastit[r][1]) {
					if (leim_ok)
						tul_tied->put_wtag(XMLhae_tagName(TAGSplitTime, TulosTags, nTulosTags), level++);
					else
						tul_tied->put_wtagparams(XMLhae_tagName(TAGSplitTime, TulosTags, nTulosTags), L"status=\"Additional\"", false, level++);
					tul_tied->put_wxml_d(XMLhae_tagName(TAGControlCode, TulosTags, nTulosTags), emva.rastit[r][0], level);
					tul_tied->put_wxml_d(XMLhae_tagName(TAGTime, TulosTags, nTulosTags), emva.rastit[r][1], level);
					}
				 else {
					if (leim_ok)
						tul_tied->put_wtag(XMLhae_tagName(TAGSplitTime, TulosTags, nTulosTags), level++);
					else
						tul_tied->put_wtagparams(XMLhae_tagName(TAGSplitTime, TulosTags, nTulosTags), L"status=\"Missing\"", false, level++);
					if (rt)
						tul_tied->put_wxml_d(XMLhae_tagName(TAGControlCode, TulosTags, nTulosTags), rt->rastikoodi[r], level);
					}
				tul_tied->put_wantitag(XMLhae_tagName(TAGSplitTime, TulosTags, nTulosTags), --level);
				}
			 }
		  }
	   tul_tied->put_wantitag(XMLhae_tagName(TAGResult, TulosTags, nTulosTags), --level);
	   }
	if (tulprm->xmlva == L'M'&& kilp.hyv()) {
		for (int iva = 1; iva <= Sarjat[kilp.Sarja()].valuku[k_pv]; iva++) {
			if ((tls = kilp.tulos2(iva)) == 0)
				continue;
			swprintf(ln, L"order=\"%d\"", iva);
			tul_tied->put_wtagparams(XMLhae_tagName(TAGSplitTime, TulosTags, nTulosTags), ln, false, level++);
			tul_tied->put_wxml_s(XMLhae_tagName(TAGTime, TulosTags, nTulosTags), sekTulos(NULL, tls, pvparam[k_pv].pyor[4]), level);
			tul_tied->put_wxml_time(XMLhae_tagName(TAGTimeStr, TulosTags, nTulosTags), 0, tls, 0, SEK, -(int)pvparam[k_pv].laikava, L',', level);
			karki = pkarki[kilp.Sarja()][iva];
			sj1 = kilp.p_sija(iva);
			if (karki) {
				tul_tied->put_wxml_s(XMLhae_tagName(TAGTimeBehind, TulosTags, nTulosTags), sekTulos(NULL, tls-karki, pvparam[k_pv].pyor[4]), level);
				tul_tied->put_wxml_time(XMLhae_tagName(TAGTimeBehindStr, TulosTags, nTulosTags), 0, tls - karki, 0, SEK, -(int)pvparam[k_pv].laikava, L',', level);
				}
			if (sj1 >= 1)
				tul_tied->put_wxml_d(XMLhae_tagName(TAGPosition, TulosTags, nTulosTags), sj1, level);
			tul_tied->put_wantitag(XMLhae_tagName(TAGSplitTime, TulosTags, nTulosTags), --level);
			}
		}
	tul_tied->put_wantitag(XMLhae_tagName(TAGPersonResult, TulosTags, nTulosTags), --level);
	}

