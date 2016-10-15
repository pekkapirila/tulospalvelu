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

#ifndef TPUTILDEFINED
#define TPUTILDEFINED

#ifdef WIN32
#include <windows.h>
#endif
#include <wchar.h>
#include <stdio.h>
#include <tptype.h>
#include <bstrings.h>
#if defined(__BORLANDC__)
#pragma option -K -a1
#endif
#ifdef _CONSOLE
//#include <bvideo.h>
#include <bscreens.h>
#endif
#include "wincom.h"
#define SEL_FORE 15
#define SEL_BACK 0
#define FORE_O 7
#define BKG_O 0
#define FORE_T 7
#define BKG_T 0

#define clrbuf(l)           memset(copybuf,' ',(l))
#define vidspaika(y,x,t,t0) \
   viwrrect((y),(x),(y),(x)+7,(const char *) aikatostr((t),(t0)),7,0,0)
#define vidspaika_l(y,x,t,t0,l) \
   viwrrect((y),(x),(y),(x)+(l)-1,(const char *) aikatostr_l((t),(t0)),7,0,0)
#define vidspaika_h(y,x,t,t0,l) \
   viwrrect((y),(x),(y),(x)+(l)-1,(const char *) aikatostr_h((t),(t0)),7,0,0)
#define vidspaika_t(y,x,t,t0,l) \
   viwrrect((y),(x),(y),(x)+(l)-1,(const char *) aikatostr_t((t),(t0)),7,0,0)
#define stpcpy(de,so)  strcpy((de),(so))+strlen((so))
#define wcpcpy(de,so)  wcscpy((de),(so))+wcslen((so))
#define chinlist(c, l) (stschind((c), (l)) >= 0)
#if defined(__BORLANDC__)
#define _itoa(x,y,z)       itoa((x),(y),(z))
#define _swab(s1,s2,n)     swab((s1),(s2),(n))
//#define _lseek(fl,off,fr)  lseek((fl),(off),(fr))
#define _filelength(fl)    filelength((fl))
#define _chsize(f,l)       chsize((f),(l))
#define _swab(x,y,n)       swab((x),(y),(n))
#else
#define farmalloc(x) malloc((x))
#define farcalloc(x, y) calloc((x), (y))
#define farfree(x) free((x))
#endif

#pragma pack(push,1)
//#pragma pack(1)
typedef struct {
   INT oper;
   INT laji;
   void * tieto;
   char * ots;
   INT y;
   INT x;
   INT l;
   INT bias;
	INT xots;
   char * term;
   int (*prefunc)(void *, void *, void *);
   int (*postfunc)(void *, void *, void *);
   void *kilp;
   } fld_tp;
#pragma pack(pop)

typedef struct {
   char * ix;
   int keyl;
	int recl;
   int nrec;
   int pos;
	char *(*keyf)(void);
   } dbindex;

typedef struct {
   char * ix;
   int keyl;
	int recl;
   int nrec;
   int pos;
   int flags;
	char *(*keyf)(void *, char *, int, int);
   } kilpindex;

typedef char *farblkptr;

typedef struct {
	int fontfamily;
	int pitch;
	int size;
	int bold;
	int italic;
	int numwidth;
	int line;
	int chwidth;
	int lnheight;
	void *hfont;
	} GDIfontTp;

