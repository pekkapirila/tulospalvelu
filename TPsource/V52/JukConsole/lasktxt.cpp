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

#if defined(MAALI) && defined(EMIT) && !defined(EILEIMAT)
#define EMITLEIMAT
#endif

#pragma pack(push,1)
typedef struct {
   int r;
   int c;
   char *t;
   } line;

typedef struct {
   int nl;
   line l[18];
   } scr;
#pragma pack(pop)

scr help0 = {
    9,
    2,0,"HELP,    sivu 1",
    4,0,"Sivu 2 :   KÄSITELTÄVÄN KILPAILIJAN VALINTA",
    6,0,"Sivu 3 :   SIIRTYMINEN KENTÄSTÄ TOISEEN, OSUUDEN VAIHTO",
    8,0,"Sivu 4 :   TIETOJEN SYÖTTÖ JA MUUTTAMINEN",
    10,0,"Sivu 5 :   YHTEISLÄHTÖ",
    12,0,"Sivu 6 :   TIEDONSIIRTO",
    14,0,"Sivu 7 :   KÄYTTÖ MAALIKELLONA",
	16, 0, "Sivu 8 :   KÄYTTÖ MAALIKELLONA, TOIMINTO 'MAALI'",
	18, 0, "Sivu 9 :   TOIMINTO 'MAALI', HAKUTOIMINNOT"
};

scr help1 = {
    9,
    2,0,"HELP,    sivu 2",
    4,0,"KÄSITELTÄVÄN KILPAILIJAN VALINTA",
    6,0,"Kilpailija valitaan normaalisti antamalla kilpaijanumero.",
    8,0,"Jos kilpailijanumeroa kysyttäessä painetaan suoraan <Return>",
    9,0,"siirrytään nimihakuun. Nimihaussa annetaan seuranimen alkuosa,",
    10,0,"painetaan <Return> ja täydennetään haku selaamalla tarvit-",
    11,0,"taessa näppäimillä 'S' ja 'E'. Saman seuran joukkueet tulevat",
    12,0,"ruudulle sarjoittain numerojärjestyksessä.",
    13,0,"Lopuksi painetaan 'H' tai <Return>."
    };

scr help2 = {
    11,
    2,0,"HELP,    sivu 3",
    4,0,"SIIRTYMINEN KENTÄSTÄ TOISEEN JA TARKASTELTAVAN OSUUDEN VAIHTO",
    6,0,"-  <Return>, '' ja <F9> : Siirry seuraavaan kenttään",
    7,0,"-  '' ja <F5>           : Siirry edelliseen kenttään",
    8,0,"-  PgDn ja <F10>         : Seuraava osuus",
    9,0,"-  PgUp ja <F6>          : Edellinen osuus",
    10,0,"-  <Tab>                 : Oikealle osuusalueella",
    11,0,"-  <Shift-Tab>           : Vasemmalle osuusalueella",
    12,0,"-  '+'                   : Poistu ruudusta TALLENTAEN tiedot",
    13,0,"-  <Esc>                 : Poistu ruudusta TALLENTAMATTA",
    14,27,"tehtyjä muutoksia" 
    };

scr help3 = {
    10,
    2,0,"HELP,    sivu 4     TIETOJEN SYÖTTÖ JA MUUTTAMINEN",
    9,0,"Aikoja ja kilpailunumeroa korjattaessa käytetään numeronäppäi-",
    10,0,"miä, nuolia '' ja '\x1A' sekä näppäimä <Home> ja <End>. Korjaus",
    11,0,"tapahtuu korvaustoimintana (Replace). Nuolien sijasta voidaan",
    12,0,"aina käyttää näppäimiä <F7> ja <F8>",
    14,0,"Muita tietoja korjattaessa voidaan käyttää lisäys (Insert) tai",
    15,0,"korvaustoimintaa; toiminta vaihdetaan näppäimellä <Ins>",
    16,0,"Kentän loppuosan poistaa yhdistelmä <Ctrl-End>",
    17,0,"Käytettävissä on myös näppäimet , \x1A F7, F8, Del, Askelpal.",
    19,0,"Poista tulos merkitsemällä maaliajaksi välilyöntejä."
    };

