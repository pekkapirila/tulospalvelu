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
#include <io.h>
#include <fcntl.h>
#include <string>
#include <process.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <bkeybrd.h>
#include <bstrings.h>
#pragma hdrstop
#include <dos.h>
#include <sys/stat.h>
#include <time.h>
#include "VDeclare.h"

#ifndef _CONSOLE
#include "UnitMain.h"
#include "UnitMsgs.h"
#endif

#define K_TIED
#define SEURALISTA 15
int yhttl = 0;

#define SEURATUL

int xmlIOF30ots(tulostusparamtp *tulprm);
int xmlIOF30loppu(tulostusparamtp *tulprm);
int xmlIOF30srjots(int sarja, tulostusparamtp *tulprm);
int xmlIOF30srjloppu(tulostusparamtp *tulprm);
void xmlIOF30tulos(kilptietue& kilp, INT sj, tulostusparamtp *tulprm);

typedef struct {
	wchar_t *tag_event;
	wchar_t *tag_evnm;
	wchar_t *tag_cl;
	wchar_t *tag_clnm;
	wchar_t *tag_tm;
	wchar_t *tag_tmid;
	wchar_t *tag_tmnm;
	wchar_t *tag_tmno;
	wchar_t *tag_tmjk;
	wchar_t *tag_country;
	wchar_t *tag_leg;
	wchar_t *tag_legno;
	wchar_t *tag_legnm;
	wchar_t *tag_ce;
	wchar_t *tag_emno;
	wchar_t *tag_ctl;
	wchar_t *tag_cn;
	wchar_t *tag_cc;
	wchar_t *tag_ct;
	wchar_t *tag_ctsecs;
	wchar_t *tag_cd;
	wchar_t *tag_res;
	wchar_t *tag_ressecs;
	wchar_t *tag_rank;
	wchar_t *tag_legres;
	wchar_t *tag_legrank;
	 } tagstp;

static tagstp tag_l = {
	  L"event",
	  L"eventname",
	  L"class",
	  L"classname",
	  L"team",
	  L"teamid",
	  L"teamname",
	  L"teamnro",
	  L"teamnmno",
	  L"country",
	  L"leg",
	  L"legnro",
	  L"name",
	  L"course",
	  L"emitnro",
	  L"control",
	  L"controlnro",
	  L"controlcode",
	  L"controltime",
	  L"CTSecs",
	  L"controltimedif",
	  L"result",
	  L"TSecs",
	  L"placement",
	  L"legresult",
	  L"legplacement"
	  };

static tagstp tag_s = {
	  L"event",
	  L"eventname",
	  L"class",
	  L"classname",
	  L"team",
	  L"teamid",
	  L"teamname",
	  L"teamnro",
	  L"teamnmno",
	  L"country",
	  L"leg",
	  L"legnro",
	  L"nm",
	  L"crs",
//	  L"splittimes",
	  L"emit",
	  L"control",
	  L"cn",
	  L"cc",
	  L"ct",
	  L"ctsecs",
	  L"cd",
	  L"result",
	  L"tsecs",
	  L"placement",
	  L"legresult",
	  L"legplacement"
	  };

static tagstp tag_iof = {
	  L"Event",
	  L"EventName",
	  L"EventClass",
	  L"ClassName",
	  L"Team",
	  L"TeamId",
	  L"TeamName",
	  L"StartNumber",
	  L"TeamNameNo",
	  L"Nationality",
	  L"Leg",
	  L"LegNo",
	  L"Name",
	  L"Course",
	  L"Emit",
	  L"Control",
	  L"ControlOrder",
	  L"CCode",
	  L"ControlTime",
	  L"CTSecs",
	  L"CTDiff",
	  L"Time",
	  L"TSecs",
	  L"Rank",
	  L"LegTime",
	  L"LegRank"
	  };

//	  L"ControlRank",
//	  L"ControlDist",
//	  L"Order",
//	  L"Identity",
//	  L"Country",
//	  L"ClassStyle",
//    L"ClassDist",
//	  L"Competitor",
//	  L"EventCode",
//	  L"Family",
//      L"Given",
//      L"ClubID",
//	  L"ClubName",
//	  L"StartTime",
//	  L"STSecs",
//	  L"Time",
//	  L"TSecs",
//      L"Rank",
//      L"Status",
//	  L"SplitTimes",
//		L"LastDate",
//		L"PartCount",


