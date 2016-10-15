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
#include <bvideo.h>
#include <bkeybrd.h>
#endif
#include <bstrings.h>
#pragma hdrstop
#include <sys/stat.h>
#include "VDeclare.h"

#define XMLSARJAT

#ifndef LUENTA
int lueIOF30VainRadatXml(wchar_t *filename, IOFCourseData *CseData);
#endif
extern INT tarkkuusparam;
bool kirjXMLkaikki;

static wchar_t Dis[5] = L"SHJA", *DisSt[4] = {L"Orienteering", L"Skiing", L"Running", L"Biathlon"};
static wchar_t Sub[7] = L"NSTPRD";
static wchar_t *SubSt[6] = {L"Normal", L"Sprint", L"Team", L"Patrol", L"Rogaining", L"Rounds"};
static wchar_t Stat[10] = L"-THKEOIVP", *StatSt[9] = {L"Open", L"OK", L"DQ", L"DNF", L"DNS", L"LAP", L"Open", L"Vacant", L"Absent"};
static wchar_t Bdg[5] = L"ETSM", *BdgSt[4] = {L"Emit", L"emiTag", L"Sirit", L"Other"};
static wchar_t *Pr[4] = {L"1", L"0,1", L"0,01", L"0,001"};
static wchar_t *Tabs[8] = {L"", L"\t", L"\t\t", L"\t\t\t", L"\t\t\t\t", L"\t\t\t\t\t", L"\t\t\t\t\t\t", L"\t\t\t\t\t\t\t"};
static wchar_t *LLaji[3] = {L"Auto", L"Individual", L"Fixed"};
static wchar_t *SakLaji[2] = {L"Shooting", L"Other"};

#define TAGEvent				0
#define TAGSoftware 			1
#define TAGVersionId 			2
#define TAGFileFormat 			3
#define TAGLegs 				4
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
#define TAGLegCount 			27
#define TAGClassCount 			28
#define TAGLeg  				29
#define TAGLegNo 				30
#define TAGMassStartxx 			31
#define TAGDate 				32
#define TAGFISData 				33
#define TAGFValue 				34
#define TAGMinPenalty 			35
#define TAGIdStart 				36
#define TAGPrecision 			37
#define TAGInternal 			38
#define TAGResults 				39
#define TAGMassStart 			40
#define TAGMassStartThreshold	41
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
#define TAGBibShift 			60
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
#define TAGMapPositionTopLeft  139
#define TAGMapPositionBottomRight 140
#define TAGNumberOfCompetitors 141
#define OldTAGMassStartThreshold	142
#define TAGTeamId 				143
#define TAGTeamName 			144
#define TAGTeamNo 				145
#define TAGListNo 				146
#define TAGHeat 				147
#define TAGLegStatus 			148
#define TAGLegResult 			149
#define TAGLegRank 				150
#define TAGEmitToSplits		    151

static wchar_t *srjtag[153] = {
	L"Event",
	L"Software",
	L"VersionId",
	L"FileFormat",
	L"Legs",
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
	L"LegCount",
	L"ClassCount",
	L"Leg",
	L"LegNo",
	L"EventRaceId",
	L"Date",
	L"FISData",
	L"FValue",
	L"MinPenalty",
	L"IdStart",
	L"Precision",
	L"Internal",
	L"Results",
	L"MassStart",
	L"MassStartThreshold",
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
	L"BibShift",
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
	L"MapPositionTopLeft",
	L"MapPositionBottomRight",
	L"NumberOfCompetitors",
	L"MassStartThreshold",
	L"TeamId",
	L"TeamName",
	L"TeamNo",
	L"ListNo",
	L"Heat",
	L"LegStatus",
	L"LegResult",
	L"LegRank",
	L"EmitToSplits",
	L""
	};

