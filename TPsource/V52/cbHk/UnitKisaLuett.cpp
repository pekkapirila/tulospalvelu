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

#include "WinHk.h"
#include "UnitKisaLuett.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormKisaLuett *FormKisaLuett;

#define TAGRacelist			   0
#define TAGRace  		   	   1
#define TAGCode  		   	   2
#define TAGDate  		   	   3
#define TAGName  		   	   4
#define TAGPath  		   	   5
#define TAGCfg  		   	   6


static wchar_t *rctag[8] = {
	L"Racelist",
	L"Race",
	L"Code",
	L"Date",
	L"Name",
	L"Path",
	L"Cfg",
	L""
};

//---------------------------------------------------------------------------
__fastcall TFormKisaLuett::TFormKisaLuett(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormKisaLuett::FormCreate(TObject *Sender)
{
	SG1->ColWidths[0] = 50 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[1] = 65 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[2] = 180 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[3] = 180 * Screen->PixelsPerInch / 96;
	SG1->ColWidths[4] = 90 * Screen->PixelsPerInch / 96;
	SG1->Cells[0][0] = L"Koodi";
	SG1->Cells[1][0] = L"Pvm.";
	SG1->Cells[2][0] = L"Nimi";
	SG1->Cells[3][0] = L"Hakemisto";
	SG1->Cells[4][0] = L"Konfiguraatio";
}
//---------------------------------------------------------------------------
void TFormKisaLuett::kirjKilpLuett(wchar_t *outfname, kisaLuettTp *kisat, int lkm)
{
	TextFl *outfl;
	INT   level = 0;
	wchar_t st[40];

	outfl = new TextFl(outfname, L"wt");
	if (outfl->IsOpen()) {
		outfl->WriteLine(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
		outfl->put_wtag(rctag[TAGRacelist], level++);
		for (int i = 0; i < lkm && kisat[i].koodi[0]; i++) {
			outfl->put_wtag(rctag[TAGRace], level++);
			outfl->put_wxml_s(rctag[TAGCode], kisat[i].koodi, level);
			outfl->put_wxml_s(rctag[TAGDate], stdDateStr(st, kisat[i].date), level);
			outfl->put_wxml_s(rctag[TAGName], kisat[i].nimi, level);
			if (wcslen(kisat[i].polku) > 0 && kisat[i].polku[wcslen(kisat[i].polku)-1] == L'\\')
				kisat[i].polku[wcslen(kisat[i].polku)-1] = 0;
			outfl->put_wxml_s(rctag[TAGPath], kisat[i].polku, level);
			outfl->put_wxml_s(rctag[TAGCfg], kisat[i].cfgfile, level);
			outfl->put_wantitag(rctag[TAGRace], --level);
			}
		outfl->put_wantitag(rctag[TAGRacelist], --level);
		}
	delete outfl;
}
//---------------------------------------------------------------------------

static int hae_tagid(wchar_t *Tag)
{
	for (unsigned int i = 0; i < sizeof(rctag)/sizeof(rctag[0]); i++)
		if (!wcscmp(Tag, rctag[i]))
			return(i);
	return(-1);
}
//---------------------------------------------------------------------------

static void haenodeid(xml_node *node, int nnode)
{
	for (int inode = 0; inode <= nnode; inode++)
		node[inode].tagid = hae_tagid(node[inode].tag);
}
//---------------------------------------------------------------------------

static int loadRaceinfo(kisaLuettTp *kisa, xml_node *node, int *inode, int nnode)
{
	int er = 0, DepthIn, haara, iC = 0;
	wchar_t st[100];

	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			haara = 0;
			switch (node[*inode].tagid) {
				case TAGCode :
					node[*inode].gettext(kisa->koodi, sizeof(kisa->koodi)/2-1);
					break;
				case TAGDate :
					node[*inode].gettext(st, sizeof(st)/2-1);
					kisa->date = decodeStdDateStr(st);
					break;
				case TAGName :
					node[*inode].gettext(kisa->nimi, sizeof(kisa->nimi)/2-1);
					break;
				case TAGPath :
					node[*inode].gettext(kisa->polku, sizeof(kisa->polku)/2-1);
					break;
				case TAGCfg :
					node[*inode].gettext(kisa->cfgfile, sizeof(kisa->cfgfile)/2-1);
					break;
				}
			}
		}
	return(er);
}
//---------------------------------------------------------------------------

static int loadRacelist(xml_node *node, int nnode, kisaLuettTp *kisat, int *lkm)
{
	int er = 0, inode, DepthIn, ikisa = 0;
	INT  maxlkm;

	maxlkm = *lkm;
	*lkm = 0;
	memset(kisat, 0, *lkm * sizeof(kisat[0]));
	haenodeid(node, nnode);
	DepthIn = node[0].depth + 1;
	for (inode = 1; inode <= nnode; inode++) {
		if (node[inode].depth == DepthIn) {
			switch (node[inode].tagid) {
				case TAGRace :
					if ((er = loadRaceinfo(kisat+ikisa,node, &inode, nnode)) != 0)
						return(er);
					ikisa++;
					*lkm = ikisa;
					break;
				}
			if (*lkm >= maxlkm-1)
				break;
			continue;
			}
		}
	return(er);
}
//---------------------------------------------------------------------------

