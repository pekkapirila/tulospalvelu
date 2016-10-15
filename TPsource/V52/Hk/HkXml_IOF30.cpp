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
#pragma hdrstop
#include "HkDeclare.h"

extern INT tarkkuusparam;
extern bool kirjXMLkaikki;

/*
static wchar_t Dis[9] = L"SHJUAMNP", *DisSt[8] = {L"Orienteering", L"Skiing", L"Running", L"Swimming", L"Biathlon", L"Skijumping", L"NavalPentathlon", L"Pentathlon"};
static wchar_t Sub[16] = L"RSPMENIOLUFCHWT";
static wchar_t *SubSt[15] = {L"Regular", L"Sprint", L"Pursuit", L"Mass start", L"Event", L"Naval", L"Military",
	L"Obstacle", L"LifeSaving", L"UtilitySwim", L"Seamanship", L"CrossCountry", L"Shooting", L"Swimming", L"Throwing"};
static wchar_t Stat[12] = L"-THKEOIVPXM", *StatSt[11] = {L"Open", L"OK", L"DQ", L"DNF", L"DNS", L"LAP", L"Open", L"Vacant", L"Absent", L"NoResult", L"Missing"};
static wchar_t Bdg[9] = L"ETS", *BdgSt[3] = {L"Emit", L"emiTag", L"Sirit"};
static wchar_t *Pr[4] = {L"1", L"0,1", L"0,01", L"0,001"};
static wchar_t *Tabs[8] = {L"", L"\t", L"\t\t", L"\t\t\t", L"\t\t\t\t", L"\t\t\t\t\t", L"\t\t\t\t\t\t", L"\t\t\t\t\t\t\t"};
*/
static wchar_t *ControlType[6] = {L"Control", L"Start", L"Finish", L"CrossingPoint", L"EndOfMarkedRoute", L""};

#define TAGEvent				0
#define TAGName 				13
#define TAGFamily 				14
#define TAGGiven 				15
#define TAGCountry 				20
#define TAGDate 				32
#define TAGClass 				43
#define TAGId					74
#define TAGOrganisation			80
#define TAGStartTime  		    81
#define TAGResult				82
#define TAGStatus				83
#define TAGCourseData  		    84
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
#define TAGPunchingUnitId	   129
#define TAGUnitCode            130
#define TAGClearECard		   131
#define TAGPublic			   132
#define TAGUseClassId  		   133
#define TAGStartQueue		   134
#define TAGPointRules		   135
#define TAGRankBased           136
#define TAGResultBased         137
#define TAGTotal		       138
#define TAGRaceCourseData 	   139
#define TAGPosition            140
#define TAGCourseFamily        141
#define TAGLength              142
#define TAGClimb               143
#define TAGMapText             144
#define TAGMapTextPosition     145
#define TAGClassCourseAssignment 146
#define TAGClassName		   147
#define TAGMapPositionTopLeft  148
#define TAGMapPositionBottomRight 149
#define TAGResultList          150
#define TAGClassResult         151
#define TAGPersonResult        152
#define TAGPerson              153
#define TAGScore		       154
#define TAGBibNumber           155
#define TAGFinishTime          156
#define TAGTime                157
#define TAGTimeBehind          158
#define TAGSplitTime           159
#define TAGPersonEntry         160
#define TAGControlCard 		   161
#define TAGRaceNumber          162
#define TAGShortName 		   163
#define TAGNationality 		   164
#define TAGEntryList		   165

static tagListTp IOF3Tags[] = {
	{TAGEvent, L"Event"},
	{TAGName, L"Name"},
	{TAGFamily, L"Family"},
	{TAGGiven, L"Given"},
	{TAGCountry, L"Country"},
	{TAGDate, L"Date"},
	{TAGClass, L"Class"},
	{TAGId, L"Id"},
	{TAGOrganisation, L"Organisation"},
	{TAGStartTime, L"StartTime"},
	{TAGResult, L"Result"},
	{TAGStatus, L"Status"},
	{TAGCourseData, L"CourseData"},
	{TAGModifyDate, L"ModifyDate"},
	{TAGClock, L"Clock"},
	{TAGMap, L"Map"},
	{TAGScale, L"Scale"},
	{TAGAutoResult, L"AutoResult"},
	{TAGStartPoint, L"StartPoint"},
	{TAGStartPointCode, L"StartPointCode"},
	{TAGControlPosition, L"ControlPosition"},
	{TAGMapPosition, L"MapPosition"},
	{TAGControl, L"Control"},
	{TAGControlCode, L"ControlCode"},
	{TAGFinishPoint, L"FinishPoint"},
	{TAGFinishPointCode, L"FinishPointCode"},
	{TAGCourse, L"Course"},
	{TAGCourseName, L"CourseName"},
	{TAGCourseId, L"CourseId"},
	{TAGClassShortName, L"ClassShortName"},
	{TAGCourseVariation, L"CourseVariation"},
	{TAGCourseVariationId, L"CourseVariationId"},
	{TAGCourseLength, L"CourseLength"},
	{TAGCourseClimb, L"CourseClimb"},
	{TAGCourseControl, L"CourseControl"},
	{TAGSequence, L"Sequence"},
	{TAGLegLength, L"LegLength"},
	{TAGDistanceToFinish, L"DistanceToFinish"},
	{TAGIOFVersion, L"IOFVersion"},
	{TAGMultiRace, L"MultiRace"},
	{TAGPunchingUnitId, L"PunchingUnitId"},
	{TAGUnitCode, L"UnitCode"},
	{TAGClearECard, L"ClearECard"},
	{TAGPublic, L"Public"},
	{TAGUseClassId, L"UseClassId"},
	{TAGStartQueue, L"StartQueue"},
	{TAGPointRules, L"PointRules"},
	{TAGRankBased, L"RankBased"},
	{TAGResultBased, L"ResultBased"},
	{TAGTotal, L"Total"},
	{TAGRaceCourseData, L"RaceCourseData"},
	{TAGPosition, L"Position"},
	{TAGCourseFamily, L"CourseFamily"},
	{TAGLength, L"Length"},
	{TAGClimb, L"Climb"},
	{TAGMapText, L"MapText"},
	{TAGMapTextPosition, L"MapTextPosition"},
	{TAGClassCourseAssignment, L"ClassCourseAssignment"},
	{TAGClassName, L"ClassName"},
	{TAGMapPositionTopLeft, L"MapPositionTopLeft"},
	{TAGMapPositionBottomRight, L"MapPositionBottomRight"},
	{TAGResultList, L"ResultList"},
	{TAGClassResult, L"ClassResult"},
	{TAGPersonResult, L"PersonResult"},
	{TAGPerson, L"Person"},
	{TAGScore, L"Score"},
	{TAGBibNumber, L"BibNumber"},
	{TAGFinishTime, L"FinishTime"},
	{TAGTime, L"Time"},
	{TAGTimeBehind, L"TimeBehind"},
	{TAGSplitTime, L"SplitTime"},
	{TAGPersonEntry, L"PersonEntry"},
	{TAGControlCard, L"ControlCard"},
	{TAGRaceNumber, L"RaceNumber"},
	{TAGShortName, L"ShortName"},
	{TAGNationality, L"Nationality"},
	{TAGEntryList, L"EntryList"}
	};

