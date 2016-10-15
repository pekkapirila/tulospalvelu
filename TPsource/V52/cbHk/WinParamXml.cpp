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

#include "WinHk.h"
#include "TulosUnit.h"
#include "TapUnit2.h"
#include "UnitEmit.h"
#include "UnitIlm2.h"
#include "UnitStatus.h"
#include "UnitAjanotto.h"
#include "UnitAikavert.h"
#include "UnitNaytaTulos.h"
#include "UnitASakot.h"
#include "UnitSakkoKierr.h"
#include "UnitAmpPaikat.h"

extern TTilanneForm *TilFrm[NTILANNEFORM];
extern TTapForm *FrmTap[NTAPFORM];
extern TFormEmit *FormEmit;

static wchar_t IkkLaji[13] = L"PTVENISARBCD", *IkkLajiSt[12] = {L"P‰‰kaavake", L"Tilanne", L"Tapahtumat",
	L"Emitluenta", L"N‰yt‰luentatulos", L"Ilmoittautumiset", L"Status", L"Ajanotto", L"Aikavertailu",
	L"AHSakot", L"AmpPaikat", L"SakkoKierr"};
static wchar_t *Pr[4] = {L"1", L"0,1", L"0,01", L"0,001"};
wchar_t *XMLTabs[8] = {L"", L"\t", L"\t\t", L"\t\t\t", L"\t\t\t\t", L"\t\t\t\t\t", L"\t\t\t\t\t\t", L"\t\t\t\t\t\t\t"};

//---------------------------------------------------------------------------
#define TAGIkkunat 			1
#define TAGIkkuna 			2
#define TAGVaakapos 		3
#define TAGPystypos 		4
#define TAGLeveys 			5
#define TAGKorkeus 			6
#define TAGIkkParam 		7
#define TAGSarja 			11
#define TAGPiste 			12
#define TAGAikaPituus 		13
#define TAGParasSija 		51
#define TAGPalkJako 		52
#define TAGYhteysIkkuna		53
#define TAGNaytaAlisarja 	103
#define TAGNaytaMaa 		104
#define TAGRajaus 			105
#define TAGEtuSuku 			106
#define TAGSeuratieto 		107
#define TAGKokonaistulos 	108
#define TAGEroaika 			109
#define TAGKaikkiajat 		110
#define TAGVainjarjestys 	111
#define TAGSmart 			112
#define TAGLahtoaika 		113
#define TAGYhteistulos 		114
#define TAGLaskeYhtpisteet 	115
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
#define TAGOdotusraja 		127
#define TAGnaytaHyl			128
#define TAGseuraaLahtoja	129
#define TAGminTaso			130
#define TAGnaytaPiilotetut	131
#define TAGnaytaOdotetut	132
#define TAGKierrosaika		133
#define TAGViimeisin		134
#define TAGEnnatys			135
#define TAGTavoite			136
#define TAGSarakeOn 		137
#define TAGAikaVert         138
#define TAGKilpnoVert       139
#define TAGNaytaTulos		140
#define TAGOkKesto			141
#define TAGOngKesto			142
#define TAGTeksti			143
#define TAGLahtoRaja		144
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
#define TAGAaniLkm       	161
#define TAGOletusPiste		162
#define TAGIdOn 			201
#define TAGKilpnoOn 		202
#define TAGLisnoOn 			203
#define TAGSNimiOn 			204
#define TAGENimiOn 			205
#define TAGSarjaOn 			206
#define TAGSukupOn 			207
#define TAGIkasrjOn 		208
#define TAGWRKoodiOn 		209
#define TAGArvoOn 			210
#define TAGSeuraOn 			211
#define TAGSeuralyhOn 		212
#define TAGMaaOn 			213
#define TAGYhdistysOn 		214
#define TAGJoukkueOn 		215
#define TAGLasnaOn 			216
#define TAGBibOn 			217
#define TAGBadgeOn 			218
#define TAGLAOn 			219
#define TAGTasOn 			220
#define TAGRataOn 			221
#define TAGPiiriOn 			222
#define TAGKnohakuOn 		223
#define TAGLisnohakuOn 		224
#define TAGNimihakuOn 		225
#define TAGNimiHThakuOn 	226
#define TAGHTSarjaOn 		227
#define TAGSeurahakuOn 		228
#define TAGSeuralisaOn 		229
#define TAGMerkAvoimeksiOn 	230
#define TAGHTBadgeOn 		231
#define TAGAutoLaikaOn 		232
#define TAGJIilmOn 			233
#define TAGAutoKnoOn 		234
#define TAGLisnoTarkOn 		235
#define TAGLisaysIx 		236
#define TAGJarjestysIx 		237
#define TAGPaivatIx 		238
#define TAGOId 				239
#define TAGOKilpno 			240
#define TAGOLisno 			241
#define TAGOSNimi 			242
#define TAGOENimi 			243
#define TAGOSarja 			244
#define TAGOWRKoodi 		245
#define TAGOArvo 			246
#define TAGOSeura 			247
#define TAGOSeuralyh 		248
#define TAGOMaa 			249
#define TAGOYhdistys 		250
#define TAGOJoukkue 		251
#define TAGOLasna 			252
#define TAGOBib 			253
#define TAGOBadge 			254
#define TAGOLA 				255
#define TAGOTas 			256
#define TAGORata 			257
#define TAGOPiiri 			258
#define TAGOSukup 			259
#define TAGOIkasrj 			260
#define TAGEdtVakBibLisa 	261
#define TAGIlmKentat 		262
#define TAGNaytaSeura 		301
#define TAGNaytaKortinKoodi 302
#define TAGHenkTietokanta 	303
#define TAGSeuraluettelo 	304
#define TAGVahvIlmAina 		305
#define TAGKuntoKone 		306
#define TAGMerkLainat 		307
#define TAGTietokantaSarjat 308
#define TAGVaadiToisto 		309
#define TAGLueVialliset 	310
#define TAGEmitvaihtoVaiheille 311
#define TAGVarit	 		312

