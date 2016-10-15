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

#ifndef UnitMySQLH
#define UnitMySQLH
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
#include "UnitMain.h"
//---------------------------------------------------------------------------
class TFormMySQL : public TForm
{
__published:	// IDE-managed Components
	TSQLConnection *SQLConnection1;
	TButton *Button1;
	TButton *Button2;
	TCheckBox *CBAvattu;
	TEdit *Edit1;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	TEdit *Edit2;
	TLabel *Label4;
	TEdit *Edit3;
	TSQLDataSet *SQLDataSet1;
	TButton *Button3;
	TOpenDialog *OpenDialog1;
	TMemo *Memo1;
	TEdit *EdtServer;
	TLabel *Palvelin;
	TLabel *Label5;
	TEdit *EdtPW;
	TLabel *Label6;
	TEdit *EdtUser;
	TLabel *Label7;
	TEdit *EdtDB;
	TButton *Button4;
	TButton *Button5;
	TButton *Button6;
	TCheckBox *CBjatkuva;
	TEdit *EdtJono;
	TLabel *Label8;
	TCheckBox *CBVarmista;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall Button5Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall Button6Click(TObject *Sender);
	void __fastcall CBjatkuvaClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormMySQL(TComponent* Owner);
	bool readSQLtulos(vatp *va, int dKilp, int os, int piste);
	bool readSQLosuus(kilptietue *kilp, int dKilp, int os);
	bool readSQLrivi(kilptietue *kilp, int dKilp);
	bool addSQLtulos(vatp *va, int dKilp, int os, int piste);
	bool addSQLosuus(kilptietue *kilp, int dKilp, int os);
	bool addSQLrivi(kilptietue *kilp, int dKilp);
	bool kirjSQLrivi(int dKilp);
	bool updateSQLtulos(vatp *va, int dKilp, int os, int piste);
	bool updateSQLosuus(kilptietue *kilp, osrec *ostiet, int dKilp, int os);
	bool updateSQLrivi(kilptietue *kilp, int dKilp);
	bool deleteSQLrivi(int dKilp);
	int __fastcall lukumSQL(void);
	int inSQLquery;
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMySQL *FormMySQL;
//---------------------------------------------------------------------------
#endif
