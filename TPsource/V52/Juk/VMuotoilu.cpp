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
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <string>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <bstrings.h>
#include <butil.h>

#pragma hdrstop

#include "VMuotoilu.h"
//---------------------------------------------------------------------------
//#pragma package(smart_init)

FldFrmtTp dspflds[] = {
	F_SJ, L"Sija", L"Sija", 0, 4, 1, 1,                   	// 0: sija
	F_EDSJ, L"EdSija", L"EdSija", 6, 0, 1, 1,                   	// 0: sija
	F_KNO, L"Nro", L"Nro",  6, 4, 1, 1,                   	// 1: numero
	F_JOUK, L"Joukkue", L"Joukkue", 11,30, 0, 1,                 // 2: joukkue
	F_MAA, L"Maa", L"Maa", 42, 3, 0, 1,                   	// 3: maa
	F_JOUKNM, L"Jk-nimi", L"Jk-nimi", 60,0, 0, 1,               // 4: joukkue
	F_JOUKID, L"Jk-id", L"Jk-id", 60,0, 0, 1,                 // 5: joukkue
	F_NIMI, L"Nimi", L"Nimi", 46,23,0, 1,                   	// 6: nimi
	F_ARVO, L"Arvo", L"Arvo", 13,0,0, 1,                   	// 7: arvo
	F_LISNO, L"Lisenssi", L"Lisenssi", 45,0, 1, 1,               // 8: lisenssi
	F_BADGE, L"Badge", L"Badge", 45,0, 1, 1,                  // 9: badge
	F_RATA, L"Rata", L"Rata", 45,0, 0, 1,                  // 9: badge
	F_OSSRA, L"Seura", L"Seura", 45,0, 0, 1,                 // 10: seura
	F_TLS, L"Tulos", L"Tulos", 70,8, 1, 1,                 	// 11: tulos
	F_ERO, L"Ero", L"Ero",  70,0, 1, 1,                  	// 12: ero k‰rkeen
	F_SAK, L"Sakot", L"Sakot",70,0, 1, 1,                   	// 13: sakot
	F_TARK, L"Hyv‰ks.", L"T", 79, 1, 0, 1,               // 14: Leimantarkastus
	F_PIST, L"Pisteet", L"Pisteet", 70, 0, 1, 1,              	// 15: pisteet
	F_OSASJ, L"Os-sija", L"OsSj", 70, 0, 1, 1,              // 16: osuuden tai v‰liajan sija
	F_OSATLS, L"Os-tulos", L"OsTls", 70,0, 1, 1,            // 17: osuuden tulos tai v‰liaika
	F_SEURSJ, L"Seur.sj.", L"Sj.", 70, 0, 0, 1,       		// 18: seuraava osatulos
	0, 0, 0, 0, 0, 0
	};

FldFrmtTp fileflds[] = {
	F_SJ, L"Sija", L"Sija", 0, 4, 1, 1,                   	// 0: sija
	F_EDSJ, L"EdSija", L"EdSija", 6, 4, 1, 1,                   	// 0: sija
	F_KNO, L"Nro", L"Nro",  15, 4, 1, 1,                   	// 1: numero
	F_JOUK, L"Joukkue", L"Joukkue", 22,40, 0, 1,                 // 2: joukkue
	F_MAA, L"Maa", L"Maa", 70, 3, 0, 1,                   	// 3: maa
	F_JOUKNM, L"Jk-nimi", L"Jk-nimi", 70,0, 0, 1,               // 4: joukkue
	F_JOUKID, L"Jk-id", L"Jk-id", 70,0, 0, 1,                 // 5: joukkue
	F_NIMI, L"Nimi", L"Nimi", 76,30,0, 1,                   	// 6: nimi
	F_ARVO, L"Arvo", L"Arvo", 110,0,0, 1,                   	// 7: arvo
	F_LISNO, L"Lisenssi", L"Lisenssi", 100,0, 1, 1,               // 8: lisenssi
	F_BADGE, L"Badge", L"Badge", 100,0, 1, 1,                 // 9: badge
	F_RATA, L"Rata", L"Rata", 100,0, 0, 1,                 // 9: badge
	F_OSSRA, L"OsSeura", L"OsSeura", 110,0, 0, 1,                 	// 10: seura
	F_TLS, L"Tulos", L"Tulos", 110,10, 1, 1,                 	// 11: tulos
	F_ERO, L"Ero", L"Ero",  121,10, 1, 1,                  	// 12: ero k‰rkeen
	F_SAK, L"Sakot", L"Sakot",121,0, 1, 1,                   	// 13: sakot
	F_TARK, L"Hyv‰ks.", L"T", 133, 1, 0, 1,               // 14: Leimantarkastus
	F_PIST, L"Pisteet", L"Pisteet", 133, 0, 1, 1,              	// 15: pisteet
	F_OSASJ, L"Os-sija", L"OsSj", 136, 4, 1, 1,              // 16: osuuden tai v‰liajan sija
	F_OSATLS, L"Os-tulos", L"OsTls", 142,10, 1, 1,            // 17: osuuden tulos tai v‰liaika
	F_SEURSJ, L"Seur.sj.", L"Sj.", 126, 0, 0, 1,       		// 18: seuraava osatulos
	0, 0, 0, 0, 0, 0
	};

FldFrmtTp fileflds_suunn[] = {
	F_SJ, L"Sija", L"Sija", 0, 4, 1, 1,                   	// 0: sija
	F_EDSJ, L"EdSija", L"EdSija", 6, 4, 1, 1,                   	// 0: sija
	F_KNO, L"Nro", L"Nro",  15, 4, 1, 1,                   	// 1: numero
	F_JOUK, L"Joukkue", L"Joukkue", 22,40, 0, 1,                 // 2: joukkue
	F_MAA, L"Maa", L"Maa", 70, 3, 0, 1,                   	// 3: maa
	F_JOUKNM, L"Jk-nimi", L"Jk-nimi", 70,0, 0, 1,               // 4: joukkue
	F_JOUKID, L"Jk-id", L"Jk-id", 70,0, 0, 1,                 // 5: joukkue
	F_NIMI, L"Nimi", L"Nimi", 76,30,0, 1,                   	// 6: nimi
	F_ARVO, L"Arvo", L"Arvo", 110,0,0, 1,                   	// 7: arvo
	F_LISNO, L"Lisenssi", L"Lisenssi", 110,0, 1, 1,               // 8: lisenssi
	F_BADGE, L"Badge", L"Badge", 110,0, 1, 1,                  // 9: badge
	F_RATA, L"Rata", L"Rata", 110,0, 0, 1,                   // 11: rata
	F_OSSRA, L"OsSeura", L"OsSeura", 110,0, 0, 1,                // 12: seura
	F_TLS, L"Tulos", L"Tulos", 110,10, 1, 1,                 	// 11: tulos
	F_ERO, L"Ero", L"Ero",  121,10, 1, 1,                  	// 12: ero k‰rkeen
	F_SAK, L"Sakot", L"Sakot",121,0, 1, 1,                   	// 13: sakot
	F_TARK, L"Hyv‰ks.", L"T", 133, 1, 0, 1,               // 14: Leimantarkastus
	F_PIST, L"Pisteet", L"Pisteet", 133, 0, 1, 1,              	// 15: pisteet
	F_OSASJ, L"Os-sija", L"OsSj", 136, 4, 1, 1,              // 16: osuuden tai v‰liajan sija
	F_OSATLS, L"Os-tulos", L"OsTls", 142,10, 1, 1,            // 17: osuuden tulos tai v‰liaika
	F_SEURSJ, L"Seur.sj.", L"Sj.", 126, 0, 0, 1,       		// 18: seuraava osatulos
	0, 0, 0, 0, 0, 0
	};

FldFrmtTp filefldsk[] = {
	F_SJ, L"Sija", L"Sija", 0, 4, 1, 2,                   	// 0: sija
	F_KNO, L"Nro", L"Nro", 6, 4, 1, 2,                   	// 1: numero
	F_JOUK, L"Joukkue", L"Joukkue", 12, 45, 0, 2,                 // 2: joukkue
	F_MAA, L"Maa", L"Maa", 60, 3, 0, 2,                   	// 3: maa
	F_JOUKNM, L"Jk-nimi", L"Jk-nimi", 60,0, 0, 2,               // 4: joukkue
	F_JOUKID, L"Jk-id", L"Jk-id", 60,0, 0, 2,                 // 5: joukkue
	F_NIMI, L"Nimi", L"Nimi", 18,40,0, 1,                  	// 6: nimi
	F_ARVO, L"Arvo", L"Arvo", 50,0,0, 1,                   	// 7: arvo
	F_LISNO, L"Lisenssi", L"LisNo", 50,0, 1, 1,              // 8: lisenssi
	F_BADGE, L"Badge", L"Badge", 50,0, 1, 1,                 // 9: badge
	F_RATA, L"Rata", L"Rata", 110,0, 0, 1,                   // 9: badge
	F_OSSRA, L"OsSeura", L"OsSeura", 110,0, 0, 1,              	// 10: seura
	F_TLS, L"Tulos", L"Tulos", 66,10, 1, 3,                 	// 11: tulos
	F_ERO, L"Ero", L"Ero",  80, 0, 1, 3,                  	// 12: ero k‰rkeen
	F_SAK, L"Sakot", L"Sakot",80, 0, 1, 1,                   	// 13: sakot
	F_TARK, L"Hyv‰ks.", L"T", 78, 0, 0, 1,               // 14: Leimantarkastus
	F_PIST, L"Pisteet", L"Pisteet", 80, 0, 1, 1,              	// 15: pisteet
	F_OSASJ, L"Os-sija", L"OsSj", 85, 4, 1, 1,              // 16: osuuden tai v‰liajan sija
	F_OSATLS, L"Os-tulos", L"OsTls", 93,10, 1, 1,            // 17: osuuden tulos tai v‰liaika
	F_OSOTS, L"Os-ots", L"Osuus", 6, 8, 0, 1,       		    // 18: osuuden otsikko
	F_VSJ, L"Vaihtosj", L"VaihtoSj", 60, 4, 1, 1,                // : sija vaihdossa
	0, 0, 0, 0, 0, 0
	};