static tagListTp IkkTags[] = {
	{TAGIkkunat, L"Ikkunat"},
	{TAGIkkuna, L"Ikkuna"},
	{TAGVaakapos, L"Vaakapos"},
	{TAGPystypos, L"Pystypos"},
	{TAGLeveys, L"Leveys"},
	{TAGKorkeus, L"Korkeus"},
	{TAGIkkParam, L"IkkunaParametrit"},
	{TAGSarja, L"Sarja"},
	{TAGPiste, L"Piste"},
	{TAGAikaPituus, L"AikaPituus"},
	{TAGParasSija, L"ParasSija"},
	{TAGPalkJako, L"PalkJako"},
	{TAGYhteysIkkuna, L"YhteysIkkuna"},
	{TAGNaytaAlisarja, L"N‰yt‰Alisarja"},
	{TAGNaytaMaa, L"N‰yt‰Maa"},
	{TAGRajaus, L"Rajaus"},
	{TAGEtuSuku, L"EtuSuku"},
	{TAGSeuratieto, L"Seuratieto"},
	{TAGKokonaistulos, L"Kokonaistulos"},
	{TAGEroaika, L"Eroaika"},
	{TAGKierrosaika, L"Kierrosaika"},
	{TAGKaikkiajat, L"Kaikkiajat"},
	{TAGVainjarjestys, L"Vainj‰rjestys"},
	{TAGSmart, L"Smart"},
	{TAGLahtoaika, L"L‰htˆaika"},
	{TAGYhteistulos, L"Yhteistulos"},
	{TAGLaskeYhtpisteet, L"LaskeYhtpisteet"},
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
	{TAGOdotusraja, L"Odotusraja"},
	{TAGIdOn, L"IdOn"},
	{TAGEnnatys, L"Ennatys"},
	{TAGTavoite, L"Tavoite"},
	{TAGSarakeOn, L"SarakeOn"},
	{TAGAikaVert, L"AikaVert"},
	{TAGKilpnoVert, L"KilpnoVert"},
	{TAGNaytaTulos, L"NaytaTulos"},
	{TAGOkKesto, L"OkKesto"},
	{TAGOngKesto, L"OngKesto"},
	{TAGTeksti, L"Teksti"},
	{TAGLahtoRaja, L"L‰htˆRaja"},
	{TAGEnnakoi, L"Ennakoi"},
	{TAGSuppeat, L"Suppeat"},
	{TAGVaadiVahvistus, L"VaadiVahvistus"},
	{TAGJono, L"Jono"},
	{TAGSyottoTila, L"SyottoTila"},
	{TAGNayttoTila, L"NayttoTila"},
	{TAGAikaKey, L"AikaKey"},
	{TAGAikaKeyShift, L"AikaKeyShift"},
	{TAGNappainPiste, L"NappainPiste"},
	{TAGAikaLisays, L"AikaLisays"},
	{TAGOletusPiste, L"OletusPiste"},
	{TAGAaniLkm, L"AaniLkm"},
	{TAGKilpnoOn, L"KilpnoOn"},
	{TAGLisnoOn, L"LisnoOn"},
	{TAGSNimiOn, L"SNimiOn"},
	{TAGENimiOn, L"ENimiOn"},
	{TAGSarjaOn, L"SarjaOn"},
	{TAGSukupOn, L"SukupOn"},
	{TAGIkasrjOn, L"IkasrjOn"},
	{TAGWRKoodiOn, L"WRKoodiOn"},
	{TAGArvoOn, L"ArvoOn"},
	{TAGSeuraOn, L"SeuraOn"},
	{TAGSeuralyhOn, L"SeuralyhOn"},
	{TAGMaaOn, L"MaaOn"},
	{TAGYhdistysOn, L"YhdistysOn"},
	{TAGJoukkueOn, L"JoukkueOn"},
	{TAGLasnaOn, L"LasnaOn"},
	{TAGBibOn, L"BibOn"},
	{TAGBadgeOn, L"BadgeOn"},
	{TAGLAOn, L"LAOn"},
	{TAGTasOn, L"TasOn"},
	{TAGRataOn, L"RataOn"},
	{TAGPiiriOn, L"PiiriOn"},
	{TAGKnohakuOn, L"KnohakuOn"},
	{TAGLisnohakuOn, L"LisnohakuOn"},
	{TAGNimihakuOn, L"NimihakuOn"},
	{TAGNimiHThakuOn, L"NimiHThakuOn"},
	{TAGHTSarjaOn, L"HTSarjaOn"},
	{TAGSeurahakuOn, L"SeurahakuOn"},
	{TAGSeuralisaOn, L"SeuralisaOn"},
	{TAGMerkAvoimeksiOn, L"MerkAvoimeksiOn"},
	{TAGHTBadgeOn, L"HTBadgeOn"},
	{TAGAutoLaikaOn, L"AutoLaikaOn"},
	{TAGJIilmOn, L"JIilmOn"},
	{TAGAutoKnoOn, L"AutoKnoOn"},
	{TAGLisnoTarkOn, L"LisnoTarkOn"},
	{TAGLisaysIx, L"LisaysIx"},
	{TAGJarjestysIx, L"JarjestysIx"},
	{TAGPaivatIx, L"PaivatIx"},
	{TAGOId, L"OId"},
	{TAGOKilpno, L"OKilpno"},
	{TAGOLisno, L"OLisno"},
	{TAGOSNimi, L"OSNimi"},
	{TAGOENimi, L"OENimi"},
	{TAGOSarja, L"OSarja"},
	{TAGOWRKoodi, L"OWRKoodi"},
	{TAGOArvo, L"OArvo"},
	{TAGOSeura, L"OSeura"},
	{TAGOSeuralyh, L"OSeuralyh"},
	{TAGOMaa, L"OMaa"},
	{TAGOYhdistys, L"OYhdistys"},
	{TAGOJoukkue, L"OJoukkue"},
	{TAGOLasna, L"OLasna"},
	{TAGOBib, L"OBib"},
	{TAGOBadge, L"OBadge"},
	{TAGOLA, L"OLA"},
	{TAGOTas, L"OTas"},
	{TAGORata, L"ORata"},
	{TAGOPiiri, L"OPiiri"},
	{TAGOSukup, L"OSukup"},
	{TAGOIkasrj, L"OIkasrj"},
	{TAGEdtVakBibLisa, L"EdtVakBibLisa"},
	{TAGIlmKentat, L"IlmKentat"},
	{TAGNaytaSeura, L"NaytaSeura"},
	{TAGNaytaKortinKoodi, L"NaytaKortinKoodi"},
	{TAGHenkTietokanta, L"HenkTietokanta"},
	{TAGSeuraluettelo, L"Seuraluettelo"},
	{TAGVahvIlmAina, L"VahvIlmAina"},
	{TAGKuntoKone, L"KuntoKone"},
	{TAGMerkLainat, L"MerkLainat"},
	{TAGTietokantaSarjat, L"TietokantaSarjat"},
	{TAGVaadiToisto, L"VaadiToisto"},
	{TAGLueVialliset, L"LueVialliset"},
	{TAGEmitvaihtoVaiheille, L"EmitvaihtoVaiheille"},
	{TAGVarit, L"Varit"},
};

