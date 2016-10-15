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
#ifndef __linux__
#include <io.h>
#include <process.h>
#include <direct.h>
#include <dos.h>
#else
#include <unistd.h>
#include <sys/types.h>
#endif
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#ifdef _CONSOLE
//#include <bvideo.h>
//#include <bkeybrd.h>
#endif
#include <bstrings.h>
#pragma hdrstop
#include <sys/stat.h>
#include "HkDeclare.h"

int lueIOF30VainRadatXml(wchar_t *filename, IOFCourseData *CseData);
extern INT tarkkuusparam;
bool kirjXMLkaikki;

static wchar_t Dis[10] = L"SHJUAMNPB", *DisSt[9] = {L"Orienteering", L"Skiing", L"Running",
	L"Swimming", L"Biathlon", L"Skijumping", L"NavalPentathlon", L"Pentathlon", L"BiathlonOrienteering"};
static wchar_t Sub[18] = L"RSPMAEDNIOLUFCHWT";
static wchar_t *SubSt[17] = {L"Regular", L"Sprint", L"Pursuit", L"Mass start", L"Pair",
	L"Event", L"Rounds", L"Naval", L"Military", L"Obstacle", L"LifeSaving", L"UtilitySwim", L"Seamanship",
	L"CrossCountry", L"Shooting", L"Swimming", L"Throwing"};
static wchar_t Stat[13] = L"-THKEOIVPXMB", *StatSt[12] = {L"Open", L"OK", L"DQ", L"DNF", L"DNS", L"LAP", L"Open", L"Vacant", L"Absent", L"NoResult", L"Missing", L"Observed"};
static wchar_t Bdg[5] = L"ETSO", *BdgSt[4] = {L"Emit", L"emiTag", L"Sirit", L"Other"};
static wchar_t YhtTls[5] = L"SLPV", *YhtTlsSt[4] = {L"Sum", L"FromRaceStart", L"Best", L"VarSum"};
static wchar_t *Pr[4] = {L"1", L"0,1", L"0,01", L"0,001"};
static wchar_t *Tabs[8] = {L"", L"\t", L"\t\t", L"\t\t\t", L"\t\t\t\t", L"\t\t\t\t\t", L"\t\t\t\t\t\t", L"\t\t\t\t\t\t\t"};


#define TAGEvent				0
#define TAGSoftware 			1
#define TAGVersionId 			2
#define TAGFileFormat 			3
#define TAGRaces 				4
#define TAGEventData			5
#define TAGIntermediaries 		6
#define TAGSport 				7
#define TAGDiscipline 			8
#define TAGSubtype 				9
#define TAGEventCode 			10
#define TAGEventClassification 	11
#define TAGTitle 				12
#define TAGName 				13
#define TAGFamily 				14
#define TAGGiven 				15
#define TAGStartDate 			16
#define TAGFinishDate 			17
#define TAGOrganizer 			18
#define TAGLocation 			19
#define TAGCountry 				20
#define TAGCalender 			21
#define TAGYear 				22
#define TAGList 				23
#define TAGEventOfficial 		24
#define TAGEventOfficialRole 	25
#define TAGPersonId 			26
#define TAGRaceCount 			27
#define TAGClassCount 			28
#define TAGRace 				29
#define TAGRaceNo 				30
#define TAGEventRaceId 			31
#define TAGDate 				32
#define TAGFISData 				33
#define TAGFValue 				34
#define TAGMinPenalty 			35
#define TAGIdStart 				36
#define TAGPrecision 			37
#define TAGInternal 			38
#define TAGResults 				39
#define TAGRaceOfficial 		40
#define TAGRaceOfficialRole 	41
#define TAGClasses 				42
#define TAGClass 				43
#define TAGClassNo 				44
#define TAGClassId 				45
#define TAGFirstRace 			46
#define TAGLastRace 			47
#define TAGFee 					48
#define TAGFull 				49
#define TAGSingleRace 			50
#define TAGReduced 				51
#define TAGResultListOrder 		52
#define TAGSpecialCoding 		53
#define TAGHandicaps 			54
#define TAGDistance 			55
#define TAGValue 				56
#define TAGUnit 				57
#define TAGStartGate 			58
#define TAGFinishGate 			59
#define TAGBibStart 			60
#define TAGFirstStart 			61
#define TAGFirstExtraStart 		62
#define TAGStartInterval 		63
#define TAGStartWave 			64
#define TAGPenaltyCount 		65
#define TAGPenaltyUnit 			66
#define TAGPointsCount 			67
#define TAGResultThreshold 		68
#define TAGIntermediary 		69
#define TAGOrder 				70
#define TAGParticipants			71
#define TAGParticipant			72
#define TAGDataStatus			73
#define TAGId					74
#define TAGPersonIdTxt			75
#define TAGPersonIdInt			76
#define TAGClub					77
#define TAGClubCode				78
#define TAGDistrict				79
#define TAGOrganization			80
#define TAGTeam					81
#define TAGResult				82
#define TAGStatus				83
#define TAGPoints				84
#define TAGRank					85
#define TAGBib					86
#define TAGBadge				87
#define TAGStartTime			88
#define TAGStartGateTime		89
#define TAGPenalty				90
#define TAGBiathlon				91
#define TAGCount				92
#define TAGSecs					93
#define TAGVariableClass		94
#define TAGVariableBib			95
#define TAGPursuit              96
#define TAGResultFormula		97
#define TAGPointFormula 		98
#define TAGUseClubId   			99
#define TAGVacantClass		   100
#define TAGCourseData  		   101
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
#define TAGCourseVariation	   119
#define TAGCourseVariationId   120
#define TAGCourseLength		   121
#define TAGCourseClimb 		   122
#define TAGCourseControl	   123
#define TAGSequence    		   124
#define TAGLegLength   		   125
#define TAGDistanceToFinish	   126
#define TAGIOFVersion      	   127
#define TAGMultiRace      	   128
#define TAGPunchingUnit		   129
#define TAGUnitCode            130
#define TAGClearECard		   131
#define TAGPublic			   132
#define TAGUseClassId  		   133
#define TAGStartQueue		   134
#define TAGPointRules		   135
#define TAGRankBased           136
#define TAGResultBased         137
#define TAGTotal		       138
#define TAGFlags		       139
#define TAGClassCourseAssignment 140
#define TAGClassName	   	   141
#define TAGMapPositionTopLeft  142
#define TAGMapPositionBottomRight 143
#define TAGSprintDef	   	   144
#define TAGBaseClass	   	   145
#define TAGHeats	   	   	   146
#define TAGHeatSize	   	   	   147
#define TAGBOOrder			   148
#define TAGBOPenalties		   149
#define TAGPersonalBest		   150
#define TAGGoal				   151
#define TAGEmitToSplits		   152
#define TAGCombinedResult	   153
#define TAGPenalties		   154
#define TAGTechnique		   155
#define TAGIntIdLen		   	   156
#define TAGPenaltyMax 		   157

static wchar_t *srjtag[159] = {
	L"Event",
	L"Software",
	L"VersionId",
	L"FileFormat",
	L"Races",
	L"EventData",
	L"Intermediaries",
	L"Sport",
	L"Discipline",
	L"Subtype",
	L"EventCode",
	L"EventClassification",
	L"Title",
	L"Name",
	L"Family",
	L"Given",
	L"StartDate",
	L"FinishDate",
	L"Organizer",
	L"Location",
	L"Country",
	L"Calender",
	L"Year",
	L"List",
	L"EventOfficial",
	L"EventOfficialRole",
	L"PersonId",
	L"RaceCount",
	L"ClassCount",
	L"Race",
	L"RaceNo",
	L"EventRaceId",
	L"Date",
	L"FISData",
	L"FValue",
	L"MinPenalty",
	L"IdStart",
	L"Precision",
	L"Internal",
	L"Results",
	L"RaceOfficial",
	L"RaceOfficialRole",
	L"Classes",
	L"Class",
	L"ClassNo",
	L"ClassId",
	L"FirstRace",
	L"LastRace",
	L"Fee",
	L"Full",
	L"SingleRace",
	L"Reduced",
	L"ResultListOrder",
	L"SpecialCoding",
	L"Handicaps",
	L"Distance",
	L"Value",
	L"Unit",
	L"StartGate",
	L"FinishGate",
	L"BibStart",
	L"FirstStart",
	L"FirstExtraStart",
	L"StartInterval",
	L"StartWave",
	L"PenaltyCount",
	L"PenaltyUnit",
	L"PointsCount",
	L"ResultThreshold",
	L"Intermediary",
	L"Order",
	L"Participants",
	L"Participant",
	L"DataStatus",
	L"Id",
	L"PersonIdTxt",
	L"PersonIdInt",
	L"Club",
	L"ClubCode",
	L"District",
	L"Organization",
	L"Team",
	L"Result",
	L"Status",
	L"Points",
	L"Rank",
	L"Bib",
	L"Badge",
	L"StartTime",
	L"StartGateTime",
	L"Penalty",
	L"Biathlon",
	L"Count",
	L"Secs",
	L"VariableClass",
	L"VariableBib",
	L"Pursuit",
	L"ResultFormula",
	L"PointFormula",
	L"UseClassId",
	L"VacantClass",
	L"CourseData",
	L"ModifyDate",
	L"Clock",
	L"Map",
	L"Scale",
	L"AutoResult",
	L"StartPoint",
	L"StartPointCode",
	L"ControlPosition",
	L"MapPosition",
	L"Control",
	L"ControlCode",
	L"FinishPoint",
	L"FinishPointCode",
	L"Course",
	L"CourseName",
	L"CourseId",
	L"ClassShortName",
	L"CourseVariation",
	L"CourseVariationId",
	L"CourseLength",
	L"CourseClimb",
	L"CourseControl",
	L"Sequence",
	L"LegLength",
	L"DistanceToFinish",
	L"IOFVersion",
	L"MultiRace",
	L"PunchingUnit",
	L"UnitCode",
	L"ClearECard",
	L"Public",
	L"UseClassId",
	L"StartQueue",
	L"PointRules",
	L"RankBased",
	L"ResultBased",
	L"Total",
	L"Flags",
	L"ClassCourseAssignment",
	L"ClassName",
	L"MapPositionTopLeft",
	L"MapPositionBottomRight",
	L"SprintDef",
	L"BaseClass",
	L"Heats",
	L"HeatSize",
	L"BOOrder",
	L"BOPenalties",
	L"PersonalBest",
	L"Goal",
	L"EmitToSplits",
	L"CombinedResult",
	L"Penalties",
	L"Technique",
	L"IntIdLen",
	L"PenaltyMax",
	L""
	};

static wchar_t haetunnus(wchar_t *Nimi, wchar_t *Koodit, wchar_t **Nimet, int N)
{
	for (int i = 0; i < N; i++) {
		if (!wcscmp(Nimi, Nimet[i]))
			return(Koodit[i]);
		}
	return(0);
}

static int PyorIx(int pyor)
{
	int i;
	if (pyor == 0)
		pyor = 1;
	pyor = SEK/pyor;
	for (i = 0; i < 3; i++) {
		if (pyor < 10)
			break;
		pyor /= 10;
		}
	return(i);
}

static wchar_t *haeteksti(wchar_t Koodi, wchar_t *Koodit, wchar_t **Nimet)
{
	static wchar_t Tyhja[2] = L"";

	int i = wcswcind(Koodi, Koodit);
	if (i >= 0)
		return(Nimet[i]);
	return(Tyhja);
}

static int hae_tagid(wchar_t *Tag)
{
	for (unsigned int i = 0; i < sizeof(srjtag)/sizeof(srjtag[0]); i++)
		if (!wcscmp(Tag, srjtag[i]))
			return(i);
	return(-1);
}

static void haenodeid(xml_node *node, int nnode)
{
	for (int inode = 0; inode <= nnode; inode++)
		node[inode].tagid = hae_tagid(node[inode].tag);
}

static int loadSoftwareData(xml_node *node, int nnode)
{
	int er = 0, inode;

	haenodeid(node, nnode);
	for (inode = 1; inode <= nnode; inode++) {
		switch (node[inode].tagid) {
			case TAGVersionId :
				break;
			case TAGFileFormat :
				break;
			case TAGRaces :
				node[inode].gettext_int(&kilpparam.n_pv);
				break;
			case TAGIntermediaries :
				node[inode].gettext_int(&kilpparam.valuku);
				break;
			}
		}
	return(er);
}

