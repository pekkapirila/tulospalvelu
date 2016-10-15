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
#include <process.h>
#include <direct.h>
#include <dos.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <bstrings.h>
#pragma hdrstop
#include <sys/stat.h>

#include "UnitMain.h"
#include "TulosUnit.h"
#include "TapUnit2.h"
#include "UnitEmit.h"
#include "UnitStatus.h"
#include "UnitAjanotto.h"
#include "UnitLuenta.h"
#include "UnitAikavert.h"
#include "UnitNaytaTulos.h"
#include "UnitComfile.h"
#include "UnitLahestyjat.h"

extern TTilanneForm *TilFrm[NTILANNEFORM];
extern TTapForm *TapFrm[NTAPFORM];
extern TFormEmit *FormEmit;

static wchar_t IkkLaji[11] = L"PTVENSALRCH", *IkkLajiSt[11] = {L"P‰‰kaavake", L"Tilanne", L"Tapahtumat",
	L"Emitluenta", L"N‰yt‰luentatulos", L"Status", L"Ajanotto", L"Sis‰‰nluenta", L"Aikavertailut", L"L‰hestyj‰t", L"Testaus"};
static wchar_t *Pr[4] = {L"1", L"0,1", L"0,01", L"0,001"};
static wchar_t *Tabs[8] = {L"", L"\t", L"\t\t", L"\t\t\t", L"\t\t\t\t", L"\t\t\t\t\t", L"\t\t\t\t\t\t", L"\t\t\t\t\t\t\t"};

//---------------------------------------------------------------------------
#define TAGIkkunat 			1
#define TAGIkkuna 			2
#define TAGVaakapos 		3
#define TAGPystypos 		4
#define TAGLeveys 			5
#define TAGKorkeus 			6
#define TAGIkkParam 		7
#define TAGSarja 			11
#define TAGOsuus 			12
#define TAGPiste 			13
#define TAGAikaPituus 		14
#define TAGEsitykset 		51
#define TAGYLahdot 			52
#define TAGEmititiedot 		53
#define TAGEmitkoodeja 		54
#define TAGYhteysIkkuna		55
#define TAGNaytaMaa 		104
#define TAGRajaus 			105
#define TAGEtuSuku 			106
#define TAGSeuratieto 		107
#define TAGEroaika 			109
#define TAGSeuraa 			110
#define TAGOsJarj 			111
#define TAGKirjasinkoko 	116
#define TAGLihavoitu 		117
#define TAGSarakeLev 		118
#define TAGKorotusraja1 	119
#define TAGKorotusraja2 	120
#define TAGSkrollausvali 	121
#define TAGSkrollaus 		122
#define TAGSkrollRiviKesto 	123
#define TAGSkrollaustapa 	124
#define TAGKokoruutu 		125
#define TAGSkrollaussarjat 	126
#define TAGOhitaTarkSan		131
#define TAGSarakeOn 		137
#define TAGAikaVert         138
#define TAGKilpnoVert       139
#define TAGNaytaTulos		140
#define TAGOkKesto			141
#define TAGOngKesto			142
#define TAGTeksti			143
#define TAGEnnakoi			151
#define TAGSuppeat          152
#define TAGVaadiVahvistus   153
#define TAGJono			    154
#define TAGSyottoTila       155
#define TAGNayttoTila       156
#define TAGAikaKey          157
#define TAGAikaKeyShift     158
#define TAGNappainPiste		159
#define TAGAikaLisays       160
#define TAGNaytaSeura 		301
#define TAGNaytaKortinKoodi 302
#define TAGHenkTietokanta 	303
#define TAGSeuraluettelo 	304
#define TAGVahvIlmAina 		305
#define TAGKoneTeht 		306
#define TAGMerkLainat 		307
#define TAGTietokantaSarjat 308
#define TAGVaadiToisto 		309
#define TAGLueVialliset 	310
#define	TAGTunnistus 		321
#define	TAGKoodi 			322
#define	TAGHuomautaohitetusta 323
#define	TAGHuomautasuljetusta 324
#define	TAGEhdOsuus 		325
#define TAGValiLaji         331
#define TAGVertAika         332
#define TAGValiKerroin      333
#define TAGLahJono          334
#define TAGTiedJono         335
#define TAGTiedosto         336
#define TAGLahYhteys        337

