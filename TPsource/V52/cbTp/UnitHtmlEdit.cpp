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

//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitHtmlEdit.h"
#ifdef MAXOSUUSLUKU
#include "UnitMain.h"
#else
#include "WinHk.h"
#endif
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "SHDocVw_OCX"
#pragma resource "*.dfm"
TFormHtmlEdit *FormHtmlEdit;

wchar_t temp_html_flnm[80] = L"___tmp_html_1.html";

//---------------------------------------------------------------------------
__fastcall TFormHtmlEdit::TFormHtmlEdit(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
}
//---------------------------------------------------------------------------
void __fastcall TFormHtmlEdit::TabHtmlShow(TObject *Sender)
{
	UnicodeString FlNm = CurrentDir+L"\\"+
		temp_html_flnm;
	WriteHtmlFile(FlNm.c_str(), true, false);
	WebBrowser1->Navigate((UnicodeString(L"file:///")+FlNm).c_str());
}
//---------------------------------------------------------------------------
void __fastcall TFormHtmlEdit::ReadHtmlFile(wchar_t *flnm)
{
	TextFl *InFl = new TextFl(flnm, L"r");
	MemoText->Clear();
	RGLuonne->ItemIndex = 1;
	if (InFl->IsOpen()) {
		wchar_t *p, line[2000], LINE[2000], sline[4000] = L"";
		bool alku = true, in_pre = false;
		int emptyline = 0;

		while (!InFl->Feof()) {
			InFl->ReadLine(line, sizeof(line)/2-1);
			if (!in_pre) {
				wtrimline(line);
				if (line[0])
					wcscat(line, L" ");
				}
			if (wcslen(line) == 0) {
				continue;
				}
			wcscpy(LINE, line);
			upcasewstr(LINE);
			if (alku) {
				if ((p = wcsstr(LINE, L"<!DOCTYPE html>\n<HTML>")) != NULL) {
					wmemmove(line, line+(p-LINE)+6, wcslen(p+6)+1);
					wmemmove(LINE, p+6, wcslen(p+6)+1);
					RGLuonne->ItemIndex = 0;
					MemoText->Clear();
					memset(sline, 0, sizeof(sline));
					}
				if ((p = wcsstr(LINE, L"<BODY>")) != NULL || (p = wcsstr(LINE, L"<BODY ")) != NULL) {
					wchar_t *p1;
					p1 = wcsstr(p+1, L">");
					wmemmove(line, line+(p1-LINE)+1, wcslen(p1));
					wmemmove(LINE, p1+1, wcslen(p1));
					alku = false;
					MemoText->Clear();
					memset(sline, 0, sizeof(sline));
					}
				}
			if ((p = wcsstr(LINE, L"</HTML>")) != NULL || (p = wcsstr(LINE, L"</BODY>")) != NULL) {
				break;
				}
			if (wcslen(line) == 0) {
				continue;
				}
			if ((p = wcsstr(LINE, L"<P>")) != NULL) {
				wmemcpy(line + (p-LINE), L"\n\n", 2);
				wmemmove(line + (p-LINE) + 2, line + (p-LINE) + 3, wcslen(p+2));
				wmemcpy(p, L"\n\n", 2);
				wmemmove(p + 2, p + 3, wcslen(p+2));
				}
			if ((p = wcsstr(LINE, L"<BR>")) != NULL) {
				wmemcpy(line + (p-LINE), L"\n", 1);
				wmemmove(line + (p-LINE) + 1, line + (p-LINE) + 4, wcslen(p+3));
				wmemcpy(p, L"\n", 1);
				wmemmove(p + 1, p + 4, wcslen(p+3));
				}
			if ((p = wcsstr(LINE, L"</P>")) != NULL) {
				wmemmove(line + (p-LINE), line + (p-LINE) + 4, wcslen(p+3));
				wmemmove(p, p + 4, wcslen(p+3));
				}
			if ((p = wcsstr(LINE, L"<PRE>")) != NULL || (p = wcsstr(LINE, L"<PRE ")) != NULL) {
				in_pre = true;
				}
			if ((p = wcsstr(LINE, L"</PRE>")) != NULL) {
				in_pre = false;
				}
			wcsncpy(sline+wcslen(sline), line, sizeof(sline)/2-wcslen(sline)-1);
			for (p = sline; *p;) {
				if (*p == L'\n') {
					*p = 0;
					if (MemoText->Lines->Count > 0 || p-sline > 0)
						MemoText->Lines->Add(sline);
					wmemmove(sline, p+1, wcslen(p+1)+1);
					p = sline;
					}
				else
					p++;
				}
			}
		if (wcslen(sline) > 0)
			MemoText->Lines->Add(sline);
		}
	delete InFl;
}
//---------------------------------------------------------------------------
void __fastcall TFormHtmlEdit::WriteHtmlFile(wchar_t *flnm, bool full, bool ansi)
{
	TextFl *OutFl = new TextFl(flnm, L"w", ansi ? L'A' : L'8');
	if (OutFl->IsOpen()) {
		bool newpar = false;

		if (full)
			OutFl->WriteLine(L"<!DOCTYPE html>\n<html><body>\n");
		for (int i = 0; i < MemoText->Lines->Count; i++) {
			if (!newpar && (i == 0 || MemoText->Lines->Strings[i].Length() == 0)) {
				OutFl->WriteLine(L"<p>");
				newpar = true;
				}
			else {
				if (!newpar)
					OutFl->WriteLine(L"<br>");
				newpar = false;
				}
			if (MemoText->Lines->Strings[i].Length() > 0) {
				OutFl->WriteLine((MemoText->Lines->Strings[i]+L"\n").c_str());
				newpar = false;
				}
			}
		if (full)
			OutFl->WriteLine(L"</body></html>\n");
		}
	delete OutFl;
}
//---------------------------------------------------------------------------

