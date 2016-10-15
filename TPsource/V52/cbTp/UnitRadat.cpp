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

#include "UnitRadat.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <bstrings.h>
#ifndef MAXOSUUSLUKU
#include "WinHk.h"
#include "HkMuotoilu.h"
#else
#include "UnitMain.h"
#endif
#include "TulostusUnit2.h"
#include "UnitKartta.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormRadat *FormRadat;

int lueVainRadatXml(wchar_t *filename, IOFCourseData *CseData);
int kirjVainRadatXml(wchar_t *filename, IOFCourseData *CseData);

//---------------------------------------------------------------------------
/*typedef struct {
   wchar_t tunnus[11];
   int rastiluku;
   int ennakko;     // nollaus ennakko sek ennen l‰htˆ‰, -32767: vaihtelee
   int maalilaji;
   int rastikoodi[MAXNRASTI];
   } ratatp;
*/

typedef struct {
   INT32 nro;
   UINT16 koodi;
   } kilptp;

static kilptp kilp[MAXNKILP];

typedef struct {
   wchar_t x;
   wchar_t y;
   } piikkitp;

static piikkitp piikit[248] = {
   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
   {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0}, {0, 0},
   {2, 1}, {3, 1}, {4, 1}, {5, 1}, {6, 1}, {7, 1}, {8, 1}, {9, 1}, {1, 2}, {2, 2},
   {3, 2}, {4, 2}, {5, 2}, {6, 2}, {7, 2}, {8, 2}, {9, 2}, {10, 2}, {1, 3}, {2, 3},
   {3, 3}, {4, 3}, {5, 3}, {6, 3}, {7, 3}, {8, 3}, {9, 3}, {10, 3}, {1, 4}, {2, 4},
   {3, 4}, {4, 4}, {5, 4}, {6, 4}, {7, 4}, {0, 0}, {8, 4}, {0, 0}, {9, 4}, {10, 4},
   {1, 5}, {2, 5}, {3, 5}, {4, 5}, {5, 5}, {6, 5}, {7, 5}, {8, 5}, {9, 5}, {10, 5},
   {1, 6}, {2, 6}, {3, 6}, {4, 6}, {7, 6}, {0, 0}, {8, 6}, {9, 6}, {0, 0}, {10, 6},
   {1, 7}, {2, 7}, {3, 7}, {4, 7}, {7, 7}, {8, 7}, {9, 7}, {0, 0}, {0, 0}, {10, 7},
   {1, 8}, {2, 8}, {3, 8}, {4, 8}, {7, 8}, {8, 8}, {9, 8}, {10, 8}, {1, 9}, {2, 9},
   {3, 9}, {4, 9}, {5, 9}, {6, 9}, {7, 9}, {8, 9}, {9, 9}, {10, 9}, {1, 10}, {2, 10},
   {3, 10}, {4, 10}, {5, 10}, {6, 10}, {7, 10}, {8, 10}, {9, 10}, {10, 10}, {1, 11}, {2, 11},
   {3, 11}, {4, 11}, {5, 11}, {6, 11}, {7, 11}, {8, 11}, {9, 11}, {10, 11}, {1, 12}, {2, 12},
   {3, 12}, {4, 12}, {5, 12}, {6, 12}, {7, 12}, {8, 12}, {9, 12}, {10, 12}, {1, 13}, {2, 13},
   {3, 13}, {4, 13}, {5, 13}, {6, 13}, {7, 13}, {8, 13}, {9, 13}, {10, 13}, {1, 14}, {2, 14},
   {3, 14}, {4, 14}, {5, 14}, {6, 14}, {7, 14}, {8, 14}, {9, 14}, {10, 14}, {2, 15}, {3, 15},
   {4, 15}, {5, 15}, {6, 15}, {7, 15}, {8, 15}, {9, 15},  {0, 0},   {0, 0},  {0, 0},  {0, 0},
   {1, 7}, {2, 7}, {3, 7}, {4, 7}, {7, 7}, {0, 0}, {8, 7}, {9, 7}, {0, 0}, {10, 7},
   {1, 8}, {2, 8}, {3, 8}, {4, 8}, {7, 8}, {8, 8}, {9, 8}, {10, 8}, {1, 9}, {2, 9},
   {3, 9}, {4, 9}, {5, 9}, {6, 9}, {7, 9}, {8, 9}, {9, 9}, {10, 9}, {1, 10}, {2, 10},
   {3, 10}, {4, 10}, {5, 10}, {6, 10}, {7, 10}, {8, 10}, {9, 10}, {10, 10}, {1, 11}, {2, 11},
   {3, 11}, {4, 11}, {5, 11}, {6, 11}, {7, 11}, {8, 11}, {9, 11}, {10, 11}, {1, 12}, {2, 12},
   {3, 12}, {4, 12}, {5, 12}, {6, 12}, {7, 12}, {8, 12}, {9, 12}, {10, 12}, {1, 13}, {2, 13},
   {3, 13}, {4, 13}, {5, 13}, {6, 13}, {7, 13}, {8, 13}, {9, 13}, {10, 13}};

static INT32 vsc = 100, psc = 100;
static INT32 marg_re = 240, marg2 = 600, top = 720, step = 1300;
static wchar_t ckdi[] = L"JPV";
static FILE *errfile;
static bool initialisoitu;
static INT errcount;
static INT sallialle31;
static INT rrlisa = -1;
#ifndef MAXOSUUSLUKU
static wchar_t rataxmlname[200] = L"radat1.xml";
static wchar_t rataxmlname0[] = L"radat1.xml";
#else
extern int ratatiedot;
static int kuntosuunnmoodi = 0;
extern wchar_t rataxmlfname[200];
static wchar_t rataxmlname[200] = L"radat.xml";
static wchar_t rataxmlname0[] = L"radat.xml";
extern IOFCourseData CourseData;
#endif

//---------------------------------------------------------------------------
__fastcall TFormRadat::TFormRadat(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	rata_re = (ratatp *) calloc(MAXRATA, sizeof(ratatp));
}
//---------------------------------------------------------------------------
__fastcall TFormRadat::~TFormRadat(void)
{
	free(rata_re);
}
//---------------------------------------------------------------------------

static void sendln_re(PRFILE *lstf, wchar_t *line)
   {
	static wchar_t buf[200];

	wcsncpy(buf+wcslen(buf), line, 199-wcslen(buf));
	if (line[wcslen(line)-1] <= 13) {
		sendwline(buf, lstf);
		memset(buf,0,sizeof(buf));
		}
   }

void kirj_error(wchar_t *msg, int viive, int errorfl)
   {
   static int count, ohita;
   static int vast;

   if (errorfl >= ohita) {
	  writeerror_w(msg, viive);
	  if (errorfl < 2 && ++count >= 6) {
		 vast = select3(3, L"N‰yt‰ jatkossa seuraavat ilmoitukset", L"Ilmoitukset",
			L"Varoitukset ja virheet", L"Pelk‰t virheet", L"Ei kumpiakaan", FormRadat);
		 switch (vast) {
			case 1:
			   ohita = 0;
			   break;
			case 2:
			   ohita = 1;
			   break;
			case 3:
			   ohita = 2;
			   break;
			}
		 count = 0;
		 }
	  }
   if (!errfile)
	  errfile = _wfopen(L"EMIT_ERR.TXT", L"wt");
   if (errfile) {
	  errcount++;
	  fputws(msg, errfile);
	  fputwc(L'\n', errfile);
	  }
   }

void TFormRadat::sortkoodit(void)
   {
   int i, ntoisto = 0, ed = -1;

   if (!nrasti)
	   return;
   n_erirasti = 0;
   qsortint((void *) koodit, nrasti, sizeof(ekditp), 0, sizeof(koodit[0].rasti) == 4);
   for (i = 1; i < nrasti; i++) {
		if (koodit[i].rasti != ed) {
			n_erirasti++;
			ed = koodit[i].rasti;
			}
	   if (koodit[i].rasti == koodit[i-1].rasti) {
			for (int j = 1; j <= i; j++) {
			   if (koodit[i].rasti != koodit[i-j].rasti)
					break;
			   if (koodit[i].koodi == koodit[i-j].koodi) {
					koodit[i].rasti = 32767;
					ntoisto++;
					break;
					}
			   }
		   }
	   }
   qsortint((void *)koodit, nrasti, sizeof(ekditp), 0, sizeof(koodit[0].rasti) == 4);
   nrasti -= ntoisto;
   for (i = nrasti; i < nrasti+ntoisto; i++) {
	   koodit[i].rasti = 0;
	   koodit[i].koodi = 0;
	   }
   for (i = 0; i < nrasti; ) {
	   int n = 1;
	   while (i + n < nrasti && koodit[i].rasti == koodit[i+n].rasti)
		   n++;
	   if (n > 1)
		   qsortint((void *)(koodit+i), n, sizeof(ekditp), (char *) &koodit[0].koodi - (char *) &koodit[0],
				sizeof(koodit[0].koodi) == 4);
	   i += n;
	   }
   }

void TFormRadat::tarkkoodit(void)
   {
   int j, k, s, s1, k1;
   wchar_t tc, msg[100];

   for (k = 0; k < nrasti; k++) {
	  if (koodit[k].rasti && koodit[k].koodi &&
		 ((koodit[k].rasti < 31 && !sallialle31) || koodit[k].koodi < 31 ||
		  koodit[k].rasti == 66 || koodit[k].koodi == 66 ||
		  koodit[k].rasti == 68 || koodit[k].koodi == 68 ||
		  koodit[k].rasti == 86 || koodit[k].koodi == 86 ||
		  koodit[k].rasti == 89 || koodit[k].koodi == 89 ||
		  koodit[k].rasti == 98 || koodit[k].koodi == 98 ||
		  koodit[k].rasti == 99 || koodit[k].koodi == 99 ||
		  koodit[k].rasti == 99 || koodit[k].koodi == 99 ||
		  koodit[k].rasti > MAXTUNNUS
		  )) {
		 swprintf(msg, L"Rastin numero %d tai e-koodi %d virheellinen tai poikkeuksellinen",
			koodit[k].rasti, koodit[k].koodi);
		 FormRadat->Memo1->Lines->Add(msg);
		}
	  }
   }

void TFormRadat::nayta_leimat(void)
{
   int k, r, c, tn;

	sortkoodit();
	for (r = 1; r < SGLeimat->RowCount; r++)
		for (c = 0; c < SGLeimat->ColCount; c++)
			SGLeimat->Cells[c][r] = L"";
	SGLeimat->RowCount = 2;
	SGLeimat->RowCount = n_erirasti + 2;
	r = 0;
	c = 1;
	tn = -1;
	for (k = 0; k < nrasti; k++) {
		if (c >= SGLeimat->ColCount || koodit[k].rasti != tn) {
			tn = koodit[k].rasti;
			c = 1;
			r++;
			if (r >= SGLeimat->RowCount-1)
			   SGLeimat->RowCount = r+2;
			SGLeimat->Cells[0][r] = UnicodeString(koodit[k].rasti);
			}
		SGLeimat->Cells[c][r] = UnicodeString(koodit[k].koodi);
		c++;
#ifdef MAXOSUUSLUKU
		if (koodit[k].pisteet) {
			SGLeimat->Cells[SGLeimat->ColCount-1][r] = UnicodeString(koodit[k].pisteet)+"p";
			}
#endif
		}
 }
void TFormRadat::lue_leimat(wchar_t *leimafnm)
   {
   TextFl *leima_file;
   int k, r, c, tn;
   wchar_t msg[80], line[200];
   static wchar_t LeimaFName[200];

   memset(koodit, 0, sizeof(koodit));
   nrasti = 0;
   if (!leimafnm) {
		if (ToimintaTila > 0)
			OpenDialog1->InitialDir = FormMain->CurrentDir;
		wcsncpy(LeimaFName, leimafname, sizeof(LeimaFName)/2-1);
		OpenDialog1->FileName = UnicodeString(leimafname);
		OpenDialog1->DefaultExt = L"lst";
		OpenDialog1->Filter = L"LST-tiedostot (*.lst)|*.LST|Kaikki tiedostot (*.*)|*.*";
		if (!OpenDialog1->Execute())
			return;
		wcsncpy(LeimaFName, OpenDialog1->FileName.c_str(), sizeof(LeimaFName)/2-1);
		}
   else
		wcsncpy(LeimaFName, leimafnm, sizeof(LeimaFName)/2-1);

   leima_file = new TextFl(LeimaFName,L"r");

   if (!leima_file->IsOpen()) {
	  swprintf(msg,L"Leimasinkooditiedoston %s avaaminen ei onnistu",
		  LeimaFName);
	  Application->MessageBoxW(msg, L"Virhe", MB_OK);
	  delete leima_file;
	  return;
	  }
   while (!leima_file->Feof()) {
		if (leima_file->ReadLine(line, 198) == NULL)
			break;
	  if (swscanf(line, L"%d %d\n", &tn, &k) < 2) break;
	  if (tn > MAXTUNNUS) {
		 swprintf(msg,L"Rastitunnus %d tiedostossa %s on liian suuri",
			tn, LeimaFName);
		 Application->MessageBoxW(msg, L"Virhe", MB_OK);
		 continue;
		 }
	  koodit[nrasti].rasti = tn;
	  koodit[nrasti].koodi = k;
	  nrasti++;
	  }
   delete leima_file;
   nayta_leimat();
   return;
   }

void TFormRadat::kirj_leimat(void)
   {
   TextFl *leima_file;
   int i;
   wchar_t msg[80], line[200];

   if (!nrasti) return;
   sortkoodit();

	if (ToimintaTila > 0)
		 SaveDialog1->InitialDir = FormMain->CurrentDir;
	SaveDialog1->FileName = UnicodeString(LeimaFName);
	SaveDialog1->DefaultExt = L"lst";
	SaveDialog1->Filter = L"LST-tiedostot (*.lst)|*.LST|Kaikki tiedostot (*.*)|*.*";
	if (!SaveDialog1->Execute())
		return;
	leima_file = new TextFl(SaveDialog1->FileName.c_str(),L"w");

   if (!leima_file->IsOpen()) {
	  Application->MessageBoxW(L"Tiedoston avaaminen ei onnistu", L"Virhe", MB_OK);
	  delete leima_file;
	  return;
	  }
   for (i = 0; i < nrasti; i++) {
	  swprintf(line, L"%3d %4d\n", koodit[i].rasti, koodit[i].koodi);
	  leima_file->WriteLine(line);
	  }
   delete leima_file;
   return;
   }

static int haeerotin(wchar_t *st, wchar_t *e)
   {
   int pos = 0;

   while (*st) {
	  if (wcswcind(*st, e) >= 0)
		 break;
	  pos++;
	  st++;
	  }
   if (*st)
	  return(pos);
   else
	  return(-1);
   }

static int tulkxml(wchar_t *line)
   {
   wchar_t *tokens[] = {
	  L"CourseData",
	  L"Course",
	  L"CourseName",
	  L"CourseId",
	  L"CourseControl",
	  L"Sequence",
	  L"ControlCode",
      L"Control",
      L"PunchingUnit",
      L"UnitCode",
	  L"ClassShortName",
      L"CourseVariation",
	  L"CourseVariationId"
	  };
   wchar_t *p, *p2;
   int i, n, lj = -100;

   p = wcsstr(line, L"<");
   if (p) {
	  n = sizeof(tokens)/sizeof(wchar_t *);
      line[0] = 0;
	  p++;
	  if (*p == L'/') {
         lj = 100;
		 p++;
		 }
      else {
		 lj = 0;
		 }
	  for (i = 0; i < n; i++) {
		 if (!wmemcmpU(p, tokens[i], wcslen(tokens[i])) && p[wcslen(tokens[i])] == L'>') {
			lj += i+1;
			p += wcslen(tokens[i])+1;
			break;
			}
		 }
	  if (lj > 0 && lj < 100) {
		 p2 = wcsstr(p, L"</");
		 if (p2 && !wmemcmpU(p2+2, tokens[i], wcslen(tokens[i])) && p2[2+wcslen(tokens[i])] == L'>') {
			while (p2 > p && p2[-1] == L' ')
			   p2--;
			*p2 = 0;
			while (*p == L' ')
			   p++;
			wcscpy(line, p);
			}
		 }
	  }
   if (lj < 0)
	  lj = 0;
   return(lj);
   }

