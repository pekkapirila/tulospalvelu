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

#include <stdlib.h>
#include <tputil.h>
#include "xmlparser.h"

static wchar_t *Tabs[8] = {L"", L"\t", L"\t\t", L"\t\t\t", L"\t\t\t\t", L"\t\t\t\t\t", L"\t\t\t\t\t\t", L"\t\t\t\t\t\t\t"};

xml_node::xml_node(void)
{
	memset(this, 0, sizeof(xml_node));
}

xml_node::~xml_node(void)
{
	nollaa();
}

void  xml_node::nollaa(void)
{
	if (param) {
		for (int i = 0; i < nparam; i++)
			delete param[i].value;
		delete[] param;
		}
	if (text)
		delete text;
	memset(this, 0, sizeof(xml_node));
}

void xml_node::operator=(xml_node& node1)
{
	this->nollaa();
	depth = node1.depth;
	memcpy(nodeno, node1.nodeno, sizeof(nodeno));
	memcpy(tag, node1.tag, sizeof(tag));
	if (node1.text) {
		text = new wchar_t[wcslen(node1.text)+1];
		wcscpy(text, node1.text);
		}
	if (node1.nparam) {
		nparam = node1.nparam;
		param = new nodeparam[node1.nparam];
		for (int i = 0; i < nparam; i++) {
			wcscpy(param[i].name, node1.param[i].name);
			param[i].value = new wchar_t[wcslen(node1.param[i].value)+1];
			wcscpy(param[i].value, node1.param[i].value);
			}
		}
}

bool xml_node::gettext(wchar_t *buf, int buflen)
{
	wmemset(buf, 0, buflen);
	if (text) {
		wcsncpy(buf, text, buflen-1);
		return(true);
	}
	return(false);
}

bool xml_node::gettext(char *buf, int buflen)
{
	memset(buf, 0, buflen);
	if (text) {
		wcstoansi(buf, text, buflen-1);
		return(true);
	}
	return(false);
}

bool xml_node::gettext_int(int *value)
{
	if (text) {
		*value = _wtoi(text);
		return(true);
	}
	*value = 0;
	return(false);
}

bool xml_node::gettext_double(double *value)
{
	if (text) {
		*value = _wtof(text);
		return(true);
	}
	*value = 0;
	return(false);
}

bool xml_node::getparam_text(wchar_t *name, wchar_t *buf, int buflen)
{
	wmemset(buf, 0, buflen);
	for (int i = 0; i < nparam; i++) {
		if (!wcscmp(name, param[i].name)) {
			wcsncpy(buf, param[i].value, buflen-1);
			return(true);
			}
		}
	return(false);
}

bool xml_node::getparam_text(wchar_t *name, char *buf, int buflen)
{
	memset(buf, 0, buflen);
	for (int i = 0; i < nparam; i++) {
		if (!wcscmp(name, param[i].name)) {
			wcstoansi(buf, param[i].value, buflen-1);
			return(true);
			}
		}
	return(false);
}

bool xml_node::getparam_int(wchar_t *name, int *value)
{
	for (int i = 0; i < nparam; i++) {
		if (!wcscmp(name, param[i].name)) {
			*value = _wtoi(param[i].value);
			return(true);
			}
		}
	*value = 0;
	return(false);
}

bool xml_node::getparam_double(wchar_t *name, double *value)
{
	for (int i = 0; i < nparam; i++) {
		if (!wcscmp(name, param[i].name)) {
			*value = _wtof(param[i].value);
			return(true);
			}
		}
	*value = 0;
	return(false);
}

xml_tree::xml_tree(void)
{
	memset(this, 0, sizeof(xml_tree));
}

xml_tree::~xml_tree(void)
{
	if (node)
		delete[] node;
	node = 0;
}

