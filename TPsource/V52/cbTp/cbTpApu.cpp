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
#include <io.h>
#include <fcntl.h>
#include <string>
#include <process.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <time.h>
#include <bstrings.h>
#include <System.DateUtils.hpp>
#pragma hdrstop
#include <sys/stat.h>
#include <dos.h>

#ifdef MAXOSUUSLUKU
#include "VDeclare.h"
#include "UnitMain.h"
#else
#include "HkDeclare.h"
#include "WinHk.h"
#endif
#include "UnitAnnaAika.h"
#include "UnitAnnaArvo.h"
#include "UnitAnnaTeksti.h"
#include "UnitSelect.h"
#include "UnitSelect3.h"
#include "UnitMessages.h"
#include "UnitMsgs.h"
#include "UnitIlmoitus.h"

void rem_pr(void);

void taustatulostus(void) {}
void DSPIQ(int nj) {}
void sccurset(int y, int x) {}
void lukumaarat(void) {}
void clrtxt(int row, int col1, int col2) {}
void clrln(int row) {}
void clearframe63(void) {}
void clearframe(void) {}
void scpclr(void) {}
void scclrmsg(int row, int col, int len) {}
void putflddsp(wchar_t *str, INT pos, INT len, INT oik, INT r) {}
void kehys(INT kl) {}
void aikaruutu(void) {}

int kbflush(void) {	return(0);}

//---------------------------------------------------------------------------

// void viwrrectw(int r1, int c1, int r2, int c2, const wchar_t *txt, int fg, int bg, int opt);

int vidspwmsg(int row, int col, int fg, int bg, const wchar_t *txt)
   {
	FormMain->dspMsg((wchar_t *)txt);
	return(wcslen(txt));
   }

//---------------------------------------------------------------------------
int vidspmsg(int row, int col, int fg, int bg, const char *txt)
   {
   wchar_t  wtxt[200];

	FormMain->dspMsg(oemtowcs(wtxt, (char *)txt, 198, 0));
	return(strlen(txt));
   }
//---------------------------------------------------------------------------

int viwrrectw(int r1, int c1, int r2, int c2, const wchar_t *txt, int fg, int bg, int opt)
   {
	int r;
   wchar_t st[82];

   for (r = 0; r <= r2-r1; r++) {
	   wmemcpy(st, txt+r*(c2-c1+1), c2-c1+1);
	   st[c2-c1+1] = 0;
	   vidspwmsg(r1+r, c1, fg, bg, st);
	   }
   return(c2-c1+1);
	}
//---------------------------------------------------------------------------

int viwrrect(int r1, int c1, int r2, int c2, const char *txt, int fg, int bg, int opt)
   {
	int r;
   char st[82];

   for (r = 0; r <= r2-r1; r++) {
	   memcpy(st, txt+r*(c2-c1+1), c2-c1+1);
	   st[c2-c1+1] = 0;
	   vidspmsg(r1+r, c1, fg, bg, st);
	   }
   return(c2-c1+1);
	}
//---------------------------------------------------------------------------

void VIDSPMSG(char *txt)
   {
   wchar_t  wmsg[1000];

	FormMain->dspMsg(ansitowcs(wmsg, txt, 999));
   }
//---------------------------------------------------------------------------

wchar_t FirstChar(UnicodeString str)
{
	if (str.Length() == 0)
		return(0);
	return(str[1]);
}

__fastcall EdtField::EdtField(void)
{
	memset(this, 0, sizeof(EdtField));
}

__fastcall EdtField::~EdtField(void)
{

}

void EdtField::Set(wchar_t *C, TEdit *E, TLabel *L, TEdit *O, TCheckBox *B, void *D, int T, int N)
{
	if (C) {
		wcsncpy(Caption, C, sizeof(Caption)/2-1);
		Caption[sizeof(Caption)/2-1] = 0;
		}
	if (E)
		Edt = E;
	if (L)
		Lbl = L;
	if (O)
		EOrd = O;
	if (B)
		CB = B;
	if (D)
		Data = D;
	DataType = T;
	Count = N;
}

//---------------------------------------------------------------------------


void vidint2(int y, int x, int l, long i)
	{
	if (i > 999) {
	  viwrrectw(y, x, y, x, L"*", 7, 0, 0);
	  i = i % 100;
	  x++;
		l--;
	  }
	vidint(y, x, l, i);
	}