int powi(int i, int j)
{
	int k = 1;

	for (; j > 0; j--)
		k *= i;
	return(k);
}

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
			case TAGLegs :
				node[inode].gettext_int(&kilpparam.n_os);
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
	int er = 0, inode, DepthIn, haara = 0;
	wchar_t ln[40], *p;

	haenodeid(node, nnode);
	DepthIn = node[0].depth + 1;
	for (inode = 1; inode <= nnode; inode++) {
		if (node[inode].depth == DepthIn) {
			haara = 0;
			switch (node[inode].tagid) {
				case TAGEventCode :
					node[inode].gettext(kilpparam.kilpkoodi, sizeof(kilpparam.kilpkoodi)/2);
					break;
				case TAGName :
					node[inode].gettext(kilpparam.Name, sizeof(kilpparam.Name)/2);
					break;
				case TAGDate :
					node[inode].gettext(kilpparam.Date, sizeof(kilpparam.Date)/2);
					break;
				case TAGTitle :
					node[inode].gettext(kilpailu, sizeof(kilpailu)/2);
					break;
				case TAGSport :
					haara = 2;
					break;
				case TAGYear :
					node[inode].gettext_int(&kilpparam.vuosi);
					break;
				case TAGLegCount :
					node[inode].gettext_int(&kilpparam.n_os_akt);
					break;
				case TAGClassCount :
					node[inode].gettext_int(&sarjaluku);
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
				case TAGPrecision :
					haara = 1;
					break;
				}
			}
		if (node[inode].depth == DepthIn + 1) {
			if (haara == 1) {
				switch (node[inode].tagid) {
					case TAGInternal :
						if (node[inode].gettext(ln, sizeof(ln)/2)) {
							if (wcslen(ln) > 2)
								kilpparam.pyor[1] = SEK/powi(10, (wcslen(ln)-2));
							else
								kilpparam.pyor[1] = SEK;
							kilpparam.laika = wcslen(ln)+7;
							}
						break;
					case TAGRank :
						if (node[inode].gettext(ln, sizeof(ln)/2)) {
							if (wcslen(ln) > 2)
								kilpparam.pyor[2] = SEK/powi(10, (wcslen(ln)-2));
							else
								kilpparam.pyor[2] = SEK;
							kilpparam.laikasj = wcslen(ln)+7;
							}
						break;
					case TAGResults :
						if (node[inode].gettext(ln, sizeof(ln)/2)) {
							if (wcslen(ln) > 2)
								kilpparam.pyor[3] = SEK/powi(10, (wcslen(ln)-2));
							else
								kilpparam.pyor[3] = SEK;
							kilpparam.laika2 = wcslen(ln)+7;
							}
						break;
					case TAGIntermediary :
						if (node[inode].gettext(ln, sizeof(ln)/2)) {
							if (wcslen(ln) > 2)
								kilpparam.pyor[4] = SEK/powi(10, (wcslen(ln)-2));
							else
								kilpparam.pyor[4] = SEK;
							kilpparam.laikava = wcslen(ln)+7;
							}
						break;
					}
				continue;
				}
			if (haara == 2) {
				switch (node[inode].tagid) {
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
					}
				continue;
				}
			continue;
			}
		}
	if (kilpparam.alalaji == L'P') {
		if (kilpparam.partio == 0)
			kilpparam.partio = 2;
		kilpparam.rogaining = false;
		}
	else if (kilpparam.alalaji == L'R') {
		kilpparam.partio = 2;
		kilpparam.rogaining = true;
		}
	else {
		kilpparam.partio = 0;
		kilpparam.rogaining = false;
		}
	return(er);
}

#ifdef XMLSARJAT
static int loadClassLegData(sarjatietue *Sarja, xml_node *node, int *inode, int nnode)
{
	int er = 0, DepthIn, haara = 0, haara2 = 0, i_os, i_va = 0;
	int iRule = 0;
	wchar_t ln[80];

	if (!node[*inode].getparam_int(L"LegNo", &i_os))
		return(1);
	i_os--;
	if (i_os < 0 || i_os >= kilpparam.n_os)
		return(1);
	Sarja->valuku[i_os] = 0;
	DepthIn = node[*inode].depth + 1;
	++*inode;
	for (; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			haara = 0;
			switch (node[*inode].tagid) {
				case TAGNumberOfCompetitors :
					node[*inode].gettext_int(&Sarja->nosuus[i_os]);
					break;
				case TAGDistance :
					haara = 1;
					break;
				case TAGMassStart :
					if (node[*inode].gettext(ln, sizeof(ln)/2))
						Sarja->ylahto[i_os] = wstrtoaika_vap(ln, t0);
					break;
				case TAGMassStartThreshold :
				case OldTAGMassStartThreshold :
					if (node[*inode].gettext(ln, sizeof(ln)/2))
						Sarja->ylkynnys[i_os] = wstrtoaika_vap(ln, t0);
					break;
				case TAGStartInterval :
					node[*inode].gettext_int(&Sarja->lahtovali[i_os]);
					break;
				case TAGPenaltyCount :
					node[*inode].gettext_int(&Sarja->paikat[i_os]);
					break;
				case TAGPenaltyUnit :
					node[*inode].getparam_text(L"type", ln, sizeof(ln)/2);
					if (wcscmp(ln, SakLaji[0]) == 0)
						node[*inode].gettext_int(&Sarja->amptsak[i_os]);
					else
						node[*inode].gettext_int(&Sarja->tsak[i_os]);
					break;
				case TAGIntermediaries :
					haara = 2;
					break;
				case TAGEmitToSplits :
					node[*inode].gettext(ln, sizeof(ln)/2-1);
					Sarja->tulkMTB(ln, i_os);
					break;
				case TAGLeg :
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
						node[*inode].gettext(Sarja->matka[i_os], sizeof(Sarja->matka[i_os]));
						break;
					case TAGUnit :
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
							break;
//							return(1);
						haara2 = 2;
						break;
					}
				continue;
				}
			continue;
			}
		if (node[*inode].depth == DepthIn+2) {
			if (haara2 == 2 && i_va < kilpparam.valuku) {
				switch (node[*inode].tagid) {
					case TAGDistance :
						node[*inode].gettext(Sarja->va_matka[i_os][i_va],
							sizeof(Sarja->va_matka[i_os][i_va]));
						if (Sarja->valuku[i_os] < i_va + 1) {
							Sarja->valuku[i_os] = i_va + 1;
							}
						break;
					case TAGId :
						node[*inode].gettext_int(&Sarja->va_koodi[i_os][i_va]);
						break;
					case TAGPublic :
						node[*inode].gettext(ln, sizeof(ln)/2-1);
						if (towupper(ln[0]) == L'Y')
							Sarja->va_piilota[i_os][i_va] = 9;
						else if (ln[wcslen(ln)-1] > L'0' && ln[wcslen(ln)-1] < L'9')
							Sarja->va_piilota[i_os][i_va] = ln[wcslen(ln)-1] - L'0';
						else
							Sarja->va_piilota[i_os][i_va] = 0;
						break;
					}
				continue;
				}
			continue;
			}
		}
	if (!er) {
		if (i_os == 0)
			Sarja->aosuus[0] = -1;
		Sarja->aosuus[i_os+1] = Sarja->aosuus[i_os] + Sarja->nosuus[i_os];
		for (int yos = Sarja->aosuus[i_os]+1; yos <= Sarja->aosuus[i_os+1]; yos++)
			Sarja->yosuus[yos] = i_os;
		if (Sarja->maxnosuus < Sarja->nosuus[i_os])
			Sarja->maxnosuus = Sarja->nosuus[i_os];
		}
	return(er);
}

