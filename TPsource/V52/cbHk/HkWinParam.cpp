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
#include <vcl.h>
#pragma hdrstop

#include "WinHk.h"
/*
FLD exp_kentat[38] = {
	{L"No", 1, 1, 1},
	{L"KilpId", 2, 1, 2},
	{L"IntId", 0, 1, 3},
	{L"Piiri", 3, 1, 4},
	{L"Sukunimi", 4, 1, 5},
	{L"Etunimi", 5, 1, 6},
	{L"Arvo", 0, 0, 7},
	{L"Seura", 6, 1, 8},
	{L"Seuralyh", 7, 1, 9},
	{L"Yhdistys", 0, 0, 10},
	{L"Joukkue", 0, 0, 11},
	{L"Maa", 8, 1, 12},
	{L"Sarja", 9, 1, 13},
	{L"Sukup", 0, 1, 14},
	{L"Synt", 0, 1, 15},
	{L"Ikäsrj", 0, 1, 16},
	{L"ArvRyhmä", 0, 0, 17},
	{L"IlmLista", 0, 0, 18},
	{L"Piste1", 0, 0, 19},
	{L"Piste2", 0, 0, 20},
	{L"Piste3", 0, 0, 21},
	{L"Bib", 1, 1, 22},
	{L"Sarja", 1, 1, 23},
	{L"Badge", 2, 1, 24},
	{L"Laina", 3, 1, 25},
	{L"Badge2", 0, 0, 26},
	{L"Laina2", 0, 0, 27},
	{L"ArvLahto", 4, 1, 28},
	{L"TotLahto", 0, 0, 29},
	{L"Tulos", 5, 1, 30},
	{L"Tark", 6, 1, 31},
	{L"Sija", 7, 1, 32},
	{L"Sakko", 8, 1, 33},
	{L"Rata", 0, 0, 34},
	{L"PvPiste1", 0, 0, 35},
	{L"PvPiste2", 0, 0, 36},
	{L"VaTulos", 0, 0, 37},
	{L"VaSija", 0, 0, 38}
	};

int Kexp_kentat = 21, Pexp_kentat = 15, Vexp_kentat = 2;
*/

FLD2 exp_kentat[] = {
	{L"No", FLDID_KILPNO, 1, 1, 1},
	{L"KilpId", FLDID_KILPID, 2, 1, 2},
	{L"IntId", FLDID_INTID, 0, 1, 3},
	{L"Piiri", FLDID_PIIRI, 3, 1, 4},
	{L"Sukunimi", FLDID_SUKUNIMI, 4, 1, 5},
	{L"Etunimi", FLDID_ETUNIMI, 5, 1, 6},
	{L"Arvo", FLDID_ARVO, 0, 0, 7},
	{L"Seura", FLDID_SEURA, 6, 1, 8},
	{L"Seuralyh", FLDID_SEURALYH, 7, 1, 9},
	{L"Yhdistys", FLDID_YHD, 0, 0, 10},
	{L"Joukkue", FLDID_JK, 0, 0, 11},
	{L"Maa", FLDID_MAA, 8, 1, 12},
	{L"Sarja", FLDID_SARJA, 9, 1, 13},
	{L"Sukup", FLDID_SUKUP, 0, 1, 14},
	{L"Synt", FLDID_SYNT, 0, 1, 15},
	{L"Ikäsrj", FLDID_IKASRJ, 0, 1, 16},
	{L"ArvRyhmä", FLDID_ARVRYH, 0, 0, 17},
	{L"IlmLista", FLDID_ILMLISTA, 0, 0, 18},
	{L"Piste1", FLDID_PISTE1, 0, 0, 19},
	{L"Piste2", FLDID_PISTE2, 0, 0, 20},
	{L"Piste3", FLDID_PISTE3, 0, 0, 21},
	{L"Bib", FLDID_BIB, 1, 1, 22},
	{L"Sarja", FLDID_VSARJA, 1, 1, 23},
	{L"Badge", FLDID_BADGE, 2, 1, 24},
	{L"Laina", FLDID_LAINA, 3, 1, 25},
	{L"Badge2", FLDID_BADGE2, 0, 0, 26},
	{L"Laina2", FLDID_LAINA2, 0, 0, 27},
	{L"ArvLahto", FLDID_LAHTOARV, 4, 1, 28},
	{L"TotLahto", FLDID_LAHTOTOT, 0, 0, 29},
	{L"Tulos", FLDID_TULOS, 5, 1, 30},
	{L"Tark", FLDID_TARK, 6, 1, 31},
	{L"Sija", FLDID_SIJA, 7, 1, 32},
	{L"AmpSakko", FLDID_AMPSAKKO, 8, 1, 33},
	{L"Sakko", FLDID_SAKKO, 8, 1, 34},
	{L"Rata", FLDID_RATA, 0, 0, 35},
	{L"Erä", FLDID_ERA, 0, 0, 36},
	{L"Selitys", FLDID_SELITYS, 8, 1, 37},
	{L"PvPiste1", FLDID_PVPISTE1, 0, 0, 38},
	{L"PvPiste2", FLDID_PVPISTE2, 0, 0, 39},
	{L"Ennätys", FLDID_ENNATYS, 0, 0, 40},
	{L"Tavoite", FLDID_TAVOITE, 0, 0, 41},
	{L"Flags", FLDID_FLAGS, 0, 0, 42},
	{L"VaTulos", FLDID_VATULOS, 0, 0, 43},
	{L"VaSija", FLDID_VASIJA, 0, 0, 44},
	{L"Nimi", FLDID_NIMI, 0, 0, 100}
	};

