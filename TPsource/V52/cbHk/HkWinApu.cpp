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
#include <System.DateUtils.hpp>
#include <stdio.h>
#include <stdlib>
#include <io.h>
#include <fcntl.h>
#include <string>
#include <process.h>
#include <errno.h>
#include <ctype.h>
#include <malloc.h>
#include <bkeybrd.h>
#include <bstrings.h>
#pragma hdrstop
#include <dos.h>
#include <sys/stat.h>
#include <time.h>
#include "WinHk.h"
#include "UnitSeurat.h"
#include "UnitAnnaAika.h"
#include "UnitAnnaArvo.h"
#include "UnitAnnaTeksti.h"
#include "UnitSelect.h"
#include "UnitSelect3.h"
#include "UnitMessages.h"
#include "UnitMsgs.h"


void kilptietue::outkilp(INT ntlisa)
{
}

INT kilptietue::inputkilp(INT addfl)
{
	return(0);
}

INT kilptietue::find(INT *d, INT nimihaku)
	{
	return(0);
	}

void korjaa(INT d, kilptietue *kilp, INT tausta, kilptietue *entkilp)
	{
	}

INT32 maaliaika(INT32 tmaali, INT t0, INT maikay, INT maikax, char *kh)
	{
	return(0);
	}

#ifdef JOUKKUEPISTEET
void joukkuepistemaarittelyt(void)
   {
	}
#endif

int tall_seura(wchar_t *nimi, wchar_t *lyhenne, wchar_t *maa, int piiri, int kielto)
{
	if (FormSeurat)
		FormSeurat->TallennaSeura(piiri, maa, lyhenne, nimi, kielto);
	return(0);
}

//---------------------------------------------------------------------------
/*void wselectopt(wchar_t *msg, wchar_t *opts, wchar_t *sel)
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
*/
//---------------------------------------------------------------------------

