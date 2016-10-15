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

#if defined(__BORLANDC__)
#pragma -K -a1
#endif
//#define TESTPRT
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include <sys\stat.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//#include <malloc.h>
#include <bvideo.h>
#include <bstrings.h>
#include <tputil.h>

#ifndef GDIPRINTER
#define GDIPRINTER
#endif
#define PRBLKSIZE 2048
#define MAXBUFFERSIZE 500
#define PRLKM 16

#define elimbl(s) stpcvt((s),6)
#define elimwbl(s) wstpcvt((s),6)

/*
typedef char *farblkptr;

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
#ifdef __cplusplus
	  TextFl *Fl;
#endif
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
*/

static PRFILE *prf[PRLKM];

extern int spoolfl;
unsigned prblocksize = PRBLKSIZE;
int tehotausta;
int naytajono = 1;

#ifdef GDIPRINTER

static int SendCharsToGDIPrinter(char *line, int len, PRFILE *pf);
static int SendWCharsToGDIPrinter(wchar_t *line, int len, PRFILE *pf);

static int GDItopmarginDef = 36;
static int GDInormalWeightDef = FW_MEDIUM;
static int GDIboldWeightDef = FW_BOLD;
static wchar_t GDIfontNames[6][30] = {
	L"Times New Roman",
	L"Arial",
	L"Arial Narrow",
	L"Calibri",
	L"Courier New",
	L"Lucida Console"};

#endif

static int StartPrtPage(PRFILE *pf)
   {
   DOC_INFO_1W DocInfo;

   // Fill in the structure with info about this "document."
	DocInfo.pDocName = L"My Document";
	DocInfo.pOutputFile = NULL;
	DocInfo.pDatatype = L"RAW";
   // Inform the spooler the document is beginning.
	if (StartDocPrinter(pf->u.wp.hPrinter, 1, (LPBYTE)&DocInfo ) == 0) {
      ClosePrinter(pf->u.wp.hPrinter);
      pf->u.wp.hPrinter = 0;
      return(1);
      }
   // Start a page.
	if (!StartPagePrinter(pf->u.wp.hPrinter)) {
		EndDocPrinter(pf->u.wp.hPrinter);
      ClosePrinter(pf->u.wp.hPrinter);
      pf->u.wp.hPrinter = 0;
      return(1);
	  }
   return(0);
	}

