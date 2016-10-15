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

#ifdef _BORLAND_
#include <vcl.h>
#endif
#include <map>
#include <stdio.h>
#include <algorithm>
#include <functional>
 using namespace std;
#include <stdlib.h>
#include <time.h>
#include <io.h>
#include <fcntl.h>
#include <string>
#include <process.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <bstrings.h>
#pragma hdrstop
#include <sys/stat.h>
#include <dos.h>

#ifdef MAXOSUUSLUKU
#include "VDeclare.h"
#else
#include "HkDeclare.h"
#endif
#include "BuildNo.h"

extern GDIfontTp DefFnt;

void rem_pr(void);

wchar_t *Build(void)
{
	static wchar_t st[30];

	wcsncpy(st, BUILD, 29);
	return(st);
}

wchar_t *VersioNo(void)
{
	static wchar_t st[20];

	wcsncpy(st, VERSIOKDI, 19);
	return(st);
}

int VersioInt(void)
{
	return((int)(100*_wtof(VERSIOKDI)));
}

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS) (HANDLE, PBOOL);

void getWinVersion(void)
{
	BOOL bIsWow64 = FALSE;
	LPFN_ISWOW64PROCESS fnIsWow64Process;
	OSVERSIONINFOEX osvi;
//	BOOL bIsWindowsXPorLater;

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);

	GetVersionEx((OSVERSIONINFO *)&osvi);
	EnvData.WinMajorVersion = osvi.dwMajorVersion;
	EnvData.WinMinorVersion = osvi.dwMinorVersion;
	wcsncpy(EnvData.CSDVersion, osvi.szCSDVersion, sizeof(EnvData.CSDVersion)/2-1);
	EnvData.isWinServer = osvi.wProductType > 1;
	fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(
		GetModuleHandle(TEXT("kernel32")),"IsWow64Process");

	if(NULL != fnIsWow64Process) {
		if (!fnIsWow64Process(GetCurrentProcess(),&bIsWow64)) {
			//handle error
			}
		}
	EnvData.isWow64_32 = bIsWow64 != 0;

	if (DeveloperMode && loki) {
		wchar_t msg[400] = L"";
		swprintf(msg, L"Windows Version %d.%d %s, Wow64Process: %s, Server: %s",
			EnvData.WinMajorVersion, EnvData.WinMinorVersion, EnvData.CSDVersion,
			EnvData.isWow64_32 ? L"Yes" : L"No", EnvData.isWinServer ? L"Yes" : L"No");
		wkirjloki(msg);
		}
}

int tm_date(int t)
{
	time_t tt;
	int date, t1;

	tt = time(0);
	date = (int)(tt/86400);
	t1 = (int)(tt - 86400LL*date - 3600 * t0) * SEK;
	t = purajak(t);
	if (t1 - t > 12*TUNTI)
		date++;
	else if (t1 - t < -12*TUNTI)
		date--;
	return(date);
}

int tm_copydate(int ut, int vt, int vdate)
{
	if (vt == AIKAJAK*TMAALI0)
		return(tm_date(ut));
	if (NORMKELLO_A(ut) - NORMKELLO_A(vt) > 12*AIKAJAK*TUNTI)
		vdate--;
	else if (NORMKELLO_A(ut) - NORMKELLO_A(vt) < -12*AIKAJAK*TUNTI)
		vdate++;
	return(vdate);
}

wchar_t *time_t_date(time_t t, wchar_t *dstr)
{
	struct tm tm_t;
	static wchar_t str[12];

	if (dstr == NULL)
		dstr = str;
	t *= 86400;
	tm_t = *gmtime(&t);
	swprintf(dstr, L"%4d-%02d-%02d", tm_t.tm_year+1900, tm_t.tm_mon+1, tm_t.tm_mday);
	return(dstr);
}

time_t date_time_t(wchar_t *st)
{
	struct tm tm_t;

	memset(&tm_t, 0, sizeof(tm_t));
	swscanf(st, L"%d-%d-%d", &tm_t.tm_year, &tm_t.tm_mon, &tm_t.tm_mday);
	if (tm_t.tm_year >= 1900)
		tm_t.tm_year -= 1900;
	else
		tm_t.tm_year = 0;
	if (tm_t.tm_mon > 0)
		tm_t.tm_mon--;
	else
		tm_t.tm_mon = 0;
	if (tm_t.tm_year > 200 || tm_t.tm_mon > 11 || tm_t.tm_mday < 1 || tm_t.tm_mday > 31)
		return(0);
	tm_t.tm_hour = 12;
	return(mktime(&tm_t)/86400);
}