tagListTp IkkTags[] = {
	{TAGIkkunat, L"Ikkunat"},
	{TAGIkkuna, L"Ikkuna"},
	{TAGVaakapos, L"Vaakapos"},
	{TAGPystypos, L"Pystypos"},
	{TAGLeveys, L"Leveys"},
	{TAGKorkeus, L"Korkeus"},
	{TAGIkkParam, L"IkkunaParametrit"},
	{TAGSarja, L"Sarja"},
	{TAGOsuus, L"Osuus"},
	{TAGPiste, L"Piste"},
	{TAGAikaPituus, L"AikaPituus"},
	{TAGEsitykset, L"Esitykset"},
	{TAGYLahdot, L"YLahdot"},
	{TAGEmititiedot, L"Emititiedot"},
	{TAGEmitkoodeja, L"Emitkoodeja"},
	{TAGYhteysIkkuna, L"YhteysIkkuna"},
	{TAGNaytaMaa, L"N‰yt‰Maa"},
	{TAGRajaus, L"Rajaus"},
	{TAGEtuSuku, L"EtuSuku"},
	{TAGSeuratieto, L"Seuratieto"},
	{TAGEroaika, L"Eroaika"},
	{TAGSeuraa, L"Seuraa"},
	{TAGOsJarj, L"OsJarj"},
	{TAGKirjasinkoko, L"Kirjasinkoko"},
	{TAGLihavoitu, L"Lihavoitu"},
	{TAGSarakeLev, L"SarakeLev"},
	{TAGKorotusraja1, L"Korotusraja1"},
	{TAGKorotusraja2, L"Korotusraja2"},
	{TAGSkrollausvali, L"Skrollausv‰li"},
	{TAGSkrollaus, L"Skrollaus"},
	{TAGSkrollRiviKesto, L"SkrollRiviKesto"},
	{TAGSkrollaustapa, L"Skrollaustapa"},
	{TAGKokoruutu, L"Kokoruutu"},
	{TAGSkrollaussarjat, L"Skrollaussarjat"},
	{TAGSarakeOn, L"SarakeOn"},
	{TAGAikaVert, L"AikaVert"},
	{TAGKilpnoVert, L"KilpnoVert"},
	{TAGNaytaTulos, L"NaytaTulos"},
	{TAGOkKesto, L"OkKesto"},
	{TAGOngKesto, L"OngKesto"},
	{TAGTeksti, L"Teksti"},
	{TAGOhitaTarkSan, L"OhitaTarkSan"},
	{TAGAikaKey, L"AikaKey"},
	{TAGAikaKeyShift, L"AikaKeyShift"},
	{TAGNappainPiste, L"NappainPiste"},
	{TAGAikaLisays, L"AikaLisays"},
	{TAGNaytaSeura, L"NaytaSeura"},
	{TAGNaytaKortinKoodi, L"NaytaKortinKoodi"},
	{TAGHenkTietokanta, L"HenkTietokanta"},
	{TAGSeuraluettelo, L"Seuraluettelo"},
	{TAGVahvIlmAina, L"VahvIlmAina"},
	{TAGKoneTeht, L"KoneTeht"},
	{TAGMerkLainat, L"MerkLainat"},
	{TAGTietokantaSarjat, L"TietokantaSarjat"},
	{TAGVaadiToisto, L"VaadiToisto"},
	{TAGLueVialliset, L"LueVialliset"},
	{TAGTunnistus, L"Tunnistus"},
	{TAGKoodi, L"Koodi"},
	{TAGHuomautaohitetusta, L"Huomautaohitetusta"},
	{TAGHuomautasuljetusta, L"Huomautasuljetusta"},
	{TAGEhdOsuus, L"EhdOsuus"},
	{TAGValiLaji, L"ValiLaji"},
	{TAGVertAika, L"VertAika"},
	{TAGValiKerroin, L"ValiKerroin"},
	{TAGLahJono, L"LahJono"},
	{TAGTiedJono, L"TiedJono"},
	{TAGTiedosto, L"Tiedosto"},
	{TAGLahYhteys, L"LahYhteys"}
};

int nIkkTags = sizeof(IkkTags)/sizeof(IkkTags[0]);

/*
static wchar_t XMLhaetunnus(wchar_t *Nimi, wchar_t *Koodit, wchar_t **Nimet, int N)
{
	for (int i = 0; i < N; i++) {
		if (!wcscmp(Nimi, Nimet[i]))
			return(Koodit[i]);
		}
	return(0);
}

static wchar_t *XMLhaeteksti(wchar_t Koodi, wchar_t *Koodit, wchar_t **Nimet)
{
	static wchar_t Tyhja[2] = L"";

	int i = wcswcind(Koodi, Koodit);
	if (i >= 0)
		return(Nimet[i]);
	return(Tyhja);
}

static int XMLhae_tagid(wchar_t *Tag, tagListTp *Tags, UINT nTags)
{
	for (UINT i = 0; i < nTags; i++)
		if (!wcscmp(Tag, Tags[i].tag))
			return(Tags[i].tagid);
	return(-1);
}

static wchar_t *XMLhae_tagName(int tagid, tagListTp *Tags, UINT nTags)
{
	static wchar_t st[1] = L"";
	for (UINT i = 0; i < nTags; i++)
		if (tagid == Tags[i].tagid)
			return(Tags[i].tag);
	return(st);
}

//---------------------------------------------------------------------------
static void XMLhaenodeid(xml_node *node, int nnode, tagListTp *Tags, UINT nTags)
{
	for (int inode = 0; inode <= nnode; inode++)
		node[inode].tagid = XMLhae_tagid(node[inode].tag, Tags, nTags);
}
*/

int lueIkkunaXml(wchar_t *filename);

void __fastcall TFormMain::HaeIkkunat(UnicodeString FlNm)
{
	lueIkkunaXml(FlNm.c_str());
}
//---------------------------------------------------------------------------

