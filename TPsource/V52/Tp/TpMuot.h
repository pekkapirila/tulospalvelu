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

#define MAXKENTTA 40

class KenttaTp {
	int			id;
	wchar_t		nimi[20];
	bool		Akt;
	int			Yks;
	int			fnt;
	int			pos;
	int			lev;
	int			tas;
	};

class TulosteTp {
public:
	int			id;
	wchar_t		luokka[10];		// Lähtö, Tulos
	wchar_t		alalaji;	// 
	int			vaiheLkm;	// 0: kaikki, 1, 2, ..: lkm
	wchar_t		nimi[40];
	int 		device;		// 0: kirjoitin, 1: teksti, 2: html
	int 		FontClass;  // 0: fixed, 1: suht
	GDIfontTp	Fnt[5];    // 0: leipä, 1: pääots, 2: alaots., 3: korost., 4: pieni
	wchar_t		kuvaus[100];
	bool		PaaOts;
	bool		AlaOts;
	bool		PSarja;
	int			Lukumaarat;
	int			YlaMarg;
	int			AlaMarg;
	int			VasMarg;
	int			OikMarg;
	int			VaiheAskel;
	bool		Paivays;
	bool		Kello;
	bool		SarakeOts;
	int			KenttaLkm;
	KenttaTp	Kentta[MAXKENTTA];
	};