static int loadClassData(xml_node *node, int nnode)
{
	int er = 0, inode, DepthIn, haara = 0, i_srj;
	wchar_t buf[100];
	sarjatietue *Sarja;

	haenodeid(node, nnode);
	if (!node[0].getparam_int(L"ClassNo", &i_srj))
		return(1);
	i_srj--;
	if (i_srj < 0 || i_srj >= sarjaluku)
		return(1);
	Sarja = Sarjat+i_srj;
	Sarja->nollaa();
	DepthIn = node[0].depth + 1;
	for (inode = 1; inode <= nnode; inode++) {
		if (node[inode].depth == DepthIn) {
			haara = 0;
			switch (node[inode].tagid) {
				case TAGClassId :
					node[inode].gettext(Sarja->sarjanimi, sizeof(Sarja->sarjanimi));
					break;
				case TAGName :
					node[inode].gettext(Sarja->psarjanimi, sizeof(Sarja->psarjanimi));
					break;
				case TAGIdStart :
					node[inode].gettext_int(&Sarja->sarjaalku);
					break;
				case TAGBibShift :
					node[inode].gettext_int(&Sarja->knosiirto);
					break;
				case TAGFee :
					haara = 1;
					break;
				case TAGLegCount :
					node[inode].gettext_int(&Sarja->ntosuus);
					break;
				case TAGResultListOrder :
					node[inode].gettext_int(&Sarja->tuljarj);
					break;
				case TAGHandicaps :
					node[inode].gettext(buf, 50);
					Sarja->taslaji = wcswcind(buf[0], L"RT") + 1;
					if (wcswcind(L'O', buf) >= 0)
						Sarja->taslaji += 10;
					break;
				case TAGStartTime :
					node[inode].gettext(buf, 50);
					Sarja->lahto = wstrtoaika_vap(buf, t0);
					break;
				case TAGStartGate :
					node[inode].gettext_int(&Sarja->lno);
					break;
				case TAGFinishGate :
					node[inode].gettext_int(&Sarja->mno);
					break;
				case TAGLegs :
					haara = 2;
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
					case TAGReduced :
						node[inode].gettext_int(&Sarja->maksual);
						break;
					}
				continue;
				}
			if (haara == 2) {
				switch (node[inode].tagid) {
					case TAGLeg :
						if ((er = loadClassLegData(Sarja, node, &inode, nnode)) != 0)
							return(er);
						break;
					}
				continue;
				}
			continue;
			}
		}
	if (!er)
		Sarja->osuusluku = Sarja->aosuus[Sarja->ntosuus] + 1;
	return(er);
}
#endif

#if !defined(LUENTA)
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
	wchar_t ln[40] = L"";
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
					node[*inode].gettext(ln, sizeof(ln)/2-1);
					break;
				case TAGCourseControl :
					if ((er = loadCourseControl(&CVar, node, inode, nnode)) != 0)
						return(er);
					break;
				}
			}
		}
	CVar.ajanottotapa = wcswcind(ln[0], L"SN-Y") + 1;
	if (CVar.ajanottotapa == 0)
		CVar.ajanottotapa = 1;
	Cs->CourseVariation[iVar] = new IOFCourseVariation;
	*Cs->CourseVariation[iVar] = CVar;
	return(er);
}

static int loadCourse(IOFCourseData *CseData, int *icourse, xml_node *node, int *inode, int nnode)
{
	int er = 0, ic, iVar = 0, DepthIn;
	wchar_t ln[40] = L"";
	IOFCourse Cs;
	IOFCourseVariation CVar;

	Cs.nollaa();
	CVar.nollaa();
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
					if (Cs.CourseVariation[0] == NULL) {
						Cs.CourseVariation[0] = new IOFCourseVariation;
						*Cs.CourseVariation[0] = CVar;
						CVar.nollaa();
						}
					if (*icourse < (int)(sizeof(CseData->Course)/sizeof(CseData->Course[0]))) {
						CseData->Course[*icourse] = new IOFCourse;
						*CseData->Course[*icourse] = Cs;
						++*icourse;
						Cs.nollaa();
						}
					else
						return(1);
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
	if (ipoint < (int)(sizeof(CseData->Point)/sizeof(CseData->Point[0]))) {
		CseData->Point[ipoint] = new IOFPoint;
		*CseData->Point[ipoint] = Pt;
		}
	else
		er = 1;
	return(er);
}