static int loadIkkuna(xml_node *node, int *inode, int nnode, wchar_t lj, int no)
{
	int er = 0, DepthIn, haara = 0, iassign = -1, val;
	TForm *Frm;

	switch (lj) {
		case L'P':
			Frm = FormMain;
			break;
		case L'T':
			if (no <= 0 || no >= NTILANNEFORM)
				return(1);
			if (TilFrm[no-1] == NULL)
				TilFrm[no-1] = new TTilanneForm(FormMain);
			Frm = TilFrm[no-1];
			break;
		case L'V':
			if (no <= 0 || no >= NTAPFORM)
				return(1);
			if (TapFrm[no-1] == NULL)
				TapFrm[no-1] = new TTapForm(FormMain);
			Frm = TapFrm[no-1];
			break;
		case L'A':
			if (no <= 0 || no >= NAJANOTOT)
				return(1);
			if (FormAjanotot[no-1] == NULL)
				FormAjanotot[no-1] = new TFormAjanotto(FormMain);
			Frm = FormAjanotot[no-1];
			break;
		case L'R':
			if (no <= 0 || no >= NAJANOTOT)
				return(1);
			if (FormAikavertailut[no-1] == NULL)
				FormAikavertailut[no-1] = new TFormAikavert(FormMain);
			Frm = FormAikavertailut[no-1];
			break;
		case L'H':
			if (no <= 0 || no >= NLAHEST)
				return(1);
			if (FormLahestyjat[no-1] == NULL)
				FormLahestyjat[no-1] = new TFormLahestyjat(FormMain);
			Frm = FormLahestyjat[no-1];
			break;
		case L'E':
			if (FormEmit == NULL)
				FormEmit = new TFormEmit(FormMain);
			Frm = FormEmit;
			break;
		case L'N':
			if (FormNaytaTulos == NULL)
				FormNaytaTulos = new TFormNaytaTulos(FormMain);
			Frm = FormNaytaTulos;
			break;
		case L'L':
			if (FormLuenta == NULL)
				FormLuenta = new TFormLuenta(FormMain);
			Frm = FormLuenta;
			break;
		case L'S':
			Frm = FormStatus;
			break;
		case L'C':
			if (FormComfile == NULL)
				FormComfile = new TFormComfile(FormMain);
			Frm = FormComfile;
			break;
		default:
			return(1);
		}

	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			switch (node[*inode].tagid) {
				case TAGVaakapos :
					node[*inode].gettext_int(&val);
					Frm->Left = val;
					break;
				case TAGPystypos :
					node[*inode].gettext_int(&val);
					Frm->Top = val;
					break;
				case TAGLeveys :
					node[*inode].gettext_int(&val);
					Frm->Width = val;
					break;
				case TAGKorkeus :
					node[*inode].gettext_int(&val);
					Frm->Height = val;
					break;
				case TAGIkkParam :
					switch (lj) {
						case L'T' :
							TilFrm[no-1]->IkkParam.readParams(node, inode, nnode);
							TilFrm[no-1]->applyParams();
							break;
						case L'V' :
							TapFrm[no-1]->IkkParam.readParams(node, inode, nnode);
							TapFrm[no-1]->applyParams();
							break;
						case L'A' :
							FormAjanotot[no-1]->IkkParam.readParams(node, inode, nnode);
							FormAjanotot[no-1]->applyParams();
							break;
						case L'R' :
							FormAikavertailut[no-1]->IkkParam.readParams(node, inode, nnode);
							FormAikavertailut[no-1]->applyParams();
							break;
						case L'H' :
							FormLahestyjat[no-1]->IkkParam.readParams(node, inode, nnode);
							FormLahestyjat[no-1]->applyParams();
							break;
						case L'S' :
							FormStatus->IkkParam.readParams(node, inode, nnode);
							FormStatus->applyParams();
							break;
						case L'E' :
							FormEmit->IkkParam.readParams(node, inode, nnode);
							FormEmit->applyParams();
							break;
						case L'N' :
							FormNaytaTulos->IkkParam.readParams(node, inode, nnode);
							FormNaytaTulos->applyParams();
							break;
						case L'L' :
							FormLuenta->IkkParam.readParams(node, inode, nnode);
							FormLuenta->applyParams();
							break;
						case L'C' :
							FormComfile->IkkParam.readParams(node, inode, nnode);
							FormComfile->applyParams();
							break;
						}
					break;
				}
			continue;
			}
		if (node[*inode].depth == DepthIn + 1) {
			continue;
			}
		}
	if (!er) {
		Frm->Show();
		}
	return(er);
}

//---------------------------------------------------------------------------
static int loadIkkunat(xml_node *node, int nnode)
{
	int er = 0, inode, DepthIn, haara = 0, iassign = -1, no = 0;
	wchar_t ln[200], lj;

	XMLhaenodeid(node, nnode, IkkTags, nIkkTags);
	DepthIn = node[0].depth + 1;
	for (inode = 1; inode <= nnode; inode++) {
		if (node[inode].depth == DepthIn) {
			haara = 0;
			switch (node[inode].tagid) {
				case TAGIkkuna :
					node[inode].getparam_text(L"Laji", ln, sizeof(ln)/2);
					lj = XMLhaetunnus(ln, IkkLaji, IkkLajiSt, sizeof(IkkLajiSt)/sizeof(IkkLajiSt[0]));
					if (wcswcind(lj, L"TVAR") >= 0)
						node[inode].getparam_int(L"Numero", &no);
					if ((er = loadIkkuna(node, &inode, nnode, lj, no)) != 0)
						return(er);
					break;
				}
			continue;
			}
		if (node[inode].depth == DepthIn + 1) {
			continue;
			}
		}
	return(er);
}

//---------------------------------------------------------------------------
int lueIkkunaXml(wchar_t *filename)
{
	TextFl *infile;
	int ntag = 100000, haara = 0, inode, inode_in, depth = 0, rv, er = 0, lntype;
	short k[MAXXMLDEPTH];
	wchar_t line[1000], ln[100], *txtbuf[MAXXMLDEPTH];
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
				nd.tagid = XMLhae_tagid(nd.tag, IkkTags, nIkkTags);
			if (lntype == 1) {
				tree.node[++inode] = nd;
				k[depth++]++;
				if (depth == 1) {
					inode_in = inode;
					switch (nd.tagid) {
						case TAGIkkunat :
							haara = 0;
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er)
								loadIkkunat(tree.node+inode_in, inode-inode_in);
							inode = inode_in-1;
							break;
						}
					continue;
					}
				if (depth > 1) {
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
		writeerror_w(msg, 0);
		}
	tree.nodecount = inode+1;
	return(0);
}