scr help4 = {
    17,
    2,0,"HELP,    sivu 5     YHTEISLÄHTÖ",
    4,0,"Yhteislähtöön osallistuminen voidaan kirjata:",
    5,0,"Automaattisesti käyttäen toimintoa A)setukset/L)ähdöt.",
    6,0,"Korjaustoiminnossa kirjoittamalla osuustietoalueella ensimmäisen",
    7,0,"    kyseisestä joukkueesta yhteislähtöön osallistuvan osuuden",
    8,0,"    kohdalle viimeiseen sarakkeeseen (sarake Y) kirjain 'Y'.",
    9,0,"Laskentatoiminnossa lopettamalla ajan syöttö painallukseen 'Y'",
    10,0,"    Tämä voidaan tehdä syötettäessä yhteislähtöön osallistuvaa",
    11,0,"    tai edellistä kilpailijaa",
    12,0,"Jos yhteislähtömerkintä tehdään viimeistä osuutta aiemmalle",
    13,0,"osuudelle, merkitään kaikki loput osuudet yhteislähtöön osal-",
    14,0,"listuviksi, ellei tätä ole muutettu valinnassa A)setukset/L)ähdöt",
    15,0,"Tarkasteltaessa yhteislähtöön osallistuvaa osuutta näkyy kuva-",
    16,0,"ruudulla yhteislähdön aika, joka annetaan ensimmäistä yhteis-",
    17,0,"lähtömerkintää tehtäessä.  Aika voidaan vaihtaa, jos yhteislähtö",
    18,0,"siirtyy tai yhteislähtöjä on useampia. Muutos voidaa tehdä auto-",
    19,0,"maattisesti tai erikseen jokaiselle yhteislähtöön osallistuvalle." 
    };

scr help5 = {
    5,
    2,0,"HELP,    sivu 6      TIEDONSIIRTO",
    4,0,"Tiedonsiirron tilanne näkyy ruudulla sen ollessa käytössä.",
	6,0, "  Myös aikataulukon tiedot siirretään toiseen MAALI-ohjelmaa",
	7,0, "käyttävään koneeseen, jos käytetään käynnistysparametria",
	8,0, "LÄHAIKA1 tai LÄHAIKA2 molemmissa koneissa."
};

scr help6 = {
   14,
    2,0,"HELP,    sivu 7     KÄYTTÖ MAALIKELLONA",
    4,0,"Ajanotto käynnistetään näppäimellä valitsemalla toiminto M)aali.",
	6,0,"Käynnistyksen yhteydessä ohjelma kysyy aikojen tallennustiedostoa,",
	7,0,"jota ei yleensä kannata muuttaa sekä ajanottoon käytettävää,",
	8,0,"näppäintä, jota ei tämän jälkeen voi käyttää mihinkään muuhun.",
    9,0,"  Toiminnossa 'Laskenta' saadaan aikamuistin vanhin aika kil-",
    10,0,"pailijalle näppäimellä 'S'. Seuraavassa valikossa voidaan hakea",
    11,0,"seuraava tai edellinen aika näppäimillä 'S' ja 'E' (toistettavia)",
    12,0,"  Esillä olevalle kilpailijalle saadaan vastaavasti sen hetkinen",
    13,0,"aika näppäimellä  'N', jota voidaan toistaa.",
    14,0,"Huom: S, E ja N eivät toimi korjattaessa jo hyväksyttyä aikaa.",
    15,0,"  Ruudun alakulmassa näkyy viimeinen tallennettu aika, sen jär-",
    16,0,"jestysnumero sekä ensimmäisen käyttämättömän ajan järjestysnumero",
    19,0,"TOIMINNON 'MAALI' OHJEET SIVULLA 8"
    };

