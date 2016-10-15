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

#include "HkMuotoilu.h"
//---------------------------------------------------------------------------
//#pragma package(smart_init)

FldFrmtTp dspflds[] = {
	{F_SJ, L"Sija", L"Sija", 0, 3, 1, 0, 1},                    	// 0: sija
	{F_KNO, L"Nro", L"Nro",  6, 4, 1, 0, 1},                   	// 1: numero
	{F_NIMI, L"Nimi", L"Nimi", 12,25,0, 0, 1},                   // 2: nimi
	{F_ETUNIMI, L"Etunimi", L"Etunimi", 12, 0, 0, 0, 0},             //
	{F_SUKUNIMI, L"Sukunimi", L"Sukunimi", 12, 0, 0, 0, 0},           //
	{F_ARVO, L"Arvo", L"Arvo", 12,0, 0, 0, 0},                   	// 3: arvo
	{F_LISNO, L"Lisenssi", L"Lisenssi", 38,6, 1, 0, 1},          // 4: lisenssi
	{F_LISNO2, L"KvId", L"KvId", 38,0, 1, 0, 0},               	// 5: Kv.id
	{F_BADGE, L"Badge", L"Badge", 38,0, 1, 0, 0},                // 6: badge
	{F_SRA, L"Seura", L"Seura", 45,10, 0, 0, 1},                 // 7: seura
	{F_SRALYH, L"Seuralyh", L"Seuralyh", 45,0, 0, 0, 0},         // 8: lyhenne
	{F_MAA, L"Maa", L"Maa", 56, 0, 0, 0, 0},                   	// 9: maa
	{F_YHD, L"Yhdistys", L"Yhdistys", 56,0, 0, 0, 0},            // 10: yhdistys
	{F_JOUK, L"Joukkue", L"Joukkue", 56,0, 0, 0, 0},             // 11: joukkue
	{F_ALISRJ, L"Alisarja", L"Alisarja", 56,0, 0, 0, 0},         // 12: alisarja
	{F_TLS, L"Tulos", L"Tulos", 56, 10, 1, 0, 1},                 // 13: tulos
	{F_ERO, L"Ero", L"Ero",  67, 10, 1, 0, 1},                  	// 14: ero k‰rkeen
	{F_SAK, L"Sakot", L"Sakot",72, 0, 1, 0, 0},                   // 15: sakot
	{F_TARK, L"Hyv‰ks.", L"Hyv‰ks.", 79,1, 0, 0, 1},             // 16: Leimantarkastus
	{F_AIKA, L"Aika", L"Aika", 72, 0, 1, 0, 0},                 	// 17: Aika ilman sakkoa
	{F_PIST, L"Pisteet", L"Pisteet", 72, 0, 1, 0, 0},            // 18: pisteet
	{0, 0, 0, 0, 0, 0, 0}
	};

FldFrmtTp fileflds[] = {
	{F_SJ, L"Sija", L"Sija", 0, 4, 1, 0, 1},                   	// 0: sija
	{F_KNO, L"Nro", L"Nro",  6, 4, 1, 0, 1},                   	// 1: numero
	{F_NIMI, L"Nimi", L"Nimi", 13,30,0, 1},                   // 2: nimi
	{F_ETUNIMI, L"Etunimi", L"Etunimi", 13, 0, 0, 0, 0},             //
	{F_SUKUNIMI, L"Sukunimi", L"Sukunimi", 13, 0, 0, 0, 0},           //
	{F_ARVO, L"Arvo", L"Arvo", 13,0,0, 0, 0},                   	// 3: arvo
	{F_LISNO, L"Lisenssi", L"Lisenssi", 45,0, 1, 0, 0},          // 4: lisenssi
	{F_LISNO2, L"KvId", L"KvId", 45,0, 1, 0, 0},               	// 5: Kv.id
	{F_BADGE, L"Badge", L"Badge", 45,0, 1, 0, 0},                // 6: badge
	{F_SRA, L"Seura", L"Seura", 45,30, 0, 0, 1},                 // 7: seura
	{F_SRALYH, L"Seuralyh", L"Seuralyh", 45,0, 0, 0, 0},         // 8: lyhenne
	{F_MAA, L"Maa", L"Maa", 78, 0, 0, 0, 0},                   	// 9: maa
	{F_YHD, L"Yhdistys", L"Yhdistys", 78, 0, 0, 0, 0},            // 10: yhdistys
	{F_JOUK, L"Joukkue", L"Joukkue", 78,0, 0, 0, 0},             // 11: joukkue
	{F_ALISRJ, L"Alisarja", L"Alisarja", 56, 0, 0, 0, 0},        // 12: alisarja
	{F_TLS, L"Tulos", L"Tulos", 78,10, 1, 0, 1},                 // 13: tulos
	{F_ERO, L"Ero", L"Ero",  90,10, 1, 0, 1},                  	// 14: ero k‰rkeen
	{F_SAK, L"Sakot", L"Sakot",90,0, 1, 0, 0},                   // 15: sakot
	{F_TARK, L"Hyv‰ks.", L"Hyv‰ks.", 78, 0, 0, 0, 0},            // 16: Leimantarkastus
	{F_AIKA, L"Aika", L"Aika", 102, 0, 1, 0, 0},                 // 17: Aika ilman sakkoa
	{F_PIST, L"Pisteet", L"Pisteet", 102, 0, 1, 0, 0},           // 18: pisteet
	{F_OSASJ, L"Osasija", L"Sj-2", 102, 3, 1, 18, 1},          // 19: vaiheen tai v‰liajan sija
	{F_OSATLS, L"Osatulos", L"Tulos-2", 106,10, 1, 18, 1},       // 20: vaiheen tulos tai v‰liaika
	{0, 0, 0, 0, 0, 0}
	};

FldFrmtTp fileflds_suunn[] = {
	{F_SJ, L"Sija", L"Sija", 0, 4, 1, 0, 1},                   	// 0: sija
	{F_KNO, L"Nro", L"Nro",  6, 4, 1, 0, 1},                   	// 1: numero
	{F_NIMI, L"Nimi", L"Nimi", 13,30,0, 0, 1},                   // 2: nimi
	{F_ETUNIMI, L"Etunimi", L"Etunimi", 13, 0, 0, 0, 0},         //
	{F_SUKUNIMI, L"Sukunimi", L"Sukunimi", 13, 0, 0, 0, 0},      //
	{F_ARVO, L"Arvo", L"Arvo", 13,0,0, 0, 0},                   	// 3: arvo
	{F_LISNO, L"Lisenssi", L"Lisenssi", 45,0, 1, 0, 0},          // 4: lisenssi
	{F_LISNO2, L"KvId", L"KvId", 45,0, 1, 0, 0},               	// 5: Kv.id
	{F_BADGE, L"Badge", L"Badge", 45,0, 1, 0, 0},                // 6: badge
	{F_SRA, L"Seura", L"Seura", 45,30, 0, 0, 1},                 // 7: seura
	{F_SRALYH, L"Seuralyh", L"Seuralyh", 45,0, 0, 0, 0},         // 8: lyhenne
	{F_MAA, L"Maa", L"Maa", 76, 0, 0, 0, 0},                   	// 9: maa
	{F_YHD, L"Yhdistys", L"Yhdistys", 60,0, 0, 0, 0},            // 10: yhdistys
	{F_JOUK, L"Joukkue", L"Joukkue", 60,0, 0, 0, 0},             // 11: joukkue
	{F_ALISRJ, L"Alisarja", L"Alisarja", 60, 0, 0, 0, 0},        // 12: alisarja
	{F_TLS, L"Tulos", L"Tulos", 81,10, 1, 0, 1},                 // 13: tulos
	{F_ERO, L"Ero", L"Ero",  93,10, 1, 0, 1},                  	// 14: ero k‰rkeen
	{F_SAK, L"Sakot", L"Sakot",93,0, 1, 0, 0},                   // 15: sakot
	{F_TARK, L"Hyv‰ks.", L"Hyv‰ks.", 105, 0, 0, 0, 0},           // 16: Leimantarkastus
	{F_AIKA, L"Aika", L"Aika", 105, 0, 1, 0, 0},                 // 17: Aika ilman sakkoa
	{F_PIST, L"Pisteet", L"Pisteet", 105, 0, 1, 0, 0},           // 18: pisteet
	{F_OSASJ, L"Osasija", L"Sj-2", 108, 3, 1, 18, 1},          // 19: vaiheen tai v‰liajan sija
	{F_OSATLS, L"Osatulos", L"Tulos-2", 112,10, 1, 18, 1},       // 20: vaiheen tulos tai v‰liaika
	{0, 0, 0, 0, 0, 0, 0}
	};

FldFrmtTp mobilflds[] = {
	{F_SJ, L"Sija", L"Sija", 0, 3, 1, 0, 1},                   	// 0: sija
	{F_KNO, L"Nro", L"Nro",  5, 4, 1, 0, 1},                   	// 1: numero
	{F_NIMI, L"Nimi", L"Nimi", 10, 25,0, 0, 1},                   // 2: nimi
	{F_ETUNIMI, L"Etunimi", L"Etunimi", 10, 0, 0, 0, 0},             //
	{F_SUKUNIMI, L"Sukunimi", L"Sukunimi", 10, 0, 0, 0, 0},           //
	{F_ARVO, L"Arvo", L"Arvo", 13,0,0, 0, 0},                   	// 3: arvo
	{F_LISNO, L"Lisenssi", L"Lisenssi", 45,0, 1, 0, 0},          // 4: lisenssi
	{F_LISNO2, L"KvId", L"KvId", 45,0, 1, 0, 0},               	// 5: Kv.id
	{F_BADGE, L"Badge", L"Badge", 45,0, 1, 0, 0},                // 6: badge
	{F_SRA, L"Seura", L"Seura", 37, 20, 0, 0, 1},                 // 7: seura
	{F_SRALYH, L"Seuralyh", L"Seuralyh", 45,0, 0, 0, 0},         // 8: lyhenne
	{F_MAA, L"Maa", L"Maa", 78, 0, 0, 0, 0},                   	// 9: maa
	{F_YHD, L"Yhdistys", L"Yhdistys", 78,0, 0, 0, 0},            // 10: yhdistys
	{F_JOUK, L"Joukkue", L"Joukkue", 78,0, 0, 0, 0},             // 11: joukkue
	{F_ALISRJ, L"Alisarja", L"Alisarja", 56, 0, 0, 0, 0},        // 12: alisarja
	{F_TLS, L"Tulos", L"Tulos", 54,10, 1, 0, 1},                 // 13: tulos
	{F_ERO, L"Ero", L"Ero",  90, 0, 1, 0, 1},                  	// 14: ero k‰rkeen
	{F_SAK, L"Sakot", L"Sakot",90,0, 1, 0, 0},                   // 15: sakot
	{F_TARK, L"Hyv‰ks.", L"Hyv‰ks.", 78, 0, 0, 0, 0},            // 16: Leimantarkastus
	{F_AIKA, L"Aika", L"Aika", 102, 0, 1, 0, 0},                 // 17: Aika ilman sakkoa
	{F_PIST, L"Pisteet", L"Pisteet", 102, 0, 1, 0, 0},           // 18: pisteet
	{F_OSASJ, L"Osasija", L"Sj-2", 63, 3, 1, 16, 1},          // 19: vaiheen tai v‰liajan sija
	{F_OSATLS, L"Osatulos", L"Tulos-2", 67,10, 1, 16, 1},       // 20: vaiheen tulos tai v‰liaika
	{0, 0, 0, 0, 0, 0, 0}
	};

FldFrmtTp prtflds_suunn[] = {
	{F_SJ, L"Sija", L"Sija",0, 4, 1, 0, 1},                   	// 0: sija
	{F_KNO, L"Nro", L"Nro", 50, 4, 1, 0, 1},                   	// 1: numero
	{F_NIMI, L"Nimi", L"Nimi",100,27,0, 0, 1},                   // 2: nimi
	{F_ETUNIMI, L"Etunimi", L"Etunimi", 100, 0, 0, 0, 0},            //
	{F_SUKUNIMI, L"Sukunimi", L"Sukunimi", 100, 0, 0, 0, 0},          //
	{F_ARVO, L"Arvo", L"Arvo", 100,0,0, 0, 0},                   // 3: arvo
	{F_LISNO, L"Lisenssi", L"Lisenssi",345,8, 1, 0, 1},          // 4: lisenssi
	{F_LISNO2, L"KvId", L"KvId", 345,0, 1, 0, 0},               	// 5: Kv.id
	{F_BADGE, L"Badge", L"Badge", 345,0, 1, 0, 0},               // 6: badge
	{F_SRA, L"Seura", L"Seura", 440,25, 0, 0, 1},                // 7: seura
	{F_SRALYH, L"Seuralyh", L"Seuralyh", 440,0, 0, 0, 0},        // 8: lyhenne
	{F_MAA, L"Maa", L"Maa", 600, 0, 0, 0, 0},    				// 9: maa
	{F_YHD, L"Yhdistys", L"Yhdistys", 440,0, 0, 0, 0},           // 10: yhdistys
	{F_JOUK, L"Joukkue", L"Joukkue", 440,0, 0, 0, 0},            // 11: joukkue
	{F_ALISRJ, L"Alisarja", L"Alisarja", 440, 0, 0, 0, 0},       // 12: alisarja
	{F_TLS, L"Tulos", L"Tulos",650,10, 1, 0, 1},                 // 13: tulos
	{F_ERO, L"Ero", L"Ero", 760,9, 1, 0, 1},                   	// 14: ero k‰rkeen
	{F_SAK, L"Sakot", L"Sakot",910,0, 1, 0, 0},                  // 15: sakot
	{F_TARK, L"Leim.hyv", L"Leim.hyv", 860,1, 0, 0, 1},          // 16: Leimantarkastus
	{F_AIKA, L"Aika", L"Aika", 860, 0, 1, 0, 0},                 // 17: Aika ilman sakkoa
	{F_PIST, L"Pisteet", L"Pisteet", 860, 0, 1, 0, 0},           // 18: pisteet
	{0, 0, 0, 0, 0, 0, 0}
	};