//---------------------------------------------------------------------------
int seurIkkParamClass::readParams(xml_node *node, int *inode, int nnode)
{
	int er = 0, pos, i, j, val, haara, DepthIn;
	wchar_t ln[402];
	kilptietue Kilp;

	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			if (node[*inode].tagid == TAGSarja) {
				node[*inode].gettext(ln, LSARJA);
				Sarja = haesarja_w(ln, true);
				}
			if (node[*inode].tagid == TAGOsuus) {
				node[*inode].gettext_int(&Osuus);
				Osuus--;
				}
			if (node[*inode].tagid == TAGPiste) {
				node[*inode].gettext_int(&Va);
				}
			if (node[*inode].tagid == TAGSeuraa) {
				node[*inode].gettext_int(&Seuraa);
				}
			if (node[*inode].tagid == TAGOsJarj) {
				node[*inode].gettext_int(&val);
				OsJarj = (val != 0);
				}
			if (node[*inode].tagid == TAGEtuSuku) {
				node[*inode].gettext_int(&val);
				EtuSuku = (val != 0);
				}
			if (node[*inode].tagid == TAGEroaika) {
				node[*inode].gettext_int(&val);
				Ero = (val != 0);
				}
			if (node[*inode].tagid == TAGKirjasinkoko) {
				node[*inode].gettext_int(&FontSize);
				}
			if (node[*inode].tagid == TAGLihavoitu) {
				node[*inode].gettext_int(&val);
				Lihavoitu = (val != 0);
				}
			if (node[*inode].tagid == TAGSarakeLev) {
				wchar_t *p;
				node[*inode].gettext(ln, 400);
				for (UINT i = 0; i < sizeof(ColW)/sizeof(ColW[0]); i++) {
					if (i == 0)
						p = wcstok(ln, L";\n");
					else
						p = wcstok(NULL, L";\n");
					if (p == NULL)
						break;
					ColW[i] = _wtoi(p);
					}
				}
			if (node[*inode].tagid == TAGAikaPituus) {
				node[*inode].gettext_int(&laika);
				}
			if (node[*inode].tagid == TAGSkrollausvali) {
				node[*inode].gettext_int(&ScrollVali);
				}
			if (node[*inode].tagid == TAGSkrollaus) {
				node[*inode].gettext_int(&val);
				ScrollOn = (val != 0);
				}
			if (node[*inode].tagid == TAGSkrollRiviKesto) {
				node[*inode].gettext_int(&ScrollRiviKesto);
				}
			if (node[*inode].tagid == TAGSkrollaustapa) {
				node[*inode].gettext_int(&ScrollType);
				}
			if (node[*inode].tagid == TAGKokoruutu) {
				node[*inode].gettext_int(&val);
				KokoRuutu = (val != 0);
				}
			if (node[*inode].tagid == TAGSkrollaussarjat) {
				node[*inode].gettext(ln, sarjaluku+nsarjayhd);
				for (int i = 0; i < sarjaluku+nsarjayhd; i++)
					ScrollSarjat[i] =  ln[i] - '0';
				}
			}
		if (node[*inode].depth == DepthIn + 1) {
			continue;
			}
		}
	return(0);
}
//---------------------------------------------------------------------------
int tapIkkParamClass::readParams(xml_node *node, int *inode, int nnode)
{
	int er = 0, pos, i, j, val, haara, DepthIn;
	wchar_t ln[402];
	kilptietue Kilp;

	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			if (node[*inode].tagid == TAGPiste) {
				node[*inode].gettext_int(&Va);
				}
			if (node[*inode].tagid == TAGNaytaMaa) {
				node[*inode].gettext_int(&val);
				naytaMaa = (val != 0);
				}
			if (node[*inode].tagid == TAGEtuSuku) {
				node[*inode].gettext_int(&val);
				EtuSuku = (val != 0);
				}
			if (node[*inode].tagid == TAGKirjasinkoko) {
				node[*inode].gettext_int(&FontSize);
				}
			if (node[*inode].tagid == TAGLihavoitu) {
				node[*inode].gettext_int(&val);
				Lihavoitu = (val != 0);
				}
			if (node[*inode].tagid == TAGSarakeLev) {
				wchar_t *p;
				node[*inode].gettext(ln, 400);
				for (UINT i = 0; i < sizeof(ColW)/sizeof(ColW[0]); i++) {
					if (i == 0)
						p = wcstok(ln, L";\n");
					else
						p = wcstok(NULL, L";\n");
					if (p == NULL)
						break;
					ColW[i] = _wtoi(p);
					}
				}
			if (node[*inode].tagid == TAGAikaPituus) {
				node[*inode].gettext_int(&laika);
				}
			if (node[*inode].tagid == TAGOhitaTarkSan) {
				node[*inode].gettext_int(&val);
				OhitaTarkSan = (val != 0);
				}
			}
		if (node[*inode].depth == DepthIn + 1) {
			continue;
			}
		}
	return(0);
}
//---------------------------------------------------------------------------
int ajatIkkParamClass::readParams(xml_node *node, int *inode, int nnode)
{
	int er = 0, pos, i, j, val, haara, DepthIn;
	wchar_t ln[402];
	kilptietue Kilp;

	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			switch(node[*inode].tagid) {
				case TAGEnnakoi:
					node[*inode].gettext_int(&val);
					Ennakoi = (val != 0);
					break;
				case TAGSuppeat:
					node[*inode].gettext_int(&val);
					Suppeat = (val != 0);
					break;
				case TAGVaadiVahvistus:
					node[*inode].gettext_int(&VaadiVahvistus);
					break;
				case TAGJono:
					node[*inode].gettext_int(&Jono);
					break;
				case TAGSyottoTila:
					node[*inode].gettext_int(&SyottoTila);
					break;
				case TAGNayttoTila:
					node[*inode].gettext_int(&NayttoTila);
					break;
				case TAGAikaKey:
					node[*inode].gettext_int(&KeyCode);
					break;
				case TAGAikaKeyShift:
					node[*inode].gettext_int(&val);
					KeyShift = IntShiftState(val);
					break;
				case TAGNappainPiste:
					node[*inode].gettext_int(&NappainPiste);
					break;
				case TAGAikaLisays:
					node[*inode].gettext_int(&AikaLisays);
					break;
				case TAGSarakeLev:
					wchar_t *p;
					node[*inode].gettext(ln, 400);
					for (UINT i = 0; i < sizeof(ColW)/sizeof(ColW[0]); i++) {
						if (i == 0)
							p = wcstok(ln, L";\n");
						else
							p = wcstok(NULL, L";\n");
						if (p == NULL)
							break;
						ColW[i] = _wtoi(p);
						}
					break;
				}
			}
		if (node[*inode].depth == DepthIn + 1) {
			continue;
			}
		}
	return(0);
}

