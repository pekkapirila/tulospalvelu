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

#include "HkDeclare.h"

static scr *helps[10];
static char saveline[80];

void prscr(scr *s)
{
   INT i;

   for (i=0; i<s->nl; i++)
      if (s->l[i].t) vidspmsg(s->l[i].r, s->l[i].c,7,0,(char *) s->l[i].t);
}

static scr help0 = {
    10,
    2,0,"HELP,    sivu 1",
    4,0,"Sivu 2 :   KÄSITELTÄVÄN KILPAILIJAN VALINTA",
    5,11,"SIIRTYMINEN KENTÄSTÄ TOISEEN",
    7,0,"Sivu 3 :   TIETOJEN SYÖTTÖ JA MUUTTAMINEN",
    9,0,"Sivu 4 :   TIEDONSIIRTO",
    11,0,"Sivu 5 :   KÄYTTÖ MAALIKELLONA",
	13, 0, "Sivu 6 :   KÄYTTÖ MAALIKELLONA, TOIMINTO 'MAALI'",
	15, 0, "Sivu 7 :   LÄHTÖAJAT LÄHTÖPORTILTA",
	17, 0, "Sivu 8 :   EMIT-TOIMINTO",
	19, 0, "Sivu 9 :   EMIT-TOIMINNON NÄPPÄIMET"
};

static char hr1[] = "KÄSITELTÄVÄN KILPAILIJAN VALINTA";
static char hr2[] = "Kilpailija valitaan normaalisti antamalla kilpaijanumero";
static char hr3[] = "tarkistusnumeroineen.  Tarkistusnumeron sijasta voi painaa";
static char hr4[] = "näppäintä '*', jolloin ohjelma laskee tarkistusnumeron";
static char hr5[] = "Jos kilpailijanumeroa kysyttäessä painetaan suoraan <Return>";
static char hr6[] = "siirrytään nimihakuun. Nimihaussa annetaan nimen alkuosa,";
static char hr7[] = "painetaan <Return> ja täydennetään haku selaamalla tarvit-";
static char hr8[] = "taessa näppäimillä 'S' ja 'E'. Lopuksi painetaan 'H'";
static char hr9[] = "SIIRTYMINEN KENTÄSTÄ TOISEEN tietoja korjattaessa tapahtuu";
static char hr10[] = "-  eteenpäin näppäimillä <Return>, '', <F9> ja <Tab>";
static char hr11[] = "-  taaksepäin näppäimillä '' ja <F5>";
static char hr12[] = "Ruutu hyväksytään näppäimellä '+'";
static char hr13[] = "Ruudusta poistutaan tallentamatta muutoksia näppäimellä <Esc>";

static scr help1 = {
    14,
    2,0,"HELP,    sivu 2",
    4,0,hr1,
    5,0,hr2,
    6,0,hr3,
    7,0,hr4,
    8,0,hr5,
    9,0,hr6,
    10,0,hr7,
    11,0,hr8,
    13,0,hr9,
    14,0,hr10,
    15,0,hr11,
    17,0,hr12,
    18,0,hr13
    };

static char h2r1[] = "HELP,    sivu 3     TIETOJEN SYÖTTÖ JA MUUTTAMINEN";
static char h2r2[] = "Maaliajan syöttö tapahtuu laskentatoiminnossa käyttäen vain";
static char h2r3[] = "numeronäppäimiä sekä näppäintä 'Askelpalautus', joka kumoaa";
static char h2r4[] = "edellisen painalluksen vaikutuksen. Syötetyt numerot korvaavat";
static char h2r5[] = "aiemmat viimeisestä alkaen. Kilpailijanumero syötetään samoin.";
static char h2r6[] = "Aikoja korjattaessa käytetään numeronäppäimiä, nuolia  ja \x32";
static char h2r7[] = "sekä näppäimä <Home> ja <End>. Korjaus tapahtuu korvaustoimin-";
static char h2r8[] = "tana (Replace). Nuolien sijasta voidaan käyttää";
static char h2r9[] = "näppäimiä <F7> ja <F8>";
static char h2r10[] = "Muita tietoja korjattaessa voidaan käyttää lisäys (Insert) tai";
static char h2r11[] = "korvaustoimintaa; toiminta vaihdetaan näppäimellä <Ins>";
static char h2r12[] = "Kentän loppuosan poistaa yhdistelmä <Ctrl-End> tai <Ctrl-Del>";
static char h2r13[] = "Käytettävissä on myös näppäimet , \x32 F7, F8, Del, Askelpal.";
static char h2r14[] = "Poista tulos antamalla sellainen maaliaika, että tulos on 0.";