int xml_node::putXmlNodeinfo(wchar_t *ln)
{
	wchar_t *p, *pend, *p1, buf[200];
	int npar = 0;

	this->nollaa();
	pend = ln + wcslen(ln);
	buf[199] = 0;
	for (p = ln; *p; p++)
		if (*p == L'=')
			npar++;
	p = wcstok(ln, L" \t");
	if (p) {
		wcsncpy(tag, p, MAXXMLTAGLEN);
		nparam = npar;
		param = new nodeparam[npar];
		for (int i = 0; i < npar; i++) {
			for (p = p+wcslen(p)+1; p < pend && *p <= L' ' ; p++) ;
			if (p >= pend)
				break;
			for (p1 = p; p1 < pend && *p1 && *p1 != L'='; p1++) ;
			if (*p1 == L'=') {
				*p1 = 0;
				wcsncpy(param[i].name, p, MAXXMLPARNAMELEN);
				param[i].name[MAXXMLPARNAMELEN] = 0;
				if (*++p1 == L'"') {
					p1++;
					p = wcsstr(p1, L"\"");
					}
				else if (*p1 == L'\'') {
					p1++;
					p = wcsstr(p1, L"'");
					}
				else {
					p = wcsstr(p1, L" ");
					}
				if (p) {
					*p = 0;
					}
				else
					p = pend;
				param[i].value = new wchar_t[wcslen(p1)+1];
				wcscpy(param[i].value, p1);
				}
			}
		}
	return(0);
}

int xml_node::interpretXmlLine(wchar_t *linein, wchar_t *ptag)
{
	wchar_t *p1, *p2, *p3;
	int type = 0;
	static wchar_t line[2000];

	this->nollaa();
	memset(tag, 0, 2*(MAXXMLTAGLEN+1));

	if (!linein || wcslen(linein) == 0)
		return(-1);
	if (wcslen(line) + wcslen(linein) < sizeof(line)/2-1)
		wcscat(line, linein);
	else {
		memset(line, 0, sizeof(line));
		return(-1);
		}

	p1 = wcsstr(line, L"<");
	if (p1 && p1[1] == L'?') {       // <? -rivi‰ ei k‰sitell‰
		memset(line, 0, sizeof(line));
		return(-2);
		}
	while (p1 && p1[1] != L'/') {   // etsit‰‰n avaavan tagin alkua (<+kirjain)
		if (p1[1] < L'A' || p1[1] > L'z' || (p1[1] > L'Z' && p1[1] < L'_'))
			p1 = wcsstr(p1+1, L"<");
		else
			break;
		}
	if (p1) {                        // etsit‰‰n avaavan tagin loppu->
		for (p2 = p1+1; *p2 && *p2 != L'>'; p2++) ;
		if (*p2 == 0) {
			return(-5);
//			p1 = 0;
//			p2 = 0;
			}
		}
	if (p1 && p1[1] != L'/') {          //  K‰sitell‰‰n avaava tag
		type = 1;
		*p2 = 0;
		if (p2[-1] == L'/') {
			p2[-1] = 0;
			type |= 4;
			}
		putXmlNodeinfo(p1+1);            // haetaan tag ja parametrit solmuun
		}
	else
		p2 = line-1;
	// Etsit‰‰n sulkevaa tagia

	if ((p1 = wcsstr(p2+1, L"<")) != NULL && p1[1] == L'/') {
		p3 = wcstok(p1+2, L" \t>");
		wcsncpy(ptag, p3, MAXXMLTAGLEN+1);
		if (type == 0) {
			memset(line, 0, sizeof(line));
			return(8);
			}
		if (wcscmp(ptag, this->tag)) {     // varmistetaan, ett‰ tag on oikea
			memset(line, 0, sizeof(line));
			return(-3);
			}
		if (type == 1 && p1 > p2+1) {
			p2++;
			*p1 = 0;
			elimwbl2(p2);
			wmemmove(linein, p2, wcslen(p2)+1);
			}
		else
			linein[0] = 0;
		type |= 2;
		}
	else if (type == 1) {
		elimwbl2(++p2);
		wmemmove(linein, p2, wcslen(p2)+1);
		}
	memset(line, 0, sizeof(line));
	return(type);
}