FldFrmtTp prtflds[] = {
	{F_SJ, L"Sija", L"Sija",0, 4, 1, 0, 1},                   	// 0: sija
	{F_KNO, L"Nro", L"Nro", 50, 4, 1, 0, 1},                   	// 1: numero
	{F_NIMI, L"Nimi", L"Nimi",100,27,0, 0, 1},                   // 2: nimi
	{F_ETUNIMI, L"Etunimi", L"Etunimi", 100, 0, 0, 0, 0},            //
	{F_SUKUNIMI, L"Sukunimi", L"Sukunimi", 100, 0, 0, 0, 0},          //
	{F_ARVO, L"Arvo", L"Arvo", 100,0,0, 0, 0},                   // 3: arvo
	{F_LISNO, L"Lisenssi", L"Lisenssi",345,8, 1, 0, 1},          // 4: lisenssi
	{F_LISNO2, L"KvId", L"KvId", 345,0, 1, 0, 0},               	// 5: Kv.id
	{F_BADGE, L"Badge", L"Badge", 345,0, 1, 0, 0},               // 6: badge
	{F_SRA, L"Seura", L"Seura", 440,28, 0, 0, 1},                // 7: seura
	{F_SRALYH, L"Seuralyh", L"Seuralyh", 440,0, 0, 0, 0},        // 8: lyhenne
	{F_MAA, L"Maa", L"Maa", 700, 3, 0, 0, 0},  					// 9: maa
	{F_YHD, L"Yhdistys", L"Yhdistys", 440,0, 0, 0, 0},           // 10: yhdistys
	{F_JOUK, L"Joukkue", L"Joukkue", 440,0, 0, 0, 0},            // 11: joukkue
	{F_ALISRJ, L"Alisarja", L"Alisarja", 440,0, 0, 0, 0},        // 12: alisarja
	{F_TLS, L"Tulos", L"Tulos",725,10, 1, 0, 1},                 // 13: tulos
	{F_ERO, L"Ero", L"Ero", 820,9, 1, 0, 1},                   	// 14: ero k‰rkeen
	{F_SAK, L"Sakot", L"Sakot",820,0, 1, 0, 0},                  // 15: sakot
	{F_TARK, L"Leim.hyv", L"Leim.hyv", 920,1, 0, 0, 1},          // 16: Leimantarkastus
	{F_AIKA, L"Aika", L"Aika", 847, 0, 1, 0, 0},                 // 17: Aika ilman sakkoa
	{F_PIST, L"Pisteet", L"Pisteet", 847, 0, 1, 0, 0},           // 18: pisteet
	{0, 0, 0, 0, 0, 0, 0}
	};

FldFrmtTp prtfldsm[] = {
	{F_SJ, L"Sija", L"Sija",0, 4, 1, 0, 1},                   	// 0: sija
	{F_KNO, L"Nro", L"Nro", 50, 4, 1, 0, 1},                   	// 1: numero
	{F_NIMI, L"Nimi", L"Nimi",100,27,0, 0, 1},                   // 2: nimi
	{F_ETUNIMI, L"Etunimi", L"Etunimi", 100, 0, 0, 0, 0},            //
	{F_SUKUNIMI, L"Sukunimi", L"Sukunimi", 100, 0, 0, 0, 0},          //
	{F_ARVO, L"Arvo", L"Arvo", 100,0,0, 0, 0},                   // 3: arvo
	{F_LISNO, L"Lisenssi", L"Lisenssi",345,0, 1, 0, 0},          // 4: lisenssi
	{F_LISNO2, L"KvId", L"KvId", 345,0, 1, 0, 0},               	// 5: Kv.id
	{F_BADGE, L"Badge", L"Badge", 345,0, 1, 0, 0},               // 6: badge
	{F_SRA, L"Seura", L"Seura", 345,25, 0, 0, 1},                // 7: seura
	{F_SRALYH, L"Seuralyh", L"Seuralyh", 440, 0, 0, 0, 0},       // 8: lyhenne
	{F_MAA, L"Maa", L"Maa", 605, 0, 0, 0, 0},						// 9: maa
	{F_YHD, L"Yhdistys", L"Yhdistys", 440,0, 0, 0, 0},           // 10: yhdistys
	{F_JOUK, L"Joukkue", L"Joukkue", 440,0, 0, 0, 0},            // 11: joukkue
	{F_ALISRJ, L"Alisarja", L"Alisarja", 440,0, 0, 0, 0},        // 12: alisarja
	{F_TLS, L"Tulos", L"Tulos",640,10, 1, 0, 1},                 // 13: tulos
	{F_ERO, L"Ero", L"Ero", 730,0, 1, 0, 0},                   	// 14: ero k‰rkeen
	{F_SAK, L"Sakot", L"Sakot",730,0, 1, 0, 0},                  // 15: sakot
	{F_TARK, L"Leim.hyv", L"Leim.hyv", 910,0, 0, 0, 0},          // 16: Leimantarkastus
	{F_AIKA, L"Aika", L"Aika", 730, 0, 1, 0, 0},                 // 17: Aika ilman sakkoa
	{F_PIST, L"Pisteet", L"Pisteet", 730, 0, 1, 0, 0},           // 18: pisteet
	{F_OSASJ, L"2.sija", L"2.sj", 740,4, 1, 0, 1},             // 19: 2. sija
	{F_OSATLS, L"2.tulos", L"2.tulos", 790,10, 1, 0, 1},         // 20: 2. tulos
	{0, 0, 0, 0, 0, 0, 0}
	};

FldFrmtTp prtfldsm_suunn[] = {
	{F_SJ, L"Sija", L"Sija",0, 4, 1, 0, 1},                   	// 0: sija
	{F_KNO, L"Nro", L"Nro", 50, 4, 1, 0, 1},                   	// 1: numero
	{F_NIMI, L"Nimi", L"Nimi",100,27,0, 0, 1},                   // 2: nimi
	{F_ETUNIMI, L"Etunimi", L"Etunimi", 100, 0, 0, 0, 0},            //
	{F_SUKUNIMI, L"Sukunimi", L"Sukunimi", 100, 0, 0, 0, 0},          //
	{F_ARVO, L"Arvo", L"Arvo", 100,0,0, 0, 0},                   // 3: arvo
	{F_LISNO, L"Lisenssi", L"Lisenssi",345,0, 1, 0, 0},          // 4: lisenssi
	{F_LISNO2, L"KvId", L"KvId", 345,0, 1, 0, 0},               	// 5: Kv.id
	{F_BADGE, L"Badge", L"Badge", 345,0, 1, 0, 0},               // 6: badge
	{F_SRA, L"Seura", L"Seura", 345, 25, 0, 0, 1},               // 7: seura
	{F_SRALYH, L"Seuralyh", L"Seuralyh", 440,0, 0, 0, 0},        // 8: lyhenne
	{F_MAA, L"Maa", L"Maa", 605, 0, 0, 0, 0},					// 9: maa
	{F_YHD, L"Yhdistys", L"Yhdistys", 440,0, 0, 0, 0},           // 10: yhdistys
	{F_JOUK, L"Joukkue", L"Joukkue", 440,0, 0, 0, 0},            // 11: joukkue
	{F_ALISRJ, L"Alisarja", L"Alisarja", 440,0, 0, 0, 0},        // 12: alisarja
	{F_TLS, L"Tulos", L"Tulos",630,10, 1, 0, 1},                 // 13: tulos
	{F_ERO, L"Ero", L"Ero", 730,0, 1, 0, 0},                   	// 14: ero k‰rkeen
	{F_SAK, L"Sakot", L"Sakot",730,0, 1, 0, 0},                  // 15: sakot
	{F_TARK, L"Leim.hyv", L"Leim.hyv", 910,1, 0, 0, 1},          // 16: Leimantarkastus
	{F_AIKA, L"Aika", L"Aika", 690, 0, 1, 0, 0},                 // 17: Aika ilman sakkoa
	{F_PIST, L"Pisteet", L"Pisteet", 690, 0, 1, 55, 0},           // 18: pisteet
	{F_OSASJ, L"2.sija", L"2.sj", 740,4, 1, 0, 1},             // 19: 2. sija
	{F_OSATLS, L"2.tulos", L"2.tulos", 790,10, 1, 0, 1},         // 20: 2. tulos
	{0, 0, 0, 0, 0, 0, 0}
	};

FldFrmtTp prtflds1[] = {
	{F_SJ, L"Sija", L"Sija",0, 3, 1, 0, 1},                   	// 0: sija
	{F_KNO, L"Nro", L"Nro", 40, 4, 1, 0, 1},                   	// 1: numero
	{F_NIMI, L"Nimi", L"Nimi",90,27,0, 0, 1},                   	// 2: nimi
	{F_ETUNIMI, L"Etunimi", L"Etunimi", 90, 0, 0, 0, 0},            //
	{F_SUKUNIMI, L"Sukunimi", L"Sukunimi", 90, 0, 0, 0, 0},          //
	{F_ARVO, L"Arvo", L"Arvo", 90,0,0, 0, 0},                   	// 3: arvo
	{F_LISNO, L"Lisenssi", L"Lisenssi",325,0, 1, 0, 0},          // 4: lisenssi
	{F_LISNO2, L"KvId", L"KvId", 325,0, 1, 0, 0},               	// 5: Kv.id
	{F_BADGE, L"Badge", L"Badge", 325,0, 1, 0, 0},               // 6: badge
	{F_SRA, L"Seura", L"Seura", 325,0, 0, 0, 0},                	// 7: seura
	{F_SRALYH, L"Seuralyh", L"Seuralyh", 325,10, 0, 0, 1},       // 8: lyhenne
	{F_MAA, L"Maa", L"Maa", 530, 3, 0, 0, 0},					// 9: maa
	{F_YHD, L"Yhdistys", L"Yhdistys", 325,0, 0, 0, 1},           // 10: seura
	{F_JOUK, L"Joukkue", L"Joukkue", 325,0, 0, 0, 0},            // 11: seura
	{F_ALISRJ, L"Alisarja", L"Alisarja", 325,0, 0, 0, 0},        // 12: alisarja
	{F_TLS, L"Yht.tulos", L"Yht.tulos",560,8, 1, 80, 1},          // 13: tulos
	{F_ERO, L"Ero", L"Ero", 670,0, 1, 0, 0},                   	// 14: ero k‰rkeen
	{F_SAK, L"Sakot", L"Sakot",670,0, 1, 0, 0},                  // 15: sakot
	{F_TARK, L"Leim.hyv", L"Leim.hyv", 560,0, 0, 0, 0},          // 16: Leimantarkastus
	{F_AIKA, L"Aika", L"Aika", 650, 0, 1, 0, 0},                 // 17: Aika ilman sakkoa
	{F_PIST, L"Pisteet", L"Pisteet", 650, 0, 1, 55, 0},           // 18: pisteet
	{F_OSASJ, L"Osasija", L"Osasj", 650, 3, 1, 80, 1},          // 19: vaiheen tai v‰liajan sija
	{F_OSATLS, L"Osatulos", L"Osatls", 690,8, 3, 80, 1},        // 20: vaiheen tulos tai v‰liaika
	{0, 0, 0, 0, 0, 0, 0}
	};

