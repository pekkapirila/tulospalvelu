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

#ifndef LASKENT_H_DEFINED
#define LASKENT_H_DEFINED
#include <stdio.h>
#include <time.h>
#include "HkDef.h"
#include <xmlparser.h>
#ifdef _CONSOLE
#include "HkConsoleDef.h"
#else
#include "WinTp.h"
#include "HkWinDef.h"
#endif

void kirjloki(char *msg);
void wkirjloki(wchar_t *msg);
void lopetus(void);

// Ohjelman rakenteen funktioita
void tausta(INT spoolfl);
void asetukset(void);
void autoalku(wchar_t *koodit);
int aloitus(int argc, wchar_t* argv[], wchar_t *cfgflname);
void korjaa(INT d, kilptietue *kilp, INT tausta, kilptietue *entkilp);
void valikko(void);
void taustaThr(LPVOID lpCn);
void lue_regnlyThread(LPVOID lpCn);
void pros_emitThread(LPVOID lpCn);
INT add(void *em);
int powi(int i, int j);
wchar_t *Build(void);
wchar_t *VersioNo(void);
int VersioInt(void);
void setBrowserEmulation(void);
void getWinVersion(void);

// Kilpailijatiedon funktioita

// Tietokantafunktioita
int JarjSeur(int yhd, int piste, int dKilp);
void SetJarjSeur(int yhd, int piste, int dKilp, int jp);
INT teejarj(bool uusinta);
int uusijarj(void);
void addjarjarr(kilptietue *kilp, int d, INT piste, int *keskeyta);
void remjarjarr(kilptietue *kilp, int d, INT piste, int *keskeyta);
INT getpos(INT kilpno, bool hyvseur = false);
int setpos(int kilpno, int d);
int nextpos(void);
int prevpos(void);
INT intsija(UINT32 *i, INT *p, INT piste, INT yhd, int *keskeyta);
int entsija(INT i, INT *p, INT piste, INT yhd, int *keskeyta);
void haesija(kilptietue *kilp, INT piste, INT yhd, int *keskeyta);
void deltall(INT d, INT kielto);
void delfrec(INT d);
void deleterec(int d);
int kirjKilpDat(wchar_t *fname);
void kirjKilpSQL(int d);
INT n_avoin(INT srj);
int haeseurvapaa(int seurvapaa);
int haeSeurVapaaKno(int ipv, int alku);
void copydfile(datafile *datfb, INT oldfile);
int seuraava(int yhd, int piste, int p);
int SeuraavaSrj(int srj, int piste, int p);
int askellasarja(int srj, int piste, int p, int *lj);
char *keyaak(void *kilp, char *key, int keylen, int flags);
char *keyjrj(void *kilp, char *key, int keylen, int flags);
int teeindeksi(kilpindex *ind, bool uusi);
void poistaindeksi(kilpindex *ind);
void kirjEventXml(wchar_t *outfname, bool kirjSarjat, bool kirjRadat, bool kirjOsanottajat);
int lueEventXml(wchar_t *filename, bool lueSarjat, bool lueRadat, bool lueOsanottajat);
int kirjXmlOo(TextFl *outfl, kilptietue &kilp, int dKilp, int piste, int level);
void luo_posarr(void);
INT16 i16swab(char *s);
int etsilisno(int lisno, int idno);
int haeSeurVakantti(int alku);
void lisaavakantit(int vakanttiluku);
void pohjustalisavaiheet(int ed_n_pv_akt);
void talljarj(void);
void kirjvaihevalinta(void);
int vaiheLkm(int sarja);
void alustaSarjatiedot(void);

int Pisteet(pistekaavatp *kva, int sj, int n, int tls, int karki);
int PisteAika(pistekaavatp *kva, int pst);
void LaskeKilpYhtPisteet(kilptietue *kilp, pistekaavatp *kva, int srj);
void LaskeYhtPisteet(pistekaavatp *kva, int srj);
void LaskeVaiheSarja(pistekaavatp *kv, int srj);
void LaskeKaikkiPisteet(void);
int teeYPisteJarj(int srj, int (**ypJrj)[4]);
int findYp(int (*ypJrj)[4], int kilpno);
int PisteKarki(int srj, int i_pv);
void PisteKarjet(void);

void teeBackupkopio(wchar_t *filename);

// Tiedonsiirron funktioita
int initcom(int cn);
void opencom(int cn, int baud, char parity, int bits, int sbits);
void closec(int cn);
void closecom(void);
void modemctrl(void);
int yhtjono(int cn, int ino);
void lueyhteysjonot(void);
void laheta(INT dkilp, INT entno, INT piste, int hyv_muutos, INT tarcomfl, INT kielto,
   INT pakota_lah);
void lahetakello(int cn, INT kielto);
int openport_x(int cn, int portti, int baud, char parity, int bits, int sbits, int xonoff);
int quesize_x(int cn, int *que);
void outquesize_x(int cn, int *que);
int lahettamatta(int cn);
void o_flush_x(int cn);
void i_flush_x(int cn);
int read_ch_x(int cn, char *ch, int *que);
int wrt_ch_x(int cn, char ch);
int read_st_x(int cn, int len, char *buf, int *nread, int *que);
int wrt_st_x(int cn, int len, char *buf, int *nsent);
int closeport_x(int cn);
int wrt_st_x_srv(int cn, int len, char *buf, int *nsent);
int wrt_st_x_srv(int cn, int len, char *buf, int *nsent);
int vaihdavaihe(int vaihe, int kielto);
int pyyda_vaihdavaihe(int vaihe, int kielto);