FldFrmtTp prtflds_suunn[] = {
	F_SJ, L"Sija", L"Sija", 0, 4, 1, 1,                   	// 0: sija
	F_EDSJ, L"EdSija", L"EdSija", 50, 4, 1, 1,                   	// 0: sija
	F_KNO, L"Nro", L"Nro", 90, 4, 1, 1,                   	// 1: numero
	F_JOUK, L"Joukkue", L"Joukkue", 140, 35, 0, 1,                 // 2: joukkue
	F_MAA, L"Maa", L"Maa", 395, 3, 0, 1,                   	// 3: maa
	F_JOUKNM, L"Jk-nimi", L"Jk-nimi", 420,0, 0, 1,               // 4: joukkue
	F_JOUKID, L"Jk-id", L"Jk-id", 420,0, 0, 1,                 // 5: joukkue
	F_NIMI, L"Nimi", L"Nimi", 450,30,0, 1,                  	// 6: nimi
	F_ARVO, L"Arvo", L"Arvo", 600,0,0, 1,                   	// 7: arvo
	F_LISNO, L"Lisenssi", L"Lisenssi", 600,0, 1, 1,              // 8: lisenssi
	F_BADGE, L"Badge", L"Badge", 600,0, 1, 1,                 // 9: badge
	F_RATA, L"Rata", L"Rata", 600,0, 0, 1,                 // 9: badge
	F_OSSRA, L"OsSeura", L"OsSeura", 600,0, 0, 1,              	// 10: seura
	F_TLS, L"Tulos", L"Tulos", 650,10, 1, 1,                 	// 11: tulos
	F_ERO, L"Ero", L"Ero",  730,10, 1, 1,                  	// 12: ero k‰rkeen
	F_SAK, L"Sakot", L"Sakot",730,0, 1, 1,                   	// 13: sakot
	F_TARK, L"Hyv‰ks.", L"T", 835, 1, 0, 1,               // 14: Leimantarkastus
	F_PIST, L"Pisteet", L"Pisteet", 840, 0, 1, 1,              	// 15: pisteet
	F_OSASJ, L"Os-sija", L"OsSj", 825, 0, 1, 1,              // 16: osuuden tai v‰liajan sija
	F_OSATLS, L"Os-tulos", L"OsTulos", 850, 8, 1, 1,            // 17: osuuden tulos tai v‰liaika
	F_SEURSJ, L"Seur.sj.", L"Sj.", 860, 0, 0, 1,       		// 18: seuraava osatulos
	0, 0, 0, 0, 0, 0
	};

FldFrmtTp prtflds[] = {
	F_SJ, L"Sija", L"Sija", 0, 4, 1, 1,                   	// 0: sija
	F_EDSJ, L"EdSija", L"EdSija", 50, 4, 1, 1,                   	// 0: sija
	F_KNO, L"Nro", L"Nro", 90, 4, 1, 1,                   	// 1: numero
	F_JOUK, L"Joukkue", L"Joukkue", 140, 35, 0, 1,                 // 2: joukkue
	F_MAA, L"Maa", L"Maa", 395, 3, 0, 1,                   	// 3: maa
	F_JOUKNM, L"Jk-nimi", L"Jk-nimi", 420,0, 0, 1,               // 4: joukkue
	F_JOUKID, L"Jk-id", L"Jk-id", 420,0, 0, 1,                 // 5: joukkue
	F_NIMI, L"Nimi", L"Nimi", 450,30,0, 1,                  	// 6: nimi
	F_ARVO, L"Arvo", L"Arvo", 600,0,0, 1,                   	// 7: arvo
	F_LISNO, L"Lisenssi", L"Lisenssi", 600,0, 1, 1,              // 8: lisenssi
	F_BADGE, L"Badge", L"Badge", 600,0, 1, 1,                 // 9: badge
	F_RATA, L"Rata", L"Rata", 600,0, 0, 1,                 // 9: badge
	F_OSSRA, L"OsSeura", L"OsSeura", 600,0, 0, 1,              	// 10: seura
	F_TLS, L"Tulos", L"Tulos", 650,10, 1, 1,                 	// 11: tulos
	F_ERO, L"Ero", L"Ero",  730,10, 1, 1,                  	// 12: ero k‰rkeen
	F_SAK, L"Sakot", L"Sakot",730,0, 1, 1,                   	// 13: sakot
	F_TARK, L"Hyv‰ks.", L"T", 835, 0, 0, 1,               // 14: Leimantarkastus
	F_PIST, L"Pisteet", L"Pisteet", 840, 0, 1, 1,              	// 15: pisteet
	F_OSASJ, L"Os-sija", L"OsSj", 825, 0, 1, 1,              // 16: osuuden tai v‰liajan sija
	F_OSATLS, L"Os-tulos", L"OsTulos", 850, 8, 1, 1,            // 17: osuuden tulos tai v‰liaika
	F_SEURSJ, L"Seur.sj.", L"Sj.", 860, 0, 0, 1,       		// 18: seuraava osatulos
	0, 0, 0, 0, 0, 0
	};

FldFrmtTp prtfldsk[] = {
	F_SJ, L"Sija", L"Sija", 0, 4, 1, 2,                   	// 0: sija
	F_KNO, L"Nro", L"Nro", 50, 4, 1, 2,                   	// 1: numero
	F_JOUK, L"Joukkue", L"Joukkue", 100, 50, 0, 2,                 // 2: joukkue
	F_MAA, L"Maa", L"Maa", 440, 3, 0, 2,                   	// 3: maa
	F_JOUKNM, L"Jk-nimi", L"Jk-nimi", 345,0, 0, 2,               // 4: joukkue
	F_JOUKID, L"Jk-id", L"Jk-id", 345,0, 0, 2,                 // 5: joukkue
	F_NIMI, L"Nimi", L"Nimi", 150,40,0, 1,                  	// 6: nimi
	F_ARVO, L"Arvo", L"Arvo", 400,0,0, 1,                   	// 7: arvo
	F_LISNO, L"Lisenssi", L"Lisenssi", 400,0, 1, 1,              // 8: lisenssi
	F_BADGE, L"Badge", L"Badge", 400,0, 1, 1,                 // 9: badge
	F_RATA, L"Rata", L"Rata", 450,0, 0, 1,                 // 9: badge
	F_OSSRA, L"OsSeura", L"OsSeura", 400,0, 0, 1,              	// 10: seura
	F_TLS, L"Tulos", L"Tulos", 590,10, 1, 3,                 	// 11: tulos
	F_ERO, L"Ero", L"Ero",  710,0, 1, 3,                  	// 12: ero k‰rkeen
	F_SAK, L"Sakot", L"Sakot",730,0, 1, 1,                   	// 13: sakot
	F_TARK, L"Hyv‰ks.", L"T", 840, 0, 0, 1,               // 14: Leimantarkastus
	F_PIST, L"Pisteet", L"Pisteet", 840, 0, 1, 1,              	// 15: pisteet
	F_OSASJ, L"Os-sija", L"OsSj", 770, 4, 1, 1,              // 16: osuuden tai v‰liajan sija
	F_OSATLS, L"Os-tulos", L"OsTulos", 810,10, 1, 1,            // 17: osuuden tulos tai v‰liaika
	F_OSOTS, L"Os-ots", L"Osuus", 50, 8, 0, 1,       		    // 18: osuuden otsikko
	F_VSJ, L"Vaihtosj", L"VaihtoSj", 540, 4, 1, 1,                // : sija vaihdossa
	0, 0, 0, 0, 0, 0
	};

GDIfontTp CurrFnt = {
	0,  // fontfamily = TimesNewRoman
	0,	 // pitch
	12, // size
	0,	 // bold
	0,	 // italic
	0,  // numwidth
	12, // line = Linespacing
	0,  // chwidth = merkki‰ rivill‰
	0,  // rivej‰ sivulla
	0}; // hfont
GDIfontTp DefFnt = {
	0,  // fontfamily = TimesNewRoman
	0,	 // pitch
	12, // size
	0,	 // bold
	0,	 // italic
	0,  // numwidth
	12, // line = Linespacing
	0,  // chwidth = merkki‰ rivill‰
	0,  // rivej‰ sivulla
	0}; // hfont
GDIfontTp TitleFnt = {
	0,  // fontfamily = TimesNewRoman
	0,	 // pitch
	16, // size
	0,	 // bold
	0,	 // italic
	0,  // numwidth
	16, // line = Linespacing
	0,  // chwidth = merkki‰ rivill‰
	0,  // rivej‰ sivulla
	0}; // hfont
GDIfontTp HeaderFnt = {
	0,  // fontfamily = TimesNewRoman
	0,	 // pitch
	12, // size
	0,	 // bold
	0,	 // italic
	0,  // numwidth
	12, // line = Linespacing
	0,  // chwidth = merkki‰ rivill‰
	0,  // rivej‰ sivulla
	0}; // hfont
GDIfontTp BoldFnt = {
	0,  // fontfamily = TimesNewRoman
	0,	 // pitch
	12, // size
	1,	 // bold
	0,	 // italic
	0,  // numwidth
	12, // line = Linespacing
	0,  // chwidth = merkki‰ rivill‰
	0,  // rivej‰ sivulla
	0}; // hfont
GDIfontTp SmallFnt = {
	0,  // fontfamily = TimesNewRoman
	0,	 // pitch
	10, // size
	0,	 // bold
	0,	 // italic
	0,  // numwidth
	12, // line = Linespacing
	0,  // chwidth = merkki‰ rivill‰
	0,  // rivej‰ sivulla
	0}; // hfont
GDIfontTp CourFnt = {
	4,  // fontfamily = CourierNew
	10,	 // pitch
	0, // size
	0,	 // bold
	0,	 // italic
	0,  // numwidth
	12, // line = Linespacing
	0,  // chwidth = merkki‰ rivill‰
	0,  // rivej‰ sivulla
	0}; // hfont
GDIfontTp RastiVaHeaderFnt = {
	1,  // fontfamily = Arial
	0,	 // pitch
	14, // size
	0,	 // bold
	0,	 // italic
	0,  // numwidth
	14, // line = Linespacing
	0,  // chwidth = merkki‰ rivill‰
	0,  // rivej‰ sivulla
	0}; // hfont
GDIfontTp RastiVaFnt = {
	1,  // fontfamily = Arial
	0,	 // pitch
	10, // size
	0,	 // bold
	0,	 // italic
	0,  // numwidth
	10, // line = Linespacing
	0,  // chwidth = merkki‰ rivill‰
	0,  // rivej‰ sivulla
	0}; // hfont