PRFILE *openprfile(wchar_t *prtfname, int GDIkirjoitin, int wait, int append,
	char *trlate, int immediate)
{
	int nprf;
	char msg[400];
	wchar_t wmsg[200], PrinterName[120];
//	char cBuffer[MAXBUFFERSIZE];
	wchar_t wcBuffer[MAXBUFFERSIZE];
	DWORD bufSz = 120, cbNeeded, cReturned, err;
	HDC     hPrinter;
	OSVERSIONINFO osvi;
	PRINTER_INFO_1W PrtInfo;
	int oletuskirjoitin = 0;

	for (nprf = 0; nprf < PRLKM; nprf++)
		if (prf[nprf] == NULL)
			break;
	if (nprf == PRLKM) {
		writeerror_w(L"Kaikki kirjoitinliitännät käytössä. Tulostus ei onnstu", 0);
		return(NULL);		
		}
	if ((prf[nprf] = (PRFILE *)malloc(sizeof(PRFILE))) != NULL) {
		memset(prf[nprf], 0, sizeof(PRFILE));
		if (GDIkirjoitin == -1) {
			wchar_t merk = L'8', *mode[5] = {L"wt", L"at", L"w+t", L"a+t", L"rt"};

			if (trlate)
				merk = *trlate;
			prf[nprf]->u.Fl = new TextFl(prtfname, mode[append], merk);
			if (!prf[nprf]->u.Fl->IsOpen()) {
				delete prf[nprf]->u.Fl;
				writeerror_w(L"Tiedoston avaaminen ei onnisunut", 0);
				free(prf[nprf]);
				prf[nprf] = NULL;
				return(NULL);
				}
			prf[nprf]->prno = -2;
			return(prf[nprf++]);
			}
		prf[nprf]->trlate = trlate;
		upcasewstr(prtfname);
		elimwbl(prtfname);
		if (!wcscmp(prtfname, L"PRN")) {
			osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
			GetVersionEx(&osvi);
			if (osvi.dwPlatformId != VER_PLATFORM_WIN32_NT) {
// Windows 95/98/Me
				if (!EnumPrintersW(
					PRINTER_ENUM_DEFAULT,   // printer object types
					NULL,                   // name of printer object
					1,                      // information level
					(LPBYTE) msg,           // printer information buffer
					sizeof(msg),            // size of printer information buffer
					&cbNeeded,              // bytes received or required
					&cReturned              // number of printers enumerated
					)) {
					swprintf(wmsg, L"Oletuskirjoitinta ei löytynyt, Virhe %d/%d", GetLastError(), cbNeeded);
					writeerror_w(wmsg, 0);
					oletuskirjoitin = -1;
					}
				else {
					memcpy(&PrtInfo, msg, sizeof(PrtInfo));
					wcscpy(PrinterName, PrtInfo.pName);
					}
				}
			else {
				if (osvi.dwMajorVersion > 4) {
// Windows 2000, XP
					if (!GetDefaultPrinterW(PrinterName, &bufSz)) {
						err = GetLastError();
						if (err == ERROR_INSUFFICIENT_BUFFER)
							writeerror_w(L"Oletuskirjoittimen nimi liian pitkä (yli 79 merkkiä)", 0);
						else
							writeerror_w(L"Oletuskirjoitinta ei ole määritelty", 0);
						oletuskirjoitin = -1;
						}
					}
				else {
// Windows NT4
// Retrieve the default string from Win.ini (the registry).
// String will be in form "printername,drivername,portname".
					if (GetProfileStringW(L"windows", L"device", L",,,", wcBuffer, MAXBUFFERSIZE) <= 0) {
						writeerror_w(L"Oletuskirjoitinta ei ole määritelty", 0);
						oletuskirjoitin = -1;
						}
					else {
// Printer name precedes first "," character...
						wcstok(wcBuffer, L",");

// If given buffer too small, set required size and fail...
						if (wcslen(wcBuffer) >= 120) {
							writeerror_w(L"Oletuskirjoittimen nimi liian pitkä (yli 119 merkkiä)", 0);
							oletuskirjoitin = -1;
							}
						else {
							wcsncpy(PrinterName, wcBuffer, 119);
							}
						}
					}
				}
			if (!oletuskirjoitin)
			oletuskirjoitin = 1;
			}
		else
			wcscpy(PrinterName, prtfname);
		if (oletuskirjoitin != -1) {
			if (!GDIkirjoitin) {
				if (OpenPrinterW(PrinterName, (LPHANDLE) &hPrinter, NULL )) {
					prf[nprf]->prno = 1;
					prf[nprf]->u.wp.hPrinter = hPrinter;
					prf[nprf]->u.wp.fname = prtfname;
					oletuskirjoitin = 0;
					}
				else
					oletuskirjoitin = 2-oletuskirjoitin;
				}
			else {
				if ((hPrinter = CreateDCW(NULL, PrinterName, NULL, NULL)) != NULL) {
					prf[nprf]->prno = 2;
					prf[nprf]->u.wp.hPrinter = hPrinter;
					prf[nprf]->u.wp.fname = prtfname;
					SetBkMode((HDC)hPrinter, TRANSPARENT);
					prf[nprf]->u.wp.xPage = GetDeviceCaps (hPrinter, HORZRES);
					prf[nprf]->u.wp.yPage = GetDeviceCaps (hPrinter, VERTRES);
					prf[nprf]->u.wp.xppi = GetDeviceCaps (hPrinter, LOGPIXELSX);
					prf[nprf]->u.wp.yppi = GetDeviceCaps (hPrinter, LOGPIXELSY);
					prf[nprf]->u.wp.GDIparam.bottommargin = GetDeviceCaps (hPrinter, PHYSICALOFFSETY);
					prf[nprf]->u.wp.GDIparam.leftmargin = GetDeviceCaps (hPrinter, PHYSICALOFFSETX);
					prf[nprf]->u.wp.GDIparam.topmargin = GDItopmarginDef * prf[nprf]->u.wp.yppi / 72;
					if (prf[nprf]->u.wp.GDIparam.topmargin < prf[nprf]->u.wp.GDIparam.bottommargin)
						prf[nprf]->u.wp.GDIparam.topmargin = prf[nprf]->u.wp.GDIparam.bottommargin;
					prf[nprf]->u.wp.GDIparam.normalWeight = GDInormalWeightDef;
					prf[nprf]->u.wp.GDIparam.boldWeight = GDIboldWeightDef;
					memcpy(prf[nprf]->u.wp.GDIparam.fontName, GDIfontNames, sizeof(prf[nprf]->u.wp.GDIparam.fontName));
					oletuskirjoitin = 0;
					}
				else
					oletuskirjoitin = 2-oletuskirjoitin;
				}
			}
		if (oletuskirjoitin) {
			if (oletuskirjoitin == 1) {
				writeerror_w(L"Oletuskirjoittimen käyttöönotto ei onnistu",0);
				wcscpy(prtfname, L"NUL");
				}
			if (!wcscmp(prtfname, L"NUL")) {
				prf[nprf]->prno = 0;
				return(prf[nprf]);
				}
			prf[nprf]->prno = -1;
			prf[nprf]->u.f.file = NULL;
// #if defined(__BORLANDC__)
//			if ((prf[nprf]->u.f.handle = open(wcstoansi(cBuffer, prtfname, 200),O_WRONLY | O_CREAT |
//				O_BINARY | (append ? O_APPEND : O_TRUNC), S_IREAD|S_IWRITE))
//				> 0) {
// #else
			if ((prf[nprf]->u.f.handle = _wopen(prtfname,O_WRONLY | O_CREAT |
				O_BINARY | (append ? O_APPEND : O_TRUNC), S_IREAD|S_IWRITE))
				> 0) {
// #endif
					prf[nprf]->u.f.file = _wfdopen(prf[nprf]->u.f.handle,
					append ? L"ab" : L"wb");
					setvbuf(prf[nprf]->u.f.file, NULL, _IONBF, 0);
					}
				if (prf[nprf]->u.f.file == NULL) {
					iowerr(prtfname, 0, -1L);
					free(prf[nprf]);
					prf[nprf] = NULL;
					return(NULL);
					}
			if (append)
				fseek(prf[nprf]->u.f.file, 0L, SEEK_END);
			prf[nprf]->u.f.fname = prtfname;
			prf[nprf]->u.f.immed = immediate;
			return(prf[nprf]);
			}
		}
	if (prf[nprf] == NULL) {
		swprintf(wmsg, L"Ei tilaa tiedoston %s avaamiselle", prtfname);
		writeerror_w(wmsg, 0);
		return(NULL);
		}
	spoolfl = 1;
	return(prf[nprf]);
}