int TFormKisaLuett::lueKilpLuett(wchar_t *infname, kisaLuettTp *kisat, int *lkm)
{
	TextFl *infile;
	int ntag = 1000, haara = 0, inode, inode_in, depth = 0, rv, er = 0, lntype;
	short k[MAXXMLDEPTH];
	wchar_t line[1000], *txtbuf[MAXXMLDEPTH];
	wchar_t msg[200] = L"", tag[MAXXMLTAGLEN+1];
	xml_tree tree;
	xml_node nd;

	memset(k, 0, sizeof(k));
	infile = new TextFl(infname, L"rt");
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
					switch (nd.tagid) {
						case TAGRacelist :
							haara = 0;
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er)
								loadRacelist(tree.node+inode_in, inode-inode_in, kisat, lkm);
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
		return(-1);
		}
	delete infile;
	if (depth) {
		er = 1;
		swprintf(msg, L"XML-tiedosto %s päättyi rivillä %d tasolla %d", infname, rv, depth);
		}
	if (er) {
		if (!msg[0]) {
			swprintf(msg, L"Tiedostossa %s virhe rivillä %d tai aikaisemmin", infname, rv);
			}
		writeerror_w(msg, 0);
		}
	tree.nodecount = inode+1;
	return(er);
}
//---------------------------------------------------------------------------

void __fastcall TFormKisaLuett::FormShow(TObject *Sender)
{
	kilpLkm = sizeof(kisaLuett)/sizeof(kisaLuett[0]);
	lueKilpLuett((FormMain->InitDir+UnicodeString(L"\\")+fnameRacelist).c_str(), kisaLuett, &kilpLkm);
	if (kilpLkm == sizeof(kisaLuett)/sizeof(kisaLuett[0]))
		kilpLkm = 0;
	naytaLista();
}
//---------------------------------------------------------------------------

void TFormKisaLuett::naytaLista(void)
{
	wchar_t st[40];

	SG1->RowCount = kilpLkm + 2;
	for (unsigned int r = 1; r < SG1->RowCount; r++) {
		if (r > kilpLkm) {
			for (int c = 0; c < SG1->ColCount; c++)
				SG1->Cells[c][r] = L"";
			}
		else {
			int pvm;

			SG1->Cells[0][r] = kisaLuett[r-1].koodi;
			if ((pvm = kisaLuett[r-1].date) > 41000 && pvm < 45000)
				SG1->Cells[1][r] = stdDateStr(st, kisaLuett[r-1].date);
			else
				SG1->Cells[1][r] = L"";
			SG1->Cells[2][r] = kisaLuett[r-1].nimi;
			SG1->Cells[3][r] = kisaLuett[r-1].polku;
			SG1->Cells[4][r] = kisaLuett[r-1].cfgfile;
			}
		}
}
//---------------------------------------------------------------------------

void TFormKisaLuett::haeLista(void)
{
	int lkm = 0;
	int pvm;

	memset(kisaLuett, 0, sizeof(kisaLuett));
	for (int r = 1; r < SG1->RowCount; r++) {
		if (SG1->Cells[0][r].Length() > 0 &&
			SG1->Cells[1][r].Length() > 0 &&
			(pvm = decodeStdDateStr(SG1->Cells[1][r].c_str())) > 41000 && pvm < 45000 &&
			SG1->Cells[2][r].Length() > 0 &&
			SG1->Cells[3][r].Length() > 0 &&
			SG1->Cells[4][r].Length() > 0) {
			wcsncpy(kisaLuett[lkm].koodi, SG1->Cells[0][r].c_str(), sizeof(kisaLuett[0].koodi)/2-1);
			kisaLuett[lkm].date = pvm;
			wcsncpy(kisaLuett[lkm].nimi, SG1->Cells[2][r].c_str(), sizeof(kisaLuett[0].nimi)/2-1);
			wcsncpy(kisaLuett[lkm].polku, SG1->Cells[3][r].c_str(), sizeof(kisaLuett[0].polku)/2-1);
			wcsncpy(kisaLuett[lkm].cfgfile, SG1->Cells[4][r].c_str(), sizeof(kisaLuett[0].cfgfile)/2-1);
			lkm++;
			}
		}
	kilpLkm = lkm;
	qsortint(kisaLuett, lkm, sizeof(kisaLuett[0]), 0, 1);
}
//---------------------------------------------------------------------------

