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

#include <stdio.h>
#include <winsock2.h>
#include <process.h>
#include <tputil.h>
#ifndef TCPSIIRTO
#define  TCPSIIRTO
#endif

#ifdef _CONSOLE
#define clrtxt(Y, X0, X1) viwrrect((Y),(X0),(Y),(X1),tyhjarivi,7,0,0)
#define clrln(Y) clrtxt((Y), 0, 79)
#endif

#define SOH 1
#define STX 2
#define ETX 3
#define ACK 6
#define NAK 21
#define IN_Q_EMPTY 10
#define UDPBUFSIZE 504
#define TCPBUFSIZE 2040
#define TCPRCVBUFSIZE 40000
#define UDPRCVBUFSIZE 40000
#define QLEN 8

#define UDP_RD_TIMEOUT 1000
#define RTT_RXTMIN   200
#define RTT_RXTMAX  5000
#define TCP_RD_TIMEOUT 5
#define TCP_SND_TIMEOUT 10
#define MAX_PORTTI  128

#define HTHREAD int
typedef struct sockaddr SA;
typedef struct sockaddr_in SAIN;
#define SAINSIZE   sizeof(SAIN)

static  HTHREAD TCPacceptThread[MAX_PORTTI];
static  HTHREAD TCPrcvThread[MAX_PORTTI];
static  HTHREAD UDPstreamThread[MAX_PORTTI];
static int WSAstarted, UDPlukum, TCPlukum;
static WSADATA  WSAdata;
static int yhtnot[MAX_PORTTI];
extern CRITICAL_SECTION tcpip_CriticalSection;
extern long tcpkaynnistys[];
static char konetn[3];
static int UDPcliWait = UDP_RD_TIMEOUT;
//long timeticks(void);

typedef struct {
	int  streamfl;
	SOCKET socksrv;
	SOCKET sockcli;
	SAIN servaddr;
	SAIN cliaddr;
	SA recvaddr;
	long rtt;
	long srtt;
	long rttvar;
	long rto;
	 char naapuri[4];
	char *rcvbuf;
	int bufhead;
	int buftail;
	long lastmsg;
//    long last_t;
//    long avlen;
//    long last_len;
//    char last_id;
//    char servbuf[UDPBUFSIZE+8];
//    char clibuf[UDPBUFSIZE+8];
	} UDPyhteystp;

static UDPyhteystp *UDPyhteys[MAX_PORTTI+20];

typedef struct {
	int serverfl;
	int connected;
	int reconnect;
	ipparamtp *ipparam;
    SOCKET sockmaster;
	SOCKET sockdata;
    SAIN masteraddr;
	SAIN dataaddr;
    SA destaddr;
    long rtt;
    long srtt;
    long rttvar; 
    long rto;
	char naapuri[4];
    char str_ip[64];
	char *rcvbuf;
	int bufhead;
	int buftail;
	long lastmsg;
	} TCPyhteystp;

#if defined(TCPLUKIJA) || defined(TCPSIIRTO) || defined(LAJUNEN)
static TCPyhteystp *TCPyhteys[MAX_PORTTI];
#endif

typedef struct {
	char destaddr[50];
	int  destport;
} TCPaddrtp;

//TCPaddrtp TCPaddr[MAX_PORTTI];


typedef struct {
    SOCKET sockcli;
	SAIN cliaddr;
   } broadcasttp;

#define MAX_BROADCAST 3

static broadcasttp broadcastyhteys[MAX_BROADCAST];
static int nBroadcast = 0;

void UDPstreamrcvTh(LPVOID lpCn);
void wkirjloki(wchar_t *msg);
wchar_t *wkello(void);
char *kello(void);
extern int loki, comfl;
extern int t0;
extern short int ySize;
extern int writeerrorOn;
extern   char tyhjarivi[];
#ifdef PING
extern int latestping[];
#endif
//long mstimer(void);
extern unsigned int TCP_on[];

#define bzero(a,len)  memset((a),0,(len))

#ifdef __linux__

int WSAStartup(int laji, WSADATA *WSAdata)
{
  return(0);
}

void WSACleanup(void)
{
}

int WSAGetLastError(void)
{
  return(0);
}

void WSASetLastError(int iError)
{
}

#endif

void writesocketerror(int source, int errcode, wchar_t *msg)
    {
    wchar_t line[81], lahteet[3][8] = {L"", L"UDP", L"TCP"};

    if (loki && errcode != 10054) {
		swprintf(line, L"%s: Socket-virhe %d: l‰hde%s. %s",
            wkello(), errcode, lahteet[source], msg);
        wkirjloki(line);
        }
    }

void UDPsetCliWait(int wait)
	{
	UDPcliWait = wait;
	}

int closeportUDP(HANDLE *hC)
{

	if (hC && (int)(*hC) > 0) {
		int yhtno = (int)(*hC) - 1;
//		SetCommTimeouts(*hC, &timeouts0);
	  closesocket(UDPyhteys[yhtno]->socksrv);
	  if (UDPyhteys[yhtno]->rcvbuf)
		  free(UDPyhteys[yhtno]->rcvbuf);
	  free(UDPyhteys[yhtno]);
	  UDPyhteys[yhtno] = 0;
	  *hC = 0;
	  }
	return(0);
}

void cleanWSA(void)
   {
   if (WSAstarted)
      WSACleanup();
   }

char *UDPnaapuri(HANDLE hC)
	{
	static char c0 = 0;
	if ((int)hC < 1)
		return(&c0);
	UDPyhteys[(int)hC - 1]->naapuri[2] = 0;
	return(UDPyhteys[(int)hC - 1]->naapuri);
	}

void UDPsetkonetunn(char *konetunn)
	{
	strncpy(konetn, konetunn, 3);
	}


UINT32 UDPaddr(HANDLE hC)
   {
   UINT32 addr;

	if ((int)hC < 1)
		return(0);
   addr = UDPyhteys[(int)hC - 1]->cliaddr.sin_addr.s_addr;
   if (addr == INADDR_NONE)
	  addr = 0;
   else
	  addr = ntohl(addr);
   return(addr);
   }

UINT16 UDPcliport(HANDLE hC)
   {
	if ((int)hC < 1)
		return(0);
   return(ntohs(UDPyhteys[(int)hC - 1]->cliaddr.sin_port));
   }

