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

#include "UnitMain.h"

FLD2 exp_kentat[37] = {
	{L"No", FLDID_KILPNO, 1, 1, 1},
	{L"JoukkueId", FLDID_JOUKKID, 0, 1, 2},
	{L"Piiri", FLDID_PIIRI, 1, 1, 3},
	{L"Seura", FLDID_SEURA, 1, 1, 4},
	{L"Joukkue", FLDID_JOUKKNO, 1, 0, 5},
	{L"JoukkNimi", FLDID_JOUKKNIMI, 0, 0, 6},
	{L"Maakdi", FLDID_MAA, 1, 1, 7},
	{L"Sarja", FLDID_SARJA, 1, 1, 8},
	{L"Sarja", FLDID_ERA, 1, 1, 9},
	{L"IlmLista", FLDID_ILMLISTA, 0, 0, 10},
	{L"Piste1", FLDID_PISTE1, 1, 0, 11},
	{L"Piste2", FLDID_PISTE2, 1, 0, 12},
	{L"Piste3", FLDID_PISTE3, 1, 0, 13},
	{L"KilpId", FLDID_KILPID, 1, 1, 14},
	{L"Sukunimi", FLDID_SUKUNIMI, 1, 1, 15},
	{L"Etunimi", FLDID_ETUNIMI, 1, 1, 16},
	{L"Arvo", FLDID_ARVO, 0, 0, 17},
	{L"Seura", FLDID_OSSEURA, 0, 1, 18},
	{L"Badge", FLDID_BADGE, 1, 1, 19},
	{L"Badge2", FLDID_BADGE2, 1, 1, 20},
	{L"Laina", FLDID_LAINA, 1, 1, 21},
	{L"Laina2", FLDID_LAINA2, 1, 1, 22},
	{L"Rata", FLDID_RATA, 1, 0, 23},
	{L"Tark", FLDID_TARK, 1, 1, 24},
	{L"AmpSakko", FLDID_AMPSAKKO, 1, 1, 25},
	{L"Sakko", FLDID_SAKKO, 1, 1, 26},
	{L"OsPst1", FLDID_OSPISTE1, 1, 0, 27},
	{L"OsPst2", FLDID_OSPISTE2, 1, 0, 28},
	{L"OsPst3", FLDID_OSPISTE3, 1, 0, 29},
	{L"Selitys", FLDID_SELITYS, 1, 1, 30},
	{L"Lahto", FLDID_LAHTO, 1, 1, 31},
	{L"Lahtolaji", FLDID_LAHTOLAJI, 1, 1, 32},
	{L"Vaihto", FLDID_MAALI, 1, 1, 33},
	{L"Sija", FLDID_SIJA, 1, 1, 34},
	{L"Nimi", FLDID_NIMI, 0, 1, 35},
	{L"VaAika", FLDID_VAAIKA, 1, 0, 36},
	{L"VaSija", FLDID_VASIJA, 1, 0, 37}
	};

int Kexp_kentat = 13, Pexp_kentat = 22, Vexp_kentat = 2;

ColTp hTGridCols0[14] = {
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
	{L"Maksu", 35, 0, 1, 0, false}
};