static int loadCourseData(xml_node *node, int nnode, IOFCourseData *CseData)
{
	int er = 0, inode, DepthIn, haara = 0, ipoint = 0, icourse = 0;
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
					if ((er = loadCourse(CseData, &icourse, node, &inode, nnode)) != 0)
						return(er);
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
			continue;
			}
		}
	return(er);
}
#endif  // !LUENTA

#ifdef EI_OLE
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
						if (j > 0 && j < (int)(sizeof(Kilp.piste)/sizeof(Kilp.piste[0]))) {
							node[inode].gettext_int(&i);
							Kilp.piste[j-1];
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
#endif

#if !defined(LUENTA)
int lueVainRadatXml(wchar_t *filename, IOFCourseData *CseData)
{
	TextFl *infile;
	int ntag = 700000, haara = 0, inode, inode_in, depth = 0, rv, er = 0, lntype, jt = 0;
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
			nd.depth = depth;
			for (int i = 0; i <= depth; i++)
				nd.nodeno[i] = k[i];           // merkitään eri tasojen järjestysnumerot solmuun
			if (lntype > 0)
				nd.tagid = hae_tagid(nd.tag);
			if (lntype == 1) {
				if (inode >= tree.maxnode-1) {
					er = 2;
					break;
					}
#ifdef _DEBUG
				if ((inode % 10000) == 0)
					jt++;
#endif
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
					switch (nd.tagid) {
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
			else if (lntype == 8) {                  // sulkeva tag uudella rivillä
				int jnode;
				depth--;
				k[depth]++;                 // kasvatetaan tason laskuria sulkevän tagin tasolla
				// Etsitään vastaava avaava tag aiemmasta ketjusta
				for (jnode = inode; jnode > 0 && tree.node[jnode].depth > depth; jnode--) ;
				if (wcscmp(tag, tree.node[jnode].tag)) {      // Varmistetaan, että tagit ovat pari
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
		swprintf(msg, L"XML-tiedoston %s lukeminen päättyi rivillä %d tasolla %d, node %d", filename, rv, depth, inode);
		}
	if (er) {
		if (!msg[0]) {
			swprintf(msg, L"Tiedostossa %s virhe rivillä %d tai aikaisemmin", filename, rv);
			}
		writeerror_w(msg, 0);
		}
#ifdef _DEBUG
	swprintf(msg, L"XML-ratatiedostosta luettu %d riviä, 10000*%d nodea", rv, jt);
	writeerror_w(msg, 0);
#endif
	tree.nodecount = inode+1;
	return(er);
}
#endif  // !defined(LUENTA)

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
			nd.depth = depth;
			for (int i = 0; i <= depth; i++)
				nd.nodeno[i] = k[i];           // merkitään eri tasojen järjestysnumerot solmuun
			if (lntype > 0)
				nd.tagid = hae_tagid(nd.tag);
			if (lntype == 1) {
				tree.node[++inode] = nd;
				k[depth++]++;
				if (depth == 1) {
					inode_in = inode;
#if !defined(LUENTA)
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
#endif  // !defined(LUENTA)
					continue;
					}
				if (depth == 2) {
					inode_in = inode;
					switch (nd.tagid) {
#ifdef XMLSARJAT
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
						case TAGClasses :
							haara = 2;
							break;
#endif
#if !defined(LUENTA)
						case TAGCourseData :
							haara = 0;
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er && lueRadat)
								loadCourseData(tree.node+inode_in, inode-inode_in, &CourseData);
							inode = inode_in-1;
							break;
#endif // !defined(LUENTA)
#ifdef EI_OLE
						case TAGParticipants :
							haara = 3;
							break;
#endif
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
#ifdef XMLSARJAT
						case TAGClass :
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er && lueSarjat && haara == 2) {
								er = loadClassData(tree.node+inode_in, inode-inode_in);
								}
							inode = inode_in-1;
							break;
#endif
#ifdef EI_OLE
						case TAGParticipant :
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er && lueOsanottajat && haara == 3) {
								er = loadParticipantData(tree.node+inode_in, inode-inode_in);
								}
							inode = inode_in-1;
							break;
#endif
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
			else if (lntype == 8) {                  // sulkeva tag uudella rivillä
				int jnode;
				depth--;
				k[depth]++;                 // kasvatetaan tason laskuria sulkevän tagin tasolla
				// Etsitään vastaava avaava tag aiemmasta ketjusta
				for (jnode = inode; jnode > 0 && tree.node[jnode].depth > depth; jnode--) ;
				if (wcscmp(tag, tree.node[jnode].tag)) {      // Varmistetaan, että tagit ovat pari
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
	else {
		if (infile)
			delete infile;
		return(-1);
		}
	delete infile;
	if (depth) {
		er = 1;
		swprintf(msg, L"XML-tiedosto %s päättyi rivillä %d tasolla %d", filename, rv, depth);
		}
	if (er) {
		if (!msg[0]) {
			swprintf(msg, L"Tiedostossa %s virhe rivillä %d tai aikaisemmin", filename, rv);
			}
		writeerror_w(msg, 0);
		}
	tree.nodecount = inode+1;
	return(er);
}

#ifdef XMLSARJAT
void kirjXmlSoftware(TextFl *outfl, int level)
{
	outfl->put_wtag(srjtag[TAGSoftware], level++);
	outfl->put_wxml_s(srjtag[TAGVersionId], kilpparam.tiedlaji, level);
	outfl->put_wtag(srjtag[TAGFileFormat], level++);
	outfl->put_wxml_d(srjtag[TAGLegs], kilpparam.n_os, level);
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
	outfl->put_wxml_s(srjtag[TAGName], kilpparam.Name, level);
	outfl->put_wxml_s(srjtag[TAGDate], kilpparam.Date, level);
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
	outfl->put_wxml_d(srjtag[TAGLegCount], kilpparam.n_os_akt, level);
	outfl->put_wxml_d(srjtag[TAGClassCount], sarjaluku, level);
	if ((i = wcswcind(kilpparam.badgelaji, Bdg)) >= 0) {
		wcscpy(ln, BdgSt[i]);
		if (kilpparam.kaksibadge == 1)
			wcscat(ln, L"*2");
		outfl->put_wxml_s(srjtag[TAGBadge], ln, level);
		}
	if ((i = wcswcind(kilpparam.badgelaji, Bdg)) >= 0) {
		outfl->put_wxml_s(srjtag[TAGBadge], BdgSt[i], level);
		}
	outfl->put_wtag(srjtag[TAGPrecision], level++);
	outfl->put_wxml_s(srjtag[TAGInternal], Pr[PyorIx(kilpparam.pyor[1])], level);
	outfl->put_wxml_s(srjtag[TAGRank], Pr[PyorIx(kilpparam.pyor[2])], level);
	outfl->put_wxml_s(srjtag[TAGResults], Pr[PyorIx(kilpparam.pyor[3])], level);
	outfl->put_wxml_s(srjtag[TAGIntermediary], Pr[PyorIx(kilpparam.pyor[4])], level);
	outfl->put_wantitag(srjtag[TAGPrecision], --level);
	outfl->put_wantitag(srjtag[TAGEventData], --level);
}

void kirjXmlClassLegs(TextFl *outfl, sarjatietue *Sarja, int level)
{
	wchar_t ln[400];

	outfl->put_wtag(srjtag[TAGLegs], level++);
	for (int os = 0; os < Sarja->ntosuus; os++) {
		swprintf(ln, L"%s LegNo=\"%d\"", srjtag[TAGLeg], os+1);
		outfl->put_wtag(ln, level++);
		if (Sarja->nosuus[os] > 1)
			outfl->put_wxml_d(srjtag[TAGNumberOfCompetitors], Sarja->nosuus[os], level);
		outfl->put_wtag(srjtag[TAGDistance], level++);
		outfl->put_wxml_s(srjtag[TAGValue], ansitowcs(ln, Sarja->matka[os], 40), level);
		outfl->put_wantitag(srjtag[TAGDistance], --level);
		if (Sarja->ylahto[os] != TMAALI0)
			outfl->put_wxml_time(srjtag[TAGMassStart], 0, Sarja->ylahto[os], t0, SEK, 8, 0, level);
		if (Sarja->ylkynnys[os] != TMAALI0)
			outfl->put_wxml_time(srjtag[TAGMassStartThreshold], 0, Sarja->ylkynnys[os], t0, SEK, 8, 0, level);
		if (Sarja->lahtovali[os] != 0)
			outfl->put_wxml_d(srjtag[TAGStartInterval], Sarja->lahtovali[os], level);
		if (Sarja->paikat[os] != 0)
			outfl->put_wxml_d(srjtag[TAGPenaltyCount], Sarja->paikat[os], level);
		if (Sarja->amptsak[os] != 0)
			outfl->put_wxml_d(srjtag[TAGPenaltyUnit], Sarja->amptsak[os], level, wconcat(L"type=\"", SakLaji[0], L"\""));
		if (Sarja->tsak[os] != 0)
			outfl->put_wxml_d(srjtag[TAGPenaltyUnit], Sarja->tsak[os], level, wconcat(L"type=\"", SakLaji[1], L"\""));
#if !defined(AMPUSUUNN)
		if (Sarja->valuku[os] > 0) {
			outfl->put_wtag(srjtag[TAGIntermediaries], level++);
			for (int j = 0; j < Sarja->valuku[os]; j++) {
				swprintf(ln, L"%s Order=\"%d\"", srjtag[TAGIntermediary], j+1);
				outfl->put_wtag(ln, level++);
				if (Sarja->va_matka[os][j][0] > 0)
					ansitowcs(ln, Sarja->va_matka[os][j], 40);
				else
					_itow(j+1, ln, 10);
				outfl->put_wxml_s(srjtag[TAGDistance], ln, level);
				if (Sarja->va_koodi[os][j] > 0)
					outfl->put_wxml_d(srjtag[TAGId], Sarja->va_koodi[os][j], level);
				if (Sarja->va_piilota[os][j] == 0)
					outfl->put_wxml_s(srjtag[TAGPublic], L"No", level);
				else if (Sarja->va_piilota[os][j] >= 9)
					outfl->put_wxml_s(srjtag[TAGPublic], L"Yes", level);
				else {
					swprintf(ln, L"Level%d", Sarja->va_piilota[os][j]);
					outfl->put_wxml_s(srjtag[TAGPublic], ln, level);
					}
				outfl->put_wantitag(srjtag[TAGIntermediary], --level);
				}
			outfl->put_wantitag(srjtag[TAGIntermediaries], --level);
			if (Sarja->mtb[os][0].alkukdi || Sarja->mtb[os][0].loppukdi) {
				swprintf(ln, L"%d", Sarja->laskemtb ? 1 : 0);
				for (int i = 0; i <= Sarja->valuku[os]; i++) {
					if (Sarja->mtb[os][i].alkukdi == 0 && Sarja->mtb[os][i].loppukdi == 0)
						break;
					swprintf(ln+wcslen(ln), L" %d %d %d %d", Sarja->mtb[os][i].alkukdi,
						Sarja->mtb[os][i].loppukdi, Sarja->mtb[os][i].piste, Sarja->mtb[os][i].pohja);
					}
				outfl->put_wxml_s(srjtag[TAGEmitToSplits], ln, level);
				}
			}
#endif
		outfl->put_wantitag(srjtag[TAGLeg], --level);
		}
	outfl->put_wantitag(srjtag[TAGLegs], --level);
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
		outfl->put_wxml_s(srjtag[TAGClassId], ansitowcs(ln, Sarjat[i].sarjanimi, 100), level);
		if (kirjXMLkaikki || Sarjat[i].psarjanimi[0])
			outfl->put_wxml_s(srjtag[TAGName], ansitowcs(ln, Sarjat[i].psarjanimi, 100), level);
		outfl->put_wxml_d(srjtag[TAGIdStart], Sarjat[i].sarjaalku, level);
		outfl->put_wxml_d(srjtag[TAGBibShift], Sarjat[i].knosiirto, level);
		if (Sarjat[i].lahto != TMAALI0)
			outfl->put_wxml_s(srjtag[TAGStartTime], aikatowstr_cols_n(ln, Sarjat[i].lahto, t0, ':', 8), level);
		outfl->put_wxml_d(srjtag[TAGStartGate], Sarjat[i].lno, level);
		outfl->put_wxml_d(srjtag[TAGFinishGate], Sarjat[i].mno, level);
		outfl->put_wtag(srjtag[TAGFee], level++);
		outfl->put_wxml_d(srjtag[TAGFull], Sarjat[i].maksu, level);
		outfl->put_wxml_d(srjtag[TAGReduced], Sarjat[i].maksual, level);
		outfl->put_wantitag(srjtag[TAGFee], --level);
		outfl->put_wxml_d(srjtag[TAGLegCount], Sarjat[i].ntosuus, level);
		outfl->put_wxml_d(srjtag[TAGResultListOrder], Sarjat[i].tuljarj, level);
		if (Sarjat[i].taslaji) {
			memset(ln, 0, sizeof(ln));
			if (Sarjat[i].taslaji % 10)
				ln[0] = L"RT"[(Sarjat[i].taslaji % 10)-1];
			if (Sarjat[i].taslaji / 10)
				wcscat(ln, L"O");
			outfl->put_wxml_s(srjtag[TAGHandicaps], ln, level);
			}
		kirjXmlClassLegs(outfl, Sarjat+i, level);
		outfl->put_wantitag(srjtag[TAGClass], --level);
		}
	outfl->put_wantitag(srjtag[TAGClasses], --level);
}
#endif

#if !defined(LUENTA)
void kirjXmlRadat(TextFl *outfl, int level, IOFCourseData *CseData)
{
	wchar_t ln[400], ln2[100];
	wchar_t *aotapa[5] = {L"",L"Separate", L"No", L"No", L"Yes"};

	if (level == 0) {
		swprintf(ln, L"%s createTime=\"%s\" creator=\"ViestiWin %s\"", srjtag[TAGCourseData], ISOdatetime(0, true), Build());
		outfl->put_wtag(ln, level++);
		}
	else
		outfl->put_wtag(srjtag[TAGCourseData], level++);
	if (!CseData->IOFVersion[0])
		wcscpy(CseData->IOFVersion, L"2.0.3");
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
			ln[0] = 0;
			ln2[0] = 0;
			outfl->put_wtag(srjtag[TAGCourse], level++);
			if (Crse->Name[0])
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
//					if (CVar->FinLeg)
					outfl->put_wxml_d(srjtag[TAGDistanceToFinish], CVar->FinLeg, level);
					}
				outfl->put_wantitag(srjtag[TAGCourseVariation], --level);
				}
			outfl->put_wantitag(srjtag[TAGCourse], --level);
			}
		}
	outfl->put_wantitag(srjtag[TAGCourseData], --level);
}
#endif // !defined(LUENTA)