static int loadEventData(xml_node *node, int nnode)
{
	int er = 0, inode;
	wchar_t ln[200], *p;

	haenodeid(node, nnode);
	for (inode = 1; inode <= nnode; inode++) {
		switch (node[inode].tagid) {
			case TAGEventCode :
				node[inode].gettext(kilpparam.kilpkoodi, sizeof(kilpparam.kilpkoodi)/2);
				break;
			case TAGTitle :
				node[inode].gettext(ln, sizeof(kilpailu)/2);
				if (wcslen(ln) > 1)
					wcscpy(kilpailu, ln);
				break;
			case TAGSport :
				break;
			case TAGDiscipline :
				if (node[inode].gettext(ln, sizeof(ln)/2))
					kilpparam.kilplaji = haetunnus(ln, Dis, DisSt,
						sizeof(DisSt)/sizeof(DisSt[0]));
				break;
			case TAGSubtype :
				if (node[inode].gettext(ln, sizeof(ln)/2))
					kilpparam.alalaji = haetunnus(ln, Sub, SubSt,
						sizeof(SubSt)/sizeof(SubSt[0]));
				break;
			case TAGYear :
				node[inode].gettext_int(&kilpparam.vuosi);
				break;
			case TAGRaceCount :
				node[inode].gettext_int(&kilpparam.n_pv_akt);
				break;
			case TAGClassCount :
				node[inode].gettext_int(&sarjaluku);
				break;
			case TAGVariableClass :
				node[inode].gettext(ln, sizeof(ln)/2);
				kilpparam.sarjavaihtelee = (towupper(ln[0]) == L'Y') ? 1 : 0;
				break;
			case TAGVariableBib :
				node[inode].gettext(ln, sizeof(ln)/2);
				kilpparam.bibvaihtelee = (towupper(ln[0]) == L'Y') ? 1 : 0;
				break;
			case TAGIntIdLen :
				node[inode].gettext_int(&kilpparam.kvlisno);
				break;
			case TAGUseClubId :
			case TAGUseClassId :
				node[inode].gettext(ln, sizeof(ln)/2);
				kilpparam.seuralyh_on = (towupper(ln[0]) == L'Y') ? 1 : 0;
				break;
			case TAGBadge :
				if (node[inode].gettext(ln, sizeof(ln)/2)) {
					if ((p = wcsstr(ln, L"*2")) != 0) {
						kilpparam.kaksibadge = 1;
						*p = 0;
						}
					kilpparam.badgelaji = haetunnus(ln, Bdg, BdgSt,
						sizeof(BdgSt)/sizeof(BdgSt[0]));
					}
				break;
			case TAGCombinedResult :
				if (node[inode].gettext(ln, sizeof(ln)/2))
					kilpparam.yhttlslaji = haetunnus(ln, YhtTls, YhtTlsSt,
						sizeof(YhtTlsSt)/sizeof(YhtTlsSt[0]));
				break;
			}
		}
	return(er);
}

static int loadRaceData(xml_node *node, int nnode)
{
	int er = 0, inode, DepthIn, haara = 0, i_pv = 0;
	wchar_t ln[40];

	haenodeid(node, nnode);
	if (!node[0].getparam_int(L"RaceNo", &i_pv))
		return(1);
	i_pv--;
	if (i_pv < 0 || i_pv >= kilpparam.n_pv_akt)
		return(1);
	DepthIn = node[0].depth + 1;
	for (inode = 1; inode <= nnode; inode++) {
		if (node[inode].depth == DepthIn) {
			haara = 0;
			switch (node[inode].tagid) {
				case TAGEventRaceId :
					node[inode].gettext(pvparam[i_pv].RaceId, sizeof(pvparam[i_pv].RaceId)/2);
					break;
				case TAGTitle :
					node[inode].gettext(pvparam[i_pv].Title, sizeof(pvparam[i_pv].Title)/2);
					break;
				case TAGDate :
					node[inode].gettext(ln, sizeof(ln)/2);
					pvparam[i_pv].Date = DateNoFromSt(ln);
					break;
				case TAGDiscipline :
					if (node[inode].gettext(ln, sizeof(ln)/2))
						pvparam[i_pv].kilplaji = haetunnus(ln, Dis, DisSt,
							sizeof(DisSt)/sizeof(DisSt[0]));
					break;
				case TAGSubtype :
					if (node[inode].gettext(ln, sizeof(ln)/2))
						pvparam[i_pv].alalaji = haetunnus(ln, Sub, SubSt,
							sizeof(SubSt)/sizeof(SubSt[0]));
					break;
				case TAGStartGate :
					if (node[inode].gettext(ln, sizeof(ln)/2))
						pvparam[i_pv].hiihtolahto = (towupper(ln[0]) == L'Y' ? 1 : 0);
					break;
//				case TAGPursuitExtra :
//					if (node[inode].gettext(ln, sizeof(ln)/2))
//						pvparam[i_pv].takaaAjoLisa = (towupper(ln[0]) == L'Y' ? 1 : 0);
//					break;
				case TAGPrecision :
					haara = 1;
					break;
				}
			continue;
			}
		if (node[inode].depth == DepthIn + 1) {
			if (haara == 1) {
				switch (node[inode].tagid) {
					case TAGInternal :
						if (node[inode].gettext(ln, sizeof(ln)/2)) {
							if (wcslen(ln) > 2)
								pvparam[i_pv].pyor[1] = SEK/powi(10, (wcslen(ln)-2));
							else
								pvparam[i_pv].pyor[1] = SEK;
							pvparam[i_pv].laika = wcslen(ln)+7;
							}
					break;
					case TAGRank :
						if (node[inode].gettext(ln, sizeof(ln)/2)) {
							if (wcslen(ln) > 2)
								pvparam[i_pv].pyor[2] = SEK/powi(10, (wcslen(ln)-2));
							else
								pvparam[i_pv].pyor[2] = SEK;
							pvparam[i_pv].laikasj = wcslen(ln)+7;
							}
					break;
					case TAGResults :
						if (node[inode].gettext(ln, sizeof(ln)/2)) {
							if (wcslen(ln) > 2)
								pvparam[i_pv].pyor[3] = SEK/powi(10, (wcslen(ln)-2));
							else
								pvparam[i_pv].pyor[3] = SEK;
							pvparam[i_pv].laika2 = wcslen(ln)+7;
							}
					break;
					case TAGPursuit :
						if (node[inode].gettext(ln, sizeof(ln)/2)) {
							if (wcslen(ln) > 2)
								pvparam[i_pv].pyor[4] = SEK/powi(10, (wcslen(ln)-2));
							else
								pvparam[i_pv].pyor[4] = SEK;
							}
					break;
					case TAGIntermediary :
						if (node[inode].gettext(ln, sizeof(ln)/2)) {
							if (wcslen(ln) > 2)
								pvparam[i_pv].pyor[5] = SEK/powi(10, (wcslen(ln)-2));
							else
								pvparam[i_pv].pyor[5] = SEK;
							pvparam[i_pv].laikava = wcslen(ln)+7;
							}
					break;
					}
				continue;
				}
			continue;
			}
		}
	if (pvparam[i_pv].laikava < 8)
		pvparam[i_pv].laikava = pvparam[i_pv].laika2;
	return(er);
}

static int loadClassRaceData(sarjatietue *Sarja, xml_node *node, int *inode, int nnode)
{
	int er = 0, DepthIn, haara = 0, haara2 = 0, i_pv, i_va = 0;
	int iRule = 0;
	wchar_t ln[80], *p;

	if (!node[*inode].getparam_int(L"RaceNo", &i_pv))
		return(1);
	i_pv--;
	if (i_pv < 0 || i_pv >= kilpparam.n_pv)
		return(1);
	DepthIn = node[*inode].depth + 1;
	++*inode;
	for (; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			return(0);
			}
		if (node[*inode].depth == DepthIn) {
			haara = 0;
			switch (node[*inode].tagid) {
				case TAGName :
					node[*inode].gettext(Sarja->pvsarjanimi[i_pv], sizeof(Sarja->pvsarjanimi[i_pv])/2);
					break;
				case TAGFlags :
					node[*inode].gettext(Sarja->flags[i_pv], sizeof(Sarja->flags[i_pv])/2);
					break;
				case TAGBibStart :
					node[*inode].gettext_int(&Sarja->bibalku[i_pv]);
					break;
				case TAGDistance :
					haara = 1;
					break;
				case TAGTechnique :
					node[*inode].gettext(Sarja->tapa[i_pv], sizeof(Sarja->tapa[i_pv])/2);
					wtransl(L"CF", L"PV", Sarja->tapa[i_pv]);
					wtransl(L"CF", L"PV", Sarja->tapa[i_pv]+1);
					break;
				case TAGStartGate :
					node[*inode].gettext_int(&Sarja->lno[i_pv]);
					break;
				case TAGStartQueue :
					node[*inode].gettext_int(&Sarja->portti[i_pv]);
					break;
				case TAGFinishGate :
					node[*inode].gettext_int(&Sarja->mno[i_pv]);
					break;
				case TAGFirstStart :
					if (node[*inode].gettext(ln, sizeof(ln)/2))
						Sarja->enslahto[i_pv] = wstrtoaika_vap(ln, t0);
					break;
				case TAGFirstExtraStart :
					if (node[*inode].gettext(ln, sizeof(ln)/2))
						Sarja->ensjlahto[i_pv] = wstrtoaika_vap(ln, t0);
					break;
				case TAGStartInterval :
					node[*inode].gettext_int(&Sarja->lahtovali[i_pv]);
					break;
				case TAGStartWave :
					node[*inode].gettext_int(&Sarja->pari[i_pv]);
					if (Sarja->pari[i_pv] < 1)
						Sarja->pari[i_pv] = 1;
					break;
				case TAGPenaltyCount :
					node[*inode].gettext_int(&Sarja->lsak[i_pv]);
					break;
				case TAGPenaltyUnit :
					node[*inode].gettext_int(&Sarja->tsak[i_pv]);
					break;
				case TAGPenaltyMax :
					node[*inode].gettext_int(&kilpparam.maxasakko);
					break;
				case TAGResultThreshold :
					node[*inode].gettext_int(&Sarja->tul_raja[i_pv]);
					break;
				case TAGResultFormula :
					node[*inode].gettext(ln, sizeof(ln)/2-1);
					Sarja->tulkFormula(ln, i_pv, 0);
					break;
				case TAGPointFormula :
					node[*inode].gettext(ln, sizeof(ln)/2-1);
					Sarja->tulkFormula(ln, i_pv, VALUKU+1);
					break;
				case TAGPointRules :
					haara = 3;
					break;
				case TAGEmitToSplits :
					node[*inode].gettext(ln, sizeof(ln)/2-1);
					Sarja->tulkMTB(ln, i_pv);
					break;
				case TAGIntermediaries :
					haara = 2;
					break;
				case TAGBOOrder :
					node[*inode].gettext(Sarja->asuunnlaji[i_pv], sizeof(Sarja->asuunnlaji[0])/2-1);
					break;
				case TAGBOPenalties :
					node[*inode].gettext(ln, sizeof(ln)/2-1);
					for (UINT i = 0; i < sizeof(Sarja->asuunnsakot)/sizeof(Sarja->asuunnsakot[0]); i++) {
						if (i == 0)
							p = wcstok(ln, L";\n");
						else
							p = wcstok(NULL, L";\n");
						if (p == NULL)
							break;
						Sarja->asuunnsakot[i_pv][i] = _wtoi(p);
						}
					break;
				case TAGMultiRace :
					node[*inode].gettext(ln, sizeof(ln)/2-1);
					Sarja->luonne[i_pv] = wcswcind(ln[0], L"SICP") - 1;
					if (Sarja->luonne[i_pv] < -1)
						Sarja->luonne[i_pv] = 0;
					break;
				case TAGRace :
					--*inode;
					return(0);
				}
			continue;
			}
		if (node[*inode].depth == DepthIn+1) {
			haara2 = 0;
			if (haara == 1) {
				switch (node[*inode].tagid) {
					case TAGValue :
						node[*inode].gettext(Sarja->matka[i_pv], sizeof(Sarja->matka[i_pv])/2);
						break;
					case TAGUnit :
	//					node[*inode].gettext(Sarja->matka[i_pv], sizeof(Sarja->matka[i_pv])/2);
						break;
					}
				continue;
				}
			if (haara == 2) {
				switch (node[*inode].tagid) {
					case TAGIntermediary :
						node[*inode].getparam_int(L"Order", &i_va);
						i_va--;
						if (i_va < 0 || i_va >= kilpparam.valuku)
							return(1);
						haara2 = 2;
						break;
					}
				continue;
				}
			if (haara == 3) {
				switch (node[*inode].tagid) {
					case TAGRankBased :
						node[*inode].gettext(ln, sizeof(ln)/2-1);
						Sarja->tulkPointFormula(ln, i_pv, iRule++);
						break;
					case TAGResultBased :
						node[*inode].gettext(ln, sizeof(ln)/2-1);
						Sarja->tulkPointFormula(ln, i_pv, -1);
						break;
					case TAGTotal :
						node[*inode].gettext(ln, sizeof(ln)/2-1);
						Sarja->tulkPointFormula(ln, i_pv, -2);
						break;
					}
				continue;
				}
			continue;
			}
		if (node[*inode].depth == DepthIn+2) {
			if (haara2 == 2) {
				switch (node[*inode].tagid) {
					case TAGDistance :
						node[*inode].gettext(Sarja->va_matka[i_pv][i_va],
							sizeof(Sarja->va_matka[i_pv][i_va])/2);
						if (Sarja->valuku[i_pv] < i_va + 1) {
							Sarja->valuku[i_pv] = i_va + 1;
							}
						break;
					case TAGId :
						node[*inode].gettext_int(&Sarja->va_koodi[i_pv][i_va]);
						break;
					case TAGPenalties :
						node[*inode].gettext_int(&Sarja->va_sakot[i_pv][i_va+1]);
						break;
					case TAGPublic :
						node[*inode].gettext(ln, sizeof(ln)/2-1);
						switch (towupper(ln[0])) {
							case L'N':
								Sarja->va_piilota[i_pv][i_va] = 0;
								break;
							case L'Y':
								Sarja->va_piilota[i_pv][i_va] = 10;
								break;
							default:
								Sarja->va_piilota[i_pv][i_va] = _wtoi(ln+wcslen(ln)-1);
								break;
							}
						break;
					case TAGResultThreshold :
						node[*inode].gettext_int(&Sarja->va_raja[i_pv][i_va]);
						break;
					case TAGResultFormula :
						node[*inode].gettext(ln, sizeof(ln)/2-1);
						Sarja->tulkFormula(ln, i_pv, i_va+1);
						break;
					}
				continue;
				}
			continue;
			}
		}
	return(er);
}