static scr help2 = {
    14,
    2,0,h2r1,
    4,0,h2r2,
    5,0,h2r3,
    6,0,h2r4,
    7,0,h2r5,
    9,0,h2r6,
    10,0,h2r7,
    11,0,h2r8,
    12,0,h2r9,
    14,0,h2r10,
    15,0,h2r11,
    16,0,h2r12,
    17,0,h2r13,
    19,0,h2r14
    };

static scr help3 = {
    10,
    2,0,"HELP,    sivu 4      TIEDONSIIRTO",
    4,0,"Tiedonsiirron tilanne näkyy ruudulla sen ollessa käytössä.",
    6,0,"  Jos jonot kasvavat on jossain vikaa. Saapuva jono kasvaa",
    7,0,"kuitenkin, kun samaa kilpailijaa tarkastellaan pitkään.",
    10,0,"Lähtevien jonon kasvu osoittaa, että tiedonsiirto ei toimi",
    11,0,"kunnolla, vaikka yhteyshäiriöistä ei ilmoitettaisi.  Syynä",
    12,0,"voi olla, että vastaanottava PC ei ole vastaanottotilassa.",
	14, 0, "  Myös aikataulukon tiedot siirretään toiseen MAALI-ohjelmaa",
	15, 0, "käyttävään koneeseen, jos käytetään käynnistysparametria",
	16, 0, "LÄHAIKA1 tai LÄHAIKA2 molemmissa koneissa."
};

static scr help4 = {
   14,
    2,0,"HELP,    sivu 5     KÄYTTÖ MAALIKELLONA",
    4,0,"Ajanotto käynnistetään näppäimellä valitsemalla toiminto M)aali.",
	6,0,"Käynnistyksen yhteydessä ohjelma kysyy aikojen tallennustiedostoa,",
	7,0,"jota ei yleensä kannata muuttaa sekä ajanottoon käytettävää",
	8,0,"näppäintä, jota ei tämän jälkeen voi käyttää mihinkään muuhun.",
    9,0,"  Toiminnossa 'Laskenta' saadaan aikamuistin vanhin aika kil-",
    10,0,"pailijalle näppäimellä 'S'. Seuraavassa valikossa voidaan hakea",
    11,0,"seuraava tai edellinen aika näppäimillä 'S' ja 'E' (toistettavia)",
    12,0,"  Esillä olevalle kilpailijalle saadaan vastaavasti sen hetkinen",
    13,0,"aika näppäimellä  'N', jota voidaan toistaa.",
    14,0,"Huom: S, E ja N eivät toimi korjattaessa jo hyväksyttyä aikaa.",
    15,0,"  Ruudun alakulmassa näkyy viimeinen tallennettu aika, sen jär-",
    16,0,"jestysnumero sekä ensimmäisen käyttämättömän ajan järjestysnumero",
    18,0,"TOIMINNON 'MAALI' OHJEET SIVULLA 6"
    };

static scr help5 = {
    17,
    2,0,"HELP,    sivu 6      KÄYTTÖ MAALIKELLONA, TOIMINTO 'MAALI'",
    4,0,"  Käsiteltävää riviä vaihdetaan näppäimillä \x18, \x19, PgUp, PgDn,",
    5,0,"Ctl-PgUp (alkuun) ja Ctl-PgDn (loppuun) sekä Alt-R (siirry",
    6,0,"valittavalle riville).",
    7,0," Aikakentän käsittelyyn siirrytään näppäimellä Tab.",
    8,0," Alt-H on kilpailijanumerohaku (toisto Alt-G). Alt-0 hakee rivit",
    9,0,"joilla ei ole numeroa ja Alt-X epäilyttävät rivit ('*'-merkki)",
    10,0,"  Aika tai kilpailunumero voidaan muuttaa kirjoittamalla uusi",
    11,0,"tieto vanhan päälle. Aikakenttään voidaan merkitä myös keskeyttä-",
    12,0,"minen tai hylkääminen näppäimillä 'K' ja 'H' sekä poistaa tällai-",
    13,0,"nen merkintä näppäimellä '-'. Edelleen voidaan kilpailunumero",
    14,0,"siirtää edelliseltä tai seuraavalta riviltä näppäimillä F8 ja F6",
    15,0,"  Toimet tallennetaan muistiin vasta vahvistuksen jälkeen.",
    16,0,"  Maalikellotiedostoon voidaan lisätä väliin tyhjä rivi näppäi-",
    17,0,"mellä F2 ja poistaa rivi, jos kilpailunumero on 0, näppäimellä F3",
    18,0,"  Maalikellotiedot voidaan tulostaa näppäimellä F10. Tulostus",
    19,0,"tapahtuu tulosluetteloille ilmoitettulla kirjoittimella."
    };