wchar_t *poista_viiva(wchar_t *stout, wchar_t *stin, int size)
{
	wchar_t *pi, *po;

	memset(stout, 0, size);
	for (pi = stin, po = stout; *pi && po < stout + size - 1; pi++, po++) {
		if (*pi == L'_')
			*po = L' ';
		else
			*po = *pi;
		}
	return(stout);
}

wchar_t *stdDateStr(wchar_t *st, int pvm)
{
	swprintf(st, L"%4d-%02d-%02d", YearOf(pvm),	MonthOf(pvm), DayOf(pvm));
	return(st);
}
//---------------------------------------------------------------------------

int decodeStdDateStr(wchar_t *st)
{
	unsigned int y, m, d;

	swscanf(st, L"%d-%d-%d", &y, &m, &d);
	return((int)EncodeDate(y, m, d));
}

void inputaika_prompt(INT32 *t,int t0, int l, wchar_t *prompt, wchar_t *ret, TForm *Sender /* = NULL */)
{
	*ret = ESC;
	if (Sender == NULL)
		Sender = FormMain;
	if ((FormAnnaAika = new TFormAnnaAika(Sender)) != NULL) {
		FormAnnaAika->Kysy(*t, prompt, t0, l);
		FormAnnaAika->ShowModal();
		if (FormAnnaAika->ModalResult == mrOk) {
			*t = FormAnnaAika->Aika;
			*ret = L'\r';
			}
		delete FormAnnaAika;
		}
}

void inputint_prompt(INT32 *i, wchar_t *prompt, wchar_t *ret, TForm *Sender /* = NULL */)
{
	if (ret)
		*ret = ESC;
	if (Sender == NULL)
		Sender = FormMain;
	if ((FormAnnaArvo = new TFormAnnaArvo(Sender)) != NULL) {
		FormAnnaArvo->Kysy(*i, prompt);
		FormAnnaArvo->ShowModal();
		if (FormAnnaArvo->ModalResult == mrOk) {
			*i = FormAnnaArvo->Arvo;
			if (ret)
				*ret = L'\r';
			}
		delete FormAnnaArvo;
		}
}

void inputstr_prompt(wchar_t *st, int l, wchar_t *prompt, wchar_t *ret, TForm *Sender /* = NULL */)
{
	if (ret)
		*ret = ESC;
	if (Sender == NULL)
		Sender = FormMain;
	if ((FormAnnaTeksti = new TFormAnnaTeksti(Sender)) != NULL) {
		FormAnnaTeksti->Kysy(st, prompt, l);
		FormAnnaTeksti->ShowModal();
		if (FormAnnaTeksti->ModalResult == mrOk) {
			wcsncpy(st, FormAnnaTeksti->Teksti.c_str(), l);
			st[l] = 0;
			if (ret)
				*ret = L'\r';
			}
		delete FormAnnaTeksti;
		}
}

int select3(int nButton, wchar_t *Teksti, wchar_t *Otsikko,
		wchar_t *Vast1, wchar_t *Vast2, wchar_t *Vast3, TComponent *Sender)
{
	int retval = 0;

	if (!Sender)
		Sender = FormMain;
	if (TFormSelect3 *FS = new TFormSelect3(Sender)) {
		FS->Kysy(nButton, Teksti, Otsikko, Vast1, Vast2, Vast3);
		retval = FS->ShowModal();
		delete FS;
	}
	return(retval);
}

INT luesarja(wchar_t *snimi, wchar_t *tc, bool salliyhd)
{
	return(0);
}

int ShiftStateInt(TShiftState KeyShift)
{
	int state = 0;
	if (KeyShift.Contains(ssShift))
		state |= 1;
	if (KeyShift.Contains(ssCtrl))
		state |= 2;
	if (KeyShift.Contains(ssAlt))
		state |= 4;
	return(state);
}

TShiftState IntShiftState(int state)
{
	TShiftState SS;
	SS.Clear();
	if (state & 1)
		SS << ssShift;
	if (state & 2)
		SS << ssCtrl;
	if (state & 4)
		SS << ssAlt;
	return(SS);
}