int TFormRadat::importradat(void)
	{
	TextFl *rata_file;
	bool addvariation = false, leimat = false;
	static INT enn = -1, nmsar = 1, rastisar = 6, tiedtype = 0;
	INT i, j, ir, koodi[MAXNRASTI], lj, state = 0, ratavalmis = 0, seq, kdi, ntoisto = 0;
	INT maalirasti = 0, ekdi, nnimi = 0, inimi, nlisakoodi = 0, matkafl = 0;
	wchar_t ch, line[1000], msg[80], *p, *p1, snimi[20][20], ssnimi[20];
	wchar_t erotin[] = L",;\t\n";
	int tapa = 1;
	int blerotin = 0;

	tiedtype = CBTiedLaji->ItemIndex;

	switch (tiedtype) {
		case 1:
		case 0:
			Lue_Xml_File(RGlisaa->ItemIndex);
			if (kuntosuunnmoodi) {
				for (ir = 0; ir < nrata_re; ir++) {
					rata_re[ir].ennakko = 0;
					rata_re[ir].maalilaji = 4;
					}
				}
			return(0);
		case 5:
			lue_radat(0);
			return(0);
		case 6:
			lue_leimat(0);
			return(0);
		}

	if (ToimintaTila > 0)
		OpenDialog1->InitialDir = FormMain->CurrentDir;
	OpenDialog2->FileName = L"";
	OpenDialog2->Filter = L"Kaikki tiedostot (*.*)|*.*";
	if (!OpenDialog2->Execute())
		return(0);
	rata_file = new TextFl(OpenDialog2->FileName.c_str(),L"r");
	if (ToimintaTila > 0) {
		SetCurrentDir(FormMain->CurrentDir);
		}

	if (!rata_file->IsOpen()) {
		delete rata_file;
		return(0);
		}

	switch (tiedtype) {
		case 2:
			matkafl = 1;
			blerotin = 0;
			nmsar = 0;
			rastisar = 7;
			nmsar = RGOcadRata->ItemIndex == 1;
			if (!CBpilkkuerotin->Checked)
				wcscpy(erotin, L";\t\n");
			break;
		case 3:
			blerotin = 0;
			nmsar = 0;
			rastisar = 5;
			wcscpy(erotin, L"-\t\n");
			break;
		default:
			blerotin = RGErotin->ItemIndex;
			nmsar = _wtoi(EdtRataSij->Text.c_str()) - 1;
			rastisar = _wtoi(EdtKoodiSij->Text.c_str()) - 1;
		}
	if (leimat) {
		memset(koodit, 0, sizeof(koodit));
		nrasti = 0;
		}
	rataMuutoksia = true;
	memset(rata_re, 0, sizeof(ratatp)*MAXRATA);
	nrata_re = 0;
	while (!rata_file->Feof()) {
	  if (rata_file->ReadLine(line, sizeof(line)/2-2) == NULL) break;
		 {
		 memset(koodi, 0, sizeof(koodi));
		 if (blerotin) {
			p = wcstok(line, L" \n");
			if (!p)
			   continue;
			for (i = 0; p && i < nmsar; i++)
			   p = wcstok(NULL, L" \n");
			if (!p)
			   continue;
			wcsncpy(snimi[0], p, 20);
			snimi[0][20] = 0;
			nnimi = 1;
			for (; p && i < rastisar; i++)
			   p = wcstok(NULL, L" \n");
			for (i = 0; p && i < MAXNRASTI; i++) {
			   koodi[i] = _wtoi(p);
			   p = wcstok(NULL, L" \n");
			   }
			}
		 else {
			p = line;
			for (i = 0; i <= nmsar; i++) {
			   j = haeerotin(p, erotin);
			   if (j < 0)
				  break;
			   p[j] = 0;
			   if (i < nmsar)
				  p += j+1;
			   }
			wcsncpy(snimi[0], p, 20);
			snimi[0][20] = 0;
			nnimi = 1;
			p += j+1;
			for (; i <= rastisar; i++) {
			   j = haeerotin(p, erotin);
			   if (j < 0)
				  break;
					if (p[j] == L'-' && i > 2 && i < rastisar-1)
						i = rastisar-1;
			   p[j] = 0;
			   if (i < rastisar)
				  p += j+1;
			   }
			for (i = 0; p && i < MAXNRASTI; i++) {
			   if (!_wtoi(p))
				  break;
			   koodi[i] = _wtoi(p);
			   p += j+1;
			   j = haeerotin(p, erotin);
			   if (matkafl == 1)
				  {
				  p += j+1;
				  j = haeerotin(p, erotin);
				  }
			   if (j < 0)
				  break;
			   }
			}
		 }
      if (nnimi && (ratavalmis || koodi[0])) {
		 for (inimi = 0; inimi < nnimi; inimi++) {
			upcasewstr(snimi[inimi]);
			elimwbl(snimi[inimi]);
            p1 = snimi[inimi];
			for (p = snimi[inimi]; *p; p++) {
			   if (*p != L' ') {
				  *p1 = *p;
				  p1++;
				  }
			   }
			if (p - p1 > 1) {
			   swprintf(msg, L"Radan nimest‰ poistettu v‰lilyˆntej‰, uusi nimi %s");
			   Application->MessageBoxW(msg, L"", MB_OK);
			   }
			if (wcslen(snimi[inimi]) > 10) {
			   swprintf(msg, L"Radan nimi %s liian pitk‰, uusi nimi %10.10s", snimi[inimi], snimi[inimi]);
			   snimi[inimi][10] = 0;
			   Application->MessageBoxW(msg, L"", MB_OK);
			   }
			for (j = 0; j < nrata_re; j++) {
			   if (!wcscmpU(snimi[inimi], rata_re[j].tunnus)) {
				  ntoisto++;
				  break;
				  }
			   }
			if (j < nrata_re)
			   continue;
			wcscpy(rata_re[nrata_re].tunnus, snimi[inimi]);
			rata_re[nrata_re].ennakko = enn;
			rata_re[nrata_re].maalilaji = tapa;
			ir = 0;
			for (j = 0; j < MAXNRASTI; j++) {
			   if (koodi[j]) {
				  rata_re[nrata_re].rastikoodi[ir] = koodi[j];
				  ir++;
				  }
//               else
//                  break;
			   }
			if (maalirasti) {
			   rata_re[nrata_re].rastikoodi[ir] = maalirasti;
			   ir++;
			   }
			rata_re[nrata_re].rastiluku = ir;
			nrata_re++;
			}
		 nnimi = 0;
		 ratavalmis = 0;
		 }
	  }
   swprintf(msg, L"Tiedostosta luettu %d rataa", nrata_re);
   if (ntoisto)
	  swprintf(msg+wcslen(msg), L". %d toistettua rataa ohitettu", ntoisto);
   Application->MessageBoxW(msg, L"", MB_OK);
   if (leimat) {
	  swprintf(msg, L"Leimasinkooditietoihn siirretty %d koodia, joista %d rastin mukaan", nrasti, nlisakoodi);
	   Application->MessageBoxW(msg, L"", MB_OK);
	  }
   delete rata_file;
   return(0);
   }

void TFormRadat::lue_radat(wchar_t *fname)
   {
   TextFl *rata_file;
   int i, j, ir, tn;
   wchar_t ch, line[300], msg[80], *p;

   if (!fname) {
		wcsncpy(RataFName, ratafname, sizeof(RataFName)/2-1);
		if (ToimintaTila > 0)
			OpenDialog1->InitialDir = FormMain->CurrentDir;
		OpenDialog1->FileName = UnicodeString(RataFName);
		OpenDialog1->DefaultExt = L"lst";
		OpenDialog1->Filter = L"LST-tiedostot (*.lst)|*.LST|Kaikki tiedostot (*.*)|*.*";
		if (!OpenDialog1->Execute())
			return;
		wcsncpy(RataFName, OpenDialog1->FileName.c_str(), sizeof(RataFName)/2-1);
		}
   else
		wcsncpy(RataFName, fname, sizeof(RataFName)/2-1);

	rata_file = new TextFl(RataFName,L"r");
	if (!rata_file->IsOpen()) {
		delete rata_file;
		return;
		}
	rataMuutoksia = true;
	memset(rata_re, 0, sizeof(ratatp)*MAXRATA);
	for (nrata_re = 0; ; nrata_re++) {
		if (rata_file->ReadLine(line, 298) == NULL) break;
	  p = wcstok(line, L" \t");
	  wcsncpy(rata_re[nrata_re].tunnus, p, 10);
	  p = wcstok(NULL, L" \t");
	  if (p) {
		 rata_re[nrata_re].ennakko = _wtoi(p);
		 p = wcstok(NULL, L" \t");
		 if (p)
			rata_re[nrata_re].maalilaji = _wtoi(p);
		 }
	  ir = 0;
	  for (i = 0; i < 2; i++) {
		 if (rata_file->ReadLine(line, 298) == NULL) goto esc;
		 p = wcstok(line, L" \t");
		 for (j = 0; j < MAXNLEIMA; j++) {
			if (!p) break;
			*p = towupper(*p);
			switch (*p) {
			   case L'J' :
				  tn = 10000;
				  break;
			   case L'P' :
				  tn = 10001;
				  break;
			   case L'V' :
				  tn = 10002;
				  break;
			   default:
				  tn = _wtoi(p);
				  break;
			   }
			if (tn == 0) break;
			if (tn <= MAXTUNNUS || tn > 9999) {
			   if (ir >= MAXNRASTI) {
				  swprintf(msg, L"\nRadalla %s liikaa rasteja (yli %d)",
					 rata_re[nrata_re].tunnus, MAXNRASTI);
				  Application->MessageBoxW(msg, L"Virhe", MB_OK);
				  goto esc;
				  }
			   rata_re[nrata_re].rastikoodi[ir] = tn;
			   ir++;
			   }
			p = wcstok(NULL, L" \t");
			}
		 }
	  rata_re[nrata_re].rastiluku = ir;
	  }
   swprintf(msg, L"Tiedostosta luettu %d rataa", nrata_re);
	   RataItems();
   Application->MessageBoxW(msg, L"Valmis", MB_OK);
   delete rata_file;
   return;
   esc:
   swprintf(msg,L"Virhe tiedostoa luettaessa. Rata %s",
	  rata_re[nrata_re].tunnus);
   Application->MessageBoxW(msg, L"Virhe", MB_OK);
   delete rata_file;
   }

void TFormRadat::RataItems(void)
	{
	CBRadat->Clear();
	for (int i = 0; i < nrata_re; i++) {
		CBRadat->Items->Add(rata_re[i].tunnus);
		}
	SGRata->RowCount = 2;
	SGRata->Cells[0][1] = L"";
	SGRata->Cells[1][1] = L"";
	SGRata->Cells[2][1] = L"";
	if (nrata_re) {
		CBRadat->ItemIndex = 0;
		nayta_rata(0);
		}
	}

void TFormRadat::kirj_radat(void)
   {
   TextFl *rata_file;
   int i, j, nr;
   wchar_t line[300];

   if (!rata_re[0].tunnus[0]) {
	   Application->MessageBoxW(L"Ei ratoja", L"Virhe", MB_OK);
	   return;
	   }
   if (ToimintaTila > 0)
		SaveDialog1->InitialDir = FormMain->CurrentDir;
	SaveDialog1->FileName = UnicodeString(RataFName);
	SaveDialog1->DefaultExt = L"lst";
	SaveDialog1->Filter = L"LST-tiedostot (*.lst)|*.LST|Kaikki tiedostot (*.*)|*.*";
	if (!SaveDialog1->Execute())
		return;
	rata_file = new TextFl(SaveDialog1->FileName.c_str(),L"w");
	if (!rata_file->IsOpen()) {
		delete rata_file;
		return;
		}
   for (j = 0; j < nrata_re; j++) {
	  if (!rata_re[j].tunnus[0])
		  continue;
	  swprintf(line, L"%-12s %d %d\n", rata_re[j].tunnus,
		 rata_re[j].ennakko, rata_re[j].maalilaji);
	  rata_file->WriteLine(line);
	  line[0] = 0;
	  nr = 40;
	  if (rata_re[j].rastiluku > 40) {
		 nr = 2 * ((rata_re[j].rastiluku + 1) / 2);
		 }
	  for (i = 0; i < nr; i++) {
		 if (rata_re[j].rastikoodi[i] > 9999) {
			wcscat(line, L"    ");
			line[wcslen(line)-2] = ckdi[rata_re[j].rastikoodi[i]-10000];
			}
		 else
			swprintf(line+wcslen(line), L"%3d ", rata_re[j].rastikoodi[i]);
		 if (i == nr/2-1 || i == nr-1) {
			line[wcslen(line)-1] = 0;
			line[wcslen(line)] = L'\n';
			rata_file->WriteLine(line);
			line[0] = 0;
			}
		 }
	  }
   delete rata_file;
   }

static void prtstr(PRFILE *lstf, INT32 xpos, INT32 ypos, wchar_t *st, int lf)
	{
	wchar_t buf[100];
	INT32 GDIhScale = 495, GDIvScale = 283;

	xpos = (GDIhScale * (xpos+marg_re)) / 1000;
	ypos = (GDIvScale * ypos) / 1000;
	putwfldGDI(st, xpos, 0, ypos, lstf);
	}

static void prtpiste(PRFILE *lstf, int x, int y, int n)
   {
	prtstr(lstf, 297 - 30*x, 680 - 30*y + n*step, L"O", 1);
   }

static void prtselitys(PRFILE *lstf, int r, int x, int y, int n)
   {
	wchar_t buf[30];

   swprintf(buf, L"%d", r);
   prtstr(lstf, 600 - 60*x + marg2 - 12 * (r > 9), 900 - 60*y + n*step, buf, 1);
   }

static void prtlisaselitys(PRFILE *lstf, wchar_t *msg, int r, int n) {
	prtstr(lstf, 400, 930 + n*step + 40*r, msg, 1);
	}

static void prtkehys(PRFILE *lstf, int n)
   {
   int i;

   for (i = 0; i <= 22; i++) {
	  prtstr(lstf, 300 - 15*i, 135 + n * step, L"-", 1);
	  }
   for (i = 2; i <= 20; i++) {
	  prtstr(lstf, 300 - 15*i, 805 + n * step, L"-", 1);
	  }
   for (i = 1; i <= 5; i++) {
	  prtstr(lstf, 300 - 60*i, 195 + n * step, L"-", 0);
	  prtstr(lstf, 300 - 60*i, 675 + n * step, L"-", 1);
	  }
   for (i = -2; i <= 17; i++) {
	  prtstr(lstf, -40, 675 - 30*i + n * step, L"|", 0);
	  prtstr(lstf, 310, 675 - 30*i + n * step, L"|", 1);
	  }
   prtstr(lstf, -25, 790 + n * step, L"\\", 0);
   prtstr(lstf, 300, 790 + n * step, L"/", 1);
   for (i = 1; i <= 10; i++) {
	  prtstr(lstf, 600 - 60*i + marg2, -60 + n * step, L"-", 0);
	  prtstr(lstf, 600 - 60*i + marg2, 900 + n * step, L"-", 1);
	  }
   for (i = 0; i <= 32; i++) {
	  prtstr(lstf, -60 + marg2, 900 - 30*i + n * step, L"|", 0);
	  prtstr(lstf, 600 + marg2, 900 - 30*i + n * step, L"|", 1);
	  }
   }