int Kexp_kentat = 21, Pexp_kentat = 21, Vexp_kentat = 2, Xexp_kentat = 1;

ColTp hTGridCols0[HT_GRIDCOLS] = {
	{L"Id", 50, 0, 1, 0, true},
	{L"Sukunimi", 80, 0, 1, 0, true},
	{L"Etunimi", 80, 0, 1, 0, true},
	{L"Seura", 100, 0, 1, 0, true},
	{L"Maa", 30, 0, 1, 0, false},
	{L"Synt.", 30, 0, 1, 0, true},
	{L"Sukup.", 30, 0, 1, 0, true},
	{L"Sarja", 60, 0, 1, 0, false},
	{L"Bib", 40, 0, 1, 0, false},
	{L"Badge", 45, 0, 1, 0, true},
	{L"Kv. id", 50, 0, 1, 0, true},
	{L"Joukkue", 55, 0, 1, 0, false},
	{L"Ranki", 50, 0, 1, 0, false},
	{L"Maksu", 35, 0, 1, 0, false},
	{L"OsOttoLkm", 60, 0, 1, 0, false},
	{L"ViimOsOtto", 60, 1, 0, 0, false},
	{L"Lisätieto", 60, 1, 0, 0, false}
};

seurIkkParamClass seurIkkParam[NTILANNEFORM];

seurIkkParamClass seurIkkParam0;
/* = {
  0, //   int Sarja;
  0, //   int Va;
  false, //   bool naytaAlisarja;
  true, //   bool naytaMaa;
  false, //   bool rajaus;
  true, //   bool EtuSuku;
  1, //   int seuraTieto;
  false, //   bool kokotulos;
  true, //   bool eroaika;
  false, //   bool kaikkiajat;
  true, //   bool vainjarj;
  false, //   bool smartJrj;
  false, //   bool lahtoaika;
  fals, //   bool yhttulos;
  10, //   int FontSize;
  false, //   bool Lihavoitu;
  {MINUUTTI, 3*MINUUTTI}, //   int korRaja[2];
  2000, //   int ScrollVali;
  300, //   int ScrollRiviKesto;
  false, //   bool ScrollOn;
  1, //   int ScrollType;
  false, //   bool KokoRuutu;
  3*MINUUTTI, //   int odRaja;
  0 //   int ScrollSarjat[MAXSARJALUKU+MAXYHD+1];
};
*/

tapIkkParamClass tapIkkParam[NTAPFORM];

tapIkkParamClass tapIkkParam0;