static int loadClassData(xml_node *node, int nnode)
{
	int er = 0, inode, DepthIn, haara = 0, i_srj, rc = 1, i_ss = 0;
	wchar_t buf[100];
	sarjatietue *Sarja;

	haenodeid(node, nnode);
	if (!node[0].getparam_int(L"ClassNo", &i_srj))
		return(1);
	i_srj--;
	if (i_srj < 0 || i_srj >= sarjaluku)
		return(1);
	Sarja = Sarjat+i_srj;
	DepthIn = node[0].depth + 1;
	for (inode = 1; inode <= nnode; inode++) {
		if (node[inode].depth == DepthIn) {
			haara = 0;
			switch (node[inode].tagid) {
				case TAGClassId :
					node[inode].gettext(Sarja->sarjanimi, sizeof(Sarja->sarjanimi)/2);
					break;
				case TAGName :
					node[inode].gettext(Sarja->psarjanimi, sizeof(Sarja->psarjanimi)/2);
					break;
				case TAGIdStart :
					node[inode].gettext_int(&Sarja->sarjaalku);
					break;
				case TAGFee :
					haara = 1;
					break;
				case TAGFirstRace :
					node[inode].gettext_int(&Sarja->enspv);
					Sarja->enspv--;
					break;
				case TAGLastRace :
					node[inode].gettext_int(&Sarja->viimpv);
					Sarja->viimpv--;
					break;
				case TAGResultListOrder :
					node[inode].gettext_int(&Sarja->tuljarj);
					break;
				case TAGHandicaps :
					node[inode].gettext_int(&Sarja->rrtas);
					if (Sarja->rrtas == 5)
						Sarja->piilotatulos = true;
					break;
				case TAGVacantClass :
					node[inode].gettext(buf, 20);
					if (towupper(buf[0]) == L'Y') {
						Sarja->vakanttisarja = true;
						kilpparam.vakanttisarja = i_srj;
						}
					break;
				case TAGRaces :
					haara = 2;
					break;
				case TAGSprintDef :
					haara = 3;
					break;
				}
			continue;
			}
		if (node[inode].depth == DepthIn + 1) {
			if (haara == 1) {
				switch (node[inode].tagid) {
					case TAGFull :
						node[inode].gettext_int(&Sarja->maksu);
						break;
					case TAGSingleRace :
						node[inode].gettext_int(&Sarja->maksu1);
						break;
					case TAGReduced :
						node[inode].gettext_int(&Sarja->maksual);
						break;
					}
				continue;
				}
			if (haara == 2) {
				switch (node[inode].tagid) {
					case TAGRace :
						if ((er = loadClassRaceData(Sarja, node, &inode, nnode)) != 0)
							return(er);
						break;
					}
				continue;
				}
			if (haara == 3) {
				switch (node[inode].tagid) {
					case TAGLastRace :
						node[inode].gettext_int(&Sarja->sprintSrj.vaihelkm);
						break;
					case TAGRace :
						node[inode].getparam_int(L"RaceNo", &rc);
						i_ss = 0;
						break;
					}
				continue;
				}
			continue;
			}
		if (node[inode].depth == DepthIn + 2) {
			if (haara == 3) {
				switch (node[inode].tagid) {
					case TAGHeats :
						node[inode].gettext_int(&Sarja->sprintSrj.eralkm[rc-2]);
						break;
					case TAGHeatSize :
						node[inode].gettext_int(&Sarja->sprintSrj.erakoko[rc-2]);
						break;
					case TAGClassId :
						node[inode].gettext(buf, 20);
						if (i_ss < Sarja->sprintSrj.eralkm[rc-2]) {
							wcsncpy(Sarja->sprintSrj.srjnm[rc-2][i_ss], buf, LSARJA);
							i_ss++;
							}
						break;
					}
				continue;
				}
			continue;
			}
		}
	for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++)
		if (ipv < Sarja->enspv || ipv > Sarja->viimpv)
			Sarja->luonne[ipv] = -1;
	return(er);
}

static int loadCourseControl(IOFCourseVariation *CVar, xml_node *node, int *inode, int nnode)
{
	int er = 0, DepthIn;
	IOFCourseControl CC;

	memset(&CC, 0, sizeof(CC));
	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			switch (node[*inode].tagid) {
				case TAGSequence :
					node[*inode].gettext_int(&CC.iPoint);
					break;
				case TAGControlCode :
					node[*inode].gettext(CC.Code, sizeof(CC.Code)/2-1);
					break;
				case TAGLegLength :
					node[*inode].gettext_int(&CC.LegLength);
					break;
				}
			}
		}
	if (CC.iPoint > 0 && CC.iPoint <= (int)(sizeof(CVar->CourseControl)/sizeof(CVar->CourseControl[0]))) {
		CVar->CourseControl[CC.iPoint-1] = new IOFCourseControl;
		*CVar->CourseControl[CC.iPoint-1] = CC;
		}
	return(er);
}

static int loadCourseVariation(IOFCourse *Cs, int iVar, xml_node *node, int *inode, int nnode)
{
	int er = 0, DepthIn;
	wchar_t ARln[40] = L"";
	IOFCourseVariation CVar;

	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			switch (node[*inode].tagid) {
				case TAGName :
					node[*inode].gettext(CVar.Name, sizeof(CVar.Name)/2-1);
					break;
				case TAGCourseVariationId :
					node[*inode].gettext(CVar.Id, sizeof(CVar.Id)/2-1);
					break;
				case TAGCourseLength :
					node[*inode].gettext_int(&CVar.Length);
					break;
				case TAGCourseClimb :
					node[*inode].gettext_int(&CVar.Climb);
					break;
				case TAGStartPointCode :
					node[*inode].gettext(CVar.Start, sizeof(CVar.Start)/2-1);
					break;
				case TAGFinishPointCode :
					node[*inode].gettext(CVar.Finish, sizeof(CVar.Finish)/2-1);
					break;
				case TAGDistanceToFinish :
					node[*inode].gettext_int(&CVar.FinLeg);
					break;
				case TAGClearECard :
					node[*inode].gettext_int(&CVar.ennakko);
					break;
				case TAGAutoResult :
					node[*inode].gettext(ARln, sizeof(ARln)/2-1);
					break;
				case TAGCourseControl :
					if ((er = loadCourseControl(&CVar, node, inode, nnode)) != 0)
						return(er);
					break;
				}
			}
		}
//	if (CVar.Finish[0])
//		CVar.ajanottotapa = 1;
//	else
//		CVar.ajanottotapa = (ln[0] == L'N' ? 2 : 4);
	CVar.ajanottotapa = wcswcind(ARln[0], L"SN-Y") + 1;
	if (CVar.ajanottotapa == 0)
		CVar.ajanottotapa = 1;
	Cs->CourseVariation[iVar] = new IOFCourseVariation;
	*Cs->CourseVariation[iVar] = CVar;
	return(er);
}

static int loadCourse(IOFCourseData *CseData, int icourse, xml_node *node, int *inode, int nnode)
{
	int er = 0, ic, iVar = 0, DepthIn;
	wchar_t ln[40] = L"";
	IOFCourse Cs;
	IOFCourseVariation CVar;

	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			switch (node[*inode].tagid) {
				case TAGCourseName :
					node[*inode].gettext(Cs.Name, sizeof(Cs.Name)/2-1);
					break;
				case TAGCourseId :
					node[*inode].gettext(Cs.Id, sizeof(Cs.Id)/2-1);
					break;
				case TAGClassShortName :
					for (ic = 0; ic < (int)(sizeof(Cs.ShortName)/sizeof(Cs.ShortName[0])) &&
						Cs.ShortName[ic][0]; ic++) ;
					if (ic < (int)(sizeof(Cs.ShortName)/sizeof(Cs.ShortName[0])))
						node[*inode].gettext(Cs.ShortName[ic], sizeof(Cs.ShortName[0])/2-1);
					break;
				case TAGCourseVariation :
					if ((er = loadCourseVariation(&Cs, iVar, node, inode, nnode)) != 0)
						return(er);
					iVar++;
					break;
				case TAGCourseLength :
					node[*inode].gettext_int(&CVar.Length);
					break;
				case TAGCourseClimb :
					node[*inode].gettext_int(&CVar.Climb);
					break;
				case TAGStartPointCode :
					node[*inode].gettext(CVar.Start, sizeof(CVar.Start)/2-1);
					break;
				case TAGFinishPointCode :
					node[*inode].gettext(CVar.Finish, sizeof(CVar.Finish)/2-1);
					break;
				case TAGDistanceToFinish :
					node[*inode].gettext_int(&CVar.FinLeg);
					break;
				case TAGClearECard :
					node[*inode].gettext_int(&CVar.ennakko);
					break;
				case TAGAutoResult :
					node[*inode].gettext(ln, sizeof(ln)/2-1);
					break;
				case TAGCourseControl :
					if ((er = loadCourseControl(&CVar, node, inode, nnode)) != 0)
						return(er);
					break;
				}
			}
		}
	if (Cs.CourseVariation[0] == NULL) {
		Cs.CourseVariation[0] = new IOFCourseVariation;
		*Cs.CourseVariation[0] = CVar;
		}
	CseData->Course[icourse] = new IOFCourse;
	*CseData->Course[icourse] = Cs;
	return(er);
}