int wcstomaksu(wchar_t *st)
{
	wchar_t *p;

	if (!st || !*st)
		return(0);
	if ((p = wcsstr(st, L",")) != NULL )
		*p = L'.';
	return((int)(100*_wtof(st)+0.1));
}

INT16 i16swab(char *s)
{
	short k;
	_swab(s, (char *)&k, 2);
	return(k);
}

wchar_t *trimstrlen(wchar_t *str, int len)
{
	str[len] = 0;
	for (int i = wcslen(str); i < len; i++)
		str[i] = L' ';
	return(str);
}
void teeBackupkopio(wchar_t *filename)
{
	wchar_t fname[300];
	FILE *fl;

	for (int i = 1; i < 200; i++) {
		swprintf(fname, L"%.295s.%d", filename, i);
		if ((fl = _wfopen(fname, L"rb")) == NULL) {
			fclose(fl);
			_wrename(filename, fname);
			break;
			}
		fclose(fl);
		}
}

bool mahtuusivulle(int l, int vm, tulostusparamtp *tulprm)
{
	int vpit;

	if (!tulprm->lstf)
		return(false);
	if (tulprm->lstf->prno != 2)
		return (l < tulprm->tulmuot.sivpit-vm);
	vpit = tulprm->lstf->u.wp.GDIparam.Currentfont.line;
	if (vm > 1) {
		vpit += (vm-1)*DefFnt.line;
		}
	return(tulprm->vpos + vpit <= tulprm->lstf->u.wp.yPage * 72 *
		(960 - tulprm->tulmuot.amarg) / 1000 / tulprm->lstf->u.wp.yppi);
}

bool tulostusparamtp::mahtuusivulle(int l, int vm)
{
	int vpit;

	if (!lstf)
		return(false);
	if (lstf->prno != 2)
		return (l < tulmuot.sivpit-vm);
	vpit = lstf->u.wp.GDIparam.Currentfont.line;
	if (vm > 1) {
		vpit += (vm-1)*DefFnt.line;
		}
	return(vpos + vpit <= lstf->u.wp.yPage * 72 * (960 - tulmuot.amarg) /
		1000 / lstf->u.wp.yppi);
}


/*
wchar_t *utf8towcs(wchar_t *wst, char *st, int len)
{
#ifdef _BORLAND_
	Utf8ToUnicode(wst, st, len);
#else
	MultiByteToWideChar(CP_UTF8, 0, st, -1, wst, len);
#endif
	return(wst);
}

char *wcstoutf8(char *st, wchar_t *wst, int len)
{
#ifdef _BORLAND_
	UnicodeToUtf8(st, wst, len);
#else
	WideCharToMultiByte(CP_UTF8, 0, wst, -1, st, len, 0, 0);
#endif
	return(st);
}
*/

char *keyfromU16name(char *key, wchar_t *name, int len, int flags)
{
	wchar_t *p, Buf[102];
	char *kp;

	memset(key, 0, len);
	wcsncpy(Buf, name, 100);
	Buf[len] = 0;
	upcasewstr(Buf);
	for (p = Buf, kp = key; *p; p++, kp++) {
		switch (*p) {
			case L'|':
				*kp = '\t';
				break;
			case L'_':
				*kp = ' ';
				break;
			case L'Å':
				*kp = 91;
				break;
			case L'Ä':
			case L'Æ':
			case L'æ':
				*kp = 92;
				break;
			case L'Ö':
			case L'ø':
			case L'Ø':
				*kp = 93;
				break;
			case L'Ü':
				*kp = 'Y';
				break;
			case L'É':
			case L'È':
			case L'Ê':
			case L'Ë':
			case L'ë':
			case L'ê':
			case L'é':
			case L'è':
				*kp = 'E';
				break;
			case L'Á':
			case L'À':
			case L'Â':
			case L'Ã':
				*kp = 'A';
				break;
			case L'W':
				if (kilpparam.WisV)
					*kp = 'V';
				break;
			default:
				if (*p > 255)
					*kp = 255;
				else
					*kp = (char) *p;
			}
		}
	return(key);
}

