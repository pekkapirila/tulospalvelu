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

#ifndef GetHttpH
#define GetHttpH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ScktComp.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TGetHttpForm : public TForm
{
__published:	// IDE-managed Components
   TEdit *EditHost;
   TLabel *Host;
   TEdit *EditPage;
   TLabel *Label2;
   TButton *Button1;
   TSaveDialog *SaveDialog1;
   TLabel *Label3;
   TEdit *Received;
   TLabel *Label5;
   TEdit *Edit3;
   TLabel *Label1;
   TEdit *Edit1;
	TEdit *EditDate;
   TLabel *Label4;
	TButton *Button2;
	TMemo *Memo1;
	TEdit *EditPort;
	TLabel *Label6;
	TEdit *EditSerial;
	TLabel *Label7;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall Button2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	void haku(void *lpcn);
	__fastcall TGetHttpForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TGetHttpForm *GetHttpForm;
//---------------------------------------------------------------------------
#endif
