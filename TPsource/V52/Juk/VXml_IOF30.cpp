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

extern INT tarkkuusparam;
extern bool kirjXMLkaikki;
void TallEhdoin(kilptietue *kilp, int pos);

// static wchar_t Stat[10] = L"-THKEOIVP", *StatSt[9] = {L"Open", L"OK", L"DQ", L"DNF", L"DNS", L"LAP", L"Open", L"Vacant", L"Absent"};
// static wchar_t Bdg[9] = L"ETS", *BdgSt[3] = {L"Emit", L"emiTag", L"Sirit"};
// static wchar_t *Pr[4] = {L"1", L"0,1", L"0,01", L"0,001"};
// static wchar_t *Tabs[8] = {L"", L"\t", L"\t\t", L"\t\t\t", L"\t\t\t\t", L"\t\t\t\t\t", L"\t\t\t\t\t\t", L"\t\t\t\t\t\t\t"};
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
#define TAGTeamEntry     	   166
#define TAGTeamEntryPerson     167
#define TAGLeg                 168
#define TAGLegOrder            169
#define TAGTeamResult          170
#define TAGTeamMemberResult    171
#define TAGOverallResult       172

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
	{TAGEntryList, L"EntryList"},
	{TAGTeamEntry, L"TeamEntry"},
	{TAGTeamEntryPerson, L"TeamEntryPerson"},
	{TAGLeg, L"Leg"},
	{TAGLegOrder, L"LegOrder"},
	{TAGTeamResult, L"TeamResult"},
	{TAGTeamMemberResult, L"TeamMemberResult"},
	{TAGOverallResult, L"OverallResult"}
	};

static int nIOF3Tags = sizeof(IOF3Tags)/sizeof(IOF3Tags[0]);
	
#ifndef LUENTA
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
//				case TAGScore :
//					break;
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
//				case TAGMapId :
//					break;
				}
			}
		}
	if (Cs.CourseVariation[0] == NULL) {
		Cs.CourseVariation[0] = new IOFCourseVariation;
		*Cs.CourseVariation[0] = CVar;
		}
	if (icourse < (int)(sizeof(CseData->Course)/sizeof(CseData->Course[0]))) {
		CseData->Course[icourse] = new IOFCourse;
		*CseData->Course[icourse] = Cs;
		}
	else
		er = 1;
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
	if (ipoint < (int)(sizeof(CseData->Point)/sizeof(CseData->Point[0]))) {
		CseData->Point[ipoint] = new IOFPoint;
		*CseData->Point[ipoint] = Pt;
		}
	else
		er = 1;
	return(er);
}

static int loadIOF30CourseData(xml_node *node, int nnode, IOFCourseData *CseData)
{
	int er = 0, inode, DepthIn, haara = 0, ipoint = 0, icourse = 0, iassign = -1;
//	IOFCourseData *CseData;

//	CseData = new IOFCourseData;
//	CseData = &CourseData;
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
/*
				case TAGPersonCourseAssignment :
					break;
				case TAGTeamCourseAssignment :
					break;
*/
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

int lueIOF30VainRadatXml(wchar_t *filename, IOFCourseData *CseData)
{
	TextFl *infile;
	int ntag = 700000, haara = 0, inode, inode_in, depth = 0, rv, er = 0, lntype;
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
			swprintf(msg, L"Tiedostossa %s virhe rivill‰ %d tai aikaisemmin", filename, rv);
			}
		writeerror_w(msg, 0, true);
		}
	tree.nodecount = inode+1;
	return(er);
}
#endif // !LUENTA