FldFrmtTp fileflds_meri5[] = {
	{F_SJ, L"Sija", L"Sija", 0, 4, 1, 0, 1},                   	// 0: sija
	{F_KNO, L"Nro", L"Nro",  6, 4, 1, 0, 1},                   	// 1: numero
	{F_NIMI, L"Nimi", L"Nimi", 13,30,0, 0, 1},                   // 2: nimi
	{F_ETUNIMI, L"Etunimi", L"Etunimi", 13, 0, 0, 0, 0},            //
	{F_SUKUNIMI, L"Sukunimi", L"Sukunimi", 13, 0, 0, 0, 0},          //
	{F_ARVO, L"Arvo", L"Arvo", 13,10,0, 0, 1},                   // 3: arvo
	{F_LISNO, L"Lisenssi", L"Lisenssi", 45,0, 1, 0, 0},          // 4: lisenssi
	{F_LISNO2, L"1.laji", L"1.laji", 45,0, 1, 0, 0},             // 5: Kv.id
	{F_BADGE, L"2.laji", L"2.laji", 45,0, 1, 0, 0},              // 6: badge
	{F_SRA, L"3.laji", L"3.laji", 45, 0, 0, 0, 1},               // 7: seura
	{F_SRALYH, L"4.laji", L"4.laji", 45, 0, 0, 0, 0},            // 8: lyhenne
	{F_MAA, L"5.laji", L"5.laji", 45, 0, 0, 0, 0},               // 9: maa
	{F_YHD, L"Yhteens‰", L"Yhteens‰", 60,0, 0, 0, 0},            // 10: yhdistys
	{F_JOUK, L"Joukkue", L"Joukkue", 60,0, 0, 0, 0},             // 11: joukkue
	{F_ALISRJ, L"Alisarja", L"Alisarja", 60, 0, 0, 0, 0},        // 12: alisarja
	{F_TLS, L"Tulos", L"Tulos", 68,10, 1, 0, 1},                 // 13: tulos
	{F_ERO, L"Ero", L"Ero",  80,0, 1, 0, 1},                  	// 14: ero k‰rkeen
	{F_SAK, L"Sakot", L"Sakot",80,12, 1, 0, 1},                  // 15: sakot
	{F_TARK, L"Hyv‰ks.", L"Hyv‰ks.", 92, 0, 0, 0, 0},            // 16: Leimantarkastus
	{F_AIKA, L"Aika", L"Aika", 92, 0, 1, 0, 0},                 	// 17: Aika ilman sakkoa
	{F_PIST, L"Pisteet", L"Pisteet", 92, 6, 1, 0, 1},           // 18: pisteet
	{F_OSASJ, L"Osasija", L"Osasj", 95, 0, 1, 8, 0},           // 19: vaiheen tai v‰liajan sija
	{F_OSATLS, L"Osatulos", L"Osa", 99,6, 1, 8, 0},         // 20: vaiheen tulos tai v‰liaika
	{0, 0, 0, 0, 0, 0, 0}
	};

FldFrmtTp prtflds_meri5[] = {
	{F_SJ, L"Sija", L"Sija",0, 4, 1, 0, 1},                   	// 0: sija
	{F_KNO, L"Nro", L"Nro", 50, 4, 1, 0, 1},                   	// 1: numero
	{F_NIMI, L"Nimi", L"Nimi",100,27,0, 0, 1},                   // 2: nimi
	{F_ETUNIMI, L"Etunimi", L"Etunimi", 100, 0, 0, 0, 0},            //
	{F_SUKUNIMI, L"Sukunimi", L"Sukunimi", 100, 0, 0, 0, 0},          //
	{F_ARVO, L"Arvo", L"Arvo", 345,10,0, 0, 1},                  // 3: arvo
	{F_LISNO, L"Lisenssi", L"Lisenssi",345,0, 1, 0, 0},          // 4: lisenssi
	{F_LISNO2, L"KvId", L"KvId", 345,0, 1, 0, 0},               	// 5: Kv.id
	{F_BADGE, L"Badge", L"Badge", 345,0, 1, 0, 0},               // 6: badge
	{F_SRA, L"Seura", L"Seura", 500,20, 0, 0, 1},                // 7: seura
	{F_SRALYH, L"Seuralyh", L"Seuralyh", 440,0, 0, 0, 0},        // 8: lyhenne
	{F_MAA, L"Maa", L"Maa", 560, 0, 0, 0, 0},					// 9: maa
	{F_YHD, L"Yhdistys", L"Yhdistys", 440,0, 0, 0, 0},           // 10: yhdistys
	{F_JOUK, L"Joukkue", L"Joukkue", 440,0, 0, 0, 0},            // 11: joukkue
	{F_ALISRJ, L"Alisarja", L"Alisarja", 440,0, 0, 0, 0},        // 12: alisarja
	{F_TLS, L"Tulos", L"Tulos",620,10, 1, 0, 1},                 // 13: tulos
	{F_ERO, L"Ero", L"Ero", 720,0, 1, 0, 0},                   	// 14: ero k‰rkeen
	{F_SAK, L"Sakot", L"Sakot",720,12, 0, 0, 1},                 // 15: sakot
	{F_TARK, L"Leim.hyv", L"Leim.hyv", 820,0, 0, 0, 0},          // 16: Leimantarkastus
	{F_AIKA, L"Aika", L"Aika", 820, 0, 1, 0, 0},                 // 17: Aika ilman sakkoa
	{F_PIST, L"Pisteet", L"Pisteet", 840, 6, 1, 0, 1},           // 18: pisteet
	{0, 0, 0, 0, 0, 0, 0}
	};

FldFrmtTp prtfldsm_meri5[] = {
	{F_SJ, L"Sija", L"Sija",0, 4, 1, 0, 1},                   	// 0: sija
	{F_KNO, L"Nro", L"Nro", 50, 4, 1, 0, 1},                   	// 1: numero
	{F_NIMI, L"Nimi", L"Nimi",100,27,0, 0, 1},                   // 2: nimi
	{F_ETUNIMI, L"Etunimi", L"Etunimi", 100, 0, 0, 0, 0},            //
	{F_SUKUNIMI, L"Sukunimi", L"Sukunimi", 100, 0, 0, 0, 0},          //
	{F_ARVO, L"Arvo", L"Arvo", 330,10,0, 0, 1},                  // 3: arvo
	{F_LISNO, L"Lisenssi", L"Lisenssi",345,0, 1, 0, 0},          // 4: lisenssi
	{F_LISNO2, L"Kv.id", L"Kv.id", 345,0, 1, 0, 0},              // 5: Kv.id
	{F_BADGE, L"Badge", L"Badge", 345,0, 1, 0, 0},               // 6: badge
	{F_SRA, L"Seura", L"Seura", 480, 15, 0, 0, 1},                // 7: seura
	{F_SRALYH, L"Lyhenne", L"Lyhenne", 440,0, 0, 0, 0},          // 8: lyhenne
	{F_MAA, L"Maa", L"Maa", 560, 0, 0, 0, 0},					// 9: maa
	{F_YHD, L"Yhteens‰", L"Yhteens‰", 440,0, 0, 0, 0},           // 10: yhdistys
	{F_JOUK, L"Joukkue", L"Joukkue", 440,0, 0, 0, 0},            // 11: joukkue
	{F_ALISRJ, L"Alisarja", L"Alisarja", 440,0, 0, 0, 0},        // 12: alisarja
	{F_TLS, L"Tulos", L"Tulos",600,00, 1, 0, 0},                 // 13: tulos
	{F_ERO, L"Ero", L"Ero", 720,0, 1, 0, 0},                   	// 14: ero k‰rkeen
	{F_SAK, L"Sakot", L"Sakot",720,0, 1, 0, 0},                  // 15: sakot
	{F_TARK, L"Leim.hyv", L"Leim.hyv", 820,0, 0, 0, 0},          // 16: Leimantarkastus
	{F_AIKA, L"Aika", L"Aika", 820, 0, 1, 0, 0},                 // 17: Aika ilman sakkoa
	{F_PIST, L"Pisteet", L"Pisteet", 610, 6, 1, 55, 1},           // 18: pisteet
	{F_OSASJ, L"2.sija", L"2.sija", 700, 0, 1, 0, 0},            // 19: 2. sija
	{F_OSATLS, L"2.tulos", L" ", 665, 6, 1, 0, 1},             // 20: 2. tulos
	{0, 0, 0, 0, 0, 0, 0}
	};

FldFrmtTp prtflds1_meri5[] = {
	{F_SJ, L"Sija", L"Sija",0, 3, 1, 0, 1},                   	// 0: sija
	{F_KNO, L"Nro", L"Nro", 50, 4, 1, 0, 1},                   	// 1: numero
	{F_NIMI, L"Nimi", L"Nimi",100,27,0, 0, 1},                   	// 2: nimi
	{F_ETUNIMI, L"Etunimi", L"Etunimi", 90, 0, 0, 0, 0},            //
	{F_SUKUNIMI, L"Sukunimi", L"Sukunimi", 90, 0, 0, 0, 0},          //
	{F_ARVO, L"Arvo", L"Arvo", 330,0,0, 0, 0},                   	// 3: arvo
	{F_LISNO, L"Lisenssi", L"Lisenssi",335,0, 1, 0, 0},          // 4: lisenssi
	{F_LISNO2, L"KvId", L"KvId", 335,0, 1, 0, 0},               	// 5: Kv.id
	{F_BADGE, L"Badge", L"Badge", 335,0, 1, 0, 0},               // 6: badge
	{F_SRA, L"Seura", L"Seura", 480, 15, 0, 0, 0},                	// 7: seura
	{F_SRALYH, L"Seuralyh", L"Lyhenne", 480, 0, 0, 0, 1},       // 8: lyhenne
	{F_MAA, L"Maa", L"Maa", 560, 0, 0, 0, 0},					// 9: maa
	{F_YHD, L"Yhdistys", L"Yhdistys", 480,0, 0, 0, 1},           // 10: seura
	{F_JOUK, L"Joukkue", L"Joukkue", 480,0, 0, 0, 0},            // 11: seura
	{F_ALISRJ, L"Alisarja", L"Alisarja", 480,0, 0, 0, 0},        // 12: alisarja
	{F_TLS, L"Yht.tulos", L"Yht.tulos",560, 0, 1, 80, 1},          // 13: tulos
	{F_ERO, L"Ero", L"Ero", 670,0, 1, 0, 0},                   	// 14: ero k‰rkeen
	{F_SAK, L"Sakot", L"Sakot",670,0, 1, 0, 0},                  // 15: sakot
	{F_TARK, L"Leim.hyv", L"Leim.hyv", 560,0, 0, 0, 0},          // 16: Leimantarkastus
	{F_AIKA, L"Aika", L"Aika", 650, 0, 1, 0, 0},                 // 17: Aika ilman sakkoa
	{F_PIST, L"Pisteet", L"Pisteet", 650, 0, 1, 55, 0},           // 18: pisteet
	{F_OSASJ, L"Osasija", L"Osasj", 650, 0, 1, 55, 1},          // 19: vaiheen tai v‰liajan sija
	{F_OSATLS, L"Osatulos", L"Osat", 610, 6, 1, 55, 1},        // 20: vaiheen tulos tai v‰liaika
	{0, 0, 0, 0, 0, 0, 0}
	};

llfldtp tulkentta1[NLLFIELDS+NFLISA] = {
   L"Numero",     L"Numero",    0, 200, 200, 200,   0, 240, 240,   4,  4,  4,  4,  4,  4,  4,   1, 1, 1, 1, 1, 1, 1,
   L"Sarja",   	  L"Sarja",     0,   0,   0,  90,   0,   0,   0,   0, 10, 10, 10,  0, 10, 10,   0, 0, 0, 0, 0, 0, 0,
   L"Alisrj",     L"Alisrj",    0,   0,   0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,   0, 0, 0, 0, 0, 0, 0,
   L"Nimi",   	  L"Nimi",    250, 320, 320, 250, 290, 360, 360,  24, 24, 24, 24, 24, 24, 24,   0, 0, 0, 0, 0, 0, 0,
   L"Aika",   	  L"Aika",     70, 110, 110,   0,  70, 110, 110,   8,  8,  8,  8,  8,  8,  8,   0, 0, 0, 0, 0, 0, 0,
   L"Seura",   	  L"Seura",   500, 570,   0, 505, 540, 605,   0,  25, 25,  0, 25, 25, 25,  0,   0, 0, 0, 0, 0, 0, 0,
   L"Maa",   	  L"Maa",     760, 825,   0, 765, 800, 850,   0,   0,  0,  0,  0,  0,  0,  0,   0, 0, 0, 0, 0, 0, 0,
   L"Lisenssi",   L"Lisenssi",170, 240, 240, 250, 210, 280, 280,   8,  8,  8,  0,  8,  8,  8,   1, 1, 1, 1, 1, 1, 1,
   L"KvId",       L"KvId",    170, 240, 240, 250, 210, 280, 280,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"Emitnro",    L"Emitnro", 250, 320, 320, 250, 290, 360, 360,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"Emitnro2",   L"Emitnro2",250, 320, 320, 250, 290, 360, 360,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"Maksu",      L"Maksu",     0,   0, 570,   0,   0,   0, 610,   0,  0,  4,  0,  0,  0,  4,   1, 1, 1, 1, 1, 1, 1,
   L"Ranki",      L"Ranki",   805,   0,   0,   0, 800,   0,   0,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"Piste2",     L"Piste2",    0,   0,   0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"Lista",      L"Lista",     0,   0, 615,   0,   0,   0, 760,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"",           L"",          0,   0,   0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,   0, 0, 0, 0, 0, 0, 0
   };

