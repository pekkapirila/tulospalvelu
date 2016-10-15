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
#include <System.IOUtils.hpp>
//#include <FileCtrl.hpp>
#pragma hdrstop
#include <process.h>

#include "UnitJakelu.h"
#include <IdAllFTPListParsers.hpp>
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "ScBridge"
#pragma link "ScSFTPClient"
#pragma link "ScSSHClient"
#pragma link "ScBridge"
#pragma resource "*.dfm"
TFormJakelu *FormJakelu;
extern CRITICAL_SECTION autotul_CriticalSection;

static bool JakYhteysOn;

//---------------------------------------------------------------------------
__fastcall TFormJakelu::TFormJakelu(TComponent* Owner)
	: TForm(Owner)
{
	Scaled = false;
	if (Screen->PixelsPerInch != 96) {
		ScaleBy(Screen->PixelsPerInch, 96);
	}
	ScSFTPClient->OnDirectoryList = ScSFTPClientDirectoryList;
	ScFileStorage->Path = WorkingDir;
	SourcePath = WorkingDir;
	if (SourcePath.Length() > 1 && SourcePath[SourcePath.Length()] != L'\\')
		SourcePath = SourcePath + L"\\";
}
//---------------------------------------------------------------------------
void __fastcall TFormJakelu::BtnOpenClick(TObject *Sender)
{
	openFTP();
}
//---------------------------------------------------------------------------
void TFormJakelu::openFTP(void)
{
	if (Protokolla == 1) {
		openSFTP();
		return;
		}
	IdFTP1->Host = EdServerAddr->Text;
	IdFTP1->Port = _wtoi(EdServerPort->Text.c_str());
	IdFTP1->Username = EdUser->Text;
	IdFTP1->Password = EdPW->Text;
	try {
		IdFTP1->Connect();
		if (EdServerPath->Text.Length() > 0)
			IdFTP1->ChangeDir(EdServerPath->Text);
		Sleep(100);
		EdServerPath->Text = IdFTP1->RetrieveCurrentDir();
		EdServerPath->Font->Color = clBlue;
		}
	catch(...) {

	}
}

MESSAGE void __fastcall TFormJakelu::dspMsgHandler(TMyMessage &msg)
{
	static int edYhtOn;
	if (msg.wparam == edYhtOn)
		return;
	edYhtOn = msg.wparam;
	if (msg.wparam) {
		LblYhteys->Caption = L"Yhteys: On";
		LblYhteys->Font->Color = clGreen;
		BtnSelaa->Enabled = true;
		}
	else {
		LblYhteys->Caption = L"Yhteys: Ei";
		LblYhteys->Font->Color = clGray;
		EdServerPath->Font->Color = clWindowText;
		BtnSelaa->Enabled = false;
		}
	LblYhteys->Refresh();
}