char *keyfromU8name(char *key, char *name, int len, int flags)
{
	wchar_t Buf[102];

	MbsToWcs(Buf, name, 100);
	Buf[100] = 0;
	return(keyfromU16name(key, Buf, len, flags));
}

wchar_t *wcstokch(wchar_t *st, wchar_t erotin)
   {
   static wchar_t *p2;
   wchar_t *p;

   if (st) {
	  p2 = st;
	  }
   if (!p2 || !*p2)
	  return(0);
   p = p2;
   while (*p2 && *p2 != erotin)
	  p2++;
   if (*p2) {
	  *p2 = 0;
	  p2++;
	  }
   return(p);
   }

int Nyt(void)
{
	return(KSEK*t_time_l(biostime(0,0), t0));
}

INT32 normkello_a(INT32 t) {
	if (t == AIKAJAK * TMAALI0)
		return(t);
	while (t > 12L*3600*10000)
		t -= 24L*3600*10000;
	while (t <= -12*3600*10000)
		t += 24L*3600*10000;
	return(t);
}

INT32 normtulos_a(INT32 t) {
	while (t >= 24L*3600*10000)
		t -= 24L*3600*10000;
	while (t < 0)
		t += 24L*3600*10000;
	return(t);
}

wchar_t *aikatowstr_cols_n(wchar_t *as, INT32 tl, int t0, wchar_t sep, int len)
{
	static wchar_t as0[20];

	if (as == NULL)
		as = as0;
	AIKATOWSTRS(as, tl, t0);
	as[len] = 0;
	for (wchar_t *p = as; *p; p++) {
		if (*p == L'.')
			*p = L':';
		if (sep && *p == L',')
			*p = sep;
		}
	return(as);
}

char *aikatostr_cols_n(char *as, INT32 tl, int t0, char sep, int len)
{
	static char as0[20];

	if (as == NULL)
		as = as0;
	AIKATOSTRS(as, tl, t0);
	as[len] = 0;
	for (char *p = as; *p; p++) {
		if (*p == '.')
			*p = ':';
		if (sep && *p == ',')
			*p = sep;
		}
	return(as);
}

wchar_t *eroaikatowstr(wchar_t *as, int ero, int len)
{
	static wchar_t as0[20];

	if (as == NULL)
		as = as0;
	if (len < 8)
		len = 8;
	if (ero == 0) {
		wcscpy(as, L"0,00");
		as[len-7] = 0;
		}
	else {
		aikatowstr_cols_n(as, abs(ero), 0, 0, len);
		elimwzb1(as);
		wmemmove(as+1, as, wcslen(as)+1);
		as[0] = (ero < 0) ? L'-' : L'+';
		}
	return(as);
}

char *eroaikatostr(char *as, int ero, int len)
{
	wchar_t ws[20];
	static char as0[20];

	if (as == NULL)
		as = as0;
	wcstoansi(as, eroaikatowstr(ws, ero, len), len);
	return(as);
}

INT32 strtoaika_vap(char *str, int t0)
{
	wchar_t s[20];

	return(wstrtoaika_vap(ansitowcs(s, str, 19), t0));
}

INT32 wstrtoaika_vap(wchar_t *str, int t0)
{
	wchar_t s[20] = L"", *p, *p1;
	INT32 t = 0;

	for (p = str; *p; p++) {
		if (*p >= L'0' && *p <= L'9')
			break;
		}
	if (*p == 0)
		return(t0 ? TMAALI0 : 0);

	wcsncpy(s, str, 16);
	if (wcsstr(s, L":") != NULL) {
		if ((p = wcsstr(s, L".")) != NULL)
			*p = L',';
		for (p = s; *p; p++)
			if (*p == L':')
				*p = L'.';
		}
	p1 = s + wcslen(s);
	if ((p = wcsstr(s, L",")) != NULL) {
		p1 = p;
		wcscat(s, L"000");
		p[4] = 0;
#if (KSEK == 10)
		p[3] = 0;
#endif
		t = _wtol(p+1);
		}
	*p1 = 0;
	for (p = p1-1; p >= s && *p != L'.'; p--);
	if (p < p1-1)
		t += SEK*_wtol(p+1);
	p1 = p;
	if (p1 > s) {
		*p1 = 0;
		for (p = p1-1; p >= s && *p != L'.'; p--);
		if (p < p1-1)
			t += MINUUTTI*_wtol(p+1);
		p1 = p;
		if (p1 > s) {
			*p1 = 0;
			for (p = p1-1; p >= s && *p != L'.'; p--);
			if (p < p1-1)
				t += TUNTI*_wtol(p+1);
			}
		}
	return(t-t0*TUNTI);
}