//---------------------------------------------------------------------------
int aikavertIkkParamClass::readParams(xml_node *node, int *inode, int nnode)
{
	int er = 0, pos, i, j, val, haara, DepthIn;
	wchar_t ln[402];
	kilptietue Kilp;

	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			wchar_t *p;
			switch(node[*inode].tagid) {
				case TAGJono:
					node[*inode].gettext(ln, 50);
					swscanf(ln, L"%d;%d", &Jono1, &Jono2);
					break;
				case TAGAikaVert:
					node[*inode].gettext(ln, 50);
					swscanf(ln, L"%d %d;%d", &AikaVert, &AikaRajat[0], &AikaRajat[1]);
					break;
				case TAGKilpnoVert:
					node[*inode].gettext_int(&NoVert);
					break;
				case TAGKirjasinkoko:
					node[*inode].gettext_int(&val);
					FontSize = val;
					break;
				case TAGSarakeLev:
					node[*inode].gettext(ln, 400);
					for (UINT i = 0; i < sizeof(ColW)/sizeof(ColW[0]); i++) {
						if (i == 0)
							p = wcstok(ln, L";\n");
						else
							p = wcstok(NULL, L";\n");
						if (p == NULL)
							break;
						ColW[i] = _wtoi(p);
						}
					break;
				case TAGSarakeOn:
					node[*inode].gettext(ln, 400);
					for (UINT i = 0; i < sizeof(Visible)/sizeof(Visible[0]); i++) {
						if (i == 0)
							p = wcstok(ln, L";\n");
						else
							p = wcstok(NULL, L";\n");
						if (p == NULL)
							break;
						Visible[i] = _wtoi(p);
						}
					break;
				}
			}
		if (node[*inode].depth == DepthIn + 1) {
			continue;
			}
		}
	return(0);
}
//---------------------------------------------------------------------------
int lahIkkParamClass::readParams(xml_node *node, int *inode, int nnode)
{
	int er = 0, pos, i, j, val, haara, DepthIn;
	wchar_t ln[402];
	kilptietue Kilp;

	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			wchar_t *p;
			switch(node[*inode].tagid) {
				case TAGKirjasinkoko:
					node[*inode].gettext_int(&val);
					FontSize = val;
					break;
				case TAGSarakeLev:
					node[*inode].gettext(ln, 400);
					for (UINT i = 0; i < sizeof(ColW)/sizeof(ColW[0]); i++) {
						if (i == 0)
							p = wcstok(ln, L";\n");
						else
							p = wcstok(NULL, L";\n");
						if (p == NULL)
							break;
						ColW[i] = _wtoi(p);
						}
					break;
				case TAGSarakeOn:
					node[*inode].gettext(ln, 400);
					for (UINT i = 0; i < sizeof(ColOn)/sizeof(ColOn[0]); i++) {
						if (i == 0)
							p = wcstok(ln, L";\n");
						else
							p = wcstok(NULL, L";\n");
						if (p == NULL)
							break;
						ColOn[i] = _wtoi(p);
						}
					break;
				}
			}
		if (node[*inode].depth == DepthIn + 1) {
			continue;
			}
		}
	return(0);
}
//---------------------------------------------------------------------------
int statusIkkParamClass::readParams(xml_node *node, int *inode, int nnode)
{
	int er = 0, pos, i, j, val, haara, DepthIn;

	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			switch(node[*inode].tagid) {
				case TAGEsitykset:
					node[*inode].gettext_int(&esitykset);
					break;
				case TAGYLahdot:
					node[*inode].gettext_int(&ylahdot);
					break;
				case TAGEmititiedot:
					node[*inode].gettext_int(&emittiedot);
					break;
				case TAGEmitkoodeja:
					node[*inode].gettext_int(&emitkoodeja);
					break;
				case TAGYhteysIkkuna:
					node[*inode].gettext_int(&yhteysikkuna);
					break;
				}
			}
		if (node[*inode].depth == DepthIn + 1) {
			continue;
			}
		}
	return(0);
}
//---------------------------------------------------------------------------
int emitIkkParamClass::readParams(xml_node *node, int *inode, int nnode)
{
	int er = 0, pos, i, j, val, haara, DepthIn;

	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			switch(node[*inode].tagid) {
				case TAGNaytaKortinKoodi:
					node[*inode].gettext_int(&val);
					NaytaKortinKoodi = (val != 0);
					break;
				case TAGKoneTeht:
					node[*inode].gettext_int(&KoneTeht);
					break;
				case TAGMerkLainat:
					node[*inode].gettext_int(&val);
					MerkLainat = (val != 0);
					break;
				case TAGVaadiToisto:
					node[*inode].gettext_int(&val);
					VaadiToisto = (val != 0);
					break;
				case TAGLueVialliset:
					node[*inode].gettext_int(&val);
					LueVialliset = (val != 0);
					break;
				}
			}
		if (node[*inode].depth == DepthIn + 1) {
			continue;
			}
		}
	return(0);
}
//---------------------------------------------------------------------------
int NaytaTulosIkkParamClass::readParams(xml_node *node, int *inode, int nnode)
{
	int er = 0, pos, i, haara, DepthIn;

	DepthIn = node[*inode].depth + 1;
	for (i = 0; i < sizeof(Teksti) / sizeof(Teksti[0]); i++)
		Teksti[i][0] = 0;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			switch(node[*inode].tagid) {
				case TAGNaytaTulos:
					node[*inode].gettext_int(&NaytaTulos);
					break;
				case TAGOkKesto:
					node[*inode].gettext_int(&OkKesto);
					break;
				case TAGOngKesto:
					node[*inode].gettext_int(&OngKesto);
					break;
				case TAGTeksti:
					node[*inode].getparam_int(L"rivi", &i);
					if (i > 0 && i <= sizeof(Teksti) / sizeof(Teksti[0]))
						node[*inode].gettext(Teksti[i-1], sizeof(Teksti[0])/2-1);
					break;
				}
			}
		if (node[*inode].depth == DepthIn + 1) {
			continue;
			}
		}
	return(0);
}
//---------------------------------------------------------------------------
int luentaIkkParamClass::readParams(xml_node *node, int *inode, int nnode)
{
	int er = 0, pos, i, j, val, haara, DepthIn;

	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			switch(node[*inode].tagid) {
				case TAGTunnistus:
					node[*inode].gettext_int(&val);
					Tunnistus = val;
					break;
				case TAGKoodi:
					node[*inode].gettext_int(&val);
					Koodi = val;
					break;
				case TAGHuomautaohitetusta:
					node[*inode].gettext_int(&val);
					Huomautaohitetusta = (val != 0);
					break;
				case TAGHuomautasuljetusta:
					node[*inode].gettext_int(&val);
					Huomautasuljetusta = (val != 0);
					break;
				case TAGEhdOsuus:
					node[*inode].gettext_int(&val);
					EhdOsuus = (val != 0);
					break;
				}
			}
		if (node[*inode].depth == DepthIn + 1) {
			continue;
			}
		}
	return(0);
}
//---------------------------------------------------------------------------
int TestausIkkParamClass::readParams(xml_node *node, int *inode, int nnode)
{
	int er = 0, pos, i, haara, DepthIn, k;
	wchar_t st[80] = L"";

	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			switch(node[*inode].tagid) {
				case TAGValiLaji:
					node[*inode].gettext_int(&ValiLaji);
					break;
				case TAGVertAika:
					node[*inode].gettext(st, sizeof(st)/2-1);
					VertAika = wstrtoaika_vap(st, t0);
					break;
				case TAGValiKerroin:
					node[*inode].gettext_int(&k);
					ValiKerroin = (double)k * 0.001;
					break;
				case TAGLahJono:
					node[*inode].gettext_int(&LahJono);
					break;
				case TAGTiedJono:
					node[*inode].gettext_int(&TiedJono);
					break;
				case TAGLahYhteys:
					node[*inode].gettext_int(&LahYhteys);
					break;
				case TAGTiedosto:
					node[*inode].gettext(Tiedosto, sizeof(Tiedosto)/2-1);
					break;
				}
			}
		if (node[*inode].depth == DepthIn + 1) {
			continue;
			}
		}
	return(0);
}
//---------------------------------------------------------------------------
void TallennaIkkunaYht(TextFl *tallfl, TForm *Frm, int level)
{
	tallfl->put_wxml_d(L"Vaakapos", Frm->Left, level);
	tallfl->put_wxml_d(L"Pystypos", Frm->Top, level);
	tallfl->put_wxml_d(L"Leveys", Frm->Width, level);
	tallfl->put_wxml_d(L"Korkeus", Frm->Height, level);
}
//---------------------------------------------------------------------------
void __fastcall TFormMain::TallennaIkkunat(UnicodeString FlNm)
{
	TextFl *tallfl;
	int level = 0;
	wchar_t ln[400];

	tallfl = new TextFl(FlNm.c_str(), L"wt");

	if (tallfl->IsOpen()) {
		tallfl->put_wtag(L"Ikkunat", level++);
		tallfl->put_wtag(L"Ikkuna Laji=\"P‰‰kaavake\"", level++);
		TallennaIkkunaYht(tallfl, FormMain, level);
		tallfl->put_wantitag(L"Ikkuna", --level);
		for (int i = 0; i < NTILANNEFORM; i++) {
			if (TilFrm[i] && TilFrm[i]->Visible) {
				swprintf(ln, L"Ikkuna Laji=\"Tilanne\" Numero=\"%d\"", i+1);
				tallfl->put_wtag(ln, level++);
				TallennaIkkunaYht(tallfl, TilFrm[i], level);
				tallfl->put_wtag(L"IkkunaParametrit", level++);
				TilFrm[i]->IkkParam.writeParams(tallfl, level);
				tallfl->put_wantitag(L"IkkunaParametrit", --level);
				tallfl->put_wantitag(L"Ikkuna", --level);
				}
			}
		for (int i = 0; i < NTAPFORM; i++) {
			if (TapFrm[i] && TapFrm[i]->Visible) {
				swprintf(ln, L"Ikkuna Laji=\"Tapahtumat\" Numero=\"%d\"", i+1);
				tallfl->put_wtag(ln, level++);
				TallennaIkkunaYht(tallfl, TapFrm[i], level);
				tallfl->put_wtag(L"IkkunaParametrit", level++);
				TapFrm[i]->IkkParam.writeParams(tallfl, level);
				tallfl->put_wantitag(L"IkkunaParametrit", --level);
				tallfl->put_wantitag(L"Ikkuna", --level);
				}
			}
		for (int i = 0; i < NAJANOTOT; i++) {
			if (FormAjanotot[i] && FormAjanotot[i]->Visible) {
				swprintf(ln, L"Ikkuna Laji=\"Ajanotto\" Numero=\"%d\"", i+1);
				tallfl->put_wtag(ln, level++);
				TallennaIkkunaYht(tallfl, FormAjanotot[i], level);
				tallfl->put_wtag(L"IkkunaParametrit", level++);
				FormAjanotot[i]->IkkParam.writeParams(tallfl, level);
				tallfl->put_wantitag(L"IkkunaParametrit", --level);
				tallfl->put_wantitag(L"Ikkuna", --level);
				}
			}
		for (int i = 0; i < NAJANOTOT; i++) {
			if (FormAikavertailut[i] && FormAikavertailut[i]->Visible) {
				swprintf(ln, L"Ikkuna Laji=\"Aikavertailu\" Numero=\"%d\"", i+1);
				tallfl->put_wtag(ln, level++);
				TallennaIkkunaYht(tallfl, FormAikavertailut[i], level);
				tallfl->put_wtag(L"IkkunaParametrit", level++);
				FormAikavertailut[i]->IkkParam.writeParams(tallfl, level);
				tallfl->put_wantitag(L"IkkunaParametrit", --level);
				tallfl->put_wantitag(L"Ikkuna", --level);
				}
			}
		if (FormEmit && FormEmit->Visible) {
			tallfl->put_wtag(L"Ikkuna Laji=\"Emitluenta\"", level++);
			TallennaIkkunaYht(tallfl, FormEmit, level);
			tallfl->put_wtag(L"IkkunaParametrit", level++);
			FormEmit->haeParams();
			FormEmit->IkkParam.writeParams(tallfl, level);
			tallfl->put_wantitag(L"IkkunaParametrit", --level);
			tallfl->put_wantitag(L"Ikkuna", --level);
			}
		if (FormNaytaTulos && FormNaytaTulos->Visible) {
			tallfl->put_wtag(L"Ikkuna Laji=\"N‰yt‰luentatulos\"", level++);
			TallennaIkkunaYht(tallfl, FormNaytaTulos, level);
			tallfl->put_wtag(L"IkkunaParametrit", level++);
			FormNaytaTulos->haeParams();
			FormNaytaTulos->IkkParam.writeParams(tallfl, level);
			tallfl->put_wantitag(L"IkkunaParametrit", --level);
			tallfl->put_wantitag(L"Ikkuna", --level);
			}
		if (FormLuenta && FormLuenta->Visible) {
			tallfl->put_wtag(L"Ikkuna Laji=\"Sis‰‰nluenta\"", level++);
			TallennaIkkunaYht(tallfl, FormLuenta, level);
			tallfl->put_wtag(L"IkkunaParametrit", level++);
			FormLuenta->haeParams();
			FormLuenta->IkkParam.writeParams(tallfl, level);
			tallfl->put_wantitag(L"IkkunaParametrit", --level);
			tallfl->put_wantitag(L"Ikkuna", --level);
			}
		if (FormStatus && FormStatus->Visible) {
			tallfl->put_wtag(L"Ikkuna Laji=\"Status\"", level++);
			TallennaIkkunaYht(tallfl, FormStatus, level);
			tallfl->put_wtag(L"IkkunaParametrit", level++);
			FormStatus->haeParams();
			FormStatus->IkkParam.writeParams(tallfl, level);
			tallfl->put_wantitag(L"IkkunaParametrit", --level);
			tallfl->put_wantitag(L"Ikkuna", --level);
			}
		tallfl->put_wantitag(L"Ikkunat", --level);
		}
	delete tallfl;
}
//---------------------------------------------------------------------------
int seurIkkParamClass::writeParams(TextFl *outfl, int level)
{
	wchar_t st[1000] = L"";

	outfl->put_wxml_s(XMLhae_tagName(TAGSarja, IkkTags, nIkkTags), Sarjat[Sarja].Sarjanimi(st), level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOsuus, IkkTags, nIkkTags), Osuus+1, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGPiste, IkkTags, nIkkTags), Va, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSeuraa, IkkTags, nIkkTags), Seuraa, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOsJarj, IkkTags, nIkkTags), (int) OsJarj, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGEroaika, IkkTags, nIkkTags), (int) Ero, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGEtuSuku, IkkTags, nIkkTags), (int) EtuSuku, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKirjasinkoko, IkkTags, nIkkTags), FontSize , level);
	outfl->put_wxml_d(XMLhae_tagName(TAGLihavoitu, IkkTags, nIkkTags), (int) Lihavoitu, level);
	st[0] = 0;
	for (UINT i = 0; i < sizeof(ColW)/sizeof(ColW[0]); i++) {
		if (i > 0)
			wcscat(st, L";");
		_itow(ColW[i], st+wcslen(st), 10);
		}
	outfl->put_wxml_s(XMLhae_tagName(TAGSarakeLev, IkkTags, nIkkTags), st, level);