class TextFl {
	FILE 	*File;
	wchar_t	TextType;		// Oem, Ansi, Wide, Vide, utf-8
	wchar_t mode[20];
	int		Skip;
	bool	ReadFl;
	bool	WriteFl;
	bool	Eof;
	void	BOM(void);
public:
	wchar_t	*FileName;
	TextFl(wchar_t *FName, wchar_t *Mode, wchar_t TextType = 0);
	~TextFl(void);
	int		Open(wchar_t *Mode = NULL);
	void	Close(void);
	bool	IsOpen(void);
	bool	Feof();
	wchar_t	*ReadLine(wchar_t *Buf, int len);
	wchar_t	ReadChar(void);
	int		WriteLine(wchar_t *Buf);
	int		WriteChar(wchar_t Char);
	char	*ReadLine8(char *Buf, int len);
	int		WriteLine8(char *Buf);
	char	*ReadLineA(char *Buf, int len);
	int		WriteLineA(char *Buf);
	int		ReadBytes(char *buf, int size, int count);
	int		WriteBytes(char *buf, int size, int count);
	int		Length(void);
	int		Rewind(void);
	void	Flush(void);
	void 	put_wxml_s(wchar_t *tag, wchar_t *value, int level, wchar_t *parstr = 0);
	void 	put_wxml_d(wchar_t *tag, INT32 value, int level, wchar_t *parstr = 0);
	void 	put_wxml_f(wchar_t *tag, double value, int prec, int level, wchar_t *parstr = 0);
	void 	put_wxml_time(wchar_t *tag, int Date, INT32 value, int t0, int tarkkuus, int len, // Date = 0: ei päiväystä
				wchar_t dcep, int level, wchar_t *parstr = 0);   // len < 0: poista etunollat
	void 	put_wtag(wchar_t *tag, int level);
	void 	put_wtagparams(wchar_t *tag, wchar_t *params, bool empty, int level);
	void 	put_wantitag(wchar_t *tag, int level);
	};

typedef struct {
	GDIfontTp Currentfont;
	int topmargin;
	int bottommargin;
	int leftmargin;
	int rightmargin;
	int extramargin;
	int normalWeight;
	int boldWeight;
	wchar_t fontName[6][30];
   } GDIparamtp;

typedef struct {
   int prno;
   char *trlate;
   union {
	  TextFl *Fl;
	  struct {
		 FILE *file;
		 wchar_t *fname;
		 int immed;
		 int handle;
		 } f;
	  struct {
		 farblkptr *blk;
		 unsigned nbl;
		 unsigned maxbl;
		 unsigned kbla;
		 unsigned kbly;
		 unsigned ibla;
		 unsigned ibly;
		 int wait;
		 } p;
		struct {
		 HANDLE hPrinter;
		 wchar_t *fname;
		 int DocOpen;
		 int PageOpen;
		 int xPage;
		 int yPage;
		 int xppi;
		 int yppi;
		 int vertpos;
		 int top;
		 GDIparamtp GDIparam;
		 } wp;
	  } u;
   } PRFILE;

double StrToDouble(wchar_t *str);
wchar_t towupper2(const wchar_t ch);
wchar_t towlower2(const wchar_t ch);
wchar_t *wconcat(const wchar_t *s1, const wchar_t *s2, const wchar_t *s3 = NULL);
int haeStrIx(const wchar_t *st, wchar_t *stArr[], int szArr);
int WinVersion(void);
void initmstimer(void);
long mstimer(void);
#ifndef _CONSOLE
int ProcessSpecialKey(void *Buf);
int CtrlHandler(unsigned long fdwCtrlType);
#else
int ProcessSpecialKey(INPUT_RECORD *Buf);
BOOL CtrlHandler(DWORD fdwCtrlType);
#endif
int selectfontGDI(PRFILE *pf, GDIfontTp *fnt);
int putfldGDI(char *str, int pos, int oik, int vpos, PRFILE *pf);
int putwfldGDI(wchar_t *str, int pos, int oik, int vpos, PRFILE *pf);
int startpageGDI(PRFILE *pf);
int startdocGDI(PRFILE *pf);
int endpageGDI(PRFILE *pf);
int enddocGDI(PRFILE *pf);
int abortdocGDI(PRFILE *pf);
void getParamsGDI(PRFILE *pf, GDIparamtp *GDIparam);
void setParamsGDI(PRFILE *pf, GDIparamtp *GDIparam);
int enumfontsGDI(PRFILE *pf, wchar_t *facename, void *fonts);
int textlengthGDI(PRFILE *pf, wchar_t *st);
char *savescr(int r0, int c0, int r1, int c1);
void restorescr(int r0, int c0, int r1, int c1, char *buf);
INT32 timeofday(void);
int  initcon(wchar_t *title);
wchar_t *wstpcvt(wchar_t *psource, int conv);
long atold(char *str);
long watold(wchar_t *str);
int memnullcmp(char *buf, int len);
void lswab(long *l);
char *itoa1(char *s, int n, int l);
wchar_t *itow1(wchar_t *s, int n, int l);
void multiswab(char *src, char *dest, int n);
void qsortint(void *base, INT nelem, INT width, INT pos, INT fl32);
INT bsrchint(INT32 x, void *base, INT nelem, INT width, INT pos, INT fl32);
void delchar(char *s, int pos, int n);
void inschar(char ch, char *s, int pos);
void delwchar(wchar_t *s, int pos, int n);
void inswchar(wchar_t ch, wchar_t *s, int pos);
void draw_hline(int row, int col, int len);
void draw_vline(int row, int col, int len);
void draw_hline2(int row, int col, int len);
void draw_vline2(int row, int col, int len);
void draw_grchar(int row, int col, int ch);
void draw_grchar2(int row, int col, int ch);
void vidint(int y, int x, int l, long i);
int testunicode(FILE *tfile, char *flnm);
char *scanconv(char *line0);
char oemtoansich(char ch);
char *oemtoansi(char *line0, int ppiste);
char ansitooemch(char ch);
char *ansitooem(char *line0);
wchar_t *oemtowcs(wchar_t *wline, char *line, int len, int ppiste);
wchar_t ansitowchar(char ch);
wchar_t *ansitowcs(wchar_t *wline, char *line, int len);
wchar_t oemtowchar(char ch);
char wchartoansi(wchar_t ch);
char *wcstoansi(char *line, wchar_t *wline, int len);
char wcrtooemch(wchar_t ch);
char *wcstooem(char *line, wchar_t *wline, int len);
#ifdef _CONSOLE
char inputch(char *ss, int x, int y, const char *term, char *tc, int numfl);
char *inputstr(char *ss, unsigned l, int x, int y, const char *term, char *tc,
																		 int numfl);