int openportUDP(HANDLE *hC, ipparamtp *ipparam)
   {
   DWORD  saddr = INADDR_NONE;
   int	yhtno, err;
   struct hostent *h_ent;
   char st[102];

   EnterCriticalSection(&tcpip_CriticalSection);
   // K‰ynnistet‰‰n SOCKET-toiminnot, ellei jo k‰ynnistetty
   if (!WSAstarted) {
	  if (!WSAStartup(2, &WSAdata))
		 WSAstarted = 1;
	  else {
		 LeaveCriticalSection(&tcpip_CriticalSection);
		 return(1);
		 }
	  }

   if ((int)(*hC) <= 0 || (int)(*hC) > (int)(sizeof(UDPyhteys)/sizeof(UDPyhteys[0])) ||
	   UDPyhteys[(int)(*hC)-1] == NULL) {
	  // alustetaan UDP-yhteys

	  for (yhtno = 0; yhtno < (int)(sizeof(UDPyhteys)/sizeof(UDPyhteys[0])); yhtno++)
		   if (UDPyhteys[yhtno] == NULL)
				break;
	  if (yhtno >= (int)(sizeof(UDPyhteys)/sizeof(UDPyhteys[0]))) {
		  LeaveCriticalSection(&tcpip_CriticalSection);
		  return(1);
		  }
	  *hC = (HANDLE) (yhtno + 1);
	  if (UDPyhteys[yhtno] == NULL)
		  UDPyhteys[yhtno] = (UDPyhteystp *) malloc(sizeof(UDPyhteystp));
	  memset(UDPyhteys[yhtno], 0, sizeof(UDPyhteystp));
	  UDPyhteys[yhtno]->rttvar = 750;
	  UDPyhteys[yhtno]->rto = 3000;
//      UDPyhteys[yhtno]->avlen = 100;

	  // alustetaan UDP server
	  if (ipparam->srvport > 0) {
		  UDPyhteys[yhtno]->socksrv = socket(AF_INET, SOCK_DGRAM, 0);

		  UDPyhteys[yhtno]->servaddr.sin_family      = AF_INET;
		  UDPyhteys[yhtno]->servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		  UDPyhteys[yhtno]->servaddr.sin_port        = htons(ipparam->srvport);
		  if (bind(UDPyhteys[yhtno]->socksrv, (SA *) &UDPyhteys[yhtno]->servaddr, SAINSIZE) != 0) {
			 LeaveCriticalSection(&tcpip_CriticalSection);
			 err = WSAGetLastError();
			 return(err);
			 }
		  if (ipparam->iptype == ipUDPSTREAM) {
			  yhtnot[yhtno] = yhtno;
			  UDPstreamThread[yhtno] = _beginthread(UDPstreamrcvTh, 40960, yhtnot+yhtno);
			  }
		  }

	   if (ipparam->iptype == ipUDPBOTH || ipparam->iptype == ipUDPCLIENT) {
		// alustetaan UDP client

		   yhtno = (int) (*hC) - 1;
		   if (!wcscmpU(ipparam->destaddr, L"AUTO")) {
			  if ((saddr = inet_addr((wcstoansi(st, ipparam->destaddr, 100)))) == INADDR_NONE) {
				 saddr = 1;
				 }
			  }
		   else if (!UDPyhteys[yhtno]->cliaddr.sin_addr.s_addr) {
			  if ((saddr = inet_addr((wcstoansi(st, ipparam->destaddr, 100)))) == INADDR_NONE) {
				 if ((h_ent = gethostbyname(st)) != NULL) {
					saddr = * (DWORD *)h_ent->h_addr_list[0];
					}
				 }
			  }
		   UDPyhteys[yhtno]->sockcli = socket(AF_INET, SOCK_DGRAM, 0);
		   UDPyhteys[yhtno]->cliaddr.sin_family      = AF_INET;
		   UDPyhteys[yhtno]->cliaddr.sin_addr.s_addr = saddr;
		   UDPyhteys[yhtno]->cliaddr.sin_port        = htons(ipparam->destport);
		   }
	   }

   LeaveCriticalSection(&tcpip_CriticalSection);
	return(0);
	}

#if defined(TCPLUKIJA) || defined(TCPSIIRTO) || defined(LAJUNEN)
int closeportTCP(HANDLE *hC)
{
	int yhtno = (int)(*hC) - 1;
   EnterCriticalSection(&tcpip_CriticalSection);
	if (yhtno >= 0 && yhtno < MAX_PORTTI && TCPyhteys[yhtno] != NULL) {
//		SetCommTimeouts(*hC, &timeouts0);
		TCPyhteys[yhtno]->connected = 0;
		if (TCPyhteys[yhtno]->sockdata)
			closesocket(TCPyhteys[yhtno]->sockdata);
		 if (TCPyhteys[yhtno]->serverfl && TCPyhteys[yhtno]->sockmaster)
			 closesocket(TCPyhteys[yhtno]->sockmaster);
		Sleep(1000);
		if (TCPyhteys[yhtno]) {
			if (TCPyhteys[yhtno]->rcvbuf)
				free(TCPyhteys[yhtno]->rcvbuf);
			free(TCPyhteys[yhtno]);
			TCPyhteys[yhtno] = 0;
			*hC = 0;
			}
		}
	LeaveCriticalSection(&tcpip_CriticalSection);
	return(0);
}

int openportBroadcast(ipparamtp *ipparam)
   {
   DWORD  saddr = INADDR_NONE;
   struct hostent *h_ent;
   char st[102];

	if (nBroadcast >= MAX_BROADCAST)
		return(nBroadcast);
   EnterCriticalSection(&tcpip_CriticalSection);
   // K‰ynnistet‰‰n SOCKET-toiminnot, ellei jo k‰ynnistetty
   if (!WSAstarted) {
	  if (!WSAStartup(2, &WSAdata))
		 WSAstarted = 1;
	  else {
		 LeaveCriticalSection(&tcpip_CriticalSection);
			return(0);
		 }
	  }

	// alustetaan UDP client

   if ((saddr = inet_addr((wcstoansi(st, ipparam->destaddr, 100)))) == INADDR_NONE)
	  if ((h_ent = gethostbyname(st)) != NULL)
		 saddr = * (DWORD *)h_ent->h_addr_list[0];
   if (saddr != INADDR_NONE) {
		 broadcastyhteys[nBroadcast].sockcli = socket(AF_INET, SOCK_DGRAM, 0);
		 broadcastyhteys[nBroadcast].cliaddr.sin_family      = AF_INET;
		 broadcastyhteys[nBroadcast].cliaddr.sin_addr.s_addr = saddr;
		 broadcastyhteys[nBroadcast].cliaddr.sin_port        = htons(ipparam->destport);
		 nBroadcast++;
		 }
   LeaveCriticalSection(&tcpip_CriticalSection);
	return(nBroadcast);
	}

char *TCPdestaddr(HANDLE hC)
{
	if ((int)hC < 1)
		return(0);
	return(TCPyhteys[(int)hC-1]->str_ip);
}

int TCPdestport(HANDLE hC)
{
	SAIN *sain;

	if ((int)hC < 1)
		return(0);
	if (!TCPyhteys[(int)hC-1]->serverfl)
		sain = (SAIN *)&TCPyhteys[(int)hC-1]->dataaddr;
	else
		sain = (SAIN *)&TCPyhteys[(int)hC-1]->destaddr;
	return(ntohs(sain->sin_port));
}

int TCPyht_on(HANDLE hC)
{
	if ((int)hC < 1)
		return(0);
	if (!TCPyhteys[(int)hC-1])
		return(0);
	return(TCPyhteys[(int)hC-1]->connected);
}