static int loadPointData(IOFCourseData *CseData, int ipoint, xml_node *node, int *inode, int nnode)
{
	int er = 0, DepthIn, haara, iC = 0;
	IOFPoint Pt;

	memset(&Pt, 0, sizeof(Pt));
	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			haara = 0;
			switch (node[*inode].tagid) {
				case TAGStartPointCode :
					Pt.PointType = L'S';
					node[*inode].gettext(Pt.Code, sizeof(Pt.Code)/2-1);
					break;
				case TAGControlCode :
					Pt.PointType = L'C';
					node[*inode].gettext(Pt.Code, sizeof(Pt.Code)/2-1);
					break;
				case TAGFinishPointCode :
					Pt.PointType = L'F';
					node[*inode].gettext(Pt.Code, sizeof(Pt.Code)/2-1);
					break;
				case TAGControlPosition :
					node[*inode].getparam_double(L"y", &Pt.CY);
					node[*inode].getparam_double(L"x", &Pt.CX);
					break;
				case TAGMapPosition :
					node[*inode].getparam_double(L"y", &Pt.MapY);
					node[*inode].getparam_double(L"x", &Pt.MapX);
					break;
				case TAGPunchingUnit :
					haara = 1;
					break;
				}
			}
		if (node[*inode].depth == DepthIn+1) {
			if (haara == 1) {
				switch (node[*inode].tagid) {
					case TAGUnitCode :
						if (iC < (int)(sizeof(Pt.ECodes)/sizeof(Pt.ECodes[0])))
							node[*inode].gettext_int(&Pt.ECodes[iC++]);
						break;
					}
				}
			}
		}
	CseData->Point[ipoint] = new IOFPoint;
	*CseData->Point[ipoint] = Pt;
	return(er);
}

static int loadCourseData(xml_node *node, int nnode, IOFCourseData *CseData)
{
	int er = 0, inode, DepthIn, haara = 0, ipoint = 0, icourse = 0, iassign = -1;
//	IOFCourseData *CseData;

//	CseData = new IOFCourseData;
//	CseData = &CourseData;
	CseData->nollaa();
	haenodeid(node, nnode);
	DepthIn = node[0].depth + 1;
	for (inode = 1; inode <= nnode; inode++) {
		if (node[inode].depth == DepthIn) {
			haara = 0;
			switch (node[inode].tagid) {
				case TAGIOFVersion :
					node[inode].getparam_text(L"version", CseData->IOFVersion, 19);
					break;
				case TAGModifyDate :
					haara = 1;
					break;
				case TAGMap :
					haara = 2;
					break;
				case TAGStartPoint :
				case TAGControl :
				case TAGFinishPoint :
					if ((er = loadPointData(CseData, ipoint, node, &inode, nnode)) != 0)
						return(er);
					ipoint++;
					break;
				case TAGCourse :
					if ((er = loadCourse(CseData, icourse, node, &inode, nnode)) != 0)
						return(er);
					icourse++;
					break;
				case TAGClassCourseAssignment :
					if (iassign < MAXSARJALUKU-1) {
						haara = 3;
						iassign++;
						CseData->ClassAssignment[iassign] = new IOFClassAssignment;
						}
					break;
				}
			continue;
			}
		if (node[inode].depth == DepthIn + 1) {
			if (haara == 1) {
				switch (node[inode].tagid) {
					case TAGDate :
						node[inode].gettext(CseData->ModifyDate, sizeof(CseData->ModifyDate)/2-1);
						break;
					case TAGClock :
						node[inode].gettext(CseData->ModifyTime, sizeof(CseData->ModifyTime)/2-1);
						break;
					}
				continue;
				}
			if (haara == 2) {
				switch (node[inode].tagid) {
					case TAGScale :
						node[inode].gettext_int(&CseData->MapScale);
						break;
					case TAGMapPosition :
						node[inode].getparam_double(L"y", &CseData->MapTY);
						node[inode].getparam_double(L"x", &CseData->MapLX);
						break;
					case TAGMapPositionTopLeft :
						node[inode].getparam_double(L"y", &CseData->MapTY);
						node[inode].getparam_double(L"x", &CseData->MapLX);
						break;
					case TAGMapPositionBottomRight :
						node[inode].getparam_double(L"y", &CseData->MapBY);
						node[inode].getparam_double(L"x", &CseData->MapRX);
						break;
					}
				continue;
				}
			if (haara == 3) {
				switch (node[inode].tagid) {
					case TAGClassName :
						node[inode].gettext(CseData->ClassAssignment[iassign]->Class, LSARJA);
						break;
					case TAGCourseName :
						node[inode].gettext(CseData->ClassAssignment[iassign]->Course, sizeof(CseData->ClassAssignment[0]->Course)/2-1);
						break;
					}
				continue;
				}
			continue;
			}
		}
	return(er);
}

static int loadParticipantRaceData(kilppvtp *Pv, int *ipv, xml_node *node, int *inode, int nnode)
{
	int er = 0, i, j, haara, haara2, DepthIn;
	wchar_t ln[100];

	if (!node[*inode].getparam_int(L"RaceNo", ipv))
		return(1);
	Pv->nollaa();
	--*ipv;
	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			return(0);
			}
		if (node[*inode].depth == DepthIn) {
			haara = 0;
			switch (node[*inode].tagid) {
				case TAGBib :
					node[*inode].gettext_int(&i);
					Pv->bib = i;
					break;
				case TAGBadge :
					node[*inode].gettext_int(&i);
					if (Pv->badge[0] == 0)
						Pv->badge[0] = i;
					else if (Pv->badge[1] == 0)
						Pv->badge[1] = i;
					break;
				case TAGClassId :
					node[*inode].gettext(ln, sizeof(ln)/2);
					Pv->sarja = haesarja_w(ln, false);
					break;
				case TAGCourse :
					node[*inode].gettext(Pv->rata, sizeof(Pv->rata)/2);
					break;
				case TAGStartTime :
					if (node[*inode].gettext(ln, sizeof(ln)/2)) {
						Pv->tlahto = wstrtoaika_vap(ln, t0);
						Pv->va[0].vatulos = Pv->tlahto;
						}
					break;
				case TAGStartGateTime :
					if (node[*inode].gettext(ln, sizeof(ln)/2))
						Pv->va[0].vatulos = wstrtoaika_vap(ln, t0);
					break;
				case TAGResult :
					if (node[*inode].gettext(ln, sizeof(ln)/2))
						Pv->va[1].vatulos = wstrtoaika_vap(ln, 0);
					break;
				case TAGPersonalBest :
					if (node[*inode].gettext(ln, sizeof(ln)/2))
						Pv->enn = wstrtoaika_vap(ln, 0);
					break;
				case TAGGoal :
					if (node[*inode].gettext(ln, sizeof(ln)/2))
						Pv->tav = wstrtoaika_vap(ln, 0);
					break;
				case TAGRank :
					break;
				case TAGStatus :
					if (node[*inode].gettext(ln, sizeof(ln)/2))
						Pv->keskhyl = haetunnus(ln, Stat, StatSt,
							sizeof(StatSt)/sizeof(StatSt[0]));
					break;
				case TAGPenalty :
					haara = 1;
					break;
				case TAGPoints :
					if (node[*inode].getparam_int(L"Order", &j)) {
						if (j > 0 && j < (int)(sizeof(Pv->pvpisteet)/sizeof(Pv->pvpisteet[0]))) {
							node[*inode].gettext_int(&i);
							Pv->pvpisteet[j-1];
							}
						}
					break;
				case TAGIntermediaries :
					haara = 2;
					break;
				}
			}
		if (node[*inode].depth == DepthIn+1) {
			haara2 = 0;
			if (haara == 1) {
				switch (node[*inode].tagid) {
					case TAGBiathlon :
						if (node[*inode].gettext(Pv->asakot, sizeof(Pv->asakot)/2))
						break;
					case TAGSecs :
						node[*inode].gettext_int(&i);
						Pv->tasoitus = i;
						break;
					}
				}
			if (haara == 2) {
				switch (node[*inode].tagid) {
					case TAGIntermediary :
						if (node[*inode].getparam_int(L"Order", &j)) {
						haara2 = 10+j;
						break;
					}
				}
			}
		}
		if (node[*inode].depth == DepthIn+2) {
			if (haara2 > 10 && haara2 <= kilpparam.valuku+10) {
				switch (node[*inode].tagid) {
					case TAGResult :
						if (node[*inode].gettext(ln, sizeof(ln)/2))
							Pv->va[haara2-9].vatulos = wstrtoaika_vap(ln, 0);
						break;
					case TAGRank :
						break;
					}
				}
			}
		}
	return(er);
}

static int loadParticipantData(xml_node *node, int nnode)
{
	int er = 0, inode, pos, i, j, haara, DepthIn;
	wchar_t ln[100];
	kilptietue Kilp;

	haenodeid(node, nnode);
	if (!node[0].getparam_int(L"Key", &pos))
		return(1);
	DepthIn = node[0].depth + 1;
	Kilp.nollaa();
	for (int i = 0; i < kilpparam.n_pv_akt; i++)
		Kilp.set_tark(L'P', i);
	for (inode = 1; inode <= nnode; inode++) {
		if (node[inode].depth < DepthIn) {
			return(0);
			}
		if (node[inode].depth == DepthIn) {
			haara = 0;
			switch (node[inode].tagid) {
				case TAGDataStatus :
					node[inode].gettext_int(&i);
					Kilp.kilpstatus = i;
					break;
				case TAGId :
					node[inode].gettext_int(&i);
					Kilp.kilpno = i;
					break;
				case TAGPersonId :
					node[inode].gettext_int(&i);
					Kilp.lisno[0] = i;
					break;
				case TAGPersonIdInt :
					node[inode].gettext_int(&i);
					Kilp.lisno[1] = i;
					break;
				case TAGPersonIdTxt :
					node[inode].gettext(Kilp.wrkoodi, sizeof(Kilp.wrkoodi)/2);
					break;
				case TAGClassId :
					node[inode].gettext(ln, sizeof(ln)/2);
					Kilp.sarja = haesarja_w(ln, false);
					if (Kilp.sarja < 0)
						Kilp.sarja = 0;
					break;
				case TAGName :
					haara = 1;
					break;
				case TAGTitle :
					node[inode].gettext(Kilp.arvo, kilpparam.larvo+1);
					break;
				case TAGClub :
					node[inode].gettext(Kilp.seura, kilpparam.lseura+1);
					break;
				case TAGClubCode :
					node[inode].gettext(Kilp.seuralyh, kilpparam.lseuralyh+1);
					break;
				case TAGCountry :
					node[inode].gettext(Kilp.maa, 4);
					break;
				case TAGDistrict :
					node[inode].gettext_int(&i);
					Kilp.piiri = i;
					break;
				case TAGOrganization :
					node[inode].gettext(Kilp.yhdistys, kilpparam.lyhdistys+1);
					break;
				case TAGTeam :
					node[inode].gettext(Kilp.joukkue, kilpparam.ljoukkue+1);
					break;
				case TAGPoints :
					if (node[inode].getparam_int(L"Order", &j)) {
						if (j > 0 && j < (int)(sizeof(Kilp.pisteet)/sizeof(Kilp.pisteet[0]))) {
							node[inode].gettext_int(&i);
							Kilp.pisteet[j-1];
							}
						}
					break;
				case TAGResult :
				case TAGRank :
				case TAGStatus :
					break;
				case TAGRaces :
					haara = 2;
					break;
				}
			continue;
			}
		if (node[inode].depth == DepthIn + 1) {
			if (haara == 1) {
				switch (node[inode].tagid) {
					case TAGFamily :
						node[inode].gettext(Kilp.sukunimi, kilpparam.lsnimi+1);
						break;
					case TAGGiven :
						node[inode].gettext(Kilp.etunimi, kilpparam.lenimi+1);
						break;
					}
				continue;
				}
			if (haara == 2) {
				switch (node[inode].tagid) {
					case TAGRace :
						kilppvtp Pv;
						int ipv;

						if ((er = loadParticipantRaceData(&Pv, &ipv, node, &inode, nnode)) != 0)
							return(er);
						if (!Pv.bib)
							Pv.bib = Kilp.kilpno;
						Kilp.SiirraPvEhdoin(&Pv, ipv);
						break;
					}
				continue;
				}
			continue;
			}
		}
	if (!er) {
		EnterCriticalSection(&tall_CriticalSection);
		Kilp.TallEhdoin(pos);
		LeaveCriticalSection(&tall_CriticalSection);
		}
	return(er);
}