static void t_initpiikit(void)
   {
   memset(FormRadat->t_piikit, 0, sizeof(FormRadat->t_piikit));
   }

void TFormRadat::t_prt(tulostusparamtp *tulprm)
   {
	wchar_t buf[100];
   int x, y;

   putfld(tulprm, L"      -------------------------------", 0, 60, 0, 0);
   endline(tulprm, 0);
   putfld(tulprm, L"     /                               \\", 0, 60, 0, 0);
   endline(tulprm, 0);
   for (y = 15; y > 0; y--) {
	  wcscpy(buf, L"     | ");
	  for (x = 10; x > 0; x--) {
		 if ((y == 6 || y == 8) && x >= 5 && x <= 6) {
			if (x == 5)
			   wcscat(buf, L"-  ");
			else
			   wcscat(buf, L"  -");
			}
		 else if (y == 7 && x >= 5 && x <= 6)
			wcscat(buf, L" | ");
		 else if (t_piikit[x][y] < 0)
			wcscat(buf, L" 0 ");
		 else if (t_piikit[x][y] == 0)
			wcscat(buf, L" . ");
			else {
			swprintf(buf+wcslen(buf), L"%2d ", t_piikit[x][y]);
			}
		 }
	  wcscat(buf, L"|");
	  putfld(tulprm, buf, 0, 80, 0, 0);
	  endline(tulprm, 0);
	  }
   putfld(tulprm, L"      -------------------------------", 0, 60, 0, 0);
   endline(tulprm, 0);
   }

static void t_prtselitys(int r, int x, int y)
   {
   FormRadat->t_piikit[x][y] = r;
   }

void TFormRadat::prtpiikit(tulostusparamtp *tulprm, int srj, int *n, int graaf)
   {
   int i, j, k = 0, m, kdi[MAXRINNKOODI], r, onkoodi;
   ratatp *prata;
   wchar_t msg[80], buf[30];

   if (*n == 0)
		initpage(tulprm,L"C");
   prata = rata_re + srj;
   if (graaf) {
	   swprintf(buf, L"%s", prata->tunnus);
	  prtstr(tulprm->lstf, 0, *n * step, buf, 1);
	  prtkehys(tulprm->lstf, *n);
	  prtpiste(tulprm->lstf, 1, 15, *n);
	  prtpiste(tulprm->lstf, 10, 15, *n);
	  prtselitys(tulprm->lstf, 0, 1, 15, *n);
	  prtselitys(tulprm->lstf, 0, 10, 15, *n);
	  }
   else {
	   swprintf(buf, L"%s", prata->tunnus);
	   putfld(tulprm, buf, 0, 80, 0, 0);
	   endline(tulprm, 0);
	  t_initpiikit();
	  t_prtselitys(-1, 1, 15);
	  t_prtselitys(-1, 10, 15);
	  }
   for (i = 0; i < prata->rastiluku; i++) {
	  onkoodi = 0;
	  r = prata->rastikoodi[i];
	  if (r >= 900) continue;
	  k++;
	  memset(kdi, 0, sizeof(kdi));
	  if (!nrasti) kdi[0] = r;
	  else {
		 for (j = 0; j < nrasti; j++) {
			if (koodit[j].rasti == r) {
			   for (m = 0; m < MAXRINNKOODI; m++)
				  if (!kdi[m]) break;
			   if (m < MAXRINNKOODI)
				  kdi[m] = koodit[j].koodi;
			   }
			}
		 }
	  for (j = 0; j < MAXRINNKOODI; j++) {
		 if (kdi[j] > 0 && kdi[j] <= (int) (sizeof(piikit)/sizeof(piikkitp)) &&
			piikit[kdi[j]-1].x) {
			onkoodi = 1;
			if (graaf) {
			   prtpiste(tulprm->lstf, piikit[kdi[j]-1].x, piikit[kdi[j]-1].y, *n);
			   prtselitys(tulprm->lstf, k, piikit[kdi[j]-1].x, piikit[kdi[j]-1].y, *n);
			   }
			else
			   t_prtselitys(k, piikit[kdi[j]-1].x, piikit[kdi[j]-1].y);
			}
		 }
	  if (!onkoodi) {
		 swprintf(msg, L"Radan %s rastin %d (tunnus %d) Emit-koodi puuttuu",
			prata->tunnus, k, r);
		 Application->MessageBoxW(msg, L"Virhe", MB_OK);
		 }
	  }
	if (tarkrata_re(srj, 0)) {
		r = 0;
		for (i = 1; i < 256; i++) {
			if (kdirastit[i][1]) {
				swprintf(msg, L"Koodi %d rasteilla %d", i, kdirastit[i][0]);
				for (j = 1; j < 8 && kdirastit[i][j]; j++) {
					swprintf(msg+wcslen(msg), L"%s %d", (j == 7 || !kdirastit[i][j+1]) ? L" ja" : L",", kdirastit[i][j]);
					}
				if (graaf) {
					prtlisaselitys(tulprm->lstf, msg, r++, *n);
					}
				else {
					putfld(tulprm, msg, 0, 80, 0, 0);
					endline(tulprm, 1);
					}
				}
			}
		}
   if (!graaf) {
	  t_prt(tulprm);
	  if (*n >= 2) {
		 endpage(tulprm);
		 *n = 0;
		 }
	  else {
		 newline(tulprm, 2);
		 ++*n;
		 }
	  }
   else {
	  if (*n) {
		 endpage(tulprm);
		 *n = 0;
		 }
	  else ++*n;
	  }
   }

void TFormRadat::tulostapiikit(tulostusparamtp *tulprm)
   {
   int n = 0, graaf = 0;
   wchar_t ch = L'G';

   if (!nrasti) {
	  Application->MessageBoxW(L"Rastien Emit-koodim‰‰rittelyt (tiedosto LEIMAT.LST) puuttuvat."
		 L"Emit-koodien oletetaan olevan samat kuin rastitunnukset.", L"Varoitus", MB_OK);
	  }
   if (ch == L'G') {
	  graaf = 1;
//	  inputint_prompt(&vsc, L"Anna tulostuskoko suhteessa normaaliin. Vaaka %", &ch);
//	  inputint_prompt(&psc, L"Anna tulostuskoko suhteessa normaaliin. Pysty %", &ch);
	  }
   for (int i = 0; i < nrata_re; i++) {
	 if (FormRadat->RataValittu(i))
		  prtpiikit(tulprm, i, &n, graaf);
	  }
   enddoc(tulprm);
   Application->MessageBoxW(L"Pyydetyt radat tulostettu", L"Valmis", MB_OK);
   }

void TFormRadat::tulostakoodit(tulostusparamtp *tulprm)
   {
   int i, j, k = 0, n = 0;
   wchar_t kdi[MAXRINNKOODI][4];
	wchar_t buf[80];

	initpage(tulprm, L"C");
   putfld(tulprm, L"RASTIT JA EMITKOODIT", 0, 60, 0, 0);
   endline(tulprm, 1);
   putfld(tulprm, L"   Rastitunnus   Emitkoodit", 0, 60, 0, 0);
   endline(tulprm, 0);
   memset(kdi, 0, sizeof(kdi));
   i = koodit[0].rasti;
   _itow(koodit[0].koodi, kdi[0], 10);
   for (j = 1; j <= nrasti; j++) {
	  if (koodit[j].rasti == i) {
		 k++;
		 }
	  else {
		 swprintf(buf, L"         %3d      %3.3s  %3.3s  %3.3s  %3.3s  %3.3s\r\n",
			i, kdi[0], kdi[1], kdi[2], kdi[3], kdi[4]);
		 putfld(tulprm, buf, 0, 80, 0, 0);
		 endline(tulprm, 0);
		 i = koodit[j].rasti;
		 k = 0;
		 memset(kdi, 0, sizeof(kdi));
		 if (!mahtuusivulle(n, 2, tulprm)) {
			endpage(tulprm);
			initpage(tulprm, L"C");
			putfld(tulprm, L"RASTIT JA EMITKOODIT", 0, 60, 0, 0);
			endline(tulprm, 1);
			putfld(tulprm, L"   Rastitunnus   Emitkoodit", 0, 60, 0, 0);
			endline(tulprm, 0);
			n = 0;
			}
		 else
			n++;
		 }
	  if (j < nrasti && k < MAXRINNKOODI)
		 _itow(koodit[j].koodi, kdi[k], 10);
	  }
   enddoc(tulprm);
   Application->MessageBoxW(L"Koodit tulostettu", L"Valmis", MB_OK);
   }

void TFormRadat::tulostaleimasinkoodit(tulostusparamtp *tulprm)
   {
   int ir, ie, j, k = 0, n = 0;
   int kdi[225][10];
	wchar_t buf[200];

	initpage(tulprm, L"C");
   putfld(tulprm, L"EMITKOODIEN KƒYTT÷", 0, 60, 0, 0);
   endline(tulprm, 1);
   putfld(tulprm, L"Emitkoodi  Rastitunnukset", 0, 60, 0, 0);
   endline(tulprm, 0);
   memset(kdi, 0, sizeof(kdi));
   ir = koodit[0].rasti;
   ie = koodit[0].koodi;
   for (j = 1; j <= nrasti; j++) {
	  if (ie > 30 && ie < 256) {
		 for (k = 0; k < 10 && kdi[ie-31][k]; k++) ;
		 if (k < 10)
			kdi[ie-31][k] = ir;
		 }
	  ir = koodit[j].rasti;
	  if (ie != koodit[j].koodi) {
		 ie = koodit[j].koodi;
//		 k = 0;
		 }
	  }
   for (j = 0; j < 225; j++) {
	  if (!kdi[j][0])
		 continue;
	  swprintf(buf, L"   %3d   ", j+31);
	  for (k = 0; k < 10 && kdi[j][k]; k++) {
		 swprintf(buf+wcslen(buf), L"  %3d", kdi[j][k]);
		 }
	  putfld(tulprm, buf, 0, 80, 0, 0);
	  endline(tulprm, 0);
	  if (!mahtuusivulle(n, 2, tulprm)) {
			endpage(tulprm);
			initpage(tulprm, L"C");
			putfld(tulprm, L"EMITKOODIEN KƒYTT÷", 0, 60, 0, 0);
			endline(tulprm, 1);
			putfld(tulprm, L"Emitkoodi  Rastitunnukset", 0, 60, 0, 0);
			endline(tulprm, 0);
			n = 0;
			}
	  else
		 n++;
	  }
   if (n) {
	  endpage(tulprm);
	  }
   enddoc(tulprm);
   Application->MessageBoxW(L"Koodit tulostettu", L"Valmis", MB_OK);
   }

void TFormRadat::prtrata(tulostusparamtp *tulprm, int srj, int *n)
   {
	int i, j, m, r;
	ratatp *prata;
	wchar_t kdi[MAXRINNKOODI][4];
	wchar_t msg[80], buf[80];
	int rr, pkdi;
	int partiokoodit[100];

	memset(partiokoodit, 0, sizeof(partiokoodit));
	prata = rata_re + srj;
	if (*n && !mahtuusivulle(*n, prata->rastiluku+6, tulprm)) {
		endpage(tulprm);
		initpage(tulprm, L"C");
		*n = 0;
		}
	newline(tulprm, 2);
	swprintf(buf, L"   %s", prata->tunnus);
	putfld(tulprm, buf, 0, 20, 0, 0);
	endline(tulprm, 1);
	putfld(tulprm, L"      Rastitunnus   Emitkoodit", 0, 60, 0, 0);
	endline(tulprm, 0);
	*n += 5;
	rr = 0;
	for (int i = 0; i < prata->rastiluku; i++) {
		if (prata->rastikoodi[i] >= 10000 && prata->rastikoodi[i] <= 10002)
		partiokoodit[rr++] = prata->rastikoodi[i];
		}
	partiokoodit[rr] = 10000;
	rr = 1;
	pkdi = 1;
	for (i = 0; i < prata->rastiluku; i++) {
	  r = prata->rastikoodi[i];
	  if (r >= 900 && r <= 9999) continue;
	  if (r > 9999) {
		 switch (partiokoodit[pkdi++]) {
			case 10000 :
			   putfld(tulprm, L"Pakollisessa j‰rjestyksess‰ juostavia", 0, 80, 0, 0);
			   endline(tulprm, 0);
			   break;
			case 10001 :
			   putfld(tulprm, L"Pakollisia, mutta vapaassa j‰rjestyksess‰ juostavia", 0, 80, 0, 0);
			   endline(tulprm, 0);
			   break;
			case 10002 :
			   putfld(tulprm, L"Vapaassa j‰rjestyksess‰ juostavia, partiokisassa valinnaisia", 0, 80, 0, 0);
			   endline(tulprm, 0);
			   break;
			}
		 ++*n;
		 }
	  else {
//		 k++;
		 memset(kdi, 0, sizeof(kdi));
		 if (nrasti) {
			for (j = 0; j < nrasti; j++) {
			   if (koodit[j].rasti == r) {
				  for (m = 0; m < MAXRINNKOODI; m++)
					 if (!kdi[m][0]) break;
				  if (m < MAXRINNKOODI)
					 _itow(koodit[j].koodi, kdi[m], 10);
				  }
			   }
			}
		 swprintf(buf, L"  %3d    %3d      %3.3s  %3.3s  %3.3s  %3.3s  %3.3s",
			i+1, r, kdi[0], kdi[1], kdi[2], kdi[3], kdi[4]);
		 putfld(tulprm, buf, 0, 80, 0, 0);
		 endline(tulprm, 0);
		 }
	  if (!mahtuusivulle(++*n, 2, tulprm)) {
		 endpage(tulprm);
		 initpage(tulprm, L"C");
		 *n = 0;
		 }
	  }
   }

void TFormRadat::tulostaradat(tulostusparamtp *tulprm)
   {
   int n = 0;

   initpage(tulprm, L"C");
   for (int i = 0; i < nrata_re; i++) {
	  if (FormRadat->RataValittu(i))
		 prtrata(tulprm, i, &n);
	  }
   enddoc(tulprm);
   Application->MessageBoxW(L"Pyydetyt radat tulostettu", L"Valmis", MB_OK);
   }