int TCPbuffered(HANDLE hC)
{
	if ((int)hC < 1)
		return(0);
	return((TCPyhteys[(int)hC-1]->bufhead - TCPyhteys[(int)hC-1]->buftail + TCPRCVBUFSIZE) % TCPRCVBUFSIZE);
}

void i_flush_TCP(HANDLE hC)
{
	if ((int)hC < 1)
		return;
	EnterCriticalSection(&tcpip_CriticalSection);
	TCPyhteys[(int)hC-1]->bufhead = TCPyhteys[(int)hC-1]->buftail;
	LeaveCriticalSection(&tcpip_CriticalSection);
}


//  int read_TCP(HANDLE hC, int buflen, char *buf, int *nread)
//     hC     TCP-yhteyden numero (HANDLE)
//     buflen siirrett‰vien merkkien maksimim‰‰r‰
//     buf    puskuri, johon merkit siiret‰‰n
//     nread  osoitin integer-muuttujaan, jossa palautetaan siirrettyjen merkkien luku
//
//     funktio palauttaa aika arvon 0
//
//     funktio katsoo onko TCP-yhteyden hC saapuvien merkkien jonossa merkkej‰ ja siirt‰‰
//     merkit puskuriin buf. Merkkej‰ siirret‰‰n korkeintaan buflen ja siirrettyjen merkkien m‰‰r‰
//     palautetaan muuttujassa nread
//     kun yhteys on client-yhteys ja yhteys on katkennut, k‰ynnist‰‰ ohjelma yhteyden uudelleenk‰ynnistysyrityksen

int read_TCP(HANDLE hC, int buflen, char *buf, int *nread)
{
	char *p;
	int yhtno;
	TCPyhteystp *yht;

	*nread = 0;
	if ((int)hC < 1)
		return(0);
	yhtno = (int)hC - 1;
	yht = TCPyhteys[yhtno];
	if (!yht->connected && !yht->serverfl)
		reconnectTCP(hC);
	if (!TCPbuffered(hC))
		return(IN_Q_EMPTY);
	EnterCriticalSection(&tcpip_CriticalSection);
	*nread = TCPbuffered(hC);
	if (*nread > buflen)
		*nread = buflen;
	for (p = buf; p < buf + *nread; p++) {
		if (!yht->rcvbuf)
			break;
		*p = yht->rcvbuf[yht->buftail];
		yht->buftail = (yht->buftail + 1) % TCPRCVBUFSIZE;
		}
	LeaveCriticalSection(&tcpip_CriticalSection);
	return(0);
}

//  int peek_TCP(HANDLE hC, int buflen, char *buf, int *nread)
//     hC     TCP-yhteyden numero (HANDLE)
//     buflen siirrett‰vien merkkien maksimim‰‰r‰
//     buf    puskuri, johon merkit siiret‰‰n
//     nread  osoitin integer-muuttujaan, jossa palautetaan siirrettyjen merkkien luku
//
//     funktio palauttaa aika arvon 0
//
//     funktio katsoo onko TCP-yhteyden hC saapuvien merkkien jonossa merkkej‰ ja siirt‰‰
//     merkit puskuriin buf. Merkkej‰ siirret‰‰n korkeintaan buflen ja siirrettyjen merkkien m‰‰r‰
//     palautetaan muuttujassa nread.
//	   poiketen aliohjelmasta read_TCP, ei merkkej‰ poisteta tiedonsiirron puskurista.
//     kun yhteys on client-yhteys ja yhteys on katkennut, k‰ynnist‰‰ ohjelma yhteyden uudelleenk‰ynnistysyrityksen

int peek_TCP(HANDLE hC, int buflen, char *buf, int *nread)
{
	char *p;
	int yhtno, prcvbuf;
	TCPyhteystp *yht;

	*nread = 0;
	if ((int)hC < 1)
		return(0);
	yhtno = (int)hC - 1;
	yht = TCPyhteys[yhtno];
	if (!yht->connected && !yht->serverfl) {
		if (!yht->ipparam)
			return(0);
		yht->ipparam->status = 1;
		if (yht->reconnect)
			reconnectTCP(hC);
		else {
			return(0);
			}
		}
	if (!TCPbuffered(hC))
		return(IN_Q_EMPTY);
	EnterCriticalSection(&tcpip_CriticalSection);
	*nread = TCPbuffered(hC);
	if (*nread > buflen)
		*nread = buflen;
	prcvbuf = yht->buftail;
	for (p = buf; p < buf + *nread; p++) {
		if (!yht->rcvbuf)
			break;
		*p = yht->rcvbuf[prcvbuf];
		prcvbuf = (prcvbuf + 1) % TCPRCVBUFSIZE;
		}
	LeaveCriticalSection(&tcpip_CriticalSection);
	return(0);
}

// void TCPrcvTh(LPVOID lpCn)
//    funktio k‰ynnistet‰‰n omana s‰ikeen‰‰n jokaiselle TCP-yhteydelle, jonka on avannut TCP client funktiossa
//    openportTCP tai TCP palvelin ohjelmassa TCPacceptTh saatuaan yhteyspyynnˆn clientilta
//
//    lpCn  on integer-muuttujan osoite. Muuttuja sis‰lt‰‰ TCP-yhteyden numeron v‰lilt‰ 0 .. MAX_PORTTI-1
//
//    funktio k‰ynnist‰‰ ikuisen loopin, joka odottaa, ett‰ vastapuoli l‰hett‰‰ merkkej‰ ja socket
//    TCPyhteys[yhtno]->sockdata ilmoittaa vastaanottaneensa merkkej‰. Funktio siirt‰‰ merkit puskuriin
//    TCPyhteys[yhtno]->rcvbuf p‰ivitt‰en seuraavaan vapaaseen paikkaan osoittavan arvon bufhead
//    puskurin t‰yttyess‰ h‰vitt‰‰ funktio merkkej‰ vanhimmasta p‰‰st‰ siirt‰m‰ll‰ osoitinta buftail
//    funktio on blocking-tilassa aina, kun uusia merkkej‰ ei ole k‰sitelt‰v‰n‰.
//    Kun funktiosta read palataan, vaikka merkkej‰ ei ole vastaanotettu, merkitsee ohjelma yhteyden suljetuksi
//    ja sulkee s‰ikeen. N‰in tapahtuu myˆs, kun socket suljetaan ohjelmasta poistuttaessa