void closeprfile(PRFILE *pf)
   {
   int i;

	if (pf == NULL) return;
	if (pf->prno == -2) {
		delete pf->u.Fl;
		pf->u.Fl = NULL;
		}
    else if (pf->prno < 0) {
      fclose(pf->u.f.file);
      }
	else if (pf->prno == 1) {
      ClosePrinter(pf->u.wp.hPrinter);
      }
	else if (pf->prno == 2) {
		DeleteDC((HDC)pf->u.wp.hPrinter);
		}
	free(pf);
	for (i = 0; i < PRLKM; i++)
		if (pf == prf[i])
			prf[i] = NULL;
	}

static int SendCharsToPrinter(char *line, int len, PRFILE *pf)
	{
	DWORD dwBytesWritten;

	if (!pf->u.wp.hPrinter)
		return(1);
	if (!pf->u.wp.PageOpen) {
		if (StartPrtPage(pf))
			return(1);
		}
	pf->u.wp.PageOpen = 2;
	if (!WritePrinter(pf->u.wp.hPrinter, line, len, &dwBytesWritten )) {
		EndPagePrinter(pf->u.wp.hPrinter);
		EndDocPrinter(pf->u.wp.hPrinter);
		ClosePrinter(pf->u.wp.hPrinter);
		pf->u.wp.hPrinter = 0;
		pf->u.wp.PageOpen = 0;
		return(1);
		}
	if (line[len-1] == 12 || line[len-1] == 4 ||
		(len > 1 && (line[len-2] == 12 || line[len-2] == 4))) {
		// End the page.
		pf->u.wp.PageOpen = 0;
		if (!EndPagePrinter(pf->u.wp.hPrinter )) {
			EndDocPrinter(pf->u.wp.hPrinter);
			ClosePrinter(pf->u.wp.hPrinter);
			pf->u.wp.hPrinter = 0;
			return(1);
			}
		// Inform the spooler that the document is ending.
		if (!EndDocPrinter(pf->u.wp.hPrinter)) {
			ClosePrinter(pf->u.wp.hPrinter );
			pf->u.wp.hPrinter = 0;
			return(1);
			}
		}
	return(0);
	}

int sendline(char *linein, PRFILE *pf)
	{
	char *p, line[2048];
	wchar_t wline[2048];

	if (!*linein || !pf->prno) return(0);
	strncpy(line, linein, 2047);
	line[2047] = 0;
	if (pf->prno == 2) {
		return(SendCharsToGDIPrinter(line, strlen(line), pf));
		}
	if (pf->prno == -2) {
		return(pf->u.Fl->WriteLine(ansitowcs(wline, line, 2047)));
		}
	if (pf->trlate) for (p=line; *p!=0; p++) *p = pf->trlate[*p];
	if (pf->prno > 0) {
		return(SendCharsToPrinter(line, strlen(line), pf));
		}
	if (pf->prno < 0 && fputs(line,pf->u.f.file) == EOF) {
		iowerr(pf->u.f.fname, 2, 0L);
		return(1);
		}
	else if (pf->u.f.immed) {
		fflush(pf->u.f.file);
	}
	return(0);
}