void __fastcall TFormKisaLuett::Button2Click(TObject *Sender)
{
	SaveDialog1->DefaultExt = L"xml";
	SaveDialog1->Filter = L"XML-tiedostot (*.xml)|*.XML|Kaikki tiedostot (*.*)|*.*";
	SaveDialog1->FileName = FormMain->InitDir+UnicodeString(L"\\")+fnameRacelist;
	if (SaveDialog1->Execute()) {
		haeLista();
		kirjKilpLuett(SaveDialog1->FileName.c_str(), kisaLuett, kilpLkm);
		Close();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKisaLuett::SG1SelectCell(TObject *Sender, int ACol, int ARow,
		  bool &CanSelect)
{
	if (ARow == SG1->RowCount-1)
		SG1->RowCount++;
}
//---------------------------------------------------------------------------

static int LuoKansio(wchar_t *dirpath)
{
	int ret = 0, level = 0;
	wchar_t paths[20][248];
	memset(paths, 0, sizeof(paths));
	wcsncpy(paths[0], dirpath, sizeof(paths[0])/2-1);
	if (paths[0][wcslen(paths[0])-1] == L'\\')
		paths[0][wcslen(paths[0])-1] = 0;
	do {
		if(!CreateDirectory(paths[level], NULL))
			ret = GetLastError();
		else {
			ret = 0;
			break;
			}
		if (ret == ERROR_ALREADY_EXISTS) {
			ret = 0;
			break;
			}
		if (ret == ERROR_PATH_NOT_FOUND) {
			wchar_t *p;
			wcscpy(paths[level+1], paths[level]);
			level++;
			for (p = paths[level]+wcslen(paths[level])-1; p >= paths[level]; p--) {
				if (*p == L'\\') {
					*p = 0;
					break;
					}
				*p = 0;
				}
			if (wcslen(paths[level]) == 0 || paths[level][wcslen(paths[level])-1] == L':')
				break;
			}
		} while(ret == ERROR_PATH_NOT_FOUND);
	while (ret == 0 && level > 0) {
		if(!CreateDirectory(paths[--level], NULL))
			ret = GetLastError();
		else
			ret = 0;
		}
	return(ret);
}
//---------------------------------------------------------------------------

void __fastcall TFormKisaLuett::BtnToteutaClick(TObject *Sender)
{
	wchar_t ch;
	if (CBLuo->Checked) {
		for (int r = 1; r < SG1->RowCount; r++) {
			if (SG1->Cells[3][r].Length() > 0)
				LuoKansio(SG1->Cells[3][r].c_str());
			}
		}
	if (EdtRunko->Text.Length() > 0 &&
		(CBSarjat->Checked || CBData->Checked || CBKonfig->Checked)) {
		if ((ch = EdtRunko->Text.c_str()[EdtRunko->Text.Length()-1]) != L':' &&
			ch != L'\\')
			EdtRunko->Text = EdtRunko->Text+L"\\";
		for (int r = 1; r < SG1->RowCount; r++) {
			if ((ch = SG1->Cells[3][r].c_str()[SG1->Cells[3][r].Length()-1]) != L':' &&
				ch != L'\\')
				SG1->Cells[3][r] = SG1->Cells[3][r]+L"\\";
			if (SG1->Cells[3][r].Length() > 1) {
				if (CBSarjat->Checked)
					CopyFile((EdtRunko->Text+L"KilpSrj.xml").c_str(),
						(SG1->Cells[3][r]+L"KilpSrj.xml").c_str(), FALSE);
				if (CBData->Checked)
					CopyFile((EdtRunko->Text+L"KILP.DAT").c_str(),
						(SG1->Cells[3][r]+L"KILP.DAT").c_str(), FALSE);
				if (CBKonfig->Checked)
					CopyFile((EdtRunko->Text+SG1->Cells[4][r]).c_str(),
						(SG1->Cells[3][r]+SG1->Cells[4][r]).c_str(), FALSE);
				}
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKisaLuett::BtnRunkoClick(TObject *Sender)
{
	OpenDialog1->Filter = L"Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		int l;
		EdtRunko->Text = OpenDialog1->FileName;
		while ((l = EdtRunko->Text.Length()) > 0) {
			if (EdtRunko->Text.SubString(l, 1) != L"\\" &&
				EdtRunko->Text.SubString(l, 1) != L":")
				EdtRunko->Text = EdtRunko->Text.SubString(1, l-1);
			else
				break;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormKisaLuett::Luekilpailuluettelo1Click(TObject *Sender)
{
	OpenDialog1->DefaultExt = L"xml";
	OpenDialog1->Filter = L"XML-tiedostot (*.xml)|*.XML|Kaikki tiedostot (*.*)|*.*";
	OpenDialog1->FileName = FormMain->InitDir+UnicodeString(L"\\")+fnameRacelist;
	if (OpenDialog1->Execute()) {
		kilpLkm = sizeof(kisaLuett)/sizeof(kisaLuett[0]);
		lueKilpLuett(OpenDialog1->FileName.c_str(), kisaLuett, &kilpLkm);
		if (kilpLkm == sizeof(kisaLuett)/sizeof(kisaLuett[0]))
			kilpLkm = 0;
		naytaLista();
		}
}
//---------------------------------------------------------------------------