void opencomRS(int cn);
int autofile(int);
void startaf(INT kysy);
void tv_asetus(void);

//DWORD WINAPI tarkcom(LPVOID lpCn);
//DWORD WINAPI tiedonsiirto(LPVOID lpCn);
//DWORD WINAPI taustaThr(LPVOID lpCn);
//DWORD WINAPI lue_regnlyThread(LPVOID lpCn);
//DWORD WINAPI pros_emitThread(LPVOID lpCn);
void tarkcom(LPVOID lpCn);

int xmlsanoma(char *buf, int maxlen, kilptietue& kilp, int dkilp, int piste, int msgno);
unsigned short chksum(char *bufptr, int len);
int addseur(int cn, const combufrec * const cbuf);
void tiedonsiirto(LPVOID lpCn);
void tiedonsiirtoUDP(LPVOID lpCn);
void tiedonsiirtoTCP(LPVOID lpCn);
void lahetaXML_TCP(combufrec *obuf, int cn);
void merk_uusinta(int cna, int cny, long ta, long ty, int autouusinta);
void lahetanumerotTCP(int cn, INT32 alku, INT32 loppu);
void uusintaTCP(int cn);
void yhteysasetukset(void);
void uusintalahetys(void);
void yhteydet(void);
extern int httphaku(wchar_t *host, int port, wchar_t *page, int secure, char *buf, int buflen, int *haettu);
void lahetasulku(char *tn, INT kielto);
void lahetaeralahto(int era, INT kielto);
void lahetaMonitorille(int yhtlkm, int yhtavattu, int jonossa, int jonoja);
int lah_tiedosto(wchar_t *tiednimi, int kielto, int flags);
void tark_tiedosto(INT cn);

// Käyttöliittymän funktioita
int parametrivirhe(wchar_t *sub, int intparam, wchar_t *strparam, wchar_t charparam);
void Wselectopt(WCHAR *msg, WCHAR *opts, WCHAR *sel);
int writeerrorkysy(WCHAR *msg);
void writeerror_w(wchar_t *txt, int wait, bool vainok);
void lukumaarat(void);
void kehys(INT kl);
void clearframe63(void);
void help(INT ih);
char *savescr(int r0, int c0, int r1, int c1);
void restorescr(int r0, int c0, int r1, int c1, char *buf);
void initscreens(void);
void outform(void);
void prscr(scr *s);
void autoruutu(void);
void chgchar(char *buf, int len);
void DSPQ(int r, int cn);
void WRITEYHT(wchar_t *st, int cn);
#ifndef _CONSOLE
void sccurset(int y, int x);
void clrtxt(int row, int col1, int col2);
void clrln(int row);
// int vidspwmsg(int row, int col, int fg, int bg, wchar_t *txt);
// int vidspmsg(int row, int col, int fg, int bg, char *txt);
// int viwrrect(int r1, int c1, int r2, int c2, const char *txt, int fg, int bg, int opt);
// int viwrrectw(int r1, int c1, int r2, int c2, const wchar_t *txt, int fg, int bg, int opt);
#endif
int wcstomaksu(wchar_t *st);
void scpclr(void);
void close_err_file(void);
void kirj_err_file(wchar_t *msg, int nayta);
wchar_t *wcstokch(wchar_t *st, wchar_t erotin);
INT32 wstrtoaika_vap(wchar_t *str, int t0);
void addtapahtuma(kilptietue *kilp, int piste);
wchar_t XMLhaetunnus(wchar_t *Nimi, wchar_t *Koodit, wchar_t **Nimet, int N);
wchar_t *XMLhaeteksti(wchar_t Koodi, wchar_t *Koodit, wchar_t **Nimet);
int XMLhae_tagid(wchar_t *Tag, tagListTp *Tags, UINT nTags);
wchar_t *XMLhae_tagName(int tagid, tagListTp *Tags, UINT nTags);
void XMLhaenodeid(xml_node *node, int nnode, tagListTp *Tags, UINT nTags);

//  Ajantoton funktioita
__int64 datetime64(aikatp *tm);
INT32 normkello_a(INT32 t);
INT32 normtulos_a(INT32 t);
int pyoristatls(int tls, int pyorista, int i_pv = -1);
void remtimer(void);
void inittimer(void);
INT32 purajak(INT32 tjak);
int tm_date(int t);
int tm_copydate(int ut, int vt, int vdate);
wchar_t *time_t_date(time_t t, wchar_t *dstr);
time_t date_time_t(wchar_t *st);
// int haejono(INT32 jono_t);
int Nyt(void);
aikatp *gettm(INT rvno, aikatp *raika, INT ino);
aikatp *puttm(INT rvno, aikatp *raika, INT ino);
//void poistarivi(int rtm, INT ino);
//void lisaarivi(int rtm, aikatp *tm, INT ino);
void writeaika(INT ino, INT p);
void writeajat(void);
void suljeajat(INT s_ino, bool delfl);
void AjatTiedostoon(wchar_t *fname, int akttm);
void AjatTiedostosta(wchar_t *fname, int akttm, bool vainajat);
wchar_t *str_piste(int piste);
INT32 maaliaika(INT32 tmaali, INT t0, INT maikay, INT maikax, wchar_t *kh);
void aikavert(void);
int  tall_rivi(INT ino, aikatp *utm, aikatp *vtm,
	INT *uptr, INT kysy, INT kielto, INT pakota_lah, bool inAjatCriticalSection);