int sendwline(wchar_t *linein, PRFILE *pf)
	{
	char *p, line[2048];
	wchar_t wline[2048];

	if (!*linein || !pf || !pf->prno) return(0);
	wcsncpy(wline, linein, 2047);
	wline[2047] = 0;
	if (pf->prno == 2) {
		return(SendWCharsToGDIPrinter(wline, wcslen(wline), pf));
		}
	if (pf->prno > 0) {
		wcstoansi(line, wline, 2047);
		if (pf->trlate) for (p=line; *p!=0; p++) *p = pf->trlate[*p];
		return(SendCharsToPrinter(line, strlen(line), pf));
		}
	if (pf->prno == -2) {
		return(pf->u.Fl->WriteLine(wline));
		}
	if (pf->prno < 0 && fputs(line,pf->u.f.file) == EOF) {
		iowerr(pf->u.f.fname, 2, 0L);
		return(1);
		}
	else if (pf->u.f.immed) {
		fflush(pf->u.f.file);
	}
	return(0);
}

int sendchars(char *line, int len, PRFILE *pf)
	{
	if (!len || !pf->prno)
		return(0);
	if (pf->prno == 2) {
		return(SendCharsToGDIPrinter(line, strlen(line), pf));
		}
	if (pf->prno > 0) {
		return(SendCharsToPrinter(line, strlen(line), pf));
		}
	if (pf->prno == -2) {
		return(pf->u.Fl->WriteBytes(line, 1, len));
		}
	if (pf->prno < 0) {
		while (len-- > 0) {
			if (fputc((pf->trlate) ? pf->trlate[*line] : *line, pf->u.f.file)
				== EOF) {
				iowerr(pf->u.f.fname, 2, 0L);
				return(1);
				}
			line++;
			}
		}
	if (pf->prno < 0 && pf->u.f.immed) {
		fflush(pf->u.f.file);
	}
	return(0);
}

int sendwchars(wchar_t *wline, int len, PRFILE *pf)
	{
	char line[400];

	if (!len || !pf->prno)
		return(0);
	if (pf->prno == 2) {
		return(SendWCharsToGDIPrinter(wline, wcslen(wline), pf));
		}
	if (pf->prno > 0) {
		wcstoansi(line, wline, 399);
		return(SendCharsToPrinter(line, strlen(line), pf));
		}
	if (pf->prno == -2) {
		return(pf->u.Fl->WriteBytes((char *)wline, 2, len));
		}
	if (pf->prno < 0) {
		while (len-- > 0) {
			if (fputwc(*wline, pf->u.f.file)
				== WEOF) {
				iowerr(pf->u.f.fname, 2, 0L);
				return(1);
				}
			wline++;
			}
		}
	if (pf->prno < 0 && pf->u.f.immed) {
		fflush(pf->u.f.file);
	}
	return(0);
}

wchar_t *prfname(PRFILE *pf)
	{
	if (pf->prno == -2)
		return(pf->u.Fl->FileName);
	else
		return(pf->u.f.fname);
	}

#ifdef GDIPRINTER

void getParamsGDI(PRFILE *pf, GDIparamtp *GDIparam)
	{
	if (!pf || pf->prno != 2 || !pf->u.wp.hPrinter)
		return;
	*GDIparam = pf->u.wp.GDIparam;
	}

void setParamsGDI(PRFILE *pf, GDIparamtp *GDIparam)
	{
	if (!pf || pf->prno != 2 || !pf->u.wp.hPrinter)
		return;
	pf->u.wp.GDIparam = *GDIparam;
	}

static int CALLBACK listfonts(const LOGFONTW *LogFont, const TEXTMETRIC *lpntme,
  DWORD FontType, LPARAM lParam)
{
	wchar_t *list = (wchar_t *) lParam;
	int size = list[0];
	int written = list[1];
	int len = list[2];

	if (written >= size)
		return(0);
	list[1] = ++written;
	wcsncpy(list+written*(len+1), LogFont->lfFaceName, len);
	list[(written+1)*(len+1)-1] = 0;
	return(written < size);
}