int ValitseHakemisto(wchar_t *Hakemisto, int len)
{
	if (Win32MajorVersion >= 6) {
		TFileOpenDialog *Dlg = new TFileOpenDialog(FormMain);
		if (Dlg) {
			Dlg->Title = L"Valitse hakemisto";
			Dlg->Options << fdoPickFolders;
			Dlg->DefaultFolder = Hakemisto;
			Dlg->FileName = Hakemisto;
			if (Dlg->Execute()) {
				wcsncpy(Hakemisto, Dlg->FileName.c_str(), len);
				Hakemisto[len-1] = 0;
				delete Dlg;
				return(0);
				}
			delete Dlg;
			}
		}
	else {
	   BROWSEINFO BrowsingInfo;
	   LPITEMIDLIST ItemID;
	   wchar_t szDirPath[MAX_PATH], szFolderName[MAX_PATH];

	   memset(&BrowsingInfo, 0, sizeof(BROWSEINFO));
	   memset(szDirPath, 0, MAX_PATH*sizeof(wchar_t));
	   memset(szFolderName, 0, MAX_PATH*sizeof(wchar_t));

	   wcsncpy(szDirPath, Hakemisto, MAX_PATH);
	   BrowsingInfo.hwndOwner = Application->Handle;
	   BrowsingInfo.lpszTitle = L"Valitse hakemisto";
	   BrowsingInfo.ulFlags = BIF_RETURNONLYFSDIRS | BIF_NEWDIALOGSTYLE;

	   ItemID = SHBrowseForFolder(&BrowsingInfo);
	   if (ItemID) {
		   SHGetPathFromIDList(ItemID, szDirPath);
		   wcsncpy(Hakemisto, szDirPath, len);
		   Hakemisto[len-1] = 0;
		   return(0);
		   }
	   }
   return(1);
}

#define MAX_MSG_BUF_SIZE 512

static wchar_t perr_ots[MAX_MSG_BUF_SIZE+1];
static wchar_t perr_msg[MAX_MSG_BUF_SIZE+1];

struct perr_ptrs_tp perr_ptrs = {perr_ots, perr_msg, 0};

/* maximum size of the buffer to be returned from FormatMessage */

//---------------------------------------------------------------------------
void __fastcall TFormMain::dspMsg(wchar_t *txt)
{
	int wparam = 0;
	int lparam = 1;
	wcsncpy(MessageText, txt, sizeof(MessageText)/2-1);
	SendMessage(FormMain->Handle,WM_MYMSGDSPMSG,wparam,lparam);
}

//---------------------------------------------------------------------------
MESSAGE void __fastcall TFormMain::dspMsgHandler(TMyMessage &msg)
{
	 if (Memo1->Lines->Count > 50)
		Memo1->Clear();
	 Memo1->Lines->Add(UnicodeString(MessageText));
	 if (FormMessages->Visible) {
		if (FormMessages->Memo1->Lines->Count > 200) {
			FormMessages->Memo1->Clear();
			}
		FormMessages->Memo1->Lines->Add(UnicodeString(MessageText));
	 }
}

static wchar_t perr_ots2[MAX_MSG_BUF_SIZE+1];
static wchar_t perr_msg2[MAX_MSG_BUF_SIZE+1];

struct perr_ptrs_tp perr_ptrs2 = {perr_ots2, perr_msg2, 0};

static TFormIlmoitus *FSwarning;

//---------------------------------------------------------------------------
MESSAGE void __fastcall TFormMain::warnMsgHandler(TMyMessage &msg)
{
	if (msg.wparam & 0x80000000) {
		if (FSwarning) {
			delete FSwarning;
			FSwarning = NULL;
			}
		return;
		}
	if (FSwarning)
		return;
	if ((FSwarning = new TFormIlmoitus(FormMain)) != NULL) {
		FSwarning->Teksti = ((perr_ptrs_tp *) msg.lparam)->msg;
		FSwarning->Otsikko = ((perr_ptrs_tp *) msg.lparam)->ots;
		FSwarning->Show();
		}
}

//---------------------------------------------------------------------------
void writewarning_w(wchar_t *txt, int wait)
{
	int retval = 0, lparam;

	wcscpy(perr_ptrs2.ots, L"Varoitus");
	wcsncpy(perr_ptrs2.msg, txt, MAX_MSG_BUF_SIZE);
	perr_ptrs2.vast = 0;
	lparam = (int) &perr_ptrs2;
	SendMessage(FormMain->Handle,WM_MYMSGDSPWARNMSG, 0, lparam);
	while (FSwarning) {
		Sleep(200);
		if (wait > 0) {
			wait -= 200;
			if (wait <= 0)
				break;
			}
		}
	if (FSwarning) {
		SendMessage(FormMain->Handle,WM_MYMSGDSPWARNMSG,0x80000000,lparam);
		}
}

//---------------------------------------------------------------------------