scr help7 = {
    18,
    2,0,"HELP,    sivu 8      KÄYTTÖ MAALIKELLONA, TOIMINTO 'MAALI'",
    4,0,"  Aikojen tallentaminen muistiin: kts. help-sivu 7. Toiminnossa",
    5,0,"'Maali' voidaan aikoihin liittää kätevästi kilpailijan numero ja",
    6,0,"suorittaa erilaisia ajanottoon liittyviä korjaustoimia. Toimet",
    7,0,"tapahtuvat korostetulle riville, jota voidaan vaihtaa näppäi-",
    8,0,"millä \x18, \x19, F5, F9, PgUp, PgDn, Ctrl-PgUp ja Ctrl-PgDn.",
    9,0,"  Aika tai kilpailunumero voidaan muuttaa kirjoittamalla uusi",
    10,0,"tieto vanhan päälle. Aikakenttään voidaan merkitä myös keskeyttä-",
    11,0,"minen tai hylkääminen näppäimillä 'K' ja 'H' sekä poistaa tällai-",
    12,0,"nen merkintä näppäimellä '-'. Edelleen voidaan kilpailunumero",
    13,0,"siirtää edelliseltä tai seuraavalta riviltä näppäimillä F8 ja F6",
    14,0,"  Toimet tallennetaan muistiin vasta vahvistuksen jälkeen.",
    15,0,"  Maalikellotiedostoon voidaan lisätä väliin tyhjä rivi näppäi-",
    16,0,"mellä F2 ja poistaa rivi, jos kilpailunumero on 0, näppäimellä F3",
    17,0,"  Maalikellotiedot voidaan tulostaa näppäimellä F10. Tulostus",
    18,0,"tapahtuu tulosluetteloille ilmoitettulla kirjoittimella.",
    19,0,"  Näppäinyhdistelmä Alt-S tuo sijoituksen näytölle, Alt-E vaihtaa",
    20,0,"aikanäyttöä, Alt-N tuo näytölle joukkueen kaikki tiedot"
    };

scr help8 = {
    8,
    2,0,"HELP,    sivu 9      TOIMINTO 'MAALI', HAKUTOIMINNOT",
    4,0,"  Alt-R    Siirry riville, jonka järjestysnumero annetaan",
    5,0,"  Alt-H    Hae seuraava rivi, jolla annettava joukkueen numero",
    6,0,"  ALT-G    Toista haku samalle joukkueen numerolle.",
    7,0,"  Alt-0 (nolla) Hae seuraava rivi, jolla ei numeroa.",
    8,0,"  Alt-X    Hae rivi, jolla epäilyttävä tieto (merkitty '*')", 
    10,0,"Kun viimeinen rivi on saavutettu, jatkuvat edellä luetellut",
    11,0,"haut enismmäiseltä riviltä."
    };

scr paavalikko = {
   11,
    2,0,"Valitse toiminto painamalla alkukirjainta",
    3,0,"M)aali          Käyttö tosiaikaiseen ajanottoon maalissa",
    5,0,"L)askenta       Tulosten, hylkäysten ym. syöttö ja korjaukset",
    7,0,"K)orjaukset     Kilpailijoiden lisäykset, poistot ja korjaukset",
    9,0,"T)ulosluettelot Tulostus näytölle tai paperille, automaattisen",
   10,16,"tulostuksen käynnistys. 'O': oikaise valintoja",
   12,0,"A)setukset      Muuta ohjelman asetuksia, mm. yhteislähtö ",
   14,0,"lE)eimat        Leimantarkastusnäyttö ja -toiminnot",
   16,0,"B)ackup         Tiedoston KILP.DAT kopiointi levykkeelle",
   18,0,"P)oistu         Lopeta ohjelman käyttö",
   20,0,"Ohjeita saa aina ruudulle painamalla näppäintä <F1>"
   };

scr asetusvalikko = {
   8,
    2,0,"T)arrojen tulostus Keskeytä/muuta kaikki/ei-lähteneet",
    4,0,"A)jat              Kellon käynti, aikojen tarkkuus, maalikello",
    6,0,"V)aroituskynnykset Muuta osuuskohtaisia minimiaikoja",
    8,0,"L)ähdöt            Muuta lähtöaikoja. Käynnistä tai keskeytä",
    9,19,"yhteislähtöjen automaattinen kirjaus",
    11,0,"M)odemi            Lähetä sanomia modemille tiedonsiirron ",
    12,19,"käynnistämiseksi tai keskeyttämiseksi",
   14,0,"loK)i              Muuta lokin tulostusta"
   };

