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

#ifndef UnitEmitSQLH
#define UnitEmitSQLH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Data.DB.hpp>
#include <Data.DBXMySQL.hpp>
#include <Data.SqlExpr.hpp>
#include <Data.FMTBcd.hpp>
#include <Vcl.Dialogs.hpp>
#include <Vcl.Buttons.hpp>
#ifndef MAXOSUUSLUKU
#include "WinHk.h"
#else
#include "UnitMain.h"
#endif
#define DEVLKM 4
//---------------------------------------------------------------------------
class TFormEmitSQL : public TForm
{
__published:	// IDE-managed Components
	TSQLConnection *SQLConnection1;
	TButton *BtnAvaa;
	TButton *BtnTestaa;
	TCheckBox *CBAvattu;
	TEdit *Edit1;
	TLabel *Label1;
	TSQLDataSet *SQLDataSet1;
	TButton *BtnParams;
	TOpenDialog *OpenDialog1;
	TEdit *EdtServer;
	TLabel *Palvelin;
	TLabel *Label5;
	TEdit *EdtPW;
	TLabel *Label6;
	TEdit *EdtUser;
	TLabel *Label7;
	TEdit *EdtDB;
	TButton *BtnSuljeYht;
	TButton *BtnClose;
	TEdit *EdtAlku1;
	TLabel *Label2;
	TLabel *Label3;
	TEdit *EdtAika;
	TButton *BtnHae;
	TLabel *Label4;
	TEdit *EdtLaite1;
	TLabel *Label8;
	TEdit *EdtPortti;
	TLabel *Label9;
	TLabel *Label10;
	TEdit *EdtLaite2;
	TEdit *EdtAlku2;
	TLabel *Label11;
	TEdit *EdtLaite3;
	TEdit *EdtAlku3;
	TLabel *Label12;
	TEdit *EdtLaite4;
	TEdit *EdtAlku4;
	TCheckBox *CBJatkuva;
	TEdit *EdtHakuVali;
	TLabel *Label13;
	TButton *BtnPaivita;
	TComboBox *CBJono;
	TLabel *Label14;
	TButton *BtnNyt;
	TBitBtn *BitBtn1;
	TLabel *Label15;
	TEdit *Ala1;
	TEdit *Ala2;
	TEdit *Ala3;
	TEdit *Ala4;
	TEdit *Yla1;
	TEdit *Yla3;
	TEdit *Yla4;
	TEdit *Yla2;
	TLabel *Label16;
	TLabel *Label17;
	TLabel *Label18;
	TLabel *Label19;
	TLabel *LblHaku;
	TLabel *Label20;
	TEdit *EdtErakoko;
	void __fastcall BtnAvaaClick(TObject *Sender);
	void __fastcall BtnTestaaClick(TObject *Sender);
	void __fastcall BtnParamsClick(TObject *Sender);
	void __fastcall BtnSuljeYhtClick(TObject *Sender);
	void __fastcall BtnCloseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BtnHaeClick(TObject *Sender);
	void __fastcall EdtLaite1Exit(TObject *Sender);
	void __fastcall EdtLaite2Exit(TObject *Sender);
	void __fastcall EdtLaite3Exit(TObject *Sender);
	void __fastcall EdtLaite4Exit(TObject *Sender);
	void __fastcall EdtAlku1Exit(TObject *Sender);
	void __fastcall EdtAlku2Exit(TObject *Sender);
	void __fastcall EdtAlku3Exit(TObject *Sender);
	void __fastcall EdtAlku4Exit(TObject *Sender);
	void __fastcall CBJatkuvaClick(TObject *Sender);
	void __fastcall EdtHakuValiExit(TObject *Sender);
	void __fastcall BtnPaivitaClick(TObject *Sender);
	void __fastcall CBJonoChange(TObject *Sender);
	void __fastcall BtnNytClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall EdtAikaChange(TObject *Sender);
	void __fastcall EdtErakokoChange(TObject *Sender);
private:	// User declarations
	MESSAGE void __fastcall PaivitaHandler(TMyMessage &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYPAIVITA, TMyMessage, PaivitaHandler);
END_MESSAGE_MAP(TComponent)
	void __fastcall EdtLaiteChange(int no);
	void __fastcall EdtAlkuChange(int no);
	TEdit *EdtLaite[DEVLKM];
	TEdit *EdtAlku[DEVLKM];
	TEdit *Ala[DEVLKM];
	TEdit *Yla[DEVLKM];
	HTHREAD haeSQLThread;
public:		// User declarations
	__fastcall TFormEmitSQL(TComponent* Owner);
	bool readSQLrivi(wchar_t *minTime);
	int inSQLquery;
	int devices[DEVLKM];
	int devCount;
	int lastNo[DEVLKM];
	int ala[DEVLKM];
	int yla[DEVLKM];
	int HakuVali;
	int hakulkm;
	wchar_t aikaraja[100];
	bool Jatkuva;
	int Jono;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormEmitSQL *FormEmitSQL;
//---------------------------------------------------------------------------
#endif
