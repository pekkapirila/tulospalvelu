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

#include "UnitSivuTuloste.h"
#ifndef MAXOSUUSLUKU
#include "WinHk.h"
#include "HkMuotoilu.h"
#else
#include "UnitMain.h"
#endif
#include "TulostusUnit2.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormSivuTuloste *FormSivuTuloste;

extern PageFieldTp PageField[];
extern UINT PageFieldCount;

//---------------------------------------------------------------------------
#define FRM_SivuMuotoilu    100
#define FRM_Kentta        	106
#define FRM_Muuttuja		112
#define FRM_VaakaPos     	113
#define FRM_PystyPos    	114
#define FRM_Merkkeja    	115
#define FRM_Koko        	116
#define FRM_Tasaus        	117
#define FRM_Valinta        	120

static tagListTp FrmTags[] = {
	{FRM_SivuMuotoilu, L"SivuMuotoilu"},
	{FRM_Kentta, L"Kentta"},
	{FRM_Muuttuja, L"Muuttuja"},
	{FRM_VaakaPos, L"VaakaPos"},
	{FRM_PystyPos, L"PystyPos"},
	{FRM_Merkkeja, L"Merkkeja"},
	{FRM_Koko, L"Koko"},
	{FRM_Tasaus, L"Tasaus"},
	{FRM_Valinta, L"Valinta"}
};

wchar_t *ValText[] = {L"Kaikki", L"Yksi", L"Sarja"};
static int nFrmTags = sizeof(FrmTags)/sizeof(FrmTags[0]);