//	outfl->put_wxml_d(XMLhae_tagName(TAGAikapituus, IkkTags, nIkkTags), laika, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSkrollausvali, IkkTags, nIkkTags), ScrollVali , level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSkrollaus, IkkTags, nIkkTags), (int) ScrollOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSkrollRiviKesto, IkkTags, nIkkTags), ScrollRiviKesto , level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSkrollaustapa, IkkTags, nIkkTags), ScrollType , level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKokoruutu, IkkTags, nIkkTags), (int) KokoRuutu, level);
	memset(st, 0, sizeof(st));
	for (int i = 0; i < sarjaluku+nsarjayhd; i++)
		st[i] = ScrollSarjat[i] + '0';
	outfl->put_wxml_s(XMLhae_tagName(TAGSkrollaussarjat, IkkTags, nIkkTags), st, level);
	return(0);
}

//---------------------------------------------------------------------------
int tapIkkParamClass::writeParams(TextFl *outfl, int level)
{
	wchar_t st[1000] = L"";

	outfl->put_wxml_d(XMLhae_tagName(TAGPiste, IkkTags, nIkkTags), Va, level);
//	outfl->put_wxml_d(XMLhae_tagName(TAGN‰yt‰Maa, IkkTags, nIkkTags), (int) naytaMaa, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKokoruutu, IkkTags, nIkkTags), (int) KokoRuutu, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGEtuSuku, IkkTags, nIkkTags), (int) EtuSuku, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKirjasinkoko, IkkTags, nIkkTags), FontSize , level);
	outfl->put_wxml_d(XMLhae_tagName(TAGLihavoitu, IkkTags, nIkkTags), (int) Lihavoitu, level);
	st[0] = 0;
	for (UINT i = 0; i < sizeof(ColW)/sizeof(ColW[0]); i++) {
		if (i > 0)
			wcscat(st, L";");
		_itow(ColW[i], st+wcslen(st), 10);
		}
	outfl->put_wxml_s(XMLhae_tagName(TAGSarakeLev, IkkTags, nIkkTags), st, level);
//	outfl->put_wxml_d(XMLhae_tagName(TAGAikapituus, IkkTags, nIkkTags), laika, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOhitaTarkSan, IkkTags, nIkkTags), OhitaTarkSan, level);
	return(0);
}