wchar_t *inputwstr(wchar_t *ws, unsigned l, int x, int y, const wchar_t *term, wchar_t *tc,
																		 int numfl);
char *inputstr_pr(char *prompt, char *ss, int l, int x, int y,
	const char *term, char *tc, int numfl);
char *inputstr_oik(char *ss, unsigned l,int x,int y, const char *term, char *tc,
	int numfl);
char *pageedit(char *s, unsigned l, int y, int x, int y1, int x1,
	const char *term, char *tc);
wchar_t *wpageedit(wchar_t *s, unsigned l, int y, int x, int y1, int x1,
	const wchar_t *term, wchar_t *tc);
#endif
void dsp_fld(fld_tp *fld);
void dsp_fld_set(fld_tp *flds);
void khtostr(char kh, char *str);
void khtowstr(wchar_t kh, wchar_t *str);
void clreol(void);
void gotoxy(int x, int y);
void getxy(int *x, int *y);
int wherex(void);
int wherey(void);
char readkbd(char  *ch2, int wait, int spoolfl);
int  kbdrdy(void);
void elimbl1(char *str);
void elimwbl1(wchar_t *str);
void elimbl2(char *str);
void elimwbl2(wchar_t *str);
void elimz(char *aikastr);
void elimzb1(char *aikastr);
void elimwz(wchar_t *aikastr);
void elimwzb1(wchar_t *aikastr);
void poistanollat(char *str);
void wpoistanollat(wchar_t *st);
char *kello(void);
wchar_t *wkello(void);
char *paiva(void);
wchar_t *wpaiva(void);
unsigned lue_v_pv(void);
void as_v_pv(unsigned v_pv);
void inctime(int edista);
void erbeep(void);
void attnbeep(void);
void tick(void);
char *conststr(char *s, char ch, unsigned n);
INT32 timeticks(void);
long t_time_l(long tics, int t0);
long t_time(long tics, int t0);
char *aikatostr(INT32 aika, int tt0);
char *aikatostr_l(INT32 aika, int tt0);
char *aikatostr_l2(INT32 aika, long tt0);
char *aikatostr_h(INT32 aika, int tt0);
char *aikatostr_t(INT32 aika, int tt0);
char *aikatostr_col(char *as, INT32 tl, int t0);
char *aikatostr_s(char *as, INT32 aika, int tt0);
char *aikatostr_ls(char *as, INT32 aika, int tt0);
char *aikatostr_l2s(char *as, INT32 aika, long tt0);
char *aikatostr_hs(char *as, INT32 aika, int tt0);
char *aikatostr_ts(char *as, INT32 aika, int tt0);
char *aikatostr_cols(char *as, INT32 tl, int t0);
wchar_t *aikatowstr_s(wchar_t *as, INT32 aika, int tt0);
wchar_t *aikatowstr_ls(wchar_t *as, INT32 aika, int tt0);
wchar_t *aikatowstr_l2s(wchar_t *as, INT32 aika, long tt0);
wchar_t *aikatowstr_hs(wchar_t *as, INT32 aika, int tt0);
wchar_t *aikatowstr_ts(wchar_t *as, INT32 aika, int tt0);
wchar_t *aikatowstr_cols(wchar_t *as, INT32 tl, int t0);
long strtoaika(char *straika,int tt0);
long strtoaika_l(char *straika,int tt0);
long strtoaika_l2(char *straika,long tt0);
long strtoaika_h(char *straika,int tt0);
long strtoaika_t(char *straika,int tt0);
long wstrtoaika(wchar_t *straika,int tt0);
long wstrtoaika_l(wchar_t *straika,int tt0);
long wstrtoaika_l2(wchar_t *straika,long tt0);
long wstrtoaika_h(wchar_t *straika,int tt0);
long wstrtoaika_t(wchar_t *straika,int tt0);
char upcasesc(char ch);
char *upcasestr(char *s);
wchar_t upcasewsc(wchar_t ch);
wchar_t *upcasewstr(wchar_t *s);
char locasesc(char ch);
wchar_t locasewsc(wchar_t ch);
char *locasename(char *s);
wchar_t *locasewname(wchar_t *s);
char *locasestr(char *s);
wchar_t *locasewstr(wchar_t *s);
void vaihdanm(char *sti, char *sto);
void vaihdawnm(wchar_t *sti, wchar_t *sto);
void esnimet(char *sti, char *etu, char *suku);
void esnimet2(char *sti, char *etu, char *suku);
void wesnimet(wchar_t *sti, wchar_t *etu, wchar_t *suku);
int tarkno(int kno);
int tarknost(char *knost);
int tarkno_os(int kno, int os);
#ifdef _CONSOLE
void inputint(short *i, int l,int x,int y, char *term, char *tc);
void inputintw(short *i, int l,int x,int y, wchar_t *term, wchar_t *tc);
void inputint_oik(int *i, int l,int x,int y, char *term, char *tc);
void inputlong(INT32 *i, int l,int x,int y, char *term, char *tc);
void inputlongw(INT32 *i, int l,int x,int y, wchar_t *term, wchar_t *tc);
void inputkno(int *kno, int x, int y, char *term, char *tc);
void input_no(int *kno, int x, int y, char *term, char *tc);
void inputknow(int *kno, int x, int y, wchar_t *term, wchar_t *tc);
void input_now(int *kno, int x, int y, wchar_t *term, wchar_t *tc);
void inputkno_os(int *kno, int *os, int x, int y, char *term, char *tc);
void inputkno_osw(int *kno, int *os, int x, int y, wchar_t *term, wchar_t *tc);
void inputkno_os_t(int *kno, int *os, int x, int y, char *term, char *tc);
void inputkno_os_tw(int *kno, int *os, int x, int y, wchar_t *term, wchar_t *tc);
void inputkno_os2(int *kno, int *os, int x, int y, char *term, char *tc);
void inputkno_os2w(int *kno, int *os, int x, int y, wchar_t *term, wchar_t *tc);
void inputaika(INT16 *t,int t0, int l, int x, int y, char *term, char *tc);
void inputaika_l(INT32 *t,int t0, int l, int x, int y, char *term, char *tc);
void inputaika_l2(INT32 *t,INT32 t0, int l, int x, int y, char *term, char *tc);
void inputaika_h(INT32 *t,int t0, int l, int x, int y, char *term, char *tc);
void inputaika_t(INT32 *t,int t0, int l, int x, int y, char *term, char *tc);
void inputaika_w(INT16 *t,int t0, int l, int x, int y, wchar_t *term, wchar_t *tc);
void inputaika_lw(INT32 *t,int t0, int l, int x, int y, wchar_t *term, wchar_t *tc);
void inputaika_l2w(INT32 *t,INT32 t0, int l, int x, int y, wchar_t *term, wchar_t *tc);
void inputaika_hw(INT32 *t,int t0, int l, int x, int y, wchar_t *term, wchar_t *tc);
void inputaika_tw(INT32 *t,int t0, int l, int x, int y, wchar_t *term, wchar_t *tc);
#endif
void selectopt(char *prompt, char *term, char *tc);
void selectxy(INT y, INT x, char *prompt, char *term, char *tc);
void clearframe(void);
char *aakjarjstr(char *knimi);
char *aakjarjstr2(char *knimi);
wchar_t *aakjarjwstr(wchar_t *knimi);
wchar_t *aakjarjwstr2(wchar_t *knimi);
char *keyfromname(char *key, char *nimi, int keylen, int flags);
char *keyfromwname(char *key, wchar_t *wnimi, int keylen, int flags);
int chint(char ch);
int wchint(wchar_t ch);
unsigned int nimiint(char *nimi);
unsigned int wnimiint(wchar_t *wnimi);
UINT32 nimilong(char *nimi);
UINT32 wnimilong(wchar_t *wnimi);
int ioerr(char *fname, int oper, long iores);
int iowerr(wchar_t *fname, int oper, long iores);
void perr(char *szFileName, int line, char *szApiName, unsigned long dwError);
void wperr(wchar_t *szFileName, int line, wchar_t *szApiName, UINT32 dwError);
void writeerror(char *msg, int kesto);
void writeerror_w(wchar_t *msg, int kesto);
void putstr(char *buf, char *s, int x, int y, int w, int rf, int df);
void putint(char *buf,int i, int x, int y, int w, int rd, int df);
void putintright(int i, char *p);
void putintrightw(int i, wchar_t *p);
int sendline(char *line, PRFILE *pf);
int sendwline(wchar_t *linein, PRFILE *pf);
int sendchars(char *line, int len, PRFILE *pf);
int sendwchars(wchar_t *wline, int len, PRFILE *pf);
// GDIkirjoitin: -1: TextFl, 0: muu kirjoitin tai tiedosto, 1: GDIkirjoitin
// wait: ei käytössä
// append: avaa tiedoston append-moodiin (myös TextFl)
// trlate: merkkien korvaus taulukolla trlate
// immed: tiedostoonkirjoitus viedään heti levylle (ei Textfl)
PRFILE *openprfile(wchar_t *prtfname, int GDIkirjoitin, int wait, int append,
	char *trlate, int immediate);