static int nIkkTags = sizeof(IkkTags)/sizeof(IkkTags[0]);

//---------------------------------------------------------------------------
int lueIkkunaXml(wchar_t *filename);

void __fastcall TFormMain::HaeIkkunat(UnicodeString FlNm)
{
	lueIkkunaXml(FlNm.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TallennaikkunatClick(TObject *Sender)
{
	static init = 0;

	if (!init) {
		SaveDialog1->FileName = GetCurrentDir()+UnicodeString(L"\\oletus.ikk");
		init = 1;
	}
	SaveDialog1->DefaultExt = L"ikk";
	SaveDialog1->Filter = L"Ikkunat (*.ikk)|*.IKK|Kaikki tiedostot (*.*)|*.*";
	if (SaveDialog1->Execute()) {
		TallennaIkkunat(SaveDialog1->FileName);
		}
	SetCurrentDir(CurrentDir);
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
			TilFrm[no-1]->TilanneFormNo = no;
			Frm = TilFrm[no-1];
			break;
		case L'V':
			if (no <= 0 || no >= NTAPFORM)
				return(1);
			if (FrmTap[no-1] == NULL)
				FrmTap[no-1] = new TTapForm(FormMain);
			Frm = FrmTap[no-1];
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
		case L'I':
			if (FormIlm2 == NULL)
				FormIlm2 = new TFormIlm2(FormMain);
			Frm = FormIlm2;
			break;
		case L'S':
			Frm = FormStatus;
			break;
		case L'B':
			if (FormASakot == NULL)
				FormASakot = new TFormASakot(FormMain);
			Frm = FormASakot;
			break;
		case L'C':
			if (FormAmpPaikat == NULL)
				FormAmpPaikat = new TFormAmpPaikat(FormMain);
			Frm = FormAmpPaikat;
			break;
		case L'D':
			if (FormSakkoKierr == NULL)
				FormSakkoKierr = new TFormSakkoKierr(FormMain);
			Frm = FormSakkoKierr;
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
							FrmTap[no-1]->IkkParam.readParams(node, inode, nnode);
							FrmTap[no-1]->applyParams();
							break;
						case L'A' :
							FormAjanotot[no-1]->IkkParam.readParams(node, inode, nnode);
							FormAjanotot[no-1]->applyParams();
							break;
						case L'R' :
							FormAikavertailut[no-1]->IkkParam.readParams(node, inode, nnode);
							FormAikavertailut[no-1]->applyParams();
							break;
						case L'I' :
							FormIlm2->IkkParam.readParams(node, inode, nnode);
							FormIlm2->applyParams();
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
					else
						no = 0;
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
	int ntag = 10000, haara = 0, inode, inode_in, depth = 0, rv, er = 0, lntype;
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
		swprintf(msg, L"XML-iedosto %s p‰‰ttyi rivill‰ %d tasolla %d", filename, rv, depth);
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
	wchar_t ln[402], *p;
	kilptietue Kilp;

	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			switch (node[*inode].tagid) {
				case TAGSarja:
					node[*inode].gettext(ln, LSARJA);
					Sarja = haesarja_w(ln, true);
					break;
				case TAGPiste:
					node[*inode].gettext_int(&Va);
					break;
				case TAGNaytaAlisarja:
					node[*inode].gettext_int(&val);
					naytaAlisarja = (val != 0);
					break;
				case TAGNaytaMaa:
					node[*inode].gettext_int(&val);
					naytaMaa = (val != 0);
					break;
				case TAGRajaus:
					node[*inode].gettext_int(&val);
					rajaus = (val != 0);
					break;
				case TAGEtuSuku:
					node[*inode].gettext_int(&val);
					EtuSuku = (val != 0);
					break;
				case TAGSeuratieto:
					node[*inode].gettext_int(&val);
					seuraTieto = val;
					break;
				case TAGKokonaistulos:
					node[*inode].gettext_int(&val);
					kokotulos = (val != 0);
					break;
				case TAGEroaika:
					node[*inode].gettext_int(&val);
					eroaika = (val != 0);
					break;
				case TAGEnnatys:
					node[*inode].gettext_int(&Ennatys);
					break;
				case TAGTavoite:
					node[*inode].gettext_int(&Tavoite);
					break;
				case TAGKierrosaika:
					node[*inode].gettext_int(&kierrosaika);
					break;
				case TAGKaikkiajat:
					node[*inode].gettext_int(&val);
					kaikkiajat = (val != 0);
					break;
				case TAGVainjarjestys:
					node[*inode].gettext_int(&val);
					vainjarj = (val != 0);
					break;
				case TAGSmart:
					node[*inode].gettext_int(&val);
					smartJrj = (val != 0);
					break;
				case TAGLahtoaika:
					node[*inode].gettext_int(&val);
					lahtoaika = (val != 0);
					break;
				case TAGYhteistulos:
					node[*inode].gettext_int(&val);
					yhttulos = (val != 0);
					break;
				case TAGViimeisin:
					node[*inode].gettext_int(&val);
					viimeisin = (val != 0);
					break;
				case TAGLaskeYhtpisteet:
					node[*inode].gettext_int(&val);
					laskeYhtpisteet = (val != 0);
					break;
				case TAGAaniLkm:
					node[*inode].gettext_int(&AaniLkm);
					break;
				case TAGKirjasinkoko:
					node[*inode].gettext_int(&val);
					FontSize = val;
					break;
				case TAGLihavoitu:
					node[*inode].gettext_int(&val);
					Lihavoitu = (val != 0);
					break;
				case TAGSarakeLev:
					node[*inode].gettext(ln, 400);
					for (UINT i = 0; i < sizeof(ColW)/sizeof(ColW[0]); i++) {
						wchar_t *p;
						if (i == 0)
							p = wcstok(ln, L";\n");
						else
							p = wcstok(NULL, L";\n");
						if (p == NULL)
							break;
						ColW[i] = _wtoi(p);
						}
					break;
				case TAGVarit:
					node[*inode].gettext(ln, 400);
					p = wcstok(ln, L";\n");
					if (p == NULL)
						break;
					AktColor = _wtoi(p);
					for (UINT i = 0; i < sizeof(Colors)/sizeof(Colors[0]); i++) {
						wchar_t *p;
						p = wcstok(NULL, L";\n");
						if (p == NULL)
							break;
						swscanf(p, L"%x", &Colors[i]);
						}
					break;
				case TAGAikaPituus:
					node[*inode].gettext_int(&laika);
					break;
				case TAGKorotusraja1:
					node[*inode].gettext_int(&korRaja[0]);
					break;
				case TAGKorotusraja2:
					node[*inode].gettext_int(&korRaja[1]);
					break;
				case TAGSkrollausvali:
					node[*inode].gettext_int(&val);
					ScrollVali = val;
					break;
				case TAGSkrollaus:
					node[*inode].gettext_int(&val);
					ScrollOn = (val != 0);
					break;
				case TAGSkrollRiviKesto:
					node[*inode].gettext_int(&val);
					ScrollRiviKesto = val;
					break;
				case TAGSkrollaustapa:
					node[*inode].gettext_int(&val);
					ScrollType = val;
					break;
				case TAGKokoruutu:
					node[*inode].gettext_int(&val);
					KokoRuutu = (val != 0);
					break;
				case TAGSkrollaussarjat:
					node[*inode].gettext(ln, sarjaluku+nsarjayhd);
					for (int i = 0; i < sarjaluku+nsarjayhd; i++)
						ScrollSarjat[i] =  ln[i] - '0';
					break;
				case TAGOdotusraja:
					node[*inode].gettext_int(&odRaja);
					break;
				case TAGnaytaHyl:
					node[*inode].gettext_int(&val);
					naytaHyl = (val != 0);
					break;
				case TAGseuraaLahtoja:
					node[*inode].gettext_int(&val);
					seuraaLahtoja = (val != 0);
					break;
				case TAGminTaso:
					node[*inode].gettext_int(&val);
					minTaso = (val != 0);
					break;
				case TAGnaytaPiilotetut:
					node[*inode].gettext_int(&val);
					naytaPiilotetut = (val != 0);
					break;
				case TAGnaytaOdotetut:
					node[*inode].gettext_int(&val);
					naytaOdotetut = (val != 0);
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
			if (node[*inode].tagid == TAGSeuratieto) {
				node[*inode].gettext_int(&seuraTieto);
				}
			if (node[*inode].tagid == TAGYhteistulos) {
				node[*inode].gettext_int(&val);
				yhttulos = (val != 0);
				}
			if (node[*inode].tagid == TAGEnnatys) {
				node[*inode].gettext_int(&Ennatys);
				}
			if (node[*inode].tagid == TAGTavoite) {
				node[*inode].gettext_int(&Tavoite);
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
				case TAGOletusPiste:
					node[*inode].gettext_int(&OletusPiste);
					break;
				case TAGAikaLisays:
					node[*inode].gettext_int(&AikaLisays);
					break;
				case TAGKirjasinkoko:
					node[*inode].gettext_int(&val);
					FontSize = val;
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
int ilmIkkParamClass::readParams(xml_node *node, int *inode, int nnode)
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
			switch (node[*inode].tagid) {
				case TAGIdOn:
					node[*inode].gettext_int(&val);
					IdOn = (val != 0);
					break;
				case TAGKilpnoOn:
					node[*inode].gettext_int(&val);
					KilpnoOn = (val != 0);
					break;
				case TAGLisnoOn:
					node[*inode].gettext_int(&val);
					LisnoOn = (val != 0);
					break;
				case TAGSNimiOn:
					node[*inode].gettext_int(&val);
					SNimiOn = (val != 0);
					break;
				case TAGENimiOn:
					node[*inode].gettext_int(&val);
					ENimiOn = (val != 0);
					break;
				case TAGSarjaOn:
					node[*inode].gettext_int(&val);
					SarjaOn = (val != 0);
					break;
				case TAGSukupOn:
					node[*inode].gettext_int(&val);
					SukupOn = (val != 0);
					break;
				case TAGIkasrjOn:
					node[*inode].gettext_int(&val);
					IkasrjOn = (val != 0);
					break;
				case TAGWRKoodiOn:
					node[*inode].gettext_int(&val);
					WRKoodiOn = (val != 0);
					break;
				case TAGArvoOn:
					node[*inode].gettext_int(&val);
					ArvoOn = (val != 0);
					break;
				case TAGSeuraOn:
					node[*inode].gettext_int(&val);
					SeuraOn = (val != 0);
					break;
				case TAGSeuralyhOn:
					node[*inode].gettext_int(&val);
					SeuralyhOn = (val != 0);
					break;
				case TAGMaaOn:
					node[*inode].gettext_int(&val);
					MaaOn = (val != 0);
					break;
				case TAGYhdistysOn:
					node[*inode].gettext_int(&val);
					YhdistysOn = (val != 0);
					break;
				case TAGJoukkueOn:
					node[*inode].gettext_int(&val);
					JoukkueOn = (val != 0);
					break;
				case TAGLasnaOn:
					node[*inode].gettext_int(&val);
					LasnaOn = (val != 0);
					break;
				case TAGBibOn:
					node[*inode].gettext_int(&val);
					BibOn = (val != 0);
					break;
				case TAGBadgeOn:
					node[*inode].gettext_int(&val);
					BadgeOn = (val != 0);
					break;
				case TAGLAOn:
					node[*inode].gettext_int(&val);
					LAOn = (val != 0);
					break;
				case TAGTasOn:
					node[*inode].gettext_int(&val);
					TasOn = (val != 0);
					break;
				case TAGRataOn:
					node[*inode].gettext_int(&val);
					RataOn = (val != 0);
					break;
				case TAGPiiriOn:
					node[*inode].gettext_int(&val);
					PiiriOn = (val != 0);
					break;
				case TAGKnohakuOn:
					node[*inode].gettext_int(&val);
					KnohakuOn = (val != 0);
					break;
				case TAGLisnohakuOn:
					node[*inode].gettext_int(&val);
					LisnohakuOn = (val != 0);
					break;
				case TAGNimihakuOn:
					node[*inode].gettext_int(&val);
					NimihakuOn = (val != 0);
					break;
				case TAGNimiHThakuOn:
					node[*inode].gettext_int(&val);
					NimiHThakuOn = (val != 0);
					break;
				case TAGHTSarjaOn:
					node[*inode].gettext_int(&val);
					HTSarjaOn = (val != 0);
					break;
				case TAGSeurahakuOn:
					node[*inode].gettext_int(&val);
					SeurahakuOn = (val != 0);
					break;
				case TAGSeuralisaOn:
					node[*inode].gettext_int(&val);
					SeuralisaOn = (val != 0);
					break;
				case TAGMerkAvoimeksiOn:
					node[*inode].gettext_int(&val);
					MerkAvoimeksiOn = (val != 0);
					break;
				case TAGHTBadgeOn:
					node[*inode].gettext_int(&val);
					HTBadgeOn = (val != 0);
					break;
				case TAGAutoLaikaOn:
					node[*inode].gettext_int(&val);
					AutoLaikaOn = (val != 0);
					break;
				case TAGJIilmOn:
					node[*inode].gettext_int(&val);
					JIilmOn = (val != 0);
					break;
				case TAGAutoKnoOn:
					node[*inode].gettext_int(&val);
					AutoKnoOn = (val != 0);
					break;
				case TAGLisnoTarkOn:
					node[*inode].gettext_int(&val);
					LisnoTarkOn = (val != 0);
					break;
				case TAGLisaysIx:
					node[*inode].gettext_int(&LisaysIx);
					break;
				case TAGJarjestysIx:
					node[*inode].gettext_int(&JarjestysIx);
					break;
				case TAGPaivatIx:
					node[*inode].gettext_int(&PaivatIx);
					break;
				case TAGOId:
					node[*inode].gettext_int(&OId);
					break;
				case TAGOKilpno:
					node[*inode].gettext_int(&OKilpno);
					break;
				case TAGOLisno:
					node[*inode].gettext_int(&OLisno);
					break;
				case TAGOSNimi:
					node[*inode].gettext_int(&OSNimi);
					break;
				case TAGOENimi:
					node[*inode].gettext_int(&OENimi);
					break;
				case TAGOSarja:
					node[*inode].gettext_int(&OSarja);
					break;
				case TAGOWRKoodi:
					node[*inode].gettext_int(&OWRKoodi);
					break;
				case TAGOArvo:
					node[*inode].gettext_int(&OArvo);
					break;
				case TAGOSeura:
					node[*inode].gettext_int(&OSeura);
					break;
				case TAGOSeuralyh:
					node[*inode].gettext_int(&OSeuralyh);
					break;
				case TAGOMaa:
					node[*inode].gettext_int(&OMaa);
					break;
				case TAGOYhdistys:
					node[*inode].gettext_int(&OYhdistys);
					break;
				case TAGOJoukkue:
					node[*inode].gettext_int(&OJoukkue);
					break;
				case TAGOLasna:
					node[*inode].gettext_int(&OLasna);
					break;
				case TAGOBib:
					node[*inode].gettext_int(&OBib);
					break;
				case TAGOBadge:
					node[*inode].gettext_int(&OBadge);
					break;
				case TAGOLA:
					node[*inode].gettext_int(&OLA);
					break;
				case TAGOTas:
					node[*inode].gettext_int(&OTas);
					break;
				case TAGORata:
					node[*inode].gettext_int(&ORata);
					break;
				case TAGOPiiri:
					node[*inode].gettext_int(&OPiiri);
					break;
				case TAGOSukup:
					node[*inode].gettext_int(&OSukup);
					break;
				case TAGOIkasrj:
					node[*inode].gettext_int(&OIkasrj);
					break;
				case TAGEdtVakBibLisa:
					node[*inode].gettext_int(&EdtVakBibLisa);
					break;
				case TAGIlmKentat:
					node[*inode].gettext_int(&IlmKentat);
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
				case TAGParasSija:
					node[*inode].gettext_int(&ParasSija);
					break;
				case TAGPalkJako:
					node[*inode].gettext_int(&val);
					PalkJako = (val != 0);
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
				case TAGNaytaSeura:
					node[*inode].gettext_int(&val);
					NaytaSeura = (val != 0);
					break;
				case TAGNaytaKortinKoodi:
					node[*inode].gettext_int(&val);
					NaytaKortinKoodi = (val != 0);
					break;
				case TAGHenkTietokanta:
					node[*inode].gettext_int(&val);
					HenkTietokanta = (val != 0);
					break;
				case TAGSeuraluettelo:
					node[*inode].gettext_int(&val);
					Seuraluettelo = (val != 0);
					break;
				case TAGVahvIlmAina:
					node[*inode].gettext_int(&val);
					VahvIlmAina = (val != 0);
					break;
				case TAGKuntoKone:
					node[*inode].gettext_int(&KuntoKone);
					break;
				case TAGMerkLainat:
					node[*inode].gettext_int(&val);
					MerkLainat = (val != 0);
					break;
				case TAGTietokantaSarjat:
					node[*inode].gettext_int(&val);
					TietokantaSarjat = (val != 0);
					break;
				case TAGVaadiToisto:
					node[*inode].gettext_int(&val);
					VaadiToisto = (val != 0);
					break;
				case TAGLueVialliset:
					node[*inode].gettext_int(&val);
					LueVialliset = (val != 0);
					break;
				case TAGEmitvaihtoVaiheille:
					node[*inode].gettext_int(&val);
					EmitvaihtoVaiheille = (val != 0);
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
				case TAGLahtoRaja:
					node[*inode].gettext_int(&LahtoRaja);
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
		tallfl->WriteLine(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
		swprintf(ln, L"%s createTime=\"%s\" creator=\"HkKisaWin %s\"", L"Ikkunat", ISOdatetime(0,1), Build());
		tallfl->put_wtag(ln, level++);
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
			if (FrmTap[i] && FrmTap[i]->Visible) {
				swprintf(ln, L"Ikkuna Laji=\"Tapahtumat\" Numero=\"%d\"", i+1);
				tallfl->put_wtag(ln, level++);
				TallennaIkkunaYht(tallfl, FrmTap[i], level);
				tallfl->put_wtag(L"IkkunaParametrit", level++);
				FrmTap[i]->IkkParam.writeParams(tallfl, level);
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
		if (FormIlm2 && FormIlm2->Visible) {
			tallfl->put_wtag(L"Ikkuna Laji=\"Ilmoittautumiset\"", level++);
			TallennaIkkunaYht(tallfl, FormIlm2, level);
			tallfl->put_wtag(L"IkkunaParametrit", level++);
			FormIlm2->haeParams();
			FormIlm2->IkkParam.writeParams(tallfl, level);
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
		if (FormASakot && FormASakot->Visible) {
			tallfl->put_wtag(L"Ikkuna Laji=\"AHSakot\"", level++);
			TallennaIkkunaYht(tallfl, FormASakot, level);
			tallfl->put_wantitag(L"Ikkuna", --level);
			}
		if (FormAmpPaikat && FormAmpPaikat->Visible) {
			tallfl->put_wtag(L"Ikkuna Laji=\"AmpPaikat\"", level++);
			TallennaIkkunaYht(tallfl, FormAmpPaikat, level);
			tallfl->put_wantitag(L"Ikkuna", --level);
			}
		if (FormSakkoKierr && FormSakkoKierr->Visible) {
			tallfl->put_wtag(L"Ikkuna Laji=\"SakkoKierr\"", level++);
			TallennaIkkunaYht(tallfl, FormSakkoKierr, level);
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

	outfl->put_wxml_s(XMLhae_tagName(TAGSarja, IkkTags, nIkkTags), Sarjat[Sarja].sarjanimi, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGPiste, IkkTags, nIkkTags), Va, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGNaytaAlisarja, IkkTags, nIkkTags), (int) naytaAlisarja, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGNaytaMaa, IkkTags, nIkkTags), (int) naytaMaa, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGnaytaHyl, IkkTags, nIkkTags), (int) naytaHyl, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGseuraaLahtoja, IkkTags, nIkkTags), (int) seuraaLahtoja, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGminTaso, IkkTags, nIkkTags), (int) minTaso, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGnaytaPiilotetut, IkkTags, nIkkTags), (int) naytaPiilotetut, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGnaytaOdotetut, IkkTags, nIkkTags), (int) naytaOdotetut, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGRajaus, IkkTags, nIkkTags), (int) rajaus, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGEtuSuku, IkkTags, nIkkTags), (int) EtuSuku, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSeuratieto, IkkTags, nIkkTags), seuraTieto, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKokonaistulos, IkkTags, nIkkTags), (int) kokotulos, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGEroaika, IkkTags, nIkkTags), (int) eroaika, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKierrosaika, IkkTags, nIkkTags), (int) kierrosaika, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKaikkiajat, IkkTags, nIkkTags), (int) kaikkiajat, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGEnnatys, IkkTags, nIkkTags), Ennatys, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGTavoite, IkkTags, nIkkTags), Tavoite, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGVainjarjestys, IkkTags, nIkkTags), (int) vainjarj, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSmart, IkkTags, nIkkTags), (int) smartJrj, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGLahtoaika, IkkTags, nIkkTags), (int) lahtoaika, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGYhteistulos, IkkTags, nIkkTags), (int) yhttulos, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGViimeisin, IkkTags, nIkkTags), (int) viimeisin, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGLaskeYhtpisteet, IkkTags, nIkkTags), (int) laskeYhtpisteet, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGAaniLkm, IkkTags, nIkkTags), AaniLkm, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKirjasinkoko, IkkTags, nIkkTags), FontSize , level);
	outfl->put_wxml_d(XMLhae_tagName(TAGLihavoitu, IkkTags, nIkkTags), (int) Lihavoitu, level);
	for (UINT i = 0; i < sizeof(ColW)/sizeof(ColW[0]); i++) {
		if (i > 0)
			wcscat(st, L";");
		_itow(ColW[i], st+wcslen(st), 10);
		}
	outfl->put_wxml_s(XMLhae_tagName(TAGSarakeLev, IkkTags, nIkkTags), st, level);
	_itow(AktColor, st, 10);
	wcscat(st, L";");
	for (UINT i = 0; i < sizeof(Colors)/sizeof(Colors[0]); i++) {
		if (i > 0)
			wcscat(st, L";");
		_itow(Colors[i], st+wcslen(st), 16);
		}
	outfl->put_wxml_s(XMLhae_tagName(TAGVarit, IkkTags, nIkkTags), st, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGAikaPituus, IkkTags, nIkkTags), laika, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKorotusraja1, IkkTags, nIkkTags), korRaja[0] , level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKorotusraja2, IkkTags, nIkkTags), korRaja[1] , level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSkrollausvali, IkkTags, nIkkTags), ScrollVali , level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSkrollaus, IkkTags, nIkkTags), (int) ScrollOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSkrollRiviKesto, IkkTags, nIkkTags), ScrollRiviKesto , level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSkrollaustapa, IkkTags, nIkkTags), ScrollType , level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKokoruutu, IkkTags, nIkkTags), (int) KokoRuutu, level);
	memset(st, 0, sizeof(st));
	for (int i = 0; i < sarjaluku+nsarjayhd; i++)
		st[i] = ScrollSarjat[i] + '0';
	outfl->put_wxml_s(XMLhae_tagName(TAGSkrollaussarjat, IkkTags, nIkkTags), st, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOdotusraja, IkkTags, nIkkTags), odRaja , level);
	return(0);
}