scr yl_ohje = {
   13,
   4,3,"Yhteislähtöjen automaattinen kirjaus soveltuu käytettäväksi",
   5,3,"jos kaikkien sarjojen ja osuuksien vaihdot suljetaan saman-",
   6,3,"aikaisesti ja kaikkien sarjojen ja osuuksien yhteislähdöt",
   7,3,"ovat yhtäaikaiset.",
   9,3,"Ohjelma kysyy ensin yhteislähdön ja vaihdon sulkemisen ajat",
   10,3,"ja käynnistää sitten vahvistuksen saatuaan yhteislähtöjen",
   11,3,"automaattisen kirjauksen.",
   13,3,"Yhteislähtömerkinnät tehdään kirjattaessa vaihdon sulkemisen",
   14,3,"jälkeisiä vaihtoaikoja - jo kirjattujen vaihtojen osalta heti",
   15,3,"vahvistuksen jälkeen. Kirjausta jatketaan kunnes automaattinen",
   16,3,"kirjaus keskeytetään tässä samassa valikossa.",
   18,3,"Ilmoitettujen sulkemis- ja lähtöaikojen muutos johtaa kaikkien",
   19,3,"yhteislähtömerkintöjen korjaamisen muutettujen aikojen mukaan."
   };

scr korjvalikko = {
   14,
   3,0,"L)isää           : Lisää kilpailijoita. Tuloksia ei voi antaa",
   4,19,"lisäystoiminnossa",
   6,0,"K)orjaa          : Korjaa kilpailijatietoja tai tuloksia",
   8,0,"P)oista          : Poista kilpailija. Poistoa ei voi tehdä, jos",
   9,19,"tulos tai hylkäysmerkintä on jo tehty.",
  10,19,"Merkintä 'Poissa' tai 'Ei lähtenyt' voidaan",
  11,19,"tehdä toiminnossa K)orjaa",
  13,0,"J)uoksujärjestys : Syötä juoksujärjestyksiä",
  14,0,".. T)iedostosta  : Lue juoksujärjestyksiä tiedostosta, joka",
  15,19,"on saatu esim. sähköpostina",
  17,0,"Kilpailija voidaan hakea käsiteltäväksi numeron tai nimen",
  18,0,"perusteella. Nimihakuun päästään syöttämällä tyhjä numerokenttä",
  19,0,"Nimihaussa voidaan antaa nimestä vain alkuosa ja täydentää",
  20,0,"selaamalla näppäimillä 'S' ja 'E'"
  };

scr tulostettava = {
  14,
   3,0,"Tulostus tapahtuu aina sarjoittain halutussa laajuudessa:",
   5,0,"I)lmoittautuneet  Kaikki täksi päiväksi ilmoittautuneet",
   6,0,"L)opettaneet      Hyväksytyt, keskeyttäneet ja hylätyt",
   7,0,"H)yväksytyt       Hyväksytyt",
   8,0,"P)arhaat          Seuraavaksi ilmoitettava määrä parhaita",
   9,0,"hY)lätyt          Hylätyt",
  10,0,"K)eskeyttäneet    Keskeyttäneet",
  11,0,"E)i lähteneet     Ei lähteneiksi merkityt",
  12,0,"A)voimet          Vailla tulosta olevat",
  13,0,"V)alitut          Seura-, piiri, ja ratatulosten laadinta",
  15,0,"Paperille tulostettaessa aloittaa uusi seura aina uuden sivun",
  16,0,"paitsi valittaessa parhaat, jolloin samalle sivulle tulostetaan",
  17,0,"niin monta sarjaa kuin mahtuu. Esimerkiksi 200 parasta tulostaa",
  18,0,"kaikki hyväksytyt säästäen paperia"
  };