int prstatus(PRFILE *pf);
void wrprbuf(PRFILE *pf, char *fname);
void closeprfile(PRFILE *pf);
wchar_t *prfname(PRFILE *pf);
int printbuf(PRFILE *prtf, char *buf, int nrivi);
void motime(void);
void transl(char *in, char *out, char *ch);
void wtransl(wchar_t *in, wchar_t *out, wchar_t *ch);
void put_xml_s(FILE *vafile, char *tag, char *value);
void put_xml_d(FILE *vafile, char *tag, INT32 value);
void put_tag(FILE *vafile, char *tag);
void put_antitag(FILE *vafile, char *tag);
void set_xml_s(char *vastr, char *tag, char *value);
void set_xml_d(char *vastr, char *tag, INT32 value);
void set_tag(char *vastr, char *tag);
void set_antitag(char *vastr, char *tag);
void set_wxml_s(wchar_t *vastr, wchar_t *tag, wchar_t *value);
void set_wxml_d(wchar_t *vastr, wchar_t *tag, INT32 value);
void set_wtag(wchar_t *vastr, wchar_t *tag);
void set_wantitag(wchar_t *vastr, wchar_t *tag);
char *w_aakjarjstr(char *jrjstr, wchar_t *nimi, int len, int wasv, int aatelifl);