static wchar_t *maalista[][2] = {
	{L"AFG", L"Afghanistan"},
	{L"ALB", L"Albania"},
	{L"ALG", L"Algeria"},
	{L"AND", L"Andorra"},
	{L"ANG", L"Angola"},
	{L"ANT", L"Antigua and Barbuda"},
	{L"ARG", L"Argentina"},
	{L"ARM", L"Armenia"},
	{L"ARU", L"Aruba"},
	{L"ASA", L"American Samoa"},
	{L"AUS", L"Australia"},
	{L"AUT", L"Austria"},
	{L"AZE", L"Azerbaijan"},
	{L"BAH", L"Bahamas"},
	{L"BAN", L"Bangladesh"},
	{L"BAR", L"Barbados"},
	{L"BDI", L"Burundi"},
	{L"BEL", L"Belgium"},
	{L"BEN", L"Benin"},
	{L"BER", L"Bermuda"},
	{L"BHU", L"Bhutan"},
	{L"BIZ", L"Belize"},
	{L"BLR", L"Belarus"},
	{L"BOL", L"Bolivia"},
	{L"BOT", L"Botswana"},
	{L"BRA", L"Brazil"},
	{L"BRN", L"Bahrain"},
	{L"BRU", L"Brunei"},
	{L"BUL", L"Bulgaria"},
	{L"BUR", L"Burkina Faso"},
	{L"CAF", L"Central African Republic"},
	{L"CAM", L"Cambodia"},
	{L"CAN", L"Canada"},
	{L"CAY", L"Cayman Islands"},
	{L"CGO", L"Congo"},
	{L"CHA", L"Chad"},
	{L"CHI", L"Chile"},
	{L"CHN", L"China"},
	{L"CIV", L"Ivory Coast"},
	{L"CMR", L"Cameroon"},
	{L"COD", L"DR Congo"},
	{L"COK", L"Cook Islands"},
	{L"COL", L"Colombia"},
	{L"COM", L"Comoros"},
	{L"CPV", L"Cape Verde"},
	{L"CRC", L"Costa Rica"},
	{L"CRO", L"Croatia"},
	{L"CUB", L"Cuba"},
	{L"CYP", L"Cyprus"},
	{L"CZE", L"Czech Republic"},
	{L"DEN", L"Denmark"},
	{L"DJI", L"Djibouti"},
	{L"DMA", L"Dominica"},
	{L"DOM", L"Dominican Republic"},
	{L"ECU", L"Ecuador"},
	{L"EGY", L"Egypt"},
	{L"ERI", L"Eritrea"},
	{L"ESA", L"El Salvador"},
	{L"ESP", L"Spain"},
	{L"EST", L"Estonia"},
	{L"ETH", L"Ethiopia"},
	{L"FIJ", L"Fiji"},
	{L"FIN", L"Finland"},
	{L"FRA", L"France"},
	{L"FSM", L"Federated States of Micronesia"},
	{L"GAB", L"Gabon"},
	{L"GAM", L"Gambia"},
	{L"GBR", L"Great Britain"},
	{L"GBS", L"Guinea-Bissau"},
	{L"GEO", L"Georgia"},
	{L"GEQ", L"Equatorial Guinea"},
	{L"GER", L"Germany"},
	{L"GHA", L"Ghana"},
	{L"GRE", L"Greece"},
	{L"GRN", L"Grenada"},
	{L"GUA", L"Guatemala"},
	{L"GUI", L"Guinea"},
	{L"GUM", L"Guam"},
	{L"GUY", L"Guyana"},
	{L"HAI", L"Haiti"},
	{L"HKG", L"Hong Kong"},
	{L"HON", L"Honduras"},
	{L"HUN", L"Hungary"},
	{L"INA", L"Indonesia"},
	{L"IND", L"India"},
	{L"IRI", L"Iran"},
	{L"IRL", L"Ireland"},
	{L"IRQ", L"Iraq"},
	{L"ISL", L"Iceland"},
	{L"ISR", L"Israel"},
	{L"ISV", L"Virgin Islands"},
	{L"ITA", L"Italy"},
	{L"IVB", L"British Virgin Islands"},
	{L"JAM", L"Jamaica"},
	{L"JOR", L"Jordan"},
	{L"JPN", L"Japan"},
	{L"KAZ", L"Kazakhstan"},
	{L"KEN", L"Kenya"},
	{L"KGZ", L"Kyrgyzstan"},
	{L"KIR", L"Kiribati"},
	{L"KOR", L"South Korea"},
	{L"KSA", L"Saudi Arabia"},
	{L"KUW", L"Kuwait"},
	{L"LAO", L"Laos"},
	{L"LAT", L"Latvia"},
	{L"LBA", L"Libya"},
	{L"LBR", L"Liberia"},
	{L"LCA", L"Saint Lucia"},
	{L"LES", L"Lesotho"},
	{L"LIB", L"Lebanon"},
	{L"LIE", L"Liechtenstein"},
	{L"LTU", L"Lithuania"},
	{L"LUX", L"Luxembourg"},
	{L"MAD", L"Madagascar"},
	{L"MAR", L"Morocco"},
	{L"MAS", L"Malaysia"},
	{L"MAW", L"Malawi"},
	{L"MDA", L"Moldova"},
	{L"MDV", L"Maldives"},
	{L"MEX", L"Mexico"},
	{L"MGL", L"Mongolia"},
	{L"MHL", L"Marshall Islands"},
	{L"MKD", L"Macedonia"},
	{L"MLI", L"Mali"},
	{L"MLT", L"Malta"},
	{L"MNE", L"Montenegro"},
	{L"MON", L"Monaco"},
	{L"MOZ", L"Mozambique"},
	{L"MRI", L"Mauritius"},
	{L"MTN", L"Mauritania"},
	{L"MYA", L"Myanmar"},
	{L"NAM", L"Namibia"},
	{L"NCA", L"Nicaragua"},
	{L"NED", L"Netherlands"},
	{L"NEP", L"Nepal"},
	{L"NGR", L"Nigeria"},
	{L"NIG", L"Niger"},
	{L"NOR", L"Norway"},
	{L"NRU", L"Nauru"},
	{L"NZL", L"New Zealand"},
	{L"OMA", L"Oman"},
	{L"PAK", L"Pakistan"},
	{L"PAN", L"Panama"},
	{L"PAR", L"Paraguay"},
	{L"PER", L"Peru"},
	{L"PHI", L"Philippines"},
	{L"PLE", L"Palestine"},
	{L"PLW", L"Palau"},
	{L"PNG", L"Papua New Guinea"},
	{L"POL", L"Poland"},
	{L"POR", L"Portugal"},
	{L"PRK", L"North Korea"},
	{L"PUR", L"Puerto Rico"},
	{L"QAT", L"Qatar"},
	{L"ROU", L"Romania"},
	{L"RSA", L"South Africa"},
	{L"RUS", L"Russia"},
	{L"RWA", L"Rwanda"},
	{L"SAM", L"Samoa"},
	{L"SEN", L"Senegal"},
	{L"SEY", L"Seychelles"},
	{L"SIN", L"Singapore"},
	{L"SKN", L"Saint Kitts and Nevis"},
	{L"SLE", L"Sierra Leone"},
	{L"SLO", L"Slovenia"},
	{L"SMR", L"San Marino"},
	{L"SOL", L"Solomon Islands"},
	{L"SOM", L"Somalia"},
	{L"SRB", L"Serbia"},
	{L"SRI", L"Sri Lanka"},
	{L"STP", L"São Tomé and Príncipe"},
	{L"SUD", L"Sudan"},
	{L"SUI", L"Switzerland"},
	{L"SUR", L"Suriname"},
	{L"SWE", L"Sweden"},
	{L"SVK", L"Slovakia"},
	{L"SWZ", L"Swaziland"},
	{L"SYR", L"Syria"},
	{L"TAN", L"Tanzania"},
	{L"TGA", L"Tonga"},
	{L"THA", L"Thailand"},
	{L"TJK", L"Tajikistan"},
	{L"TKM", L"Turkmenistan"},
	{L"TLS", L"Timor-Leste"},
	{L"TOG", L"Togo"},
	{L"TPE", L"Chinese Taipei"},
	{L"TRI", L"Trinidad and Tobago"},
	{L"TUN", L"Tunisia"},
	{L"TUR", L"Turkey"},
	{L"TUV", L"Tuvalu"},
	{L"UAE", L"United Arab Emirates"},
	{L"UGA", L"Uganda"},
	{L"UKR", L"Ukraine"},
	{L"URU", L"Uruguay"},
	{L"USA", L"United States"},
	{L"UZB", L"Uzbekistan"},
	{L"VAN", L"Vanuatu"},
	{L"VEN", L"Venezuela"},
	{L"VIE", L"Vietnam"},
	{L"VIN", L"Saint Vincent and the Grenadines"},
	{L"YEM", L"Yemen"},
	{L"ZAM", L"Zambia"},
	{L"ZIM", L"Zimbabwe"}
	};