void __fastcall TFormJakelu::IdFTP1Connected(TObject *Sender)
{
	Connected = 1;
	SendMessage(FormJakelu->Handle,WM_MYMSGDSPMSG,1,0);
}
//---------------------------------------------------------------------------
void __fastcall TFormJakelu::IdFTP1Disconnected(TObject *Sender)
{
	Connected = 0;
	SendMessage(FormJakelu->Handle,WM_MYMSGDSPMSG,0,0);
}
//---------------------------------------------------------------------------
void TFormJakelu::CloseAll(void)
{
	KeskeytaLahetys = true;
	Sleep(500);
	if (Connected == 2) {
		CloseAllSFTP();
		return;
		}
	if (!Connected)
		return;
	Connected = 0;
	try {
		IdFTP1->Disconnect();
		}
	catch(...) {
	}
	Panel1->Visible = false;
	ClientWidth = Panel1->Left - 4;
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::BtnCloseClick(TObject *Sender)
{
	CloseAll();
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::BtnSelFileClick(TObject *Sender)
{
	const SELDIRHELP = 0;

	if (RGTiedot->ItemIndex == 0) {
		OpenDialog1->Filter = L"Html-tiedostot (*.html, .htm)|*.html;*.htm|Kaikki tiedostot (*.*)|*.*";
		OpenDialog1->Execute();
		SourcePath = TPath::GetDirectoryName(OpenDialog1->FileName);
		EdSource->Text = OpenDialog1->FileName;
		SourceFiles = EdSource->Text;
		}
	else if (RGTiedot->ItemIndex == 1) {
		wchar_t pathStr[MAX_PATH];
		wcsncpy(pathStr, WorkingDir, MAX_PATH);
		if (!ValitseHakemisto(pathStr, MAX_PATH)) {
			if (wcslen(pathStr) > 0 && pathStr[wcslen(pathStr)-1] != L'\\')
				wcscat(pathStr, L"\\");
			EdSource->Text = pathStr;
			}
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::EdSourceExit(TObject *Sender)
{
	if (RGTiedot->ItemIndex == 0) {
		SourcePath = TPath::GetDirectoryName(EdSource->Text);
		SourceFiles = EdSource->Text;
		}
	else if (RGTiedot->ItemIndex == 1) {
		SourcePath = EdSource->Text;
		if (SourcePath.Length() > 1 && SourcePath[SourcePath.Length()] != L'\\')
			SourcePath = SourcePath + L"\\";
		EdSource->Text = SourcePath;
	}
}
//---------------------------------------------------------------------------

void TFormJakelu::SendOnceFTP(void)
{
	bool tempConnect = false;
	TStringDynArray Files;
	TSearchOption searchOption = TSearchOption::soTopDirectoryOnly;

	try {
		if (Connected == 0) {
			IdFTP1->Connect();
			tempConnect = true;
			}
		IdFTP1->ChangeDir(ServerPath);
		if (!lahKaikki) {
			Files.Length = 1;
			Files[0] = TPath::GetFullPath(SourceFiles);
			}
		else {
			Files = TDirectory::GetFiles(SourcePath);
			}
		for (int i = 0; i < Files.Length; i++) {
			if (inLopetus || KeskeytaLahetys)
				break;
			UnicodeString FlNm = TPath::GetFileName(Files[i]);
			try {
				IdFTP1->Delete(FlNm+L".__tmp__");
				}
			catch (Exception& EidReplyRFCError) {
				}
			try {
				IdFTP1->Put(Files[i],FlNm+L".__tmp__");
				try {
					IdFTP1->Delete(FlNm);
					}
				catch (Exception& EidReplyRFCError){
					}
				IdFTP1->Rename(FlNm+L".__tmp__",FlNm);
				if (DeleteAfterSend)
					DeleteFile(Files[i]);
				}
			catch(Exception& EidReplyRFCError) {
				}
			}
		}
	catch(...) {
		}
	if (Connected == 1 && tempConnect && !KeepOpen)
		IdFTP1->Disconnect();
}
//---------------------------------------------------------------------------

static void SendOnce(void *ptr)
{

	if (inLopetus || FormJakelu->LahetysKesken)
		return;
	FormJakelu->KeskeytaLahetys = false;
	EnterCriticalSection(&autotul_CriticalSection);
	FormJakelu->LahetysKesken = true;
	LeaveCriticalSection(&autotul_CriticalSection);
	if (FormJakelu->Protokolla == 1) {
		FormJakelu->SendOnceSFTP();
		}
	else {
		FormJakelu->SendOnceFTP();
		}
	FormJakelu->LahetysKesken = false;
}
//---------------------------------------------------------------------------
void TFormJakelu::SendFiles(void)
{
	_beginthread(SendOnce, 30000, NULL);
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::BtnSendOnceClick(TObject *Sender)
{
	SendFiles();
}
//---------------------------------------------------------------------------


void __fastcall TFormJakelu::RGAuthClick(TObject *Sender)
{
	GBKey->Visible = RGAuth->ItemIndex == 1;
	PanelPW->Visible = !GBKey->Visible;
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::FileViewDblClick(TObject *Sender)
{
  TTreeNode *Node;

  Node = GetSelectedNode();
  if (Node == NULL)
	return;

  switch (Node->ImageIndex) {
	case 0:
//	  btViewFileClick(Sender);
	  break;
	case 1:
	  OpenDir(GetRootDir() + Node->Text, L"..");
	  break;
	case 2:
	  OpenDir(GetRootDir() + Node->Text, ExtractFileName(StringReplace(lbRootDir->Text, L"/", PathDelim, TReplaceFlags() << rfReplaceAll)));
	 break;
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::FileViewKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)

{
  if (Key == VK_RETURN)
	FileViewDblClick(Sender);
}
//---------------------------------------------------------------------------

UnicodeString __fastcall TFormJakelu::GetRootDir(void)
{
	UnicodeString Result;

	Result = lbRootDir->Text;
	if (IsDelimiter(L'/', Result, Result.Length()) && Result[Result.Length()] == L'/')
		return(Result);
	if (IsDelimiter(L'\\', Result, Result.Length()) && Result[Result.Length()] == L'\\')
		return(Result);

	Result = Result + '/';
	return(Result);
}
//---------------------------------------------------------------------------

TTreeNode *TFormJakelu::GetSelectedNode(void)
{
	if (FileView->Items->Count == 0)
		return(NULL);

	if (FileView->Selected)
		return(FileView->Selected);
	return(FileView->Items->Item[0]);
}
//---------------------------------------------------------------------------

void TFormJakelu::OpenDir(const UnicodeString Path, const UnicodeString SelectedName)
{
	TCursor OldCursor;
	TTreeNode *Node;
	int i;

	if (Protokolla == 1) {
		OpenDirSFTP(Path, SelectedName);
		return;
		}
	OldCursor = Screen->Cursor;
	try {
		Screen->Cursor = crHourGlass;
//		lbRootDir->Text = IdFTP1->RetrieveCurrentDir();
		try {
			IdFTP1->ChangeDir(Path);
			lbRootDir->Text = IdFTP1->RetrieveCurrentDir();
			IdFTP1->List();
			FileView->Items->Clear();
			for (int i = -1; i < IdFTP1->DirectoryListing->Count; i++) {
				Node = FileView->Items->Add(NULL, "");
				if (i == -1) {
					if (lbRootDir->Text != L"/")
						Node->Text = L"..";
					else
						continue;
					}
				else
					Node->Text = IdFTP1->DirectoryListing->Items[i]->FileName;
				if (Node->Text == L"..") {
					Node->ImageIndex = 2;
					Node->SelectedIndex = 2;
					}
				else if (IdFTP1->DirectoryListing->Items[i]->ItemType == ditDirectory) {
					Node->ImageIndex = 1;
					Node->SelectedIndex = 1;
					}
				else {
					Node->Text = Node->Text + L" - " + UnicodeString(IdFTP1->DirectoryListing->Items[i]->Size);
					}
				}
//		ScSFTPClient->ReadDirectory(Handle);
//		} while(!ScSFTPClient->EOF);}
			}
		__finally {
//	  ScSFTPClient->CloseHandle(Handle);
			}

		if (SelectedName.Length() > 0) {
			for (i = 0; i < FileView->Items->Count; i++) {
				if (FileView->Items->Item[i]->Text.LowerCase() == SelectedName.LowerCase()) {
					FileView->Selected = FileView->Items->Item[i];
					return;
					}
				}
			}
		}
	__finally {
		Screen->Cursor = OldCursor;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::BtnSelaaClick(TObject *Sender)
{
	Panel1->Visible = true;
	ClientWidth = Panel1->Left + Panel1->Width + 2;
	lbRootDir->Text = L".";
	OpenDir(lbRootDir->Text);
}
//---------------------------------------------------------------------------


void __fastcall TFormJakelu::FormResize(TObject *Sender)
{
	if (Panel1->Visible) {
		Panel1->Width = ClientWidth - Panel1->Left - 2;
		FileView->Width = Panel1->Width;
		lbRootDir->Width = Panel1->Width - lbRootDir->Left;
		}
	else if (ClientWidth > Panel1->Left - 4) {
		ClientWidth = Panel1->Left - 4;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::CBPoistaClick(TObject *Sender)
{
	DeleteAfterSend = CBPoista->Checked;
}
//---------------------------------------------------------------------------


void __fastcall TFormJakelu::RGTiedotClick(TObject *Sender)
{
	lahKaikki = RGTiedot->ItemIndex == 1;
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::EdServerAddrChange(TObject *Sender)
{
	IdFTP1->Host = EdServerAddr->Text;
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::EdUserChange(TObject *Sender)
{
	IdFTP1->Username = EdUser->Text;
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::EdServerPortChange(TObject *Sender)
{
	IdFTP1->Port = _wtoi(EdServerPort->Text.c_str());
}
//---------------------------------------------------------------------------


void __fastcall TFormJakelu::EdPWChange(TObject *Sender)
{
	IdFTP1->Password = EdPW->Text;
}

//---------------------------------------------------------------------------
void __fastcall TFormJakelu::BitBtn1Click(TObject *Sender)
{
	Application->HelpKeyword(L"Auto tiedostotulostus");
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::FormShow(TObject *Sender)
{
	CBAutoJakelu->Checked = autofileparam.autokomentolaji == 2;
	FormResize(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::RGKeepOpenClick(TObject *Sender)
{
	KeepOpen = RGKeepOpen->ItemIndex > 0;
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::allennamritykset1Click(TObject *Sender)
{
	SaveDialog1->Filter = L"XML-tiedostot (*.xml)|*.xml|Kaikki tiedostot (*.*)|*.*";
	SaveDialog1->DefaultExt = L"xml";
	SaveDialog1->InitialDir = WorkingDir;
	SaveDialog1->FileName = autofileparam.jakelumaar;
	if (SaveDialog1->Execute()) {
		TallJakeluMaaritykset(SaveDialog1->FileName.c_str());
		wcsncpy(autofileparam.jakelumaar, SaveDialog1->FileName.c_str(),
			sizeof(autofileparam.jakelumaar)/2-1);
		}
	SaveDialog1->InitialDir = WorkingDir;
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::Luemritykset1Click(TObject *Sender)
{
	OpenDialog1->Filter = L"XML-tiedostot (*.xml)|*.xml|Kaikki tiedostot (*.*)|*.*";
	OpenDialog1->DefaultExt = L"xml";
	OpenDialog1->InitialDir = WorkingDir;
	OpenDialog1->FileName = autofileparam.jakelumaar;
	if (OpenDialog1->Execute()) {
		LueJakeluMaaritykset(OpenDialog1->FileName.c_str());
		wcsncpy(autofileparam.jakelumaar, OpenDialog1->FileName.c_str(),
			sizeof(autofileparam.jakelumaar)/2-1);
		}
	OpenDialog1->InitialDir = WorkingDir;
}
//---------------------------------------------------------------------------

#define JAK_JakeluParam 	100
#define JAK_Protocol  		101
#define JAK_ServerAddr  	102
#define JAK_ServerPort  	103
#define JAK_User			104
#define JAK_AuthMethod  	105
#define JAK_Password    	106
#define JAK_PublikKey   	107
#define JAK_LocalDataType 	108
#define JAK_LocalDataName 	109
#define JAK_ServerPath 		110
#define JAK_DelAfterSend 	111
#define JAK_KeepOpen  		112

static tagListTp JakTags[] = {
	{JAK_JakeluParam, L"JakeluParam"},
	{JAK_Protocol, L"Protocol"},
	{JAK_ServerAddr, L"ServerAddr"},
	{JAK_ServerPort, L"ServerPort"},
	{JAK_User, L"User"},
	{JAK_AuthMethod, L"AuthMethod"},
	{JAK_Password, L"Password"},
	{JAK_PublikKey, L"PublikKey"},
	{JAK_LocalDataType, L"LocalDataType"},
	{JAK_LocalDataName, L"LocalDataName"},
	{JAK_ServerPath, L"ServerPath"},
	{JAK_DelAfterSend, L"DelAfterSend"},
	{JAK_KeepOpen, L"KeepOpen"}
	};

static int nJakTags = sizeof(JakTags)/sizeof(JakTags[0]);

static wchar_t TxtLaji[] = L"FSPKTD", *TxtSt[] = {L"FTP", L"SFTP", L"Password", L"PublicKey",
	L"File", L"Directory"};

void TFormJakelu::TallJakeluMaaritykset(wchar_t *MaarFile)
{
	TextFl *tallfl;
	int level = 0;
	wchar_t ln[400];

	tallfl = new TextFl(MaarFile, L"wt");

	if (tallfl->IsOpen()) {
		tallfl->WriteLine(L"<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>\n");
		swprintf(ln, L"%s createTime=\"%s\" creator=\"HkKisaWin %s\"", L"JakeluParam", ISOdatetime(0,1), Build());
		tallfl->put_wtag(ln, level++);
		tallfl->put_wxml_s(XMLhae_tagName(JAK_Protocol, JakTags, nJakTags),
			XMLhaeteksti(L"FS"[Protokolla], TxtLaji, TxtSt), level);
		tallfl->put_wxml_s(XMLhae_tagName(JAK_ServerAddr, JakTags, nJakTags), EdServerAddr->Text.c_str(), level);
		tallfl->put_wxml_d(XMLhae_tagName(JAK_ServerPort, JakTags, nJakTags), _wtoi(EdServerPort->Text.c_str()), level);
		tallfl->put_wxml_s(XMLhae_tagName(JAK_User, JakTags, nJakTags), EdUser->Text.c_str(), level);
		tallfl->put_wxml_s(XMLhae_tagName(JAK_AuthMethod, JakTags, nJakTags),
			XMLhaeteksti(L"PK"[RGAuth->ItemIndex], TxtLaji, TxtSt), level);
		if (RGAuth->ItemIndex == 0) {
			tallfl->put_wxml_s(XMLhae_tagName(JAK_Password, JakTags, nJakTags), EdPW->Text.c_str(), level);
			}
		if (RGAuth->ItemIndex == 1) {
			tallfl->put_wxml_s(XMLhae_tagName(JAK_PublikKey, JakTags, nJakTags), CBKeyFile->Text.c_str(), level);
			}
		tallfl->put_wxml_s(XMLhae_tagName(JAK_LocalDataType, JakTags, nJakTags),
			XMLhaeteksti(L"TD"[RGTiedot->ItemIndex], TxtLaji, TxtSt), level);
		tallfl->put_wxml_s(XMLhae_tagName(JAK_LocalDataName, JakTags, nJakTags), EdSource->Text.c_str(), level);
		tallfl->put_wxml_s(XMLhae_tagName(JAK_ServerPath, JakTags, nJakTags), EdServerPath->Text.c_str(), level);
		tallfl->put_wxml_d(XMLhae_tagName(JAK_DelAfterSend, JakTags, nJakTags), CBPoista->Checked ? 1 : 0, level);
		tallfl->put_wxml_d(XMLhae_tagName(JAK_KeepOpen, JakTags, nJakTags), RGKeepOpen->ItemIndex, level);
		tallfl->put_wantitag(L"JakeluParam", --level);
		}
	delete tallfl;
}
//---------------------------------------------------------------------------
int TFormJakelu::loadJakeluParam(xml_node *node, int nnode)
{
	int er = 0, inode, DepthIn, haara = 0, iassign = -1, no = 0, val;
	wchar_t ln[200], lj;
	bool Protokolla0;

	XMLhaenodeid(node, nnode, JakTags, nJakTags);
	DepthIn = node[0].depth + 1;
	for (inode = 1; inode <= nnode; inode++) {
		if (node[inode].depth == DepthIn) {
			haara = 0;
			switch (node[inode].tagid) {
				case JAK_Protocol :
					node[inode].gettext(ln, 10);
					lj = XMLhaetunnus(ln, TxtLaji, TxtSt, sizeof(TxtSt)/sizeof(TxtSt[0]));
					Protokolla0 = wcswcind(lj, L"FS");
					break;
				case JAK_ServerAddr:
					node[inode].gettext(ln, sizeof(ln)/2-1);
					EdServerAddr->Text = ln;
					break;
				case JAK_ServerPort:
					node[inode].gettext_int(&val);
					EdServerPort->Text = val;
					break;
				case JAK_User:
					node[inode].gettext(ln, sizeof(ln)/2-1);
					EdUser->Text = ln;
					break;
				case JAK_AuthMethod:
					node[inode].gettext(ln, 10);
					lj = XMLhaetunnus(ln, TxtLaji, TxtSt, sizeof(TxtSt)/sizeof(TxtSt[0]));
					RGAuth->ItemIndex = wcswcind(lj, L"PK");
					break;
				case JAK_Password:
					node[inode].gettext(ln, sizeof(ln)/2-1);
					EdPW->Text = ln;
					break;
				case JAK_PublikKey:
					node[inode].gettext(ln, sizeof(ln)/2-1);
					CBKeyFile->Text = ln;
					break;
				case JAK_LocalDataType:
					node[inode].gettext(ln, 10);
					lj = XMLhaetunnus(ln, TxtLaji, TxtSt, sizeof(TxtSt)/sizeof(TxtSt[0]));
					RGTiedot->ItemIndex = wcswcind(lj, L"TD");
					lahKaikki = RGTiedot->ItemIndex == 1;
					break;
				case JAK_LocalDataName:
					node[inode].gettext(ln, sizeof(ln)/2-1);
					EdSource->Text = ln;
					break;
				case JAK_ServerPath:
					node[inode].gettext(ln, sizeof(ln)/2-1);
					EdServerPath->Text = ln;
					break;
				case JAK_DelAfterSend:
					node[inode].gettext_int(&val);
					CBPoista->Checked = val;
					DeleteAfterSend = CBPoista->Checked;
					break;
				case JAK_KeepOpen:
					node[inode].gettext_int(&val);
					KeepOpen = val;
					RGKeepOpen->ItemIndex = val;
					break;
				}
			continue;
			}
		if (node[inode].depth == DepthIn + 1) {
			continue;
			}
		}
	if (Protokolla0) {
		rbFTP->Checked = false;
		Protokolla = Protokolla0;
		if (rbSFTP->Checked)
			ProtokollaValinnat();
		else
			rbSFTP->Checked = true;
		}
	else {
		rbSFTP->Checked = false;
		Protokolla = Protokolla0;
		if (rbFTP->Checked)
			ProtokollaValinnat();
		else
			rbFTP->Checked = true;
		}
	Refresh();
	return(er);
}

//---------------------------------------------------------------------------

int TFormJakelu::LueJakeluMaaritykset(wchar_t *MaarFile)
{
	TextFl *infile;
	int ntag = 10000, haara = 0, inode, inode_in, depth = 0, rv, er = 0, lntype;
	short k[MAXXMLDEPTH];
	wchar_t line[1000], ln[100], *txtbuf[MAXXMLDEPTH];
	wchar_t msg[200] = L"", tag[MAXXMLTAGLEN+1];
	xml_tree tree;
	xml_node nd;

	memset(k, 0, sizeof(k));
	infile = new TextFl(MaarFile, L"rt");
	if (infile->IsOpen()) {
		for (int i = 0; i < MAXXMLDEPTH; i++) {
			txtbuf[i] = new wchar_t[XMLTXTBUFLEN+1];
			memset(txtbuf[i], 0, 2*(XMLTXTBUFLEN+1));
			}
		tree.node = new xml_node[ntag];
		tree.maxnode = ntag;
		inode = -1;
		rv = 0;
		while (!er && !infile->Feof()) {
			if (infile->ReadLine(line, 999) == NULL)
				break;
			rv++;
			elimwbl2(line);
			lntype = nd.interpretXmlLine(line, tag);
			if (lntype == -5)
				continue;
			nd.depth = depth;
			for (int i = 0; i <= depth; i++)
				nd.nodeno[i] = k[i];           // merkitään eri tasojen järjestysnumerot solmuun
			if (lntype > 0)
				nd.tagid = XMLhae_tagid(nd.tag, JakTags, nJakTags);
			if (lntype == 1) {
				tree.node[++inode] = nd;
				k[depth++]++;
				if (depth == 1) {
					inode_in = inode;
					switch (nd.tagid) {
						case JAK_JakeluParam :
							haara = 0;
							er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
							--depth;
							if (!er)
								loadJakeluParam(tree.node+inode_in, inode-inode_in);
							inode = inode_in-1;
							break;
						}
					continue;
					}
				if (depth > 1) {
					er = tree.loadbranch(infile, &inode, depth, &rv, txtbuf, k);
					--depth;
					}
				}
			else if (lntype == 8) {                  // sulkeva tag uudella rivillä
				int jnode;
				depth--;
				k[depth]++;                 // kasvatetaan tason laskuria sulkevän tagin tasolla
				// Etsitään vastaava avaava tag aiemmasta ketjusta
				for (jnode = inode; jnode > 0 && tree.node[jnode].depth > depth; jnode--) ;
				if (wcscmp(tag, tree.node[jnode].tag)) {      // Varmistetaan, että tagit ovat pari
					er = 1;
					swprintf(msg, L"XML-tiedoston tulkinta ei onnnistu. Rivi %d, Tag: '%.30s', odotettu: '%.30s'",
						rv, tag, tree.node[jnode].tag);
					break;
					}
				if (wcslen(txtbuf[depth])) {    // Tallennetaan tason mahdollinen teksti solmuun
					tree.node[jnode].text = new wchar_t[wcslen(txtbuf[depth])+1];
					wcscpy(tree.node[jnode].text, txtbuf[depth]);
					}
				if (depth == 0) {
					break;
					}
				continue;
				}
			}      // while(!infile->Feof())
		delete[] tree.node;
		tree.node = 0;
		for (int i=0; i < MAXXMLDEPTH; i++)
			delete[] txtbuf[i];
		}  		  // infile->Open()
	else
		return(-1);
	delete infile;
	if (depth) {
		er = 1;
		swprintf(msg, L"XML-tiedosto %s päättyi rivillä %d tasolla %d", MaarFile, rv, depth);
		}
	if (er) {
		if (!msg[0]) {
			swprintf(msg, L"Tiedostossa %s virhe rivillä %d tai aikaisemmin", MaarFile, rv);
			}
		writeerror_w(msg, 0, true);
		}
	tree.nodecount = inode+1;
	EdSourceExit(this);
	ServerPath = EdServerPath->Text;
	return(0);
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::EdServerPathExit(TObject *Sender)
{
	ServerPath = EdServerPath->Text;
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::Luouusiavaintiedostopari1Click(TObject *Sender)
{
	wchar_t st[100] = L"", ch;
  TCursor OldCursor = Screen->Cursor;

	inputstr_prompt(st, 99, L"Anna avaintiedoston nimi ilman päätettä", &ch, this);
	if (ch == ESC)
		return;

  try {
	Screen->Cursor = crHourGlass;

	TScKey *Key = ScFileStorage->Keys->FindKey(st);
	TScAsymmetricAlgorithm Algorithm;
	int BitCount;

	if (Key == NULL) {
	  Key = new TScKey(ScFileStorage->Keys);
	  Key->KeyName = st;
	  Algorithm = aaRSA;
	  BitCount = 1024;
	}
	else {
	  Key->Ready = true;
	  Algorithm = Key->Algorithm;
	  BitCount = Key->BitCount;
	}

	try {
	  Key->Generate(Algorithm, BitCount, Scbridge::Random);
	  Key->ExportTo(Key->KeyName + L".pub", true, L"", saTripleDES_cbc, kfDefault, L"");

	  String msg = L"Avaintiedosto on generoitu ohjelman datahakemistoon.\n";
	  msg += L"Ota avain käyttöön siirtämällä julkinen avaintiedosto '" + Key->KeyName;
	  msg += ".pub' palvelimelle käytettävän SSH-palvelimen ohjeiden mukaisesti.";
	  MessageDlg(msg, mtInformation, TMsgDlgButtons() << mbOK, 0);
	}
	catch (Exception &ex) {
	  MessageDlg("Avaimen generointi ei onnistunut: " + ex.Message, mtWarning, TMsgDlgButtons() << mbOK, 0);
    }
  }
  __finally {
    Screen->Cursor = OldCursor;
  }
}
//---------------------------------------------------------------------------



void __fastcall TFormJakelu::ScSSHClientBeforeConnect(TObject *Sender)
{
  ScSSHClient->HostName = EdServerAddr->Text;
  ScSSHClient->Port = _wtoi(EdServerPort->Text.c_str());
  ScSSHClient->User = EdUser->Text;

  if (RGAuth->ItemIndex == 0) {
	ScSSHClient->Authentication = atPassword;
	ScSSHClient->Password = EdPW->Text;
  }
  else {
	TScKey *Key;
	UnicodeString KNm;
	int ll;

	ScSSHClient->Authentication = atPublicKey;
	ScSSHClient->PrivateKeyName = CBKeyFile->Text;

	if (ScFileStorage->Keys->FindKey(ScSSHClient->PrivateKeyName) == NULL) {
	   throw Exception("Avaintiedosto ei käytettävissä");
	   }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormJakelu::ScSSHClientServerKeyValidate(TObject *Sender, TScKey *NewServerKey,
          bool &Accept)
{
	String fp, msg;

	TScKey *Key = ScFileStorage->Keys->FindKey(ScSSHClient->HostName);
	if ((Key == NULL) || (!Key->Ready))  {
		NewServerKey->GetFingerprint(haMD5, fp);
		Key = new TScKey(NULL);
		try {
			Key->Assign(NewServerKey);
			Key->KeyName = ScSSHClient->HostName;
			ScFileStorage->Keys->Add(Key);
			}
		catch (Exception &exception) {
			Key->Free();
			throw;
			}

		Accept = true;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::ScSSHClientAfterConnect(TObject *Sender)
{
	Connected = 2;
	SendMessage(FormJakelu->Handle,WM_MYMSGDSPMSG,1,0);
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::ScSSHClientAfterDisconnect(TObject *Sender)
{
	Connected = 0;
	SendMessage(FormJakelu->Handle,WM_MYMSGDSPMSG,0,0);
}
//---------------------------------------------------------------------------
void TFormJakelu::openSFTP(void)
{
	try {
		ScSSHClient->Connect();
		ScSFTPClient->Initialize();
		if (EdServerPath->Text.Length() > 0)
			ScSFTPClient->OpenDirectory(EdServerPath->Text);
		Sleep(100);
		EdServerPath->Font->Color = clBlue;
		}
	catch(...) {

	}
}
//---------------------------------------------------------------------------
void TFormJakelu::SendOnceSFTP(void)
{
	bool tempConnect = false;

	TStringDynArray Files;
	TSearchOption searchOption = TSearchOption::soTopDirectoryOnly;

	try {
		if (Connected == 0) {
			ScSSHClient->Connect();
			ScSFTPClient->Initialize();
			tempConnect = true;
			}
		lbRootDir->Text = ScSFTPClient->RetrieveAbsolutePath(ServerPath);
		ScSFTPClient->OpenDirectory(lbRootDir->Text);
		if (!lahKaikki) {
			Files.Length = 1;
			Files[0] = TPath::GetFullPath(SourceFiles);
			}
		else {
			Files = TDirectory::GetFiles(SourcePath);
			}
		for (int i = 0; i < Files.Length; i++) {
			UnicodeString FlNm = TPath::GetFileName(Files[i]);
			if (inLopetus || KeskeytaLahetys)
				break;
			try {
				ScSFTPClient->RemoveFile(GetRootDir()+FlNm+L".__tmp__");
				}
			catch (Exception& EScSFTPClientError) {
				}
			try {
				ScSFTPClient->UploadFile(Files[i], GetRootDir()+FlNm+L".__tmp__", False);
				try {
					ScSFTPClient->RemoveFile(GetRootDir()+FlNm);
					}
				catch (Exception& EScSFTPClientError){
					}
				ScSFTPClient->RenameFile(GetRootDir()+FlNm+L".__tmp__",GetRootDir()+FlNm);
				if (DeleteAfterSend)
					DeleteFile(Files[i]);
				}
			catch(Exception& EScSFTPClientError) {
				}
			}
		if (FileView && FileView->Visible)
			OpenDirSFTP(lbRootDir->Text);
		}
	catch(...) {
		}
	if (Connected == 2 && tempConnect && !KeepOpen) {
		ScSFTPClient->Disconnect();
		ScSSHClient->Disconnect();
		}
}
//---------------------------------------------------------------------------
void __fastcall TFormJakelu::OpenDirSFTP(const UnicodeString Path, const UnicodeString SelectedName)
{
	TCursor OldCursor;
	TScSFTPFileHandle Handle;
	int i;

	OldCursor = Screen->Cursor;
	try {
		Screen->Cursor = crHourGlass;

		lbRootDir->Text = ScSFTPClient->RetrieveAbsolutePath(Path.Trim());

		Handle = ScSFTPClient->OpenDirectory(lbRootDir->Text);
		try {
			FileView->Items->Clear();
			do {
				ScSFTPClient->ReadDirectory(Handle);
				} while(!ScSFTPClient->EOF());
			}
		__finally {
			ScSFTPClient->CloseHandle(Handle);
			}

		if (SelectedName.Length() > 0) {
			for (i = 0; i < FileView->Items->Count; i++) {
				if (FileView->Items->Item[i]->Text.LowerCase() == SelectedName.LowerCase()) {
					FileView->Selected = FileView->Items->Item[i];
					return;
					}
				}
			}
		}
	__finally {
		Screen->Cursor = OldCursor;
		}
}
//---------------------------------------------------------------------------
void TFormJakelu::CloseAllSFTP(void)
{
	if (Connected != 2)
		return;
	Connected = 0;
	try {
		ScSFTPClient->Disconnect();
		ScSSHClient->Disconnect();
		}
	catch(...) {
	}
	Panel1->Visible = false;
	ClientWidth = Panel1->Left - 4;
}
//---------------------------------------------------------------------------


void __fastcall TFormJakelu::ScSFTPClientDirectoryList(TObject *Sender, const UnicodeString Path,
		  const TScSFTPFileHandle Handle, TScSFTPFileInfo *FileInfo,
          bool EOF)
{
TTreeNode *Node;

  if (FileInfo == NULL || FileInfo->Filename == L".")
	 return;

  Node = FileView->Items->Add(NULL, "");
  if (FileInfo->Filename == L"..") {
	Node->ImageIndex = 2;
	Node->SelectedIndex = 2;
	}
  else if (FileInfo->Longname.Length() > 0 && FileInfo->Longname[1] == L'd') {
	Node->ImageIndex = 1;
	Node->SelectedIndex = 1;
	}

  Node->Text = FileInfo->Filename; // for sorting
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::CBKeyFileDropDown(TObject *Sender)
{
  ScFileStorage->Keys->GetKeyNames(CBKeyFile->Items);
}
//---------------------------------------------------------------------------
void TFormJakelu::ProtokollaValinnat(void)
{
	switch (Protokolla) {
		case 0:
//			rbFTP->Checked = true;
//			rbSFTP->Checked = false;
			RGAuth->Visible = false;
			PanelPW->Visible = true;
			GBKey->Visible = false;
			EdServerPort->Text = 21;
			break;
		case 1:
//			rbFTP->Checked = false;
//			rbSFTP->Checked = true;
			RGAuth->Visible = true;
			PanelPW->Visible = RGAuth->ItemIndex == 0;
			GBKey->Visible = RGAuth->ItemIndex == 1;
			if (_wtoi(EdServerPort->Text.c_str()) < 22)
				EdServerPort->Text = 22;
			break;
		}
	Luouusiavaintiedostopari1->Visible = Protokolla == 1;
}
//---------------------------------------------------------------------------


void __fastcall TFormJakelu::rbFTPClick(TObject *Sender)
{
	CloseAll();
	Protokolla = 0;
	ProtokollaValinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::rbSFTPClick(TObject *Sender)
{
	CloseAll();
	Protokolla = 1;
	ProtokollaValinnat();
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::BtnOtaPolkuClick(TObject *Sender)
{
	EdServerPath->Text = lbRootDir->Text;
	ServerPath = EdServerPath->Text;
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::BtnLuoKansioClick(TObject *Sender)
{
	if (lbRootDir->Text.Length() < 2)
		return;
	switch (Connected) {
		case 1:
			IdFTP1->MakeDir(lbRootDir->Text);
			OpenDir(lbRootDir->Text);
			break;
		case 2:
			ScSFTPClient->MakeDirectory(lbRootDir->Text);
			OpenDirSFTP(lbRootDir->Text);
			break;
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::BtnPoistaTiedostoClick(TObject *Sender)
{
	TTreeNode *Node;
	UnicodeString FlNm;
	int Pos;

	if (Application->MessageBoxW(L"Haluatko varmasti poistaa valitun tiedoston?", L"", MB_YESNO) != IDYES)
		return;
	Node = GetSelectedNode();
	if (Node != NULL && Node->SelectedIndex == 0) {
		switch (Connected) {
			case 1:
				FlNm = Node->Text;
				Pos = FlNm.Pos(L" - ");
				if (Pos > 0)
					FlNm = FlNm.SubString(1, Pos-1);
				IdFTP1->Delete(FlNm);
				OpenDir(lbRootDir->Text);
				break;
			case 2:
				ScSFTPClient->RemoveFile(GetRootDir() + Node->Text);
				OpenDirSFTP(lbRootDir->Text);
			break;
			}
		}
}
//---------------------------------------------------------------------------

void __fastcall TFormJakelu::BtnPaivitaClick(TObject *Sender)
{
	OpenDir(lbRootDir->Text);
}
//---------------------------------------------------------------------------