static int nIOF3Tags = sizeof(IOF3Tags)/sizeof(IOF3Tags[0]);

static int loadIOF30CourseControl(IOFCourseVariation *CVar, xml_node *node, int *inode, int nnode)
{
	int er = 0, DepthIn, iType = 0, haara = 0;
	wchar_t ln[100] = L"";
	IOFCourseControl CC;

	memset(&CC, 0, sizeof(CC));
	node[*inode].getparam_text(L"type", ln, sizeof(ln)/2-1);
	if (ln[0]) {
		for (iType = 0; iType < sizeof(ControlType)/sizeof(ControlType[0]); iType++)
			if (wcscmp(ln, ControlType[iType]) == 0)
				break;
		}
	if (iType >= sizeof(ControlType)/sizeof(ControlType[0]))
		iType = 0;
	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			switch (node[*inode].tagid) {
				case TAGControl :
					node[*inode].gettext(CC.Code, sizeof(CC.Code)/2-1);
					break;
				case TAGMapText :
					break;
				case TAGMapTextPosition :
					break;
				case TAGLegLength :
					node[*inode].gettext_int(&CC.LegLength);
					break;
				}
			}
		}
	switch (iType) {
		int iPoint;
		case 0:
			for (iPoint = 0; iPoint < (int)(sizeof(CVar->CourseControl)/sizeof(CVar->CourseControl[0])); iPoint++)
				if (CVar->CourseControl[iPoint] == NULL)
					break;
			if (iPoint < (int)(sizeof(CVar->CourseControl)/sizeof(CVar->CourseControl[0]))) {
				CC.iPoint = iPoint+1;
				CVar->CourseControl[iPoint] = new IOFCourseControl;
				*CVar->CourseControl[iPoint] = CC;
				}
			break;
		case 1:
			wcsncpy(CVar->Start, CC.Code, sizeof(CVar->Start)/2-1);
			CVar->StartLeg = CC.LegLength;
			break;
		case 2:
			wcsncpy(CVar->Finish, CC.Code, sizeof(CVar->Finish)/2-1);
			CVar->FinLeg = CC.LegLength;
			CVar->ajanottotapa = 1;
			break;
		}
	return(er);
}

static int loadIOF30Course(IOFCourseData *CseData, int icourse, xml_node *node, int *inode, int nnode)
{
	int er = 0, iVar = 0, DepthIn;
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
				case TAGName :
					node[*inode].gettext(Cs.Name, sizeof(Cs.Name)/2-1);
					wcsncpy(CVar.Name, Cs.Name, sizeof(CVar.Name)/2-1);
					break;
				case TAGId :
					node[*inode].gettext(Cs.Id, sizeof(Cs.Id)/2-1);
					wcsncpy(CVar.Id, Cs.Id, sizeof(CVar.Id)/2-1);
					break;
				case TAGCourseFamily :
					break;
				case TAGLength :
					node[*inode].gettext_int(&CVar.Length);
					break;
				case TAGClimb :
					node[*inode].gettext_int(&CVar.Climb);
					break;
				case TAGCourseControl :
					if ((er = loadIOF30CourseControl(&CVar, node, inode, nnode)) != 0)
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

static int loadIOF30PointData(IOFCourseData *CseData, int ipoint, xml_node *node, int *inode, int nnode)
{
	int er = 0, DepthIn, haara, iType = 0, iC = 0;
	wchar_t ln[100];
	IOFPoint Pt;

	memset(&Pt, 0, sizeof(Pt));
	node[*inode].getparam_text(L"type", ln, sizeof(ln)/2-1);
	if (ln[0]) {
		for (iType = 0; iType < sizeof(ControlType)/sizeof(ControlType[0]); iType++)
			if (wcscmp(ln, ControlType[iType]) == 0)
				break;
		}
	if (iType >= sizeof(ControlType)/sizeof(ControlType[0]))
		iType = 0;
	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			haara = 0;
			switch (node[*inode].tagid) {
				case TAGId :
					Pt.PointType = L"CSFCCCC"[iType];
					node[*inode].gettext(Pt.Code, sizeof(Pt.Code)/2-1);
					break;
				case TAGPunchingUnitId :
					if (iC < (int)(sizeof(Pt.ECodes)/sizeof(Pt.ECodes[0])))
						node[*inode].gettext_int(&Pt.ECodes[iC++]);
					break;
				case TAGPosition :
					node[*inode].getparam_double(L"lat", &Pt.CY);
					node[*inode].getparam_double(L"lng", &Pt.CX);
					break;
				case TAGMapPosition :
					node[*inode].getparam_double(L"y", &Pt.MapY);
					node[*inode].getparam_double(L"x", &Pt.MapX);
					break;
				}
			}
		}
	CseData->Point[ipoint] = new IOFPoint;
	*CseData->Point[ipoint] = Pt;
	return(er);
}

