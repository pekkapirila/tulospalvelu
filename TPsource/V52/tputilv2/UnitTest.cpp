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
#include <System.hpp>
#include "UnitTest.h"
#include <tputil.h>
//extern PACKAGE int __fastcall Utf8ToUnicode _DEPRECATED_ATTRIBUTE0 (WideChar * Dest, char * Source, int MaxChars);
//extern PACKAGE int __fastcall UnicodeToUtf8 _DEPRECATED_ATTRIBUTE0 (WideChar * Dest, char * Source, int MaxChars);
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TForm1 *Form1;
//---------------------------------------------------------------------------
__fastcall TForm1::TForm1(TComponent* Owner)
	: TForm(Owner)
{
	setlocale(LC_CTYPE, ".1252");
	_setmbcp(_MB_CP_LOCALE);
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button1Click(TObject *Sender)
{
	TextFl *InFile;
	wchar_t types[] = L"A8W", Buf[10000] = L"", *p = Buf;
	int nread;

	Memo1->Clear();
	OpenDialog1->Execute();
	InFile = new TextFl(OpenDialog1->FileName.c_str(), L"r", types[RadioGroup1->ItemIndex]);
	while (InFile->IsOpen() && !InFile->Feof() && (p-Buf + 12) < sizeof(Buf)/2) {
		switch(RadioGroup2->ItemIndex) {
			case 0:
				InFile->ReadLine(Buf, 500);
				if (wcslen(Buf) > 0) {
					Buf[wcslen(Buf)-1] = 0;
					Memo1->Lines->Add(UnicodeString(Buf));
					}
				break;
			case 1:
				*p = InFile->ReadChar();
				p++;
				break;
			case 2:
				nread = InFile->ReadBytes((char *) p, 1, 10);
				p += nread;
				break;
			}
		}
	if (RadioGroup2->ItemIndex > 0) {
		Memo1->Lines->Append(UnicodeString(Buf));
	}
	delete InFile;
}
//---------------------------------------------------------------------------
void __fastcall TForm1::Button2Click(TObject *Sender)
{
	TextFl *OutFile;
	wchar_t types[] = L"A8W", Buf[500] = L"";
	int r = 0, len;

	SaveDialog1->Execute();
	OutFile = new TextFl(SaveDialog1->FileName.c_str(), L"w", types[RadioGroup1->ItemIndex]);
	switch(RadioGroup2->ItemIndex) {
		case 0:
			while (OutFile->IsOpen() && r < Memo1->Lines->Count) {
				wcsncpy(Buf, Memo1->Lines->Strings[r].c_str(), 498);
				Buf[498] = 0;
				wcscat(Buf, L"\n");
				OutFile->WriteLine(Buf);
				r++;
				}
			break;
		case 1:
			while (OutFile->IsOpen() && r < Memo1->Lines->Text.Length()) {
				wcsncpy(Buf, Memo1->Lines->Text.SubString(r+1,1).c_str(), 1);
				OutFile->WriteChar(Buf[0]);
				r++;
				}
			break;
		case 2:
			while (OutFile->IsOpen() && r < Memo1->Lines->Text.Length()) {
				len = Memo1->Lines->Text.Length() - r;
				if (len > 10) {
					len = 10;
					}
				wcsncpy(Buf, Memo1->Lines->Text.SubString(r+1,len).c_str(), len);
				Buf[len] = 0;
				OutFile->WriteBytes((char *) Buf, 2, len);
				r += len;
				}
			break;
		}
	delete OutFile;
}
//---------------------------------------------------------------------------
#ifdef EI_OLE
static char *crnltonl(char* out, char* in)
	{
	char *p = out;

	while (*in){
		if (in[0] == '\r' && in[1] == '\n')
			in++;
		*out++ = *in++;
		}
	*out = 0;
	return(p);
	}

static wchar_t *wcrnltonl(wchar_t* out, wchar_t* in)
	{
	wchar_t *p = out;

	while (*in){
		if (in[0] == L'\r' && in[1] == L'\n')
			in++;
		*out++ = *in++;
		}
	*out = 0;
	return(p);
	}

static char *nltocrnl(char* out, char* in)
	{
	char *p = out;

	while (*in){
		if (*in == '\n')
			*(out++) = '\r';
		*out++ = *in++;
		}
	*out = 0;
	return(p);
	}

static wchar_t *wnltocrnl(wchar_t* out, wchar_t* in)
	{
	wchar_t *p = out;

	while (*in){
		if (*in == L'\n')
			*(out++) = L'\r';
		*out++ = *in++;
		}
	*out = 0;
	return(p);
	}

void TextFl::BOM(void)
{
	if (ftell(File) != 0)
		return;
	if (ReadFl && Skip) {
		fseek(File, Skip, SEEK_SET);
		}
	else if (WriteFl) {
		switch (TextType) {
			case L'8':
				fwrite("\xef\xbb\xbf", 3, 1, File);
				Skip = 3;
				break;
			case L'W':
				fwrite("\xff\xfe", 2, 1, File);
				Skip = 2;
				break;
			}
		}
}

TextFl::TextFl(wchar_t *FName, wchar_t *Mode, wchar_t TxtTp)
{
	File = NULL;
	TextType = 0;
	ReadFl = false;
	WriteFl = false;
	Eof = false;
	Skip = 0;

	if (!FName)
		return;

	FileName = new wchar_t[wcslen(FName)+1];
	wcscpy(FileName, FName);

	if (Mode) {
		for (wchar_t *p = Mode; *p; p++) {
			switch (towupper(*p)) {
				case L'R' :
					ReadFl = true;
					break;
				case L'W' :
				case L'A' :
					WriteFl = true;
					break;
				case L'+' :
					WriteFl = true;
					ReadFl = true;
					break;
				}
			}
		}

	if (!WriteFl)
		ReadFl = true;

	wchar_t Tp = 0;

	if (TxtTp && ((Tp = towupper(TxtTp)) == L'O' || Tp == L'A' || Tp == L'W' || Tp == L'V' || Tp == L'8'))
		TextType = Tp;

	if (ReadFl) {
		char buf[4] = "";
		int nread;

		File = _wfopen(FileName, L"rb");
		if (File) {
			nread = fread(buf, 1, 3, File);
			if (nread >= 2) {
				if (!memcmp(buf, "\xff\xfe", 2)) {
					TextType = L'W';
					Skip = 2;
					}
				else if (!memcmp(buf, "\xfe\xff", 2)) {
					TextType = L'V';
					Skip = 2;
					}
				else if (!memcmp(buf, "\xef\xbb\xbf", 3)) {
					TextType = L'8';
					Skip = 3;
					}
				}
			if (TextType == 0)
				TextType = L'A';

			fclose(File);
			}
		else if (!WriteFl)
			return;
		}

	if (!TextType)
		TextType = L'8';

	if (ReadFl && WriteFl)
		wcscpy(mode, L"r+b");
	else if (ReadFl)
		wcscpy(mode, L"rb");
	else
		wcscpy(mode, L"wb");

	if (TextType == L'W')
		wcscat(mode, L", ccs=UTF-16LE");

	if (_wfopen_s(&File, FileName, mode))
		File = NULL;
	BOM();
}

TextFl::~TextFl(void)
{
	if (File)
		fclose(File);
	delete[] FileName;
}

int TextFl::Open(wchar_t *Mode)
{
	ReadFl = false;
	WriteFl = false;

	if (File)
		fclose(File);

	if (Mode) {
		for (wchar_t *p = Mode; *p; p++) {
			switch (towupper(*p)) {
				case L'R' :
					ReadFl = true;
					break;
				case L'W' :
				case L'A' :
					WriteFl = true;
					break;
				case L'+' :
					WriteFl = true;
					ReadFl = true;
					break;
				}
			}
		}

	wchar_t mode[16];

	if (ReadFl && WriteFl)
		wcscpy(mode, L"r+b");
	else if (ReadFl)
		wcscpy(mode, L"rb");
	else
		wcscpy(mode, L"wb");

	if (TextType == L'W')
		wcscat(mode, L", ccs=UTF-16LE");

	if (_wfopen_s(&File, FileName, mode))
		File = NULL;
	return (File == NULL);
}

void TextFl::Close(void)
{
	if (File)
		fclose(File);
	File = NULL;
}

bool TextFl::IsOpen(void)
{
	return(File != NULL);
}

wchar_t	*TextFl::ReadLine(wchar_t *Buf, int len)
{
	wchar_t *rBuf = NULL;

	if (File == NULL)
		return(NULL);

	UCHAR *cBuf = new UCHAR[2*len+50];
	wchar_t *wBuf = new wchar_t[len+20];

	memset(cBuf, 0, 2*len+10);
	Buf[0] = 0;
	switch(TextType) {
		case L'8':
			if (fgets((char *)cBuf, len+20, File) != NULL) {
//				Utf8ToUnicode(wBuf, cBuf, len+20);
				mbstowcs(wBuf, (char *)cBuf, len);
				rBuf = Buf;
				}
			break;
		case L'W':
			if (fgetws(wBuf, len, File) != NULL)
				rBuf = Buf;
			break;
		case L'A':
			if (fgets((char *)cBuf, len, File) != NULL) {
				ansitowcs(wBuf, cBuf, len);
				rBuf = Buf;
				}
			break;
		case L'O':
			if (fgets((char *)cBuf, len, File) != NULL) {
				rBuf = Buf;
				oemtowcs(wBuf, cBuf, len, 0);
				}
			break;
		}

	if (rBuf == NULL)
		Eof = true;
	wcrnltonl(Buf, wBuf);
	delete[] cBuf;
	delete[] wBuf;
	return(rBuf);
}

wchar_t	TextFl::ReadChar(void)
{
	wchar_t Ch = 0;
	wint_t iCh = WEOF;
	char cBuf[4]= "";
	int ic, l = 1;

	if (File == NULL)
		return(WEOF);

	switch(TextType) {
		case L'8':
			if ((ic = fgetc(File)) != EOF) {
				cBuf[0] = ic;
				if (ic > 127) {
					if ((ic = fgetc(File)) != EOF) {
						cBuf[1] = ic;
						l = 2;
						}
					}
				mbtowc(&Ch, cBuf, l);
				}
			else {
				Ch = WEOF;
				Eof = true;
				}
			break;
		case L'W':
//			if (fread((char *) &Ch, 2, 1, File) == 0) {
			iCh = fgetwc(File);
			Ch = iCh;
//
//			else {
//				Ch = WEOF;
//				Eof = true;
//				}
			break;
		case L'A':
			if ((ic = fgetc(File)) != EOF) {
				Ch = ansitowchar(ic);
				}
			else {
				Ch = WEOF;
				Eof = true;
				}
			break;
		case L'O':
			if ((ic = fgetc(File)) != EOF) {
				Ch = oemtowchar(ic);
				}
			else {
				Ch = WEOF;
				Eof = true;
				}
			break;
		}
	return(Ch);
}

int	TextFl::ReadBytes(char *buf, int size, int count)
{
	if (File == NULL)
		return(0);

	return(fread(buf, size, count, File));
}

int	TextFl::WriteLine(wchar_t *Buf)
{
	int nwritten, l = 0;
	wchar_t *wBuf = new wchar_t[wcslen(Buf)+20];

	wnltocrnl(wBuf, Buf);
	switch(TextType) {
		case L'W':
//			l = wcslen(wBuf);
//			nwritten = fwrite((char *)Buf, 2, l, File);
			nwritten = fputws(wBuf, File);
			break;
		case L'8':
		case L'A':
		case L'O':
			UCHAR *cBuf = new UCHAR[wcslen(wBuf)+100];
			memset(cBuf, 0, wcslen(wBuf)+100);

			if (TextType == L'A')
				wcstoansi(cBuf, wBuf, wcslen(wBuf)+10);
			else if (TextType == L'O')
				wcstooem(cBuf, wBuf, wcslen(wBuf)+10);
			else {
//				AnsiString AS;
//				WideString WS = WideString(wBuf);

				UnicodeToUtf8((char *)cBuf, wBuf, wcslen(wBuf)+100);
				}
//				wcstombs((char *)cBuf, wBuf, wcslen(wBuf)+100);
			nwritten = fputs((char *)cBuf, File);
			delete[] cBuf;
			break;
		}

	delete[] wBuf;
	return(nwritten);
}

int	TextFl::WriteChar(wchar_t Char)
{
	int nwritten = 0, l = 1;
	char cBuf[46] = "";

	switch(TextType) {
		case L'W':
			if (Char == L'\n') {
				nwritten = fputwc(L'\r', File);
				}
			nwritten += fputwc(Char, File);
/*
				memcpy(cBuf, L"\r\n", 4);
				l = 2;
				}
			else
				memcpy(cBuf, &Char, 2);
			nwritten = (int) fwrite(cBuf, 2, l, File);
*/
			break;
		case L'8':
			if (Char == L'\n') {
				memcpy(cBuf, "\r\n", 2);
				}
			else {
				wchar_t wBuf[2] = L" ";
				wBuf[0] = Char;
				UnicodeToUtf8((char *)cBuf, wBuf, 3);
//				wctomb(cBuf, Char);
				}
			nwritten = (int) fputs(cBuf, File);
			break;
		case L'A':
		case L'O':
			if (TextType == L'A')
				cBuf[0] = wchartoansi(Char);
			else
				cBuf[0] = wcrtooemch(Char);
			if (cBuf[0] == '\n') {
				nwritten = fputc('\r', File);
				}
			nwritten += fputc(cBuf[0], File);
			break;
		}
	return(nwritten);
}

int	TextFl::WriteBytes(char *buf, int size, int count)
{
	if (File == NULL)
		return(0);

	return(fwrite(buf, size, count, File));
}

bool TextFl::Feof(void)
	{
	return(feof(File) != 0);
	}

int TextFl::Rewind(void)
{
	int ret;

	Eof = false;
	ret = fseek(File, 0, SEEK_SET);
	BOM();
	return(ret);
}

void TextFl::put_wxml_s(wchar_t *tag, wchar_t *value)
	{
	wchar_t *Buf = new wchar_t[2*wcslen(value) + 2*wcslen(tag) + 10];

	set_wxml_s(Buf, tag, value);
	WriteLine(Buf);
	delete[] Buf;
	}

void TextFl::put_wxml_d(wchar_t *tag, INT32 value)
	{
	wchar_t *Buf = new wchar_t[2*wcslen(tag) + 30];

	set_wxml_d(Buf, tag, value);
	WriteLine(Buf);
	delete[] Buf;
	}

void TextFl::put_wtag(wchar_t *tag)
	{
	wchar_t *Buf = new wchar_t[wcslen(tag) + 10];

	set_wtag(Buf, tag);
	WriteLine(Buf);
	delete[] Buf;
	}

void TextFl::put_wantitag(wchar_t *tag)
	{
	wchar_t *Buf = new wchar_t[wcslen(tag) + 10];

	set_wantitag(Buf, tag);
	WriteLine(Buf);
	delete[] Buf;
	}

int TextFl::Length(void)
	{
	int pos, len;

	pos = (int) ftell(File);
	fseek(File, 0, SEEK_END);
	len = (int) ftell(File);
	fseek(File, pos, SEEK_SET);
	return(len);
	}
#endif