wchar_t *MaaNimi(wchar_t *maa)
{
	static wchar_t tyhja[2] = L"";

	for (int i = 0; i < (int)(sizeof(maalista)/sizeof(maalista[0])); i++)
		if (!wcscmp(maa, maalista[i][0]))
			return(maalista[i][1]);
	return(tyhja);
}

wchar_t *ISOtime(wchar_t *st, wchar_t *datestr, int time)
{
	static wchar_t buf[40];

	if (!st)
		st = buf;
	st[0] = 0;
	if (datestr) {
		wcscpy(st, datestr);
		wcscat(st, L"T");
		}
#ifndef MAXOSUUSLUKU
	aikatowstr_cols_n(st+wcslen(st), time, t0, L'.', pvparam[k_pv].laika2);
#else
	aikatowstr_cols_n(st+wcslen(st), time, t0, L'.', kilpparam.laika2);
#endif
	return(st);
}

wchar_t *sekTulos(wchar_t *st, int t, int pyor)
{
	static wchar_t buf[10];
	wchar_t dbuf[6];

	if (st == NULL)
		st = buf;
	t = pyor * ((t + 48*TUNTI) / pyor) - 48*TUNTI;
	_itow(t/SEK, st, 10);
	if (pyor < SEK) {
		swprintf(dbuf, L".%03d", t%SEK);
		if (pyor == SEK/10)
			dbuf[2] = 0;
		if (pyor == SEK/100)
			dbuf[3] = 0;
		wcscat(st, dbuf);
		}
	return(st);
}

