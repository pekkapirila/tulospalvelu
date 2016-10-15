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

#include <string.h>
#include <ctype.h>
#include <tputil.h>

void TextFl::BOM(void)
{
	if (ftell(File) != 0)
		return;
	if (ReadFl && Skip) {
		fseek(File, Skip, SEEK_SET);
		}
	else if (WriteFl) {
		fclose(File);
		File = _wfopen(FileName, L"wb");
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
		fclose(File);
		wchar_t mode2[20];
		wcscpy(mode2, mode);
		mode2[0] = L'a';
		_wfopen_s(&File, FileName, mode2);
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

	wchar_t Tp;

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
		wcscpy(mode, L"r+t");
	else if (ReadFl)
		wcscpy(mode, L"rt");
	else
		wcscpy(mode, L"wt");

	switch (TextType) {
		case L'8':
			wcscat(mode, L", ccs=UTF-8");
			if (WriteFl && !ReadFl)
				Skip = 3;
			break;
		case L'W':
//			wcscat(mode, L", ccs=UTF-16LE");
			wcscat(mode, L", ccs=UNICODE");
			if (WriteFl && !ReadFl)
				Skip = 2;
			break;
		}
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
		wcscpy(mode, L"r+t");
	else if (ReadFl)
		wcscpy(mode, L"rt");
	else
		wcscpy(mode, L"wt");

	switch (TextType) {
		case L'8':
			wcscat(mode, L", ccs=UTF-8");
			break;
		case L'W':
//			wcscat(mode, L", ccs=UTF-16LE");
			wcscat(mode, L", ccs=UNICODE");
			break;
		}
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

	Buf[0] = 0;
	switch(TextType) {
		case L'8':
		case L'W':
			rBuf = fgetws(Buf, len, File);
			break;
		case L'A':
		case L'O':
			UCHAR *cBuf = new UCHAR[len];

			if (fgets((char *)cBuf, len, File) != NULL) {
				rBuf = Buf;
				if (TextType == L'A')
					ansitooem(cBuf);
				oemtowcs(Buf, cBuf, len, 0);
				}
			delete[] cBuf;
			break;
		}
	if (rBuf == NULL)
		Eof = true;
	return(rBuf);
}

wchar_t	TextFl::ReadChar(void)
{
	wchar_t Ch = 0;
	wint_t iCh;

	if (File == NULL)
		return(WEOF);

	switch(TextType) {
		case L'8':
		case L'W':
			iCh = fgetwc(File);
			if (iCh != WEOF) {
				Ch = (wchar_t) iCh;
				}
			else
				Eof = true;
			break;
		case L'A':
		case L'O':
			int ic;

			if ((ic = fgetc(File)) != EOF) {
				if (TextType == L'O')
					Ch = oemtowchar(ic);
				else
					Ch = ansitowchar(ic);
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
	int nwritten;

	switch(TextType) {
		case L'8':
		case L'W':
			nwritten = fputws(Buf, File);
			break;
		case L'A':
		case L'O':
			UCHAR *cBuf = new UCHAR[wcslen(Buf)+1];

			if (TextType == L'A')
				wcstoansi(cBuf, Buf, wcslen(Buf)+1);
			else
				wcstooem(cBuf, Buf, wcslen(Buf)+1);
			nwritten = fputs((char *)cBuf, File);
			delete[] cBuf;
			break;
		}

	return(nwritten);
}

int	TextFl::WriteChar(wchar_t Char)
{
	int nwritten;

	switch(TextType) {
		case L'8':
		case L'W':
			nwritten = (int) fputwc(Char, File);
			break;
		case L'A':
		case L'O':
			char cC;

			if (TextType == L'A')
				cC = wchartoansi(Char);
			else
				cC = wcrtooemch(Char);
			nwritten = fputc(cC, File);
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
	return(feof(File));
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