tulosmuottp tulosmuot;
/* = {
	1,   // snimifl;
	1,	 // knofl;
	1,   // tpvafl;
	1,   // tklofl;
	1,   // lkmfl;
	0,   // otsikot;
	-1,   // etusuku;
	0,   // nollakarki;
	20,   // marg;
	20,   // omarg;
	30,   // ymarg;
	30,   // amarg;
	0,   // tabs;
	1,   // numlev;
	0,	 // levunit;
	60,  // sivpit;
	200, // rivpit
	1,	 // aikapari
	0,   // uusisivu
	0,   // esirivit;
	120, // tiiv_len
	0,	 // tiiv_rivit
	0,	 // tiiv_fontit
	1,	 // tiiv_osno
	2,	 // sarjalinkit; // 0: Ei, 1: on, 2: toistuu
	1, 	 // tauluhtml;
	1, 	 // fontfamily;
	1, 	 // fontsize;
	0 	 // cssfl;
	};
*/
tulosmuottp filetulosmuot;
/* = {
	1,   // snimifl;
	1,	 // knofl;
	1,   // tpvafl;
	1,   // tklofl;
	1,   // lkmfl;
	0,   // otsikot;
	-1,   // etusuku;
	0,   // nollakarki;
	0,   // marg;
	0,   // omarg;
	0,   // ymarg;
	0,   // amarg;
	0,   // tabs;
	1,   // numlev;
	0,	 // levunit;
	99999, // sivpit;
	999, // rivpit
	1,	 // aikapari
	0,   // uusisivu
	0,   // esirivit;
	250, // tiiv_len
	0,	 // tiiv_rivit
	0,	 // tiiv_fontit
	0,	 // tiiv_osno
	0,	 // tiiv_arvo
	2,	 // sarjalinkit; // 0: Ei, 1: on, 2: toistuu
	1, 	 // tauluhtml;
	1, 	 // fontfamily;
	1, 	 // fontsize;
	0 	 // cssfl;
	};
*/

wchar_t init_fnt[PRKOODIPIT+1];
wchar_t pots_on[PRKOODIPIT+1];
wchar_t pots_pois[PRKOODIPIT+1];
wchar_t aots_on[PRKOODIPIT+1];
wchar_t aots_pois[PRKOODIPIT+1];
wchar_t lih_on[PRKOODIPIT+1];
wchar_t lih_pois[PRKOODIPIT+1];
wchar_t pienet_on[PRKOODIPIT+1];
wchar_t pienet_pois[PRKOODIPIT+1];

INT n_prtflds = sizeof(prtflds)/sizeof(FldFrmtTp)-1;
INT n_prtfldsk = sizeof(prtfldsk)/sizeof(FldFrmtTp)-1;
int n_dspflds = sizeof(dspflds)/sizeof(FldFrmtTp)-1;
int n_fileflds = sizeof(fileflds)/sizeof(FldFrmtTp)-1;
int n_filefldsk = sizeof(filefldsk)/sizeof(FldFrmtTp)-1;

wchar_t paaots[125] = L"";
wchar_t sarjaots[125] = L"";
wchar_t alaots[141] = L"";

wchar_t initfont_emrap[PRKOODIPIT+1] =
   L"\x26\x1b&l26A\x1b&l0O\x1b(s0p10h0s0b4099T\x1b&l8C\x1b(10U";
wchar_t initfont_emva[PRKOODIPIT+1] =
   L"\x26\x1b&l26A\x1b&l1O\x1b(s0p15h0s0b4099T\x1b&l6C\x1b(10U";

INT valinelendef, vapagelendef, vapageskip, tarkrappageskip;
INT valinelen = 75, vapagelen = 55;

tulosmuottp::tulosmuottp(void)
{
	nollaa();
}

tulosmuottp::~tulosmuottp(void)
{

}

void tulosmuottp::operator=(tulosmuottp& muot)
{
	memcpy(this, &muot, sizeof(tulosmuottp));
}

void tulosmuottp::nollaa(void)
{
	memset(this, 0, sizeof(tulosmuottp));
	snimifl = 1;       // Ei k‰ytˆss‰
	knofl = 1;
	tpvafl = 1;
	tklofl = 1;
	lkmfl = 1;         // Ei k‰ytˆss‰
	otsikot = 0;
	etusuku = -1;
	nollakarki = 0;    // Ei k‰ytˆss‰
	marg = 0;    // Kirjoitin : 20
	omarg = 20;
	ymarg = 30;
	amarg = 30;
	tabs = 0;
	numlev = 1;
	levunit = 0;  // 0: merkkej‰, 1: poins, 2: 1/1000 rivi
	sivpit = 99999;   // Kirjoitin : 60
	rivpit = 999;     // Kirjoitin: 200
	tiiv_len = 250;   // Kirjoitin: 120
	tiiv_rivit = 0;
	tiiv_fontit = 0;
	tiiv_osno = 0;    // Kirjoitin: 1
	tiiv_arvo = 0;
	aikapari = 1;
	esirivit = 0;
	sarjalinkit = 2; // 0: Ei, 1: kerran, html : 2: toistuu
	tauluhtml = 6;  // 1: taulukko, 2: leveydet, 4: raastiva-leveydet
	riviryhma = 3;
	fontfamily = 0;
	fontsize = 1;
//	des_sep = L',';
	paaotskoko = 150;
	alaotskoko = 120;
	border = 0;
	bordercolor = 0xd8d8d8;
	vpad = 1;
	hpad = 5;
	sarotsbold = 1;
	bgcolor = 0xffffff;
	fontcolor = 0;
	thcolor = 0xffffff;
	thfontcolor = 0;
	td1color = 0xffffff;
	td1fontcolor = 0;
	td2color = 0xffffff;
	td2fontcolor = 0;
	tdjkcolor = 0xffffff;
	tdjkfontcolor = 0;
	framecolor = 0xe8ffff;
	framefontcolor = 0;
	eborder = 1;
	efontsize = 80;
	thecolor = 0xd0ffff;
	thefontcolor = 0;
	tdecolor = 0xe8ffff;
	tdefontcolor = 0;
	levkerr = 8;
}

void initHtmlHeader(bool pakota)
{
   if (!lsttulparam.title[0] || pakota) {
	  if (paaots[0])
		 wcsncpy(filetulparam.title, paaots, 120);
	  else if (sarjaots[0])
		 wcsncpy(filetulparam.title, sarjaots, 120);
	  else
		 wcsncpy(filetulparam.title, kilpailu, 120);
	  }
   filetulparam.title[120] = 0;
   elimwbl(lsttulparam.title);
  if (!filetulparam.header[0] || pakota) {
	 if (paaots[0])
		wcsncpy(filetulparam.header, paaots, 120);
	 else if (sarjaots[0])
		wcsncpy(filetulparam.header, sarjaots, 120);
	  else
		 wcsncpy(filetulparam.header, kilpailu, 120);
	 }
  filetulparam.header[120] = 0;
  elimwbl(filetulparam.header);
  if (!pakota) {
	wcscpy(aftulparam.title, filetulparam.title);
	wcscpy(aftulparam.header, filetulparam.header);
  }
}

void init_muotoilut(void)
{
	tulosmuot.sivpit = 60;
	tulosmuot.rivpit = 200;
	tulosmuot.tiiv_len = 120;
	tulosmuot.tiiv_osno = 1;
	tulosmuot.marg = 20;
}

INT tulkhp(wchar_t *fnt, wchar_t *pap, wchar_t *as, wchar_t *msto, wchar_t *extra,
   INT *mlj, INT *mtasm, INT *mkoko, INT *mrivi)
   {
   wchar_t *p, *pa, *ps, *pn, alku[30], st[30] = L"", numst[10];
   INT jfl = 0, n, muu;

   pa = alku;
   if (mtasm) *mtasm = 0;
   if (extra) extra[0] = 0;
   for (p = fnt+1; *p; p++) {
      if (*p == ESC) {
		 if (extra && st[0]) wcscat(extra, st);
		 memset(alku, 0, sizeof(alku));
		 memset(st, 0, sizeof(st));
		 pa = alku;
         ps = st;
         *(pa++) = *p;
         *(ps++) = *p;
         jfl = 1;
         continue;
		 }
      switch (jfl) {
         case 0:
			continue;
         case 1:
            switch (*p) {
			   case L'(':
				  jfl = 2;
				  break;
			   case L'&':
				  jfl = 3;
				  break;
			   default:
				  jfl = 9;
			   }
			*(pa++) = *p;
			*(ps++) = *p;
			break;
		 case 2:
			if (*p == L's') {
			   jfl = 21;
			   *(pa++) = *p;
			   *(ps++) = *p;
               break;
			   }
			pn = numst;
			memset(numst, 0, sizeof(numst));
			while (*p >= L'0' && *p <= L'9') {
			   *(ps++) = *p;
			   *(pn++) = *p;
			   p++;
			   }
			muu = 1;
			if (numst[0]) {
			   n = _wtoi(numst);
			   muu = 0;
			   switch (*p) {
				  case L'u':
				  case L'U':
					 if (msto) {
						switch (n) {
						   case 8:
							  msto[0] = L'R';
							  break;
						   case 10:
						   case 12:
							  msto[0] = L'P';
							  break;
						   case 9:
						   case 19:
							  msto[0] = L'L';
							  break;
						   default:
							  msto[0] = L'R';
							  break;
						   }
						}
					 break;
				  default:
					 muu = 1;
				  }
			   }
			if (muu) {
			   wcscat(st, numst);
			   ps = st + wcslen(st);
			   *(ps++) = towupper(*p);
			   }
			else st[0] = 0;
			if (*p <= L'Z') jfl = 0;
			else {
			   wcscpy(st, alku);
			   ps = st + wcslen(ps);
			   }
			break;
		 case 21:
			pn = numst;
			memset(numst, 0, sizeof(numst));
			while (*p >= L'0' && *p <= L'9') {
			   *(ps++) = *p;
			   *(pn++) = *p;
			   p++;
			   }
			muu = 1;
			if (numst[0]) {
			   n = _wtoi(numst);
			   muu = 0;
               switch (*p) {
				  case L'p':
                  case L'P':
					 break;
				  case L'h':
				  case L'H':
                     if (mkoko) *mkoko = 120/n;
					 break;
                  case L'v':
				  case L'V':
					 if (mkoko) *mkoko = n;
					 break;
				  case L's':
				  case L'S':
					 if (mtasm) *mtasm += 2*n;
					 break;
                  case L'b':
				  case L'B':
					 if (mtasm && n) *mtasm += 1;
                     break;
				  case L't':
                  case L'T':
                     if (mlj) *mlj = n;
					 break;
                  default:
					 muu = 1;
				  }
			   }
			if (muu) {
			   wcscat(st, numst);
			   ps = st + wcslen(st);
			   *(ps++) = towupper(*p);
			   }
			else st[0] = 0;
			if (*p <= L'Z') jfl = 0;
			else {
			   wcscpy(st, alku);
			   ps = st + wcslen(ps);
               }
            break;
		 case 3:
            *(pa++) = *p;
            *(ps++) = *p;
			if (*p == L'l') {
			   p++;
			   pn = numst;
			   memset(numst, 0, sizeof(numst));
			   while (*p >= L'0' && *p <= L'9') {
				  *(ps++) = *p;
                  *(pn++) = *p;
				  p++;
				  }
			   muu = 1;
			   if (numst[0]) {
				  n = _wtoi(numst);
				  muu = 0;
				  switch (*p) {
					 case L'a':
					 case L'A':
						if (pap) {
						   if (n == 27) wcscpy(pap, L"A3");
						   else wcscpy(pap, L"A4");
                           }
                        break;
					 case L'c':
                     case L'C':
                        if (mrivi) *mrivi = n;
						break;
					 case L'd':
                     case L'D':
						if (mrivi) *mrivi = 48/n;
						break;
                     case L'o':
                     case L'O':
                        if (as) as[0] = (n & 1) ? L'V' : L'P';
						break;
                     default:
                        muu = 1;
					 }
                  }
               if (muu) {
				  wcscat(st, numst);
				  ps = st + wcslen(st);
				  *(ps++) = towupper(*p);
				  }
			   else st[0] = 0;
			   if (*p <= L'Z') jfl = 0;
			   else {
				  wcscpy(st, alku);
				  ps = st + wcslen(ps);
				  }
			   }
			break;
		 case 9:
			*(ps++) = *p;
			*(pa++) = *p;
			break;
		 }
	  }
   if (extra && !jfl && st[0]) wcscat(extra, st);
   return(0);
   }