llfldtp tulkentta2[NLLFIELDS+NFLISA] = {
   L"Numero",      L"Numero",    0,  20,  20,  21,   0,  24,  24,   4,  4,  4,  4,  4,  4,  4,   1, 1, 1, 1, 1, 1, 1,
   L"Sarja",       L"Sarja",     0,   0,   0,  10,   0,   0,   0,   0, 10, 10, 10,  0, 10, 10,   0, 0, 0, 0, 0, 0, 0,
   L"Alisrj",      L"Alisrj",    0,   0,   0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,   0, 0, 0, 0, 0, 0, 0,
   L"Nimi",        L"Nimi",     26,  34,  34,  26,  30,  38,  38,  28, 28, 28, 28, 28, 28, 28,   0, 0, 0, 0, 0, 0, 0,
   L"Aika",        L"Aika",      6,  11,  11,   0,   6,  11,  11,   8,  8,  8,  8,  5,  5,  5,   0, 0, 0, 0, 0, 0, 0,
   L"Seura",       L"Seura",    55,  63,   0,  55,  59,  67,   0,  30, 30,  0, 30, 30, 30,  0,   0, 0, 0, 0, 0, 0, 0,
   L"Maa",         L"Maa",      86,  84,   0,  86,  90,  98,   0,   0,  0,  0,  0,  0,  0,  0,   0, 0, 0, 0, 0, 0, 0,
   L"Lisenssi",    L"Lisenssi", 17,  25,  25,  26,  21,  29,  29,   8,  8,  8,  0,  8,  8,  8,   1, 1, 1, 1, 1, 1, 1,
   L"KvId",        L"KvId",     17,  25,  25,  26,  21,  29,  29,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"Emitnro",     L"Emitnro",  26,  34,  34,  26,  30,  38,  38,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"Emitnro2",    L"Emitnro2", 26,  34,  34,  26,  30,  38,  38,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"Maksu",       L"Maksu",     0,   0,  63,   0,   0,   0,  67,   0,  0,  4,  0,  0,  0,  4,   1, 1, 1, 1, 1, 1, 1,
   L"Ranki",       L"Ranki",    91,   0,   0,   0,  95,   0,   0,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"Piste2",      L"Piste2",    0,   0,   0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"Lista",       L"Lista",     0,   0,  69,   0,   0,   0,  72,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"",            L"",          0,   0,   0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,   0, 0, 0, 0, 0, 0, 0
   };

llfldtp tulkentta_suunn_1[NLLFIELDS+NFLISA] = {
   L"Numero",     L"Numero",    0, 200, 200, 200,   0, 240, 240,   4,  4,  4,  4,  4,  4,  4,   1, 1, 1, 1, 1, 1, 1,
   L"Sarja",   	  L"Sarja",     0,   0,   0,  90,   0,   0,   0,   0, 10, 10, 10,  0, 10, 10,   0, 0, 0, 0, 0, 0, 0,
   L"Alisrj",     L"Alisrj",    0,   0,   0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,   0, 0, 0, 0, 0, 0, 0,
   L"Nimi",   	  L"Nimi",    330, 400, 400, 330, 370, 440, 440,  24, 24, 24, 24, 24, 24, 24,   0, 0, 0, 0, 0, 0, 0,
   L"Aika",   	  L"Aika",     70, 110, 110,   0,  70, 110, 110,   8,  8,  8,  8,  8,  8,  8,   0, 0, 0, 0, 0, 0, 0,
   L"Seura",   	  L"Seura",   575, 645,   0, 580, 620, 685,   0,  25, 25,  0, 25, 25, 25,  0,   0, 0, 0, 0, 0, 0, 0,
   L"Maa",   	  L"Maa",     835, 900,   0, 840, 880, 930,   0,   0,  0,  0,  0,  0,  0,  0,   0, 0, 0, 0, 0, 0, 0,
   L"Lisenssi",   L"Lisenssi",160, 240, 240, 250, 210, 280, 280,   8,  8,  8,  0,  8,  8,  8,   1, 1, 1, 1, 1, 1, 1,
   L"KvId",       L"KvId",    160, 240, 240, 250, 210, 280, 280,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"Emitnro",    L"Emitnro", 250, 320, 320, 250, 290, 360, 360,   7,  7,  7,  7,  7,  7,  7,   1, 1, 1, 1, 1, 1, 1,
   L"Emitnro2",   L"Emitnro2",250, 320, 320, 250, 290, 360, 360,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"Maksu",      L"Maksu",     0,   0, 645,   0,   0,   0, 690,   0,  0,  4,  0,  0,  0,  4,   1, 1, 1, 1, 1, 1, 1,
   L"Ranki",      L"Ranki",   885,   0,   0,   0, 880,   0,   0,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"Piste2",     L"Piste2",    0,   0,   0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"Lista",      L"Lista",     0,   0, 690,   0,   0,   0, 740,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"",           L"",          0,   0,   0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,   0, 0, 0, 0, 0, 0, 0
   };

llfldtp tulkentta_suunn_2[NLLFIELDS+NFLISA] = {
   L"Numero",      L"Numero",    0,  20,  20,  21,   0,  24,  24,   4,  4,  4,  4,  4,  4,  4,   1, 1, 1, 1, 1, 1, 1,
   L"Sarja",       L"Sarja",     0,   0,   0,  10,   0,   0,   0,   0, 10, 10, 10,  0, 10, 10,   0, 0, 0, 0, 0, 0, 0,
   L"Alisrj",      L"Alisrj",    0,   0,   0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,   0, 0, 0, 0, 0, 0, 0,
   L"Nimi",        L"Nimi",     35,  42,  42,  35,  38,  46,  46,  28, 28, 28, 28, 28, 28, 28,   0, 0, 0, 0, 0, 0, 0,
   L"Aika",        L"Aika",      6,  11,  11,   0,   6,  11,  11,   8,  8,  8,  8,  5,  5,  5,   0, 0, 0, 0, 0, 0, 0,
   L"Seura",       L"Seura",    64,  71,   0,  64,  67,  75,   0,  30, 30,  0, 30, 30, 30,  0,   0, 0, 0, 0, 0, 0, 0,
   L"Maa",         L"Maa",      95,  92,   0,  95,  98, 106,   0,   0,  0,  0,  0,  0,  0,  0,   0, 0, 0, 0, 0, 0, 0,
   L"Lisenssi",    L"Lisenssi", 17,  25,  25,  26,  21,  29,  29,   8,  8,  8,  0,  8,  8,  8,   1, 1, 1, 1, 1, 1, 1,
   L"KvId",        L"KvId",     17,  25,  25,  26,  21,  29,  29,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"Emitnro",     L"Emitnro",  26,  34,  34,  26,  30,  38,  38,   7,  7,  7,  7,  7,  7,  7,   1, 1, 1, 1, 1, 1, 1,
   L"Emitnro2",    L"Emitnro2", 26,  34,  34,  26,  30,  38,  38,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"Maksu",       L"Maksu",     0,   0,  71,   0,   0,   0,  75,   0,  0,  4,  0,  0,  0,  4,   1, 1, 1, 1, 1, 1, 1,
   L"Ranki",       L"Ranki",   100,   0,   0,   0, 103,   0,   0,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"Piste2",      L"Piste2",    0,   0,   0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"Lista",       L"Lista",     0,   0,  77,   0,   0,   0,  80,   0,  0,  0,  0,  0,  0,  0,   1, 1, 1, 1, 1, 1, 1,
   L"",            L"",          0,   0,   0,   0,   0,   0,   0,   0,  0,  0,  0,  0,  0,  0,   0, 0, 0, 0, 0, 0, 0
   };

llfldtp *tulkentta[] = {tulkentta1, tulkentta2};

llparamtp llparam =
	{L"", L"", L"", L"", L"",
	{0, 0},
	{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}},
	{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}},
	{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}},
	{{1, 1, 1, 1, 1, 1, 1}, {0, 0, 0, 0, 0, 0, 0}},
	{{0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0}},
	{{70, 70, 70}, {7, 7, 7}},
	{{30, 30, 30, 30, 30, 30, 30}, {0, 0, 0, 0, 0, 0, 0}}
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

/*
typedef struct {
   int snimifl;
   int tpvafl;
   int tklofl;
   int otsikot;
   int vaihdanimet;
   int nollakarki;
   int marg;
   int tabs;
   int numlev;
   int levunit;
   int sivpit;
   int esirivit;
} tulosmuottp;
*/

tulosmuottp tulosmuot;
tulosmuottp filetulosmuot;
tulosmuottp mobiltulosmuot;

wchar_t init_fnt[PRKOODIPIT+1];
wchar_t pots_on[PRKOODIPIT+1];
wchar_t pots_pois[PRKOODIPIT+1];
wchar_t aots_on[PRKOODIPIT+1];
wchar_t aots_pois[PRKOODIPIT+1];
INT n_prtflds = sizeof(prtflds)/sizeof(FldFrmtTp)-1;
INT n_prtfldsm = sizeof(prtfldsm)/sizeof(FldFrmtTp)-1;
INT n_prtflds1 = sizeof(prtflds1)/sizeof(FldFrmtTp)-1;
int n_dspflds = sizeof(dspflds)/sizeof(FldFrmtTp)-1;
int n_fileflds = sizeof(fileflds)/sizeof(FldFrmtTp)-1;
int n_mobilflds = sizeof(mobilflds)/sizeof(FldFrmtTp)-1;

wchar_t paaots[125] = L"";
wchar_t sarjaots[125] = L"";
wchar_t alaots[141] = L"";

wchar_t initfont_emrap[PRKOODIPIT+1] =
   L"\x26\x1b&l26A\x1b&l0O\x1b(s0p10h0s0b4099T\x1b&l8C\x1b(10U";
wchar_t initfont_emva[PRKOODIPIT+1] =
   L"\x26\x1b&l26A\x1b&l1O\x1b(s0p15h0s0b4099T\x1b&l6C\x1b(10U";

INT valinelendef, vapagelendef, vapageskip, tarkrappageskip;
INT valinelen = 75, vapagelen = 55;

// K‰ytˆss‰ ovat luokat aa, ab, sarjalinkit, sarjanimi, sarjatiedot, tulokset, tulframe, runko

void suunn_lahtoluett(void)
{
}

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
	snimifl = 1;
	tpvafl = 1;   // mobil : 0
	tklofl = 1;   // mobil : 0
	lkmfl = 1;    // mobil : 0
	matkafl = 1;
	otsikot = 0;  // html, mobil : 2
	vaihdanimet = 0;
	nollakarki = 0;
	marg = 0;
	omarg = 20;
	ymarg = 30;
	amarg = 30;
	tabs = 0;
	numlev = 1;
	levunit = 0;  // 0: merkkej‰, 1: poins, 2: 1/1000 rivi
	sivpit = 99999;   // Kirjoitin : 62
	esirivit = 0;
	sarjalinkit = 2; // 0: Ei, mobil : 1: kerran, html : 2: toistuu
	tauluhtml = 3;   // 0: ei, 1: vapaa lev, 3: px-leveydet, 5: %-levydet
	riviryhma = 3;
	fontfamily = 0;
	fontsize = 1;       // kirjoitin 0, html: 1, mobil : 3;
	des_sep = L',';
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
	framecolor = 0xe8ffff;
	framefontcolor = 0;
	eborder = 1;
	efontsize = 80;
	thecolor = 0xd0ffff;
	thefontcolor = 0;
	tdecolor = 0xe8ffff;
	tdefontcolor = 0;
}

void init_muotoilut(void)
{
	tulosmuot.sivpit = 62;
	if (kuntosuunnmoodi == 1 || kuntosuunnmoodi == 2)
		filetulosmuot.otsikot = 1;
	mobiltulosmuot.sarjalinkit = 1;
	mobiltulosmuot.tauluhtml = 5;
	mobiltulosmuot.fontsize = 2;
	mobiltulosmuot.tpvafl = 0;
	mobiltulosmuot.tklofl = 0;
	mobiltulosmuot.lkmfl = 0;

	if (wcswcind(kilpparam.kilplaji, L"SB") >= 0) {
		memcpy(prtflds, prtflds_suunn, (n_prtflds+1)*sizeof(prtflds[0]));
		memcpy(prtfldsm, prtfldsm_suunn, (n_prtfldsm+1)*sizeof(prtfldsm[0]));
		memcpy(fileflds, fileflds_suunn, (n_fileflds+1)*sizeof(fileflds[0]));
		memcpy(tulkentta1, tulkentta_suunn_1, sizeof(tulkentta1));
		memcpy(tulkentta2, tulkentta_suunn_2, sizeof(tulkentta2));
		}
	if (wcswcind(kilpparam.kilplaji, L"NP") >= 0) {
		memcpy(prtflds, prtflds_meri5, (n_prtflds+1)*sizeof(prtflds[0]));
		memcpy(prtfldsm, prtfldsm_meri5, (n_prtfldsm+1)*sizeof(prtfldsm[0]));
		memcpy(prtflds1, prtflds1_meri5, (n_prtflds1+1)*sizeof(prtflds1[0]));
		memcpy(fileflds, fileflds_meri5, (n_fileflds+1)*sizeof(fileflds[0]));
		memcpy(mobilflds, fileflds_meri5, (n_mobilflds+1)*sizeof(mobilflds[0]));
		}
}

static const wchar_t *ffstr[] = {L"Verdana,Arial,Sans-serif", L"Times,Serif", L"'Monaco','Lucida Console','Courier','Monospace'"};
static const wchar_t *fszstr[] = {L"small", L"medium", L"large", L"x-large", L"xx-large"};
static wchar_t cssstr[] = L"<link rel=\"stylesheet\" type=\"text/css\" href=\"tuloslue.css\">\n";
static const wchar_t *fwgtstr[] = {L"normal", L"bold"};