static int loadIOF30PersonEntry(kilptietue &Kilp, xml_node *node, int *inode, int nnode)
{
	int er = 0, i, i_os = 1, a_os = 1, haara, DepthIn;
	wchar_t ln[100], enimi[100] = L"", snimi[100] = L"";
	osrec Os;

	XMLhaenodeid(node, nnode, IOF3Tags, nIOF3Tags);
	Os.nollaa();
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
				case TAGLeg :
					node[*inode].gettext_int(&i_os);
					break;
				case TAGLegOrder :
					node[*inode].gettext_int(&a_os);
					break;
				case TAGOrganisation :
					haara = 2;
					break;
				case TAGControlCard :
					node[*inode].gettext_int(&i);
					if (i) {
						if (Os.badge[0] == 0)
							Os.badge[0] = i;
						else if (Os.badge[1] == 0)
							Os.badge[1] = i;
						}
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
						Os.lisno = i;
						break;
					case TAGNationality :
						node[*inode].getparam_text(L"code", ln, 4);
						break;
					}
				continue;
				}
			if (haara == 2) {
				switch (node[*inode].tagid) {
					case TAGName :
						node[*inode].gettext(Os.osseura, sizeof(Os.osseura));
						break;
					case TAGCountry :
						node[*inode].getparam_text(L"code", ln, 4);
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
						node[*inode].gettext(snimi, sizeof(snimi)/2);
						break;
					case TAGGiven :
						node[*inode].gettext(enimi, sizeof(snimi)/2);
						break;
					}
				continue;
				}
			continue;
			}
		}
	Os.setNimi(enimi, snimi);
	if (i_os > 0 && i_os <= kilpparam.ntosuus && a_os > 0 && a_os <= kilpparam.maxnosuus) {
		i_os = Kilp.Osuus(i_os-1, a_os-1);
		if (i_os >= 0 && i_os < kilpparam.osuusluku)
			Kilp.ostiet[i_os] = Os;
		}
	return(er);
}

static int loadIOF30TeamEntry(xml_node *node, int *inode, int nnode)
{
	int er = 0, i, j, haara, DepthIn;
	wchar_t ln[100], jknm[100] = L"";
	kilptietue Kilp;
	bool yhdistelma = false;

	XMLhaenodeid(node, nnode, IOF3Tags, nIOF3Tags);
	Kilp.nollaa();
	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			haara = 0;
			switch (node[*inode].tagid) {
				case TAGTeamEntryPerson :
					if ((er = loadIOF30PersonEntry(Kilp, node, inode, nnode)) != 0)
						return(er);
					break;
				case TAGName :
					node[*inode].gettext(jknm, sizeof(jknm)/2);
					break;
				case TAGOrganisation :
					haara = 2;
					break;
				case TAGId :
					node[*inode].gettext_int(&i);
					Kilp.kilpno = i;
					break;
				case TAGClass :
					haara = 3;
					break;
				}
			continue;
			}
		if (node[*inode].depth == DepthIn + 1) {
			if (haara == 2) {
				switch (node[*inode].tagid) {
					case TAGName :
						Kilp.Seura(ln, sizeof(ln)/2-1);
						if (wcslen(ln) > 0 && (int) wcslen(ln) < kilpparam.lseura) {
							yhdistelma = true;
							wcscat(ln, L"/");
							}
						node[*inode].gettext(ln+wcslen(ln), sizeof(ln)/2-wcslen(ln));
						Kilp.setSeura(ln);
						break;
					case TAGCountry :
						node[*inode].getparam_text(L"code", ln, 4);
						Kilp.setMaa(ln);
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
		}
	if (!er) {
		Kilp.Seura(ln, sizeof(ln)/2-1);
		if (!yhdistelma && wcslen(jknm) > wcslen(ln) && wmemcmp(jknm, ln, wcslen(ln)) == 0) {
			wchar_t *p = jknm + strlen(Kilp.seura);
			while (*p && *p < L'0')
				p++;
			if (*p && (j = _wtoi(p)) > 0)
				Kilp.joukkue = j;
			}
		else {
			wchar_t *p = jknm + wcslen(jknm) - 1;
			if (jknm[wcslen(jknm)-1] >= L'0' && jknm[wcslen(jknm)-1] <= L'9' &&
				jknm[wcslen(jknm)-1] != ln[wcslen(ln)-1]) {
				while (p > jknm && p[-1] >= L'0' && p[-1] <= L'9')
				p--;
				p[-1] = 0;
				if ((j = _wtoi(p)) > 0)
					Kilp.joukkue = j;
				}
			elimwbl(jknm);
			if (yhdistelma && (wcslen(jknm) > wcslen(ln) || wcslen(ln) == kilpparam.lseura))
				Kilp.setSeura(jknm);
			}
		EnterCriticalSection(&tall_CriticalSection);
		TallEhdoin(&Kilp, 0);
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
				case TAGTeamEntry :
					if ((er = loadIOF30TeamEntry(node, &inode, nnode)) != 0)
						return(er);
					break;
				}
			continue;
			}
		}
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
				nd.nodeno[i] = k[i];           // merkit‰‰n eri tasojen j‰rjestysnumerot solmuun
			if (lntype > 0)
				nd.tagid = XMLhae_tagid(nd.tag, IOF3Tags, nIOF3Tags);
			if (lntype == 1) {
				tree.node[++inode] = nd;
				k[depth++]++;
				if (depth == 1) {
					inode_in = inode;
					switch (nd.tagid) {
#ifndef LUENTA
						case TAGCourseData :
							haara = 0;
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er && lueRadat)
								loadIOF30CourseData(tree.node+inode_in, inode-inode_in, &CourseData);
							inode = inode_in-1;
							break;
#endif
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
#ifndef LUENTA
					case TAGCourseData:
							haara = 0;
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er && lueRadat)
								loadIOF30CourseData(tree.node+inode_in, inode-inode_in, &CourseData);
							inode = inode_in-1;
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
				if (depth > 2) {
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
			swprintf(msg, L"Tiedostossa %s virhe rivill‰ %d tai aikaisemmin", filename, rv);
			}
		writeerror_w(msg, 0, true);
		}
	tree.nodecount = inode+1;
	return(er);
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
			L"\tcreator=\"ViestiWin %s\"\n"
			L"\tstatus=\"%s\"",
		XMLhae_tagName(TAGResultList, IOF3Tags, nIOF3Tags), ISOdatetime(0, true), Build(), L"Complete");
	outfl->put_wtag(ln, level++);

	outfl->put_wtag(XMLhae_tagName(TAGEvent, IOF3Tags, nIOF3Tags), level++);
	outfl->put_wxml_s(XMLhae_tagName(TAGId, IOF3Tags, nIOF3Tags), kilpparam.kilpkoodi, level);
	outfl->put_wxml_s(XMLhae_tagName(TAGName, IOF3Tags, nIOF3Tags), kilpparam.Name, level);
	outfl->put_wtag(XMLhae_tagName(TAGStartTime, IOF3Tags, nIOF3Tags), level++);
	outfl->put_wxml_s(XMLhae_tagName(TAGDate, IOF3Tags, nIOF3Tags), kilpparam.Date, level);