#if defined(EMITLEIMAT)
void vasivu(void)
   {
   INT koko, rv;
   wchar_t vaas[2] = L"V", pap[3] = L"A4";

   tulkhp(init_fnt, pap, NULL, NULL, NULL, NULL, NULL, NULL, NULL);
   tulkhp(initfont_emva, NULL, vaas, NULL, NULL, NULL, NULL, &koko, &rv);
   if (vaas[0] == L'V') {
	  valinelen = 1345/koko;
	  vapagelen = 325/rv;
	  }
   else {
	  valinelen = 950/koko;
	  vapagelen = 455/rv;
	  }
   if (pap[1] == L'3') {
	  valinelen = (7*valinelen)/5;
	  vapagelen = (7*vapagelen)/5;
	  }
	if (vapagelendef) {
		vapagelen = vapagelendef;
		}
	if (valinelendef) {
		valinelen = valinelendef;
		}
   valinelen -= valinelen % 13;
	vapagelen -= vapageskip;
   }
#endif

void lueGDIfont(TextFl* luetfmtf, GDIfontTp *fnt)
	{
	wchar_t Buf[100];
	GDIfontTp Fnt0;

	luetfmtf->ReadLine(Buf, 100);
	Fnt0 = *fnt;
	swscanf(Buf, L"%d %d %d %d %d %d\n",
		&fnt->fontfamily, &fnt->pitch, &fnt->size,
		&fnt->bold, &fnt->italic, &fnt->line);
	if (memcmp(fnt, &Fnt0, sizeof(Fnt0)))
		fnt->hfont = NULL;
	}

void tallGDIfont(TextFl* luetfmtf, GDIfontTp *fnt)
	{
	wchar_t Buf[100];
	swprintf(Buf, L"%d %d %d %d %d %d\n",
		fnt->fontfamily, fnt->pitch, fnt->size,
		fnt->bold, fnt->italic, fnt->line);
	luetfmtf->WriteLine(Buf);
	}

void luekoodi(TextFl* luetfmtf, wchar_t *koodi)
   {
   INT i, l;
   wchar_t s[202];
   wchar_t *p;

   memset(koodi, 0, PRKOODIPIT);
   luetfmtf->ReadLine(s, 200);
   p = wcstok(s, L" \n");
   l = _wtoi(p);
   l = min(l,PRKOODIPIT-1);
   koodi[0] = (wchar_t) l;
   for (i = 1; i <= l; i++) {
	  if ((p = wcstok(NULL, L" \n")) == NULL) break;
	  koodi[i] = (wchar_t) _wtoi(p);
	  }
   }

void tallkoodi(TextFl* luetfmtf, wchar_t *koodi)
   {
   INT i;
   wchar_t s[202];
   wchar_t *p;

   _itow(koodi[0], s, 10);
   for (i = 1; i <= koodi[0]; i++) {
	  p = s + wcslen(s);
	  *p = L' ';
	  _itow(koodi[i], p+1, 10);
	  }
   wcscat(s, L"\n");
   luetfmtf->WriteLine(s);
   }

void lue_muotoilu(wchar_t *luetfnm, int kysy)
   {
   INT j,nfld,prnt, tt, ps, us;
   wchar_t ch, *p, ln[102];
   FldFrmtTp *fld;
   TextFl *luetfmtf = NULL;

   if (kysy) {
      ch = L' ';
	  wselectopt(L"Luetaanko muotoilu levylt‰ (K/E)", L"KE", &ch);
      clrln(ySize-3);
	  if (ch == L'K') {
         vidspwmsg(ySize-3,0,7,0,L"Tiedoston nimi:");
         for (;;) {
#ifdef _CONSOLE
			inputwstr(luetfnm, 39, 16, ySize-3, L"\r\x1b", &ch, FALSE);
			clrln(ySize-1);
#else
			 inputstr_prompt(luetfnm, 39, L"Tiedoston nimi", &ch);
#endif
			if (ch == ESC)
				return;
		    luetfmtf = new TextFl(luetfnm,L"r");
			if (!luetfmtf->IsOpen()) {
				delete luetfmtf;
				luetfmtf = NULL;
			   erbeep();
			   vidspwmsg(ySize-1,0,0,7,L"EI L÷YDY!");
               }
			else break;
            }
		 }
	  else
		  return;
      }
   else {
	   luetfmtf = new TextFl(luetfnm,L"r");
		if (!luetfmtf->IsOpen()) {
			delete luetfmtf;
			writeerror_w(L"Tulostusmuotoilutiedostoa ei lˆydy!",0, true);
			return;
			}
		}

	  if (luetfmtf->ReadLine(ln, 100)) {
		  tt = -1;
		 if(swscanf(ln,L"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", &prnt,
			&us, &tulosmuot.sivpit, &tulosmuot.tpvafl, &tulosmuot.tklofl, &ps, &tulosmuot.numlev,
			&tulosmuot.marg, &tulosmuot.omarg, &tulosmuot.ymarg, &tulosmuot.amarg,
			&tulosmuot.snimifl, &tulosmuot.etusuku, &tulosmuot.esirivit,
			&tulosmuot.otsikot, &tulosmuot.tiiv_len, &tulosmuot.tiiv_rivit,
			&tulosmuot.tiiv_fontit, &tulosmuot.tiiv_osno, &tt) < 15) {
			iowerr(luetfnm,1, 1L);
			goto sulje;
			}
		 }
	  else {
		 iowerr(luetfnm,1, 1L);
		 goto sulje;
		 }
	  if (luetfmtf->ReadLine(ln, 100)) {
		  tt = -1;
		 if(swscanf(ln,L"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
			&filetulosmuot.sivpit, &filetulosmuot.tpvafl, &filetulosmuot.tklofl, &ps, &filetulosmuot.numlev,
			&filetulosmuot.marg, &filetulosmuot.omarg, &filetulosmuot.ymarg, &filetulosmuot.amarg,
			&filetulosmuot.snimifl, &filetulosmuot.etusuku, &filetulosmuot.esirivit,
			&filetulosmuot.otsikot, &filetulosmuot.tiiv_len, &filetulosmuot.tiiv_rivit,
			&filetulosmuot.tiiv_fontit, &filetulosmuot.tiiv_osno) < 12) {
			iowerr(luetfnm,1, 1L);
			goto sulje;
			}
		 }
	  else {
		 iowerr(luetfnm,1, 1L);
		 goto sulje;
		 }
		lsttulparam.tulmuot = tulosmuot;
#ifdef _CONSOLE
		if (prnt == GDIPRINTER && lsttulparam.printer != prnt) {
//			writeerror_w(L"Windows kirjoitinajurin muotoilu ohitetaan", 0);
//			return;
			}
		if (lsttulparam.printer == GDIPRINTER && lsttulparam.printer != prnt) {
			writeerror_w(L"Muotoilu ei sovi Windows kirjoitinajurille", 0);
			return;
			}
		lsttulparam.printer = prnt;
#else
		if (prnt != GDIPRINTER) {
			writeerror_w(L"Muotoilu ei sovi Windows kirjoitinajurille", 0);
			return;
			}
#endif
	  fld = prtflds;
	  nfld = sizeof(prtflds)/sizeof(FldFrmtTp)-1;
	  for (;;) {
		 for (j = 0; j < nfld; j++) {
			if (luetfmtf->ReadLine(ln, 100)) {
				if (swscanf(ln, L"%s %d %d %d\n", fld[j].name,
				   &fld[j].pos, &fld[j].len, &fld[j].oik) != 4) {
				   iowerr(luetfnm,1, 1L);
				   goto sulje;
				   }
				for (p = fld[j].name; *p; p++)
				   if (*p == L'_') *p = L' ';
				}
			}
		 if (fld == dspflds) break;
		 if (fld == prtflds) {
			fld = prtfldsk;
			nfld = sizeof(prtfldsk)/sizeof(FldFrmtTp)-1;
			continue;
			}
		 if (fld == prtfldsk) {
			fld = fileflds;
			nfld = sizeof(fileflds)/sizeof(FldFrmtTp)-1;
			continue;
			}
		 if (fld == fileflds) {
			fld = filefldsk;
			nfld = sizeof(filefldsk)/sizeof(FldFrmtTp)-1;
			continue;
			}
		 fld = dspflds;
		 nfld = sizeof(dspflds)/sizeof(FldFrmtTp)-1;
		 }
#ifdef _CONSOLE
		if (lsttulparam.printer == GDIPRINTER) {
#endif
			lueGDIfont(luetfmtf, &DefFnt);
			lueGDIfont(luetfmtf, &TitleFnt);
			lueGDIfont(luetfmtf, &HeaderFnt);
			lueGDIfont(luetfmtf, &BoldFnt);
			lueGDIfont(luetfmtf, &SmallFnt);
			lueGDIfont(luetfmtf, &CourFnt);
		// Seuraava rivi lukee ylim. rivin
		  if (luetfmtf->ReadLine(ln, 100) == NULL) {
			  iowerr(luetfnm, 1, 2L);
			   goto sulje;
				}
#ifdef _CONSOLE

			}
		else {
		 luekoodi(luetfmtf, init_fnt);
		 luekoodi(luetfmtf, pots_on);
		 luekoodi(luetfmtf, pots_pois);
		 luekoodi(luetfmtf, aots_on);
		 luekoodi(luetfmtf, aots_pois);
		 luekoodi(luetfmtf, lih_on);
		 luekoodi(luetfmtf, lih_pois);
		 luekoodi(luetfmtf, pienet_on);
		 luekoodi(luetfmtf, pienet_pois);
#ifdef MAKI
			luekoodi(luetfmtf, atxt_on);
			luekoodi(luetfmtf, atxt_pois);
#else
		 luekoodi(luetfmtf, initfont_emva);
#endif
		 }
#endif
	  if (luetfmtf->ReadLine(paaots, 82) == NULL) {
		 iowerr(luetfnm, 1, 2L);
		 goto sulje;
		 }
	  paaots[wcslen(paaots)-2] = 0;
	  if (luetfmtf->ReadLine(sarjaots, 82) == NULL) {
		 iowerr(luetfnm, 1, 3L);
		 goto sulje;
		 }
	  sarjaots[wcslen(sarjaots)-2] = 0;
	  if (luetfmtf->ReadLine(alaots, 82) == NULL) {
		 iowerr(luetfnm, 1, 2L);
		 goto sulje;
		 }
	  alaots[wcslen(alaots)-2] = 0;

sulje:
   if (luetfmtf)
	  delete luetfmtf;

//   if (pitkaseura && lue_sr_nimet(1)) {
//	  pitkaseura = 0;
//	  pitkaseuram = 0;
//	  }
	scale = 10;
#ifdef _CONSOLE
   if (lsttulparam.printer == POSTSCRIPT || lsttulparam.printer == LJETII) scale = 7;
	else if (lsttulparam.printer == GDIPRINTER)
		scale = 10;
   else scale = 1;
   if (lsttulparam.printer == LJETII) {
#if defined(EMITLEIMAT)
	  vasivu();
#endif
//	  if (init_fnt[0] && !tulkhp(init_fnt, 0, 0, 0, 0, &j, 0, 0, 0))
//		 suhtfont = (j == 4099) ? 0 : 1;
	  }
#endif
//	muotoiluluettu = 1;
   }