static scr help6 = {
    14,
    2,0,"HELP,    sivu 7      LÄHTÖAJAT LÄHTÖPORTILTA",
    4,0,"Lähtöaikojen kirjaaminen automaattisesti lähtöportin ajanotosta",
    5,0,"edellyttää, että käytettävä ajanottoliittymä tai maalikellon",
    6,0,"sanomatyyppi on määritelty antamaan lähtöaikoja. Määrittely",
    7,0,"tehdään joko käynnistysparametrilla PISTEET tai valinnassa",
    8,0,"Asetukset/Ajat/Maalikello/Pisteen tunnistus.",
    10,0,"Lähtöajat tallennetaan samaan tiedostoon ja näytetään samalla",
    11,0,"näytöllä kuin muut ajat, ellei käytetä käynnistysparametria LAJAT",
    13,0,"Jos parametri LAJAT on annettu, käsitellään lähtöajat erikseen.",
    14,0,"Tällöin vaihdetaan ajanottonäyttöä käyttäen näppäinyhdistelmää",
    15,0,"Alt-A. Näppäinyhdistelmällä Alt-L saadaan pieni viimeiset lähtö-",
    16,0,"ajat sisältävä ikkuna lukumäärätietojen tilalle. Saman näppäilyn",
    17,0,"toistaminen palauttaa lukumäärät.",
    19,0,"Lähtöajan tunnus ajanottonäytöllä on 'L'."
    };

static scr help7 = {
    13,
    2,0,"HELP,    sivu 8      EMIT-TOIMINNOT",
    4,3,"Näytettävä kilpailija vaihdetaan selaamalla näppäimillä",
    5,3,"PgDn/S: Seuraava",
	 6,3,"PgUp/D: eDellinen",
    7,3,"V / A:  Viimeinen / Alkuun",
    8,3,"G:      tietueen järjnro",
	 9,3,"kilpailijanumero voidaan syöttää suoraan",
	 10,3,"Tab:    Emit-koodin valintaan",
    12,3,"Hylkäys- ja keskeyttämismerkinnät voidaan tehdä sekä leiman-",
    13,3,"tarkastus että korjausnäytöllä",
    15,3,"Raportteja ja tulosteita tehdään sekä leimantarkastusnäytön",
	 16,3,"valinnasta R että tulosluetteloissa",
    19,3,"Lisää seuraavalla sivulla"
    };

static scr help8 = {
    17,
    2,0,"HELP,    sivu 9      EMIT-TOIMINNOT - 2",
	 4,3,"X     : Vaihda korttiin liitetty kilpailija", 
	 5,3,"Z     : Etsi seuraava virheleimaus tai hyväksymätön tietue",
    6,3,"Ctrl-S: Etsi leimoja vastaava sarja.",
    7,3,"Ctrl-T: Laske tulos viimeisen rastin ja lukemishetken ajoista.",
    8,3,"Ctrl-E: Muokkaa Emit-koodeja tai väliaikoja",
	 9,3,"U     : Emit-tiedon poisto",
	 10,3,"O     : Rastin leimasinkoodien muuttaminen",
	 11,3,"Alt-K : Katsele ja muokkaa kilpailijatietoja",
	 12,3,"M     : MTR-laitteen toiminnot",
	 13,3,"N     : Seuraava tieto tiedostosta EMIT_IN.DAT",
    15,3,"Toimintoa AUTOKILP (korttia vastaavan kilpailijan automaatti-",
    16,3,"nen liittäminen aikaan) ohjataan seuraavilla näppäimillä lei-",
    17,3,"mantarkastusnäytöllä: Alt-M tuo esille ja piiloittaa aikojen",
    18,3,"kohdistusnäytön, Alt-J ja Alt-U siirtävät seuraavan ajan",
    19,3,"kohdistinta. Kohdistin voidaan siirtää myös maalitoiminnon",
    20,3,"näytöllä korostetulle riville näppäimillä Ctrl-V."
    };