//#ifdef EI_OLE
void kirjXmlOoPerus(TextFl *outfl, kilptietue &kilp, int level)
{
//	wchar_t ln[400];
}

int kirjXmlOoOsuus(TextFl *outfl, kilptietue &kilp, int os, int piste, int level)
{
	wchar_t ln[400];
	bool tlstietue = false;

	if (level < 0) {
		tlstietue = true;
		outfl->put_wtag(L"ResultRecord", 0);
		level = 1;
		outfl->put_wtag(srjtag[TAGParticipant], level++);
		outfl->put_wxml_s(srjtag[TAGClassId], Sarjat[kilp.sarja].Sarjanimi(), level);
		outfl->put_wxml_d(srjtag[TAGId], kilp.kilpno, level);
		outfl->put_wxml_s(srjtag[TAGClub], kilp.Seura(ln, LSEURA), level);
		outfl->put_wxml_d(srjtag[TAGTeamNo], kilp.joukkue, level);
		}
	swprintf(ln, L"%s LegNo=\"%d\"", srjtag[TAGLeg], os+1);
	outfl->put_wtag(ln, level++);
	for (int ii = 0; ii < 1; ii++) {
		outfl->put_wtag(srjtag[TAGName], level++);
		outfl->put_wxml_s(srjtag[TAGFamily], kilp.SukuNimi(ln, OSNIMIL, os), level);
		outfl->put_wxml_s(srjtag[TAGGiven], kilp.EtuNimi(ln, OSNIMIL, os), level);
		outfl->put_wantitag(srjtag[TAGName], --level);
		if (piste < -9 && (kirjXMLkaikki || kilp.ostiet[os].lisno))
			outfl->put_wxml_d(srjtag[TAGPersonId], kilp.ostiet[os].lisno, level);
		if (piste < -9 && (kirjXMLkaikki || kilp.ostiet[os].arvo[0]))
			outfl->put_wxml_s(srjtag[TAGTitle], ansitowcs(ln, kilp.ostiet[os].arvo, 20), level);
		if (piste < -9 && (kirjXMLkaikki || kilp.ostiet[os].osseura[0]))
			outfl->put_wxml_s(srjtag[TAGClub], ansitowcs(ln, kilp.ostiet[os].osseura, 40), level);
		if (piste < -9 && (kirjXMLkaikki || kilp.ostiet[os].badge[0]))
			outfl->put_wxml_d(srjtag[TAGBadge], kilp.ostiet[os].badge[0], level);
		if (piste < -9 && (kirjXMLkaikki || kilp.ostiet[os].badge[1]))
			outfl->put_wxml_d(srjtag[TAGBadge], kilp.ostiet[os].badge[1], level);
		outfl->put_wxml_s(srjtag[TAGLegStatus], haeteksti(kilp.wTark(os, false), Stat, StatSt), level);
		outfl->put_wxml_s(srjtag[TAGStatus], haeteksti(kilp.tTark(Sarjat[kilp.sarja].yosuus[os]), Stat, StatSt), level);
		if (kilp.wTark(os, false) == L'P')
			break;
		if (piste < -9 && (kirjXMLkaikki || kilp.ostiet[os].hajonta[0]))
			outfl->put_wxml_s(srjtag[TAGCourse], kilp.ostiet[os].Hajonta(ln), level);
		if ((piste < -9 || piste == -1) && (kirjXMLkaikki || kilp.ostiet[os].ylahto != TMAALI0)) {
			outfl->put_wxml_time(srjtag[TAGStartTime], 0, kilp.ostiet[os].ylahto, t0, SEK, 11, 0, level);
			outfl->put_wxml_s(srjtag[TAGCourse], LLaji[kilp.ostiet[os].lahtolaji], level);
			}
		if ((piste < -9 || piste == 0) && (kirjXMLkaikki || kilp.tTulos(Sarjat[kilp.sarja].yosuus[os], 0) != 0)) {
			outfl->put_wxml_time(srjtag[TAGResult], 0, kilp.tTulos(Sarjat[kilp.sarja].yosuus[os], 0), 0, SEK, kilpparam.laika, 0, level);
			outfl->put_wxml_d(srjtag[TAGRank], kilp.Sija(Sarjat[kilp.sarja].yosuus[os], 0), level);
			}
		if ((piste < -9 || piste == 0) && (kirjXMLkaikki || kilp.osTulos(os, 0, false) != 0)) {
			outfl->put_wxml_time(srjtag[TAGLegResult], 0, kilp.osTulos(os, 0, false), 0, SEK, kilpparam.laika, 0, level);
			outfl->put_wxml_d(srjtag[TAGLegRank], kilp.osSija(os), level);
			}
		if ((piste < -9 || piste == -3) && (kirjXMLkaikki || Sarjat[kilp.sarja].paikat[os] || kilp.ostiet[os].sakko)) {
			outfl->put_wtag(srjtag[TAGPenalty], level++);
			if (Sarjat[kilp.sarja].paikat[os]) {
				outfl->put_wxml_s(srjtag[TAGBiathlon], ansitowcs(ln, kilp.ostiet[os].ampsakot, 10), level);
				}
			if (kilp.ostiet[os].sakko) {
				outfl->put_wxml_d(srjtag[TAGSecs], kilp.ostiet[os].sakko/SEK, level);
				}
			outfl->put_wantitag(srjtag[TAGPenalty], --level);
			}
		int ala = 1, yla, val = 0;
		yla = Sarjat[kilp.sarja].valuku[os];
		if (piste < -9) {
			for (int i = ala; i <= yla; i++)
				val += kilp.Tulos(os, i);
			}
		if (piste > 0) {
			ala = piste;
			yla = piste;
			val = 1;
			}
		if (val) {
			outfl->put_wtag(srjtag[TAGIntermediaries], level++);
			for (int i = ala; i <= yla; i++)
				if (kirjXMLkaikki || kilp.Tulos(os, i)) {
					swprintf(ln, L"%s Order=\"%d\"", srjtag[TAGIntermediary], i+1);
					outfl->put_wtag(ln, level++);
					outfl->put_wxml_time(srjtag[TAGResult], 0, kilp.Tulos(os, i), 0, SEK, kilpparam.laika, 0, level);
					outfl->put_wxml_d(srjtag[TAGRank], kilp.Sija(os, i), level);
					outfl->put_wantitag(srjtag[TAGIntermediary], --level);
					}
			outfl->put_wantitag(srjtag[TAGIntermediaries], --level);
			}
		}
	outfl->put_wantitag(srjtag[TAGLeg], --level);
	if (tlstietue) {
		outfl->put_wantitag(srjtag[TAGParticipant], --level);
		outfl->put_wantitag(L"ResultRecord", --level);
		}
	return(0);
}