void __fastcall TFormHtmlEdit::FormResize(TObject *Sender)
{
	PageControl1->Height = ClientHeight - PageControl1->Top - 4;
	PageControl1->Width = ClientWidth - PageControl1->Left - 4;
	MemoText->Height = PageControl1->ClientHeight - MemoText->Top - 4;
	MemoText->Width = PageControl1->ClientWidth - MemoText->Left - 4;
	WebBrowser1->Height = PageControl1->ClientHeight - WebBrowser1->Top - 4;
	WebBrowser1->Width = PageControl1->ClientWidth - WebBrowser1->Left - 4;
}
//---------------------------------------------------------------------------

void __fastcall TFormHtmlEdit::FormShow(TObject *Sender)
{
	Luetiedosto1Click(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormHtmlEdit::Luetiedosto1Click(TObject *Sender)
{
	OpenDialog1->InitialDir = CurrentDir;
	OpenDialog1->DefaultExt = L"html";
	OpenDialog1->Filter = L"Html-tiedostot (*.html, *htm)|*.html;*.htm|Kaikki tiedostot (*.*)|*.*";
	if (OpenDialog1->Execute()) {
		FileName = OpenDialog1->FileName;
		ReadHtmlFile(FileName.c_str());
		}
	SetCurrentDir(CurrentDir);
	PageControl1->ActivePageIndex = 0;
	PageControl1->ActivePage->Refresh();
}
//---------------------------------------------------------------------------

bool __fastcall TFormHtmlEdit::TallennaNimella(void)
{
	SaveDialog1->InitialDir = CurrentDir;
	SaveDialog1->FileName = FileName;
	SaveDialog1->DefaultExt = L"html";
	SaveDialog1->Filter = L"Html-tiedostot (*.html, *htm)|*.html;*.htm|Kaikki tiedostot (*.*)|*.*";
	if (SaveDialog1->Execute()) {
		FileName = SaveDialog1->FileName;
		WriteHtmlFile(FileName.c_str(), RGLuonne->ItemIndex == 0, ISO885911->Checked);
		}
	else
		return(false);
	SetCurrentDir(CurrentDir);
	return(true);
}
//---------------------------------------------------------------------------

void __fastcall TFormHtmlEdit::allennanimell1Click(TObject *Sender)
{
	TallennaNimella();
}
//---------------------------------------------------------------------------

void __fastcall TFormHtmlEdit::Tallenna1Click(TObject *Sender)
{
	WriteHtmlFile(FileName.c_str(), RGLuonne->ItemIndex == 0, ISO885911->Checked);
}
//---------------------------------------------------------------------------

void __fastcall TFormHtmlEdit::UTF81Click(TObject *Sender)
{
	UTF81->Checked = true;
	ISO885911->Checked = false;
}
//---------------------------------------------------------------------------

void __fastcall TFormHtmlEdit::ISO885911Click(TObject *Sender)
{
	UTF81->Checked = false;
	ISO885911->Checked = true;
}
//---------------------------------------------------------------------------

void __fastcall TFormHtmlEdit::Button2Click(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TFormHtmlEdit::Button1Click(TObject *Sender)
{
	if (TallennaNimella())
		ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TFormHtmlEdit::FormCreate(TObject *Sender)
{
#ifdef MAXOSUUSLUKU
	CurrentDir = GetCurrentDir();
#else
	CurrentDir = FormMain->CurrentDir;
#endif
}
//---------------------------------------------------------------------------