int lueVainRadatXml(wchar_t *filename, IOFCourseData *CseData)
{
	TextFl *infile;
	int ntag = 100000, haara = 0, inode, inode_in, depth = 0, rv, er = 0, lntype;
	short k[MAXXMLDEPTH];
	wchar_t line[1000], ln[100], *txtbuf[MAXXMLDEPTH];
	wchar_t msg[200] = L"", tag[MAXXMLTAGLEN+1];
	xml_tree tree;
	xml_node nd;
	bool IOFVersion30 = false;

	memset(k, 0, sizeof(k));
	infile = new TextFl(filename, L"rt");
	if (infile->IsOpen()) {
		for (int i = 0; i < MAXXMLDEPTH; i++) {
			txtbuf[i] = new wchar_t[XMLTXTBUFLEN+1];
			memset(txtbuf[i], 0, 2*(XMLTXTBUFLEN+1));
			}
		tree.node = new xml_node[ntag];
		tree.maxnode = ntag;
		inode = -1;
		rv = 0;
		while (!er && !infile->Feof()) {
			if (infile->ReadLine(line, 999) == NULL)
				break;
			rv++;
			elimwbl2(line);
			lntype = nd.interpretXmlLine(line, tag);
			if (lntype == -5)
				continue;
			nd.depth = depth;
			for (int i = 0; i <= depth; i++)
				nd.nodeno[i] = k[i];           // merkit‰‰n eri tasojen j‰rjestysnumerot solmuun
			if (lntype > 0)
				nd.tagid = hae_tagid(nd.tag);
			if (lntype == 1) {
				tree.node[++inode] = nd;
				k[depth++]++;
				if (depth == 1) {
					inode_in = inode;
					switch (nd.tagid) {
						case TAGCourseData :
							haara = 0;
							tree.node[0].getparam_text(L"iofVersion", ln, sizeof(ln)/2);
							if (ln[0] == L'3') {
								IOFVersion30 = true;
								break;
								}
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er)
								loadCourseData(tree.node+inode_in, inode-inode_in, CseData);
							inode = inode_in-1;
							break;
						}
					if (IOFVersion30)
						break;
					continue;
					}
				if (depth == 2) {
					inode_in = inode;
					switch (nd.tagid) {
						case TAGCourseData :
							haara = 0;
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er)
								loadCourseData(tree.node+inode_in, inode-inode_in, CseData);
							inode = inode_in-1;
							break;
						default:
							haara = 0;
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							break;
						}
					continue;
					}
				if (depth == 3) {
					inode_in = inode;
					er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
					--depth;
					continue;
					}
				if (depth > 3) {
					er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
					--depth;
					}
				}
			else if (lntype == 8) {                  // sulkeva tag uudella rivill‰
				int jnode;
				depth--;
				k[depth]++;                 // kasvatetaan tason laskuria sulkev‰n tagin tasolla
				// Etsit‰‰n vastaava avaava tag aiemmasta ketjusta
				for (jnode = inode; jnode > 0 && tree.node[jnode].depth > depth; jnode--) ;
				if (wcscmp(tag, tree.node[jnode].tag)) {      // Varmistetaan, ett‰ tagit ovat pari
					er = 1;
					swprintf(msg, L"XML-tiedoston tulkinta ei onnnistu. Rivi %d, Tag: '%.30s', odotettu: '%.30s'",
						rv, tag, tree.node[jnode].tag);
					break;
					}
				if (wcslen(txtbuf[depth])) {    // Tallennetaan tason mahdollinen teksti solmuun
					tree.node[jnode].text = new wchar_t[wcslen(txtbuf[depth])+1];
					wcscpy(tree.node[jnode].text, txtbuf[depth]);
					}
				if (depth == 0) {
					break;
					}
				continue;
				}
			}      // while(!infile->Feof())
		delete[] tree.node;
		tree.node = 0;
		for (int i=0; i < MAXXMLDEPTH; i++)
			delete[] txtbuf[i];
		}  		  // infile->Open()
	else
		return(-1);
	delete infile;
	if (IOFVersion30) {
		return(lueIOF30VainRadatXml(filename, CseData));
		}
	if (depth) {
		er = 1;
		swprintf(msg, L"XML-tiedosto %s p‰‰ttyi rivill‰ %d tasolla %d", filename, rv, depth);
		}
	if (er) {
		if (!msg[0]) {
			swprintf(msg, L"Tiedostossa %s virhe rivill‰ %d tai aikaisemmin", filename, rv);
			}
		writeerror_w(msg, 0);
		}
	tree.nodecount = inode+1;
	return(er);
}

int lueEventXml(wchar_t *filename, bool lueSarjat, bool lueRadat, bool lueOsanottajat)
{
	TextFl *infile;
	int ntag = 100000, haara = 0, inode, inode_in, depth = 0, rv, er = 0, lntype;
	short k[MAXXMLDEPTH];
	wchar_t line[1000], *txtbuf[MAXXMLDEPTH];
	wchar_t msg[200] = L"", tag[MAXXMLTAGLEN+1];
	xml_tree tree;
	xml_node nd;

	memset(k, 0, sizeof(k));
	infile = new TextFl(filename, L"rt");
	if (infile->IsOpen()) {
		for (int i = 0; i < MAXXMLDEPTH; i++) {
			txtbuf[i] = new wchar_t[XMLTXTBUFLEN+1];
			memset(txtbuf[i], 0, 2*(XMLTXTBUFLEN+1));
			}
		tree.node = new xml_node[ntag];
		tree.maxnode = ntag;
		inode = -1;
		rv = 0;
		while (!er && !infile->Feof()) {
			if (infile->ReadLine(line, 999) == NULL)
				break;
			rv++;
			elimwbl2(line);
			lntype = nd.interpretXmlLine(line, tag);
			if (lntype == -5)
				continue;
			nd.depth = depth;
			for (int i = 0; i <= depth; i++)
				nd.nodeno[i] = k[i];           // merkit‰‰n eri tasojen j‰rjestysnumerot solmuun
			if (lntype > 0)
				nd.tagid = hae_tagid(nd.tag);
			if (lntype == 1) {
				tree.node[++inode] = nd;
				k[depth++]++;
				if (depth == 1) {
					inode_in = inode;
					switch (nd.tagid) {
						case TAGCourseData :
							haara = 0;
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er && lueRadat)
								loadCourseData(tree.node+inode_in, inode-inode_in, &CourseData);
							inode = inode_in-1;
							break;
						}
					continue;
					}
				if (depth == 2) {
					inode_in = inode;
					switch (nd.tagid) {
						case TAGSoftware :
							haara = 0;
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er)
								er = loadSoftwareData(tree.node+inode_in, inode-inode_in);
							inode = inode_in-1;
							break;
						case TAGEventData :
							haara = 0;
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er && lueSarjat)
								er = loadEventData(tree.node+inode_in, inode-inode_in);
							inode = inode_in-1;
							break;
						case TAGRaces :
							haara = 1;
							break;
						case TAGClasses :
							haara = 2;
							break;
						case TAGCourseData :
							haara = 0;
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er && lueRadat)
								loadCourseData(tree.node+inode_in, inode-inode_in, &CourseData);
							inode = inode_in-1;
							break;
						case TAGParticipants :
							haara = 3;
							break;
						default:
							haara = 0;
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							break;
						}
					continue;
					}
				if (depth == 3) {
					inode_in = inode;
					switch (nd.tagid) {
						case TAGRace :
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er && lueSarjat && haara == 1) {
								er = loadRaceData(tree.node+inode_in, inode-inode_in);
								}
							inode = inode_in-1;
							break;
						case TAGClass :
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er && lueSarjat && haara == 2) {
								er = loadClassData(tree.node+inode_in, inode-inode_in);
								}
							inode = inode_in-1;
							break;
						case TAGParticipant :
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er && lueOsanottajat && haara == 3) {
								er = loadParticipantData(tree.node+inode_in, inode-inode_in);
								}
							inode = inode_in-1;
							break;
						default:
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							break;
						}
					continue;
					}
				if (depth > 3) {
					er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
					--depth;
					}
				}
			else if (lntype == 8) {                  // sulkeva tag uudella rivill‰
				int jnode;
				depth--;
				k[depth]++;                 // kasvatetaan tason laskuria sulkev‰n tagin tasolla
				// Etsit‰‰n vastaava avaava tag aiemmasta ketjusta
				for (jnode = inode; jnode > 0 && tree.node[jnode].depth > depth; jnode--) ;
				if (wcscmp(tag, tree.node[jnode].tag)) {      // Varmistetaan, ett‰ tagit ovat pari
					er = 1;
					swprintf(msg, L"XML-tiedoston tulkinta ei onnnistu. Rivi %d, Tag: '%.30s', odotettu: '%.30s'",
						rv, tag, tree.node[jnode].tag);
					break;
					}
				if (wcslen(txtbuf[depth])) {    // Tallennetaan tason mahdollinen teksti solmuun
					tree.node[jnode].text = new wchar_t[wcslen(txtbuf[depth])+1];
					wcscpy(tree.node[jnode].text, txtbuf[depth]);
					}
				if (depth == 0) {
					break;
					}
				continue;
				}
			}      // while(!infile->Feof())
		delete[] tree.node;
		tree.node = 0;
		for (int i=0; i < MAXXMLDEPTH; i++)
			delete[] txtbuf[i];
		}  		  // infile->Open()
	else
		return(-1);
	delete infile;
	if (depth) {
		er = 1;
		swprintf(msg, L"XML-tiedosto %s p‰‰ttyi rivill‰ %d tasolla %d", filename, rv, depth);
		}
	if (er) {
		if (!msg[0]) {
			swprintf(msg, L"Tiedostossa virhe rivill‰ %d tai aikaisemmin", filename, rv);
			}
		writeerror_w(msg, 0);
		}
	tree.nodecount = inode+1;
	if (lueSarjat) {
		for (int srj = 0; srj < sarjaluku; srj++) {
			if (Sarjat[srj].sprintSrj.vaihelkm > 1) {
				for (int i = 0; i < Sarjat[srj].sprintSrj.vaihelkm - 1; i++) {
					for (int j = 0; j < Sarjat[srj].sprintSrj.eralkm[i]; j++) {
						int ss;
						if ((ss = haesarja_w(Sarjat[srj].sprintSrj.srjnm[i][j], false)) >= 0)
							Sarjat[srj].sprintSrj.sarjat[i][j] = ss;
						}
					}
				}
			}
		}
	return(er);
}

void kirjXmlSoftware(TextFl *outfl, int level)
{
	outfl->put_wtag(srjtag[TAGSoftware], level++);
	outfl->put_wxml_s(srjtag[TAGVersionId], kilpparam.tiedlaji, level);
	outfl->put_wtag(srjtag[TAGFileFormat], level++);
	outfl->put_wxml_d(srjtag[TAGRaces], kilpparam.n_pv, level);
	outfl->put_wxml_d(srjtag[TAGIntermediaries], kilpparam.valuku, level);
	outfl->put_wantitag(srjtag[TAGFileFormat], --level);
	outfl->put_wantitag(srjtag[TAGSoftware], --level);
}