//---------------------------------------------------------------------------
int tapIkkParamClass::writeParams(TextFl *outfl, int level)
{
	wchar_t st[1000] = L"";

	outfl->put_wxml_d(XMLhae_tagName(TAGPiste, IkkTags, nIkkTags), Va, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGNaytaMaa, IkkTags, nIkkTags), (int) naytaMaa, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKokoruutu, IkkTags, nIkkTags), (int) KokoRuutu, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGEtuSuku, IkkTags, nIkkTags), (int) EtuSuku, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSeuratieto, IkkTags, nIkkTags), seuraTieto, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGYhteistulos, IkkTags, nIkkTags), (int) yhttulos, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGEnnatys, IkkTags, nIkkTags), Ennatys, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGTavoite, IkkTags, nIkkTags), Tavoite, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKirjasinkoko, IkkTags, nIkkTags), FontSize , level);
	outfl->put_wxml_d(XMLhae_tagName(TAGLihavoitu, IkkTags, nIkkTags), (int) Lihavoitu, level);
	for (UINT i = 0; i < sizeof(ColW)/sizeof(ColW[0]); i++) {
		if (i > 0)
			wcscat(st, L";");
		_itow(ColW[i], st+wcslen(st), 10);
		}
	outfl->put_wxml_s(XMLhae_tagName(TAGSarakeLev, IkkTags, nIkkTags), st, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGAikaPituus, IkkTags, nIkkTags), laika, level);
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
	outfl->put_wxml_d(XMLhae_tagName(TAGOletusPiste, IkkTags, nIkkTags), OletusPiste, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGAikaLisays, IkkTags, nIkkTags), AikaLisays, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKirjasinkoko, IkkTags, nIkkTags), FontSize , level);
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
int ilmIkkParamClass::writeParams(TextFl *outfl, int level)
{
	outfl->put_wxml_d(XMLhae_tagName(TAGIdOn, IkkTags, nIkkTags), (int) IdOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKilpnoOn, IkkTags, nIkkTags), (int) KilpnoOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGLisnoOn, IkkTags, nIkkTags), (int) LisnoOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSNimiOn, IkkTags, nIkkTags), (int) SNimiOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGENimiOn, IkkTags, nIkkTags), (int) ENimiOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSarjaOn, IkkTags, nIkkTags), (int) SarjaOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSukupOn, IkkTags, nIkkTags), (int) SukupOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGIkasrjOn, IkkTags, nIkkTags), (int) IkasrjOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGWRKoodiOn, IkkTags, nIkkTags), (int) WRKoodiOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGArvoOn, IkkTags, nIkkTags), (int) ArvoOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSeuraOn, IkkTags, nIkkTags), (int) SeuraOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSeuralyhOn, IkkTags, nIkkTags), (int) SeuralyhOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGMaaOn, IkkTags, nIkkTags), (int) MaaOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGYhdistysOn, IkkTags, nIkkTags), (int) YhdistysOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGJoukkueOn, IkkTags, nIkkTags), (int) JoukkueOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGLasnaOn, IkkTags, nIkkTags), (int) LasnaOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGBibOn, IkkTags, nIkkTags), (int) BibOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGBadgeOn, IkkTags, nIkkTags), (int) BadgeOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGLAOn, IkkTags, nIkkTags), (int) LAOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGTasOn, IkkTags, nIkkTags), (int) TasOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGRataOn, IkkTags, nIkkTags), (int) RataOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGPiiriOn, IkkTags, nIkkTags), (int) PiiriOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKnohakuOn, IkkTags, nIkkTags), (int) KnohakuOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGLisnohakuOn, IkkTags, nIkkTags), (int) LisnohakuOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGNimihakuOn, IkkTags, nIkkTags), (int) NimihakuOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGNimiHThakuOn, IkkTags, nIkkTags), (int) NimiHThakuOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGHTSarjaOn, IkkTags, nIkkTags), (int) HTSarjaOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSeurahakuOn, IkkTags, nIkkTags), (int) SeurahakuOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSeuralisaOn, IkkTags, nIkkTags), (int) SeuralisaOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGMerkAvoimeksiOn, IkkTags, nIkkTags), (int) MerkAvoimeksiOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGHTBadgeOn, IkkTags, nIkkTags), (int) HTBadgeOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGAutoLaikaOn, IkkTags, nIkkTags), (int) AutoLaikaOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGJIilmOn, IkkTags, nIkkTags), (int) JIilmOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGAutoKnoOn, IkkTags, nIkkTags), (int) AutoKnoOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGLisnoTarkOn, IkkTags, nIkkTags), (int) LisnoTarkOn, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGLisaysIx, IkkTags, nIkkTags), (int) LisaysIx, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGJarjestysIx, IkkTags, nIkkTags), (int) JarjestysIx, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGPaivatIx, IkkTags, nIkkTags), (int) PaivatIx, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOId, IkkTags, nIkkTags), (int) OId, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOKilpno, IkkTags, nIkkTags), (int) OKilpno, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOLisno, IkkTags, nIkkTags), (int) OLisno, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOSNimi, IkkTags, nIkkTags), (int) OSNimi, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOENimi, IkkTags, nIkkTags), (int) OENimi, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOSarja, IkkTags, nIkkTags), (int) OSarja, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOWRKoodi, IkkTags, nIkkTags), (int) OWRKoodi, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOArvo, IkkTags, nIkkTags), (int) OArvo, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOSeura, IkkTags, nIkkTags), (int) OSeura, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOSeuralyh, IkkTags, nIkkTags), (int) OSeuralyh, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOMaa, IkkTags, nIkkTags), (int) OMaa, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOYhdistys, IkkTags, nIkkTags), (int) OYhdistys, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOJoukkue, IkkTags, nIkkTags), (int) OJoukkue, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOLasna, IkkTags, nIkkTags), (int) OLasna, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOBib, IkkTags, nIkkTags), (int) OBib, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOBadge, IkkTags, nIkkTags), (int) OBadge, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOLA, IkkTags, nIkkTags), (int) OLA, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOTas, IkkTags, nIkkTags), (int) OTas, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGORata, IkkTags, nIkkTags), (int) ORata, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOPiiri, IkkTags, nIkkTags), (int) OPiiri, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOSukup, IkkTags, nIkkTags), (int) OSukup, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOIkasrj, IkkTags, nIkkTags), (int) OIkasrj, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGEdtVakBibLisa, IkkTags, nIkkTags), (int) EdtVakBibLisa, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGIlmKentat, IkkTags, nIkkTags), (int) IlmKentat, level);
	return(0);
}