MESSAGE void __fastcall TFormMain::errorMsgHandler(TMyMessage &msg)
{
	int retval = 0;
	int wait;
	bool vainok;

	wait = (msg.wparam & 0x0000ffff);
	vainok = (msg.wparam & 0x10000000) != 0;
	if (TFormSelect3 *FS = new TFormSelect3(FormMain)) {
		FS->Kysy(vainok ? 1 : 2, ((perr_ptrs_tp *) msg.lparam)->msg ,
			((perr_ptrs_tp *) msg.lparam)->ots, L"OK", L"Poistu ohjelmasta", L"", wait);
		retval = FS->ShowModal();
		delete FS;
		((perr_ptrs_tp *) msg.lparam)->vast = 3 - 2 * retval;
		if (retval == 2) {
			suljesuoraan = 1;
			Close();
			}
		}
}

//---------------------------------------------------------------------------
void writeerror_w(wchar_t *txt, int wait, bool vainok)
{
	int lparam;
	wcscpy(perr_ptrs2.ots, L"Virhe");
	wcsncpy(perr_ptrs2.msg, txt, MAX_MSG_BUF_SIZE);
	perr_ptrs2.vast = 0;
	lparam = (int) &perr_ptrs2;
	SendMessage(FormMain->Handle,WM_MYMSGDSPERRORMSG,wait + (vainok ? 0x10000000 : 0),lparam);
	while (perr_ptrs2.vast == 0)
		Sleep(200);
}

//---------------------------------------------------------------------------
void writeerror_w(wchar_t *txt, int wait)
{
	int lparam;
	wcscpy(perr_ptrs2.ots, L"Virhe");
	wcsncpy(perr_ptrs2.msg, txt, MAX_MSG_BUF_SIZE);
	perr_ptrs2.vast = 0;
	lparam = (int) &perr_ptrs2;
	SendMessage(FormMain->Handle,WM_MYMSGDSPERRORMSG,wait,lparam);
	while (perr_ptrs2.vast == 0)
		Sleep(200);
}

//---------------------------------------------------------------------------
void writeerror(char *txt, int wait)
{
	wchar_t msg[500] = L"";
	writeerror_w(ansitowcs(msg, txt, sizeof(msg)/2-1), wait);
}

//---------------------------------------------------------------------------
void Wselectxy(INT y, INT x, wchar_t *prompt, wchar_t *term, wchar_t *tc)
	{
	TValitse *Valitse = new TValitse(FormMain);
	wchar_t pmt[1000], st[2] = L" ";

	wcsncpy(pmt, prompt, 1000);
	pmt[999] = 0;
	Valitse->Sanoma->Lines->Text = UnicodeString(pmt);
	Valitse->Optiot->Text = UnicodeString((wchar_t *)term);
	st[0] = *tc;
	Valitse->Valittu->Text = st;
	Valitse->ShowModal();
	delete Valitse;
	*tc = selected;
	}

//---------------------------------------------------------------------------
MESSAGE void __fastcall TFormMain::SelectOptHandler(TMyMessage &msg)
{
	wchar_t wch;

	wch = msg.wparam;
	Wselectxy(0, 0, ((perr_ptrs_tp *) msg.lparam)->msg,
		((perr_ptrs_tp *) msg.lparam)->ots, &wch);
	((perr_ptrs_tp *) msg.lparam)->vast = wch;
}

//---------------------------------------------------------------------------
void Win_wselectopt(wchar_t *msg, wchar_t *opts, wchar_t *sel)
	{
	int lparam, wparam;
	wcscpy(perr_ptrs2.ots, opts);
	wcsncpy(perr_ptrs2.msg, msg, MAX_MSG_BUF_SIZE);
	perr_ptrs2.vast = 0;
	lparam = (int) &perr_ptrs2;
	wparam = *sel;
	SendMessage(FormMain->Handle,WM_MYSELECTOPT,wparam,lparam);
	while (perr_ptrs2.vast == 0)
		Sleep(200);
	*sel = perr_ptrs2.vast;
}
//---------------------------------------------------------------------------
void Win_selectopt(char *msg, char *opts, char *sel)
{
	wchar_t wmsg[300], wopts[20], wsel;

	oemtowcs(wmsg, msg, 298, 0);
	oemtowcs(wopts, opts, 19, 0);
	wsel = ansitowchar(*sel);
	Win_wselectopt(wmsg, wopts, &wsel);
	*sel = wchartoansi(wsel);
}
//---------------------------------------------------------------------------
void nayta_virhesanomat(void)
{
	FILE *err_file;

	err_file = _wfopen(L"DATA_ERR.TXT", L"rt");
	if (err_file) {
		TFormMsgs *FrmMsgs = new TFormMsgs(FormMain);
		wchar_t line[400];
		while (!feof(err_file)) {
			if (!fgetws(line, 399, err_file))
				break;
			wtrimline(line);
			if (wcslen(line) > 0)
				FrmMsgs->Memo1->Lines->Add(line);
			}
		fclose(err_file);
		FrmMsgs->ShowModal();
		delete FrmMsgs;
		}
}