void TCPrcvTh(LPVOID lpCn)
{
	int yhtno, nread;
	int err = 0;
	char buf[TCPBUFSIZE], *p;
	wchar_t wprs[80], st[40];

	yhtno = *((int *) lpCn);

	if (!TCPyhteys[yhtno] || !TCPyhteys[yhtno]->ipparam)
		return;
	TCPyhteys[yhtno]->connected = 1;
	TCPyhteys[yhtno]->ipparam->status = 0;
	if (!TCPyhteys[yhtno]->rcvbuf) {
		TCPyhteys[yhtno]->rcvbuf = (char *) calloc(1,TCPRCVBUFSIZE);
		}
	while (TCPyhteys[yhtno] && TCPyhteys[yhtno]->rcvbuf && comfl) {
		if (TCPbuffered((HANDLE)(yhtno+1)) + TCPBUFSIZE + 1 >= TCPRCVBUFSIZE) {
			Sleep(300);
			continue;
			}
		if (!TCPyhteys[yhtno])
			break;
		nread = recv(TCPyhteys[yhtno]->sockdata, buf, sizeof(buf), 0);
		if (nread <= 0) {
			if (nread == SOCKET_ERROR) {
				err = WSAGetLastError();
				}
			if (err != WSAETIMEDOUT)
				break;
			else {
				if (TCPyhteys[yhtno])
					TCPyhteys[yhtno]->connected = -1;
				Sleep(1000);
				continue;
				}
			}
		else {
			TCPyhteys[yhtno]->connected = 1;
			TCPyhteys[yhtno]->lastmsg = t_time_l(timeticks(), t0);
			}
		EnterCriticalSection(&tcpip_CriticalSection);
		for (p = buf; nread; nread--) {
			if (!TCPyhteys[yhtno] || !TCPyhteys[yhtno]->rcvbuf)
				break;
			TCPyhteys[yhtno]->rcvbuf[TCPyhteys[yhtno]->bufhead] = *p;
			p++;
			TCPyhteys[yhtno]->bufhead = (TCPyhteys[yhtno]->bufhead + 1) % TCPRCVBUFSIZE;
			if (TCPyhteys[yhtno]->bufhead == TCPyhteys[yhtno]->buftail)
				TCPyhteys[yhtno]->buftail = (TCPyhteys[yhtno]->buftail + 1) % TCPRCVBUFSIZE;
			}
		LeaveCriticalSection(&tcpip_CriticalSection);
		}
	if (err) {
		writesocketerror(2, err, L"receive TCP - disconnected");
		}
	if (TCPyhteys[yhtno]) {
		TCPyhteys[yhtno]->connected = 0;
		if (TCPyhteys[yhtno]->ipparam) {
			TCPyhteys[yhtno]->ipparam->status = 1;
			if (TCPyhteys[yhtno]->serverfl) {
				swprintf(wprs, L"%s: TCP-yhteys %15.15s porttiin %d katkaistu",
					wkello(), ansitowcs(st, TCPyhteys[yhtno]->str_ip, 30), TCPyhteys[yhtno]->ipparam->srvport);
				TCPyhteys[yhtno]->str_ip[0] = 0;
				}
			else
				swprintf(wprs, L"%s: TCP-yhteys %15.15s:%d katkaistu",
					wkello(), TCPyhteys[yhtno]->ipparam->destaddr, TCPyhteys[yhtno]->ipparam->destport);
			vidspwmsg(ySize-1,0,0,7, wprs);
			if (loki)
				wkirjloki(wprs);
			if (comfl && TCPyhteys[yhtno] && TCPyhteys[yhtno]->ipparam && !TCPyhteys[yhtno]->connected &&
				!TCPyhteys[yhtno]->serverfl) {
				TCPyhteys[yhtno]->ipparam->status = 1;
				if (TCPyhteys[yhtno]->reconnect) {
					writesocketerror(2, err, L"Reconnect TCP");
					if (TCPyhteys[yhtno] && reconnectTCP((HANDLE)(yhtno+1))) {
						err = WSAGetLastError();
						WSASetLastError(0);
						writesocketerror(2, err, L"Reconnect TCP failed");
						}
					}
				}
			}
		}
}

// void TCPacceptTh(LPVOID lpCn)
//    funktio k‰ynnistet‰‰n omana s‰ikeen‰‰n jokaiselle TCP-palvelimelle
//
//    lpCn  on integer-muuttujan osoite. Muuttuja sis‰lt‰‰ TCP-yhteyden numeron v‰lilt‰ 0 .. MAX_PORTTI-1
//
//    funktio k‰ynnist‰‰ ikuisen loopin, joka kuuntelee yhteydelle m‰‰ritelty‰ porttia ja
//	  vastaanotettuaan yhteyspyynnˆn socketiksi TCPyhteys[yhtno]->sockdata avaa yhteyden luomalla uuden s‰ikeen
//    funktiolle TCPrcvTh antanen parametriksi TCP-yhteyden numeron.
//    Ohjelma tallettaa vastapuolen osoitteen tekstimuotoisena muuttujaan TCPyhteys[yhtno]->str_ip.
//    S‰ie on funktion accept odotustilassa aina paitsi vastaanotettuaan yhteyspyynnˆn ja avatessaan yhteyden
//    S‰ikeest‰ poistutaan, kun socket suljetaan, jolloin accept palauttaa arvon INVALID_SOCKET ja s‰ie suljetaan.

void TCPacceptTh(LPVOID lpCn)
	{
	int alen, yhtno;
	UINT32 addr;
	SAIN *sain;

	yhtno = *((int *) lpCn);
	yhtnot[yhtno] = yhtno;
	alen = sizeof(TCPyhteys[yhtno]->destaddr);
	sain = (SAIN *) &TCPyhteys[yhtno]->destaddr;
	while (comfl && TCPyhteys[yhtno] && TCPyhteys[yhtno]->ipparam) {
		SOCKET sock;
		sock = accept(TCPyhteys[yhtno]->sockmaster, &TCPyhteys[yhtno]->destaddr, &alen);
		if (TCPyhteys[yhtno]) {
			TCPyhteys[yhtno]->sockdata = sock;
			if (sock != INVALID_SOCKET) {
				EnterCriticalSection(&tcpip_CriticalSection);
				TCPrcvThread[yhtno] = _beginthread(TCPrcvTh, 40960, yhtnot+yhtno);
				addr = sain->sin_addr.s_addr;
				if (addr == INADDR_NONE)
					addr = 0;
				else
					addr = ntohl(addr);
				if (!TCPyhteys[yhtno])
					break;
				sprintf(TCPyhteys[yhtno]->str_ip, "%u.%u.%u.%u", (addr/16777216) & 255, (addr/65536) & 255,
							(addr/256) & 255, addr & 255);
				TCPyhteys[yhtno]->str_ip[15] = 0;
				LeaveCriticalSection(&tcpip_CriticalSection);
				TCPyhteys[yhtno]->ipparam->status = 0;
				}
			}
		}
	if (TCPyhteys[yhtno]) {
		if (TCPyhteys[yhtno]->rcvbuf)
			free(TCPyhteys[yhtno]->rcvbuf);
		free(TCPyhteys[yhtno]);
		TCPyhteys[yhtno] = 0;
		}
	}