void asetavertajat(wchar_t *ln);
int  tall_kno(INT ino, INT rtm, INT kno);
int  tall_aika(INT ino, INT wtm, INT32 t);
int  lisaa_aika(INT ino, INT32 t, int piste, int ed_t, int ed_date);
int  poista_aika(INT ino, aikatp *rtm);
void laheta_aika(aikatp *dtm, aikatp *itm, INT kielto, INT pakota_lah);
void ajanotto(void);
void aseta_s4(void);
void SiritSync(bool set_time);
INT maaraa_piste(kilptietue *kilp, INT32 t);
INT va_piste(INT piste);
void lueemitkoodit(void);
void pistetunn(void);
void laheta_em(emittp *em, int rajaus, INT kielto);
int tee_emva(emitvatp *emva, emittp *em);
void kirjemvaloki(combufrec *ibuf);
INT addem(emittp *em, INT kielto);
int getEmitJarr(int kilpno, INT16 *toinen);
void setEmitJarr(int empos, int kilpno, int toinen);
INT bdg2kno(INT32 badge);
INT addbadge(INT32 badge, INT kno, INT kysyfl);
INT rembadge(INT32 badge, INT kno, INT kysyfl);
void paivita_aikanaytto(void);
void lueeratiedot(wchar_t *fname);
INT32 meriytulos(kilptietue *kilp, int pv);
int tunn_hae_kno(INT32 t);

INT getem(emittp *em, INT kilpno, INT16 toinen);
INT putem(emittp *em, INT32 ep, int inCrSc);
INT tark_badge(emittp *em);
INT lue_leimat(INT r);
INT lue_radat(INT r);
INT lue_radat_xml(INT r);
INT lueemitfile(void);
void close_sakcom(void);
void openlukijaUDP(int cn);
int openlukijaTCP(int cn);
void lueRatatiedotUudelleen(void);
int haeradansarja(wchar_t *tunnus, int *nsarja);
ratatp *haerata_sarja(int srj, int i_pv);
int haeSeurVapaaBadge(int entbadge);

INT32 e_maaliaika(emittp *em, kilptietue *kilp, INT32 *tlahto);
void clrleimat(void);
wchar_t outleimat(kilptietue *kilp, emittp *em, INT selaus, int loppuosa);
INT naytatiedot(emittp *em, kilptietue *kilp, int loppuosa);
INT lueem(emittp *em, INT ep);
void vaihda_badge(UINT32 badge);
void emit_tallenna(kilptietue *kilp, wchar_t trk, wchar_t *selitys, INT cn);
INT haelukija(emittp *em);
int em_HaeViimeinen(void);
int em_HaeEnsimmainen(void);
int em_HaeSeuraava(int eptr);
int em_HaeEdellinen(int eptr);
void lueseuraava(INT *kilpno, emittp *em);
void lueedellinen(INT *kilpno, emittp *em);
void poista_emit(emittp *em);
INT leimat(void);
void emitlista(tulostusparamtp *tulprm);
void emit_rap(tulostusparamtp *tulprm, INT kilpno);
void tark_rap(tulostusparamtp *tulprm, INT kilpno);
void tarkyhteenveto(wchar_t co);
wchar_t tarktulostettavat(void);
void tarkraportit(tulostusparamtp *tulprm, wchar_t trk);
void kirjvaliajat(wchar_t *flnm);
void emitvaliajat(INT *srj, tulostusparamtp *tulprm);
//void emitvaliajat(INT *srj, wchar_t co, wchar_t cr, INT nparas, int moniriviva);
INT etsiesarja(emittp *em);
INT etsierata(emittp *em);
ratatp *haerata(kilptietue *kilp);
int haeratano(kilptietue *kilp);
INT maalirasti(ratatp *rt, INT koodi);
void aikaruutu(void);
void seur_mtr(void);
void aseta_mtr(void);
INT tarkista(emittp *em, INT *tulkinta, INT haku, kilptietue *kilp1);
INT32 haeennakko(emittp *em, kilptietue *kilp);
INT pros_emit(kilptietue *kilp0, wchar_t *msgtxt);
void lue_lainat(int poista, int varoita);
bool on_lainakortti(int badge);
INT lainakortti(kilptietue *kilp);
int bSrchInt(int val, int *arr, int n);
void addemitanal(emittp *em, int uusi);
int laskeemitvaliajat(emittp *em, kilptietue *kilp, emitvaanaltp **eva, rastivaanaltp **rstva, int *valuku);
void laskevertajat(void);
INT haebdg(INT32 badge);
void KirjaaEmit(int badge, int srj);
void KirjaaEmitKilp(int badge, int srj);
void lueSeuraavaEmit(void);
void vapautaemitmuisti(bool vapautarata, bool vapautaleimat, bool vapautalainaemit, bool vapautaanal);
int onrata(wchar_t *tunnus);
void haeEmiterot(void);
void emitLuettu(int badge);
void tulostaRastiVa(tulostusparamtp *tulprm, int kilpno);
void uusiemitanalyysi(void);
int haekoordix(wchar_t *tunnus);
void haerastit(int ecode, int *rst, int *nr);
void laskeEm_VA(int d, bool sail);

void add_sakko(int d);