void kirjXmlEventData(TextFl *outfl, int level)
{
	wchar_t ln[400];
	int i;

	outfl->put_wtag(srjtag[TAGEventData], level++);
	outfl->put_wxml_s(srjtag[TAGEventCode], kilpparam.kilpkoodi, level);
	outfl->put_wtag(srjtag[TAGTitle], level++);
	swprintf(ln, L"%s%s\n", Tabs[level], kilpailu);
	outfl->WriteLine(ln);
	outfl->put_wantitag(srjtag[TAGTitle], --level);
	if ((i = wcswcind(kilpparam.kilplaji, Dis)) >= 0) {
		outfl->put_wtag(srjtag[TAGSport], level++);
		outfl->put_wxml_s(srjtag[TAGDiscipline], DisSt[i], level);
		if ((i = wcswcind(kilpparam.alalaji, Sub)) >= 0) {
			outfl->put_wxml_s(srjtag[TAGSubtype], SubSt[i], level);
			}
		outfl->put_wantitag(srjtag[TAGSport], --level);
		}
	outfl->put_wxml_d(srjtag[TAGYear], kilpparam.vuosi, level);
	outfl->put_wxml_d(srjtag[TAGRaceCount], kilpparam.n_pv_akt, level);
	outfl->put_wxml_d(srjtag[TAGClassCount], sarjaluku, level);
	outfl->put_wxml_s(srjtag[TAGVariableClass], kilpparam.sarjavaihtelee ? L"Yes" : L"No", level);
	outfl->put_wxml_s(srjtag[TAGVariableBib], kilpparam.bibvaihtelee ? L"Yes" : L"No", level);
	outfl->put_wxml_d(srjtag[TAGIntIdLen], kilpparam.kvlisno, level);
	outfl->put_wxml_s(srjtag[TAGUseClubId], kilpparam.seuralyh_on ? L"Yes" : L"No", level);
	if ((i = wcswcind(kilpparam.badgelaji, Bdg)) >= 0) {
		wcscpy(ln, BdgSt[i]);
		if (kilpparam.kaksibadge == 1)
			wcscat(ln, L"*2");
		outfl->put_wxml_s(srjtag[TAGBadge], ln, level);
		}
	if ((i = wcswcind(kilpparam.yhttlslaji, YhtTls)) >= 0) {
		outfl->put_wxml_s(srjtag[TAGCombinedResult], YhtTlsSt[i], level);
		}
	outfl->put_wantitag(srjtag[TAGEventData], --level);
}

void kirjXmlRaceData(TextFl *outfl, int level)
{
	wchar_t ln[400];
	int i;

	outfl->put_wtag(srjtag[TAGRaces], level++);
	for (int pv = 0; pv < kilpparam.n_pv_akt; pv++) {
		swprintf(ln, L"%s RaceNo=\"%d\"", srjtag[TAGRace], pv+1);
		outfl->put_wtag(ln, level++);
		if (kirjXMLkaikki || pvparam[pv].RaceId[0])
			outfl->put_wxml_s(srjtag[TAGEventRaceId], pvparam[pv].RaceId, level);
		if (kirjXMLkaikki || pvparam[pv].Title[0])
			outfl->put_wxml_s(srjtag[TAGTitle], pvparam[pv].Title, level);
		if (kirjXMLkaikki || pvparam[pv].Date)
			outfl->put_wxml_s(srjtag[TAGDate], stDateNo(NULL, pvparam[pv].Date), level);
		if ((i = wcswcind(pvparam[pv].kilplaji, Dis)) >= 0) {
			outfl->put_wxml_s(srjtag[TAGDiscipline], DisSt[i], level);
			}
		if ((i = wcswcind(pvparam[pv].alalaji, Sub)) >= 0) {
			outfl->put_wxml_s(srjtag[TAGSubtype], SubSt[i], level);
			}
		outfl->put_wxml_s(srjtag[TAGStartGate], pvparam[pv].hiihtolahto ? L"Yes" : L"No", level);
//		outfl->put_wxml_s(srjtag[TAGPursuitExtra], pvparam[pv].takaaAjoLisa ? L"Yes" : L"No", level);
		outfl->put_wtag(srjtag[TAGPrecision], level++);
		outfl->put_wxml_s(srjtag[TAGInternal], Pr[PyorIx(pvparam[pv].pyor[1])], level);
		outfl->put_wxml_s(srjtag[TAGRank], Pr[PyorIx(pvparam[pv].pyor[2])], level);
		outfl->put_wxml_s(srjtag[TAGResults], Pr[PyorIx(pvparam[pv].pyor[3])], level);
		outfl->put_wxml_s(srjtag[TAGPursuit], Pr[PyorIx(pvparam[pv].pyor[4])], level);
		outfl->put_wxml_s(srjtag[TAGIntermediary], Pr[PyorIx(pvparam[pv].pyor[5])], level);
		outfl->put_wantitag(srjtag[TAGPrecision], --level);
		outfl->put_wantitag(srjtag[TAGRace], --level);
		}
	outfl->put_wantitag(srjtag[TAGRaces], --level);
}

void kirjXmlClassRaces(TextFl *outfl, sarjatietue *Sarja, int level)
{
	wchar_t ln[400];
	wchar_t *Luonne[4] = {L"Skip", L"Independent", L"Continuation", L"Pursuit"};

	outfl->put_wtag(srjtag[TAGRaces], level++);
	for (int pv = Sarja->enspv; pv <= Sarja->viimpv; pv++) {
		swprintf(ln, L"%s RaceNo=\"%d\"", srjtag[TAGRace], pv+1);
		outfl->put_wtag(ln, level++);
		if (kirjXMLkaikki || Sarja->pvsarjanimi[pv][0])
			outfl->put_wxml_s(srjtag[TAGName], Sarja->pvsarjanimi[pv], level);
		if (kirjXMLkaikki || Sarja->flags[pv][0])
			outfl->put_wxml_s(srjtag[TAGFlags], Sarja->flags[pv], level);
		outfl->put_wxml_d(srjtag[TAGBibStart], Sarja->bibalku[pv], level);
		if (kirjXMLkaikki || Sarja->tapa[pv][0]) {
			wcsncpy(ln, Sarja->tapa[pv], 3);
			wtransl(L"VP", L"FC", ln);
			wtransl(L"VP", L"FC", ln+1);
			ln[2] = 0;
			outfl->put_wxml_s(srjtag[TAGTechnique], ln, level);
			}
		outfl->put_wtag(srjtag[TAGDistance], level++);
		outfl->put_wxml_s(srjtag[TAGValue], Sarja->matka[pv], level);
		outfl->put_wantitag(srjtag[TAGDistance], --level);
		outfl->put_wxml_d(srjtag[TAGStartGate], Sarja->lno[pv], level);
		outfl->put_wxml_d(srjtag[TAGStartQueue], Sarja->portti[pv], level);
		outfl->put_wxml_d(srjtag[TAGFinishGate], Sarja->mno[pv], level);
		outfl->put_wxml_time(srjtag[TAGFirstStart], 0, Sarja->enslahto[pv], t0, SEK, 8, 0, level);
		outfl->put_wxml_time(srjtag[TAGFirstExtraStart], 0, Sarja->ensjlahto[pv], t0, SEK, 8, 0, level);
		outfl->put_wxml_d(srjtag[TAGStartInterval], Sarja->lahtovali[pv], level);
		outfl->put_wxml_d(srjtag[TAGStartWave], Sarja->pari[pv], level);
		outfl->put_wxml_d(srjtag[TAGPenaltyCount], Sarja->lsak[pv], level);
		outfl->put_wxml_d(srjtag[TAGPenaltyUnit], Sarja->tsak[pv], level);
		outfl->put_wxml_d(srjtag[TAGPenaltyMax], kilpparam.maxasakko, level);
		outfl->put_wxml_d(srjtag[TAGResultThreshold], Sarja->tul_raja[pv], level);
		if (kilpparam.kilplaji == L'B') {
			outfl->put_wxml_s(srjtag[TAGBOOrder], Sarja->asuunnlaji[pv], level);
			ln[0] = 0;
			for (int i = 0; i < (int) wcslen(Sarja->asuunnlaji[pv]); i++) {
				if (i > 0)
					wcscat(ln, L";");
				_itow(Sarja->asuunnsakot[pv][i], ln+wcslen(ln), 10);
				}
			outfl->put_wxml_s(srjtag[TAGBOPenalties], ln, level);
			}
		outfl->put_wxml_s(srjtag[TAGMultiRace], Luonne[Sarja->luonne[pv]+1], level);
		if (kirjXMLkaikki || Sarja->kaava[pv][0].on) {
			outfl->put_wxml_s(srjtag[TAGResultFormula], Sarja->writeFormula(ln, pv, 0), level);
			}
		if (kirjXMLkaikki || Sarja->kaava[pv][VALUKU+1].on) {
			outfl->put_wxml_s(srjtag[TAGPointFormula], Sarja->writeFormula(ln, pv, VALUKU+1), level);
			}
		if (kirjXMLkaikki || Sarja->pistekaava[pv].laji > 0 || Sarja->pistekaava[pv].yhtpistvaihelkm) {
			outfl->put_wtag(srjtag[TAGPointRules], level++);
			if (kirjXMLkaikki || (Sarja->pistekaava[pv].laji & 1)) {
				for (int iRule = 0; iRule < 5; iRule++) {
					if ((kirjXMLkaikki && iRule == 0) || Sarja->pistekaava[pv].a[iRule] != 0) {
						outfl->put_wxml_s(srjtag[TAGRankBased], Sarja->writePointFormula(ln, pv, iRule), level);
						}
					}
				}
			if (kirjXMLkaikki || (Sarja->pistekaava[pv].laji & 2)) {
				outfl->put_wxml_s(srjtag[TAGResultBased], Sarja->writePointFormula(ln, pv, -1), level);
				}
			if (kirjXMLkaikki || Sarja->pistekaava[pv].yhtpistvaihelkm) {
				outfl->put_wxml_s(srjtag[TAGTotal], Sarja->writePointFormula(ln, pv, -2), level);
				}
			outfl->put_wantitag(srjtag[TAGPointRules], --level);
			}
		outfl->put_wtag(srjtag[TAGIntermediaries], level++);
		for (int j = 0; j < Sarja->valuku[pv]; j++) {
			swprintf(ln, L"%s Order=\"%d\"", srjtag[TAGIntermediary], j+1);
			outfl->put_wtag(ln, level++);
			if (kirjXMLkaikki || Sarja->va_matka[pv][j][0])
				outfl->put_wxml_s(srjtag[TAGDistance], Sarja->va_matka[pv][j], level);
			if (kirjXMLkaikki || Sarja->va_koodi[pv][j])
				outfl->put_wxml_d(srjtag[TAGId], Sarja->va_koodi[pv][j], level);
			if (kirjXMLkaikki || wcswcind(kilpparam.kilplaji, L"AB") >= 0)
				outfl->put_wxml_d(srjtag[TAGPenalties], Sarja->va_sakot[pv][j+1], level);
			if (Sarja->va_piilota[pv][j] == 0)
				outfl->put_wxml_s(srjtag[TAGPublic], L"No", level);
			else if (Sarja->va_piilota[pv][j] >= 9)
				outfl->put_wxml_s(srjtag[TAGPublic], L"Yes", level);
			else {
				swprintf(ln, L"Level%d", Sarja->va_piilota[pv][j]);
				outfl->put_wxml_s(srjtag[TAGPublic], ln, level);
				}
			if (kirjXMLkaikki || Sarja->va_raja[pv][j])
				outfl->put_wxml_d(srjtag[TAGResultThreshold], Sarja->va_raja[pv][j], level);
			if (kirjXMLkaikki || Sarja->kaava[pv][j+1].on) {
				outfl->put_wxml_s(srjtag[TAGResultFormula], Sarja->writeFormula(ln, pv, j+1), level);
				}
			outfl->put_wantitag(srjtag[TAGIntermediary], --level);
			}
		outfl->put_wantitag(srjtag[TAGIntermediaries], --level);
		if (kirjXMLkaikki || Sarja->mtb[pv][0].alkukdi || Sarja->mtb[pv][0].loppukdi) {
			swprintf(ln, L"%d", Sarja->laskemtb[pv] ? 1 : 0);
			for (int i = 0; i <= Sarja->valuku[pv]; i++) {
				if (Sarja->mtb[pv][i].alkukdi == 0 && Sarja->mtb[pv][i].loppukdi == 0)
					break;
				swprintf(ln+wcslen(ln), L" %d %d %d %d", Sarja->mtb[pv][i].alkukdi,
					Sarja->mtb[pv][i].loppukdi, Sarja->mtb[pv][i].piste, Sarja->mtb[pv][i].pohja);
				}
			outfl->put_wxml_s(srjtag[TAGEmitToSplits], ln, level);
			}
		outfl->put_wantitag(srjtag[TAGRace], --level);
		}
	outfl->put_wantitag(srjtag[TAGRaces], --level);
}