//  int openportTCP(HANDLE *hC, ipparamtp *ipparam)
//      hC         osoitin TCP-yhteydelle annettavaan numeroon (HANDLE)
//		ipparam	   avattavaa yhteytt‰ koskevat parametrit (portit, vastapuolen osoite, yhteyden tyyppi)
//
//	    ohjelma palauttaa arvon 0, kun k‰ynnistys onnistuu t‰m‰n ohjelman osalta ja muulloin virhekoodin
//
//		Ohjelma k‰ynnist‰‰ joko TCP clientin tai TCP palvelimen riippuen siit‰, onko vastapuolen ip-osoite
//      ilmoitettu parametrissa ipparam vai ei (client, kun ip-osoite on annettu)
//
//      Vermistettuaan, ett‰ WSA on k‰ynniss‰, ohjelma m‰‰r‰‰ yhteyden numeroksi ensimm‰isen vapaan numeron ja
//      tallentaa sen parametrin hC osoittamaan osoitteeseen.
//
//      Clientin tapauksessa yritt‰‰ ohjelma avata yhteyden osoitteeseen destaddr:destport ja se onnistuessa
//      k‰ynnist‰‰ yhteydelle uutena s‰ikeen‰ funktion TCPrcvTh. K‰ytett‰v‰ socket on TCPyhteys[yhtno]->sockdata
//
//	    Palvelimen tapauksessa ohjelma luo socketin TCPyhteys[yhtno]->sockmaster ja k‰ynnist‰‰ uudessa s‰ikeess‰
//      funktion TCPacceptTh ottamaan vastaan yhteyspyyntˆj‰.
//