wchar_t *css_string(tulostusparamtp *tulprm, int laji)
{
	static wchar_t prs[5000];
	static INT bgred = 0xe8, bggreen = 0xff, bgblue=0xff;
	int colW[] = {35, 200, 28, 48, 66};
	int fsz = 30, fsz0 = 130;

	if (tulprm->tulmuot.cssfl == 1)
		tulprm->writehtml(cssstr);
	else if (tulprm->tulmuot.cssfl == 0) {
		wcscpy(prs, L"<style>\n");
		if (tulprm->tulmuot.tauluhtml) {
			tulprm->setActFlds(-1);
			wchar_t bddefst[80] = L"", tbldefst[80] = L"";
			int bdr = 0, cl2, fntcl2, clh, fntclh;
			if (tulprm->tulostettava == L'E' || tulprm->TlsJaRva) {
				fsz = tulprm->tulmuot.efontsize + fsz;
				bdr = tulprm->tulmuot.eborder;
				cl2 = tulprm->tulmuot.tdecolor;
				fntcl2 = tulprm->tulmuot.tdefontcolor;
				clh = tulprm->tulmuot.thecolor;
				fntclh = tulprm->tulmuot.thefontcolor;
				swprintf(tbldefst, L".rvatbl {font-size:%d%%; }\n", tulprm->tulmuot.efontsize);
				}
			else {
				bdr = tulprm->tulmuot.border;
				cl2 = tulprm->tulmuot.td2color;
				fntcl2 = tulprm->tulmuot.td2fontcolor;
				clh = tulprm->tulmuot.thcolor;
				fntclh = tulprm->tulmuot.thfontcolor;
				if (tulprm->tulmuot.tauluhtml & 4)
					wcscpy(tbldefst, L"table {width:100%;}\n");
				}

			if (bdr) {
				swprintf(bddefst, L"table, td, th {border: %dpx solid #%6.6x;}\n",
					bdr, tulprm->tulmuot.bordercolor);
				}
			else {
				wcscpy(bddefst, L"table, td, th {border: 0px none;}\n");
				}
			tulprm->writehtml(prs);
			swprintf(prs,
				L"body {background-color: #%6.6x; color: #%6.6x; font-family:%s; font-size:%s;}\n"
				L".runko {background-color: #%6.6x; color: #%6.6x;}\n"
				L"h2 {font-size:%d%%;}\n"
				L"h3 {font-size:%d%%}\n"
				L"%s"
				L"table {border-collapse: collapse;}\n"
				L"%s"
				L"td, th {padding:%dpx %dpx %dpx %dpx;}\n"
				L"th {font-weight:%s; background-color: #%6.6x; color: #%6.6x;}\n",
				tulprm->tulmuot.bgcolor, tulprm->tulmuot.fontcolor,
				ffstr[tulprm->tulmuot.fontfamily], fszstr[tulprm->tulmuot.fontsize],
				tulprm->tulmuot.framecolor, tulprm->tulmuot.framefontcolor,
				tulprm->tulmuot.paaotskoko, tulprm->tulmuot.alaotskoko,
				tbldefst,
				bddefst,
				tulprm->tulmuot.vpad, tulprm->tulmuot.hpad, tulprm->tulmuot.vpad, tulprm->tulmuot.hpad,
				fwgtstr[tulprm->tulmuot.sarotsbold ? 1 : 0], clh, fntclh);
			tulprm->writehtml(prs);
			for (int ig = 0; ig < 2; ig++) {
				prs[0] = 0;
				for (int ii = 0; ii < tulprm->tulmuot.riviryhma; ii++) {
					swprintf(prs + wcslen(prs), L".restbl tr:nth-of-type(%dn+%d),\n",
						2*tulprm->tulmuot.riviryhma, ig*tulprm->tulmuot.riviryhma + ii + (tulprm->tulmuot.otsikot ? 2 : 1));
					}
				swprintf(prs + wcslen(prs) - 2, L" {background-color: #%6.6x; color: #%6.6x;}\n",
					ig == 0 ? tulprm->tulmuot.td1color : cl2, ig == 0 ? tulprm->tulmuot.td1fontcolor : fntcl2);
				tulprm->writehtml(prs);
				}
			for (int ii = 0; ii < tulprm->n_aFlds; ii++) {
				swprintf(prs,
					L".restbl tr td:nth-child(%d),\n"
					L".restbl tr th:nth-child(%d) {text-align: %s; %s: %d%s; }\n", ii+1, ii+1,
						tulprm->aFld[ii].align ? L"right" : L"left",
						 (tulprm->tulmuot.tauluhtml & 4) ? L"width" : L"min-width",
						 tulprm->aFld[ii].len * ((tulprm->tulmuot.tauluhtml & 4) ? 100 / tulprm->totW : 1000 / fsz0),
						 (tulprm->tulmuot.tauluhtml & 4) ? L"%%" : L"px");
				tulprm->writehtml(prs);
				}
//			wcscpy(prs, L".rvatbl tr {text-align: right;}\n");
//			tulprm->writehtml(prs);
			if (tulprm->tulostettava == L'E' || tulprm->TlsJaRva) {
				if (tulprm->TlsJaRva == 2 || tulprm->piste == 3) {
					wchar_t *rvarv[2] =
						{L"border-bottom-style: none; padding-bottom: 0; padding-top:",
						L"border-top-style: none; padding-top: 0; padding-bottom:"};

					prs[0] = 0;
					for (int ii = 0; ii < 4; ii++) {
						swprintf(prs + wcslen(prs), L".rvatbl tr:nth-of-type(4n+%d) td"
							L" {background-color: #%6.6x; color: #%6.6x; %s %dpx;}\n",
							ii + 2,
							ii < 2 ? tulprm->tulmuot.td1color : cl2, ii < 2 ? tulprm->tulmuot.td1fontcolor : fntcl2,
							rvarv[ii%2], tulprm->tulmuot.vpad+2);
						}
					tulprm->writehtml(prs);
					}
				else {
					for (int ig = 0; ig < 2; ig++) {
						prs[0] = 0;
						for (int ii = 0; ii < tulprm->tulmuot.riviryhma; ii++)
							swprintf(prs + wcslen(prs), L".rvatbl tr:nth-of-type(%dn+%d),\n",
								2*tulprm->tulmuot.riviryhma, ig*tulprm->tulmuot.riviryhma+ii+2);
						swprintf(prs + wcslen(prs) - 2, L" {background-color: #%6.6x; color: #%6.6x;}\n",
							ig == 0 ? tulprm->tulmuot.td1color : cl2, ig == 0 ? tulprm->tulmuot.td1fontcolor : fntcl2);
						}
					tulprm->writehtml(prs);
					}
				swprintf(prs,
					L".rvatbl tr td:nth-child(1),\n"
					L".rvatbl tr th:nth-child(1) {text-align: right; min-width: %dpx; }\n", colW[0]*fsz/fsz0);
				tulprm->writehtml(prs);
				swprintf(prs,
					L".rvatbl tr td:nth-child(2),\n"
					L".rvatbl tr th:nth-child(2) {text-align: left; min-width: %dpx; }\n", colW[1]*fsz/fsz0);
				tulprm->writehtml(prs);
				swprintf(prs,
					L".rvatbl tr td:nth-child(2n+3):not(:last-child),\n"
					L".rvatbl tr th:nth-child(2n+3):not(:last-child) {border-right-style: none; padding-right: 0; font-style: italic; text-align: right; min-width: %dpx; }\n", colW[2]*fsz/fsz0);
				tulprm->writehtml(prs);
				swprintf(prs,
					L".rvatbl tr td:nth-child(2n+4),\n"
					L".rvatbl tr th:nth-child(2n+4) {border-left-style: none; text-align: right; min-width: %dpx; }\n", colW[3]*fsz/fsz0);
				tulprm->writehtml(prs);
				swprintf(prs,
					L".rvatbl tr td:nth-child(2n+3):last-child,\n"
					L".rvatbl tr th:nth-child(2n+3):last-child {text-align: right; width: %dpx;}\n", colW[4]*fsz/fsz0);
				tulprm->writehtml(prs);
				}
			prs[0] = 0;
//				}
			}
		else {
			swprintf(prs + wcslen(prs),
				L"body {background-color: #%6.6x; color: #%6.6x; font-family:%s; font-size:%s;}\n"
				L"h2 {font-size:%d%%;}\n"
				L"h3 {font-size:%d%%}\n"
				L".sarjalinkit {font-family:%s;}\n"
				L".tulokset {background-color: #%6.6x; color: #%6.6x; font-family:%s;}\n"
				L".runko {background-color: #%6.6x;}\n",
				tulprm->tulmuot.bgcolor, tulprm->tulmuot.fontcolor,
				ffstr[tulprm->tulmuot.fontfamily], fszstr[tulprm->tulmuot.fontsize],
				tulprm->tulmuot.paaotskoko, tulprm->tulmuot.alaotskoko,
				ffstr[0], tulprm->tulmuot.bgcolor, tulprm->tulmuot.fontcolor, ffstr[2],
				tulprm->tulmuot.framecolor);
			}
		wcscat(prs, L"</style>\n");
		tulprm->writehtml(prs);
		}
	return(prs);
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
		if(swscanf(ln,L"%d %d %d %d %d %d %d %d %d %d %d %d %d %d\n", &prnt,
			&us, &tulosmuot.sivpit, &tulosmuot.tpvafl, &tulosmuot.tklofl, &ps, &tulosmuot.numlev,
			&tulosmuot.marg, &tulosmuot.snimifl, &tulosmuot.vaihdanimet, &tulosmuot.esirivit,
			&tulosmuot.otsikot, &tulosmuot.nollakarki, &tt) < 10) {
			iowerr(luetfnm,1, 1L);
			goto sulje;
			}
		}
	else {
		iowerr(luetfnm,1, 1L);
		goto sulje;
		}
	if (tt != -1)
		tulosmuot.lkmfl = tt != 0;
	filetulosmuot.tpvafl = tulosmuot.tpvafl;
	filetulosmuot.tklofl = tulosmuot.tklofl;
	filetulosmuot.lkmfl = tulosmuot.lkmfl;
	filetulosmuot.snimifl = tulosmuot.snimifl;
	filetulosmuot.vaihdanimet = tulosmuot.vaihdanimet;
	filetulosmuot.otsikot = tulosmuot.otsikot;
	filetulosmuot.nollakarki = tulosmuot.nollakarki;
	lsttulparam.tulmuot = tulosmuot;
#ifdef _CONSOLE
	if (prnt == GDIPRINTER && lsttulparam.printer != prnt) {
		writeerror_w(L"Windows kirjoitinajurin muotoilu ohitetaan", 0);
		return;
		}
	if (lsttulparam.printer == GDIPRINTER && lsttulparam.printer != prnt) {
		writeerror_w(L"Muotoilu ei sovi Windows kirjoitinajurille", 0);
		return;
		}
	lsttulparam.printer = prnt;
#else
	if (prnt != GDIPRINTER) {
		writeerror_w(L"Muotoilu ei sovi Windows kirjoitinajurille", 0, true);
		return;
		}