void kirjXmlOsanottajat(TextFl *outfl, int level)
{
	wchar_t ln[400];
	int d;
	kilptietue kilp;

	outfl->put_wtag(srjtag[TAGParticipants], level++);
	for (d = 1; d < (int) datf2.numrec; d++) {
		kilp.getrec(d);
		if (kilp.kilpstatus != 0)
			continue;
		swprintf(ln, L"%s Key=\"%d\"", srjtag[TAGParticipant], d);
		outfl->put_wtag(ln, level++);
		outfl->put_wxml_s(srjtag[TAGClassId], Sarjat[kilp.sarja].Sarjanimi(), level);
		outfl->put_wxml_d(srjtag[TAGId], kilp.kilpno, level);
		outfl->put_wxml_d(srjtag[TAGTeamId], kilp.joukkid, level);
		outfl->put_wxml_s(srjtag[TAGClub], kilp.Seura(ln, LSEURA), level);
		outfl->put_wxml_d(srjtag[TAGTeamNo], kilp.joukkue, level);
		if (kirjXMLkaikki || kilp.joukknimi[0])
			outfl->put_wxml_s(srjtag[TAGTeamName], ansitowcs(ln, kilp.joukknimi, LJK+1), level);
		if (kirjXMLkaikki || kilp.maa[0])
			outfl->put_wxml_s(srjtag[TAGCountry], kilp.Maa(ln), level);
		if (kirjXMLkaikki || kilp.piiri)
			outfl->put_wxml_d(srjtag[TAGDistrict], kilp.piiri, level);
		if (kirjXMLkaikki || kilp.ilmlista)
			outfl->put_wxml_d(srjtag[TAGListNo], kilp.ilmlista, level);
		if (kirjXMLkaikki || kilp.era)
			outfl->put_wxml_d(srjtag[TAGHeat], kilp.era, level);
		for (int i = 0; i < 3; i++) {
			if (kirjXMLkaikki || kilp.pisteet[i]) {
				swprintf(ln, L"%s Order=\"%d\"", srjtag[TAGPoints], i+1);
				outfl->put_wtag(ln, level++);
				outfl->put_wxml_d(srjtag[TAGValue], kilp.pisteet[i], level);
				outfl->put_wantitag(srjtag[TAGPoints], --level);
				}
			}
		outfl->put_wtag(srjtag[TAGLegs], level++);
		for (int os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
			kirjXmlOoOsuus(outfl, kilp, os, -99, level);
			}
		outfl->put_wantitag(srjtag[TAGLegs], --level);
		outfl->put_wantitag(srjtag[TAGParticipant], --level);
		}
	outfl->put_wantitag(srjtag[TAGParticipants], --level);
}
//#endif