int TFormRadat::tarkrata_re(int k, int varoita)
   {
   int i, ii, j, r, rr, s, er, ret = 0;
   wchar_t snimi[12];
   int rst[8], llst[251];
   wchar_t msg[80], st[12];

	memset(kdirastit, 0, sizeof(kdirastit));
	memset(llst, 0, sizeof(llst));
   wcscpy(snimi, rata_re[k].tunnus);

	for (i = 0; i < rata_re[k].rastiluku; i++) {
		if (rata_re[k].rastikoodi[i] == 0 || rata_re[k].rastikoodi[i] > 9999)
			continue;

		rr = 0;
		for (r = 0; r < nrasti; r++) {
			if (koodit[r].rasti == rata_re[k].rastikoodi[i]) {
				if (koodit[r].koodi > 0 && koodit[r].koodi < 256) {
					for (j = 0; j < 8; j++) {
						if (kdirastit[koodit[r].koodi][j] == 0) {
							kdirastit[koodit[r].koodi][j] = i+1;
							if (j > 0)
								ret = 1;
							break;
							}
						}
					}
				else if (koodit[r].koodi == 0) {
					varoita = 1;
					koodit[r].koodi = rata_re[k].rastikoodi[i];
					swprintf(msg, L"VAROITUS: Leimasintietoihin lis‰tty rastille %d koodi %d", koodit[r].rasti, koodit[r].koodi);
					FormRadat->Memo1->Lines->Add(msg);
					}
				rr = r+1;
				}
			}
		if (rr == 0) {
			varoita = 1;
			koodit[nrasti].rasti = rata_re[k].rastikoodi[i];
			koodit[nrasti].koodi = rata_re[k].rastikoodi[i];
			swprintf(msg, L"VAROITUS: Leimasintietoihin lis‰tty rastille %d koodi %d", koodit[nrasti].rasti, koodit[nrasti].koodi);
			FormRadat->Memo1->Lines->Add(msg);
			nrasti++;
			sortkoodit();
			}
		}
	if (!varoita)
		return(ret);
	nayta_leimat();
	for (i = 1; i < 256; i++) {
		memset(rst, 0, sizeof(rst));
		if (kdirastit[i][1]) {
			er = 0;
			ii = 0;
			for (j = 1; j < 8 && kdirastit[i][j]; j++) {
				if (kdirastit[i][j] == kdirastit[i][j-1]+1)
					er = 1;
				if (j == 7 && rata_re[k].maalilaji == 4)
					er += 2;
				ii = j;
				}
			if (er) {
				if (er > 1)
					wcscpy(st, L"maalirasti");
				else
					_itow(kdirastit[i][ii], st, 10);
				swprintf(msg, L"VIRHE: Leimasin %d radalla %s rasteilla %d", i, snimi, kdirastit[i][0]);
				for (j = 1; j < ii; j++) {
					swprintf(msg+wcslen(msg), L", %d", kdirastit[i][j]);
					}
				swprintf(msg+wcslen(msg), L" ja %s", st);
				FormRadat->Memo1->Lines->Add(msg);
				}
			else {
				swprintf(msg, L"VAROITUS: Leimasin %d radalla %s rasteilla %d", i, snimi, kdirastit[i][0]);
				for (j = 1; j <= ii; j++) {
					wsprintf(msg+wcslen(msg), L"%s %d", (j == ii) ? L" ja" : L",", kdirastit[i][j]);
					}
				FormRadat->Memo1->Lines->Add(msg);
				}
			}
		}
   return(ret);
   }

int TFormRadat::tarkradat(void)
   {
   int k, ret = 0;

   if (!nrasti)
	  return(0);
   for (k = 0; k < nrata_re; k++) {
		if (tarkrata_re(k, 1))
			ret = 1;
		}
   return(ret);
   }

int TFormRadat::tarksarjat(void)
   {
   wchar_t msg[100];
   int k, ret = 0;

   for (int r = 1; r < SGSarjat->RowCount; r++) {
	   if (ToimintaTila > 0) {
			if (SGSarjat->Cells[0][r].Length() > 0 &&
				haesarja_w(SGSarjat->Cells[0][r].c_str(), false) < 0) {
				FormRadat->Memo1->Lines->Add(L"VIRHE: Sarjaa "+SGSarjat->Cells[0][r]+L" ei ole");
				ret = 1;
				}
			}
	   if (SGSarjat->Cells[1][r].Length() > 0) {
		   for (k = 0; k < nrata_re; k++)
				if (!wcscmpU(SGSarjat->Cells[1][r].c_str(), rata_re[k].tunnus))
					break;
		   if (k >= nrata_re) {
				FormRadat->Memo1->Lines->Add(L"VIRHE: Sarjan "+SGSarjat->Cells[0][r]+L" rataa "+SGSarjat->Cells[1][r]+L" ei ole");
				ret = 1;
				}
		   }
	   }
   return(0);
   }

bool TFormRadat::haerata_re(wchar_t *rata)
{
	for (int k = 0; k < nrata_re; k++)
		if (!wcscmpU(rata, rata_re[k].tunnus))
			return(true);
	return(false);
}

int TFormRadat::tarkkilpailijat(void)
{
	kilptietue kilp;
	wchar_t msg[100];
#ifndef MAXOSUUSLUKU
	for (int d = 1; d < nrec; d++) {
		kilp.GETREC(d);
		if (!kilp.kilpstatus && wcswcind(kilp.tark(k_pv), L"PVE") < 0 &&
			Sarjat[kilp.Sarja()].luonne[k_pv] >= 0) {
//			k_pv >= Sarjat[kilp.Sarja()].enspv && k_pv <= Sarjat[kilp.Sarja()].viimpv) {
			if (!haerata_re(Sarjat[kilp.Sarja()].rata[k_pv]) && !haerata_re(Sarjat[kilp.Sarja()].sarjanimi) &&
				!haerata_re(kilp.pv[k_pv].rata)) {
				swprintf(msg, L"VIRHE: Kilpailijan %d sarjaa %s tai rataa %s ei radoissa",
					kilp.id(), Sarjat[kilp.Sarja()].sarjanimi, kilp.pv[k_pv].rata);
				FormRadat->Memo1->Lines->Add(msg);
				}
			}
		}
#endif
	return(0);
}

//---------------------------------------------------------------------------
void TFormRadat::nayta_rata(int r)
{
	int rr, vast, pkdi;
	int partiokoodit[100];

	memset(partiokoodit, 0, sizeof(partiokoodit));
	if (r >= 0) {
		trata = rata_re[kRata];
		TallRata(&trata, true);
		if (kRata != r && trata.rastiluku > 0 && memcmp(&trata, rata_re+kRata, sizeof(trata))) {
			vast = select3(3, L"K‰sitelt‰v‰ rata muuttunut, valitse menettely",
				L"Muutoksia", L"S‰ilyt‰ muutokset muistiin", L"Hylk‰‰ muutokset",
				L"Palaa k‰sitelt‰v‰‰n rataan", this);
			switch(vast) {
				case 1:
					rata_re[kRata] = trata;
					rataMuutoksia = true;
					break;
				case 2:
					break;
				case 3:
					CBRadat->ItemIndex = kRata;
					return;
				}
			}
		kRata = r;
		trata = rata_re[kRata];
		EdtLahto->Text = rata_re[r].lahto;
		EdtMaali->Text = rata_re[r].maali;
		EdtRNimi->Text = rata_re[r].tunnus;
		EdtRatapit->Text = rata_re[r].ratapit;
		EdtNousu->Text = rata_re[r].nousu;
		if (rata_re[r].ennakko < 0) {
			CBLahto->ItemIndex = 0;
			EdtEnnakko->Text = UnicodeString(-1);
			EdtEnnakko->Visible = false;
			Label4->Visible = false;
			}
		else if (rata_re[r].ennakko == 0) {
			CBLahto->ItemIndex = 1;
			EdtEnnakko->Text = UnicodeString(0);
			EdtEnnakko->Visible = false;
			Label4->Visible = false;
			}
		else {
			CBLahto->ItemIndex = 2;
			EdtEnnakko->Text = UnicodeString(rata_re[r].ennakko);
			EdtEnnakko->Visible = true;
			Label4->Visible = true;
			}
		if (rata_re[r].maalilaji == 1) {
			CBMaali->ItemIndex = 1;
			CBAutoTulos->Visible = false;
			CBAutoTulos->Checked = false;
			EdtMatkaMaaliin->Text = rata_re[r].viitmatka;
			EdtMatkaMaaliin->Visible = true;
			}
		else if (rata_re[r].maalilaji == 2) {
			CBMaali->ItemIndex = 0;
			CBAutoTulos->Checked = false;
			CBAutoTulos->Visible = true;
			EdtMatkaMaaliin->Visible = false;
			}
		else {
			CBMaali->ItemIndex = 0;
			CBAutoTulos->Checked = true;
			CBAutoTulos->Visible = true;
			EdtMatkaMaaliin->Visible = false;
			}
		LblMatkaMaaliin->Visible = EdtMatkaMaaliin->Visible;
		SGRata->RowCount = rata_re[r].rastiluku + 2;
		rr = 0;
		for (int i = 0; i < rata_re[r].rastiluku; i++) {
			if (rata_re[r].rastikoodi[i] >= 10000 && rata_re[r].rastikoodi[i] <= 10002)
				partiokoodit[rr++] = rata_re[r].rastikoodi[i];
			}
		partiokoodit[rr] = 10000;
		rr = 1;
		pkdi = 1;
		for (int i = 0; i < rata_re[r].rastiluku; i++) {
			if (rata_re[r].rastikoodi[i] >= 10000 && rata_re[r].rastikoodi[i] <= 10002) {
				SGRata->Cells[0][i+1] = UnicodeString(L"");
				SGRata->Cells[1][i+1] = UnicodeString(ckdi[partiokoodit[pkdi++]-10000]);
				SGRata->Cells[2][i+1] = UnicodeString(L"");
				}
			else {
				SGRata->Cells[0][i+1] = UnicodeString(rr++);
				SGRata->Cells[1][i+1] = UnicodeString(rata_re[r].rastikoodi[i]);
				SGRata->Cells[2][i+1] = UnicodeString(rata_re[r].matka[i]);
				}
			}
		SGRata->Cells[0][rata_re[r].rastiluku+1] = UnicodeString(L"");
		SGRata->Cells[1][rata_re[r].rastiluku+1] = UnicodeString(L"");
		SGRata->Cells[2][rata_re[r].rastiluku+1] = UnicodeString(L"");
		}
	RGUNimi->Visible = false;
	RGUNimi->ItemIndex = -1;
}
//---------------------------------------------------------------------------
void __fastcall TFormRadat::CBRadatChange(TObject *Sender)
{
	int r;

	if ((r = CBRadat->ItemIndex) >= 0) {
		nayta_rata(r);
		LblVaroitus->Visible = CBRadat->Text.Length() > HAJONTA;
		}
}