void help(INT ih)
{
   INT i, x, y, h, l, tcomfl0;
   wchar_t ch;
#ifndef __linux__
   char *svscr = 0;
#else
   USHORT *svscr = 0;
#endif

   tcomfl0 = tcomfl;
   tcomfl = TRUE;
   helps[0] = &help0;
   helps[1] = &help1;
   helps[2] = &help2;
   helps[3] = &help3;
   helps[4] = &help4;
   helps[5] = &help5;
   helps[6] = &help6;
   helps[7] = &help7;
   helps[8] = &help8;
   sccurst(&y, &x, &h, &l);
   svscr = savescr(2,0,ySize-5,64);
   virdrect(ySize-3,0,ySize-3,79,saveline,0);
   do {
      for (i=2; i<ySize-4; i++) clrtxt(i,0,64);
      prscr(helps[ih]);
      ch = L' ';
      wselectopt(L"Valitse sivu 1, 2, 3, 4, 5, 6, 7, 8 tai 9,  <Esc> lopettaaksesi",
         L"123456789\x1B", &ch);
      if (ch == ESC) break;
      ih = ch - L'1';
      }
   while (1);
   restorescr(2,0,ySize-5,64,svscr);
   viwrrect(ySize-3,0,ySize-3,79,saveline,7,0,0);
   sccurset(y,x);
   tcomfl = tcomfl0;
}

scr paavalikko = {
#ifdef MAKI
   13,
    2,0,"Valitse toiminto painamalla tunnuskirjainta",
    3,0,"M)aali          Ajanottotoiminnot, lähtö, maali ja väliajat",
    5,0,"K)orjaa         Kilpailijoiden lisäykset, poistot ja korjaukset",
	 6,16,               "hylkäysten ja poissaolojen syöttö",
	 8,0,"S)elostaja      Sarjan tilanteen seuranta valittavassa pisteessä",
   10,0,"T)ulos          Tulostus näytölle tai paperille, automaattisen",
   11,16,"tulostuksen käynnistys. 'O': Oikaise kysymyksiä",
	13,0,"taU)lu          Tulostaulun ohjaus",
   15,0,"A)setukset      Muuta ohjelman erilaisia asetuksia",
	16,0,"Y)hteys         Tiedonsiirtoyhteyksien seuranta ja hallinta",
   17,0,"B)ackup         Tiedoston KILP.DAT kopiointi levykkeelle",
   18,0,"P)oistu         Lopeta ohjelman käyttö",
   20,0,"Ohjeita saa aina ruudulle painamalla näppäintä <F1>"
#else
   12,
    2,0,"Valitse toiminto painamalla tunnuskirjainta",
    3,0,"M)aali          Käyttö tosiaikaiseen ajanottoon maalissa",
    5,0,"L)askenta       Tulosten, käsinsyöttö esim. maalipöytäkirjoista",
    7,0,"K)orjaukset     Kilpailijoiden lisäykset, poistot ja korjaukset",
    9,0,"T)ulosluettelot Tulostus näytölle tai paperille, automaattisen",
   10,16,"tulostuksen käynnistys. 'O': Oikaise kysymyksiä",
   12,0,"lE)imat         EMIT-leimauskorttien tietojen käsittely,",
   14,0,"Y)hteys         Tiedonsiirron seuranta ja hallinta,",
   16,0,"A)setukset      Muuta ohjelman erilaisia asetuksia,",
   17,0,"B)ackup         Tiedoston KILP.DAT kopiointi levykkeelle,",
   18,0,"P)oistu         Lopeta ohjelman käyttö",
   20,0,"Ohjeita saa aina ruudulle painamalla näppäintä <F1>"
#endif
};