int openportTCP(HANDLE *hC, ipparamtp *ipparam)
	{
	DWORD  saddr;
	int	yhtno, retcode = 0, n;
	struct hostent *h_ent;
	char st[102];

	yhtno = (int) *hC - 1;
	*hC = 0;
	EnterCriticalSection(&tcpip_CriticalSection);
	// K‰ynnistet‰‰n SOCKET-toiminnot, ellei jo k‰ynnistetty
	if (!WSAstarted) {
		if (!WSAStartup(2, &WSAdata))
			WSAstarted = 1;
		else {
			LeaveCriticalSection(&tcpip_CriticalSection);
			return(99);
			}
		}

	if (yhtno >= 0 && yhtno < MAX_PORTTI && TCPyhteys[yhtno] && 
		TCPyhteys[yhtno]->ipparam != 0 && TCPyhteys[yhtno]->connected) {
		LeaveCriticalSection(&tcpip_CriticalSection);
		return(98);
		}
		
	for (yhtno = 0; yhtno < MAX_PORTTI; yhtno++)
		if (TCPyhteys[yhtno] == NULL || TCPyhteys[yhtno]->ipparam == 0)
			break;

	if (yhtno < MAX_PORTTI) {
		yhtnot[yhtno] = yhtno;
		*hC = (HANDLE)(yhtno + 1);

		if (TCPyhteys[yhtno] == NULL) {
			TCPyhteys[yhtno] = (TCPyhteystp *) malloc(sizeof(TCPyhteystp));
			}

		memset(TCPyhteys[yhtno], 0, sizeof(TCPyhteystp));
		TCPyhteys[yhtno]->ipparam = ipparam;
		TCPyhteys[yhtno]->reconnect = ipparam->noreconnect == 0;
		TCPyhteys[yhtno]->rttvar = 750;
		TCPyhteys[yhtno]->rto = 3000;

		if (!ipparam->destaddr[0])
			TCPyhteys[yhtno]->serverfl = 1;

		// alustetaan TCP client

		if (!TCPyhteys[yhtno]->serverfl) {
			if ((saddr = inet_addr((wcstoansi(st, ipparam->destaddr, 100)))) == INADDR_NONE) {
				if ((h_ent = gethostbyname(st)) != NULL)
					saddr = * (DWORD *)h_ent->h_addr_list[0];
				}
			strcpy(TCPyhteys[yhtno]->str_ip, st);
			TCPyhteys[yhtno]->sockdata = socket(AF_INET, SOCK_STREAM, 0);
			TCPyhteys[yhtno]->dataaddr.sin_family      = AF_INET;
			TCPyhteys[yhtno]->dataaddr.sin_addr.s_addr = saddr;
			TCPyhteys[yhtno]->dataaddr.sin_port        = htons(ipparam->destport);
			for (n = 0; n < 6; n++) {
				if (TCP_on[n] == TCPyhteys[yhtno]->dataaddr.sin_addr.s_addr)
					break;
				if (!TCP_on[n]) {
					TCP_on[n] = TCPyhteys[yhtno]->dataaddr.sin_addr.s_addr;
					break;
					}
				}

			if (connect(TCPyhteys[yhtno]->sockdata, (SA *) &TCPyhteys[yhtno]->dataaddr,
				sizeof(SAIN))) {
				retcode = WSAGetLastError();
				}
			else {
				TCPrcvThread[yhtno] = _beginthread(TCPrcvTh, 40960, yhtnot+yhtno);
				}
			}
		else {

		// alustetaan TCP server

			TCPyhteys[yhtno]->sockmaster = INVALID_SOCKET;
			if (ipparam->srvport <= 0)
				retcode = 1;
			else {
				if ((TCPyhteys[yhtno]->sockmaster = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
					retcode = WSAGetLastError();
					}
				}
			if (!retcode) {
				TCPyhteys[yhtno]->masteraddr.sin_family      = AF_INET;
				TCPyhteys[yhtno]->masteraddr.sin_addr.s_addr = htonl(INADDR_ANY);
				TCPyhteys[yhtno]->masteraddr.sin_port        = htons(ipparam->srvport);
				if (bind(TCPyhteys[yhtno]->sockmaster, (SA *) &TCPyhteys[yhtno]->masteraddr,
					SAINSIZE)) {
					retcode = WSAGetLastError();
					}
				else {
					if (listen(TCPyhteys[yhtno]->sockmaster, QLEN) == SOCKET_ERROR) {
						retcode = WSAGetLastError();
						}
					else {
						Sleep(300);
						TCPacceptThread[yhtno] = _beginthread(TCPacceptTh, 40960, yhtnot+yhtno);
						}
					}
				}
			}
		if (retcode && TCPyhteys[yhtno]) {
			if (TCPyhteys[yhtno]->rcvbuf)
				free(TCPyhteys[yhtno]->rcvbuf);
			free(TCPyhteys[yhtno]);
			TCPyhteys[yhtno] = 0;
			}
		}

	LeaveCriticalSection(&tcpip_CriticalSection);
	return(retcode);
}

/*
void setUDPtimeouts(HANDLE hC, int rdconst, int rdmult, int rdintvl,
					int wrtconst, int wrtmult)
{
   timeouts.ReadIntervalTimeout = rdintvl;
   timeouts.ReadTotalTimeoutMultiplier = rdmult;
   timeouts.ReadTotalTimeoutConstant = rdconst;
   timeouts.WriteTotalTimeoutMultiplier = wrtmult;
   timeouts.WriteTotalTimeoutConstant = wrtconst;
   SetCommTimeouts(hC, &timeouts);
}
*/

#define RCWAIT 15000

int reconnectTCP(HANDLE hC)
	{
    int	n, yhtno, retcode, msnow;
	char prs[82];
	static int inrcTCP[MAX_PORTTI], lastRC[MAX_PORTTI];

   yhtno = (int) hC - 1;
   if (yhtno < 0)
	   return(1);
   if (!comfl || !TCPyhteys[yhtno]->ipparam || TCPyhteys[yhtno]->serverfl ||
	   TCPyhteys[yhtno]->connected)
	   return(0);
   if (inrcTCP[yhtno])
	  return(0);
   msnow = mstimer();
   if (msnow - lastRC[yhtno] < RCWAIT)
	   return(0);
   lastRC[yhtno] = msnow;
   inrcTCP[yhtno] = 1;
   for (n = 0; n < 6; n++) {
	  if (!TCPyhteys[yhtno]->ipparam ||
		 TCP_on[n] == TCPyhteys[yhtno]->dataaddr.sin_addr.s_addr)
		 break;
	  }
#ifdef PING
   if (n < 6 && mstimer() - latestping[n] > 30000) {
	  inrcTCP[yhtno] = 0;
	  return 0;
	  }
#endif
   clrln(ySize-1);
   sprintf(prs, "%s: TCP-yhteyden %15.15s uudelleenk‰ynnistys aloitettu",
	  kello(), TCPyhteys[yhtno]->str_ip);
   vidspmsg(ySize-1,0,0,7, prs);
   writeerrorOn = 30;
   closesocket(TCPyhteys[yhtno]->sockdata);
   Sleep(1000);
   if (TCPyhteys[yhtno]->ipparam) {
	   TCPyhteys[yhtno]->sockdata = socket(AF_INET, SOCK_STREAM, 0);
	   retcode = connect(TCPyhteys[yhtno]->sockdata, (SA *) &TCPyhteys[yhtno]->dataaddr, SAINSIZE);
	   clrln(ySize-1);
	   if (!retcode) {
		  TCPrcvThread[yhtno] = _beginthread(TCPrcvTh, 40960, yhtnot+yhtno);
		  sprintf(prs, "%s: TCP-yhteyden %15.15s   uudelleenk‰ynnistys onnistui",
			 kello(), TCPyhteys[yhtno]->str_ip);
		  tcpkaynnistys[yhtno] = timeticks()+1;
		  }
	   else
		  sprintf(prs, "%s: TCP-yhteyden %15.15s   uudelleenk‰ynnistys ep‰onnistui",
			 kello(), TCPyhteys[yhtno]->str_ip);
	   vidspmsg(ySize-1,0,0,7, prs);
	   }
   writeerrorOn = 50;
   inrcTCP[yhtno] = 0;
   return(retcode);
   }

int wrt_st_TCP(HANDLE hC, int len, char *buf, int *nsent)
{
	int err = 0, n_ready = 0, yhtno;
	fd_set  sfds;
	struct timeval tv = {TCP_SND_TIMEOUT, 0};

   yhtno = (int) hC - 1;
	if (len <= 0 || yhtno < 0)
		return(0);
	*nsent = 0;
	if (TCPyhteys[yhtno]->connected) {
		EnterCriticalSection(&tcpip_CriticalSection);
		FD_ZERO(&sfds);
		FD_SET(TCPyhteys[yhtno]->sockdata, &sfds);

		if ((n_ready = select(FD_SETSIZE, 0, &sfds, 0, &tv)) == SOCKET_ERROR && comfl) {
			err = WSAGetLastError();
			WSASetLastError(0);
			n_ready = 0;
			writesocketerror(2, err, L"send TCP (select)");
			}
		if (n_ready && FD_ISSET(TCPyhteys[yhtno]->sockdata, &sfds)) {
			*nsent = send(TCPyhteys[yhtno]->sockdata, buf, len, 0);
			if (*nsent == SOCKET_ERROR) {
				err = WSAGetLastError();
				WSASetLastError(0);
				*nsent = 0;
				writesocketerror(2, err, L"send TCP");
				}
			}
		if (*nsent > 0) {
			TCPyhteys[yhtno]->lastmsg = t_time_l(timeticks(), t0);
			TCPyhteys[yhtno]->connected = 1;
			}
		else {
			if (TCPyhteys[yhtno]->reconnect == 2 || err != WSAETIMEDOUT)
				TCPyhteys[yhtno]->connected = 0;
			else
				TCPyhteys[yhtno]->connected = -1;
			}
		LeaveCriticalSection(&tcpip_CriticalSection);
		}
	if (comfl && TCPyhteys[yhtno]->ipparam && !TCPyhteys[yhtno]->connected &&
		!TCPyhteys[yhtno]->serverfl) {
		TCPyhteys[yhtno]->ipparam->status = 1;
		if (TCPyhteys[yhtno]->reconnect) {
			writesocketerror(2, err, L"Reconnect TCP");
			if (reconnectTCP(hC)) {
				err = WSAGetLastError();
				WSASetLastError(0);
				writesocketerror(2, err, L"Reconnect TCP failed");
				}
			}
		else {
			err = 0;
			}
		}
	return(err);
}

int wrt_ch_TCP(HANDLE hC, char ch)
{
	int nsent;

	return(wrt_st_TCP(hC, 1, &ch, &nsent));
}

#endif  // TCPLUKIJA || TCPSIIRTO || defined(LAJUNEN)

static long rtt_minmax(long rto)
   {
   if (rto < RTT_RXTMIN)
	  rto = RTT_RXTMIN;
   else if (rto > RTT_RXTMAX)
	  rto = RTT_RXTMAX;
   return(rto);
   }

#define UDPBUF 4096

int UDPcliAllowBroadcast(HANDLE hC)
{
	int allow = 1;

	if ((int)hC < 1)
		return(0);
	return(setsockopt(
		UDPyhteys[(int)hC-1]->sockcli,
		SOL_SOCKET,
		SO_BROADCAST,
		(const char *)&allow,
		sizeof(int)));
}

int UDPsrvAllowBroadcast(HANDLE hC)
{
	int allow = 1;

	if ((int)hC < 1)
		return(0);
	return(setsockopt(
		UDPyhteys[(int)hC-1]->socksrv,
		SOL_SOCKET,
		SO_BROADCAST,
		(const char *)&allow,
		sizeof(int)));
}

int UDPstreambuffered(HANDLE hC)
{
	if ((int)hC < 1)
		return(0);
	if (!UDPyhteys[(int)hC-1]->streamfl)
		return(0);
	return((UDPyhteys[(int)hC-1]->bufhead - UDPyhteys[(int)hC-1]->buftail + UDPRCVBUFSIZE) % UDPRCVBUFSIZE);
}

void i_flush_UDPstream(HANDLE hC)
{
	if ((int)hC < 1)
		return;
	if (!UDPyhteys[(int)hC-1]->streamfl)
		return;
	EnterCriticalSection(&tcpip_CriticalSection);
	UDPyhteys[(int)hC-1]->bufhead = UDPyhteys[(int)hC-1]->buftail;
	LeaveCriticalSection(&tcpip_CriticalSection);
}

int read_UDPstream(HANDLE hC, int buflen, char *buf, int *nread)
{
	char *p;
	int yhtno;
	UDPyhteystp *yht;

	if ((int)hC < 1)
		return(0);
	yhtno = (int)hC - 1;
	yht = UDPyhteys[yhtno];
	*nread = UDPstreambuffered(hC);
	if (*nread > buflen)
		*nread = buflen;
	for (p = buf; p < buf + *nread; p++) {
		*p = yht->rcvbuf[yht->buftail];
		yht->buftail = (yht->buftail + 1) % UDPRCVBUFSIZE;
		}
	return(0);
}

int peek_UDPstream(HANDLE hC, int buflen, char *buf, int *nread)
{
	char *p;
	int yhtno, rcvbufp;
	UDPyhteystp *yht;

	if ((int)hC < 1)
		return(0);
	yhtno = (int)hC - 1;
	yht = UDPyhteys[yhtno];
	*nread = UDPstreambuffered(hC);
	if (*nread > buflen)
		*nread = buflen;
	rcvbufp = yht->buftail;
	for (p = buf; p < buf + *nread; p++) {
		*p = yht->rcvbuf[rcvbufp];
		rcvbufp = (rcvbufp + 1) % UDPRCVBUFSIZE;
		}
	return(0);
}

void UDPstreamrcvTh(LPVOID lpCn)
{
	int yhtno, nread;
	char buf[UDPBUF], *p;

	yhtno = *((int *) lpCn);
	if (UDPyhteys[yhtno]->cliaddr.sin_addr.s_addr == INADDR_NONE)
	  return;

	if (!UDPyhteys[yhtno]->rcvbuf) {
		UDPyhteys[yhtno]->rcvbuf = (char *) calloc(1,UDPRCVBUFSIZE);
		}
	UDPyhteys[yhtno]->streamfl = 1;
	while (UDPyhteys[yhtno]->rcvbuf && comfl) {
		read_UDP_0((HANDLE)(yhtno+1), sizeof(buf), buf, &nread);
		if (nread <= 0 || nread >= sizeof(buf))
			break;
		UDPyhteys[yhtno]->lastmsg = t_time_l(timeticks(), t0);
		for (p = buf; nread; nread--) {
			UDPyhteys[yhtno]->rcvbuf[UDPyhteys[yhtno]->bufhead] = *p;
			p++;
			UDPyhteys[yhtno]->bufhead = (UDPyhteys[yhtno]->bufhead + 1) % UDPRCVBUFSIZE;
			if (UDPyhteys[yhtno]->bufhead == UDPyhteys[yhtno]->buftail)
				UDPyhteys[yhtno]->buftail = (UDPyhteys[yhtno]->buftail + 1) % UDPRCVBUFSIZE;
			}
		}
}

int read_UDP_0(HANDLE hC, int len_t, char *pbuf, int *nread)
{
   int frlen, err = 0;
   SAIN *addr_in;

   *nread = 0;
	if ((int)hC < 1)
		return(0);
   if (UDPyhteys[(int)hC-1]->cliaddr.sin_addr.s_addr == INADDR_NONE)
	  return(0);

	frlen = sizeof(SAIN);
	*nread = recvfrom(UDPyhteys[(int)hC-1]->socksrv, pbuf, len_t, 0,
	   &UDPyhteys[(int)hC-1]->recvaddr, &frlen);
	if (*nread == SOCKET_ERROR) {
	   err = WSAGetLastError();
	   *nread = 0;
		 if (err != 10004)
			 writesocketerror(1, err, L"receive UDP (server)");
	   WSASetLastError(0);
	   }
	else {
	   addr_in = (SAIN *) &UDPyhteys[(int)hC-1]->recvaddr;
	   if (*nread > 0 &&
		  UDPyhteys[(int)hC-1]->cliaddr.sin_addr.s_addr != addr_in->sin_addr.s_addr) {
		  UDPyhteys[(int)hC-1]->lastmsg = t_time_l(timeticks(), t0);
		  UDPyhteys[(int)hC-1]->cliaddr.sin_addr.s_addr =
			 addr_in->sin_addr.s_addr;
		  }
		}
	return(err);
	}


int read_UDP(HANDLE hC, int len, char *buf, int *nread)
{

   int err = 0, len_t;
   char udpbuf[UDPBUF], *pbuf;

   *nread = 0;
	if ((int)hC < 1)
		return(0);
   if (UDPyhteys[(int)hC-1]->cliaddr.sin_addr.s_addr == INADDR_NONE)
	  return(0);

   if (len < UDPBUF-5) {
	  pbuf = udpbuf;
	  len_t = len + 5;
	  }
   else {
	  pbuf = buf;
	  len_t = len;
	  }

	err = read_UDP_0(hC, len_t, pbuf, nread);
	if (*nread != SOCKET_ERROR) {
	   if (len_t == len+5 && pbuf[0] == STX) {
		  if (*nread > 5)
			 *nread -= 5;
		  else
			 *nread = 0;
		  memcpy(buf, pbuf+5, *nread);
//          UDPyhteys[(int)hC]->last_t = 0;
		  if ((*nread == 1 && buf[0] == NAK) || (*nread > 3 && buf[0] == SOH && buf[1]+buf[2] == 255)) {
//             if (UDPyhteys[(int)hC]->last_id != pbuf[6])
//                UDPyhteys[(int)hC]->last_t = *(long *)(pbuf+1);
//             UDPyhteys[(int)hC]->last_id = pbuf[6];
			 if (*(short *)(pbuf+1)) {
					 UDPyhteys[(int)hC-1]->cliaddr.sin_port = htons(*(short *)(pbuf+1));
					 }
				 strncpy(UDPyhteys[(int)hC-1]->naapuri, pbuf+3, 2);
			 }
		  }
	   else if (pbuf != buf)
		  memcpy(buf, pbuf, *nread);
	   }

	return(err);
}

int read_UDPcli_0(HANDLE hC, int len_t, char *pbuf, int *nread)
{
   int err = 0, n_ready;
   fd_set  rfds;
   struct timeval tv = {0,0};

   *nread = 0;
	if ((int)hC < 1)
		return(0);
   if (UDPyhteys[(int)hC-1]->cliaddr.sin_addr.s_addr == INADDR_NONE)
	  return(0);
	if (UDPcliWait > 1000)
		tv.tv_sec = UDPcliWait/1000;
	else
		tv.tv_usec = UDPcliWait;
//   tv.tv_sec = rtt_viive(UDPyhteys[(int)hC], UDPyhteys[(int)hC]->last_len);
#ifdef DBGRTT
   t_in = mstimer();
#endif
   FD_ZERO(&rfds);
   FD_SET(UDPyhteys[(int)hC-1]->sockcli, &rfds);
   if ((n_ready = select(FD_SETSIZE, &rfds, 0, 0, &tv)) == SOCKET_ERROR) {
	   err = WSAGetLastError();
	   WSASetLastError(0);
	   n_ready = 0;
	   writesocketerror(2, err, L"receive UDP (client/select)");
	   }
   if (n_ready && FD_ISSET(UDPyhteys[(int)hC-1]->sockcli, &rfds)) {
	  *nread = recvfrom(UDPyhteys[(int)hC-1]->sockcli, pbuf, len_t, 0, 0, 0);
	  if (*nread == SOCKET_ERROR) {
		  err = WSAGetLastError();
		  WSASetLastError(0);
		  *nread = 0;
			 writesocketerror(2, err, L"receive UDP (client)");
		  }
	  UDPyhteys[(int)hC-1]->lastmsg = t_time_l(timeticks(), t0);
	  }
   return(err);
}

int read_UDPcli(HANDLE hC, int len, char *buf, int *nread)
{
   int err = 0, len_t;
   char udpbuf[UDPBUF], *pbuf;
#ifdef DBGRTT
   long t_in;
#endif


   *nread = 0;
   if ((int)hC < 1 || UDPyhteys[(int)hC-1]->cliaddr.sin_addr.s_addr == INADDR_NONE)
	  return(0);

   if (len < UDPBUF-4) {
	  pbuf = udpbuf;
	  len_t = len + 4;
	  }
   else {
	  pbuf = buf;
	  len_t = len;
	  }

   err = read_UDPcli_0(hC, len_t, pbuf, nread);
   if (len_t == len+4) {
	  if (*nread > 4)
		 *nread -= 4;
	  else
		 *nread = 0;
#ifdef DBGRTT
	  if (*nread > 3 && pbuf[4] == ACK && pbuf[5]+pbuf[6] == 255)
		 rtt_calc(UDPyhteys[(int)hC-1], mstimer(), *((long *)pbuf), t_in, (int)pbuf[5]);
#else
//      if (*nread > 3 && pbuf[4] == ACK && pbuf[5]+pbuf[6] == 255)
//         rtt_calc(UDPyhteys[(int)hC], mstimer(), *((long *)pbuf));
#endif
		if (*nread) {
			strncpy(UDPyhteys[(int)hC-1]->naapuri, pbuf+2, 2);
			memcpy(buf, pbuf+4, *nread);
			}
	  }
	return(err);
}

int wrt_st_UDP_0(HANDLE hC, int len, char *buf, int *nsent)
{
   int err = 0;

//   if (UDPyhteys[(int)hC]->cliaddr.sin_addr.s_addr == INADDR_NONE ||
//		UDPyhteys[(int)hC]->cliaddr.sin_port == 0) {
	*nsent = 0;
	if ((int)hC < 1)
		return(0);
   if (UDPyhteys[(int)hC-1]->cliaddr.sin_port == 0) {
	 return(0);
	 }
   *nsent = sendto(UDPyhteys[(int)hC-1]->sockcli, buf, len, 0,
	  (SA *)&UDPyhteys[(int)hC-1]->cliaddr, SAINSIZE);
   if (*nsent == SOCKET_ERROR) {
	  err = WSAGetLastError();
	  WSASetLastError(0);
	  *nsent = 0;
	   if (err != 10065)
			writesocketerror(1, err, L"send UDP (client)");
	  }
//   else {
//      UDPyhteys[(int)hC]->last_len = len;
   return(err);
}


int wrt_st_UDP(HANDLE hC, int len, char *buf, int *nsent)
{
   int err = 0;
   char udpbuf[UDPBUF];
#ifdef DBGRTT
   wchar_t msg[120];
#endif

//   if (len > 1 && len < UDPBUF-5) {
	*nsent = 0;
	if ((int)hC < 1)
		return(0);
   if (len < UDPBUF-5) {
	  memcpy(udpbuf+5, buf, len);
	  buf = udpbuf;
	  udpbuf[0] = STX;
//      *((long *)(udpbuf+1)) = mstimer();
		*((short *)(udpbuf+1)) = ntohs(UDPyhteys[(int)hC-1]->servaddr.sin_port);
		memcpy(udpbuf+3, konetn, 2);
	  len += 5;
	 }
   err = wrt_st_UDP_0(hC, len, buf, nsent);

#ifdef DBGRTT
	  if (loki && len > 6) {
		 swprintf(msg, L"%7ld, wrt, L %4d, id %4d",
			*((long *)(udpbuf+1)), len, (int)udpbuf[6]);
		 wkirjloki(msg);
		 }
#endif
//      }
	return(err);
}

int wrt_st_UDPsrv_0(HANDLE hC, int len, char *buf, int *nsent)
{
	int err = 0;

	*nsent = 0;
	if ((int)hC < 1)
		return(0);
	*nsent = sendto(UDPyhteys[(int)hC-1]->socksrv, buf, len, 0,
		  &UDPyhteys[(int)hC-1]->recvaddr, SAINSIZE);
	if (*nsent == SOCKET_ERROR) {
		err = WSAGetLastError();
		WSASetLastError(0);
		*nsent = 0;
		writesocketerror(1, err, L"send UDP (server)");
		}
	return(err);
}


int wrt_st_UDPsrv(HANDLE hC, int len, char *buf, int *nsent)
{
	int err = 0;
	char udpbuf[UDPBUF];

	*nsent = 0;
	if ((int)hC < 1)
		return(0);
	if (len < UDPBUF-4) {
		memcpy(udpbuf+4, buf, len);
		buf = udpbuf;
//        if (buf[4] == ACK && buf[5]+buf[6] == 255 && buf[5] == UDPyhteys[(int)hC]->last_id)
//           *((long *)udpbuf) = UDPyhteys[(int)hC]->last_t;
//        else
//           *((long *)udpbuf) = 0;
		*((short *)udpbuf) = 0;
		  memcpy(udpbuf+2, konetn, 2);
		len += 4;
	   }
	err = wrt_st_UDPsrv_0(hC, len, buf, nsent);

	return(err);
}

int wrt_ch_UDP(HANDLE hC, char ch)
{
	int nsent;

	return(wrt_st_UDP(hC, 1, &ch, &nsent));
}

int wrt_ch_UDPsrv(HANDLE hC, char ch)
{
	int nsent;

	return(wrt_st_UDPsrv(hC, 1, &ch, &nsent));
}

int wrt_st_broadcast(int len, char *buf)
{
   int err = 0;
   int yht, nsent;

   for (yht = 0; yht < nBroadcast; yht++) {
	   if (broadcastyhteys[yht].cliaddr.sin_addr.s_addr == INADDR_NONE ||
			broadcastyhteys[yht].cliaddr.sin_port == 0) {
			continue;
			}
		nsent = sendto(broadcastyhteys[yht].sockcli, buf, len, 0,
			(SA *)&broadcastyhteys[yht].cliaddr, SAINSIZE);
		if (nsent == SOCKET_ERROR) {
			err = WSAGetLastError();
			WSASetLastError(0);
			if (err != 10065) {
				writesocketerror(1, err, L"broadcast");
				break;
				}
			}
		}
   return(err);
}