/*
void tall_muotoilu(wchar_t *luetfnm) {
   wchar_t *p, Buf[400], term[] = L"+\r\t\x0b\x05\x18\x1b";
   INT j,nfld;
   FldFrmtTp *fld;
   TextFl *luetfmtf;
   int printer = GDIPRINTER;

#ifdef _CONSOLE
	printer = lsttulparam.printer;
#endif
   luetfmtf = new TextFl(luetfnm,L"wt");
   if (!luetfmtf->IsOpen()) {
	  delete luetfmtf;
	  writeerror_w(L"Tulostusmuotoilutiedoston kirjoitaminen ei onnistu!",0);
	  return;
	  }
   else {
	  swprintf(Buf,L"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", printer,
		 1, tulosmuot.sivpit, tulosmuot.tpvafl, tulosmuot.tklofl, 0, tulosmuot.numlev,
		 tulosmuot.marg, tulosmuot.omarg, tulosmuot.ymarg, tulosmuot.amarg,
		 tulosmuot.snimifl, tulosmuot.etusuku, tulosmuot.esirivit,
		 tulosmuot.otsikot, tulosmuot.tiiv_len, tulosmuot.tiiv_rivit,
		 tulosmuot.tiiv_fontit, tulosmuot.tiiv_osno);
	  luetfmtf->WriteLine(Buf);
	  swprintf(Buf,L"%d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d\n",
		 filetulosmuot.sivpit, filetulosmuot.tpvafl, filetulosmuot.tklofl, 0, filetulosmuot.numlev,
		 filetulosmuot.marg, filetulosmuot.omarg, filetulosmuot.ymarg, filetulosmuot.amarg,
		 filetulosmuot.snimifl, filetulosmuot.etusuku, filetulosmuot.esirivit,
		 filetulosmuot.otsikot, filetulosmuot.tiiv_len, filetulosmuot.tiiv_rivit,
		 filetulosmuot.tiiv_fontit, filetulosmuot.tiiv_osno);
	  luetfmtf->WriteLine(Buf);
	  fld = prtflds;
	  nfld = sizeof(prtflds)/sizeof(FldFrmtTp)-1;
	  for (;;) {
		 for (j = 0; j < nfld; j++) {
			for (p = fld[j].name; *p; p++)
			   if (*p == L' ')
					*p = L'_';
			swprintf(Buf, L"%s %d %d %d\n", fld[j].name,
			   fld[j].pos, fld[j].len, fld[j].oik);
			luetfmtf->WriteLine(Buf);
			}
		 if (fld == dspflds) break;
		 if (fld == prtflds) {
			fld = prtfldsk;
			nfld = sizeof(prtfldsk)/sizeof(FldFrmtTp)-1;
			continue;
			}
		 if (fld == prtfldsk) {
			fld = fileflds;
			nfld = sizeof(fileflds)/sizeof(FldFrmtTp)-1;
			continue;
			}
		 if (fld == fileflds) {
			fld = filefldsk;
			nfld = sizeof(filefldsk)/sizeof(FldFrmtTp)-1;
			continue;
			}
		 fld = dspflds;
		 nfld = sizeof(dspflds)/sizeof(FldFrmtTp)-1;
		 }
#ifdef _CONSOLE
		if (lsttulparam.printer == GDIPRINTER) {
#endif
			tallGDIfont(luetfmtf, &DefFnt);
			tallGDIfont(luetfmtf, &TitleFnt);
			tallGDIfont(luetfmtf, &HeaderFnt);
			tallGDIfont(luetfmtf, &BoldFnt);
			tallGDIfont(luetfmtf, &SmallFnt);
			tallGDIfont(luetfmtf, &CourFnt);
			// Seuraava rivi kirjoittaa ylim. rivin
			luetfmtf->WriteLine(L"(vararivi)\n");
#ifdef _CONSOLE
			}
		else {
		 tallkoodi(luetfmtf, init_fnt);
		 tallkoodi(luetfmtf, pots_on);
		 tallkoodi(luetfmtf, pots_pois);
		 tallkoodi(luetfmtf, aots_on);
		 tallkoodi(luetfmtf, aots_pois);
		 tallkoodi(luetfmtf, lih_on);
		 tallkoodi(luetfmtf, lih_pois);
		 tallkoodi(luetfmtf, pienet_on);
		 tallkoodi(luetfmtf, pienet_pois);
		 tallkoodi(luetfmtf, initfont_emva);
		 }
#endif
	  swprintf(Buf,L"%s*\n%s*\n%s*\n",paaots,sarjaots,alaots);
	  luetfmtf->WriteLine(Buf);
	  }
   delete luetfmtf;
   }
*/

#define FRM_Muotoilu    	100
#define FRM_TulosLuettelot	101
#define FRM_Kirjasinlajit	103
#define FRM_TulosLuettelo	104
#define FRM_Kentta        	106
#define FRM_Nimi        	111
#define FRM_Muuttuja		112
#define FRM_Otsikko     	113
#define FRM_Sijainti    	114
#define FRM_Merkkeja    	115
#define FRM_Leveys        	116
#define FRM_Tasaus        	117
#define FRM_Aktiivinen 		118
#define FRM_Kohde			119
#define FRM_ToistaSarja		201
#define FRM_Paivays         202
#define FRM_TulostusAika    203
#define FRM_SarakeOtsikot   204
#define FRM_EtuSuku         205
#define FRM_NollaKarki      206
#define FRM_VasMarg         207
#define FRM_OikMarg         208
#define FRM_YlaMarg         209
#define FRM_AlaMarg         210
#define FRM_Tabs            211
#define FRM_NumLeveys       212
#define FRM_SivuPit         214
#define FRM_EsiRivit        215
#define FRM_TiivPituus      216
#define FRM_TiivRivit       217
#define FRM_TiivFontit      218
#define FRM_TiivOsuusNo     219
#define FRM_TiivArvo     	220
#define FRM_PaaOts          221
#define FRM_SarjaOts        222
#define FRM_AlaOts          223
#define FRM_Lukumaarat      224
#define FRM_Linkit      	225
#define FRM_TaulHtml      	226
#define FRM_RiviRyhma      	227
#define FRM_SarLevKerroin   228
#define FRM_Kirjasin		309
#define FRM_FontFamily		310
#define FRM_Tuumalla   		311
#define FRM_Koko            312
#define FRM_Lihav           313
#define FRM_Kursiv          314
#define FRM_Rivivali        315
#define FRM_Vari            316
#define FRM_Viiva           317
#define FRM_Css             318

static tagListTp FrmTags[] = {
	{FRM_Muotoilu, L"Muotoilu"},
	{FRM_TulosLuettelot, L"TulosLuettelot"},
	{FRM_Kirjasinlajit, L"Kirjasinlajit"},
	{FRM_TulosLuettelo, L"TulosLuettelo"},
	{FRM_Kentta, L"Kentta"},
	{FRM_Nimi, L"Nimi"},
	{FRM_Muuttuja, L"Muuttuja"},
	{FRM_Otsikko, L"Otsikko"},
	{FRM_Sijainti, L"Sijainti"},
	{FRM_Leveys, L"Leveys"},
	{FRM_Tasaus, L"Tasaus"},
	{FRM_Aktiivinen, L"Aktiivinen"},
	{FRM_Kohde, L"Kohde"},
	{FRM_ToistaSarja, L"ToistaSarja"},
	{FRM_Paivays, L"Paivays"},
	{FRM_TulostusAika, L"TulostusAika"},
	{FRM_SarakeOtsikot, L"SarakeOtsikot"},
	{FRM_EtuSuku, L"EtuSuku"},
	{FRM_NollaKarki, L"NollaKarki"},
	{FRM_VasMarg, L"VasMarg"},
	{FRM_OikMarg, L"OikMarg"},
	{FRM_YlaMarg, L"YlaMarg"},
	{FRM_AlaMarg, L"AlaMarg"},
	{FRM_Tabs, L"Tabs"},
	{FRM_NumLeveys, L"NumLeveys"},
	{FRM_SivuPit, L"SivuPit"},
	{FRM_EsiRivit, L"EsiRivit"},
	{FRM_TiivPituus, L"TiivPituus"},
	{FRM_TiivRivit, L"TiivRivit"},
	{FRM_TiivFontit, L"TiivFontit"},
	{FRM_TiivOsuusNo, L"TiivOsuusNo"},
	{FRM_TiivArvo, L"TiivArvo"},
	{FRM_PaaOts, L"PaaOts"},
	{FRM_SarjaOts, L"SarjaOts"},
	{FRM_AlaOts, L"AlaOts"},
	{FRM_Lukumaarat, L"Lukumaarat"},
	{FRM_Linkit, L"Linkit"},
	{FRM_TaulHtml, L"TaulHtml"},
	{FRM_RiviRyhma, L"RiviRyhma"},
	{FRM_SarLevKerroin, L"SarLevKerroin"},
	{FRM_Kirjasin, L"Kirjasin"},
	{FRM_FontFamily, L"FontFamily"},
	{FRM_Tuumalla, L"Tuumalla"},
	{FRM_Koko, L"Koko"},
	{FRM_Lihav, L"Lihav"},
	{FRM_Kursiv, L"Kursiv"},
	{FRM_Rivivali, L"Rivivali"},
	{FRM_Vari, L"Vari"},
	{FRM_Viiva, L"Viiva"},
	{FRM_Css, L"CssTiedot"}
	};