int UStrToAika(UnicodeString USt, int t0)
{
	return(wstrtoaika_vap(USt.c_str(), t0));
}

void setBrowserEmulation(void)
{
	wchar_t IEkey[200], val[100] = L"", val2[100] = L"";
	LONG ret;
	HKEY hkIEkey = 0, hkEmKey = 0;
	DWORD len = 100, DWEmul = 0;
	wchar_t msg[400] = L"";

	if (exeName[0] == 0)
		return;

	wcscpy(IEkey, L"SOFTWARE\\Microsoft\\Internet Explorer\\");
	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, IEkey, 0, KEY_WOW64_32KEY | KEY_READ, &hkIEkey) == ERROR_SUCCESS) {
		ret = RegQueryValueEx(hkIEkey, L"Version", NULL, NULL, (LPBYTE)val, &len);
		if (DeveloperMode)
			swprintf(msg, L"RegOpenKeyEx(HKEY_LOCAL_MACHINE) - Version: ret: %d, len: %d val: %s\n",
				ret, len, val);
		if (ret == ERROR_SUCCESS && len > 2) {
			EnvData.IEVersio = val[0] - L'0';
			if (wmemcmp(val, L"9.1", 3) == 0) {
				ret = RegQueryValueEx(hkIEkey, L"svcVersion", NULL, NULL, (LPBYTE)val2, &len);
				if (DeveloperMode)
					swprintf(msg + wcslen(msg), L"RegOpenKeyEx(HKEY_LOCAL_MACHINE) - svcVersion: ret: %d, len: %d val: %s\n",
						ret, len, val2);
				if (ret == ERROR_SUCCESS && len > 3 && val2[0] == L'1' && val2[2] == L'.')
					EnvData.IEVersio = val2[1] - L'0' + 10;
				}
			}
		RegCloseKey(hkIEkey);
		if (EnvData.IEVersio >= 9) {
			DWORD disp = 0;
			wchar_t *disptxt[3] = {L"", L"/Created", L"/Opened"};
			wcscat(IEkey, L"Main\\FeatureControl\\FEATURE_BROWSER_EMULATION\\");
			if ((ret = RegCreateKeyEx(HKEY_CURRENT_USER, IEkey, 0, 0, 0,
				KEY_WOW64_32KEY | KEY_READ  | KEY_SET_VALUE, 0, &hkEmKey, &disp)) == ERROR_SUCCESS) {
				if (DeveloperMode)
					swprintf(msg + wcslen(msg), L"RegCreateKeyEx(HKEY_CURRENT_USER) ret: %d, Disp: %d%s\n",
						ret, disp, disp <= 2 ? disptxt[disp] : L"");
				len = 4;
				ret = RegQueryValueEx(hkEmKey, exeName, NULL, NULL, (LPBYTE)&DWEmul, &len);
				if (DeveloperMode)
					swprintf(msg + wcslen(msg), L"RegQueryValueEx(HKEY_CURRENT_USER) ret: %d, val: %d\n",
						ret, DWEmul);
				if (ret == ERROR_SUCCESS)
					EnvData.IEEmul = DWEmul;
				if (ret != ERROR_SUCCESS || len < 4 || DWEmul < 1000*EnvData.IEVersio) {
					DWEmul = 1000*EnvData.IEVersio;
					ret = RegSetValueEx(hkEmKey, exeName, 0, REG_DWORD, (LPBYTE)&DWEmul, 4);
					if (ret == ERROR_SUCCESS)
						EnvData.IEEmul = DWEmul;
					if (DeveloperMode)
						swprintf(msg + wcslen(msg), L"RegSetValueEx(HKEY_CURRENT_USER) ret: %d\n",
							ret);
					}
				RegCloseKey(hkEmKey);
				}
			}
		}
	if (DeveloperMode && msg[0])
		wkirjloki(msg);
}