scr asetusvalikko = {
	9,
    2,0,"S)arjat            Sarjakohtaiset muutokset, sprintin siirrot",
    4,0,"M)aaliajat         Vaihda eri maalien ajat, jotta avoinna",
    5,20, "olevat tulokset arvioitaisiin oikein",
    7,0,"A)jat              Kellon käynti, kellonajan lähettäminen,",
    8,20, "esitystarkkuus, väliaikojen syöttötapa,",
    9,20, "väliaikapisteet, maaliaikojen näppäily,",
   10,20, "maalikellon pisteen tunnistus ja asetukset",
   12, 0,"W                 Vaiheen vaihto ohjelman ollessa käynnissä",
   14,0,"E)mit              Emittietojen tulostusasetukset"
   };

scr modemohje = {
   6,
   15,0,"    Modemiyhteys voidaan käynnistää, ellei sitä jo ole, lähettä-",
   16,0,"    mällä toiselle modemille viesti 'ata' ja toiselle 'ath1o'.",
   17,0,"    Modemin sanomakaiutus, joka saa ohjelman virheellisesti",
   18,0,"    uskomaan että yhteys toimii, lakkaa sanomalla 'ate0'.",
   19,0,"    Toimivalta linjalta siirrytään komentomoodiin käskyllä K).",
   20,0,"    Puhelu katkaistaan sitten komennolla 'ath0'"
   };