//---------------------------------------------------------------------------
int ajatIkkParamClass::writeParams(TextFl *outfl, int level)
{
	wchar_t st[1000] = L"";

	outfl->put_wxml_d(XMLhae_tagName(TAGEnnakoi, IkkTags, nIkkTags), (int) Ennakoi, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSuppeat, IkkTags, nIkkTags), (int) Suppeat, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGVaadiVahvistus, IkkTags, nIkkTags), VaadiVahvistus, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGJono, IkkTags, nIkkTags), Jono, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSyottoTila, IkkTags, nIkkTags), SyottoTila, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGNayttoTila, IkkTags, nIkkTags), NayttoTila, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGAikaKey, IkkTags, nIkkTags), KeyCode, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGAikaKeyShift, IkkTags, nIkkTags), ShiftStateInt(KeyShift), level);
	outfl->put_wxml_d(XMLhae_tagName(TAGNappainPiste, IkkTags, nIkkTags), NappainPiste, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGAikaLisays, IkkTags, nIkkTags), AikaLisays, level);
	st[0] = 0;
	for (UINT i = 0; i < sizeof(ColW)/sizeof(ColW[0]); i++) {
		if (i > 0)
			wcscat(st, L";");
		_itow(ColW[i], st+wcslen(st), 10);
		}
	outfl->put_wxml_s(XMLhae_tagName(TAGSarakeLev, IkkTags, nIkkTags), st, level);
	return(0);
}