int xmlemitvasanoma(char *buf, int maxlen, emitvatp emva, int msgno);
void importsifile(void);
void si_tcpimport(void);
void takaa_ajolahdot(void);
void luelahdepisteet(void);
void tall_maki(wchar_t *sanoma);
void naytakilpailija(kilptietue *kilp);
void siirra_taulu(INT t_no);
void teevuorotaulu(int sarja, int uudet);
INT32 pituuspisteet(kilptietue *kilp, INT kierros);
INT32 tuomaripisteet(kilptietue *kilp, INT kierros, INT tuomari);
INT32 tyylipisteet(kilptietue *kilp, INT kierros);
INT32 hyppypisteet(kilptietue *kilp, INT kierros);
INT32 yhtpisteet(kilptietue *kilp, INT kierros);
INT32 makitulos(kilptietue *kilp, int piste, wchar_t *st);
wchar_t *format1(INT32 arvo, wchar_t *buf, int len, wchar_t decsep);
void outmakiform(void);
void outmakikilp(kilptietue *kilp, INT ntlisa);
void aktivointi(int kno, int kierros, int kielto);
void lahetaaktivointi(int kno, int kierros, int kielto);
void vaihdakierros(int kierros, int kielto);
void poistakierrostulokset(int sarja, int kierros);
void nollaamakikierrokset(kilptietue *kilp);
void lahetakierros(int kierros, int kielto);
INT32 makiaika(INT32 pisteet, int sallineg);
int seurvuoro(int lisa);

// Tulostusfunktioita
wchar_t *trimstrlen(wchar_t *str, int len);
void sijat(INT srj);
void sijat2(INT srj, bool tall, bool yhdarr);
wchar_t *pitka_nimi(wchar_t *seura);
void putfld(tulostusparamtp *tulprm, wchar_t *str, INT pos, INT len, INT oik, INT r);
void putfld_int(tulostusparamtp *tulprm, int k, INT pos, INT len, INT oik, INT r);
void newline(tulostusparamtp *tulprm, INT n);
void endpage(tulostusparamtp *tulprm);
void enddoc(tulostusparamtp *tulprm);
void abortdoc(tulostusparamtp *tulprm);
void initline(tulostusparamtp *tulprm);
void endline(tulostusparamtp *tulprm, int n);
void bold_on(tulostusparamtp *tulprm);
void bold_pois(tulostusparamtp *tulprm);
void paaots_on(tulostusparamtp *tulprm);
void paaots_pois(tulostusparamtp *tulprm);
void aliots_on(tulostusparamtp *tulprm);
void aliots_pois(tulostusparamtp *tulprm);
void kirjLisaRivit(tulostusparamtp *tulprm, wchar_t *LisaRivit);
//void writehtml(wchar_t *rivi);
void printbufhp(PRFILE *tarf);
void writeline(wchar_t *rivi);
//void puts_f(wchar_t *line);
void init_ps(tulostusparamtp *tulprm);
void initgdiprinter(tulostusparamtp *tulprm);
wchar_t *insdes(wchar_t *st, int des, wchar_t dessign);
wchar_t *otsTeksti(wchar_t *out, wchar_t *in, int len, int i_pv);

int initpage(tulostusparamtp *tulprm, const wchar_t *fnt);
void writeline_x(wchar_t *line);
void autotulostus(void);
void merkisto(wchar_t merkit);
int list(wchar_t kohde, wchar_t tiedlaji, wchar_t tulostettava, wchar_t jarjestys,
	wchar_t pisteval, wchar_t rajaus, int alaraja, int ylaraja, INT16 *sarjat,
	wchar_t *kohdenimi, wchar_t sralaji, wchar_t merkisto, wchar_t erotin,
	wchar_t *PisteLista, wchar_t (*seuralista)[LSEURA+5], wchar_t *SukupRaj,
	INT16 *IkaSrj, wchar_t *Lisarivit, UINT32 options, UINT32 options2);
void lue_muotoilu(wchar_t *luetfnm, int kysy);
void lue_ll_muotoilu(wchar_t *luetfnm, INT alku);
void kirj_ll_muotoilu(wchar_t *luetfnm);
void tall_muotoilu(wchar_t *luetfnm);
void ssltulos(kilptietue *kilp, tulostusparamtp *tulprm);
void shltulos(kilptietue *kilp, int sarja, tulostusparamtp *tulprm);
void liittotulos(kilptietue *kilp, int srj, tulostusparamtp *tulprm);
int tulostasarja(INT *srj, tulostusparamtp *tulostusparam, int *l, int *sv, int autotl);
bool mahtuusivulle(int l, int vm, tulostusparamtp *tulprm);
wchar_t *MaaNimi(wchar_t *maa);
wchar_t *IOFStatus(kilptietue *kilp);
wchar_t *ISOtime(wchar_t *st, wchar_t *datestr, int time);
wchar_t *sekTulos(wchar_t *st, int t, int pyor);

wchar_t *aikatowstr_cols_n(wchar_t *as, INT32 tl, int t0, wchar_t sep, int len);
wchar_t *eroaikatowstr(wchar_t *as, int ero, int len);
void sivuTuloste(kilptietue *kilp);
void tulostaParhaat(int srj, int lkm);

#ifdef DBGFILE
void opendbgfile(void);
void closedbgfile(void);
#endif


int luekentat(int mista);

//  Sarjafunktioita