#if !defined(LUENTA)
void kirjVainRadatXml(wchar_t *outfname, IOFCourseData *CseData)
{
	TextFl *outfl;
	INT   level = 0;
//	wchar_t ln[200];

	outfl = new TextFl(outfname, L"wt");
	if (outfl->IsOpen()) {
		outfl->WriteLine(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
//		swprintf(ln, L"%s createTime=\"%s\" creator=\"ViestiWin %s\"", srjtag[TAGEvent], ISOdatetime(), Build());
//		outfl->put_wtag(ln, level++);
		kirjXmlRadat(outfl, level, CseData);
//		outfl->put_wantitag(srjtag[TAGEvent], --level);
		}
	delete outfl;
}
#endif // !defined(LUENTA)
void kirjEventXml(wchar_t *outfname, bool kirjSarjat, bool kirjRadat, bool kirjOsanottajat)
{
#ifdef XMLSARJAT
	TextFl *outfl;
	INT   level = 0;
	wchar_t ln[200];

	if (wcslen(kilpparam.kilpkoodi) == 0) {
		wcscpy(kilpparam.kilpkoodi, L"KOODI");
		}
	outfl = new TextFl(outfname,L"wt");
	if (outfl->IsOpen()) {
		outfl->WriteLine(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
		swprintf(ln, L"%s createTime=\"%s\" creator=\"ViestiWin %s\"", srjtag[TAGEvent], ISOdatetime(0, true), Build());
		outfl->put_wtag(ln, level++);
		if (kirjSarjat) {
			hae_enslahto();
				// Software version data
			kirjXmlSoftware(outfl, level);
// Event data
			kirjXmlEventData(outfl, level);
// Class definitions
			kirjXmlClasses(outfl, level);
			}
#if !defined(LUENTA)
		if (kirjRadat) {
			kirjXmlRadat(outfl, level, &CourseData);
			}
#endif // !defined(LUENTA)
		if (kirjOsanottajat) {
			kirjXmlOsanottajat(outfl, level);
			}
		outfl->put_wantitag(srjtag[TAGEvent], --level);
		}
	else
		writeerror_w(
			L"Sarjatietotiedoston avaus kirjoitusta varten ei onnistu", 0);
	delete outfl;
#endif
}

