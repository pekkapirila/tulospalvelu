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

#define VERSIOKDI L"5.21"
#define BUILD L"5.21.002/2015-07-18"

#ifdef KEHITYSLISTA

writeerror_w toiminta
Tuloksen tallentaminen kuntosarjaan kansallisessa kisassa (maalilaji=4, ennakko=0) -- tehty
Ratatietojen säilyminen siirryttäessä uuden lisäämiseen                   -- kunnossa ??
- käytettäessä valikkotoimia
Emitanalyysin toimivuus ratojen uudelleen lukemisen jälkeen
Ryhmän (perheen) ilmoittautuminen ja tuloksen kirjaus yhdellä Emit-kortilla kuntokisaan

Muutoksia:

2015-07-18:  UnitIlm2, Valitse numerokenttä kokonaan, kun arvo on 0.
2015-07-14:  UnitTuloskaava: Estetään sisällöttömän kaavan ottaminen käyttöön
2015-07-14:  Käyttöön XE8 ja versio 5.21
2015-07-09:  HkInit: Korjattu VIP-korostusten käyttö
2015-07-08:  Lisätty mahdollisuus muuttaa yhden ampumapaikan sakkojen maksimimäärää
2015-07-07:  TulostusUnit2: Korjattu oman XML-standardin valinta
2015-07-07:	 HkTulostus, HkTls, etc.: Muutettu pisteisiin liittyvän takaa-ajon käsittelyä
2015-07-06:  HkKisaLahtLuett: Merkintä EI LÄHTIJÖILLE ajalle, jolle osuisi vain poissaoleva
2015-07-06:  HkTulostus: Korjaus autotulostuksen käynnistymiseen
2015-07-02:  HkTls, TulosUnit, TakaaAjoUnit: Pisteisiin perustuvan  takaa-ajon seurannan toteutus
2015-07-01:  HkTulostus: Korjaus yhteispisteiden tulostukseen (edeltävän muutoksen täydennys)
2015-06-30:  HkEmit: Emitanalyysi käynnistetään uudelleen ratojen uuden luennan jälkeen
2015-06-29:  HkTulostus, HkTls, TulosUnit: Pisteiden tulostus kisan aikana päivittyy
2015-06-11:  TulostusUnit2: Korjaus xml-starndardin valinnan sallimiseen
2015-06-07:  MuotoiluUnit, HkTulostus: Matkan tulostamisen vaihtoehtoja lisätty
2015-06-07:  HkTulostus, HkMuotoilu: Muutoksia muotoilun perusoletuksiin
2015-06-06:  HkTulostus, HkApu: Mahdollisuus sisällyttää otsikoihin tietoa vaiheesta
2015-06-06:  HkTulostus: Korjattu yhden vaiheen pisteiden tulostus
2015-06-04:  UnitSarjatiedot: Korjauksia ja täydennyksiä vaiheen luonteen käsittelyyn
2015-06-03:  UnitPistelasku: Ohjelma pyytää uudelleenkäynnistystä, kun yhteispisteiden käyttötapa vaatii uudelleenindeksoinnin
2015-05-31:  UnitEmit: Ohjelma laskee sarjaa/rataa vaihdettaessa tuloksen, jos se on mahdollista
2015-05-31:  HkTulostus: Korjaus automaattisen tiedostotulostuksen toimintaan ilman tiedoston AUTOFILE.LST käyttöä
2015-05-26:  Täsmennyksiä tiedostulostuksen merkistövalintaan
2015-05-25:  Kirjastofunktio StrToDouble hoitamaan vaihteleva desimaalimerkki
2015-05-25:  TulostusUnit2: Pikavalinnat kolmeen html-muotoiluun
2015-05-25:	 HkTulostus: Korjaus automaattiseen tiedostosiirtoon, kun ei otsikkoa
2015-05-25:	 UnitAjanotto: Rivin väritys lähdekoodin perusteella
2015-05-24:  HkIx: Korjattu numeroindeksi
2015-05-23:  UnitKilpMaaritys, HkTulostus: kv-lisenssiin voidaan sisällyttää maakoodi
2015-05-23:  UnitExport, UnitOsanottajat: osanottajien poisto pysyvästi
2015-05-23:  UnitPistelasku: Pisteiden laskenta aiemmille vaiheille
2015-05-23:  UnitAjanotto: Parannettu numeroiden syöttöä taulukossa
2015-05-10:  HkMuotoilu, MuotoiluUnit - Uusi valinta taulukon leveyden käsittelyyn.
2015-05-10:  HkTulostus, ym. - Korjauksia yli 200 väliajan tulostukseen
2015-05-09:  UnitAikaVertailu - parannus taulukon näyttämien rivien päivittymiseen
2015-05-09:  WinUdp, TpCom - korjaus yli 200 väliajan tiedonsiirtoon
2015-05-08:  UnitIlm2 - Toteuttamattomat järjestykset pois pudotusvalikosta
2015-05-08:  HkIx ym. - Kilpailijanumerot indeksiin idindex
2015-05-06:  HkDef.h - VALUKU = 250
2015-05-03:  TulostusUnit2 - option 0x01000000 kaksoiskäyttö hallintaan (lehdistö ja rastiväliajat)
2015-05-03:  HkMuotoilu - Emitkoodin tilavaraus lähtöluettelossa kirjoittimelle hieman suuremmaksi
2015-05-03:  HkTulostus, HkMuotoilu, MuotoiluUnit - mahdollisuus valita, lisätäänkö matka sarjaotsikkoon
2015-05-02:  UnitSarja, UnitASuunn, HkTls - korjauksia ja muutoksia ampumasuunnistuksen sakkojen käsittelyyn
2015-05-02:  UnitSakkoKierr - Taulukoiden Avoimet ja EdAvoimet indeksiksi tietueen sijainti eikä kilpno
2015-05-02:  HkInit, TapUnit2 - Taulukon viplaji indeksiksi tietueen sijainti eikä kilpno
2015-05-02:  HkIx - Taulukon yhdsija indeksiksi tietueen sijainti eikä kilpno
2015-05-02:  HkIx, HkEmit - Taulukon emitjarr indeksiksi tietueen sijainti eikä kilpno
2015-05-02:  TulostusUnit2, MuotiluUnitsparam initialisoidaan kaikissa ohjelman käyttötilanteissa
2015-05-01:  UnitIlm2 - Sarjatieto otetaan huomioon kaavakkeen näytettävän tilan varauksessa
2015-04-28:  UnitAjanotto - lisätty aika valittavissa 1 ms lisäyksellä aiempaan aikaan
2015-04-28:  HkTulostus - korjattu väliaikatulostus
2015-04-27:  TulosUnit - yhteistulosten näyttö vaiheen vaihdon jälkeen
2015-04-27:  UnitImport - Mahdollisuus kirjata Irman 1. vaiheen tiedot kaikkiin vaiheisiin
2015-04-26:  HkTulostus - Korjaus joukkuetuloksiin, kun samasta seurasta sallitaan useita joukkueita
2015-04-26:  HkAjat - Poistettu ylimääräinen otetun ajan kasvattaminen 0,0001 sek
2015-04-24:  HkEmit, HkMuotoilu, HkTulostus - Tilanne rasteilla ja rastivälit samaan taulukkoon
2015-04-23:  HkEmit, HkMuotoilu, HkTulostus - CSS3:n käyttöönotto
2015-04-22:  HkSrj - sarjayhdistelmä KAIKKI korjattu
2015-04-22:  HkEmit, HkMuotoilu, HkTulostus - html-tiedostojen tiivistäminen
2015-04-19:  UnitEmit, UnitHylkRap - Täydennyksiä hylkäysraprtion toteutukseen
2015-04-19:  UnitHylkRap - Toteutettu
2015-04-19:  TulostusUnit2 - Automaattisen tulostuksen käynnistäminen vaatii valintojen tekemisen
2015-04-17:  cbTpApu, UnitIlmoitus - Varoitussanoma toimimaan ongelmattomasti
2015-04-17:  UnitKilpailijatiedot - PvGrid rivin valinta esivalmistelutilassa, kun 1. vaihe puuttuu
2015-04-17:  HkEmit - lukijaleiman puuttumisen virhehallinta parannettu
2015-04-17:  HkInit - wmemcmp -> wmemcmpU
2015-04-16:  HkMuotoilu, MuotoiluUnit, HkEmit - rastiväliaikatuloste html-taulukkoon
2015-04-15:  HkEmit - Korjattu funktioiden getEmitJarr ja setEmitJarr kilpailijanumeron laillisuuden varmistus
2015-04-15:  WinHk ja UnitEmit - Selkeytetty Emit-kaavakkeen ja taustasäikeen käynnistymisvaihetta

#endif