//---------------------------------------------------------------------------
__fastcall TFormSivuTuloste::TFormSivuTuloste(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormSivuTuloste::FormResize(TObject *Sender)
{
	SG1->Height = ClientHeight - SG1->Top - 4;
}
//---------------------------------------------------------------------------
void __fastcall TFormSivuTuloste::BtnOtaKayttoonClick(TObject *Sender)
{
	for (int r = 1; r <= PageFieldCount; r++) {
		PageField[r-1].hpos = _wtoi(SG1->Cells[1][r].c_str());
		PageField[r-1].vpos = _wtoi(SG1->Cells[2][r].c_str());
		PageField[r-1].len = _wtoi(SG1->Cells[3][r].c_str());
		PageField[r-1].koko = _wtoi(SG1->Cells[4][r].c_str());
		PageField[r-1].oik = towupper(SG1->Cells[5][r][1]) == L'O' ? 1 : 0;
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSivuTuloste::CBJatkuvaClick(TObject *Sender)
{
	lisaToimetOn = CBJatkuva->Checked;
	if (lisaToimetOn) {
		sivutulostusparam = lsttulparam;
		if (sivutulostusparam.lstf == NULL) {
			sivutulostusparam.printer = GDIPRINTER;
			merkisto(L'G');
			if ((sivutulostusparam.lstf =
				openprfile(TulosteForm->KirjoitinVal->Text.c_str(),
				1, TRUE, FALSE, trlate, FALSE))	== NULL) {
				return;
				}
			if (sivutulostusparam.lstf && sivutulostusparam.lstf->prno == 2)
				initgdiprinter(&sivutulostusparam);
			}
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSivuTuloste::BtnKirjoitaClick(TObject *Sender)
{
	kilptietue kilp;
	int kno;

	sivutulostusparam = lsttulparam;
	if (sivutulostusparam.lstf == NULL) {
		sivutulostusparam.printer = GDIPRINTER;
		merkisto(L'G');
		if ((sivutulostusparam.lstf =
			openprfile(TulosteForm->KirjoitinVal->Text.c_str(),
			1, TRUE, FALSE, trlate, FALSE))	== NULL) {
			return;
			}
		if (sivutulostusparam.lstf && sivutulostusparam.lstf->prno == 2)
			initgdiprinter(&sivutulostusparam);
		}
	kno = _wtoi(EdtKno->Text.c_str());
	for	(int d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (kilp.kilpstatus || !kilp.lasna())
			continue;
		if (CBValinta->ItemIndex == 1 && kno != kilp.id())
			continue;
		if (CBValinta->ItemIndex > 1 && CBValinta->ItemIndex-2 != kilp.Sarja())
			continue;
		sivuTuloste(&kilp);
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSivuTuloste::Suljekaavake1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormSivuTuloste::allennatehdytvalinnat1Click(TObject *Sender)
{
	SaveDialog1->InitialDir = FormMain->CurrentDir;
	SaveDialog1->FileName = PgMaarFlNm;
	SaveDialog1->DefaultExt = L"xml";
	SaveDialog1->Filter = L"Xml-tiedosto (*.xml)|*.xml|Kaikki tiedostot (*.*)|*.*";
	if (SaveDialog1->Execute()) {
		KirjoitaValinnat(SaveDialog1->FileName.c_str());
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormSivuTuloste::Luevalinnattiedostosta1Click(TObject *Sender)
{
	OpenDialog1->InitialDir = FormMain->CurrentDir;
	OpenDialog1->FileName = PgMaarFlNm;
	OpenDialog1->DefaultExt = L"xml";
	OpenDialog1->Filter = L"Xml-tiedosto (*.xml)|*.xml|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		LueValinnat(OpenDialog1->FileName.c_str());
		}
}

//---------------------------------------------------------------------------

int TFormSivuTuloste::tulkXMLSivuMuotoilu(xml_node *node, int nnode)
{
	int er = 0, inode, DepthIn, haara = 0, iassign = -1, rv = 0, lj;
	wchar_t ln[40];

	XMLhaenodeid(node, nnode, FrmTags, nFrmTags);
	DepthIn = node[0].depth + 1;
	for (inode = 1; inode <= nnode; inode++) {
		if (node[inode].depth == DepthIn) {
			haara = 0;
			switch (node[inode].tagid) {
				case FRM_Kentta :
					haara = 1;
					rv++;
					break;
				case FRM_Valinta :
					node[inode].getparam_text(L"laji", ln, 30);
					for (lj = 0; lj < sizeof(ValText)/sizeof(ValText[0]); lj++)
						if (wcscmp(ln, ValText[lj]) == 0)
							break;
					if (lj == 2) {
						node[inode].gettext(ln, 30);
						lj = 2 + haesarja_w(ln, true);
						if (lj < 2)
							lj = 2;
						}
					CBValinta->ItemIndex = lj;
					break;
				}
			continue;
			}
		if (node[inode].depth == DepthIn + 1) {
			if (haara == 1 && rv > 0 && rv <= PageFieldCount) {
				switch (node[inode].tagid) {
					case FRM_Muuttuja:
						node[inode].gettext(PageField[rv-1].name, sizeof(PageField[0].name)/2-1);
						break;
					case FRM_VaakaPos:
						node[inode].gettext_int(&PageField[rv-1].hpos);
						break;
					case FRM_PystyPos:
						node[inode].gettext_int(&PageField[rv-1].vpos);
						break;
					case FRM_Merkkeja:
						node[inode].gettext_int(&PageField[rv-1].len);
						break;
					case FRM_Koko:
						node[inode].gettext_int(&PageField[rv-1].koko);
						break;
					case FRM_Tasaus:
						node[inode].gettext_int(&PageField[rv-1].oik);
						break;
					}
				}
			continue;
			}
		}
	return(er);
}

//---------------------------------------------------------------------------

int __fastcall TFormSivuTuloste::LueValinnat(wchar_t *MuotFile)
{
	TextFl *infile = NULL;
	int ntag = 100, haara = 0, inode, inode_in, depth = 0, rv, er = 0, lntype;
	short k[MAXXMLDEPTH];
	wchar_t line[1000], *txtbuf[MAXXMLDEPTH];
	wchar_t msg[200] = L"", tag[MAXXMLTAGLEN+1];
	xml_tree tree;
	xml_node nd;

	memset(k, 0, sizeof(k));
	infile = new TextFl(MuotFile, L"rt");
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
				nd.tagid = XMLhae_tagid(nd.tag, FrmTags, nFrmTags);
			if (lntype == 1) {
				tree.node[++inode] = nd;
				k[depth++]++;
				if (depth == 1) {
					inode_in = inode;
					switch (nd.tagid) {
						case FRM_SivuMuotoilu :
							haara = 0;
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er)
								tulkXMLSivuMuotoilu(tree.node+inode_in, inode-inode_in);
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
			else if (lntype == 8) {                  // sulkeva tag uudella rivillä
				int jnode;
				depth--;
				k[depth]++;                 // kasvatetaan tason laskuria sulkevän tagin tasolla
				// Etsitään vastaava avaava tag aiemmasta ketjusta
				for (jnode = inode; jnode > 0 && tree.node[jnode].depth > depth; jnode--) ;
				if (wcscmp(tag, tree.node[jnode].tag)) {      // Varmistetaan, että tagit ovat pari
					er = 1;
					swprintf(msg, L"XML-muotoilun tulkinta ei onnnistu. Rivi %d, Tag: '%.30s', odotettu: '%.30s'",
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
		swprintf(msg, L"XML-tiedosto %s päättyi rivillä %d tasolla %d", MuotFile, rv, depth);
		}
	if (er) {
		if (!msg[0]) {
			swprintf(msg, L"Tiedostossa %s virhe rivillä %d tai aikaisemmin", MuotFile, rv);
			}
		writeerror_w(msg, 0, true);
		}
	tree.nodecount = inode+1;
	NaytaValinnat();
	return(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormSivuTuloste::KirjoitaValinnat(wchar_t *flnm)
{
	int level = 0, nfld;
	TextFl *tallfl = NULL;
	wchar_t ln[400];

	tallfl = new TextFl(flnm, L"w");
	if (tallfl->IsOpen()) {
		tallfl->WriteLine(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
		swprintf(ln, L"%s createTime=\"%s\" creator=\"HkKisaWin %s\"", L"SivuMuotoilu", ISOdatetime(0,1), Build());
		tallfl->put_wtag(ln, level++);

		for (int r = 1; r <= PageFieldCount; r++) {
			if (PageField[r-1].name[0] == 0)
				continue;
			tallfl->put_wtagparams(XMLhae_tagName(FRM_Kentta, FrmTags, nFrmTags), (L"rivi='"+UnicodeString(r)+L"'").c_str(), false, level++);
			tallfl->put_wxml_s(XMLhae_tagName(FRM_Muuttuja, FrmTags, nFrmTags), PageField[r-1].name, level);
			tallfl->put_wxml_d(XMLhae_tagName(FRM_VaakaPos, FrmTags, nFrmTags), PageField[r-1].hpos, level);
			tallfl->put_wxml_d(XMLhae_tagName(FRM_PystyPos, FrmTags, nFrmTags), PageField[r-1].vpos, level);
			tallfl->put_wxml_d(XMLhae_tagName(FRM_Merkkeja, FrmTags, nFrmTags), PageField[r-1].len, level);
			tallfl->put_wxml_d(XMLhae_tagName(FRM_Koko, FrmTags, nFrmTags), PageField[r-1].koko, level);
			tallfl->put_wxml_d(XMLhae_tagName(FRM_Tasaus, FrmTags, nFrmTags), PageField[r-1].oik, level);
			tallfl->put_wantitag(XMLhae_tagName(FRM_Kentta, FrmTags, nFrmTags), --level);
			}
		if (CBValinta->ItemIndex <= 1)
			tallfl->put_wtagparams(XMLhae_tagName(FRM_Valinta, FrmTags, nFrmTags),
				wconcat(L"laji=\"", ValText[CBValinta->ItemIndex], L"\""), true, level);
		else
			tallfl->put_wxml_s(XMLhae_tagName(FRM_Valinta, FrmTags, nFrmTags), CBValinta->Text.c_str(), level,
				L"laji=\"Sarja\"");
		}
	if (tallfl)
		delete tallfl;
}
//---------------------------------------------------------------------------
void __fastcall TFormSivuTuloste::NaytaValinnat(void)
{
	for (int r = 1; r <= PageFieldCount; r++) {
		SG1->Cells[0][r] = PageField[r-1].name;
		SG1->Cells[1][r] = PageField[r-1].hpos;
		SG1->Cells[2][r] = PageField[r-1].vpos;
		SG1->Cells[3][r] = PageField[r-1].len;
		SG1->Cells[4][r] = PageField[r-1].koko;
		SG1->Cells[5][r] = PageField[r-1].oik ? L"Oik" : L"Vas";
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormSivuTuloste::FormCreate(TObject *Sender)
{
	SG1->ColWidths[0] = 60 * Screen->PixelsPerInch / 96;
	SG1->Cells[0][0] = L"Kenttä";
	SG1->Cells[1][0] = L"X-pos";
	SG1->Cells[2][0] = L"Y-pos";
	SG1->Cells[3][0] = L"Pituus";
	SG1->Cells[4][0] = L"Koko";
	SG1->Cells[5][0] = L"Tas.";
	SG1->RowCount = PageFieldCount+1;
	NaytaValinnat();
	CBJatkuva->Checked = lisaToimetOn;
	CBValinta->Clear();
	CBValinta->Items->Add(L"Kaikki läsnäolevat");
	CBValinta->Items->Add(L"Oheinen numero");
	for (int srj = 0; srj < sarjaluku+nsarjayhd; srj++) {
		CBValinta->Items->Add(Sarjat[srj].sarjanimi);
		}
	TextFl *InFile;
	wcscpy(PgMaarFlNm, L"PgTulMaar.xml");
	LueValinnat(PgMaarFlNm);
}
//---------------------------------------------------------------------------

void __fastcall TFormSivuTuloste::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Kilpailijakohtaiset tulosteet");
}
//---------------------------------------------------------------------------