int enumfontsGDI(PRFILE *pf, wchar_t *facename, void *fonts)
{
	LOGFONTW LogFont;
	int ret;

	if (!pf || pf->prno != 2 || !pf->u.wp.hPrinter)
		return(1);
	LogFont.lfCharSet = ANSI_CHARSET;
	wcsncpy(LogFont.lfFaceName, facename, LF_FACESIZE);
	LogFont.lfPitchAndFamily = 0;
#ifdef _BORLAND_
	ret = EnumFontFamiliesExW((HDC)pf->u.wp.hPrinter, &LogFont, (int (CALLBACK *)(const tagLOGFONTW *, const tagTEXTMETRICW *, unsigned long, long))listfonts, (LPARAM) fonts, 0);
#else
	ret = EnumFontFamiliesExW((HDC)pf->u.wp.hPrinter, &LogFont, listfonts, (LPARAM) fonts, 0);
#endif
	return(ret);
}

int selectfontGDI(PRFILE *pf, GDIfontTp *fnt)
	{
	LOGFONTW LogFont;
	HFONT hFont;
	int err = 0, yPg2;
	SIZE szMetric;
	HGDIOBJ hObj;

	if (!pf || pf->prno != 2 || !pf->u.wp.hPrinter)
		return(1);

//	int left;
//	pf->u.wp.xPage = GetDeviceCaps ((HDC)pf->u.wp.hPrinter, HORZRES);
//	pf->u.wp.yPage = GetDeviceCaps ((HDC)pf->u.wp.hPrinter, VERTRES);
//	pf->u.wp.xppi = GetDeviceCaps ((HDC)pf->u.wp.hPrinter, LOGPIXELSX);
//	pf->u.wp.yppi = GetDeviceCaps ((HDC)pf->u.wp.hPrinter, LOGPIXELSY);
	pf->u.wp.top = pf->u.wp.GDIparam.topmargin;
//	left = pf->u.wp.GDIparam.leftmargin;
//	if (pf->u.wp.top < pf->u.wp.yppi*pf->u.wp.GDIparam.topmargin/72) {
		yPg2 = pf->u.wp.yPage - 2 * (pf->u.wp.top + pf->u.wp.GDIparam.bottommargin);
//		pf->u.wp.top = pf->u.wp.yppi*pf->u.wp.GDIparam.topmargin/72 - pf->u.wp.top;
//		}
//	pf->u.wp.GDIparam.extramargin = pf->u.wp.GDIparam.leftmargin - (1000*left)/pf->u.wp.xPage;
//	if (pf->u.wp.GDIparam.extramargin < 0)
//		pf->u.wp.GDIparam.extramargin = 0;
	if (fnt->hfont)
		hFont = (HFONT) fnt->hfont;
	else {
		memset(&LogFont, 0, sizeof(LogFont));
		LogFont.lfItalic = fnt->italic;
		LogFont.lfWeight = fnt->bold ? pf->u.wp.GDIparam.boldWeight : pf->u.wp.GDIparam.normalWeight;
		LogFont.lfCharSet = ANSI_CHARSET;
		LogFont.lfOutPrecision = OUT_DEFAULT_PRECIS;
		LogFont.lfQuality = DEFAULT_QUALITY;
		switch (fnt->fontfamily) {
			case 0:
				LogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_ROMAN;
				LogFont.lfHeight = -MulDiv(fnt->size, pf->u.wp.yppi, 72);
				wcscpy(LogFont.lfFaceName, pf->u.wp.GDIparam.fontName[0]);
				break;
			case 1:
				LogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
				LogFont.lfHeight = -MulDiv(fnt->size, pf->u.wp.yppi, 72);
				wcscpy(LogFont.lfFaceName, pf->u.wp.GDIparam.fontName[1]);
				break;
			case 2:
				LogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
				LogFont.lfHeight = -MulDiv(fnt->size, pf->u.wp.yppi, 72);
				wcscpy(LogFont.lfFaceName, pf->u.wp.GDIparam.fontName[2]);
				break;
			case 3:
				LogFont.lfPitchAndFamily = VARIABLE_PITCH | FF_SWISS;
				LogFont.lfHeight = -MulDiv(fnt->size, pf->u.wp.yppi, 72);
				wcscpy(LogFont.lfFaceName, pf->u.wp.GDIparam.fontName[3]);
				break;
			case 4:
				LogFont.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;
				LogFont.lfWidth = pf->u.wp.xppi / fnt->pitch;
				wcscpy(LogFont.lfFaceName, pf->u.wp.GDIparam.fontName[4]);
				if (!fnt->size)
					fnt->size = 12;
				break;
			case 5:
				LogFont.lfPitchAndFamily = FIXED_PITCH | FF_MODERN;
				LogFont.lfWidth = pf->u.wp.xppi / fnt->pitch;
				wcscpy(LogFont.lfFaceName, pf->u.wp.GDIparam.fontName[5]);
				if (!fnt->size)
					fnt->size = 12;
				break;
			}
		hFont = CreateFontIndirectW(&LogFont);
		}
	if (hFont) {
		hObj = SelectObject((HDC)pf->u.wp.hPrinter, hFont);
		if (!hObj)
			hFont = NULL;
		fnt->hfont = hFont;
		pf->u.wp.GDIparam.Currentfont = *fnt;
		}
	else {
		*fnt = pf->u.wp.GDIparam.Currentfont;
		err = 1;
		}
	GetTextExtentPoint32W((HDC)pf->u.wp.hPrinter, L"0123456789", 10, &szMetric);
	fnt->numwidth = szMetric.cx / 10;
	if (!fnt->line)
		fnt->line = fnt->size;
	fnt->lnheight = (72*yPg2) / (pf->u.wp.yppi*fnt->line);
	fnt->chwidth = pf->u.wp.xPage/fnt->numwidth;
	fnt->numwidth = (1000*fnt->numwidth)/pf->u.wp.xPage;
	pf->u.wp.GDIparam.Currentfont = *fnt;
	return(err);
	}