scr korjvalikko = {
   11,
   5,0,"L)isää    : Lisää kilpailijoita. Tuloksia ei voi antaa",
   6,12,"lisäystoiminnossa",
   8,0,"K)orjaa   : Korjaa kilpailijatietoja tai tuloksia",
  10,0,"P)oista   : Poista kilpailija. Poistoa ei voi tehdä, jos",
  11,12,"tulos tai hylkäysmerkintä on jo tehty.",
  12,12,"Merkintä 'Poissa' tai 'Ei lähtenyt' voidaan",
  13,12,"tehdä toiminnossa K)orjaa",
  15,0,"Kilpailija voidaan hakea käsiteltäväksi numeron tai nimen",
  16,0,"perusteella. Nimihakuun päästään syöttämällä tyhjä numerokenttä",
  17,0,"Nimihaussa voidaan antaa nimestä vain alkuosa ja täydentää",
  18,0,"selaamalla näppäimillä 'S' ja 'E'"
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

scr tulostettava = {
  14,
   3,0,"Tulostus tapahtuu aina sarjoittain halutussa laajuudessa:",
   5,0,"I)lmoittautuneet  Kaikki täksi päiväksi ilmoittautuneet",
   6,0,"L)opettaneet      Hyväksytyt, keskeyttäneet ja hylätyt",
   7,0,"T)ulokset         Hyväksytyt",
   8,0,"P)arhaat          Seuraavaksi ilmoitettava määrä parhaita",
   9,0,"H)ylätyt          Hylätyt",
  10,0,"K)eskeyttäneet    Keskeyttäneet",
  11,0,"E)i lähtennet     Ei lähteneet",
  12,0,"A)voimet          Avoinna olevat kilpailijat",
  13,0,"S)eura (piiri)    Seura- ja piiritulosten laadinta",
  15,0,"Ellei muotoilussa ole toisin pyydetty, aloittaa paperille",
  16,0,"tulostettaessa uusi sarja aina uuden sivun paitsi valittaessa",
  17,0,"parhaat, jolloin samalle sivulle tulostetaan niin monta",
  18,0,"sarjaa kuin mahtuu."
  };

scr tulvalikko = {
   12,
   4,0,"N)äytölle        Tulostus näytölle",
   6,0,"P)aperille       Tulostus kirjoittimelle tai kirjoittimen",
   7,17,"korvaavaan tiedostoon",
   9,0,"A)utomaattinen   Muuttuneiden sarjojen tulostus paperille",
   10,17,"automaattisesti, kun muutoksia on kohta annet-",
   11,17,"tava määrä. Sarjat tulostetaan kiireellisyys-",
   12,17,"järjestyksessä. Käytä valintaa 'Automaattinen'",
   13,17,"myös lopettaaksesi automaattinen tulostus",
   15,0,"M)uotoilu        Muuta kirjoittimelle tulostettavien tulos-",
   16,17,"luetteloiden muotoilua",
   18,0,"T)yhjennä jono   Tyhjennä taustatulostuksen tulostusjono",
	20,0,"tI)edostoon      Tulokset, teksti-, XML- yms. tiedostoihin"
   };

scr pistetunnistus = {
   12,
   5,0,"Otetun ajan käyttökohde voidaan tunnistaa, joko oletusvalinnan,",
   6,0,"ajanottotavan, maalikellon liitännän tai kilpailijan saaman",
   7,0,"ajan perusteella. Jos 'Oletusvalinnalle' annetaan muu arvo kuin",
   8,0,"'A)utomaattinen', käytetään tässä tietokoneessa tätä oletusta.",
   9,0,"Muussa tapauksessa ratkaisee ajanottotapa, jos vastaava valinta",
   10,0,"ei ole 'A)utomaattinen'. Jos sekä oletusvalinta, että käytetun",
   11,0,"ajanototavan valinnat ovat 'A)utomaattinen', valitaan käyttc-",
   12,0,"kohde ajan perusteella. Käyttökohteiden koodit ovat:",
   14,4,"A)utomaattinen  : valinta muilla perusteilla",
   15,4,"L)ähtö          : lähtöaika",
   16,4,"M)aali          : maaliintuloaika",
   17,4,"1, 2, 3 tai 4   : väliaikapisteen järjestysnumero"
   };

scr tulvalikko2 = {
   9,
   4,0,"K)ilpailijat      Tulostaa kilpailijaittaiset tulokset",
   6,0,"Y)hteenveto       Sarjoittain tulosten, keskeyttäneitten,",
   7,18,"hylättyjen ja avoinna olevien lukumäärät",
   8,18,"sekä avoimille mahdolliset ajat ja sijat",
  10,0,"V)äliajat         Väliajat sarjoittain näytölle tai paperille.",
  12,0,"M)uut             Aakkos- ja numerojärjestys, lähtöajat,",
  13,18,"suppeat tulokset lehdistölle käyttäjän muok-",
  14,18,"kaamina. Huom. Laajemmat tiedot lehdistölle",
  15,18,"tehdään tiedostotulostusvalinnassa.",
   };

scr backupohje = {
   10,
   5,0,"Toiminnolla 'Backup' luodaan levylle uusi kopio kilpailun",
   6,0,"tämänhetkisestä tilanteesta joko tulospalvelun varmistamiseksi",
   7,0,"tai tilanteen siirtämiseksi toiselle tietokoneelle.",
   9,0,"Ohjelman kysyessä ilmoitetaan luotavan tiedoston nimi, joka",
   10,0,"voi sisältää polkumäärittelyn levyasematunnuksineen tai ilman.",
   11,0,"Kun back-uptiedosto on luotu, voidaan se siirtää tai kopioida.",
	12,0,"Polku voi sisältää myös verkkolevyn tunnuksen esim. muodossa:",
	14,0,"\\\\192.168.1.10\\c\\kisa\\kilp.dat",
	16,0,"mikä viittaa verkon koneen 192.168.1.10 nimellä C jaetun",
	17,0,"levyaseman tiedostoon \\kisa\\kilp.dat"
   };
scr valiaikaohje = {
   7,
   14,0,"VÄLIAIKAPISTEET",
   15,0,"Kaikista väliaikapisteistä on ilmoitettava sijainti sekä aika,",
   16,0,"jota nopeammin kukaan ei varmasti ehdi kyseiseen pisteeseen.",
   17,0,"Ohjelma päättelee tämän ajan perusteella, mikä väliaikapiste",
   18,0,"on kyseessä, joten ajan on toisaalta ylitettävä edellisen",
   19,0,"pisteen huonoin aika.",
   20,0,"Väliaikapisteiden määrää voi lisätä vain ohjelmalla ILMOITT."
   };
scr laskevaliaikaohje = {
   7,
   4,0,"VÄLIAIKOJEN LASKENTA",
   6,0,"Valittavaan väliaikapisteeseen voidaan tallentaa kaikkien,",
   7,0,"puuttuvien aikojen paikalle vakioaika sekunteina..",
   8,0,"Erotuksena kahdesta väliajasta tai loppuajasta ja väliajasta",
   9,0,"voidaan laskea ajat väliaikapisteeseen.",
   10,0,"Molemmat toimet voidaan tehdä kaikille tai osalle sarjoista.",
   11,0,"Valitun pisteen aimmat tulokset muuttuvat peruuttamattomasti."
   };