int luesarjat(void);
void kirjsarjat(void);
int haesarja_w(WCHAR *snimi, bool salliyhd);
INT luesarja(wchar_t *snimi, wchar_t *tc, bool salliyhd = false);
void luesarjayhdistelmat(void);
void vaihdakilpsrj(int ala, int yla, int muutos, bool indeksoi);
wchar_t *StrAlisarja(wchar_t *str, kilptietue *kilp);
void asetaSrjLahto(int srj, int aika, bool siirto, bool korjaatulos, bool aina_arvottu);
void asetaEraLahto(int ryh, int aika, bool korjaatulos, bool aina_arvottu);

//  Seurafunktioita
void kirj_seurat(void);
INT tall_seura(wchar_t *snimi, wchar_t *lyhenne, wchar_t *maa, int piiri, int kielto);
void lah_seura(wchar_t *snimi, wchar_t *lyhenne, wchar_t *maa, int piiri, int kielto);
int HaeSeuraLista(int sralaji, bool maafl, int Vaihe, wchar_t (*sra)[LSEURA+5], int *nsra, int maxnsra);

void add_gaztapahtuma(kilptietue *kilp);

void lueKorRajat(void);


extern	 CRITICAL_SECTION idindex_CriticalSection;
extern   CRITICAL_SECTION tall_CriticalSection;
extern	 CRITICAL_SECTION print_CriticalSection;
extern	 CRITICAL_SECTION autotul_CriticalSection;
extern   CRITICAL_SECTION loki_CriticalSection;
extern   CRITICAL_SECTION inb_CriticalSection;
extern   CRITICAL_SECTION outb_CriticalSection;
extern   CRITICAL_SECTION kilpdat_CriticalSection;
extern   CRITICAL_SECTION ajat_CriticalSection;
extern   CRITICAL_SECTION emit_CriticalSection;
extern   CRITICAL_SECTION rastiva_CriticalSection;
extern   CRITICAL_SECTION tcpip_CriticalSection;
extern   CRITICAL_SECTION tcp_CriticalSection[MAX_PORTTI];
extern	 CRITICAL_SECTION seura_CriticalSection;
extern	CRITICAL_SECTION seur_badge_CriticalSection;
extern	HTHREAD hTarkcomThread, hTsThread[MAX_PORTTI];
extern	HTHREAD hLuepakThread[MAX_PORTTI];
extern	HTHREAD taustaThread, hLkmThread;
extern	HTHREAD hAikacomThread;
extern   wchar_t  inbfilename[];

extern	envdatatp EnvData;
extern  bool poistaSulje;
extern	int inLopetus;
extern	int katkaisuPyydetty;
extern	wchar_t exeName[50];
extern	wchar_t WorkingDir[200];
extern	bool TestiToiminnot;
extern   int taustaon, taustasulj, siimport, nakviive, inGDIdoc;
extern  INT intv[];
extern	wchar_t sulkusalasana[12];
extern   IV_KEY keytab[];
extern  INT16 ySize, xSize;
extern  int writeerrorOn;
extern struct {
	wchar_t afname[202];
	int autokomentolaji;
	wchar_t autokomento[202];
	wchar_t jakelumaar[202];
	bool vainMuutt;
	int afvali;
	bool af_kaikki;
	bool af_flag;
	wchar_t af_xmlva;
	wchar_t aflstnm[202];
	wchar_t sanafname[202];
	wchar_t sanafchars;
	int kopiofl;
	wchar_t wtitlea[202];
	wchar_t wheadera[202];
	} autofileparam;
extern  HANDLE hStdIn, hStdOut, hConIn, hConOut;
extern   int UDPviive_lue, UDPviive_lah, UDPviive_ts, UDPCliWait;
#ifdef EMITLEIMAT
extern	int maxemvalah;
extern  int pros_emit_on;
extern	bool emitluenta;
extern	int emitTunnistus;
extern  HTHREAD hProsEmitThread;
extern  int sjbox;
#endif
/*
extern GDIfontTp CurrFnt;
extern GDIfontTp DefFnt;
extern GDIfontTp TitleFnt;
extern GDIfontTp HeaderFnt;
extern GDIfontTp CourFnt;
*/
extern   virhesantp virhesanoma;
extern	 bool hav_virheet;
extern   bool suljesuoraan;
extern   bool vaiheenvaihto;
extern   INT  max_lahportti, max_portti;
extern   wchar_t ch_piste[];
extern   wchar_t aika_jono[NREGNLY][16];
extern   INT maxjono;
extern   wchar_t jonopiste[MAXJONO];
extern   INT32 tl_ylaraja;
extern   INT tunn_srj;
extern   INT tunn_raja;
extern   INT kierrosva[];
extern   INT kierrosvafl;
extern   INT va_vuoro;
extern	INT vavuoro_ala;
extern	INT vavuoro_yla;
extern	int JoKirjattuKesto;
extern   INT lahetacr;
extern   char tyhjarivi[];
extern   char dhigh[];
extern   char shigh[];
extern   char  sgl[];
extern   char  dbl[];
extern   char sgle[];
extern   char dble[];
extern   INT ndhigh;
extern   INT nshigh;
extern INT   nchsgle;
extern INT   nchdble;
extern   wchar_t  dataf2name[];
extern   wchar_t  dataf3name[];
extern   wchar_t  datafbname[];
extern   wchar_t  namefname[];
extern   wchar_t  sarjafname[];
extern   wchar_t  sarjaxmlfname[];
extern   wchar_t  obfilename[];
extern   wchar_t  luetfnm[];
extern   wchar_t  ll_luetfnm[];
extern	 wchar_t  leimafname[];
extern	 wchar_t  ratafname[];
extern   wchar_t aikaflname[2][200];
extern	 wchar_t rataxmlfname[];
extern	 int ratatiedot;
extern	int minvakanttiluku;
extern	int lisavakanttialku;
extern	bool sailTiedPos;