#endif
	fld = prtflds;
	nfld = sizeof(prtflds)/sizeof(FldFrmtTp)-1;
	for (;;) {
		for (j = 0; j < nfld; j++) {
			if (luetfmtf->ReadLine(ln, 100)) {
				if (swscanf(ln, L"%s %d %d %d\n", fld[j].ots,
					&fld[j].pos, &fld[j].len, &fld[j].oik) != 4) {
					iowerr(luetfnm,1, 1L);
					goto sulje;
					}
				for (p = fld[j].ots; *p; p++)
					if (*p == L'_')
						*p = L' ';
				}
			}
		if (fld == dspflds)
			break;
		if (fld == prtflds) {
#ifdef MAKI
			fld = prtfldsmk;
			nfld = sizeof(prtfldsmk)/sizeof(FldFrmtTp)-1;
			continue;
			}
		if (fld == prtfldsmk) {
			fld = prtfldsmy;
			nfld = sizeof(prtfldsmy)/sizeof(FldFrmtTp)-1;
			continue;
			}
		if (fld == prtfldsmy) {
#endif
			fld = prtfldsm;
			nfld = sizeof(prtfldsm)/sizeof(FldFrmtTp)-1;
			continue;
			}
		if (fld == prtfldsm) {
			fld = prtflds1;
			nfld = sizeof(prtflds1)/sizeof(FldFrmtTp)-1;
			continue;
			}
		if (fld == prtflds1) {
			fld = fileflds;
			nfld = sizeof(fileflds)/sizeof(FldFrmtTp)-1;
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
#ifdef MAKI
		luekoodi(luetfmtf, atxt_on);
		luekoodi(luetfmtf, atxt_pois);
#else
		luekoodi(luetfmtf, initfont_emva);
#endif
		}
#endif
	memset(ln, 0, sizeof(ln));
	if (luetfmtf->ReadLine(ln, 82) == NULL) {
		iowerr(luetfnm, 1, 2L);
		goto sulje;
		}
	if (wcslen(ln) > 3) {
		wtrimline(ln);
		wcscpy(paaots, ln);
		}
	memset(ln, 0, sizeof(ln));
	if (luetfmtf->ReadLine(ln, 82) == NULL) {
		iowerr(luetfnm, 1, 2L);
		goto sulje;
		}
	if (wcslen(ln) > 3) {
		wtrimline(ln);
		wcscpy(sarjaots, ln);
		}
	memset(ln, 0, sizeof(ln));
	if (luetfmtf->ReadLine(ln, 82) == NULL) {
		iowerr(luetfnm, 1, 2L);
		goto sulje;
		}
	if (wcslen(ln) > 3) {
		wtrimline(ln);
		wcscpy(alaots, ln);
		}

sulje:
	if (luetfmtf)
		delete luetfmtf;

//	scale = 10;
#ifdef _CONSOLE
//	if (lsttulparam.printer == POSTSCRIPT || lsttulparam.printer == LJETII)
//		scale = 7;
//	else if (lsttulparam.printer == GDIPRINTER)
//		scale = 10;
//	else
//		scale = 1;
	if (lsttulparam.printer == LJETII) {
#if defined(EMITLEIMAT)
		vasivu();
#endif
//		if (init_fnt[0] && !tulkhp(init_fnt, 0, 0, 0, 0, &j, 0, 0, 0))
//			suhtfont = (j == 4099) ? 0 : 1;
		}
#endif
//	muotoiluluettu = 1;
}

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
	  writeerror_w(L"Tulostusmuotoilutiedoston kirjoitaminen ei onnistu!",0, true);
	  return;
	  }
   else {
	  swprintf(Buf,L"%d %d %d %d %d %d %d %d %d %d %d %d %d\n", printer,
		 1, tulosmuot.sivpit, tulosmuot.tpvafl, tulosmuot.tklofl, 0, tulosmuot.numlev,
		 tulosmuot.marg, tulosmuot.snimifl, tulosmuot.vaihdanimet, tulosmuot.esirivit,
		 tulosmuot.otsikot, tulosmuot.nollakarki);
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
#ifdef MAKI
			   fld = prtfldsmk;
			   nfld = sizeof(prtfldsmk)/sizeof(FldFrmtTp)-1;
			   continue;
			   }
			if (fld == prtfldsmk) {
			   fld = prtfldsmy;
			   nfld = sizeof(prtfldsmy)/sizeof(FldFrmtTp)-1;
			   continue;
			   }
			if (fld == prtfldsmy) {
#endif
			fld = prtfldsm;
			nfld = sizeof(prtfldsm)/sizeof(FldFrmtTp)-1;
			continue;
			}
		 if (fld == prtfldsm) {
			fld = prtflds1;
			nfld = sizeof(prtflds1)/sizeof(FldFrmtTp)-1;
			continue;
			}
		 if (fld == prtflds1) {
			fld = fileflds;
			nfld = sizeof(fileflds)/sizeof(FldFrmtTp)-1;
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
		 tallkoodi(luetfmtf, initfont_emva);
		 }
#endif
	  swprintf(Buf,L"%s*\n%s*\n%s*\n",paaots,sarjaots,alaots);
	  luetfmtf->WriteLine(Buf);
	  }
   delete luetfmtf;
   }

void kirj_ll_muotoilu(wchar_t *luetfnm) // , int laji)
{
	INT j, laji;
	wchar_t line[200];
	TextFl *luetfmtf = NULL;

	luetfmtf = new TextFl(luetfnm,L"wt");
	if (luetfmtf->IsOpen()) {
		for (laji = 0; laji < 2; laji++) {
			 swprintf(line,L"%d %d %d\n", 7, 60, 1);
			 luetfmtf->WriteLine(line);
			 for (j = 0; j < N_LL_LAJI; j++) {
				swprintf(line,L"%d %d %d %d %d %d %d %d %d\n",
				   1, llparam.pvfl[laji][j], llparam.klfl[laji][j],
				   1, llparam.marg[laji][j], llparam.matkafl[laji][j],
				   llparam.vaihdanimet[laji][j], llparam.tarknofl[laji][j],
				   j > 3 ? llparam.aikaporras[laji][j-4] : 0);
					luetfmtf->WriteLine(line);
				   }
			 for (j = 0; j < (int) NLLFIELDS; j++) {
				swprintf(line, L"%s\n", tulkentta[laji][j].nimi);
				luetfmtf->WriteLine(line);
				swprintf(line, L"Positiot:  %3d %3d %3d %3d %3d %3d %3d\n",
				   tulkentta[laji][j].pos[0], tulkentta[laji][j].pos[1],
				   tulkentta[laji][j].pos[2], tulkentta[laji][j].pos[3],
				   tulkentta[laji][j].pos[4], tulkentta[laji][j].pos[5],
				   tulkentta[laji][j].pos[6]);
				luetfmtf->WriteLine(line);
				swprintf(line, L"Leveydet:  %3d %3d %3d %3d %3d %3d %3d\n",
				   tulkentta[laji][j].len[0], tulkentta[laji][j].len[1],
				   tulkentta[laji][j].len[2], tulkentta[laji][j].len[3],
				   tulkentta[laji][j].len[4], tulkentta[laji][j].len[5],
				   tulkentta[laji][j].len[6]);
				luetfmtf->WriteLine(line);
				swprintf(line, L"Oikealle:  %3d %3d %3d %3d %3d %3d %3d\n",
				   tulkentta[laji][j].tas[0], tulkentta[laji][j].tas[1],
				   tulkentta[laji][j].tas[2], tulkentta[laji][j].tas[3],
				   tulkentta[laji][j].tas[4], tulkentta[laji][j].tas[5],
				   tulkentta[laji][j].tas[6]);
				luetfmtf->WriteLine(line);
				}
			}
		luetfmtf->WriteLine(llparam.luetots0);
		luetfmtf->WriteLine(L"\n");
		luetfmtf->WriteLine(llparam.luetots1);
		luetfmtf->WriteLine(L"\n");
		luetfmtf->WriteLine(llparam.luetots21);
		luetfmtf->WriteLine(L"\n");
		luetfmtf->WriteLine(llparam.luetots22);
		luetfmtf->WriteLine(L"\n");
		luetfmtf->WriteLine(llparam.luetots3);
		luetfmtf->WriteLine(L"\n");
		}
	delete luetfmtf;
}

void lue_ll_muotoilu(wchar_t *luetfnm, INT alku)
   {
   INT i, j, prnt, spit, nlev, us;
   wchar_t ss[20], line[200], ots[100];
   TextFl *luetfmtf = NULL;

   luetfmtf = new TextFl(luetfnm,L"rt");
   if (luetfmtf->IsOpen()) {
		for (int lj = 0; lj < 2; lj++) {
		   luetfmtf->ReadLine(line, 198);
		   if (swscanf(line,L"%d %d %d\n", &prnt, &spit, &nlev) < 3) {
			 iowerr(luetfnm,1, 1L);
			 goto sulje;
			 }
		  for (j = 0; j < N_LL_LAJI; j++) {
			 luetfmtf->ReadLine(line, 198);
			 if (swscanf(line, L"%d %d %d %d %d %d %d %d %d\n",
				&us, &llparam.pvfl[lj][j], &llparam.klfl[lj][j],
				&nlev, &llparam.marg[lj][j],
				&llparam.matkafl[lj][j], &llparam.vaihdanimet[lj][j], &llparam.tarknofl[lj][j],
				j > 3 ? &llparam.aikaporras[lj][j-4] : &i) < 9) {
				iowerr(luetfnm,1, 1L);
				goto sulje;
				}
			 }
		  for (j = 0; j < (int) NLLFIELDS; j++) {
			 if (luetfmtf->ReadLine(ots, 98) == NULL)
				break;
			 if (luetfmtf->ReadLine(line, 198) == NULL)
				break;
			 if (wmemcmp(ots, L"Emitnro", 7) == 0 && j == LLF_LISNO2)
				j++;
			 if (wmemcmp(ots, L"Maksu", 5) == 0 && j == LLF_EMIT2)
				j++;
			 if (swscanf(line, L"%s %d %d %d %d %d %d %d\n",
				ss, &tulkentta[lj][j].pos[0], &tulkentta[lj][j].pos[1],
					&tulkentta[lj][j].pos[2], &tulkentta[lj][j].pos[3],
					&tulkentta[lj][j].pos[4], &tulkentta[lj][j].pos[5],
					&tulkentta[lj][j].pos[6])
				!= 8) {
				iowerr(luetfnm,1, 1L);
				goto sulje;
				}
			 luetfmtf->ReadLine(line, 198);
			 if (swscanf(line, L"%s %d %d %d %d %d %d %d\n",
				ss, &tulkentta[lj][j].len[0], &tulkentta[lj][j].len[1],
					&tulkentta[lj][j].len[2], &tulkentta[lj][j].len[3],
					&tulkentta[lj][j].len[4], &tulkentta[lj][j].len[5],
					&tulkentta[lj][j].len[6])
				!= 8) {
				iowerr(luetfnm,1, 1L);
				goto sulje;
				}
			 luetfmtf->ReadLine(line, 198);
			 if (swscanf(line, L"%s %d %d %d %d %d %d %d\n",
				ss, &tulkentta[lj][j].tas[0], &tulkentta[lj][j].tas[1],
					&tulkentta[lj][j].tas[2], &tulkentta[lj][j].tas[3],
					&tulkentta[lj][j].tas[4], &tulkentta[lj][j].tas[5],
					&tulkentta[lj][j].tas[6])
				!= 8) {
				iowerr(luetfnm,1, 1L);
				goto sulje;
				}
			 }
		  }
	  if  (luetfmtf->ReadLine(llparam.luetots0, sizeof(llparam.luetots0)/2-1) == NULL) {
		 iowerr(luetfnm, 1, 2L);
		 goto sulje;
		 }
	  wtrimline(llparam.luetots0);
	  if  (luetfmtf->ReadLine(llparam.luetots1, sizeof(llparam.luetots1)/2-1) == NULL) {
		 iowerr(luetfnm, 1, 2L);
		 goto sulje;
		 }
	  wtrimline(llparam.luetots1);
	  if  (luetfmtf->ReadLine(llparam.luetots21, sizeof(llparam.luetots21)/2-1) == NULL) {
		 iowerr(luetfnm, 1, 2L);
		 goto sulje;
		 }
	  wtrimline(llparam.luetots21);
	  if  (luetfmtf->ReadLine(llparam.luetots22, sizeof(llparam.luetots22)/2-1) == NULL) {
		 iowerr(luetfnm, 1, 2L);
		 goto sulje;
		 }
	  wtrimline(llparam.luetots22);
	  if  (luetfmtf->ReadLine(llparam.luetots3, sizeof(llparam.luetots3)/2-1) == NULL) {
		 iowerr(luetfnm, 1, 2L);
		 goto sulje;
		 }
	  wtrimline(llparam.luetots3);
	  }
sulje:
   delete luetfmtf;
   }

#define FRM_Muotoilu    	100
#define FRM_TulosLuettelot	101
#define FRM_LahtoLuettelot	102
#define FRM_Kirjasinlajit	103
#define FRM_TulosLuettelo	104
#define FRM_LahtoLuettelo	105
#define FRM_Kentta        	106
#define FRM_Nimi        	111
#define FRM_Muuttuja		112
#define FRM_Otsikko     	113
#define FRM_Sijainti    	114
#define FRM_Merkkeja    	115
#define FRM_Leveys        	116
#define FRM_Tasaus        	117
#define FRM_Askel        	118
#define FRM_Aktiivinen 		119
#define FRM_Kohde			120
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
#define FRM_Lukumaarat      216
#define FRM_Linkit      	217
#define FRM_TaulHtml      	218
#define FRM_RiviRyhma      	219
#define FRM_PaaOts          221
#define FRM_SarjaOts        222
#define FRM_AlaOts          223
#define FRM_OtsMatka        224
#define FRM_LLOts0			301
#define FRM_LLOts1          302
#define FRM_LLOts21         303
#define FRM_LLOts22         304
#define FRM_LLOts3          305
#define FRM_NaytaMatka      306
#define FRM_TarkNo          307
#define FRM_AikaPorras      308
#define FRM_Kirjasin		309
#define FRM_FontFamily		310
#define FRM_Tuumalla   		311
#define FRM_Koko            312
#define FRM_Lihav           313
#define FRM_Kursiv          314
#define FRM_Rivivali        315
#define FRM_Vari            316
#define FRM_Viiva           317
#define FRM_Css             327
#define FRM_Desim           328