// putfldGDI sijoittaa tekstin paikkaan, jonka pystysijainti ilmoitetaan
// 1/72 tuuman yksiköissä ja vaakasijainti yksiköissä tulostusalue/1000
// ohjelma palauttaa tulostussijainnin yksiköissä 1/72 tuumaa
// putwfldGDI jakaa, kun oik < 0, liian pitkät rivit usealle samasta vaakakohdasta 
// alkavalle riville jakaen välilyöntien kohdilta

int putfldGDI(char *str, int pos, int oik, int vpos, PRFILE *pf)
	{
	SIZE szMetric;
	wchar_t st[302];

	if (!pf || pf->prno != 2)
		return(-1);
	pf->u.wp.PageOpen = 2;
	pos = (pos+pf->u.wp.GDIparam.extramargin) * pf->u.wp.xPage / 1000;
	vpos = vpos * pf->u.wp.yppi / 72 + pf->u.wp.top;
	ansitowcs(st, str, 299);
	st[300] = 0;
//	if (pf->trlate)
//		for (p=st; *p!=0; p++)
//			*p = pf->trlate[*p];
	if (oik) {
		// Retrieve the width of the string str

		GetTextExtentPoint32W((HDC)pf->u.wp.hPrinter, st, wcslen(st), &szMetric);

		// Compute the starting point for the text-output operation.

		pos -= szMetric.cx;
		}

	TextOutW((HDC)pf->u.wp.hPrinter, pos, vpos, st, wcslen(st));

	return(vpos*72/pf->u.wp.yppi);
	}

int putwfldGDI(wchar_t *str, int pos, int oik, int vpos, PRFILE *pf)
	{
	SIZE szMetric;
	wchar_t st[2002];
	int ret = 1;

	if (!pf || pf->prno != 2)
		return(-1);
	pf->u.wp.PageOpen = 2;
	pos = (pos+pf->u.wp.GDIparam.extramargin) * pf->u.wp.xPage / 1000;
	vpos = vpos * pf->u.wp.yppi / 72 + pf->u.wp.top;
	wcsncpy(st, str, 2000);
	st[2000] = 0;
	if (oik) {
		// Retrieve the width of the string str

		GetTextExtentPoint32W((HDC)pf->u.wp.hPrinter, st, wcslen(st), &szMetric);

		// Compute the starting point for the text-output operation.

		if (oik > 0)
			pos -= szMetric.cx;
		else {
			while (pos + szMetric.cx > pf->u.wp.xPage - pf->u.wp.GDIparam.leftmargin
				- pf->u.wp.GDIparam.rightmargin) {
				int i, ii = -1;
				i = (wcslen(st) * (pf->u.wp.xPage - pos - pf->u.wp.GDIparam.leftmargin
					- pf->u.wp.GDIparam.rightmargin)) / szMetric.cx;
				do {
					i--;
					for (; i > 0 && st[i] != L' '; i--);
					if (ii > 0)
						st[ii] = L' ';
					ii = i;
					st[i] = 0;
					GetTextExtentPoint32W((HDC)pf->u.wp.hPrinter, st, wcslen(st), &szMetric);
					} while (i > 0 && pos + szMetric.cx > pf->u.wp.xPage - pf->u.wp.GDIparam.leftmargin
						 - pf->u.wp.GDIparam.rightmargin);
				ret = TextOutW((HDC)pf->u.wp.hPrinter, pos, vpos, st, wcslen(st));
				if (!ret)
					break;
				wmemmove(st, st+i+1, wcslen(st+i+1)+1);
				GetTextExtentPoint32W((HDC)pf->u.wp.hPrinter, st, wcslen(st), &szMetric);
				if ((pf->u.wp.yPage-vpos) * 72 / (pf->u.wp.yppi * pf->u.wp.GDIparam.Currentfont.line) - 1 > 0)
					vpos += pf->u.wp.yppi * pf->u.wp.GDIparam.Currentfont.line / 72;
				else {
					endpageGDI(pf);
					startpageGDI(pf);
					vpos = pf->u.wp.top;
					}
				}
			}
		}

	if (ret)
		ret = TextOutW((HDC)pf->u.wp.hPrinter, pos, vpos, st, wcslen(st));

	return(ret ? (vpos - pf->u.wp.top) * 72 / pf->u.wp.yppi : -9999);
	}