extern	int ToimintaTila;
extern	wchar_t  alkulayout[];
extern   INT  kaynnistysvaihe;
extern   INT k_pv, arv_pv;
extern   int vaihevalinta;
extern	INT valparas;
extern   kilptietue *kilparr;
extern	sarjatietue Sarjat[MAXSARJALUKU+MAXYHD];
extern	INT selostaja;
extern   INT ilmkentat;
extern   int kuntosuunnmoodi;
extern   int vakanttialku;
extern   int seurVapaaBadge;
extern   bool ProsEmitOn;
//#if defined(MERI5)
// extern	wchar_t *lajinimet[5], *lajinm[5];
extern	int meri3;
extern	INT32 m3vert[5][MAXSARJALUKU];
//#endif
extern	 int sotilas;
extern   INT bootfl;
extern   INT nrec;
extern   INT kfirstfree;
extern   INT maxrec;
extern   datafile datf2,datf3;
extern   FILE *fdatf2;
//extern   INT *posarr;
extern   jarjtulostp *jarjtulos[MAXYHD+1][VALUKU+4];
//extern   INT *jarjseur[MAXYHD][VALUKU+4];
extern   INT16 *jrjseur[MAXYHD+1];
extern   INT xmlsarja;
extern   wchar_t tcptulos;
extern   INT  server_on;
extern   INT taulu_com, taulu_baud, tauluviive, udptaulu;
extern   int vaintaulu;
extern	 kilpindex  ixarr[NINDEX];
extern	 kilpindex  *ljarjindex,  *sarjaindex;
extern	 kilpindex  *srjaakindex,  *seuraindex, *seuranoindex;
extern	 kilpindex  *lahtoindex, *aakkosindex;
extern	 kilpindex  *nroindex, *srjnroindex, *idindex;
extern 	 kilpindex  *rataindex;
extern   kilpindex nameindex;
extern	 kilpindex pstindex;
extern	 kilpindex vaihetlsindex;
extern   int reindex, finfirst, lahtojarjfl, takaa_ajofl;
extern   INT keylen;
extern   wchar_t  kilpailu[83];
//extern   PRFILE  *lstf;
extern   tulostusparamtp lsttulparam;
extern   tulostusparamtp tartulparam;
extern   tulostusparamtp lokitulparam;
extern   tulostusparamtp *emittulparam;
extern	 tulostusparamtp aftulparam;
extern   tulostusparamtp autotulostusparam;
extern   tulostusparamtp sivutulostusparam;
extern	 bool lisaToimetOn;
extern   INT prblockbits;
extern   INT  origverify;
extern   INT esta_tall;
extern   char konetunn[3];
extern   wchar_t naapuri[MAX_PORTTI][3];
extern   INT talltaul;
extern   INT iono;
extern   INT tarfl;
extern   INT lahtarfl;
extern   INT hptarra, ktarra, rtarra, ntarra, ftarra;
extern   INT listfl;
extern   INT yvfl;
extern   INT spoolfl;
extern   INT tarknofl;
extern	char dec_sep;
extern	wchar_t wdec_sep;
extern   INT lajat_p;
extern   INT chkstk;
extern   INT autofl;
extern   wchar_t merkit;
extern   INT pscr;
extern   wchar_t *psinit;
extern   INT innpfl;
extern   wchar_t msga[81];
extern   INT  kynnys;
extern   INT  pfl;
extern   INT stopprt;
extern   INT verify;
extern   wchar_t listfname[201];
extern   wchar_t tarfname[201];
extern   INT lisaa_edelle;
extern   INT  t0;
extern   INT32 t0tics;
extern   INT  nilmt;
extern   INT  minkilpno;
extern   INT  maxkilpno;
extern   INT  sarjaluku;
extern   INT  yhtlahto;
extern   INT maxlsak[N_PV];
extern   INT32 pkarki[MAXSARJALUKU+MAXYHD][VALUKU+2];
extern   INT  vainalarajat, vainvaliajat;
extern   INT nsarjayhd, nsarjayhd_tul, sarjayhdva[MAXYHD];
extern   WCHAR yhdsarjat[MAXYHD][MAXSARJALUKU];
//extern   wchar_t yhdnimet[MAXYHD][LSARJA+1];
extern	INT  html_sail_nimi;
//extern   INT  takaa_ajo;
extern   kilptietue *aktkilp;
extern   INT  pistefl;
extern   INT  ol_piste;
extern   wchar_t *header;
// extern   INT ok;
extern   INT  ntulost;
extern   INT  nhylt,nkeskt;
extern   INT  neilt, nvakt;
extern   INT  nilm[MAXSARJALUKU+MAXYHD];
extern   INT  nyilm[MAXSARJALUKU+MAXYHD];
extern   INT  ntulos[MAXSARJALUKU+MAXYHD][VALUKU+2];
extern   INT  nhyl[MAXSARJALUKU+MAXYHD];
extern   INT  nkesk[MAXSARJALUKU+MAXYHD];
extern   INT  neil[MAXSARJALUKU+MAXYHD];
extern   INT  nvak[N_PV][MAXSARJALUKU+MAXYHD];
extern   INT  neilaika[MAXSARJALUKU+MAXYHD];
extern   int  nilm_pv[N_PV][MAXSARJALUKU+MAXYHD];
extern   INT  nva[VALUKU];
extern   INT  nesitt, nesit[MAXSARJALUKU+MAXYHD];
extern   INT16  *yhdsija[MAXYHD][VALUKU+2];
extern   INT  pfl;
extern   INT  backfl;
extern   INT  nimiixfl;
extern   INT  korjfl;
extern   INT  eiltarra;
extern   INT  initkehys;
extern   INT  initform;
extern   INT  prior[], fprior[];
extern   INT  prior0[];
extern   bool vaaditarkastus;
extern   INT  tauko;
extern   INT  tofs;
extern   int  maxyhteys;
extern   int  portbase;
//extern   wchar_t copybuf[CBUFL];
extern   INT  portti[];
extern   INT  comtype[MAX_PORTTI];
extern   ipparamtp ipparam[MAX_PORTTI+1];
extern	 int  broadcasts;
extern   HANDLE hComm[MAX_PORTTI];
#ifdef TCPSIIRTO
extern   INT  fileyht, tcpyhteys;
extern   long tcpkaynnistys[MAX_PORTTI+1];
#endif
extern   int TCPviive_lah[MAX_PORTTI];
extern   int TCPviive_nak[MAX_PORTTI];
extern   int TCPviive_pak[MAX_PORTTI];
extern   char   trlate[];
extern   INT  trlst,trtar,trloki;
extern   INT  f_ansi;
extern   INT  iobufr;
extern   INT  iobufmaxnr;
extern   INT  kaksoistar;
extern   INT  nktarra;
extern   INT  wtarra;
extern   INT  nrtarra;
extern   char tarstr[][13];
extern   tarrak kentta[25];     
extern   char form[], kehys1[];
extern   char savebox[], errbox[];
extern   int comfl, comopen[MAX_PORTTI], comclose[MAX_PORTTI];
extern   INT tcomfl;
extern   INT tulfl;
extern   INT estavalitys[MAX_LAHPORTTI];
extern   UINT32 kirjheti;
extern   INT immed;
extern   INT  lahec[MAX_LAHPORTTI];
extern   INT  lahkello[MAX_LAHPORTTI];
extern   INT  lahkelloraja[MAX_LAHPORTTI];
extern   bool  lahkellovain[MAX_LAHPORTTI];
extern   INT baud[], nprtch, comkesto[];
extern   char pty[MAX_PORTTI];
extern   int combits[MAX_PORTTI];
extern   int stopbits[MAX_PORTTI];
extern   INT obfile, levypusk;
extern   FILE *fobfile;
extern   INT inbens, inbseur;
extern   INT32 cjens[], cjseur[];
extern   INT lah_alku[MAX_LAHPORTTI];
extern   INT lah_vaihto[MAX_LAHPORTTI];
extern   INT tarlahcom[MAX_LAHPORTTI];
extern   INT lahcom[];
extern   INT vastcom[];
extern   INT yhteys_on[];
extern   int lah_tiedostot[MAX_LAHPORTTI];
extern   bool lahkaikkitied;
extern 	 int lahcomserver[];
extern   int keyclose[];                   /* Käyttäjä sulkenut portin */
extern   int jonokynnys;
extern   INT dcom;
extern   INT yhteysalku;
extern   int monitoriLkm;
//extern   char prs[];
extern   int bytecount, bytecountmax;
extern   INT ennakoi;
extern	 int aikaero[NREGNLY];
extern   aikajonotp *aikajono[MAXJONO];
extern   INT muut_time;
extern   INT od_fl, lajatfl,akttm, taustaaika, yhtjonot;
extern   INT32 od_aika;
extern 	 INT odota;
extern   INT modif[];
extern   char tlsmuutos[MAXSARJALUKU][VALUKU+1];
extern   INT timefl;
extern   INT aani;
extern   INT n_lisaa, kunto, seurvapaa;
extern   INT loki, lokiTCP;
extern   HANDLE hLoki;
extern   wchar_t lokifname[];
extern   INT com_ajat[];
extern   INT ajat_on, status_on;
extern   INT kuulpiste;
extern   INT lukum_fl;
extern	INT  pakotalaika;
extern   INT  nilmt;
extern   INT  nyilmt;
extern   INT  nytulost;
extern   INT  nilm[];
extern   INT  nyilm[];
//extern   INT  nytulos[];
extern   INT  sjfl[];
extern   INT jalku[][11+VALUKU];
extern   int tmaali;
extern   int maaliajat[];
extern   INT va_kello;
extern   int cn_port[MAX_PORTTI];
#ifdef TV1
extern   INT tv_fl, tv_test;
extern   INT tv_com, tv_baud;
extern   int cn_tv;
#endif
extern   INT vainoletus;
extern   INT maxntime;
extern   INT timerfl;
extern	INT pakotalaika;
#ifdef MAALI
extern   INT syotto;
extern   INT ajanottofl, sail_ajat;
extern   INT aika_com, com_aika, aika_esto, kello_esto;
extern   char aika_int_mask;
extern   INT regnly[NREGNLY];
extern   int regnlyhetki[NREGNLY];
extern   INT port_regnly[NREGNLY];
extern   INT racpisteet;
extern   INT aika_tunnus[NREGNLY][45];
extern	INT aika_tunnus_lkm;
extern	INT swimlaji;
extern  bool rataTunnistus;
extern	INT estaylim, ylimraja, estaneg, samapisteraja, uusinaika;
extern	lahdepsttp *lahdepisteet[MAXSARJALUKU+1];
extern	lahdepsttp *lahdejonot[MAXSARJALUKU+1];
extern   INT lahdepistehaku, lahdejonohaku;
extern   INT regnly_no[NREGNLY], kello_baud, kello_bits;
extern   int regnly_open[NREGNLY];
extern   INT cn_regnly[2*NREGNLY];
extern   INT od_regnly;
extern   INT32 t0_regnly, t0_regn[NREGNLY];
extern   INT32 maxero;
extern   INT kerro_regnly;
extern   INT usb_regnly[NREGNLY];
extern	 int pollaa, TCPpoll[NREGNLY];
extern   INT pyyntoviive;
#endif
extern   INT  va_kuollut;
#ifdef AUTOVA
extern   INT vaikavert, ilmtunt, vaajat;
#endif
#if defined(TXTTV)
extern   int tv_com, tv_baud;
extern   int tv_pty, tv_bits, tv_fc;
extern   int tv_open, suoracom, tv_test;
#endif
#ifdef MAASTO
extern   INT osarja[4][MAXSARJALUKU];
extern   UINT32 kl_kaava;
extern   UINT32 kl_ohjeaika;
extern   INT kl_x[KL_LUKU], kl_y[KL_LUKU];
#endif
extern   INT32 eralahto[MAXERA];
extern   INT16 era_akt;
extern   INT era_lahdot;
#ifdef UINTIxx
extern   eratp *erataul;
#endif
extern sotlajitp sotlajit[10];
extern   bdg_kno_tp *bdg_kno;
extern   INT nbadge, emitfl;
extern   INT emitloki;
extern   INT *emitjarr[2];
extern   INT emittime;
extern   INT  ohitatoisto;
extern   INT  siritreuna;
extern	 bool siritloki;
extern	 bool siritsync;
extern	 int sirithead, sirittail;
extern	 wchar_t siritbuf[SIRITBUFLEN+1];
extern   int valonaytto;
extern   int aikaTCPstatus;
extern   int muutoskysely, emitjono, elahtojono, tunnjono, tunnrajat[2];
#if defined(EMIT)
extern   IOFCourseData CourseData;

