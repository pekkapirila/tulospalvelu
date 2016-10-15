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

#ifndef UnitASakotH
#define UnitASakotH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include "VDeclare.h"
//---------------------------------------------------------------------------
class TFormASakot : public TForm
{
__published:	// IDE-managed Components
	TEdit *EditKno;
	TEdit *EditNimi;
	TEdit *EditS1;
	TEdit *EditS2;
	TEdit *EditS3;
	TEdit *EditS4;
	TEdit *EditAika;
	TEdit *EditPiste;
	TStringGrid *StringGrid1;
	TButton *Button1;
	TButton *Button2;
	TButton *Button3;
	TButton *Button4;
	TEdit *EditOsuus;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label3;
	void __fastcall EditKnoKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall Button2Click(TObject *Sender);
	void __fastcall Button3Click(TObject *Sender);
	void __fastcall EditS1KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall EditS2KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EditS3KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EditS4KeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall EditS4Exit(TObject *Sender);
	void __fastcall EditS1Exit(TObject *Sender);
	void __fastcall EditS2Exit(TObject *Sender);
	void __fastcall EditS3Exit(TObject *Sender);
	void __fastcall Button4Click(TObject *Sender);
	void __fastcall EditOsuusExit(TObject *Sender);
	void __fastcall EditOsuusKeyPress(TObject *Sender, System::WideChar &Key);
	void __fastcall EditKnoExit(TObject *Sender);
private:	// User declarations
	MESSAGE void __fastcall AddSakkoHandler(TMyMessage &msg);

BEGIN_MESSAGE_MAP
	MESSAGE_HANDLER(WM_MYADDSAKKO, TMyMessage, AddSakkoHandler);
END_MESSAGE_MAP(TComponent)
	void __fastcall EditSExit(int paikka);
	void __fastcall NaytaKilp(void);
	TEdit *EditS[4];
	kilptietue Kilp;
	int dKilp;
	int Osuus;
public:		// User declarations
	__fastcall TFormASakot(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormASakot *FormASakot;
//---------------------------------------------------------------------------
#endif
