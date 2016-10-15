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

#ifndef XMLPARSERDEFINED
#define XMLPARSERDEFINED

#define MAXXMLPARNAMELEN	31
#define MAXXMLDEPTH	20
#define MAXXMLTAGLEN	31
#define XMLTXTBUFLEN 2000

typedef struct {
	wchar_t name[MAXXMLPARNAMELEN+1];
	wchar_t *value;
} nodeparam;

class xml_node {
public:
	short	depth;
	short	nodeno[MAXXMLDEPTH];
	wchar_t	tag[MAXXMLTAGLEN+1];
	short   tagid;
	short   nparam;
	wchar_t *text;
	nodeparam *param;

	xml_node(void);
	~xml_node(void);
	void	nollaa(void);
	void 	operator=(xml_node& node1);
	bool	gettext(wchar_t *buf, int buflen);
	bool	gettext(char *buf, int buflen);
	bool	gettext_int(int *value);
	bool	gettext_double(double *value);
	bool	getparam_text(wchar_t *name, wchar_t *buf, int buflen);
	bool	getparam_text(wchar_t *name, char *buf, int buflen);
	bool	getparam_int(wchar_t *name, int *value);
	bool	getparam_double(wchar_t *name, double *value);
	int 	interpretXmlLine(wchar_t *line, wchar_t *tag);
	int 	putXmlNodeinfo(wchar_t *ln);
};

class xml_tree {
public:
	int		nodecount;
	int		maxnode;
	xml_node *node;

	xml_tree(void);
	~xml_tree(void);
	int		loadtree(wchar_t *filename);
	int		loadbranch(TextFl *infile, int *inode, int depth, int *rv, wchar_t **textbuf, short *k);
	int		writexml(wchar_t *filename);
	int		search_node(short depth, short *k, wchar_t *name);
};

#endif