static wchar_t *FRM_Kohteet[] = {L"None", L"Epson", L"HPLaserJet", L"Postscript", L"Proprinter",
	L"TxtTab", L"Canon", L"GDIprinter"};
static wchar_t *FRM_Lajit[] = {L"Kirjoitin_osuus", L"Kirjoitin_kaikki", L"Tiedosto_osuus", L"Tiedosto_kaikki", L"N‰yttˆ"};
static wchar_t *FRM_Units[] = {L"Merkit", L"Points", L"Rivi/1000"};
static wchar_t *FRM_FontKohde[] = {L"Perus", L"Otsikko", L"Alaots", L"Lihavoitu", L"Pieni", L"Vakiolev"};
static wchar_t *FRM_FontFamilyName[] = {L"Times New Roman", L"Arial", L"Arial Narrow", L"Calibri", L"Courier New", L"Lucida Console"};
static wchar_t *FRM_VariKohde[] = {L"Tausta", L"Merkit", L"RiviTausta1", L"RiviMerkit1", L"RiviTausta2", L"RiviMerkit2", L"JkRiviTausta", L"JkRiviMerkit",
	L"SarOtsTausta", L"SarOtsMerkit", L"Viivat", L"KehysTausta", L"KehysMerkit", L"RiviTaustaRva", L"RiviMerkitRva",L"SarOtsTaustaRva", L"SarOtsMerkitRva"};
static wchar_t *FRM_TauluHtml[] = {L"tila=0", L"tila=1", L"tila=2", L"tila=3", L"tila=4", L"tila=5", L"tila=6"};

static int nFrmTags = sizeof(FrmTags)/sizeof(FrmTags[0]);
static GDIfontTp *fonts[] = {&DefFnt, &TitleFnt, &HeaderFnt, &BoldFnt, &SmallFnt, &CourFnt};
FldFrmtTp *frmflds[] = {prtflds, prtfldsk, fileflds, filefldsk, dspflds};
int n_frmflds[] = {n_prtflds, n_prtfldsk, n_fileflds, n_filefldsk, n_dspflds};

void TallXMLMuotoilu(wchar_t *MuotFile)
{
	TextFl *tallfl;
	int level = 0, nfld;
	wchar_t ln[400];
	tulosmuottp *muot;

	tallfl = new TextFl(MuotFile, L"wt");

	if (tallfl->IsOpen()) {
		tallfl->WriteLine(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
		swprintf(ln, L"%s createTime=\"%s\" creator=\"ViestiWin %s\"", L"Muotoilu", ISOdatetime(0,1), Build());
		tallfl->put_wtag(ln, level++);

		tallfl->put_wtag(XMLhae_tagName(FRM_TulosLuettelot, FrmTags, nFrmTags), level++);
		tallfl->put_wtagparams(XMLhae_tagName(FRM_Kohde, FrmTags, nFrmTags), L"laji=\"Kirjoitin\"", false, level++);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_SivuPit, FrmTags, nFrmTags), tulosmuot.sivpit, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Paivays, FrmTags, nFrmTags), (int) tulosmuot.tpvafl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_TulostusAika, FrmTags, nFrmTags), (int) tulosmuot.tklofl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Lukumaarat, FrmTags, nFrmTags), (int) tulosmuot.lkmfl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_VasMarg, FrmTags, nFrmTags), (int) tulosmuot.marg, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_OikMarg, FrmTags, nFrmTags), (int) tulosmuot.omarg, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_YlaMarg, FrmTags, nFrmTags), (int) tulosmuot.ymarg, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_AlaMarg, FrmTags, nFrmTags), (int) tulosmuot.amarg, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_ToistaSarja, FrmTags, nFrmTags), (int) tulosmuot.snimifl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_EtuSuku, FrmTags, nFrmTags), (int) tulosmuot.etusuku, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_EsiRivit, FrmTags, nFrmTags), (int) tulosmuot.esirivit, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_SarakeOtsikot, FrmTags, nFrmTags), (int) tulosmuot.otsikot, level);
		tallfl->put_wantitag(XMLhae_tagName(FRM_Kohde, FrmTags, nFrmTags), --level);
		tallfl->put_wtagparams(XMLhae_tagName(FRM_Kohde, FrmTags, nFrmTags), L"laji=\"Tiedosto\"", false, level++);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Paivays, FrmTags, nFrmTags), (int) filetulosmuot.tpvafl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_TulostusAika, FrmTags, nFrmTags), (int) filetulosmuot.tklofl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Lukumaarat, FrmTags, nFrmTags), (int) tulosmuot.lkmfl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_VasMarg, FrmTags, nFrmTags), (int) filetulosmuot.marg, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_OikMarg, FrmTags, nFrmTags), (int) filetulosmuot.omarg, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_ToistaSarja, FrmTags, nFrmTags), (int) tulosmuot.snimifl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_EtuSuku, FrmTags, nFrmTags), (int) filetulosmuot.etusuku, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_SarakeOtsikot, FrmTags, nFrmTags), (int) filetulosmuot.otsikot, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Linkit, FrmTags, nFrmTags), (int) filetulosmuot.sarjalinkit, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_FontFamily, FrmTags, nFrmTags), (int) filetulosmuot.fontfamily, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Css, FrmTags, nFrmTags), (int) filetulosmuot.cssfl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Koko, FrmTags, nFrmTags), (int) filetulosmuot.fontsize, level, L"kohde=Perus");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Koko, FrmTags, nFrmTags), (int) filetulosmuot.paaotskoko, level, L"kohde=PaaOts");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Koko, FrmTags, nFrmTags), (int) filetulosmuot.alaotskoko, level, L"kohde=AlaOts");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Koko, FrmTags, nFrmTags), (int) filetulosmuot.efontsize, level, L"kohde=RastiVa");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.bgcolor, level, L"kohde=Tausta");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.fontcolor, level, L"kohde=Merkit");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.framecolor, level, L"kohde=KehysTausta");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.framefontcolor, level, L"kohde=KehysMerkit");

		tallfl->put_wtagparams(XMLhae_tagName(FRM_TaulHtml, FrmTags, nFrmTags), FRM_TauluHtml[filetulosmuot.tauluhtml], false, level++);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_SarLevKerroin, FrmTags, nFrmTags), (int) filetulosmuot.levkerr, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_RiviRyhma, FrmTags, nFrmTags), (int) filetulosmuot.riviryhma, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.td1color, level, L"kohde=RiviTausta1");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.td1fontcolor, level, L"kohde=RiviMerkit1");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.td2color, level, L"kohde=RiviTausta2");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.td2fontcolor, level, L"kohde=RiviMerkit2");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.tdjkcolor, level, L"kohde=JkRiviTausta");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.tdjkfontcolor, level, L"kohde=JkRiviMerkit");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.thcolor, level, L"kohde=SarOtsTausta");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.thfontcolor, level, L"kohde=SarOtsMerkit");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.tdecolor, level, L"kohde=RiviTaustaRva");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.tdefontcolor, level, L"kohde=RiviMerkitRva");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.thecolor, level, L"kohde=SarOtsTaustaRva");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.thefontcolor, level, L"kohde=SarOtsMerkitRva");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.bordercolor, level, L"kohde=Viivat");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Viiva, FrmTags, nFrmTags), (int) filetulosmuot.border, level, L"kohde=Yleis");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Viiva, FrmTags, nFrmTags), (int) filetulosmuot.eborder, level, L"kohde=RastiVa");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_VasMarg, FrmTags, nFrmTags), (int) filetulosmuot.hpad, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_YlaMarg, FrmTags, nFrmTags), (int) filetulosmuot.vpad, level);
		tallfl->put_wantitag(XMLhae_tagName(FRM_TaulHtml, FrmTags, nFrmTags), --level);

		tallfl->put_wantitag(XMLhae_tagName(FRM_Kohde, FrmTags, nFrmTags), --level);
		for (int laji = 0; laji < (int) sizeof(frmflds)/sizeof(frmflds[0]); laji++) {
			FldFrmtTp *fld;
			fld = frmflds[laji];
			nfld = n_frmflds[laji];
			if (laji < 2)
				muot = &tulosmuot;
			else
				muot = &filetulosmuot;
			tallfl->put_wtagparams(XMLhae_tagName(FRM_TulosLuettelo, FrmTags, nFrmTags),
				wconcat(L"laji=\"", FRM_Lajit[laji], L"\""), false, level++);
			if (laji == 3)
				tallfl->put_wxml_d(XMLhae_tagName(FRM_TiivPituus, FrmTags, nFrmTags), (int) muot->tiiv_len, level);
			if (laji == 1 || laji == 3) {
				tallfl->put_wxml_d(XMLhae_tagName(FRM_TiivRivit, FrmTags, nFrmTags), (int) muot->tiiv_rivit, level);
				tallfl->put_wxml_d(XMLhae_tagName(FRM_TiivFontit, FrmTags, nFrmTags), (int) muot->tiiv_fontit, level);
				tallfl->put_wxml_d(XMLhae_tagName(FRM_TiivOsuusNo, FrmTags, nFrmTags), (int) muot->tiiv_osno, level);
				tallfl->put_wxml_d(XMLhae_tagName(FRM_TiivArvo, FrmTags, nFrmTags), (int) muot->tiiv_arvo, level);
				}
			for (int k = 0; k < nfld; k++) {
				tallfl->put_wtagparams(XMLhae_tagName(FRM_Kentta, FrmTags, nFrmTags),
					wconcat(L"nimi=\"", fld[k].name, L"\""), false, level++);
				tallfl->put_wxml_s(XMLhae_tagName(FRM_Otsikko, FrmTags, nFrmTags), fld[k].ots, level);
				tallfl->put_wxml_d(XMLhae_tagName(FRM_Sijainti, FrmTags, nFrmTags), fld[k].pos, level);
				tallfl->put_wxml_d(XMLhae_tagName(FRM_Leveys, FrmTags, nFrmTags), fld[k].len, level,
					 wconcat(L"yksikko=\"", FRM_Units[tulosmuot.levunit], L"\""));
				tallfl->put_wxml_d(XMLhae_tagName(FRM_Tasaus, FrmTags, nFrmTags), fld[k].oik, level);
				tallfl->put_wxml_d(XMLhae_tagName(FRM_Aktiivinen, FrmTags, nFrmTags), fld[k].akt, level);
				tallfl->put_wantitag(XMLhae_tagName(FRM_Kentta, FrmTags, nFrmTags), --level);
				}
			tallfl->put_wantitag(XMLhae_tagName(FRM_TulosLuettelo, FrmTags, nFrmTags), --level);
			}
		tallfl->put_wxml_s(XMLhae_tagName(FRM_PaaOts, FrmTags, nFrmTags), paaots, level);
		tallfl->put_wxml_s(XMLhae_tagName(FRM_SarjaOts, FrmTags, nFrmTags), sarjaots, level);
		tallfl->put_wxml_s(XMLhae_tagName(FRM_AlaOts, FrmTags, nFrmTags), alaots, level);
		tallfl->put_wantitag(XMLhae_tagName(FRM_TulosLuettelot, FrmTags, nFrmTags), --level);

		tallfl->put_wtag(XMLhae_tagName(FRM_Kirjasinlajit, FrmTags, nFrmTags), level++);
		for (int k = 0; k < 4; k++) {
			tallfl->put_wtagparams(XMLhae_tagName(FRM_Kirjasin, FrmTags, nFrmTags),
				wconcat(L"kohde=\"", FRM_FontKohde[k], L"\""), false, level++);
			tallfl->put_wxml_s(XMLhae_tagName(FRM_FontFamily, FrmTags, nFrmTags),
				FRM_FontFamilyName[fonts[k]->fontfamily], level);
			if (fonts[k]->pitch != 0)
				tallfl->put_wxml_d(XMLhae_tagName(FRM_Tuumalla, FrmTags, nFrmTags), fonts[k]->pitch, level);
			if (fonts[k]->size != 0)
				tallfl->put_wxml_d(XMLhae_tagName(FRM_Koko, FrmTags, nFrmTags), fonts[k]->size, level);
			tallfl->put_wxml_d(XMLhae_tagName(FRM_Lihav, FrmTags, nFrmTags), fonts[k]->bold, level);
			tallfl->put_wxml_d(XMLhae_tagName(FRM_Kursiv, FrmTags, nFrmTags), fonts[k]->italic, level);
			tallfl->put_wxml_d(XMLhae_tagName(FRM_Rivivali, FrmTags, nFrmTags), fonts[k]->line, level);
			tallfl->put_wantitag(XMLhae_tagName(FRM_Kirjasin, FrmTags, nFrmTags), --level);
			}
		tallfl->put_wantitag(XMLhae_tagName(FRM_Kirjasinlajit, FrmTags, nFrmTags), --level);

		tallfl->put_wantitag(L"Muotoilu", --level);
		}
	delete tallfl;
}