wchar_t XMLhaetunnus(wchar_t *Nimi, wchar_t *Koodit, wchar_t **Nimet, int N)
{
	for (int i = 0; i < N; i++) {
		if (!wcscmp(Nimi, Nimet[i]))
			return(Koodit[i]);
		}
	return(0);
}

//---------------------------------------------------------------------------
wchar_t *XMLhaeteksti(wchar_t Koodi, wchar_t *Koodit, wchar_t **Nimet)
{
	static wchar_t Tyhja[2] = L"";

	int i = wcswcind(Koodi, Koodit);
	if (i >= 0)
		return(Nimet[i]);
	return(Tyhja);
}

//---------------------------------------------------------------------------
int XMLhae_tagid(wchar_t *Tag, tagListTp *Tags, UINT nTags)
{
	for (UINT i = 0; i < nTags; i++)
		if (!wcscmp(Tag, Tags[i].tag))
			return(Tags[i].tagid);
	return(-1);
}

//---------------------------------------------------------------------------
wchar_t *XMLhae_tagName(int tagid, tagListTp *Tags, UINT nTags)
{
	static wchar_t st[1] = L"";
	for (UINT i = 0; i < nTags; i++)
		if (tagid == Tags[i].tagid)
			return(Tags[i].tag);
	return(st);
}

//---------------------------------------------------------------------------
void XMLhaenodeid(xml_node *node, int nnode, tagListTp *Tags, UINT nTags)
{
	for (int inode = 0; inode <= nnode; inode++)
		node[inode].tagid = XMLhae_tagid(node[inode].tag, Tags, nTags);
}