int	xml_tree::loadbranch(TextFl *infile, int *inode, int depth, int *rv, wchar_t **txtbuf, short *k)
{
	int er = 0, lntype, inode_in;
	wchar_t line[1000];
	wchar_t msg[200], tag[MAXXMLTAGLEN+1];
	xml_node nd;

	inode_in = *inode;
	for (int i = depth+1; i < MAXXMLDEPTH; i++)
		k[i] = 0;
	while (!infile->Feof()) {
	   if (infile->ReadLine(line, 999) == NULL)
			break;
		(*rv)++;
		elimwbl2(line);
		if (line[0] == L'<') {
			while (wcsstr(line, L">") == NULL && wcslen(line) < sizeof(line)/2 - 20) {
				wchar_t *p;
				wcscat(line, L" ");
				p = line+wcslen(line);
				if (infile->ReadLine(p, sizeof(line)/2-wcslen(line)-1) == NULL)
					break;
				(*rv)++;
				elimwbl2(p);
				}
			}
		if (*inode >= maxnode-1) {
			er = 1;
			swprintf(msg, L"XML-tiedoston tulkinta ei onnnistu. Varattu puskuritila (%d solmua) ei riit‰", maxnode);

			break;
			}
		lntype = nd.interpretXmlLine(line, tag);
		if (lntype == -5)
			continue;
		if (lntype == -3) {
			er = 1;
			swprintf(msg, L"XML-tiedoston tulkinta ei onnnistu. Rivi %d, avaava tag: <%.30s>, sulkeva poikkeaa",
				*rv, node[*inode].tag);
			break;
			}
		if (lntype < 0)     					// <? -rivit ja tyhj‰t rivit ohitetaan
			continue;
		if (lntype > 0 && (lntype & 1)) {
			for (int i = 0; i <= depth; i++)
				nd.nodeno[i] = k[i];           // merkit‰‰n eri tasojen j‰rjestysnumerot solmuun
			nd.depth = depth;                  // merkit‰‰n taso
			node[++*inode] = nd;                // siirret‰‰n tiedot solmupuuhun
			if (lntype & 4) {
				k[depth]++;                    // kasvatetaan tasokohtaista j‰rjestysnumeroa tyhj‰n
				continue;                      // tagin tapauksessa ja siirryt‰‰n seur. riville
				}
			else {
				txtbuf[depth][0] = 0;          // nollataan tekstimuisti k‰sitelt‰v‰ll‰ tasolla
				if (lntype & 2) {
					if (*line) {
						node[*inode].text = new wchar_t[wcslen(line)+1];
						wcscpy(node[*inode].text, line);
						}
					k[depth]++;                            // kasvatetaan tasokohtaista j‰rjestysnumroa
					continue;
					}
				// T‰h‰n tullaan, kun tag j‰‰ auki, seuraava tag on svemm‰ll‰ tasolla
				depth++;
				k[depth] = 0;           // seuraavan tason j‰rjestysnumero alkaa nollasta
				}
			}
		else if (lntype == 8) {                  // sulkeva tag uudella rivill‰
			int jnode;
			depth--;
			k[depth]++;                 // kasvatetaan tason laskuria sulkev‰n tagin tasolla
			// Etsit‰‰n vastaava avaava tag aiemmasta ketjusta
			for (jnode = *inode; jnode > inode_in && node[jnode].depth > depth; jnode--) ;
			if (wcscmp(tag, node[jnode].tag)) {      // Varmistetaan, ett‰ tagit ovat pari
				er = 1;
				swprintf(msg, L"XML-tiedoston tulkinta ei onnnistu. Rivi %d, Tag: '%.30s', odotettu: '%.30s'",
					*rv, tag, node[jnode].tag);
				break;
				}
			if (wcslen(txtbuf[depth])) {    // Tallennetaan tason mahdollinen teksti solmuun
				node[jnode].text = new wchar_t[wcslen(txtbuf[depth])+1];
				wcscpy(node[jnode].text, txtbuf[depth]);
				}
			if (jnode == inode_in) {
				break;
				}
			continue;
			}
		else if (lntype == 0) {							// teksti‰
			if (wcslen(line) > 0) {     // siirret‰‰n omalla rivill‰‰n oleva teksti tekstimuistiin lis‰ten
				int dpth = depth - 1;   // eteen v‰lilyˆnti, jos aiempaakin teksti‰
				if (dpth < 0)
					dpth = 0;
				if (txtbuf[dpth][0] && XMLTXTBUFLEN-wcslen(txtbuf[dpth]) > 1)
					wcscat(txtbuf[dpth], L" ");
				wcsncpy(txtbuf[dpth]+wcslen(txtbuf[dpth]), line, XMLTXTBUFLEN-wcslen(txtbuf[dpth]));
				}
			}
		}
	nodecount = *inode+1;
	return(er);
}