//---------------------------------------------------------------------------
int statusIkkParamClass::writeParams(TextFl *outfl, int level)
{
	outfl->put_wxml_d(XMLhae_tagName(TAGParasSija, IkkTags, nIkkTags), (int) ParasSija, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGPalkJako, IkkTags, nIkkTags), (int) PalkJako, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGYhteysIkkuna, IkkTags, nIkkTags), (int) yhteysikkuna, level);
	return(0);
}
//---------------------------------------------------------------------------

int emitIkkParamClass::writeParams(TextFl *outfl, int level)
{
	outfl->put_wxml_d(XMLhae_tagName(TAGNaytaSeura, IkkTags, nIkkTags), (int) NaytaSeura, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGNaytaKortinKoodi, IkkTags, nIkkTags), (int) NaytaKortinKoodi, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGHenkTietokanta, IkkTags, nIkkTags), (int) HenkTietokanta, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGSeuraluettelo, IkkTags, nIkkTags), (int) Seuraluettelo, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGVahvIlmAina, IkkTags, nIkkTags), (int) VahvIlmAina, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGKuntoKone, IkkTags, nIkkTags), (int) KuntoKone, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGMerkLainat, IkkTags, nIkkTags), (int) MerkLainat, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGTietokantaSarjat, IkkTags, nIkkTags), (int) TietokantaSarjat, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGVaadiToisto, IkkTags, nIkkTags), (int) VaadiToisto, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGLueVialliset, IkkTags, nIkkTags), (int) LueVialliset, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGEmitvaihtoVaiheille, IkkTags, nIkkTags), (int) EmitvaihtoVaiheille, level);
	return(0);
}
//---------------------------------------------------------------------------
int NaytaTulosIkkParamClass::writeParams(TextFl *outfl, int level)
{
	outfl->put_wxml_d(XMLhae_tagName(TAGNaytaTulos, IkkTags, nIkkTags), (int) NaytaTulos, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOkKesto, IkkTags, nIkkTags), (int) OkKesto, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGOngKesto, IkkTags, nIkkTags), (int) OngKesto, level);
	outfl->put_wxml_d(XMLhae_tagName(TAGLahtoRaja, IkkTags, nIkkTags), (int) LahtoRaja, level);
	for (int r = 0; r < sizeof(Teksti) / sizeof(Teksti[0]); r++)
		if (Teksti[r][0]) {
			outfl->put_wxml_s(XMLhae_tagName(TAGTeksti, IkkTags, nIkkTags), Teksti[r], level,
				(UnicodeString(L"rivi=\"")+UnicodeString(r+1)+L"\"").c_str());
			}
	return(0);
}
//---------------------------------------------------------------------------