void kirjXmlClasses(TextFl *outfl, int level)
{
	wchar_t ln[400];

	outfl->put_wtag(srjtag[TAGClasses], level++);
	for (int i = 0 ; i < sarjaluku; i++) {
		swprintf(ln, L"%s ClassNo=\"%d\"", srjtag[TAGClass], i+1);
		outfl->put_wtag(ln, level++);
//			outfl->put_wtag(srjtag[TAGClass], level++);
//			outfl->put_wxml_d(srjtag[TAGClassNo], i+1, level);
		outfl->put_wxml_s(srjtag[TAGClassId], Sarjat[i].sarjanimi, level);
		if (kirjXMLkaikki || Sarjat[i].psarjanimi[0])
			outfl->put_wxml_s(srjtag[TAGName], Sarjat[i].psarjanimi, level);
		outfl->put_wxml_d(srjtag[TAGIdStart], Sarjat[i].sarjaalku, level);
		outfl->put_wtag(srjtag[TAGFee], level++);
		outfl->put_wxml_d(srjtag[TAGFull], Sarjat[i].maksu, level);
		outfl->put_wxml_d(srjtag[TAGSingleRace], Sarjat[i].maksu1, level);
		outfl->put_wxml_d(srjtag[TAGReduced], Sarjat[i].maksual, level);
		outfl->put_wantitag(srjtag[TAGFee], --level);
		outfl->put_wxml_d(srjtag[TAGFirstRace], Sarjat[i].enspv+1, level);
		outfl->put_wxml_d(srjtag[TAGLastRace], Sarjat[i].viimpv+1, level);
		outfl->put_wxml_d(srjtag[TAGResultListOrder], Sarjat[i].tuljarj, level);
		outfl->put_wxml_d(srjtag[TAGHandicaps], Sarjat[i].rrtas, level);
		if (Sarjat[i].vakanttisarja)
			outfl->put_wxml_s(srjtag[TAGVacantClass], L"Yes", level);
		else if (kirjXMLkaikki)
			outfl->put_wxml_s(srjtag[TAGVacantClass], L"No", level);
		if (Sarjat[i].sprintSrj.vaihelkm > 1) {
			outfl->put_wtag(srjtag[TAGSprintDef], level++);
			outfl->put_wxml_d(srjtag[TAGLastRace], Sarjat[i].sprintSrj.vaihelkm, level);
			for (int j = 1; j < Sarjat[i].sprintSrj.vaihelkm; j++) {
				swprintf(ln, L"%s RaceNo=\"%d\"", srjtag[TAGRace], j+1);
				outfl->put_wtag(ln, level++);
				outfl->put_wxml_d(srjtag[TAGHeats], Sarjat[i].sprintSrj.eralkm[j-1], level);
				outfl->put_wxml_d(srjtag[TAGHeatSize], Sarjat[i].sprintSrj.erakoko[j-1], level);
				for (int srj = 0; srj < Sarjat[i].sprintSrj.eralkm[j-1]; srj++)
					outfl->put_wxml_s(srjtag[TAGClassId],
						Sarjat[Sarjat[i].sprintSrj.sarjat[j-1][srj]].sarjanimi, level);
				outfl->put_wantitag(srjtag[TAGRace], --level);
				}
			outfl->put_wantitag(srjtag[TAGSprintDef], --level);
			}
// Races within classes
		kirjXmlClassRaces(outfl, Sarjat+i, level);
		outfl->put_wantitag(srjtag[TAGClass], --level);
		}
	outfl->put_wantitag(srjtag[TAGClasses], --level);
}

void kirjXmlRadat(TextFl *outfl, int level, IOFCourseData *CseData)
{
	wchar_t ln[400], ln2[100];
	wchar_t *aotapa[5] = {L"",L"Separate", L"No", L"No", L"Yes"};

	if (level == 0) {
		swprintf(ln, L"%s createTime=\"%s\" creator=\"HkKisaWin %s\"", srjtag[TAGCourseData], ISOdatetime(0,1), Build());
		outfl->put_wtag(ln, level++);
		}
	else
		outfl->put_wtag(srjtag[TAGCourseData], level++);
	swprintf(ln, L"version=\"%s\"", CseData->IOFVersion);
	outfl->put_wtagparams(srjtag[TAGIOFVersion], ln, true, level);
	if (CseData->ModifyDate[0]) {
		outfl->put_wtag(srjtag[TAGModifyDate], level++);
		outfl->put_wxml_s(srjtag[TAGDate], CseData->ModifyDate, level);
		if (CseData->ModifyDate[0])
			outfl->put_wxml_s(srjtag[TAGClock], CseData->ModifyTime, level);
		outfl->put_wantitag(srjtag[TAGModifyDate], --level);
		}
	if (CseData->MapScale || CseData->MapTY != 0 || CseData->MapLX != 0 || CseData->MapBY != 0 || CseData->MapRX != 0) {
		outfl->put_wtag(srjtag[TAGMap], level++);
		if (CseData->MapScale)
			outfl->put_wxml_d(srjtag[TAGScale], CseData->MapScale, level);
		if (CseData->MapTY != 0 || CseData->MapLX != 0) {
			swprintf(ln, L"x=\"%.1f\" y=\"%.1f\"", CseData->MapLX, CseData->MapTY);
			outfl->put_wtagparams(srjtag[TAGMapPositionTopLeft], ln, true, level);
			}
		if (CseData->MapBY != 0 || CseData->MapRX != 0) {
			swprintf(ln, L"x=\"%.1f\" y=\"%.1f\"", CseData->MapRX, CseData->MapBY);
			outfl->put_wtagparams(srjtag[TAGMapPositionBottomRight], ln, true, level);
			}
		outfl->put_wantitag(srjtag[TAGMap], --level);
		}
	if (CseData->Point[0]) {
		for (int i = 0 ; i < MAXNPISTE && CseData->Point[i]; i++) {
			ln[0] = 0;
			ln2[0] = 0;
			switch (CseData->Point[i]->PointType) {
				case L'S':
					wcscpy(ln, srjtag[TAGStartPoint]);
					wcscpy(ln2, srjtag[TAGStartPointCode]);
					break;
				case L'C':
					wcscpy(ln, srjtag[TAGControl]);
					wcscpy(ln2, srjtag[TAGControlCode]);
					break;
				case L'F':
					wcscpy(ln, srjtag[TAGFinishPoint]);
					wcscpy(ln2, srjtag[TAGFinishPointCode]);
					break;
				}
			outfl->put_wtag(ln, level++);
			outfl->put_wxml_s(ln2, CseData->Point[i]->Code, level);
			if (CseData->Point[i]->CY != 0 || CseData->Point[i]->CX != 0) {
				swprintf(ln2, L"x=\"%.3f\" y=\"%.3f\"", CseData->Point[i]->CX, CseData->Point[i]->CY);
				outfl->put_wtagparams(srjtag[TAGControlPosition], ln2, true, level);
				}
			if (CseData->Point[i]->MapY != 0 || CseData->Point[i]->MapX != 0) {
				swprintf(ln2, L"x=\"%.3f\" y=\"%.3f\"", CseData->Point[i]->MapX, CseData->Point[i]->MapY);
				outfl->put_wtagparams(srjtag[TAGMapPosition], ln2, true, level);
				}
			if (CseData->Point[i]->ECodes[0]) {
				outfl->put_wtag(srjtag[TAGPunchingUnit], level++);
				for (unsigned int iC = 0; iC < sizeof(CseData->Point[0]->ECodes)/sizeof(CseData->Point[0]->ECodes[0]);
					iC++)
					if (CseData->Point[i]->ECodes[iC] > 0)
						outfl->put_wxml_d(srjtag[TAGUnitCode], CseData->Point[i]->ECodes[iC], level);
				outfl->put_wantitag(srjtag[TAGPunchingUnit], --level);
				}
			outfl->put_wantitag(ln, --level);
			}
		}
	if (CseData->Course[0]) {
		for (int i = 0 ; i < MAXNRATA && CseData->Course[i]; i++) {
			IOFCourse *Crse = CseData->Course[i];
			if (!Crse->Name[0])
				continue;
			ln[0] = 0;
			ln2[0] = 0;
			outfl->put_wtag(srjtag[TAGCourse], level++);
			outfl->put_wxml_s(srjtag[TAGCourseName], Crse->Name, level);
			outfl->put_wxml_s(srjtag[TAGCourseId], Crse->Id, level);
			for (unsigned int j = 0; j < sizeof(Crse->ShortName)/sizeof(Crse->ShortName[0]) && Crse->ShortName[j][0]; j++)
				outfl->put_wxml_s(srjtag[TAGClassShortName], Crse->ShortName[j], level);
			for (unsigned int j = 0; j < sizeof(Crse->CourseVariation)/sizeof(Crse->CourseVariation[0]) &&
				Crse->CourseVariation[j]; j++) {
				IOFCourseVariation *CVar = Crse->CourseVariation[j];
				outfl->put_wtag(srjtag[TAGCourseVariation], level++);
				outfl->put_wxml_s(srjtag[TAGCourseVariationId], CVar->Id, level);
				if (CVar->Name[0])
					outfl->put_wxml_s(srjtag[TAGName], CVar->Name, level);
				outfl->put_wxml_d(srjtag[TAGCourseLength], CVar->Length, level);
				outfl->put_wxml_d(srjtag[TAGCourseClimb], CVar->Climb, level);
				outfl->put_wxml_d(srjtag[TAGClearECard], CVar->ennakko, level);
				if (CVar->ajanottotapa < 1 || CVar->ajanottotapa > 4)
					CVar->ajanottotapa = 1;
				outfl->put_wxml_s(srjtag[TAGAutoResult], aotapa[CVar->ajanottotapa], level);
				if (CVar->Start[0])
					outfl->put_wxml_s(srjtag[TAGStartPointCode], CVar->Start, level);
				for (unsigned int k = 0; k < sizeof(CVar->CourseControl)/sizeof(CVar->CourseControl[0])
					&& CVar->CourseControl[k]; k++) {
					outfl->put_wtag(srjtag[TAGCourseControl], level++);
					outfl->put_wxml_d(srjtag[TAGSequence], k+1, level);
					outfl->put_wxml_s(srjtag[TAGControlCode], CVar->CourseControl[k]->Code, level);
					if (CVar->CourseControl[k]->LegLength)
						outfl->put_wxml_d(srjtag[TAGLegLength], CVar->CourseControl[k]->LegLength, level);
					outfl->put_wantitag(srjtag[TAGCourseControl], --level);
					}
				if (CVar->Finish[0]) {
					outfl->put_wxml_s(srjtag[TAGFinishPointCode], CVar->Finish, level);
					outfl->put_wxml_d(srjtag[TAGDistanceToFinish], CVar->FinLeg, level);
					}
				outfl->put_wantitag(srjtag[TAGCourseVariation], --level);
				}
			outfl->put_wantitag(srjtag[TAGCourse], --level);
			}
		}
	if (CseData->ClassAssignment[0]) {
		for (int iA = 0; iA < sizeof(CseData->ClassAssignment)/sizeof(CseData->ClassAssignment[0]); iA++) {
			if (CseData->ClassAssignment[iA] == NULL)
				break;
			if (!wcscmp(CseData->ClassAssignment[iA]->Class, CseData->ClassAssignment[iA]->Course))
				continue;
			outfl->put_wtag(srjtag[TAGClassCourseAssignment], level++);
			outfl->put_wxml_s(srjtag[TAGClassName], CseData->ClassAssignment[iA]->Class, level);
			outfl->put_wxml_s(srjtag[TAGCourseName], CseData->ClassAssignment[iA]->Course, level);
			outfl->put_wantitag(srjtag[TAGClassCourseAssignment], --level);
			}
		}
	outfl->put_wantitag(srjtag[TAGCourseData], --level);
}

