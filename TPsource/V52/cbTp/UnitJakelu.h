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

#ifndef UnitJakeluH
#define UnitJakeluH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdTCPClient.hpp>
#include <IdTCPConnection.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <IdExplicitTLSClientServerBase.hpp>
#include <IdFTP.hpp>
#include <Vcl.Mask.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Buttons.hpp>
#include <Vcl.Menus.hpp>
#include "ScBridge.hpp"
#include "ScSFTPClient.hpp"
#include "ScSSHClient.hpp"
#include <System.ImageList.hpp>
#ifndef MAXOSUUSLUKU
#include "HkDeclare.h"
#else
#include "VDeclare.h"
#endif
#ifdef EOF
#undef EOF
#endif

//---------------------------------------------------------------------------
class TFormJakelu : public TForm
{
__published:	// IDE-managed Components
	TEdit *EdServerAddr;
	TLabel *Label1;
	TLabel *Label2;
	TEdit *EdServerPath;
	TLabel *Label4;
	TEdit *EdUser;
	TRadioGroup *RGAuth;
	TOpenDialog *OpenDialog1;
	TRadioGroup *RGTiedot;
	TLabel *Label6;
	TEdit *EdSource;
	TButton *BtnSelFile;
	TButton *BtnOpen;
	TRadioGroup *RGKeepOpen;
	TButton *BtnClose;
	TButton *BtnSendOnce;
	TIdFTP *IdFTP1;
	TLabel *Label7;
	TMaskEdit *EdServerPort;
	TLabel *LblYhteys;
	TGroupBox *GBKey;
	TButton *BtnSelaa;
	TPanel *Panel1;
	TPanel *Panel2;
	TEdit *lbRootDir;
	TTreeView *FileView;
	TImageList *ImageList1;
	TCheckBox *CBPoista;
	TBitBtn *BitBtn1;
	TMainMenu *MainMenu1;
	TMenuItem *iedosto1;
	TMenuItem *allennamritykset1;
	TMenuItem *Luemritykset1;
	TSaveDialog *SaveDialog1;
	TCheckBox *CBAutoJakelu;
	TScSFTPClient *ScSFTPClient;
	TScSSHClient *ScSSHClient;
	TScFileStorage *ScFileStorage;
	TMenuItem *Luouusiavaintiedostopari1;
	TComboBox *CBKeyFile;
	TPanel *PanelPW;
	TEdit *EdPW;
	TLabel *LblPW;
	TGroupBox *GBProtokolla;
	TRadioButton *rbFTP;
	TRadioButton *rbSFTP;
	TButton *BtnOtaPolku;
	TButton *BtnLuoKansio;
	TButton *BtnPoistaTiedosto;
	TButton *BtnPaivita;
	void __fastcall BtnOpenClick(TObject *Sender);
	void __fastcall IdFTP1Connected(TObject *Sender);
	void __fastcall IdFTP1Disconnected(TObject *Sender);
	void __fastcall BtnCloseClick(TObject *Sender);
	void __fastcall BtnSelFileClick(TObject *Sender);
	void __fastcall BtnSendOnceClick(TObject *Sender);
	void __fastcall RGAuthClick(TObject *Sender);
	void __fastcall FileViewDblClick(TObject *Sender);
	void __fastcall FileViewKeyDown(TObject *Sender, WORD &Key, TShiftState Shift);
	void __fastcall BtnSelaaClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall CBPoistaClick(TObject *Sender);
	void __fastcall EdSourceExit(TObject *Sender);
	void __fastcall RGTiedotClick(TObject *Sender);
	void __fastcall EdServerAddrChange(TObject *Sender);
	void __fastcall EdUserChange(TObject *Sender);
	void __fastcall EdServerPortChange(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall allennamritykset1Click(TObject *Sender);
	void __fastcall Luemritykset1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall EdPWChange(TObject *Sender);
	void __fastcall RGKeepOpenClick(TObject *Sender);
	void __fastcall EdServerPathExit(TObject *Sender);
	void __fastcall Luouusiavaintiedostopari1Click(TObject *Sender);
	void __fastcall ScSSHClientBeforeConnect(TObject *Sender);
	void __fastcall ScSSHClientServerKeyValidate(TObject *Sender, TScKey *NewServerKey,
		  bool &Accept);
	void __fastcall ScSSHClientAfterConnect(TObject *Sender);
	void __fastcall ScSSHClientAfterDisconnect(TObject *Sender);
	void __fastcall ScSFTPClientDirectoryList(TObject *Sender, const UnicodeString Path,
		  const TScSFTPFileHandle Handle, TScSFTPFileInfo *FileInfo,
		  bool EOF);
	void __fastcall CBKeyFileDropDown(TObject *Sender);
	void __fastcall rbFTPClick(TObject *Sender);
	void __fastcall rbSFTPClick(TObject *Sender);
	void __fastcall BtnOtaPolkuClick(TObject *Sender);
	void __fastcall BtnLuoKansioClick(TObject *Sender);
	void __fastcall BtnPoistaTiedostoClick(TObject *Sender);
	void __fastcall BtnPaivitaClick(TObject *Sender);

private:	// User declarations
	MESSAGE void __fastcall dspMsgHandler(TMyMessage &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYMSGDSPMSG, TMyMessage, dspMsgHandler);
END_MESSAGE_MAP(TComponent)

	UnicodeString __fastcall GetRootDir(void);
	TTreeNode *GetSelectedNode(void);
	void OpenDir(const UnicodeString Path, const UnicodeString SelectedName = L"");
	void __fastcall OpenDirSFTP(const UnicodeString Path, const UnicodeString SelectedName = L"");
	void openSFTP(void);
	void CloseAllSFTP(void);
	void ProtokollaValinnat(void);
	int Connected;
	bool DeleteAfterSend;
	UnicodeString ServerPath;
	UnicodeString SourcePath;
	UnicodeString SourceFiles;
	bool lahKaikki;
public:		// User declarations
	__fastcall TFormJakelu(TComponent* Owner);
	void openFTP(void);
	void SendOnceSFTP(void);
	void SendOnceFTP(void);
	void SendFiles(void);
	void CloseAll(void);
	void TallJakeluMaaritykset(wchar_t *MaarFile);
	int LueJakeluMaaritykset(wchar_t *MaarFile);
	int loadJakeluParam(xml_node *node, int nnode);
	void NaytaYhteystila(void);
	bool KeepOpen;
	bool Initialized;
	bool LahetysKesken;
	int Protokolla;
	bool KeskeytaLahetys;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormJakelu *FormJakelu;
//---------------------------------------------------------------------------
#endif
