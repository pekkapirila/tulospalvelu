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

#ifndef UnitASuunnH
#define UnitASuunnH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include "HkDeclare.h"
//---------------------------------------------------------------------------
class TFormASuunn : public TForm
{
__published:	// IDE-managed Components
	TEdit *EditKno;
	TEdit *EditNimi;
	TEdit *EditA1;
	TEdit *EditA2;
	TEdit *EditAX;
	TEdit *EditKl;
	TEdit *EditAika;
	TEdit *EditPiste;
	TStringGrid *StringGrid1;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label3;
	TLabel *Label7;
	TEdit *EditKlX;
	TLabel *Label8;
	void __fastcall EditKnoChange(TObject *Sender);
	void __fastcall EditKnoKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall EditS1KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall EditS2KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EditS3KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EditKlKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall EditKlExit(TObject *Sender);
	void __fastcall EditA1Exit(TObject *Sender);
	void __fastcall EditA2Exit(TObject *Sender);
	void __fastcall EditAXExit(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall EditKlXKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EditKlXExit(TObject *Sender);
private:	// User declarations
	MESSAGE void __fastcall AddSakkoHandler(TMyMessage &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYADDSAKKO, TMyMessage, AddSakkoHandler);
END_MESSAGE_MAP(TComponent)
	void __fastcall EditSExit(int paikka);
	TEdit *EditS[5];
	kilptietue Kilp;
	int dKilp;
public:		// User declarations
	__fastcall TFormASuunn(TComponent* Owner);
   wchar_t Kohde[30];
};
//---------------------------------------------------------------------------
extern PACKAGE TFormASuunn *FormASuunn;
//---------------------------------------------------------------------------
#endif