int	xml_tree::loadtree(wchar_t *filename)
{
	TextFl *infile;
	int ntag = 0, inode, depth = 0, rv, er = 0;
	short k[MAXXMLDEPTH];
	wchar_t line[1000], *txtbuf[MAXXMLDEPTH];
	wchar_t msg[200], tag[MAXXMLTAGLEN+1];
	xml_node nd;

	memset(k, 0, sizeof(k));
	infile = new TextFl(filename, L"rt");
	if (infile->IsOpen()) {
		while (!infile->Feof()) {
		   if (infile->ReadLine(line, 999) == NULL)
				break;
		   if (wcsstr(line, L"</"))
				ntag++;
		   if (wcsstr(line, L"/>"))
				ntag++;
		   }
		if (ntag) {
			for (int i = 0; i < MAXXMLDEPTH; i++) {
				txtbuf[i] = new wchar_t[XMLTXTBUFLEN+1];
				memset(txtbuf[i], 0, 2*(XMLTXTBUFLEN+1));
				}
			ntag += 10;
			node = new xml_node[ntag];
			maxnode = ntag;
			nodecount = 0;
			infile->Rewind();
			inode = 0;
			rv = 0;
			while (!infile->Feof()) {
				if (infile->ReadLine(line, 999) == NULL)
					break;
				rv++;
				elimwbl2(line);
				if (nd.interpretXmlLine(line, tag) == 1) {
					node[0] = nd;
					k[depth++]++;
					break;
					}
				}
			er = loadbranch(infile, &inode, depth--, &rv, txtbuf, k);
			for (int i=0; i < MAXXMLDEPTH; i++)
				delete[] txtbuf[i];
			}      // ntag > 0
		}  		  // infile->Open()
	delete infile;
	if (depth) {
		er = 1;
		swprintf(msg, L"XML-tiedosto p‰‰ttyi rivill‰ %d tasolla %d", rv, depth);
		}
	if (er)
		writeerror_w(msg, 0);
	nodecount = inode+1;
	return(er);
}

int	xml_tree::search_node(short depth, short *k, wchar_t *name)
{
	int n = 0;
	if (depth) {
		for (int i = 1; i <= depth; i++) {
			while (n < nodecount && (node[n].depth < i || node[n].nodeno[i] < k[i]))
				n++;
			}
		for ( ; n < nodecount; n++) {
			for (int i = 1; i <= depth; i++) {
				if ((node[n].depth < depth || node[n].nodeno[i] > k[i]))
				   return(-1);
				}
			if (node[n].depth == depth && !wcscmp(name, node[n].tag))
				return(n);
			}
		}
	else {
		for (n = 0 ; n < nodecount; n++) {
			if (!wcscmp(name, node[n].tag))
				return(n);
			}
		}
	return(-1);
}

int	xml_tree::writexml(wchar_t *filename)
{
	TextFl *outfile;
	int td, inode, er = 0;
	wchar_t line[2000], tagbuf[MAXXMLDEPTH][MAXXMLTAGLEN+1];

	if (nodecount == 0)
		return(0);
	outfile = new TextFl(filename, L"wt");
	if (outfile->IsOpen()) {
		outfile->WriteLine(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
		line[0] = 0;
		for (inode = 0; inode < nodecount-1; inode++) {
			td = node[inode].depth;
			if (td > 7)
				td = 7;
			swprintf(line, L"%s<%s", Tabs[td], node[inode].tag);
			for (int i = 0; i < node[inode].nparam; i++) {
				swprintf(line + wcslen(line), L" %s=\"%s\"",
					node[inode].param[i].name, node[inode].param[i].value);
				}
			wcscat(line, L">");
			if (node[inode+1].depth > node[inode].depth) {
				wcscpy(tagbuf[node[inode].depth], node[inode].tag);
				wcscat(line, L"\n");
				if (node[inode].text) {
					outfile->WriteLine(line);
					swprintf(line, L"%s\t%s\n", Tabs[td], node[inode].text);
					}
				}
			else {
				if (node[inode].text)
					wcscat(line, node[inode].text);
				swprintf(line+wcslen(line), L"</%s>\n", node[inode].tag);
				}
			outfile->WriteLine(line);
			for (int dpth = node[inode].depth-1; dpth >= node[inode+1].depth; dpth--) {
				td = dpth;
				if (td > 7)
					td = 7;
				swprintf(line, L"%s</%s>\n", Tabs[td], tagbuf[dpth]);
				tagbuf[dpth][0] = 0;
				outfile->WriteLine(line);
				}
			line[0] = 0;
			}
		for (int dpth = node[inode].depth-1; dpth >= 0; dpth--) {
			td = dpth;
			if (td > 7)
				td = 7;
			swprintf(line, L"%s</%s>\n", Tabs[td], tagbuf[dpth]);
			tagbuf[dpth][0] = 0;
			outfile->WriteLine(line);
			}
		}
	delete outfile;
	return(er);
}