static tagstp tag_a;

int xmlots(tulostusparamtp *tulprm)
   {
   wchar_t line[202];
   TextFl *tul_tied;

   if (tulprm->optiot & 0x00400000)
	   return(xmlIOF30ots(tulprm));
   switch ((tulprm->optiot & 0x00f00000)/0x00100000) {
	   case 0:
		  tag_a = tag_s;
		  break;
	   case 1:
		  tag_a = tag_l;
		  break;
	   case 2:
		  tag_a = tag_iof;
		  break;
	   }
	if (tulprm->merkit == L'A')
	   swprintf(line, L"<?xml version=\"1.0\" encoding=\"ISO-8859-1\" standalone=\"yes\"?>\n"
		  L"<!-- ?xml-stylesheet type=\"text/xsl\" href=\"tlsmuoto.xsl\"? -->\n"
		  L"<%s Type=\"Relay\">\n", tag_a.tag_event);
	else
	   swprintf(line, L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n"
		  L"<!-- ?xml-stylesheet type=\"text/xsl\" href=\"tlsmuoto.xsl\"? -->\n"
		  L"<%s Type=\"Relay\">\n", tag_a.tag_event);

   tul_tied = tulprm->lstf->u.Fl;
   tul_tied->WriteLine(line);
   wcscpy(line, kilpailu);
   elimwbl(line);
   tul_tied->put_wxml_s(tag_a.tag_evnm, line, 0);
   return(0);
   }

int xmlloppu(tulostusparamtp *tulprm)
   {
   TextFl *tul_tied;

   if (tulprm->optiot & 0x00400000)
	   return(xmlIOF30loppu(tulprm));
   tul_tied = tulprm->lstf->u.Fl;

   tul_tied->put_wantitag(tag_a.tag_event, 0);
   return(0);
   }

int xmlsrjots(int sarja, tulostusparamtp *tulprm)
   {
   wchar_t buf[40];
   TextFl *tul_tied;

   kaikkisijat(tulprm, sarja);
   if (tulprm->optiot & 0x00400000)
	   return(xmlIOF30srjots(sarja, tulprm));
   tul_tied = tulprm->lstf->u.Fl;
   tul_tied->put_wtag(tag_a.tag_cl, 0);
   tul_tied->put_wxml_s(tag_a.tag_clnm, Sarjat[sarja].Sarjanimi(buf), 0);
   return(0);
   }

int xmlsrjloppu(tulostusparamtp *tulprm)
   {
   TextFl *tul_tied;

   if (tulprm->optiot & 0x00400000)
	   return(xmlIOF30srjloppu(tulprm));
   tul_tied = tulprm->lstf->u.Fl;
   tul_tied->put_wantitag(tag_a.tag_cl, 0);
   return(0);
   }