//---------------------------------------------------------------------------
static int tulkXMLTlKentta(int laji, wchar_t *knt, xml_node *node, int *inode, int nnode)
{
	int er = 0, DepthIn, haara = 0, iassign = -1, no = 0, kentta;
	wchar_t ln[200];

	for (kentta = 0; kentta < n_frmflds[laji]; kentta++)
		if (wcscmp(knt, frmflds[laji][kentta].name) == 0)
			break;
	if (kentta == n_frmflds[laji])
		return(1);
	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			haara = 0;
			switch (node[*inode].tagid) {
				case FRM_Otsikko:
					node[*inode].gettext(frmflds[laji][kentta].ots,
						sizeof(frmflds[laji][kentta].ots)/2-1);
					break;
				case FRM_Sijainti:
					node[*inode].gettext_int(&frmflds[laji][kentta].pos);
					break;
				case FRM_Leveys:
					node[*inode].getparam_text(L"yksikko", ln, 20);
					tulosmuot.levunit = haeStrIx(ln, FRM_Units, sizeof(FRM_Units)/sizeof(FRM_Units[0]));
					node[*inode].gettext_int(&frmflds[laji][kentta].len);
					break;
				case FRM_Tasaus:
					node[*inode].gettext_int(&frmflds[laji][kentta].oik);
					break;
				case FRM_Aktiivinen:
					node[*inode].gettext_int(&frmflds[laji][kentta].akt);
					break;
				}
			continue;
			}
		if (node[*inode].depth == DepthIn + 1) {
			continue;
			}
		}
	return(er);
}

//---------------------------------------------------------------------------
static int tulkXMLTlMuotoilu(int laji, xml_node *node, int *inode, int nnode)
{
	int er = 0, DepthIn, haara = 0, iassign = -1, no = 0, val;
	wchar_t ln[200];
	tulosmuottp *muot;

	if (laji < 2)
		muot = &tulosmuot;
	else
		muot = &filetulosmuot;
	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			haara = 0;
			switch (node[*inode].tagid) {
				case FRM_TiivPituus:
					node[*inode].gettext_int(&val);
					muot->tiiv_len = val;
					break;
				case FRM_TiivRivit:
					node[*inode].gettext_int(&val);
					muot->tiiv_len = val;
					break;
				case FRM_TiivFontit:
					node[*inode].gettext_int(&val);
					muot->tiiv_fontit = val;
					break;
				case FRM_TiivOsuusNo:
					node[*inode].gettext_int(&val);
					muot->tiiv_osno = val;
					break;
				case FRM_TiivArvo:
					node[*inode].gettext_int(&val);
					muot->tiiv_arvo = val;
					break;
				case FRM_Kentta :
					node[*inode].getparam_text(L"nimi", ln, 30);
					tulkXMLTlKentta(laji, ln, node, inode, nnode);
					break;
				}
			continue;
			}
		if (node[*inode].depth == DepthIn + 1) {
			continue;
			}
		}
	return(er);
}

//---------------------------------------------------------------------------

