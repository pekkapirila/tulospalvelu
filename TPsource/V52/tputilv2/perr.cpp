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

#include <windows.h>
#include <stdio.h>
#include <tputil.h>

extern void lopetus(void);

/* maximum size of the buffer to be returned from FormatMessage */
#define MAX_MSG_BUF_SIZE 512

void perr(char *szFileName, int line, char *szApiName, unsigned long dwError)
{
  char szTemp[1024];
  DWORD cMsgLen;
  char *msgBuf; /* buffer for message text from system */
  int iButtonPressed; /* receives button pressed in the error box */

  /* format our error message */
  sprintf(szTemp, "%s: Error %d from %s on line %d:\n", szFileName,
      dwError, szApiName, line);
  /* get the text description for that error number from the system */
  cMsgLen = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
      FORMAT_MESSAGE_ALLOCATE_BUFFER | 40, NULL, dwError,
      MAKELANGID(0, SUBLANG_ENGLISH_US), (LPTSTR) &msgBuf, MAX_MSG_BUF_SIZE,
      NULL);
  if (!cMsgLen)
    sprintf(szTemp + strlen(szTemp), "Unable to obtain error message text! \n"
        "%s: Error %d from %s on line %d", __FILE__,
        GetLastError(), "FormatMessage", __LINE__);
  else
    strcat(szTemp, msgBuf);
  strcat(szTemp, "\n\nContinue execution?");
  MessageBeep(MB_ICONEXCLAMATION);
  iButtonPressed = MessageBoxA(NULL, szTemp, "Console API Error",
      MB_ICONEXCLAMATION | MB_YESNO | MB_SETFOREGROUND);
  /* free the message buffer returned to us by the system */
  if (cMsgLen)
	LocalFree((HLOCAL) msgBuf);
  if (iButtonPressed == IDNO)
//    exit(1);
	lopetus();
  return;
}

void wperr(LPWSTR szFileName, int line, LPWSTR szApiName, DWORD dwError)
{
  wchar_t szTemp[1024];
  DWORD cMsgLen;
  wchar_t *msgBuf; /* buffer for message text from system */
  int iButtonPressed; /* receives button pressed in the error box */

  /* format our error message */
  swprintf(szTemp, L"%s: Error %d from %s on line %d:\n", szFileName,
	  dwError, szApiName, line);
  /* get the text description for that error number from the system */
  cMsgLen = FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM |
	  FORMAT_MESSAGE_ALLOCATE_BUFFER | 40, NULL, dwError,
	  MAKELANGID(0, SUBLANG_ENGLISH_US), (LPTSTR) &msgBuf, MAX_MSG_BUF_SIZE,
	  NULL);
  if (!cMsgLen)
	swprintf(szTemp + wcslen(szTemp), L"Unable to obtain error message text! \n"
		L"%s: Error %d from %s on line %d", __FILE__,
		GetLastError(), L"FormatMessage", __LINE__);
  else
	wcscat(szTemp, msgBuf);
  wcscat(szTemp, L"\n\nContinue execution?");
  MessageBeep(MB_ICONEXCLAMATION);
  iButtonPressed = MessageBoxW(NULL, szTemp, L"Console API Error",
	  MB_ICONEXCLAMATION | MB_YESNO | MB_SETFOREGROUND);
  /* free the message buffer returned to us by the system */
  if (cMsgLen)
	LocalFree((HLOCAL) msgBuf);
  if (iButtonPressed == IDNO)
//    exit(1);
	lopetus();
  return;
}