// textlengthGDI palauttaa tekstikentän pituuden rivin yksikössä rivi/1000

int textlengthGDI(PRFILE *pf, wchar_t *st)
{
	SIZE szMetric;

	GetTextExtentPoint32W((HDC)pf->u.wp.hPrinter, st, wcslen(st), &szMetric);
	return(szMetric.cx * 1000 / pf->u.wp.xPage);
}

int startdocGDI(PRFILE *pf)
	{
	DOCINFOW di;
	int nError;

	// Zero and then initialize the members of a DOCINFO structure.

	if (!pf || pf->prno != 2 || !pf->u.wp.hPrinter)
		return(0);
	 if (pf->u.wp.DocOpen)
		 enddocGDI(pf);
	 pf->u.wp.vertpos = 0;
	memset( &di, 0, sizeof(DOCINFO) );
    di.cbSize = sizeof(DOCINFO); 
	di.lpszDocName = L"Tulospalvelu";
	 di.lpszOutput = (LPCWSTR) NULL;
	di.lpszDatatype = (LPCWSTR) NULL;
	di.fwType = 0;
 
	 // Begin a print job by calling the StartDoc function.

	 nError = StartDocW((HDC)pf->u.wp.hPrinter, &di); 
	 if (nError == SP_ERROR)
	 { 
		  writeerror_w(L"Virhe kirjoitintulostusta käynnistettäessä", 0);
		  goto Error; 
	 }
	 pf->u.wp.DocOpen = 1;
 
	Error:
	return(0);
	}

int startpageGDI(PRFILE *pf)
	{
	int nError;

    // Zero and then initialize the members of a DOCINFO structure. 

 	if (!pf || pf->prno != 2 || !pf->u.wp.hPrinter)
		return(0);

	 // Already open, but not written to

	 if (pf->u.wp.PageOpen == 1)
		return(0);

	 // Already open and written to

	 if (pf->u.wp.PageOpen > 1)
		endpageGDI(pf);

	 if (!pf->u.wp.DocOpen)
		 startdocGDI(pf);
 
    // Inform the driver that the application is about to begin 
    // sending data. 
 
    nError = StartPage((HDC)pf->u.wp.hPrinter); 
	 if (nError <= 0)
    { 
		  writeerror_w(L"Virhe sivun tulostusta käynnistettäessä", 0);
		  goto Error;
	 }
	SetTextAlign((HDC)pf->u.wp.hPrinter, TA_TOP | TA_LEFT);
    pf->u.wp.PageOpen = 1;
	pf->u.wp.vertpos = 0;

	Error:
	return(0);
	}

int endpageGDI(PRFILE *pf)
	{
	int ret = 1;

 	if (!pf || pf->prno != 2 || !pf->u.wp.hPrinter)
		return(1);
   if (!pf->u.wp.PageOpen)
	   return(1);


   if (pf->u.wp.PageOpen == 1)
	   return(0);

   pf->u.wp.PageOpen = 0;
   if ((ret = EndPage((HDC)pf->u.wp.hPrinter)) <= 0) {
      EndDoc((HDC)pf->u.wp.hPrinter);
      DeleteDC((HDC)pf->u.wp.hPrinter);
      pf->u.wp.hPrinter = 0;
      return(ret ? ret : 1);
      }
	return(0);
	}

int enddocGDI(PRFILE *pf)
	{
 	if (!pf || pf->prno != 2 || !pf->u.wp.hPrinter)
		return(1);
   if (!pf->u.wp.DocOpen)
	   return(1);
   if (pf->u.wp.PageOpen > 1)
		endpageGDI(pf);
    pf->u.wp.PageOpen = 0;
	pf->u.wp.DocOpen = 0;
    if (EndDoc((HDC)pf->u.wp.hPrinter) <= 0) {
		DeleteDC((HDC)pf->u.wp.hPrinter );
		pf->u.wp.hPrinter = 0;
		return(1);
		}
	return(0);
	}