//---------------------------------------------------------------------------
void __fastcall TFormRadat::BtnTarkistaClick(TObject *Sender)
{
	Memo1->Clear();
	tarkradat();
	tarkkoodit();
#ifndef MAXOSUUSLUKU
	tarksarjat();
	if (ToimintaTila == 2 && emitfl) {
		tarkkilpailijat();
		}
#endif
	PageControl1->ActivePageIndex = 5;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::SGLeimatClick(TObject *Sender)
{
	AktLRow = SGLeimat->Row;
	AktLCol = SGLeimat->Col;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::SGRataClick(TObject *Sender)
{
	AktRRow = SGRata->Row;
	AktRCol = SGRata->Col;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnRataTyhjennaClick(TObject *Sender)
{
	for (int r = 1; r < SGRata->RowCount; r++) {
		SGRata->Cells[0][r] = L"";
		SGRata->Cells[1][r] = L"";
		SGRata->Cells[2][r] = L"";
	}
	SGRata->RowCount = 2;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnRataLisaaRastiClick(TObject *Sender)
{
	if (AktRRow < 1)
		AktRRow = 1;
	SGRata->RowCount++;
	for (int r = SGRata->RowCount-1; r > AktRRow; r--) {
		SGRata->Cells[0][r-1] = UnicodeString(r-1);
		SGRata->Cells[1][r] = SGRata->Cells[1][r-1];
		SGRata->Cells[2][r] = SGRata->Cells[2][r-1];
		}
	SGRata->Cells[1][AktRRow] = L"";
	SGRata->Cells[2][AktRRow] = L"";
	SGRata->Row = AktRRow;
	SGRata->Col = 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnRataPoistaRastiClick(TObject *Sender)
{
	if (AktRRow < 1 || AktRRow >= SGRata->RowCount-1)
		return;
	for (int r = AktRRow; r < SGRata->RowCount-1; r++) {
		SGRata->Cells[0][r] = UnicodeString(r);
		SGRata->Cells[1][r] = SGRata->Cells[1][r+1];
		SGRata->Cells[2][r] = SGRata->Cells[2][r+1];
		}
	SGRata->Cells[0][SGRata->RowCount-1] = L"";
	SGRata->Cells[1][SGRata->RowCount-1] = L"";
	SGRata->Cells[2][SGRata->RowCount-1] = L"";
	SGRata->Row = AktRRow;
	SGRata->Col = 1;
	SGRata->RowCount--;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::TallRata(ratatp *rata, bool sailSF)
{
	int ir, tn, tn1 = 10000, ip;
	wchar_t lahto[6], maali[6];

	wcsncpy(lahto, rata->lahto, sizeof(lahto)/2-1);
	wcsncpy(maali, rata->maali, sizeof(maali)/2-1);
	memset(rata, 0, sizeof(ratatp));
	if (sailSF) {
		wcsncpy(rata->lahto, lahto, sizeof(rata->lahto)/2-1);
		wcsncpy(rata->maali, maali, sizeof(rata->maali)/2-1);
		}
	for (ir = 0; ir < MAXNRASTI && ir < SGRata->RowCount-1; ir++) {
		tn = _wtoi(SGRata->Cells[1][ir+1].c_str());
		if (tn == 0) {
			if (SGRata->Cells[1][ir+1].Length() > 0 &&
				(ip = wcswcind(towupper2(SGRata->Cells[1][ir+1].c_str()[0]), ckdi)) > -1) {
				tn = tn1;
				tn1 = 10000+ip;
				}
			else
				break;
			}
		rata->rastikoodi[ir] = tn;
		rata->matka[ir] = _wtoi(SGRata->Cells[2][ir+1].c_str());
		}
	rata->rastiluku = ir;
	wcsncpy(rata->tunnus, EdtRNimi->Text.c_str(), sizeof(rata_re[0].tunnus)/2-1);
	wcsncpy(rata->lahto, EdtLahto->Text.c_str(), sizeof(rata_re[0].lahto)/2-1);
	wcsncpy(rata->maali, EdtMaali->Text.c_str(), sizeof(rata_re[0].maali)/2-1);
	rata->ratapit = _wtoi(EdtRatapit->Text.c_str());
	rata->nousu = _wtoi(EdtNousu->Text.c_str());

	switch (CBLahto->ItemIndex) {
		case 0:
			rata->ennakko = -1;
			break;
		case 1:
			rata->ennakko = 0;
			break;
		case 2:
			rata->ennakko = _wtoi(EdtEnnakko->Text.c_str());
			break;
		default:
			rata->ennakko = -1;
			break;
		}
	switch (CBMaali->ItemIndex) {
		case 0:
			rata->maalilaji = CBAutoTulos->Checked ? 4 : 2;
			rata->viitmatka = 0;
			break;
		case 1:
			rata->maalilaji = 1;
			rata->viitmatka = _wtoi(EdtMatkaMaaliin->Text.c_str());
			break;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnTallRataClick(TObject *Sender)
{
	int krata;

	if (!RGUNimi->Visible || RGUNimi->ItemIndex < 1) {
		krata = CBRadat->ItemIndex;
		}
	else {
		if (nrata_re > MAXRATA) {
			Application->MessageBoxW(L"Ratojen maksimim‰‰r‰ t‰ynn‰", L"Este", MB_OK);
			return;
			}
		krata = nrata_re;
		nrata_re++;
		}
	TallRata(rata_re+krata, true);
	CBRadat->Clear();
	for (int ir = 0; ir < nrata_re; ir++)
		CBRadat->Items->Add(rata_re[ir].tunnus);
	CBRadat->ItemIndex = krata;
	kRata = krata;
	nayta_rata(krata);
	rataMuutoksia = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::EdtRNimiChange(TObject *Sender)
{
	RGUNimi->Visible = (EdtRNimi->Text != CBRadat->SelText);
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnSuljeClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnTallKooditClick(TObject *Sender)
{
	int tn, kdi;

	memset(koodit, 0, sizeof(koodit));
	nrasti = 0;
	for (int r = 1; r < SGLeimat->RowCount; r++) {
		if ((tn = _wtoi(SGLeimat->Cells[0][r].c_str())) > 0) {
			for (int c = 1; c < SGLeimat->ColCount; c++) {
				if ((kdi = _wtoi(SGLeimat->Cells[c][r].c_str())) > 0) {
					koodit[nrasti].rasti = tn;
#ifdef MAXOSUUSLUKU
					if (towupper(SGLeimat->Cells[c][r].c_str()[SGLeimat->Cells[c][r].Length()-1]) == L'P') {
						if (nrasti == 0 || koodit[nrasti-1].rasti != tn) {
							koodit[nrasti].koodi = tn;
							nrasti++;
							}
						koodit[nrasti-1].pisteet = kdi;
						}
					else
#endif
						{
						koodit[nrasti].koodi = kdi;
						nrasti++;
						}
					}
				}
			}
		}
	nayta_leimat();
	rataMuutoksia = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnLeimatLisaaRastiClick(TObject *Sender)
{
	if (AktLRow < 1)
		AktLRow = 1;
	SGLeimat->RowCount++;
	for (int r = SGLeimat->RowCount-1; r > AktLRow; r--) {
		for (int c = 0; c < SGLeimat->ColCount; c++) {
			SGLeimat->Cells[c][r] = SGLeimat->Cells[c][r-1];
			}
		}
	for (int c = 0; c < SGLeimat->ColCount; c++)
		SGLeimat->Cells[c][AktLRow] = L"";
	SGLeimat->Row = AktLRow;
	SGLeimat->Col = 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnLeimatPoistaRastiClick(TObject *Sender)
{
	if (AktLRow < 1)
		return;
	for (int r = AktLRow; r < SGLeimat->RowCount-1; r++) {
		for (int c = 0; c < SGLeimat->ColCount; c++) {
			if (SGLeimat->Cells[c][r+1] == NULL)
				SGLeimat->Cells[c][r] = L"";
			else
				SGLeimat->Cells[c][r] = SGLeimat->Cells[c][r+1];
			}
		}
	SGLeimat->Row = AktLRow;
	SGLeimat->RowCount--;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnTyhjKooditClick(TObject *Sender)
{
	memset(koodit, 0, sizeof(koodit));
	nrasti = 0;
	n_erirasti = 0;
	nayta_leimat();
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnPeruutaKoodiClick(TObject *Sender)
{
	nayta_leimat();
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnPoistaRataClick(TObject *Sender)
{
	int krata;
	for (krata = CBRadat->ItemIndex; krata < nrata_re; krata++) {
		memcpy(rata_re+krata, rata_re+krata+1, sizeof(ratatp));
		}
	nrata_re--;
	memset(rata_re+nrata_re, 0, sizeof(ratatp));
	krata = CBRadat->ItemIndex;
	if (krata >= nrata_re)
		krata = nrata_re - 1;
	CBRadat->Clear();
	for (int ir = 0; ir < nrata_re; ir++)
		CBRadat->Items->Add(rata_re[ir].tunnus);
	CBRadat->ItemIndex = krata;
	nayta_rata(krata);
	rataMuutoksia = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::CBTiedLajiChange(TObject *Sender)
{
	switch (CBTiedLaji->ItemIndex) {
		case 0:
			GBXML->Visible = true;
			RGXMLrata->Visible = true;
			GBcp->Visible = false;
			GBOcad8->Visible = false;
			break;
		case 1:
			GBXML->Visible = true;
			RGXMLrata->Visible = false;
			RGXMLrata->ItemIndex = 0;
			GBcp->Visible = false;
			GBOcad8->Visible = false;
			break;
		case 2:
			GBXML->Visible = false;
			GBcp->Visible = false;
			GBOcad8->Visible = true;
			break;
		case 3:
			GBXML->Visible = false;
			GBcp->Visible = false;
			GBOcad8->Visible = false;
			break;
		case 4:
			GBXML->Visible = false;
			GBcp->Visible = true;
			GBOcad8->Visible = false;
			break;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnImportClick(TObject *Sender)
{
	importradat();
	RataItems();
	nayta_leimat();
	nayta_koordinaatit();
	RGlisaa->Enabled = xmlLuettu;
	EdtRataLkm->Text = nrata_re;
	EdtRastiLkm->Text = nrasti;
	RGlisaa->ItemIndex = RGlisaa->Enabled ? 2 : 1;
	rataMuutoksia = true;
	if (kRata >= 0 && kRata < nrata_re)
		trata = rata_re[kRata];
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::TabSheet1Show(TObject *Sender)
{
	TulosteForm->enumPrinters(KirjoitinVal);
	NaytaRatavalinnat();
}
//---------------------------------------------------------------------------
int TFormRadat::haerata(wchar_t *ratanimi)
{
	int krata;

	for (krata = 0; krata < nrata_re; krata++)
		if (!wcscmpU(ratanimi, rata_re[krata].tunnus))
			return(krata);
	return(-1);
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnLisKaikkiClick(TObject *Sender)
{
	for (int krata = 0; krata < nrata_re; krata++)
		RataValinnat[krata] = true;
	NaytaRatavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnLisValitutClick(TObject *Sender)
{
	for (int rv = 0; rv < LBRadat->Items->Count; rv++) {
		if (LBRadat->Selected[rv]) {
			int krata;
			if ((krata = haerata(LBRadat->Items->Strings[rv].c_str())) >= 0)
				RataValinnat[krata] = true;
			}
		}
	NaytaRatavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnPoisValitutClick(TObject *Sender)
{
	for (int rv = 0; rv < LBRataVal->Items->Count; rv++) {
		if (LBRataVal->Selected[rv]) {
			int krata;
			if ((krata = haerata(LBRataVal->Items->Strings[rv].c_str())) >= 0)
				RataValinnat[krata] = false;
			}
		}
	NaytaRatavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnPoisKaikkiClick(TObject *Sender)
{
	for (int krata = 0; krata < nrata_re; krata++)
		RataValinnat[krata] = false;
	NaytaRatavalinnat();
}
//---------------------------------------------------------------------------
bool TFormRadat::RataValittu(int krata)
{
	for (int rv = 0; rv < LBRataVal->Items->Count; rv++) {
		if (haerata(LBRataVal->Items->Strings[rv].c_str()) == krata)
			return(true);
		}
	return(false);
}
//---------------------------------------------------------------------------
void TFormRadat::NaytaRatavalinnat(void)
{
	LBRadat->Clear();
	LBRataVal->Clear();
	for (int krata = 0; krata < nrata_re; krata++) {
		if (RataValinnat[krata])
			LBRataVal->Items->Add(UnicodeString(rata_re[krata].tunnus));
		else
			LBRadat->Items->Add(UnicodeString(rata_re[krata].tunnus));
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::LBRadatDblClick(TObject *Sender)
{
	int krata;
	if ((krata = haerata(LBRadat->Items->Strings[LBRadat->ItemIndex].c_str())) >= 0)
		RataValinnat[krata] = true;
	NaytaRatavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::LBRataValDblClick(TObject *Sender)
{
	int krata;
	if ((krata = haerata(LBRataVal->Items->Strings[LBRataVal->ItemIndex].c_str())) >= 0)
		RataValinnat[krata] = false;
	NaytaRatavalinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnTulostaClick(TObject *Sender)
{
tulostusparamtp tulprm;

	tulprm.printer = GDIPRINTER;
	if (!tulprm.lstf) {
		if ((tulprm.lstf = openprfile(KirjoitinVal->Items->Strings[KirjoitinVal->ItemIndex].c_str(),
			1, TRUE, FALSE, 0, FALSE))
			== NULL) {
			Application->MessageBoxW(L"Kirjoittimen k‰yttˆ ei onnistu", L"Virhe", MB_OK);
			return;
			}
		if (tulprm.lstf && tulprm.lstf->prno == 2)
			initgdiprinter(&tulprm);
		}
	switch (CBTuloste->ItemIndex) {
		case 0:
			tulostapiikit(&tulprm);
			break;
		case 1:
			tulostaradat(&tulprm);
			break;
		case 2:
			tulostakoodit(&tulprm);
			break;
		case 3:
			tulostaleimasinkoodit(&tulprm);
			break;
		}
	closeprfile(tulprm.lstf);
	tulprm.lstf = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnOtaKayttoonClick(TObject *Sender)
{
	if (Application->MessageBoxW(L"Tiedot tallennetaan tiedostoon ja sitten luetaan samannimisest‰ tiedostosta kuin tulospalvelua k‰ynnistett‰ess‰. "
		L"Virheelliset tiedot voivat aiheuttaa h‰iriˆit‰.",
		L"Vahvista tietojen vaihto", MB_YESNO) == IDYES) {
		kirjXmlTiedot(false, false);
		lueRatatiedotUudelleen();
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::FormResize(TObject *Sender)
{
	PageControl1->Height = ClientHeight - 25 * Screen->PixelsPerInch / 96;
	SGRata->Height = TabSheetRadat->ClientHeight - SGRata->Top - 6 * Screen->PixelsPerInch / 96;
	SGLeimat->Height = TabSheetRadat->ClientHeight - SGLeimat->Top - 6 * Screen->PixelsPerInch / 96;
	LBRadat->Height = TabSheetRadat->ClientHeight - LBRadat->Top - 6 * Screen->PixelsPerInch / 96;
	LBRataVal->Height = TabSheetRadat->ClientHeight - LBRataVal->Top - 6 * Screen->PixelsPerInch / 96;
	Memo1->Height = TabSheetRadat->ClientHeight - Memo1->Top - 6 * Screen->PixelsPerInch / 96;
	SGSarjat->Height = TabSheetSarjat->ClientHeight - SGSarjat->Top - 6 * Screen->PixelsPerInch / 96;
	SGKoord->Height = TabSheetKoord->ClientHeight - SGKoord->Top - 6 * Screen->PixelsPerInch / 96;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Ratatiedot");
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::FormShow(TObject *Sender)
{
	if (!initialisoitu) {
		if (emitfl) {
			CBNaytaTall->Checked = false;
			if (ratatiedot == 2) {
				wcsncpy(rataxmlname , rataxmlfname, sizeof(rataxmlname)/2-1);
				Lue_Radat_Xml(rataxmlfname, 1);
				}
			else {
				lue_leimat(leimafname);
				lue_radat(ratafname);
				}
			}
	   BtnOtaKayttoon->Visible = (ToimintaTila == 2 && emitfl);
#ifndef MAXOSUUSLUKU
		if (kuntosuunnmoodi)
			CBLahto->ItemIndex = 1;
#endif
		initialisoitu = true;
		}
	if (emitfl) {
		CBNaytaTall->Visible = true;
		GBTall->Visible = CBNaytaTall->Checked;
		}
	TabSheetRadat->Visible = true;
	PageControl1->ActivePageIndex = 0;
	rataMuutoksia = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::CBLahtoChange(TObject *Sender)
{
	EdtEnnakko->Visible = CBLahto->ItemIndex == 2;
	Label4->Visible = EdtEnnakko->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::CBMaaliChange(TObject *Sender)
{
	EdtMatkaMaaliin->Visible = CBMaali->ItemIndex == 1;
	LblMatkaMaaliin->Visible = EdtMatkaMaaliin->Visible;
	CBAutoTulos->Visible = CBMaali->ItemIndex == 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::SGRataMouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGRata->RowCount - SGRata->FixedRows >= SGRata->VisibleRowCount + SGRata->TopRow)
		SGRata->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::SGRataMouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGRata->TopRow > SGRata->FixedRows)
		SGRata->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::SGLeimatMouseWheelDown(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGLeimat->RowCount - SGLeimat->FixedRows >= SGLeimat->VisibleRowCount + SGLeimat->TopRow)
		SGLeimat->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::SGLeimatMouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGLeimat->TopRow > SGLeimat->FixedRows)
		SGLeimat->TopRow--;
}
//---------------------------------------------------------------------------

static int Maalikoodi, AjanottoTapa;
static int Ennakko = -99;

void __fastcall TFormRadat::Lue_Xml_File(int lisaa)
{
	if (ToimintaTila == 0) {
		FormMain->BtnTulPalv->Enabled = false;
	}
	if (ToimintaTila > 0)
		OpenDialog1->InitialDir = FormMain->CurrentDir;
	OpenDialog1->FileName = rataxmlname;
	OpenDialog1->DefaultExt = L"xml";
	OpenDialog1->Filter = L"XML-tiedostot (*.XML)|*.XML";
	if (!OpenDialog1->Execute())
		return;
//	wcsncpy(rataxmlname, OpenDialog1->FileName.c_str(), sizeof(rataxmlname)/2-1);
	Lue_Radat_Xml(OpenDialog1->FileName.c_str(), lisaa);
	}

void __fastcall TFormRadat::BtnLueXmlClick(TObject *Sender)
{
	int vast = IDNO;
	if (xmlLuettu) {
		vast = Application->MessageBoxW(L"S‰ilytet‰‰nkˆ aimmat tiedot t‰ydent‰en ja p‰ivitt‰en niit‰?",
			L"Valitse", MB_YESNOCANCEL);
		if (vast == IDCANCEL)
			return;
		}
	Lue_Xml_File(vast == IDYES ? 2 : 1);
}

void TFormRadat::Lue_Radat_Xml(wchar_t *ratafname, int lisaa)
{
	int nkoodi, maxnkoodi;
	ekditp *pkdi;
	maptp Map1;

//	if (lueEventXml(ratafname, false, true, false))
	IOFCourseData *CseData;

	if (lisaa != 1)
		CseData = new IOFCourseData;
	else
		CseData = &ReCourseData;
	if (lueVainRadatXml(ratafname, CseData))
		return;
#ifndef MAXOSUUSLUKU
	SGSarjat->RowCount = 2;
	SGSarjat->Cells[0][1] = L"";
	SGSarjat->Cells[1][1] = L"";

	if (CseData->ClassAssignment[0]) {
		int vast = 1;

		if (ToimintaTila != 2) {
			for (int iA = 0; iA < sizeof(CseData->ClassAssignment)/sizeof(CseData->ClassAssignment[0]); iA++) {
				if (CseData->ClassAssignment[iA] == NULL)
					break;
				if (wcscmpU(CseData->ClassAssignment[iA]->Class, CseData->ClassAssignment[iA]->Course)) {
					vast = 0;
					break;
					}
				}
			}
		if (vast == 0) {
			vast = select3(3, L"Ratatietoihin sis‰ltyy ratojen kohdentamisia sarjoille. Valitse menettely",
				L"Kohdennus sarjoille", L"S‰ilyt‰ ratojen nimet ja kytke sarjoihin",
				L"K‰yt‰ jokaiselle sarjalle omaa rataa", L"J‰t‰ kohdennukset huomioimatta", this);
			}
		if (vast == 1) {
			if (ToimintaTila == 0) {
				for (int iA = 0; iA < sizeof(CseData->ClassAssignment)/sizeof(CseData->ClassAssignment[0]); iA++) {
					if (CseData->ClassAssignment[iA] == NULL)
						break;
					SGSarjat->RowCount = iA+2;
					SGSarjat->Cells[0][iA+1] = CseData->ClassAssignment[iA]->Class;
					SGSarjat->Cells[1][iA+1] = CseData->ClassAssignment[iA]->Course;
					}
				}
			else {
				SGSarjat->RowCount = sarjaluku+1;
				for (int srj = 0; srj < sarjaluku; srj++) {
					SGSarjat->Cells[0][srj+1] = Sarjat[srj].sarjanimi;
					SGSarjat->Cells[1][srj+1] = L"";
					for (int iA = 0; iA < sizeof(CseData->ClassAssignment)/sizeof(CseData->ClassAssignment[0]); iA++) {
						if (CseData->ClassAssignment[iA] == NULL)
							break;
						if (!wcscmpU(CseData->ClassAssignment[iA]->Class, Sarjat[srj].sarjanimi))
							SGSarjat->Cells[1][srj+1] = CseData->ClassAssignment[iA]->Course;
						}
					if (SGSarjat->Cells[1][srj+1].Length() == 0) {
						for (int iCt = 0; iCt < sizeof(CseData->Course)/sizeof(CseData->Course[0]) &&
							CseData->Course[iCt] != NULL; iCt++) {
							if (!wcscmpU(CseData->Course[iCt]->CourseVariation[0]->Name, Sarjat[srj].sarjanimi)) {
								SGSarjat->Cells[1][srj+1] = CseData->Course[iCt]->CourseVariation[0]->Name;
								break;
								}
							}
						}
					}
				}
			}
		if (vast == 2) {
			int kohd[MAXSARJALUKU];
			memset(kohd, 0, sizeof(kohd));
			for (int iA = 0; iA < sizeof(CseData->ClassAssignment)/sizeof(CseData->ClassAssignment[0]); iA++) {
				if (CseData->ClassAssignment[iA] == NULL)
					break;
				for (int iCt = 0; iCt < sizeof(CseData->Course)/sizeof(CseData->Course[0]) &&
					CseData->Course[iCt] != NULL; iCt++) {
					if(!wcscmpU(CseData->ClassAssignment[iA]->Course, CseData->Course[iCt]->Name)) {
						kohd[iA] = iCt+1;
						break;
						}
					}
				}
			for (int iA = 0; iA < sizeof(CseData->ClassAssignment)/sizeof(CseData->ClassAssignment[0]); iA++) {
				if (kohd[iA]) {
					int iCt = kohd[iA]-1;
					if (wcscmpU(CseData->ClassAssignment[iA]->Course, CseData->Course[iCt]->Name)) {
						for (; iCt < sizeof(CseData->Course)/sizeof(CseData->Course[0]) &&
							CseData->Course[iCt] != NULL; iCt++) ;
						if (iCt >= sizeof(CseData->Course)/sizeof(CseData->Course[0]))
							break;
						CseData->Course[iCt] = new IOFCourse;
						*CseData->Course[iCt] = *CseData->Course[kohd[iA]-1];
						}
					wcsncpy(CseData->Course[iCt]->Name, CseData->ClassAssignment[iA]->Class,
						sizeof(CseData->Course[0]->Name)/2-1);
					wcsncpy(CseData->Course[iCt]->CourseVariation[0]->Name, CseData->ClassAssignment[iA]->Class,
						sizeof(CseData->Course[0]->CourseVariation[0]->Name)/2-1);
					}
				}
			}
		}
#endif
	if (lisaa != 1) {
		UINT32 iCt;
		int iCtV;

		for (iCt = 0; iCt < sizeof(CourseData.Course)/sizeof(CourseData.Course[0]) &&
			ReCourseData.Course[iCt] != NULL; iCt++) ;
		for (unsigned int iC = 0; CseData->Course[iC] != NULL; iC++) {
			if (iCt >= sizeof(CourseData.Course)/sizeof(CourseData.Course[0]))
				break;
			iCtV = -1;
			if (lisaa == 2) {
				for (UINT32 iCt1 = 0; iCt1 < sizeof(CourseData.Course)/sizeof(CourseData.Course[0]) &&
					ReCourseData.Course[iCt1] != NULL; iCt1++) {
					if (!wcscmpU(ReCourseData.Course[iCt1]->Name, CseData->Course[iC]->Name)) {
						iCtV = iCt1;
						break;
						}
					}
				}
			if (iCtV >= 0) {
				*ReCourseData.Course[iCtV] = *CseData->Course[iC];
				}
			else {
				ReCourseData.Course[iCt] = new IOFCourse;
				*ReCourseData.Course[iCt] = *CseData->Course[iC];
				iCt++;
				}
			}
		for (iCt = 0; iCt < sizeof(CourseData.Point)/sizeof(CourseData.Point[0]) &&
			ReCourseData.Point[iCt] != NULL; iCt++) ;
		for (unsigned int iC = 0; CseData->Point[iC] != NULL; iC++) {
			if (iCt >= sizeof(CourseData.Point)/sizeof(CourseData.Point[0]))
				break;
			iCtV = -1;
			for (UINT32 iCt1 = 0; iCt1 < sizeof(CourseData.Point)/sizeof(CourseData.Point[0]) &&
				ReCourseData.Point[iCt1] != NULL; iCt1++) {
				if (!wcscmpU(ReCourseData.Point[iCt1]->Code, CseData->Point[iC]->Code)) {
					iCtV = iCt1;
					break;
					}
				}
			if (iCtV >= 0) {
				if (lisaa == 2)
					*ReCourseData.Point[iCtV] = *CseData->Point[iC];
				}
			else {
				ReCourseData.Point[iCt] = new IOFPoint;
				*ReCourseData.Point[iCt] = *CseData->Point[iC];
				iCt++;
				}
			}
		ReCourseData.CopyMap(*CseData);
		delete CseData;
		}
	maxnkoodi = sizeof(koodit) / sizeof(koodit[0]);
	memset(koodit, 0, sizeof(koodit));
	nrasti = 0;
	pkdi = koodit;
	if (CBXMLLeimat->Checked) {
		ReCourseData.siirraXmlKuviot(&pkdi, &nrasti, &maxnkoodi);
		nayta_leimat();
		}
	if (rata_re)
		free(rata_re);
	rata_re = NULL;
	nrata_re = MAXRATA;
	rata_re = (ratatp *) calloc(MAXRATA, sizeof(ratatp));
	ReCourseData.siirraXmlRadat(&rata_re, &nrata_re, L"VNYS"[RGXMLrata->ItemIndex], Maalikoodi, AjanottoTapa, Ennakko);
	ReCourseData.siirraXmlKartta(&Map1);
	if (Map.scale == 0 || lisaa == 1)
		Map.scale = Map1.scale;
	if (Map.leftX == 0 || lisaa == 1)
		Map.leftX = Map1.leftX;
	if (Map.topY == 0 || lisaa == 1)
		Map.topY = Map1.topY;
	if (Map.rightX == 0 || lisaa == 1)
		Map.rightX = Map1.rightX;
	if (Map.btmY == 0 || lisaa == 1)
		Map.btmY = Map1.btmY;
	nayta_koordinaatit();
	CBRadat->ItemIndex = 0;
	rataMuutoksia = false;
	xmlLuettu = true;
	RataItems();
	nayta_rata(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnKirjXmlClick(TObject *Sender)
{
	kirjXmlTiedot(false, true);
}

int TFormRadat::vertSarjat(IOFClassAssignment **CA)
{
	int n1 = 0, n2 = 0;

	for	(int r = 1; r < SGSarjat->RowCount; r++) {
		if (SGSarjat->Cells[0][r].Length() > 0 && SGSarjat->Cells[1][r].Length() > 0 &&
			wcscmp(SGSarjat->Cells[0][r].c_str(), SGSarjat->Cells[1][r].c_str()))
			n1++;
		}
	for (int iA = 0; iA < sizeof(ReCourseData.ClassAssignment)/sizeof(CA[0]); iA++) {
		if (!CA[iA])
			break;
		if (wcscmp(CA[iA]->Class, CA[iA]->Course))
			n2++;
		}
	if (n1 != n2) {
		rataMuutoksia = true;
		return(1);
		}
	n2 = 0;
	for	(int r = 1; r < SGSarjat->RowCount; r++) {
		if (SGSarjat->Cells[0][r].Length() > 0 && SGSarjat->Cells[1][r].Length() > 0 &&
			wcscmp(SGSarjat->Cells[0][r].c_str(), SGSarjat->Cells[1][r].c_str())) {
			for (int iA = 0; iA < sizeof(ReCourseData.ClassAssignment)/sizeof(CA[0]); iA++) {
				if (!CA[iA])
					break;
				if (wcscmp(CA[iA]->Class, CA[iA]->Course) && !wcscmp(CA[iA]->Class, SGSarjat->Cells[0][r].c_str()) &&
					!wcscmp(SGSarjat->Cells[1][r].c_str(), CA[iA]->Course))
					n2++;
				}
			}
		}
	if (n1 != n2) {
		rataMuutoksia = true;
		return(1);
		}
	return(0);
}

void TFormRadat::tallSarjat(IOFClassAssignment **CA)
{
	int n1 = 0;

	for	(int r = 1; r < SGSarjat->RowCount; r++) {
		if (n1 >= sizeof(ReCourseData.ClassAssignment)/sizeof(CA[0]))
			break;
		if (SGSarjat->Cells[0][r].Length() > 0 && SGSarjat->Cells[1][r].Length() > 0 &&
			wcscmp(SGSarjat->Cells[0][r].c_str(), SGSarjat->Cells[1][r].c_str())) {
			if (!CA[n1])
				CA[n1] = new IOFClassAssignment;
			wcsncpy(CA[n1]->Class, SGSarjat->Cells[0][r].c_str(), sizeof(CA[n1]->Class)/2-1);
			wcsncpy(CA[n1]->Course, SGSarjat->Cells[1][r].c_str(), sizeof(CA[n1]->Course)/2-1);
			n1++;
			}
		}
	for (; n1 < sizeof(ReCourseData.ClassAssignment)/sizeof(CA[0]); n1++) {
		if (!CA[n1])
			break;
		delete CA[n1];
		CA[n1] = NULL;
		}
}

int TFormRadat::kirjXmlTiedot(bool kysy, bool talldialog)
{
	ekditp tkoodit[2000];
	bool tmpRataMuutoksia;

	if (!rata_re)
		return(0);
	TallRata(&trata, true);
	if (trata.rastiluku > 0 && memcmp(&trata, rata_re+kRata, sizeof(trata))) {
		int vast = select3(3, L"K‰sitelt‰v‰ rata muuttunut, valitse menettely",
			L"Muutoksia", L"Tallenna muutokset", L"Hylk‰‰ muutokset",
			L"Palaa k‰sitelt‰v‰‰n rataan tallentamatta ratoja", this);
		switch(vast) {
			case 1:
				rata_re[kRata] = trata;
				rataMuutoksia = true;
				break;
			case 2:
				break;
			case 3:
				return(1);
			}
		}
	tmpRataMuutoksia = rataMuutoksia;
	memcpy(tkoodit, koodit, sizeof(koodit));
	BtnTallKooditClick(this);
	if (memcmp(tkoodit, koodit, sizeof(koodit))) {
		if (Application->MessageBoxW(L"Leimasinkoodeissa muistiin tallentamattomia muutoksia, sis‰llytet‰‰kˆ tallennukseen?",
			L"Tallennus", MB_YESNO) == IDNO) {
			memcpy(koodit, tkoodit, sizeof(koodit));
			rataMuutoksia = tmpRataMuutoksia;
			}
		}
	else
		rataMuutoksia = tmpRataMuutoksia;
	vertSarjat(ReCourseData.ClassAssignment);
	if (Application->MessageBoxW(L"Tarkastetaanko tietojen yhteensopivuus?", L"Tarkastus", MB_YESNO) == IDYES) {
		BtnTarkistaClick(this);
		if (Memo1->Lines->Count > 0) {
			if (Application->MessageBoxW(L"Jatketaanko tallennusta?", L"Tallennus", MB_YESNO) == IDNO)
				return(1);
			rataMuutoksia = true;
			}
		}
	if (!rataMuutoksia)
		return(0);
	if (kysy) {
		int vast;
		vast = select3(3, L"Tiedoissa muutoksia, joita ei ole tallennettu tiedostoon. Tallennetaanko nyt?",
			L"Tallennetaanko", L"Tallenna", L"Palaa ratojen k‰sittelyyn", L"Hylk‰‰ muutokset", this);
		if (vast > 1)
			return(vast-1);
		}
	if (CBKirjKoord->Checked && TallKoord(rastikoord))
		return(1);
	ReCourseData.haeXmlKuviot(koodit, CBKirjKoord->Checked ? rastikoord : NULL, nrasti);
	ReCourseData.haeXmlRadat(rata_re, nrata_re);
	ReCourseData.haeXmlKartta(&Map);

	tallSarjat(ReCourseData.ClassAssignment);
	SaveDialog1->FileName = rataxmlname;
	if (talldialog) {
		if (ToimintaTila > 0)
			SaveDialog1->InitialDir = FormMain->CurrentDir;
		SaveDialog1->DefaultExt = L"xml";
		SaveDialog1->Filter = L"XML-tiedostot (*.xml)|*.xml";
		if (!SaveDialog1->Execute())
			return(1);
		wcsncpy(rataxmlname, SaveDialog1->FileName.c_str(), sizeof(rataxmlname)/2-1);
		}
	else {
		wcsncpy(rataxmlname, FormMain->CurrentDir.c_str(), sizeof(rataxmlname)/2-1);
		wcscat(rataxmlname, L"\\");
		wcscat(rataxmlname, rataxmlfname);
		}
	if (kirjVainRadatXml(rataxmlname, &ReCourseData)) {
		Application->MessageBoxW(L"Ratatietojen avaaminen kirjoitettavaksi ei onnistunut.",
			L"Virhe", MB_OK);
		return(1);
		}
	rataMuutoksia = false;
	if (ToimintaTila == 2 && comfl && maxyhteys && memnullcmp((char *)lah_tiedostot, sizeof(lah_tiedostot))) {
		if (Application->MessageBoxW(L"L‰hetet‰‰nkˆ tiedosto verkon muille tietokoneille?",
			L"L‰hetys", MB_YESNO) == IDYES) {
			if (lah_tiedosto(rataxmlname, 0, 2))
				Application->MessageBoxW(L"Tiedoston l‰hett‰minen ei onnistunut. Tiedoston on sijaittava tyˆkansiossa tai sen alihakemistossa",
					L"Ei l‰hetetty", MB_OK);
			}
		}
	return(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::PageControl1Change(TObject *Sender)
{
	if (PageControl1->ActivePageIndex == 2) {
		RGlisaa->Enabled = xmlLuettu;
		EdtRataLkm->Text = nrata_re;
		EdtRastiLkm->Text = nrasti;
		}
	RGlisaa->ItemIndex = RGlisaa->Enabled ? 2 : 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::SGRataSetEditText(TObject *Sender, int ACol, int ARow,
          const UnicodeString Value)
{
	if (Value.Length() > 0 && ARow == SGRata->RowCount-1) {
		SGRata->RowCount++;
		SGRata->Cells[0][ARow] = ARow;
		for (int col = 0; col < SGRata->ColCount; col++)
			SGRata->Cells[col][ARow+1] = L"";
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::SGLeimatSetEditText(TObject *Sender, int ACol, int ARow,
		  const UnicodeString Value)
{
	if (Value.Length() > 0 && ARow == SGLeimat->RowCount-1) {
		SGLeimat->RowCount++;
		for (int col = 0; col < SGLeimat->ColCount; col++)
			SGLeimat->Cells[col][ARow+1] = L"";
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::EdtRNimiExit(TObject *Sender)
{
	wchar_t rnimi[40] = L"";
	wcsncpy(rnimi, EdtRNimi->Text.c_str(), sizeof(rata_re[0].tunnus)/2-1);
	EdtRNimi->Text = upcasewstr(rnimi);
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnOletuskooditClick(TObject *Sender)
{
	memset(koodit, 0, sizeof(koodit));
	nrasti = 0;
	n_erirasti = 0;
	for (int i = 31; i < 250; i++) {
		if (i == 66 || i == 68 || i == 86 || i == 89 || i == 98 || i == 99)
			continue;
		koodit[nrasti].rasti = i;
		koodit[nrasti].koodi = i;
		nrasti++;
		}
	n_erirasti = nrasti;
	rataMuutoksia = true;
	nayta_leimat();
}
//---------------------------------------------------------------------------


void __fastcall TFormRadat::LuevanhaRADATLST1Click(TObject *Sender)
{
	if (ToimintaTila == 0) {
		FormMain->BtnTulPalv->Enabled = false;
		}
	lue_radat(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::KirjoitavanhaRADATLST1Click(TObject *Sender)
{
	if (!rata_re)
		return;
	TallRata(&trata, true);
	if (trata.rastiluku > 0 && memcmp(&trata, rata_re+kRata, sizeof(trata))) {
		int vast = select3(3, L"K‰sitelt‰v‰ rata muuttunut, valitse menettely",
			L"Muutoksia", L"Tallenna muutokset", L"Hylk‰‰ muutokset",
			L"Palaa k‰sitelt‰v‰‰n rataan tallentamatta ratoja", this);
		switch(vast) {
			case 1:
				rata_re[kRata] = trata;
				rataMuutoksia = true;
				break;
			case 2:
				break;
			case 3:
				return;
			}
		}
	kirj_radat();
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::LuevanhaLEIMATLST1Click(TObject *Sender)
{
	lue_leimat(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::KirjoitavanhaLEIMATLST1Click(TObject *Sender)
{
	ekditp tkoodit[2000];

	memcpy(tkoodit, koodit, sizeof(koodit));
	BtnTallKooditClick(Sender);
	if (memcmp(tkoodit, koodit, sizeof(koodit))) {
		if (Application->MessageBoxW(L"Koodeissa muistiin tallentamattomia muutoksia, sis‰llytet‰‰kˆ tallennukseen?",
			L"Tallennus", MB_YESNO) == IDNO)
			memcpy(koodit, tkoodit, sizeof(koodit));
		}
	kirj_leimat();
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::Kopioiradannollausennakkokaikkiinratoihin1Click(TObject *Sender)

{
	int enn = -1;

	if (!rata_re)
		return;
	TallRata(&trata, true);
	if (trata.rastiluku > 0 && memcmp(&trata, rata_re+kRata, sizeof(trata))) {
		int vast = select3(2, L"K‰sitelt‰v‰ rata muuttunut, valitse menettely",
			L"Muutoksia", L"Tallenna muutokset ja kopioi ennakko kaikkiin",
			L"Palaa k‰sitelt‰v‰‰n rataan kopioimatta ennakkoa", L"", this);
		switch(vast) {
			case 1:
				rata_re[kRata] = trata;
				rataMuutoksia = true;
				break;
			case 2:
				return;
			}
		}
	switch (CBLahto->ItemIndex) {
		case 0:
			enn = -1;
			break;
		case 1:
			enn = 0;
			break;
		case 2:
			enn = _wtoi(EdtEnnakko->Text.c_str());
			break;
		}
	for	(int i = 0; i < nrata_re; i++) {
		rata_re[i].ennakko = enn;
		}
	rataMuutoksia = true;
	nayta_rata(CBRadat->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::Kopioiradanmaalivalinnatkaikkiinratoihin1Click(TObject *Sender)

{
	int mlaji = 4, vm = 0;

	if (!rata_re)
		return;
	TallRata(&trata, true);
	if (trata.rastiluku > 0 && memcmp(&trata, rata_re+kRata, sizeof(trata))) {
		int vast = select3(2, L"K‰sitelt‰v‰ rata muuttunut, valitse menettely",
			L"Muutoksia", L"Tallenna muutokset ja kopioi valinnat kaikkiin",
			L"Palaa k‰sitelt‰v‰‰n rataan kopioimatta valintoja", L"", this);
		switch(vast) {
			case 1:
				rata_re[kRata] = trata;
				rataMuutoksia = true;
				break;
			case 2:
				return;
			}
		}
	switch (CBMaali->ItemIndex) {
		case 0:
			mlaji = CBAutoTulos->Checked ? 4 : 2;
			vm = 0;
			break;
		case 1:
			mlaji = 1;
			vm = _wtoi(EdtMatkaMaaliin->Text.c_str());
			break;
		}
	for	(int i = 0; i < nrata_re; i++) {
		rata_re[i].maalilaji = mlaji;
		rata_re[i].viitmatka = vm;
		}
	rataMuutoksia = true;
	nayta_rata(CBRadat->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::Liskaikkiinratoihinmaalinrastikoodi1Click(TObject *Sender)

{
	int r, lisa = 0;
	wchar_t ch;

	if (!rata_re)
		return;
	TallRata(&trata, true);
	if (trata.rastiluku > 0 && memcmp(&trata, rata_re+kRata, sizeof(trata))) {
		int vast = select3(2, L"K‰sitelt‰v‰ rata muuttunut, valitse menettely",
			L"Muutoksia", L"Tallenna muutokset ja lis‰‰ rastikoodi",
			L"Palaa k‰sitelt‰v‰‰n rataan lis‰‰m‰tt‰ rastikoodia", L"", this);
		switch(vast) {
			case 1:
				rata_re[kRata] = trata;
				rataMuutoksia = true;
				break;
			case 2:
				return;
			}
		}
	inputint_prompt(&lisa, L"T‰ss‰ annetaan koodi, kun maalirastilla on leimasin. Anna maalirastin koodi, (arvoa 0 ei lis‰t‰)", &ch, this);
	if (ch == ESC || lisa <= 0)
		return;
	for (r = 0; r < sizeof(rastikoord)/sizeof(rastikoord[0]); r++)
		if (rastikoord[r].tunnus[0] == 0 || lisa <= _wtoi(rastikoord[r].tunnus))
			break;
	if (lisa < _wtoi(rastikoord[r].tunnus)) {
		memmove(rastikoord+r+1, rastikoord+r, sizeof(rastikoord) - (r+1)*sizeof(rastikoord[0]));
		_itow(lisa, rastikoord[r].tunnus, 10);
		rastikoord[r].mapX = 0;
		rastikoord[r].mapY = 0;
		nayta_koordinaatit();
		}
	for	(int i = 0; i < nrata_re; i++) {
		if (rata_re[i].rastiluku < sizeof(rata_re[i].rastikoodi)/sizeof(rata_re[i].rastikoodi[0]) &&
			rata_re[i].rastikoodi[rata_re[i].rastiluku-1] != lisa) {
			rata_re[i].rastikoodi[rata_re[i].rastiluku] = lisa;
//			memset(rata_re[i].maali, 0, sizeof(rata_re[i].maali));
			if (rata_re[i].viitmatka && rata_re[i].matka[rata_re[i].rastiluku-1]) {
				rata_re[i].matka[rata_re[i].rastiluku] = rata_re[i].matka[rata_re[i].rastiluku-1] + rata_re[i].viitmatka;
				rata_re[i].viitmatka = 0;
				}
			if (rata_re[i].maali[0]) {
				for (int j = 0; j < sizeof(rastikoord)/sizeof(rastikoord[0]); j++) {
					if (rastikoord[j].tunnus[0] == 0)
						break;
					if (!wcscmpU(rastikoord[j].tunnus, rata_re[i].maali)) {
						rastikoord[r].mapX = rastikoord[j].mapX;
						rastikoord[r].mapY = rastikoord[j].mapY;
						}
					}
				}
			for (int r = 0; r < MAXNPISTE; r++) {
				if (rastikoord[r].tunnus[0] == 0)
					break;
				if (!wcscmpU(rastikoord[r].tunnus, rata_re[i].maali)) {
					int rr;
					wchar_t st[10];
					_itow(lisa, st, 10);
					for (rr = 0; r < MAXNPISTE; rr++) {
						if (rastikoord[rr].tunnus[0] == 0)
							break;
						if (!wcscmpU(rastikoord[rr].tunnus, st))
							break;
						}
					if (rr < MAXNPISTE && rastikoord[rr].tunnus[0] == 0) {
						wcscpy(rastikoord[rr].tunnus, st);
						rastikoord[rr].mapX = rastikoord[r].mapX;
						rastikoord[rr].mapY = rastikoord[r].mapY;
						}
					}
				}
			if (rata_re[i].maalilaji == 1)
				rata_re[i].maalilaji = 4;
//			memset(rata_re[i].maali, 0, sizeof(rata_re[i].maali));
			rata_re[i].rastiluku++;
			}
		}
	rataMuutoksia = true;
	nayta_rata(CBRadat->ItemIndex);
	nayta_koordinaatit();
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::Suljekaavake1Click(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::Lueradatjarastitxml1Click(TObject *Sender)
{
	BtnLueXmlClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::allennaradatjarastit1Click(TObject *Sender)
{
	BtnKirjXmlClick(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BitBtn2Click(TObject *Sender)
{
	Application->HelpKeyword(L"Ratatietojen siirto muista ohjelmista");
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::CBNaytaTallClick(TObject *Sender)
{
	GBTall->Visible = CBNaytaTall->Checked;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BitBtn3Click(TObject *Sender)
{
	Application->HelpKeyword(L"Ratatietojen muokkaus");
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BitBtn4Click(TObject *Sender)
{
	Application->HelpKeyword(L"Leimasinkoodien tallennus");
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BitBtn5Click(TObject *Sender)
{
	Application->HelpKeyword(L"Ratatulosteet");
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BitBtn6Click(TObject *Sender)
{
	Application->HelpKeyword(L"Ratam‰‰ritysten tarkastaminen");
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::Help1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Ratatiedot");
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::RGUNimiClick(TObject *Sender)
{
	if (!rata_re)
		return;
	if (RGUNimi->ItemIndex > 0) {
		TallRata(&trata, true);
		if (trata.rastiluku > 0 && memcmp(&trata.rastiluku, &rata_re[kRata].rastiluku,
			sizeof(trata)-sizeof(trata.tunnus))) {
			int vast = select3(2, L"K‰sitelt‰v‰ rata muuttunut, valitse menettely",
				L"Muutoksia", L"Tallenna muutokset aimpaan rataan ennen uuden lis‰yst‰",
				L"J‰t‰ aiempi rata ennalleen ja k‰yt‰ muutoksia vain uudessa", L"", this);
			switch(vast) {
				case 1:
					memcpy(&rata_re[kRata].rastiluku, &trata.rastiluku,
						sizeof(trata)-sizeof(trata.tunnus));
					rataMuutoksia = true;
					break;
				case 2:
					break;
				}
			}
		}
}
//---------------------------------------------------------------------------


void __fastcall TFormRadat::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	if (kirjXmlTiedot(true, true) == 1) {
		CanClose = false;
		return;
		}
	if (rata_re)
		free(rata_re);
	rata_re = NULL;
	nrata_re = 0;
	CBRadat->Items->Clear();
	CBRadat->Text = L"";
	EdtRNimi->Text = L"";
	EdtMatkaMaaliin->Text = L"";
	EdtLahto->Text = L"";
	EdtMaali->Text = L"";
	EdtRatapit->Text = L"";
	EdtNousu->Text = L"";
	BtnRataTyhjennaClick(this);
	BtnTyhjKooditClick(this);
	BtnTyhjKoordClick(this);
	xmlLuettu = false;
	initialisoitu = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::SGSarjatMouseWheelDown(TObject *Sender, TShiftState Shift,
          TPoint &MousePos, bool &Handled)
{
	if (SGSarjat->RowCount - SGSarjat->FixedRows >= SGSarjat->VisibleRowCount + SGSarjat->TopRow)
		SGSarjat->TopRow++;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::SGSarjatMouseWheelUp(TObject *Sender, TShiftState Shift,
		  TPoint &MousePos, bool &Handled)
{
	if (SGSarjat->TopRow > SGSarjat->FixedRows)
		SGSarjat->TopRow--;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::SGSarjatSetEditText(TObject *Sender, int ACol, int ARow,
          const UnicodeString Value)
{
	if (Value.Length() > 0 && ARow == SGSarjat->RowCount-1) {
		SGSarjat->RowCount++;
		for (int col = 0; col < SGSarjat->ColCount; col++)
			SGSarjat->Cells[col][ARow+1] = L"";
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::SGSarjatKeyPress(TObject *Sender, System::WideChar &Key)

{
	if (Key == L' ')
		Key = 0;
	Key = towupper2(Key);
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::FormCreate(TObject *Sender)
{
#ifndef MAXOSUUSLUKU
	SGSarjat->RowCount = 2;
	SGSarjat->Cells[0][0] = L"Sarja";
	SGSarjat->Cells[1][0] = L"Rata";
#else
	TabSheetSarjat->TabVisible = false;
	TabSheetSarjat->Visible = false;
	SGSarjat->Enabled = false;
	Label22->Visible = false;
	Label21->Caption = L"Kaavake k‰ytˆss‰ vain henkilˆkohtaisissa kilpailuissa";
#endif
	TabSheetRadat->Visible = true;
	PageControl1->ActivePageIndex = 0;
	SGRata->RowCount = 2;
	SGRata->Cells[0][0] = L"J‰rj.";
	SGRata->Cells[1][0] = L"Koodi";
	SGRata->Cells[2][0] = L"L‰hdˆst‰ (m)";
	SGRata->Cells[0][1] = L"";
	SGRata->Cells[1][1] = L"";
	SGRata->Cells[2][1] = L"";
	SGRata->ColWidths[2] = 90 * Screen->PixelsPerInch / 96;
	SGLeimat->RowCount = 2;
	SGLeimat->Cells[0][0] = L"Rasti";
	SGLeimat->Cells[1][0] = L"Leimat";
//   ButtonTest1->Visible = DeveloperMode;
//   ButtonTest2->Visible = DeveloperMode;
//   BtnLueXml->Visible = DeveloperMode;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::Avaakartta1Click(TObject *Sender)
{
	if (!FormKartta)
		FormKartta = new TFormKartta(this);
	FormKartta->Show();
	if (FormKartta->WindowState == wsMinimized)
		FormKartta->WindowState = wsNormal;
	FormKartta->BringToFront();
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::Nytrata1Click(TObject *Sender)
{
	if (FormKartta) {
		FormKartta->naytaLeimat(rata_re+kRata, NULL, NULL);
		FormKartta->NaytaOtsikko(UnicodeString(L"Rata ")+rata_re[kRata].tunnus);
		}
}
//---------------------------------------------------------------------------

void TFormRadat::nayta_koordinaatit(void)
{
	int i, n = 0;
	EdtScale->Text = Map.scale;
	EdtMapLeft->Text = Map.leftX;
	EdtMapRight->Text = Map.rightX;
	EdtMapTop->Text = Map.topY;
	EdtMapBottom->Text = Map.btmY;
	SGKoord->Cells[0][0] = L"Koodi";
	SGKoord->Cells[1][0] = L"Laji";
	SGKoord->Cells[2][0] = L"X";
	SGKoord->Cells[3][0] = L"Y";
	for (n = 0; n < sizeof(rastikoord)/sizeof(rastikoord[0]); n++)
		if (rastikoord[n].tunnus[0] == 0)
			break;
	SGKoord->RowCount = n + 2;
	SGKoord->Cells[0][1] = L"";
	SGKoord->Cells[1][1] = L"";
	SGKoord->Cells[2][1] = L"";
	SGKoord->Cells[3][1] = L"";
	for (i = 0; i < n; i++) {
		SGKoord->Cells[0][i+1] = rastikoord[i].tunnus;
		if (wcswcind(rastikoord[i].tunnus[0], L"FM") >= 0) {
			SGKoord->Cells[1][i+1] = L"Finish";
			}
		else if (wcswcind(rastikoord[i].tunnus[0], L"SL") >= 0) {
			SGKoord->Cells[1][i+1] = L"Start";
			}
		else {
			SGKoord->Cells[1][i+1] = L"Control";
			}
		SGKoord->Cells[2][i+1] = rastikoord[i].mapX;
		SGKoord->Cells[3][i+1] = rastikoord[i].mapY;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::KeyToUpper(TObject *Sender, System::WideChar &Key)
{
	Key = towupper2(Key);
}
//---------------------------------------------------------------------------

int __fastcall TFormRadat::TallKoord(rastikoordtp *koord)
{
	int err_row = 0, k = 0;
	double val;

	for (int r = 1; r < SGKoord->RowCount; r++) {
		if (SGKoord->Cells[0][r].Length() == 0)
			continue;
		for (int c = 2; c < 4; c++) {
			try {
				val = SGKoord->Cells[c][r].ToDouble();
				} catch (...) {
				err_row = r;
				}
			if (err_row)
				break;
			}
		if (err_row)
			break;
		}
	if (err_row) {
		Application->MessageBoxW((L"Virheellinen koordinaatti rivill‰ "+UnicodeString(err_row)).c_str(),
			L"Este", MB_OK);
		return(1);
		}
	for (int r = 0; r < 4; r++) {
		TEdit *Edt;
		switch (r) {
			case 0:
				Edt = EdtMapLeft;
				break;
			case 1:
				Edt = EdtMapTop;
				break;
			case 2:
				Edt = EdtMapRight;
				break;
			case 3:
				Edt = EdtMapBottom;
				break;
			}
		if (Edt->Text.Length() == 0)
			continue;
		try {
			val = Edt->Text.ToDouble();
			} catch (...) {
			err_row = r+1;
			}
		if (err_row)
			break;
		switch (r) {
			case 0:
				Map.leftX = val;
				break;
			case 1:
				Map.topY = val;
				break;
			case 2:
				Map.rightX = val;
				break;
			case 3:
				Map.btmY = val;
				break;
			}
		}
	if (err_row) {
		Application->MessageBoxW(L"Virheellinen koordinaatti kartan m‰‰rityksiss‰",
			L"Este", MB_OK);
		return(1);
		}
	Map.scale = _wtoi(EdtScale->Text.c_str());
	memset(koord, 0, sizeof(rastikoord));
	for (int r = 1; r < SGKoord->RowCount; r++) {
		if (SGKoord->Cells[0][r].Length() == 0)
			continue;
		koord[k].mapX = SGKoord->Cells[2][r].ToDouble();
		koord[k].mapY = SGKoord->Cells[3][r].ToDouble();
		wcsncpy(koord[k].tunnus, SGKoord->Cells[0][r].c_str(), sizeof(koord[k].tunnus)/2-1);
		k++;
		}
	return(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnVainKoordClick(TObject *Sender)
{
	IOFCourseData TmpCourseData;

	if (TallKoord(rastikoord))
		return;
	TmpCourseData.haeXmlKuviot(koodit, rastikoord, nrasti);
	TmpCourseData.haeXmlKartta(&Map);
	if (ToimintaTila > 0)
		SaveDialog1->InitialDir = FormMain->CurrentDir;
	SaveDialog1->FileName = rataxmlname;
	SaveDialog1->DefaultExt = L"xml";
	SaveDialog1->Filter = L"XML-tiedostot (*.xml)|*.xml";
	if (!SaveDialog1->Execute())
		return;
//	wcsncpy(rataxmlname, SaveDialog1->FileName.c_str(), sizeof(rataxmlname)/2-1);
	if (kirjVainRadatXml(SaveDialog1->FileName.c_str(), &TmpCourseData)) {
		Application->MessageBoxW(L"Ratatietojen avaaminen kirjoitettavaksi ei onnistunut.",
			L"Virhe", MB_OK);
		return;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnTyhjKoordClick(TObject *Sender)
{
	for (int r = 1; r < SGKoord->RowCount; r++)
		for (int c = 0; c < SGKoord->ColCount; c++)
			SGKoord->Cells[c][r] = L"";
	SGKoord->RowCount = 2;
	EdtMapLeft->Text = L"0,0";
	EdtMapRight->Text = L"0,0";
	EdtMapTop->Text = L"0,0";
	EdtMapBottom->Text = L"0,0";
	if (ToimintaTila != 2)
		memset(rastikoord, 0, sizeof(rastikoord));
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnPoistaKoordClick(TObject *Sender)
{
	if (SGKoord->Row > 0) {
		for (int r = SGKoord->Row; r < SGKoord->RowCount-1; r++) {
			for (int c = 0; c < SGKoord->ColCount; c++)
				SGKoord->Cells[c][r] = SGKoord->Cells[c][r+1];
			}
		for (int c = 0; c < SGKoord->ColCount; c++)
			SGKoord->Cells[c][SGKoord->Row-1] = L"";
		if (SGKoord->RowCount > 2)
			SGKoord->RowCount--;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::SGKoordSetEditText(TObject *Sender, int ACol, int ARow,
		  const UnicodeString Value)
{
	double val;
	wchar_t ch;

	if (ACol > 1) {
		if (Value.Length() > 0 && (ch = Value.c_str()[Value.Length()]) >= L'0' && ch <= L'9') {
			try {
				val = Value.ToDouble();
				} catch (...) {
				val = tmpdbl;
				}
			SGKoord->Cells[ACol][ARow] = val;
			tmpdbl = val;
			}
		}
	if (Value.Length() > 0 && ARow == SGKoord->RowCount-1) {
		SGKoord->RowCount++;
		for (int col = 0; col < SGKoord->ColCount; col++)
			SGKoord->Cells[col][ARow+1] = L"";
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::SGKoordGetEditText(TObject *Sender, int ACol, int ARow,
		  UnicodeString &Value)
{
	double val;
	if (ACol > 1) {
		try {
			val = Value.ToDouble();
			} catch (...) {
			val = tmpdbl;
			}
		tmpdbl = val;
		Value = val;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnKoordKayttoonClick(TObject *Sender)
{
	TallKoord(rastikoord);
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnLueKoordCsvClick(TObject *Sender)
{
	wchar_t *p, *fields[10], st[100] = L"", erottimet[6] = L";";
	int nfld = 0, k = -2;
	TextFl *koord_file;
	wchar_t msg[80], line[200];

	if (ToimintaTila > 0)
		OpenDialog1->InitialDir = FormMain->CurrentDir;
	OpenDialog1->DefaultExt = L"csv";
	OpenDialog1->Filter = L"csv-tiedostot (*.csv)|*.CSV|Kaikki tiedostot (*.*)|*.*";
	if (!OpenDialog1->Execute())
		return;

	koord_file = new TextFl(OpenDialog1->FileName.c_str(),L"r");
	if (!koord_file->IsOpen()) {
		Application->MessageBoxW(L"Koordinaattitiedoston avaaminen ei onnistu", L"Virhe", MB_OK);
		delete koord_file;
		return;
		}
	memset(rastikoord, 0, sizeof+(rastikoord));
	memset(fields, 0, sizeof(fields));
	while (k < (int)(sizeof(rastikoord)/sizeof(rastikoord[0])) && !koord_file->Feof()) {
		if (koord_file->ReadLine(line, 198) == NULL)
			break;
		nfld = getfields(line, fields, sizeof(fields)/sizeof(fields[0]), erottimet, NULL, true, false);
		if (nfld >= 3) {
			if (k >= 0)
				wcsncpy(rastikoord[k].tunnus, fields[0], sizeof(rastikoord[k].tunnus)/2-1);
			for (p = fields[1]; *p; p++)
				if (*p == L',')
					*p = L'.';
			if (k == -2)
				Map.leftX = _wtof(fields[1]);
			else if (k == -1)
				Map.rightX = _wtof(fields[1]);
			else
				rastikoord[k].mapX = _wtof(fields[1]);
			for (p = fields[2]; *p; p++)
				if (*p == L',')
					*p = L'.';
			if (k == -2) {
				Map.topY = _wtof(fields[2]);
				}
			else if (k == -1) {
				Map.btmY = _wtof(fields[2]);
				}
			else
				rastikoord[k].mapY = _wtof(fields[2]);
			if (k < 0 || rastikoord[k].mapX || rastikoord[k].mapY)
				k++;
			else
				memset(rastikoord+k, 0, sizeof(rastikoord[k]));
			}
		deletefields(fields, sizeof(fields)/sizeof(fields[0]));
		}
	delete koord_file;
	nayta_koordinaatit();
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnKirjKoordCsvClick(TObject *Sender)
{
	wchar_t *p, *fields[10], st[100] = L"", erotin = L';';
	int nfld = 0, k = 0;
	TextFl *koord_file;
	wchar_t msg[80], line[200];

	if (TallKoord(rastikoord))
		return;
	if (ToimintaTila > 0)
		SaveDialog1->InitialDir = FormMain->CurrentDir;
	SaveDialog1->DefaultExt = L"csv";
	SaveDialog1->Filter = L"csv-tiedostot (*.csv)|*.CSV|Kaikki tiedostot (*.*)|*.*";
	if (!SaveDialog1->Execute())
		return;

	koord_file = new TextFl(SaveDialog1->FileName.c_str(),L"w");
	if (!koord_file->IsOpen()) {
		Application->MessageBoxW(L"Koordinaattitiedoston avaaminen ei onnistu", L"Virhe", MB_OK);
		delete koord_file;
		return;
		}
	swprintf(line, L"%s%c%.3f%c%.3f\n", L"K0", erotin,
		Map.leftX, erotin, Map.topY);
	koord_file->WriteLine(line);
	swprintf(line, L"%s%c%.3f%c%.3f\n", L"K1", erotin,
		Map.rightX, erotin, Map.btmY);
	koord_file->WriteLine(line);
	for (int i = 0; i < sizeof(rastikoord)/sizeof(rastikoord[0]); i++) {
		if (rastikoord[i].tunnus[0] == 0)
			break;
		swprintf(line, L"%s%c%.3f%c%.3f\n", rastikoord[i].tunnus, erotin,
			rastikoord[i].mapX, erotin,rastikoord[i].mapY);
		koord_file->WriteLine(line);
		}
	delete koord_file;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::MerkLahtokoodiClick(TObject *Sender)

{
	wchar_t ch, koodi[30] = L"";

	if (!rata_re)
		return;
	TallRata(&trata, true);
	if (trata.rastiluku > 0 && memcmp(&trata, rata_re+kRata, sizeof(trata))) {
		int vast = select3(2, L"K‰sitelt‰v‰ rata muuttunut, valitse menettely",
			L"Muutoksia", L"Tallenna muutokset ja lis‰‰ rastikoodi",
			L"Palaa k‰sitelt‰v‰‰n rataan lis‰‰m‰tt‰ rastikoodia", L"", this);
		switch(vast) {
			case 1:
				rata_re[kRata] = trata;
				rataMuutoksia = true;
				break;
			case 2:
				return;
			}
		}
	inputstr_prompt(koodi, sizeof(rata_re[0].lahto)/2-1, L"T‰ss‰ annettavaa koodia k‰ytet‰‰n vain karttatoiminnoissa.", &ch, this);
	if (ch == ESC)
		return;
	for	(int i = 0; i < nrata_re; i++) {
		wcsncpy(rata_re[i].lahto, koodi, sizeof(rata_re[i].lahto)/2-1);
		}
	rataMuutoksia = true;
	nayta_rata(CBRadat->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::MerkMaalikoodiClick(TObject *Sender)

{
	wchar_t ch, koodi[30] = L"";

	if (!rata_re)
		return;
	TallRata(&trata, true);
	if (trata.rastiluku > 0 && memcmp(&trata, rata_re+kRata, sizeof(trata))) {
		int vast = select3(2, L"K‰sitelt‰v‰ rata muuttunut, valitse menettely",
			L"Muutoksia", L"Tallenna muutokset ja lis‰‰ rastikoodi",
			L"Palaa k‰sitelt‰v‰‰n rataan lis‰‰m‰tt‰ rastikoodia", L"", this);
		switch(vast) {
			case 1:
				rata_re[kRata] = trata;
				rataMuutoksia = true;
				break;
			case 2:
				return;
			}
		}
	inputstr_prompt(koodi, sizeof(rata_re[0].maali)/2-1, L"T‰ss‰ annettavaa koodia k‰ytet‰‰n vain karttatoiminnoissa. "
		L"Koodi tarvitaan, vain kun maalissa ei ole leimasinta", &ch, this);
	if (ch == ESC)
		return;
	for	(int i = 0; i < nrata_re; i++) {
		wcsncpy(rata_re[i].maali, koodi, sizeof(rata_re[i].maali)/2-1);
		}
	rataMuutoksia = true;
	nayta_rata(CBRadat->ItemIndex);
}
//---------------------------------------------------------------------------
static bool haeKoord(wchar_t *tunnus, double *X, double *Y)
{
	if (tunnus[0]) {
		for (int i = 0; i < sizeof(rastikoord)/sizeof(rastikoord[0]); i++) {
			if (rastikoord[i].tunnus[0] == 0)
				break;
			if (!wcscmpU(tunnus, rastikoord[i].tunnus)) {
				*X = rastikoord[i].mapX;
				*Y = rastikoord[i].mapY;
				return(true);
				}
			}
		}
	*X = 999999;
	*Y = 999999;
	return(false);
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::Laskerastivlienmatkatkoordinaateista1Click(TObject *Sender)
{
	double X1, Y1, X2, Y2;
	int scale, mtk, alkulisa = 0, loppulisa = 0;
	wchar_t ch;

	scale = _wtoi(EdtScale->Text.c_str());
	if (scale < 100) {
		Application->MessageBoxW(L"Kartan mittakaavaa ei ole m‰‰ritelty tai sen arvo on alle 100", L"Este", MB_OK);
		return;
		}
	inputint_prompt(&alkulisa, L"Anna kaikkien ratojen ensimm‰isen rastiv‰lin mahdollinen lis‰matka", &ch, this);
	if (ch == ESC)
		return;
	inputint_prompt(&loppulisa, L"Anna kaikkien ratojen loppuviitoituksen mahdollinen lis‰matka", &ch, this);
	if (ch == ESC)
		return;
	for	(int i = 0; i < nrata_re; i++) {
		haeKoord(rata_re[i].lahto, &X1, &Y1);
		for (int r = 0; r <= rata_re[i].rastiluku; r++) {
			wchar_t tunnus[20];
			if (r == rata_re[i].rastiluku)
				haeKoord(rata_re[i].maali, &X1, &Y1);
			else {
				_itow(rata_re[i].rastikoodi[r], tunnus, 10);
				haeKoord(tunnus, &X2, &Y2);
				}
			if (X1 < 999999 && X2 < 999999)
				mtk = (int) (scale * sqrt((X2-X1)*(X2-X1)+(Y2-Y1)*(Y2-Y1))/1000);
			if (r == rata_re[i].rastiluku)
				rata_re[i].viitmatka = (mtk > 0 ? mtk+loppulisa : 0);
			else if (r == 0)
				rata_re[i].matka[r] = mtk + alkulisa;
			else {
				rata_re[i].matka[r] = rata_re[i].matka[r-1] + mtk;
				}
			X1 = X2;
			Y1 = Y2;
			}
		if (rata_re[i].viitmatka == 0 && loppulisa > 0)
			rata_re[i].matka[rata_re[i].rastiluku-1] += loppulisa;
		}
	rataMuutoksia = true;
	nayta_rata(CBRadat->ItemIndex);
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnPoistaKohdennusClick(TObject *Sender)
{
	if (SGSarjat->Row < 1)
		return;
	for (int r = SGSarjat->Row; r < SGSarjat->RowCount-1; r++) {
		for (int c = 0; c < SGSarjat->ColCount; c++)
			SGSarjat->Cells[c][r] = SGSarjat->Cells[c][r+1];
		}
	for (int c = 0; c < SGSarjat->ColCount; c++)
		SGSarjat->Cells[c][SGSarjat->RowCount-1] = L"";
	if (SGSarjat->RowCount > 2)
		SGSarjat->RowCount--;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::BtnTyhjKohdennuksetClick(TObject *Sender)
{
	while (SGSarjat->RowCount > 2 || SGSarjat->Cells[0][1].Length() > 0) {
	   SGSarjat->Row = 1;
	   BtnPoistaKohdennusClick(Sender);
	   }
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::SGLeimatExit(TObject *Sender)
{
	if (SGLeimat->Row > 0) {

	}
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::Merkitsekaikkienratojenpituudetmaalinmukaan1Click(TObject *Sender)

{
	for	(int i = 0; i < nrata_re; i++) {
		rata_re[i].ratapit = rata_re[i].matka[rata_re[i].rastiluku-1] + rata_re[i].viitmatka;
		}
	nayta_rata(CBRadat->ItemIndex);
	rataMuutoksia = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormRadat::Nytrastit1Click(TObject *Sender)
{
	if (FormKartta) {
		FormKartta->naytaRastit();
		}
}
//---------------------------------------------------------------------------