//---------------------------------------------------------------------------
int aikavertIkkParamClass::writeParams(TextFl *outfl, int level)
{
	wchar_t st[1000] = L"";

	swprintf(st, L"%d;%d", Jono1, Jono2);
	outfl->put_wxml_s(XMLhae_tagName(TAGJono, IkkTags, nIkkTags), st, level);
	swprintf(st, L"%d;%d;%d", AikaVert, AikaRajat[0], AikaRajat[1]);
	outfl->put_wxml_s(XMLhae_tagName(TAGAikaVert, IkkTags, nIkkTags), st, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKilpnoVert, IkkTags, nIkkTags), NoVert , level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKirjasinkoko, IkkTags, nIkkTags), FontSize , level);
	st[0] = 0;
	for (UINT i = 0; i < sizeof(ColW)/sizeof(ColW[0]); i++) {
		if (i > 0)
			wcscat(st, L";");
		_itow(ColW[i], st+wcslen(st), 10);
		}
	outfl->put_wxml_s(XMLhae_tagName(TAGSarakeLev, IkkTags, nIkkTags), st, level);
	st[0] = 0;
	for (UINT i = 0; i < sizeof(Visible)/sizeof(Visible[0]); i++) {
		if (i > 0)
			wcscat(st, L";");
		_itow(Visible[i], st+wcslen(st), 10);
		}
	outfl->put_wxml_s(XMLhae_tagName(TAGSarakeOn, IkkTags, nIkkTags), st, level);
	return(0);
}
//---------------------------------------------------------------------------
int lahIkkParamClass::writeParams(TextFl *outfl, int level)
{
	wchar_t st[1000] = L"";

	outfl->put_wxml_d(XMLhae_tagName(TAGKirjasinkoko, IkkTags, nIkkTags), FontSize , level);
	for (UINT i = 0; i < sizeof(ColW)/sizeof(ColW[0]); i++) {
		if (i > 0)
			wcscat(st, L";");
		_itow(ColW[i], st+wcslen(st), 10);
		}
	outfl->put_wxml_s(XMLhae_tagName(TAGSarakeLev, IkkTags, nIkkTags), st, level);
	st[0] = 0;
	for (UINT i = 0; i < sizeof(ColOn)/sizeof(ColOn[0]); i++) {
		if (i > 0)
			wcscat(st, L";");
		_itow(ColOn[i], st+wcslen(st), 10);
		}
	outfl->put_wxml_s(XMLhae_tagName(TAGSarakeOn, IkkTags, nIkkTags), st, level);
	return(0);
}
//---------------------------------------------------------------------------
int statusIkkParamClass::writeParams(TextFl *outfl, int level)
{
	outfl->put_wxml_d(XMLhae_tagName(TAGEsitykset, IkkTags, nIkkTags), (int) esitykset, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGYLahdot, IkkTags, nIkkTags), (int) ylahdot, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGEmititiedot, IkkTags, nIkkTags), (int) emittiedot, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGEmitkoodeja, IkkTags, nIkkTags), (int) emitkoodeja, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGYhteysIkkuna, IkkTags, nIkkTags), (int) yhteysikkuna, level);
	return(0);
}
//---------------------------------------------------------------------------

int emitIkkParamClass::writeParams(TextFl *outfl, int level)
{
	outfl->put_wxml_d(XMLhae_tagName(TAGNaytaKortinKoodi, IkkTags, nIkkTags), (int) NaytaKortinKoodi, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKoneTeht, IkkTags, nIkkTags), (int) KoneTeht, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGMerkLainat, IkkTags, nIkkTags), (int) MerkLainat, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGVaadiToisto, IkkTags, nIkkTags), (int) VaadiToisto, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGLueVialliset, IkkTags, nIkkTags), (int) LueVialliset, level);
	return(0);
}
//---------------------------------------------------------------------------
int luentaIkkParamClass::writeParams(TextFl *outfl, int level)
{
	outfl->put_wxml_d(XMLhae_tagName(TAGTunnistus, IkkTags, nIkkTags), (int) Tunnistus, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKoodi, IkkTags, nIkkTags), (int) Koodi, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGHuomautaohitetusta, IkkTags, nIkkTags), (int) Huomautaohitetusta, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGHuomautasuljetusta, IkkTags, nIkkTags), (int) Huomautasuljetusta, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGEhdOsuus, IkkTags, nIkkTags), (int) EhdOsuus, level);
	return(0);
}
//---------------------------------------------------------------------------
int NaytaTulosIkkParamClass::writeParams(TextFl *outfl, int level)
{
	outfl->put_wxml_d(XMLhae_tagName(TAGNaytaTulos, IkkTags, nIkkTags), (int) NaytaTulos, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOkKesto, IkkTags, nIkkTags), (int) OkKesto, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOngKesto, IkkTags, nIkkTags), (int) OngKesto, level);
	for (int r = 0; r < sizeof(Teksti) / sizeof(Teksti[0]); r++)
		if (Teksti[r][0]) {
			outfl->put_wxml_s(XMLhae_tagName(TAGTeksti, IkkTags, nIkkTags), Teksti[r], level,
				(UnicodeString(L"rivi=\"")+UnicodeString(r+1)+L"\"").c_str());
			}
	return(0);
}
//---------------------------------------------------------------------------
int TestausIkkParamClass::writeParams(TextFl *outfl, int level)
{
	wchar_t st[20];
	outfl->put_wxml_d(XMLhae_tagName(TAGValiLaji, IkkTags, nIkkTags), ValiLaji, level);
	if (VertAika != TMAALI0)
		outfl->put_wxml_s(XMLhae_tagName(TAGVertAika, IkkTags, nIkkTags), aikatowstr_cols_n(st, VertAika, t0, 0, 8), level);
	outfl->put_wxml_d(XMLhae_tagName(TAGValiKerroin, IkkTags, nIkkTags), (int) (1000*ValiKerroin), level);
	outfl->put_wxml_d(XMLhae_tagName(TAGLahJono, IkkTags, nIkkTags), LahJono, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGTiedJono, IkkTags, nIkkTags), TiedJono, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGLahYhteys, IkkTags, nIkkTags), LahYhteys, level);
	outfl->put_wxml_s(XMLhae_tagName(TAGTiedosto, IkkTags, nIkkTags), Tiedosto, level);
	return(0);
}
//---------------------------------------------------------------------------