#ifndef MAXOSUUSLUKU
int haesarja_w(wchar_t *snimi, bool salliyhd)
{
   int i;

   i = 0;
   elimwbl(snimi);
   do {
	  if (!wcscmpU(snimi, Sarjat[i].sarjanimi))
		 return(i);
   } while (++i < sarjaluku + (salliyhd ? nsarjayhd : 0));
   return(-1);
}

static struct {
	int cn;
	int cnino;
	int ino;
	} yhtj[4*MAXJONO];

static int nyhtjono;

void lueyhteysjonot(void)
{
	wchar_t line[100], *p, filenm[] = L"yhtjonot.txt";
	TextFl *yhtfile;

	line[sizeof(line)/2-1] = 0;
	yhtfile = new TextFl(filenm, L"rt");
	if (yhtfile->IsOpen()) {
		while (!yhtfile->Feof() && nyhtjono < 4*MAXJONO) {
			if (yhtfile->ReadLine(line, sizeof(line)/2-1) != NULL) {
				p = wcstok(line, L" ,;\t\n");
				if (p) {
					if ((yhtj[nyhtjono].cn = _wtoi(p)) > 0)
						p = wcstok(NULL, L" ,;\t\n");
					else
						p = NULL;
					}
				if (p) {
					if ((yhtj[nyhtjono].cnino = _wtoi(p)) > 0)
						p = wcstok(NULL, L" ,;\t\n");
					else
						p = NULL;
					}
				if (p) {
					if ((yhtj[nyhtjono].ino = _wtoi(p)) > 0
						&& yhtj[nyhtjono].ino <= MAXJONO) {
						nyhtjono++;
						if (yhtj[nyhtjono].ino > maxjono)
							maxjono = yhtj[nyhtjono].ino;
						}
					}
				}
			}
		delete yhtfile;
		}
}

int yhtjono(int cn, int ino)
{
	int j;

	for (j = 0; j < nyhtjono; j++)
		if (cn+1 == yhtj[j].cn && ino+1 == yhtj[j].cnino)
			return(yhtj[j].ino);
	return(0);
}
#endif

void kirjloki(char *tmsg)
{
	wchar_t wmsg[200];

	wkirjloki(oemtowcs(wmsg, tmsg, 198, 0));
}

static FILE *err_file;
static int n_err_file;
#ifndef _CONSOLE
int select3(int nButton, wchar_t *Teksti, wchar_t *Otsikko,
		wchar_t *Vast1, wchar_t *Vast2, wchar_t *Vast3, TComponent *Sender);
#endif

void kirj_err_file(wchar_t *msg, int nayta)
	{
	static int lopilm;
	wchar_t msg2[200];

	if (nayta && !lopilm) {
#ifdef _CONSOLE
		wchar_t ch = L'J';

		vidspwmsg(ySize-1,0,0,7,msg);
		wcscpy(msg2, L"J)atka, L)opeta nämä virheilmoitukset");
		wselectopt(msg2, L"JL", &ch);
		if (ch == L'L')
			lopilm = 1;
#else
		int vast;

		swprintf(msg2, L"%s \nJatketaanko näiden virheilmoitusten näyttämistä", msg);
		vast = select3(2, msg2, L"Toistuva virheilmoitus", L"Näytä edelleen", L"Ohita ilmoitukset jatkossa", L"", 0);
		if (vast == 2)
			lopilm = 1;
#endif
		}
	if (!err_file) {
		err_file = _wfopen(L"DATA_ERR.TXT", L"wt");
		}
	if (err_file) {
		n_err_file++;
		fputws(msg, err_file);
		fputwc(L'\n', err_file);
		}
	}

void close_err_file(void)
	{
	if (err_file) {
		wchar_t msg[200];
		fclose(err_file);
		err_file = 0;
		swprintf(msg, L"Tiedostoon DATA_ERR.TXT kirjoitettu %d virhetietoa", n_err_file);
#ifdef _CONSOLE
		writeerror_w(msg, 0);
#else
		if (select3(2, msg, L"Virheitä tiedoissa", L"Näytä tiedot", L"Ohita", L"", 0) == 1)
			nayta_virhesanomat();
#endif
		}
	}