void xmltulos(kilptietue& kilp, INT sj, tulostusparamtp *tulprm)
   {
   wchar_t as[16], st[80];
#ifndef LUENTA
   INT edva, va[51];
   emittp em;
   ratatp *rt;
#endif
   INT ep = -1, os, i, sj1, mukaan;
   static int order;
   int os_ala, os_yla;
   INT32 tl, tl0 = TMAALI0;
   TextFl *tul_tied;

   if (tulprm->optiot & 0x00400000)
	   return(xmlIOF30tulos(kilp, sj, tulprm));
   tul_tied = tulprm->lstf->u.Fl;
	if (!tulprm->emitva)
		tulprm->poistanollat = true;

   if (tulprm->viimos == L'K') {
		os_ala = 0;
		os_yla = Sarjat[kilp.sarja].osuusluku-1;
		if (tulprm->optiot & 0x00040000) {
			os_yla = 2;
			}
		else if (tulprm->optiot & 0x00080000) {
			os_ala = 3;
			}
	   }
   else {
		os_ala = tulprm->osuus;
		os_yla = tulprm->osuus;
		}
	if (tulprm->tulostettava == L'L' || kilp.Maali(0, 0) != TMAALI0 ||
		(wcswcind(kilp.Tark(0, 1), L"E-") < 0)) {
		tul_tied->put_wtag(tag_a.tag_tm,0);
		if (order > 1 && sj == 1)
			order = 1;
		else
			order++;
		if (!tulprm->emitva)
			tul_tied->put_wxml_d(L"order", order, 0);
		tul_tied->put_wxml_d(tag_a.tag_tmid, kilp.kilpno,0);
		if (!tulprm->emitva)
			tul_tied->put_wxml_s(tag_a.tag_clnm, Sarjat[kilp.sarja].Sarjanimi(st), 0);
		tul_tied->put_wxml_s(tag_a.tag_tmnm, kilp.Seura(st, kilpparam.lseura),0);
		tul_tied->put_wxml_d(tag_a.tag_tmno, kilp.joukkue,0);
		if (!tulprm->emitva)
			tul_tied->put_wxml_s(tag_a.tag_tmjk, kilp.Joukkue(st, kilpparam.lseura+3),0);
		tul_tied->put_wxml_s(tag_a.tag_country, kilp.Maa(),0);
		if ((tl = tulos(&kilp, os_yla, 0)) != 0) {
			if (tl != tl0)
				sj1 = sj;
			tl0 = tl;
			aikatowstr_cols_n(as, tl, 0, tulprm->decsep, kilpparam.laika2);
			if (tulprm->poistanollat)
				wpoistanollat(as);
			tul_tied->put_wxml_s(tag_a.tag_res, as,0);
			tul_tied->put_wxml_d(tag_a.tag_ressecs, tl/SEK,0);
			tul_tied->put_wxml_d(tag_a.tag_rank, sj1,0);
			}
		else if (wcswcind(kilp.Tark(os_yla, 1), L"KHSE") >= 0) {
			if (tulprm->language > 0)
				wcscpy(as, L"DidNotFinish");
			else
				wcscpy(as, L"Keskeyttänyt");
			for (os = os_ala; os <= os_yla; os++) {
				if (kilp.Tark(os, 1) == L'H') {
					if (tulprm->language > 0)
						wcscpy(as, L"Disqualified");
					else
						wcscpy(as, L"Hylätty");
					break;
					}
				}
			tul_tied->put_wxml_s(tag_a.tag_res, as,0);
			}
		for (os = os_ala; os <= os_yla; os++) {
			mukaan = 0;
			if (tulprm->emitva) {
#ifndef LUENTA
				ep = getem(&em, kilp.kilpno, os);
#endif
				}
			else {
				if (kilp.Sulj(os))
					break;
				for (i = 0; i <= kilpparam.maxvaluku; i++) {
					if (tulos(&kilp, os, i)) {
						mukaan = 1;
						break;
						}
					}
				}
			if (tulprm->tulostettava == L'L' || mukaan || kilp.Maali(os, 0) != TMAALI0 || ep >= 0) {
				tul_tied->put_wtag(tag_a.tag_leg,0);
				if (!tulprm->emitva)
					tul_tied->put_wxml_d(tag_a.tag_tmid, kilp.kilpno,0);
				tul_tied->put_wxml_d(tag_a.tag_legno, os+1,0);
				tul_tied->put_wxml_s(tag_a.tag_legnm, kilp.Nimi(st, kilpparam.lnimi, os, tulprm->tulmuot.etusuku),0);
				tul_tied->put_wxml_s(tag_a.tag_ce, kilp.ostiet[os].Hajonta(st),0);
				if (tulprm->emitva) {
#ifndef LUENTA
					tul_tied->put_wxml_d(tag_a.tag_emno, kilp.ostiet[os].badge[0],0);
					if (ep >= 0) {
						vatulkinta(&em, va);
						rt = getrata(kilp.ostiet[os].hajonta);
						if (rt && rt->rastiluku != 0) {
							for (i = 0; i < rt->rastiluku; i++) {
								if (va[i]) {
									tul_tied->put_wtag(tag_a.tag_ctl,0);
									tul_tied->put_wxml_d(tag_a.tag_cn, i+1,0);
									tul_tied->put_wxml_d(tag_a.tag_cc, rt->rastikoodi[i],0);
									aikatowstr_cols_n(as, SEK*va[i], 0, tulprm->decsep, 8);
									if (tulprm->poistanollat)
										wpoistanollat(as);
									tul_tied->put_wxml_s(tag_a.tag_ct, as,0);
									if (i > 0)
										edva = va[i-1];
									else
										edva = 0;
									aikatowstr_cols_n(as, SEK*(va[i]-edva), 0, tulprm->decsep, 8);
									wpoistanollat(as);
									tul_tied->put_wxml_s(tag_a.tag_cd, as,0);
									tul_tied->put_wantitag(tag_a.tag_ctl,0);
									}
								}
							}
						}
#endif
					}
				else {
/*
					for (i = 1; i <= kilpparam.maxvaluku; i++) {
						if ((tl = tulos(&kilp, os, i)) != 0) {
							tul_tied->put_wtag(tag_a.tag_ctl,0);
							tul_tied->put_wxml_d(tag_a.tag_cn, i,0);
							aikatowstr_cols_n(as, tl, 0, tulprm->decsep, kilpparam.laika2);
							if (tulprm->poistanollat)
								wpoistanollat(as);
							tul_tied->put_wxml_s(tag_a.tag_ct, as,0);
							tul_tied->put_wxml_d(tag_a.tag_ctsecs, tl/SEK,0);
							tul_tied->put_wxml_d(tag_a.tag_rank, kilp.Sija(os, i),0);
							tul_tied->put_wantitag(tag_a.tag_ctl,0);
							}
						}
*/
					}
				if (tulprm->emitva)
					tl = osuustulos(&kilp, os, 0);
				else
					tl = tulos(&kilp, os, 0);
				if (tl) {
					aikatowstr_cols_n(as, tl, 0, tulprm->decsep, kilpparam.laika2);
					if (tulprm->poistanollat)
						wpoistanollat(as);
					tul_tied->put_wxml_s(tag_a.tag_res, as,0);
					tul_tied->put_wxml_d(tag_a.tag_ressecs, tl/SEK,0);
					if (!tulprm->emitva)
						tul_tied->put_wxml_d(tag_a.tag_rank, kilp.Sija(os, 0),0);
					if (!tulprm->emitva)
						tul_tied->put_wxml_s(tag_a.tag_ct, L"OK",0);
					}
				else if (wcswcind(kilp.Tark(os, 1), L"KH") >= 0) {
					switch (kilp.Tark(os, 1)) {
						case L'H':
							if (tulprm->language > 0)
								wcscpy(as, L"DQ");
							else
								wcscpy(as, L"Hylätty");
							break;
						case L'K':
							if (tulprm->language > 0)
								wcscpy(as, L"DNF");
							else
								wcscpy(as, L"Kesk.");
							break;
						}
//					tul_tied->put_wtag(tag_a.tag_ctl,0);
					tul_tied->put_wxml_d(tag_a.tag_cn, i,0);
					tul_tied->put_wxml_s(tag_a.tag_ct, as,0);
					tul_tied->put_wxml_s(tag_a.tag_res, as,0);
//					tul_tied->put_wantitag(tag_a.tag_ctl,0);
					}
				if (!tulprm->emitva && (tl = kilp.osTulos(os, 0, true)) != 0) {
					aikatowstr_cols_n(as, tl, 0, tulprm->decsep, kilpparam.laika2);
					if (tulprm->poistanollat)
						wpoistanollat(as);
					tul_tied->put_wxml_s(tag_a.tag_legres, as,0);
					if (os == 0)
						tul_tied->put_wxml_d(tag_a.tag_legrank, kilp.Sija(os, 0),0);
					else
						tul_tied->put_wxml_d(tag_a.tag_legrank, kilp.ostiet[os].ossija,0);
					}
				tul_tied->put_wantitag(tag_a.tag_leg,0);
				}
			}
		tul_tied->put_wantitag(tag_a.tag_tm,0);
		}
}