static int tulkXMLTlMuotoilut(xml_node *node, int *inode, int nnode)
{
	int er = 0, DepthIn, haara = 0, iassign = -1, no = 0, val, ii = 0;
	wchar_t ln[200];
	tulosmuottp *muot = NULL;

	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			haara = 0;
			switch (node[*inode].tagid) {
				case FRM_Kohde:
					node[*inode].getparam_text(L"laji", ln, 30);
					if (wcscmp(ln, L"Tiedosto") == 0) {
						muot = &filetulosmuot;
						haara = 1;
						}
					else if (wcscmp(ln, L"Kirjoitin") == 0) {
						muot = &tulosmuot;
						haara = 1;
						}
					else
						muot = NULL;
//					node[*inode].gettext(ln, 100);
//					lsttulparam.printer = haeStrIx(ln, FRM_Kohteet,
//						sizeof(FRM_Kohteet)/sizeof(FRM_Kohteet[0]));
					break;
				case FRM_SivuPit:
					node[*inode].gettext_int(&tulosmuot.sivpit);
					break;
				case FRM_Paivays:
					node[*inode].gettext_int(&val);
					tulosmuot.tpvafl = val;
					filetulosmuot.tpvafl = val;
					break;
				case FRM_TulostusAika:
					node[*inode].gettext_int(&val);
					tulosmuot.tklofl = val;
					filetulosmuot.tklofl = val;
					break;
				case FRM_NumLeveys:
					node[*inode].gettext_int(&tulosmuot.numlev);
					break;
				case FRM_VasMarg:
					node[*inode].gettext_int(&tulosmuot.marg);
					break;
				case FRM_OikMarg:
					node[*inode].gettext_int(&tulosmuot.omarg);
					break;
				case FRM_YlaMarg:
					node[*inode].gettext_int(&tulosmuot.ymarg);
					break;
				case FRM_AlaMarg:
					node[*inode].gettext_int(&tulosmuot.amarg);
					break;
				case FRM_ToistaSarja:
					node[*inode].gettext_int(&val);
					tulosmuot.snimifl = val;
					filetulosmuot.snimifl = val;
					break;
				case FRM_EtuSuku:
					node[*inode].gettext_int(&val);
					tulosmuot.etusuku = val;
					filetulosmuot.etusuku = val;
					break;
				case FRM_EsiRivit:
					node[*inode].gettext_int(&tulosmuot.esirivit);
					break;
				case FRM_SarakeOtsikot:
					node[*inode].gettext_int(&val);
					tulosmuot.otsikot = val;
					filetulosmuot.otsikot = val;
					break;
				case FRM_TiivPituus:
					node[*inode].gettext_int(&val);
					tulosmuot.tiiv_len = val;
					filetulosmuot.tiiv_len = val;
					break;
				case FRM_TiivRivit:
					node[*inode].gettext_int(&val);
					tulosmuot.tiiv_len = val;
					filetulosmuot.tiiv_len = val;
					break;
				case FRM_TiivFontit:
					node[*inode].gettext_int(&val);
					tulosmuot.tiiv_fontit = val;
					filetulosmuot.tiiv_fontit = val;
					break;
				case FRM_TiivOsuusNo:
					node[*inode].gettext_int(&val);
					tulosmuot.tiiv_osno = val;
					filetulosmuot.tiiv_osno = val;
					break;
				case FRM_TiivArvo:
					node[*inode].gettext_int(&val);
					tulosmuot.tiiv_arvo = val;
					filetulosmuot.tiiv_arvo = val;
					break;
				case FRM_PaaOts:
					node[*inode].gettext(ln, 198);
					if (wcslen(ln) > 1)
						wcsncpy(paaots, ln, sizeof(paaots)/2-1);
					break;
				case FRM_SarjaOts:
					node[*inode].gettext(ln, 198);
					wcsncpy(sarjaots, ln, sizeof(sarjaots)/2-1);
					break;
				case FRM_AlaOts:
					node[*inode].gettext(ln, 198);
					wcsncpy(alaots, ln, sizeof(alaots)/2-1);
					break;
				case FRM_TulosLuettelo :
					node[*inode].getparam_text(L"laji", ln, 30);
					val = haeStrIx(ln, FRM_Lajit, sizeof(FRM_Lajit)/sizeof(FRM_Lajit[0]));
					tulkXMLTlMuotoilu(val, node, inode, nnode);
					break;
				}
			continue;
			}
		if (node[*inode].depth == DepthIn + 1) {
			if (haara == 1 && muot) {
				switch (node[*inode].tagid) {
					case FRM_SivuPit:
						node[*inode].gettext_int(&muot->sivpit);
						break;
					case FRM_Paivays:
						node[*inode].gettext_int(&val);
						muot->tpvafl = val;
						break;
					case FRM_TulostusAika:
						node[*inode].gettext_int(&val);
						muot->tklofl = val;
						break;
					case FRM_Lukumaarat:
						node[*inode].gettext_int(&val);
						muot->lkmfl = val;
						break;
					case FRM_NumLeveys:
						node[*inode].gettext_int(&muot->numlev);
						break;
					case FRM_VasMarg:
						node[*inode].gettext_int(&muot->marg);
						break;
					case FRM_OikMarg:
						node[*inode].gettext_int(&muot->omarg);
						break;
					case FRM_YlaMarg:
						node[*inode].gettext_int(&muot->ymarg);
						break;
					case FRM_AlaMarg:
						node[*inode].gettext_int(&muot->amarg);
						break;
					case FRM_ToistaSarja:
						node[*inode].gettext_int(&val);
						muot->snimifl = val;
						break;
					case FRM_EtuSuku:
						node[*inode].gettext_int(&val);
						muot->etusuku = val;
						break;
					case FRM_EsiRivit:
						node[*inode].gettext_int(&muot->esirivit);
						break;
					case FRM_SarakeOtsikot:
						node[*inode].gettext_int(&val);
						muot->otsikot = val;
						break;
					case FRM_TiivPituus:
						node[*inode].gettext_int(&val);
						muot->tiiv_len = val;
						break;
					case FRM_TiivRivit:
						node[*inode].gettext_int(&val);
						muot->tiiv_len = val;
						break;
					case FRM_TiivFontit:
						node[*inode].gettext_int(&val);
						muot->tiiv_fontit = val;
						break;
					case FRM_TiivOsuusNo:
						node[*inode].gettext_int(&val);
						muot->tiiv_osno = val;
						break;
					case FRM_TiivArvo:
						node[*inode].gettext_int(&val);
						muot->tiiv_arvo = val;
						break;
					case FRM_Linkit:
						node[*inode].gettext_int(&val);
						muot->sarjalinkit = val;
						break;
					case FRM_TaulHtml:
						node[*inode].getparam_int(L"tila", &val);
						muot->tauluhtml = val;
						haara = 2;
						break;
					case FRM_SarLevKerroin:
						node[*inode].gettext_int(&val);
						muot->levkerr = val;
						break;
					case FRM_RiviRyhma:
						node[*inode].gettext_int(&val);
						muot->riviryhma = val;
						break;
					case FRM_FontFamily:
						node[*inode].gettext_int(&val);
						muot->fontfamily = val;
						break;
					case FRM_Koko:
						node[*inode].getparam_text(L"kohde", ln, 20);
						node[*inode].gettext_int(&val);
						if (wcscmp(ln, L"PaaOts") == 0)
							muot->paaotskoko = val;
						else
						if (wcscmp(ln, L"AlaOts") == 0)
							muot->alaotskoko = val;
						else if (wcscmp(ln, L"RastiVa") == 0)
							muot->efontsize = val;
						else
							muot->fontsize = val;
						break;
					case FRM_Vari:
						node[*inode].getparam_text(L"kohde", ln, 20);
						ii = haeStrIx(ln, FRM_VariKohde, sizeof(FRM_VariKohde)/sizeof(FRM_VariKohde[0]));
						node[*inode].gettext_int(&val);
						switch (ii) {
							case 0:
								muot->bgcolor = val;
								break;
							case 1:
								muot->fontcolor = val;
								break;
							case 2:
								muot->td1color = val;
								break;
							case 3:
								muot->td1fontcolor = val;
								break;
							case 4:
								muot->td2color = val;
								break;
							case 5:
								muot->td2fontcolor = val;
								break;
							case 6:
								muot->tdjkcolor = val;
								break;
							case 7:
								muot->tdjkfontcolor = val;
								break;
							case 8:
								muot->thcolor = val;
								break;
							case 9:
								muot->thfontcolor = val;
								break;
							case 10:
								muot->bordercolor = val;
								break;
							case 11:
								muot->framecolor = val;
								break;
							case 12:
								muot->framefontcolor = val;
								break;
							case 13:
								muot->tdecolor = val;
								break;
							case 14:
								muot->tdefontcolor = val;
								break;
							case 15:
								muot->thecolor = val;
								break;
							case 16:
								muot->thefontcolor = val;
								break;
							}
						break;
					case FRM_Css:
						node[*inode].gettext_int(&val);
						muot->cssfl = val;
						break;
					}
				}
			continue;
			}
		if (node[*inode].depth == DepthIn + 2) {
			if (haara == 2 && muot) {
				switch (node[*inode].tagid) {
					case FRM_RiviRyhma:
						node[*inode].gettext_int(&val);
						muot->riviryhma = val;
						break;
					case FRM_FontFamily:
						node[*inode].gettext_int(&val);
						muot->fontfamily = val;
						break;
					case FRM_Koko:
						node[*inode].getparam_text(L"kohde", ln, 20);
						node[*inode].gettext_int(&val);
						if (wcscmp(ln, L"PaaOts") == 0)
							muot->paaotskoko = val;
						else if (wcscmp(ln, L"AlaOts") == 0)
							muot->alaotskoko = val;
						else if (wcscmp(ln, L"RastiVa") == 0)
							muot->efontsize = val;
						else
							muot->fontsize = val;
						break;
					case FRM_Viiva:
						node[*inode].getparam_text(L"kohde", ln, 20);
						node[*inode].gettext_int(&val);
						if (wcscmp(ln, L"RastiVa") == 0)
							muot->eborder = val;
						else
							muot->border = val;
						break;
					case FRM_Vari:
						node[*inode].getparam_text(L"kohde", ln, 20);
						ii = haeStrIx(ln, FRM_VariKohde, sizeof(FRM_VariKohde)/sizeof(FRM_VariKohde[0]));
						node[*inode].gettext_int(&val);
						switch (ii) {
							case 0:
								muot->bgcolor = val;
								break;
							case 1:
								muot->fontcolor = val;
								break;
							case 2:
								muot->td1color = val;
								break;
							case 3:
								muot->td1fontcolor = val;
								break;
							case 4:
								muot->td2color = val;
								break;
							case 5:
								muot->td2fontcolor = val;
								break;
							case 6:
								muot->tdjkcolor = val;
								break;
							case 7:
								muot->tdjkfontcolor = val;
								break;
							case 8:
								muot->thcolor = val;
								break;
							case 9:
								muot->thfontcolor = val;
								break;
							case 10:
								muot->bordercolor = val;
								break;
							case 11:
								muot->framecolor = val;
								break;
							case 12:
								muot->framefontcolor = val;
								break;
							case 13:
								muot->tdecolor = val;
								break;
							case 14:
								muot->tdefontcolor = val;
								break;
							case 15:
								muot->thecolor = val;
								break;
							case 16:
								muot->thefontcolor = val;
								break;
							}
						break;
					case FRM_VasMarg:
						node[*inode].gettext_int(&muot->hpad);
						break;
					case FRM_YlaMarg:
						node[*inode].gettext_int(&muot->vpad);
						break;
					}
				}
			continue;
			}
		}
	return(er);
}

//---------------------------------------------------------------------------
static int tulkXMLKirjasin(int kohde, xml_node *node, int *inode, int nnode)
{
	int er = 0, DepthIn, haara = 0, iassign = -1, no = 0;
	wchar_t ln[200];

	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			haara = 0;
			switch (node[*inode].tagid) {
				case FRM_FontFamily:
					node[*inode].gettext(ln, 190);
					fonts[kohde]->fontfamily = haeStrIx(ln, FRM_FontFamilyName,
						sizeof(FRM_FontFamilyName)/sizeof(FRM_FontFamilyName[0]));
					break;
				case FRM_Koko:
					node[*inode].gettext_int(&fonts[kohde]->size);
					break;
				case FRM_Tuumalla:
					node[*inode].gettext_int(&fonts[kohde]->pitch);
					break;
				case FRM_Lihav:
					node[*inode].gettext_int(&fonts[kohde]->bold);
					break;
				case FRM_Kursiv:
					node[*inode].gettext_int(&fonts[kohde]->italic);
					break;
				case FRM_Rivivali:
					node[*inode].gettext_int(&fonts[kohde]->line);
					break;
				}
			continue;
			}
		if (node[*inode].depth == DepthIn + 1) {
			continue;
			}
		}
	fonts[kohde]->hfont = NULL;
	return(er);
}

//---------------------------------------------------------------------------
static int tulkXMLKirjLajit(xml_node *node, int *inode, int nnode)
{
	int er = 0, DepthIn, haara = 0, iassign = -1, no = 0, val;
	wchar_t ln[200];

	DepthIn = node[*inode].depth + 1;
	for (++*inode; *inode <= nnode; (*inode)++) {
		if (node[*inode].depth < DepthIn) {
			--*inode;
			break;
			}
		if (node[*inode].depth == DepthIn) {
			haara = 0;
			switch (node[*inode].tagid) {
				case FRM_Kirjasin :
					node[*inode].getparam_text(L"kohde", ln, 30);
					val = haeStrIx(ln, FRM_FontKohde, sizeof(FRM_FontKohde)/sizeof(FRM_FontKohde[0]));
					tulkXMLKirjasin(val, node, inode, nnode);
					break;
				}
			continue;
			}
		if (node[*inode].depth == DepthIn + 1) {
			continue;
			}
		}
	return(er);
}

//---------------------------------------------------------------------------
static int tulkXMLMuotoilu(xml_node *node, int nnode)
{
	int er = 0, inode, DepthIn, haara = 0, iassign = -1, no = 0;

	XMLhaenodeid(node, nnode, FrmTags, nFrmTags);
	DepthIn = node[0].depth + 1;
	for (inode = 1; inode <= nnode; inode++) {
		if (node[inode].depth == DepthIn) {
			haara = 0;
			switch (node[inode].tagid) {
				case FRM_TulosLuettelot :
					tulkXMLTlMuotoilut(node, &inode, nnode);
					break;
				case FRM_Kirjasinlajit :
					tulkXMLKirjLajit(node, &inode, nnode);
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

int LueXMLMuotoilu(wchar_t *MuotFile)
{
	TextFl *infile;
	int ntag = 10000, haara = 0, inode, inode_in, depth = 0, rv, er = 0, lntype;
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
				nd.nodeno[i] = k[i];           // merkit‰‰n eri tasojen j‰rjestysnumerot solmuun
			if (lntype > 0)
				nd.tagid = XMLhae_tagid(nd.tag, FrmTags, nFrmTags);
			if (lntype == 1) {
				tree.node[++inode] = nd;
				k[depth++]++;
				if (depth == 1) {
					inode_in = inode;
					switch (nd.tagid) {
						case FRM_Muotoilu :
							haara = 0;
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er)
								tulkXMLMuotoilu(tree.node+inode_in, inode-inode_in);
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
		swprintf(msg, L"XML-tiedosto %s p‰‰ttyi rivill‰ %d tasolla %d", MuotFile, rv, depth);
		}
	if (er) {
		if (!msg[0]) {
			swprintf(msg, L"Tiedostossa %s virhe rivill‰ %d tai aikaisemmin", MuotFile, rv);
			}
		writeerror_w(msg, 0, true);
		}
	tree.nodecount = inode+1;
	return(0);
}
//---------------------------------------------------------------------------