scr tulvalikko = {
   10,
   5,0,"N)äytölle        Tulostus näytölle",
   7,0,"P)aperille       Tulostus kirjoittimelle tai kirjoittimen",
   8,17,"korvaavaan tiedostoon",
   10,0,"A)utomaattinen   Muuttuneiden sarjojen tulostus paperille",
   11,17,"automaattisesti, kun muutoksia on kohta annet-",
   12,17,"tava määrä. Sarjat tulostetaan kiireellisyys-",
   13,17,"järjestyksessä. Käytä valintaa 'Automaattinen'",
   14,17,"myös lopettaaksesi automaattinen tulostus",
   16,0,"M)uotoilu       Muuta tulosluettelon muotoilua",
   17,0,"T)yhjennä jono  Tyhjennä taustatulostuksen tulostusjono"
   };

scr tulvalikko2 = {
   10,
   4,0,"J)oukkueet        Tulostaa joukkueiden tulokset",
   6,0,"O)suudet          Tulostaa henkilökohtaiset osuusajat",
   8,0,"Y)hteeenveto      Sarjoittain tulosten, keskeyttäneitten,",
   9,18,"hylättyjen ja avoinna olevien lukumäärät",
  10,18,"sekä avoimille mahdolliset ajat ja sijat",
  12,0,"L)ehdistö         Laatii käyttäjän avustuksella tulosluettelon",
  13,18,"lehdistön toivomassa muodossa. Käyttäjä",
  14,18,"voi muokata tekstiä vapaasti ruutueditori-",
  15,18,"moodissa.  Mm. seurojen nimiä on syytä",
  16,18,"täydentää.",
   };

scr tulosuudet = {
   13,
   5,0,"Valittaessa yhden osuuden tulostus tulostuu kustakin jouk-",
   6,0,"kueesta kokonaisaika sekä osuuden juoksija ja osuusaika.",
   7,0,"Valinta 'Y)hteisaika' tulostaa vain yhteisajat.",
   8,0,"Valinta 'K)aikki' tulostaa sekä yhteisajat että kaikkien",
   9,0,"osuuksien juoksijat, väliajat ja osuusajat. Lukumäärät",
   10,0,"lasketaan tässä vaihtoehdossa olettaen, että avoimia",
   11,0,"tuloksia ei ole.",
   13,0,"Automaattinen tulostus tapahtuu aina osuus kerrallaan",
   14,0,"seuraten jatkuvasti kaikille osuuksille tulevia uusia",
   15,0,"tietoja.",
   17,0,"Tulostettavat sarjat ilmoitetaan antamalla sarjaluettelon",
   18,0,"järjestyksessä ensimmäinen ja viimeinen peräkkäin tulostettava",
   19,0,"sarja. Uusi sarja-alue voidaan ilmoitta heti edellisen jälkeen."
   };

scr backupohje = {
   10,
   5,0,"Toiminnolla 'Backup' luodaan levykkeelle uusi kopio kilpailun",
   6,0,"tämänhetkisestä tilanteesta joko tulospalvelun varmistamiseksi",
   7,0,"tai tilanteen siirtämiseksi toiselle tietokoneelle.",
   9,0,"Jos käytössä on kaksi levyasemaa, saadaan back-up-kopio",
   10,0,"asemassa Dr2 nyt olevalle levykkeelle, minkä jälkeen tilanne",
   11,0,"kopioidaan asemaan pantavalle uudelle levykkeelle.",
   12,0,"Jos käytössä on vain yksi levyasema, tehdään kopio asemaan B",
   13,0,"pantavalle levylle.",
   15,0,"HUOM! Jos backupin teko keskeytyy levykevirheen johdosta,",
   16,0,"valitse 'O)hita toiminto' toistuvasti ja lopuksi J)atka."
   };

scr tiedostoohje = {
   8,
   5,0,"Tiedostoon tulostus tapahtuu aina uuteen tiedostoon, joka",
   6,0,"korvaa aiemman saman nimisen tiedoston.",
   8,0,"Tiedosto suljetaan, kun palataan päävalikkoon, minkä jälkeen",
   9,0,"voidaan levyke tai muistitikku poistaa, jos tiedosto on tehty",
	10,0,"poistettavalle tallennusvälineelle",
   12,0,"Vaihtoehtoisesti voidaan tiedosto laatia kiintolevylle,",
   13,0,"mistä se voidaan kopioida käyttöjärjestelmätoiminnoilla",
   14,0,"päävalikkoon palaamisen jälkeen."
   };