//	outfl->put_wxml_s(XMLhae_tagName(TAGTime, IOF3Tags, nIOF3Tags), , level);
	outfl->put_wantitag(XMLhae_tagName(TAGStartTime, IOF3Tags, nIOF3Tags), --level);
/*
	outfl->put_wtag(XMLhae_tagName(TAGEndTime, IOF3Tags, nIOF3Tags), level++);
	outfl->put_wxml_s(XMLhae_tagName(TAGDate, IOF3Tags, nIOF3Tags), kilpailu, level);
	outfl->put_wxml_s(XMLhae_tagName(TAGTime, IOF3Tags, nIOF3Tags), kilpailu, level);
	outfl->put_wantitag(XMLhae_tagName(TAGEndTime, IOF3Tags, nIOF3Tags), --level);
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
   wchar_t Buf[40];
   TextFl *tul_tied;

   tul_tied = tulprm->lstf->u.Fl;
   tul_tied->put_wtag(XMLhae_tagName(TAGClassResult, IOF3Tags, nIOF3Tags), level++);

   tul_tied->put_wtag(XMLhae_tagName(TAGClass, IOF3Tags, nIOF3Tags), level++);
   tul_tied->put_wxml_s(XMLhae_tagName(TAGId, IOF3Tags, nIOF3Tags), Sarjat[sarja].Sarjanimi(Buf), level);
   if (Sarjat[sarja].psarjanimi[0])
		tul_tied->put_wxml_s(XMLhae_tagName(TAGName, IOF3Tags, nIOF3Tags), Sarjat[sarja].pSarjanimi(Buf), level);
   else
	   tul_tied->put_wxml_s(XMLhae_tagName(TAGName, IOF3Tags, nIOF3Tags), Sarjat[sarja].Sarjanimi(Buf), level);
   tul_tied->put_wantitag(XMLhae_tagName(TAGClass, IOF3Tags, nIOF3Tags), --level);
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
   wchar_t ln[100], ln1[50];
   INT piste = 0, level = 1;
   INT32 tl;
   TextFl *tul_tied;
#ifndef LUENTA
   emittp em;
   ratatp *rt;
#endif

   tul_tied = tulprm->lstf->u.Fl;

   tul_tied->put_wtag(XMLhae_tagName(TAGTeamResult, IOF3Tags, nIOF3Tags), level++);

   tul_tied->put_wxml_s(XMLhae_tagName(TAGName, IOF3Tags, nIOF3Tags), kilp.Joukkue(ln, 60), level);
   tul_tied->put_wtag(XMLhae_tagName(TAGOrganisation, IOF3Tags, nIOF3Tags), level++);
   tul_tied->put_wxml_s(XMLhae_tagName(TAGName, IOF3Tags, nIOF3Tags), kilp.Seura(ln, 60), level);
   if (kilp.maa[0]) {
	   swprintf(ln, L"code=\"%s\"", kilp.Maa(ln1));
	   tul_tied->put_wxml_s(XMLhae_tagName(TAGCountry, IOF3Tags, nIOF3Tags), MaaNimi(ln1), level, ln);
	   }
   tul_tied->put_wantitag(XMLhae_tagName(TAGOrganisation, IOF3Tags, nIOF3Tags), --level);
   tul_tied->put_wxml_d(XMLhae_tagName(TAGBibNumber, IOF3Tags, nIOF3Tags), kilp.kilpno, level);

   for (int os = 0; os < Sarjat[kilp.sarja].osuusluku; os++) {
	   tul_tied->put_wtag(XMLhae_tagName(TAGTeamMemberResult, IOF3Tags, nIOF3Tags), level++);

	   tul_tied->put_wtag(XMLhae_tagName(TAGPerson, IOF3Tags, nIOF3Tags), level++);
	   if (kilp.ostiet[os].lisno)
		  tul_tied->put_wxml_d(XMLhae_tagName(TAGId, IOF3Tags, nIOF3Tags), kilp.ostiet[os].lisno, level);
	   tul_tied->put_wtag(XMLhae_tagName(TAGName, IOF3Tags, nIOF3Tags), level++);
	   tul_tied->put_wxml_s(XMLhae_tagName(TAGFamily, IOF3Tags, nIOF3Tags), kilp.SukuNimi(ln, 40, os), level);
	   tul_tied->put_wxml_s(XMLhae_tagName(TAGGiven, IOF3Tags, nIOF3Tags), kilp.EtuNimi(ln, 40, os), level);
	   tul_tied->put_wantitag(XMLhae_tagName(TAGName, IOF3Tags, nIOF3Tags), --level);
	   tul_tied->put_wantitag(XMLhae_tagName(TAGPerson, IOF3Tags, nIOF3Tags), --level);

	   tul_tied->put_wtag(XMLhae_tagName(TAGResult, IOF3Tags, nIOF3Tags), level++);
	   tul_tied->put_wxml_d(XMLhae_tagName(TAGLeg, IOF3Tags, nIOF3Tags), os+1, level);
	   swprintf(ln, L"%d-%d", kilp.kilpno, os+1);
	   tul_tied->put_wxml_s(XMLhae_tagName(TAGBibNumber, IOF3Tags, nIOF3Tags), ln, level);
	   if (kilp.Lahto(os) != TMAALI0)
			tul_tied->put_wxml_s(XMLhae_tagName(TAGStartTime, IOF3Tags, nIOF3Tags), ISOtime(0, 0, kilp.Lahto(os)), level);
	   if (kilp.Maali(os, 0) != TMAALI0)
			tul_tied->put_wxml_s(XMLhae_tagName(TAGFinishTime, IOF3Tags, nIOF3Tags), ISOtime(0, 0, kilp.Maali(os, 0)), level);
	   if ((tl = kilp.osTulos(os, 0, true)) != 0) {
		   tul_tied->put_wxml_s(XMLhae_tagName(TAGTime, IOF3Tags, nIOF3Tags), sekTulos(NULL, tl, kilpparam.pyor[3]), level);
		   tul_tied->put_wxml_s(XMLhae_tagName(TAGTimeBehind, IOF3Tags, nIOF3Tags),
			   sekTulos(NULL, tl-pkarki[kilp.sarja][Sarjat[kilp.sarja].yosuus[os]+1][kilpparam.valuku+1], kilpparam.pyor[3]), level, L"type=\"Leg\"");
		   tul_tied->put_wxml_d(XMLhae_tagName(TAGPosition, IOF3Tags, nIOF3Tags), kilp.osSija(os), level, L"type=\"Leg\"");
		   }
	   tul_tied->put_wxml_s(XMLhae_tagName(TAGStatus, IOF3Tags, nIOF3Tags), IOFStatus(&kilp, os), level);

	   tul_tied->put_wtag(XMLhae_tagName(TAGOverallResult, IOF3Tags, nIOF3Tags), level++);
	   if ((tl = kilp.Tulos(os, 0)) != 0) {
		   tul_tied->put_wxml_s(XMLhae_tagName(TAGTime, IOF3Tags, nIOF3Tags), sekTulos(NULL, tl, kilpparam.pyor[3]), level);
		   tul_tied->put_wxml_s(XMLhae_tagName(TAGTimeBehind, IOF3Tags, nIOF3Tags), sekTulos(NULL, tl-pkarki[kilp.sarja][Sarjat[kilp.sarja].yosuus[os]+1][0], kilpparam.pyor[3]), level);
		   tul_tied->put_wxml_d(XMLhae_tagName(TAGPosition, IOF3Tags, nIOF3Tags), kilp.Sija(os, 0), level);
		   }
	   tul_tied->put_wxml_s(XMLhae_tagName(TAGStatus, IOF3Tags, nIOF3Tags), IOFStatus(&kilp, os), level);
	   tul_tied->put_wantitag(XMLhae_tagName(TAGOverallResult, IOF3Tags, nIOF3Tags), --level);


#ifndef LUENTA
	   if (kilp.ostiet[os].hajonta[0] && (rt = haerata(&kilp, os)) != NULL &&
			!strcmp(kilp.ostiet[os].hajonta, rt->rata)) {
		   tul_tied->put_wtag(XMLhae_tagName(TAGCourse, IOF3Tags, nIOF3Tags), level++);
		   tul_tied->put_wxml_s(XMLhae_tagName(TAGName, IOF3Tags, nIOF3Tags), rt->tunnus, level);
		   tul_tied->put_wxml_d(XMLhae_tagName(TAGLength, IOF3Tags, nIOF3Tags), rt->ratapit, level);
		   if (rt->nousu)
			   tul_tied->put_wxml_d(XMLhae_tagName(TAGLength, IOF3Tags, nIOF3Tags), rt->nousu, level);
		   tul_tied->put_wantitag(XMLhae_tagName(TAGCourse, IOF3Tags, nIOF3Tags), --level);
		   }
	   if (tulprm->tulostettava == L'E') {
		  getem(&em, kilp.kilpno, os);
		  if (em.kilpno == kilp.kilpno && em.osuus == os) {
			  emitvatp emva;
			  bool leim_ok = true;

			 tee_emva(&emva, &em);
			 rt = haerata(&kilp, os);
			 for (int r = 0; r < emva.rastiluku; r++) {
				 if (kilp.wTark(os) != L'T' && emva.rastit[r][1] == 0)
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
#endif  // !LUENTA
	   tul_tied->put_wantitag(XMLhae_tagName(TAGResult, IOF3Tags, nIOF3Tags), --level);
	   tul_tied->put_wantitag(XMLhae_tagName(TAGTeamMemberResult, IOF3Tags, nIOF3Tags), --level);
	   }
   tul_tied->put_wantitag(XMLhae_tagName(TAGTeamResult, IOF3Tags, nIOF3Tags), --level);
   }

