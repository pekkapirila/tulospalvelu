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
#include <windows.h>
#include <tputil.h>
#include <wincom.h>

#define INQEMPTY 10

int baudRate[12] = {    75,  150,  300,   600,
					  1200, 2400, 4800,  9600,
					 19200,38400,57600,115200};

COMMTIMEOUTS timeouts0;

static int inRead(HANDLE hc, int in)
{
   static HANDLE rds[21];
   int i;

   for (i = 0; i < 20; i++)
      if (hc == rds[i] || rds[i] == 0) break;
   if (rds[i] == 0) {
      if (!in || i >= 20)
         return(1);
      rds[i] = hc;
      return(0);
      }
   if (!in && i < 20) {
      memmove(rds+i, rds+i+1, (20 - i)*sizeof(HANDLE));
      return(0);
      }
   return(1);
}

int closeport(HANDLE *hC)
{
	if (*hC) {
		SetCommTimeouts(*hC, &timeouts0);
		try {
			CloseHandle(*hC);
		} catch (...) {
		}
	}
	*hC = 0;
	return(0);
}

int setportparam(HANDLE hC, int baud, char parity, int bits, int sbits, int xonoff)
   {
   DWORD  er;
   wchar_t commStr[20], prs[82];
   DCB dcb;
   COMSTAT CommStat;
   COMMTIMEOUTS timeouts;

   FillMemory(&dcb, sizeof(dcb), 0);
   dcb.DCBlength = sizeof(dcb);
   swprintf(commStr, L"%d,%c,%d,%d",
	   baudRate[baud],
	   parity,			/* no parity */
	   bits,			/* data bits */
       sbits);			/* stop bits */
   if (!BuildCommDCBW(commStr, &dcb)) {
	   swprintf(prs, L"BuildComm(%s) ei onnistu", commStr);
       writeerror_w(prs,0);
       return(1);
   }
   dcb.fOutxDsrFlow = FALSE;	  /* do not require DSR to transmit */
   dcb.fOutxCtsFlow = FALSE;	  /* do not require CTS to transmit */
   dcb.fRtsControl = RTS_CONTROL_ENABLE;
   dcb.fDtrControl = DTR_CONTROL_ENABLE;
   if (xonoff) {
      dcb.fInX = TRUE;
	  dcb.fOutX = TRUE;
      }
   if (!SetCommState(hC, &dcb)) {
	   swprintf(prs, L"SetCommState ei onnistu");
       writeerror_w(prs,0);
       return(1);
   }
   GetCommTimeouts(hC, &timeouts);
   timeouts0 = timeouts;
   timeouts.ReadIntervalTimeout = MAXDWORD;
   timeouts.ReadTotalTimeoutMultiplier = 0;
   timeouts.ReadTotalTimeoutConstant = 0;
   timeouts.WriteTotalTimeoutMultiplier = 0;
   timeouts.WriteTotalTimeoutConstant = 0;
   SetCommTimeouts(hC, &timeouts);
   ClearCommError(hC, &er, &CommStat);
   return(0);
   }

int openport(HANDLE *hC, int portti, int baud, char parity, int bits, int sbits, int xonoff)
   {
   int retcode;
   wchar_t gszPort[20];
   wchar_t prs[82];

   swprintf(gszPort, L"\\\\.\\COM%d", portti);
   *hC = CreateFileW(	gszPort,
					GENERIC_READ | GENERIC_WRITE,
					0,
					0,
					OPEN_EXISTING,
					0,
					0);
   if (*hC == INVALID_HANDLE_VALUE) {
      swprintf(prs,L"Portin COM%d avaaminen ei onnistu", portti);
      writeerror_w(prs,0);
      return(1);
	  }
   retcode = setportparam(*hC, baud, parity, bits, sbits, xonoff);
   return(retcode);
   }

void setcomtimeouts(HANDLE hC, int rdconst, int rdmult, int rdintvl,
					int wrtconst, int wrtmult)
{
   COMMTIMEOUTS timeouts;

   timeouts.ReadIntervalTimeout = rdintvl;
   timeouts.ReadTotalTimeoutMultiplier = rdmult;
   timeouts.ReadTotalTimeoutConstant = rdconst;
   timeouts.WriteTotalTimeoutMultiplier = wrtmult;
   timeouts.WriteTotalTimeoutConstant = wrtconst;
   SetCommTimeouts(hC, &timeouts);
}

int quesize(HANDLE hC, int *que)
   {
   DWORD  er;
   COMSTAT CommStat;
   int ret_code = 0;

   *que = 0;
   if (!ClearCommError(hC, &er, &CommStat))
      return(32);
   if (er)
      ret_code = er+16;
   *que = CommStat.cbInQue;
   return(ret_code);
   }

void outquesize(HANDLE hC, int *que)
   {
   DWORD  er;
   COMSTAT CommStat;

   ClearCommError(hC, &er, &CommStat);
   *que = CommStat.cbOutQue;
   }

void o_flush(HANDLE hC)
{
	DWORD  er;
	COMSTAT CommStat;

	ClearCommError(hC, &er, &CommStat);
	PurgeComm(hC, PURGE_TXCLEAR);
	ClearCommError(hC, &er, &CommStat);
}

void i_flush(HANDLE hC)
{
	DWORD  er;
	COMSTAT CommStat;

	ClearCommError(hC, &er, &CommStat);
	PurgeComm(hC, PURGE_RXCLEAR);
	ClearCommError(hC, &er, &CommStat);
}

int read_ch(HANDLE hC, char *ch, int *que)
{
	DWORD  dwRead;
   DWORD er;

   if (inRead(hC, 1)) {
      *que = 0;
      *ch = 0;
      return(65534);
      }
	ReadFile(hC, ch, 1, &dwRead, NULL);
   er = quesize(hC, que);
   if (!er && !dwRead)
      er = INQEMPTY;
   inRead(hC, 0);
	return(er);
}

int wrt_ch(HANDLE hC, char ch)
{
	DWORD  dwWritten;
	DWORD  er;
	COMSTAT CommStat;

	WriteFile(hC, &ch, 1, &dwWritten, NULL);
	ClearCommError(hC, &er, &CommStat);
	return(er);
}

int read_st(HANDLE hC, int len, char *buf, int *nread, int *que)
{
	DWORD  dwRead;
   DWORD er;

   if (inRead(hC, 1)) {
      *que = 0;
      *nread = 0;
      return(65534);
      }
	ReadFile(hC, buf, len, &dwRead, NULL);
	*nread = (int) dwRead;
   er = quesize(hC, que);
   inRead(hC, 0);
	return(er);
}

int wrt_st(HANDLE hC, int len, char *buf, int *nsent)
{
	DWORD  dwWritten;
	DWORD  er;
	COMSTAT CommStat;

	WriteFile(hC, buf, len, &dwWritten, NULL);
	*nsent = dwWritten;
	ClearCommError(hC, &er, &CommStat);
	return(er);
}