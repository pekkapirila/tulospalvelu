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

#ifndef _WINCOM_DEF_
#define _WINCOM_DEF_

#define ipNONE 0
#define ipUDPBOTH 1
#define ipUDPSTREAM 2
#define ipUDPCLIENT 3
#define ipUDPSERVER 4
#define ipTCPCLIENT 11
#define ipTCPSERVER 12

#define comtpRS 0
#define comtpUDP 1
#define comtpUDPSTREAM 5
#define comtpTCP 2
#define comtpTCPSRV 6
#define comtpTCPXML 10
#define comtpTCPSRVXML 22
#define comtpTCPLOKI 14
#define comtpTCPIMPORT 18


typedef void *HANDLE;

typedef struct {
	int iptype;
    UINT16 srvport;
    wchar_t destaddr[64];
    UINT16 destport;
	int noreconnect;
	int status;
    } ipparamtp;

#ifdef  __cplusplus_x
extern "C" {
#endif
int closeport(HANDLE *hC);
int openport(HANDLE *hC, int portti, int baud, char parity, int bits, int sbits, int xonoff);
int setportparam(HANDLE hC, int baud, char parity, int bits, int sbits, int xonoff);
void setcomtimeouts(HANDLE hc, int rdconst, int rdmult, int rdintvl,
					int wrtconst, int wrtmult);
int quesize(HANDLE hC, int *que);
void outquesize(HANDLE hC, int *que);
void o_flush(HANDLE hC);
void i_flush(HANDLE hC);
int read_ch(HANDLE hC, char *ch, int *que);
int wrt_ch(HANDLE hC, char ch);
int read_st(HANDLE hC, int len, char *buf, int *nread, int *que);
int wrt_st(HANDLE hC, int len, char *buf, int *nsent);

UINT32 UDPaddr(HANDLE hC);
UINT16 UDPcliport(HANDLE hC);
char *TCPdestaddr(HANDLE hC);
int TCPdestport(HANDLE hC);
int TCPyht_on(HANDLE hC);
char *UDPnaapuri(HANDLE hC);
void UDPsetkonetunn(char *konetunn);
int closeportUDP(HANDLE *hC);
int openportUDP(HANDLE *hC, ipparamtp *ipparam);
int read_UDP_0(HANDLE hC, int len, char *buf, int *nread);
int read_UDPcli_0(HANDLE hC, int len, char *buf, int *nread);
int wrt_st_UDP_0(HANDLE hC, int len, char *buf, int *nsent);
int wrt_st_UDPsrv_0(HANDLE hC, int len, char *buf, int *nsent);
int read_UDP(HANDLE hC, int len, char *buf, int *nread);
int read_UDPcli(HANDLE hC, int len, char *buf, int *nread);
int UDPstreambuffered(HANDLE hC);
void i_flush_UDPstream(HANDLE hC);
int read_UDPstream(HANDLE hC, int len, char *buf, int *nread);
int peek_UDPstream(HANDLE hC, int len, char *buf, int *nread);
int wrt_st_UDP(HANDLE hC, int len, char *buf, int *nsent);
int wrt_st_UDPsrv(HANDLE hC, int len, char *buf, int *nsent);
int wrt_ch_UDP(HANDLE hC, char ch);
int wrt_ch_UDPsrv(HANDLE hC, char ch);
int UDPcliAllowBroadcast(HANDLE hC);
int UDPsrvAllowBroadcast(HANDLE hC);
int openportBroadcast(ipparamtp *ipparam);
int wrt_st_broadcast(int len, char *buf);
int openportTCP(HANDLE *hC, ipparamtp *ipparam);
int reconnectTCP(HANDLE hC);
int TCPbuffered(HANDLE hC);
void i_flush_TCP(HANDLE hC);
int read_TCP(HANDLE hC, int buflen, char *buf, int *nread);
int peek_TCP(HANDLE hC, int len, char *buf, int *nread);
int wrt_ch_TCP(HANDLE hC, char ch);
int wrt_st_TCP(HANDLE hC, int len, char *buf, int *nsent);
int closeportTCP(HANDLE *hC);
#ifdef EI_OLE
int openportTCPsrv(HANDLE *hC, ipparamtp *ipparam);
int read_TCPsrv(HANDLE hC, int len, char *buf, int *nread);
int wrt_ch_TCPsrv(HANDLE hC, char ch);
int wrt_st_TCPsrv(HANDLE hC, int len, char *buf, int *nsent);
int closeportTCPsrv(HANDLE *hC);
#endif
void cleanWSA(void);
void UDPsetCliWait(int wait);

#ifdef  __cplusplus_x
}
#endif
#endif  // _WINCOM_DEF_
