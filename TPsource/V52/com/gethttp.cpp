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

#define _WININET_
#define WininetHPP
#include <Windows.h>
#include <WinHTTP.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <process.h>
// #include <tputil.h>

int getwebPage(HINTERNET hSession, LPCWSTR host, int port, LPCWSTR page, int secure, char *buf, int buflen, FILE *outfile, int *dwTot)
{
	DWORD dwSize = 0;
	DWORD dwDownloaded = 0;
	LPSTR pszOutBuffer = 0, pbuf;
	int  bResults = 0;
	HINTERNET  hConnect = NULL,
				 hRequest = NULL;

	if (!buf && !outfile)
		return(FALSE);

	pbuf = buf;
	*dwTot = 0;

	// Specify an HTTP server.
	hConnect = WinHttpConnect( hSession, host, port, 0 );

	// Create an HTTP request handle.
	if( hConnect )
		hRequest = WinHttpOpenRequest( hConnect, L"GET",
								   page, NULL, WINHTTP_NO_REFERER,
								   WINHTTP_DEFAULT_ACCEPT_TYPES,
								   secure ? WINHTTP_FLAG_SECURE : 0);

	// Send a request.
	if( hRequest )
		bResults = !WinHttpSendRequest( hRequest,
								   WINHTTP_NO_ADDITIONAL_HEADERS, 0,
								   WINHTTP_NO_REQUEST_DATA, 0,
								   0, 0 );

	// End the request.
	if( !bResults )
		bResults = !WinHttpReceiveResponse( hRequest, NULL );

	// Keep checking for data until there is nothing left.
	if (!bResults)
		{
		do
			{
	  // Check for available data.
			dwSize = 0;
			if (!WinHttpQueryDataAvailable( hRequest, &dwSize )) {
				bResults = -1;
				break;
				}
		  // Allocate space for the buffer.
		  if (dwSize)
			   pszOutBuffer = (char *)malloc(dwSize+1);
		  if( !pszOutBuffer )
				{
				if (dwSize)
					bResults = -3;
				dwSize=0;
				}
		  else
				{
				if (dwSize)
					{
			  // Read the data.
					ZeroMemory( pszOutBuffer, dwSize+1 );

					if(!WinHttpReadData(hRequest, (LPVOID)pszOutBuffer,
							dwSize, &dwDownloaded)) {
						free(pszOutBuffer);
						bResults = -2;
						break;
						}
					if (buf) {
						if (*dwTot+dwSize < (unsigned int) buflen) {
							memcpy(pbuf, pszOutBuffer, dwSize);
							pbuf += dwSize;
							*dwTot += dwSize;
							}
						}
					else {
						fputs( pszOutBuffer, outfile);
						*dwTot += dwSize;
						}

			// Free the memory allocated to the buffer.
					free(pszOutBuffer);
					}
				}
			} while( dwSize > 0 );
		}
	if (hRequest) 
		WinHttpCloseHandle(hRequest);
	else 
		bResults = -4;
	if (hConnect) 
		WinHttpCloseHandle(hConnect);
	else 
		bResults = -5;
	return(bResults);
}

int httphaku(wchar_t *host, int port, wchar_t *page, int secure, char *buf, int buflen, int *haettu)
{
  int bResults = 0;
  HINTERNET  hSession = NULL;
  char tmpflnm[60] = "temp.txt";
  FILE *tmpfile = NULL;
  int er = 0;

  // Use WinHttpOpen to obtain a session handle.
  hSession = WinHttpOpen( L"WinHTTP Example/1.0",
								  WINHTTP_ACCESS_TYPE_DEFAULT_PROXY,
								  WINHTTP_NO_PROXY_NAME,
								  WINHTTP_NO_PROXY_BYPASS, 0 );

  if ( hSession )
  {
	memset(buf, 0, buflen);

	// Read the data.

	 bResults = getwebPage(hSession, host, port, page, secure, buf, buflen, tmpfile, haettu);
		  // Free the memory allocated to the buffer.

	if (bResults > -3 && bResults != 0)
		er = GetLastError( );
	else
		er = bResults;
	// Close any open handles.
	if (hSession) WinHttpCloseHandle( hSession );
  }
  else
	  er = -6;
  return(er);
  }

//  = -6: Istunnon avaaminen ei onnistunut
// bResult = -5: Yhteyden avaaminen ei onnistunut
// bResult = -4: Pyynnön lähettäminen ei onnistunut
// bResult = -3: Muisti loppui
// bResult = -2: Virhe tietoa luettaessa
// bResult = -1: Virhe tiedon saatavuutta kysyttäessä