static tagListTp FrmTags[] = {
	{FRM_Muotoilu, L"Muotoilu"},
	{FRM_TulosLuettelot, L"TulosLuettelot"},
	{FRM_LahtoLuettelot, L"LahtoLuettelot"},
	{FRM_Kirjasinlajit, L"Kirjasinlajit"},
	{FRM_TulosLuettelo, L"TulosLuettelo"},
	{FRM_LahtoLuettelo, L"LahtoLuettelo"},
	{FRM_Kentta, L"Kentta"},
	{FRM_Nimi, L"Nimi"},
	{FRM_Muuttuja, L"Muuttuja"},
	{FRM_Otsikko, L"Otsikko"},
	{FRM_Sijainti, L"Sijainti"},
	{FRM_Leveys, L"Leveys"},
	{FRM_Tasaus, L"Tasaus"},
	{FRM_Askel, L"Askel"},
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
	{FRM_Lukumaarat, L"Lukumaarat"},
	{FRM_Linkit, L"Linkit"},
	{FRM_TaulHtml, L"TaulHtml"},
	{FRM_RiviRyhma, L"RiviRyhma"},
	{FRM_PaaOts, L"PaaOts"},
	{FRM_SarjaOts, L"SarjaOts"},
	{FRM_AlaOts, L"AlaOts"},
	{FRM_OtsMatka, L"OtsMatka"},
	{FRM_LLOts0, L"LLOts0"},
	{FRM_LLOts1, L"LLOts1"},
	{FRM_LLOts21, L"LLOts21"},
	{FRM_LLOts22, L"LLOts22"},
	{FRM_LLOts3, L"LLOts3"},
	{FRM_NaytaMatka, L"NaytaMatka"},
	{FRM_TarkNo, L"TarkNo"},
	{FRM_AikaPorras, L"AikaPorras"},
	{FRM_Kirjasin, L"Kirjasin"},
	{FRM_FontFamily, L"FontFamily"},
	{FRM_Tuumalla, L"Tuumalla"},
	{FRM_Koko, L"Koko"},
	{FRM_Lihav, L"Lihav"},
	{FRM_Kursiv, L"Kursiv"},
	{FRM_Rivivali, L"Rivivali"},
	{FRM_Vari, L"Vari"},
	{FRM_Viiva, L"Viiva"},
	{FRM_Css, L"CssTiedot"},
	{FRM_Desim, L"Desim"}
	};

static wchar_t *FRM_Kohteet[] = {L"None", L"Epson", L"HPLaserJet", L"Postscript", L"Proprinter",
	L"TxtTab", L"Canon", L"GDIprinter"};
static wchar_t *FRM_Lajit[] = {L"Kirjoitin_1v", L"Kirjoitin_vy", L"Kirjoitin_kaikki", L"Tiedosto", L"Mobiili", L"N‰yttˆ"};
static wchar_t *FRM_Units[] = {L"Merkit", L"Points", L"Rivi/1000"};
static wchar_t *FRM_FontKohde[] = {L"Perus", L"Otsikko", L"Alaots", L"Vakiolev"};
static wchar_t *FRM_FontFamilyName[] = {L"Times New Roman", L"Arial", L"Arial Narrow", L"Calibri", L"Courier New", L"Lucida Console"};
static wchar_t *FRM_LlLajit[] = {L"Sarjat", L"Aakkos", L"Seurat", L"L‰hdˆt", L"Sarjat-2", L"Aakkos-2", L"Seurat-2"};
static wchar_t *FRM_LlKohde[] = {L"Kirjoitin", L"Tiedosto"};
static wchar_t *FRM_LlKentta[] = {L"KilpNo", L"Sarja", L"AliSarja", L"Nimi", L"Aika", L"Seura", L"Maa", L"LisNo", L"KvId",
	L"Badge", L"Badge2", L"Maksu", L"Ranki", L"Piste2", L"Lista", L"Aika2"};
static wchar_t *FRM_VariKohde[] = {L"Tausta", L"Merkit", L"RiviTausta1", L"RiviMerkit1", L"RiviTausta2", L"RiviMerkit2",
	L"SarOtsTausta", L"SarOtsMerkit", L"Viivat", L"KehysTausta", L"KehysMerkit", L"RiviTaustaRva", L"RiviMerkitRva",L"SarOtsTaustaRva", L"SarOtsMerkitRva"};
static wchar_t *FRM_TauluHtml[] = {L"tila=0", L"tila=1", L"tila=2", L"tila=3", L"tila=4", L"tila=5"};

static int nFrmTags = sizeof(FrmTags)/sizeof(FrmTags[0]);
static GDIfontTp *fonts[] = {&DefFnt, &TitleFnt, &HeaderFnt, &CourFnt};
FldFrmtTp *frmflds[] = {prtflds, prtfldsm, prtflds1, fileflds, mobilflds, dspflds};
int n_frmflds[] = {n_prtflds, n_prtfldsm, n_prtflds1, n_fileflds, n_mobilflds, n_dspflds};

void TallXMLMuotoilu(wchar_t *MuotFile)
{
	TextFl *tallfl;
	int level = 0, nfld;
	wchar_t ln[400];

	tallfl = new TextFl(MuotFile, L"wt");

	if (tallfl->IsOpen()) {
		tallfl->WriteLine(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
		swprintf(ln, L"%s createTime=\"%s\" creator=\"HkKisaWin %s\"", L"Muotoilu", ISOdatetime(0,1), Build());
		tallfl->put_wtag(ln, level++);

		tallfl->put_wtag(XMLhae_tagName(FRM_TulosLuettelot, FrmTags, nFrmTags), level++);
		tallfl->put_wtagparams(XMLhae_tagName(FRM_Kohde, FrmTags, nFrmTags), L"laji=\"Kirjoitin\"", false, level++);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_SivuPit, FrmTags, nFrmTags), tulosmuot.sivpit, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Paivays, FrmTags, nFrmTags), (int) tulosmuot.tpvafl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_TulostusAika, FrmTags, nFrmTags), (int) tulosmuot.tklofl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Lukumaarat, FrmTags, nFrmTags), (int) tulosmuot.lkmfl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_OtsMatka, FrmTags, nFrmTags), (int) tulosmuot.matkafl, level);