static int loadIOF30CourseData(xml_node *node, int nnode, IOFCourseData *CseData)
{
	int er = 0, inode, DepthIn, haara = 0, ipoint = 0, icourse = 0, iassign = -1;

	CseData->nollaa();
	XMLhaenodeid(node, nnode, IOF3Tags, nIOF3Tags);
	DepthIn = node[0].depth + 1;
	for (inode = 1; inode <= nnode; inode++) {
		if (node[inode].depth == DepthIn) {
			haara = 0;
			switch (node[inode].tagid) {
				case TAGMap :
					haara = 2;
					break;
				case TAGControl :
					if ((er = loadIOF30PointData(CseData, ipoint, node, &inode, nnode)) != 0)
						return(er);
					ipoint++;
					break;
				case TAGCourse :
					if ((er = loadIOF30Course(CseData, icourse, node, &inode, nnode)) != 0)
						return(er);
					icourse++;
					break;
				case TAGClassCourseAssignment :
					if (iassign < MAXSARJALUKU-1) {
						haara = 1;
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
					case TAGClassName :
						node[inode].gettext(CseData->ClassAssignment[iassign]->Class, LSARJA);
						break;
					case TAGCourseName :
						node[inode].gettext(CseData->ClassAssignment[iassign]->Course, sizeof(CseData->ClassAssignment[0]->Course)/2-1);
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

static int loadIOF30PersonEntry(xml_node *node, int *inode, int nnode)
{
	int er = 0, i, haara, DepthIn;
	double val;
	wchar_t ln[100];
	kilptietue Kilp;

	XMLhaenodeid(node, nnode, IOF3Tags, nIOF3Tags);
	Kilp.nollaa();
	for (int i = 0; i < kilpparam.n_pv_akt; i++)
		Kilp.set_tark(L'P', i);
	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			haara = 0;
			switch (node[*inode].tagid) {
				case TAGPerson :
					haara = 1;
					break;
				case TAGOrganisation :
					haara = 2;
					break;
				case TAGControlCard :
					node[*inode].gettext_int(&i);
					if (i) {
						for (int k = 0; k < 2; k++) {
							if (Kilp.pv[0].badge[k] == 0) {
								for (int ipv = 0; ipv < kilpparam.n_pv_akt; ipv++)
									Kilp.pv[ipv].badge[k] = i;
								break;
								}
							}
						}
					break;
				case TAGScore :
					node[*inode].gettext_double(&val);
					node[*inode].getparam_text(L"type", ln, 98);
					if (val != 0 && wcslen(ln) > 0 && towupper(ln[0]) == L'R')
						Kilp.pisteet[2] = (int)(1000*val);
					break;
				case TAGId :
					node[*inode].gettext_int(&i);
					Kilp.kilpno = i;
					break;
				case TAGClass :
					haara = 3;
					break;
				case TAGRaceNumber :
					node[*inode].gettext_int(&i);
					Kilp.set_tark(L'-', i-1);
					break;
				}
			continue;
			}
		if (node[*inode].depth == DepthIn + 1) {
			if (haara == 1) {
				switch (node[*inode].tagid) {
					case TAGName :
						haara = 11;
						break;
					case TAGId :
						node[*inode].gettext_int(&i);
						node[*inode].getparam_text(L"type", ln, 98);
						if (wcslen(ln) > 0 && wcswcind(towupper(ln[0]), L"WIF") >= 0)
							Kilp.lisno[1] = i;
						else
							Kilp.lisno[0] = i;
						break;
					case TAGNationality :
						node[*inode].getparam_text(L"code", Kilp.maa, 4);
						break;
					}
				continue;
				}
			if (haara == 2) {
				switch (node[*inode].tagid) {
					case TAGName :
						node[*inode].gettext(Kilp.seura, kilpparam.lseura+1);
						break;
					case TAGShortName :
						node[*inode].gettext(Kilp.seuralyh, kilpparam.lseuralyh+1);
						break;
					case TAGCountry :
						node[*inode].getparam_text(L"code", Kilp.maa, 4);
						break;
					}
				continue;
				}
			if (haara == 3) {
				switch (node[*inode].tagid) {
					case TAGName :
						node[*inode].gettext(ln, sizeof(ln)/2);
						i = haesarja_w(ln, false);
						if (i >= 0)
							Kilp.sarja = i;
						break;
					case TAGShortName :
						node[*inode].gettext(ln, sizeof(ln)/2);
						i = haesarja_w(ln, false);
						if (i >= 0)
							Kilp.sarja = i;
						break;
					}
				continue;
				}
			continue;
			}
		if (node[*inode].depth == DepthIn + 2) {
			if (haara == 11) {
				switch (node[*inode].tagid) {
					case TAGFamily :
						node[*inode].gettext(Kilp.sukunimi, kilpparam.lsnimi+1);
						break;
					case TAGGiven :
						node[*inode].gettext(Kilp.etunimi, kilpparam.lenimi+1);
						break;
					}
				continue;
				}
			continue;
			}
		}
	if (!er) {
		for (i = 0; i < kilpparam.n_pv_akt; i++)
			if (Kilp.tark(i) != L'P')
				break;
		if (i == kilpparam.n_pv_akt) {
			for (i = Sarjat[Kilp.Sarja(-1)].enspv; i <= Sarjat[Kilp.Sarja(-1)].viimpv; i++)
				Kilp.set_tark(L'-', i);
		}

		EnterCriticalSection(&tall_CriticalSection);
		Kilp.TallEhdoin(0);
		LeaveCriticalSection(&tall_CriticalSection);
		}
	return(er);
}

static int loadIOF30EntryData(xml_node *node, int nnode)
{
	int er = 0, inode, inode_in, DepthIn, haara = 0;

	XMLhaenodeid(node, nnode, IOF3Tags, nIOF3Tags);
	DepthIn = node[0].depth + 1;
	for (inode = 1; inode <= nnode; inode++) {
		if (node[inode].depth == DepthIn) {
			haara = 0;
			inode_in = inode;
			switch (node[inode].tagid) {
				case TAGPersonEntry :
					if ((er = loadIOF30PersonEntry(node, &inode, nnode)) != 0)
						return(er);
					break;
				}
			continue;
			}
		}
	return(er);
}

int lueIOF30VainRadatXml(wchar_t *filename, IOFCourseData *CseData)
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
				nd.nodeno[i] = k[i];           // merkitään eri tasojen järjestysnumerot solmuun
			if (lntype > 0)
				nd.tagid = XMLhae_tagid(nd.tag, IOF3Tags, nIOF3Tags);

			if (lntype == 1) {
				tree.node[++inode] = nd;
				k[depth++]++;
				if (depth == 2) {
					inode_in = inode;
					switch (nd.tagid) {
						case TAGRaceCourseData :
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er)
								loadIOF30CourseData(tree.node+inode_in, inode-inode_in, CseData);
							inode = inode_in-1;
							break;
						case TAGEvent :
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							break;
						}
					continue;
					}
				if (depth > 2) {
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
	if (depth) {
		er = 1;
		swprintf(msg, L"XML-tiedosto %s päättyi rivillä %d tasolla %d", filename, rv, depth);
		}
	if (er) {
		if (!msg[0]) {
			swprintf(msg, L"Tiedostossa %s virhe rivillä %d tai aikaisemmin", filename, rv);
			}
		writeerror_w(msg, 0, true);
		}
	tree.nodecount = inode+1;
	return(er);
}

int lueIOF30EventXml(wchar_t *filename, bool lueSarjat, bool lueRadat, bool lueOsanottajat)
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
				nd.nodeno[i] = k[i];           // merkitään eri tasojen järjestysnumerot solmuun
			if (lntype > 0)
				nd.tagid = XMLhae_tagid(nd.tag, IOF3Tags, nIOF3Tags);
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
								loadIOF30CourseData(tree.node+inode_in, inode-inode_in, &CourseData);
							inode = inode_in-1;
							break;
						case TAGEntryList :
							haara = 0;
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er && lueOsanottajat)
								loadIOF30EntryData(tree.node+inode_in, inode-inode_in);
							inode = inode_in-1;
							break;
						}
					continue;
					}
				if (depth == 2) {
					inode_in = inode;
					switch (nd.tagid) {
						case TAGCourseData :
							haara = 0;
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er && lueRadat)
								loadIOF30CourseData(tree.node+inode_in, inode-inode_in, &CourseData);
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
				if (depth > 2) {
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
	if (depth) {
		er = 1;
		swprintf(msg, L"XML-tiedosto %s päättyi rivillä %d tasolla %d", filename, rv, depth);
		}
	if (er) {
		if (!msg[0]) {
			swprintf(msg, L"Tiedostossa %s virhe rivillä %d tai aikaisemmin", filename, rv);
			}
		writeerror_w(msg, 0, true);
		}
	tree.nodecount = inode+1;
	return(er);
}

void kirjIOF30XmlRadat(TextFl *outfl, int level, IOFCourseData *CseData)
{
	wchar_t ln[400], ln2[100];

	outfl->put_wtag(XMLhae_tagName(TAGCourseData, IOF3Tags, nIOF3Tags), level++);
	swprintf(ln, L"version=\"%s\"", CseData->IOFVersion);
	outfl->put_wtagparams(XMLhae_tagName(TAGIOFVersion, IOF3Tags, nIOF3Tags), ln, true, level);
	if (CseData->ModifyDate[0]) {
		outfl->put_wtag(XMLhae_tagName(TAGModifyDate, IOF3Tags, nIOF3Tags), level++);
		outfl->put_wxml_s(XMLhae_tagName(TAGDate, IOF3Tags, nIOF3Tags), CseData->ModifyDate, level);
		if (CseData->ModifyDate[0])
			outfl->put_wxml_s(XMLhae_tagName(TAGClock, IOF3Tags, nIOF3Tags), CseData->ModifyTime, level);
		outfl->put_wantitag(XMLhae_tagName(TAGModifyDate, IOF3Tags, nIOF3Tags), --level);
		}
	if (CseData->MapScale || CseData->MapTY != 0 || CseData->MapLX != 0 || CseData->MapBY != 0 || CseData->MapRX != 0) {
		outfl->put_wtag(XMLhae_tagName(TAGMap, IOF3Tags, nIOF3Tags), level++);
		if (CseData->MapScale)
			outfl->put_wxml_d(XMLhae_tagName(TAGScale, IOF3Tags, nIOF3Tags), CseData->MapScale, level);
		if (CseData->MapTY != 0 || CseData->MapLX != 0) {
			swprintf(ln, L"x=\"%.1f\" y=\"%.1f\"", CseData->MapLX, CseData->MapTY);
			outfl->put_wtagparams(XMLhae_tagName(TAGMapPositionTopLeft, IOF3Tags, nIOF3Tags), ln, true, level);
			}
		if (CseData->MapBY != 0 || CseData->MapRX != 0) {
			swprintf(ln, L"x=\"%.1f\" y=\"%.1f\"", CseData->MapRX, CseData->MapBY);
			outfl->put_wtagparams(XMLhae_tagName(TAGMapPositionBottomRight, IOF3Tags, nIOF3Tags), ln, true, level);
			}
		outfl->put_wantitag(XMLhae_tagName(TAGMap, IOF3Tags, nIOF3Tags), --level);
		}
	if (CseData->Point[0]) {
		for (int i = 0 ; i < MAXNPISTE && CseData->Point[i]; i++) {
			ln[0] = 0;
			ln2[0] = 0;
			switch (CseData->Point[i]->PointType) {
				case L'S':
					wcscpy(ln, XMLhae_tagName(TAGStartPoint, IOF3Tags, nIOF3Tags));
					wcscpy(ln2, XMLhae_tagName(TAGStartPointCode, IOF3Tags, nIOF3Tags));
					break;
				case L'C':
					wcscpy(ln, XMLhae_tagName(TAGControl, IOF3Tags, nIOF3Tags));
					wcscpy(ln2, XMLhae_tagName(TAGControlCode, IOF3Tags, nIOF3Tags));
					break;
				case L'F':
					wcscpy(ln, XMLhae_tagName(TAGFinishPoint, IOF3Tags, nIOF3Tags));
					wcscpy(ln2, XMLhae_tagName(TAGFinishPointCode, IOF3Tags, nIOF3Tags));
					break;
				}
			outfl->put_wtag(ln, level++);
			outfl->put_wxml_s(ln2, CseData->Point[i]->Code, level);
			if (CseData->Point[i]->CY) {
				swprintf(ln2, L"x=\"%.3f\" y=\"%.3f\"", CseData->Point[i]->CX, CseData->Point[i]->CY);
				outfl->put_wtagparams(XMLhae_tagName(TAGControlPosition, IOF3Tags, nIOF3Tags), ln2, true, level);
				}
			if (CseData->Point[i]->MapY) {
				swprintf(ln2, L"x=\"%.3f\" y=\"%.3f\"", CseData->Point[i]->MapX, CseData->Point[i]->MapY);
				outfl->put_wtagparams(XMLhae_tagName(TAGMapPosition, IOF3Tags, nIOF3Tags), ln2, true, level);
				}
			if (CseData->Point[i]->ECodes[0]) {
				outfl->put_wtag(XMLhae_tagName(TAGPunchingUnitId, IOF3Tags, nIOF3Tags), level++);
				for (unsigned int iC = 0; iC < sizeof(CseData->Point[0]->ECodes)/sizeof(CseData->Point[0]->ECodes[0]);
					iC++)
					if (CseData->Point[i]->ECodes[iC] > 0)
						outfl->put_wxml_d(XMLhae_tagName(TAGUnitCode, IOF3Tags, nIOF3Tags), CseData->Point[i]->ECodes[iC], level);
				outfl->put_wantitag(XMLhae_tagName(TAGPunchingUnitId, IOF3Tags, nIOF3Tags), --level);
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
			outfl->put_wtag(XMLhae_tagName(TAGCourse, IOF3Tags, nIOF3Tags), level++);
			outfl->put_wxml_s(XMLhae_tagName(TAGCourseName, IOF3Tags, nIOF3Tags), Crse->Name, level);
			outfl->put_wxml_s(XMLhae_tagName(TAGCourseId, IOF3Tags, nIOF3Tags), Crse->Id, level);
			for (unsigned int j = 0; j < sizeof(Crse->ShortName)/sizeof(Crse->ShortName[0]) && Crse->ShortName[j][0]; j++)
				outfl->put_wxml_s(XMLhae_tagName(TAGClassShortName, IOF3Tags, nIOF3Tags), Crse->ShortName[j], level);
			for (unsigned int j = 0; j < sizeof(Crse->CourseVariation)/sizeof(Crse->CourseVariation[0]) &&
				Crse->CourseVariation[j]; j++) {
				IOFCourseVariation *CVar = Crse->CourseVariation[j];
				outfl->put_wtag(XMLhae_tagName(TAGCourseVariation, IOF3Tags, nIOF3Tags), level++);
				outfl->put_wxml_s(XMLhae_tagName(TAGCourseVariationId, IOF3Tags, nIOF3Tags), CVar->Id, level);
				if (CVar->Name[0])
					outfl->put_wxml_s(XMLhae_tagName(TAGName, IOF3Tags, nIOF3Tags), CVar->Name, level);
				outfl->put_wxml_d(XMLhae_tagName(TAGCourseLength, IOF3Tags, nIOF3Tags), CVar->Length, level);
				outfl->put_wxml_d(XMLhae_tagName(TAGCourseClimb, IOF3Tags, nIOF3Tags), CVar->Climb, level);
				outfl->put_wxml_d(XMLhae_tagName(TAGClearECard, IOF3Tags, nIOF3Tags), CVar->ennakko, level);
				outfl->put_wxml_s(XMLhae_tagName(TAGAutoResult, IOF3Tags, nIOF3Tags), CVar->ajanottotapa == 4 ? L"Yes" : L"No", level);
				if (CVar->Start[0])
					outfl->put_wxml_s(XMLhae_tagName(TAGStartPointCode, IOF3Tags, nIOF3Tags), CVar->Start, level);
				for (unsigned int k = 0; k < sizeof(CVar->CourseControl)/sizeof(CVar->CourseControl[0])
					&& CVar->CourseControl[k]; k++) {
					outfl->put_wtag(XMLhae_tagName(TAGCourseControl, IOF3Tags, nIOF3Tags), level++);
					outfl->put_wxml_d(XMLhae_tagName(TAGSequence, IOF3Tags, nIOF3Tags), k+1, level);
					outfl->put_wxml_s(XMLhae_tagName(TAGControlCode, IOF3Tags, nIOF3Tags), CVar->CourseControl[k]->Code, level);
					if (CVar->CourseControl[k]->LegLength)
						outfl->put_wxml_d(XMLhae_tagName(TAGLegLength, IOF3Tags, nIOF3Tags), CVar->CourseControl[k]->LegLength, level);
					outfl->put_wantitag(XMLhae_tagName(TAGCourseControl, IOF3Tags, nIOF3Tags), --level);
					}
				if (CVar->Finish[0]) {
					outfl->put_wxml_s(XMLhae_tagName(TAGFinishPointCode, IOF3Tags, nIOF3Tags), CVar->Finish, level);
					if (CVar->FinLeg)
						outfl->put_wxml_d(XMLhae_tagName(TAGDistanceToFinish, IOF3Tags, nIOF3Tags), CVar->FinLeg, level);
					}
				outfl->put_wantitag(XMLhae_tagName(TAGCourseVariation, IOF3Tags, nIOF3Tags), --level);
				}
			outfl->put_wantitag(XMLhae_tagName(TAGCourse, IOF3Tags, nIOF3Tags), --level);
			}
		}
	outfl->put_wantitag(XMLhae_tagName(TAGCourseData, IOF3Tags, nIOF3Tags), --level);
}

void kirjIOF30VainRadatXml(wchar_t *outfname, IOFCourseData *CseData)
{
	TextFl *outfl;
	INT   level = 0;

	outfl = new TextFl(outfname, L"wt");
	if (outfl->IsOpen()) {
		outfl->WriteLine(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
		outfl->put_wtag(XMLhae_tagName(TAGEvent, IOF3Tags, nIOF3Tags), level++);
		kirjIOF30XmlRadat(outfl, level, CseData);
		outfl->put_wantitag(XMLhae_tagName(TAGEvent, IOF3Tags, nIOF3Tags), --level);
		}
	delete outfl;
}

//=============== IOF ResultList ================================

int xmlIOF30ots(tulostusparamtp *tulprm)
   {
   wchar_t ln[602], level = 0;
   TextFl *outfl;


   outfl = tulprm->lstf->u.Fl;

	if (wcslen(kilpparam.kilpkoodi) == 0) {
		wcscpy(kilpparam.kilpkoodi, L"KOODI");
		}
	outfl->WriteLine(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
	swprintf(ln, L"%s xmlns=\"http://www.orienteering.org/datastandard/3.0\"\n"
			L"\txmlns:xsi=\"http://www.w3.org/2001/XMLSchema-instance\"\n"
			L"\tiofVersion=\"3.0\"\n"
			L"\tcreateTime=\"%s\"\n"
			L"\tcreator=\"HkKisaWin %s\"\n"
			L"\tstatus=\"%s\"",
		XMLhae_tagName(TAGResultList, IOF3Tags, nIOF3Tags), ISOdatetime(0,1), Build(), L"Complete");
	outfl->put_wtag(ln, level++);

	outfl->put_wtag(XMLhae_tagName(TAGEvent, IOF3Tags, nIOF3Tags), level++);
	outfl->put_wxml_s(XMLhae_tagName(TAGId, IOF3Tags, nIOF3Tags), pvparam[k_pv].RaceId, level);
	outfl->put_wxml_s(XMLhae_tagName(TAGName, IOF3Tags, nIOF3Tags), pvparam[k_pv].Title, level);
	outfl->put_wtag(XMLhae_tagName(TAGStartTime, IOF3Tags, nIOF3Tags), level++);
	outfl->put_wxml_s(XMLhae_tagName(TAGDate, IOF3Tags, nIOF3Tags), stDateNo(NULL, pvparam[k_pv].Date), level);
//	outfl->put_wxml_s(srjtag[TAGTime], , level);
	outfl->put_wantitag(XMLhae_tagName(TAGStartTime, IOF3Tags, nIOF3Tags), --level);
/*
	outfl->put_wtag(srjtag[TAGEndTime], level++);
	outfl->put_wxml_s(srjtag[TAGDate], kilpailu, level);
	outfl->put_wxml_s(srjtag[TAGTime], kilpailu, level);
	outfl->put_wantitag(srjtag[TAGEndTime], --level);
*/
	outfl->put_wantitag(XMLhae_tagName(TAGEvent, IOF3Tags, nIOF3Tags), --level);

   return(0);
   }

int xmlIOF30loppu(tulostusparamtp *tulprm)
   {
   TextFl *tul_tied;

   tul_tied = tulprm->lstf->u.Fl;
   tul_tied->put_wantitag(XMLhae_tagName(TAGResultList, IOF3Tags, nIOF3Tags), 0);
   return(0);
   }

int xmlIOF30srjots(int sarja, tulostusparamtp *tulprm)
   {
   int level = 1;
   TextFl *tul_tied;
   ratatp *rt;

   tul_tied = tulprm->lstf->u.Fl;
   tul_tied->put_wtag(XMLhae_tagName(TAGClassResult, IOF3Tags, nIOF3Tags), level++);

   tul_tied->put_wtag(XMLhae_tagName(TAGClass, IOF3Tags, nIOF3Tags), level++);
   tul_tied->put_wxml_s(XMLhae_tagName(TAGId, IOF3Tags, nIOF3Tags), Sarjat[sarja].sarjanimi, level);
   if (Sarjat[sarja].psarjanimi[0])
		tul_tied->put_wxml_s(XMLhae_tagName(TAGName, IOF3Tags, nIOF3Tags), Sarjat[sarja].psarjanimi, level);
   else
		tul_tied->put_wxml_s(XMLhae_tagName(TAGName, IOF3Tags, nIOF3Tags), Sarjat[sarja].sarjanimi, level);
   tul_tied->put_wantitag(XMLhae_tagName(TAGClass, IOF3Tags, nIOF3Tags), --level);

   if ((rt = haerata_sarja(sarja, k_pv)) != NULL) {
	   tul_tied->put_wtag(XMLhae_tagName(TAGCourse, IOF3Tags, nIOF3Tags), level++);
	   tul_tied->put_wxml_d(XMLhae_tagName(TAGLength, IOF3Tags, nIOF3Tags), rt->ratapit, level);
	   if (rt->nousu)
		   tul_tied->put_wxml_d(XMLhae_tagName(TAGLength, IOF3Tags, nIOF3Tags), rt->nousu, level);
	   tul_tied->put_wantitag(XMLhae_tagName(TAGCourse, IOF3Tags, nIOF3Tags), --level);
	   }

   return(0);
   }

int xmlIOF30srjloppu(tulostusparamtp *tulprm)
   {
   TextFl *tul_tied;

   tul_tied = tulprm->lstf->u.Fl;
   tul_tied->put_wantitag(XMLhae_tagName(TAGClassResult, IOF3Tags, nIOF3Tags), 1);
   return(0);
   }

void xmlIOF30tulos(kilptietue& kilp, INT sj, tulostusparamtp *tulprm)
   {
   wchar_t ln[100];
   INT piste = 0, level = 1;
   emittp em;
   ratatp *rt;
   TextFl *tul_tied;
   static INT32 karki;

   if (sj == 1)
	   karki = kilp.tulos2(0);
   tul_tied = tulprm->lstf->u.Fl;

   tul_tied->put_wtag(XMLhae_tagName(TAGPersonResult, IOF3Tags, nIOF3Tags), level++);

   tul_tied->put_wtag(XMLhae_tagName(TAGPerson, IOF3Tags, nIOF3Tags), level++);
   if (kilp.lisno[1]) {
	   tul_tied->put_wxml_d(XMLhae_tagName(TAGId, IOF3Tags, nIOF3Tags), kilp.lisno[1], level, L"type=\"IOF\"");
	   }
   if (tulprm->xmlstd != L'J')
	   tul_tied->put_wxml_d(XMLhae_tagName(TAGId, IOF3Tags, nIOF3Tags), kilp.lisno[0], level, L"type=\"local\"");
//   if (kilp.lisno[1] && kilp.maa[0]) {
//		swprintf(ln, L"%s%d", kilp.maa, kilp.lisno[1]);
//	   tul_tied->put_wxml_s(srjtag[TAGId], ln, level, L"type=\"WRE\"");
//		}
   tul_tied->put_wtag(XMLhae_tagName(TAGName, IOF3Tags, nIOF3Tags), level++);
   tul_tied->put_wxml_s(XMLhae_tagName(TAGFamily, IOF3Tags, nIOF3Tags), kilp.sukunimi, level);
   tul_tied->put_wxml_s(XMLhae_tagName(TAGGiven, IOF3Tags, nIOF3Tags), kilp.etunimi, level);
   tul_tied->put_wantitag(XMLhae_tagName(TAGName, IOF3Tags, nIOF3Tags), --level);
   tul_tied->put_wantitag(XMLhae_tagName(TAGPerson, IOF3Tags, nIOF3Tags), --level);

   if (tulprm->xmlstd != L'J') {
	   tul_tied->put_wtag(XMLhae_tagName(TAGOrganisation, IOF3Tags, nIOF3Tags), level++);
	   tul_tied->put_wxml_s(XMLhae_tagName(TAGId, IOF3Tags, nIOF3Tags), kilp.seuralyh, level);
	   tul_tied->put_wxml_s(XMLhae_tagName(TAGName, IOF3Tags, nIOF3Tags), kilp.seura, level);
	   if (kilp.maa[0]) {
		   swprintf(ln, L"code=\"%s\"", kilp.maa);
		   tul_tied->put_wxml_s(XMLhae_tagName(TAGCountry, IOF3Tags, nIOF3Tags), MaaNimi(kilp.maa), level, ln);
		   }
	   tul_tied->put_wantitag(XMLhae_tagName(TAGOrganisation, IOF3Tags, nIOF3Tags), --level);
	   }

   tul_tied->put_wtag(XMLhae_tagName(TAGResult, IOF3Tags, nIOF3Tags), level++);
   tul_tied->put_wxml_d(XMLhae_tagName(TAGBibNumber, IOF3Tags, nIOF3Tags), kilp.id(), level);
   if (kilp.TLahto(0) != TMAALI0)
		tul_tied->put_wxml_time(XMLhae_tagName(TAGStartTime, IOF3Tags, nIOF3Tags), pvparam[k_pv].Date, kilp.TLahto(), t0, SEK, 8, L'.', level);
   if (kilp.maali(0) != TMAALI0)
		tul_tied->put_wxml_time(XMLhae_tagName(TAGFinishTime, IOF3Tags, nIOF3Tags), pvparam[k_pv].Date + (kilp.maali(0) < kilp.TLahto() ? 1 : 0),
			kilp.maali(0), t0, SEK, pvparam[k_pv].laika, L'.', level);
   if (kilp.hyv() && kilp.tulos2(0)) {
	   tul_tied->put_wxml_s(XMLhae_tagName(TAGTime, IOF3Tags, nIOF3Tags), sekTulos(NULL, kilp.tulos2(0), pvparam[k_pv].pyor[3]), level);
	   tul_tied->put_wxml_s(XMLhae_tagName(TAGTimeBehind, IOF3Tags, nIOF3Tags), sekTulos(NULL, kilp.tulos2(0)-karki, pvparam[k_pv].pyor[3]), level);
	   if (sj >= 1)
		   tul_tied->put_wxml_d(XMLhae_tagName(TAGPosition, IOF3Tags, nIOF3Tags), sj, level);
	   }
   tul_tied->put_wxml_s(XMLhae_tagName(TAGStatus, IOF3Tags, nIOF3Tags), IOFStatus(&kilp), level);
   if (kilp.pv[k_pv].rata[0] && (rt = haerata(&kilp)) != NULL &&
		!wcscmp(kilp.pv[k_pv].rata, rt->tunnus)) {
	   tul_tied->put_wtag(XMLhae_tagName(TAGCourse, IOF3Tags, nIOF3Tags), level++);
	   tul_tied->put_wxml_s(XMLhae_tagName(TAGName, IOF3Tags, nIOF3Tags), kilp.pv[k_pv].rata, level);

	   if (rt) {
		   tul_tied->put_wxml_d(XMLhae_tagName(TAGLength, IOF3Tags, nIOF3Tags), rt->ratapit, level);
		   if (rt->nousu)
			   tul_tied->put_wxml_d(XMLhae_tagName(TAGLength, IOF3Tags, nIOF3Tags), rt->nousu, level);
		   }
	   tul_tied->put_wantitag(XMLhae_tagName(TAGCourse, IOF3Tags, nIOF3Tags), --level);
	   }
   if (tulprm->tulostettava == L'E') {
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
					tul_tied->put_wtag(XMLhae_tagName(TAGSplitTime, IOF3Tags, nIOF3Tags), level++);
				else
					tul_tied->put_wtagparams(XMLhae_tagName(TAGSplitTime, IOF3Tags, nIOF3Tags), L"status=\"Additional\"", false, level++);
				tul_tied->put_wxml_d(XMLhae_tagName(TAGControlCode, IOF3Tags, nIOF3Tags), emva.rastit[r][0], level);
				tul_tied->put_wxml_d(XMLhae_tagName(TAGTime, IOF3Tags, nIOF3Tags), emva.rastit[r][1], level);
				}
			 else {
				if (leim_ok)
					tul_tied->put_wtag(XMLhae_tagName(TAGSplitTime, IOF3Tags, nIOF3Tags), level++);
				else
					tul_tied->put_wtagparams(XMLhae_tagName(TAGSplitTime, IOF3Tags, nIOF3Tags), L"status=\"Missing\"", false, level++);
				if (rt)
					tul_tied->put_wxml_d(XMLhae_tagName(TAGControlCode, IOF3Tags, nIOF3Tags), rt->rastikoodi[r], level);
				}
			tul_tied->put_wantitag(XMLhae_tagName(TAGSplitTime, IOF3Tags, nIOF3Tags), --level);
			}
		 }
	  }
   tul_tied->put_wantitag(XMLhae_tagName(TAGResult, IOF3Tags, nIOF3Tags), --level);
   tul_tied->put_wantitag(XMLhae_tagName(TAGPersonResult, IOF3Tags, nIOF3Tags), --level);
   }