extern rastikoordtp rastikoord[MAXNPISTE];
extern maptp Map;
extern   INT  ajatfl, ilmlainat, emitkd, relax, monirata, emitsivu;
extern   INT  ohitaleimat, eilavoin;
extern   int maalileimasin;
extern   ratatp *rata;
extern   INT  nrata, kaikki_ajat[NREGNLY+1], vaadiaika, esitark, esiluenta, vainpiste[NREGNLY+1], luesuoraan;
extern   int  lukijalahde[NREGNLY];
extern   INT  luesuoraanviive;
extern   int emitajat, lahtopoikk;
extern	 int eNollausAjat;
extern   INT  autosarja, autorata;
extern   wchar_t maalitp[10];
extern   INT com_emit[];
extern   INT com_emitva[];
extern   int  emitfile;
extern   FILE *femitfile;
extern   wchar_t emitfname[];
extern   UINT *emitpos;
extern   INT emitptr, emithead, emithead2, annaseur;
extern   INT estaemittoisto;
extern	 int lukutoisto, sallisummavirhe;
extern	 int emitaglisa;
extern   INT autorap, comautorap;
extern   wchar_t autorapsrj[MAXSARJALUKU];
extern   INT ennakko;
extern   wchar_t viimtark;
extern   INT autokilp;
extern   INT seurkrivi;
extern   INT selaus;
extern   bool emitvaloki;
#endif
#if defined(COMETNO)
extern   INT  emitkd;
#endif
#ifdef JAAMA
extern	int jaamapisteet;
#endif
#ifdef MAKI
extern	int aktkilpailija, kilpailijataulu;
extern	int tuomari_yht;
extern	int yhdistetty, yhdist_katto;
extern	int aktkierros;
extern	int makipv;
extern	int nkierros[MAXSARJALUKU];
extern	int ptspermin;
extern	int tuomariluku;
extern	int pituuskerroin;
extern	int kpiste;
extern	int pituuscomet, nopeuscomet;
extern	int cometluku;
extern	wchar_t makiotsikko[TAULU_RIVPIT+1];
extern   int makihaku, autoselostaja;
extern   int yhteispisteet;
extern   int makivuoro[200];
#endif
#ifdef DBGFILE
extern   int dbgtofile;
#endif
extern   int lkmThreadOn;
//extern   OSVERSIONINFO osvi;
extern   kilpparamtp kilpparam;
extern 	 pvparamtp pvparam[N_PV];
extern   int datatallfl[];
extern   int in_data;
//extern int printer, tabs;
extern rvalitp *rvalit;
extern int rvluku;
extern bool origemvabadge;
extern int analvertosuus, emitanal_fl;
extern int analvertmax;
extern int TlsLstPaivita;
extern int NimiHTHaku;
extern int seurahaku;
extern bool vahvistaaina;
extern wchar_t fnameRacelist[];
extern wchar_t seuraTiedNm[200];
extern int KuntoMaali;

extern int (*korRaja)[2];
extern WCHAR *vipLaji;
extern unsigned int korVari[6];
extern int bibsiirtoalue;

#ifdef EMITHTTP
extern int inhttpHaku;
extern HTHREAD httphakuThread;
#endif

extern eThakuParamtp eTParam;
extern sqlparamtp sqlparam;
extern sqlparamtp sqlEparam;
extern bool inkirjSQLThread;
extern bool DeveloperMode;

extern int testi;

#endif