#ifndef DEF_BSCREENS
void sccurset(int y, int x);
void scclrmsg(int, int, int);
int sccurst(int *prow, int *pcol, int *phigh, int *plow);
int scbox(int u_row, int u_col, int l_row, int l_col, int boxtype, char boxchar, int attrib);
#endif
#ifndef DEF_BVIDEO
int vidspmsg(int row, int col, int fg, int bg, const char *txt);
int viwrrect(int r1, int c1, int r2, int c2, const char *txt, int fg, int bg, int opt);
int vidspwmsg(int row, int col, int fg, int bg, const wchar_t *txt);
int viwrrectw(int r1, int c1, int r2, int c2, const wchar_t *txt, int fg, int bg, int opt);
int virdrect(int u_row, int u_col, int l_row, int l_col,
	char *buf, int option);
#endif
#if !defined(_CONSOLE) && !defined(CONSOLELIB)
void clrtxt(int row, int col1, int col2);
void clrln(int row);
#endif
char * bsrch(char *key, char *base, INT *nelem, INT keylen, INT width);
void quicksort(char *base, int nelem, int keylen, int width);
int addix(kilpindex *idx, char *key, int d);
int delix(kilpindex *idx, char *key, int d);
int searchix(kilpindex *idx, char *key, int *d);
int nextix(kilpindex *idx, char *key, int *d);
int previx(kilpindex *idx, char *key, int *d);
int IntInList(const int val, const wchar_t * const lst);
int DateNo(int y, int m, int d);
int yDateNo(int dd);
int mDateNo(int dd);
int dDateNo(int dd);
wchar_t *stDateNo(wchar_t *st, int dd);
int DateNoFromSt(wchar_t *st);
long msdaytime(void);
wchar_t *ISOdatetime(int des, bool zone);
int MbsToWcs(wchar_t *wst, char *st, int len);
int WcsToMbs(char *st, wchar_t *wst, int len);
int wcscmpU(const wchar_t *s1, const wchar_t *s2);
int wmemcmpU(const wchar_t *s1, const wchar_t *s2, int len);
int strcmpU(const char *s1, const char *s2);
int memcmpU(const char *s1, const char *s2, int len);
char *trimline(char *line);
wchar_t *wtrimline(wchar_t *line);
int kbready_w(wchar_t *ch, int *key);
int kbdrdy_w(void);
wchar_t readkbd_w(wchar_t *ch2, int wait);
void wselectopt(wchar_t *prompt, wchar_t *term, wchar_t *tc);
void wselectxy(INT y, INT x, wchar_t *prompt, wchar_t *term, wchar_t *tc);
wchar_t *inputwstr2(wchar_t *s, unsigned l, int x, int y, const wchar_t *term, wchar_t *tc, int numfl);
int getfields(wchar_t *line, wchar_t *fields[], int maxfields, wchar_t *sep, wchar_t *quotes, bool singlesep, bool deleteold);
void deletefields(wchar_t *fields[], int maxfields);
void wput_xml_s(TextFl *fl, wchar_t *tag, wchar_t *value, int level);
void wput_xml_d(TextFl *fl, wchar_t *tag, INT32 value, int level);
void wput_xml_f(TextFl *fl, wchar_t *tag, double value, int prec, int level);
void wput_tag(TextFl *fl, wchar_t *tag, int level);
void wput_tagparams(TextFl *fl, wchar_t *tag, wchar_t *params, bool empty, int level);
void wput_antitag(TextFl *fl, wchar_t *tag, int level);
void wput_xml_params_s(TextFl *fl, wchar_t *tag, wchar_t *params, wchar_t *value, int level);
void wput_xml_params_d(TextFl *fl, wchar_t *tag, wchar_t *params, int value, int level);
void wput_xml_params_f(TextFl *fl, wchar_t *tag, wchar_t *params, double value, int prec, int level);
void wset_xml_s(wchar_t *vastr, wchar_t *tag, wchar_t *value, int level, wchar_t *parstr);
void wset_xml_d(wchar_t *vastr, wchar_t *tag, INT32 value, int level, wchar_t *parstr);
void wset_xml_f(wchar_t *vastr, wchar_t *tag, double value, int prec, int level, wchar_t *parstr);
void wset_xml_s(wchar_t *vastr, wchar_t *tag, wchar_t *value, int level);
void wset_xml_d(wchar_t *vastr, wchar_t *tag, INT32 value, int level);
void wset_xml_f(wchar_t *vastr, wchar_t *tag, double value, int prec, int level);
void wset_tag(wchar_t *vastr, wchar_t *tag, int level);
void wset_tagparams(wchar_t *vastr, wchar_t *tag, wchar_t *params, bool empty, int level);
void wset_antitag(wchar_t *vastr, wchar_t *tag, int level);

#endif