int kirjXmlOo(TextFl *outfl, kilptietue &kilp, int d, int piste, int level)
{
	wchar_t ln[400];
	bool tlstietue = false;

	if (level < 0) {
		tlstietue = true;
		outfl->put_wtag(L"ResultRecord", 0);
		level = 1;
		}
	if (d > 0) {
		swprintf(ln, L"%s Key=\"%d\"", srjtag[TAGParticipant], d);
		outfl->put_wtag(ln, level++);
		}
	else
		outfl->put_wtag(srjtag[TAGParticipant], level++);
	for (int ii = 0; ii < 1; ii++) {
		if (piste < -9) {
			outfl->put_wxml_s(srjtag[TAGClassId], Sarjat[kilp.sarja].sarjanimi, level);
			outfl->put_wxml_d(srjtag[TAGId], kilp.kilpno, level);
			outfl->put_wxml_d(srjtag[TAGPersonId], kilp.lisno[0], level);
			outfl->put_wxml_d(srjtag[TAGPersonIdInt], kilp.lisno[1], level);
			if (kilp.wrkoodi[0])
				outfl->put_wxml_s(srjtag[TAGPersonIdTxt], kilp.wrkoodi, level);
			}
		outfl->put_wtag(srjtag[TAGName], level++);
		outfl->put_wxml_s(srjtag[TAGFamily], kilp.sukunimi, level);
		outfl->put_wxml_s(srjtag[TAGGiven], kilp.etunimi, level);
		outfl->put_wantitag(srjtag[TAGName], --level);
		if (piste < -9) {
			if (kirjXMLkaikki || kilp.arvo[0])
				outfl->put_wxml_s(srjtag[TAGTitle], kilp.arvo, level);
			if (kirjXMLkaikki || kilp.seura[0])
				outfl->put_wxml_s(srjtag[TAGClub], kilp.seura, level);
			if (kirjXMLkaikki || kilp.seuralyh[0])
				outfl->put_wxml_s(srjtag[TAGClubCode], kilp.seuralyh, level);
			if (kirjXMLkaikki || kilp.maa[0])
				outfl->put_wxml_s(srjtag[TAGCountry], kilp.maa, level);
			if (kirjXMLkaikki || kilp.piiri)
				outfl->put_wxml_d(srjtag[TAGDistrict], kilp.piiri, level);
			if (kirjXMLkaikki || kilp.yhdistys[0])
				outfl->put_wxml_s(srjtag[TAGOrganization], kilp.yhdistys, level);
			if (kirjXMLkaikki || kilp.joukkue[0])
				outfl->put_wxml_s(srjtag[TAGTeam], kilp.joukkue, level);
			}
		for (int i = 0; i < 3; i++) {
			if ((piste < -9 || piste == 0 || kilpparam.valuku+1) && (kirjXMLkaikki || kilp.pisteet[i])) {
				swprintf(ln, L"%s Order=\"%d\"", srjtag[TAGPoints], i+1);
				outfl->put_wtag(ln, level++);
				outfl->put_wxml_d(srjtag[TAGValue], kilp.pisteet[i], level);
				outfl->put_wantitag(srjtag[TAGPoints], --level);
				}
			}
		int epv = Sarjat[kilp.sarja].enspv;
		int vpv = Sarjat[kilp.sarja].viimpv;
		if (piste >= -1) {
			epv = k_pv;
			vpv = k_pv;
			}
		if (piste < -9 && vpv > epv && kilp.y_hyv(vpv) && kilp.ytulos(vpv)) {
			outfl->put_wxml_time(srjtag[TAGResult], 0, kilp.ytulos(vpv), 0, SEK, pvparam[vpv].laika, 0, level);
			outfl->put_wxml_d(srjtag[TAGRank], kilp.y_sija(vpv), level);
			outfl->put_wxml_s(srjtag[TAGStatus], haeteksti(kilp.ytark(vpv), Stat, StatSt), level);
			}
		else if (piste == kilpparam.valuku+1 && k_pv > epv) {
			outfl->put_wxml_time(srjtag[TAGResult], 0, kilp.ytulos(k_pv), 0, SEK, pvparam[k_pv].laika, 0, level);
			outfl->put_wxml_d(srjtag[TAGRank], kilp.y_sija(k_pv), level);
			outfl->put_wxml_s(srjtag[TAGStatus], haeteksti(kilp.ytark(k_pv), Stat, StatSt), level);
			}
		outfl->put_wtag(srjtag[TAGRaces], level++);
		for (int pv = epv; pv <= vpv; pv++) {
			if (kilp.tark(pv) == L'P')
				break;
			swprintf(ln, L"%s RaceNo=\"%d\"", srjtag[TAGRace], pv+1);
			outfl->put_wtag(ln, level++);
			if (kirjXMLkaikki || kilp.pv[pv].bib)
				outfl->put_wxml_d(srjtag[TAGBib], kilp.pv[pv].bib, level);
			if (piste < -9) {
				if (kirjXMLkaikki || kilp.pv[pv].badge[0])
					outfl->put_wxml_d(srjtag[TAGBadge], kilp.pv[pv].badge[0], level);
				if (kirjXMLkaikki || kilp.pv[pv].badge[1])
					outfl->put_wxml_d(srjtag[TAGBadge], kilp.pv[pv].badge[1], level);
				}
			if (kirjXMLkaikki || kilp.pv[pv].rata[0])
				outfl->put_wxml_s(srjtag[TAGCourse], kilp.pv[pv].rata, level);
			if (piste >= -1 || kirjXMLkaikki || kilpparam.sarjavaihtelee)
				outfl->put_wxml_s(srjtag[TAGClassId], Sarjat[kilp.pv[pv].sarja].sarjanimi, level);
			if (piste < -9 || piste == 0 || kilpparam.valuku+1) {
				if (kirjXMLkaikki || kilp.pv[pv].tlahto != TMAALI0)
					outfl->put_wxml_time(srjtag[TAGStartTime], 0, kilp.pv[pv].tlahto, t0, SEK, 8, 0, level);
				}
			if (piste < -9 || piste == -1) {
				if (kirjXMLkaikki || pvparam[pv].hiihtolahto && kilp.pv[pv].va[0].vatulos != TMAALI0)
					outfl->put_wxml_time(srjtag[TAGStartGateTime], 0, kilp.pv[pv].va[0].vatulos, t0, SEK, pvparam[pv].laika, 0, level);
				}
			if ((piste < -9 && (kirjXMLkaikki || kilp.tulos_pv(pv) != 0)) || piste == 0 || kilpparam.valuku+1) {
				outfl->put_wxml_time(srjtag[TAGResult], 0, kilp.tulos_pv(pv, piste > -9), 0, SEK, pvparam[pv].laika, 0, level);
				outfl->put_wxml_d(srjtag[TAGRank], kilp.sija(pv), level);
				}
			outfl->put_wxml_s(srjtag[TAGStatus], haeteksti(kilp.tark(pv), Stat, StatSt), level);
			if ((piste < -9 || piste == 0 || kilpparam.valuku+1) && (kirjXMLkaikki || Sarjat[kilp.sarja].lsak[pv] || kilp.pv[pv].tasoitus)) {
				outfl->put_wtag(srjtag[TAGPenalty], level++);
				if (Sarjat[kilp.sarja].lsak[pv]) {
					outfl->put_wxml_s(srjtag[TAGBiathlon], kilp.pv[pv].asakot, level);
					}
				else {
					outfl->put_wxml_d(srjtag[TAGSecs], kilp.pv[pv].tasoitus, level);
					}
				outfl->put_wantitag(srjtag[TAGPenalty], --level);
				}
			if (kirjXMLkaikki || kilp.pv[pv].enn != 0)
				outfl->put_wxml_time(srjtag[TAGPersonalBest], 0, kilp.pv[pv].enn, 0, SEK, 8, 0, level);
			if (kirjXMLkaikki || kilp.pv[pv].tav != 0)
				outfl->put_wxml_time(srjtag[TAGGoal], 0, kilp.pv[pv].tav, 0, SEK, 8, 0, level);
			for (int i = 0; i < 2; i++) {
				if ((piste < -9 || piste == 0 || kilpparam.valuku+1) && (kirjXMLkaikki || kilp.pv[pv].pvpisteet[i])) {
					swprintf(ln, L"%s Order=\"%d\"", srjtag[TAGPoints], i+1);
					outfl->put_wtag(ln, level++);
					outfl->put_wxml_d(srjtag[TAGValue], kilp.pv[pv].pvpisteet[i], level);
					outfl->put_wantitag(srjtag[TAGPoints], --level);
					}
				}
			int ala = 1, yla, val = 0;
			yla = Sarjat[kilp.sarja].valuku[pv];
			if (piste < -9) {
				for (int i = ala; i <= yla; i++)
					val += kilp.pv[pv].va[i+1].vatulos;
				}
			if (piste > 0) {
				ala = piste;
				yla = piste;
				val = 1;
				}
			if (val) {
				outfl->put_wtag(srjtag[TAGIntermediaries], level++);
				for (int i = ala; i <= yla; i++) {
					if (kirjXMLkaikki || kilp.pv[pv].va[i+1].vatulos || piste > 0) {
						swprintf(ln, L"%s Order=\"%d\"", srjtag[TAGIntermediary], i);
						outfl->put_wtag(ln, level++);
						outfl->put_wxml_time(srjtag[TAGResult], 0, kilp.pv[pv].va[i+1].vatulos, 0, SEK, pvparam[pv].laika, 0, level);
						outfl->put_wxml_d(srjtag[TAGRank], kilp.pv[pv].va[i+1].vasija, level);
						outfl->put_wantitag(srjtag[TAGIntermediary], --level);
						}
					}
				outfl->put_wantitag(srjtag[TAGIntermediaries], --level);
				}
			outfl->put_wantitag(srjtag[TAGRace], --level);
			}
		outfl->put_wantitag(srjtag[TAGRaces], --level);
		outfl->put_wantitag(srjtag[TAGParticipant], --level);
		}
	if (tlstietue) {
		outfl->put_wantitag(L"ResultRecord", --level);
		}
	return(0);
}

void kirjXmlOsanottajat(TextFl *outfl, int level)
{
	int d;
	kilptietue kilp;

	outfl->put_wtag(srjtag[TAGParticipants], level++);
	for (d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (kilp.kilpstatus != 0 || !kilp.kirjoitaTiedostoon())
			continue;
		kirjXmlOo(outfl, kilp, d, -99, level);
		}
	outfl->put_wantitag(srjtag[TAGParticipants], --level);
}

int kirjVainRadatXml(wchar_t *outfname, IOFCourseData *CseData)
{
	TextFl *outfl;
	INT   level = 0, err = 0;
//	wchar_t ln[200];

	outfl = new TextFl(outfname, L"wt");
	if (outfl->IsOpen()) {
		outfl->WriteLine(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
//		swprintf(ln, L"%s createTime=\"%s\" creator=\"HkKisaWin %s\"", srjtag[TAGEvent], ISOdatetime(0,1), Build());
//		outfl->put_wtag(ln, level++);
		wcscpy(CseData->IOFVersion, L"2.0.3");
		kirjXmlRadat(outfl, level, CseData);
		}
	else
		err = 1;
	delete outfl;
	return(err);
}

void kirjEventXml(wchar_t *outfname, bool kirjSarjat, bool kirjRadat, bool kirjOsanottajat)
{
	TextFl *outfl;
	INT   level = 0;
	wchar_t ln[200];

	if (wcslen(kilpparam.kilpkoodi) == 0) {
		wcscpy(kilpparam.kilpkoodi, L"KOODI");
		}
	outfl = new TextFl(outfname,L"wt");
	if (outfl->IsOpen()) {
		outfl->WriteLine(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
//		   L"<?xml-stylesheet type=\"text/xsl\" href=\"%s\"?>", style);
		swprintf(ln, L"%s createTime=\"%s\" creator=\"HkKisaWin %s\"", srjtag[TAGEvent], ISOdatetime(0,1), Build());
		outfl->put_wtag(ln, level++);
		if (kirjSarjat) {
// Software version data
			kirjXmlSoftware(outfl, level);
// Event data
			kirjXmlEventData(outfl, level);
// Race definitions
			kirjXmlRaceData(outfl, level);
// Class definitions
			kirjXmlClasses(outfl, level);
			}
		if (kirjRadat) {
			kirjXmlRadat(outfl, level, &CourseData);
			}
		if (kirjOsanottajat) {
			kirjXmlOsanottajat(outfl, level);
			}
		outfl->put_wantitag(srjtag[TAGEvent], --level);
		}
	else
		writeerror_w(
			L"Sarjatietotiedoston avaus kirjoitusta varten ei onnistu", 0);
	delete outfl;
}