int abortdocGDI(PRFILE *pf)
	{
 	if (!pf || pf->prno != 2 || !pf->u.wp.hPrinter)
		return(1);
   if (!pf->u.wp.DocOpen)
	   return(1);
    pf->u.wp.PageOpen = 0;
	pf->u.wp.DocOpen = 0;
   if (AbortDoc((HDC)pf->u.wp.hPrinter) <= 0) {
      DeleteDC((HDC)pf->u.wp.hPrinter );
      pf->u.wp.hPrinter = 0;
      return(1);
      }
	return(0);
	}

// SendCharsToGDIPrinter
// ohjelma palauttaa vielä sivulle mahtuvien rivien lukumäärän
// mukaanlukien keskeneräinen käsiteltävänä oleva rivi 
// Virhetilanteessa palautetaa -999

static int SendCharsToGDIPrinter(char *line, int len, PRFILE *pf)
	{
	int nlc = 0, prtPage = 0;
	char *p;

 	if (!pf || pf->prno != 2 || !pf->u.wp.hPrinter)
      return(-100);
   if (!pf->u.wp.PageOpen) {
		if (startpageGDI(pf))
			return(-999);
		 }
	pf->u.wp.PageOpen = 2;
	if (pf->u.wp.vertpos > 72*pf->u.wp.yPage/pf->u.wp.yppi)
		pf->u.wp.vertpos = 0;
	line[len] = 0;
	for (p = line; *p;) {
		if (*p == 12) {
			prtPage = 1;
			}
		if (*p == 10) {
			nlc++;
			}
		if (*p == 10 || *p == 12 || *p == 13) {
			*p = 0;
			if (line[0]) {
			   if (putfldGDI(line, 0, 0, pf->u.wp.vertpos, pf) < -900) {
					DeleteDC((HDC)pf->u.wp.hPrinter);
			      pf->u.wp.hPrinter = 0;
			      return(-999);
					}
				}
			pf->u.wp.vertpos += nlc*pf->u.wp.GDIparam.Currentfont.line;
			nlc = 0;
			if (prtPage || pf->u.wp.vertpos > 72*pf->u.wp.yPage/pf->u.wp.yppi) {
      // End the page and document.
				if (enddocGDI(pf)) {
		         DeleteDC((HDC)pf->u.wp.hPrinter);
		         pf->u.wp.hPrinter = 0;
				   return(-999);
		         }
				pf->u.wp.vertpos = 0;
				prtPage = 0;
				}
			if (!p[1])
				break;
			memmove(line, p+1, strlen(p+1)+1);
			p = line;
			}
		else
			p++;
		}
	return((pf->u.wp.yPage-pf->u.wp.vertpos*pf->u.wp.yppi/72)/pf->u.wp.GDIparam.Currentfont.line);
	}

static int SendWCharsToGDIPrinter(wchar_t *line, int len, PRFILE *pf)
	{
	int nlc = 0, prtPage = 0;
	wchar_t *p;

	if (!pf || pf->prno != 2 || !pf->u.wp.hPrinter)
		return(-100);
	if (!pf->u.wp.PageOpen) {
		if (startpageGDI(pf))
			return(-999);
		}
	pf->u.wp.PageOpen = 2;
	if (pf->u.wp.vertpos > 72*pf->u.wp.yPage/pf->u.wp.yppi)
		pf->u.wp.vertpos = 0;
	line[len] = 0;
	for (p = line; *p;) {
		if (*p == 12) {
			prtPage = 1;
			}
		if (*p == 10) {
			nlc++;
			}
		if (*p == 10 || *p == 12 || *p == 13) {
			*p = 0;
			if (line[0]) {
				if (putwfldGDI(line, 0, 0, pf->u.wp.vertpos, pf) < -900) {
					DeleteDC((HDC)pf->u.wp.hPrinter);
					pf->u.wp.hPrinter = 0;
					return(-999);
					}
				}
			pf->u.wp.vertpos += nlc*pf->u.wp.GDIparam.Currentfont.line;
			nlc = 0;
			if (prtPage || pf->u.wp.vertpos > 72*pf->u.wp.yPage/pf->u.wp.yppi) {
		// End the page and document.
				if (enddocGDI(pf)) {
					DeleteDC((HDC)pf->u.wp.hPrinter);
					pf->u.wp.hPrinter = 0;
					return(-999);
					}
				pf->u.wp.vertpos = 0;
				prtPage = 0;
				}
			if (!p[1])
				break;
			memmove(line, p+1, 2*(wcslen(p+1)+1));
			p = line;
			}
		else
			p++;
		}
	return((pf->u.wp.yPage-pf->u.wp.vertpos*pf->u.wp.yppi/72)/pf->u.wp.GDIparam.Currentfont.line);
	}

#endif