void Perr(int ok, wchar_t *api)
{
	wchar_t line[200], *pMsg = 0;
	int er;

	if (ok) 
		return;
	er = GetLastError();
	if (FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, 
		NULL, er, 0, (LPWSTR) &pMsg, 0, 0) > 0 && pMsg) {
		swprintf(line, L"Virhe (API: %.30s): %.150s", api, pMsg);
		wtrimline(line);
#ifdef _CONSOLE
		line[80] = 0;
#endif
		}
	else
		swprintf(line, L"Virhe %d, API: %s", er, api);
	writeerror_w(line, 0);
}

#ifdef _CONSOLE
void clearframe63(void)
   {
   INT r;

   for (r = 2; r < ySize-4; r++) clrtxt(r,0,63);
   }

void clearframe(void)
   {
   INT r;

   for (r = 2; r < ySize-4; r++) clrln(r);
   }

#ifndef MAXOSUUSLUKU
INT luesarja(wchar_t *snimi, wchar_t *tc, bool salliyhd)
	{
	INT  srj = -1;
	wchar_t   sn[16] = L"         " ;
	int nn;

	nn = sarjaluku + (salliyhd ? nsarjayhd : 0);
	if (snimi) {
		wcscpy(sn, snimi);
		srj = haesarja_w(sn, salliyhd);
		}
	do {
		vidspwmsg(ySize-1,0,7,0, L"Sarjoja voi selata näppäimillä \x19 \x18 PgDn PgUp");
		do {
			if (srj >= 0 && srj < nn)
				wcscpy(sn, Sarjat[srj].sarjanimi);
		 inputwstr(sn,10,20,ySize-3,L"\x0D\x1B\xCA\xCC\x05\x18\x03\x12",tc,FALSE);
			switch (*tc) {
				case PGUP :
					srj = 0;
					break;
				case PGDN :
					srj = nn - 1;
					break;
				case UP :
					if (srj > 0) srj--;
					else srj = 0;
					break;
				case DOWN :
					if (srj < 0)
						srj = 0;
					else
						if (srj < nn - 1)
							srj++;
					break;
				}
			} while(*tc == UP || *tc == DOWN || *tc == PGUP || *tc == PGDN);
		clrln(ySize-1);
		if (*tc == 202 || *tc == 204) srj = 0;
		else if(*tc != ESC) {
			srj = haesarja_w(sn, salliyhd);
			if (srj < 0) {
				erbeep();
				vidspwmsg(ySize-1,22,7,0,L"Virheellinen sarja");
				}
			}
		else srj = nn + 1;
		} while (srj < 0 && *tc != ESC);
	clrln(ySize-1);
	return(srj);
	}

#else

INT luesarja(char *snimi, char *tc)
	{
	INT  srj = -1;
	char   sn[16] = "          " ;

	if (snimi) {
		strcpy(sn, snimi);
		srj = haesarja(sn, true);
		}
	do {
		vidspmsg(ySize-1,0,7,0, "Sarjoja voi selata näppäimillä \x19 \x18 PgDn PgUp");
		do {
			if (srj >= 0 && srj < sarjaluku)
				strcpy(sn, Sarjat[srj].sarjanimi);
		 inputstr(sn,10,20,ySize-3,"\x0D\x1B\xCA\xCC\x05\x18\x03\x12",tc,FALSE);
			switch (*tc) {
				case PGUP :
					srj = 0;
					break;
				case PGDN :
					srj = sarjaluku-1;
					break;
				case UP :
					if (srj > 0) srj--;
					else srj = 0;
					break;
				case DOWN :
					if (srj < 0)
						srj = 0;
					else
						if (srj < sarjaluku-1)
							srj++;
					break;
				}
			} while(*tc == UP || *tc == DOWN || *tc == PGUP || *tc == PGDN);
		clrln(ySize-1);
		if (*tc == 202 || *tc == 204) srj = 0;
		else if(*tc != ESC) {
			srj = haesarja(sn, true);
			if (srj < 0) {
				erbeep();
				vidspwmsg(ySize-1,22,7,0,L"Virheellinen sarja");
				}
			}
		else srj = sarjaluku+1;
		} while (srj < 0 && *tc != ESC);
	clrln(ySize-1);
	return(srj);
	}

#endif
#endif