//		tallfl->put_wxml_d(XMLhae_tagName(FRM_NumLeveys, FrmTags, nFrmTags), (int) tulosmuot.numlev, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_VasMarg, FrmTags, nFrmTags), (int) tulosmuot.marg, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_OikMarg, FrmTags, nFrmTags), (int) tulosmuot.omarg, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_YlaMarg, FrmTags, nFrmTags), (int) tulosmuot.ymarg, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_AlaMarg, FrmTags, nFrmTags), (int) tulosmuot.amarg, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_ToistaSarja, FrmTags, nFrmTags), (int) tulosmuot.snimifl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_EtuSuku, FrmTags, nFrmTags), (int) tulosmuot.vaihdanimet, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_EsiRivit, FrmTags, nFrmTags), (int) tulosmuot.esirivit, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_SarakeOtsikot, FrmTags, nFrmTags), (int) tulosmuot.otsikot, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_NollaKarki, FrmTags, nFrmTags), (int) tulosmuot.nollakarki, level);
		wsprintf(ln, L"%d;%s", tulosmuot.pistedesim, tulosmuot.des_sep == L'.' ? L"Piste" : L"Pilkku");
		tallfl->put_wxml_s(XMLhae_tagName(FRM_Desim, FrmTags, nFrmTags), ln, level);
		tallfl->put_wantitag(XMLhae_tagName(FRM_Kohde, FrmTags, nFrmTags), --level);

		tallfl->put_wtagparams(XMLhae_tagName(FRM_Kohde, FrmTags, nFrmTags), L"laji=\"Tiedosto\"", false, level++);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Paivays, FrmTags, nFrmTags), (int) filetulosmuot.tpvafl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_TulostusAika, FrmTags, nFrmTags), (int) filetulosmuot.tklofl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Lukumaarat, FrmTags, nFrmTags), (int) filetulosmuot.lkmfl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_OtsMatka, FrmTags, nFrmTags), (int) filetulosmuot.matkafl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_VasMarg, FrmTags, nFrmTags), (int) filetulosmuot.marg, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_OikMarg, FrmTags, nFrmTags), (int) filetulosmuot.omarg, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_ToistaSarja, FrmTags, nFrmTags), (int) filetulosmuot.snimifl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_EtuSuku, FrmTags, nFrmTags), (int) filetulosmuot.vaihdanimet, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_NollaKarki, FrmTags, nFrmTags), (int) filetulosmuot.nollakarki, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Linkit, FrmTags, nFrmTags), (int) filetulosmuot.sarjalinkit, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_FontFamily, FrmTags, nFrmTags), (int) filetulosmuot.fontfamily, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_SarakeOtsikot, FrmTags, nFrmTags), filetulosmuot.otsikot + 16*filetulosmuot.sarotsbold, level);
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
		tallfl->put_wxml_d(XMLhae_tagName(FRM_RiviRyhma, FrmTags, nFrmTags), (int) filetulosmuot.riviryhma, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.td1color, level, L"kohde=RiviTausta1");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.td1fontcolor, level, L"kohde=RiviMerkit1");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.td2color, level, L"kohde=RiviTausta2");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) filetulosmuot.td2fontcolor, level, L"kohde=RiviMerkit2");
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

		tallfl->put_wtagparams(XMLhae_tagName(FRM_Kohde, FrmTags, nFrmTags), L"laji=\"Mobiili\"", false, level++);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Paivays, FrmTags, nFrmTags), (int) mobiltulosmuot.tpvafl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_TulostusAika, FrmTags, nFrmTags), (int) mobiltulosmuot.tklofl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Lukumaarat, FrmTags, nFrmTags), (int) mobiltulosmuot.lkmfl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_OtsMatka, FrmTags, nFrmTags), (int) mobiltulosmuot.matkafl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_VasMarg, FrmTags, nFrmTags), (int) mobiltulosmuot.marg, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_OikMarg, FrmTags, nFrmTags), (int) mobiltulosmuot.omarg, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_ToistaSarja, FrmTags, nFrmTags), (int) mobiltulosmuot.snimifl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_EtuSuku, FrmTags, nFrmTags), (int) mobiltulosmuot.vaihdanimet, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_NollaKarki, FrmTags, nFrmTags), (int) mobiltulosmuot.nollakarki, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Linkit, FrmTags, nFrmTags), (int) mobiltulosmuot.sarjalinkit, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_FontFamily, FrmTags, nFrmTags), (int) mobiltulosmuot.fontfamily, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_SarakeOtsikot, FrmTags, nFrmTags), mobiltulosmuot.otsikot + 16*mobiltulosmuot.sarotsbold, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Css, FrmTags, nFrmTags), (int) mobiltulosmuot.cssfl, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Koko, FrmTags, nFrmTags), (int) mobiltulosmuot.fontsize, level, L"kohde=Perus");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Koko, FrmTags, nFrmTags), (int) mobiltulosmuot.paaotskoko, level, L"kohde=PaaOts");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Koko, FrmTags, nFrmTags), (int) mobiltulosmuot.alaotskoko, level, L"kohde=AlaOts");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Koko, FrmTags, nFrmTags), (int) mobiltulosmuot.efontsize, level, L"kohde=RastiVa");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) mobiltulosmuot.bgcolor, level, L"kohde=Tausta");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) mobiltulosmuot.fontcolor, level, L"kohde=Merkit");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) mobiltulosmuot.framecolor, level, L"kohde=KehysTausta");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) mobiltulosmuot.framefontcolor, level, L"kohde=KehysMerkit");

		tallfl->put_wtagparams(XMLhae_tagName(FRM_TaulHtml, FrmTags, nFrmTags), FRM_TauluHtml[mobiltulosmuot.tauluhtml], false, level++);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_RiviRyhma, FrmTags, nFrmTags), (int) mobiltulosmuot.riviryhma, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) mobiltulosmuot.td1color, level, L"kohde=RiviTausta1");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) mobiltulosmuot.td1fontcolor, level, L"kohde=RiviMerkit1");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) mobiltulosmuot.td2color, level, L"kohde=RiviTausta2");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) mobiltulosmuot.td2fontcolor, level, L"kohde=RiviMerkit2");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) mobiltulosmuot.thcolor, level, L"kohde=SarOtsTausta");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) mobiltulosmuot.thfontcolor, level, L"kohde=SarOtsMerkit");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) mobiltulosmuot.tdecolor, level, L"kohde=RiviTaustaRva");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) mobiltulosmuot.tdefontcolor, level, L"kohde=RiviMerkitRva");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) mobiltulosmuot.thecolor, level, L"kohde=SarOtsTaustaRva");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) mobiltulosmuot.thefontcolor, level, L"kohde=SarOtsMerkitRva");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Vari, FrmTags, nFrmTags), (int) mobiltulosmuot.bordercolor, level, L"kohde=Viivat");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Viiva, FrmTags, nFrmTags), (int) mobiltulosmuot.border, level, L"kohde=Yleis");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_Viiva, FrmTags, nFrmTags), (int) mobiltulosmuot.eborder, level, L"kohde=RastiVa");
		tallfl->put_wxml_d(XMLhae_tagName(FRM_VasMarg, FrmTags, nFrmTags), (int) mobiltulosmuot.hpad, level);
		tallfl->put_wxml_d(XMLhae_tagName(FRM_YlaMarg, FrmTags, nFrmTags), (int) mobiltulosmuot.vpad, level);
		tallfl->put_wantitag(XMLhae_tagName(FRM_TaulHtml, FrmTags, nFrmTags), --level);

		tallfl->put_wantitag(XMLhae_tagName(FRM_Kohde, FrmTags, nFrmTags), --level);

		for (int laji = 0; laji < (int) sizeof(frmflds)/sizeof(frmflds[0]); laji++) {
			FldFrmtTp *fld;
			fld = frmflds[laji];
			nfld = n_frmflds[laji];
			tallfl->put_wtagparams(XMLhae_tagName(FRM_TulosLuettelo, FrmTags, nFrmTags),
				wconcat(L"laji=\"", FRM_Lajit[laji], L"\""), false, level++);
			for (int k = 0; k < nfld; k++) {
				tallfl->put_wtagparams(XMLhae_tagName(FRM_Kentta, FrmTags, nFrmTags),
					wconcat(L"nimi=\"", fld[k].name, L"\""), false, level++);
				tallfl->put_wxml_s(XMLhae_tagName(FRM_Otsikko, FrmTags, nFrmTags), fld[k].ots, level);
				tallfl->put_wxml_d(XMLhae_tagName(FRM_Sijainti, FrmTags, nFrmTags), fld[k].pos, level);
				tallfl->put_wxml_d(XMLhae_tagName(FRM_Leveys, FrmTags, nFrmTags), fld[k].len, level,
					 wconcat(L"yksikko=\"", FRM_Units[tulosmuot.levunit], L"\""));
				tallfl->put_wxml_d(XMLhae_tagName(FRM_Tasaus, FrmTags, nFrmTags), fld[k].oik, level);
				tallfl->put_wxml_d(XMLhae_tagName(FRM_Askel, FrmTags, nFrmTags), fld[k].askel, level);
				tallfl->put_wxml_d(XMLhae_tagName(FRM_Aktiivinen, FrmTags, nFrmTags), fld[k].akt, level);
				tallfl->put_wantitag(XMLhae_tagName(FRM_Kentta, FrmTags, nFrmTags), --level);
				}
			tallfl->put_wantitag(XMLhae_tagName(FRM_TulosLuettelo, FrmTags, nFrmTags), --level);
			}
		tallfl->put_wxml_s(XMLhae_tagName(FRM_PaaOts, FrmTags, nFrmTags), paaots, level);
		tallfl->put_wxml_s(XMLhae_tagName(FRM_SarjaOts, FrmTags, nFrmTags), sarjaots, level);
		tallfl->put_wxml_s(XMLhae_tagName(FRM_AlaOts, FrmTags, nFrmTags), alaots, level);
		tallfl->put_wantitag(XMLhae_tagName(FRM_TulosLuettelot, FrmTags, nFrmTags), --level);

		tallfl->put_wtag(XMLhae_tagName(FRM_LahtoLuettelot, FrmTags, nFrmTags), level++);
		for (int kohde = 0; kohde < 2; kohde++) {
			for (int laji = 0; laji < N_LL_LAJI; laji++) {
				swprintf(ln, L"kohde=\"%s\" laji=\"%s\"", FRM_LlKohde[kohde], FRM_LlLajit[laji]);
				tallfl->put_wtagparams(XMLhae_tagName(FRM_LahtoLuettelo, FrmTags, nFrmTags),
					 ln, false, level++);
				tallfl->put_wxml_d(XMLhae_tagName(FRM_Paivays, FrmTags, nFrmTags),
					(int) llparam.pvfl[kohde][laji], level);
				tallfl->put_wxml_d(XMLhae_tagName(FRM_TulostusAika, FrmTags, nFrmTags),
					(int) llparam.klfl[kohde][laji], level);
				tallfl->put_wxml_d(XMLhae_tagName(FRM_VasMarg, FrmTags, nFrmTags),
					(int) llparam.marg[kohde][laji], level);
				tallfl->put_wxml_d(XMLhae_tagName(FRM_NaytaMatka, FrmTags, nFrmTags),
					(int) llparam.matkafl[kohde][laji], level);
				tallfl->put_wxml_d(XMLhae_tagName(FRM_EtuSuku, FrmTags, nFrmTags),
					(int) llparam.vaihdanimet[kohde][laji], level);
				tallfl->put_wxml_d(XMLhae_tagName(FRM_TarkNo, FrmTags, nFrmTags),
					(int) llparam.tarknofl[kohde][laji], level);
				if (laji > 3)
					tallfl->put_wxml_d(XMLhae_tagName(FRM_AikaPorras, FrmTags, nFrmTags),
						(int) llparam.aikaporras[kohde][laji], level);

				for (int k = 0; k < NLLFIELDS; k++) {
					tallfl->put_wtagparams(XMLhae_tagName(FRM_Kentta, FrmTags, nFrmTags),
						 wconcat(L"nimi=\"", tulkentta[kohde][k].nimi, L"\""), false, level++);
					tallfl->put_wxml_s(XMLhae_tagName(FRM_Otsikko, FrmTags, nFrmTags), tulkentta[kohde][k].ots, level);
					tallfl->put_wxml_d(XMLhae_tagName(FRM_Sijainti, FrmTags, nFrmTags), tulkentta[kohde][k].pos[laji], level);
					tallfl->put_wxml_d(XMLhae_tagName(FRM_Leveys, FrmTags, nFrmTags), tulkentta[kohde][k].len[laji], level,
						 wconcat(L"laji=\"", FRM_Units[0], L"\""));
					tallfl->put_wxml_d(XMLhae_tagName(FRM_Tasaus, FrmTags, nFrmTags), tulkentta[kohde][k].tas[laji], level);

					tallfl->put_wantitag(XMLhae_tagName(FRM_Kentta, FrmTags, nFrmTags), --level);
					}
				tallfl->put_wantitag(XMLhae_tagName(FRM_LahtoLuettelo, FrmTags, nFrmTags), --level);
				}
			}
		tallfl->put_wxml_s(XMLhae_tagName(FRM_LLOts0, FrmTags, nFrmTags), llparam.luetots0, level);
		tallfl->put_wxml_s(XMLhae_tagName(FRM_LLOts1, FrmTags, nFrmTags), llparam.luetots1, level);
		tallfl->put_wxml_s(XMLhae_tagName(FRM_LLOts21, FrmTags, nFrmTags), llparam.luetots21, level);
		tallfl->put_wxml_s(XMLhae_tagName(FRM_LLOts22, FrmTags, nFrmTags), llparam.luetots22, level);
		tallfl->put_wxml_s(XMLhae_tagName(FRM_LLOts3, FrmTags, nFrmTags), llparam.luetots3, level);
		tallfl->put_wantitag(XMLhae_tagName(FRM_LahtoLuettelot, FrmTags, nFrmTags), --level);

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
				case FRM_Askel:
					node[*inode].gettext_int(&frmflds[laji][kentta].askel);
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
	int er = 0, DepthIn, haara = 0, iassign = -1, no = 0, val, ii;
	wchar_t ln[200], *p;
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
					else if (wcscmp(ln, L"Mobiili") == 0) {
						muot = &mobiltulosmuot;
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
					break;
				case FRM_TulostusAika:
					node[*inode].gettext_int(&val);
					tulosmuot.tklofl = val;
					break;
				case FRM_Lukumaarat:
					node[*inode].gettext_int(&val);
					tulosmuot.lkmfl = val;
					break;
				case FRM_OtsMatka:
					node[*inode].gettext_int(&val);
					tulosmuot.matkafl = val;
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
					break;
				case FRM_EtuSuku:
					node[*inode].gettext_int(&val);
					tulosmuot.vaihdanimet = val;
					break;
				case FRM_EsiRivit:
					node[*inode].gettext_int(&tulosmuot.esirivit);
					break;
				case FRM_SarakeOtsikot:
					node[*inode].gettext_int(&val);
					tulosmuot.otsikot = val;
					break;
				case FRM_NollaKarki:
					node[*inode].gettext_int(&val);
					tulosmuot.nollakarki = val;
					break;
				case FRM_Desim:
					node[*inode].gettext(ln, 198);
					if ((p = wcstok(ln, L";")) != NULL) {
						tulosmuot.pistedesim = _wtoi(ln);
						if ((p = wcstok(NULL, L";")) != NULL)
							tulosmuot.des_sep = (wmemcmpU(p, L"PISTE", 5) == 0) ? L'.' : L',';
						}
					filetulosmuot.pistedesim = tulosmuot.pistedesim;
					filetulosmuot.des_sep = tulosmuot.des_sep;
					mobiltulosmuot.pistedesim = tulosmuot.pistedesim;
					mobiltulosmuot.des_sep = tulosmuot.des_sep;
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
					case FRM_OtsMatka:
						node[*inode].gettext_int(&val);
						tulosmuot.matkafl = val;
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
						muot->vaihdanimet = val;
						break;
					case FRM_EsiRivit:
						node[*inode].gettext_int(&muot->esirivit);
						break;
					case FRM_SarakeOtsikot:
						node[*inode].gettext_int(&val);
						muot->otsikot = val & 3;
						muot->sarotsbold = (val & 16) / 16;
						break;
					case FRM_NollaKarki:
						node[*inode].gettext_int(&val);
						muot->nollakarki = val;
						break;
					case FRM_Desim:
						node[*inode].gettext(ln, 198);
						if ((p = wcstok(ln, L";")) != NULL) {
							muot->pistedesim = _wtoi(ln);
							if ((p = wcstok(NULL, L";")) != NULL)
								muot->des_sep = (wmemcmpU(p, L"PISTE", 5) == 0) ? L'.' : L',';
							}
						filetulosmuot.pistedesim = muot->pistedesim;
						filetulosmuot.des_sep = muot->des_sep;
						mobiltulosmuot.pistedesim = muot->pistedesim;
						mobiltulosmuot.des_sep = muot->des_sep;
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
								muot->thcolor = val;
								break;
							case 7:
								muot->thfontcolor = val;
								break;
							case 8:
								muot->bordercolor = val;
								break;
							case 9:
								muot->framecolor = val;
								break;
							case 10:
								muot->framefontcolor = val;
								break;
							case 11:
								muot->tdecolor = val;
								break;
							case 12:
								muot->tdefontcolor = val;
								break;
							case 13:
								muot->thecolor = val;
								break;
							case 14:
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
								muot->thcolor = val;
								break;
							case 7:
								muot->thfontcolor = val;
								break;
							case 8:
								muot->bordercolor = val;
								break;
							case 9:
								muot->framecolor = val;
								break;
							case 10:
								muot->framefontcolor = val;
								break;
							case 11:
								muot->tdecolor = val;
								break;
							case 12:
								muot->tdefontcolor = val;
								break;
							case 13:
								muot->thecolor = val;
								break;
							case 14:
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
static int tulkXMLLlKentta(int kohde, int laji, wchar_t *knt, xml_node *node, int *inode, int nnode)
{
	int er = 0, DepthIn, haara = 0, iassign = -1, no = 0, kentta;

	for (kentta = 0; kentta < NLLFIELDS; kentta++)
		if (wcscmp(knt, tulkentta[kohde][kentta].nimi) == 0)
			break;
	if (kentta == NLLFIELDS)
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
					node[*inode].gettext(tulkentta[kohde][kentta].ots,
						sizeof(tulkentta[kohde][kentta].ots)/2-1);
					break;
				case FRM_Sijainti:
					node[*inode].gettext_int(&tulkentta[kohde][kentta].pos[laji]);
					break;
				case FRM_Leveys:
					node[*inode].gettext_int(&tulkentta[kohde][kentta].len[laji]);
					break;
				case FRM_Tasaus:
					node[*inode].gettext_int(&tulkentta[kohde][kentta].tas[laji]);
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
static int tulkXMLLlMuotoilu(int kohde, int laji, xml_node *node, int *inode, int nnode)
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
				case FRM_Paivays:
					node[*inode].gettext_int(&val);
					llparam.pvfl[kohde][laji] = val;
					break;
				case FRM_TulostusAika:
					node[*inode].gettext_int(&val);
					llparam.klfl[kohde][laji] = val;
					break;
				case FRM_VasMarg:
					node[*inode].gettext_int(&llparam.marg[kohde][laji]);
					break;
				case FRM_NaytaMatka:
					node[*inode].gettext_int(&val);
					llparam.matkafl[kohde][laji] = val;
					break;
				case FRM_EtuSuku:
					node[*inode].gettext_int(&val);
					llparam.vaihdanimet[kohde][laji] = val;
					break;
				case FRM_TarkNo:
					node[*inode].gettext_int(&val);
					llparam.tarknofl[kohde][laji] = val;
					break;
				case FRM_AikaPorras:
					node[*inode].gettext_int(&val);
					llparam.aikaporras[kohde][laji] = val;
					break;
				case FRM_Kentta :
					node[*inode].getparam_text(L"nimi", ln, 30);
					tulkXMLLlKentta(kohde, laji, ln, node, inode, nnode);
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
static int tulkXMLLlMuotoilut(xml_node *node, int *inode, int nnode)
{
	int er = 0, DepthIn, haara = 0, iassign = -1, no = 0, val, lj;
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
				case FRM_LahtoLuettelo :
					node[*inode].getparam_text(L"kohde", ln, 30);
					val = haeStrIx(ln, FRM_LlKohde, sizeof(FRM_LlKohde)/sizeof(FRM_LlKohde[0]));
					node[*inode].getparam_text(L"laji", ln, 30);
					lj = haeStrIx(ln, FRM_LlLajit, sizeof(FRM_LlLajit)/sizeof(FRM_LlLajit[0]));
					tulkXMLLlMuotoilu(val, lj, node, inode, nnode);
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
				case FRM_LahtoLuettelot :
					tulkXMLLlMuotoilut(node, &inode, nnode);
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
	lsttulparam.tulmuot = tulosmuot;
	return(0);
}
//---------------------------------------------------------------------------

